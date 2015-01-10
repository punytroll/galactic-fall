/**
 * galactic-fall
 * Copyright (C) 2015  Hagen Möbius
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

#ifndef GRAPHICS_STYLE_H
#define GRAPHICS_STYLE_H

#include <string>

class Color;

namespace Graphics
{
	class Style
	{
	public:
		// constructor & destructor
		Style(void);
		explicit Style(const Graphics::Style * Style);
		~Style(void);
		// getters
		const Color * GetDiffuseColor(void) const;
		const std::string & GetProgramIdentifier(void) const;
		float GetShininess(void) const;
		const Color * GetSpecularColor(void) const;
		// setters
		void SetDiffuseColor(const Color & DiffuseColor);
		void SetProgramIdentifier(const std::string & ProgramIdentifier);
		void SetShininess(float Shininess);
		void SetSpecularColor(const Color & SpecularColor);
	private:
		Color * _DiffuseColor;
		std::string _ProgramIdentifier;
		float _Shininess;
		Color * _SpecularColor;
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
	
	inline const Color * Style::GetDiffuseColor(void) const
	{
		return _DiffuseColor;
	}
	
	inline const std::string & Style::GetProgramIdentifier(void) const
	{
		return _ProgramIdentifier;
	}
	
	inline float Style::GetShininess(void) const
	{
		return _Shininess;
	}
	
	inline const Color * Style::GetSpecularColor(void) const
	{
		return _SpecularColor;
	}
	
	inline void Style::SetProgramIdentifier(const std::string & ProgramIdentifier)
	{
		_ProgramIdentifier = ProgramIdentifier;
	}
	
	inline void Style::SetShininess(float Shininess)
	{
		_Shininess = Shininess;
	}
}

#endif