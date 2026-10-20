/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__GRAPHICS__SYSTEM_NODE_H
#define GALACTIC_FALL__SOURCE__GRAPHICS__SYSTEM_NODE_H

#include <graphics/node.h>

namespace Graphics
{
	class SystemNode : public Graphics::Node
	{
	public:
		// constructor & destructor
		SystemNode();
		virtual ~SystemNode();
		// getters
		auto GetCommodityLayer() -> Graphics::Node *;
		auto GetParticleSystemLayer() -> Graphics::Node *;
		auto GetPlanetLayer() -> Graphics::Node *;
		auto GetShipLayer() -> Graphics::Node *;
		auto GetShotLayer() -> Graphics::Node *;
		// modifiers
		virtual void Destroy() override;
        
	private:
		Graphics::Node * m_CommodityLayer;
		Graphics::Node * m_ParticleSystemLayer;
		Graphics::Node * m_PlanetLayer;
		Graphics::Node * m_ShipLayer;
		Graphics::Node * m_ShotLayer;
	};
	
	inline auto SystemNode::GetCommodityLayer() -> Graphics::Node *
	{
		return m_CommodityLayer;
	}
	
	inline auto SystemNode::GetParticleSystemLayer() -> Graphics::Node *
	{
		return m_ParticleSystemLayer;
	}
	
	inline auto SystemNode::GetPlanetLayer() -> Graphics::Node *
	{
		return m_PlanetLayer;
	}
	
	inline auto SystemNode::GetShipLayer() -> Graphics::Node *
	{
		return m_ShipLayer;
	}
	
	inline auto SystemNode::GetShotLayer() -> Graphics::Node *
	{
		return m_ShotLayer;
	}
}

#endif
