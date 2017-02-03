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

#include "../../application/i_application_context.h"

#include "mermaid_base/mermaid_base.h"

namespace Mermaid { namespace Engine { namespace Managers { namespace Plugin
{
	typedef Mermaid::Base::AssetInfo       PluginInfo;
	typedef Mermaid::Base::SemanticVersion PluginVersion;
    typedef Mermaid::Base::SemanticVersion PluginSupportedHostVersion;
	
	class IPlugin: public Mermaid::Base::IDisposable
	{
	public:
		IPlugin(Mermaid::Engine::Application::IApplicationContext * const applicationContext) { m_ApplicationContext=applicationContext;};
		virtual ~IPlugin() {};

		virtual const std::string                GetPluginUID()                  = 0;
		virtual const std::string	             GetPluginCategoryUID()          = 0;
		virtual const PluginInfo                 GetPluginInfo()                 = 0;
		virtual const PluginVersion              GetPluginVersion()              = 0;
		virtual const PluginSupportedHostVersion GetPluginSupportedHostVersion() = 0;

		virtual Mermaid::Engine::Application::IApplicationContext * const getApplicationContext() { return m_ApplicationContext; };
	
	private:
		Mermaid::Engine::Application::IApplicationContext * m_ApplicationContext;
	};
}}}}

#define MERMAID_EXPORT_PLUGIN(PluginClassName) \
PluginClassName* pluginInstance = NULL; \
\
extern "C" MERMAID_API Mermaid::Engine::Managers::Plugin::IPlugin * mermaid_start_plugin(Mermaid::Engine::Application::IApplicationContext * const applicationContext) \
{ \
	if(pluginInstance == NULL) \
	{ \
		pluginInstance = new PluginClassName(applicationContext); \
	} \
	return pluginInstance; \
}; \
\
extern "C" MERMAID_API void mermaid_stop_plugin(void) \
{ \
	if(pluginInstance != NULL) \
	{ \
        pluginInstance->Dispose(); \
		delete pluginInstance; \
		pluginInstance = NULL; \
	} \
}

typedef Mermaid::Engine::Managers::Plugin::IPlugin* (*mermaid_start_plugin_function)(Mermaid::Engine::Application::IApplicationContext * const);
typedef void                                        (*mermaid_stop_plugin_function)  (void);

#define MERMAID_START_PLUGIN(handler, applicationContext) ((mermaid_start_plugin_function) Mermaid::Base::SharedLibrary::GetSymbolPointer(handler, "mermaid_start_plugin"))(applicationContext)
#define MERMAID_STOP_PLUGIN(handler)   ((mermaid_stop_plugin_function)   Mermaid::Base::SharedLibrary::GetSymbolPointer(handler, "mermaid_stop_plugin"))()