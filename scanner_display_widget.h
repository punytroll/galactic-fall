#ifndef SCANNER_DISPLAY_WIDGET_H
#define SCANNER_DISPLAY_WIDGET_H

#include "camera.h"
#include "perspective.h"
#include "widget.h"

class Ship;

class ScannerDisplayWidget : public Widget
{
public:
	ScannerDisplayWidget(void);
	// setters
	void SetFocus(Ship * Ship);
	// actors
	virtual void Draw(void) const;
private:
	Ship * m_Focus;
	mutable Camera m_Camera;
	mutable Perspective m_Perspective;
};

#endif
