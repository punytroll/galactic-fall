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

#ifndef GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__ARCHIVE_H
#define GALACTIC_FALL__LIBRARIES__ARXX__INCLUDE__ARCHIVE_H

#include <map>
#include <vector>

#include <arxx/common.h>

/**
 * @file Archive.h
 * 
 * This file declares the Arxx::Archive class and Arxx::Archive::iterator and Arxx::Archive::const_iterator iterator classes.
 **/

namespace Arxx
{
	class Buffer;
	
	class Item;
	
	class Reference;
	
	class ReferenceImplementation;
	
	/**
	 * @brief The Arxx::Archive class is a container for an arbitrary amount of Arxx::Item objects.
	 *
	 * The main purpose of Arxx::Archive is to handle input and output from and to ARX archives. But it also is responsible for the management of unique IDs and lets you access items you only know by ID.
	 * 
	 * In order to be able to manage input and output Arxx::Archive provides two functions: Arxx::Archive::bLoad() and Arxx::Archive::vSave().
	 **/
	class Archive
	{
	public:
		/**
		 * @brief An iterator class defining basic operations to iterate the Arxx::Item objects in an archive.
		 *
		 * This iterator class encapsules the internal form of Item object storage in an archive. By using this iterator class, which is what you have to do, you are safe from changes in implementation detail as the container used to store Arxx::Item object pointers. Additionally it is the prefered way to make a visit to each item in an archive.
		 **/
		class iterator
		{
		public:
			/**
			 * @brief The constructor of an Arxx::Item iterator.
			 * @param iItem The STL iterator from Arxx::Archive's aggregated Item object container. The internal representation of the iterator.
			 *
			 * This constructor creates a an Item iterator from an STL iterator.
			 **/
			iterator(std::map<Arxx::u4byte, Arxx::Item *>::iterator Iterator);
			
			/**
			 * @brief The destructor of an Item iterator.
			 **/
			~iterator();
			
			/**
			 * @brief Advances the iterator.
			 *
			 * Advances the Item iterator. It is not defined which will be the next Arxx::Item the iterator points to. It is only assured that every Item will be passed once.
			 **/
			auto operator++() -> Arxx::Archive::iterator &;
			
			/**
			 * @brief This will return the Arxx::Item that this iterator points to.
			 * 
			 * This is the non-const version of the operator*() so it returns a non const Item reference.
			 **/
			auto operator*() -> Arxx::Item *;
			
			/**
			 * @brief This will return the Arxx::Item that this iterator points to.
			 * 
			 * This is the const version of the operator*() thus the returned reference is const as well.
			 **/
			auto operator*() const -> Arxx::Item const *;
			
			/**
			 * @brief The Arxx::Item accessor.
			 **/
			auto operator->() -> Arxx::Item *;
			
			/**
			 * @brief Tests two iterators for equality.
			 * @param iItem The iterator to check against.
			 **/
			auto operator==(Arxx::Archive::iterator const & Other) const -> bool;
			
			/**
			 * @brief Tests two iterators for inequality.
			 * @param iItem The iterator to check against.
			 **/
			auto operator!=(Arxx::Archive::iterator const & Other) const -> bool;
            
		private:
			/**
			 * @brief The internal STL iterator.
			 * 
			 * This is an iterator into the Arxx::Archive's internal Item map @a Arxx::Archive::m_Items.
			 **/
			std::map<Arxx::u4byte, Arxx::Item *>::iterator m_Iterator;
            
		};
		
		/**
		 * @brief An iterator class defining basic operations to iterate the Arxx::Item objects in an Arxx::Archive.
		 *
		 * This iterator class encapsules the internal form of Item object storage in an archive. By using this iterator class, which is what you have to do, you are safe from changes in implementation detail as the container used to store Item object pointers. Additionally it is the prefered way to make a visit to each Item in an archive.
		 **/
		class const_iterator
		{
		public:
			/**
			 * @brief The constructor of a const Item iterator.
			 * @param iItem The STL iterator from Arxx::Archive's aggregated Item object container. The internal representation of the iterator.
			 *
			 * This constructor creates a an Item iterator from an STL iterator.
			 **/
			const_iterator(std::map<Arxx::u4byte, Arxx::Item *>::const_iterator Iterator);
			
			/**
			 * @brief The destructor of a const Item iterator.
			 **/
			~const_iterator();
			
