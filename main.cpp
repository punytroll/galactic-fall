/**
 * galactic-fall
 * Copyright (C) 2006  Hagen MÃ¶bius
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

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

#include <GL/glx.h>
#include <GL/gl.h>

#include <math3d/matrix4f.h>
#include <math3d/vector2f.h>
#include <math3d/vector4f.h>

#include <Archive.h>
#include <BufferReader.h>
#include <Item.h>
#include <URI.h>

#include "arx_reading.h"
#include "arx_resources.h"
#include "callbacks.h"
#include "camera.h"
#include "cargo.h"
#include "character.h"
#include "color.h"
#include "command_mind.h"
#include "commodity.h"
#include "commodity_manager.h"
#include "destroy_listener.h"
#include "game_time.h"
#include "globals.h"
#include "label.h"
#include "map_dialog.h"
#include "map_knowledge.h"
#include "math.h"
#include "mind.h"
#include "mini_map.h"
#include "model.h"
#include "model_manager.h"
#include "perspective.h"
#include "planet.h"
#include "planet_dialog.h"
#include "real_time.h"
#include "scanner_display.h"
#include "ship.h"
#include "ship_class.h"
#include "ship_class_manager.h"
#include "shot.h"
#include "star.h"
#include "state_machine.h"
#include "states.h"
#include "string_cast.h"
#include "system.h"
#include "system_manager.h"
#include "user_interface.h"
#include "widget.h"
#include "xml_puny_dom.h"
#include "xml_stream.h"

int g_LastMotionX(-1);
int g_LastMotionY(-1);
int g_MouseButton(-1);
Camera g_Camera;
ModelManager g_ModelManager;
ShipClassManager g_ShipClassManager(&g_ModelManager);
CommodityManager g_CommodityManager;
SystemManager g_SystemManager(&g_CommodityManager);
CommandMind * g_InputMind(0);
Mind * g_OutputMind(0);
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
PlanetDialog * g_PlanetDialog;
MapDialog * g_MapDialog;
UserInterface g_UserInterface;
std::multimap< double, Callback0< void > * > g_GameTimeTimeoutNotifications;
std::multimap< double, Callback0< void > * >::iterator g_SpawnShipTimeoutIterator;
std::multimap< double, Callback0< void > * > g_RealTimeTimeoutNotifications;
std::multimap< double, Callback0< void > * >::iterator g_MessageTimeoutIterator;
Widget * g_Scanner(0);
MiniMap * g_MiniMap(0);
ScannerDisplay * g_ScannerDisplay(0);
Display * g_Display;
GLXContext g_GLXContext;
Window g_Window;
Perspective g_MainPerspective;

enum WantReturnCode
{
	OK,
	NO_JUMP_TARGET,
	NO_LAND_TARGET,
	NO_SCOOP_TARGET,
	NOT_ENOUGH_CREDITS,
	NOT_ENOUGH_FUEL,
	TOO_FAR_AWAY,
	TOO_FAST,
	TOO_HIGH_RELATIVE_VELOCITY,
	TOO_NEAR_TO_SYSTEM_CENTER
};

int WantToJump(Ship * Ship, System * System)
{
	if(System == 0)
	{
		return NO_JUMP_TARGET;
	}
	// only let the ships jump if they are more than 280 clicks from system center
	if(Ship->GetPosition().length_squared() < 78400.0f)
	{
		return TOO_NEAR_TO_SYSTEM_CENTER;
	}
	// only let ships jump if they have enough fuel
	if(Ship->GetFuel() < Ship->GetShipClass()->GetJumpFuel())
	{
		return NOT_ENOUGH_FUEL;
	}
	
	return OK;
}

int WantToLand(Character * Character, Ship * Ship, Planet * Planet)
{
	if(Planet == 0)
	{
		return NO_LAND_TARGET;
	}
	// test distance
	if((Planet->GetPosition() - Ship->GetPosition()).length_squared() > Planet->GetSize() * Planet->GetSize())
	{
		return TOO_FAR_AWAY;
	}
	// test speed (should be relative speed but planets have no speed, yet)
	if(Ship->GetVelocity().length_squared() > 2.0f)
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

int WantToScoop(Ship * Ship, Cargo * Cargo)
{
	if(Cargo == 0)
	{
		return NO_SCOOP_TARGET;
	}
	// test distance
	if((Cargo->GetPosition() - Ship->GetPosition()).length_squared() > 5.0f * Cargo->GetRadialSize() * Cargo->GetRadialSize())
	{
		return TOO_FAR_AWAY;
	}
	// test speed
	if((Ship->GetVelocity() - Cargo->GetVelocity()).length_squared() > 2.0f)
	{
		return TOO_HIGH_RELATIVE_VELOCITY;
	}
	
	return OK;
}

float CalculateTime(void)
{
	double DeltaSeconds(RealTime::GetInterval());
	
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
	g_MessageLabel->Hide();
	g_MessageTimeoutIterator = g_RealTimeTimeoutNotifications.end();
}

void SetMessage(const std::string & Message)
{
	g_MessageLabel->SetString(Message);
	g_MessageLabel->SetPosition(math3d::vector2f((g_Width - 6 * Message.length()) / 2, 40.0f));
	g_MessageLabel->Show();
	/// TODO: Make the 2.0f seconds timeout configurable via the game configuration archive.
	if(g_MessageTimeoutIterator != g_RealTimeTimeoutNotifications.end())
	{
		g_RealTimeTimeoutNotifications.erase(g_MessageTimeoutIterator);
	}
	g_MessageTimeoutIterator = g_RealTimeTimeoutNotifications.insert(std::make_pair(RealTime::GetTime() + 2.0f, new FunctionCallback0< void >(HideMessage)));
}

void DrawSelection(Position * Position, float RadialSize)
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
	g_RealTimeTimeoutNotifications.insert(std::make_pair(RealTime::GetTime() + 5.0f, new FunctionCallback0< void >(CollectWidgets)));
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
	g_UserInterface.Draw();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
}

void RemoveCargoFromSystem(System * System, std::list< Cargo * >::iterator CargoIterator)
{
	// basically this functions only does the last call
	// all other stuff should be changed to references, so it is done automatically
	Cargo * Cargo(*CargoIterator);
	
	if(Cargo == g_Camera.GetFocus())
	{
		g_Camera.SetFocus(0);
	}
	if((g_OutputMind != 0) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0) && (g_OutputMind->GetCharacter()->GetShip()->GetTarget() == Cargo))
	{
		g_OutputMind->GetCharacter()->GetShip()->SetTarget(0);
	}
	if((g_InputMind != 0) && (g_InputMind->GetCharacter() != 0) && (g_InputMind->GetCharacter()->GetShip() != 0) && (g_InputMind->GetCharacter()->GetShip()->GetTarget() == Cargo))
	{
		g_InputMind->GetCharacter()->GetShip()->SetTarget(0);
	}
	System->GetCargos().erase(CargoIterator);
}

void RemoveShipFromSystem(System * System, std::list< Ship * >::iterator ShipIterator)
{
	// basically this functions only does the last call
	// all other stuff should be changed to references, so it is done automatically
	Ship * Ship(*ShipIterator);
	
	if(Ship == g_Camera.GetFocus())
	{
		g_Camera.SetFocus(0);
	}
	if((g_OutputMind != 0) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0) && (g_OutputMind->GetCharacter()->GetShip()->GetTarget() == Ship))
	{
		g_OutputMind->GetCharacter()->GetShip()->SetTarget(0);
	}
	if((g_InputMind != 0) && (g_InputMind->GetCharacter() != 0) && (g_InputMind->GetCharacter()->GetShip() != 0) && (g_InputMind->GetCharacter()->GetShip()->GetTarget() == Ship))
	{
		g_InputMind->GetCharacter()->GetShip()->SetTarget(0);
	}
	System->GetShips().erase(ShipIterator);
}

void RemoveShotFromSystem(System * System, std::list< Shot * >::iterator ShotIterator)
{
	Shot * Shot(*ShotIterator);
	
	if(Shot == g_Camera.GetFocus())
	{
		g_Camera.SetFocus(0);
	}
	if((g_OutputMind != 0) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0) && (g_OutputMind->GetCharacter()->GetShip()->GetTarget() == Shot))
	{
		g_OutputMind->GetCharacter()->GetShip()->SetTarget(0);
	}
	if((g_InputMind != 0) && (g_InputMind->GetCharacter() != 0) && (g_InputMind->GetCharacter()->GetShip() != 0) && (g_InputMind->GetCharacter()->GetShip()->GetTarget() == Shot))
	{
		g_InputMind->GetCharacter()->GetShip()->SetTarget(0);
	}
	System->GetShots().erase(ShotIterator);
}

void DeleteCargo(Cargo * Cargo)
{
	delete Cargo;
}

void DeleteShip(Ship * Ship)
{
	std::set< Object * > & Manifest(Ship->GetManifest());
	
	while(Manifest.empty() == false)
	{
		Object * ManifestObject(*(Manifest.begin()));
		
		Ship->RemoveObject(ManifestObject);
		
		Character * ManifestCharacter(dynamic_cast< Character * >(ManifestObject));
		
		if(ManifestCharacter != 0)
		{
			Mind * ManifestCharacterMind;
			
			while((ManifestCharacterMind = ManifestCharacter->ReleaseMind()) != 0)
			{
				// TODO: don't delete it if it is the input focus mind
				delete ManifestCharacterMind;
			}
			delete ManifestCharacter;
		}
		else
		{
			std::cerr << "Unknown object type for object '" << ManifestObject->GetObjectIdentifier() << "' in ship '" << Ship->GetObjectIdentifier() << "'." << std::endl;
			
			delete ManifestObject;
		}
	}
	delete Ship;
}

void DeleteShot(Shot * Shot)
{
	delete Shot;
}

void CalculateCharacters(void)
{
	std::set< Character * > & Characters(Character::GetCharacters());
	
	for(std::set< Character * >::iterator CharacterIterator = Characters.begin(); CharacterIterator != Characters.end(); ++CharacterIterator)
	{
		(*CharacterIterator)->Update();
	}
}

void CalculateMovements(System * System)
{
	float Seconds(CalculateTime());
	
	if(System != 0)
	{
		// TODO: it is unclear, which Ships to update really.
		std::list< Ship * > & Ships(System->GetShips());
		std::list< Ship * >::iterator ShipIterator(Ships.begin());
		
		while(ShipIterator != Ships.end())
		{
			Ship * Ship(*ShipIterator);
			
			// increment up here because Update() might invalidate the iterator
			++ShipIterator;
			Ship->Update(Seconds);
			if(Ship->GetCurrentSystem() != System)
			{
				if((g_OutputMind == 0) || (g_OutputMind->GetCharacter() == 0) || (g_OutputMind->GetCharacter()->GetShip() != Ship))
				{
					RemoveShipFromSystem(Ship->GetCurrentSystem(), std::find(Ship->GetCurrentSystem()->GetShips().begin(), Ship->GetCurrentSystem()->GetShips().end(), Ship));
					DeleteShip(Ship);
				}
			}
		}
		
		std::list< Cargo * > & Cargos(System->GetCargos());
		std::list< Shot * > & Shots(System->GetShots());
		
		for(std::list< Cargo * >::const_iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
		{
			(*CargoIterator)->Move(Seconds);
		}
		for(std::list< Shot * >::iterator ShotIterator = Shots.begin(); ShotIterator != Shots.end();)
		{
			bool DeleteShot(false);
			
			if((*ShotIterator)->Update(Seconds) == true)
			{
				for(std::list< Ship * >::iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
				{
					Ship * Ship(*ShipIterator);
					
					if((*ShotIterator)->GetShooter() != Ship)
					{
						if(((*ShotIterator)->GetPosition() - Ship->GetPosition()).length_squared() < ((*ShotIterator)->GetRadialSize() * (*ShotIterator)->GetRadialSize() + Ship->GetRadialSize() * Ship->GetRadialSize()))
						{
							Ship->SetHull(Ship->GetHull() - (*ShotIterator)->GetDamage());
							if(Ship->GetHull() <= 0.0f)
							{
								std::set< Object * >::iterator ManifestIterator;
								std::set< Object * >::iterator NextIterator((*ShipIterator)->GetManifest().begin());
								
								while(NextIterator != (*ShipIterator)->GetManifest().end())
								{
									ManifestIterator = NextIterator;
									++NextIterator;
									
									Cargo * TheCargo(dynamic_cast< Cargo * >(*ManifestIterator));
									
									if(TheCargo != 0)
									{
										(*ShipIterator)->GetManifest().erase(ManifestIterator);
										TheCargo->SetPosition((*ShipIterator)->GetPosition());
										TheCargo->SetVelocity((*ShipIterator)->GetVelocity() * 0.8f + math3d::vector2f(GetRandomFloat(-0.5f, 0.5f), GetRandomFloat(-0.5f, 0.5f)));
										(*ShipIterator)->GetCurrentSystem()->AddCargo(TheCargo);
									}
								}
								RemoveShipFromSystem(System, ShipIterator);
								DeleteShip(Ship);
							}
							DeleteShot = true;
							
							break;
						}
					}
				}
				if(DeleteShot == false)
				{
					for(std::list< Cargo * >::iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
					{
						Cargo * Cargo(*CargoIterator);
						
						if(((*ShotIterator)->GetPosition() - Cargo->GetPosition()).length_squared() < ((*ShotIterator)->GetRadialSize() * (*ShotIterator)->GetRadialSize() + Cargo->GetRadialSize() * Cargo->GetRadialSize()))
						{
							Cargo->SetHull(Cargo->GetHull() - (*ShotIterator)->GetDamage());
							if(Cargo->GetHull() <= 0.0f)
							{
								RemoveCargoFromSystem(System, CargoIterator);
								DeleteCargo(Cargo);
							}
							DeleteShot = true;
							
							break;
						}
					}
				}
			}
			else
			{
				DeleteShot = true;
			}
			if(DeleteShot == true)
			{
				// replace with calls to DeleteShot() and RemoveShotFromSystem()
				delete *ShotIterator;
				ShotIterator = Shots.erase(ShotIterator);
			}
			else
			{
				++ShotIterator;
			}
		}
	}
}

void ProcessGameTimeTimeouts(void)
{
	// call all game time timeouts
	double StopGameTime(GameTime::Get());
	
	while((g_GameTimeTimeoutNotifications.size() > 0) && (StopGameTime > g_GameTimeTimeoutNotifications.begin()->first))
	{
		// call the notification callback object
		(*(g_GameTimeTimeoutNotifications.begin()->second))();
		// delete the notification callback object
		delete g_GameTimeTimeoutNotifications.begin()->second;
		// remove the notification callback from the multimap
		g_GameTimeTimeoutNotifications.erase(g_GameTimeTimeoutNotifications.begin());
	}
}

void ProcessRealTimeTimeouts(void)
{
	// call all real time timeouts
	double StopTime(RealTime::GetTime());
	
	while((g_RealTimeTimeoutNotifications.size() > 0) && (StopTime > g_RealTimeTimeoutNotifications.begin()->first))
	{
		// call the notification callback object
		(*(g_RealTimeTimeoutNotifications.begin()->second))();
		// delete the notification callback object
		delete g_RealTimeTimeoutNotifications.begin()->second;
		// remove the notification callback from the multimap
		g_RealTimeTimeoutNotifications.erase(g_RealTimeTimeoutNotifications.begin());
	}
}

void UpdateUserInterface(void)
{
	if((g_OutputMind != 0) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0))
	{
		// display the name of the target
		if(g_OutputMind->GetCharacter()->GetShip()->GetTarget() != 0)
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
		g_FuelLabel->SetString("Fuel: " + to_string_cast(100.0f * g_OutputMind->GetCharacter()->GetShip()->GetFuel() / g_OutputMind->GetCharacter()->GetShip()->GetFuelCapacity(), 2) + "%");
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
	}
}

void Render(System * System)
{
	glViewport(0, 0, static_cast< GLsizei >(g_Width), static_cast< GLsizei >(g_Height));
	glMatrixMode(GL_PROJECTION);
	g_MainPerspective.Draw();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	g_Camera.Draw();
	
	const Star * CurrentStar(System->GetStar());
	
	if(CurrentStar != 0)
	{
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, math3d::vector4f(CurrentStar->GetPosition().m_V.m_A[0], CurrentStar->GetPosition().m_V.m_A[1], 100.0f, 0.0f).m_V.m_A);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, CurrentStar->GetColor().GetColor().m_V.m_A);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	if(System != 0)
	{
		const std::list< Planet * > & Planets(System->GetPlanets());
		const std::list< Ship * > & Ships(System->GetShips());
		const std::list< Cargo * > & Cargos(System->GetCargos());
		const std::list< Shot * > & Shots(System->GetShots());
		
		for(std::list< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			(*PlanetIterator)->Draw();
		}
		for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			(*ShipIterator)->Draw();
		}
		for(std::list< Cargo * >::const_iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			(*CargoIterator)->Draw();
		}
		for(std::list< Shot * >::const_iterator ShotIterator = Shots.begin(); ShotIterator != Shots.end(); ++ShotIterator)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			(*ShotIterator)->Draw();
		}
	}
	// HUD
	if((g_OutputMind != 0) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0) && (g_OutputMind->GetCharacter()->GetShip()->GetTarget() != 0))
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		DrawSelection(g_OutputMind->GetCharacter()->GetShip()->GetTarget(), g_OutputMind->GetCharacter()->GetShip()->GetTarget()->GetRadialSize());
		
		math3d::vector2f RelativePosition(g_OutputMind->GetCharacter()->GetShip()->GetTarget()->GetPosition() - g_OutputMind->GetCharacter()->GetShip()->GetPosition());
		
		RelativePosition.normalize();
		glPushMatrix();
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glTranslatef(g_OutputMind->GetCharacter()->GetShip()->GetPosition().m_V.m_A[0], g_OutputMind->GetCharacter()->GetShip()->GetPosition().m_V.m_A[1], 0.0f);
		glRotatef(GetRadians(RelativePosition) * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
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
	DisplayUserInterface();
}

void Resize(void)
{
	if(g_Height == 0)
	{
		g_Height = 1;
	}
	glViewport(0, 0, static_cast< GLint >(g_Width), static_cast< GLint >(g_Height));
	g_MainPerspective.SetAspect(g_Width / g_Height);
	g_UserInterface.GetRootWidget()->SetSize(math3d::vector2f(g_Width, g_Height));
	g_Scanner->SetPosition(math3d::vector2f(0.0f, g_Height - 240.0f));
	g_MiniMap->SetPosition(math3d::vector2f(g_Width - 220.0f, g_Height - 240.0f));
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
			if(g_CurrentSystem->IsLinkedToSystem(g_MapDialog->GetSelectedSystem()) == true)
			{
				g_InputMind->SelectLinkedSystem(g_MapDialog->GetSelectedSystem());
			}
			g_MapDialog = 0;
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
			Ship * Ship(*System->GetShips().begin());
			
			RemoveShipFromSystem(System, System->GetShips().begin());
			DeleteShip(Ship);
		}
		System->ClearCargos();
		while(System->GetShots().empty() == false)
		{
			Shot * Shot(*System->GetShots().begin());
			
			RemoveShotFromSystem(System, System->GetShots().begin());
			DeleteShot(Shot);
		}
	}
}

void SpawnShip(System * System, const std::string & IdentifierPrefix)
{
	std::string ShipClassIdentifier;
	
	if(GetRandomUniform() > 0.5f)
	{
		ShipClassIdentifier = "fighter";
	}
	else
	{
		ShipClassIdentifier = "transporter";
	}
	
	Ship * NewShip(new Ship(g_ShipClassManager.Get(ShipClassIdentifier)));
	
	NewShip->SetObjectIdentifier(IdentifierPrefix + "::ship(" + NewShip->GetShipClass()->GetIdentifier() + ")");
	NewShip->SetPosition(math3d::vector2f(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f)));
	NewShip->SetFuel(NewShip->GetFuelCapacity());
	NewShip->SetCurrentSystem(System);
	if(ShipClassIdentifier == "fighter")
	{
		NewShip->m_Fire = true;
	}
	
	Character * NewCharacter(new Character());
	
	NewCharacter->SetObjectIdentifier(IdentifierPrefix + "::character(" + NewShip->GetShipClass()->GetIdentifier() + ")");
	NewCharacter->SetShip(NewShip);
	
	StateMachineMind * NewMind(new StateMachineMind());
	
	NewMind->SetObjectIdentifier(IdentifierPrefix + "::mind(state_machine)");
	NewMind->SetCharacter(NewCharacter);
	NewMind->GetStateMachine()->SetState(new SelectSteering(NewShip, NewMind->GetStateMachine()));
	NewCharacter->PossessByMind(NewMind);
	NewShip->AddObject(NewCharacter);
	System->AddShip(NewShip);
}

void SpawnShipOnTimeout(System * SpawnInSystem)
{
	std::stringstream IdentifierPrefix;
	
	IdentifierPrefix << "::system(" << SpawnInSystem->GetIdentifier() << ")::created_at_game_time(" << std::fixed << GameTime::Get() << ")";
	
	SpawnShip(SpawnInSystem, IdentifierPrefix.str());
	g_SpawnShipTimeoutIterator = g_GameTimeTimeoutNotifications.insert(std::make_pair(GameTime::Get() + GetRandomFloatFromExponentialDistribution(30.0f), Bind1(Function(SpawnShipOnTimeout), SpawnInSystem)));
}

void PopulateSystem(System * System)
{
	int NumberOfShips(GetRandomInteger(5));
	
	for(int ShipNumber = 1; ShipNumber <= NumberOfShips; ++ShipNumber)
	{
		std::stringstream IdentifierPrefix;
		
		IdentifierPrefix << "::system(" << System->GetIdentifier() << ")::created_at(" << std::fixed << RealTime::GetTime() << "[" << ShipNumber << "])";
		
		SpawnShip(System, IdentifierPrefix.str());
	}
}

void OnOutputFocusEnterSystem(System * EnterSystem)
{
	assert(g_SpawnShipTimeoutIterator == g_GameTimeTimeoutNotifications.end());
	g_SpawnShipTimeoutIterator = g_GameTimeTimeoutNotifications.insert(std::make_pair(GameTime::Get() + GetRandomFloatFromExponentialDistribution(30.0f), Bind1(Function(SpawnShipOnTimeout), EnterSystem)));
}

void OnOutputFocusLeaveSystem(System * System)
{
	if(g_SpawnShipTimeoutIterator != g_GameTimeTimeoutNotifications.end())
	{
		delete g_SpawnShipTimeoutIterator->second;
		g_GameTimeTimeoutNotifications.erase(g_SpawnShipTimeoutIterator);
		g_SpawnShipTimeoutIterator = g_GameTimeTimeoutNotifications.end();
	}
}

void GameFrame(void)
{
	System * CurrentSystem(g_CurrentSystem);
	
	ProcessGameTimeTimeouts();
	ProcessRealTimeTimeouts();
	CalculateCharacters();
	CalculateMovements(CurrentSystem);
	UpdateUserInterface();
	Render(CurrentSystem);
	if((g_OutputMind != 0) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0) && (g_OutputMind->GetCharacter()->GetShip()->GetCurrentSystem() != g_CurrentSystem))
	{
		OnOutputFocusLeaveSystem(g_CurrentSystem);
		g_CurrentSystem = g_OutputMind->GetCharacter()->GetShip()->GetCurrentSystem();
		OnOutputFocusEnterSystem(g_CurrentSystem);
		EmptySystem(CurrentSystem);
		PopulateSystem(g_CurrentSystem);
	}
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
	if(g_UserInterface.MouseButton(MouseButton, EV_DOWN, X, Y) == true)
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
	if(g_UserInterface.MouseButton(MouseButton, EV_UP, X, Y) == true)
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
	g_UserInterface.MouseMotion(X, Y);
	
	int DeltaX(X - g_LastMotionX);
	int DeltaY(Y - g_LastMotionY);
	
	g_LastMotionX = X;
	g_LastMotionY = Y;
	if(g_MouseButton == 2) // MouseButton: MIDDLE
	{
		const math3d::vector3f & CameraPosition(g_Camera.GetPosition());
		
		g_Camera.SetPosition(CameraPosition.m_V.m_A[0] - static_cast< float >(DeltaX) * 0.0008f * CameraPosition.m_V.m_A[2], CameraPosition.m_V.m_A[1] + static_cast< float >(DeltaY) * 0.0008f * CameraPosition.m_V.m_A[2]);
	}
}

void KeyDown(unsigned int KeyCode)
{
	if(g_UserInterface.Key(KeyCode, EV_DOWN) == true)
	{
		return;
	}
	switch(KeyCode)
	{
	case 9:  // Key: ESCAPE
	case 24: // Key: Q
		{
			g_Quit = true;
			
			break;
		}
	case 26: // Key: E
		{
			if(g_InputMind != 0)
			{
				g_InputMind->TargetPreviousShip();
			}
			
			break;
		}
	case 27: // Key: R
		{
			if(g_InputMind != 0)
			{
				g_InputMind->TargetNextShip();
			}
			
			break;
		}
	case 32: // Key: O
		{
			if(g_InputMind != 0)
			{
				g_InputMind->TargetPreviousPlanet();
			}
			
			break;
		}
	case 33: // Key: P
		{
			if(g_InputMind != 0)
			{
				g_InputMind->TargetNextPlanet();
			}
			
			break;
		}
	case 35: // Key: ]
		{
			const std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
			const Ship * FocusShip(dynamic_cast< const Ship * >(g_Camera.GetFocus()));
			
			std::list< Ship * >::const_iterator ShipIterator(find(Ships.begin(), Ships.end(), FocusShip));
			
			if((ShipIterator == Ships.end()) || (++ShipIterator == Ships.end()))
			{
				g_Camera.SetFocus(Ships.front());
			}
			else
			{
				g_Camera.SetFocus(*ShipIterator);
			}
			
			break;
		}
	case 39: // Key: S
		{
			if(g_InputMind != 0)
			{
				switch(WantToScoop(g_InputMind->GetCharacter()->GetShip(), dynamic_cast< Cargo * >(g_InputMind->GetCharacter()->GetShip()->GetTarget())))
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
			
			break;
		}
	case 40: // Key: D
		{
			if(g_InputMind != 0)
			{
				g_InputMind->TargetPreviousCargo();
			}
			
			break;
		}
	case 41: // Key: F
		{
			if(g_InputMind != 0)
			{
				g_InputMind->TargetNextCargo();
			}
			
			break;
		}
	case 44: // Key: J
		{
			if(g_InputMind != 0)
			{
				switch(WantToJump(g_InputMind->GetCharacter()->GetShip(), g_InputMind->GetCharacter()->GetShip()->GetLinkedSystemTarget()))
				{
				case OK:
					{
						if(g_InputMind != 0)
						{
							g_InputMind->Jump();
						}
						
						break;
					}
				case TOO_NEAR_TO_SYSTEM_CENTER:
					{
						SetMessage("You are too near to the system center to jump.");
						
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
			
			break;
		}
	case 46: // Key: L
		{
			if(g_InputMind != 0)
			{
				Planet * SelectedPlanet(dynamic_cast< Planet * >(g_InputMind->GetCharacter()->GetShip()->GetTarget()));
				
				switch(WantToLand(g_InputMind->GetCharacter(), g_InputMind->GetCharacter()->GetShip(), SelectedPlanet))
				{
				case OK:
					{
						g_InputMind->GetCharacter()->RemoveCredits(SelectedPlanet->GetLandingFee());
						g_InputMind->Land();
						g_Pause = true;
						g_PlanetDialog = new PlanetDialog(g_UserInterface.GetRootWidget(), SelectedPlanet, g_InputMind->GetCharacter());
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
			
			break;
		}
	case 50: // Key: RIGHT SHIFT
		{
			std::stringstream IdentifierPrefix;
			
			IdentifierPrefix << "::system(" << g_CurrentSystem->GetIdentifier() << ")::created_at(" << std::fixed << RealTime::GetTime() << ")";
			SpawnShip(g_InputMind->GetCharacter()->GetShip()->GetCurrentSystem(), IdentifierPrefix.str());
			
			break;
		}
	case 54: // Key: C
		{
			if(g_InputMind != 0)
			{
				SetMessage("Jettison cargo.");
				g_InputMind->Jettison();
			}
			
			break;
		}
	case 57: // Key: N
		{
			if(g_InputMind != 0)
			{
				g_InputMind->SelectNextLinkedSystem();
			}
			
			break;
		}
	case 58: // Key: M
		{
			if(g_MapDialog == 0)
			{
				g_Pause = true;
				g_MapDialog = new MapDialog(g_UserInterface.GetRootWidget(), g_OutputMind->GetCharacter()->GetShip()->GetCurrentSystem(), g_OutputMind->GetCharacter());
				g_MapDialog->GrabKeyFocus();
				g_MapDialog->AddDestroyListener(&g_GlobalDestroyListener);
				g_InputMind->DisableAccelerate();
				g_InputMind->DisableTurnLeft();
				g_InputMind->DisableTurnRight();
			}
			
			break;
		}
	case 59: // Key: COMMA
		{
			SetTimeWarp(g_TimeWarp * 1.1f);
			
			break;
		}
	case 60: // Key: PERIODE
		{
			SetTimeWarp(g_TimeWarp / 1.1f);
			
			break;
		}
	case 65: // Key: SPACE
		{
			if(g_InputMind != 0)
			{
				g_InputMind->EnableFire();
			}
			
			break;
		}
	case 67: // Key: F1
		{
			XMLStream XML(std::cout);
			
			XML << element << "save";
			XML << element << "system" << attribute << "identifier" << value << g_CurrentSystem->GetIdentifier() << end;
			XML << element << "time-warp" << attribute << "value" << value << g_TimeWarp << end;
			XML << element << "character";
			XML << element << "credits" << attribute << "value" << value << g_InputMind->GetCharacter()->GetCredits() << end;
			XML << element << "map-knowledge";
			
			const std::set< System * > & ExploredSystems(g_InputMind->GetCharacter()->GetMapKnowledge()->GetExploredSystems());
			
			for(std::set< System * >::const_iterator ExploredSystemIterator = ExploredSystems.begin(); ExploredSystemIterator != ExploredSystems.end(); ++ExploredSystemIterator)
			{
				XML << element << "explored-system" << attribute << "identifier" << value << (*ExploredSystemIterator)->GetIdentifier() << end;
			}
			XML << end; // map-knowledge
			XML << end; // character
			
			Ship * Ship(g_InputMind->GetCharacter()->GetShip());
			
			XML << element << "ship" << attribute << "class-identifier" << value << Ship->GetShipClass()->GetIdentifier() << attribute << "object-identifier" << value << Ship->GetObjectIdentifier();
			XML << element << "fuel" << attribute << "value" << value << Ship->GetFuel() << end;
			XML << element << "hull" << attribute << "value" << value << Ship->GetHull() << end;
			XML << element << "position" << attribute << "x" << value << Ship->GetPosition().m_V.m_A[0] << attribute << "y" << value << Ship->GetPosition().m_V.m_A[1] << end;
			XML << element << "angular-position" << attribute << "value" << value << Ship->GetAngularPosition() << end;
			XML << element << "velocity" << attribute << "x" << value << Ship->GetVelocity().m_V.m_A[0] << attribute << "y" << value << Ship->GetVelocity().m_V.m_A[1] << end;
			XML << element << "manifest";
			
			std::set< Object * >::const_iterator ManifestIterator(Ship->GetManifest().begin());
			
			while(ManifestIterator != Ship->GetManifest().end())
			{
				Cargo * TheCargo(dynamic_cast< Cargo * >(*ManifestIterator));
				
				if(TheCargo != 0)
				{
					XML << element << "cargo" << attribute << "commodity-identifier" << value << TheCargo->GetCommodity()->GetIdentifier() << end;
				}
				++ManifestIterator;
			}
			XML << end; // manifest
			XML << end; // ship
			XML << element << "camera";
			XML << element << "position" << attribute << "x" << value << g_Camera.GetPosition().m_V.m_A[0] << attribute << "y" << value << g_Camera.GetPosition().m_V.m_A[1] << attribute << "z" << value << g_Camera.GetPosition().m_V.m_A[2] << end;
			if(g_Camera.GetFocus()->GetObjectIdentifier() == "")
			{
				g_Camera.GetFocus()->GenerateObjectIdentifier();
			}
			XML << element << "focus" << attribute << "object-identifier" << value << g_Camera.GetFocus()->GetObjectIdentifier() << end;
			XML << element << "field-of-view" << attribute << "radians" << value << g_Camera.GetFieldOfView() << end;
			XML << end; // camera
			XML << end; // save
			
			break;
		}
	case 68: // Key: F2
		{
			XMLStream Out(std::cout);
			
			Object::Dump(Out);
			std::cout << std::endl;
			
			break;
		}
	case 97: // Key: HOME
		{
			g_Camera.SetPosition(0.0f, 0.0f);
			
			break;
		}
	case 98: // Key: UP
		{
			if(g_InputMind != 0)
			{
				g_InputMind->EnableAccelerate();
			}
			
			break;
		}
	case 100: // Key: LEFT
		{
			if(g_InputMind != 0)
			{
				g_InputMind->EnableTurnLeft();
			}
			
			break;
		}
	case 102: // Key: RIGHT
		{
			if(g_InputMind != 0)
			{
				g_InputMind->EnableTurnRight();
			}
			
			break;
		}
	}
}

void KeyUp(unsigned char KeyCode)
{
	if(g_UserInterface.Key(KeyCode, EV_UP) == true)
	{
		return;
	}
	switch(KeyCode)
	{
	case 65: // Key: SPACE
		{
			if(g_InputMind != 0)
			{
				g_InputMind->DisableFire();
			}
			
			break;
		}
	case 98:  // Key: UP
		{
			if(g_InputMind != 0)
			{
				g_InputMind->DisableAccelerate();
			}
			
			break;
		}
	case 100: // Key: LEFT
		{
			if(g_InputMind != 0)
			{
				g_InputMind->DisableTurnLeft();
			}
			
			break;
		}
	case 102: // Key: RIGHT
		{
			if(g_InputMind != 0)
			{
				g_InputMind->DisableTurnRight();
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
	// TODO: Make configurable via data.arx
	math3d::vector4f GlobalAmbientLightColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GlobalAmbientLightColor.m_V.m_A);
	Resize();
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
				std::cout << "Motion:         x=" << Event.xbutton.x << "   y=" << Event.xbutton.y << std::endl;
				MouseMotion(Event.xmotion.x, Event.xmotion.y);
				
				break;
			}
		case ButtonPress:
			{
				std::cout << "ButtonPress:    button=" << Event.xbutton.button << "   x=" << Event.xbutton.x << "   y=" << Event.xbutton.y << std::endl;
				MouseButtonDown(Event.xbutton.button, Event.xbutton.x, Event.xbutton.y);
				
				break;
			}
		case ButtonRelease:
			{
				std::cout << "ButtonRelease:  button=" << Event.xbutton.button << "   x=" << Event.xbutton.x << "   y=" << Event.xbutton.y << std::endl;
				MouseButtonUp(Event.xbutton.button, Event.xbutton.x, Event.xbutton.y);
				
				break;
			}
		case KeyPress:
			{
				std::cout << "KeyPress:       state=" << Event.xkey.state << "   keycode=" << Event.xkey.keycode << std::endl;
				KeyDown(Event.xkey.keycode);
				
				break;
			}
		case KeyRelease:
			{
				std::cout << "KeyRelease:     state=" << Event.xkey.state << "   keycode=" << Event.xkey.keycode << std::endl;
				KeyUp(Event.xkey.keycode);
				
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

void LoadSavegame(const Element * SaveElement)
{
	if(SaveElement->GetName() != "save")
	{
		throw std::runtime_error("The savegame is not rooted at a \"save\" element.");
	}
	
	const std::vector< Element * > & SaveChilds(SaveElement->GetChilds());
	std::string System;
	Character * PlayerCharacter(0);
	Ship * PlayerShip(0);
	
	for(std::vector< Element * >::const_iterator SaveChild = SaveChilds.begin(); SaveChild != SaveChilds.end(); ++SaveChild)
	{
		if((*SaveChild)->GetName() == "system")
		{
			System = (*SaveChild)->GetAttribute("identifier");
		}
		else if((*SaveChild)->GetName() == "time-warp")
		{
			SetTimeWarp(from_string_cast< float >((*SaveChild)->GetAttribute("value")));
		}
		else if((*SaveChild)->GetName() == "character")
		{
			PlayerCharacter = new Character();
			PlayerCharacter->SetObjectIdentifier((*SaveChild)->GetAttribute("object-identifier"));
			g_OutputMind = g_InputMind = new CommandMind();
			PlayerCharacter->PossessByMind(g_InputMind);
			g_InputMind->SetCharacter(PlayerCharacter);
			g_OutputMind->SetCharacter(PlayerCharacter);
			for(std::vector< Element * >::const_iterator CharacterChild = (*SaveChild)->GetChilds().begin(); CharacterChild != (*SaveChild)->GetChilds().end(); ++CharacterChild)
			{
				if((*CharacterChild)->GetName() == "credits")
				{
					PlayerCharacter->SetCredits(from_string_cast< float >((*CharacterChild)->GetAttribute("value")));
				}
				else if((*CharacterChild)->GetName() == "map-knowledge")
				{
					for(std::vector< Element * >::const_iterator MapKnowledgeChild = (*CharacterChild)->GetChilds().begin(); MapKnowledgeChild != (*CharacterChild)->GetChilds().end(); ++MapKnowledgeChild)
					{
						if((*MapKnowledgeChild)->GetName() == "explored-system")
						{
							PlayerCharacter->GetMapKnowledge()->AddExploredSystem(g_SystemManager.Get((*MapKnowledgeChild)->GetAttribute("identifier")));
						}
					}
				}
			}
		}
		else if((*SaveChild)->GetName() == "ship")
		{
			PlayerShip = new Ship(g_ShipClassManager.Get((*SaveChild)->GetAttribute("class-identifier")));
			PlayerShip->SetObjectIdentifier((*SaveChild)->GetAttribute("object-identifier"));
			for(std::vector< Element * >::const_iterator ShipChild = (*SaveChild)->GetChilds().begin(); ShipChild != (*SaveChild)->GetChilds().end(); ++ShipChild)
			{
				if((*ShipChild)->GetName() == "fuel")
				{
					PlayerShip->SetFuel(from_string_cast< float >((*ShipChild)->GetAttribute("value")));
				}
				else if((*ShipChild)->GetName() == "hull")
				{
					PlayerShip->SetHull(from_string_cast< float >((*ShipChild)->GetAttribute("value")));
				}
				else if((*ShipChild)->GetName() == "position")
				{
					PlayerShip->SetPosition(math3d::vector2f(from_string_cast< float >((*ShipChild)->GetAttribute("x")), from_string_cast< float >((*ShipChild)->GetAttribute("y"))));
				}
				else if((*ShipChild)->GetName() == "velocity")
				{
					PlayerShip->SetVelocity(math3d::vector2f(from_string_cast< float >((*ShipChild)->GetAttribute("x")), from_string_cast< float >((*ShipChild)->GetAttribute("y"))));
				}
				else if((*ShipChild)->GetName() == "angular-position")
				{
					PlayerShip->SetAngularPosition(from_string_cast< float >((*ShipChild)->GetAttribute("value")));
				}
				else if((*ShipChild)->GetName() == "manifest")
				{
					for(std::vector< Element * >::const_iterator ManifestChild = (*ShipChild)->GetChilds().begin(); ManifestChild != (*ShipChild)->GetChilds().end(); ++ManifestChild)
					{
						if((*ManifestChild)->GetName() == "cargo")
						{
							PlayerShip->AddObject(new Cargo(g_CommodityManager.Get((*ManifestChild)->GetAttribute("commodity-identifier"))));
						}
					}
				}
				else if((*ShipChild)->GetName() == "name")
				{
					PlayerShip->SetName((*ShipChild)->GetAttribute("value"));
				}
				else
				{
					throw std::runtime_error("The \"ship\" element \"" + (*SaveChild)->GetAttribute("object-identifier") + "\" contains an unidentified child element \"" + (*ShipChild)->GetName() + "\".");
				}
			}
		}
		else if((*SaveChild)->GetName() == "camera")
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
					
					if(FocusPosition != 0)
					{
						g_Camera.SetFocus(FocusPosition);
					}
					else
					{
						g_Camera.SetFocus(PlayerShip);
					}
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
			}
		}
	}
	g_CurrentSystem = g_SystemManager.Get(System);
	OnOutputFocusEnterSystem(g_CurrentSystem);
	if(PlayerShip != 0)
	{
		PlayerCharacter->SetShip(PlayerShip);
		PlayerShip->AddObject(PlayerCharacter);
		g_CurrentSystem->AddShip(PlayerShip);
		PlayerShip->SetCurrentSystem(g_CurrentSystem);
	}
	RealTime::Invalidate();
	PopulateSystem(g_CurrentSystem);
}

bool LoadSavegame(const std::string & LoadSavegameFileName)
{
	std::ifstream InputStream(LoadSavegameFileName.c_str());
	
	if(InputStream == false)
	{
		std::cerr << "The savegame file \"" << LoadSavegameFileName << "\" does not exist or is not readable." << std::endl;
		
		return false;
	}
	
	Document SavegameDocument(InputStream);
	
	if(SavegameDocument.GetDocumentElement() == 0)
	{
		std::cerr << "The file \"" << LoadSavegameFileName << "\" does not seem to be parsable." << std::endl;
		
		return false;
	}
	else
	{
		try
		{
			LoadSavegame(SavegameDocument.GetDocumentElement());
		}
		catch(std::runtime_error & Exception)
		{
			std::cerr << "The file \"" << LoadSavegameFileName << "\" contains invalid data:\n\t" << Exception.what() << std::endl;
			
			return false;
		}
		
		return true;
	}
}

int main(int argc, char ** argv)
{
	// setup the random number generator for everyday use
	srand(time(0));
	// static initialization of data independent globals
	g_MainPerspective.SetNearClippingPlane(1.0f);
	g_MainPerspective.SetFarClippingPlane(1000.f);
	g_MessageTimeoutIterator = g_RealTimeTimeoutNotifications.end();
	g_SpawnShipTimeoutIterator = g_GameTimeTimeoutNotifications.end();
	
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
	}
	
	// ui setup
	Arxx::URI URI(DataFileName);
	Arxx::Archive Archive;
	
	if(Archive.bLoad(URI) == false)
	{
		std::cerr << "Could not find or open " << URI << "." << std::endl;
		
		return 1;
	}
	// data reading
	ReadModels(Archive, &g_ModelManager);
	ReadCommodities(Archive, &g_CommodityManager);
	ReadShipClasses(Archive, &g_ShipClassManager);
	ReadSystems(Archive, &g_SystemManager);
	ReadSystemLinks(Archive, &g_SystemManager);
	ReadUserInterface(Archive, &g_UserInterface);
	
	g_CreditsLabel = dynamic_cast< Label * >(g_UserInterface.GetWidget("/credits"));
	g_FuelLabel = dynamic_cast< Label * >(g_UserInterface.GetWidget("/fuel"));
	g_HullLabel = dynamic_cast< Label * >(g_UserInterface.GetWidget("/hull"));
	g_MessageLabel = dynamic_cast< Label * >(g_UserInterface.GetWidget("/message"));
	g_SystemLabel = dynamic_cast< Label * >(g_UserInterface.GetWidget("/system"));
	g_TimeWarpLabel = dynamic_cast< Label * >(g_UserInterface.GetWidget("/time_warp"));
	g_MiniMap = dynamic_cast< MiniMap * >(g_UserInterface.GetWidget("/mini_map"));
		g_CurrentSystemLabel = dynamic_cast< Label * >(g_UserInterface.GetWidget("/mini_map/current_system"));
	g_Scanner = g_UserInterface.GetWidget("/scanner");
		g_TargetLabel = dynamic_cast< Label * >(g_UserInterface.GetWidget("/scanner/target"));
		g_ScannerDisplay = dynamic_cast< ScannerDisplay * >(g_UserInterface.GetWidget("/scanner/display"));
	
	// initialize the player (initial load)
	if(LoadSavegame(LoadSavegameFileName) == false)
	{
		return 1;
	}
	
	// setting up the player environment
	if((g_OutputMind != 0) && (g_OutputMind->GetCharacter() != 0) && (g_OutputMind->GetCharacter()->GetShip() != 0))
	{
		g_MiniMap->SetFocus(g_OutputMind->GetCharacter()->GetShip());
		g_ScannerDisplay->SetFocus(g_OutputMind->GetCharacter()->GetShip());
	}
	// set first timeout for widget collector, it will reinsert itself on callback
	g_RealTimeTimeoutNotifications.insert(std::make_pair(RealTime::GetTime() + 5.0f, Function(CollectWidgets)));
	// setting up the graphical environment
	CreateWindow();
	InitializeOpenGL();
	while(g_Quit == false)
	{
		ProcessEvents();
		GameFrame();
		glXSwapBuffers(g_Display, g_Window);
	}
	DestroyWindow();
	
	return 0;
}

