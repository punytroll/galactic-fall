#ifndef MAP_DIALOG_H
#define MAP_DIALOG_H

#include "clicked_listener.h"
#include "key_listener.h"
#include "mouse_button_listener.h"
#include "mouse_motion_listener.h"
#include "widget.h"

class Button;
class Label;
class System;

class MapDialog : public ClickedListener, public KeyListener, public MouseButtonListener, public MouseMotionListener, public Widget
{
public:
	MapDialog(Widget * SupWidget, System * System);
	virtual void Draw(void) const;
	System * GetSelectedSystem(void);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
	virtual void OnMouseMotion(Widget * EventSource, float X, float Y);
private:
	void DrawSystem(const System * DrawSystem) const;
	System * m_System;
	Label * m_TitleLabel;
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	float m_Scale;
	System * m_SelectedSystem;
	Widget * m_Grabbed;
	math3d::vector2f m_GrabPosition;
};

inline System * MapDialog::GetSelectedSystem(void)
{
	return m_SelectedSystem;
}

#endif
