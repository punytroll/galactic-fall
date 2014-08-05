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

#ifndef UI_OBJECT_INFORMATION_DIALOG_H
#define UI_OBJECT_INFORMATION_DIALOG_H

#include "../referencing.h"
#include "window.h"

class Object;
class Quaternion;
class Vector3f;

namespace UI
{
	class Button;
	class ScrollBox;
	
	class ObjectInformationDialog : public UI::Window
	{
	public:
		ObjectInformationDialog(UI::Widget * SupWidget, const Reference< Object > & Object);
	private:
		// callbacks
		void _OnObjectClicked(const Reference< Object > Object);
		void _OnCloseClicked(void);
		void _OnRefreshClicked(void);
		// helper functions and actions
		float _AddObjectProperty(float Top, float Indentation, const Reference< Object > & Object);
		float _AddSeparator(float Top, float Indentation, const std::string & Separator);
		float _AddString(float Top, float Indentation, const std::string & String);
		float _AddStringProperty(float Top, float Indentation, const std::string & PropertyName, const std::string & PropertyValue);
		std::string _GetPositionString(const Vector3f & Position);
		std::string _GetOrientationString(const Quaternion & Orientation);
		void _Refresh(void);
		// member variables
		Reference< Object > _Object;
		UI::Button * _CloseButton;
		UI::ScrollBox * _PropertiesScrollBox;
		UI::Button * _RefreshButton;
	};
}

#endif
