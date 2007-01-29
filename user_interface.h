#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>

#include "destroy_listener.h"

class Widget;

class UserInterface : public DestroyListener
{
public:
	UserInterface(void);
	void Draw(void) const;
	bool MouseButton(int Button, int State, float X, float Y);
	bool Key(int Key, int State);
	void MouseMotion(float X, float Y);
	// setters
	void SetCaptureWidget(Widget * Widget);
	void ReleaseCaptureWidget(void);
	// getters
	Widget * GetWidget(const std::string & Path);
	Widget * GetRootWidget(void);
	Widget * GetCaptureWidget(void);
	// virtual event handlers
	virtual void OnDestroy(Widget * EventSource);
private:
	Widget * m_RootWidget;
	Widget * m_HoverWidget;
	Widget * m_CaptureWidget;
};

inline Widget * UserInterface::GetRootWidget(void)
{
	return m_RootWidget;
}

inline Widget * UserInterface::GetCaptureWidget(void)
{
	return m_CaptureWidget;
}

#endif
