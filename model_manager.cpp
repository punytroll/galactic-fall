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

void LoadModelFromElement(ModelManager * ModelManager, const Element * ModelElement)
{
	Model * NewModel(ModelManager->Create(ModelElement->GetAttribute("identifier")));
	
	const std::vector< Element * > & Childs(ModelElement->GetChilds());
	std::vector< Element * >::const_iterator ChildIterator(Childs.begin());
	std::map< std::string, std::vector< math3d::vector4f >::size_type > Points;
	std::map< std::string, std::pair< std::vector< math3d::vector4f >::size_type, math3d::vector4f > > TrianglePoints;
	
	while(ChildIterator != Childs.end())
	{
		Element * Child(*ChildIterator);
		
		if(Child->GetName() == "point")
		{
			Points[Child->GetAttribute("identifier")] = NewModel->AddPoint(math3d::vector4f(string_cast< float >(Child->GetAttribute("position-x")), string_cast< float >(Child->GetAttribute("position-y")), string_cast< float >(Child->GetAttribute("position-z")), 0.0f));
		}
		else if(Child->GetName() == "triangle-point")
		{
			TrianglePoints[Child->GetAttribute("identifier")] = std::make_pair(Points[Child->GetChilds().front()->GetAttribute("point-identifier")], math3d::vector4f(string_cast< float >(Child->GetAttribute("normal-x")), string_cast< float >(Child->GetAttribute("normal-y")), string_cast< float >(Child->GetAttribute("normal-z")), 0.0f));
		}
		else if(Child->GetName() == "triangle")
		{
			const std::pair< std::vector< math3d::vector4f >::size_type, math3d::vector4f > TrianglePoint1(TrianglePoints.find(Child->GetChild(0)->GetAttribute("triangle-point-identifier"))->second);
			const std::pair< std::vector< math3d::vector4f >::size_type, math3d::vector4f > TrianglePoint2(TrianglePoints.find(Child->GetChild(1)->GetAttribute("triangle-point-identifier"))->second);
			const std::pair< std::vector< math3d::vector4f >::size_type, math3d::vector4f > TrianglePoint3(TrianglePoints.find(Child->GetChild(2)->GetAttribute("triangle-point-identifier"))->second);
			
			NewModel->AddTriangle(TrianglePoint1.first, TrianglePoint1.second, TrianglePoint2.first, TrianglePoint2.second, TrianglePoint3.first, TrianglePoint3.second);
		}
		++ChildIterator;
	}
}

void LoadModelsFromFile(ModelManager * ModelManager, const std::string & FileName)
{
	std::ifstream FileStream(FileName.c_str());
	
	if(FileStream)
	{
		Document Document(FileStream);
		const Element * RootElement(Document.GetRootElement());
		
		if(RootElement == 0)
		{
			return;
		}
		
		const std::vector< Element * > & RootElementChilds(RootElement->GetChilds());
		std::vector< Element * >::const_iterator RootElementChild(RootElementChilds.begin());
		
		while(RootElementChild != RootElementChilds.end())
		{
			if((*RootElementChild)->GetName() == "model")
			{
				LoadModelFromElement(ModelManager, *RootElementChild);
			}
			++RootElementChild;
		}
	}
}
