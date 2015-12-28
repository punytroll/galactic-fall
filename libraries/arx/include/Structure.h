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

#ifndef ARXX_STRUCTURE_H
#define ARXX_STRUCTURE_H

#include <list>
#include <map>
#include <vector>

#include "Common.h"
#include "Reference.h"

/**
 * @file Structure.h
 * 
 * This file declares the Structure class, the Structure::Relation class and iterator classes for both.
 **/

namespace Arxx
{
	class Archive;
	
	class Buffer;
	
	class Item;
	
	/**
	 * @brief The structure of a ARX archive is saved inside these objects.
	 * 
	 * This Structure class provides a general but standarized way to structure the items in an archive.
	 **/
	class Structure
	{
		friend class Arxx::Archive;
	public:
		/**
		 * @brief The object that holds references to other items.
		 * 
		 * A Relation is a container for Arxx::Reference objects, which are identified by the unique ID of the referenced Arxx::Item. A Relation is a multiset so that one Arxx::Item may be referenced multiple times.
		 **/
		class Relation
		{
			friend class Arxx::Structure;
		public:
			typedef std::multimap< Arxx::u4byte, Arxx::Reference >::size_type size_type;
			
			class const_iterator;
			
			/**
			 * @brief An iterator class defining basic operations to iterate the Reference objects in a Relation.
			 *
			 * This iterator class encapsules the internal form of Reference storage in a Relation. By using this iterator class, which is what you have to do, you are safe from changes in implementation detail as the container used to store Reference objects. Additionally it is the prefered way to make a visit to each Reference in a Relation.
			 **/
			class iterator
			{
				friend class Arxx::Structure::Relation::const_iterator;
			public:
				/**
				 * @brief The constructor of a Reference iterator.
				 * @param iReference The STL iterator from the Relation's aggregated Reference container. The internal representation of the iterator.
				 *
				 * This constructor creates a Reference iterator from an STL iterator.
				 **/
				iterator(std::multimap< Arxx::u4byte, Arxx::Reference >::iterator iReference);
				
				/**
				 * @brief The destructor of a Reference iterator.
				 **/
				~iterator(void);
				
				/**
				 * @brief Advances the iterator.
				 *
				 * Advances the Reference iterator. It is not defined which will be the next Reference the iterator points to. It is only assured that every Reference in the Relation will be passed once.
				 **/
				iterator & operator++(void);
				
				/**
				 * @brief This will return the Reference that this iterator points to.
				 * 
				 * This is the non-const version of the operator*() so it returns a non-const Reference.
				 **/
				Arxx::Reference & operator*(void);
				
				/**
				 * @brief This will return the Reference that this iterator points to.
				 * 
				 * This is the const version of the operator*() thus the returned Reference is const as well.
				 **/
				const Arxx::Reference & operator*(void) const;
				
				/**
				 * @brief The Reference accessor.
				 **/
				Arxx::Reference * operator->(void);
				
				/**
				 * @brief Tests two iterators for equality.
				 * @param iReference The iterator to check against.
				 **/
				bool operator==(const Arxx::Structure::Relation::iterator & iReference) const;
				
				/**
				 * @brief Tests two iterators for inequality.
				 * @param iReference The iterator to check against.
				 **/
				bool operator!=(const Arxx::Structure::Relation::iterator & iReference) const;
				
				/**
				 * @brief Tests two iterators for equality.
				 * @param iReference The iterator to check against.
				 **/
				bool operator==(const Arxx::Structure::Relation::const_iterator & iReference) const;
				
				/**
				 * @brief Tests two iterators for inequality.
				 * @param iReference The iterator to check against.
				 **/
				bool operator!=(const Arxx::Structure::Relation::const_iterator & iReference) const;
			private:
				/**
				 * @brief The internal STL iterator.
				 * 
				 * This is an iterator into the Relation's internal Reference map.
				 **/
				std::multimap< Arxx::u4byte, Arxx::Reference >::iterator m_iReference;
			};
			
