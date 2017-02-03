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

import os, sys, platform
from enums import OSName



class System:
    @staticmethod
    def is_windows():
	   return True if os.name.lower() == 'nt' else False

    @staticmethod
    def is_cygwin():
       return True if sys.platform.lower() == 'cygwin' else False    

    @staticmethod
    def is_linux():
	   return True if sys.platform.lower().startswith('linux') else False

    @staticmethod
    def is_osx():
	   True if platform.system().lower() == "darwin" else False

    @staticmethod
    def is_unix():
        if System.is_posix() and not System.is_linux() and not System.is_cygwin():
            return True
        else:
    	   return False

    @staticmethod
    def is_posix():
        return True if os.name == 'posix' else False

    @staticmethod
    def get_current_os_name():
        if System.is_windows():
            return OSName.WIN
        elif System.is_cygwin():
            return OSName.CYGWIN
        elif System.is_linux():
            return OSName.LINUX
        elif System.is_osx():
            return OSName.OSX
        elif System.is_unix():
            return OSName.UNIX
        elif System.is_posix():
            return OSName.POSIX

    @staticmethod
    def validate_current_os(valid_os_names=[]):
        current_os_name = System.get_current_os_name()
        for os_name in valid_os_names:
            if current_os_name == os_name:
                return True
        return False
