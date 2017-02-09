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

    self.output_dir = self.generator.path.get_bld().make_node(TextUtils.normalise_filename(self.generator.name))

    ret = super(embedres, self).runnable_status()
    return ret

  def run(self):
      def get_file_namespace(root_namespace, source_dir_node, file_node):
        ns = os.path.split(file_node.path_from(source_dir_node))[0].replace(os.sep, "::")
        return self.root_namespace + ('::' + ns if ns else '')

      for file_node in self.inputs:
        file_name = os.path.basename(file_node.abspath())

        resource_data = file_node.read('rb')
        resource_data_size = os.path.getsize(file_node.abspath())
         
        include_node = self.output_dir.find_or_declare(file_node.change_ext('.h').get_src().path_from(self.generator.path)) 
        include_namespace = get_file_namespace(self.root_namespace, self.generator.path, file_node) 
        include_namespace_split = include_namespace.split("::")
        include_node.write ( "/****************************************************\n" +
                             " * (Auto-generated file; DO NOT EDIT MANUALLY!)\n" +
                             " * Generation date: " + str(datetime.datetime.now()) + "\n" +
                             " ****************************************************/\n" +
                             "\n" +
                             "#pragma once\n" +
                             "\n" +
                             "".join(["namespace " + n + " { " for n in include_namespace_split]) +
                             "\n" +
                             "  extern const char* " + TextUtils.normalise_filename(file_name) + ";\n" +
                             "  const int          " + TextUtils.normalise_filename(file_name) + "_SIZE = " + str(resource_data_size) + ";\n" +
                             "".join(["}" for n in include_namespace_split]) + "\n")

        cpp_node = self.output_dir.find_or_declare(file_node.change_ext('.cpp').get_src().path_from(self.generator.path))
        cpp_node.write ( "/****************************************************\n" +
                         " * (Auto-generated file; DO NOT EDIT MANUALLY!)\n" +
                         " * Generation date: " + str(datetime.datetime.now()) + "\n" +
                         " ****************************************************/\n" +
                         "\n" +
                         "#include \"" + os.path.basename(include_node.abspath()) + "\"\n" +
                         "\n" +
                         "static const unsigned char data[] = {" + "".join(map(lambda ch: str(ord(ch)) + ',', resource_data)) + "0,0};\n" +
                         "\n" +
                         "const char* " + include_namespace + "::" + TextUtils.normalise_filename(file_name) + " = (const char*) data;\n")
        
        self.outputs.append(include_node)
        self.outputs.append(cpp_node)

      main_include_node = self.generator.path.get_bld().find_or_declare("resources.h")
      main_include_node.write ( "/****************************************************\n" +
                                " * (Auto-generated file; DO NOT EDIT MANUALLY!)\n" +
                                " * Generation date: " + str(datetime.datetime.now()) + "\n" +
                                " ****************************************************/\n" +
                                "\n" +
                                ##
                                ##
                                ## FIX THIS!!!
                                ##
                                ##
                                "".join(["#include \"" + self.output_dir.find_or_declare(file_node.change_ext('.h').get_src().path_from(self.generator.path)).abspath() + "\"\n"  for input in self.inputs]))
      
      self.outputs.append(main_include_node)

      return 0




@feature('embedres')
def create_embedres_task(self):
  resources  = getattr(self, 'resources', [])
  self.embedres_task = self.create_task('embedres', resources)
  self.embedres_task.root_namespace = getattr(self, 'root_namespace', 'Embed') + '::' + TextUtils.normalise_filename(self.name)
  self.embedres_task.generator.export_includes  = self.path.get_bld().abspath()



def configure(conf):
  pass




@feature('*')
@before_method('process_source')
def process_add_res_src(self):
  try:
    for x in self.to_list(getattr(self, 'use', [])):
        y = self.bld.get_tgen_by_name(x)
        y.post()
        if getattr(y, 'embedres_task', None):
          if y.embedres_task.hasrun==Task.SUCCESS:
            self.source.extend(y.embedres_task.outputs)
  except Exception, e:
    pass