			/**
			 * @brief An iterator class defining basic operations to iterate the Reference objects in a Relation.
			 *
			 * This iterator class encapsules the internal form of Reference storage in a Relation. By using this iterator class, which is what you have to do, you are safe from changes in implementation detail as the container used to store Reference objects. Additionally it is the prefered way to make a visit to each Reference in a Relation.
			 **/
			class const_iterator
			{
				friend class Arxx::Structure::Relation::iterator;
			public:
				/**
				 * @brief The constructor of a const Reference iterator.
				 * @param iReference The STL iterator from the Relation's aggregated Reference container. The internal representation of the iterator.
				 *
				 * This constructor creates a const Reference iterator from an STL iterator.
				 **/
				const_iterator(std::multimap< Arxx::u4byte, Arxx::Reference >::const_iterator iReference);
				
				/**
				 * @brief The constructor of a const Reference iterator from a non-const iterator.
				 * @param iIterator An iterator of a Reference.
				 *
				 * This constructor creates a const Reference iterator from a non-const Reference iterator.
				 **/
				const_iterator(Arxx::Structure::Relation::iterator iIterator);
				
				/**
				 * @brief The destructor of a const Reference iterator.
				 **/
				~const_iterator(void);
				
				/**
				 * @brief Advances the iterator.
				 *
				 * Advances the Reference iterator. It is not defined which will be the next Reference the iterator points to. It is only assured that every Reference in the Relation will be passed once.
				 **/
				const_iterator & operator++(void);
				
				/**
				 * @brief This will return the Arxx::Reference that this iterator points to.
				 * 
				 * This is the const version of the operator*() thus the returned Reference is const as well.
				 **/
				const Arxx::Reference & operator*(void) const;
				
				/**
				 * @brief The Reference accessor.
				 **/
				const Arxx::Reference * operator->(void);
				
				/**
				 * @brief Tests two iterators for equality.
				 * @param iReference The iterator to check against.
				 **/
				bool operator==(const Arxx::Structure::Relation::const_iterator & iReference) const;
				
				/**
				 * @brief Tests two iterators for inequality.
				 * @param iReference The iterator to check against.
				 **/
				bool operator!=(const Arxx::Structure::Relation::const_iterator & iReference) const;
			private:
				/**
				 * @brief The internal STL iterator.
				 * 
				 * This is an iterator into a Relation's internal Reference map.
				 **/
				std::multimap< Arxx::u4byte, Arxx::Reference >::const_iterator m_iReference;
			};
			
			/**
			 * @brief Adds a unique ID reference to the relation.
			 * @param u4UniqueID The unique ID that is to be added to the relation.
			 * 
			 * This function adds a reference to a unique ID to the relation.
			 **/
			void vAdd(Arxx::u4byte u4UniqueID);
			
			/**
			 * @brief Removes a reference with a certain unique ID from the relation.
			 * @param u4UniqueID The unique ID that is to be deleted from the relation.
			 * 
			 * This function removes a reference with a certain unique ID from the relation.
			 **/
			bool bDelete(Arxx::u4byte u4UniqueID);
			
			/**
			 * @brief Gets a list of pointers to the items with the specified name.
			 * @param Name The name of the items which are to be retrieved.
			 * @return All items in the relation with a the name @a Name.
			 * @note Should the relation contain an item with the name @a Name multiple times it will be returned multiple times.
			 * @note All unresolved item references will be skipped.
			 * 
			 * This function can be used to retrieve a pointers to items that are linked to in this relation.
			 * 
			 * Relations contain items without regard of their names. Therefore multiple items inside a relation may have the same name and all these items are returned.
			 **/
			std::list< Arxx::Item * > GetItems(const std::string & Name);
			
			/**
			 * @brief Returns the number of references in the relation.
			 **/
			size_type size(void) const;
			
			/**
			 * @brief Returns the first reference iterator of the relation.
			 **/
			Arxx::Structure::Relation::iterator begin(void);
			
