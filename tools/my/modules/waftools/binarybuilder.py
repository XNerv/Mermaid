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



class EntityType:
  UNKNOWN   = 'unknown'
  DIRECTORY = 'directory'
  FILE      = 'file'



class Entity:
  def __init__(self, entity_type=EntityType.UNKNOWN, entity_name='', entity_path=''):
    self.is_hidden  = False
    self.EntityType = entity_type
    self.EntityName = entity_name
    self.EntityPath = entity_path
    self.Entities   = []


          
class EntityTree:
  def __init__(self, root_nodes="", include_patterns=[], exclude_patterns=[]):
    self.include_patterns=include_patterns
    self.exclude_patterns=exclude_patterns
    self.Root = []
    for root_node in root_nodes:
      self.Root.append(self.__build_the_tree(root_node))
    self.__file_count = 0

  def __build_the_tree(self, node):
    entity = Entity(entity_type=EntityType.UNKNOWN, entity_name=os.path.basename(node.abspath()), entity_path=node.abspath())    
    entity.is_hidden = self.__is_hidden_entity(entity) 
    if os.path.isdir(node.abspath()):
      entity.EntityType = EntityType.DIRECTORY
      for item_node in sorted(node.ant_glob(incl=self.include_patterns, excl=self.exclude_patterns)):
        entity.Entities.append(self.__build_the_tree(item_node))
      return entity
    elif os.path.isfile(node.abspath()):
      entity.EntityType = EntityType.FILE
      return entity
    else:
      entity.EntityType = EntityType.UNKNOWN
      return entity

  def __is_hidden_entity (self, entity):
    return (entity.EntityName == ".git" or 
            entity.EntityName == ".svn" or 
            entity.EntityName.endswith(".scc") or 
            entity.EntityName.startswith("."))

  def get_file_count(self):
    return self.__get_file_count(self.Root)

  def __get_file_count(self, entities):
    count = 0 
    for entity in entities:
      if entity.EntityType == EntityType.DIRECTORY:
        count = count + self.__get_file_count(entity.Entities)
      elif entity.EntityType == EntityType.FILE and not entity.is_hidden:
        count = count + 1
    return count


  



