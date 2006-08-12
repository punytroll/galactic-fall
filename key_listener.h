#ifndef KEY_LISTENER_H
#define KEY_LISTENER_H

class Widget;

class KeyListener
{
public:
	virtual ~KeyListener(void);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
};

#endif
