#ifndef CHARACTER_H
#define CHARACTER_H

class MapKnowledge;

class Character
{
public:
	Character(void);
	// getters
	const float & GetCredits(void) const;
	MapKnowledge * GetMapKnowledge(void);
	// setters
	void SetCredits(float Credits);
	// modifiers
	void AddCredits(const float & Credits);
	bool RemoveCredits(const float & Credits);
private:
	MapKnowledge * m_MapKnowledge;
	float m_Credits;
};

inline const float & Character::GetCredits(void) const
{
	return m_Credits;
}

inline void Character::SetCredits(float Credits)
{
	m_Credits = Credits;
}

inline MapKnowledge * Character::GetMapKnowledge(void)
{
	return m_MapKnowledge;
}

#endif
