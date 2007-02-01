#include <GL/gl.h>
#include <GL/glu.h>

#include <math3d/vector4f.h>

#include "color.h"
#include "commodity.h"
#include "planet.h"

PlanetCommodity::PlanetCommodity(Commodity * Commodity) :
	m_Commodity(Commodity),
	m_BasePriceModifier(1.0f)
{
}

PlanetCommodity::~PlanetCommodity(void)
{
}

float PlanetCommodity::GetPrice(void) const
{
	return m_Commodity->GetBasePrice() * m_BasePriceModifier;
}

void PlanetCommodity::SetBasePriceModifier(const float & BasePriceModifier)
{
	m_BasePriceModifier = BasePriceModifier;
}

Planet::Planet(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_AllowRefuelling(false),
	m_Color(0)
{
}

Planet::~Planet(void)
{
	while(m_Commodities.size() > 0)
	{
		delete m_Commodities.back();
		m_Commodities.pop_back();
	}
	delete m_Color;
	m_Color = 0;
}

void Planet::SetName(const std::string & Name)
{
	m_Name = Name;
}

void Planet::SetAllowRefuelling(bool AllowRefuelling)
{
	m_AllowRefuelling = AllowRefuelling;
}

void Planet::SetDescription(const std::string & Description)
{
	m_Description = Description;
}

void Planet::SetSize(const float & Size)
{
	m_Size = Size;
	SetRadialSize(m_Size / 2.0f);
}

void Planet::SetColor(const Color & NewColor)
{
	delete m_Color;
	m_Color = new Color(NewColor);
}

void Planet::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	if(m_Color != 0)
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Color->GetColor().m_V.m_A);
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
		glMaterialfv(GL_FRONT, GL_SPECULAR, (math3d::vector4f(1.0f, 1.0f, 1.0f, 1.0f) - m_Color->GetColor()).m_V.m_A);
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, math3d::vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
	}
	
	GLUquadric * Quadric(gluNewQuadric());
	
	gluSphere(Quadric, GetRadialSize(), 40, 80);
	/* TODO: This code allows an athmosphere around the planet ... optimize and make usable via a planet property
	glEnable(GL_BLEND);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, math3d::vector4f(1.0f, 1.0f, 1.0f, 0.35f).m_V.m_A);
	glMaterialfv(GL_FRONT, GL_SPECULAR, math3d::vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
	gluSphere(Quadric, m_Size / 1.95f, 40, 80);
	glDisable(GL_BLEND);
	*/
	gluDeleteQuadric(Quadric);
	glPopMatrix();
}

PlanetCommodity * Planet::CreateCommodity(Commodity * Commodity)
{
	/// @todo check whether the commodity already exists
	m_Commodities.push_back(new PlanetCommodity(Commodity));
	
	return m_Commodities.back();
}
