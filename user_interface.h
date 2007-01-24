#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

class Widget;

class UserInterface
{
public:
	UserInterface(void);
	void Draw(void) const;
	bool MouseButton(int Button, int State, float X, float Y);
	bool Key(int Key, int State);
	void MouseMotion(float X, float Y);
	Widget * GetRootWidget(void);
private:
	Widget * m_RootWidget;
	Widget * m_HoverWidget;
};

inline Widget * UserInterface::GetRootWidget(void)
{
	return m_RootWidget;
}

#endif
