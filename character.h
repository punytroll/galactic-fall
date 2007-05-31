#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"

class MapKnowledge;
class Ship;

class Character : public Object
{
public:
	Character(void);
	// getters
	float GetCredits(void);
	MapKnowledge * GetMapKnowledge(void);
	Ship * GetShip(void);
	const Ship * GetShip(void) const;
	// setters
	void SetCredits(float Credits);
	void SetShip(Ship * Ship);
	// modifiers
	void AddCredits(const float & Credits);
	bool RemoveCredits(const float & Credits);
private:
	float m_Credits;
	MapKnowledge * m_MapKnowledge;
	Ship * m_Ship;
};

inline float Character::GetCredits(void)
{
	return m_Credits;
}

inline MapKnowledge * Character::GetMapKnowledge(void)
{
	return m_MapKnowledge;
}

inline Ship * Character::GetShip(void)
{
	return m_Ship;
}

inline const Ship * Character::GetShip(void) const
{
	return m_Ship;
}

inline void Character::SetCredits(float Credits)
{
	m_Credits = Credits;
}

inline void Character::SetShip(Ship * Ship)
{
	m_Ship = Ship;
}

#endif
