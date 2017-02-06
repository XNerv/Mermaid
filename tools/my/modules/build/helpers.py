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

import sys, os
from waflib.Configure import conf
from waflib.Utils import to_list

# prepend paths to the python modules search path. 'PREPEND' is necessary 
# here to make the command './waf distcheck ...' work correctly when 
# checking the dist file. thus, don't use 'APPEND' path here!
sys.path.insert(1, os.path.join('.', 'tools', 'my', 'modules'))

from common.enums import OSName
from build.enums  import ProjectCategory, ProjectGroup, ProjectBuildGroup
from build.flags  import set_flags

@conf
def BUILD_STATIC_LIBRARY(bld, *k, **kw):
    if 'target' not in kw:
        bld.fatal("""\ntarget was not provided. Aborting! {error:32d78782}""")
    
    project_group = ProjectGroup.APP
    project_groups_to_build=bld.options.proj_groups.split(',')
    if project_group not in project_groups_to_build:
        return
    kw['project_group'] = project_group
    
    project_build_group = None
    if 'group' in kw:
        project_build_group = kw['group']
    else:
        kw['group'] = ProjectBuildGroup.MAIN
        project_build_group = kw['group']

    project_category = None
    if 'project_category' in kw:
        project_category = kw['project_category']
    else:
        kw['project_category'] = ProjectCategory.MAIN
        project_category = kw['project_category']
    kw = set_flags(bld, project_category, **kw)

    if bld.options.with_sdk:
        if 'install_path' not in kw:
            kw['install_path'] = '${LIBDIR}'

        if 'sdk_install_callback' in kw:
            kw['sdk_install_callback'](bld)       
    else:
        kw['install_path'] = None 

    return bld.stlib(*k, **kw)



@conf
def BUILD_SHARED_LIBRARY(bld, *k, **kw):
    if 'target' not in kw:
        bld.fatal("""\ntarget was not provided. Aborting! {error:208db39b}""")

    project_group = ProjectGroup.APP
    project_groups_to_build=bld.options.proj_groups.split(',')
    if project_group not in project_groups_to_build:
        return
    kw['project_group'] = project_group

    project_build_group = None
    if 'group' in kw:
        project_build_group = kw['group']
    else:
        kw['group'] = ProjectBuildGroup.MAIN
        project_build_group = kw['group']

    project_category = None
    if 'project_category' in kw:
        project_category = kw['project_category']
    else:
        kw['project_category'] = ProjectCategory.MAIN
        project_category = kw['project_category']
    kw = set_flags(bld, project_category, **kw)

    defines = ['BUILDING_SHARED_LIBRARY']
    if 'defines' in kw:
        kw['defines'] = to_list(kw['defines']) + defines 
    else:
        kw['defines'] = defines
 
    if bld.env.PROJECT_OS_NAME == OSName.WIN:
        if 'install_path' not in kw:
            kw['install_path'] = '${BINDIR}'

        if bld.options.with_sdk:
            if 'install_path_implib' not in kw:
                kw['install_path_implib'] = '${LIBDIR}'
        else:
            features = ['suppress_import_library']
            if 'features' in kw:
                kw['features'] = to_list(kw['features']) + features 
            else:
                kw['features'] = features
    else:
        if 'install_path' not in kw:
            kw['install_path'] = '${LIBDIR}'

    if bld.options.with_sdk and 'sdk_install_callback' in kw:
        kw['sdk_install_callback'](bld)

    return bld.shlib(*k, **kw)



@conf
def BUILD_PLUGIN(bld, *k, **kw):
    if 'target' not in kw:
        bld.fatal("""\ntarget was not provided. Aborting! {error:f1e16874}""")

    project_group = ProjectGroup.APP
    project_groups_to_build=bld.options.proj_groups.split(',')
    if project_group not in project_groups_to_build:
        return
    kw['project_group'] = project_group

    project_build_group = None
    if 'group' in kw:
        project_build_group = kw['group']
    else:
        kw['group'] = ProjectBuildGroup.MAIN
        project_build_group = kw['group']

    kw = set_flags(bld, ProjectCategory.MAIN, **kw)
    
    features = ['suppress_import_library']
    if 'features' in kw:
        kw['features'] = to_list(kw['features']) + features 
    else:
        kw['features'] = features

    if 'install_path' not in kw:
        kw['install_path'] = '${PLUGINDIR}' + os.sep + kw['target']

    if bld.options.with_sdk and 'sdk_install_callback' in kw:
        bld.fatal("""\n'sdk_install_callback' is not supported when building plugins. Aborting! {error:9d05519a}""")

    plugin = bld.BUILD_SHARED_LIBRARY(*k, **kw)
    plugin.env.cxxshlib_PATTERN = '%s.plugin'
    plugin.env.cshlib_PATTERN   = '%s.plugin'

    return plugin



