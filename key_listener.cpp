#include "key_listener.h"

KeyListener::~KeyListener(void)
{
}

bool KeyListener::OnKey(Widget * EventSource, int Key, int State)
{
	return false;
}
