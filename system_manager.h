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
	const std::map< std::string, System * > & GetSystems(void) const;
	System * Get(const std::string & Identifier);
	System * Create(const std::string & Identifier);
	void Destroy(const std::string & Identifier);
	CommodityManager * GetCommodityManager(void);
private:
	CommodityManager * m_CommodityManager;
	std::map< std::string, System * > m_Systems;
};

inline const std::map< std::string, System * > & SystemManager::GetSystems(void) const
{
	return m_Systems;
}

inline CommodityManager * SystemManager::GetCommodityManager(void)
{
	return m_CommodityManager;
}

#endif
