/**
 * galactic-fall
 * Copyright (C) 2006-2019  Hagen Möbius
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

#include <algebra/quaternion.h>
#include <algebra/vector2f.h>
#include <algebra/vector3f.h>

#include <graphics/color_rgb.h>
#include <graphics/color_rgbo.h>
#include <graphics/engine.h>
#include <graphics/model_manager.h>
#include <graphics/style.h>

#include "buffer_reading.h"
#include "globals.h"
#include "visualization_prototype.h"

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Graphics::ColorRGB & Color)
{
	float Red;
	float Green;
	float Blue;
	
	BufferReader >> Red >> Green >> Blue;
	Color.Set(Red, Green, Blue);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Graphics::ColorRGBO & Color)
{
	float Red;
	float Green;
	float Blue;
	float Opacity;
	
	BufferReader >> Red >> Green >> Blue >> Opacity;
	Color.Set(Red, Green, Blue, Opacity);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Quaternion & Quaternion)
{
	float W;
	float X;
	float Y;
	float Z;
	
	BufferReader >> W >> X >> Y >> Z;
	Quaternion.Set(W, X, Y, Z);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Vector2f & Vector)
{
	float x;
	float y;
	
	BufferReader >> x >> y;
	Vector.Set(x, y);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, Vector3f & Vector)
{
	float x;
	float y;
	float z;
	
	BufferReader >> x >> y >> z;
	Vector.Set(x, y, z);
	
	return BufferReader;
}

Arxx::BufferReader & operator>>(Arxx::BufferReader & BufferReader, VisualizationPrototype & VisualizationPrototype)
{
	std::string ModelIdentifier;
	
	BufferReader >> ModelIdentifier;
	
	assert(g_GraphicsEngine != nullptr);
	assert(g_GraphicsEngine->GetModelManager() != nullptr);
	
	auto Model(g_GraphicsEngine->GetModelManager()->Get(ModelIdentifier));
	
	if(Model == nullptr)
	{
		throw std::runtime_error("Could not find the model \"" + ModelIdentifier + "\".");
	}
	VisualizationPrototype.SetModel(Model);
	
	std::uint32_t VisualizationPartCount;
	
	BufferReader >> VisualizationPartCount;
	for(auto VisualizationPartIndex = 0ul; VisualizationPartIndex < VisualizationPartCount; ++VisualizationPartIndex)
	{
		std::string PartIdentifier;
		Graphics::ColorRGBO PartDiffuseColor;
		bool PartValidSpecularColor;
		Graphics::ColorRGBO PartSpecularColor;
		bool PartValidShininess;
		float PartShininess;
		std::string PartProgramIdentifier;
		
		BufferReader >> PartIdentifier >> PartDiffuseColor >> PartValidSpecularColor >> PartSpecularColor >> PartValidShininess >> PartShininess >> PartProgramIdentifier;
		
		auto PartStyle(new Graphics::Style());
		
		PartStyle->SetDiffuseColor(PartDiffuseColor);
		if(PartValidSpecularColor == true)
		{
			PartStyle->SetSpecularColor(PartSpecularColor);
		}
		if(PartValidShininess == true)
		{
			PartStyle->SetShininess(PartShininess);
		}
		PartStyle->SetProgramIdentifier(PartProgramIdentifier);
		VisualizationPrototype.SetPartStyle(PartIdentifier, PartStyle);
	}
	
	return BufferReader;
}
