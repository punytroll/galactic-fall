/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <GL/glx.h>
#include <GL/gl.h>

#include "arx_reading.h"
#include "asset_class.h"
#include "asset_class_manager.h"
#include "callbacks.h"
#include "camera.h"
#include "character.h"
#include "color.h"
#include "command_mind.h"
#include "commodity.h"
#include "commodity_class.h"
#include "commodity_class_manager.h"
#include "destroy_listener.h"
#include "draw_text.h"
#include "galaxy.h"
#include "game_time.h"
#include "globals.h"
#include "goals.h"
#include "graphics_engine.h"
#include "graphics_mesh_manager.h"
#include "graphics_model.h"
#include "graphics_model_manager.h"
#include "graphics_model_object.h"
#include "graphics_particle_system.h"
#include "graphics_scene.h"
#include "graphics_texture_manager.h"
#include "key_event_information.h"
#include "label.h"
#include "load_game_dialog.h"
#include "map_dialog.h"
#include "map_knowledge.h"
#include "math.h"
#include "mind.h"
#include "mini_map_display.h"
#include "mount_weapon_dialog.h"
#include "object_factory.h"
#include "perspective.h"
#include "planet.h"
#include "planet_dialog.h"
#include "real_time.h"
#include "save_game_dialog.h"
#include "scanner_display.h"
#include "ship.h"
#include "ship_class.h"
#include "ship_class_manager.h"
#include "shot.h"
#include "slot.h"
#include "slot_class_manager.h"
#include "star.h"
#include "star_map_display.h"
#include "state_machine.h"
#include "states.h"
#include "string_cast.h"
#include "system.h"
#include "timeout_notifications.h"
#include "timing_dialog.h"
#include "user_interface.h"
#include "visualizations.h"
#include "weapon.h"
#include "weapon_class.h"
#include "weapon_class_manager.h"
#include "widget.h"
#include "xml_puny_dom.h"
#include "xml_stream.h"

class SystemStatistics
{
public:
	// setters
	void SetFramesPerSecond(float FramesPerSecond)
	{
		m_FramesPerSecond = FramesPerSecond;
	}
	
	void SetAISecondsPerFrame(float AISecondsPerFrame)
	{
		m_AISecondsPerFrame = AISecondsPerFrame;
	}
	
	void SetGraphicsSecondsPerFrame(float GraphicsSecondsPerFrame)
	{
		m_GraphicsSecondsPerFrame = GraphicsSecondsPerFrame;
	}
	
	void SetPhysicsSecondsPerFrame(float PhysicsSecondsPerFrame)
	{
		m_PhysicsSecondsPerFrame = PhysicsSecondsPerFrame;
	}
	
	void SetTotalSecondsPerFrame(float TotalSecondsPerFrame)
	{
		m_TotalSecondsPerFrame = TotalSecondsPerFrame;
	}
	
	void SetTotalSecondsPerFrameProcessing(float TotalSecondsPerFrameProcessing)
	{
		m_TotalSecondsPerFrameProcessing = TotalSecondsPerFrameProcessing;
	}
	
	// getters
	float GetFramesPerSecond(void) const
	{
		return m_FramesPerSecond;
	}
	
	float GetAISecondsPerFrame(void) const
	{
		return m_AISecondsPerFrame;
	}
	
	float GetGraphicsSecondsPerFrame(void) const
	{
		return m_GraphicsSecondsPerFrame;
	}
	
	float GetPhysicsSecondsPerFrame(void) const
	{
		return m_PhysicsSecondsPerFrame;
	}
	
	float GetTotalSecondsPerFrame(void) const
	{
		return m_TotalSecondsPerFrame;
	}
	
	float GetTotalSecondsPerFrameProcessing(void) const
	{
		return m_TotalSecondsPerFrameProcessing;
	}
private:
	float m_FramesPerSecond;
	float m_AISecondsPerFrame;
	float m_GraphicsSecondsPerFrame;
	float m_PhysicsSecondsPerFrame;
	float m_TotalSecondsPerFrame;
	float m_TotalSecondsPerFrameProcessing;
};

// these objects are exported via globals.h
AssetClassManager * g_AssetClassManager(0);
CommodityClassManager * g_CommodityClassManager(0);
Galaxy * g_Galaxy(0);
Graphics::Engine * g_GraphicsEngine(0);
Graphics::Scene * g_MainScene(0);
Graphics::Node * g_CommodityLayer(0);
Graphics::Node * g_PlanetLayer(0);
Graphics::Node * g_ShipLayer(0);
Graphics::Node * g_ShotLayer(0);
Graphics::Node * g_ParticleSystemsLayer(0);
Graphics::MeshManager * g_MeshManager(0);
Graphics::ModelManager * g_ModelManager(0);
ObjectFactory * g_ObjectFactory(0);
ShipClassManager * g_ShipClassManager(0);
SlotClassManager * g_SlotClassManager(0);
Graphics::TextureManager * g_TextureManager(0);
UserInterface * g_UserInterface(0);
WeaponClassManager * g_WeaponClassManager(0);

int g_LastMotionX(-1);
int g_LastMotionY(-1);
int g_MouseButton(-1);
Camera g_Camera;
Reference< CommandMind > g_InputMind;
Reference< Mind > g_OutputMind;
float g_Width(0.0f);
float g_Height(0.0f);
Label * g_SystemLabel(0);
Label * g_TimeWarpLabel(0);
Label * g_TargetLabel(0);
Label * g_CreditsLabel(0);
Label * g_FuelLabel(0);
Label * g_HullLabel(0);
Label * g_MessageLabel(0);
Label * g_CurrentSystemLabel(0);
System * g_CurrentSystem;
float g_TimeWarp(1.0f);
bool g_Quit(false);
bool g_Pause(false);
PlanetDialog * g_PlanetDialog(0);
MapDialog * g_MapDialog(0);
MountWeaponDialog * g_MountWeaponDialog(0);
LoadGameDialog * g_LoadGameDialog(0);
SaveGameDialog * g_SaveGameDialog(0);
TimingDialog * g_TimingDialog(0);
TimeoutNotificationManager * g_GameTimeTimeoutNotifications;
TimeoutNotification g_SpawnShipTimeoutNotification;
TimeoutNotificationManager * g_RealTimeTimeoutNotifications;
TimeoutNotification g_MessageTimeoutNotification;
Widget * g_MiniMap(0);
Widget * g_Scanner(0);
MiniMapDisplay * g_MiniMapDisplay(0);
ScannerDisplay * g_ScannerDisplay(0);
Display * g_Display;
GLXContext g_GLXContext;
Window g_Window;
Perspective g_MainPerspective;
bool g_EchoEvents(false);
bool g_DumpEndReport(false);
bool g_TakeScreenShot(false);
SystemStatistics * g_SystemStatistics;
std::map< Graphics::Node *, Reference< Graphics::Node > > g_VisualizationReferences;
ResourceReader * g_ResourceReader(0);

int WantToJump(Ship * Ship, System * System)
{
	if(System == 0)
	{
		return NO_JUMP_TARGET;
	}
	// only let the ship jump if it is not near any other stellar object
	const std::vector< Planet * > & Planets(Ship->GetCurrentSystem()->GetPlanets());
	
	for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
	{
		if((Ship->GetPosition() - (*PlanetIterator)->GetPosition()).SquaredLength() < 4 * (*PlanetIterator)->GetSize() * (*PlanetIterator)->GetSize())
		{
			return TOO_NEAR_TO_STELLAR_OBJECT;
		}
	}
	// only let ships jump if they have enough fuel
	if(Ship->GetFuel() < Ship->GetShipClass()->GetJumpFuel())
	{
		return NOT_ENOUGH_FUEL;
	}
	
	return OK;
}

int WantToLand(const Character * Character, const Ship * Ship, const Planet * Planet)
{
	if(Planet == 0)
	{
		return NO_LAND_TARGET;
	}
	// test distance
	if((Planet->GetPosition() - Ship->GetPosition()).SquaredLength() > Planet->GetSize() * Planet->GetSize())
	{
		return TOO_FAR_AWAY;
	}
	// test speed (should be relative speed but planets have no speed, yet)
	if(Ship->GetVelocity().SquaredLength() > 2.0f)
	{
		return TOO_FAST;
	}
	// test credits
	if(Character->GetCredits() < Planet->GetLandingFee())
	{
		return NOT_ENOUGH_CREDITS;
	}
	
	return OK;
}

int WantToScoop(const Ship * Ship, const Commodity * Commodity)
{
	if(Commodity == 0)
	{
		return NO_SCOOP_TARGET;
	}
	// test distance
	if((Commodity->GetPosition() - Ship->GetPosition()).SquaredLength() > 5.0f * Commodity->GetRadialSize() * Commodity->GetRadialSize())
	{
		return TOO_FAR_AWAY;
	}
	// test speed
	if((Ship->GetVelocity() - Commodity->GetVelocity()).SquaredLength() > 2.0f)
	{
		return TOO_HIGH_RELATIVE_VELOCITY;
	}
	// test cargo hold size on the ship
	if(Ship->GetAvailableSpace() < 1.0f)
	{
		return NOT_ENOUGH_SPACE;
	}
	
	return OK;
}

float CalculateTime(void)
{
	static double LastTime(RealTime::Get());
	
	RealTime::Invalidate();
	
	double DeltaSeconds(RealTime::Get() - LastTime);
	
	LastTime = RealTime::Get();
	if(g_Pause == false)
	{
		DeltaSeconds *= g_TimeWarp;
		GameTime::Set(GameTime::Get() + DeltaSeconds);
		
		return DeltaSeconds;
	}
	else
	{
		return 0.0f;
	}
}

void HideMessage(void)
{
	g_MessageLabel->SetVisible(false);
}

void SetMessage(const std::string & Message)
{
	g_MessageLabel->SetString(Message);
	g_MessageLabel->SetVisible(true);
	/// TODO: Make the 2.0f seconds timeout configurable via the game configuration archive.
	if(g_MessageTimeoutNotification.IsValid() == true)
	{
		g_MessageTimeoutNotification.Dismiss();
	}
	g_MessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, new FunctionCallback0< void >(HideMessage));
}

void DrawSelection(const Position * Position, float RadialSize)
{
	static const float OuterFactor(0.9f);
	static const float InnerFactor(1.1f);
	float OuterSize(RadialSize / OuterFactor);
	float InnerSize(RadialSize / InnerFactor);
	
	glPushMatrix();
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glTranslatef(Position->GetPosition().m_V.m_A[0], Position->GetPosition().m_V.m_A[1], 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-OuterSize, -InnerSize);
	glVertex2f(-OuterSize, -OuterSize);
	glVertex2f(-InnerSize, -OuterSize);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(-OuterSize, InnerSize);
	glVertex2f(-OuterSize, OuterSize);
	glVertex2f(-InnerSize, OuterSize);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(OuterSize, -InnerSize);
	glVertex2f(OuterSize, -OuterSize);
	glVertex2f(InnerSize, -OuterSize);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(OuterSize, InnerSize);
	glVertex2f(OuterSize, OuterSize);
	glVertex2f(InnerSize, OuterSize);
	glEnd();
	glPopAttrib();
	glPopMatrix();
}

void CollectWidgets(void)
{
	std::list< Widget * > & DestroyedWidgets(Widget::GetDestroyedWidgets());
	
	while(DestroyedWidgets.size() > 0)
	{
		delete DestroyedWidgets.front();
		DestroyedWidgets.pop_front();
	}
	/// TODO: Make the 5.0f seconds timeout configurable via the game configuration archive.
	g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 5.0f, new FunctionCallback0< void >(CollectWidgets));
}

