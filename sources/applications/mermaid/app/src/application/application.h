// +---------------------------------------------------------------------------
// |
// |   Mermaid GPL Source Code
// |   Copyright (c) 2013-2016 XNerv Ltda (http://xnerv.com). All rights reserved.
// |
// |   This file is part of the Mermaid GPL Source Code.
// |
// |   Mermaid Source Code is free software: you can redistribute it and/or
// |   modify it under the terms of the GNU General Public Li  cense
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

#include "mermaid_engine/application/i_application_context.h"

#include "base_application.h"

namespace Mermaid { namespace Application
{
    class App : public BaseApp, public Mermaid::Engine::Application::IApplicationContext
    {
    public:
		static App& getInstance();

        const juce::String getAppName() override;
		void		       setAppName(const juce::String& name) override;
		const Mermaid::Base::SemanticVersion getAppVersion() override;

		juce::ApplicationCommandManager& getAppCommandManager() override;
		Mermaid::Engine::Managers::Menu::MenuRepository& getAppMenuRepository() override;

        App();
        ~App();
    };
}}