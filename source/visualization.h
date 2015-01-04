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

#ifndef VISUALIZATION_H
#define VISUALIZATION_H

namespace Graphics
{
	class Node;
}

class Visualization
{
public:
	// constructor & destructor
	Visualization(void);
	~Visualization(void);
	// getters
	Graphics::Node * GetGraphics(void);
	bool GetUpdateOrientation(void);
	bool GetUpdatePosition(void);
	// setters
	void SetGraphics(Graphics::Node * Graphics);
	void SetUpdateOrientation(bool UpdateOrientation);
	void SetUpdatePosition(bool UpdatePosition);
private:
	Graphics::Node * _Graphics;
	bool _UpdateOrientation;
	bool _UpdatePosition;
};

inline Graphics::Node * Visualization::GetGraphics(void)
{
	return _Graphics;
}

inline bool Visualization::GetUpdateOrientation(void)
{
	return _UpdateOrientation;
}

inline bool Visualization::GetUpdatePosition(void)
{
	return _UpdatePosition;
}

inline void Visualization::SetGraphics(Graphics::Node * Graphics)
{
	_Graphics = Graphics;
}

inline void Visualization::SetUpdateOrientation(bool UpdateOrientation)
{
	_UpdateOrientation = UpdateOrientation;
}

inline void Visualization::SetUpdatePosition(bool UpdatePosition)
{
	_UpdatePosition = UpdatePosition;
}

#endif
