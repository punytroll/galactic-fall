#include "map_knowledge.h"
#include "system.h"

void MapKnowledge::AddExploredSystem(System * ExploredSystem)
{
	m_ExploredSystems.insert(ExploredSystem);
	
	std::set< System * >::iterator UnexploredSytemIterator(m_UnexploredSystems.find(ExploredSystem));
	
	if(UnexploredSytemIterator != m_UnexploredSystems.end())
	{
		m_UnexploredSystems.erase(UnexploredSytemIterator);
	}
	
	const std::list< System * > & LinkedSystems(ExploredSystem->GetLinkedSystems());
	
	for(std::list< System * >::const_iterator LinkedSystemIterator = LinkedSystems.begin(); LinkedSystemIterator != LinkedSystems.end(); ++LinkedSystemIterator)
	{
		if(m_ExploredSystems.find(*LinkedSystemIterator)  == m_ExploredSystems.end())
		{
			m_UnexploredSystems.insert(*LinkedSystemIterator);
		}
	}
}
