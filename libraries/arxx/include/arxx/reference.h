/**
 * ARX - Advanced Resource Archives
 * Copyright (C) 2005-2026  Hagen Möbius
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

#ifndef GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__REFERENCE_H
#define GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__REFERENCE_H

#include <arxx/common.h>

/**
 * @file reference.h
 * 
 * This file declares the Arxx::Reference class, a class for unified Arxx::Item access in resolved and unresolved environments.
 **/

namespace Arxx
{
	class Item;
	class Archive;
	class ReferenceCore;
	
	/**
	 * @brief Instances of this class represent items either by unique ID or if known by pointer.
	 * 
	 * All in all this is a cache for Arxx::Item pointers.
     * Generally, you would identify Arxx::Item instances by their unique IDs and retrieve the appropriate Arxx::Item pointer via the associated library.
     * However, returning the item corresponding to a certain unique ID is costly and should only be done very seldom.
     * Therefore this class provides an abstraction for references to items.
	 * 
	 * When created via the copy constructor Reference objects share a reference counted ReferenceImplementation object that holds the actual reference.
     * This means that, once a unique ID is resolved into the appropriate Item pointer it is done so everywhere.
	 **/
	class Reference
	{
	public:
        friend Arxx::Archive;
        
		/**
		 * @brief Constructs a Arxx::Reference from a Arxx::Item.
		 * @param Item The Arxx::Item that this Reference should refer to.
		 * 
		 * This constructor will of course create a new ReferenceCore object, so it can store its resolved reference.
		 **/
		explicit Reference(Arxx::Item & Item);
		
		/**
		 * @brief Constructs a Arxx::Reference from a unique ID.
		 * @param UniqueID The unique ID that this Reference should refer to.
		 * @param Archive Optional Archive that this reference belongs to.
		 * 
		 * This constructor will of course create a new ReferenceCore object, so it can store its unresolved reference.
		 **/
		Reference(std::uint32_t ItemIdentifier, Arxx::Archive * Archive = nullptr);
		
		/**
		 * @brief Constructs a Arxx::Reference from another reference object.
		 * @param Other The other reference object that this reference should copy.
		 * 
		 * This constructor will use the Arxx::ReferenceCore object from @a Other.
		 **/
		Reference(Arxx::Reference const & Other);
		
		/**
		 * @brief Destroys a Arxx::Reference object.
		 * 
		 * This mainly releases the underlying Arxx::ReferenceCore object.
		 **/
		virtual ~Reference();
		
		/**
		 * @brief Returns the item identifier of the reference.
		 * 
		 * It does not matter if the reference is resolved or unresolved, the item identifier is certain to be returned.
		 **/
		auto GetItemIdentifier() const -> std::uint32_t;
		
		/**
		 * @brief Returns the Arxx::Item pointer of the reference.
		 * 
		 * If the reference is resolved this correctly returns the Arxx::Item's pointer.
         * In case of an unresolved reference the returned pointer is `nullptr`.
		 * 
		 * @sa Arxx::ReferenceCore::GetItem().
		 **/
		auto GetItem() -> Arxx::Item *;
		
		/**
		 * @brief Returns the Arxx::Item pointer of the reference.
		 * 
		 * If the reference is resolved this correctly returns the Arxx::Item's pointer.
         * In case of an unresolved reference the returned pointer is `nullptr`.
		 * 
		 * @sa Arxx::ReferenceCore::GetItem().
		 **/
		auto GetItem() const -> Arxx::Item const *;
		
		/**
		 * @brief Returns the number of Arxx::Reference objects that reference the same ReferenceCore as _this_ Reference.
		 * @sa Arxx::ReferenceCore::GetReferenceCount().
		 **/
		auto GetReferenceCount() const -> std::uint32_t;
		
		/**
		 * @brief Resolves an unresolved reference with a given item.
		 * @param Item The Arxx::Item that will be used to resolve the reference.
		 * @sa Arxx::ReferenceCore::Resolve().
		 **/
		auto Resolve(Arxx::Item & Item) -> void;
		
		/**
		 * @brief Unresolves the resolved reference.
		 * @sa Arxx::ReferenceCore::Unresolve().
		 **/
		auto Unresolve() -> void;
		
		/**
		 * @brief This function will attach another Reference object representing the same Item.
		 * @param Reference A Reference that is to be attached to this Reference set.
		 * 
		 * A Reference set is a number of Reference objects that use the same ReferenceCore.
		 * This function will call Reference.Detach if GetReferenceCount() is greater than 1.
		 * This function will return with false if either the item identifiers don't match or @a Reference is resolved.
		 **/
		auto Attach(Arxx::Reference & Reference) -> bool;
		
		/**
		 * @brief This function will detach the Reference object from the Reference set.
		 * 
		 * A Reference set is a number of Reference objects that use the same ReferenceCore.
		 * This function will release the old m_ReferenceCore, and create a new ReferenceCore object to be used by _this_ Reference.
		 * This Reference and its ReferenceCore will be unresolved after this, and decoupled from any Archive that it may have been connected to prior to this call.
		 **/
		auto Detach() -> bool;
		
		auto operator<(Arxx::Reference const & Other) const -> bool
		{
			return GetItemIdentifier() < Other.GetItemIdentifier();
		}
        
	private:
		/**
		 * @brief This function will decouple the reference core from the associated archive.
		 **/
		auto m_DecoupleFromArchive() -> void;
        
		/**
		 * @brief Small helper function.
		 **/
		auto m_SetCore(Arxx::ReferenceCore * ReferenceCore) -> void;
		
		Arxx::ReferenceCore * m_ReferenceCore;
        
	};
}

#endif
