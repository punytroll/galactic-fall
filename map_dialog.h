#ifndef MAP_DIALOG_H
#define MAP_DIALOG_H

#include "clicked_listener.h"
#include "key_listener.h"
#include "mouse_button_listener.h"
#include "window.h"

class Button;
class Character;
class Label;
class System;

class MapDialog : public ClickedListener, public KeyListener, virtual public MouseButtonListener, public WWindow
{
public:
	MapDialog(Widget * SupWidget, System * System, Character * Character);
	virtual void Draw(void) const;
	System * GetSelectedSystem(void);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
private:
	void DrawSystem(const System * DrawSystem) const;
	System * m_System;
	Character * m_Character;
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
