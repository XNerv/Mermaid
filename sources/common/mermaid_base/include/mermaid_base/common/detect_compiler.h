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

#pragma once

#if defined(DOXYGEN)
    /** 
     * 	@ingroup    mermaid_base
     * 	
     * 	@def        MERMAID_COMPILER_MSVC
     * 	
     *  @brief      The target compiler is MSVC.
     */
#	 define MERMAID_COMPILER_MSVC
     
    /**
     * 	@ingroup    mermaid_base
     *
     * 	@def        MERMAID_COMPILER_GCC
     *
     *  @brief      The target compiler is GCC.
     */
#	 define MERMAID_COMPILER_GCC

#elif defined(_MSC_VER)
#    if _MSC_VER >= 1500
#        define MERMAID_COMPILER_MSVC
#    else
#        error MSVC++ compiler 9.0 or higher is required.
#    endif

#elif defined(__GNUC__)
#    if __GNUC__ >= 4
#        define MERMAID_COMPILER_GCC
#    else
#        error GCC compiler greater than 4 is required.
#    endif

#else
#    error The compiler is not supported.	
#endif