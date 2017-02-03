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

#include "mermaid_base/mermaid_base.h"

#include "../managers/menu/menu_manager.h"

namespace Mermaid { namespace Engine { namespace Application
{
	class IApplicationContext
	{
	public:
		virtual ~IApplicationContext() {};

		virtual const juce::String getAppName() = 0;
		virtual void		       setAppName(const juce::String& name) = 0;
		virtual const Mermaid::Base::SemanticVersion getAppVersion() = 0;

		virtual  juce::ApplicationCommandManager& getAppCommandManager() = 0;
		virtual  Mermaid::Engine::Managers::Menu::MenuRepository& getAppMenuRepository() = 0;
	};
}}}