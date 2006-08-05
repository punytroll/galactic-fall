#ifndef CHARACTER_H
#define CHARACTER_H

class MapKnowledge;

class Character
{
public:
	Character(void);
	void AddCredits(const float & Credits);
	bool RemoveCredits(const float & Credits);
	const float & GetCredits(void) const;
	MapKnowledge * GetMapKnowledge(void);
private:
	float m_Credits;
	MapKnowledge * m_MapKnowledge;
};

inline const float & Character::GetCredits(void) const
{
	return m_Credits;
}

inline MapKnowledge * Character::GetMapKnowledge(void)
{
	return m_MapKnowledge;
}

#endif
