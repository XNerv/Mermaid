#// +--------------------------------------------------------------------------
#// |
#// |   Mermaid GPL Source Code
#// |   Copyright (c) 2013-2016 XNerv Ltda (http://xnerv.com). All rights reserved.
#// |
#// |   This file is part of the Mermaid GPL Source Code.
#// |
#// |   Mermaid Source Code is free software: you can redistribute it and/or
#// |   modify it under the terms of the GNU General Public License
#// |   as published by the Free Software Foundation, either version 3
#// |   of the License, or (at your option) any later version.
#// |
#// |   Mermaid Source Code is distributed in the hope that it will be useful,
#// |   but WITHOUT ANY WARRANTY; without even the implied warranty of
#// |   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#// |   GNU General Public License for more details.
#// |
#// |   You should have received a copy of the GNU General Public License
#// |   along with Mermaid Source Code. If not, see <http://www.gnu.org/licenses/>.
#// |
#// +--------------------------------------------------------------------------

import os
import re
import sys
import time
import string
import datetime
from pprint import pprint
from waflib import Task, Utils, Node, Logs
from waflib.Tools import c_aliases
from waflib.TaskGen import feature, before_method, after_method, extension, HEADER_EXTS





class Utils:
    @staticmethod
    def normalise_filename(filename):
        filename = Utils.filter_text(text = filename, valid_chars = "-_. %s%s" % (string.ascii_letters, string.digits), replacements = {' ':'_', '.':'_', '-':'_'})
        return filename.lower()

    @staticmethod
    def normalise_namespace(namespace):
        filename = Utils.filter_text(text = namespace, valid_chars = "-_. %s%s" % (string.ascii_letters, string.digits), replacements = {' ':'_', '.':'_', '-':'_'})
        return filename.lower()

    @staticmethod
    def filter_text(text, valid_chars, replacements):
        text = "".join(c for c in text if c in valid_chars)
        text = "".join([replacements.get(c, c) for c in text])
        return text

    @staticmethod
    def change_ext(path, new_ext):
        return os.path.splitext(path)[0] + new_ext





