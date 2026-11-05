/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__COMMODITY_H
#define GALACTIC_FALL__SOURCE__COMMODITY_H

#include <algebra/axis_angle.h>
#include <algebra/vector3f.h>

#include "object.h"

class Visualization;

class Commodity : public Object
{
public:
    Commodity(void);
    virtual ~Commodity(void);
    // getters
    auto GetAngularVelocity() const -> Algebra::AxisAngle const &;
    float GetHull(void) const;
    const Vector3f & GetVelocity(void) const;
    // setters
    auto SetAngularVelocity(Algebra::AxisAngle const & AngularVelocity) -> void;
    void SetHull(float Hull);
    void SetVelocity(const Vector3f & Velocity);
private:
    // slot for the object aspect Update
    bool Update(float Seconds);
    void _UpdateVisualization(Visualization * Visualization);
private:
    Algebra::AxisAngle m_AngularVelocity;
    float m_Hull;
    Vector3f m_Velocity;
    // no copy constructor
    Commodity(const Commodity & Commodity);
};

inline auto Commodity::GetAngularVelocity() const -> Algebra::AxisAngle const &
{
    return m_AngularVelocity;
}

inline float Commodity::GetHull(void) const
{
    return m_Hull;
}

inline const Vector3f & Commodity::GetVelocity(void) const
{
    return m_Velocity;
}

inline auto Commodity::SetAngularVelocity(Algebra::AxisAngle const & AngularVelocity) -> void
{
    m_AngularVelocity = AngularVelocity;
}

inline void Commodity::SetHull(float Hull)
{
    m_Hull = Hull;
}

inline void Commodity::SetVelocity(const Vector3f & Velocity)
{
    m_Velocity = Velocity;
}

#endif
