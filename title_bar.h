#ifndef TITLE_BAR_H
#define TITLE_BAR_H

#include "label.h"
#include "mouse_button_listener.h"
#include "mouse_motion_listener.h"

class TitleBar : public Label, public MouseButtonListener, public MouseMotionListener
{
public:
	TitleBar(Widget * SupWidget, const std::string & Title);
private:
	math3d::vector2f m_GrabPosition;
};

#endif
