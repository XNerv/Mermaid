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

#include "mermaid_engine/managers/menu/menu.h"
#include "mermaid_engine/managers/menu/menu_manager.h"

#include "../../application/commandIDs.h"

#include "main_menu_bar.h"

Mermaid::Windows::MainWindow::MainMenuBar::MainMenuBar(juce::ApplicationCommandManager*  applicationCommandManager) : m_ApplicationCommandManager(applicationCommandManager)
{
	setApplicationCommandManagerToWatch(applicationCommandManager);
}

Mermaid::Windows::MainWindow::MainMenuBar::~MainMenuBar()
{

}

juce::StringArray Mermaid::Windows::MainWindow::MainMenuBar::getMenuBarNames()
{
	auto menuRepository = Mermaid::Engine::Managers::Menu::MenuRepository::getInstance();
	auto menus = menuRepository->getMenus();

	juce::StringArray names = juce::StringArray();
	for (int i=0; i<menus->size(); i++)
    {
		auto menu = menus->getUnchecked(i);
		names.add(menu->getName());
	}

	return names;
}

juce::PopupMenu Mermaid::Windows::MainWindow::MainMenuBar::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
	auto applicationMenuManager = Mermaid::Engine::Managers::Menu::MenuRepository::getInstance();
	auto menus = applicationMenuManager->getMenus();

	auto menu = menus->getUnchecked(topLevelMenuIndex);

	return *menu->getPopupMenu();
}

void Mermaid::Windows::MainWindow::MainMenuBar::createMenu(juce::PopupMenu& menu, const juce::String& menuName)
{
	//if (menuName == "File")             createFileMenu(menu);
	//else if (menuName == "Edit")        createEditMenu(menu);
	//else if (menuName == "View")        createViewMenu(menu);
	//else if (menuName == "Window")      createWindowMenu(menu);
	//else if (menuName == "Tools")       createToolsMenu(menu);
	//else if (menuName == "GUI Editor")  createGUIEditorMenu(menu);
	//else                                
	//	jassertfalse; // names have changed?
}


void Mermaid::Windows::MainWindow::MainMenuBar::menuItemSelected(int menuItemID, int topLevelMenuIndexp)
{

}
