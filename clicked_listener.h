#ifndef CLICKED_LISTENER_H
#define CLICKED_LISTENER_H

class Widget;

class ClickedListener
{
public:
	virtual ~ClickedListener(void);
	virtual bool OnClicked(Widget * EventSource);
};

#endif