void DisplayUserInterface(void)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, static_cast< GLfloat >(g_Width), 0.0, static_cast< GLfloat >(g_Height), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(0.0f, -g_Height, 0.0f);
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);
	g_UserInterface->Draw();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
}

void DeleteObject(Object * Object)
{
	Object->Destroy();
	delete Object;
}

void CalculateCharacters(void)
{
	std::set< Character * > & Characters(Character::GetCharacters());
	
	for(std::set< Character * >::iterator CharacterIterator = Characters.begin(); CharacterIterator != Characters.end(); ++CharacterIterator)
	{
		(*CharacterIterator)->Update();
	}
}

Reference< Graphics::ParticleSystem > CreateParticleSystem(const std::string & ParticleSystemClassIdentifier)
{
	Graphics::ParticleSystem * ParticleSystem(new Graphics::ParticleSystem());
	
	if(ParticleSystemClassIdentifier == "hit")
	{
		ParticleSystem->SetTimeOfDeath(GameTime::Get() + 3.0);
		for(int Index = 0; Index < 30; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			Vector2f Velocity(GetRandomFloat(0.0f, 0.5f) + GetRandomFloatFromExponentialDistribution(1.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
			
			Particle.m_Position.Set(0.0f, 0.0f, 0.0f);
			Particle.m_Velocity.Set(Velocity[0], Velocity[1], 0.0f);
			Particle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(0.3f, 0.8f);
			Particle.m_Color.Set(GetRandomFloat(0.4f, 0.5f), GetRandomFloat(0.35f, 0.45f), GetRandomFloat(0.35f, 0.65f), 0.3f);
			Particle.m_Size = GetRandomFloat(0.25f, 0.4f);
			ParticleSystem->AddParticle(Particle);
		}
		ParticleSystem->AddSystemScriptLine("kill-old");
		ParticleSystem->AddSystemScriptLine("move");
		ParticleSystem->AddSystemScriptLine("update-particles");
		ParticleSystem->AddParticleScriptLine("kill-old");
		ParticleSystem->AddParticleScriptLine("move");
	}
	else if(ParticleSystemClassIdentifier == "explosion")
	{
		ParticleSystem->SetTimeOfDeath(GameTime::Get() + 3.0);
		for(int Index = 0; Index < 500; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			Vector2f Velocity(GetRandomFloat(0.0f, 2.5f) + GetRandomFloatFromExponentialDistribution(2.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
			
			Particle.m_Velocity.Set(Velocity[0], Velocity[1], 0.0f);
			Particle.m_Position.Set(0.0f, 0.0f, 0.0f);
			Particle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(1.0f, 2.5f);
			Particle.m_Color.Set(GetRandomFloat(0.4f, 0.8f), GetRandomFloat(0.2f, 0.4f), GetRandomFloat(0.05f, 0.15f), 0.5f);
			Particle.m_Size = 1.0f;
			ParticleSystem->AddParticle(Particle);
		}
		for(int Index = 0; Index < 5; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			Vector2f Position(GetRandomFloat(0.0f, 6.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
			
			Particle.m_Position.Set(Position[0], Position[1], 0.0f);
			Particle.m_Velocity.Set(0.0f, 0.0f, 0.0f);
			Particle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(0.1f, 0.22f);
			Particle.m_Color.Set(0.9f, GetRandomFloat(0.9f, 1.0f), GetRandomFloat(0.95f, 1.0f), 0.15f);
			Particle.m_Size = 20.0f;
			ParticleSystem->AddParticle(Particle);
		}
		
		Graphics::ParticleSystem::Particle Particle;
		Vector2f Position(GetRandomFloat(0.0f, 6.0f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
		
		Particle.m_Position.Set(Position[0], Position[1], 0.0f);
		Particle.m_Velocity.Set(0.0f, 0.0f, 0.0f);
		Particle.m_TimeOfDeath = GameTime::Get() + GetRandomDouble(0.1f, 0.15f);
		Particle.m_Color.Set(0.9f, GetRandomFloat(0.9f, 1.0f), GetRandomFloat(0.95f, 1.0f), 0.3f);
		Particle.m_Size = 200.0f;
		ParticleSystem->AddParticle(Particle);
		ParticleSystem->AddSystemScriptLine("kill-old");
		ParticleSystem->AddSystemScriptLine("move");
		ParticleSystem->AddSystemScriptLine("update-particles");
		ParticleSystem->AddParticleScriptLine("kill-old");
		ParticleSystem->AddParticleScriptLine("move");
	}
	else if(ParticleSystemClassIdentifier == "engine_glow")
	{
		ParticleSystem->AddSystemScriptLine("update-particles");
		ParticleSystem->AddParticleScriptLine("kill-old");
		ParticleSystem->AddParticleScriptLine("move");
	}
	else
	{
		throw std::runtime_error("Could not create a paricle system with the class identifier'" + ParticleSystemClassIdentifier + "'.");
	}
	
	Reference< Graphics::Node > ParticleSystemReference(*ParticleSystem);
	
	g_VisualizationReferences[ParticleSystem] = ParticleSystemReference;
	
	return ParticleSystemReference;
}

void CalculateMovements(System * System, float Seconds)
{
	assert(System != 0);
	
	// TODO: it is unclear, which Ships to update really.
	const std::list< Ship * > & Ships(System->GetShips());
	std::list< Ship * >::const_iterator ShipIterator(Ships.begin());
	
	while(ShipIterator != Ships.end())
	{
		Ship * Ship(*ShipIterator);
		
		// increment up here because Update() might invalidate the iterator
		++ShipIterator;
		Ship->Update(Seconds);
		if(Ship->GetCurrentSystem() != System)
		{
			// if another ship jumps out of the system ... remove it
			if((g_OutputMind == false) || (g_OutputMind->GetCharacter() == 0) || (g_OutputMind->GetCharacter()->GetShip() != Ship))
			{
				DeleteObject(Ship);
				Ship = 0;
			}
		}
		if((Ship != 0) && (Ship->GetVisualization() != 0))
		{
			// update visualization
			Ship->GetVisualization()->SetPosition(Ship->GetPosition());
			Ship->GetVisualization()->SetOrientation(Ship->GetAngularPosition());
		}
	}
	
	const std::list< Commodity * > & Commodities(System->GetCommodities());
	
	for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
	{
		Commodity * Commodity(*CommodityIterator);
		
		Commodity->Move(Seconds);
		// update visualization
		Commodity->GetVisualization()->SetPosition(Commodity->GetPosition());
		Commodity->GetVisualization()->SetOrientation(Commodity->GetAngularPosition());
	}
	
	const std::list< Shot * > & Shots(System->GetShots());
	std::list< Shot * >::const_iterator ShotIterator(Shots.begin());
	
	while(ShotIterator != Shots.end())
	{
		Shot * TheShot(*ShotIterator);
		std::list< Shot * >::const_iterator NextIterator(ShotIterator);
		
		++NextIterator;
		
		if((*ShotIterator)->Update(Seconds) == false)
		{
			DeleteObject(TheShot);
			TheShot = 0;
		}
		else
		{
			// update visualization
			TheShot->GetVisualization()->SetPosition(TheShot->GetPosition());
			TheShot->GetVisualization()->SetOrientation(TheShot->GetAngularPosition());
		}
		// test for collisions with ships
		if(TheShot != 0)
		{
			for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
			{
				Ship * TheShip(*ShipIterator);
				
				if((TheShot->GetShooter() == true) && (TheShot->GetShooter().Get() != TheShip))
				{
					if((TheShot->GetPosition() - TheShip->GetPosition()).SquaredLength() < (TheShot->GetRadialSize() * TheShot->GetRadialSize() + TheShip->GetRadialSize() * TheShip->GetRadialSize()))
					{
						Reference< Graphics::ParticleSystem > NewHitParticleSystem(CreateParticleSystem("hit"));
						
						NewHitParticleSystem->SetPosition(TheShot->GetPosition());
						NewHitParticleSystem->SetVelocity((TheShot->GetVelocity() * 0.2f) + (TheShip->GetVelocity() * 0.8f));
						g_ParticleSystemsLayer->AddNode(NewHitParticleSystem.Get());
						TheShip->SetHull(TheShip->GetHull() - TheShot->GetDamage());
						if(TheShip->GetHull() <= 0.0f)
						{
							Reference< Graphics::ParticleSystem > NewExplosionParticleSystem(CreateParticleSystem("explosion"));
							
							NewExplosionParticleSystem->SetPosition(TheShip->GetPosition());
							NewExplosionParticleSystem->SetVelocity(TheShip->GetVelocity() * 0.5f);
							g_ParticleSystemsLayer->AddNode(NewExplosionParticleSystem.Get());
							
							std::set< Object * >::const_iterator ManifestIterator;
							std::set< Object * >::const_iterator NextIterator(TheShip->GetContent().begin());
							
							while(NextIterator != TheShip->GetContent().end())
							{
								ManifestIterator = NextIterator;
								++NextIterator;
								
								Commodity * TheCommodity(dynamic_cast< Commodity * >(*ManifestIterator));
								
								if(TheCommodity != 0)
								{
									TheShip->RemoveContent(TheCommodity);
									TheCommodity->SetPosition(TheShip->GetPosition());
									
									Vector2f VelocityPart(GetRandomFloat(0.1f, 1.2f), GetRandomFloat(0.0f, 2 * M_PI), Vector2f::InitializeMagnitudeAngle);
									
									TheCommodity->SetVelocity(TheShip->GetVelocity() * 0.8f + Vector3f(VelocityPart[0], VelocityPart[1], 0.0f));
									TheShip->GetCurrentSystem()->AddContent(TheCommodity);
									// add visualization of the commodity
									VisualizeCommodity(TheCommodity, g_CommodityLayer);
								}
							}
							DeleteObject(TheShip);
						}
						DeleteObject(TheShot);
						TheShot = 0;
						
						break;
					}
				}
			}
		}
		if(TheShot != 0)
		{
			for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
			{
				Commodity * TheCommodity(*CommodityIterator);
				
				if((TheShot->GetPosition() - TheCommodity->GetPosition()).SquaredLength() < (TheShot->GetRadialSize() * TheShot->GetRadialSize() + TheCommodity->GetRadialSize() * TheCommodity->GetRadialSize()))
				{
					Reference< Graphics::ParticleSystem > NewHitParticleSystem(CreateParticleSystem("hit"));
					
					NewHitParticleSystem->SetPosition(TheShot->GetPosition());
					NewHitParticleSystem->SetVelocity((TheShot->GetVelocity() * 0.4f) + (TheCommodity->GetVelocity() * 0.6f));
					g_ParticleSystemsLayer->AddNode(NewHitParticleSystem.Get());
					TheCommodity->SetHull(TheCommodity->GetHull() - TheShot->GetDamage());
					if(TheCommodity->GetHull() <= 0.0f)
					{
						Reference< Graphics::ParticleSystem > NewHitParticleSystem(CreateParticleSystem("hit"));
						
						NewHitParticleSystem->SetPosition(TheCommodity->GetPosition());
						NewHitParticleSystem->SetVelocity(TheCommodity->GetVelocity() * 0.5f);
						g_ParticleSystemsLayer->AddNode(NewHitParticleSystem.Get());
						DeleteObject(TheCommodity);
					}
					DeleteObject(TheShot);
					TheShot = 0;
					
					break;
				}
			}
		}
		ShotIterator = NextIterator;
	}
}

void UpdateUserInterface(void)
{
	if(g_TimingDialog != 0)
	{
		g_TimingDialog->UpdateFramesPerSecond(g_SystemStatistics->GetFramesPerSecond());
		g_TimingDialog->UpdateAISecondsPerFrame(g_SystemStatistics->GetAISecondsPerFrame());
		g_TimingDialog->UpdateGraphicsSecondsPerFrame(g_SystemStatistics->GetGraphicsSecondsPerFrame());
		g_TimingDialog->UpdatePhysicsSecondsPerFrame(g_SystemStatistics->GetPhysicsSecondsPerFrame());
		g_TimingDialog->UpdateTotalSecondsPerFrame(g_SystemStatistics->GetTotalSecondsPerFrame());
		g_TimingDialog->UpdateTotalSecondsPerFrameProcessing(g_SystemStatistics->GetTotalSecondsPerFrameProcessing());
	}
	if(g_Galaxy != 0)
	{
		g_TimeWarpLabel->SetVisible(true);
	}
	else
	{
		g_TimeWarpLabel->SetVisible(false);
	}
	if((g_OutputMind == true) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0))
	{
		g_TargetLabel->SetVisible(true);
		g_SystemLabel->SetVisible(true);
		g_FuelLabel->SetVisible(true);
		g_HullLabel->SetVisible(true);
		g_CreditsLabel->SetVisible(true);
		g_MiniMap->SetVisible(true);
		g_Scanner->SetVisible(true);
		// display the name of the target
		if(g_OutputMind->GetCharacter()->GetShip()->GetTarget() == true)
		{
			g_TargetLabel->SetString(g_OutputMind->GetCharacter()->GetShip()->GetTarget()->GetName());
		}
		else
		{
			g_TargetLabel->SetString("");
		}
		// display the name of the linked system
		if(g_OutputMind->GetCharacter()->GetShip()->GetLinkedSystemTarget() != 0)
		{
			const std::set< System * > UnexploredSystems(g_OutputMind->GetCharacter()->GetMapKnowledge()->GetUnexploredSystems());
			
			if(UnexploredSystems.find(g_OutputMind->GetCharacter()->GetShip()->GetLinkedSystemTarget()) == UnexploredSystems.end())
			{
				g_SystemLabel->SetString(g_OutputMind->GetCharacter()->GetShip()->GetLinkedSystemTarget()->GetName());
			}
			else
			{
				g_SystemLabel->SetString("Unknown System");
			}
		}
		else
		{
			g_SystemLabel->SetString("");
		}
		// display fuel
		g_FuelLabel->SetString("Fuel: " + to_string_cast(g_OutputMind->GetCharacter()->GetShip()->GetFuel(), 2));
		// display hull
		g_HullLabel->SetString("Hull: " + to_string_cast(g_OutputMind->GetCharacter()->GetShip()->GetHull(), 2));
		// display credits in every cycle
		g_CreditsLabel->SetString("Credits: " + to_string_cast(g_OutputMind->GetCharacter()->GetCredits()));
		// display the current system
		g_CurrentSystemLabel->SetString(g_OutputMind->GetCharacter()->GetShip()->GetCurrentSystem()->GetName());
		// set system label color according to jump status
		if(WantToJump(g_OutputMind->GetCharacter()->GetShip(), g_OutputMind->GetCharacter()->GetShip()->GetLinkedSystemTarget()) == OK)
		{
			g_SystemLabel->GetForegroundColor().Set(0.7f, 0.8f, 1.0f);
		}
		else
		{
			g_SystemLabel->GetForegroundColor().Set(0.4f, 0.4f, 0.4f);
		}
		g_ScannerDisplay->SetOwner(g_OutputMind->GetCharacter()->GetShip()->GetReference());
		g_ScannerDisplay->Update();
		g_MiniMapDisplay->SetOwner(g_OutputMind->GetCharacter()->GetShip()->GetReference());
	}
	else
	{
		g_TargetLabel->SetVisible(false);
		g_SystemLabel->SetVisible(false);
		g_FuelLabel->SetVisible(false);
		g_HullLabel->SetVisible(false);
		g_CreditsLabel->SetVisible(false);
		g_MiniMap->SetVisible(false);
		g_Scanner->SetVisible(false);
	}
}

void PrepareGraphicalOutput(void)
{
	glViewport(0, 0, static_cast< GLsizei >(g_Width), static_cast< GLsizei >(g_Height));
	glMatrixMode(GL_PROJECTION);
	g_MainPerspective.Draw();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	g_Camera.Draw();
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderSystem(System * System)
{
	assert(System != 0);
	
	const Star * CurrentStar(System->GetStar());
	
	if(CurrentStar != 0)
	{
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, Vector4f(CurrentStar->GetPosition().m_V.m_A[0], CurrentStar->GetPosition().m_V.m_A[1], 100.0f, 0.0f).m_V.m_A);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, CurrentStar->GetColor().GetColor().m_V.m_A);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}
	// disable lighting by default, nodes have to activate it if they want it
	glDisable(GL_LIGHTING);
	g_MainScene->Render();
	// HUD
	if((g_OutputMind == true) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0) && (g_OutputMind->GetCharacter()->GetShip()->GetTarget() == true))
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		DrawSelection(g_OutputMind->GetCharacter()->GetShip()->GetTarget().Get(), g_OutputMind->GetCharacter()->GetShip()->GetTarget()->GetRadialSize());
		
		Vector3f RelativePosition(g_OutputMind->GetCharacter()->GetShip()->GetTarget()->GetPosition() - g_OutputMind->GetCharacter()->GetShip()->GetPosition());
		
		RelativePosition.Normalize();
		glPushMatrix();
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glTranslatef(g_OutputMind->GetCharacter()->GetShip()->GetPosition().m_V.m_A[0], g_OutputMind->GetCharacter()->GetShip()->GetPosition().m_V.m_A[1], 0.0f);
		glRotatef(GetRadians(Vector2f(RelativePosition[0], RelativePosition[1])) * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 0.5f, 0.5f);
		glBegin(GL_LINES);
		glVertex2f(20.0f, 0.0f);
		glVertex2f(30.0f, 0.0f);
		glVertex2f(26.0f, 4.0f);
		glVertex2f(30.0f, 0.0f);
		glVertex2f(26.0f, -4.0f);
		glVertex2f(30.0f, 0.0f);
		glEnd();
		glPopAttrib();
		glPopMatrix();
	}
}

void Resize(void)
{
	if(g_Height == 0)
	{
		g_Height = 1;
	}
	glViewport(0, 0, static_cast< GLint >(g_Width), static_cast< GLint >(g_Height));
	g_MainPerspective.SetAspect(g_Width / g_Height);
	g_UserInterface->GetRootWidget()->SetSize(Vector2f(g_Width, g_Height));
}

class GlobalDestroyListener : public DestroyListener
{
public:
	virtual void OnDestroy(Widget * EventSource)
	{
		if(EventSource == g_PlanetDialog)
		{
			g_PlanetDialog = 0;
			g_Pause = false;
		}
		else if(EventSource == g_MapDialog)
		{
			if((g_InputMind == true) && (g_InputMind->GetCharacter() != 0) && (g_InputMind->GetCharacter()->GetShip() != 0) && (g_InputMind->GetCharacter()->GetShip()->GetCurrentSystem()->IsLinkedToSystem(g_MapDialog->GetStarMapDisplay()->GetSelectedSystem()) == true))
			{
				g_InputMind->SelectLinkedSystem(g_MapDialog->GetStarMapDisplay()->GetSelectedSystem());
			}
			g_MapDialog = 0;
			g_Pause = false;
		}
		else if(EventSource == g_MountWeaponDialog)
		{
			g_MountWeaponDialog = 0;
		}
		else if(EventSource == g_SaveGameDialog)
		{
			g_SaveGameDialog = 0;
			g_Pause = false;
		}
		else if(EventSource == g_LoadGameDialog)
		{
			g_LoadGameDialog = 0;
			g_Pause = false;
		}
	}
} g_GlobalDestroyListener;

void EmptySystem(System * System)
{
	if(System != 0)
	{
		while(System->GetShips().empty() == false)
		{
			DeleteObject(System->GetShips().front());
		}
		while(System->GetCommodities().empty() == false)
		{
			DeleteObject(System->GetCommodities().front());
		}
		while(System->GetShots().empty() == false)
		{
			DeleteObject(System->GetShots().front());
		}
	}
}

void SpawnShip(System * System, const std::string & IdentifierSuffix, std::string ShipClassIdentifier = "")
{
	if(ShipClassIdentifier == "")
	{
		if(GetRandomUniform() > 0.5f)
		{
			ShipClassIdentifier = "fighter";
		}
		else
		{
			ShipClassIdentifier = "transporter";
		}
	}
	
	Ship * NewShip(new Ship(g_ShipClassManager->Get(ShipClassIdentifier)));
	
	NewShip->SetObjectIdentifier("::ship(" + NewShip->GetShipClass()->GetIdentifier() + ")" + IdentifierSuffix);
	NewShip->SetPosition(Vector3f(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f), 0.0f));
	NewShip->SetAngularPosition(Quaternion(GetRandomFloat(0.0f, 2.0f * M_PI), Quaternion::InitializeRotationZ));
	
	Vector2f Velocity(GetRandomFloat(0.0f, NewShip->GetShipClass()->GetMaximumSpeed()), GetRandomFloat(0.0f, 2.0f * M_PI), Vector2f::InitializeMagnitudeAngle);
	
	NewShip->SetVelocity(Vector3f(Velocity[0], Velocity[1], 0.0f));
	NewShip->SetFuel(NewShip->GetFuelCapacity());
	NewShip->SetCurrentSystem(System);
	
	Character * NewCharacter(new Character());
	
	NewCharacter->SetObjectIdentifier("::character(" + NewShip->GetShipClass()->GetIdentifier() + ")" + IdentifierSuffix);
	NewCharacter->GetMapKnowledge()->AddExploredSystem(System);
	if(ShipClassIdentifier == "fighter")
	{
		NewCharacter->SetCredits(200 + GetRandomU4Byte(50, 250));
		
		Weapon * NewWeapon(new Weapon(g_WeaponClassManager->Get("light_laser")));
		
		NewWeapon->SetObjectIdentifier("::weapon(" + NewWeapon->GetWeaponClass()->GetIdentifier() + ")::created_for(" + NewShip->GetObjectIdentifier() + ")" + IdentifierSuffix);
		NewShip->AddContent(NewWeapon);
		NewShip->Mount(NewWeapon, "front_gun");
	}
	else if(ShipClassIdentifier == "transporter")
	{
		NewCharacter->SetCredits(GetRandomU4Byte(500, 2500));
		for(int NumberOfAssetClasses = static_cast< int >(GetRandomFloatFromExponentialDistribution(2)); NumberOfAssetClasses > 0; --NumberOfAssetClasses)
		{
			const std::map< std::string, AssetClass * > & AssetClasses(g_AssetClassManager->GetAssetClasses());
			std::map< std::string, AssetClass * >::const_iterator AssetClassIterator(AssetClasses.begin());
			
			for(std::map< std::string, AssetClass * >::size_type Choice = GetRandomInteger(AssetClasses.size() - 1); Choice > 0; --Choice)
			{
				++AssetClassIterator;
			}
			
			int AmountOfAssets(GetRandomIntegerFromExponentialDistribution(NewShip->GetShipClass()->GetMaximumAvailableSpace() / g_ObjectFactory->GetSpaceRequirement(AssetClassIterator->second->GetObjectType(), AssetClassIterator->second->GetObjectClass())));
			
			if(AmountOfAssets <= NewShip->GetAvailableSpace())
			{
				while(AmountOfAssets > 0)
				{
					Object * NewCommodity(g_ObjectFactory->Create(AssetClassIterator->second->GetObjectType(), AssetClassIterator->second->GetObjectClass()));
					
					NewCommodity->SetObjectIdentifier("::" + AssetClassIterator->second->GetObjectType() + "(" + AssetClassIterator->second->GetIdentifier() + ")::(" + to_string_cast(NumberOfAssetClasses) + "|" + to_string_cast(AmountOfAssets) + ")" + IdentifierSuffix);
					NewShip->AddContent(NewCommodity);
					--AmountOfAssets;
				}
			}
		}
	}
	if(GetRandomBoolean(0.2) == true)
	{
		NewShip->SetHull(GetRandomFloat(0.1f * NewShip->GetShipClass()->GetHull(), 0.9f * NewShip->GetShipClass()->GetHull()));
	}
	NewShip->SetFuel(GetRandomFloat(0.1f * NewShip->GetShipClass()->GetFuelHoldSize(), 0.8f * NewShip->GetShipClass()->GetFuelHoldSize()));
	NewCharacter->SetShip(NewShip);
	if(ShipClassIdentifier == "transporter")
	{
		StateMachineMind * NewMind(new StateMachineMind());
		
		NewMind->SetObjectIdentifier("::mind(state_machine)" + IdentifierSuffix);
		NewMind->SetCharacter(NewCharacter);
		NewMind->GetStateMachine()->SetState(new SelectSteering(NewMind));
		NewMind->GetStateMachine()->SetGlobalState(new MonitorFuel(NewMind));
		NewCharacter->AddContent(NewMind);
	}
	else
	{
		GoalMind * NewMind(new GoalMind());
		
		NewMind->SetObjectIdentifier("::mind(goal)" + IdentifierSuffix);
		NewMind->SetCharacter(NewCharacter);
		NewMind->AddContent(new GoalFighterThink(NewMind));
		NewCharacter->AddContent(NewMind);
	}
	NewShip->AddContent(NewCharacter);
	System->AddContent(NewShip);
	// add visualization
	VisualizeShip(NewShip, g_ShipLayer);
}

void SpawnShipOnTimeout(System * SpawnInSystem)
{
	std::stringstream IdentifierSuffix;
	
	IdentifierSuffix << "::created_at_game_time(" << std::fixed << GameTime::Get() << ")::in_system(" << SpawnInSystem->GetIdentifier() << ")";
	
	SpawnShip(SpawnInSystem, IdentifierSuffix.str());
	g_SpawnShipTimeoutNotification = g_GameTimeTimeoutNotifications->Add(GameTime::Get() + GetRandomFloatFromExponentialDistribution(1.0f / SpawnInSystem->GetTrafficDensity()), Bind1(Function(SpawnShipOnTimeout), SpawnInSystem));
}

void PopulateSystem(System * System)
{
	int NumberOfShips(GetRandomInteger(5));
	
	for(int ShipNumber = 1; ShipNumber <= NumberOfShips; ++ShipNumber)
	{
		std::stringstream IdentifierSuffix;
		
		IdentifierSuffix << "::created_at_game_time(" << std::fixed << GameTime::Get() << "[" << ShipNumber << "])::in_system(" << System->GetIdentifier() << ")";
		
		SpawnShip(System, IdentifierSuffix.str());
	}
}

void OnOutputEnterSystem(System * EnterSystem)
{
	assert(g_SpawnShipTimeoutNotification.IsValid() == false);
	assert(g_MainScene == 0);
	g_SpawnShipTimeoutNotification = g_GameTimeTimeoutNotifications->Add(GameTime::Get() + GetRandomFloatFromExponentialDistribution(1.0f / EnterSystem->GetTrafficDensity()), Bind1(Function(SpawnShipOnTimeout), EnterSystem));
	// build the static setup of the scene
	g_MainScene = new Graphics::Scene();
	g_GraphicsEngine->AddScene(g_MainScene);
	g_CommodityLayer = new Graphics::Node();
	g_ParticleSystemsLayer = new Graphics::Node();
	g_PlanetLayer = new Graphics::Node();
	g_ShipLayer = new Graphics::Node();
	g_ShotLayer = new Graphics::Node();
	g_MainScene->AddNode(g_PlanetLayer);
	g_MainScene->AddNode(g_CommodityLayer);
	g_MainScene->AddNode(g_ShotLayer);
	g_MainScene->AddNode(g_ShipLayer);
	g_MainScene->AddNode(g_ParticleSystemsLayer);
	
	// add visualizations for all objects in the system
	const std::vector< Planet * > & Planets(EnterSystem->GetPlanets());
	
	for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
	{
		VisualizePlanet(*PlanetIterator, g_PlanetLayer);
	}
	
	const std::list< Ship * > & Ships(EnterSystem->GetShips());
	
	for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
	{
		VisualizeShip(*ShipIterator, g_ShipLayer);
	}
}

void OnOutputLeaveSystem(System * System)
{
	assert(System != 0);
	if(g_SpawnShipTimeoutNotification.IsValid() == true)
	{
		g_SpawnShipTimeoutNotification.Dismiss();
	}
	// clear scene
	g_GraphicsEngine->RemoveScene(g_MainScene);
	g_MainScene->Destroy();
}

void OnGraphicsNodeDestroy(Graphics::Node * Node)
{
	if(Node == g_CommodityLayer)
	{
		g_CommodityLayer = 0;
	}
	else if(Node == g_ParticleSystemsLayer)
	{
		g_ParticleSystemsLayer = 0;
	}
	else if(Node == g_PlanetLayer)
	{
		g_PlanetLayer = 0;
	}
	else if(Node == g_ShipLayer)
	{
		g_ShipLayer = 0;
	}
	else if(Node == g_ShotLayer)
	{
		g_ShotLayer = 0;
	}
	else if(Node == g_MainScene)
	{
		g_MainScene = 0;
	}
	else
	{
		Object * TheObject(Object::GetObject(Node));
		
		if(TheObject != 0)
		{
			assert(TheObject->GetVisualization() == Node);
			TheObject->UnsetVisualization();
		}
	}
	
	std::map< Graphics::Node *, Reference< Graphics::Node > >::iterator VisualizationReferenceIterator(g_VisualizationReferences.find(Node));
	
	if(VisualizationReferenceIterator != g_VisualizationReferences.end())
	{
		VisualizationReferenceIterator->second.Invalidate();
		g_VisualizationReferences.erase(VisualizationReferenceIterator);
	}
	
	delete Node;
}

std::string MakeTimeStampedFileName(const std::string & Name, const std::string & Extension)
{
	time_t RawTime(time(0));
	struct tm Time;
	
	localtime_r(&RawTime, &Time);
	
	char DateTime[16];
	
	strftime(DateTime, 16, "%Y%m%d-%H%M%S", &Time);
	
	return Name + '-' + DateTime + '.' + Extension;
}

void TakeScreenShot(void)
{
	GLubyte * ScreenshotData(new GLubyte[static_cast< GLsizei >(g_Width) * static_cast< GLsizei >(g_Height) * 3]);
	
	// store the read buffer and change it to the front buffer so we get what we see
	glPushAttrib(GL_PIXEL_MODE_BIT);
	glReadBuffer(GL_FRONT);
	// now read the bits
	glReadPixels(0, 0, static_cast< GLsizei >(g_Width), static_cast< GLsizei >(g_Height), GL_RGB, GL_UNSIGNED_BYTE, ScreenshotData);
	// revert the read buffer
	glPopAttrib();
	
	// the file name with the current datetime in the format YYYYMMDD-HHMMSS
	std::ofstream OutputFileStream(MakeTimeStampedFileName("screenshot", "tex").c_str());
	
	if(OutputFileStream)
	{
		uint32_t Value;
		
		Value = htonl(static_cast< GLsizei >(g_Width));
		OutputFileStream.write(reinterpret_cast< const char * >(&Value), 4);
		Value = htonl(static_cast< GLsizei >(g_Height));
		OutputFileStream.write(reinterpret_cast< const char * >(&Value), 4);
		Value = htonl(2); // this is the tex image data format 2: RGB
		OutputFileStream.write(reinterpret_cast< const char * >(&Value), 4);
		OutputFileStream.write(reinterpret_cast< const char * >(ScreenshotData), static_cast< GLsizei >(g_Width) * static_cast< GLsizei >(g_Height) * 3);
	}
}

void GameFrame(void)
{
	RealTime::Invalidate();
	
	static double FrameTimeBegin(RealTime::Get());
	double FrameTimeEnd(RealTime::Get());
	double FrameTimeDelta(FrameTimeEnd - FrameTimeBegin);
	
	// now set the next begin time to the current end time
	FrameTimeBegin = FrameTimeEnd;
	
	double FrameProcessingTimeBegin(RealTime::Get());
	System * CurrentSystem(g_CurrentSystem);
	
	g_GameTimeTimeoutNotifications->Process(GameTime::Get());
	g_RealTimeTimeoutNotifications->Process(RealTime::Get());
	RealTime::Invalidate();
	
	double AITimeBegin(RealTime::Get());
	
	CalculateCharacters();
	RealTime::Invalidate();
	
	double AITimeEnd(RealTime::Get());
	double AITimeDelta(AITimeEnd - AITimeBegin);
	double PhysicsTimeBegin(RealTime::Get());
	float Seconds(CalculateTime());
	
	if(g_MainScene != 0)
	{
		g_MainScene->Update(Seconds);
	}
	if(CurrentSystem != 0)
	{
		CalculateMovements(CurrentSystem, Seconds);
	}
	RealTime::Invalidate();
	
	double PhysicsTimeEnd(RealTime::Get());
	double PhysicsTimeDelta(PhysicsTimeEnd - PhysicsTimeBegin);
	
	UpdateUserInterface();
	RealTime::Invalidate();
	
	double GraphicsTimeBegin(RealTime::Get());
	
	PrepareGraphicalOutput();
	if(CurrentSystem != 0)
	{
		RenderSystem(CurrentSystem);
	}
	DisplayUserInterface();
	RealTime::Invalidate();
	
	double GraphicsTimeEnd(RealTime::Get());
	double GraphicsTimeDelta(GraphicsTimeEnd - GraphicsTimeBegin);
	
	if(g_TakeScreenShot == true)
	{
		TakeScreenShot();
		g_TakeScreenShot = false;
	}
	if((g_OutputMind == true) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0) && (g_OutputMind->GetCharacter()->GetShip()->GetCurrentSystem() != g_CurrentSystem))
	{
		OnOutputLeaveSystem(g_CurrentSystem);
		g_CurrentSystem = g_OutputMind->GetCharacter()->GetShip()->GetCurrentSystem();
		OnOutputEnterSystem(g_CurrentSystem);
		EmptySystem(CurrentSystem);
		PopulateSystem(g_CurrentSystem);
	}
	RealTime::Invalidate();
	
	double FrameProcessingTimeEnd(RealTime::Get());
	double FrameProcessingTimeDelta(FrameProcessingTimeEnd - FrameProcessingTimeBegin);
	
	g_SystemStatistics->SetFramesPerSecond(1.0f / FrameTimeDelta);
	g_SystemStatistics->SetAISecondsPerFrame(AITimeDelta);
	g_SystemStatistics->SetGraphicsSecondsPerFrame(GraphicsTimeDelta);
	g_SystemStatistics->SetPhysicsSecondsPerFrame(PhysicsTimeDelta);
	g_SystemStatistics->SetTotalSecondsPerFrame(FrameTimeDelta);
	g_SystemStatistics->SetTotalSecondsPerFrameProcessing(FrameProcessingTimeDelta);
}

void SetTimeWarp(float TimeWarp)
{
	g_TimeWarp = TimeWarp;
	
	std::stringstream TimeWarpString;
	
	TimeWarpString << "Time Warp: " << to_string_cast(g_TimeWarp, 2);
	g_TimeWarpLabel->SetString(TimeWarpString.str());
}

void MouseButtonDown(int MouseButton, int X, int Y)
{
	if(g_UserInterface->MouseButton(MouseButton, EV_DOWN, X, Y) == true)
	{
		return;
	}
	switch(MouseButton)
	{
	case 4: // MouseButton: WHEEL_UP
		{
			g_Camera.MoveIn();
			
			break;
		}
	case 5: // MouseButton: WHEEL_DOWN
		{
			g_Camera.MoveOut();
			
			break;
		}
	case 2: // MouseButton: MIDDLE
		{
			g_LastMotionX = X;
			g_LastMotionY = Y;
			g_MouseButton = 2;
			
			break;
		}
	}
}

void MouseButtonUp(int MouseButton, int X, int Y)
{
	if(g_UserInterface->MouseButton(MouseButton, EV_UP, X, Y) == true)
	{
		return;
	}
	switch(MouseButton)
	{
	case 2: // MouseButton: MIDDLE
		{
			g_MouseButton = -1;
			
			break;
		}
	}
}

void MouseMotion(int X, int Y)
{
	g_UserInterface->MouseMotion(X, Y);
	
	int DeltaX(X - g_LastMotionX);
	int DeltaY(Y - g_LastMotionY);
	
	g_LastMotionX = X;
	g_LastMotionY = Y;
	if(g_MouseButton == 2) // MouseButton: MIDDLE
	{
		const Vector3f & CameraPosition(g_Camera.GetPosition());
		
		g_Camera.SetPosition(CameraPosition.m_V.m_A[0] - static_cast< float >(DeltaX) * 0.0008f * CameraPosition.m_V.m_A[2], CameraPosition.m_V.m_A[1] + static_cast< float >(DeltaY) * 0.0008f * CameraPosition.m_V.m_A[2]);
	}
}

void PurgeGame(void)
{
	if(g_CurrentSystem != 0)
	{
		OnOutputLeaveSystem(g_CurrentSystem);
		EmptySystem(g_CurrentSystem);
	}
	if(g_Galaxy != 0)
	{
		g_Galaxy->Destroy();
		delete g_Galaxy;
	}
	g_CurrentSystem = 0;
	g_Galaxy = 0;
}

void LoadGameFromElement(const Element * SaveElement)
{
	if(SaveElement->GetName() != "save")
	{
		throw std::runtime_error("The savegame is not rooted at a \"save\" element.");
	}
	
	const std::vector< Element * > & SaveChilds(SaveElement->GetChilds());
	std::string CurrentSystem;
	std::string InputMindObjectIdentifier;
	std::string OutputMindObjectIdentifier;
	
	// setup the game world
	PurgeGame();
	g_Galaxy = new Galaxy();
	g_Galaxy->SetObjectIdentifier("::galaxy");
	g_ResourceReader->ReadSystems();
	g_ResourceReader->ReadSystemLinks();
	for(std::vector< Element * >::const_iterator SaveChild = SaveChilds.begin(); SaveChild != SaveChilds.end(); ++SaveChild)
	{
		if((*SaveChild)->GetName() == "current-system")
		{
			CurrentSystem = (*SaveChild)->GetAttribute("identifier");
		}
		else if((*SaveChild)->GetName() == "time-warp")
		{
			SetTimeWarp(from_string_cast< float >((*SaveChild)->GetAttribute("value")));
		}
		else if((*SaveChild)->GetName() == "input-mind")
		{
			InputMindObjectIdentifier = (*SaveChild)->GetAttribute("object-identifier");
		}
		else if((*SaveChild)->GetName() == "output-mind")
		{
			OutputMindObjectIdentifier = (*SaveChild)->GetAttribute("object-identifier");
		}
		else if((*SaveChild)->GetName() == "mind")
		{
			Mind * NewMind(0);
			
			if((*SaveChild)->GetAttribute("class") == "command-mind")
			{
				NewMind = new CommandMind();
			}
			else
			{
				throw std::runtime_error("The \"mind\" element \"" + (*SaveChild)->GetAttribute("object-identifier") + "\" contains an unidentified class attribute \"" + (*SaveChild)->GetAttribute("class") + "\".");
			}
			NewMind->SetObjectIdentifier((*SaveChild)->GetAttribute("object-identifier"));
			for(std::vector< Element * >::const_iterator MindChild = (*SaveChild)->GetChilds().begin(); MindChild != (*SaveChild)->GetChilds().end(); ++MindChild)
			{
				if((*MindChild)->GetName() == "character")
				{
					// on second pass
				}
				else
				{
					throw std::runtime_error("The \"mind\" element \"" + (*SaveChild)->GetAttribute("object-identifier") + "\" contains an unidentified child element \"" + (*MindChild)->GetName() + "\".");
				}
			}
		}
		else if((*SaveChild)->GetName() == "character")
		{
			Character * TheCharacter(new Character());
			
			TheCharacter->SetObjectIdentifier((*SaveChild)->GetAttribute("object-identifier"));
			for(std::vector< Element * >::const_iterator CharacterChild = (*SaveChild)->GetChilds().begin(); CharacterChild != (*SaveChild)->GetChilds().end(); ++CharacterChild)
			{
				if((*CharacterChild)->GetName() == "credits")
				{
					TheCharacter->SetCredits(from_string_cast< u4byte >((*CharacterChild)->GetAttribute("value")));
				}
				else if((*CharacterChild)->GetName() == "map-knowledge")
				{
					for(std::vector< Element * >::const_iterator MapKnowledgeChild = (*CharacterChild)->GetChilds().begin(); MapKnowledgeChild != (*CharacterChild)->GetChilds().end(); ++MapKnowledgeChild)
					{
						if((*MapKnowledgeChild)->GetName() == "explored-system")
						{
							TheCharacter->GetMapKnowledge()->AddExploredSystem(g_Galaxy->GetSystem((*MapKnowledgeChild)->GetAttribute("identifier")));
						}
					}
				}
				else if((*CharacterChild)->GetName() == "ship")
				{
					// in second pass
				}
				else
				{
					throw std::runtime_error("The \"character\" element \"" + (*SaveChild)->GetAttribute("object-identifier") + "\" contains an unidentified child element \"" + (*CharacterChild)->GetName() + "\".");
				}
			}
		}
		else if((*SaveChild)->GetName() == "ship")
		{
			Ship * TheShip(new Ship(g_ShipClassManager->Get((*SaveChild)->GetAttribute("class-identifier"))));
			
			TheShip->SetObjectIdentifier((*SaveChild)->GetAttribute("object-identifier"));
			for(std::vector< Element * >::const_iterator ShipChild = (*SaveChild)->GetChilds().begin(); ShipChild != (*SaveChild)->GetChilds().end(); ++ShipChild)
			{
				if((*ShipChild)->GetName() == "fuel")
				{
					TheShip->SetFuel(from_string_cast< float >((*ShipChild)->GetAttribute("value")));
				}
				else if((*ShipChild)->GetName() == "hull")
				{
					TheShip->SetHull(from_string_cast< float >((*ShipChild)->GetAttribute("value")));
				}
				else if((*ShipChild)->GetName() == "position")
				{
					TheShip->SetPosition(Vector3f(from_string_cast< float >((*ShipChild)->GetAttribute("x")), from_string_cast< float >((*ShipChild)->GetAttribute("y")), 0.0f));
				}
				else if((*ShipChild)->GetName() == "velocity")
				{
					TheShip->SetVelocity(Vector3f(from_string_cast< float >((*ShipChild)->GetAttribute("x")), from_string_cast< float >((*ShipChild)->GetAttribute("y")), 0.0f));
				}
				else if((*ShipChild)->GetName() == "angular-position")
				{
					TheShip->SetAngularPosition(Quaternion(from_string_cast< float >((*ShipChild)->GetAttribute("w")), from_string_cast< float >((*ShipChild)->GetAttribute("x")), from_string_cast< float >((*ShipChild)->GetAttribute("y")), from_string_cast< float >((*ShipChild)->GetAttribute("z"))));
				}
				else if((*ShipChild)->GetName() == "content")
				{
					for(std::vector< Element * >::const_iterator ContentChild = (*ShipChild)->GetChilds().begin(); ContentChild != (*ShipChild)->GetChilds().end(); ++ContentChild)
					{
						if((*ContentChild)->GetName() == "commodity")
						{
							TheShip->AddContent(g_ObjectFactory->Create((*ContentChild)->GetName(), (*ContentChild)->GetAttribute("class-identifier")));
						}
						else if((*ContentChild)->GetName() == "weapon")
						{
							Weapon * NewWeapon(new Weapon(g_WeaponClassManager->Get((*ContentChild)->GetAttribute("class-identifier"))));
							
							NewWeapon->SetObjectIdentifier((*ContentChild)->GetAttribute("object-identifier"));
							TheShip->AddContent(NewWeapon);
							if((*ContentChild)->HasAttribute("mounted-on") == true)
							{
								TheShip->Mount(NewWeapon, (*ContentChild)->GetAttribute("mounted-on"));
							}
						}
						else
						{
							throw std::runtime_error("The \"content\" element of the \"ship\" element \"" + (*SaveChild)->GetAttribute("object-identifier") + "\" contains a child element \"" + (*ContentChild)->GetName() + "\" which could not be handled.");
						}
					}
				}
				else if((*ShipChild)->GetName() == "name")
				{
					TheShip->SetName((*ShipChild)->GetAttribute("value"));
				}
				else if((*ShipChild)->GetName() == "system")
				{
					System * TheSystem(g_Galaxy->GetSystem((*ShipChild)->GetAttribute("identifier")));
					
					TheSystem->AddContent(TheShip);
					TheShip->SetCurrentSystem(TheSystem);
				}
				else
				{
					throw std::runtime_error("The \"ship\" element \"" + (*SaveChild)->GetAttribute("object-identifier") + "\" contains an unidentified child element \"" + (*ShipChild)->GetName() + "\".");
				}
			}
		}
		else if((*SaveChild)->GetName() == "main-camera")
		{
			for(std::vector< Element * >::const_iterator CameraChild = (*SaveChild)->GetChilds().begin(); CameraChild != (*SaveChild)->GetChilds().end(); ++CameraChild)
			{
				if((*CameraChild)->GetName() == "position")
				{
					g_Camera.SetPosition(from_string_cast< float >((*CameraChild)->GetAttribute("x")), from_string_cast< float >((*CameraChild)->GetAttribute("y")), from_string_cast< float >((*CameraChild)->GetAttribute("z")));
				}
				else if((*CameraChild)->GetName() == "focus")
				{
					Position * FocusPosition(dynamic_cast< Position * >(Object::GetObject((*CameraChild)->GetAttribute("object-identifier"))));
					
					if(FocusPosition == 0)
					{
						throw std::runtime_error("The \"main-camera\" element focusses an object with a non-existing identifier \"" + (*CameraChild)->GetAttribute("object-identifier") + "\".");
					}
					g_Camera.SetFocus(FocusPosition->GetReference());
				}
				else if((*CameraChild)->GetName() == "field-of-view")
				{
					if((*CameraChild)->HasAttribute("degree") == true)
					{
						g_Camera.SetFieldOfView(from_string_cast< float >((*CameraChild)->GetAttribute("degree")) * M_PI / 360.0f);
					}
					else if((*CameraChild)->HasAttribute("radians") == true)
					{
						g_Camera.SetFieldOfView(from_string_cast< float >((*CameraChild)->GetAttribute("radians")));
					}
					g_MainPerspective.SetFieldOfView(g_Camera.GetFieldOfView());
				}
				else
				{
					throw std::runtime_error("The \"main-camera\" element contains an unidentified child element \"" + (*CameraChild)->GetName() + "\".");
				}
			}
		}
		else
		{
			throw std::runtime_error("The \"save\" element contains an unidentified child element \"" + (*SaveChild)->GetName() + "\".");
		}
	}
	// in the second pass we do a fast skip over the childs to resolve any object references
	// no errors except resolving errors will be displayed
	// here, all th dynamic_casts are done
	// at the moment this also resolves back references/containments
	/// @todo The back references/containments should be dealt with by save the galaxy's structure recursively
	for(std::vector< Element * >::const_iterator SaveChild = SaveChilds.begin(); SaveChild != SaveChilds.end(); ++SaveChild)
	{
		if((*SaveChild)->GetName() == "mind")
		{
			Mind * TheMind(dynamic_cast< Mind * >(Object::GetObject((*SaveChild)->GetAttribute("object-identifier"))));
			
			for(std::vector< Element * >::const_iterator MindChild = (*SaveChild)->GetChilds().begin(); MindChild != (*SaveChild)->GetChilds().end(); ++MindChild)
			{
				if((*MindChild)->GetName() == "character")
				{
					Character * TheCharacter(dynamic_cast< Character * >(Object::GetObject((*MindChild)->GetAttribute("object-identifier"))));
					
					TheMind->SetCharacter(TheCharacter);
					TheCharacter->AddContent(TheMind);
				}
			}
		}
		else if((*SaveChild)->GetName() == "character")
		{
			Character * TheCharacter(dynamic_cast< Character * >(Object::GetObject((*SaveChild)->GetAttribute("object-identifier"))));
			
			for(std::vector< Element * >::const_iterator CharacterChild = (*SaveChild)->GetChilds().begin(); CharacterChild != (*SaveChild)->GetChilds().end(); ++CharacterChild)
			{
				if((*CharacterChild)->GetName() == "ship")
				{
					Ship * TheShip(dynamic_cast< Ship * >(Object::GetObject((*CharacterChild)->GetAttribute("object-identifier"))));
					
					TheCharacter->SetShip(TheShip);
					TheShip->AddContent(TheCharacter);
				}
			}
		}
	}
	assert(CurrentSystem.empty() == false);
	g_CurrentSystem = g_Galaxy->GetSystem(CurrentSystem);
	assert(g_CurrentSystem != 0);
	if(InputMindObjectIdentifier.empty() == false)
	{
		g_InputMind = Object::GetObject(InputMindObjectIdentifier)->GetReference();
	}
	if(OutputMindObjectIdentifier.empty() == false)
	{
		g_OutputMind = Object::GetObject(OutputMindObjectIdentifier)->GetReference();
	}
	OnOutputEnterSystem(g_CurrentSystem);
	RealTime::Invalidate();
	PopulateSystem(g_CurrentSystem);
	// setting up the player environment
	if((g_OutputMind == true) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0))
	{
		g_MiniMapDisplay->SetOwner(g_OutputMind->GetCharacter()->GetShip()->GetReference());
		g_ScannerDisplay->SetOwner(g_OutputMind->GetCharacter()->GetShip()->GetReference());
	}
}

bool LoadGameFromInputStream(std::istream & InputStream)
{
	Document Document(InputStream);
	
	if(Document.GetDocumentElement() == 0)
	{
		std::cerr << "The game file does not seem to be parsable." << std::endl;
		
		return false;
	}
	else
	{
		try
		{
			LoadGameFromElement(Document.GetDocumentElement());
		}
		catch(std::runtime_error & Exception)
		{
			std::cerr << "The game file contains invalid data:\n\t" << Exception.what() << std::endl;
			
			return false;
		}
		
		return true;
	}
}

bool LoadGameFromFileName(const std::string & FileName)
{
	std::ifstream InputFileStream(FileName.c_str());
	
	if(InputFileStream == false)
	{
		std::cerr << "The savegame file \"" << FileName << "\" does not exist or is not readable." << std::endl;
		
		return false;
	}
	
	return LoadGameFromInputStream(InputFileStream);
}

void SaveGame(std::ostream & OStream)
{
	XMLStream XML(OStream);
	
	XML << element << "save";
	
	if(g_CurrentSystem != 0)
	{
		XML << element << "current-system" << attribute << "identifier" << value << g_CurrentSystem->GetIdentifier() << end;
	}
	XML << element << "time-warp" << attribute << "value" << value << g_TimeWarp << end;
	if(g_InputMind == true)
	{
		XML << element << "input-mind" << attribute << "object-identifier" << value << g_InputMind->GetObjectIdentifier() << end;
	}
	if(g_OutputMind == true)
	{
		XML << element << "output-mind" << attribute << "object-identifier" << value << g_OutputMind->GetObjectIdentifier() << end;
	}
	if(g_InputMind == true)
	{
		// save the input mind
		XML << element << "mind" << attribute << "class" << value << "command-mind" << attribute << "object-identifier" << value << g_InputMind->GetObjectIdentifier();
		if(g_InputMind->GetCharacter() != 0)
		{
			XML << element << "character" << attribute << "object-identifier" << value << g_InputMind->GetCharacter()->GetObjectIdentifier() << end;
		}
		XML << end;
		// save the character for the input mind
		if(g_InputMind->GetCharacter() != 0)
		{
			XML << element << "character" << attribute << "object-identifier" << value << g_InputMind->GetCharacter()->GetObjectIdentifier();
			if(g_InputMind->GetCharacter()->GetShip() != 0)
			{
				XML << element << "ship" << attribute << "object-identifier" << value << g_InputMind->GetCharacter()->GetShip()->GetObjectIdentifier() << end;
			}
			XML << element << "credits" << attribute << "value" << value << g_InputMind->GetCharacter()->GetCredits() << end;
			XML << element << "map-knowledge";
			
			const std::set< System * > & ExploredSystems(g_InputMind->GetCharacter()->GetMapKnowledge()->GetExploredSystems());
			
			for(std::set< System * >::const_iterator ExploredSystemIterator = ExploredSystems.begin(); ExploredSystemIterator != ExploredSystems.end(); ++ExploredSystemIterator)
			{
				XML << element << "explored-system" << attribute << "identifier" << value << (*ExploredSystemIterator)->GetIdentifier() << end;
			}
			XML << end; // map-knowledge
			XML << end; // character
			if(g_InputMind->GetCharacter()->GetShip() != 0)
			{
				Ship * Ship(g_InputMind->GetCharacter()->GetShip());
				
				XML << element << "ship" << attribute << "class-identifier" << value << Ship->GetShipClass()->GetIdentifier() << attribute << "object-identifier" << value << Ship->GetObjectIdentifier();
				XML << element << "system" << attribute << "identifier" << value << g_InputMind->GetCharacter()->GetShip()->GetCurrentSystem()->GetIdentifier() << end;
				XML << element << "fuel" << attribute << "value" << value << Ship->GetFuel() << end;
				XML << element << "hull" << attribute << "value" << value << Ship->GetHull() << end;
				XML << element << "position" << attribute << "x" << value << Ship->GetPosition().m_V.m_A[0] << attribute << "y" << value << Ship->GetPosition().m_V.m_A[1] << end;
				XML << element << "angular-position" << attribute << "w" << value << Ship->GetAngularPosition()[0] << attribute << "x" << value << Ship->GetAngularPosition()[1] << attribute << "y" << value << Ship->GetAngularPosition()[2] << attribute << "z" << value << Ship->GetAngularPosition()[3] << end;
				XML << element << "velocity" << attribute << "x" << value << Ship->GetVelocity().m_V.m_A[0] << attribute << "y" << value << Ship->GetVelocity().m_V.m_A[1] << end;
				XML << element << "content";
				
				std::set< Object * >::const_iterator ContentIterator(Ship->GetContent().begin());
				
				while(ContentIterator != Ship->GetContent().end())
				{
					Commodity * TheCommodity(dynamic_cast< Commodity * >(*ContentIterator));
					Weapon * TheWeapon(dynamic_cast< Weapon * >(*ContentIterator));
					
					if(TheCommodity != 0)
					{
						XML << element << "commodity" << attribute << "class-identifier" << value << TheCommodity->GetCommodityClass()->GetIdentifier() << end;
					}
					else if(TheWeapon != 0)
					{
						XML << element << "weapon" << attribute << "object-identifier" << value << TheWeapon->GetObjectIdentifier() << attribute << "class-identifier" << value << TheWeapon->GetWeaponClass()->GetIdentifier();
						if(TheWeapon->GetSlot() != 0)
						{
							XML << attribute << "mounted-on" << value << TheWeapon->GetSlot()->GetIdentifier();
						}
						XML << end;
					}
					++ContentIterator;
				}
				XML << end; // content
				XML << element << "name" << attribute << "value" << value << Ship->GetName() << end;
				XML << end; // ship
			}
		}
	}
	// save main camera properties
	XML << element << "main-camera";
	XML << element << "position" << attribute << "x" << value << g_Camera.GetPosition().m_V.m_A[0] << attribute << "y" << value << g_Camera.GetPosition().m_V.m_A[1] << attribute << "z" << value << g_Camera.GetPosition().m_V.m_A[2] << end;
	if(g_Camera.GetFocus() != 0)
	{
		if(g_Camera.GetFocus()->GetObjectIdentifier() == "")
		{
			g_Camera.GetFocus()->GenerateObjectIdentifier();
		}
		XML << element << "focus" << attribute << "object-identifier" << value << g_Camera.GetFocus()->GetObjectIdentifier() << end;
	}
	XML << element << "field-of-view" << attribute << "radians" << value << g_Camera.GetFieldOfView() << end;
	XML << end; // camera
	XML << end; // save
	OStream << std::endl;
}

void KeyEvent(const KeyEventInformation & KeyEventInformation)
{
	if(g_UserInterface->Key(KeyEventInformation) == true)
	{
		return;
	}
	switch(KeyEventInformation.GetKeyCode())
	{
	case 9:  // Key: ESCAPE
		{
			if(KeyEventInformation.IsDown() == true)
			{
				g_Quit = true;
			}
			
			break;
		}
	case 22: // Key: BACKSPACE
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					DeleteObject(g_InputMind->GetCharacter()->GetShip());
				}
				else if(g_OutputMind == true)
				{
					DeleteObject(g_OutputMind->GetCharacter()->GetShip());
				}
			}
			
			break;
		}
	case 23: // Key: TABULATOR
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->Refuel();
				}
			}
			
			break;
		}
	case 24: // Key: Q
		{
			if(KeyEventInformation.IsDown() == true)
			{
				g_DumpEndReport = true;
				g_Quit = true;
			}
			
			break;
		}
	case 25: // Key: W
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if((g_MountWeaponDialog == 0) && (g_OutputMind == true))
				{
					g_MountWeaponDialog = new MountWeaponDialog(g_UserInterface->GetRootWidget(), g_OutputMind->GetCharacter()->GetShip());
					g_MountWeaponDialog->GrabKeyFocus();
					g_MountWeaponDialog->AddDestroyListener(&g_GlobalDestroyListener);
				}
			}
			
			break;
		}
	case 26: // Key: E
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->TargetPreviousShip();
				}
			}
			
			break;
		}
	case 27: // Key: R
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->TargetNextShip();
				}
			}
			
			break;
		}
	case 32: // Key: O
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->TargetPreviousPlanet();
				}
			}
			
			break;
		}
	case 33: // Key: P
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->TargetNextPlanet();
				}
			}
			
			break;
		}
	case 35: // Key: ]
		{
			if(KeyEventInformation.IsDown() == true)
			{
				const std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
				const Ship * FocusShip(dynamic_cast< const Ship * >(g_Camera.GetFocus().Get()));
				
				std::list< Ship * >::const_iterator ShipIterator(find(Ships.begin(), Ships.end(), FocusShip));
				
				if((ShipIterator == Ships.end()) || (++ShipIterator == Ships.end()))
				{
					g_Camera.SetFocus(Ships.front()->GetReference());
				}
				else
				{
					g_Camera.SetFocus((*ShipIterator)->GetReference());
				}
			}
			
			break;
		}
	case 37: // Key: LEFT CONTROL
		{
			if(KeyEventInformation.IsDown() == true)
			{
				std::stringstream IdentifierPrefix;
				
				IdentifierPrefix << "::system(" << g_CurrentSystem->GetIdentifier() << ")::created_at_game_time(" << std::fixed << GameTime::Get() << ")";
				SpawnShip(g_CurrentSystem, IdentifierPrefix.str(), "fighter");
			}
			
			break;
		}
	case 38: // Key: A
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->TargetNearestCargo();
				}
			}
			
			break;
		}
	case 39: // Key: S
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					switch(WantToScoop(g_InputMind->GetCharacter()->GetShip(), dynamic_cast< const Commodity * >(g_InputMind->GetCharacter()->GetShip()->GetTarget().Get())))
					{
					case OK:
						{
							g_InputMind->Scoop();
							
							break;
						}
					case NO_SCOOP_TARGET:
						{
							g_InputMind->TargetNearestCargo();
							
							break;
						}
					case NOT_ENOUGH_SPACE:
						{
							SetMessage("Your cargo hold has not enough spare space to accomodate this item.");
							
							break;
						}
					case TOO_FAR_AWAY:
						{
							SetMessage("You are too far away from the cargo to scoop it up.");
							
							break;
						}
					case TOO_HIGH_RELATIVE_VELOCITY:
						{
							SetMessage("Your relative velocity to the cargo is too high to scoop it up.");
							
							break;
						}
					}
				}
			}
			
			break;
		}
	case 40: // Key: D
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->TargetPreviousCargo();
				}
			}
			
			break;
		}
	case 41: // Key: F
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->TargetNextCargo();
				}
			}
			
			break;
		}
	case 44: // Key: J
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					switch(WantToJump(g_InputMind->GetCharacter()->GetShip(), g_InputMind->GetCharacter()->GetShip()->GetLinkedSystemTarget()))
					{
					case OK:
						{
							g_InputMind->Jump();
							
							break;
						}
					case TOO_NEAR_TO_STELLAR_OBJECT:
						{
							SetMessage("You are too near to a stellar object to jump.");
							
							break;
						}
					case NOT_ENOUGH_FUEL:
						{
							SetMessage("You do not have enough fuel to jump.");
							
							break;
						}
					case NO_JUMP_TARGET:
						{
							SetMessage("No system selected to jump to.");
							
							break;
						}
					}
				}
			}
			
			break;
		}
	case 46: // Key: L
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					const Planet * SelectedPlanet(dynamic_cast< const Planet * >(g_InputMind->GetCharacter()->GetShip()->GetTarget().Get()));
					
					switch(WantToLand(g_InputMind->GetCharacter(), g_InputMind->GetCharacter()->GetShip(), SelectedPlanet))
					{
					case OK:
						{
							g_InputMind->GetCharacter()->RemoveCredits(SelectedPlanet->GetLandingFee());
							g_InputMind->Land();
							g_InputMind->GetCharacter()->GetShip()->SetHull(g_InputMind->GetCharacter()->GetShip()->GetShipClass()->GetHull());
							g_Pause = true;
							g_PlanetDialog = new PlanetDialog(g_UserInterface->GetRootWidget(), const_cast< Planet * >(SelectedPlanet), g_InputMind->GetCharacter());
							g_PlanetDialog->GrabKeyFocus();
							g_PlanetDialog->AddDestroyListener(&g_GlobalDestroyListener);
							
							break;
						}
					case NO_LAND_TARGET:
						{
							g_InputMind->TargetNearestPlanet();
							
							break;
						}
					case NOT_ENOUGH_CREDITS:
						{
							SetMessage("You don't have enough credits to pay the landing fee.");
							
							break;
						}
					case TOO_FAR_AWAY:
						{
							SetMessage("You are too far away from the planet to land.");
							
							break;
						}
					case TOO_FAST:
						{
							SetMessage("You are too fast to land on the planet.");
							
							break;
						}
					}
				}
			}
			
			break;
		}
	case 50: // Key: LEFT SHIFT
		{
			if(KeyEventInformation.IsDown() == true)
			{
				std::stringstream IdentifierPrefix;
				
				IdentifierPrefix << "::system(" << g_CurrentSystem->GetIdentifier() << ")::created_at_game_time(" << std::fixed << GameTime::Get() << ")";
				SpawnShip(g_CurrentSystem, IdentifierPrefix.str());
			}
			
			break;
		}
	case 54: // Key: C
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					SetMessage("Jettison cargo.");
					g_InputMind->Jettison();
				}
			}
			
			break;
		}
	case 57: // Key: N
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->SelectNextLinkedSystem();
				}
			}
			
			break;
		}
	case 58: // Key: M
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if((g_MapDialog == 0) && (g_OutputMind == true))
				{
					g_Pause = true;
					g_MapDialog = new MapDialog(g_UserInterface->GetRootWidget(), g_OutputMind->GetCharacter()->GetShip()->GetCurrentSystem(), g_OutputMind->GetCharacter());
					g_MapDialog->GrabKeyFocus();
					g_MapDialog->AddDestroyListener(&g_GlobalDestroyListener);
					if(g_InputMind == true)
					{
						g_InputMind->DisableAccelerate();
						g_InputMind->DisableTurnLeft();
						g_InputMind->DisableTurnRight();
					}
				}
			}
			
			break;
		}
	case 59: // Key: COMMA
		{
			if(KeyEventInformation.IsDown() == true)
			{
				SetTimeWarp(g_TimeWarp / 1.1f);
			}
			
			break;
		}
	case 60: // Key: PERIODE
		{
			if(KeyEventInformation.IsDown() == true)
			{
				SetTimeWarp(g_TimeWarp * 1.1f);
			}
			
			break;
		}
	case 61: // Key: SLASH
		{
			if(KeyEventInformation.IsDown() == true)
			{
				SetTimeWarp(1.0f);
			}
			
			break;
		}
	case 65: // Key: SPACE
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->EnableFire();
				}
			}
			else
			{
				if(g_InputMind == true)
				{
					g_InputMind->DisableFire();
				}
			}
			
			break;
		}
	case 67: // Key: F1
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_SaveGameDialog == 0)
				{
					g_Pause = true;
					g_SaveGameDialog = new SaveGameDialog(g_UserInterface->GetRootWidget(), Function(SaveGame));
					g_SaveGameDialog->GrabKeyFocus();
					g_SaveGameDialog->AddDestroyListener(&g_GlobalDestroyListener);
				}
			}
			
			break;
		}
	case 68: // Key: F2
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_LoadGameDialog == 0)
				{
					g_Pause = true;
					g_LoadGameDialog = new LoadGameDialog(g_UserInterface->GetRootWidget(), Function(LoadGameFromInputStream));
					g_LoadGameDialog->GrabKeyFocus();
					g_LoadGameDialog->AddDestroyListener(&g_GlobalDestroyListener);
				}
			}
			
			break;
		}
	case 75: // Key: F9
		{
			if(KeyEventInformation.IsDown() == true)
			{
				std::ofstream OutputFileStream(MakeTimeStampedFileName("object-report", "xml").c_str());
				XMLStream Out(OutputFileStream);
				
				Object::Dump(Out);
				OutputFileStream << std::endl;
			}
			
			break;
		}
	case 76: // Key: F10
		{
			if(KeyEventInformation.IsDown() == True)
			{
				g_TakeScreenShot = true;
			}
			
			break;
		}
	case 95: // Key: F11
		{
			if(KeyEventInformation.IsDown() == true)
			{
				PurgeGame();
			}
			
			break;
		}
	case 96: // Key: F12
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_TimingDialog == 0)
				{
					g_TimingDialog = new TimingDialog(g_UserInterface->GetRootWidget());
				}
				else
				{
					g_TimingDialog->Destroy();
					g_TimingDialog = 0;
				}
			}
			
			break;
		}
	case 97: // Key: HOME
		{
			if(KeyEventInformation.IsDown() == true)
			{
				g_Camera.SetPosition(0.0f, 0.0f);
			}
			
			break;
		}
	case 98: // Key: UP
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->EnableAccelerate();
				}
			}
			else
			{
				if(g_InputMind == true)
				{
					g_InputMind->DisableAccelerate();
				}
			}
			
			break;
		}
	case 99: // Key: PAGE UP
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == false)
				{
					std::set< Character * > Characters(Character::GetCharacters());
					
					if(g_OutputMind == false)
					{
						if(Characters.empty() == false)
						{
							g_OutputMind = (*(Characters.rbegin()))->GetActiveMind()->GetReference();
						}
					}
					else
					{
						std::set< Character * >::iterator CharacterIterator(Characters.find(g_OutputMind->GetCharacter()));
						
						assert(CharacterIterator != Characters.end());
						if(CharacterIterator == Characters.begin())
						{
							g_OutputMind.Clear();
						}
						else
						{
							--CharacterIterator;
							g_OutputMind = (*CharacterIterator)->GetActiveMind()->GetReference();
						}
					}
				}
			}
			
			break;
		}
	case 100: // Key: LEFT
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->EnableTurnLeft();
				}
			}
			else
			{
				if(g_InputMind == true)
				{
					g_InputMind->DisableTurnLeft();
				}
			}
			
			break;
		}
	case 102: // Key: RIGHT
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == true)
				{
					g_InputMind->EnableTurnRight();
				}
			}
			else
			{
				if(g_InputMind == true)
				{
					g_InputMind->DisableTurnRight();
				}
			}
			
			break;
		}
	case 105: // Key: PAGE DOWN
		{
			if(KeyEventInformation.IsDown() == true)
			{
				if(g_InputMind == false)
				{
					std::set< Character * > Characters(Character::GetCharacters());
					
					if(g_OutputMind == false)
					{
						if(Characters.empty() == false)
						{
							g_OutputMind = (*(Characters.begin()))->GetActiveMind()->GetReference();
						}
					}
					else
					{
						std::set< Character * >::iterator CharacterIterator(Characters.find(g_OutputMind->GetCharacter()));
						
						assert(CharacterIterator != Characters.end());
						++CharacterIterator;
						if(CharacterIterator == Characters.end())
						{
							g_OutputMind.Clear();
						}
						else
						{
							g_OutputMind = (*CharacterIterator)->GetActiveMind()->GetReference();
						}
					}
				}
			}
			
			break;
		}
	}
}

