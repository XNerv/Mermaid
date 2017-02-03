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

#include "mermaid_gui/splash_screen.h"

Mermaid::GUI::SplashScreen::SplashScreen(const juce::Image& image, bool useDropShadow) : juce::SplashScreen("SplashScreen", image, useDropShadow)
{

}

Mermaid::GUI::SplashScreen::~SplashScreen()
{

}

void Mermaid::GUI::SplashScreen::showMessage
(
    juce::AttributedString& attributedMessage, 
    juce::Rectangle<float>  attributedMessageDrawingArea
)
{
    m_AttributedMessage            = attributedMessage;
    m_AttributedMessageDrawingArea = attributedMessageDrawingArea;

    repaint();
}

void Mermaid::GUI::SplashScreen::paint(juce::Graphics& graphics)
{
    juce::SplashScreen::paint(graphics);
    m_AttributedMessage.draw(graphics, m_AttributedMessageDrawingArea);
}