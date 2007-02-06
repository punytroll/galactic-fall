#ifndef COMMODITY_MANAGER_H
#define COMMODITY_MANAGER_H

#include <map>
#include <string>

class Element;
class Commodity;

class CommodityManager
{
public:
	~CommodityManager(void);
	Commodity * Get(const std::string & Identifier);
	Commodity * Create(const std::string & Identifier);
	void Destroy(const std::string & Identifier);
private:
	std::map< std::string, Commodity * > m_Commodities;
};

#endif
