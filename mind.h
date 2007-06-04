#ifndef MIND_H
#define MIND_H

#include "object.h"

class Character;

/**
 * A Mind is the aggregation of the internal state of a character.
 * The Mind is meant to be inherited by classes which implement the resulting behavior.
 **/

class Mind : public Object
{
public:
	void SetCharacter(Character * Character);
	Character * GetCharacter(void);
	const Character * GetCharacter(void) const;
	virtual void Update(void) = 0;
protected:
	/// no internal state yet
	Character * m_Character;
};

inline void Mind::SetCharacter(Character * Character)
{
	m_Character = Character;
}

inline Character * Mind::GetCharacter(void)
{
	return m_Character;
}

inline const Character * Mind::GetCharacter(void) const
{
	return m_Character;
}

class StateMachine;

class StateMachineMind : public Mind
{
public:
	StateMachineMind(void);
	~StateMachineMind(void);
	virtual void Update(void);
	// getters
	StateMachine * GetStateMachine(void);
private:
	StateMachine * m_StateMachine;
};

inline StateMachine * StateMachineMind::GetStateMachine(void)
{
	return m_StateMachine;
}

#endif
