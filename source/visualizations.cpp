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
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "visualizations.h"
#include "weapon.h"
#include "weapon_class.h"

void UnvisualizeObject(Object * Object)
{
	assert(Object != 0);
	assert(Object->GetVisualization() != 0);
	
	Object->UnsetVisualization();
	Object->GetVisualization()->Remove();
	Object->GetVisualization()->Destroy();
}

void VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container)
{
	assert(Commodity != 0);
	assert(Commodity->GetVisualization() == 0);
	assert(Container != 0);
	
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	Graphics::Material * Material(new Graphics::Material());
	
	Material->SetDiffuseColor(*(Commodity->GetCommodityClass()->GetColor()));
	Visualization->AddMaterial(Commodity->GetCommodityClass()->GetModel()->GetIdentifier(), Material);
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetModel(Commodity->GetCommodityClass()->GetModel());
	Visualization->SetOrientation(Commodity->GetAngularPosition());
	Visualization->SetPosition(Commodity->GetPosition());
	Visualization->SetUseLighting(true);
	// set as the object's visualization
	Commodity->SetVisualization(Visualization);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizePlanet(Planet * Planet, Graphics::Node * Container)
{
	assert(Planet != 0);
	assert(Planet->GetVisualization() == 0);
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
	Visualization->SetPosition(Planet->GetPosition());
	Visualization->SetScale(Planet->GetRadialSize());
	Visualization->SetUseLighting(true);
	// set as the object's visualization
	Planet->SetVisualization(Visualization);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizeShip(Ship * Ship, Graphics::Node * Container)
{
	assert(Ship != 0);
	assert(Ship->GetVisualization() == 0);
	assert(Ship->GetEngineGlowParticleSystem() == false);
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
	Visualization->SetOrientation(Ship->GetAngularPosition());
	Visualization->SetPosition(Ship->GetPosition());
	Visualization->SetUseLighting(true);
	// set as the object's visualization
	Ship->SetVisualization(Visualization);
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
	Graphics::ParticleSystem * EngineGlowParticleSystem(new Graphics::ParticleSystem());
	
	EngineGlowParticleSystem->AddSystemScriptLine("update-particles");
	EngineGlowParticleSystem->AddParticleScriptLine("kill-old");
	EngineGlowParticleSystem->AddParticleScriptLine("move");
	EngineGlowParticleSystem->SetPosition(Vector3f(Ship->GetShipClass()->GetExhaustOffset()));
	
	Reference< Graphics::ParticleSystem > EngineGlowParticleSystemReference(*EngineGlowParticleSystem);
	
	Ship->SetEngineGlowParticleSystem(EngineGlowParticleSystemReference);
	Visualization->AddNode(EngineGlowParticleSystem);
}

void VisualizeShot(Shot * Shot, Graphics::Node * Container)
{
	assert(Shot != 0);
	assert(Shot->GetVisualization() == 0);
	assert(Container != 0);
	
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	Graphics::Material * Material(new Graphics::Material());
	
	Material->SetDiffuseColor(*(Shot->GetWeaponClass()->GetParticleColor()));
	Visualization->AddMaterial(Shot->GetWeaponClass()->GetParticleModel()->GetIdentifier(), Material);
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetModel(Shot->GetWeaponClass()->GetParticleModel());
	Visualization->SetOrientation(Shot->GetAngularPosition());
	Visualization->SetPosition(Shot->GetPosition());
	Visualization->SetUseBlending(true);
	Visualization->SetUseLighting(false);
	Shot->SetVisualization(Visualization);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container)
{
	assert(Weapon != 0);
	assert(Weapon->GetVisualization() == 0);
	assert(Container != 0);
	
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	Graphics::Material * Material(new Graphics::Material());
	
	Material->SetDiffuseColor(*(Weapon->GetWeaponClass()->GetModelColor()));
	Visualization->AddMaterial(Weapon->GetWeaponClass()->GetModel()->GetIdentifier(), Material);
	Visualization->SetModel(Weapon->GetWeaponClass()->GetModel());
	Visualization->SetOrientation(Weapon->GetSlot()->GetOrientation() * Weapon->GetOrientation());
	Visualization->SetPosition(Weapon->GetPosition() + Weapon->GetSlot()->GetPosition());
	Visualization->SetUseLighting(true);
	Weapon->SetVisualization(Visualization);
	// add to the scene
	Container->AddNode(Visualization);
}
