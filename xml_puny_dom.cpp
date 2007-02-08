#include <stdexcept>
#include <iostream>
#include <stack>

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