			/**
			 * @brief Advances the iterator.
			 *
			 * Advances the Item iterator. It is not defined which will be the next Arxx::Item the iterator points to. It is only assured that every Item will be passed once.
			 **/
			auto operator++() -> Arxx::Archive::const_iterator &;
			
			/**
			 * @brief This will return the Arxx::Item that this iterator points to.
			 * 
			 * This is the const version of the operator*() thus the returned Item reference is const as well.
			 **/
			auto operator*() const -> Arxx::Item const *;
			
			/**
			 * @brief The Arxx::Item accessor.
			 **/
			auto operator->() const -> Arxx::Item const *;
			
			/**
			 * @brief Tests two iterators for equality.
			 * @param iItem The iterator to check against.
			 **/
			auto operator==(const Arxx::Archive::const_iterator & Other) const -> bool;
			
			/**
			 * @brief Tests two iterators for inequality.
			 * @param iItem The iterator to check against.
			 **/
			auto operator!=(const Arxx::Archive::const_iterator & Other) const -> bool;
            
		private:
			/**
			 * @brief The internal STL iterator.
			 * 
			 * This is an iterator into Arxx::Archive's internal Item map @a Arxx::Archive::m_Items.
			 **/
			std::map<Arxx::u4byte, Arxx::Item *>::const_iterator m_Iterator;
            
		};
		
		typedef std::map<Arxx::u4byte, Arxx::Item *>::size_type size_type;
		
		/**
		 * @brief The default constructor.
		 *
		 * This constructor creates and initializes a Arxx::Archive.
		 **/
		Archive();
        
		/**
		 * @brief No copy semantic.
		 **/
		Archive(Arxx::Archive const & Other) = delete;
		
		/**
		 * @brief The destructor for a Arxx::Archive.
		 * 
		 * The destructor will call Arxx::Archive::vClose().
		 **/
		virtual ~Archive();
		
		/**
		 * @brief No assignment semantic.
		 **/
		auto operator=(Arxx::Archive const & Other) -> Arxx::Archive & = delete;
		
		/**
		 * @brief Load the content of an ARX archive into this Arxx::Archive.
		 * @param FilePath The path to the archive file.
		 * @return A boolean value indicating whether the ARX archive could be loaded or not. An error can have many reasons (file existance, permissions and content) but there is no facility to report which error occured. If you feel a need for it, please notify me.
		 * 
		 * This function tries to load the ARX archive at the location indicated by @a FilePath. If the archive is found and could be opened, the current content of the Arxx::Archive will be closed (via Arxx::Archive::vClose()). Note that if the archive proves to be no valid ARX archive the current content is not available anymore.
		 **/
		auto Load(std::string const & FilePath) -> bool;
		
		/**
		 * @brief Saves the content to an ARX archive.
		 * @param FilePath The path to the new archive file.
		 * @param AutoCompress Whether all Arxx::Item objects should be auto-compressed.
		 * @throw Arxx::file_error If the temporary archive in /tmp cannot be created for writing.
		 *
		 * Given the @a FilePath, the content of an Arxx::Archive instance will be saved to the location it indicates. The target will be overwritten if it exists but only after it has been fully assembled in a temporary archive. The temorary archive will be stored under "/tmp/" with the name "ARX" followed by the timestamp. After the saving process to that temporary file it will be moved to the target location, using the command `mv --force PathToTemporaryFile @a FilePath`
		 */
		auto Save(std::string const & FilePath, bool AutoCompress = false) -> void;
		
		/**
		 * @brief Closes the Archive.
		 * 
		 * Closing an Archive means purging it of all Arxx::Item objects. All Item objects in the Archive will be deleted by their respective Arxx::ItemFactory objects. If an Item has no associated Arxx::ItemFactory set it won't get deleted since the Archive does not know how. In most cases this will be true for Items on the stack but may also be true for wrongly done ItemFactory derivates.
		 * 
		 * @note Not every ItemFactory, that does not set an Item's ItemFactory member correctly on creation, can be considered 'wrong'. If the ItemFactory still has references to the Items, it is absolutely perfect for having persistent Items inside an Archive.
		 **/
		auto Close() -> void;
		
