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

#include <GL/gl.h>

#include "color.h"
#include "commodity.h"
#include "commodity_class.h"
#include "globals.h"
#include "graphics/engine.h"
#include "graphics/model.h"
#include "graphics/model_manager.h"
#include "graphics/model_node.h"
#include "graphics/particle_system_node.h"
#include "graphics/style.h"
#include "graphics/system_node.h"
#include "object_aspect_accessory.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_physical.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "system.h"
#include "visualization.h"
#include "visualization_prototype.h"
#include "visualizations.h"
#include "weapon.h"
#include "weapon_class.h"

// This map holds the References to the Graphics::Nodes that have to be referenced in the Game subsystem
std::map< Graphics::Node *, ObjectAspectVisualization * > g_ObjectVisualizations;

static Visualization * VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container);
static Visualization * VisualizePlanet(Planet * Planet, Graphics::Node * Container);
static Visualization * VisualizeShip(Ship * Ship, Graphics::Node * Container);
static Visualization * VisualizeShot(Shot * Shot, Graphics::Node * Container);
static Visualization * VisualizeSystem(System * System, Graphics::Node * Container);
static Visualization * VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container);

void InvalidateVisualizationReference(Graphics::Node * Node)
{
	std::map< Graphics::Node *, ObjectAspectVisualization * >::iterator ObjectVisualizationIterator(g_ObjectVisualizations.find(Node));
	
	if(ObjectVisualizationIterator != g_ObjectVisualizations.end())
	{
		ObjectVisualizationIterator->second->RemoveGraphics(Node);
		g_ObjectVisualizations.erase(ObjectVisualizationIterator);
	}
}

Visualization * VisualizeObject(Object * Object, Graphics::Node * Container)
{
	if(Object->GetTypeIdentifier() == "commodity")
	{
		return VisualizeCommodity(static_cast< Commodity * >(Object), Container);
	}
	else if(Object->GetTypeIdentifier() == "planet")
	{
		return VisualizePlanet(static_cast< Planet * >(Object), Container);
	}
	else if(Object->GetTypeIdentifier() == "ship")
	{
		return VisualizeShip(static_cast< Ship * >(Object), Container);
	}
	else if(Object->GetTypeIdentifier() == "shot")
	{
		return VisualizeShot(static_cast< Shot * >(Object), Container);
	}
	else if(Object->GetTypeIdentifier() == "system")
	{
		return VisualizeSystem(static_cast< System * >(Object), Container);
	}
	else if(Object->GetTypeIdentifier() == "weapon")
	{
		return VisualizeWeapon(static_cast< Weapon * >(Object), Container);
	}
	else
	{
		std::cerr << "Don't know how to visualize an object with the type identifier \"" << Object->GetTypeIdentifier() << "\"." << std::endl;
		assert(false);
	}
}

Visualization * VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container)
{
	assert(Commodity != nullptr);
	assert(Commodity->GetAspectVisualization() != nullptr);
	assert(Commodity->GetAspectVisualization()->GetVisualizationPrototype() != nullptr);
	
	Graphics::Node * Graphics(VisualizePrototype(Commodity->GetAspectVisualization()->GetVisualizationPrototype()));
	
	g_ObjectVisualizations[Graphics] = Commodity->GetAspectVisualization();
	
	// set as the object's visualization
	auto Visualization(Commodity->GetAspectVisualization()->CreateVisualizationForGraphics(Graphics));
	
	// add to the scene
	assert(Container != nullptr);
	Container->AddNode(Graphics);
	
	return Visualization;
}

void VisualizeParticleSystem(Graphics::ParticleSystem * ParticleSystem, System * System)
{
	assert(ParticleSystem != nullptr);
	assert(System != nullptr);
	assert(System->GetAspectVisualization() != nullptr);
	for(auto Visualization : System->GetAspectVisualization()->GetVisualizations())
	{
		VisualizeParticleSystem(ParticleSystem, (static_cast< Graphics::SystemNode * >(Visualization->GetGraphics()))->GetParticleSystemLayer());
	}
}

void VisualizeParticleSystem(Graphics::ParticleSystem * ParticleSystem, Graphics::Node * Container)
{
	assert(ParticleSystem != nullptr);
	assert(Container != nullptr);
	
	Graphics::ParticleSystemNode * Graphics(new Graphics::ParticleSystemNode());
	
	Graphics->SetParticleSystem(ParticleSystem);
	Container->AddNode(Graphics);
}

Visualization * VisualizePlanet(Planet * Planet, Graphics::Node * Container)
{
	assert(Planet != nullptr);
	assert(Planet->GetAspectVisualization() != nullptr);
	assert(Planet->GetAspectVisualization()->GetVisualizationPrototype() != nullptr);
	
	Graphics::Node * Graphics(VisualizePrototype(Planet->GetAspectVisualization()->GetVisualizationPrototype()));
	
	assert(Planet->GetAspectPhysical() != nullptr);
	Graphics->SetScale(Planet->GetAspectPhysical()->GetRadialSize());
	Graphics->SetNormalize(true);
	g_ObjectVisualizations[Graphics] = Planet->GetAspectVisualization();
	
	// set as the object's visualization
	auto Visualization(Planet->GetAspectVisualization()->CreateVisualizationForGraphics(Graphics));
	
	// add to the scene
	assert(Container != nullptr);
	Container->AddNode(Graphics);
	
	return Visualization;
}

