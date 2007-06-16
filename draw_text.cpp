/**
 * galactic-fall
 * Copyright (C) 2006  Hagen MÃ¶bius
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

#define CHARACTERS 95
#define CHARACTEROFFSET 32

const GLubyte g_Letters[][12] = 
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 00 => ' '
	{ 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00 }, // 01 => '!'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x50, 0x50, 0x00 }, // 02 => '"'
	{ 0x00, 0x00, 0x00, 0x50, 0x50, 0xF8, 0x50, 0xF8, 0x50, 0x50, 0x00, 0x00 }, // 03 => '#'
	{ 0x00, 0x00, 0x00, 0x20, 0xF0, 0x28, 0x70, 0xA0, 0x78, 0x20, 0x00, 0x00 }, // 04 => '$'
	{ 0x00, 0x00, 0x00, 0x18, 0x98, 0x40, 0x20, 0x10, 0xC8, 0xC0, 0x00, 0x00 }, // 05 => '%'
	{ 0x00, 0x00, 0x00, 0x68, 0x90, 0xA8, 0x40, 0x80, 0x80, 0x70, 0x00, 0x00 }, // 06 => '&'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x00 }, // 07 => '''
	{ 0x00, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x08 }, // 08 => '('
	{ 0x00, 0x40, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x40 }, // 09 => ')'
	{ 0x00, 0x00, 0x00, 0x00, 0x20, 0xA8, 0x70, 0xA8, 0x20, 0x00, 0x00, 0x00 }, // 10 => '*'
	{ 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x00, 0x00, 0x00 }, // 11 => '+'
	{ 0x00, 0x40, 0x20, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 12 => ','
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 13 => '-'
	{ 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 14 => '.'
	{ 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08 }, // 15 => '/'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0xC8, 0xA8, 0x98, 0x88, 0x70, 0x00 }, // 16 => '0'
	{ 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x10, 0x00 }, // 17 => '1'
	{ 0x00, 0x00, 0x00, 0xF8, 0x80, 0x40, 0x20, 0x10, 0x08, 0x88, 0x70, 0x00 }, // 18 => '2'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x08, 0x08, 0x30, 0x08, 0x88, 0x70, 0x00 }, // 19 => '3'
	{ 0x00, 0x00, 0x00, 0x38, 0x10, 0xF8, 0x50, 0x50, 0x30, 0x30, 0x10, 0x00 }, // 20 => '4'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x08, 0x08, 0xF0, 0x80, 0x80, 0xF8, 0x00 }, // 21 => '5'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x40, 0x30, 0x00 }, // 22 => '6'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x88, 0xF8, 0x00 }, // 23 => '7'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x88, 0x88, 0x70, 0x00 }, // 24 => '8'
	{ 0x00, 0x00, 0x00, 0x60, 0x10, 0x08, 0x78, 0x88, 0x88, 0x88, 0x70, 0x00 }, // 25 => '9'
	{ 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00 }, // 26 => ':'
	{ 0x00, 0x40, 0x20, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00 }, // 27 => ';'
	{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0xC0, 0x30, 0x0C, 0x00, 0x00, 0x00 }, // 28 => '<'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00 }, // 29 => '='
	{ 0x00, 0x00, 0x00, 0x00, 0xC0, 0x30, 0x0C, 0x30, 0xC0, 0x00, 0x00, 0x00 }, // 30 => '>'
	{ 0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x20, 0x10, 0x08, 0x88, 0x70, 0x00 }, // 31 => '?'
	{ 0x00, 0x00, 0x00, 0x70, 0x80, 0xB0, 0xB8, 0xB8, 0x88, 0x70, 0x00, 0x00 }, // 32 => '@'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x50, 0x20, 0x00, 0x00 }, // 33 => 'A'
	{ 0x00, 0x00, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0x88, 0x88, 0xF0, 0x00, 0x00 }, // 34 => 'B'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x80, 0x80, 0x80, 0x88, 0x70, 0x00, 0x00 }, // 35 => 'C'
	{ 0x00, 0x00, 0x00, 0xE0, 0x90, 0x88, 0x88, 0x88, 0x90, 0xE0, 0x00, 0x00 }, // 36 => 'D'
	{ 0x00, 0x00, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x00, 0x00 }, // 37 => 'E'
	{ 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x00, 0x00 }, // 38 => 'F'
	{ 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0x98, 0x80, 0x88, 0x70, 0x00, 0x00 }, // 39 => 'G'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x00, 0x00 }, // 40 => 'H'
	{ 0x00, 0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00 }, // 41 => 'I'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x08, 0x08, 0x08, 0x38, 0x00, 0x00 }, // 42 => 'J'
	{ 0x00, 0x00, 0x00, 0x88, 0x90, 0xA0, 0xC0, 0xA0, 0x90, 0x88, 0x00, 0x00 }, // 43 => 'K'
	{ 0x00, 0x00, 0x00, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00 }, // 44 => 'L'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0xA8, 0xA8, 0xD8, 0x88, 0x00, 0x00 }, // 45 => 'M'
	{ 0x00, 0x00, 0x00, 0x88, 0x98, 0x98, 0xA8, 0xC8, 0xC8, 0x88, 0x00, 0x00 }, // 46 => 'N'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00 }, // 47 => 'O'
	{ 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xF0, 0x88, 0x88, 0xF0, 0x00, 0x00 }, // 48 => 'P'
	{ 0x00, 0x00, 0x18, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00 }, // 49 => 'Q'
	{ 0x00, 0x00, 0x00, 0x88, 0x90, 0xA0, 0xF0, 0x88, 0x88, 0xF0, 0x00, 0x00 }, // 50 => 'R'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x08, 0x70, 0x80, 0x88, 0x70, 0x00, 0x00 }, // 51 => 'S'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x00 }, // 52 => 'T'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00 }, // 53 => 'U'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x50, 0x50, 0x88, 0x88, 0x88, 0x00, 0x00 }, // 54 => 'V'
	{ 0x00, 0x00, 0x00, 0x88, 0xD8, 0xA8, 0xA8, 0x88, 0x88, 0x88, 0x00, 0x00 }, // 55 => 'W'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x50, 0x20, 0x50, 0x88, 0x88, 0x00, 0x00 }, // 56 => 'X'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x50, 0x88, 0x88, 0x00, 0x00 }, // 57 => 'Y'
	{ 0x00, 0x00, 0x00, 0xF8, 0x80, 0x40, 0x20, 0x10, 0x08, 0xF8, 0x00, 0x00 }, // 58 => 'Z'
	{ 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x38 }, // 59 => '['
	{ 0x00, 0x00, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80 }, // 60 => '\'
	{ 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70 }, // 61 => ']'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x50, 0x20, 0x00 }, // 62 => '^'
	{ 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // 63 => '_'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00 }, // 64 => '`'
	{ 0x00, 0x00, 0x00, 0x68, 0x98, 0x88, 0x88, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 65 => 'a'
	{ 0x00, 0x00, 0x00, 0xF0, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x80, 0x00 }, // 66 => 'b'
	{ 0x00, 0x00, 0x00, 0x78, 0x80, 0x80, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 67 => 'c'
	{ 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0x88, 0x78, 0x08, 0x08, 0x08, 0x00 }, // 68 => 'd'
	{ 0x00, 0x00, 0x00, 0x70, 0x80, 0xF8, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00 }, // 69 => 'e'
	{ 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0xF0, 0x40, 0x40, 0x38, 0x00 }, // 70 => 'f'
	{ 0x70, 0x08, 0x08, 0x78, 0x88, 0x88, 0x88, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 71 => 'g'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x80, 0x00 }, // 72 => 'h'
	{ 0x00, 0x00, 0x00, 0x70, 0x20, 0x20, 0x20, 0x60, 0x00, 0x20, 0x20, 0x00 }, // 73 => 'i'
	{ 0x70, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38, 0x00, 0x08, 0x08, 0x00 }, // 74 => 'j'
	{ 0x00, 0x00, 0x00, 0x48, 0x50, 0x60, 0x50, 0x48, 0x40, 0x40, 0x40, 0x00 }, // 75 => 'k'
	{ 0x00, 0x00, 0x00, 0x70, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0x00 }, // 76 => 'l'
	{ 0x00, 0x00, 0x00, 0x88, 0xA8, 0xA8, 0xA8, 0xD0, 0x00, 0x00, 0x00, 0x00 }, // 77 => 'm'
	{ 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0xC8, 0xB0, 0x00, 0x00, 0x00, 0x00 }, // 78 => 'n'
	{ 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00, 0x00, 0x00 }, // 79 => 'o'
	{ 0x80, 0x80, 0x80, 0xF0, 0x88, 0x88, 0x88, 0xF0, 0x00, 0x00, 0x00, 0x00 }, // 80 => 'p'
	{ 0x08, 0x08, 0x08, 0x78, 0x88, 0x88, 0x88, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 81 => 'q'
	{ 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x60, 0x58, 0x00, 0x00, 0x00, 0x00 }, // 82 => 'r'
	{ 0x00, 0x00, 0x00, 0xF0, 0x08, 0x70, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00 }, // 83 => 's'
	{ 0x00, 0x00, 0x00, 0x18, 0x20, 0x20, 0x20, 0x70, 0x20, 0x20, 0x20, 0x00 }, // 84 => 't'
	{ 0x00, 0x00, 0x00, 0x68, 0x98, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00 }, // 85 => 'u'
	{ 0x00, 0x00, 0x00, 0x20, 0x20, 0x50, 0x50, 0xD8, 0x00, 0x00, 0x00, 0x00 }, // 86 => 'v'
	{ 0x00, 0x00, 0x00, 0x50, 0xA8, 0xA8, 0xA8, 0x88, 0x00, 0x00, 0x00, 0x00 }, // 87 => 'w'
	{ 0x00, 0x00, 0x00, 0x88, 0x50, 0x20, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00 }, // 88 => 'x'
	{ 0x70, 0x08, 0x08, 0x78, 0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00 }, // 89 => 'y'
	{ 0x00, 0x00, 0x00, 0xF8, 0x40, 0x20, 0x10, 0xF8, 0x00, 0x00, 0x00, 0x00 }, // 90 => 'z'
	{ 0x00, 0x08, 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08 }, // 91 => '{'
	{ 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 }, // 92 => '|'
	{ 0x00, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x40 }, // 93 => '}'
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xA8, 0x40, 0x00, 0x00, 0x00, 0x00 }  // 94 => '~'
};

void DrawText(const std::string & String)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glRasterPos2f(0.0f, 12.0f);
	for(std::string::size_type I = 0; I < String.length(); ++I)
	{
		glBitmap(6, 12, 0, 0, 6, 0, g_Letters[String[I] - CHARACTEROFFSET]);
	}
}

void DrawTextWithoutTranslation(const std::string & String)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glRasterPos2f(0.0f, 0.0f);
	for(std::string::size_type I = 0; I < String.length(); ++I)
	{
		glBitmap(6, 12, 0, 0, 6, 0, g_Letters[String[I] - CHARACTEROFFSET]);
	}
}
