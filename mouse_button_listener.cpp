#include "mouse_button_listener.h"

MouseButtonListener::~MouseButtonListener(void)
{
}

bool MouseButtonListener::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	return false;
}
