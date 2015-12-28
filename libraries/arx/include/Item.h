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

#ifndef ARXX_ITEM_H
#define ARXX_ITEM_H

#include "Data.h"
#include "Structure.h"

/**
 * @file Item.h
 * 
 * This file declares the interface of Arxx::Item.
 **/

namespace Arxx
{
	class Archive;
	
	/**
	 * @brief The Arxx::Item is the basic class for any data storaging.
	 *
	 * The Item is a thin wrapper around Arxx::Data but the inheritance ensures you can also treat it as an Arxx::Data object.
	 *
	 * It adds the basics of ARX to the data storage: Names, IDs, Types, SubTypes and a connection to a parenting Arxx::Archive.
	 *
	 * Additionally, at the moment it provides basic functionality for the structure, but that is about to change. Structure should only be handled by external structure classes.
	 **/
	class Item : public Arxx::Data
	{
		friend class Arxx::Archive;
	public:
		static Arxx::Item * Create(void);
		static Arxx::Item * Create(Arxx::u4byte Identifier);
		static Arxx::Item * Create(Arxx::Archive & Archive);
		static Arxx::Item * Create(Arxx::Archive & Archive, Arxx::u4byte Identifier);
		static Arxx::Item * Create(Arxx::Archive * Archive);
		static Arxx::Item * Create(Arxx::Archive * Archive, Arxx::u4byte Identifier);
		static void Delete(Arxx::Item * Item);
		
		/**
		 * @brief This function returns the identifier of @em this Arxx::Item.
		 * 
		 * Returns the identifier of @em this item. Remember that it is unique inside the associated archive.
		 **/
		Arxx::u4byte GetIdentifier(void) const;
		
		/**
		 * @brief Sets @em this item's unique identifier inside the library.
		 * @param u4UniqueID The new unique identifier of this item.
		 * 
		 * This function is trivial for items that don't belong to any library. The unique identifier has no meaning there and will just get assigned.
		 * 
		 * If @em this Arxx::Item is inside an Arxx:Archive the function is responsible for checking that the new identifier is not used yet.
		 **/
		void SetIdentifier(Arxx::u4byte Identifier);
		
		/**
		 * @brief Returns a const reference to the items name.
		 *
		 * This function is called for const items and returns a const reference to the item's name.
		 **/
		const std::string & GetName(void) const;
		
		/**
		 * @brief Set the item's name.
		 * @param Name The new name of the item.
		 *
		 * This function lets you change the name of an item to any value representable in a std::string.
		 **/
		void SetName(const std::string & Name);
		
		/**
		 * @brief Returns the type identifier of @em this item.
		 * 
		 * The type identifier has no sematic meaning to ARX or libarxx. It only serves the purpose of getting a meaning by the application.
		 *
		 * The default value for the type is 0xFFFFFFFF.
		 **/
		Arxx::u4byte GetType(void) const;
		
		/**
		 * @brief Returns the sub-type identifier of @em this item.
		 * 
		 * As the type the sub-type identifier has no sematic meaning to ARX or libarxx. It is meant to be placed into meaning by the application.
		 *
		 * The default value for the sub type is 0xFFFFFFFF.
		 **/
		Arxx::u4byte GetSubType(void) const;
		
		/**
		 * @brief Returns the major version number of @em this item.
		 **/
		Arxx::u1byte GetMajorVersionNumber(void) const;
		
		/**
		 * @brief Returns the minor version number of @em this item.
		 **/
		Arxx::u1byte GetMinorVersionNumber(void) const;
		
		/**
		 * @brief Returns the revision number of @em this item.
		 **/
		Arxx::u1byte GetRevisionNumber(void) const;
		
		/**
		 * @brief Returns the candidate number of @em this item.
		 **/
		Arxx::u1byte GetCandidateNumber(void) const;
		
		/**
		 * @brief Enables you to set the type identifier of @em this item.
		 * @param Type The type identifier you whish to set for @em this item.
		 * 
		 * The type identifier has no sematic meaning to ARX or libarxx. It only serves the purpose of getting a meaning by the application.
		 **/
		void SetType(Arxx::u4byte Type);
		
		/**
		 * @brief Enables you to set the sub-type identifier of @em this item.
		 * @param SubType The sub-type identifier you whish to set for @em this item.
		 * 
		 * As the type the sub-type identifier has no sematic meaning to ARX or libarxx. It is meant to be placed into meaning by the application.
		 **/
		void SetSubType(Arxx::u4byte SubType);
		
