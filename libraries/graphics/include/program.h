/**
 * galactic-fall
 * Copyright (C) 2015-2019  Hagen Möbius
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

#ifndef GRAPHICS_PROGRAM_H
#define GRAPHICS_PROGRAM_H

#include <cstdint>
#include <list>
#include <map>
#include <string>

#include <GL/gl.h>

class Matrix3f;
class Matrix4f;
class Vector3f;
class Vector4f;

namespace Graphics
{
	class ColorRGB;
	class ColorRGBO;
	class RenderContext;
	class ShadingManager;
	
	class Program
	{
		friend Graphics::RenderContext;
	public:
		enum class UniformContent : std::uint32_t
		{
			ColorRGBO4F,
			ViewToMonitorMatrix4x4F,
			WorldToViewMatrix4x4F,
			ModelToWorldMatrix4x4F,
			WorldToMonitorMatrix4x4F,
			ModelToMonitorMatrix4x4F,
			ModelToWorldNormalMatrix3x3F,
			CameraPositionVector3F,
			LightDirectionVector3F,
			LightColorRGB3F,
			MaterialColorRGB3F,
			MaterialColorRGBO4F,
			MaterialShininessF,
			Texture,
			WorldClippingBottomF,
			WorldClippingLeftF,
			WorldClippingRightF,
			WorldClippingTopF
		};
		
		// constructor
		Program(const std::string & Identifier);
		~Program(void);
		// getters
		const std::string & GetIdentifier(void) const;
		const std::map< GLint, Graphics::Program::UniformContent > & GetUniforms(void) const;
		bool IsBuilt(void) const;
		// modifiers
		void AddShaderIdentifier(const std::string & ShaderIdentifier);
		void Build(Graphics::ShadingManager * ShadingManager);
		void Dispose(void);
	private:
		void _Activate(void);
		void _Deactivate(void);
		void _SetUniform(GLint Location, const Graphics::ColorRGB & Color);
		void _SetUniform(GLint Location, const Graphics::ColorRGBO & Color);
		void _SetUniform(GLint Location, float Float);
		void _SetUniform(GLint Location, int Integer);
		void _SetUniform(GLint Location, const Matrix3f & Matrix);
		void _SetUniform(GLint Location, const Matrix4f & Matrix);
		void _SetUniform(GLint Location, const Vector3f & Vector);
		void _SetUniform(GLint Location, const Vector4f & Vector);
		GLuint _Handle;
		std::string _Identifier;
		std::list< std::string > _ShaderIdentifiers;
		std::map< GLint, Graphics::Program::UniformContent > _Uniforms;
	};
	
	inline const std::string & Graphics::Program::GetIdentifier(void) const
	{
		return _Identifier;
	}
	
	inline const std::map< GLint, Graphics::Program::UniformContent > & Program::GetUniforms(void) const
	{
		return _Uniforms;
	}
	
	inline bool Graphics::Program::IsBuilt(void) const
	{
		return _Handle != 0;
	}
}

#endif
