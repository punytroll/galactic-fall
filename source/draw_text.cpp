/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include <GL/gl.h>

#include <string>

#include "globals.h"
#include "graphics/engine.h"
#include "graphics/gl.h"
#include "graphics/texture.h"
#include "graphics/texture_manager.h"
#include "real_time.h"
#include "system_statistics.h"

#define CHARACTERS 95
#define CHARACTEROFFSET 32

GLuint g_CharacterCallLists(0);
const Graphics::Texture * g_FontTexture(0);

void InitializeFont(void)
{
	assert(g_GraphicsEngine != 0);
	assert(g_GraphicsEngine->GetTextureManager() != 0);
	g_FontTexture = g_GraphicsEngine->GetTextureManager()->Get("font");
	assert(g_FontTexture != 0);
	
	const float CharacterWidth(6.0f / 128.0f);
	const float CharacterHeight(12.0f / 64.0f);
	const unsigned int CharactersPerLine(128 / 6);
	
	g_CharacterCallLists = GLGenLists(CHARACTERS);
	assert(g_CharacterCallLists != 0);
	for(unsigned int CharacterIndex = 0; CharacterIndex < CHARACTERS; ++CharacterIndex)
	{
		GLNewList(g_CharacterCallLists + CharacterIndex, GL_COMPILE);
		GLBegin(GL_QUADS);
		GLTexCoord2f((CharacterIndex % CharactersPerLine) * CharacterWidth, 1.0f - (CharacterHeight * static_cast< unsigned int >(CharacterIndex / CharactersPerLine + 1)));
		GLVertex2f(0.0f, 12.0f);
		GLTexCoord2f((CharacterIndex % CharactersPerLine + 1) * CharacterWidth, 1.0f - (CharacterHeight * static_cast< unsigned int >(CharacterIndex / CharactersPerLine + 1)));
		GLVertex2f(6.0f, 12.0f);
		GLTexCoord2f((CharacterIndex % CharactersPerLine + 1) * CharacterWidth, 1.0f - (CharacterHeight * static_cast< unsigned int >(CharacterIndex / CharactersPerLine)));
		GLVertex2f(6.0f, 0.0f);
		GLTexCoord2f((CharacterIndex % CharactersPerLine) * CharacterWidth, 1.0f - (CharacterHeight * static_cast< unsigned int >(CharacterIndex / CharactersPerLine)));
		GLVertex2f(0.0f, 0.0f);
		GLEnd();
		GLTranslatef(6.0f, 0.0f, 0.0f);
		GLEndList();
	}
}

void DeinitializeFont(void)
{
	GLDeleteLists(g_CharacterCallLists, CHARACTERS);
	g_FontTexture = 0;
}

void DrawText(const std::string & String)
{
	RealTime::Invalidate();
	
	double Begin(RealTime::Get());
	
	GLPushAttrib(GL_ENABLE_BIT);
	GLEnable(GL_TEXTURE_2D);
	assert(g_FontTexture != 0);
	g_FontTexture->Activate();
	GLListBase(g_CharacterCallLists - CHARACTEROFFSET);
	GLCallLists(String.length(), GL_UNSIGNED_BYTE, String.c_str());
	GLPopAttrib();
	RealTime::Invalidate();
	
	double End(RealTime::Get());
	double Delta(End - Begin);
	
	g_SystemStatistics->SetFontSecondsThisFrame(g_SystemStatistics->GetFontSecondsThisFrame() + Delta);
}
