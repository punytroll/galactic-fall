#include "object.h"

void Object::vSetName(const std::string & sName)
{
	m_sName = sName;
}

const std::string & Object::sGetName(void) const
{
	return m_sName;
}

std::string & Object::sGetNameToModify(void)
{
	return m_sName;
}
