/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include "color.h"
#include "commodity.h"
#include "commodity_class.h"
#include "globals.h"
#include "graphics_model_object.h"
#include "model_manager.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "visualizations.h"
#include "weapon_class.h"

void VisualizeCommodity(Commodity * Commodity, Graphics::Node * Container)
{
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetDiffuseColor(*(Commodity->GetCommodityClass()->GetColor()));
	Visualization->SetModel(Commodity->GetCommodityClass()->GetModel());
	Visualization->SetOrientation(Commodity->GetAngularPosition());
	Visualization->SetPosition(Commodity->GetPosition());
	Visualization->SetUseLighting(true);
	// set as the object's visualization
	Commodity->SetVisualization(Visualization);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizePlanet(Planet * Planet, Graphics::Node * Container)
{
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	
	Visualization->SetDiffuseColor(Planet->GetColor());
	Visualization->SetModel(g_ModelManager->Get("planet"));
	Visualization->SetNormalize(true);
	Visualization->SetOrientation(Quaternion(true));
	Visualization->SetPosition(Planet->GetPosition());
	Visualization->SetScale(Planet->GetRadialSize());
	Visualization->SetShininess(10.0f);
	Visualization->SetSpecularColor(Color(0.25f, 0.25f, 0.25f, 1.0f));
	Visualization->SetUseLighting(true);
	// set as the object's visualization
	Planet->SetVisualization(Visualization);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizeShip(Ship * Ship, Graphics::Node * Container)
{
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetDiffuseColor(*(Ship->GetShipClass()->GetColor()));
	Visualization->SetModel(Ship->GetShipClass()->GetModel());
	Visualization->SetOrientation(Quaternion(Ship->GetAngularPosition(), Quaternion::InitializeRotationZ));
	Visualization->SetPosition(Ship->GetPosition());
	Visualization->SetShininess(40.0f);
	Visualization->SetSpecularColor(Color(0.45f, 0.45f, 0.45f, 1.0f));
	Visualization->SetUseLighting(true);
	// set as the object's visualization
	Ship->SetVisualization(Visualization);
	// add to the scene
	Container->AddNode(Visualization);
}

void VisualizeShot(Shot * Shot, Graphics::Node * Container)
{
	Graphics::ModelObject * Visualization(new Graphics::ModelObject());
	
	Visualization->SetClearDepthBuffer(true);
	Visualization->SetDiffuseColor(*(Shot->GetWeaponClass()->GetParticleColor()));
	Visualization->SetModel(Shot->GetWeaponClass()->GetParticleModel());
	Visualization->SetOrientation(Shot->GetAngularPosition());
	Visualization->SetPosition(Shot->GetPosition());
	Visualization->SetUseBlending(true);
	Visualization->SetUseLighting(false);
	Shot->SetVisualization(Visualization);
	// add to the scene
	Container->AddNode(Visualization);
}