			/**
			 * @brief Returns the end reference iterator of the relation.
			 **/
			Arxx::Structure::Relation::iterator end(void);
			
			/**
			 * @brief Returns the first reference iterator of the relation.
			 **/
			Arxx::Structure::Relation::const_iterator begin(void) const;
			
			/**
			 * @brief Returns the end reference iterator of the relation.
			 **/
			Arxx::Structure::Relation::const_iterator end(void) const;
			
			/**
			 * @brief Returns the name of the relation.
			 **/
			const std::string & sGetName(void) const;
		private:
			/**
			 * @brief A constructor that links a relation to a specific Arxx::Structure.
			 * @param Structure The structure object that this relation is linked to.
			 * @param sName The unique name of the relation inside the related Structure.
			 **/
			Relation(Arxx::Structure & Structure, const std::string & sName);
			
			/**
			 * @brief The related Structure object.
			 * 
			 * This is set by the constructor and cannot be changed. Every Relation object is strictly bound to exactly one Structure at creation time.
			 **/
			Arxx::Structure & m_Structure;
			
			/**
			 * @brief The Item references stored in the Relation.
			 **/
			std::multimap< Arxx::u4byte, Arxx::Reference > m_References;
			
			/**
			 * @brief The name of the relation.
			 **/
			std::string m_sName;
		};
		
		friend class Arxx::Structure::Relation;
		
		class const_iterator;
		
		/**
		 * @brief An iterator class defining basic operations to iterate the Relation objects in a Structure.
		 *
		 * This iterator class encapsules the internal form of Relation storage in a Structure. By using this iterator class, which is what you have to do, you are safe from changes in implementation detail as the container used to store Relation objects. Additionally it is the prefered way to make a visit to each Relation in a Structure.
		 **/
		class iterator
		{
			friend class Arxx::Structure::const_iterator;
		public:
			/**
			 * @brief The constructor of a Relation iterator.
			 * @param iRelation The STL iterator from the Structure's aggregated Relation container. The internal representation of the iterator.
			 *
			 * This constructor creates a Relation iterator from an STL iterator.
			 **/
			iterator(std::map< std::string, Arxx::Structure::Relation >::iterator iRelation);
			
			/**
			 * @brief The destructor of a Relation iterator.
			 **/
			~iterator(void);
			
			/**
			 * @brief Advances the iterator.
			 *
			 * Advances the Relation iterator. It is not defined which will be the next Relation the iterator points to. It is only assured that every Relation in the Structure will be passed once.
			 **/
			iterator & operator++(void);
			
			/**
			 * @brief This will return the Relation that this iterator points to.
			 * 
			 * This is the non-const version of the operator*() so it returns a non-const Relation.
			 **/
			Arxx::Structure::Relation & operator*(void);
			
			/**
			 * @brief This will return the Relation that this iterator points to.
			 * 
			 * This is the const version of the operator*() thus the returned Relation is const as well.
			 **/
			const Arxx::Structure::Relation & operator*(void) const;
			
			/**
			 * @brief The Relation accessor.
			 **/
			Arxx::Structure::Relation * operator->(void);
			
			/**
			 * @brief Tests two iterators for equality.
			 * @param iRelation The iterator to check against.
			 **/
			bool operator==(const Arxx::Structure::iterator & iRelation) const;
			
			/**
			 * @brief Tests two iterators for inequality.
			 * @param iRelation The iterator to check against.
			 **/
			bool operator!=(const Arxx::Structure::iterator & iRelation) const;
			
			/**
			 * @brief Tests two iterators for equality.
			 * @param iRelation The iterator to check against.
			 **/
			bool operator==(const Arxx::Structure::const_iterator & iRelation) const;
			
			/**
			 * @brief Tests two iterators for inequality.
			 * @param iRelation The iterator to check against.
			 **/
			bool operator!=(const Arxx::Structure::const_iterator & iRelation) const;
		private:
			/**
			 * @brief The internal STL iterator.
			 * 
			 * This is an iterator into the Structure's internal Relation map.
			 **/
			std::map< std::string, Arxx::Structure::Relation >::iterator m_iRelation;
		};
		
