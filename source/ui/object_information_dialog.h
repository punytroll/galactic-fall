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
		bool OnMouseButton(int Button, int State, float X, float Y);
		void OnObjectClicked(const Reference< Object > Object);
		void OnOKClicked(void);
		void OnRefreshClicked(void);
		// helper functions and actions
		float AddObjectProperty(float Top, float Indentation, const Reference< Object > & Object);
		float AddSeparator(float Top, float Indentation, const std::string & Separator);
		float AddString(float Top, float Indentation, const std::string & String);
		float AddStringProperty(float Top, float Indentation, const std::string & PropertyName, const std::string & PropertyValue);
		void Refresh(void);
		// member variables
		Reference< Object > m_Object;
		UI::Button * m_OKButton;
		UI::ScrollBox * m_PropertiesScrollBox;
		UI::Button * m_RefreshButton;
	};
}

#endif
