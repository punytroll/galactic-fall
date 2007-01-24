#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include "mouse_button_listener.h"
#include "mouse_motion_listener.h"
#include "widget.h"

class ClickedListener;

class Button : public Widget, public MouseButtonListener, public MouseMotionListener
{
public:
	Button(Widget * SupWidget);
	~Button(void);
	void AddClickedListener(ClickedListener * ClickedListener);
	// listener handlers
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
private:
	std::list< ClickedListener * > m_ClickedListeners;
};

#endif
