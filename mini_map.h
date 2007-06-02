#ifndef MINI_MAP_H
#define MINI_MAP_H

#include "camera.h"
#include "perspective.h"
#include "widget.h"

class Ship;

class MiniMap : public Widget
{
public:
	MiniMap(void);
	void SetFocus(Ship * Ship);
	virtual void Draw(void) const;
private:
	Ship * m_Focus;
	Camera m_Camera;
	Perspective m_Perspective;
};

#endif