Visualization * VisualizeShip(Ship * Ship, Graphics::Node * Container)
{
	assert(Ship != nullptr);
	assert(Ship->GetAspectVisualization() != nullptr);
	assert(Ship->GetAspectVisualization()->GetVisualizationPrototype() != nullptr);
	
	Graphics::Node * Graphics(VisualizePrototype(Ship->GetAspectVisualization()->GetVisualizationPrototype()));
	
	g_ObjectVisualizations[Graphics] = Ship->GetAspectVisualization();
	
	// set as the object's visualization
	auto Visualization(Ship->GetAspectVisualization()->CreateVisualizationForGraphics(Graphics));
	
	// add to the scene
	assert(Container != nullptr);
	Container->AddNode(Graphics);
	
	// add accessory visualizations
	assert(Ship->GetAspectOutfitting() != nullptr);
	
	const std::map< std::string, Slot * > & Slots(Ship->GetAspectOutfitting()->GetSlots());
	
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = Slots.begin(); SlotIterator != Slots.end(); ++SlotIterator)
	{
		if((SlotIterator->second->GetVisualizeAccessory() == true) && (SlotIterator->second->GetMountedObject() != nullptr))
		{
			VisualizeObject(SlotIterator->second->GetMountedObject(), Graphics);
		}
	}
	// add engine glow particle system
	assert(Ship->GetEngineGlowParticleSystem() != nullptr);
	VisualizeParticleSystem(Ship->GetEngineGlowParticleSystem(), Graphics);
	
	return Visualization;
}

Visualization * VisualizeShot(Shot * Shot, Graphics::Node * Container)
{
	assert(Shot != nullptr);
	assert(Shot->GetAspectVisualization() != nullptr);
	assert(Shot->GetAspectVisualization()->GetVisualizationPrototype() != nullptr);
	
	Graphics::Node * Graphics(VisualizePrototype(Shot->GetAspectVisualization()->GetVisualizationPrototype()));
	
	Graphics->SetUseBlending(true);
	Graphics->SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	g_ObjectVisualizations[Graphics] = Shot->GetAspectVisualization();
	
	// set as the object's visualization
	auto Visualization(Shot->GetAspectVisualization()->CreateVisualizationForGraphics(Graphics));
	
	// add to the scene
	assert(Container != nullptr);
	Container->AddNode(Graphics);
	
	return Visualization;
}

Visualization * VisualizeSystem(System * System, Graphics::Node * Container)
{
	assert(System != nullptr);
	assert(System->GetAspectVisualization() != nullptr);
	
	auto Graphics(new Graphics::SystemNode());
	
	g_ObjectVisualizations[Graphics] = System->GetAspectVisualization();
	
	// set as the object's visualization
	auto Visualization(System->GetAspectVisualization()->CreateVisualizationForGraphics(Graphics));
	
	// add visualizations for all objects in the system
	for(auto Planet : System->GetPlanets())
	{
		VisualizeObject(Planet, Graphics->GetPlanetLayer());
	}
	
	const std::list< Ship * > & Ships();
	
	for(auto Ship : System->GetShips())
	{
		VisualizeObject(Ship, Graphics->GetShipLayer());
	}
	// add to the container node
	assert(Container != nullptr);
	Container->AddNode(Graphics);
	
	return Visualization;
}

Visualization * VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container)
{
	assert(Weapon != nullptr);
	assert(Weapon->GetAspectVisualization() != nullptr);
	assert(Weapon->GetAspectVisualization()->GetVisualizationPrototype() != nullptr);
	
	Graphics::Node * Graphics(VisualizePrototype(Weapon->GetAspectVisualization()->GetVisualizationPrototype()));
	
	g_ObjectVisualizations[Graphics] = Weapon->GetAspectVisualization();
	
	// set as the object's visualization
	auto Visualization(Weapon->GetAspectVisualization()->CreateVisualizationForGraphics(Graphics));
	
	// add to the container node
	assert(Container != nullptr);
	Container->AddNode(Graphics);
	
	return Visualization;
}

Graphics::Node * VisualizePrototype(const VisualizationPrototype * VisualizationPrototype)
{
	assert(VisualizationPrototype != nullptr);
	
	auto Visualization(new Graphics::ModelNode());
	
	for(auto & PartStyle : VisualizationPrototype->GetPartStyles())
	{
		Visualization->AddStyle(PartStyle.first, new Graphics::Style(PartStyle.second));
	}
	Visualization->SetModel(VisualizationPrototype->GetModel());
	
	return Visualization;
}
