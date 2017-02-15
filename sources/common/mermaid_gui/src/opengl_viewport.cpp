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

#include "mermaid_gui/opengl_viewport.h"

Mermaid::GUI::OpenGLViewport::OpenGLViewport(const juce::String& name) : Component(name)
{
    setOpaque(true);
    this->setSize(600, 600);
}

Mermaid::GUI::OpenGLViewport::~OpenGLViewport()
{
    m_OpenGLContext.detach();
}

void Mermaid::GUI::OpenGLViewport::paint(juce::Graphics& graphics)
{
    // You can add your component specific drawing code here!
    // This will draw over the top of the openGL background.
    graphics.fillAll(juce::Colour(0, 0, 0));
    graphics.setColour(juce::Colours::white);
    graphics.setFont(20);
    graphics.drawText("OpenGL Example", 25, 20, 300, 30, juce::Justification::left);
    graphics.drawLine(20, 20, 170, 20);
    graphics.drawLine(20, 50, 170, 50);
}

