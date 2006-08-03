#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <map>
#include <string>

class CommodityManager;
class Element;
class System;

class SystemManager
{
public:
	SystemManager(CommodityManager * CommodityManager);
	~SystemManager(void);
	System * Get(const std::string & Identifier);
	System * Create(const std::string & Identifier);
	void Destroy(const std::string & Identifier);
	CommodityManager * GetCommodityManager(void);
private:
	CommodityManager * m_CommodityManager;
	std::map< std::string, System * > m_Systems;
};

inline CommodityManager * SystemManager::GetCommodityManager(void)
{
	return m_CommodityManager;
}

void LoadSystemFromElement(SystemManager * SystemManager, const Element * SystemElement);
void LoadSystemsFromFile(SystemManager * SystemManager, const std::string & FileName);
void LoadSystemLinkFromElement(SystemManager * SystemManager, const Element * SystemLinkElement);
void LoadSystemLinksFromFile(SystemManager * SystemManager, const std::string & FileName);

#endif
