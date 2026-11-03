/**
 * galactic-fall
 * Copyright (C) 2007-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>
#include <sstream>

#include <algebra/quaternion.h>

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
#include "projectile_weapon.h"
#include "ship.h"
#include "shot.h"
#include "slot.h"
#include "visualization.h"
#include "visualization_prototype.h"

ProjectileWeapon::ProjectileWeapon()
{
    // initialize object aspects
    GetAspectUpdate()->SetCallback(std::bind(&ProjectileWeapon::m_Update, this, std::placeholders::_1));
    GetAspectVisualization()->SetUpdateVisualizationCallback(std::bind(&ProjectileWeapon::m_UpdateVisualization, this, std::placeholders::_1));
}

auto ProjectileWeapon::m_Update([[maybe_unused]] float Seconds) -> bool
{
    if((_Fire == true) && (GetEnergy() >= GetEnergyCapacity()))
    {
        SetEnergy(0.0f);
        
        auto Container = GetContainer();
        
        assert(Container != nullptr);
        assert(Container->GetAspectPosition() != nullptr);
        assert(Container->GetContainer() != nullptr);
        
        auto NewShot = dynamic_cast<Shot *>(g_ObjectFactory->Create("shot", "", true));
        
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
        NewShot->SetShooter(Container);
        
        // calculating the shot's position in the world coordinate system
        auto ShotPosition = _MuzzlePosition;
        
        ShotPosition.Rotate(GetAspectPosition()->GetOrientation());
        ShotPosition.Rotate(GetAspectAccessory()->GetSlot()->GetOrientation());
        ShotPosition.Translate(GetAspectAccessory()->GetSlot()->GetPosition());
        ShotPosition.Rotate(Container->GetAspectPosition()->GetOrientation());
        ShotPosition.Translate(Container->GetAspectPosition()->GetPosition());
        NewShot->GetAspectPosition()->SetPosition(ShotPosition);
        
        // calculating the shot's angular position in world coordinate system
        auto ShotOrientation = Container->GetAspectPosition()->GetOrientation();
        
        ShotOrientation.Rotate(GetAspectAccessory()->GetSlot()->GetOrientation());
        ShotOrientation.Rotate(GetAspectPosition()->GetOrientation());
        NewShot->GetAspectPosition()->SetOrientation(ShotOrientation);
        assert(Container->GetTypeIdentifier() == "ship");
        
        auto TheShip = dynamic_cast<Ship *>(Container);
        
        assert(TheShip != nullptr);
        
        // calculate the shot's velocity
        auto ShotVelocity = Vector3f::CreateTranslationX(_ShotExitSpeed);
        
        ShotVelocity.Rotate(ShotOrientation);
        NewShot->SetVelocity(TheShip->GetVelocity() + Vector3f::CreateFromComponents(ShotVelocity[0], ShotVelocity[1], 0.0f));
        Container->GetContainer()->GetAspectObjectContainer()->AddContent(NewShot);
    }
    
    return true;
}

auto ProjectileWeapon::m_UpdateVisualization(Visualization * Visualization) -> void
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
