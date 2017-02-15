// +---------------------------------------------------------------------------
// |
// |   Mermaid GPL Source Code
// |   Copyright (c) 2013-2016 XNerv Ltda (http://xnerv.com). All rights reserved.
// |
// |   This file is part of the Mermaid GPL Source Code.
// |
// |   Mermaid Source Code is free software: you can redistribute it and/or
// |   modify it under the terms of the GNU General  Public License
// |   as published by the Free Software Foundation, either version 3
// |   of the License, or (at your option) any later version.
// |
// |   Mermaid Source Code is distributed   in the hope that it will be useful,
// |   but WITHOUT ANY WARRANTY; without even the implied warranty of
// |   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// |   GNU General Public License for more details.
// | 
// |   You should have received a copy of the GNU General Public License
// |   along with Mermaid Source Code. If not, see <http://www.gnu.org/licenses/>  .
// |
// +---------------------------------------------------------------------------

#pragma once

#include "juce.h"

#include "mermaid_base/mermaid_base.h"

namespace Mermaid { namespace GUI
{
    class MERMAID_API OpenGLViewport : public juce::Component
    {
    public:
        OpenGLViewport(const juce::String& componentName = juce::String::empty);

        virtual ~OpenGLViewport();

    protected:
        // Inherited via Component
        void paint(juce::Graphics& graphics) override;

    private:
        juce::OpenGLContext m_OpenGLContext;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLViewport)
    };
}}