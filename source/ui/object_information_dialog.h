/**
 * galactic-fall
 * Copyright (C) 2008-2018  Hagen Möbius
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

#ifndef UI_OBJECT_INFORMATION_DIALOG_H
#define UI_OBJECT_INFORMATION_DIALOG_H

#include "window.h"

class Object;
class Quaternion;
class Vector3f;

namespace UI
{
	class ScrollBox;
	
	class ObjectInformationDialog : public UI::Window
	{
	public:
		// constructor
		ObjectInformationDialog(Object * Object);
	private:
		// callbacks
		void _OnCloseClicked(void);
		void _OnDestroying(UI::Event & DestroyingEvent);
		void _OnRefreshClicked(void);
		void _OnObjectDestroying(void);
		// helper functions and actions
		float _AddObjectProperty(float Top, float Indentation, Object *	Object);
		float _AddSeparator(float Top, float Indentation, const std::string & Separator);
		float _AddString(float Top, float Indentation, const std::string & String);
		float _AddStringProperty(float Top, float Indentation, const std::string & PropertyName, const std::string & PropertyValue);
		std::string _GetPositionString(const Vector3f & Position);
		std::string _GetOrientationString(const Quaternion & Orientation);
		void _Refresh(void);
		// member variables
		Object * _Object;
		Connection _ObjectDestroyingConnection;
		UI::ScrollBox * _PropertiesScrollBox;
	};
}

#endif
