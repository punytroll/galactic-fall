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

#ifndef VISUALIZATION_PROTOTYPE_H
#define VISUALIZATION_PROTOTYPE_H

#include <map>
#include <string>

namespace Graphics
{
	class Model;
	class Style;
}

class VisualizationPrototype
{
public:
	// constructor & destructor
	VisualizationPrototype(void);
	VisualizationPrototype(const VisualizationPrototype * VisualizationPrototype);
	~VisualizationPrototype(void);
	// getters
	const Graphics::Model * GetModel(void) const;
	std::map< std::string, Graphics::Style * > & GetPartStyles(void);
	const std::map< std::string, Graphics::Style * > & GetPartStyles(void) const;
	// setters
	void SetModel(const Graphics::Model * Model);
	/**
	 * @note Passes memory management responsibility for @a PartMaterial to the visualization prototype.
	 **/
	void SetPartStyle(const std::string & Identifier, Graphics::Style * PartStyle);
private:
	const Graphics::Model * _Model;
	std::map< std::string, Graphics::Style * > _PartStyles;
};

inline const Graphics::Model * VisualizationPrototype::GetModel(void) const
{
	return _Model;
}

inline std::map< std::string, Graphics::Style * > & VisualizationPrototype::GetPartStyles(void)
{
	return _PartStyles;
}

inline const std::map< std::string, Graphics::Style * > & VisualizationPrototype::GetPartStyles(void) const
{
	return _PartStyles;
}

inline void VisualizationPrototype::SetModel(const Graphics::Model * Model)
{
	_Model = Model;
}

#endif
