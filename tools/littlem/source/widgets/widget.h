#ifndef LITTLEM_WIDGETS_WIDGET_H
#define LITTLEM_WIDGETS_WIDGET_H

#include "boxes.h"
#include "key_acceptor.h"

class Color;

class Widget : public KeyAcceptor, public Boxes::Box
{
public:
	Widget(void);
	virtual ~Widget(void);
	virtual void vDraw(void);
	void vDelete(void);
	void vSetBackgroundColor(const Color & BackgroundColor);
	bool bAcceptMouseButtons(int iButton, int iState, float fX, float fY);
	// virtual signals
	virtual bool bOnMouseButton(int iButton, int iState, float fX, float fY);
private:
	Color * m_pBackgroundColor;
};

#endif