		/**
		 * @brief An iterator class defining basic operations to iterate the Relation objects in a Structure.
		 *
		 * This iterator class encapsules the internal form of Relation storage in a Structure. By using this iterator class, which is what you have to do, you are safe from changes in implementation detail as the container used to store Relation objects. Additionally it is the prefered way to make a visit to each Relation in a Structure.
		 **/
		class const_iterator
		{
			friend class Arxx::Structure::iterator;
		public:
			/**
			 * @brief The constructor of a const Relation iterator.
			 * @param iRelation The STL iterator from the Structure's aggregated Relation container. The internal representation of the iterator.
			 *
			 * This constructor creates a const Relation iterator from an STL iterator.
			 **/
			const_iterator(std::map< std::string, Arxx::Structure::Relation >::const_iterator iRelation);
			
			/**
			 * @brief The constructor of a const Relation iterator from a non-const Relation iterator.
			 * @param iRelation The Relation iterator from the Structure's aggregated Relation container.
			 *
			 * This constructor creates a const Relation iterator from a non-const Relation iterator.
			 **/
			const_iterator(Arxx::Structure::iterator iRelation);
			
			/**
			 * @brief The destructor of a const Relation iterator.
			 **/
			~const_iterator(void);
			
			/**
			 * @brief Advances the iterator.
			 *
			 * Advances the Relation iterator. It is not defined which will be the next Relation the iterator points to. It is only assured that every Relation in the Structure will be passed once.
			 **/
			const_iterator & operator++(void);
			
			/**
			 * @brief This will return the Arxx::Relation that this iterator points to.
			 * 
			 * This is the const version of the operator*() thus the returned Relation is const as well.
			 **/
			const Arxx::Structure::Relation & operator*(void) const;
			
			/**
			 * @brief The Relation accessor.
			 **/
			const Arxx::Structure::Relation * operator->(void);
			
			/**
			 * @brief Tests two iterators for equality.
			 * @param iRelation The iterator to check against.
			 **/
			bool operator==(const Arxx::Structure::const_iterator & iRelation) const;
			
			/**
			 * @brief Tests two iterators for inequality.
			 * @param iRelation The iterator to check against.
			 **/
			bool operator!=(const Arxx::Structure::const_iterator & iRelation) const;
		private:
			/**
			 * @brief The internal STL iterator.
			 * 
			 * This is an iterator into a Structure's internal Relation map.
			 **/
			std::map< std::string, Arxx::Structure::Relation >::const_iterator m_iRelation;
		};
		
		typedef std::map< std::string, Arxx::Structure::Relation >::size_type size_type;
		
		/**
		 * @brief This constructor binds the structure to a specific Arxx::Item.
		 * @param Item The Arxx::Item that this structure is linked to.
		 **/
		Structure(Arxx::Item & Item);
		
		/**
		 * @brief Adds an unresolved item reference to a relation.
		 * @param sRelation The relation that is changed with this call.
		 * @param u4UniqueID A unique ID that is to be added to the relation.
		 * @return A boolean value indicating whether the operation has been performed successfully.
		 * 
		 * This function will add the unique ID @a u4UniqueID to the relation identified by @a sRelation.
		 * 
		 * There are two things to consider:
		 * - The unique ID may not be invalid, that is equal to g_u4InvalidID.
		 * - You cannot change the "reference" relation with this function.
		 * 
		 * Violating one of these conditions will abort the call and return without having done anything.
		 * 
		 * This function will create the relation if it does not exist yet and the unresolved reference with the unique ID @a u4UniqueID will be the only member of this new relation.
		 **/
		bool bAdd(Arxx::u4byte u4UniqueID, const std::string & sRelation = "child");
		
