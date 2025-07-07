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

#include <arxx/common.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::id_not_unique                                                                            //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::id_not_unique::id_not_unique(std::string const & String) :
	std::invalid_argument{String}
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::zlib_error                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::zlib_error::zlib_error(std::string const & String) :
	std::runtime_error{String}
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::bzlib_error                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::bzlib_error::bzlib_error(std::string const & String) :
	std::runtime_error{String}
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::bad_file_format                                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::bad_file_format::bad_file_format(std::string const & String) :
	std::logic_error{String}
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Arxx::file_error                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
Arxx::file_error::file_error(std::string const & String) :
	std::logic_error{String}
{
}
