#ifndef WIDGET_H
#define WIDGET_H

#include <list>

#include <math3d/vector2f.h>

class Color;
class DestroyListener;
class MouseButtonListener;

class Widget
{
public:
	Widget(Widget * SupWidget);
	virtual ~Widget(void);
	virtual void Draw(void) const;
	void SetBackgroundColor(const Color & BackgroundColor);
	void SetPosition(const math3d::vector2f & Position);
	void SetSize(const math3d::vector2f & Size);
	const Color & GetBackgroundColor(void) const;
	const math3d::vector2f GetPosition(void) const;
	const math3d::vector2f GetSize(void) const;
	void AddSubWidget(Widget * SubWidget);
	void RemoveSubWidget(Widget * SubWidget);
	void Destroy(void);
	// receive input
	bool MouseButton(int Button, int State, float X, float Y);
	// signal listeners
	void AddDestroyListener(DestroyListener * DestroyListener);
	void AddMouseButtonListener(MouseButtonListener * MouseButtonListener);
	// static manager functions
	static std::list< Widget * > & GetTopLevelWidgets(void);
	static std::list< Widget * > & GetDestroyedWidgets(void);
private:
	Widget * m_SupWidget;
	Color * m_BackgroundColor;
	math3d::vector2f m_Position;
	math3d::vector2f m_Size;
	std::list< Widget * > m_SubWidgets;
	// listeners
	std::list< DestroyListener * > m_DestroyListeners;
	std::list< MouseButtonListener * > m_MouseButtonListeners;
	// static manager properties
	static std::list< Widget * > m_TopLevelWidgets;
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

inline std::list< Widget * > & Widget::GetTopLevelWidgets(void)
{
	return m_TopLevelWidgets;
}

inline std::list< Widget * > & Widget::GetDestroyedWidgets(void)
{
	return m_DestroyedWidgets;
}

#endif
