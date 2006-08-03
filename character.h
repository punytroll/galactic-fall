#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
public:
	void AddCredits(const float & Credits);
	bool RemoveCredits(const float & Credits);
	const float & GetCredits(void) const;
private:
	float m_Credits;
};

inline const float & Character::GetCredits(void) const
{
	return m_Credits;
}

#endif
