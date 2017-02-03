#! /usr/bin/env python
# encoding: utf-8
# Avalanche Studios 2009-2011
# Thomas Nagy 2011

# 
# 2013-2016 - Modified by thiago da silva andrade soares 
# (thiagosilvaandradesoares@gmail.com) especially for the 'mermaid' project.
#
# TODO: Clean up this file =^.~=
#
# WARNING:
#     This version was heavily modified and is not compatible with the 
#     original version. So do not update it with the original version.
#

"""
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
"""

"""
To add this tool to your project:
def options(conf):
	opt.load('msvs')

It can be a good idea to add the sync_exec tool too.

To generate solution files:
$ waf configure msvs

To customize the outputs, provide subclasses in your wscript files:

from waflib.extras import msvs
class vsnode_target(msvs.vsnode_target):
	def get_build_command(self, props):
		# likely to be required
		return "waf.bat build"
	def collect_source(self):
		# likely to be required
		...
class msvs_bar(msvs.msvs_generator):
	def init(self):
		msvs.msvs_generator.init(self)
		self.vsnode_target = vsnode_target

The msvs class re-uses the same build() function for reading the targets (task generators),
you may therefore specify msvs settings on the context object:

def build(bld):
	bld.solution_name = 'foo.sln'
	bld.waf_command = 'waf.bat'
	bld.projects_dir = bld.srcnode.make_node('.depproj')
	bld.projects_dir.mkdir()

ASSUMPTIONS:
* a project can be either a directory or a target, vcxproj files are written only for targets that have source files
* each project is a vcxproj file, therefore the project uuid needs only to be a hash of the absolute path
"""

import os, re, sys
import uuid # requires python 2.5
from optparse import SUPPRESS_HELP
from waflib.Build import BuildContext
from waflib import Utils, TaskGen, Logs, Task, Context, Node, Options
from common.enums  import OSName, ArchName


#
# TODO: How can I improve this?
#
HEADERS_GLOB = '**/(*.h|*.hpp|*.hxx|*.H|*.inl|*.c|*.cpp|*.cxx|*.asm)'


PROJECT_TEMPLATE = r'''<?xml version="1.0" encoding="UTF-8"?>
<Project DefaultTargets="Build" ToolsVersion="${project.tools_version}" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
	<ItemGroup Label="ProjectConfigurations">
		${for b in project.build_properties}
		<ProjectConfiguration Include="${b.configuration}|${b.platform}">
			<Configuration>${b.configuration}</Configuration>
			<Platform>${b.platform}</Platform>
		</ProjectConfiguration>
		${endfor}
	</ItemGroup>

	<PropertyGroup Label="Globals">
		<ProjectGuid>{${project.uuid}}</ProjectGuid>
		<Keyword>MakeFileProj</Keyword>
		<ProjectName>${project.name}</ProjectName>
	</PropertyGroup>

	<Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />

	${for b in project.build_properties}
	<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='${b.configuration}|${b.platform}'" Label="Configuration">
		<ConfigurationType>Makefile</ConfigurationType>

		<PlatformToolset>${project.toolset_version}</PlatformToolset>
		
		<OutDir>${b.outdir}</OutDir>
	</PropertyGroup>
	${endfor}

	<Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
	<ImportGroup Label="ExtensionSettings">
	</ImportGroup>

	${for b in project.build_properties}
	<ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='${b.configuration}|${b.platform}'">
		<Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
	</ImportGroup>
	${endfor}

	${for b in project.build_properties}
	<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='${b.configuration}|${b.platform}'">
		<NMakeBuildCommandLine>${xml:project.get_build_command(b)}</NMakeBuildCommandLine>
		<NMakeReBuildCommandLine>${xml:project.get_rebuild_command(b)}</NMakeReBuildCommandLine>
		<NMakeCleanCommandLine>${xml:project.get_clean_command(b)}</NMakeCleanCommandLine>
		<NMakeIncludeSearchPath>${xml:b.includes_search_path}</NMakeIncludeSearchPath>
		<NMakePreprocessorDefinitions>${xml:b.preprocessor_definitions};$(NMakePreprocessorDefinitions)</NMakePreprocessorDefinitions>
		<IncludePath>${xml:b.includes_search_path}</IncludePath>
		<ExecutablePath>$(ExecutablePath)</ExecutablePath>

		${if getattr(b, 'output_file', None)}
		<NMakeOutput>${xml:b.output_file}</NMakeOutput>
		${endif}
		${if getattr(b, 'deploy_dir', None)}
		<RemoteRoot>${xml:b.deploy_dir}</RemoteRoot>
		${endif}

		${if getattr(b, 'output_file', None)}
		<LocalDebuggerCommand>${xml:b.output_file}</LocalDebuggerCommand>
		${endif}
		<LocalDebuggerWorkingDirectory>${b.outdir}</LocalDebuggerWorkingDirectory>
    	<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
	</PropertyGroup>
	${endfor}

	${for b in project.build_properties}
		${if getattr(b, 'deploy_dir', None)}
	<ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='${b.configuration}|${b.platform}'">
		<Deploy>
			<DeploymentType>CopyToHardDrive</DeploymentType>
		</Deploy>
	</ItemDefinitionGroup>
		${endif}
	${endfor}

	<ItemGroup>
		${for x in project.source}
		<${project.get_key(x)} Include='${x.abspath()}' />
		${endfor}
	</ItemGroup>
	<Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
	<ImportGroup Label="ExtensionTargets">
	</ImportGroup>
</Project>
'''

