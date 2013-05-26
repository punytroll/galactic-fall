#ifndef LITTLEM_WIDGETS_COLOR_H
#define LITTLEM_WIDGETS_COLOR_H

class Color
{
public:
	Color(float fRed, float fGreen, float fBlue);
	Color(const Color & OtherColor);
	~Color(void);
	
	union
	{
		float m_Color[4];
		struct
		{
			float m_fRed;
			float m_fGreen;
			float m_fBlue;
			float m_fAlpha;
		};
	};
};

#endif
