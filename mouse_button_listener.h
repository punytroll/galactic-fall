#ifndef MOUSE_BUTTON_LISTENER_H
#define MOUSE_BUTTON_LISTENER_H

class Widget;

class MouseButtonListener
{
public:
	virtual ~MouseButtonListener(void);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
};

#endif
