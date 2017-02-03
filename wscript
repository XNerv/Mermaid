#!/usr/bin/env python
# encoding: utf-8

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

import os, sys, itertools
from optparse import SUPPRESS_HELP

# prepend paths to the python modules search path. 'PREPEND' is necessary 
# here to make the command './waf distcheck ...' work correctly when 
# checking the dist file. thus, don't use 'APPEND' path here!
sys.path.insert(1, os.path.join('.', 'tools', 'my', 'modules'))
sys.path.insert(1, os.path.join('.', 'tools', 'my', 'modules', 'waftools'))
sys.path.insert(1, os.path.join('.', 'tools', 'thirdparty', 'semantic_version'))

from build.enums import VariantName, MsvcVersionName, ProjectGroup, ProjectBuildGroup
from build.helpers import *
from build.workaround import *

from common.enums  import OSName, ArchName
from common.system import System

from waflib import Logs, Options, ConfigSet
import semantic_version



##################
# os validations
if not System.validate_current_os([OSName.WIN, OSName.LINUX]):
    Logs.error("\nUnsupported platform. Aborting! See the 'README' file to know the supported platforms.")
    sys.exit(1)



#########################
# add tools to the path
if System.is_windows():
    os.environ["PATH"] = os.path.abspath(os.path.join('tools', 'thirdparty', 'doxygen'))         + os.pathsep + os.environ["PATH"]
    os.environ["PATH"] = os.path.abspath(os.path.join('tools', 'thirdparty', 'graphviz', 'bin')) + os.pathsep + os.environ["PATH"]



####################
# global variables
APPNAME = 'mermaid'

if os.path.exists('VERSION'):
    version_file = open('VERSION')
    try:
        content = version_file.readline().strip()
    finally:
        version_file.close()

    if semantic_version.validate(content):
        VERSION = content
    else:
        Logs.error("\nThe project version specified in the file 'VERSION' is invalid! The version must follow the specification 'SemVer' ( http://semver.org/spec/v2.0.0-rc.1.html )")
        sys.exit(1)      
else:
    Logs.error("\nThe file 'VERSION' cannot be found! A file named 'VERSION' with the current project version must exist in the project root directory.")
    sys.exit(1)

top                          = '.'
project_root_dir             = os.path.abspath(top)
project_src_dir              = os.path.join(project_root_dir, 'sources')
project_thirdparty_dir       = os.path.join(project_root_dir, 'thirdparty')
project_tools_dir            = os.path.join(project_root_dir, 'tools')
project_my_tools_dir         = os.path.join(project_tools_dir, 'my')
project_thirdparty_tools_dir = os.path.join(project_tools_dir, 'thirdparty')
project_docs_dir             = os.path.join(project_root_dir, 'docs')
project_resources_dir        = os.path.join(project_root_dir, 'resources', System.get_current_os_name())
project_common_resources_dir = os.path.join(project_root_dir, 'resources', 'common') 
project_build_dir            = os.path.join(project_root_dir, 'build', System.get_current_os_name())
project_artefacts_dir        = os.path.join(project_build_dir, 'artefacts')
project_build_dir_temp       = os.path.join(project_artefacts_dir, 'tmp')
project_ide_projects_dir     = os.path.join(project_build_dir, 'projects')
out                          = project_build_dir_temp



######################################################
# hack waf to allow installation of static libraries
from waflib.Tools.c import cstlib
from waflib.Tools.cxx import cxxstlib
from waflib.Tools.fc import fcstlib
cstlib.inst_to = cxxstlib.inst_to = fcstlib.inst_to = '${LIBDIR}'



