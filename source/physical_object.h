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

#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include <map>
#include <vector>

#include "position.h"
#include "type_definitions.h"

namespace Graphics
{
	class Node;
}

class PhysicalObject : public Position
{
public:
	virtual ~PhysicalObject(void);
	virtual void Draw(void) const = 0;
	// getters
	const std::string & GetName(void) const;
	float GetRadialSize(void) const;
	unsigned_numeric GetSpaceRequirement(void) const;
	// setters
	void SetName(const std::string & Name);
	void SetRadialSize(float RadialSize);
	void SetSpaceRequirement(unsigned_numeric SpaceRequirement);
	// modifiers
	void AddVisualization(Graphics::Node * Visualization);
	static void RemoveVisualizations(Graphics::Node * Visualization);
	void RemoveVisualization(Graphics::Node * Visualization);
private:
	static std::map< Graphics::Node *, PhysicalObject * > m_VisualizationBackReferences;
	std::string m_Name;
	float m_RadialSize;
	unsigned_numeric m_SpaceRequirement;
	std::vector< Graphics::Node * > m_Visualizations;
};

inline const std::string & PhysicalObject::GetName(void) const
{
	return m_Name;
}

inline float PhysicalObject::GetRadialSize(void) const
{
	return m_RadialSize;
}

inline unsigned_numeric PhysicalObject::GetSpaceRequirement(void) const
{
	return m_SpaceRequirement;
}

inline void PhysicalObject::SetName(const std::string & Name)
{
	m_Name = Name;
}

inline void PhysicalObject::SetRadialSize(float RadialSize)
{
	m_RadialSize = RadialSize;
}

inline void PhysicalObject::SetSpaceRequirement(unsigned_numeric SpaceRequirement)
{
	m_SpaceRequirement = SpaceRequirement;
}

inline void PhysicalObject::AddVisualization(Graphics::Node * Visualization)
{
	m_VisualizationBackReferences[Visualization] = this;
	m_Visualizations.push_back(Visualization);
}

inline void PhysicalObject::RemoveVisualizations(Graphics::Node * Visualization)
{
	std::map< Graphics::Node *, PhysicalObject * >::iterator Iterator(m_VisualizationBackReferences.find(Visualization));
	
	if(Iterator != m_VisualizationBackReferences.end())
	{
		Iterator->second->RemoveVisualization(Visualization);
		m_VisualizationBackReferences.erase(Iterator);
	}
}

inline void PhysicalObject::RemoveVisualization(Graphics::Node * Visualization)
{
	m_Visualizations.erase(std::find(m_Visualizations.begin(), m_Visualizations.end(), Visualization));
}

#endif
