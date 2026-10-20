/**
 * galactic-fall
 * Copyright (C) 2019  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <graphics/render_context.h>

#include "../ui/user_interface.h"
#include "ui_node.h"

Graphics::UINode::UINode() :
	m_UserInterface{nullptr}
{
}

auto Graphics::UINode::Draw(Graphics::RenderContext * RenderContext) -> void
{
	if(m_UserInterface != nullptr)
	{
		m_UserInterface->Draw(RenderContext);
	}
}

auto Graphics::UINode::SetUserInterface(UI::UserInterface * UserInterface) -> void
{
	m_UserInterface = UserInterface;
}
