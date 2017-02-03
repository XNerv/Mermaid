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

#include "juce.h"

#include "commandIDs.h"

#include "mermaid_engine/managers/plugin/plugin_manager.h"
#include "mermaid_gui/mermaid_gui.h"

#include "windows/main_window/main_window.h"

namespace Mermaid { namespace Application
{
    class BaseApp : public juce::JUCEApplication, juce::MessageListener
    {
    public:
        static BaseApp& getInstance();

        const juce::String getApplicationName() override;
        const juce::String getApplicationVersion() override;

        static juce::ApplicationCommandManager& getCommandManager();
        
        BaseApp();
        ~BaseApp();

    private:
        void         initialise(const juce::String& commandLine) override;
        void         shutdown() override;
        virtual void handleMessage(const juce::Message& message);
        bool moreThanOneInstanceAllowed() override;
        void openMainWindow();

        void initCommandManager();
        void getAllCommands(juce::Array<juce::CommandID>& commands) override;
        void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;
    
        void beforeLoadPluginsCallback();
        void pluginLoadedCallback(Mermaid::Engine::Managers::Plugin::IPlugin* plugin, const juce::String path);
        void afterLoadPluginsCallback();
        void pluginLoadingErrorCallback(Mermaid::Engine::Managers::Plugin::IPlugin* const plugin, const juce::String path, const Mermaid::Engine::Managers::Plugin::PluginRepository::PluginLoadingErrorType loadingErrorType);
    
    private:
        Mermaid::GUI::SplashScreen*				  m_StartupSplashScreen;
		Mermaid::Windows::MainWindow::MainWindow* m_MainWindow;

        juce::ScopedPointer<juce::ApplicationCommandManager> m_CommandManager;
    };
}}