class embedres(Task.Task):
    """
    Task to process embedres targets
    """
    def __init__(self, *k, **kw):
        Task.Task.__init__(self, *k, **kw)
        self.ext_out = ['.h', '.cpp']
        self.quiet   = True
        self.color   = 'PINK'
        self.hasrun  = Task.NOT_RUN

    def runnable_status(self): 
        for task in self.run_after:
          if not task.hasrun:
            return Task.ASK_LATER

        bld = self.generator.bld

        self.source_dir = self.generator.path
        self.output_dir = self.source_dir.get_bld().make_node(Utils.normalise_filename(self.generator.name))
        self.generator.export_includes = self.output_dir.parent.abspath()

        self.set_inputs(getattr(self.generator, 'resources', []))

        ret = super(embedres, self).runnable_status()
        if ret == Task.SKIP_ME:
            raw_deps = bld.raw_deps[self.uid()]
            if raw_deps[0] != self.signature():
                return Task.RUN_ME
            nodes = raw_deps[1:]
            for x in nodes:
                try:
                    os.stat(x.abspath())
                except:
                    return Task.RUN_ME
            self.set_outputs(nodes)

            self.compile_generated_sources()

        return ret

    def run(self):
        bld = self.generator.bld

        def get_resource_namespace(resource_node):
            root_namespace = getattr(self.generator, 'root_namespace', 'Embed') + '::' + Utils.normalise_namespace(self.generator.name)
            ns = os.path.split(resource_node.path_from(self.source_dir))[0]
            ns = "::".join([Utils.normalise_namespace(n) for n in ns.split(os.sep)])
            return root_namespace + ('::' + ns if ns else '')
  
        def generate_source(resource_node):    
            generation_date = datetime.datetime.now()

            resource_name        = os.path.basename(resource_node.abspath())
            resource_name_normalised = Utils.normalise_filename(resource_name)

            resource_data      = resource_node.read('rb')
            resource_data_size = os.path.getsize(resource_node.abspath())
             
            include_node = self.output_dir.find_or_declare(Utils.change_ext(os.path.join("src", resource_node.get_bld().path_from(self.source_dir.get_bld())), '.h'))
            cpp_node = self.output_dir.find_or_declare(Utils.change_ext(os.path.join("src", resource_node.get_bld().path_from(self.source_dir.get_bld())), '.cpp'))

            new_node_sign =  resource_node.get_bld_sig()
            old_node_sign = 0
            try:
              old_node_sign=bld.task_sigs[resource_node]
            except:
              pass
            if new_node_sign != old_node_sign:
                Logs.warn('embedres: new or modified resource. processing %s' % resource_node.abspath())

                bld.task_sigs[resource_node] = new_node_sign

                include_namespace       = get_resource_namespace(resource_node) 
                include_namespace_split = include_namespace.split("::")

                include_node.write ( "/****************************************************\n" +
                                     " * (Auto-generated file; DO NOT EDIT MANUALLY!)\n" +
                                     " * Generation date: " + str(generation_date) + "\n" +
                                     " ****************************************************/\n" +
                                     "\n" +
                                     "#pragma once\n" +
                                     "\n" +
                                     "".join(["namespace " + n + " { " for n in include_namespace_split]) +
                                     "\n" +
                                     "  extern const char* " + resource_name_normalised + ";\n" +
                                     "  const int          " + resource_name_normalised + "_SIZE = " + str(resource_data_size) + ";\n" +
                                     "".join(["}" for n in include_namespace_split]) + "\n")
            
                cpp_node.write ( "/****************************************************\n" +
                                 " * (Auto-generated file; DO NOT EDIT MANUALLY!)\n" +
                                 " * Generation date: " + str(generation_date) + "\n" +
                                 " ****************************************************/\n" +
                                 "\n" +
                                 "#include \"" + os.path.basename(include_node.abspath()) + "\"\n" +
                                 "\n" +
                                 "static const unsigned char data[] = {" + "".join(map(lambda ch: str(ord(ch)) + ',', resource_data)) + "0,0};\n" +
                                 "\n" +
                                 "const char* " + include_namespace + "::" + resource_name_normalised + " = (const char*) data;\n")
            else:
                Logs.info('embedres: unmodified resource. skipping %s' % resource_node.abspath())

            self.outputs.append(include_node)
            self.outputs.append(cpp_node)

        def generate_main_include():
            generation_date = datetime.datetime.now()

            main_include_node = self.output_dir.find_or_declare("resources.h")
            main_include_node.write ( "/****************************************************\n" +
                                      " * (Auto-generated file; DO NOT EDIT MANUALLY!)\n" +
                                      " * Generation date: " + str(generation_date) + "\n" +
                                      " ****************************************************/\n" +
                                      "\n" +
                                      "".join(["#include \"" + os.path.join("src", Utils.change_ext(input.get_bld().path_from(self.source_dir.get_bld()), '.h')).replace("\\", "/") + "\"\n"  for input in self.inputs]))
            
            self.outputs.append(main_include_node)

        for resource_node in self.inputs:
            generate_source(resource_node)

        generate_main_include()

        bld.raw_deps[self.uid()] = [self.signature()] + self.outputs

        self.compile_generated_sources()

    def compile_generated_sources(self):
        self.more_tasks = []
        for source in self.outputs:
            if any([source.name.endswith(h_ext) for h_ext in HEADER_EXTS]):
                continue
            tsk = self.generator.create_compiled_task('c' if source.name.endswith('.c') else 'cxx', source)
            self.more_tasks.append(tsk)





@feature('embedres')
def process_embedres(self):
    self.embedres_task = self.create_task('embedres')
    self.embedres_task.generator.export_includes = self.path.get_bld().abspath()