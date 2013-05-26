#ifndef LITTLEM_WIDGETS_LABEL_H
#define LITTLEM_WIDGETS_LABEL_H

#include "widget.h"

class Label : public Widget
{
public:
	Label(const std::string & sString = "");
	virtual void vDraw(void);
	void vSetString(const std::string & sString);
private:
	std::string m_sString;
};

#endif
