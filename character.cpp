#include "character.h"

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
