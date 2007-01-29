#include <assert.h>

#include <GL/gl.h>

#include "model.h"
#include "ship.h"

Ship::Ship(ShipClass * ShipClass) :
	m_Accelerate(false),
	m_TurnLeft(false),
	m_TurnRight(false),
	m_ShipClass(ShipClass),
	m_Fuel(0.0f)
{
	SetRadialSize(m_ShipClass->GetModel()->GetRadialSize());
}

void Ship::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	m_ShipClass->GetModel()->Draw();
	glPopMatrix();
}

void Ship::Move(float Seconds)
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
		float FuelConsumption(m_ShipClass->GetTurnFuel() * Seconds);
		
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
}

float Ship::GetFuelCapacity(void) const
{
	return m_ShipClass->GetFuelHoldSize();
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

void Ship::Jump(void)
{
	assert(m_Fuel >= m_ShipClass->GetJumpFuel());
	m_Fuel -= m_ShipClass->GetJumpFuel();
}

void Ship::Refuel(float Fuel)
{
	assert(m_Fuel + Fuel <= GetFuelCapacity());
	m_Fuel += Fuel;
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
