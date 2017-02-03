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
#include <assert.h>

#include "mermaid_engine/managers/plugin/plugin_repository.h"

using namespace Mermaid::Engine::Managers::Plugin;

PluginRepository& PluginRepository::getInstance()
{
    static PluginRepository instance; 
    return instance;
}

void PluginRepository::initialize
(
	Mermaid::Engine::Application::IApplicationContext* applicationContext,
    const juce::String searchPaths, 

    BeforeLoadPluginsCallback  beforeLoadPluginsCallback, 
    PluginLoadedCallback       pluginLoadedCallback, 
    AfterLoadPluginsCallback   afterLoadPluginsCallback,
    PluginLoadingErrorCallback pluginLoadingErrorCallback
)
{
	m_ApplicationContext = applicationContext;
    m_SearchPaths = searchPaths;

    m_BeforeLoadPluginsCallback  = beforeLoadPluginsCallback;
    m_PluginLoadedCallback       = pluginLoadedCallback;
    m_AfterLoadPluginsCallback   = afterLoadPluginsCallback;
    m_PluginLoadingErrorCallback = pluginLoadingErrorCallback; 

    m_PluginEntities.clear();

    startThread();
}

void Mermaid::Engine::Managers::Plugin::PluginRepository::run()
{
    juce::FileSearchPath    fileSearchPath = juce::FileSearchPath(m_SearchPaths);
    juce::Array<juce::File> result;

    fileSearchPath.findChildFiles(result, juce::File::TypesOfFileToFind::findFiles, true, MERMAID_PLUGIN_EXTENSION_NAME);

    m_BeforeLoadPluginsCallback();

    for (juce::File &file : result)
    {
        DLL_HANDLER   handler = Mermaid::Base::SharedLibrary::Load(file.getFullPathName().toStdString());
        IPlugin*      plugin  = MERMAID_START_PLUGIN(handler, m_ApplicationContext);
        
        if(getByPluginUID(plugin->GetPluginUID()) == nullptr)
        {
            m_PluginEntities.add(new PluginEntity(plugin, handler, file.getFullPathName()));
            m_PluginLoadedCallback(plugin, file.getFullPathName());
        }
        else
        {
            m_PluginLoadingErrorCallback(plugin, file.getFullPathName(), PluginLoadingErrorType::ANOTHER_PLUGIN_WITH_THE_SAME_PLUGIN_UID_IS_ALREADY_LOADED);
            MERMAID_STOP_PLUGIN(handler);
        }

        juce::Thread::sleep(1000);
    }

    m_AfterLoadPluginsCallback();
}

juce::Array<PluginEntity*>const & PluginRepository::getAll() const
{  
    return m_PluginEntities;
}

PluginEntity * const PluginRepository::getByPluginUID(const juce::String pluginUID) const
{   
    for (PluginEntity* pluginEntity : m_PluginEntities)
    {
        if (pluginEntity->getPlugin()->GetPluginUID() == pluginUID)
        {
            return pluginEntity;
        }
    }

    return nullptr;
}

void PluginRepository::unloadAll()
{
    for (int i = m_PluginEntities.size() -1; i >= 0; i--)
    {
        PluginRepository::unload(m_PluginEntities[i]);
    }
}

void PluginRepository::unload(PluginEntity* pluginEntity)
{
    if (pluginEntity != nullptr)
    {
        if(m_PluginEntities.size() > 0)
        {
            int index = m_PluginEntities.indexOf(pluginEntity);
            if (index != -1)
            {
                DLL_HANDLER handler = pluginEntity->getHandler();
                MERMAID_STOP_PLUGIN(handler);

                m_PluginEntities.remove(index);
                delete pluginEntity;
                pluginEntity = nullptr;
            }
            else
            {
                throw std::runtime_error("Cannot unload the plugin. Plugin not found.");
            }
        }
    }
    else
    {
        throw std::invalid_argument("The argument pluginEntity shold not be nullptr");
    }

    return;
}

PluginRepository::PluginRepository() : juce::Thread("LoadPlugins")
{

}

PluginRepository::~PluginRepository()
{
    stopThread(2000);
}