		/**
		 * @brief Deletes an unresolved item reference from a relation.
		 * @param sRelation The relation that is changed with this call.
		 * @param u4UniqueID The unique item id that is tried to be deleted from the relation.
		 * @return A boolean value indicating whether the operation has been performed successfully.
		 * 
		 * This function will delete the unresolved item reference referring to an item with unique id u4UniqueID from the relation indicated by @a sRelation.
		 * 
		 * If the unresolved item reference is not found in the relation the function may issue a second search.
		 * -# Given the item that @a this Structure belongs to is registered at a library, the function will try to resolve the unique ID with Arxx::Library::pGetItem() (complexity in O(log(n))). If a resolved item reference to an item with this unique id is in the relation it is also found via pGetItem() and can and will be deleted from the relation. If pGetItem does not find the item there will be no such reference in the relation.
		 * -# Given the item that @a this Structure belongs to is not registered at a library, the function will iterate through the resolved item references in the relation trying to find one that refers to an item with this unique id.
		 * 
		 * There are two things to consider:
		 * - The unique ID may not be invalid, that is equal to g_u4InvalidID.
		 * - You can not modify the relation "reference" with this function.
		 * 
		 * Any atempt to do so as well as deleting from a non-existent relation will be silently ignored.
		 * 
		 * If the deleted item reference was the last one in the relation so that the relation @a sRelation is now empty it is deleted from the relations container.
		 **/
		bool bDelete(Arxx::u4byte u4UniqueID, const std::string & sRelation = "child");
		
		/**
		 * @brief The const accessor for constant relations.
		 * @param sRelation The relation's name.
		 * @throw std::out_of_range If a relation with the name @a sRelation does not exist in the structure.
		 * 
		 * This function returns a reference to a Arxx::Structure::Relation object which you can use to query the content of this relation.
		 **/
		const Arxx::Structure::Relation & GetRelation(const std::string & sRelation) const;
		
		/**
		 * @brief The accessor for relations.
		 * @param sRelation The relation's name.
		 * @throw std::out_of_range If a relation with the name @a sRelation does not exist in the structure.
		 * 
		 * This function returns a reference to a Arxx::Structure::Relation object which you can use to query and modify the content of this relation.
		 **/
		Arxx::Structure::Relation & GetRelation(const std::string & sRelation);
		
		/**
		 * @brief Query whether a paricular relation exists.
		 * @param sRelation The name of the relation.
		 **/
		bool bHasRelation(const std::string & sRelation) const;
		
		/**
		 * @brief Returns the number of Relations in the Structure.
		 **/
		size_type size(void) const;
		
		/**
		 * @brief Returns the first Relation iterator of the Structure.
		 **/
		Arxx::Structure::iterator begin(void);
		
		/**
		 * @brief Returns the end Relation iterator of the Structure.
		 **/
		Arxx::Structure::iterator end(void);
		
		/**
		 * @brief Returns the first const Relation iterator of the Structure.
		 **/
		Arxx::Structure::const_iterator begin(void) const;
		
		/**
		 * @brief Returns the end const Relation iterator of the Structure.
		 **/
		Arxx::Structure::const_iterator end(void) const;
	private:
		friend Arxx::Buffer & Arxx::operator<<(Arxx::Buffer & Buffer, const Arxx::Structure & Structure);
		
		/**
		 * @brief Writes the Structure information to a Buffer.
		 * @param Buffer The Buffer that will receive the structural information.
		 * 
		 * This function simply appends the structure information to the @a Buffer.
		 **/
		void vWriteToBuffer(Arxx::Buffer & Buffer) const;
		
		void vReadFromStream(Arxx::u4byte u4StructureDataLength, std::istream & IStream);
		
		
		void vRemoveRelation(Arxx::Structure::Relation * pRelation);
		Arxx::Item & m_Item;
		std::map< std::string, Arxx::Structure::Relation > m_Relations;
	};
	
	/**
	 * @brief Convenience function that streams the structural information into a Arxx::Buffer.
	 **/
	Arxx::Buffer & operator<<(Arxx::Buffer & Buffer, const Arxx::Structure & Structure);
}

#endif