FILTER_TEMPLATE = '''<?xml version="1.0" encoding="UTF-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
	<ItemGroup>
		${for x in project.source}
			<${project.get_key(x)} Include="${x.abspath()}">
				<Filter>${project.get_filter_name(x.parent)}</Filter>
			</${project.get_key(x)}>
		${endfor}
	</ItemGroup>
	<ItemGroup>
		${for x in project.dirs()}
			<Filter Include="${project.get_filter_name(x)}">
				<UniqueIdentifier>{${project.make_uuid(x.abspath())}}</UniqueIdentifier>
			</Filter>
		${endfor}
	</ItemGroup>
</Project>
'''

SOLUTION_TEMPLATE = '''Microsoft Visual Studio Solution File, Format Version ${project.numver}
# Visual Studio ${project.vsver}
${for p in project.all_projects}
Project("{${p.ptype()}}") = "${p.name}", "${p.title}", "{${p.uuid}}"
EndProject${endfor}
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		${if project.all_projects}
		${for (configuration, platform) in project.all_projects[0].ctx.project_configurations()}
		${configuration}|${platform} = ${configuration}|${platform}
		${endfor}
		${endif}
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
		${for p in project.all_projects}
			${if hasattr(p, 'source')}
			${for b in p.build_properties}
		{${p.uuid}}.${b.configuration}|${b.platform}.ActiveCfg = ${b.configuration}|${b.platform}
			${if getattr(p, 'is_active', None)}
		{${p.uuid}}.${b.configuration}|${b.platform}.Build.0 = ${b.configuration}|${b.platform}
			${endif}
			${if getattr(p, 'is_deploy', None)}
		{${p.uuid}}.${b.configuration}|${b.platform}.Deploy.0 = ${b.configuration}|${b.platform}
			${endif}
			${endfor}
			${endif}
		${endfor}
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
	GlobalSection(NestedProjects) = preSolution
	${for p in project.all_projects}
		${if p.parent}
		{${p.uuid}} = {${p.parent.uuid}}
		${endif}
	${endfor}
	EndGlobalSection
EndGlobal
'''

