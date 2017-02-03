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

namespace Mermaid {  namespace GUI
{
    /**
     * @ingroup    mermaid_gui
     *
     * @brief      An Enhanced Splash screen component.
     */
    class MERMAID_API SplashScreen : public juce::SplashScreen
    {
    public:
        SplashScreen
        (
           const juce::Image& backgroundImage, 
           bool               useDropShadow = false
        );
        
        virtual ~SplashScreen();
        
        virtual void showMessage
        (
            juce::AttributedString& attributedMessage, 
            juce::Rectangle<float>  attributedMessageDrawingArea = juce::Rectangle<float>(0.0f, 0.0f, 0.0f, 0.0f)
        );
  
    protected:
        void paint(juce::Graphics& graphics) override;
    
    private:
        juce::AttributedString m_AttributedMessage;
        juce::Rectangle<float> m_AttributedMessageDrawingArea;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SplashScreen)
    };
}}