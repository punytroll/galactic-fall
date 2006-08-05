#include "character.h"
#include "map_knowledge.h"

Character::Character(void) :
	m_MapKnowledge(new MapKnowledge())
{
}

void Character::AddCredits(const float & Credits)
{
	m_Credits += Credits;
}

bool Character::RemoveCredits(const float & Credits)
{
	if(m_Credits < Credits)
	{
		return false;
	}
	else
	{
		m_Credits -= Credits;
		
		return true;
	}
}
