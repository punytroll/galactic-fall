#ifndef STAR_H
#define STAR_H

#include "color.h"
#include "position.h"

class Star : public Position
{
public:
	Star(void);
	// getters
	const Color & GetColor(void) const;
	// setters
	void SetColor(const Color & Color);
private:
	Color m_Color;
};

inline const Color & Star::GetColor(void) const
{
	return m_Color;
}

inline void Star::SetColor(const Color & Color)
{
	m_Color = Color;
}

#endif
