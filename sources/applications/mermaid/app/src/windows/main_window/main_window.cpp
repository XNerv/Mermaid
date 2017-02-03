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

#include "../../application/application.h"

#include "main_window.h"
 
Mermaid::Windows::MainWindow::MainWindow::MainWindow() : DocumentWindow
(   
    Mermaid::Application::App::getInstance().getApplicationName(), 
    juce::Colours::darkgrey, 
    DocumentWindow::allButtons,
    true
)
{
    setResizable(true, false); 
	setTitleBarHeight(25);
    setContentOwned(m_MainComponent, false);

	this->m_MainMenuBar = new MainMenuBar(&Mermaid::Application::App::getInstance().getCommandManager());
	setMenuBar(this->m_MainMenuBar);

	

	/*addKeyListener(Mermaid::Application::App::getInstance().getCommandManager().getKeyMappings());*/
}

Mermaid::Windows::MainWindow::MainWindow::~MainWindow()
{

}