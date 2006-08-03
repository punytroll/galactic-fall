#ifndef MAP_DIALOG_H
#define MAP_DIALOG_H

#include "clicked_listener.h"
#include "mouse_button_listener.h"
#include "widget.h"

class Button;
class Label;
class System;

class MapDialog : public ClickedListener, public MouseButtonListener, public Widget
{
public:
	MapDialog(Widget * SupWidget, System * System);
	virtual void Draw(void) const;
	System * GetSelectedSystem(void);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
private:
	System * m_System;
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	float m_Scale;
	System * m_SelectedSystem;
};

inline System * MapDialog::GetSelectedSystem(void)
{
	return m_SelectedSystem;
}

#endif
