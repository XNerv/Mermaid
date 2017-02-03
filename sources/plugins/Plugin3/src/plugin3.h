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

#include "mermaid_engine/mermaid_engine.h"

class Plugin3 : public Mermaid::Engine::Managers::Plugin::IPlugin
{
public:
	Plugin3(Mermaid::Engine::Application::IApplicationContext* applicationContext): IPlugin(applicationContext)
	{
		/*IPlugin::getApplicationContext()->getAppMenuRepository().getMenu(4)->getPopupMenu()->addItem(132545, "Plugin 3");*/
	};

	const std::string                                              GetPluginUID();
	const std::string				                               GetPluginCategoryUID();
	const Mermaid::Engine::Managers::Plugin::PluginInfo                 GetPluginInfo();
	const Mermaid::Engine::Managers::Plugin::PluginVersion              GetPluginVersion();
	const Mermaid::Engine::Managers::Plugin::PluginSupportedHostVersion GetPluginSupportedHostVersion();

	void Dispose();
};

MERMAID_EXPORT_PLUGIN(Plugin3);