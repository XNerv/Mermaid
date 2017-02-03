#! /usr/bin/env python
# encoding: UTF-8
# Thomas Nagy 2008-2010 (ita)

"""

Doxygen support

Variables passed to bld():
* doxyfile -- the Doxyfile to use
* install_path -- where to install the documentation
* pars -- dictionary overriding doxygen configuration settings

When using this tool, the wscript will look like:

	def options(opt):
		opt.load('doxygen')

	def configure(conf):
		conf.load('doxygen')
		# check conf.env.DOXYGEN, if it is mandatory

	def build(bld):
		if bld.env.DOXYGEN:
			bld(features="doxygen", doxyfile='Doxyfile', ...)
"""

import os, os.path, sys, re
from pprint import pprint
from waflib import Utils
from waflib import Task, Utils, Node
from waflib.TaskGen import feature



DOXY_STR = '"${DOXYGEN}" - '
DOXY_FMTS = 'html latex man rft xml'.split()
DOXY_FILE_PATTERNS = '*.' + ' *.'.join('''
"*.c", "*.cc", "*.cxx", "*.cpp", "*.c++", "*.d", "*.java", "*.ii", "*.ixx",
"*.ipp", "*.i++", "*.inl", "*.h", "*.hh", "*.hxx", "*.hpp", "*.h++", "*.idl",
"*.odl", "*.cs", "*.php", "*.php3", "*.inc", "*.m", "*.mm", "*.dox", "*.py",
"*.f90", "*.f", "*.for", "*.vhd", "*.vhdl", "*.tcl", "*.md", "*.markdown", 
"*.C", "*.CC", "*.C++", "*.II", "*.I++", "*.H", "*.HH", "*.H++", "*.CS", 
"*.PHP", "*.PHP3", "*.M", "*.MM", "*.PY", "*.F90", "*.F", "*.VHD", "*.VHDL",
"*.TCL", "*.MD", "*.MARKDOWN"
'''.split())



def parse_doxy_text(txt):
	re_rl = re.compile('\\\\\r*\n', re.MULTILINE) # regular expression to address 
	                                              # the following tag structure.
	                                              #
	                                              # TAG = value [value, ...] \
						 						  # 	  value [value, ...] \
						 						  # 	  value [value, ...]
	re_nl = re.compile('\r*\n', re.MULTILINE)

	table = {}
	txt   = re_rl.sub('', txt)
	
	lines = re_nl.split(txt)
	for line in lines:
		line = line.strip()
		if not line or line.startswith('#') or line.find('=') < 0:
			continue
		if line.find('+=') > 0:
			tmp = line.split('+=', 1) # split the line on the first ocorrence of '+='
			key   = tmp[0].strip()
			value = tmp[1].strip()
			if key in table:
				table[key] = (table[key] + ' ' + value).strip()
			else:
				table[key] = value
		else:
			tmp = line.split('=', 1) # split the line on the first ocorrence of '='
			key   = tmp[0].strip()
			value = tmp[1].strip()
			table[key] = value
	return table