void CreateWindow(void)
{
	g_Display = XOpenDisplay(0);
	
	int ScreenNumber(DefaultScreen(g_Display));
	
	g_Width = DisplayWidth(g_Display, ScreenNumber);
	g_Height = DisplayHeight(g_Display, ScreenNumber);
	
	int GLXAttributes[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, GLX_ALPHA_SIZE, 4, GLX_DEPTH_SIZE, 16, 0 };
	XVisualInfo * VisualInfo(glXChooseVisual(g_Display, ScreenNumber, GLXAttributes));
	
	if(VisualInfo == 0)
	{
		std::cerr << "Sorry, could not find a suitable visualization mode." << std::endl;
		exit(1);
	}
	g_GLXContext = glXCreateContext(g_Display, VisualInfo, 0, GL_TRUE);
	
	XSetWindowAttributes WindowAttributes;
	
	WindowAttributes.colormap = XCreateColormap(g_Display, RootWindow(g_Display, VisualInfo->screen), VisualInfo->visual, AllocNone);
	WindowAttributes.border_pixel = 0;
	WindowAttributes.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | PointerMotionMask;
	//WindowAttributes.override_redirect = True;
	// create the window
	g_Window = XCreateWindow(g_Display, RootWindow(g_Display, VisualInfo->screen), 0, 0, static_cast< unsigned int >(g_Width), static_cast< unsigned int >(g_Height), 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWBorderPixel | CWColormap | CWEventMask/* | CWOverrideRedirect*/, &WindowAttributes);
	
	Atom wmDelete = XInternAtom(g_Display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(g_Display, g_Window, &wmDelete, 1);
	XSetStandardProperties(g_Display, g_Window, "galactic-fall 0.2", "TITLE", None, NULL, 0, NULL);
	// show the window
	XMapRaised(g_Display, g_Window);
	// grab the keyboard so we get keyboard events and windowmanager can interfere
	//XGrabKeyboard(g_Display, g_Window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
	// grab the mouse, so the mouse cursor can't leave our window
	//XGrabPointer(g_Display, g_Window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, g_Window, None, CurrentTime);
	// don't allow key repeats
	//XAutoRepeatOff(g_Display);
	glXMakeCurrent(g_Display, g_Window, g_GLXContext);
}

void InitializeOpenGL(void)
{
	InitializeFonts();
	
	// TODO: Make configurable via data.arx
	Vector4f GlobalAmbientLightColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GlobalAmbientLightColor.m_V.m_A);
	Resize();
}

