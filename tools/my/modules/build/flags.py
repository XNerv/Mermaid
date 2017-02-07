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

from collections import OrderedDict
from waflib.Utils import to_list

from common.enums  import OSName, ArchName
from common.system import System
from build.enums import ProjectCategory, AttributeName, VariantName

################ 
# msvc defines
def get_msvc_defines_for_debug(bld, proj_category):
    return ["DEBUG"] # enable debug mode

def get_msvc_defines_for_release(bld, proj_category):
    return ["NDEBUG", 'PORRA_MERMAID'] # disable debug mode

def get_msvc_defines_for_x86(bld, proj_category):
    return ['OLA_X86']
                     
def get_msvc_defines_for_x64(bld, proj_category):
    return ['OLA_X64']

def get_msvc_defines(bld, proj_category):
    return ['DONT_SET_USING_JUCE_NAMESPACE',
            'WIN32',                                      
            '_CRT_SECURE_NO_WARNINGS'] # no security warnings

############### 
# gcc defines
def get_gcc_defines_for_debug(bld, proj_category):
    return ["DEBUG"] # enable debug mode

def get_gcc_defines_for_release(bld, proj_category):
    return ["NDEBUG"] # disable debug mode

def get_gcc_defines_for_x86(bld, proj_category):
    return []
    
def get_gcc_defines_for_x64(bld, proj_category):
    return []

def get_gcc_defines(bld, proj_category):
    return []

############### 
# gxx defines
def get_gxx_defines_for_debug(bld, proj_category):
    return ["DEBUG"] # enable debug mode

def get_gxx_defines_for_release(bld, proj_category):
    return ["NDEBUG"] # disable debug mode

def get_gxx_defines_for_x86(bld, proj_category):
    return []
    
def get_gxx_defines_for_x64(bld, proj_category):
    return []

def get_gxx_defines(bld, proj_category):
    return ['DONT_SET_USING_JUCE_NAMESPACE']

############### 
# msvc cflags
def get_msvc_cflags_for_debug(bld, proj_category):
    return ['/Od',   # disable optimizations
            '/MDd']  # multi-threaded debug dll

def get_msvc_cflags_for_release(bld, proj_category):
    return ['/O2',  # maximize speed
            '/Oy-', # suppresses creation of frame pointers on the call stack
            '/MD']  # mult-threaded dll

def get_msvc_cflags_for_x86(bld, proj_category):
    return []
                     
def get_msvc_cflags_for_x64(bld, proj_category):
    return []

def get_msvc_cflags(bld, proj_category):
    return ['/TC',     # treat all files named on the command line as C source files
            '/Za',     # disable microsoft language extensions for C
            '/nologo'] # prevents display of the copyright message and 
                       # version number of msvc

################# 
# msvc cxxflags
def get_msvc_cxxflags_for_debug(bld, proj_category):
    return ['/Od',   # disable optimizations
            '/MDd',  # multi-threaded debug dll
            '/Zi',   # write debug info in a pdb file 
            '/FS']   # force synchronous pdb writes

def get_msvc_cxxflags_for_release(bld, proj_category):
    return ['/O2',  # maximize speed
            '/Oy-', # suppresses creation of frame pointers on the call stack
            '/MD']  # mult-threaded dll

def get_msvc_cxxflags_for_x86(bld, proj_category):
    return []
                     
def get_msvc_cxxflags_for_x64(bld, proj_category):
    return []

def get_msvc_cxxflags(bld, proj_category):
    return ['/GR',          # enable run-time type information (RTTI) 
            '/EHsc',        # enable exception handling model
            '/Zc:wchar_t',  # wchar_t as a native type
            '/Zc:forScope', # implement standard C++ behavior for the 'for'
                            # statement loops
            '/fp:precise',  # specify floating-point behavior
            '/nologo']      # prevents display of the copyright message and 
                            # version number of msvc

