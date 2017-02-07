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
    
class VariantName:
    DEBUG   = 'debug'
    RELEASE = 'release'



class CompilerName:
	MSVC = 'msvc'
	GCC  = 'gcc'
	GXX  = 'gxx'
    


class MsvcVersionName:
	V120 = '12.0'



class AttributeName:
	CFLAGS    = 'cflags'
	CXXFLAGS  = 'cxxflags'
	LINKFLAGS = 'linkflags'
	DEFINES   = 'defines'
	


class ProjectCategory:
	THIRDYPARTY = 'thirdyparty'
	MAIN        = 'main'



class ProjectBuildGroup:
	TOOLS     = 'tools'
	RESOURCES = 'resources'
	MAIN      = 'main'



class ProjectGroup:
	APP = 'app' # executables, static and shared libraries
	RES = 'res' # images, configuration files and etc...
	DOC = 'doc' # documentation