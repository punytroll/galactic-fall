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

#include <algebra/vector2f.h>

#include "color_rgbo.h"
#include "engine.h"
#include "drawing.h"
#include "gl.h"
#include "render_context.h"
#include "style.h"
#include "texture_manager.h"

#include "../globals.h"
#include "../real_time.h"
#include "../system_statistics.h"

const Graphics::Texture * Graphics::Drawing::_FontTexture{nullptr};

void Graphics::Drawing::DrawBoxFromPositionAndSize(Graphics::RenderContext * RenderContext, const Vector2f & Position, const Vector2f & Size)
{
	Graphics::Drawing::DrawBox(RenderContext, Position[0], Position[1], Position[1] + Size[1], Position[0] + Size[0]);
}

void Graphics::Drawing::DrawBox(Graphics::RenderContext * RenderContext, float Left, float Top, float Bottom, float Right)
{
	GLuint VertexArray{0};
	
	GLGenVertexArrays(1, &VertexArray);
	GLBindVertexArray(VertexArray);
	
	const GLsizei NumberOfVertices{4};
	float Vertices[2 * NumberOfVertices];
	
	Vertices[0] = Left;
	Vertices[1] = Top;
	Vertices[2] = Left;
	Vertices[3] = Bottom;
	Vertices[4] = Right;
	Vertices[5] = Bottom;
	Vertices[6] = Right;
	Vertices[7] = Top;
	
	GLuint VertexBuffer{0};
	
	GLGenBuffers(1, &VertexBuffer);
	GLBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	GLBufferData(GL_ARRAY_BUFFER, 2 * NumberOfVertices * sizeof(GLfloat), Vertices, GL_STREAM_DRAW);
	GLVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	GLEnableVertexAttribArray(0);
	GLDrawArrays(GL_TRIANGLE_FAN, 0, NumberOfVertices);
	GLBindVertexArray(0);
	GLDeleteBuffers(1, &VertexBuffer);
	GLDeleteVertexArrays(1, &VertexArray);
}

void Graphics::Drawing::DrawPoints(Graphics::RenderContext * RenderContext, const std::vector< Vector2f > & Positions, const std::vector< Graphics::ColorRGBO > & Colors)
{
	GLuint VertexArray{0};
	
	GLGenVertexArrays(1, &VertexArray);
	GLBindVertexArray(VertexArray);
	
	auto NumberOfVertices{static_cast< GLsizei >(Positions.size())};
	float VertexData[2 * NumberOfVertices];
	float ColorData[4 * NumberOfVertices];
	
	for(auto Index = 0u; Index < Positions.size(); ++Index)
	{
		VertexData[Index * 2 + 0] = Positions[Index][0];
		VertexData[Index * 2 + 1] = Positions[Index][1];
		ColorData[Index * 4 + 0] = Colors[Index].GetRed();
		ColorData[Index * 4 + 1] = Colors[Index].GetGreen();
		ColorData[Index * 4 + 2] = Colors[Index].GetBlue();
		ColorData[Index * 4 + 3] = Colors[Index].GetOpacity();
	}
	
	GLuint Buffers[2];
	
	GLGenBuffers(2, Buffers);
	GLBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	GLBufferData(GL_ARRAY_BUFFER, 2 * NumberOfVertices * sizeof(GLfloat), VertexData, GL_STREAM_DRAW);
	GLVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	GLEnableVertexAttribArray(0);
	GLBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	GLBufferData(GL_ARRAY_BUFFER, 4 * NumberOfVertices * sizeof(GLfloat), ColorData, GL_STREAM_DRAW);
	GLVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	GLEnableVertexAttribArray(1);
	assert(RenderContext != nullptr);
	RenderContext->ActivateProgram();
	GLDrawArrays(GL_POINTS, 0, NumberOfVertices);
	RenderContext->DeactivateProgram();
	GLBindVertexArray(0);
	GLDeleteBuffers(2, Buffers);
	GLDeleteVertexArrays(1, &VertexArray);
}

void Graphics::Drawing::DrawText(Graphics::RenderContext * RenderContext, const Vector2f & Position, const std::string & Text)
{
	Graphics::Drawing::_DrawText(RenderContext, Position[0], Position[1], 12.0f, Text, "font", true);
}

void Graphics::Drawing::DrawText(Graphics::RenderContext * RenderContext, float Left, float Top, const std::string & Text)
{
	Graphics::Drawing::_DrawText(RenderContext, Left, Top, 12.0f, Text, "font", true);
}

