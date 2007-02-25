#ifndef MIND_H
#define MIND_H

#include "object.h"

class Character;
class Ship;

/**
 * A Mind is the aggregation of the internal state of a character.
 * The Mind is meant to be inherited by classes which implement the resulting behavior.
 **/

class Mind : public Object
{
public:
	void SetCharacter(Character * Character);
	void SetShip(Ship * Ship);
	virtual void Update(void) = 0;
protected:
	/// no internal state yet
	Character * m_Character;
	Ship * m_Ship;
};

inline void Mind::SetCharacter(Character * Character)
{
	m_Character = Character;
}

inline void Mind::SetShip(Ship * Ship)
{
	m_Ship = Ship;
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
