#ifndef MOUSE_MOTION_LISTENER_H
#define MOUSE_MOTION_LISTENER_H

class Widget;

class MouseMotionListener
{
public:
	virtual ~MouseMotionListener(void);
	virtual void OnMouseMotion(Widget * EventSource, float X, float Y);
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
};

#endif
