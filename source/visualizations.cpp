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
#include "graphics_material.h"
#include "graphics_model.h"
#include "graphics_model_manager.h"
#include "graphics_model_object.h"
#include "graphics_particle_system.h"
#include "object_aspect_accessory.h"
#include "object_aspect_position.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
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
	
	Reference< Graphics::Node > VisualizationReference(Object->GetVisualization());
	
	assert(VisualizationReference.IsValid() == true);
	Object->UnsetVisualization();
	VisualizationReference->Remove();
	VisualizationReference->Destroy();
}

void VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container)
{
	assert(Commodity != 0);
	assert(Commodity->GetVisualization().IsValid() == false);
	assert(Container != 0);
	
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	Graphics::Material * Material(new Graphics::Material());
	
	Material->SetDiffuseColor(*(Commodity->GetCommodityClass()->GetColor()));
	Visualization->AddMaterial(Commodity->GetCommodityClass()->GetModel()->GetIdentifier(), Material);
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetModel(Commodity->GetCommodityClass()->GetModel());
	Visualization->SetOrientation(Commodity->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Commodity->GetAspectPosition()->GetPosition());
	Visualization->SetUseLighting(true);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Commodity->SetVisualization(VisualizationReference);
	// add to the scene
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
	assert(Planet->GetVisualization().IsValid() == false);
	assert(Container != 0);
	
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	Graphics::Material * Material(new Graphics::Material());
	
	Material->SetDiffuseColor(Planet->GetColor());
	Material->SetShininess(10.0f);
	Material->SetSpecularColor(Color(0.25f, 0.25f, 0.25f, 1.0f));
	Visualization->AddMaterial(g_ModelManager->Get("planet")->GetIdentifier(), Material);
	Visualization->SetModel(g_ModelManager->Get("planet"));
	Visualization->SetNormalize(true);
	Visualization->SetOrientation(Quaternion(true));
	Visualization->SetPosition(Planet->GetAspectPosition()->GetPosition());
	Visualization->SetScale(Planet->GetRadialSize());
	Visualization->SetUseLighting(true);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Planet->SetVisualization(VisualizationReference);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizeShip(Ship * Ship, Graphics::Node * Container)
{
	assert(Ship != 0);
	assert(Ship->GetVisualization().IsValid() == false);
	assert(Ship->GetEngineGlowParticleSystem().IsValid() == false);
	assert(Container != 0);
	
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	const std::map< std::string, Graphics::Material * > & PartMaterials(Ship->GetShipClass()->GetPartMaterials());
	
	for(std::map< std::string, Graphics::Material * >::const_iterator PartMaterialIterator = PartMaterials.begin(); PartMaterialIterator != PartMaterials.end(); ++PartMaterialIterator)
	{
		Graphics::Material * Material(new Graphics::Material(*(PartMaterialIterator->second)));
		
		Visualization->AddMaterial(PartMaterialIterator->first, Material);
	}
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetModel(Ship->GetShipClass()->GetModel());
	Visualization->SetOrientation(Ship->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Ship->GetAspectPosition()->GetPosition());
	Visualization->SetUseLighting(true);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Ship->SetVisualization(VisualizationReference);
	// add to the scene
	Container->AddNode(Visualization);
	
	// add weapon visualizations
	const std::map< std::string, Slot * > & Slots(Ship->GetSlots());
	
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
	
	EngineGlowParticleSystem->SetPosition(Vector3f(Ship->GetShipClass()->GetExhaustOffset()));
	Ship->SetEngineGlowParticleSystem(EngineGlowParticleSystemReference);
}

void VisualizeShot(Shot * Shot, Graphics::Node * Container)
{
	assert(Shot != 0);
	assert(Shot->GetVisualization().IsValid() == false);
	assert(Container != 0);
	
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	Graphics::Material * Material(new Graphics::Material());
	
	Material->SetDiffuseColor(*(Shot->GetWeaponClass()->GetParticleColor()));
	Visualization->AddMaterial(Shot->GetWeaponClass()->GetParticleModel()->GetIdentifier(), Material);
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetModel(Shot->GetWeaponClass()->GetParticleModel());
	Visualization->SetOrientation(Shot->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Shot->GetAspectPosition()->GetPosition());
	Visualization->SetUseBlending(true);
	Visualization->SetUseLighting(false);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Shot->SetVisualization(VisualizationReference);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container)
{
	assert(Weapon != 0);
	assert(Weapon->GetVisualization().IsValid() == false);
	assert(Container != 0);
	
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	Graphics::Material * Material(new Graphics::Material());
	
	Material->SetDiffuseColor(*(Weapon->GetWeaponClass()->GetModelColor()));
	Visualization->AddMaterial(Weapon->GetWeaponClass()->GetModel()->GetIdentifier(), Material);
	Visualization->SetModel(Weapon->GetWeaponClass()->GetModel());
	Visualization->SetOrientation(Weapon->GetAspectAccessory()->GetSlot()->GetOrientation() * Weapon->GetAspectPosition()->GetOrientation());
	Visualization->SetPosition(Weapon->GetAspectPosition()->GetPosition() + Weapon->GetAspectAccessory()->GetSlot()->GetPosition());
	Visualization->SetUseLighting(true);
	
	// create the Reference
	Reference< Graphics::Node > VisualizationReference(*Visualization);
	
	// add it to the global container
	g_VisualizationReferences[Visualization] = VisualizationReference;
	// set as the object's visualization
	Weapon->SetVisualization(VisualizationReference);
	// add to the scene
	Container->AddNode(Visualization);
}
