#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

class Widget;

class UserInterface
{
public:
	UserInterface(void);
	void Draw(void) const;
	bool MouseButton(int Button, int State, float X, float Y);
	Widget * GetRootWidget(void);
private:
	Widget * m_RootWidget;
};

inline Widget * UserInterface::GetRootWidget(void)
{
	return m_RootWidget;
}

#endif
