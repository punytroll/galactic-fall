#ifndef WIDGET_H
#define WIDGET_H

#include <list>

#include <math3d/vector2f.h>

class Color;
class DestroyListener;
class KeyListener;
class MouseButtonListener;

class Widget
{
public:
	Widget(Widget * SupWidget);
	virtual ~Widget(void);
	virtual void Draw(void) const;
	void Hide(void);
	void Show(void);
	void SetBackgroundColor(const Color & BackgroundColor);
	void SetPosition(const math3d::vector2f & Position);
	void SetSize(const math3d::vector2f & Size);
	void AddSubWidget(Widget * SubWidget);
	void RemoveSubWidget(Widget * SubWidget);
	void Destroy(void);
	void SetKeyFocus(Widget * KeyFocus);
	void GrabKeyFocus(void);
	// getters
	const Color & GetBackgroundColor(void) const;
	const math3d::vector2f GetPosition(void) const;
	const math3d::vector2f GetSize(void) const;
	Widget * GetRootWidget(void);
	const Widget * GetRootWidget(void) const;
	Widget * GetKeyFocus(void);
	bool IsVisible(void) const;
	// receive input
	bool MouseButton(int Button, int State, float X, float Y);
	bool Key(int Key, int State);
	// signal listeners
	void AddDestroyListener(DestroyListener * DestroyListener);
	void AddKeyListener(KeyListener * KeyListener);
	void AddMouseButtonListener(MouseButtonListener * MouseButtonListener);
	// static manager functions
	static std::list< Widget * > & GetDestroyedWidgets(void);
private:
	Widget * m_SupWidget;
	Color * m_BackgroundColor;
	math3d::vector2f m_Position;
	math3d::vector2f m_Size;
	std::list< Widget * > m_SubWidgets;
	bool m_Visible;
	Widget * m_KeyFocus;
	// listeners
	std::list< DestroyListener * > m_DestroyListeners;
	std::list< KeyListener * > m_KeyListeners;
	std::list< MouseButtonListener * > m_MouseButtonListeners;
	// static manager properties
	static std::list< Widget * > m_DestroyedWidgets;
};

inline const Color & Widget::GetBackgroundColor(void) const
{
	return *m_BackgroundColor;
}

inline const math3d::vector2f Widget::GetPosition(void) const
{
	return m_Position;
}

inline const math3d::vector2f Widget::GetSize(void) const
{
	return m_Size;
}

inline std::list< Widget * > & Widget::GetDestroyedWidgets(void)
{
	return m_DestroyedWidgets;
}

inline Widget * Widget::GetRootWidget(void)
{
	Widget * SupWidget(this);
	
	while(SupWidget->m_SupWidget != 0)
	{
		SupWidget = SupWidget->m_SupWidget;
	}
	
	return SupWidget;
}

inline const Widget * Widget::GetRootWidget(void) const
{
	const Widget * SupWidget(this);
	
	while(SupWidget->m_SupWidget != 0)
	{
		SupWidget = SupWidget->m_SupWidget;
	}
	
	return SupWidget;
}

inline Widget * Widget::GetKeyFocus(void)
{
	return m_KeyFocus;
}

inline bool Widget::IsVisible(void) const
{
	return m_Visible;
}

#endif
