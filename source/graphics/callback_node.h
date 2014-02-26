/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#ifndef GRAPHICS_CALLBACK_NODE_H
#define GRAPHICS_CALLBACK_NODE_H

#include <functional>

#include "node.h"

namespace Graphics
{
	class CallbackNode : public Graphics::Node
	{
	public:
		// constructor & destructor
		CallbackNode(void);
		virtual ~CallbackNode(void);
		// setters
		void SetDrawCallback(std::function< void (void) > Callback);
		// modifiers
		virtual void Draw(void);
	private:
		std::function< void (void) > _DrawCallback;
	};
	
	inline void CallbackNode::SetDrawCallback(std::function< void (void) > Callback)
	{
		_DrawCallback = Callback;
	}
}

#endif
