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
from waflib.TaskGen import feature, before_method, after_method, extension





class TextUtils:
    @staticmethod
    def filter_text(text, valid_chars, replacements):
        text = "".join(c for c in text if c in valid_chars)
        text = "".join([replacements.get(c, c) for c in text])
        return text

    @staticmethod
    def normalise_filename(filename):
        filename = TextUtils.filter_text(text = filename, valid_chars = "-_. %s%s" % (string.ascii_letters, string.digits), replacements = {' ':'_', '.':'_', '-':'_'})
        return filename.lower()

    @staticmethod
    def normalise_namespace(namespace):
        filename = TextUtils.filter_text(text = namespace, valid_chars = "-_. %s%s" % (string.ascii_letters, string.digits), replacements = {' ':'_', '.':'_', '-':'_'})
        return filename.lower()





class embedres(Task.Task):
    """
    Task to process embedres targets
    """
    def __init__(self, *k, **kw):
        Task.Task.__init__(self, *k, **kw)
        self.quiet  = True # Disable the warnings raised when a task has no outputs
        self.color  = 'PINK'
        self.hasrun = Task.NOT_RUN

    def runnable_status(self): 
        for task in self.run_after:
          if not task.hasrun:
            return Task.ASK_LATER

        self.source_dir = self.generator.path
        self.output_dir = self.source_dir.get_bld().make_node(TextUtils.normalise_filename(self.generator.name))
        self.root_namespace = getattr(self.generator, 'root_namespace', 'Embed') + '::' + TextUtils.normalise_namespace(self.generator.name)
        self.generator.export_includes  = self.output_dir.parent.abspath()

        self.set_inputs(getattr(self.generator, 'resources', []))

        ret = super(embedres, self).runnable_status()
        if ret == Task.SKIP_ME:
            lst = self.generator.bld.raw_deps[self.uid()]
            if lst[0] != self.signature():
                return Task.RUN_ME

            nodes = lst[1:]
            for x in nodes:
                try:
                    os.stat(x.abspath())
                except:
                    return Task.RUN_ME

            nodes = lst[1:]
            self.set_outputs(nodes)
            self.add_cxx_tasks(nodes)

        return ret

    def run(self):
        def get_resource_namespace(resource_node):
            ns = os.path.split(resource_node.path_from(self.source_dir))[0]
            ns = "::".join([TextUtils.normalise_namespace(n) for n in ns.split(os.sep)])
            return self.root_namespace + ('::' + ns if ns else '')
  
        def change_ext(path, new_ext):
            return os.path.splitext(path)[0] + new_ext
  
        def generate_source(resource_node):
            generation_date = datetime.datetime.now()

            resource_name        = os.path.basename(resource_node.abspath())
            resource_name_normalised = TextUtils.normalise_filename(resource_name)

            resource_data      = resource_node.read('rb')
            resource_data_size = os.path.getsize(resource_node.abspath())
             
            include_node            = self.output_dir.find_or_declare(change_ext(os.path.join("src", resource_node.get_bld().path_from(self.source_dir.get_bld())), '.h'))
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
  
            cpp_node = self.output_dir.find_or_declare(change_ext(os.path.join("src", resource_node.get_bld().path_from(self.source_dir.get_bld())), '.cpp'))
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
            
            self.outputs.append(cpp_node)
  
        def generate_main_include():
            generation_date = datetime.datetime.now()

            main_include_node = self.output_dir.find_or_declare("resources.h")
            main_include_node.write ( "/****************************************************\n" +
                                    " * (Auto-generated file; DO NOT EDIT MANUALLY!)\n" +
                                    " * Generation date: " + str(generation_date) + "\n" +
                                    " ****************************************************/\n" +
                                    "\n" +
                                    "".join(["#include \"" + os.path.join("src", change_ext(input.get_bld().path_from(self.source_dir.get_bld()), '.h')).replace("\\", "/") + "\"\n"  for input in self.inputs]))
  
        for resource_node in self.inputs:
            generate_source(resource_node)
  
        generate_main_include()

        self.generator.bld.raw_deps[self.uid()] = [self.signature()] + self.outputs
        self.add_cxx_tasks(self.outputs)

        return 0

    def add_cxx_tasks(self, lst):
        self.more_tasks = []
        for node in lst:
            if node.name.endswith('.h'):
                continue
            tsk = self.generator.create_compiled_task('cxx', node)
            tsk.env=self.env
            tsk.generator=self.generator

            self.more_tasks.append(tsk)

            tsk.env.append_value('INCPATHS', [node.parent.abspath()])

            #if getattr(self.generator, 'link_task', None):
            #    self.generator.link_task.set_run_after(tsk)
            #    self.generator.link_task.inputs.append(tsk.outputs[0])
            #    self.generator.link_task.inputs.sort(key=lambda x: x.abspath())





@feature('embedres')
def process_embedres(self):
    self.embedres_task=self.create_task('embedres')





def configure(conf):
    pass