@conf
def BUILD_PROGRAM(bld, *k, **kw):
    if 'target' not in kw:
        bld.fatal("""\ntarget was not provided. Aborting! {error:cd04df5a}""")

    project_group = ProjectGroup.APP
    project_groups_to_build=bld.options.proj_groups.split(',')
    if project_group not in project_groups_to_build:
        return
    kw['project_group'] = project_group

    project_build_group = None
    if 'group' in kw:
        project_build_group = kw['group']
    else:
        kw['group'] = ProjectBuildGroup.MAIN
        project_build_group = kw['group']

    project_category = None
    if 'project_category' in kw:
        project_category = kw['project_category']
    else:
        kw['project_category'] = ProjectCategory.MAIN
        project_category = kw['project_category']
    kw = set_flags(bld, project_category, **kw)

    if 'subsystem' not in kw:
        kw['subsystem'] = 'console'

    if 'install_path' not in kw:
        kw['install_path'] = '${BINDIR}'

    if bld.options.with_sdk and 'sdk_install_callback' in kw:
        bld.fatal("""\n'sdk_install_callback' is not supported when building programs. Aborting! {error:eac33acc}""")

    return bld.program(*k, **kw)



@conf
def EMBED_RESOURCES(bld, *k, **kw):
    project_group = ProjectGroup.RES
    project_groups_to_build=bld.options.proj_groups.split(',')
    if project_group not in project_groups_to_build:
        return
    kw['project_group'] = project_group
    kw['group'] = ProjectBuildGroup.RESOURCES

    kw['project_category'] = None

    features = ['embedres']
    if 'features' in kw:
        kw['features'] = to_list(kw['features']) + features 
    else:
        kw['features'] = features
 
    return bld(*k, **kw)



@conf
def BUILD_DOCUMENTATION(bld, *k, **kw):
    if 'target' not in kw:
        bld.fatal("""\ntarget was not provided. Aborting! {error:65e041c7}""")

    project_group = ProjectGroup.DOC
    project_groups_to_build=bld.options.proj_groups.split(',')
    if project_group not in project_groups_to_build:
        return
    kw['project_group'] = project_group

    kw['project_category'] = None

    features = ['doxygen']
    if 'features' in kw:
        kw['features'] = to_list(kw['features']) + features 
    else:
        kw['features'] = features

    if 'doxyfile' not in kw:
        kw['doxyfile'] = 'doxygen.conf'

    if 'install_path' not in kw:
        kw['install_path'] = '${DOCSDIR}' + os.sep + kw['target']

    return bld(*k, **kw)



@conf
def BUILD_THIRDPARTY_STATIC_LIBRARY(bld, *k, **kw):
    kw['project_category'] = ProjectCategory.THIRDYPARTY 
    return bld.BUILD_STATIC_LIBRARY(*k, **kw)



@conf
def BUILD_THIRDPARTY_SHARED_LIBRARY(bld, *k, **kw):
    kw['project_category'] = ProjectCategory.THIRDYPARTY
    return bld.BUILD_SHARED_LIBRARY(*k, **kw)



@conf
def BUILD_THIRDPARTY_PROGRAM(bld, *k, **kw):
    kw['project_category'] = ProjectCategory.THIRDYPARTY
    return bld.BUILD_PROGRAM(*k, **kw)



@conf
def RECURSE(ctx, *k, **kw):
    if 'mandatory' not in kw:
        kw['mandatory'] = False

    if 'once' not in kw:
        kw['once'] = False

    ctx.recurse(*k, **kw)