###########################################
# here we define the command-line options
def options(opt):
    config_group = opt.get_option_group('configure options')
    config_group.add_option('--variant', 
                            action='store', 
                            default=VariantName.DEBUG,
                            help='project variant to configure (%s, all)' % ', '.join([VariantName.DEBUG, VariantName.RELEASE]) + ' [default: %default]')
    config_group.add_option('--arch',
                            action='store',
                            default=None,
                            help='configure the arch to build for (%s, all)' % ', '.join([ArchName.X86, ArchName.X64]))
    config_group.add_option('--warning-flags-thirdparty',
                            action='store',
                            default=None,
                            help='warning flags for thirdparty projects')
    config_group.add_option('--warning-flags-main',
                            action='store',
                            default=None,
                            help='warning flags for main projects')
    config_group.add_option('-t', '--top', help=SUPPRESS_HELP)
    config_group.add_option('-o', '--out', help=SUPPRESS_HELP)
    config_group.add_option('--prefix', help=SUPPRESS_HELP)

    build_group = opt.get_option_group('build and install options')
    build_group.add_option('--proj-groups',
                            action='store',
                            default=','.join([ProjectGroup.APP, ProjectGroup.RES]),
                            help='project groups (%s)' % ', '.join([ProjectGroup.APP,ProjectGroup.RES,ProjectGroup.DOC]) + ' [default: %default]')

    install_group = opt.get_option_group('install/uninstall options')
    install_group.add_option('--destdir', help=SUPPRESS_HELP)
    install_group.add_option('--with-sdk',
                            action='store_true',
                            default=False,
                            help='install sdk files (includes, libs, etc)')
    
    if System.is_windows():
        opt.load('msvs')    # enable command to generate visual studio solution
    
    opt.load('doxygen')
    opt.load('binarybuilder')

    opt.recurse([
                    project_src_dir,
                    project_thirdparty_dir,
                    project_docs_dir,
                    project_resources_dir,
                    project_common_resources_dir
                ],
                mandatory=False)



###########################################
# here we define/check the configurations
def configure(conf):
    conf.check_waf_version(mini="1.8.0")
    
    if conf.options.arch == ArchName.X86: 
        archs  = [ArchName.X86]
    elif conf.options.arch == ArchName.X64:
        archs  = [ArchName.X64]
    elif conf.options.arch == 'all':
        archs  = [ArchName.X86, ArchName.X64]
    elif conf.options.arch == None:
        conf.fatal("""\nNeed to supply the 'arch' to build for. Aborting! See the 'README' file for more info.""")
    else:
        conf.fatal("""\nInvalid 'arch'. Aborting! See the 'README' file to know the supported 'arch(s)'.""")

    if conf.options.variant == VariantName.DEBUG: 
        variants  = [VariantName.DEBUG]
    elif conf.options.variant == VariantName.RELEASE:
        variants  = [VariantName.RELEASE]
    elif conf.options.variant == 'all':
        variants  = [VariantName.DEBUG, VariantName.RELEASE]
    else:
        conf.fatal("""\nInvalid 'variant'. Aborting! See the 'README' file to know the supported 'variant(s)'.""")

    conf.env.PROJECT_NAME                     = APPNAME
    conf.env.PROJECT_VERSION                  = VERSION
    conf.env.PROJECT_VARIANTS                 = variants
    conf.env.PROJECT_ARCHS                    = archs
    conf.env.PROJECT_ROOT_DIR                 = project_root_dir
    conf.env.PROJECT_TOOLS_DIR                = project_tools_dir
    conf.env.PROJECT_MY_TOOLS_DIR             = project_my_tools_dir
    conf.env.PROJECT_THIRDPARTY_TOOLS_DIR     = project_thirdparty_tools_dir
    conf.env.PROJECT_BUILD_DIR                = project_build_dir
    conf.env.PROJECT_ARTEFACTS_DIR            = project_artefacts_dir
    conf.env.PROJECT_BUILD_DIR_TEMP           = project_build_dir_temp
    conf.env.PROJECT_IDE_PROJECTS_DIR         = project_ide_projects_dir
    conf.env.PROJECT_THIRDPARTY_DIR           = project_thirdparty_dir
    conf.env.PROJECT_DOCS_DIR                 = project_docs_dir
    conf.env.PROJECT_RESOURCES_DIR            = project_resources_dir
    conf.env.PROJECT_COMMON_RESOURCES_DIR     = project_common_resources_dir
    conf.env.PROJECT_OS_NAME                  = System.get_current_os_name()
    conf.env.PROJECT_SRC_DIR                  = project_src_dir
    conf.env.PROJECT_THIRDPARTY_DIR           = project_thirdparty_dir 
    conf.env.PROJECT_WARNING_FLAGS_THIRDPARTY = conf.options.warning_flags_thirdparty
    conf.env.PROJECT_WARNING_FLAGS_MAIN       = conf.options.warning_flags_main

    __generate_commands(conf) # need predefine 'PROJECT_ARCHS' and 'PROJECT_VARIANTS' before this call

    default_env = conf.env
    for variant_name, arch_name in itertools.product(variants, archs):    
        variant_env = default_env.derive()
        variant_env.detach()
        conf.setenv('_'.join([variant_name, arch_name]), env=variant_env)

        conf.env.PROJECT_VARIANT_NAME = variant_name
    	conf.env.PROJECT_ARCH_NAME    = arch_name

        conf.env.PREFIX       = os.path.join(project_artefacts_dir, variant_name, arch_name)
        conf.env.BINDIR       = os.path.join(conf.env.PREFIX, 'bin')
        conf.env.LIBDIR       = os.path.join(conf.env.PREFIX, 'lib')
        conf.env.INCLUDEDIR   = os.path.join(conf.env.PREFIX, 'include')
        conf.env.PLUGINDIR    = os.path.join(conf.env.PREFIX, 'plugins')
        conf.env.RESOURCESDIR = os.path.join(conf.env.PREFIX, 'resources')
        conf.env.DOCSDIR      = os.path.join(conf.env.PREFIX, 'docs')

        if System.is_windows():
            conf.env.PROJECT_MSVC_VERSION_NAME = MsvcVersionName.V120
            conf.env['MSVC_VERSIONS'] = [__get_normalized_msvc_version_name(conf.env.PROJECT_MSVC_VERSION_NAME)] 
            conf.env['MSVC_TARGETS']  = [arch_name]
            
            conf.load('msvc')
            
        elif System.is_linux():
            conf.env.PROJECT_GCC_VERSION_NAME = None
            conf.env.PROJECT_GXX_VERSION_NAME = None
            
            conf.load('gcc')
            conf.load('gxx')
        else:
            conf.fatal("""\nUnsupported platform (""" + System.get_current_os_name() + """). Aborting! {error:707a4461}.""")

        conf.load('doxygen')
        conf.load('binarybuilder')

        conf.RECURSE([
                        conf.env.PROJECT_SRC_DIR,
                        conf.env.PROJECT_THIRDPARTY_DIR,
                        conf.env.PROJECT_DOCS_DIR,
                        conf.env.PROJECT_RESOURCES_DIR,
                        conf.env.PROJECT_COMMON_RESOURCES_DIR
                    ])



