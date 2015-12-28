/**
 * libarxx - Advanced Resource files in C++
 * Copyright (C) 2005  Hagen Möbius
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

#include "../include/Common.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::id_not_unique                                                                            //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::id_not_unique::id_not_unique(const std::string & sString) :
	std::invalid_argument(sString)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::zlib_error                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::zlib_error::zlib_error(const std::string & sString) :
	std::runtime_error(sString)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::bzlib_error                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::bzlib_error::bzlib_error(const std::string & sString) :
	std::runtime_error(sString)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::bad_file_format                                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::bad_file_format::bad_file_format(const std::string & sString) :
	std::logic_error(sString)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::file_error                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::file_error::file_error(const std::string & sString) :
	std::logic_error(sString)
{
}
