#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <ostream>
#include <set>
#include <string>

class XMLStream;

class Object
{
public:
	Object(void);
	virtual ~Object(void);
	void SetObjectIdentifier(const std::string & ObjectIdentifier);
	void GenerateObjectIdentifier(void);
	const std::string & GetObjectIdentifier(void) const;
	
	static void Dump(std::ostream & OStream);
	static void Dump(XMLStream & XML);
private:
	std::string m_ObjectIdentifier;
	
	static std::set< Object * > m_Objects;
	static std::map< std::string, Object * > m_IdentifiedObjects;
};

inline const std::string & Object::GetObjectIdentifier(void) const
{
	return m_ObjectIdentifier;
}

#endif