COMPILE_TEMPLATE = '''def f(project):
	lst = []
	def xml_escape(value):
		return value.replace("&", "&amp;").replace('"', "&quot;").replace("'", "&apos;").replace("<", "&lt;").replace(">", "&gt;")

	%s

	#f = open('cmd.txt', 'w')
	#f.write(str(lst))
	#f.close()
	return ''.join(lst)
'''
reg_act = re.compile(r"(?P<backslash>\\)|(?P<dollar>\$\$)|(?P<subst>\$\{(?P<code>[^}]*?)\})", re.M)
def compile_template(line):
	"""
	Compile a template expression into a python function (like jsps, but way shorter)
	"""
	extr = []
	def repl(match):
		g = match.group
		if g('dollar'): return "$"
		elif g('backslash'):
			return "\\"
		elif g('subst'):
			extr.append(g('code'))
			return "<<|@|>>"
		return None

	line2 = reg_act.sub(repl, line)
	params = line2.split('<<|@|>>')
	assert(extr)


	indent = 0
	buf = []
	app = buf.append

	def app(txt):
		buf.append(indent * '\t' + txt)

	for x in range(len(extr)):
		if params[x]:
			app("lst.append(%r)" % params[x])

		f = extr[x]
		if f.startswith('if') or f.startswith('for'):
			app(f + ':')
			indent += 1
		elif f.startswith('py:'):
			app(f[3:])
		elif f.startswith('endif') or f.startswith('endfor'):
			indent -= 1
		elif f.startswith('else') or f.startswith('elif'):
			indent -= 1
			app(f + ':')
			indent += 1
		elif f.startswith('xml:'):
			app('lst.append(xml_escape(%s))' % f[4:])
		else:
			#app('lst.append((%s) or "cannot find %s")' % (f, f))
			app('lst.append(%s)' % f)

	if extr:
		if params[-1]:
			app("lst.append(%r)" % params[-1])

	fun = COMPILE_TEMPLATE % "\n\t".join(buf)
	return Task.funex(fun)


re_blank = re.compile('(\n|\r|\\s)*\n', re.M)
def rm_blank_lines(txt):
	txt = re_blank.sub('\r\n', txt)
	return txt

BOM = '\xef\xbb\xbf'
try:
	BOM = bytes(BOM, 'iso8859-1') # python 3
except:
	pass

def stealth_write(self, data, flags='wb'):
	try:
		x = unicode
	except:
		data = data.encode('utf-8') # python 3
	else:
		data = data.decode(sys.getfilesystemencoding(), 'replace')
		data = data.encode('utf-8')

	if self.name.endswith('.vcproj') or self.name.endswith('.vcxproj'):
		data = BOM + data

	try:
		txt = self.read(flags='rb')
		if txt != data:
			raise ValueError('must write')
	except (IOError, ValueError):
		self.write(data, flags=flags)
	else:
		Logs.debug('msvs: skipping %s' % self.abspath())
Node.Node.stealth_write = stealth_write

re_quote = re.compile("[^a-zA-Z0-9-]")
def quote(s):
	return re_quote.sub("_", s)

def xml_escape(value):
	return value.replace("&", "&amp;").replace('"', "&quot;").replace("'", "&apos;").replace("<", "&lt;").replace(">", "&gt;")

def make_uuid(v, prefix = None):
	"""
	simple utility function
	"""
	if isinstance(v, dict):
		keys = list(v.keys())
		keys.sort()
		tmp = str([(k, v[k]) for k in keys])
	else:
		tmp = str(v)
	d = Utils.md5(tmp.encode()).hexdigest().upper()
	if prefix:
		d = '%s%s' % (prefix, d[8:])
	gid = uuid.UUID(d, version = 4)
	return str(gid).upper()

def diff(node, fromnode):
	# difference between two nodes, but with "(..)" instead of ".."
	c1 = node
	c2 = fromnode

	c1h = c1.height()
	c2h = c2.height()

	lst = []
	up = 0

	while c1h > c2h:
		lst.append(c1.name)
		c1 = c1.parent
		c1h -= 1

	while c2h > c1h:
		up += 1
		c2 = c2.parent
		c2h -= 1

	while id(c1) != id(c2):
		lst.append(c1.name)
		up += 1

		c1 = c1.parent
		c2 = c2.parent

	for i in range(up):
		lst.append('(..)')
	lst.reverse()
	return tuple(lst)

class build_property(object):
	pass

class vsnode(object):
	"""
	Abstract class representing visual studio elements
	We assume that all visual studio nodes have a uuid and a parent
	"""
	def __init__(self, ctx):
		self.ctx = ctx # msvs context
		self.name = '' # string, mandatory
		self.vspath = '' # path in visual studio (name for dirs, absolute path for projects)
		self.uuid = '' # string, mandatory
		self.parent = None # parent node for visual studio nesting

	def get_waf(self):
		"""
		Override in subclasses...
		"""
		return 'cd /d "%s" & %s' % (self.ctx.srcnode.abspath(), getattr(self.ctx, 'waf_command', 'waf.bat'))

	def ptype(self):
		"""
		Return a special uuid for projects written in the solution file
		"""
		pass

	def write(self):
		"""
		Write the project file, by default, do nothing
		"""
		pass

	def make_uuid(self, val):
		"""
		Alias for creating uuid values easily (the templates cannot access global variables)
		"""
		return make_uuid(val)