void Graphics::Drawing::DrawTextWithoutClippingRightHanded(Graphics::RenderContext * RenderContext, float Left, float Top, float Height, const std::string & Text)
{
	Graphics::Drawing::_DrawText(RenderContext, Left, Top, Height, Text, "font_without_clipping", false);
}

void Graphics::Drawing::_DrawText(Graphics::RenderContext * RenderContext, float Left, float Top, float CharacterHeight, const std::string & Text, const std::string & ProgamName, bool LeftHanded)
{
	if(Text.empty() == false)
	{
		RealTime::Invalidate();
		
		double Begin(RealTime::Get());
		const std::uint32_t CHARACTERS{95};
		const std::uint32_t CHARACTEROFFSET{32};
		GLuint VertexArray{0};
		
		GLGenVertexArrays(1, &VertexArray);
		GLBindVertexArray(VertexArray);
		
		const GLsizei NumberOfVertices{static_cast< GLsizei >(4 * Text.length())};
		float Vertices[2 * NumberOfVertices];
		float TextureCoordinates[2 * NumberOfVertices];
		unsigned short Indices[NumberOfVertices + Text.length() - 1];
		const float GlyphWidth{6.0f / 128.0f};
		const float GlyphHeight{12.0f / 64.0f};
		float CharacterWidth{CharacterHeight / 2.0f};
		const unsigned int GlyphsPerLine{128 / 6};
		float TextLeft{Left};
		int CharacterIndex{0};
		int IndexIndex{0};
		
		for(auto Character : Text)
		{
			if(IndexIndex > 0)
			{
				Indices[IndexIndex++] = 0xFFFF;
			}
			
			auto GlyphIndex{Character - CHARACTEROFFSET};
			
			assert((GlyphIndex >= 0) && (GlyphIndex < CHARACTERS));
			Vertices[CharacterIndex * 8 + 0] = TextLeft;
			Vertices[CharacterIndex * 8 + 1] = Top;
			TextureCoordinates[CharacterIndex * 8 + 0] = (GlyphIndex % GlyphsPerLine) * GlyphWidth;
			TextureCoordinates[CharacterIndex * 8 + 1] = 1.0f - (GlyphHeight * static_cast< unsigned int >(GlyphIndex / GlyphsPerLine));
			Indices[IndexIndex++] = CharacterIndex * 4 + 0;
			Vertices[CharacterIndex * 8 + 2] = TextLeft;
			if(LeftHanded == true)
			{
				Vertices[CharacterIndex * 8 + 3] = Top + CharacterHeight;
			}
			else
			{
				Vertices[CharacterIndex * 8 + 3] = Top - CharacterHeight;
			}
			TextureCoordinates[CharacterIndex * 8 + 2] = (GlyphIndex % GlyphsPerLine) * GlyphWidth;
			TextureCoordinates[CharacterIndex * 8 + 3] = 1.0f - (GlyphHeight * static_cast< unsigned int >(GlyphIndex / GlyphsPerLine + 1));
			Indices[IndexIndex++] = CharacterIndex * 4 + 1;
			Vertices[CharacterIndex * 8 + 4] = TextLeft + CharacterWidth;
			if(LeftHanded == true)
			{
				Vertices[CharacterIndex * 8 + 5] = Top + CharacterHeight;
			}
			else
			{
				Vertices[CharacterIndex * 8 + 5] = Top - CharacterHeight;
			}
			TextureCoordinates[CharacterIndex * 8 + 4] = (GlyphIndex % GlyphsPerLine + 1) * GlyphWidth;
			TextureCoordinates[CharacterIndex * 8 + 5] = 1.0f - (GlyphHeight * static_cast< unsigned int >(GlyphIndex / GlyphsPerLine + 1));
			Indices[IndexIndex++] = CharacterIndex * 4 + 2;
			Vertices[CharacterIndex * 8 + 6] = TextLeft + CharacterWidth;
			Vertices[CharacterIndex * 8 + 7] = Top;
			TextureCoordinates[CharacterIndex * 8 + 6] = (GlyphIndex % GlyphsPerLine + 1) * GlyphWidth;
			TextureCoordinates[CharacterIndex * 8 + 7] = 1.0f - (GlyphHeight * static_cast< unsigned int >(GlyphIndex / GlyphsPerLine));
			Indices[IndexIndex++] = CharacterIndex * 4 + 3;
			CharacterIndex += 1;
			TextLeft += CharacterWidth;
		}
		
		GLuint Buffers[3];
		
		GLGenBuffers(3, Buffers);
		// index buffer
		GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[0]);
		GLBufferData(GL_ELEMENT_ARRAY_BUFFER, (NumberOfVertices + Text.length() - 1) * sizeof(unsigned short), Indices, GL_STREAM_DRAW);
		// vertex coordinates
		GLBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
		GLBufferData(GL_ARRAY_BUFFER, 2 * NumberOfVertices * sizeof(GLfloat), Vertices, GL_STREAM_DRAW);
		GLVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		GLEnableVertexAttribArray(0);
		// texture coordinates
		GLBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
		GLBufferData(GL_ARRAY_BUFFER, 2 * NumberOfVertices * sizeof(GLfloat), TextureCoordinates, GL_STREAM_DRAW);
		GLVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		GLEnableVertexAttribArray(1);
		assert(RenderContext != nullptr);
		RenderContext->SetProgramIdentifier(ProgamName);
		RenderContext->SetTexture(_FontTexture);
		RenderContext->ActivateProgram();
		GLEnable(GL_PRIMITIVE_RESTART);
		GLPrimitiveRestartIndex(0xFFFF);
		GLDrawElements(GL_TRIANGLE_FAN, NumberOfVertices + Text.length() - 1, GL_UNSIGNED_SHORT, NULL);
		GLBindVertexArray(0);
		GLDeleteBuffers(3, Buffers);
		GLDeleteVertexArrays(1, &VertexArray);
		RenderContext->SetTexture(nullptr);
		RenderContext->UnsetProgramIdentifier();
		RenderContext->DeactivateProgram();
		RealTime::Invalidate();
		
		double End(RealTime::Get());
		double Delta(End - Begin);
		
		g_SystemStatistics->SetFontSecondsThisFrame(g_SystemStatistics->GetFontSecondsThisFrame() + Delta);
	}
}

