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

#ifndef GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__COMMON_H
#define GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__COMMON_H

#include <sys/types.h>

#include <stdexcept>
#include <string>

/**
 * @file Common.h
 * 
 * The Common.h include file contains the most basic set of declarations for libarxx.
 * 
 * The declarations in this file are intended to be self-satisfactory except for system includes so that every other include file can include them without care of forward declarations.
 **/

namespace Arxx
{
	/**
	 * @brief A type that should always be mapped to an unsigned 4 byte-long value.
	 * 
	 * If your architecture trips here, email the AUTHOR.
	 **/
	typedef u_int32_t u4byte;
	/**
	 * @brief A type that should always be mapped to an unsigned 8 byte-long value.
	 * 
	 * If your architecture trips here, email the AUTHOR.
	 **/
	typedef u_int64_t u8byte;
	
	/**
	 * @brief A type that should always be mapped to an unsigned 1 byte-long value.
	 * 
	 * If your architecture trips here, email the AUTHOR.
	 **/
	typedef u_int8_t u1byte;
	
	/**
	 * @brief A libarxx wide constant that always stands for invalid or not specified unique or non-unique IDs.
	 *
	 * This value mainly serves as a return value. If you call a function that returns an Item's unique ID this value is sematically the same as if the Arxx::Item pointer was returned with `nullptr`.
	 **/
	const u4byte g_InvalidItemIdentifier = 0xFFFFFFFF;
	
	/**
	 * @brief An exception class indicating that a given ID is not unique.
	 * 
	 * This exception is thrown whenever you wish to assign a new ID to an item and that ID is not unique inside the corresponding Arxx::Archive.
	 **/
	class id_not_unique : public std::invalid_argument
	{
	public:
		/**
		 * @brief The constructor of an id_not_unique exception.
		 * @param sString A string with further information for this exception.
		 *
		 * This exception is derieved from std::invalid_argument and the string passed via sString is directed to the constructor of std::invalid_argument and thus may be requested via the what() function of std::exception.
		 **/
		id_not_unique(const std::string & sString);
	};
	
	/**
	 * @brief An exception class indicating that the zlib library reports an error.
	 * 
	 * This exception is thrown whenever calls to the zlib library result in errors that may not be compensated inside libarxx.
	 **/
	class zlib_error : public std::runtime_error
	{
	public:
		/**
		 * @brief The constructor of a zlib_error exception.
		 * @param sString A string with further information for this exception.
		 *
		 * This exception is derieved from std::runtime_error and the string passed via sString is directed to the constructor of std::runtime_error so it may be requested via the what() function of std::exception.
		 **/
		zlib_error(const std::string & sString);
	};
	
	/**
	 * @brief An exception class indicating that the bzlib library reports an error.
	 * 
	 * This exception is thrown whenever calls to the bzlib library result in errors that may not be compensated inside libarxx.
	 **/
	class bzlib_error : public std::runtime_error
	{
	public:
		/**
		 * @brief The constructor of a bzlib_error exception.
		 * @param sString A string with further information for this exception.
		 *
		 * This exception is derieved from std::runtime_error and the string passed via sString is directed to the constructor of std::runtime_error so it may be requested via the what() function of std::exception.
		 **/
		bzlib_error(const std::string & sString);
	};
	
	/**
	 * @brief An exception class indicating an invalid file format in an ARX archive.
	 *
	 * This exception is thrown by the Arxx::Archive::bLoad() funtion to indicate that the archive that was to be opened is in an invalid format, could not be read as an ARX archive and thus wasn't loaded at all.
	 **/
	class bad_file_format : public std::logic_error
	{
	public:
		/**
		 * @brief The constructor of a bad_file_format exception.
		 * @param sString A string with further information for this exception.
		 *
		 * This exception is derieved from std::logic_error and the string passed via sString is directed to the constructor of std::logic_error so it may be requested via the what() function of std::exception.
		 **/
		bad_file_format(const std::string & sString);
	};
	
	/**
	 * @brief An exception class indicating that an error occured while reading or writing from or to a file.
	 *
	 * Most probably this exception is throw when you try to read a file that you don't have the access rights to read from or want to write to a file that you are not allowed to write to.
	 **/
	class file_error : public std::logic_error
	{
	public:
		/**
		 * @brief The constructor of a file_error exception.
		 * @param sString A string with further information for this exception.
		 *
		 * This exception is derieved from std::logic_error and the string passed via sString is directed to the constructor of std::logic_error so it may be requested via the what() function of std::exception.
		 **/
		file_error(const std::string & sString);
	};
}

#endif
