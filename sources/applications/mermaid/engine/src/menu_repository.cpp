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

#include <algorithm>

#include "mermaid_engine/managers/menu/menu.h"
#include "mermaid_engine/managers/menu/menu_repository.h"


Mermaid::Engine::Managers::Menu::MenuRepository::MenuRepository()
{
   
}

Mermaid::Engine::Managers::Menu::MenuRepository * const Mermaid::Engine::Managers::Menu::MenuRepository::getInstance()
{
    static MenuRepository instance; 
    return &instance;
}

Mermaid::Engine::Managers::Menu::MenuRepository::Menu * const Mermaid::Engine::Managers::Menu::MenuRepository::addMenu(const int id, const juce::String name)
{
    return m_Menus.add(new Menu(id, name));
}

Mermaid::Engine::Managers::Menu::MenuRepository::Menu * const Mermaid::Engine::Managers::Menu::MenuRepository::getMenu(const int id)
{
	for (auto const &n : m_Menus)
    {
		if(n->getID()==id)
		{
			return n;
		}
    }

	return NULL;
}

juce::OwnedArray<Mermaid::Engine::Managers::Menu::MenuRepository::Menu> * const Mermaid::Engine::Managers::Menu::MenuRepository::getMenus()
{
	return &m_Menus;
}

