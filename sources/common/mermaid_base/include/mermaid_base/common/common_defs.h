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

#include "detect_os.h"
#include "detect_compiler.h"
#include "defines.h"

#if defined(DOXYGEN)
    /**
     *  @ingroup    mermaid_base
     *
     *  @def 	     MERMAID_CURRENT_FUNCTION
     *
     *  @brief      Valid only within a function and returns the signature of the enclosing function (as a string).
     */
# 	define MERMAID_CURRENT_FUNCTION

#elif defined(MERMAID_COMPILER_MSVC)
#    define MERMAID_SYMBOL_IMPORT __declspec(dllimport)
#    define MERMAID_SYMBOL_EXPORT __declspec(dllexport)
#    define MERMAID_SYMBOL_LOCAL
#
#    define MERMAID_CURRENT_FUNCTION __FUNCSIG__

#elif defined(MERMAID_COMPILER_GCC)
#    define MERMAID_SYMBOL_IMPORT __attribute__ ((visibility ("default")))
#    define MERMAID_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
#    define MERMAID_SYMBOL_LOCAL  __attribute__ ((visibility ("hidden")))

#    define MERMAID_CURRENT_FUNCTION __PRETTY_FUNCTION__
#endif

#if defined(DOXYGEN)
    /**
     *  @ingroup   mermaid_base
     *
     *  @def 	    MERMAID_API
     *
     *  @brief     Macro to import/export symbols from the public API.
     */
#    define MERMAID_API

#elif defined(BUILDING_SHARED_LIBRARY)
#    define MERMAID_API MERMAID_SYMBOL_EXPORT

#else								   
#    define MERMAID_API MERMAID_SYMBOL_IMPORT
#endif

#define MERMAID_TEXTIFY2(A) #A
#define MERMAID_TEXTIFY(A) MERMAID_TEXTIFY2(A)