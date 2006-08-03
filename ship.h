#ifndef SHIP_H
#define SHIP_H

#include <map>

#include <math3d/vector2f.h>

#include "position.h"

class Commodity;
class ShipClass;

class Ship : public Position
{
public:
	Ship(ShipClass * ShipClass);
	void Draw(void) const;
	void Move(float Seconds);
	// enabler
	bool m_Accelerate;
	bool m_TurnLeft;
	bool m_TurnRight;
	// current state
	math3d::vector2f m_Velocity;
	float m_AngularPosition;
	// getters
	const ShipClass * GetShipClass(void) const;
	const math3d::vector2f & GetVelocity(void) const;
	float GetFreeCargoHoldSize(void) const;
	float GetCargo(const Commodity * CargoCommodity) const;
	// modifiers
	bool AddCargo(const Commodity * CargoCommodity, float Amount);
	bool RemoveCargo(const Commodity * CargoCommodity, float Amount);
private:
	// ship class
	ShipClass * m_ShipClass;
	std::map< const Commodity *, float > m_Cargo;
};

inline const ShipClass * Ship::GetShipClass(void) const
{
	return m_ShipClass;
}

inline const math3d::vector2f & Ship::GetVelocity(void) const
{
	return m_Velocity;
}

inline float Ship::GetCargo(const Commodity * CargoCommodity) const
{
	std::map< const Commodity *, float >::const_iterator CargoIterator(m_Cargo.find(CargoCommodity));
	
	return ((CargoIterator == m_Cargo.end()) ? (0) : (CargoIterator->second));
}

#endif