from waflib import Build, Utils, TaskGen
#########################
# entry point for build
def build(bld):
    # post the task generators group after group
    bld.post_mode = Build.POST_LAZY

    # the build groups will be executed in order
    if ProjectBuildGroup.TOOLS not in bld.group_names:
        bld.add_group(ProjectBuildGroup.TOOLS)
    if ProjectBuildGroup.RESOURCES not in bld.group_names:
        bld.add_group(ProjectBuildGroup.RESOURCES)
    if ProjectBuildGroup.MAIN not in bld.group_names:
        bld.add_group(ProjectBuildGroup.MAIN)

    if bld.variant == '':
        if len(bld.env.PROJECT_VARIANTS) > 0 and len(bld.env.PROJECT_ARCHS) > 0:
            for variant_name in bld.env.PROJECT_VARIANTS:
                for arch_name in bld.env.PROJECT_ARCHS:
                    bld.variant = '_'.join([variant_name, arch_name])        
        else:
            bld.fatal("""\nSomething has gone wrong. Aborting! {error:f5214518}.""")
            
    bld.RECURSE([
                   bld.env.PROJECT_SRC_DIR,
                   bld.env.PROJECT_THIRDPARTY_DIR,
                   bld.env.PROJECT_DOCS_DIR,
                   bld.env.PROJECT_RESOURCES_DIR,
                   bld.env.PROJECT_COMMON_RESOURCES_DIR
               ])



########################################################################################################
# entry point for initializations. OBS: every command or compound command call this function only once
def init(ctx):
    __generate_commands(ctx)



