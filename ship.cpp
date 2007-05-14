#include <assert.h>

#include <sstream>

#include <GL/gl.h>

#include "color.h"
#include "model.h"
#include "real_time.h"
#include "ship.h"
#include "shot.h"
#include "system.h"

Ship::Ship(ShipClass * ShipClass) :
	m_Accelerate(false),
	m_TurnLeft(false),
	m_TurnRight(false),
	m_Fire(false),
	m_Velocity(true),
	m_AngularPosition(0.0f),
	m_ShipClass(ShipClass),
	m_Fuel(0.0f),
	m_NextTimeToFire(0.0)
{
	SetRadialSize(m_ShipClass->GetModel()->GetRadialSize());
}

void Ship::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, math3d::vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
	if(m_ShipClass->GetColor() != 0)
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_ShipClass->GetColor()->GetColor().m_V.m_A);
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, math3d::vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
	}
	m_ShipClass->GetModel()->Draw();
	glPopMatrix();
}

void Ship::Update(float Seconds)
{
	if(m_TurnLeft == true)
	{
		float FuelConsumption(m_ShipClass->GetTurnFuel() * Seconds);
		
		if(m_Fuel >= FuelConsumption)
		{
			m_AngularPosition += GetTurnSpeed() * Seconds;
			m_Fuel -= FuelConsumption;
		}
	}
	if(m_TurnRight == true)
	{
		float FuelConsumption(m_ShipClass->GetTurnFuel() * Seconds);
		
		if(m_Fuel >= FuelConsumption)
		{
			m_AngularPosition -= GetTurnSpeed() * Seconds;
			m_Fuel -= FuelConsumption;
		}
	}
	m_Position += m_Velocity * Seconds;
	if(m_Accelerate == true)
	{
		float FuelConsumption(m_ShipClass->GetForwardFuel() * Seconds);
		
		if(m_Fuel >= FuelConsumption)
		{
			math3d::vector2f ForwardThrust(GetForwardThrust(), m_AngularPosition, math3d::vector2f::magnitude_angle);
			
			m_Position += ForwardThrust * (Seconds * Seconds / 2.0f);
			m_Velocity += ForwardThrust * Seconds;
			if(m_Velocity.length() > GetMaximumSpeed())
			{
				m_Velocity.normalize();
				m_Velocity.scale(GetMaximumSpeed());
			}
			m_Fuel -= FuelConsumption;
		}
	}
	if(m_Fire == true)
	{
		if(IsReadyToFire() == true)
		{
			std::stringstream IdentifierStream;
			
			IdentifierStream << "::system(" << GetCurrentSystem()->GetIdentifier() << ")::created_at(" << RealTime::GetTime() << ")::";
			
			Shot * NewShot(new Shot(GetAngularPosition(), GetVelocity() + math3d::vector2f(30.0f, GetAngularPosition(), math3d::vector2f::magnitude_angle)));
			
			NewShot->SetObjectIdentifier(IdentifierStream.str() + "shot");
			NewShot->SetPosition(GetPosition());
			GetCurrentSystem()->AddShot(NewShot);
			ResetNextTimeToFire();
		}
	}
}

float Ship::GetFuelCapacity(void) const
{
	return m_ShipClass->GetFuelHoldSize();
}

float Ship::GetFreeCargoHoldSize(void) const
{
	float CargoHoldSize(m_ShipClass->GetCargoHoldSize());
	std::map< const Commodity *, float >::const_iterator Commodity(m_Commodities.begin());
	
	while(Commodity != m_Commodities.end())
	{
		CargoHoldSize -= Commodity->second;
		++Commodity;
	}
	
	return CargoHoldSize;
}

bool Ship::AddCommodities(const Commodity * CargoCommodity, float Amount)
{
	if(Amount > GetFreeCargoHoldSize())
	{
		return false;
	}
	else
	{
		std::map< const Commodity *, float >::iterator CommodityIterator(m_Commodities.find(CargoCommodity));
		
		if(CommodityIterator == m_Commodities.end())
		{
			m_Commodities[CargoCommodity] = Amount;
		}
		else
		{
			CommodityIterator->second += Amount;
		}
		
		return true;
	}
}

bool Ship::RemoveCommodities(const Commodity * CargoCommodity, float Amount)
{
	std::map< const Commodity *, float >::iterator CommodityIterator(m_Commodities.find(CargoCommodity));
	
	if(CommodityIterator == m_Commodities.end())
	{
		return false;
	}
	if(CommodityIterator->second < Amount)
	{
		return false;
	}
	CommodityIterator->second -= Amount;
	if(CommodityIterator->second == 0)
	{
		m_Commodities.erase(CommodityIterator);
	}
	
	return true;
}

bool Ship::IsReadyToFire(void)
{
	return m_NextTimeToFire <= RealTime::GetTime();
}

void Ship::ResetNextTimeToFire(void)
{
	m_NextTimeToFire = RealTime::GetTime() + 0.25;
}
