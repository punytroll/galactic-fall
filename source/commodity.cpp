/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <algebra/vector4f.h>

#include <graphics/node.h>

#include "commodity.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "visualization.h"

Commodity::Commodity(void) :
    m_AngularVelocity{Algebra::AxisAngle::CreateZero()},
    m_Hull(20.0f),
    m_Velocity(Vector3f::CreateZero())
{
    // initialize required aspects
    AddAspectName();
    AddAspectPhysical();
    AddAspectPosition();
    AddAspectUpdate();
    GetAspectUpdate()->SetCallback(std::bind(&Commodity::Update, this, std::placeholders::_1));
    AddAspectVisualization();
    GetAspectVisualization()->SetUpdateVisualizationCallback(std::bind(&Commodity::_UpdateVisualization, this, std::placeholders::_1));
}

Commodity::~Commodity(void)
{
}

bool Commodity::Update(float Seconds)
{
    GetAspectPosition()->ModifyOrientation(Quaternion::CreateFromAxisComponentsAndAngle(m_AngularVelocity[0], m_AngularVelocity[1], m_AngularVelocity[2], m_AngularVelocity[3] * Seconds));
    GetAspectPosition()->ModifyPosition(m_Velocity * Seconds);
    
    return true;
}

void Commodity::_UpdateVisualization(Visualization * Visualization)
{
    assert(Visualization != nullptr);
    assert(Visualization->GetGraphics() != nullptr);
    assert(GetAspectPosition() != nullptr);
    if(Visualization->GetUpdateOrientation() == true)
    {
        Visualization->GetGraphics()->SetOrientation(GetAspectPosition()->GetOrientation());
    }
    if(Visualization->GetUpdatePosition() == true)
    {
        Visualization->GetGraphics()->SetPosition(GetAspectPosition()->GetPosition());
    }
}
