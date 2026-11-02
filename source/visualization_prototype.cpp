/**
 * galactic-fall
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include "graphics/model.h"
#include "graphics/style.h"
#include "visualization_prototype.h"

VisualizationPrototype::VisualizationPrototype(void) :
	_Model(nullptr)
{
}

VisualizationPrototype::VisualizationPrototype(const VisualizationPrototype * VisualizationPrototype) :
	_Model(VisualizationPrototype->_Model)
{
	for(auto & PartStyle : VisualizationPrototype->_PartStyles)
	{
		_PartStyles[PartStyle.first] = new Graphics::Style(PartStyle.second);
	}
}

VisualizationPrototype::VisualizationPrototype(const VisualizationPrototype & VisualizationPrototype) :
	_Model(VisualizationPrototype._Model)
{
	for(auto & PartStyle : VisualizationPrototype._PartStyles)
	{
		_PartStyles[PartStyle.first] = new Graphics::Style(PartStyle.second);
	}
}

VisualizationPrototype::~VisualizationPrototype(void)
{
	_Model = nullptr;
	while(_PartStyles.empty() == false)
	{
		delete _PartStyles.begin()->second;
		_PartStyles.erase(_PartStyles.begin());
	}
}

auto VisualizationPrototype::GetMarkerLength(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<float> const &
{
	assert(_Model != nullptr);
    
	return _Model->GetMarkerLength(PartIdentifier, MarkerIdentifier);
}

auto VisualizationPrototype::GetMarkerOrientation(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Quaternion> const &
{
	assert(_Model != nullptr);
    
	return _Model->GetMarkerOrientation(PartIdentifier, MarkerIdentifier);
}

auto VisualizationPrototype::GetMarkerPosition(std::string_view PartIdentifier, std::string_view MarkerIdentifier) const -> std::optional<Vector3f> const &
{
	assert(_Model != nullptr);
    
	return _Model->GetMarkerPosition(PartIdentifier, MarkerIdentifier);
}

void VisualizationPrototype::SetPartStyle(const std::string & PartIdentifier, Graphics::Style * PartStyle)
{
	auto PartStyleIterator(_PartStyles.find(PartIdentifier));
	
	if(PartStyleIterator != _PartStyles.end())
	{
		delete PartStyleIterator->second;
		PartStyleIterator->second = PartStyle;
	}
	else
	{
		_PartStyles[PartIdentifier] = PartStyle;
	}
}
