#include "title_bar.h"

TitleBar::TitleBar(Widget * SupWidget, const std::string & Title) :
	Label(SupWidget, Title)
{
	SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	SetBackgroundColor(Color(0.2f, 0.2f, 0.4f));
	AddMouseButtonListener(this);
	AddMouseMotionListener(this);
}