#############################################################################
# here we create the commands. Ex: build:debug:x86, clean:all:x86 etc... 
def __generate_commands(ctx):
    if not ctx.cmd == 'configure' and not ctx.cmd == 'init':
        ctx.fatal("""\n'Generate commands' function called from a invalid context (""" + ctx.cmd + """). Aborting! {error:ffe2f6f3}""")

    config_data = __get_config_data(ctx)
    if config_data != None:
        from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext
        context_types = [BuildContext, CleanContext, InstallContext, UninstallContext]

        for context_type, variant_name, arch_name in itertools.product(
            context_types, config_data.PROJECT_VARIANTS, config_data.PROJECT_ARCHS):

            command_name = context_type.__name__.replace('Context','').lower()
            class tmp(context_type):
                cmd     = ':'.join([command_name, variant_name, arch_name])
                variant = '_'.join([variant_name, arch_name])
                fun     = 'build'

        for context_type, variant_name in itertools.product(context_types, config_data.PROJECT_VARIANTS):
            command_name = context_type.__name__.replace('Context','').lower()
            class tmp(context_type):
                cmd = ':'.join([command_name, variant_name, 'all'])
                fun = '__exec_commands'
                returned_tasks=[]  # this is needed to allow '-p' option to work
                def compile(self): # need this, don't remove! (a long history =^.~=)
                    pass

        for context_type, arch_name in itertools.product(context_types, config_data.PROJECT_ARCHS):
            command_name = context_type.__name__.replace('Context','').lower()
            class tmp(context_type):
                cmd = ':'.join([command_name, 'all', arch_name])
                fun = '__exec_commands'
                returned_tasks=[]  # this is needed to allow '-p' option to work
                def compile(self): # need this, don't remove! (a long history =^.~=)
                    pass

        for context_type in context_types:
            command_name = context_type.__name__.replace('Context','').lower()
            class tmp(context_type):
                cmd = command_name
                fun = '__exec_commands'
                returned_tasks=[]  # this is needed to allow '-p' option to work 
                def compile(self): # need this, don't remove! (a long history =^.~=)
                    pass

            class tmp(context_type):
                cmd = ':'.join([command_name, 'all', 'all'])
                fun = '__exec_commands'
                returned_tasks=[]  # this is needed to allow '-p' option to work
                def compile(self): # need this, don't remove! (yes! a long history =^.~=)
                    pass



def __exec_commands(ctx):
    config_data = __get_config_data(ctx) 
    if config_data != None:
        cmd_elements = ctx.cmd.split(':') 
        if len(cmd_elements) == 3:
            command_element = cmd_elements[0]
            variant_element = cmd_elements[1]
            arch_element    = cmd_elements[2]        
        elif len(cmd_elements) == 1:
            command_element = ctx.cmd
            variant_element = 'all'
            arch_element    = 'all'
        else:
            ctx.fatal("""\nInvalid command (""" + ctx.cmd + """). Aborting! {error:0c04027d}""")

        if variant_element == 'all' and arch_element == 'all':
            for variant_name, arch_name in itertools.product(config_data.PROJECT_VARIANTS, config_data.PROJECT_ARCHS):
                Options.commands.insert(0, ':'.join([command_element, variant_name, arch_name]))

        elif variant_element == 'all':
                for variant_name in config_data.PROJECT_VARIANTS:
                    Options.commands.insert(0, ':'.join([command_element, variant_name, arch_element]))

        elif arch_element == 'all':
            for arch_name in config_data.PROJECT_ARCHS:
                Options.commands.insert(0, ':'.join([command_element, variant_element, arch_name]))
    else:
        ctx.fatal("""\nUnable to obtain the configuration data. Aborting! {error:b673fd07}""")



def __get_config_data(ctx):
    if ctx.cmd == 'configure':
        return ctx.env

    config_data_file_path = os.path.join(project_build_dir_temp, 'c4che', '_cache.py')
    if os.path.exists(config_data_file_path):
        config_data = ConfigSet.ConfigSet()
        config_data.load(config_data_file_path)
        return config_data
    else:
        return None                



def __get_normalized_msvc_version_name(msvc_version_name):
    if msvc_version_name == MsvcVersionName.V120:
        return "msvc 12.0"
    else:
        conf.fatal("""\nInvalid compiler version name (""" + msvc_version_name + """). Aborting! {error:e884f84b}""")