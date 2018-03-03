/**
 * galactic-fall
 * Copyright (C) 2008-2018  Hagen Möbius
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

const Quaternion * VisualizationPrototype::GetMarkerOrientation(const std::string & PartIdentifier, const std::string MarkerIdentifier) const
{
	assert(_Model != nullptr);
	return _Model->GetMarkerOrientation(PartIdentifier, MarkerIdentifier);
}

const Vector3f * VisualizationPrototype::GetMarkerPosition(const std::string & PartIdentifier, const std::string MarkerIdentifier) const
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