class vsnode_vsdir(vsnode):
	"""
	Nodes representing visual studio folders (which do not match the filesystem tree!)
	"""
	VS_GUID_SOLUTIONFOLDER = "2150E333-8FDC-42A3-9474-1A3956D46DE8"
	def __init__(self, ctx, uuid, name, vspath=''):
		vsnode.__init__(self, ctx)
		self.title = self.name = name
		self.uuid = uuid
		self.vspath = vspath or name

	def ptype(self):
		return self.VS_GUID_SOLUTIONFOLDER

class vsnode_project(vsnode):
	"""
	Abstract class representing visual studio project elements
	A project is assumed to be writable, and has a node representing the file to write to
	"""
	VS_GUID_VCPROJ = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
	def ptype(self):
		return self.VS_GUID_VCPROJ

	def __init__(self, ctx, node):
		vsnode.__init__(self, ctx)
		self.path = node
		self.uuid = make_uuid(node.abspath())
		self.name = node.name
		self.title = self.path.abspath()
		self.source = [] # list of node objects
		self.build_properties = [] # list of properties (nmake commands, output dir, etc)
		self.tools_version=ctx.tools_version
		self.tools_version=ctx.tools_version
		self.toolset_version=ctx.toolset_version

	def dirs(self):
		"""
		Get the list of parent folders of the source files (header files included)
		for writing the filters
		"""
		lst = []
		def add(x):
			if x.height() > self.tg.path.height() and x not in lst:
				lst.append(x)
				add(x.parent)
		for x in self.source:
			add(x.parent)
		return lst

	def write(self):
		Logs.debug('msvs: creating %r' % self.path)

		# first write the project file
		template1 = compile_template(PROJECT_TEMPLATE)
		proj_str = template1(self)
		proj_str = rm_blank_lines(proj_str)
		self.path.stealth_write(proj_str)

		# then write the filter
		template2 = compile_template(FILTER_TEMPLATE)
		filter_str = template2(self)
		filter_str = rm_blank_lines(filter_str)
		tmp = self.path.parent.make_node(self.path.name + '.filters')
		tmp.stealth_write(filter_str)

	def get_key(self, node):
		"""
		required for writing the source files
		"""
		name = node.name
		if name.endswith('.cpp') or name.endswith('.c'):
			return 'ClCompile'
		return 'ClInclude'

	def collect_properties(self):
		"""
		Returns a list of triplet (configuration, platform, output_directory)
		"""
		ret = []
		for c in self.ctx.configurations:
			for p in self.ctx.platforms:
				x = build_property()
				x.outdir = self.ctx.projects_dir.abspath()

				x.configuration = c
				x.platform = p

				x.preprocessor_definitions = ''
				x.includes_search_path = ''

				# can specify "deploy_dir" too
				ret.append(x)
		self.build_properties = ret

	def get_command(self, cmds, props):
		commands = ''
		for cmd in cmds:
			if cmd == 'msvs':
				commands = 'msvs'
			else:
				commands = commands + ' ' + ':'.join([cmd, props.configuration.lower(), 'x86' if props.platform == 'Win32' else 'x64'])	
		
		return ' '.join([self.get_waf(), commands, '--execsolution=' + self.ctx.get_solution_node().abspath()])

	def get_build_command(self, props):
		return self.get_command(['install'], props)

	def get_clean_command(self, props):
		return self.get_command(['clean'], props)

	def get_rebuild_command(self, props):
		return self.get_command(['clean', 'install'], props)

	def get_filter_name(self, node):
		lst = diff(node, self.tg.path)
		return '\\'.join(lst) or '.'

class vsnode_alias(vsnode_project):
	def __init__(self, ctx, node, name):
		vsnode_project.__init__(self, ctx, node)
		self.name = name
		self.output_file = ''

class vsnode_build_all(vsnode_alias):
	"""
	Fake target used to emulate the behaviour of "make all" (starting one process by target is slow)
	This is the only alias enabled by default
	"""
	def __init__(self, ctx, node, name='build_all'):
		vsnode_alias.__init__(self, ctx, node, name)
		self.is_active = True