class ResourceGenerator:
  def __init__(self, root_namespace, entity_tree, source_directory, output_directory):
    self.root_namespace   = root_namespace
    self.entity_tree      = entity_tree
    self.source_directory = source_directory
    self.output_directory = output_directory
    self.generation_date  = datetime.datetime.now()
    
  def generate_resource(self, entity, namespace):
    base_path = os.path.abspath(os.path.join(self.output_directory, "source", os.path.relpath(os.path.dirname(entity.EntityPath), self.source_directory))) 

    if not os.path.exists(base_path):
      os.makedirs(base_path)

    header_file = os.path.join(base_path, TextUtils.normalise_filename(entity.EntityName) + ".h")
    cpp_file    = os.path.join(base_path, TextUtils.normalise_filename(entity.EntityName) + ".cpp")
    
    try:
      header = open(header_file, 'wb')
    except IOError:     
      print "Failed to open the header file " + header_file
      sys.exit(1)     
  
    try:
      cpp = open(cpp_file, 'wb')
    except IOError:     
      print "Failed to open the cpp file " + cpp_file
      sys.exit(1)

    try:
      resource           = open(entity.EntityPath, 'rb')
      resource_data_size = os.path.getsize(entity.EntityPath)
      print "Size: " + str(resource_data_size)
    except IOError:     
      print "Failed to open the resource file " + entity.EntityPath
      sys.exit(1)

    header.write ("/****************************************************  \n")
    header.write (" * (Auto-generated file; DO NOT EDIT MANUALLY!)        \n")
    header.write (" * Generation date: " + str(self.generation_date) +   "\n")
    header.write (" ****************************************************/ \n")
    header.write ("\n")
    header.write ("#pragma once\n")
    header.write ("\n")
    namespaces = namespace.split("::")
    [header.write ("namespace " + n + " { ") for n in namespaces]
    header.write ("\n")
    header.write ("  extern const char* " + TextUtils.normalise_filename(entity.EntityName) + ";                                       \n")
    header.write ("  const int          " + TextUtils.normalise_filename(entity.EntityName) + "_SIZE = " + str(resource_data_size) + ";\n")
    [header.write ("}") for n in namespaces]
    header.close()

    cpp.write ("/****************************************************  \n")
    cpp.write (" * (Auto-generated file; DO NOT EDIT MANUALLY!)        \n")
    cpp.write (" * Generation date: " + str(self.generation_date) + "  \n")
    cpp.write (" ****************************************************/ \n")
    cpp.write ("\n")
    cpp.write ("#include \"" + os.path.basename(header_file) + "\"\n")
    cpp.write ("\n")
    cpp.write ("static const unsigned char data[] = {") 
    for line in resource:
      for rd in line:
        byte_str = str(ord(rd))
        cpp.write (byte_str + ',') 
    cpp.write ("0,0};\n")
    cpp.write ("\n")
    cpp.write ("const char* " + namespace + "::" + TextUtils.normalise_filename(entity.EntityName) + " = (const char*) data;\n")
    cpp.close()

    self.main_header.write ("#include \"" + os.path.relpath(header_file, start=self.output_directory) + "\"\n")

  def generate(self):
    base_path = os.path.abspath(self.output_directory) 

    if not os.path.exists(base_path):
      os.makedirs(base_path)

    main_header_file = os.path.join(base_path, "resources.h")
    
    try:
      self.main_header = open(main_header_file, 'wb')
    except IOError:     
      print "Failed to open the main header file " + main_header_file
      sys.exit(1)  

    self.main_header.write ("/****************************************************    \n")
    self.main_header.write (" * (Auto-generated file; DO NOT EDIT MANUALLY!).         \n")
    self.main_header.write (" * Generation date: " + str(datetime.datetime.now()) +  "\n")
    self.main_header.write (" ****************************************************/ \n\n")

    self.__generate(self.entity_tree.Root, self.root_namespace)

    self.main_header.close()

  def __generate(self, entities, namespace):
    for entity in sorted(entities, key=lambda i: i.EntityType, reverse=True):
      if entity.EntityType == EntityType.DIRECTORY:
        inner_namespace = namespace + '::' + TextUtils.normalise_filename(entity.EntityName)
        self.__generate(entity.Entities, inner_namespace)
      elif entity.EntityType == EntityType.FILE:
        self.generate_resource(entity, namespace)
      else:
        print "Unknown file " + entity.EntityPath



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

    self.entity_tree = EntityTree(root_nodes=self.resource_dir_nodes, include_patterns=self.include_patterns, exclude_patterns=self.exclude_patterns)

    self.output_dir = self.generator.path.get_bld().make_node(TextUtils.normalise_filename(self.generator.name))

    ret = super(embedres, self).runnable_status()
    #if ret == Task.SKIP_ME:
    #  if self.generator.bld.raw_deps[self.uid()] != [self.signature()] + [self.entity_tree.get_file_count()] + self.outputs:
    #    return Task.RUN_ME
    
    return ret

  def scan(self):
    nodes = [] 
    for resource_node in self.resource_dir_nodes:
      if os.path.isdir(resource_node.abspath()):
        for node in resource_node.ant_glob(incl=self.include_patterns, excl=self.exclude_patterns):
          nodes.append(node)
      else:
        self.bld.fatal("""\nThe resource [""" + resource_node + """] must be a directory! Aborting! {error:dcebb765}""")
    nodes.append(self.generator.path.find_node('wscript_build'))
    return (nodes, time.time())

  def run(self):
    try:
      resourceGenerator = ResourceGenerator(self.root_namespace, entity_tree=self.entity_tree, source_directory=self.generator.path.abspath(), output_directory=self.output_dir.abspath())
      resourceGenerator.generate()
      self.outputs = self.output_dir.ant_glob('**/*.cpp', quiet=True)
      #self.generator.bld.raw_deps[self.uid()] = [self.signature()] + [self.entity_tree.get_file_count()] + self.outputs
      return 0
    except Exception, e:
      return -1

  def post_run(self):
    nodes = self.output_dir.ant_glob('**/*', quiet=True)
    for node in nodes:
      node.sig = Utils.h_file(node.abspath())
    return Task.Task.post_run(self)



@feature('embedres')
def create_embedres_task(self):
  if not getattr(self, 'target', None):
        bld.fatal("""\nThe target attribute was not provided! Aborting! {error:221b227b}""")

  if not getattr(self, 'resource_dirs', None):
    self.bld.fatal("""\nThe resource_dirs attribute  was not provided! Aborting! {error:35969830}""")

  resource_dir_nodes = []
  resource_dirs      = Utils.to_list(self.resource_dirs)
  for resource_dir in resource_dirs:
    resource_dir_node = self.path.find_node(resource_dir)
    if not resource_dir_node:
      self.bld.fatal("""\nThe resource directory [""" + resource_dir + """] was not found! Aborting! {error:4d2cc556}""")
    if not os.path.isdir(resource_dir_node.abspath()):
      self.bld.fatal("""\nThe resource [""" + resource_dir + """] must be a directory! Aborting! {error:7f49cf45}""")
    resource_dir_nodes.append(resource_dir_node)

  self.embedres_task                    = self.create_task('embedres', [])
  self.embedres_task.resource_dir_nodes = resource_dir_nodes

  self.embedres_task.root_namespace             = getattr(self, 'root_namespace', 'Embed') + '::' + TextUtils.normalise_filename(self.name)
  self.embedres_task.include_patterns           = Utils.to_list(getattr(self, 'include_patterns', ['**/*']))
  self.embedres_task.exclude_patterns           = Utils.to_list(getattr(self, 'exclude_patterns', [])) + ['wscript', 'wscript_build']
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



@feature('*')
@after_method('process_source')
@before_method('apply_link')
def process_link_res_src(self):
  try:
    tg=self.bld.get_tgen_by_name(self.target)
    tg.post()
    for x in self.to_list(getattr(tg, 'use', [])):
        y = self.bld.get_tgen_by_name(x)
        y.post()
        if getattr(y, 'embedres_task', None):
          for item in y.embedres_task.output_dir.ant_glob('**/*.o', quiet=True):
            item.sig = Utils.h_file(item.abspath())
            self.add_those_o_files(item)
  except Exception, e:
    pass