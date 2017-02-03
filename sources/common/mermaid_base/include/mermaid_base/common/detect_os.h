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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(DOXYGEN)
	/** 
	 * 	@ingroup    mermaid_base
	 * 	
	 * 	@def 		MERMAID_OS_FAMILY_WINDOWS
	 * 	
	 *  @brief 		The target operational system is Windows.
	 */
#    define MERMAID_OS_FAMILY_WINDOWS
#endif

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(DOXYGEN)
 	/** 
 	 * 	@ingroup    mermaid_base
 	 * 	
 	 * 	@def 		MERMAID_OS_FAMILY_LINUX
 	 * 	
 	 *  @brief 		The target operational system is Linux.
 	 */
#    define MERMAID_OS_FAMILY_LINUX
#endif

#if defined(__APPLE__) && defined(__MACH__) || defined(DOXYGEN)
	/** 
	 * 	@ingroup    mermaid_base
	 * 	
	 * 	@def 		MERMAID_OS_FAMILY_OSX
	 * 	
	 *  @brief 		The target operational system is OSX.
	 */
#    define MERMAID_OS_FAMILY_OSX
#endif

#if defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))  || defined(DOXYGEN)
	/** 
	 * 	@ingroup    mermaid_base
	 * 	
	 * 	@def 		MERMAID_OS_FAMILY_UNIX
	 * 	
	 *  @brief 		The target operational system is UNIX.
	 */
#    define MERMAID_OS_FAMILY_UNIX
#endif