############### 
# gcc cflags
def get_gcc_cflags_for_debug(bld, proj_category):
    return ['-g',  # generate debug info
            '-O0'] # disable optimizations

def get_gcc_cflags_for_release(bld, proj_category):
    return ['-O2'] # optmize for speed

def get_gcc_cflags_for_x86(bld, proj_category):
    return ['-m32'] # compile 32-bits
    
def get_gcc_cflags_for_x64(bld, proj_category):
    return ['-m64'] # compile 64-bits

def get_gcc_cflags(bld, proj_category):
    return ['-std=c99']  # c99 as C language standard 

############### 
# gxx cxxflags
def get_gxx_cxxflags_for_debug(bld, proj_category):
    return ['-g',  # generate debug info
            '-O0'] # disable optimizations

def get_gxx_cxxflags_for_release(bld, proj_category):
    return ['-O2'] # optmize for speed

def get_gxx_cxxflags_for_x86(bld, proj_category):
    return ['-m32'] # compile 32-bits
    
def get_gxx_cxxflags_for_x64(bld, proj_category):
    return ['-m64'] # compile 64-bits

def get_gxx_cxxflags(bld, proj_category):
    return ['-std=c++11'] # c++11 as C++ language standard
              


################## 
# msvc linkflags
def get_msvc_linkflags_for_debug(bld, proj_category):
    return ['/DEBUG']

def get_msvc_linkflags_for_release(bld, proj_category):
    return []

def get_msvc_linkflags_for_x86(bld, proj_category):
    return []
                     
def get_msvc_linkflags_for_x64(bld, proj_category):
    return []

def get_msvc_linkflags(bld, proj_category):
    return ['/NOLOGO'] # prevents display of the copyright message and
                       # version number of link

#################
# gcc linkflags  
def get_gcc_linkflags_for_debug(bld, proj_category):
    return []

def get_gcc_linkflags_for_release(bld, proj_category):
    return []

def get_gcc_linkflags_for_x86(bld, proj_category):
    return ['-m32'] # link 32-bits
    
def get_gcc_linkflags_for_x64(bld, proj_category):
    return ['-m64'] # link 64-bits

def get_gcc_linkflags(bld, proj_category):
    return []

################# 
# gxx linkflags  
def get_gxx_linkflags_for_debug(bld, proj_category):
    return []

def get_gxx_linkflags_for_release(bld, proj_category):
    return []

def get_gxx_linkflags_for_x86(bld, proj_category):
    return ['-m32'] # link 32-bits
    
def get_gxx_linkflags_for_x64(bld, proj_category):
    return ['-m64'] # link 64-bits 

def get_gxx_linkflags(bld, proj_category):
    return []



