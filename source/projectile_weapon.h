/**
 * galactic-fall
 * Copyright (C) 2007-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__PROJECTILE_WEAPON_H
#define GALACTIC_FALL__SOURCE__PROJECTILE_WEAPON_H

#include "weapon.h"

class Visualization;

class ProjectileWeapon : public Weapon
{
public:
    ProjectileWeapon();
    
private:
    auto m_Update(float Seconds) -> bool;
    auto m_UpdateVisualization(Visualization * Visualization) -> void;
};

#endif
