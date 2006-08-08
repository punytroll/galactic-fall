#ifndef LABEL_H
#define LABEL_H

#include <string>

#include "color.h"
#include "widget.h"

class Label : public Widget
{
public:
	enum HorizontalAlignment
	{
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_HORIZONTAL_CENTER
	};
	
	enum VerticalAlignment
	{
		ALIGN_TOP,
		ALIGN_BOTTOM,
		ALIGN_VERTICAL_CENTER
	};
	
	Label(Widget * SupWidget);
	Label(Widget * SupWidget, const std::string & String);
	~Label(void);
	void SetString(const std::string & String);
	void SetForegroundColor(const Color & ForegroundColor);
	void SetWrap(bool Wrap);
	void SetWordWrap(bool WordWrap);
	void SetHorizontalAlignment(const Label::HorizontalAlignment & HorizontalAlignment);
	void SetVerticalAlignment(const Label::VerticalAlignment & VerticalAlignment);
	virtual void Draw(void) const;
	const std::string & GetString(void) const;
	const Color & GetForegroundColor(void) const;
	Color & GetForegroundColor(void);
	const bool & GetWrap(void) const;
	const bool & GetWordWrap(void) const;
	const Label::HorizontalAlignment & GetHorizontalAlignment(void) const;
	const Label::VerticalAlignment & GetVerticalAlignment(void) const;
private:
	std::string m_String;
	Color m_ForegroundColor;
	bool m_Wrap;
	bool m_WordWrap;
	HorizontalAlignment m_HorizontalAlignment;
	VerticalAlignment m_VerticalAlignment;
};

inline const std::string & Label::GetString(void) const
{
	return m_String;
}

inline const Color & Label::GetForegroundColor(void) const
{
	return m_ForegroundColor;
}

inline Color & Label::GetForegroundColor(void)
{
	return m_ForegroundColor;
}

inline const bool & Label::GetWrap(void) const
{
	return m_Wrap;
}

inline const bool & Label::GetWordWrap(void) const
{
	return m_WordWrap;
}

inline const Label::HorizontalAlignment & Label::GetHorizontalAlignment(void) const
{
	return m_HorizontalAlignment;
}

inline const Label::VerticalAlignment & Label::GetVerticalAlignment(void) const
{
	return m_VerticalAlignment;
}

#endif