class vsnode_project_view(vsnode_alias):
	"""
	Fake target used to emulate a file system view
	"""
	def __init__(self, ctx, node, name='project_view'):
		vsnode_alias.__init__(self, ctx, node, name)
		self.tg = self.ctx() # fake one, cannot remove
		self.exclude_files = Node.exclude_regs + '''
waf-1.7.*
waf3-1.7.*/**
.waf-1.7.*
.waf3-1.7.*/**
**/*.sdf
**/*.suo
**/*.ncb
**/%s
		''' % Options.lockfile

	def collect_source(self):
		# this is likely to be slow
		self.source = self.ctx.srcnode.ant_glob('**', excl=self.exclude_files)

	def get_build_command(self, props):
		return self.get_command([self.ctx.cmd], props)

	def get_clean_command(self, props):
		return ""

	def get_rebuild_command(self, props):
		return self.get_build_command(props)

class vsnode_target(vsnode_project):
	"""
	Visual studio project representing a targets (programs, libraries, etc) and bound
	to a task generator
	"""
	def __init__(self, ctx, tg):
		"""
		A project is more or less equivalent to a file/folder
		"""
		base = getattr(ctx, 'projects_dir', None) or tg.path
		node = base.make_node(quote(tg.name) + ctx.project_extension) # the project file as a Node
		vsnode_project.__init__(self, ctx, node)
		self.name = quote(tg.name)
		self.tg		= tg  # task generator
		self.ctx    = ctx 

	def get_command(self, cmds, props):
		commands = ''
		for cmd in cmds:
			commands = commands + ' ' + ':'.join([cmd, props.configuration.lower(), 'x86' if props.platform == 'Win32' else 'x64'])	
		
		if getattr(self, 'tg', None):
			commands += " --targets=%s" % self.tg.name

		return ' '.join([self.get_waf(), commands, '--execsolution=' + self.ctx.get_solution_node().abspath()])

	def collect_source(self):
		tg = self.tg
		source_files = tg.to_nodes(getattr(tg, 'source', []))
		include_dirs = Utils.to_list(getattr(tg, 'msvs_includes', []))
		additional_files = Utils.to_list(getattr(tg, 'additional_files', [])) 	
		include_files = []
		for x in include_dirs:
			if isinstance(x, str):
				x = tg.path.find_node(x)
			if x:
				lst = [y for y in x.ant_glob(HEADERS_GLOB, flat=False)]
				include_files.extend(lst)

		for x in additional_files:
			if isinstance(x, str):
				x = tg.path.find_node(x)
			if x:
				include_files.extend([x])

		# remove duplicates
		self.source.extend(list(set(source_files + include_files)))
		self.source.sort(key=lambda x: x.abspath())

	def collect_properties(self):
		"""
		Visual studio projects are associated with platforms and configurations (for building especially)
		"""
		super(vsnode_target, self).collect_properties()
		for x in self.build_properties:
			variant_name = x.configuration.lower() + '_' + ('x86' if x.platform == 'Win32' else 'x64')
			target = self.ctx.get_target_variant(self.tg.name, variant_name)

			install_path = ''
			if hasattr(target, 'install_path'):
				# TODO:
				# 	The folowing code is very ungly, fix this!
				try:
					install_path = Utils.subst_vars(getattr(target, 'install_path', ''), target.env)
				except Exception, e:
					pass
				
			x.outdir = install_path
			x.preprocessor_definitions = ''
			x.includes_search_path = ''

			try:
				tsk = target.link_task
			except AttributeError:
				pass
			else:	
				x.output_file = os.path.join(install_path, os.path.basename(tsk.outputs[0].abspath())) 
				x.preprocessor_definitions = ';'.join(tsk.env.DEFINES)
				x.includes_search_path = ';'.join(target.env.INCPATHS)