void Graphics::Drawing::DrawTexture(Graphics::RenderContext * RenderContext, float Left, float Top, float Bottom, float Right, Graphics::Texture * Texture)
{
	GLuint VertexArray{0};
	
	GLGenVertexArrays(1, &VertexArray);
	GLBindVertexArray(VertexArray);
	
	const GLsizei NumberOfVertices{4};
	float Vertices[2 * NumberOfVertices];
	float TextureCoordinates[2 * NumberOfVertices];
	
	Vertices[0] = Left;
	TextureCoordinates[0] = 0.0f;
	Vertices[1] = Top;
	TextureCoordinates[1] = 1.0f;
	Vertices[2] = Left;
	TextureCoordinates[2] = 0.0f;
	Vertices[3] = Bottom;
	TextureCoordinates[3] = 0.0f;
	Vertices[4] = Right;
	TextureCoordinates[4] = 1.0f;
	Vertices[5] = Bottom;
	TextureCoordinates[5] = 0.0f;
	Vertices[6] = Right;
	TextureCoordinates[6] = 1.0f;
	Vertices[7] = Top;
	TextureCoordinates[7] = 1.0f;
	
	GLuint Buffers[2];
	
	GLGenBuffers(2, Buffers);
	GLBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	GLBufferData(GL_ARRAY_BUFFER, 2 * NumberOfVertices * sizeof(GLfloat), Vertices, GL_STREAM_DRAW);
	GLVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	GLEnableVertexAttribArray(0);
	GLBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	GLBufferData(GL_ARRAY_BUFFER, 2 * NumberOfVertices * sizeof(GLfloat), TextureCoordinates, GL_STREAM_DRAW);
	GLVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	GLEnableVertexAttribArray(1);
	RenderContext->GetStyle()->SetProgramIdentifier("ui_texture");
	RenderContext->SetTexture(Texture);
	RenderContext->ActivateProgram();
	GLDrawArrays(GL_TRIANGLE_FAN, 0, NumberOfVertices);
	RenderContext->SetTexture(nullptr);
	RenderContext->DeactivateProgram();
	GLBindVertexArray(0);
	GLDeleteBuffers(2, Buffers);
	GLDeleteVertexArrays(1, &VertexArray);
}

void Graphics::Drawing::InitializeFont(void)
{
	assert(g_GraphicsEngine != nullptr);
	assert(g_GraphicsEngine->GetTextureManager() != nullptr);
	_FontTexture = g_GraphicsEngine->GetTextureManager()->Get("font");
	assert(_FontTexture != nullptr);
}

void Graphics::Drawing::DeinitializeFont(void)
{
	_FontTexture = nullptr;
}