class doxygen(Task.Task):
	"""
	Task to process doxygen targets
	"""

	def __init__(self, *k, **kw):
		Task.Task.__init__(self, *k, **kw)
		self.hasrun = 0



	vars  = ['DOXYGEN', 'DOXYFLAGS']
	color = 'PINK'



	def runnable_status(self):
		# wait for dependent tasks to be complete
		for task in self.run_after:
			if not task.hasrun:
				return Task.ASK_LATER

		if not getattr(self, 'pars', None):
			doxyfile_node     = self.inputs[0]
			doxyfile_dir_node = doxyfile_node.parent

			# initialize self.pars with parameters readed from doxyfile
			txt = doxyfile_node.read() 
			self.pars = parse_doxy_text(txt)

			# override self.pars with any parameters passed to the task generator
			if getattr(self.generator, 'pars', None):
				for key, value in self.generator.pars.items():
					self.pars[key] = value

			if self.pars.get('OUTPUT_DIRECTORY'):
				output_directory_node = doxyfile_dir_node.get_bld().make_node(self.pars['OUTPUT_DIRECTORY'])
			else:
				# if no OUTPUT_DIRECTORY was specified in the doxyfile or task 
				# generator, build the output directory name from the doxyfile name
				output_directory_node = doxyfile_dir_node.get_bld().make_node(doxyfile_node.name)
			output_directory_node.mkdir()
			self.pars['OUTPUT_DIRECTORY'] = output_directory_node.abspath()

			if self.pars.get('GENERATE_TAGFILE'):
				generate_tagfile = self.pars['GENERATE_TAGFILE']
				generate_tagfile_node = output_directory_node.make_node(generate_tagfile)
				generate_tagfile_node.parent.mkdir()
				self.pars['GENERATE_TAGFILE'] = generate_tagfile_node.abspath()

			for task in self.run_after:
				dependent_task_install_path = Utils.subst_vars(getattr(task.generator, 'install_path'), task.env)
				main_task_install_path      = Utils.subst_vars(getattr(self.generator, 'install_path'), self.env)
				self.pars['TAGFILES'] = self.pars['TAGFILES'] + ' ' + (task.pars['GENERATE_TAGFILE'] + '=' + os.path.relpath(os.path.join(dependent_task_install_path, task.pars['HTML_OUTPUT']), os.path.join(main_task_install_path, self.pars['HTML_OUTPUT'])))

			self.doxy_inputs = getattr(self, 'doxy_inputs', [])
			if self.pars.get('INPUT'):
				re_path_split = re.compile(r'"[^"]+"|\'[^\']+\'|[^"\'\s]+') # to allow parse paths with space. Ex: INPUT: ./first/path "./second path" ... 
				inputs = re.findall(re_path_split, self.pars['INPUT'])
				for input in inputs:
					if os.path.isabs(input):
						node = self.generator.bld.root.find_node(input)
					else:
						node = doxyfile_dir_node.find_node(input)
					if not node:
						self.generator.bld.fatal('Could not find the doxygen INPUT %r' % input)
					self.doxy_inputs.append(node)
			else:
				self.doxy_inputs.append(doxyfile_dir_node)

		if not getattr(self, 'output_dir', None):
			self.output_dir = self.generator.bld.root.find_dir(self.pars.get('OUTPUT_DIRECTORY'))
		else:
			if not isinstance(self.output_dir, Node.Node):
				self.output_dir = self.generator.bld.root.find_dir(self.output_dir)
		if not self.output_dir:
			raise ValueError('Bad things was happened!')
				
		self.signature()

		ret = Task.Task.runnable_status(self)
		if ret == Task.SKIP_ME:
			# in case the files were removed
			self.add_install()
		return ret

	def scan(self):
		exclude_patterns = self.pars.get('EXCLUDE_PATTERNS', '').split()
		file_patterns    = self.pars.get('FILE_PATTERNS', '').split()
		if not file_patterns:
			file_patterns = DOXY_FILE_PATTERNS
		if self.pars.get('RECURSIVE', '').upper() == 'YES':
			file_patterns = ["**/%s" % pattern for pattern in file_patterns]
		
		nodes = []
		names = []
		for doxy_input in self.doxy_inputs:
			if os.path.isdir(doxy_input.abspath()):
				for node in doxy_input.ant_glob(incl=file_patterns, excl=exclude_patterns):
					nodes.append(node)
			else:
				nodes.append(node)
		return (nodes, names)

	def run(self):
		doxyfile_node     = self.inputs[0]
		doxyfile_dir_node = doxyfile_node.parent

		pars = self.pars.copy()
		input_data = '\n'.join(['%s = %s' % (key, pars[key]) for key in pars])
		input_data = input_data.encode() # for python 3
		cmd = Utils.subst_vars(DOXY_STR, self.env)
		proc = Utils.subprocess.Popen(cmd, shell=True, stdin=Utils.subprocess.PIPE, env=self.env.env or None, cwd=doxyfile_dir_node.abspath())
		proc.communicate(input_data)
		return proc.returncode

	def post_run(self):
		nodes = self.output_dir.ant_glob('**/*', quiet=True)
		for node in nodes:
			node.sig = Utils.h_file(node.abspath())
		self.add_install()
		return Task.Task.post_run(self)

	def add_install(self):
		nodes = self.output_dir.ant_glob('**/*', quiet=True)
		self.outputs += nodes
		if getattr(self.generator, 'install_path', None):
			if not getattr(self.generator, 'doxy_tar', None):
				self.generator.bld.install_files(self.generator.install_path,
					self.outputs,
					postpone=False,
					cwd=self.output_dir,
					relative_trick=True)

@feature('doxygen')
def create_doxygen(self):
	"""
	Creates a doxygen task (feature 'doxygen')
	"""

	if not getattr(self, 'doxyfile', None):
		self.bld.fatal('The doxygen configuration file was not specified!')

	doxyfile_node = self.doxyfile
	if not isinstance(doxyfile_node, Node.Node):
		doxyfile_node = self.path.find_node(doxyfile_node)

	if not doxyfile_node:
		self.bld.fatal('The doxygen configuration file (' + self.doxyfile + ') was not found!')

	self.doxygen_task = self.create_task('doxygen', doxyfile_node)

	ref_docs = self.to_list(getattr(self, 'ref_docs', []))
	for ref_doc in ref_docs:
		try: 
			tg = self.bld.get_tgen_by_name(ref_doc)
			tg.post()

			if isinstance(getattr(tg, 'doxygen_task', None), doxygen):
				tg.pars = getattr(tg, 'pars', {})
				tg.pars["GENERATE_TAGFILE"]	= tg.get_name() + '.tag'
			else:
				self.bld.fatal('The following reference documentation is not a valid doxygen target : ' + ref_doc)

			self.doxygen_task.set_run_after(tg.doxygen_task)
		except Exception:
			pass



def configure(conf):
	'''
	Check if doxygen and tar commands are present in the system

	If the commands are present, then conf.env.DOXYGEN and conf.env.TAR
	variables will be set. Detection can be controlled by setting DOXYGEN and
	TAR environmental variables.
	'''

	conf.find_program('doxygen', var='DOXYGEN', mandatory=False)
