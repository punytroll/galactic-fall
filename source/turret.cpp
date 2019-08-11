/**
 * galactic-fall
 * Copyright (C) 2007-2019  Hagen Möbius
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

#include <assert.h>

#include <sstream>

#include <algebra/quaternion.h>

#include <graphics/mesh.h>
#include <graphics/mesh_node.h>
#include <graphics/model.h>
#include <graphics/node.h>

#include <string_cast/string_cast.h>

#include "battery.h"
#include "game_time.h"
#include "globals.h"
#include "math.h"
#include "object_aspect_accessory.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "object_factory.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "visualization.h"
#include "visualization_prototype.h"
#include "turret.h"

Turret::Turret(void) :
	_Fire{false},
	_GunPropertiesValid{false},
	_MuzzlePosition{Vector3f::CreateZero()},
	_ShotDamage{0.0f},
	_ShotExitSpeed{0.0f},
	_ShotLifeTime{0.0f},
	_ShotVisualizationPrototype{nullptr},
	_TurretAngle{0.0f}
{
	// initialize object aspects
	AddAspectAccessory();
	AddAspectName();
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(std::bind(&Turret::_Update, this, std::placeholders::_1));
	AddAspectVisualization();
	GetAspectVisualization()->SetUpdateVisualizationCallback(std::bind(&Turret::_UpdateVisualization, this, std::placeholders::_1));
}

Turret::~Turret(void)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = nullptr;
}

float Turret::GetMaximumEnergyInput(float Seconds) const
{
	if(_Fire == true)
	{
		return Physics::Energy::Device::GetMaximumEnergyInput(Seconds);
	}
	else
	{
		return 0.0f;
	}
}

float Turret::GetMaximumEnergyOutput(float Seconds) const
{
	if(_Fire == false)
	{
		return Physics::Energy::Device::GetMaximumEnergyOutput(Seconds);
	}
	else
	{
		return 0.0f;
	}
}

void Turret::_CalculateMuzzleProperties(Vector3f & MuzzlePosition, Quaternion & MuzzleOrientation, Vector3f & MuzzleDirection)
{
	assert(_GunPropertiesValid == true);
	assert(GetAspectPosition() != nullptr);
	assert(GetAspectAccessory() != nullptr);
	assert(GetAspectAccessory()->GetSlot() != nullptr);
	assert(GetContainer() != nullptr);
	assert(GetContainer()->GetAspectPosition() != nullptr);
	MuzzlePosition = _MuzzlePosition;
	MuzzlePosition.Rotate(Quaternion::CreateAsRotationZ(_TurretAngle));
	MuzzlePosition.Rotate(_GunOrientation);
	MuzzlePosition.Translate(_GunPosition);
	MuzzlePosition.Rotate(GetAspectPosition()->GetOrientation());
	MuzzlePosition.Rotate(GetAspectAccessory()->GetSlot()->GetOrientation());
	MuzzlePosition.Translate(GetAspectAccessory()->GetSlot()->GetPosition());
	MuzzlePosition.Rotate(GetContainer()->GetAspectPosition()->GetOrientation());
	MuzzlePosition.Translate(GetContainer()->GetAspectPosition()->GetPosition());
	MuzzleOrientation = GetContainer()->GetAspectPosition()->GetOrientation();
	MuzzleOrientation.Rotate(GetAspectAccessory()->GetSlot()->GetOrientation());
	MuzzleOrientation.Rotate(GetAspectPosition()->GetOrientation());
	MuzzleOrientation.Rotate(_GunOrientation);
	MuzzleOrientation.RotateZ(_TurretAngle);
	MuzzleDirection = Vector3f::CreateTranslationX(1.0f);
	MuzzleDirection.Rotate(MuzzleOrientation);
}

void Turret::SetShotVisualizationPrototype(const VisualizationPrototype & ShotVisualizationPrototype)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = new VisualizationPrototype(ShotVisualizationPrototype);
}

bool Turret::_Update(float Seconds)
{
	if(_GunPropertiesValid == false)
	{
		assert(GetAspectVisualization() != nullptr);
		assert(GetAspectVisualization()->GetVisualizationPrototype() != nullptr);
		assert(GetAspectVisualization()->GetVisualizationPrototype()->GetModel() != nullptr);
		for(auto & Part : GetAspectVisualization()->GetVisualizationPrototype()->GetModel()->GetParts())
		{
			if(Part.second->Identifier == "gun")
			{
				_GunOrientation = Part.second->Orientation;
				_GunPosition = Part.second->Position;
				
				break;
			}
		}
		_GunPropertiesValid = true;
	}
	assert(GetAspectAccessory() != nullptr);
	if(GetAspectAccessory()->GetSlot() != nullptr)
	{
		auto Container(GetContainer());
		
		assert(Container != nullptr);
		assert(Container->GetTypeIdentifier() == "ship");
		
		auto TheShip(dynamic_cast< Ship * >(Container));
		
		assert(TheShip != nullptr);
		
		Vector3f MuzzlePosition;
		Quaternion MuzzleOrientation;
		Vector3f MuzzleDirection;
		auto AreMuzzlePropertiesValid(false);
		
		if(TheShip->GetTarget() != nullptr)
		{
			if(AreMuzzlePropertiesValid == false)
			{
				_CalculateMuzzleProperties(MuzzlePosition, MuzzleOrientation, MuzzleDirection);
				AreMuzzlePropertiesValid = true;
			}
			assert(TheShip->GetTarget()->GetAspectPosition() != nullptr);
			
			auto Dot(MuzzleDirection.Cross(TheShip->GetTarget()->GetAspectPosition()->GetPosition() - MuzzlePosition).Dot(Vector3f::CreateTranslationZ(1.0f)));
			
			if(Dot > 0.01f)
			{
				_TurretAngle += Seconds;
				if(_TurretAngle > 2.0f * M_PI)
				{
					_TurretAngle -= 2.0f * M_PI;
				}
			}
			else if(Dot < -0.01f)
			{
				_TurretAngle -= Seconds;
				if(_TurretAngle < 0.0f)
				{
					_TurretAngle += 2.0f * M_PI;
				}
			}
		}
		if((_Fire == true) && (GetEnergy() >= GetEnergyCapacity()))
		{
			SetEnergy(0.0f);
			assert(Container->GetAspectPosition() != nullptr);
			assert(Container->GetContainer() != nullptr);
			
			auto NewShot(dynamic_cast< Shot * >(g_ObjectFactory->Create("shot", "", true)));
			
			assert(NewShot != nullptr);
			// set up type specific things
			NewShot->SetDamage(_ShotDamage);
			NewShot->SetTimeOfDeath(GameTime::Get() + _ShotLifeTime);
			// set up physical aspect
			assert(NewShot->GetAspectPhysical() != nullptr);
			assert(_ShotVisualizationPrototype != nullptr);
			NewShot->GetAspectPhysical()->SetRadialSize(_ShotVisualizationPrototype->GetModel()->GetRadialSize());
			// set up visualization aspect
			assert(NewShot->GetAspectVisualization() != nullptr);
			NewShot->GetAspectVisualization()->SetVisualizationPrototype(_ShotVisualizationPrototype);
			NewShot->SetShooter(Container);
			
			if(AreMuzzlePropertiesValid == false)
			{
				_CalculateMuzzleProperties(MuzzlePosition, MuzzleOrientation, MuzzleDirection);
				AreMuzzlePropertiesValid = true;
			}
			assert(NewShot->GetAspectPosition() != nullptr);
			NewShot->GetAspectPosition()->SetPosition(MuzzlePosition);
			NewShot->GetAspectPosition()->SetOrientation(MuzzleOrientation);
			
			auto ShotVelocity(MuzzleDirection * _ShotExitSpeed);
			
			NewShot->SetVelocity(TheShip->GetVelocity() + Vector3f::CreateFromComponents(ShotVelocity[0], ShotVelocity[1], 0.0f));
			Container->GetContainer()->GetAspectObjectContainer()->AddContent(NewShot);
		}
	}
	
	return true;
}

void Turret::_UpdateVisualization(Visualization * Visualization)
{
	assert(GetAspectAccessory() != nullptr);
	if(GetAspectAccessory()->GetSlot() != nullptr)
	{
		assert(Visualization != nullptr);
		assert(Visualization->GetGraphics() != nullptr);
		assert(GetAspectPosition() != nullptr);
		if(Visualization->GetUpdateOrientation() == true)
		{
			Visualization->GetGraphics()->SetOrientation(GetAspectAccessory()->GetSlot()->GetOrientation() * GetAspectPosition()->GetOrientation());
		}
		if(Visualization->GetUpdatePosition() == true)
		{
			Visualization->GetGraphics()->SetPosition(GetAspectAccessory()->GetSlot()->GetPosition() + GetAspectPosition()->GetPosition());
		}
		for(auto Node : Visualization->GetGraphics()->GetContent())
		{
			auto MeshNode(dynamic_cast< Graphics::MeshNode * >(Node));
			
			assert(MeshNode != nullptr);
			assert(MeshNode->GetMesh() != nullptr);
			if(MeshNode->GetMesh()->GetIdentifier() == "light_laser_turret_gun")
			{
				MeshNode->SetOrientation(Quaternion::CreateAsRotationZ(_TurretAngle));
			}
		}
	}
}
