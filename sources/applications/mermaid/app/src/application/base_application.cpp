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

#include <functional>
#include <typeinfo>

#include "mermaid_engine/managers/menu/menu_manager.h"
#include "application.h"
#include "base_application.h"
#include "messages.h"
#include "resources.h"


void Mermaid::Application::BaseApp::initialise(const juce::String& commandLine)
{
    initCommandManager();

    // Open splash screen.
    m_StartupSplashScreen = new  Mermaid::GUI::SplashScreen
    (
        juce::ImageFileFormat::loadFrom(Embed::app_res::images::splash_screen_png, Embed::app_res::images::splash_screen_png_SIZE)
    );
    
    // Load plugins.
    Mermaid::Engine::Managers::Plugin::PluginRepository::getInstance().initialize
    (
		&Mermaid::Application::App::getInstance(),
        juce::File::getCurrentWorkingDirectory().getParentDirectory().getChildFile(MERMAID_PLUGIN_DIRECTORY_NAME).getFullPathName(),

        std::bind(&Mermaid::Application::BaseApp::beforeLoadPluginsCallback, this),
        std::bind(&Mermaid::Application::BaseApp::pluginLoadedCallback, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&Mermaid::Application::BaseApp::afterLoadPluginsCallback, this),
        std::bind(&Mermaid::Application::BaseApp::pluginLoadingErrorCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    );

	/*Mermaid::Engine::Managers::Menu::MenuRepository::getInstance()->addMenu(1, "File");
	Mermaid::Engine::Managers::Menu::MenuRepository::getInstance()->addMenu(2, "Edit");
	Mermaid::Engine::Managers::Menu::MenuRepository::getInstance()->addMenu(3, "View");
	Mermaid::Engine::Managers::Menu::MenuRepository::getInstance()->addMenu(4, "Plugins");
	Mermaid::Engine::Managers::Menu::MenuRepository::getInstance()->addMenu(5, "Help");

	Mermaid::Engine::Managers::Menu::MenuRepository::getInstance()->getMenu(1)->getPopupMenu()->addItem(256475, "New");
	Mermaid::Engine::Managers::Menu::MenuRepository::getInstance()->getMenu(1)->getPopupMenu()->addItem(568553, "Save");
	Mermaid::Engine::Managers::Menu::MenuRepository::getInstance()->getMenu(1)->getPopupMenu()->addSeparator();*/
}

void Mermaid::Application::BaseApp::beforeLoadPluginsCallback()
{
    
}

void Mermaid::Application::BaseApp::pluginLoadedCallback(Mermaid::Engine::Managers::Plugin::IPlugin* plugin, const juce::String path)
{
    // Update splash screen message.
    juce::File file(path);
    juce::String statusText = file.getFileName();
    juce::Rectangle<float> area(88.0f, 420.0f, 555.0f, 33.0f);
    juce::AttributedString statusTextAttr("Loading . . .  " + statusText);
    statusTextAttr.setJustification(juce::Justification::centredLeft);
    statusTextAttr.setWordWrap(juce::AttributedString::none);
    statusTextAttr.setColour(juce::Colour(212, 212, 212));

    const juce::MessageManagerLock lock (juce::Thread::getCurrentThread());
    if (lock.lockWasGained()) 
    {
        this->m_StartupSplashScreen->showMessage(statusTextAttr, area);
    } 
}

void Mermaid::Application::BaseApp::afterLoadPluginsCallback()
{
    postMessage(new OpenMainWindowMessage());
    
    // Close splash screen.
    const juce::MessageManagerLock lock (juce::Thread::getCurrentThread());
    if (lock.lockWasGained()) 
    {
        m_StartupSplashScreen->deleteAfterDelay(juce::RelativeTime(), false);
    }
}

void Mermaid::Application::BaseApp::pluginLoadingErrorCallback(Mermaid::Engine::Managers::Plugin::IPlugin* const plugin, const juce::String path, const Mermaid::Engine::Managers::Plugin::PluginRepository::PluginLoadingErrorType loadingErrorType)
{
    if(loadingErrorType == Mermaid::Engine::Managers::Plugin::PluginRepository::PluginLoadingErrorType::ANOTHER_PLUGIN_WITH_THE_SAME_PLUGIN_UID_IS_ALREADY_LOADED)
    {
        int a = 2;
    }
}

void Mermaid::Application::BaseApp::handleMessage(const juce::Message& message)
{ 
    if(typeid(message) == typeid(OpenMainWindowMessage))
    {
        openMainWindow();
    }
}

void Mermaid::Application::BaseApp::openMainWindow()
{
	m_MainWindow = new Mermaid::Windows::MainWindow::MainWindow();
    m_MainWindow->centreWithSize(m_MainWindow->getParentWidth(), m_MainWindow->getParentHeight());
    m_MainWindow->setName(getApplicationName());
    m_MainWindow->setVisible(true);
}

const juce::String Mermaid::Application::BaseApp::getApplicationName()
{
    return "MERMAID_APP_NAME";
}

const juce::String Mermaid::Application::BaseApp::getApplicationVersion()
{
    return "MERMAID_APP_VERSION";
}

void Mermaid::Application::BaseApp::shutdown()
{
    if (m_MainWindow != 0)
    {
        deleteAndZero(m_MainWindow);
    }
}

bool Mermaid::Application::BaseApp::moreThanOneInstanceAllowed()
{
    return false;
}

Mermaid::Application::BaseApp& Mermaid::Application::BaseApp::getInstance()
{
    Mermaid::Application::BaseApp* const app = dynamic_cast<Mermaid::Application::BaseApp*>(juce::JUCEApplication::getInstance());
	
    return *app;
}

juce::ApplicationCommandManager& Mermaid::Application::BaseApp::getCommandManager()
{
    juce::ApplicationCommandManager* cm = Mermaid::Application::BaseApp::getInstance().m_CommandManager;
    return *cm;
}

void Mermaid::Application::BaseApp::getAllCommands(juce::Array<juce::CommandID>& commands)
{
    //juce::JUCEApplication::getAllCommands (commands);

    const juce::CommandID ids[] = 
    { 
        CommandIDs::newProject,
        CommandIDs::open,
        CommandIDs::saveAll,
    };

    commands.addArray(ids, juce::numElementsInArray(ids));
}

void Mermaid::Application::BaseApp::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandIDs::newProject:
        result.setInfo("New Project...", "Creates a new Jucer project", CommandCategories::general, 0);
        //result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
        break;

    case CommandIDs::open:
        result.setInfo("Open...", "Opens a Jucer project", CommandCategories::general, 0);
		result.defaultKeypresses.add(juce::KeyPress('o', juce::ModifierKeys::ctrlModifier, 'o'));
        break;

    case CommandIDs::saveAll:
        result.setInfo("Save All", "Saves all open documents", CommandCategories::general, 0);
        //result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
        break;

    default:
        //juce::JUCEApplication::getInstance()->getCommandInfo(commandID, result);
        break;
    }
}

bool Mermaid::Application::BaseApp::perform(const Mermaid::Application::BaseApp::InvocationInfo& info)
{
    //switch (info.commandID)
    //{
    //case CommandIDs::newProject: createNewProject(); break;
    //    /*case CommandIDs::open: askUserToOpenFile(); break;*/
    //default:                     return JUCEApplication::perform(info);
    //}

    return true;
}

void Mermaid::Application::BaseApp::initCommandManager()
{
    m_CommandManager = new juce::ApplicationCommandManager();
    m_CommandManager->registerAllCommandsForTarget(this);
}

Mermaid::Application::BaseApp::BaseApp() : m_MainWindow(0), m_StartupSplashScreen(0)
{
    
}

Mermaid::Application::BaseApp::~BaseApp()
{
    
}