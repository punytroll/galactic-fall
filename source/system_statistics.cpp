/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#include "system_statistics.h"

void SystemStatistics::NextFrame(void)
{
	_FontSecondsLastFrame = _FontSecondsThisFrame;
	_FontSecondsThisFrame = 0.0f;
	_ParticleSystemsDrawnThisFrame = 0;
	_ParticleSystemsUpdatedThisFrame = 0;
	_ParticlesDrawnThisFrame = 0;
	_ParticlesUpdatedThisFrame = 0;
	_ShipsInCurrentSystemThisFrame = 0;
	_CommoditiesInCurrentSystemThisFrame = 0;
	_ShotsInCurrentSystemThisFrame = 0;
}
