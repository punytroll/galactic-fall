#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include "mouse_button_listener.h"
#include "widget.h"

class ClickedListener;

class Button : public Widget, public MouseButtonListener
{
public:
	Button(Widget * SupWidget);
	~Button(void);
	void AddClickedListener(ClickedListener * ClickedListener);
	// listener handlers
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
private:
	std::list< ClickedListener * > m_ClickedListeners;
};

#endif
