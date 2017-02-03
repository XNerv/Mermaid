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

#include <functional>

#include "juce.h"

#include "plugin_entity.h"

namespace Mermaid { namespace Engine { namespace Managers { namespace Plugin 
{
    class MERMAID_API PluginRepository : juce::Thread
    {
    public:
        enum class PluginLoadingErrorType
        {
            ANOTHER_PLUGIN_WITH_THE_SAME_PLUGIN_UID_IS_ALREADY_LOADED
        };

    public:
        typedef std::function<void (void)>                               BeforeLoadPluginsCallback;
        typedef std::function<void (IPlugin* const, const juce::String)> PluginLoadedCallback;
        typedef std::function<void (void)>                               AfterLoadPluginsCallback;
        typedef std::function<void (IPlugin* const, const juce::String, const PluginLoadingErrorType)> PluginLoadingErrorCallback;

        static PluginRepository& getInstance();

        void initialize
        (
		   Mermaid::Engine::Application::IApplicationContext* applicationContext,
           const juce::String searchPaths, 
        
           BeforeLoadPluginsCallback  beforeLoadPluginsCallback, 
           PluginLoadedCallback       pluginLoadedCallback, 
           AfterLoadPluginsCallback   afterLoadPluginsCallback,
           PluginLoadingErrorCallback pluginLoadingErrorCallback
        ); 

        juce::Array<PluginEntity*> const & getAll() const;
        PluginEntity * const               getByPluginUID(const juce::String pluginUID) const;

        void unloadAll();
        void unload(PluginEntity* pluginEntity);

        
        ~PluginRepository();

    private:
		virtual void run();
        PluginRepository();
        
    private:
		Mermaid::Engine::Application::IApplicationContext* m_ApplicationContext;

        juce::Array<PluginEntity*> m_PluginEntities;
        juce::String               m_SearchPaths;
        
        BeforeLoadPluginsCallback  m_BeforeLoadPluginsCallback;
        PluginLoadedCallback       m_PluginLoadedCallback;
        AfterLoadPluginsCallback   m_AfterLoadPluginsCallback;
        PluginLoadingErrorCallback m_PluginLoadingErrorCallback;
    };
}}}}