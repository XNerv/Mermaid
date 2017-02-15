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

#include "mermaid_gui/mermaid_gui.h"

namespace Mermaid { namespace Windows { namespace MainWindow
{
	class MainComponent : public juce::Component
	{
	public:
		MainComponent()
		{
            m_Layout_horizontal.setItemLayout(0, 50, 700, 50);
            m_Layout_horizontal.setItemLayout(1, 50, 700, 50);
            m_Layout_horizontal.setItemLayout(2, 8, 8, 8);
            m_Layout_horizontal.setItemLayout(3, 50, 700, 50);
            m_Layout_horizontal.setItemLayout(4, 50, 700, 50);

            m_Layout_vertical.setItemLayout(0, 50, 700, 50);
            m_Layout_vertical.setItemLayout(3, 50, 700, 50);
            m_Layout_vertical.setItemLayout(2, 8, 8, 8);
            m_Layout_vertical.setItemLayout(1, 50, 700, 50);
            m_Layout_vertical.setItemLayout(4, 50, 700, 50);

            m_OpenGLViewport1 = new Mermaid::GUI::OpenGLViewport();
            addAndMakeVisible(m_OpenGLViewport1);

            m_OpenGLViewport2 = new Mermaid::GUI::OpenGLViewport();
            addAndMakeVisible(m_OpenGLViewport2);

            m_OpenGLViewport3 = new Mermaid::GUI::OpenGLViewport();
            addAndMakeVisible(m_OpenGLViewport3);

            m_OpenGLViewport4 = new Mermaid::GUI::OpenGLViewport();
            addAndMakeVisible(m_OpenGLViewport4);

            m_StretchableLayoutResizerBar_horizontal = new juce::StretchableLayoutResizerBar(&m_Layout_horizontal, 2, true);
            addAndMakeVisible(m_StretchableLayoutResizerBar_horizontal);

            m_StretchableLayoutResizerBar_vertical = new juce::StretchableLayoutResizerBar(&m_Layout_vertical, 2, false);
            addAndMakeVisible(m_StretchableLayoutResizerBar_vertical);
		}
	
		~MainComponent()
		{
	
		}
	
		void resized()
		{
            Component* comps_horizontal[] = { m_OpenGLViewport1, m_OpenGLViewport2, m_StretchableLayoutResizerBar_horizontal, m_OpenGLViewport3,  m_OpenGLViewport4 };
            m_Layout_horizontal.layOutComponents(comps_horizontal, 5, 0, 0, getWidth(), getHeight(), false, true);

            //Component* comps_vertical[] = { m_OpenGLViewport1, m_OpenGLViewport3, m_StretchableLayoutResizerBar_vertical,  m_OpenGLViewport2,  m_OpenGLViewport4 };
            //m_Layout_vertical.layOutComponents(comps_vertical, 5, 0, 0, getWidth(), getHeight(), true, true);
		}
	
		void paint(juce::Graphics& graphics)
		{
            graphics.fillAll(juce::Colour(38, 38, 38));
		}

    private:
        juce::StretchableLayoutManager m_Layout_horizontal;
        juce::StretchableLayoutManager m_Layout_vertical;
        
        juce::ScopedPointer<juce::StretchableLayoutResizerBar>  m_StretchableLayoutResizerBar_horizontal;
        juce::ScopedPointer<juce::StretchableLayoutResizerBar>  m_StretchableLayoutResizerBar_vertical;

        juce::ScopedPointer<Mermaid::GUI::OpenGLViewport> m_OpenGLViewport1;
        juce::ScopedPointer<Mermaid::GUI::OpenGLViewport> m_OpenGLViewport2;
        juce::ScopedPointer<Mermaid::GUI::OpenGLViewport> m_OpenGLViewport3;
        juce::ScopedPointer<Mermaid::GUI::OpenGLViewport> m_OpenGLViewport4;
	};
}}}