/**
 * galactic-fall
 * Copyright (C) 2015-2025  Hagen Möbius
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

#ifndef GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__STYLE_H
#define GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__STYLE_H

#include <string>

namespace Graphics
{
	class ColorRGBO;
	
	class Style
	{
	public:
		// constructor & destructor
		Style(void);
		explicit Style(const Graphics::Style * Style);
		~Style(void);
		// getters
		const Graphics::ColorRGBO & GetDiffuseColor(void) const;
		const std::string & GetProgramIdentifier(void) const;
		float GetShininess(void) const;
		const Graphics::ColorRGBO & GetSpecularColor(void) const;
		bool HasDiffuseColor(void) const;
		bool HasShininess(void) const;
		bool HasSpecularColor(void) const;
		// setters
		void SetDiffuseColor(const Graphics::ColorRGBO & DiffuseColor);
		void SetProgramIdentifier(const std::string & ProgramIdentifier);
		void SetShininess(float Shininess);
		void SetSpecularColor(const Graphics::ColorRGBO & SpecularColor);
	private:
		Graphics::ColorRGBO * _DiffuseColor;
		std::string _ProgramIdentifier;
		float * _Shininess;
		Graphics::ColorRGBO * _SpecularColor;
	private:
		/**
		 * @brief Hidden copy constructor.
		 **/
		Style(const Graphics::Style & Style);
		
		/**
		 * @brief Hidden assignment operator.
		 **/
		Graphics::Style & operator=(const Graphics::Style & Style);
	};
	
	inline const std::string & Style::GetProgramIdentifier(void) const
	{
		return _ProgramIdentifier;
	}
	
	inline bool Style::HasDiffuseColor(void) const
	{
		return _DiffuseColor != nullptr;
	}
	
	inline bool Style::HasShininess(void) const
	{
		return _Shininess != nullptr;
	}
	
	inline bool Style::HasSpecularColor(void) const
	{
		return _SpecularColor != nullptr;
	}
	
	inline void Style::SetProgramIdentifier(const std::string & ProgramIdentifier)
	{
		_ProgramIdentifier = ProgramIdentifier;
	}
}

#endif