void DeinitializeOpenGL(void)
{
	DeinitializeFonts();
}

void ProcessEvents(void)
{
	XEvent Event;
	
	while(XPending(g_Display) > 0)
	{
		XNextEvent(g_Display, &Event);
		switch(Event.type)
		{
		case ConfigureNotify:
			{
				if((Event.xconfigure.width != g_Width) || (Event.xconfigure.height != g_Height))
				{
					g_Width = Event.xconfigure.width;
					g_Height = Event.xconfigure.height;
					Resize();
				}
				
				break;
			}
		case MotionNotify:
			{
				if(g_EchoEvents == true)
				{
					std::cout << "Motion:         x=" << Event.xbutton.x << "   y=" << Event.xbutton.y << std::endl;
				}
				MouseMotion(Event.xmotion.x, Event.xmotion.y);
				
				break;
			}
		case ButtonPress:
			{
				if(g_EchoEvents == true)
				{
					std::cout << "ButtonPress:    button=" << Event.xbutton.button << "   x=" << Event.xbutton.x << "   y=" << Event.xbutton.y << std::endl;
				}
				MouseButtonDown(Event.xbutton.button, Event.xbutton.x, Event.xbutton.y);
				
				break;
			}
		case ButtonRelease:
			{
				if(g_EchoEvents == true)
				{
					std::cout << "ButtonRelease:  button=" << Event.xbutton.button << "   x=" << Event.xbutton.x << "   y=" << Event.xbutton.y << std::endl;
				}
				MouseButtonUp(Event.xbutton.button, Event.xbutton.x, Event.xbutton.y);
				
				break;
			}
		case KeyPress:
		case KeyRelease:
			{
				KeyEventInformation KeyEventInformation(Event.xkey);
				
				if(g_EchoEvents == true)
				{
					std::cout << "KeyEvent:       state=" << Event.xkey.state << "   keycode=" << KeyEventInformation.GetKeyCode() << "   string=" << KeyEventInformation.GetString() << "  [length=" << KeyEventInformation.GetString().length() << "]   keysym=" << KeyEventInformation.GetKeySymbol() << std::endl;
				}
				KeyEvent(KeyEventInformation);
				
				break;
			}
		}
	}
}

