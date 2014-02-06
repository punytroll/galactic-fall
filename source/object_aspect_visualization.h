/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#ifndef OBJECT_ASPECT_VISUALIZATION_H
#define OBJECT_ASPECT_VISUALIZATION_H

class Visualization;
class VisualizationPrototype;

namespace Graphics
{
	class Node;
}

class ObjectAspectVisualization
{
public:
	// constructor & destructor
	ObjectAspectVisualization(void);
	~ObjectAspectVisualization(void);
	// getters
	Visualization * GetVisualization(void);
	VisualizationPrototype * GetVisualizationPrototype(void);
	// setters
	/**
	 * @note Passes memory management reponsibility of the VisualizationPrototype to this ObjectAspectVisualization.
	 **/
	void SetVisualizationPrototype(VisualizationPrototype * VisualizationPrototype);
	// modifiers
	void AddGraphics(Graphics::Node * Graphics);
	void Destroy(void);
	void DestroyVisualization(Graphics::Node * Container);
	void RemoveGraphics(Graphics::Node * Graphics);
private:
	Visualization * _Visualization;
	VisualizationPrototype * _VisualizationPrototype;
};

inline VisualizationPrototype * ObjectAspectVisualization::GetVisualizationPrototype(void)
{
	return _VisualizationPrototype;
}

#endif
