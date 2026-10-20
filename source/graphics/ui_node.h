/**
 * galactic-fall
 * Copyright (C) 2019-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__GRAPHICS__UI_NODE_H
#define GALACTIC_FALL__SOURCE__GRAPHICS__UI_NODE_H

#include <graphics/node.h>

namespace UI
{
	class UserInterface;
}

namespace Graphics
{
	class RenderContext;
	
	class UINode : public Graphics::Node
	{
	public:
		UINode();
		virtual auto Draw(Graphics::RenderContext * RenderContext) -> void override;
		auto SetUserInterface(UI::UserInterface * UserInterface) -> void;
	private:
		UI::UserInterface * m_UserInterface;
	};
}

#endif