void DestroyWindow(void)
{
	XAutoRepeatOn(g_Display);
    if(g_GLXContext != 0)
    {
        if(glXMakeCurrent(g_Display, None, 0) == 0)
        {
            std::cerr << "Could not release GLX context." << std::endl;
        }
        glXDestroyContext(g_Display, g_GLXContext);
        g_GLXContext = 0;
    }
    XCloseDisplay(g_Display);
	g_Display = 0;
}

int main(int argc, char ** argv)
{
	// setup the random number generator for everyday use
	srand(time(0));
	// static initialization of data independent globals
	g_MainPerspective.SetNearClippingPlane(1.0f);
	g_MainPerspective.SetFarClippingPlane(1000.f);
	g_AssetClassManager = new AssetClassManager();
	g_CommodityClassManager = new CommodityClassManager();
	g_Galaxy = 0;
	g_GraphicsEngine = new Graphics::Engine();
	g_GraphicsEngine->SetOnDestroyCallback(Function(OnGraphicsNodeDestroy));
	g_MainScene = 0;
	g_MeshManager = new Graphics::MeshManager();
	g_ModelManager = new Graphics::ModelManager();
	g_ObjectFactory = new ObjectFactory();
	g_ShipClassManager = new ShipClassManager();
	g_SlotClassManager = new SlotClassManager();
	g_TextureManager = new Graphics::TextureManager();
	g_UserInterface = new UserInterface();
	g_WeaponClassManager = new WeaponClassManager();
	g_GameTimeTimeoutNotifications = new TimeoutNotificationManager();
	g_RealTimeTimeoutNotifications = new TimeoutNotificationManager();
	g_SystemStatistics = new SystemStatistics();
	
	// parse command line
	std::vector< std::string > Arguments(argv, argv + argc);
	std::string LoadSavegameFileName("data/savegame_default.xml");
	std::string DataFileName("data/data.arx");
	
	for(std::vector< std::string >::size_type Index = 0; Index < Arguments.size(); ++Index)
	{
		if(Arguments[Index].substr(0, 6) == "--load")
		{
			if(Arguments[Index].length() > 7)
			{
				LoadSavegameFileName = Arguments[Index].substr(7);
			}
		}
		else if(Arguments[Index].substr(0, 6) == "--data")
		{
			if(Arguments[Index].length() > 7)
			{
				DataFileName = Arguments[Index].substr(7);
			}
		}
		else if(Arguments[Index] == "--echo-events")
		{
			g_EchoEvents = true;
		}
	}
	
	// try loading the game data archive
	g_ResourceReader = new ResourceReader(DataFileName.substr(0, DataFileName.rfind('/')) + '/');
	if(g_ResourceReader->LoadArchive(DataFileName) == false)
	{
		return 1;
	}
	
	// read the data from the archive
	g_ResourceReader->ReadMeshes();
	g_ResourceReader->ReadModels();
	g_ResourceReader->ReadAssetClasses();
	g_ResourceReader->ReadCommodityClasses();
	g_ResourceReader->ReadSlotClasses();
	g_ResourceReader->ReadShipClasses();
	g_ResourceReader->ReadUserInterface();
	g_ResourceReader->ReadWeaponClasses();
	
	// setup the global variables for the user interface
	g_CreditsLabel = dynamic_cast< Label * >(g_UserInterface->GetWidget("/credits"));
	g_FuelLabel = dynamic_cast< Label * >(g_UserInterface->GetWidget("/fuel"));
	g_HullLabel = dynamic_cast< Label * >(g_UserInterface->GetWidget("/hull"));
	g_MessageLabel = dynamic_cast< Label * >(g_UserInterface->GetWidget("/message"));
	g_SystemLabel = dynamic_cast< Label * >(g_UserInterface->GetWidget("/system"));
	g_TimeWarpLabel = dynamic_cast< Label * >(g_UserInterface->GetWidget("/time_warp"));
	g_MiniMap = dynamic_cast< Widget * >(g_UserInterface->GetWidget("/mini_map"));
		g_CurrentSystemLabel = dynamic_cast< Label * >(g_UserInterface->GetWidget("/mini_map/current_system"));
		g_MiniMapDisplay = dynamic_cast< MiniMapDisplay * >(g_UserInterface->GetWidget("/mini_map/display"));
	g_Scanner = g_UserInterface->GetWidget("/scanner");
		g_TargetLabel = dynamic_cast< Label * >(g_UserInterface->GetWidget("/scanner/target"));
		g_ScannerDisplay = dynamic_cast< ScannerDisplay * >(g_UserInterface->GetWidget("/scanner/display"));
	
	// sanity asserts
	assert(g_CreditsLabel != 0);
	assert(g_FuelLabel != 0);
	assert(g_HullLabel != 0);
	assert(g_MessageLabel != 0);
	assert(g_SystemLabel != 0);
	assert(g_TimeWarpLabel != 0);
	assert(g_MiniMap != 0);
	assert(g_CurrentSystemLabel != 0);
	assert(g_MiniMapDisplay != 0);
	assert(g_Scanner != 0);
	assert(g_TargetLabel != 0);
	assert(g_ScannerDisplay != 0);
	// set first timeout for widget collector, it will reinsert itself on callback
	g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 5.0f, Function(CollectWidgets));
	// setting up the graphical environment
	CreateWindow();
	InitializeOpenGL();
	// since reading the textures already creates them we have to do this after initializing OpenGL
	g_ResourceReader->ReadTextures();
	
	// load the specified savegame
	if(LoadGameFromFileName(LoadSavegameFileName) == false)
	{
		return 1;
	}
	
	// main loop
	while(g_Quit == false)
	{
		ProcessEvents();
		GameFrame();
		glXSwapBuffers(g_Display, g_Window);
	}
	// cleanup
	PurgeGame();
	DeinitializeOpenGL();
	DestroyWindow();
	// if requested print some final debugging information
	if(g_DumpEndReport == true)
	{
		XMLStream Out(std::cout);
		
		Object::Dump(Out);
		std::cout << std::endl;
	}
	delete g_AssetClassManager;
	delete g_CommodityClassManager;
	delete g_GameTimeTimeoutNotifications;
	delete g_GraphicsEngine;
	delete g_ModelManager;
	delete g_ObjectFactory;
	delete g_RealTimeTimeoutNotifications;
	delete g_ShipClassManager;
	delete g_SlotClassManager;
	delete g_SystemStatistics;
	delete g_TextureManager;
	delete g_UserInterface;
	delete g_WeaponClassManager;
	delete g_ResourceReader;
	
	return 0;
}