def set_flags(bld, proj_category, **kw):
    if proj_category == ProjectCategory.MAIN:
        if bld.env.PROJECT_WARNING_FLAGS_MAIN == []:
            if 'PROJECT_MSVC_VERSION_NAME' in bld.env:
                WARNING_FLAGS = ['/w'] # disables all compiler warnings
            elif 'PROJECT_GCC_VERSION_NAME' in bld.env or 'PROJECT_GXX_VERSION_NAME' in bld.env:
                WARNING_FLAGS = ['-w'] # disables all compiler warnings
            else:
                ctx.fatal("""\nSomething has gone wrong. Aborting! {error:90cfbe48}.""")
        else:
            WARNING_FLAGS = bld.env.PROJECT_WARNING_FLAGS_MAIN.split(',')
    elif proj_category == ProjectCategory.THIRDYPARTY:
        if bld.env.PROJECT_WARNING_FLAGS_THIRDPARTY == []:
            if 'PROJECT_MSVC_VERSION_NAME' in bld.env:
                WARNING_FLAGS = ['/w'] # disables all compiler warnings
            elif 'PROJECT_GCC_VERSION_NAME' in bld.env or 'PROJECT_GXX_VERSION_NAME' in bld.env:
                WARNING_FLAGS = ['-w'] # disables all compiler warnings
            else:
                ctx.fatal("""\nSomething has gone wrong. Aborting! {error:b71a3be3}.""")
        else:
            WARNING_FLAGS = bld.env.PROJECT_WARNING_FLAGS_THIRDPARTY.split(',')

    if AttributeName.DEFINES in kw:
        kw[AttributeName.DEFINES] = list(OrderedDict.fromkeys(to_list(kw[AttributeName.DEFINES]) + get_flags(bld, proj_category, AttributeName.DEFINES)))
    else:
       kw[AttributeName.DEFINES] = list(OrderedDict.fromkeys(get_flags(bld, proj_category, AttributeName.DEFINES)))

    if AttributeName.CFLAGS in kw:
        kw[AttributeName.CFLAGS] = list(OrderedDict.fromkeys(to_list(kw[AttributeName.CFLAGS]) + get_flags(bld, proj_category, AttributeName.CFLAGS) + WARNING_FLAGS))
    else:
       kw[AttributeName.CFLAGS] = list(OrderedDict.fromkeys(get_flags(bld, proj_category, AttributeName.CFLAGS) + WARNING_FLAGS))

    if AttributeName.CXXFLAGS in kw:
        kw[AttributeName.CXXFLAGS] = list(OrderedDict.fromkeys(to_list(kw[AttributeName.CXXFLAGS]) + get_flags(bld, proj_category, AttributeName.CXXFLAGS) + WARNING_FLAGS))
    else:
       kw[AttributeName.CXXFLAGS] = list(OrderedDict.fromkeys(get_flags(bld, proj_category, AttributeName.CXXFLAGS) + WARNING_FLAGS))

    if AttributeName.LINKFLAGS in kw:
        kw[AttributeName.LINKFLAGS] = list(OrderedDict.fromkeys(to_list(kw[AttributeName.LINKFLAGS]) + get_flags(bld, proj_category, AttributeName.LINKFLAGS)))
    else:
       kw[AttributeName.LINKFLAGS] = list(OrderedDict.fromkeys(get_flags(bld, proj_category, AttributeName.LINKFLAGS)))

    return kw  



