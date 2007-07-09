/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#ifndef XML_PUNY_DOM_H
#define XML_PUNY_DOM_H

#include <istream>
#include <vector>
#include <map>
#include <string>

class Document;

class Element
{
public:
	Element(Document * Document, Element * Parent, const std::string & Name, const std::map< std::string, std::string > & Attributes);
	const Element * GetParent(void) const;
	const std::string & GetName(void) const;
	const std::map< std::string, std::string > & GetAttributes(void) const;
	const std::string & GetAttribute(const std::string & AttributeName) const;
	bool HasAttribute(const std::string & AttributeName) const;
	const std::vector< Element * > & GetChilds(void) const;
	const Element * GetChild(std::vector< Element * >::size_type Index) const;
private:
	Document * m_Document;
	Element * m_Parent;
	std::string m_Name;
	std::map< std::string, std::string > m_Attributes;
	std::vector< Element * > m_Childs;
};

class Document
{
public:
	Document(std::istream & Stream);
	const Element * GetDocumentElement(void) const;
private:
	Element * m_DocumentElement;
};

inline const Element * Document::GetDocumentElement(void) const
{
	return m_DocumentElement;
}

#endif
