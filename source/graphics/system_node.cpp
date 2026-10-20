/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include "system_node.h"

Graphics::SystemNode::SystemNode() :
	m_CommodityLayer{new Graphics::Node()},
	m_ParticleSystemLayer{new Graphics::Node()},
	m_PlanetLayer{new Graphics::Node()},
	m_ShipLayer{new Graphics::Node()},
	m_ShotLayer{new Graphics::Node()}
{
	m_CommodityLayer->SetClearDepthBuffer(true);
	m_PlanetLayer->SetClearDepthBuffer(true);
	m_ShipLayer->SetClearDepthBuffer(true);
	m_ShotLayer->SetClearDepthBuffer(true);
	AddNode(m_PlanetLayer);
	AddNode(m_CommodityLayer);
	AddNode(m_ShipLayer);
	AddNode(m_ShotLayer);
	AddNode(m_ParticleSystemLayer);
}

Graphics::SystemNode::~SystemNode()
{
	assert(m_CommodityLayer == nullptr);
	assert(m_ParticleSystemLayer == nullptr);
	assert(m_PlanetLayer == nullptr);
	assert(m_ShipLayer == nullptr);
	assert(m_ShotLayer == nullptr);
}

void Graphics::SystemNode::Destroy()
{
	assert(m_CommodityLayer != nullptr);
	Node::_Destroy(m_CommodityLayer);
	delete m_CommodityLayer;
	m_CommodityLayer = nullptr;
	assert(m_ParticleSystemLayer != nullptr);
	Node::_Destroy(m_ParticleSystemLayer);
	delete m_ParticleSystemLayer;
	m_ParticleSystemLayer = nullptr;
	assert(m_PlanetLayer != nullptr);
	Node::_Destroy(m_PlanetLayer);
	delete m_PlanetLayer;
	m_PlanetLayer = nullptr;
	assert(m_ShipLayer != nullptr);
	Node::_Destroy(m_ShipLayer);
	delete m_ShipLayer;
	m_ShipLayer = nullptr;
	assert(m_ShotLayer != nullptr);
	Node::_Destroy(m_ShotLayer);
	delete m_ShotLayer;
	m_ShotLayer = nullptr;
	Graphics::Node::Destroy();
}
