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

#include "system_node.h"

Graphics::SystemNode::SystemNode(void) :
	_CommodityLayer(new Graphics::Node()),
	_ParticleSystemLayer(new Graphics::Node()),
	_PlanetLayer(new Graphics::Node()),
	_ShipLayer(new Graphics::Node()),
	_ShotLayer(new Graphics::Node())
{
	_CommodityLayer->SetClearDepthBuffer(true);
	_PlanetLayer->SetClearDepthBuffer(true);
	_ShipLayer->SetClearDepthBuffer(true);
	_ShotLayer->SetClearDepthBuffer(true);
	AddNode(_PlanetLayer);
	AddNode(_CommodityLayer);
	AddNode(_ShipLayer);
	AddNode(_ShotLayer);
	AddNode(_ParticleSystemLayer);
}

Graphics::SystemNode::~SystemNode(void)
{
	assert(_CommodityLayer == nullptr);
	assert(_ParticleSystemLayer == nullptr);
	assert(_PlanetLayer == nullptr);
	assert(_ShipLayer == nullptr);
	assert(_ShotLayer == nullptr);
}

void Graphics::SystemNode::Destroy(void)
{
	assert(_CommodityLayer != nullptr);
	Node::_Destroy(_CommodityLayer);
	delete _CommodityLayer;
	_CommodityLayer = nullptr;
	assert(_ParticleSystemLayer != nullptr);
	Node::_Destroy(_ParticleSystemLayer);
	delete _ParticleSystemLayer;
	_ParticleSystemLayer = nullptr;
	assert(_PlanetLayer != nullptr);
	Node::_Destroy(_PlanetLayer);
	delete _PlanetLayer;
	_PlanetLayer = nullptr;
	assert(_ShipLayer != nullptr);
	Node::_Destroy(_ShipLayer);
	delete _ShipLayer;
	_ShipLayer = nullptr;
	assert(_ShotLayer != nullptr);
	Node::_Destroy(_ShotLayer);
	delete _ShotLayer;
	_ShotLayer = nullptr;
	Graphics::Node::Destroy();
}
