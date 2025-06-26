/**
 * libarxx - Advanced Resource files in C++
 * Copyright (C) 2005-2025  Hagen Möbius
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

#ifndef GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__ARXX_H
#define GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__ARXX_H

#include <arxx/Archive.h>
#include <arxx/Common.h>
#include <arxx/Item.h>

/**
 * @mainpage
 *
 * Welcome to Advanced Resource file for C++. This library provides classes and functions to open, modify and save ARX archives.
 *
 * ARX is just another file format. It is designed to contain an arbitrary amount of data chunks in a compressed state. Additionally the data chunks may be named, typed, versioned and structured in a directory tree. It therefore very much resembles any form of tar.bz2, tar.gz, rar or zip with the files beeing the data chunks.
 *
 * From a pragmatic point of view this very much covers the functionality but I wouldn't have done this library if it didn't provide at least some advantage or additional feature.
 *
 * To start reading and learning about libarxx you could start with Arxx::Archive which is the entry point for almost every Arxx session. Also the Arxx::Item is very important. Additionally the Modules page is recommended for reading.
 **/

/**
 * @brief The Arxx namespace which contains all the C++ classes you need to work with ARX archives.
 *
 * In this namespace the most important classes are Arxx::Item and Arxx::Archive.
 **/
namespace Arxx
{
};

#endif
