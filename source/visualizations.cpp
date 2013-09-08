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
#include "graphics/particle_system.h"
#include "object_aspect_accessory.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "visualization_prototype.h"
#include "visualizations.h"
#include "weapon.h"
#include "weapon_class.h"

// This map holds the References to the Graphics::Nodes that have to be referenced in the Game subsystem
std::map< Graphics::Node *, Reference< Graphics::Node > > g_VisualizationReferences;

Graphics::ParticleSystem * CreateParticleSystem(const std::string & ParticleSystemClassIdentifier);

static void VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container);
static void VisualizePlanet(Planet * Planet, Graphics::Node * Container);
static void VisualizeShip(Ship * Ship, Graphics::Node * Container);
static void VisualizeShot(Shot * Shot, Graphics::Node * Container);
static void VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container);

void InvalidateVisualizationReference(Graphics::Node * Node)
{
	std::map< Graphics::Node *, Reference< Graphics::Node > >::iterator VisualizationReferenceIterator(g_VisualizationReferences.find(Node));
	
	if(VisualizationReferenceIterator != g_VisualizationReferences.end())
	{
		VisualizationReferenceIterator->second.Invalidate();
		g_VisualizationReferences.erase(VisualizationReferenceIterator);
	}
}

void UnvisualizeObject(Object * Object)
{
	assert(Object != 0);
	assert(Object->GetAspectVisualization() != 0);
	assert(Object->GetAspectVisualization()->GetVisualization().IsValid() == true);
	Object->GetAspectVisualization()->UnsetVisualization();
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
	
	Graphics::Node * Visualization(VisualizePrototype(Commodity->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Visualization->SetUseLighting(true);
	assert(Commodity->GetAspectPosition() != 0);
	Visualization->SetOrientation(Commodity->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Commodity->GetAspectPosition()->GetPosition());
	
	// create and store the visualization reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	assert(Commodity->GetAspectVisualization()->GetVisualization().IsValid() == false);
	Commodity->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the scene
	assert(Container != 0);
	Container->AddNode(Visualization);
}

Reference< Graphics::Node > & VisualizeParticleSystem(Graphics::Node * ParticleSystem, Graphics::Node * Container)
{
	assert(ParticleSystem != 0);
	assert(Container != 0);
	
	Reference< Graphics::Node > ParticleSystemReference(*ParticleSystem);
	Reference< Graphics::Node > & Result(g_VisualizationReferences.insert(std::make_pair(ParticleSystem, ParticleSystemReference)).first->second);
	
	Container->AddNode(ParticleSystem);
	
	return Result;
}

void VisualizePlanet(Planet * Planet, Graphics::Node * Container)
{
	assert(Planet != 0);
	assert(Planet->GetAspectVisualization() != 0);
	assert(Planet->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	
	Graphics::Node * Visualization(VisualizePrototype(Planet->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Visualization->SetUseLighting(true);
	Visualization->SetOrientation(Quaternion(true));
	assert(Planet->GetAspectPosition() != 0);
	Visualization->SetPosition(Planet->GetAspectPosition()->GetPosition());
	assert(Planet->GetAspectPhysical() != 0);
	Visualization->SetScale(Planet->GetAspectPhysical()->GetRadialSize());
	Visualization->SetNormalize(true);
	
	// create and store the visualization reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	assert(Planet->GetAspectVisualization()->GetVisualization().IsValid() == false);
	Planet->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the scene
	assert(Container != 0);
	Container->AddNode(Visualization);
}

void VisualizeShip(Ship * Ship, Graphics::Node * Container)
{
	assert(Ship != 0);
	assert(Ship->GetAspectVisualization() != 0);
	assert(Ship->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	
	Graphics::Node * Visualization(VisualizePrototype(Ship->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Visualization->SetUseLighting(true);
	assert(Ship->GetAspectPosition() != 0);
	Visualization->SetOrientation(Ship->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Ship->GetAspectPosition()->GetPosition());
	
	// create and store the visualization reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	assert(Ship->GetAspectVisualization()->GetVisualization().IsValid() == false);
	Ship->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the scene
	assert(Container != 0);
	Container->AddNode(Visualization);
	
	// add accessory visualizations
	assert(Ship->GetAspectOutfitting() != 0);
	
	const std::map< std::string, Slot * > & Slots(Ship->GetAspectOutfitting()->GetSlots());
	
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = Slots.begin(); SlotIterator != Slots.end(); ++SlotIterator)
	{
		if((SlotIterator->second->GetVisualizeAccessory() == true) && (SlotIterator->second->GetMountedObject().IsValid() == true))
		{
			VisualizeObject(SlotIterator->second->GetMountedObject().Get(), Visualization);
		}
	}
	
	// add engine glow particle system
	Graphics::ParticleSystem * EngineGlowParticleSystem(CreateParticleSystem("engine_glow"));
	Reference< Graphics::ParticleSystem > EngineGlowParticleSystemReference(VisualizeParticleSystem(EngineGlowParticleSystem, Visualization));
	
	EngineGlowParticleSystem->SetPosition(Vector3f(Ship->GetExhaustOffset()));
	assert(Ship->GetEngineGlowParticleSystem().IsValid() == false);
	Ship->SetEngineGlowParticleSystem(EngineGlowParticleSystemReference);
}

void VisualizeShot(Shot * Shot, Graphics::Node * Container)
{
	assert(Shot != 0);
	assert(Shot->GetAspectVisualization() != 0);
	assert(Shot->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	
	Graphics::Node * Visualization(VisualizePrototype(Shot->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Visualization->SetUseBlending(true);
	Visualization->SetUseLighting(false);
	assert(Shot->GetAspectPosition() != 0);
	Visualization->SetOrientation(Shot->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Shot->GetAspectPosition()->GetPosition());
	
	// create and store the visualization reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	assert(Shot->GetAspectVisualization()->GetVisualization().IsValid() == false);
	Shot->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the scene
	assert(Container != 0);
	Container->AddNode(Visualization);
}

void VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container)
{
	assert(Weapon != 0);
	assert(Weapon->GetAspectVisualization() != 0);
	assert(Weapon->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	
	Graphics::Node * Visualization(VisualizePrototype(Weapon->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Visualization->SetUseLighting(true);
	assert(Weapon->GetAspectPosition() != 0);
	Visualization->SetOrientation(Weapon->GetAspectAccessory()->GetSlot()->GetOrientation() * Weapon->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Weapon->GetAspectAccessory()->GetSlot()->GetPosition() + Weapon->GetAspectPosition()->GetPosition());
	
	// create and store the visualization reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	assert(Weapon->GetAspectVisualization()->GetVisualization().IsValid() == false);
	Weapon->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the container node
	assert(Container != 0);
	Container->AddNode(Visualization);
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
