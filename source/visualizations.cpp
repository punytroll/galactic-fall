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
	
	Reference< Graphics::Node > VisualizationReference(Object->GetAspectVisualization()->GetVisualization());
	
	assert(VisualizationReference.IsValid() == true);
	Object->GetAspectVisualization()->UnsetVisualization();
	VisualizationReference->Remove();
	VisualizationReference->Destroy();
}

void VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container)
{
	assert(Commodity != 0);
	assert(Commodity->GetAspectVisualization() != 0);
	assert(Commodity->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	assert(Commodity->GetAspectVisualization()->GetVisualization().IsValid() == false);
	assert(Container != 0);
	
	Graphics::ModelNode * Visualization(new Graphics::ModelNode());
	const std::map< std::string, Graphics::Material * > & PartMaterials(Commodity->GetAspectVisualization()->GetVisualizationPrototype()->GetPartMaterials());
	
	for(std::map< std::string, Graphics::Material * >::const_iterator PartMaterialIterator = PartMaterials.begin(); PartMaterialIterator != PartMaterials.end(); ++PartMaterialIterator)
	{
		Visualization->AddMaterial(PartMaterialIterator->first, new Graphics::Material(PartMaterialIterator->second));
	}
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetModel(Commodity->GetAspectVisualization()->GetVisualizationPrototype()->GetModel());
	Visualization->SetOrientation(Commodity->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Commodity->GetAspectPosition()->GetPosition());
	Visualization->SetUseLighting(true);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Commodity->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizeObject(Object * Object, Graphics::Node * Container)
{
	if(Object->GetTypeIdentifier() == "weapon")
	{
		VisualizeWeapon(static_cast< Weapon * >(Object), Container);
	}
	else
	{
		assert(false);
	}
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
	assert(Planet->GetAspectVisualization()->GetVisualization().IsValid() == false);
	assert(Planet->GetAspectPhysical() != 0);
	assert(Planet->GetAspectPosition() != 0);
	assert(Container != 0);
	
	Graphics::ModelNode * Visualization(new Graphics::ModelNode());
	Graphics::Material * Material(new Graphics::Material());
	
	Material->SetDiffuseColor(new Color(Planet->GetColor()));
	Material->SetShininess(10.0f);
	Material->SetSpecularColor(new Color(0.25f, 0.25f, 0.25f, 1.0f));
	Visualization->AddMaterial(g_GraphicsEngine->GetModelManager()->Get("planet")->GetIdentifier(), Material);
	Visualization->SetModel(g_GraphicsEngine->GetModelManager()->Get("planet"));
	Visualization->SetNormalize(true);
	Visualization->SetOrientation(Quaternion(true));
	Visualization->SetPosition(Planet->GetAspectPosition()->GetPosition());
	Visualization->SetScale(Planet->GetAspectPhysical()->GetRadialSize());
	Visualization->SetUseLighting(true);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Planet->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizeShip(Ship * Ship, Graphics::Node * Container)
{
	assert(Ship != 0);
	assert(Ship->GetAspectVisualization() != 0);
	assert(Ship->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	assert(Ship->GetAspectVisualization()->GetVisualization().IsValid() == false);
	assert(Ship->GetEngineGlowParticleSystem().IsValid() == false);
	assert(Container != 0);
	
	Graphics::ModelNode * Visualization(new Graphics::ModelNode());
	const std::map< std::string, Graphics::Material * > & PartMaterials(Ship->GetAspectVisualization()->GetVisualizationPrototype()->GetPartMaterials());
	
	for(std::map< std::string, Graphics::Material * >::const_iterator PartMaterialIterator = PartMaterials.begin(); PartMaterialIterator != PartMaterials.end(); ++PartMaterialIterator)
	{
		Visualization->AddMaterial(PartMaterialIterator->first, new Graphics::Material(PartMaterialIterator->second));
	}
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetModel(Ship->GetAspectVisualization()->GetVisualizationPrototype()->GetModel());
	Visualization->SetOrientation(Ship->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Ship->GetAspectPosition()->GetPosition());
	Visualization->SetUseLighting(true);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Ship->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the scene
	Container->AddNode(Visualization);
	
	// add weapon visualizations
	const std::map< std::string, Slot * > & Slots(Ship->GetAspectOutfitting()->GetSlots());
	
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = Slots.begin(); SlotIterator != Slots.end(); ++SlotIterator)
	{
		Weapon * MountedWeapon(dynamic_cast< Weapon * >(SlotIterator->second->GetMountedObject().Get()));
		
		if(MountedWeapon != 0)
		{
			VisualizeWeapon(MountedWeapon, Visualization);
		}
	}
	
	// add engine glow particle system
	Graphics::ParticleSystem * EngineGlowParticleSystem(CreateParticleSystem("engine_glow"));
	Reference< Graphics::ParticleSystem > EngineGlowParticleSystemReference(VisualizeParticleSystem(EngineGlowParticleSystem, Visualization));
	
	EngineGlowParticleSystem->SetPosition(Vector3f(Ship->GetExhaustOffset()));
	Ship->SetEngineGlowParticleSystem(EngineGlowParticleSystemReference);
}

void VisualizeShot(Shot * Shot, Graphics::Node * Container)
{
	assert(Shot != 0);
	assert(Shot->GetAspectVisualization() != 0);
	assert(Shot->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	assert(Shot->GetAspectVisualization()->GetVisualization().IsValid() == false);
	assert(Container != 0);
	
	Graphics::ModelNode * Visualization(new Graphics::ModelNode());
	const std::map< std::string, Graphics::Material * > & PartMaterials(Shot->GetAspectVisualization()->GetVisualizationPrototype()->GetPartMaterials());
	
	for(std::map< std::string, Graphics::Material * >::const_iterator PartMaterialIterator = PartMaterials.begin(); PartMaterialIterator != PartMaterials.end(); ++PartMaterialIterator)
	{
		Visualization->AddMaterial(PartMaterialIterator->first, new Graphics::Material(PartMaterialIterator->second));
	}
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetModel(Shot->GetAspectVisualization()->GetVisualizationPrototype()->GetModel());
	Visualization->SetOrientation(Shot->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Shot->GetAspectPosition()->GetPosition());
	Visualization->SetUseBlending(true);
	Visualization->SetUseLighting(false);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Shot->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container)
{
	assert(Weapon != 0);
	assert(Weapon->GetAspectVisualization() != 0);
	assert(Weapon->GetAspectVisualization()->GetVisualizationPrototype() != 0);
	assert(Weapon->GetAspectVisualization()->GetVisualization().IsValid() == false);
	assert(Container != 0);
	
	Graphics::ModelNode * Visualization(new Graphics::ModelNode());
	const std::map< std::string, Graphics::Material * > & PartMaterials(Weapon->GetAspectVisualization()->GetVisualizationPrototype()->GetPartMaterials());
	
	for(std::map< std::string, Graphics::Material * >::const_iterator PartMaterialIterator = PartMaterials.begin(); PartMaterialIterator != PartMaterials.end(); ++PartMaterialIterator)
	{
		Visualization->AddMaterial(PartMaterialIterator->first, new Graphics::Material(PartMaterialIterator->second));
	}
	Visualization->SetModel(Weapon->GetAspectVisualization()->GetVisualizationPrototype()->GetModel());
	Visualization->SetOrientation(Weapon->GetAspectAccessory()->GetSlot()->GetOrientation() * Weapon->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Weapon->GetAspectPosition()->GetPosition() + Weapon->GetAspectAccessory()->GetSlot()->GetPosition());
	Visualization->SetUseLighting(true);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Weapon->GetAspectVisualization()->SetVisualization(VisualizationReference);
	// add to the scene
	Container->AddNode(Visualization);
}
