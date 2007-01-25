#ifndef WIDGET_H
#define WIDGET_H

#include <iostream>
#include <list>
#include <string>

#include <math3d/vector2f.h>

#include "destroy_listener.h"

class Color;
class KeyListener;
class MouseButtonListener;
class MouseMotionListener;

class Widget : public DestroyListener
{
public:
	Widget(Widget * SupWidget = 0, const std::string & Name = "");
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
	void SetName(const std::string & Name);
	// getters
	const Color & GetBackgroundColor(void) const;
	const math3d::vector2f GetPosition(void) const;
	const math3d::vector2f GetSize(void) const;
	const std::string & GetName(void) const;
	Widget * GetRootWidget(void);
	const Widget * GetRootWidget(void) const;
	Widget * GetSupWidget(void);
	const Widget * GetSupWidget(void) const;
	Widget * GetSubWidget(const std::string & Name);
	const Widget * GetSubWidget(const std::string & Name) const;
	Widget * GetKeyFocus(void);
	bool IsVisible(void) const;
	// receive input
	bool MouseButton(int Button, int State, float X, float Y);
	bool Key(int Key, int State);
	void MouseMotion(float X, float Y);
	// MouseEnter may depend on the fact that m_HoverWidget on the m_SupWidget is set to this
	// MouseEnter on the new hover widget is called after MouseLeave on the old hover widget
	void MouseEnter(void);
	// MouseLeave may depend on the fact that m_HoverWidget on the m_SupWidget is already set to the new widget
	// MouseLeave on the old hover widget is called before MouseEnter on the new hover widget
	void MouseLeave(void);
	// add signal listeners
	void AddDestroyListener(DestroyListener * DestroyListener);
	void AddKeyListener(KeyListener * KeyListener);
	void AddMouseButtonListener(MouseButtonListener * MouseButtonListener);
	void AddMouseMotionListener(MouseMotionListener * MouseMotionListener);
	// remove signal listeners
	void RemoveDestroyListener(DestroyListener * DestroyListener);
	// static manager functions
	static std::list< Widget * > & GetDestroyedWidgets(void);
protected:
	virtual void OnDestroy(Widget * EventSource);
private:
	std::string m_Name;
	Widget * m_SupWidget;
	Widget * m_HoverWidget;
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
	std::list< MouseMotionListener * > m_MouseMotionListeners;
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

inline const std::string & Widget::GetName(void) const
{
	return m_Name;
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

inline Widget * Widget::GetSupWidget(void)
{
	return m_SupWidget;
}

inline const Widget * Widget::GetSupWidget(void) const
{
	return m_SupWidget;
}

inline Widget * Widget::GetSubWidget(const std::string & Name)
{
	for(std::list< Widget * >::iterator Iterator = m_SubWidgets.begin(); Iterator != m_SubWidgets.end(); ++Iterator)
	{
		if((*Iterator)->GetName() == Name)
		{
			return *Iterator;
		}
	}
}

inline const Widget * Widget::GetSubWidget(const std::string & Name) const
{
	for(std::list< Widget * >::const_iterator Iterator = m_SubWidgets.begin(); Iterator != m_SubWidgets.end(); ++Iterator)
	{
		if((*Iterator)->GetName() == Name)
		{
			return *Iterator;
		}
	}
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
