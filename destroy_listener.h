#ifndef DESTROY_LISTENER_H
#define DESTROY_LISTENER_H

class Widget;

class DestroyListener
{
public:
	virtual ~DestroyListener(void);
	virtual void OnDestroy(Widget * EventSource);
};

#endif
