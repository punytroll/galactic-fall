#include <iostream>

#include <GL/glut.h>

#include "button.h"
#include "clicked_listener.h"

Button::Button(Widget * SupWidget) :
	Widget(SupWidget)
{
	AddMouseButtonListener(this);
}

Button::~Button(void)
{
}

void Button::AddClickedListener(ClickedListener * ClickedListener)
{
	m_ClickedListeners.push_back(ClickedListener);
}

bool Button::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if((Button == GLUT_LEFT_BUTTON) && (State == GLUT_UP))
	{
		for(std::list< ClickedListener * >::iterator ClickedListenerIterator = m_ClickedListeners.begin(); ClickedListenerIterator != m_ClickedListeners.end(); ++ClickedListenerIterator)
		{
			if((*ClickedListenerIterator)->OnClicked(this) == true)
			{
				return true;
			}
		}
	}
	
	return false;
}
