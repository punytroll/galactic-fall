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

#include <assert.h>

#include <sstream>

#include <algebra/quaternion.h>

#include <string_cast/string_cast.h>

#include "battery.h"
#include "game_time.h"
#include "globals.h"
#include "graphics/model.h"
#include "graphics/node.h"
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
#include "weapon.h"

Weapon::Weapon(void) :
	_EnergyUsagePerShot(0.0f),
	_Fire(false),
	_NextTimeToFire(0.0),
	_ReloadTime(0.0f),
	_ShotDamage(0.0f),
	_ShotExitPosition(Vector3f::CreateZero()),
	_ShotExitSpeed(0.0f),
	_ShotLifeTime(0.0f),
	_ShotVisualizationPrototype(nullptr)
{
	// initialize object aspects
	AddAspectAccessory();
	AddAspectName();
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(std::bind(&Weapon::_Update, this, std::placeholders::_1));
	AddAspectVisualization();
	GetAspectVisualization()->SetUpdateVisualizationCallback(std::bind(&Weapon::_UpdateVisualization, this, std::placeholders::_1));
}

Weapon::~Weapon(void)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = nullptr;
}

void Weapon::SetShotVisualizationPrototype(const VisualizationPrototype * ShotVisualizationPrototype)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = new VisualizationPrototype(ShotVisualizationPrototype);
}

bool Weapon::_Update(float Seconds)
{
	if((_Fire == true) && (_NextTimeToFire <= GameTime::Get()))
	{
		auto Container(GetContainer());
		
		assert(Container != nullptr);
		assert(Container->GetTypeIdentifier() == "ship");
		
		auto TheShip(dynamic_cast< Ship * >(Container));
		
		assert(TheShip != nullptr);
		if((TheShip->GetBattery() != nullptr) && (TheShip->GetBattery()->GetEnergy() >= _EnergyUsagePerShot))
		{
			TheShip->GetBattery()->SetEnergy(TheShip->GetBattery()->GetEnergy() - _EnergyUsagePerShot);
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
			assert(NewShot->GetAspectPosition() != nullptr);
			NewShot->SetShooter(Container->GetReference());
			
			// calculating the shot's position in the world coordinate system
			Vector3f ShotPosition(_ShotExitPosition);
			
			ShotPosition.Rotate(GetAspectPosition()->GetOrientation());
			ShotPosition.Rotate(GetAspectAccessory()->GetSlot()->GetOrientation());
			ShotPosition.Translate(GetAspectAccessory()->GetSlot()->GetPosition());
			ShotPosition.Rotate(Container->GetAspectPosition()->GetOrientation());
			ShotPosition.Translate(Container->GetAspectPosition()->GetPosition());
			NewShot->GetAspectPosition()->SetPosition(ShotPosition);
			
			// calculating the shot's angular position in world coordinate system
			Quaternion ShotOrientation(Container->GetAspectPosition()->GetOrientation());
			
			ShotOrientation.Rotate(GetAspectAccessory()->GetSlot()->GetOrientation());
			ShotOrientation.Rotate(GetAspectPosition()->GetOrientation());
			NewShot->GetAspectPosition()->SetOrientation(ShotOrientation);
			
			// calculate the shot's velocity
			auto ShotVelocity(Vector3f::CreateTranslationX(_ShotExitSpeed));
			
			ShotVelocity.Rotate(ShotOrientation);
			NewShot->SetVelocity(TheShip->GetVelocity() + Vector3f::CreateFromComponents(ShotVelocity[0], ShotVelocity[1], 0.0f));
			Container->GetContainer()->GetAspectObjectContainer()->AddContent(NewShot);
			_NextTimeToFire = GameTime::Get() + _ReloadTime;
		}
	}
	
	return true;
}

void Weapon::_UpdateVisualization(Visualization * Visualization)
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
	}
}
