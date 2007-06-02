#ifndef SCANNER_DISPLAY_H
#define SCANNER_DISPLAY_H

#include "camera.h"
#include "perspective.h"
#include "widget.h"

class Ship;

class ScannerDisplay : public Widget
{
public:
	ScannerDisplay(void);
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