class msvs_generator(BuildContext):
	'''generates a visual studio 2013 solution'''
	cmd = 'msvs'
	fun = 'build'

	def init(self):
		"""
		Some data that needs to be present
		"""	
		for arch in self.env.PROJECT_ARCHS:
			if (not arch == ArchName.X86) and (not arch == ArchName.X64): 
				ctx.fatal("""\nUnknown arch(s) for Visual Studio projects. (""" + self.env.PROJECT_ARCHS + """). Aborting! {error:d1d27079}""")

		if ArchName.X86 in self.env.PROJECT_ARCHS and ArchName.X64 not in self.env.PROJECT_ARCHS:
			self.platforms = ['Win32']
		elif ArchName.X86 not in self.env.PROJECT_ARCHS and ArchName.X64 in self.env.PROJECT_ARCHS:
			self.platforms = ['x64']
		elif ArchName.X86 in self.env.PROJECT_ARCHS and ArchName.X64 in self.env.PROJECT_ARCHS:
			self.platforms = ['Win32', 'x64']

		self.configurations = [item.capitalize() for item in self.env.PROJECT_VARIANTS]

		if not getattr(self, 'all_projects', None):
			self.all_projects = []
		if not getattr(self, 'project_extension', None):
			self.project_extension = '.vcxproj'
		if not getattr(self, 'projects_dir', None):
			self.projects_dir = self.root.make_node(os.path.join(self.env.PROJECT_IDE_PROJECTS_DIR, self.env.PROJECT_NAME))
			self.projects_dir.mkdir()

		self.solution_name = os.path.join(os.path.dirname(self.projects_dir.abspath()), self.env.PROJECT_NAME + '.sln')

		# bind the classes to the object, so that subclass can provide custom generators
		if not getattr(self, 'vsnode_vsdir', None):
			self.vsnode_vsdir = vsnode_vsdir
		if not getattr(self, 'vsnode_target', None):
			self.vsnode_target = vsnode_target
		if not getattr(self, 'vsnode_build_all', None):
			self.vsnode_build_all = vsnode_build_all
		if not getattr(self, 'vsnode_project_view', None):
			self.vsnode_project_view = vsnode_project_view

		self.numver = '12.00'
		self.vsver	= '2013'
		self.tools_version = '12.00'
		self.toolset_version = 'v120'

	def execute(self):
		"""
		Entry point
		"""
		self.restore()
		if not self.all_envs:
			self.load_envs()
		self.RECURSE([self.run_dir])
		
		self.init()

		# two phases for creating the solution
		self.collect_projects() # add project objects into "self.all_projects"
		self.write_files() # write the corresponding project and solution files

	def collect_projects(self):
		"""
		Fill the list self.all_projects with project objects
		Fill the list of build targets
		"""
		self.collect_targets()
		self.add_aliases()
		self.collect_dirs()
		default_project = getattr(self, 'default_project', None)
		def sortfun(x):
			if x.name == default_project:
				return ''
			return getattr(x, 'path', None) and x.path.abspath() or x.name
		self.all_projects.sort(key=sortfun)

	def write_files(self):
		"""
		Write the project and solution files from the data collected
		so far. It is unlikely that you will want to change this
		"""
		for p in self.all_projects:
			p.write()
		
		# and finally write the solution file
		node = self.get_solution_node()
		node.parent.mkdir()
		Logs.warn('Creating %r' % node)
		template1 = compile_template(SOLUTION_TEMPLATE)
		sln_str = template1(self)
		sln_str = rm_blank_lines(sln_str)
		node.stealth_write(sln_str)

	def get_solution_node(self):
		"""
		The solution filename is required when writing the .vcproj files
		return self.solution_node and if it does not exist, make one
		"""
		try:
			return self.solution_node
		except:
			pass

		solution_name = getattr(self, 'solution_name', None)
		if not solution_name:
			solution_name = getattr(Context.g_module, Context.APPNAME, 'project') + '.sln'
		if os.path.isabs(solution_name):
			self.solution_node = self.root.make_node(solution_name)
		else:
			self.solution_node = self.srcnode.make_node(solution_name)
		return self.solution_node

	def project_configurations(self):
		"""
		Helper that returns all the pairs (config,platform)
		"""
		ret = []
		
		for c in self.configurations:
			for p in self.platforms:
				ret.append((c, p))
		return ret

	def collect_targets(self):
		"""
		Process the list of task generators
		"""
		for tg in self.group_names['main']:
			if not isinstance(tg, TaskGen.task_gen):
				continue

			if not hasattr(tg, 'msvs_includes'):
				tg.msvs_includes = tg.to_list(getattr(tg, 'includes', [])) + tg.to_list(getattr(tg, 'export_includes', []))
			
			tg.post()
			
			if not getattr(tg, 'link_task', None):
				continue

			p = self.vsnode_target(self, tg)
			p.collect_source() # delegate this processing
			p.collect_properties()
			self.all_projects.append(p)

	def get_target_variant(self, target_name, variant_name):
		self._cache_targets_variants() 
		return self.targets_variants[target_name + '_' + variant_name]	

	def _cache_targets_variants(self):
		try:
			if self.targets_variants:
				return
		except:
			pass

		self.targets_variants = {}

		# this stores a list of targets with their respective context in an instance 
		# variable named 'targets_variants'.
		#
		# 'targets_variants' is a dictionary with keys in the format 
		# [target_name + '_' + variant_name], ex: ['kernel' + '_' + 'debug_x86'].
		for variant_name in self.env.PROJECT_VARIANTS:
			for arch_name in self.env.PROJECT_ARCHS:
				self.group_names={}
				del self.groups[:]
				
				self.restore()
				self.load_envs()
				
				self.variant = variant_name + '_' + arch_name
				self.RECURSE([self.run_dir])

				for tg in self.group_names['main']:
					if not isinstance(tg, TaskGen.task_gen):
						continue

					tg.post()

					if not getattr(tg, 'link_task', None):
						continue

					self.targets_variants[tg.name + '_' + self.variant] = tg

		self.group_names={}
		del self.groups[:]

		self.variant = ''
		self.RECURSE([self.run_dir])



	def add_aliases(self):
		"""
		Add a specific target that emulates the "make all" necessary for Visual studio when pressing F7
		We also add an alias for "make install" (disabled by default)
		"""
		base = getattr(self, 'projects_dir', None) or self.tg.path
		
		node_project = base.make_node('build_all' + self.project_extension) # Node
		p_build = self.vsnode_build_all(self, node_project)
		p_build.collect_properties()
		self.all_projects.append(p_build)

		node_project = base.make_node('project_view' + self.project_extension) # Node
		p_view = self.vsnode_project_view(self, node_project)
		p_view.collect_source()
		p_view.collect_properties()
		self.all_projects.append(p_view)

		n = self.vsnode_vsdir(self, make_uuid(self.srcnode.abspath() + 'build_aliases'), "build_aliases")
		p_build.parent = p_view.parent = n
		self.all_projects.append(n)

	def collect_dirs(self):
		"""
		Create the folder structure in the Visual studio project view
		"""
		seen = {}
		def make_parents(proj):
			# look at a project, try to make a parent
			if getattr(proj, 'parent', None):
				# aliases already have parents
				return
			x = proj.iter_path
			if x in seen:
				proj.parent = seen[x]
				return

			# There is not vsnode_vsdir for x.
			# So create a project representing the folder "x"
			n = proj.parent = seen[x] = self.vsnode_vsdir(self, make_uuid(x.abspath()), x.name)
			n.iter_path = x.parent
			self.all_projects.append(n)

			# recurse up to the project directory
			if x.height() > self.srcnode.height() + 1:
				make_parents(n)

		for p in self.all_projects[:]: # iterate over a copy of all projects
			if not getattr(p, 'tg', None):
				# but only projects that have a task generator
				continue

			# make a folder for each task generator
			p.iter_path = p.tg.path
			make_parents(p)


def options(ctx):
	"""
	If the msvs option is used, try to detect if the build is made from visual studio
	"""
	ctx.add_option('--execsolution', action='store', help=SUPPRESS_HELP)

	old = BuildContext.execute
	def override_build_state(ctx):
		def lock(rm, add):
			uns = ctx.options.execsolution.replace('.sln', rm)
			uns = ctx.root.make_node(uns)
			try:
				uns.delete()
			except:
				pass

			uns = ctx.options.execsolution.replace('.sln', add)
			uns = ctx.root.make_node(uns)
			try:
				uns.write('')
			except:
				pass

		if ctx.options.execsolution:
			ctx.launch_dir = Context.top_dir # force a build for the whole project (invalid cwd when called by visual studio)
			lock('.lastbuildstate', '.unsuccessfulbuild')
			old(ctx)
			lock('.unsuccessfulbuild', '.lastbuildstate')
		else:
			old(ctx)
	BuildContext.execute = override_build_state

