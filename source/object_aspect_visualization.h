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

#include "referencing.h"

class VisualizationPrototype;

namespace Graphics
{
	class Node;
}

class ObjectAspectVisualization
{
public:
	// constructor & destructor
	~ObjectAspectVisualization(void);
	// getters
	Reference< Graphics::Node > & GetVisualization(void);
	const VisualizationPrototype * GetVisualizationPrototype(void) const;
	// setters
	void SetVisualization(Reference< Graphics::Node > & Visualization);
	void SetVisualizationPrototype(const VisualizationPrototype * VisualizationPrototype);
	// modifiers
	void Destroy(void);
	void UnsetVisualization(void);
private:
	Reference< Graphics::Node > m_Visualization;
	const VisualizationPrototype * m_VisualizationPrototype;
};

inline Reference< Graphics::Node > & ObjectAspectVisualization::GetVisualization(void)
{
	return m_Visualization;
}

inline const VisualizationPrototype * ObjectAspectVisualization::GetVisualizationPrototype(void) const
{
	return m_VisualizationPrototype;
}

inline void ObjectAspectVisualization::SetVisualizationPrototype(const VisualizationPrototype * VisualizationPrototype)
{
	m_VisualizationPrototype = VisualizationPrototype;
}

#endif
