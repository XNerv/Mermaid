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

Mermaid::Engine::Managers::Menu::MenuRepository::Menu::Menu()
{
    
}

Mermaid::Engine::Managers::Menu::MenuRepository::Menu::Menu(const int id, const juce::String name)
{		        
    m_ID        = id;
	m_Name      = name;
	m_PopupMenu = new juce::PopupMenu();
}

Mermaid::Engine::Managers::Menu::MenuRepository::Menu::~Menu()
{
    
}

const int Mermaid::Engine::Managers::Menu::MenuRepository::Menu::getID()
{
    return m_ID;
}

const juce::String Mermaid::Engine::Managers::Menu::MenuRepository::Menu::getName()
{
    return m_Name;
}

void Mermaid::Engine::Managers::Menu::MenuRepository::Menu::setName(const juce::String name)
{
    m_Name = name;
}

juce::PopupMenu * const  Mermaid::Engine::Managers::Menu::MenuRepository::Menu::getPopupMenu()
{
    return m_PopupMenu;
}

