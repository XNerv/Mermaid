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

#include <stdexcept>

#include "Plugin3.h"

const std::string Plugin3::GetPluginUID()
{ 
	return "68e0a79f-435a-4395-9560-528a9f347bd8"; 
};

const std::string Plugin3::GetPluginCategoryUID()
{
	return "a7b048fe-179d-459b-86c3-28a1c644b931";
};
 
const Mermaid::Engine::Managers::Plugin::PluginInfo Plugin3::GetPluginInfo()
{
	return Mermaid::Engine::Managers::Plugin::PluginInfo("Plugin3", "XNerv", "(c) 2013 XNerv Ltda", "BSD", "A test plugin.");
};

const Mermaid::Engine::Managers::Plugin::PluginVersion Plugin3::GetPluginVersion()
{
	return Mermaid::Engine::Managers::Plugin::PluginVersion(1, 2, 3, "build.8");
};

const Mermaid::Engine::Managers::Plugin::PluginSupportedHostVersion Plugin3::GetPluginSupportedHostVersion()
{
	return Mermaid::Engine::Managers::Plugin::PluginSupportedHostVersion(4, 5, 6, "build.9");
};

void Plugin3::Dispose()
{
	
};