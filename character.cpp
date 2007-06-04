#include "character.h"
#include "map_knowledge.h"
#include "mind.h"

std::set< Character * > Character::m_Characters;

Character::Character(void) :
	m_Credits(0.0f),
	m_MapKnowledge(new MapKnowledge()),
	m_Ship(0)
{
	m_Characters.insert(this);
}

Character::~Character(void)
{
	m_Characters.erase(m_Characters.find(this));
}

void Character::Update(void)
{
	if(m_Minds.size() > 0)
	{
		m_Minds.top()->Update();
	}
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

void Character::PossessByMind(Mind * Mind)
{
	m_Minds.push(Mind);
}

Mind * Character::ReleaseMind(void)
{
	Mind * Result(m_Minds.top());
	
	m_Minds.pop();
	
	return Result;
}

CharacterWithShip::CharacterWithShip(const Ship * Ship) :
	m_Ship(Ship)
{
}
	
bool CharacterWithShip::operator()(const Character * Character) const
{
	return m_Ship == Character->GetShip();
}
