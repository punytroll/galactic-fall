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

#ifndef OBJECT_ASPECT_POSITION_H
#define OBJECT_ASPECT_POSITION_H

#include <algebra/quaternion.h>
#include <algebra/vector3f.h>

class ObjectAspectPosition
{
public:
	ObjectAspectPosition(void);
	// getters
	const Quaternion & GetOrientation(void) const;
	const Vector3f & GetPosition(void) const;
	// setters
	void SetOrientation(const Quaternion & Orientation);
	void SetPosition(const Vector3f & Position);
	// modifiers
	void ModifyOrientation(const Quaternion & DeltaOrientation);
	void ModifyPosition(const Vector3f & DeltaPosition);
private:
	Quaternion _Orientation;
	Vector3f _Position;
};

inline const Quaternion & ObjectAspectPosition::GetOrientation(void) const
{
	return _Orientation;
}

inline const Vector3f & ObjectAspectPosition::GetPosition(void) const
{
	return _Position;
}

inline void ObjectAspectPosition::ModifyOrientation(const Quaternion & DeltaOrientation)
{
	_Orientation.Rotate(DeltaOrientation);
}

inline void ObjectAspectPosition::ModifyPosition(const Vector3f & DeltaPosition)
{
	_Position.Translate(DeltaPosition);
}

inline void ObjectAspectPosition::SetOrientation(const Quaternion & Orientation)
{
	_Orientation = Orientation;
}

inline void ObjectAspectPosition::SetPosition(const Vector3f & Position)
{
	_Position = Position;
}

#endif
