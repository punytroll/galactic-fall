#ifndef WINDOW_H
#define WINDOW_H

#include "mouse_button_listener.h"
#include "mouse_motion_listener.h"
#include "widget.h"

class Label;

class WWindow : virtual public MouseButtonListener, virtual public MouseMotionListener, public Widget
{
public:
	WWindow(Widget * SupWidget, const std::string & Title);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
	virtual void OnMouseMotion(Widget * EventSource, float X, float Y);
private:
	Label * m_TitleLabel;
	math3d::vector2f m_GrabPosition;
};

#endif
