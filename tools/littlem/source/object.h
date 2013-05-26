#ifndef LITTLEM_OBJECT_H
#define LITTLEM_OBJECT_H

#include <string>

class Object
{
public:
	void vSetName(const std::string & sName);
	const std::string & sGetName(void) const;
	std::string & sGetNameToModify(void);
private:
	std::string m_sName;
};

#endif
