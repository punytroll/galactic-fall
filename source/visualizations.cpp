/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#include <iostream>

#include "color.h"
#include "commodity.h"
#include "commodity_class.h"
#include "globals.h"
#include "graphics/engine.h"
#include "graphics/material.h"
#include "graphics/model.h"
#include "graphics/model_manager.h"
#include "graphics/model_node.h"
#include "graphics/particle_system_node.h"
#include "object_aspect_accessory.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "visualization.h"
#include "visualization_prototype.h"
#include "visualizations.h"
#include "weapon.h"
#include "weapon_class.h"

// This map holds the References to the Graphics::Nodes that have to be referenced in the Game subsystem
std::map< Graphics::Node *, ObjectAspectVisualization * > g_ObjectVisualizations;

static void VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container);
static void VisualizePlanet(Planet * Planet, Graphics::Node * Container);
static void VisualizeShip(Ship * Ship, Graphics::Node * Container);
static void VisualizeShot(Shot * Shot, Graphics::Node * Container);
static void VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container);

void InvalidateVisualizationReference(Graphics::Node * Node)
{
	std::map< Graphics::Node *, ObjectAspectVisualization * >::iterator ObjectVisualizationIterator(g_ObjectVisualizations.find(Node));
	
	if(ObjectVisualizationIterator != g_ObjectVisualizations.end())
	{
		ObjectVisualizationIterator->second->UnsetVisualization();
		g_ObjectVisualizations.erase(ObjectVisualizationIterator);
	}
}

void UnvisualizeObject(Object * Object, Graphics::Node * Container)
{
	assert(Object != 0);
	assert(Object->GetAspectVisualization() != 0);
	assert(Object->GetAspectVisualization()->GetVisualization() != 0);
	assert(Object->GetAspectVisualization()->GetVisualization()->GetGraphics() != 0);
	assert(Object->GetAspectVisualization()->GetVisualization()->GetGraphics()->GetContainer() == Container);
	Object->GetAspectVisualization()->DestroyVisualization();
}

void VisualizeObject(Object * Object, Graphics::Node * Container)
{
	if(Object->GetTypeIdentifier() == "commodity")
	{
		VisualizeCommodity(static_cast< Commodity * >(Object), Container);
	}
	else if(Object->GetTypeIdentifier() == "planet")
	{
		VisualizePlanet(static_cast< Planet * >(Object), Container);
	}
	else if(Object->GetTypeIdentifier() == "ship")
	{
		VisualizeShip(static_cast< Ship * >(Object), Container);
	}
	else if(Object->GetTypeIdentifier() == "shot")
	{
		VisualizeShot(static_cast< Shot * >(Object), Container);
	}
	else if(Object->GetTypeIdentifier() == "weapon")
	{
		VisualizeWeapon(static_cast< Weapon * >(Object), Container);
	}
	else
	{
		std::cerr << "Don't know how to visualize an object with the type identifier \"" << Object->GetTypeIdentifier() << "\"." << std::endl;
		assert(false);
	}
}

void VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container)
{
	assert(Commodity != 0);
	assert(Commodity->GetAspectVisualization() != 0);
	assert(Commodity->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	
	Graphics::Node * Graphics(VisualizePrototype(Commodity->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Graphics->SetUseLighting(true);
	assert(Commodity->GetAspectPosition() != 0);
	Graphics->SetOrientation(Commodity->GetAspectPosition()->GetOrientation());
	Graphics->SetPosition(Commodity->GetAspectPosition()->GetPosition());
	g_ObjectVisualizations[Graphics] = Commodity->GetAspectVisualization();
	// set as the object's visualization
	assert(Commodity->GetAspectVisualization()->GetVisualization() == 0);
	Commodity->GetAspectVisualization()->SetGraphics(Graphics);
	// add to the scene
	assert(Container != 0);
	Container->AddNode(Graphics);
}

void VisualizeParticleSystem(Graphics::ParticleSystem * ParticleSystem, Graphics::Node * Container)
{
	assert(ParticleSystem != 0);
	assert(Container != 0);
	
	Graphics::ParticleSystemNode * Graphics(new Graphics::ParticleSystemNode());
	
	Graphics->SetParticleSystem(ParticleSystem);
	Container->AddNode(Graphics);
}

void VisualizePlanet(Planet * Planet, Graphics::Node * Container)
{
	assert(Planet != 0);
	assert(Planet->GetAspectVisualization() != 0);
	assert(Planet->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	
	Graphics::Node * Graphics(VisualizePrototype(Planet->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Graphics->SetUseLighting(true);
	Graphics->SetOrientation(Quaternion(true));
	assert(Planet->GetAspectPosition() != 0);
	Graphics->SetPosition(Planet->GetAspectPosition()->GetPosition());
	assert(Planet->GetAspectPhysical() != 0);
	Graphics->SetScale(Planet->GetAspectPhysical()->GetRadialSize());
	Graphics->SetNormalize(true);
	g_ObjectVisualizations[Graphics] = Planet->GetAspectVisualization();
	// set as the object's visualization
	assert(Planet->GetAspectVisualization()->GetVisualization() == 0);
	Planet->GetAspectVisualization()->SetGraphics(Graphics);
	// add to the scene
	assert(Container != 0);
	Container->AddNode(Graphics);
}

void VisualizeShip(Ship * Ship, Graphics::Node * Container)
{
	assert(Ship != 0);
	assert(Ship->GetAspectVisualization() != 0);
	assert(Ship->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	
	Graphics::Node * Graphics(VisualizePrototype(Ship->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Graphics->SetUseLighting(true);
	assert(Ship->GetAspectPosition() != 0);
	Graphics->SetOrientation(Ship->GetAspectPosition()->GetOrientation());
	Graphics->SetPosition(Ship->GetAspectPosition()->GetPosition());
	g_ObjectVisualizations[Graphics] = Ship->GetAspectVisualization();
	// set as the object's visualization
	assert(Ship->GetAspectVisualization()->GetVisualization() == 0);
	Ship->GetAspectVisualization()->SetGraphics(Graphics);
	// add to the scene
	assert(Container != 0);
	Container->AddNode(Graphics);
	
	// add accessory visualizations
	assert(Ship->GetAspectOutfitting() != 0);
	
	const std::map< std::string, Slot * > & Slots(Ship->GetAspectOutfitting()->GetSlots());
	
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = Slots.begin(); SlotIterator != Slots.end(); ++SlotIterator)
	{
		if((SlotIterator->second->GetVisualizeAccessory() == true) && (SlotIterator->second->GetMountedObject().IsValid() == true))
		{
			VisualizeObject(SlotIterator->second->GetMountedObject().Get(), Graphics);
		}
	}
	// add engine glow particle system
	assert(Ship->GetEngineGlowParticleSystem() != 0);
	VisualizeParticleSystem(Ship->GetEngineGlowParticleSystem(), Graphics);
}

void VisualizeShot(Shot * Shot, Graphics::Node * Container)
{
	assert(Shot != 0);
	assert(Shot->GetAspectVisualization() != 0);
	assert(Shot->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	
	Graphics::Node * Graphics(VisualizePrototype(Shot->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Graphics->SetUseBlending(true);
	Graphics->SetUseLighting(false);
	assert(Shot->GetAspectPosition() != 0);
	Graphics->SetOrientation(Shot->GetAspectPosition()->GetOrientation());
	Graphics->SetPosition(Shot->GetAspectPosition()->GetPosition());
	g_ObjectVisualizations[Graphics] = Shot->GetAspectVisualization();
	// set as the object's visualization
	assert(Shot->GetAspectVisualization()->GetVisualization() == 0);
	Shot->GetAspectVisualization()->SetGraphics(Graphics);
	// add to the scene
	assert(Container != 0);
	Container->AddNode(Graphics);
}

void VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container)
{
	assert(Weapon != 0);
	assert(Weapon->GetAspectVisualization() != 0);
	assert(Weapon->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	
	Graphics::Node * Graphics(VisualizePrototype(Weapon->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Graphics->SetUseLighting(true);
	assert(Weapon->GetAspectPosition() != 0);
	Graphics->SetOrientation(Weapon->GetAspectAccessory()->GetSlot()->GetOrientation() * Weapon->GetAspectPosition()->GetOrientation());
	Graphics->SetPosition(Weapon->GetAspectAccessory()->GetSlot()->GetPosition() + Weapon->GetAspectPosition()->GetPosition());
	g_ObjectVisualizations[Graphics] = Weapon->GetAspectVisualization();
	// set as the object's visualization
	assert(Weapon->GetAspectVisualization()->GetVisualization() == 0);
	Weapon->GetAspectVisualization()->SetGraphics(Graphics);
	// add to the container node
	assert(Container != 0);
	Container->AddNode(Graphics);
}

Graphics::Node * VisualizePrototype(const VisualizationPrototype * VisualizationPrototype)
{
	assert(VisualizationPrototype != 0);
	
	Graphics::ModelNode * Visualization(new Graphics::ModelNode());
	const std::map< std::string, Graphics::Material * > & PartMaterials(VisualizationPrototype->GetPartMaterials());
	
	for(std::map< std::string, Graphics::Material * >::const_iterator PartMaterialIterator = PartMaterials.begin(); PartMaterialIterator != PartMaterials.end(); ++PartMaterialIterator)
	{
		Visualization->AddMaterial(PartMaterialIterator->first, new Graphics::Material(PartMaterialIterator->second));
	}
	Visualization->SetModel(VisualizationPrototype->GetModel());
	
	return Visualization;
}