		/**
		 * @brief Registers an Item to be a part of an Archive.
		 * @param Item A reference to the Item that is to be registerd.
		 * @throw std::invalid_argument If @a Item is already registered at another Archive.
		 * 
		 * First of all this function will check whether the item is already assigned to an Archive and will throw an exception of type std::invalid_argument.
		 * 
		 * If the Item is not yet registered at another Archive, this function will set Arxx::Item::m_pArchive to point to this Archive.
		 * 
		 * If the Item is not yet associated to any Arxx::Archive this function will decide on one of the following behavior, depending on whether the unique ID of @a Item is equal to g_u4InvalidID or not.
		 * - If the unique ID is equal to g_u4InvalidID this function will search for a unique ID not yet used inside this Archive.
		 * - If the unique ID is any other value, this function will allow the registration if that ID is not yet used by another item in this Archive. Not alowing a registration means to throw an exception of the type Arxx::id_not_unique.
		 **/
		auto Register(Arxx::Item * Item) -> void;
		
		/**
		 * @brief Unregisters an Item.
		 * @param Item A reference to the Item that is to be unregisterd.
		 * @throw std::invalid_argument If @a Item is not registered in the Archive.
		 * 
		 * This function will remove @a Item from an Archive by erasing the appropriate iterator from the Item object container owned by the Archive.
		 * 
		 * If @a Item is not correctly registered in the Archive an std::invalid_argument exception is thrown.
		 * 
		 * @note This function will change @a Item! Note that the item's @a m_Archive field will be set to `nullptr` if no errors occure.
		 **/
		auto Unregister(Arxx::Item * Item) -> void;
		
		/**
		 * @brief Sets a root Item.
		 * @param Item A pointer to an Arxx::Item that is to be set as the archive's root item or nullptr.
		 * @throw std::invalid_argument If the Item that @a pItem points to is not registered in the Archive.
		 * 
		 * This function will modify the @a m_RootItem member of the Archive setting it to @a Item.
		 * 
		 * You may pass @a Item `nullptr` to unset the root Item of the Archive.
		 **/
		auto SetRootItem(Arxx::Item * Item) -> void;
		
		/**
		 * @brief This function finds the item with a given item identifier.
		 * @param ItemIdentifier The identifier of the item which is to be found in the archive.
		 * 
		 * If the identifier is unknown in the archive, the function will return `nullptr`.
		 * 
		 * @note This function guarantees to be of complexity in O(log(n)) where n is the number of items in the archive.
		 **/
		auto GetItem(Arxx::u4byte ItemIdentifier) -> Arxx::Item *;
		
		/**
		 * @brief This function finds the item with a given item identifier.
		 * @param ItemIdentifier The identifier of the item which is to be found in the archive.
		 * 
		 * If the identifier is unknown in the archive this function will return `nullptr`.
		 * 
		 * @note This function guarantees to be of complexity in O(log(n)) where n is the number of items in the archive.
		 **/
		auto GetItem(Arxx::u4byte ItemIdentifier) const -> Arxx::Item const *;
		
		/**
		 * @brief Retrieves an item from the archive that is identified by a given path.
		 * @param Path A string with the path information.
		 * @return A pointer to an item that is identified by the path expression.
		 * 
		 * Based on the name attribute of items and the structural information, this function walks through the @a path trying to find the appropriate item, or returns `nullptr`.
		 * 
		 * @note Since item names do not need to be unique inside a relation, this function only finds the first item on the path.
		 * 
		 * The default relation to be searched is "child". However, the @a Path may contain information concerning the name of the relation that is to be searched for the appropriate item:
		 * @code /sub/one::name/down::another @endcode
		 * 
		 * In this path the first item name is "sub" and an item with that name is searched inside the "child" relation of the root item. The second item name is "name" which needs to be located inside the "one" relation of the item "/sub". The third item name is "another" which needs to be located inside the "down" relation.
		 **/
		auto GetItem(std::string Path) -> Arxx::Item *;
		
		/**
		 * @brief Retrieves a const item from the archive that is identified by a given path.
		 * @param Path A string with the path information.
		 * @return A pointer to an item that is identified by the path expression.
		 * 
		 * Based on the name attribute of items and the structural information this function walks through the @a Path trying to find the appropriate item, or returns `nullptr`.
		 * 
		 * @note Since item names do not need to be unique inside a relation, this function only finds the first item at the path.
		 * 
		 * The default relation to be searched is "child". However, the @a Path may contain information concerning the name of the relation that is to be searched for the appropriate item:
		 * @code /sub/one::name/down::another @endcode
		 * 
		 * In this path the first item name is "sub" and an item with that name is searched inside the "child" relation of the root item. The second item name is "name" which needs to be located inside the "one" relation of the item "/sub". The third item name is "another" which needs to be located inside the "down" relation.
		 **/
		auto GetItem(std::string Path) const -> Arxx::Item const *;
		
