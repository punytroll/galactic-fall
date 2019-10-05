/**
 * galactic-fall
 * Copyright (C) 2014-2019  Hagen Möbius
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

#ifndef GRAPHICS_SYSTEM_NODE_H
#define GRAPHICS_SYSTEM_NODE_H

#include "node.h"

namespace Graphics
{
	class SystemNode : public Graphics::Node
	{
	public:
		// constructor & destructor
		SystemNode(void);
		virtual ~SystemNode(void);
		// getters
		Graphics::Node * GetCommodityLayer(void);
		Graphics::Node * GetParticleSystemLayer(void);
		Graphics::Node * GetPlanetLayer(void);
		Graphics::Node * GetShipLayer(void);
		Graphics::Node * GetShotLayer(void);
		// modifiers
		virtual void Destroy(void) override;
	private:
		Graphics::Node * _CommodityLayer;
		Graphics::Node * _ParticleSystemLayer;
		Graphics::Node * _PlanetLayer;
		Graphics::Node * _ShipLayer;
		Graphics::Node * _ShotLayer;
	};
	
	inline Graphics::Node * SystemNode::GetCommodityLayer(void)
	{
		return _CommodityLayer;
	}
	
	inline Graphics::Node * SystemNode::GetParticleSystemLayer(void)
	{
		return _ParticleSystemLayer;
	}
	
	inline Graphics::Node * SystemNode::GetPlanetLayer(void)
	{
		return _PlanetLayer;
	}
	
	inline Graphics::Node * SystemNode::GetShipLayer(void)
	{
		return _ShipLayer;
	}
	
	inline Graphics::Node * SystemNode::GetShotLayer(void)
	{
		return _ShotLayer;
	}
}

#endif
