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

#include <assert.h>

#include <algorithm>
#include <iostream>
#include <stack>
#include <stdexcept>

#include "xml_parser.h"
#include "xml_puny_dom.h"

class DOMReader : public XMLParser
{
public:
	DOMReader(std::istream & Stream, Document * Document, Element ** RootElement);
	virtual void ElementStart(const std::string & Name, const std::map< std::string, std::string > & Attributes);
	virtual void ElementEnd(const std::string & Name);
private:
	Document * m_Document;
	Element ** m_RootElement;
	std::stack< Element * > m_ElementStack;
};

DOMReader::DOMReader(std::istream & Stream, Document * Document, Element ** RootElement) :
	XMLParser(Stream),
	m_Document(Document),
	m_RootElement(RootElement)
{
}

void DOMReader::ElementStart(const std::string & Name, const std::map< std::string, std::string > & Attributes)
{
	if(*m_RootElement == 0)
	{
		// we've got the root element
		m_ElementStack.push(*m_RootElement = new Element(m_Document, 0, Name, Attributes));
	}
	else
	{
		m_ElementStack.push(new Element(m_Document, m_ElementStack.top(), Name, Attributes));
	}
}

void DOMReader::ElementEnd(const std::string & Name)
{
	if(m_ElementStack.size() == 0)
	{
		std::cerr << "Got '/" << Name << "' but stack is empty." << std::endl;
		
		throw std::domain_error(Name);
	}
	if(Name != m_ElementStack.top()->GetName())
	{
		std::cerr << "Got '/" << Name << "' but expected '/" << m_ElementStack.top()->GetName() << "'." << std::endl;
		
		throw std::domain_error(Name);
	}
	m_ElementStack.pop();
}

Element::Element(Document * Document, Element * Parent, const std::string & Name, const std::map< std::string, std::string > & Attributes) :
	m_Document(Document),
	m_Parent(Parent),
	m_Name(Name),
	m_Attributes(Attributes)
{
	if(m_Parent != 0)
	{
		m_Parent->m_Childs.push_back(this);
	}
}

Element::~Element(void)
{
	// delete it from the parent's child vector
	if(m_Parent != 0)
	{
		std::vector< Element * >::iterator ParentsChildIterator(std::find(m_Parent->m_Childs.begin(), m_Parent->m_Childs.end(), this));
		
		assert(ParentsChildIterator != m_Parent->m_Childs.end());
		m_Parent->m_Childs.erase(ParentsChildIterator);
		m_Parent = 0;
	}
	while(m_Childs.empty() == false)
	{
		// erasing the child from the m_Childs vector will happen in the childs destructor
		delete m_Childs.front();
	}
	m_Document = 0;
}

const std::string & Element::GetName(void) const
{
	return m_Name;
}

const Element * Element::GetParent(void) const
{
	return m_Parent;
}

const std::map< std::string, std::string > & Element::GetAttributes(void) const
{
	return m_Attributes;
}

const std::string & Element::GetAttribute(const std::string & AttributeName) const
{
	return m_Attributes.find(AttributeName)->second;
}

bool Element::HasAttribute(const std::string & AttributeName) const
{
	return m_Attributes.find(AttributeName) != m_Attributes.end();
}

const std::vector< Element * > & Element::GetChilds(void) const
{
	return m_Childs;
}

const Element * Element::GetChild(std::vector< Element * >::size_type Index) const
{
	return m_Childs[Index];
}

Document::Document(std::istream & Stream) :
	m_DocumentElement(0)
{
	DOMReader DOMReader(Stream, this, &m_DocumentElement);
	
	DOMReader.Parse();
}

Document::~Document(void)
{
	delete m_DocumentElement;
	m_DocumentElement = 0;
}
