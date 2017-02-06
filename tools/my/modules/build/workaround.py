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

from waflib.TaskGen import feature, before_method, after_method
from waflib.Tools import ccroot

# don't make the import library of a shared library
@feature('suppress_import_library')
@before_method('apply_implib')
def remove_apply_implib_method(self):
    try:
        self.meths.remove('apply_implib')
    except:
        pass



@feature('c', 'cxx')
@after_method('apply_flags_msvc')
def fix_apply_flags_msvc(self):
    is_static = isinstance(self.link_task, ccroot.stlink_task)
    if not is_static:
        for f in self.env.LINKFLAGS:
            d = f.lower()
            if d[1:] == 'debug':
                try:
                    pdb_node = self.link_task.outputs[0].change_ext('.pdb')

                    self.env.append_value('CXXFLAGS', '/Fd' + pdb_node.abspath())
                    self.env.append_value('CFLAGS',   '/Fd' + pdb_node.abspath())

                    # this fix the install and uninstall of pdb files  
                    # when the option '--targets=xxx' is provided
                    self.install_task.inputs.append(pdb_node)
                except AttributeError:
                    pass
                break