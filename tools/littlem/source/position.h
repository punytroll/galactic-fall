#ifndef LITTLEM_POSITION_H
#define LITTLEM_POSITION_H

#include <algebra/vector4f.h>

#include "object.h"

class Position : public Object
{
public:
	Position(void);
	Position(float fX, float fY, float fZ);
	Position(const Position & Position);
	virtual ~Position(void);
	virtual void vDraw(void);
	virtual void vDrawSelection(void);
	Vector4f m_Position;
};

#endif
