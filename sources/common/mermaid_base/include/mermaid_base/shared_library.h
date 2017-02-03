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
// |   GNU General Public License for more details .
// |
// |   You should have received a copy of the GNU General Public License
// |   along with Mermaid Source Code. If not, see <http://www.gnu.org/licenses/>.
// |
// +---------------------------------------------------------------------------

#pragma once

#include "common/common_defs.h"

#include <string>

#if defined(MERMAID_COMPILER_MSVC)
	#include <windows.h>
	typedef HMODULE DLL_HANDLER;
	typedef FARPROC DLL_SYMBOL_POINTER;
#else 
	typedef void* DLL_HANDLER;
	typedef void* DLL_SYMBOL_POINTER;
#endif

/**  
 * @defgroup 	mermaid_base_SharedLibrary SharedLibrary
 * 
 * @ingroup  	mermaid_base
 */ 
namespace Mermaid { namespace Base { namespace SharedLibrary
{
	/**
	 * @ingroup    mermaid_base_SharedLibrary
	 *
	 * @brief      Load a shared library at runtime.
	 *
	 * @details    Load a shared library at runtime in a cross platform manner.
	 *
	 * @param[in]  path  A path to the shared library to be loaded.
	 *
	 * @return     An handle to the loaded shared library. If the function
	 *             fails, the return value is NULL.
	 *
	 * @note       This function return a DLL_HANDLER that in Windows is of type
	 *             [HMODULE](https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx)
	 *             and in *nix is of type <b>void *</b>
	 */
	DLL_HANDLER Load(const std::string& path);

	/**
	 * @ingroup    mermaid_base_SharedLibrary
	 *
	 * @brief      Unload a shared library at runtime.
	 *
	 * @param[in]  handler  A shared library handler.
	 *
	 * @return     true if success, otherwise false.
	 */
	bool Unload(DLL_HANDLER handler);

	/**
	 * @ingroup    mermaid_base_SharedLibrary
	 *
	 * @brief      Get a symbol pointer from the loaded shared library.
	 *
	 * @param[in]  handler     A shared library handler.
	 * @param[in]  symbolName  A symbol name (Exported variable or function).
	 *
	 * @return     A symbol pointer from a shared library. If the function fails, the return value is NULL.
	 *
	 * @note       This function return a DLL_SYMBOL_POINTER that in Windows
	 *             is of type
	 *             [FARPROC](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633571(v=vs.85).aspx)
	 *             and in *nix is of type <b>void *</b>
	 */
	DLL_SYMBOL_POINTER GetSymbolPointer(DLL_HANDLER handler, const std::string& symbolName);
}}}