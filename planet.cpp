#include <GL/gl.h>
#include <GL/glut.h>

#include "commodity.h"
#include "planet.h"

PlanetCommodity::PlanetCommodity(Commodity * Commodity) :
	m_Commodity(Commodity),
	m_BasePriceModifier(1.0f)
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
	m_Identifier(Identifier)
{
}

Planet::~Planet(void)
{
	while(m_Commodities.size() > 0)
	{
		delete m_Commodities.back();
		m_Commodities.pop_back();
	}
}

void Planet::SetName(const std::string & Name)
{
	m_Name = Name;
}

void Planet::SetDescription(const std::string & Description)
{
	m_Description = Description;
}

void Planet::SetSize(const float & Size)
{
	m_Size = Size;
}

void Planet::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glutSolidSphere(m_Size / 2.0f, 40, 8);
	glPopMatrix();
}

PlanetCommodity * Planet::CreateCommodity(Commodity * Commodity)
{
	/// @todo check whether the commodity already exists
	m_Commodities.push_back(new PlanetCommodity(Commodity));
	
	return m_Commodities.back();
}
