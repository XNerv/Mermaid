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

#include "mermaid_engine/managers/plugin/plugin_entity.h"

//void Mermaid::Base::Extensibility::PluginEntity::initialize
//(
//    IPlugin* const plugin, DLL_HANDLER handler, const juce::String& pluginPhysicalPath
//) 
//{
//    m_Plugin             = plugin;
//    m_PluginPhysicalPath = pluginPhysicalPath;
//    m_Handler            = handler;
//}

const juce::String Mermaid::Engine::Managers::Plugin::PluginEntity::getPluginPhysicalPath() const
{
    return m_PluginPhysicalPath;
}

Mermaid::Engine::Managers::Plugin::IPlugin* const Mermaid::Engine::Managers::Plugin::PluginEntity::getPlugin() const
{
    return m_Plugin;
}

DLL_HANDLER Mermaid::Engine::Managers::Plugin::PluginEntity::getHandler() const
{
    return m_Handler;
}

Mermaid::Engine::Managers::Plugin::PluginEntity::PluginEntity(const PluginEntity& pluginEntity)
{
    m_Plugin = pluginEntity.m_Plugin;
    m_PluginPhysicalPath = pluginEntity.m_PluginPhysicalPath;
    m_Handler = pluginEntity.m_Handler;
}

Mermaid::Engine::Managers::Plugin::PluginEntity& Mermaid::Engine::Managers::Plugin::PluginEntity::operator=(const PluginEntity& pluginEntity)
{
    m_Plugin = pluginEntity.m_Plugin;
    m_PluginPhysicalPath = pluginEntity.m_PluginPhysicalPath;
    m_Handler = pluginEntity.m_Handler;

    return *this;
}

bool Mermaid::Engine::Managers::Plugin::PluginEntity::operator==(const PluginEntity& pluginEntity) const
{
   return (m_Plugin->GetPluginUID() == pluginEntity.getPlugin()->GetPluginUID());
}

bool Mermaid::Engine::Managers::Plugin::PluginEntity::operator!=(const PluginEntity& pluginEntity) const
{
   return !operator==(pluginEntity);
}

Mermaid::Engine::Managers::Plugin::PluginEntity::PluginEntity(IPlugin* const plugin, DLL_HANDLER handler, const juce::String& pluginPhysicalPath)
{
    m_Plugin             = plugin;
    m_PluginPhysicalPath = pluginPhysicalPath;
    m_Handler            = handler;
}

Mermaid::Engine::Managers::Plugin::PluginEntity::~PluginEntity()
{

}