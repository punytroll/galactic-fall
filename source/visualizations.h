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

class Object;
class System;
class Visualization;
class VisualizationPrototype;

namespace Graphics
{
	class Node;
	class ParticleSystem;
}

void InvalidateVisualizationReference(Graphics::Node * Node);
Visualization * VisualizeObject(Object * Object, Graphics::Node * Container);
void VisualizeParticleSystem(Graphics::ParticleSystem * ParticleSystem, System * System);
void VisualizeParticleSystem(Graphics::ParticleSystem * ParticleSystem, Graphics::Node * Container);
Graphics::Node * VisualizePrototype(const VisualizationPrototype * VisualizationPrototype);

#endif
