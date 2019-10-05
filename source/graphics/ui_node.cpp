/**
 * galactic-fall
 * Copyright (C) 2019  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include <graphics/render_context.h>

#include "../ui/user_interface.h"
#include "ui_node.h"

Graphics::UINode::UINode(void) :
	_UserInterface{nullptr}
{
}

void Graphics::UINode::Draw(Graphics::RenderContext * RenderContext)
{
	if(_UserInterface != nullptr)
	{
		_UserInterface->Draw(RenderContext);
	}
}

void Graphics::UINode::SetUserInterface(UI::UserInterface * UserInterface)
{
	_UserInterface = UserInterface;
}