		/**
		 * @brief Sets the version number of this @em item.
		 * @param MajorVersionNumber The new major version number of the item.
		 * @param MinorVersionNumber The new minor version number of the item.
		 * @param RevisionNumber The new revision number of the item.
		 * @param CandidateNumber The new candidate number of the item.
		 *
		 * Sets the version numbers of the item. Note that this information is purely related to the content and in no way related to the components or structure of the Arxx::Item.
		 **/
		void SetVersionNumbers(Arxx::u1byte MajorVersionNumber, Arxx::u1byte MinorVersionNumber, Arxx::u1byte RevisionNumber, Arxx::u1byte CandidateNumber);
		
		/**
		 * @brief Returns a pointer to the registered Archive.
		 *
		 * The function will return `0` if there is no Archive set for the Item.
		 **/
		Arxx::Archive * GetArchive(void) const;
		
		/**
		 * @brief Returns the Structure component associated with this Item.
		 **/
		Arxx::Structure & GetStructure(void);
		
		/**
		 * @brief Returns the const Structure component associated with this Item.
		 **/
		const Arxx::Structure & GetStructure(void) const;
	protected:
		virtual bool _Fetch(Arxx::u4byte Offset, Arxx::u4byte Length);
	private:
		/**
		 * @brief The default constructor is private.
		 * @note Use Arxx::Item::Create() to create new items.
		 **/
		Item(void);
		
		/**
		 * @brief The destructor for items.
		 *
		 * This destructor is virtual so that you can savely derieve from the Arxx::Item.
		 **/
		virtual ~Item();
		
		/**
		 * @brief Hide the copy constructor from user space. We won't need it in any tool here.
		 * @sa If you ever should be in need of this functionality I need to figure out the semantics.
		 **/
		Item(const Item & Item);
		
		/**
		 * @brief Hide to assignment operator from user space.
		 * @sa If you are in need of this functionality, tell me so I can figure out the semantis needed for this.
		 **/
		Item & operator=(const Item & Item);
		
		/**
		 * @brief The item's identifier to identify it inside a library.
		 * 
		 * The item's identifier will identify the item in a library. The identifier allows access to the item in O(log(n)).
		 * 
		 * Depending on the constructor this field is either initialized with 0xFFFFFFFF or with the identifier passed to the constructor.
		 **/
		u4byte _Identifier;
		
		/**
		 * @brief The item's name in string form.
		 * 
		 * The name is in no way meant to identify the item. It's just used to have a short description of the item (although no length restriction is given).
		 * 
		 * Despite this, the name can be used to build paths over the items of the library.
		 * 
		 * Paths look like this:
		 * @code
		 * /
		 * /some
		 * /some/path
		 * /[child]some/[child]path
		 * /[child]some/path/[parent]some/path
		 * /[child]some/[other]path
		 * @endcode
		 * 
		 * After creating an Item this value is intentionally "".
		 **/
		std::string _Name;
		
		/**
		 * @brief A type identifier for @em this item.
		 * 
		 * The type of an item has no semantic meaning inside ARX or libarxx. It exists only to be used by applications to identify the type of the data of @em this item. Think of it as a replacement for extensions.
		 * 
		 * After creating an Item this value is intentionally 0xFFFFFFFF.
		 **/
		Arxx::u4byte _Type;
		
		/**
		 * @brief A subtype identifier for @em this item.
		 * 
		 * Like the type the subtype has no semantic meaning  inside ARX or libarxx. Ite exists only to be used by the application to identify subtypes of types. This is useful for example to distinguish between different versions of data format that belong to the same type. It can also be used to highlight semantic meaning of a general data format on filesystem layer, like identifying all kinds of log files with a certain type and distinguishing between error, info, debug and critical with different subtypes.
		 * 
		 * After creating an Item this value is intentionally 0xFFFFFFFF.
		 **/
		Arxx::u4byte _SubType;
		
		/**
		 * @brief The major version number of the item.
		 **/
		Arxx::u1byte _MajorVersionNumber;
		
		/**
		 * @brief The minor version number of the item.
		 **/
		Arxx::u1byte _MinorVersionNumber;
		
		/**
		 * @brief The revision number of the item.
		 **/
		Arxx::u1byte _RevisionNumber;
		
		/**
		 * @brief The candidate number of the item.
		 **/
		Arxx::u1byte _CandidateNumber;
		
		/**
		 * @brief A pointer to the Archive.
		 * 
		 * Depending on the constructor this field is either initialized with 0 or with the address of the Archive passed to the constructor.
		 * 
		 * It is changed in conjuction with the Arxx::Archive::vRegisterItem() and Arxx::Archive::vUnregisterItem() functions.
		 **/
		Arxx::Archive * _Archive;
		
		/**
		 * @brief The structural information of an item.
		 **/
		Arxx::Structure _Structure;
	};
}

#endif
