#ifndef SHIP_CLASS_MANAGER_H
#define SHIP_CLASS_MANAGER_H

#include <map>
#include <string>

class Element;
class ModelManager;
class ShipClass;

class ShipClassManager
{
public:
	ShipClassManager(ModelManager * ModelManager);
	~ShipClassManager(void);
	ModelManager * GetModelManager(void);
	ShipClass * Get(const std::string & Identifier);
	ShipClass * Create(const std::string & Identifier);
	void Destroy(const std::string & Identifier);
private:
	ModelManager * m_ModelManager;
	std::map< std::string, ShipClass * > m_ShipClasses;
};

inline ModelManager * ShipClassManager::GetModelManager(void)
{
	return m_ModelManager;
}

#endif
