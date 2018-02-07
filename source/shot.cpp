/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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

#include "game_time.h"
#include "graphics/node.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "shot.h"
#include "visualization.h"

Shot::Shot(void) :
	_Damage(0.0),
	_Shooter(nullptr),
	_TimeOfDeath(0.0),
	_Velocity(Vector3f::CreateZero())
{
	// initialize object aspects
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(std::bind(&Shot::_Update, this, std::placeholders::_1));
	AddAspectVisualization();
	GetAspectVisualization()->SetUpdateVisualizationCallback(std::bind(&Shot::_UpdateVisualization, this, std::placeholders::_1));
}

Shot::~Shot(void)
{
	if(_Shooter != nullptr)
	{
		assert(_ShooterDestroyingConnection.IsValid() == true);
		_ShooterDestroyingConnection.Disconnect();
		_Shooter = nullptr;
	}
	assert(_ShooterDestroyingConnection.IsValid() == false);
}

void Shot::SetShooter(Object * Shooter)
{
	assert((Shooter != nullptr) && (_Shooter == nullptr) && (_ShooterDestroyingConnection.IsValid() == false));
	_Shooter = Shooter;
	_ShooterDestroyingConnection = _Shooter->ConnectDestroyingCallback(std::bind(&Shot::_OnShooterDestroying, this));
	assert(_ShooterDestroyingConnection.IsValid() == true);
}

void Shot::_OnShooterDestroying(void)
{
	assert(_Shooter != nullptr);
	assert(_ShooterDestroyingConnection.IsValid() == true);
	_ShooterDestroyingConnection.Disconnect();
	assert(_ShooterDestroyingConnection.IsValid() == false);
	_Shooter = nullptr;
}

bool Shot::_Update(float Seconds)
{
	if(_TimeOfDeath >= GameTime::Get())
	{
		assert(GetAspectPosition() != nullptr);
		GetAspectPosition()->ModifyPosition(_Velocity * Seconds);
		
		return true;
	}
	else
	{
		return false;
	}
}

void Shot::_UpdateVisualization(Visualization * Visualization)
{
	assert(Visualization != nullptr);
	assert(Visualization->GetGraphics() != nullptr);
	assert(GetAspectPosition() != nullptr);
	if(Visualization->GetUpdateOrientation() == true)
	{
		Visualization->GetGraphics()->SetOrientation(GetAspectPosition()->GetOrientation());
	}
	if(Visualization->GetUpdatePosition() == true)
	{
		Visualization->GetGraphics()->SetPosition(GetAspectPosition()->GetPosition());
	}
}
