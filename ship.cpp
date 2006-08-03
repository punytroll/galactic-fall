#include <GL/gl.h>

#include "model.h"
#include "ship.h"
#include "ship_class.h"

Ship::Ship(ShipClass * ShipClass) :
	m_ShipClass(ShipClass)
{
}

void Ship::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	m_ShipClass->m_Model->Draw();
	glPopMatrix();
}

void Ship::Move(float Seconds)
{
	if(m_TurnLeft == true)
	{
		m_AngularPosition += m_ShipClass->m_TurnSpeed * Seconds;
	}
	if(m_TurnRight == true)
	{
		m_AngularPosition -= m_ShipClass->m_TurnSpeed * Seconds;
	}
	m_Position += m_Velocity * Seconds;
	if(m_Accelerate == true)
	{
		math3d::vector2f ForwardThrust(m_ShipClass->m_ForwardThrust, m_AngularPosition, math3d::vector2f::magnitude_angle);
		
		m_Position += ForwardThrust * (Seconds * Seconds / 2.0f);
		m_Velocity += ForwardThrust * Seconds;
		if(m_Velocity.length() > m_ShipClass->m_MaximumVelocity)
		{
			m_Velocity.normalize();
			m_Velocity.scale(m_ShipClass->m_MaximumVelocity);
		}
	}
}

float Ship::GetFreeCargoHoldSize(void) const
{
	float CargoHoldSize(m_ShipClass->GetCargoHoldSize());
	std::map< const Commodity *, float >::const_iterator CargoIterator(m_Cargo.begin());
	
	while(CargoIterator != m_Cargo.end())
	{
		CargoHoldSize -= CargoIterator->second;
		++CargoIterator;
	}
	
	return CargoHoldSize;
}

bool Ship::AddCargo(const Commodity * CargoCommodity, float Amount)
{
	if(Amount > GetFreeCargoHoldSize())
	{
		return false;
	}
	else
	{
		std::map< const Commodity *, float >::iterator CargoIterator(m_Cargo.find(CargoCommodity));
		
		if(CargoIterator == m_Cargo.end())
		{
			m_Cargo[CargoCommodity] = Amount;
		}
		else
		{
			m_Cargo[CargoCommodity] += Amount;
		}
		
		return true;
	}
}

bool Ship::RemoveCargo(const Commodity * CargoCommodity, float Amount)
{
	std::map< const Commodity *, float >::iterator CargoIterator(m_Cargo.find(CargoCommodity));
	
	if(CargoIterator == m_Cargo.end())
	{
		return false;
	}
	if(CargoIterator->second < Amount)
	{
		return false;
	}
	CargoIterator->second -= Amount;
	if(CargoIterator->second == 0)
	{
		m_Cargo.erase(CargoIterator);
	}
	
	return true;
}