		/**
		 * @brief Provides access to the root item.
		 * 
		 * This function returns the m_RootItem member of the archive, so if the archive has no root item set, the function will return `nullptr`.
		 **/
		auto GetRootItem() -> Arxx::Item *;
		
		/**
		 * @brief Provides const access to the root item.
		 * 
		 * This function returns the m_RootItem member of the archive, so if the archive has no root item set, the function will return `nullptr`.
		 **/
		auto GetRootItem() const -> Arxx::Item const *;
		
		/**
		 * @brief Returns a Arxx::Reference for a given unique ID.
		 * @param ItemIdentifier The item identifier which you will get an Arxx::Reference for.
		 * 
		 * If a Arxx::Item object with the given unique ID is registered in the Archive the Reference will be resolved. If no such Item is registered the Reference will be unresolved, but will be linked to the Archive, so that, once an Item with that unique ID is registered at the Archive, the Reference will be resolved.
		 **/
		auto GetReference(Arxx::u4byte ItemIdentifier) -> Arxx::Reference;
		
		/**
		 * @brief Returns a Arxx::Archive::iterator to the first Item.
		 * 
		 * This function is provided so you can iterate through all the Item objects registered in an Archive. It follows STL standards.
		 **/
		auto begin() -> Arxx::Archive::iterator;
		
		/**
		 * @brief Returns a Arxx::Archive::iterator behind the last Item.
		 * 
		 * This function is provided so you can iterate through all the Item objects registered in an Archive. It follows STL standards.
		 **/
		auto end() -> Arxx::Archive::iterator;
		
		/**
		 * @brief Returns a Arxx::Archive::const_iterator to the first Item.
		 * 
		 * This function is provided so you can iterate through all the Item objects registered in a const Archive. It follows STL standards.
		 **/
		auto begin() const -> Arxx::Archive::const_iterator;
		
		/**
		 * @brief Returns a Arxx::Archive::const_iterator behind the last Item.
		 * 
		 * This function is provided so you can iterate through all the Item objects registered in a const Archive. It follows STL standards.
		 **/
		auto end() const -> Arxx::Archive::const_iterator;
		
		/**
		 * @brief Returns the number of Item objects.
		 * 
		 * This function returns the number of Arxx::Item objects registered in the Archive. It is designed to be STL compatible.
		 **/
		auto size() const -> Arxx::Archive::size_type;
		
		/**
		 * @brief Returns the number of resolved and unresolved references in the Archive.
		 **/
		auto GetNumberOfReferences() const -> Arxx::u4byte;
		
		/**
		 * @brief This function releases a Reference inside the Archive.
		 * 
		 * This is an internal function, but as the API user will never lay hands on a Arxx::ReferenceImplementation object it is still public to avoid friend declarations.
		 * 
		 * This function is called whenever the reference count of an unresolved Arxx::ReferenceImplementation object reaches `1`, because in that case the associated Archive object is the only reference holder of the ReferenceImplementation object. Since unresolved References serve no purpose other than to be resolved magically there is no sense in keeping it only inside the Archive.
		 * 
		 * This function will retrieve the associated Reference object and will remove it from the m_References map thus releasing the ReferenceImplementation object which then is deleted.
		 **/
		auto ReleaseReference(Arxx::ReferenceImplementation * Reference) -> void;
		
		/**
		 * @brief Fetch data from the archive file into the buffer.
		 * @param Offset The offset inside the archive file.
		 * @param Length The amount of data to read from the archive file.
		 * @param Buffer The buffer to fill with the data.
		 **/
		auto Fetch(Arxx::u4byte Offset, Arxx::u4byte Length, Arxx::Buffer * Buffer) -> bool;
        
	private:
		/**
		 * @brief The reader function that retrieves Item header information from a channel.
		 **/
		auto m_Read_2_1_0_0(Arxx::u4byte ItemCount) -> void;
		
		std::map<Arxx::u4byte, Arxx::Item *> m_Items;
		std::map<Arxx::u4byte, Arxx::Reference> m_References;
		Arxx::Item * m_RootItem{nullptr};
		
		/**
		 * @brief The archive file stream.
		 **/
		std::istream * m_IStream{nullptr};
        
	};
}

#endif
