#include <fstream>

#include "model.h"
#include "model_manager.h"
#include "string_cast.h"
#include "xml_puny_dom.h"

ModelManager::~ModelManager(void)
{
	while(m_Models.size() > 0)
	{
		Destroy(m_Models.begin()->first);
	}
}

Model * ModelManager::Get(const std::string & Identifier)
{
	std::map< std::string, Model * >::iterator ModelIterator(m_Models.find(Identifier));
	
	if(ModelIterator == m_Models.end())
	{
		return 0;
	}
	else
	{
		return ModelIterator->second;
	}
}

Model * ModelManager::Create(const std::string & Identifier)
{
	if(m_Models.find(Identifier) != m_Models.end())
	{
		return 0;
	}
	else
	{
		Model * NewModel(new Model(Identifier));
		
		m_Models[NewModel->GetIdentifier()] = NewModel;
		
		return NewModel;
	}
}

void ModelManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, Model * >::iterator ModelIterator(m_Models.find(Identifier));
	
	if(ModelIterator != m_Models.end())
	{
		delete ModelIterator->second;
		m_Models.erase(ModelIterator);
	}
}
