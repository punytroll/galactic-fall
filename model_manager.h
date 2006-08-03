#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include <map>
#include <string>

class Element;
class Model;

class ModelManager
{
public:
	~ModelManager(void);
	Model * Get(const std::string & Identifier);
	Model * Create(const std::string & Identifier);
	void Destroy(const std::string & Identifier);
private:
	std::map< std::string, Model * > m_Models;
};

void LoadModelFromElement(ModelManager * ModelManager, const Element * ModelElement);
void LoadModelsFromFile(ModelManager * ModelManager, const std::string & FileName);

#endif
