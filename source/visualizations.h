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

#ifndef VISUALIZATIONS_H
#define VISUALIZATIONS_H

class Commodity;
class Object;
class Planet;
class Ship;
class Shot;
class Weapon;

namespace Graphics
{
	class Node;
}

void InvalidateVisualizationReference(Graphics::Node * Node);
void UnvisualizeObject(Object * Object);
void VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container);
void VisualizeObject(Object * Object, Graphics::Node * Container);
Reference< Graphics::Node > & VisualizeParticleSystem(Graphics::Node * ParticleSystem, Graphics::Node * Container);
void VisualizePlanet(Planet * Planet, Graphics::Node * Container);
void VisualizeShip(Ship * Ship, Graphics::Node * Container);
void VisualizeShot(Shot * Shot, Graphics::Node * Container);
void VisualizeWeapon(Weapon * Weapon, Graphics::Node * Container);

#endif
