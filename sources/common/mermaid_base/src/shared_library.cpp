// +---------------------------------------------------------------------------
// |
// |   Mermaid GPL Source Code
// |   Copyright (c) 2013-2016 XNerv Ltda (http://xnerv.com). All rights reserved.
// |
// |   This file is part of the Mermaid GPL Source Code.
// |
// |   Mermaid Source Code is free software: you can redistribute it and/or
// |   modify it under the terms of the GNU General Public License
// |   as published by the Free Software Foundation, either version 3
// |   of the License, or (at your option) any later version.
// |
// |   Mermaid Source Code is distributed in the hope that it will be useful,
// |   but WITHOUT ANY WARRANTY; without even the implied warranty of
// |   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// |   GNU General Public License for more details.
// |
// |   You should have received a copy of the GNU General Public License
// |   along with Mermaid Source Code. If not, see <http://www.gnu.org/licenses/>.
// |
// +---------------------------------------------------------------------------

#include "../include/mermaid_base/shared_library.h"

#if defined(MERMAID_OS_FAMILY_UNIX)
	#include <dlfcn.h>
#endif

DLL_HANDLER Mermaid::Base::SharedLibrary::Load(const std::string& path)
{
#if defined(MERMAID_COMPILER_MSVC)
	DLL_HANDLER handler = LoadLibraryA(path.c_str());
#else
	DLL_HANDLER handler = dlopen(path.c_str(), RTLD_LAZY);
#endif

	return handler;
}

bool Mermaid::Base::SharedLibrary::Unload(DLL_HANDLER handler)
{
#if defined(MERMAID_COMPILER_MSVC)
	if (FreeLibrary(handler) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
#else
	if (dlclose(handler) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
#endif
}

DLL_SYMBOL_POINTER Mermaid::Base::SharedLibrary::GetSymbolPointer(DLL_HANDLER handler, const std::string& symbolName)
{
#if defined(MERMAID_COMPILER_MSVC)
	DLL_SYMBOL_POINTER symbolPointer = GetProcAddress(handler, symbolName.c_str());
#else
	DLL_SYMBOL_POINTER symbolPointer = dlsym(handler, symbolName.c_str());
#endif 

	return symbolPointer;
}