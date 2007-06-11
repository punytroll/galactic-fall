#ifndef COMMAND_MIND_H
#define COMMAND_MIND_H

#include "mind.h"

class PhysicalObject;
class System;

/** The name CommandMind results from the fact that this mind can be controlled via commands. **/
class CommandMind : public Mind
{
public:
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
	void TargetPhysicalObject(PhysicalObject * PhysicalObject);
};

#endif
