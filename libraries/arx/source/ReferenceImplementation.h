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

#ifndef ARXX_REFERENCEIMPLEMENTATION_H
#define ARXX_REFERENCEIMPLEMENTATION_H

#include <Common.h>

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
		static Arxx::ReferenceImplementation * pGetReference(Arxx::Item & Item);
		
		/**
		 * @brief Returns a new ReferenceImplementation instance created from a unique ID.
		 * @param u4UniqueID The unique ID that the new reference shoud refer to.
		 * @param pArchive Since unique IDs are not strictly bound to but only sensible in Archives, this lets you pass the appropriate Archive.
		 * 
		 * The new reference of course is unresolved.
		 **/
		static Arxx::ReferenceImplementation * pGetReference(Arxx::u4byte u4UniqueID, Arxx::Archive * pArchive = 0);
		
		/**
		 * @brief Increases the reference counter and conveniently returns the parameter.
		 * @param pReference The ReferenceImplementation object that you want to reference.
		 * 
		 * Since the new reference is equal to the old reference its resolved status is equal as well.
		 **/
		static Arxx::ReferenceImplementation * pGetReference(Arxx::ReferenceImplementation * pReference);
		
		/**
		 * @brief Decreases the reference counter and possible deletes the reference.
		 * @param pReference The ReferenceImplementation object that you want to release.
		 * @return A boolean value indicating whether the reference was deleted or not.
		 * 
		 * If decreasing the reference counter of @a pReference make it equal to zero, the object pReference is deleted.
		 **/
		static bool bRelease(Arxx::ReferenceImplementation * pReference);
		
		/**
		 * @brief Returns the unique ID of the reference.
		 * 
		 * It does not matter if the reference is resolved or unresolved, the unique ID is certain to be returned.
		 **/
		Arxx::u4byte u4GetUniqueID(void) const;
		
		/**
		 * @brief Returns the Arxx::Item pointer of the reference.
		 * 
		 * If the reference is resolved this correctly returns the Arxx::Item's pointer. In case of an unresolved reference the returned pointer is 0.
		 **/
		Arxx::Item * pGetItem(void);
		
		/**
		 * @brief Returns the Arxx::Item pointer of the reference.
		 * 
		 * If the reference is resolved this correctly returns the Arxx::Item's pointer. In case of an unresolved reference the returned pointer is 0.
		 **/
		const Arxx::Item * pGetItem(void) const;
		
		/**
		 * @brief Returns m_u4ReferenceCounter, the number of Arxx::Reference objects that hold @em this ReferenceImplementation object.
		 **/
		Arxx::u4byte u4GetReferenceCount(void) const;
		
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
		void vResolve(Arxx::Item & Item);
		
		/**
		 * @brief Unresolves the resolved reference by setting its Arxx::Item pointer to 0.
		 **/
		void vUnresolve(void);
		
		/**
		 * @brief This function sets m_pArchive to `0`.
		 **/
		void vDecoupleFromArchive(void);
	private:
		/**
		 * @brief The constructor of a ReferenceImplementation object.
		 * 
		 * Instances of this class are only created via the pGetReference() calls.
		 **/
		ReferenceImplementation(void);
		
		/**
		 * @brief The destructor of a ReferenceImplementation object.
		 * 
		 * Instances of this class may only be destroyed using the vRelease() call.
		 **/
		~ReferenceImplementation(void);
		
		/**
		 * @brief No copy semantic for ReferenceImplementation objects.
		 **/
		ReferenceImplementation(const Arxx::ReferenceImplementation & Reference);
		
		/**
		 * @brief No assigment semantic for ReferenceImplementation objects.
		 **/
		Arxx::ReferenceImplementation & operator=(const Arxx::ReferenceImplementation & Reference);
		
		Arxx::u4byte m_u4UniqueID;
		Arxx::Item * m_pItem;
		Arxx::u4byte m_u4ReferenceCount;
		Arxx::Archive * m_pArchive;
	};
}

#endif
