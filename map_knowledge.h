#ifndef MAP_KNOWLEDGE_H
#define MAP_KNOWLEDGE_H

#include <set>

class System;

class MapKnowledge
{
public:
	void AddExploredSystem(System * ExploredSystem);
	const std::set< System * > GetExploredSystems(void) const;
	const std::set< System * > GetUnexploredSystems(void) const;
private:
	std::set< System * > m_ExploredSystems;
	std::set< System * > m_UnexploredSystems;
};

inline const std::set< System * > MapKnowledge::GetExploredSystems(void) const
{
	return m_ExploredSystems;
}

inline const std::set< System * > MapKnowledge::GetUnexploredSystems(void) const
{
	return m_UnexploredSystems;
}

#endif
