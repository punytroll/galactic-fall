/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#ifndef COMMAND_MIND_H
#define COMMAND_MIND_H

#include "mind.h"

class PhysicalObject;
class System;

/** The name CommandMind results from the fact that this mind can be controlled via commands. **/
class CommandMind : public Mind
{
public:
	virtual void HandleMessage(Message * Message);
	virtual void Update(void);
	
	// commands
	void EnableAccelerate(void);
	void DisableAccelerate(void);
	void EnableTurnLeft(void);
	void DisableTurnLeft(void);
	void EnableTurnRight(void);
	void DisableTurnRight(void);
	void EnableFire(void);
	void DisableFire(void);
	void Jettison(void);
	void Jump(void);
	void Land(void);
	void Refuel(void);
	void Scoop(void);
	void SelectLinkedSystem(System * LinkedSystem);
	void SelectNextLinkedSystem(void);
	void TargetPreviousCargo(void);
	void TargetNearestCargo(void);
	void TargetNextCargo(void);
	void TargetPreviousPlanet(void);
	void TargetNearestPlanet(void);
	void TargetNextPlanet(void);
	void TargetPreviousShip(void);
	void TargetNextShip(void);
	void TargetPhysicalObject(Reference< PhysicalObject > & PhysicalObject);
};

#endif
