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

#ifndef GALACTIC_FALL__LIBRARIES__ARXX__SOURCE__REFERENCEIMPLEMENTATION_H
#define GALACTIC_FALL__LIBRARIES__ARXX__SOURCE__REFERENCEIMPLEMENTATION_H

#include <arxx/common.h>

namespace Arxx
{
	class Item;
	
	class Archive;
	
	/**
	 * @brief The real Arxx::Item reference used by Arxx::Reference to implement shared references.
	 **/
	class ReferenceImplementation
	{
	public:
		/**
		 * @brief Returns a new ReferenceImplementation instance created from a Arxx::Item.
		 * @param Item The Arxx::Item that the new reference should refer to.
		 * 
		 * Of course, the new reference is resolved.
		 **/
		static auto Create(Arxx::Item & Item) -> Arxx::ReferenceImplementation *;
		
		/**
		 * @brief Returns a new ReferenceImplementation instance created from a unique ID.
		 * @param u4UniqueID The unique ID that the new reference shoud refer to.
		 * @param pArchive Since unique IDs are not strictly bound to but only sensible in Archives, this lets you pass the appropriate Archive.
		 * 
		 * The new reference of course is unresolved.
		 **/
		static auto Create(Arxx::u4byte ItemIdentifier, Arxx::Archive * Archive = nullptr) -> Arxx::ReferenceImplementation *;
		
		/**
		 * @brief Increases the reference counter and conveniently returns the parameter.
		 * @param pReference The ReferenceImplementation object that you want to reference.
		 * 
		 * Since the new reference is equal to the old reference its resolved status is equal as well.
		 **/
		static auto Create(Arxx::ReferenceImplementation * ReferenceImplementation) -> Arxx::ReferenceImplementation *;
		
		/**
		 * @brief Decreases the reference counter and possible deletes the reference.
		 * @param pReference The ReferenceImplementation object that you want to release.
		 * @return A boolean value indicating whether the reference was deleted or not.
		 * 
		 * If decreasing the reference counter of @a pReference make it equal to zero, the object pReference is deleted.
		 **/
		static auto Release(Arxx::ReferenceImplementation * ReferenceImplementation) -> bool;
		
		/**
		 * @brief No copy semantic for ReferenceImplementation objects.
		 **/
		ReferenceImplementation(Arxx::ReferenceImplementation const & Reference) = delete;
		
		/**
		 * @brief No assigment semantic for ReferenceImplementation objects.
		 **/
		auto operator=(Arxx::ReferenceImplementation const & Reference) -> Arxx::ReferenceImplementation & = delete;
		
		/**
		 * @brief Returns the unique ID of the reference.
		 * 
		 * It does not matter if the reference is resolved or unresolved, the unique ID is certain to be returned.
		 **/
		auto GetItemIdentifier() const -> Arxx::u4byte;
		
		/**
		 * @brief Returns the Arxx::Item pointer of the reference.
		 * 
		 * If the reference is resolved this correctly returns the Arxx::Item's pointer. In case of an unresolved reference the returned pointer is 0.
		 **/
		auto GetItem() -> Arxx::Item *;
		
		/**
		 * @brief Returns the Arxx::Item pointer of the reference.
		 * 
		 * If the reference is resolved this correctly returns the Arxx::Item's pointer. In case of an unresolved reference the returned pointer is 0.
		 **/
		auto GetItem() const -> Arxx::Item const *;
		
		/**
		 * @brief Returns m_u4ReferenceCounter, the number of Arxx::Reference objects that hold @em this ReferenceImplementation object.
		 **/
		auto GetReferenceCount() const -> Arxx::u4byte;
		
		/**
		 * @brief Resolves an unresolved reference with a given item.
		 * @param Item The Arxx::Item that will be used to resolve the reference.
		 * 
		 * This functions checks for two things:
		 * - The reference must not be resolved yet.
		 * - The @a Item must have the same ID as the unresolved reference.
		 * 
		 * Not meeting one of these requirements will throw an std::runtime_error exception.
		 **/
		auto Resolve(Arxx::Item & Item) -> void;
		
		/**
		 * @brief Unresolves the resolved reference by setting its Arxx::Item pointer to 0.
		 **/
		auto Unresolve() -> void;
		
		/**
		 * @brief This function sets m_pArchive to `0`.
		 **/
		auto DecoupleFromArchive() -> void;
	private:
		/**
		 * @brief The constructor of a ReferenceImplementation object.
		 * 
		 * Instances of this class are only created via the pGetReference() calls.
		 **/
		ReferenceImplementation();
		
		/**
		 * @brief The destructor of a ReferenceImplementation object.
		 * 
		 * Instances of this class may only be destroyed using the vRelease() call.
		 **/
		~ReferenceImplementation();
		
		Arxx::u4byte m_ItemIdentifier;
		Arxx::Item * m_Item;
		Arxx::u4byte m_ReferenceCount;
		Arxx::Archive * m_Archive;
	};
}

#endif