def get_flags(bld, proj_category, attribute_name):
    variant_name = bld.env.PROJECT_VARIANT_NAME
    arch_name    = bld.env.PROJECT_ARCH_NAME
    os_name      = bld.env.PROJECT_OS_NAME
    
    if os_name == OSName.WIN:
        if 'PROJECT_MSVC_VERSION_NAME' in bld.env:
            msvc_version_name = bld.env.PROJECT_MSVC_VERSION_NAME
            if variant_name == VariantName.DEBUG:
                if arch_name == ArchName.X86:
                    if attribute_name == AttributeName.DEFINES:
                        return get_msvc_defines_for_debug(bld, proj_category)   + get_msvc_defines_for_x86(bld, proj_category)   + get_msvc_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CFLAGS:
                        return get_msvc_cflags_for_debug(bld, proj_category)    + get_msvc_cflags_for_x86(bld, proj_category)    + get_msvc_cflags(bld, proj_category)
                    elif attribute_name == AttributeName.CXXFLAGS:
                        return get_msvc_cxxflags_for_debug(bld, proj_category)  + get_msvc_cxxflags_for_x86(bld, proj_category)  + get_msvc_cxxflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_msvc_linkflags_for_debug(bld, proj_category) + get_msvc_linkflags_for_x86(bld, proj_category) + get_msvc_linkflags(bld, proj_category)
                
                elif arch_name == ArchName.X64:
                    if attribute_name == AttributeName.DEFINES:
                        return get_msvc_defines_for_debug(bld, proj_category)   + get_msvc_defines_for_x64(bld, proj_category)   + get_msvc_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CFLAGS:
                        return get_msvc_cflags_for_debug(bld, proj_category)    + get_msvc_cflags_for_x64(bld, proj_category)    + get_msvc_cflags(bld, proj_category)
                    elif attribute_name == AttributeName.CXXFLAGS:
                        return get_msvc_cxxflags_for_debug(bld, proj_category)  + get_msvc_cxxflags_for_x64(bld, proj_category)  + get_msvc_cxxflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_msvc_linkflags_for_debug(bld, proj_category) + get_msvc_linkflags_for_x64(bld, proj_category) + get_msvc_linkflags(bld, proj_category)
            
            elif variant_name == VariantName.RELEASE:
                if arch_name == ArchName.X86:
                    if attribute_name == AttributeName.DEFINES:
                        return get_msvc_defines_for_release(bld, proj_category)   + get_msvc_defines_for_x86(bld, proj_category)   + get_msvc_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CFLAGS:
                        return get_msvc_cflags_for_release(bld, proj_category)    + get_msvc_cflags_for_x86(bld, proj_category)    + get_msvc_cflags(bld, proj_category)
                    elif attribute_name == AttributeName.CXXFLAGS:
                        return get_msvc_cxxflags_for_release(bld, proj_category)  + get_msvc_cxxflags_for_x86(bld, proj_category)  + get_msvc_cxxflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_msvc_linkflags_for_release(bld, proj_category) + get_msvc_linkflags_for_x86(bld, proj_category) + get_msvc_linkflags(bld, proj_category)
                
                elif arch_name == ArchName.X64:
                    if attribute_name == AttributeName.DEFINES:
                        return get_msvc_defines_for_release(bld, proj_category)   + get_msvc_defines_for_x64(bld, proj_category)   + get_msvc_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CFLAGS:
                        return get_msvc_cflags_for_release(bld, proj_category)    + get_msvc_cflags_for_x64(bld, proj_category)    + get_msvc_cflags(bld, proj_category)
                    elif attribute_name == AttributeName.CXXFLAGS:
                        return get_msvc_cxxflags_for_release(bld, proj_category)  + get_msvc_cxxflags_for_x64(bld, proj_category)  + get_msvc_cxxflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_msvc_linkflags_for_release(bld, proj_category) + get_msvc_linkflags_for_x64(bld, proj_category) + get_msvc_linkflags(bld, proj_category)
    
    elif System.is_linux():
        if 'PROJECT_GCC_VERSION_NAME' in bld.env:
            gcc_version_name = bld.env.PROJECT_GCC_VERSION_NAME
            if variant_name == VariantName.DEBUG:
                if arch_name == ArchName.X86:
                    if attribute_name == AttributeName.DEFINES:
                        return get_gcc_defines_for_debug(bld, proj_category)   + get_gcc_defines_for_x86(bld, proj_category)   + get_gcc_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CFLAGS:
                        return get_gcc_cflags_for_debug(bld, proj_category)    + get_gcc_cflags_for_x86(bld, proj_category)    + get_gcc_cflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_gcc_linkflags_for_debug(bld, proj_category) + get_gcc_linkflags_for_x86(bld, proj_category) + get_gcc_linkflags(bld, proj_category)
                
                elif arch_name == ArchName.X64:
                    if attribute_name == AttributeName.DEFINES:
                        return get_gcc_defines_for_debug(bld, proj_category)   + get_gcc_defines_for_x64(bld, proj_category)   + get_gcc_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CFLAGS:
                        return get_gcc_cflags_for_debug(bld, proj_category)    + get_gcc_cflags_for_x64(bld, proj_category)    + get_gcc_cflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_gcc_linkflags_for_debug(bld, proj_category) + get_gcc_linkflags_for_x64(bld, proj_category) + get_gcc_linkflags(bld, proj_category)
            
            elif variant_name == VariantName.RELEASE:
                if arch_name == ArchName.X86:
                    if attribute_name == AttributeName.DEFINES:
                        return get_gcc_defines_for_release(bld, proj_category)   + get_gcc_defines_for_x86(bld, proj_category)   + get_gcc_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CFLAGS:
                        return get_gcc_cflags_for_release(bld, proj_category)    + get_gcc_cflags_for_x86(bld, proj_category)    + get_gcc_cflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_gcc_linkflags_for_release(bld, proj_category) + get_gcc_linkflags_for_x86(bld, proj_category) + get_gcc_linkflags(bld, proj_category)
                
                elif arch_name == ArchName.X64:
                    if attribute_name == AttributeName.DEFINES:
                        return get_gcc_defines_for_release(bld, proj_category)   + get_gcc_defines_for_x64(bld, proj_category)   + get_gcc_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CFLAGS:
                        return get_gcc_cflags_for_release(bld, proj_category)    + get_gcc_cflags_for_x64(bld, proj_category)    + get_gcc_cflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_gcc_linkflags_for_release(bld, proj_category) + get_gcc_linkflags_for_x64(bld, proj_category) + get_gcc_linkflags(bld, proj_category)

        if 'PROJECT_GXX_VERSION_NAME' in bld.env:
            gxx_version_name = bld.env.PROJECT_GXX_VERSION_NAME
            if variant_name == VariantName.DEBUG:
                if arch_name == ArchName.X86:
                    if attribute_name == AttributeName.DEFINES:
                        return get_gxx_defines_for_debug(bld, proj_category)   + get_gxx_defines_for_x86(bld, proj_category)   + get_gxx_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CXXFLAGS:
                        return get_gxx_cxxflags_for_debug(bld, proj_category)  + get_gxx_cxxflags_for_x86(bld, proj_category)  + get_gxx_cxxflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_gxx_linkflags_for_debug(bld, proj_category) + get_gxx_linkflags_for_x86(bld, proj_category) + get_gxx_linkflags(bld, proj_category)
                
                elif arch_name == ArchName.X64:
                    if attribute_name == AttributeName.DEFINES:
                        return get_gxx_defines_for_debug(bld, proj_category)   + get_gxx_defines_for_x64(bld, proj_category)   + get_gxx_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CXXFLAGS:
                        return get_gxx_cxxflags_for_debug(bld, proj_category)  + get_gxx_cxxflags_for_x64(bld, proj_category)  + get_gxx_cxxflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_gxx_linkflags_for_debug(bld, proj_category) + get_gxx_linkflags_for_x64(bld, proj_category) + get_gxx_linkflags(bld, proj_category)
            
            elif variant_name == VariantName.RELEASE:
                if arch_name == ArchName.X86:
                    if attribute_name == AttributeName.DEFINES:
                        return get_gxx_defines_for_release(bld, proj_category)   + get_gxx_defines_for_x86(bld, proj_category)   + get_gxx_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CXXFLAGS:
                        return get_gxx_cxxflags_for_release(bld, proj_category)  + get_gxx_cxxflags_for_x86(bld, proj_category)  + get_gxx_cxxflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_gxx_linkflags_for_release(bld, proj_category) + get_gxx_linkflags_for_x86(bld, proj_category) + get_gxx_linkflags(bld, proj_category)
                elif arch_name == ArchName.X64:
                    if attribute_name == AttributeName.DEFINES:
                        return get_gxx_defines_for_release(bld, proj_category)   + get_gxx_defines_for_x64(bld, proj_category)   + get_gxx_defines(bld, proj_category)
                    elif attribute_name == AttributeName.CXXFLAGS:
                        return get_gxx_cxxflags_for_release(bld, proj_category)  + get_gxx_cxxflags_for_x64(bld, proj_category)  + get_gxx_cxxflags(bld, proj_category)
                    elif  attribute_name == AttributeName.LINKFLAGS:
                        return get_gxx_linkflags_for_release(bld, proj_category) + get_gxx_linkflags_for_x64(bld, proj_category) + get_gxx_linkflags(bld, proj_category)
    else:
        ctx.fatal("""\nUnsupported platform (""" + System.get_current_os_name() + """). Aborting! {error:1f98d3e9}.""")