#include <errno.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

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
#include "commodity.h"
#include "commodity_manager.h"
#include "destroy_listener.h"
#include "globals.h"
#include "label.h"
#include "map_dialog.h"
#include "map_knowledge.h"
#include "model.h"
#include "model_manager.h"
#include "planet.h"
#include "planet_dialog.h"
#include "real_time.h"
#include "ship.h"
#include "ship_class.h"
#include "ship_class_manager.h"
#include "star.h"
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
Camera g_RadarCamera;
Camera g_MiniMapCamera;
float g_GameTime;
ModelManager g_ModelManager;
ShipClassManager g_ShipClassManager(&g_ModelManager);
CommodityManager g_CommodityManager;
SystemManager g_SystemManager(&g_CommodityManager);
Character * g_PlayerCharacter(0);
Ship * g_PlayerShip(0);
Ship * g_InputFocus(0);
float g_Width(0.0f);
float g_Height(0.0f);
Label * g_SystemLabel(0);
Label * g_TimeWarpLabel(0);
Label * g_TargetLabel(0);
Label * g_CreditsLabel(0);
Label * g_FuelLabel(0);
Label * g_MessageLabel(0);
Label * g_CurrentSystemLabel(0);
System * g_CurrentSystem;
float g_TimeWarp(1.0f);
bool g_Quit(false);
bool g_Pause(false);
PlanetDialog * g_PlanetDialog;
MapDialog * g_MapDialog;
int g_WidgetCollectorCountDownInitializer(10);
int g_WidgetCollectorCountDown(g_WidgetCollectorCountDownInitializer);
UserInterface g_UserInterface;
std::multimap< double, Callback0< void > * > g_TimeoutNotifications;
Widget * g_RadarWidget(0);
Widget * g_MiniMapWidget(0);
Display * g_Display;
GLXContext g_GLXContext;
Window g_Window;

void NormalizeRadians(float & Radians)
{
	while(Radians < 0.0f)
	{
		Radians += 2 * M_PI;
	}
	while(Radians > 2 * M_PI)
	{
		Radians -= 2 * M_PI;
	}
}

float NormalizedRadians(float Radians)
{
	while(Radians < 0.0f)
	{
		Radians += 2 * M_PI;
	}
	while(Radians > 2 * M_PI)
	{
		Radians -= 2 * M_PI;
	}
	
	return Radians;
}

float GetRadians(const math3d::vector2f & Vector)
{
	float Radians(acosf(Vector.m_V.m_A[0]));
	
	if(Vector.m_V.m_A[1] >= 0)
	{
		return Radians;
	}
	else
	{
		return NormalizedRadians(-Radians);
	}
}

enum WantToJumpCode
{
	OK_TO_JUMP,
	TOO_NEAR_TO_SYSTEM_CENTER,
	NOT_ENOUGH_FUEL
};

int WantToJump(Ship * Ship)
{
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
	
	return OK_TO_JUMP;
}

float CalculateTime(void)
{
	double DeltaSeconds(RealTime::GetInterval());
	
	if(g_Pause == false)
	{
		DeltaSeconds *= g_TimeWarp;
		g_GameTime += DeltaSeconds;
		
		return DeltaSeconds;
	}
	else
	{
		return 0.0f;
	}
}

void SetMessage(const std::string & Message)
{
	g_MessageLabel->SetString(Message);
	g_MessageLabel->SetPosition(math3d::vector2f((g_Width - 6 * Message.length()) / 2, 40.0f));
	g_MessageLabel->Show();
	/// TODO: Make the 2.0f seconds timeout configurable via the game configuration archive.
	g_TimeoutNotifications.insert(std::make_pair(RealTime::GetTime() + 2.0f, new MemberCallback0< void, Label >(g_MessageLabel, &Label::Hide)));
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

void DrawPlanetSelection(Planet * Planet)
{
	DrawSelection(Planet, Planet->GetRadialSize());
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
	g_TimeoutNotifications.insert(std::make_pair(RealTime::GetTime() + 5.0f, new FunctionCallback0< void >(CollectWidgets)));
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

void Render(void)
{
	float Seconds(CalculateTime());
	
	if(g_CurrentSystem != 0)
	{
		const std::list< Ship * > Ships(g_CurrentSystem->GetShips());
		const std::list< Cargo * > Cargos(g_CurrentSystem->GetCargos());
		
		for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
		{
			(*ShipIterator)->Move(Seconds);
		}
		for(std::list< Cargo * >::const_iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
		{
			(*CargoIterator)->Move(Seconds);
		}
		// player ship has been updated, display new fuel
		g_FuelLabel->SetString("Fuel: " + to_string_cast(100.0f * g_PlayerShip->GetFuel() / g_PlayerShip->GetFuelCapacity()) + "%");
		// display credits in every cycle
		g_CreditsLabel->SetString("Credits: " + to_string_cast(g_PlayerCharacter->GetCredits()));
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, static_cast< GLsizei >(g_Width), static_cast< GLsizei >(g_Height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0 * g_Width / g_Height, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_DEPTH_BUFFER_BIT);
	g_Camera.Draw();
	
	const Star * CurrentStar(g_CurrentSystem->GetStar());
	
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
	if(g_CurrentSystem != 0)
	{
		const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
		const std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
		const std::list< Cargo * > & Cargos(g_CurrentSystem->GetCargos());
		
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
	}
	// HUD
	if(g_PlayerShip->GetTarget() != 0)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		DrawSelection(g_PlayerShip->GetTarget(), g_PlayerShip->GetTarget()->GetRadialSize());
		
		math3d::vector2f RelativePosition(g_PlayerShip->GetTarget()->GetPosition() - g_PlayerShip->GetPosition());
		
		RelativePosition.normalize();
		glPushMatrix();
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glTranslatef(g_PlayerShip->GetPosition().m_V.m_A[0], g_PlayerShip->GetPosition().m_V.m_A[1], 0.0f);
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
	// user interface updates
	if((g_PlayerShip->GetLinkedSystemTarget() != 0) && (WantToJump(g_PlayerShip) == OK_TO_JUMP))
	{
		g_SystemLabel->GetForegroundColor().Set(0.7f, 0.8f, 1.0f);
	}
	else
	{
		g_SystemLabel->GetForegroundColor().Set(0.4f, 0.4f, 0.4f);
	}
	
	double StopTime(RealTime::GetTime());
	
	while((g_TimeoutNotifications.size() > 0) && (StopTime > g_TimeoutNotifications.begin()->first))
	{
		// call the notification callback object
		(*(g_TimeoutNotifications.begin()->second))();
		// delete the notification callback object
		delete g_TimeoutNotifications.begin()->second;
		// remove the notification callback from the multimap
		g_TimeoutNotifications.erase(g_TimeoutNotifications.begin());
	}
	DisplayUserInterface();
	// radar
	if(g_PlayerShip->GetTarget() != 0)
	{
		float RadialSize(g_PlayerShip->GetTarget()->GetRadialSize());
		float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
		
		glViewport(0, 0, 220, 220);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(360.0f * asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)) / M_PI, 1.0, 1, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_DEPTH_BUFFER_BIT);
		g_RadarCamera.SetPosition(0.0f, 0.0f, 4.0f * RadialSize);
		g_RadarCamera.SetFocus(g_PlayerShip->GetTarget());
		g_RadarCamera.Draw();
		if(CurrentStar != 0)
		{
			glLightfv(GL_LIGHT0, GL_POSITION, math3d::vector4f(CurrentStar->GetPosition().m_V.m_A[0], CurrentStar->GetPosition().m_V.m_A[1], 100.0f, 0.0f).m_V.m_A);
		}
		g_PlayerShip->GetTarget()->Draw();
	}
	// mini map
	if(g_CurrentSystem != 0)
	{
		glViewport(static_cast< GLint >(g_Width - 220.0f), 0, 220, 220);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, 1.0, 1, 10000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_DEPTH_BUFFER_BIT);
		g_MiniMapCamera.Draw();
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
		const std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
		const std::list< Cargo * > & Cargos(g_CurrentSystem->GetCargos());
		
		glBegin(GL_POINTS);
		glColor3f(0.8f, 0.8f, 0.8f);
		for(std::list< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
		{
			if(*PlanetIterator == g_PlayerShip->GetTarget())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*PlanetIterator)->GetPosition().m_V.m_A[0], (*PlanetIterator)->GetPosition().m_V.m_A[1]);
			if(*PlanetIterator == g_PlayerShip->GetTarget())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
		{
			// Ship is not an object yet
			if(*ShipIterator == g_PlayerShip->GetTarget())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*ShipIterator)->GetPosition().m_V.m_A[0], (*ShipIterator)->GetPosition().m_V.m_A[1]);
			if(*ShipIterator == g_PlayerShip->GetTarget())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Cargo * >::const_iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
		{
			if(*CargoIterator == g_PlayerShip->GetTarget())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*CargoIterator)->GetPosition().m_V.m_A[0], (*CargoIterator)->GetPosition().m_V.m_A[1]);
			if(*CargoIterator == g_PlayerShip->GetTarget())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		glEnd();
		glPopAttrib();
	}
}

void Resize(void)
{
	if(g_Height == 0)
	{
		g_Height = 1;
	}
	glViewport(0, 0, static_cast< GLint >(g_Width), static_cast< GLint >(g_Height));
	g_UserInterface.GetRootWidget()->SetSize(math3d::vector2f(g_Width, g_Height));
	g_RadarWidget->SetPosition(math3d::vector2f(0.0f, g_Height - 240.0f));
	g_MiniMapWidget->SetPosition(math3d::vector2f(g_Width - 220.0f, g_Height - 240.0f));
}

void SelectLinkedSystem(System * LinkedSystem)
{
	g_PlayerShip->SetLinkedSystemTarget(LinkedSystem);
	if(g_PlayerShip->GetLinkedSystemTarget() != 0)
	{
		const std::set< System * > UnexploredSystems(g_PlayerCharacter->GetMapKnowledge()->GetUnexploredSystems());
		
		if(UnexploredSystems.find(g_PlayerShip->GetLinkedSystemTarget()) == UnexploredSystems.end())
		{
			g_SystemLabel->SetString(g_PlayerShip->GetLinkedSystemTarget()->GetName());
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
}

void SelectPlanet(Planet * Planet)
{
	g_PlayerShip->SetTarget(Planet);
	if(g_PlayerShip->GetTarget() != 0)
	{
		g_TargetLabel->SetString(Planet->GetName());
	}
	else
	{
		g_TargetLabel->SetString("");
	}
}

void SelectCargo(Cargo * Cargo)
{
	g_PlayerShip->SetTarget(Cargo);
	if(g_PlayerShip->GetTarget() != 0)
	{
		g_TargetLabel->SetString(Cargo->GetCommodity()->GetName());
	}
	else
	{
		g_TargetLabel->SetString("");
	}
}

void SelectShip(Ship * Ship)
{
	g_PlayerShip->SetTarget(Ship);
	if(g_PlayerShip->GetTarget() != 0)
	{
		g_TargetLabel->SetString(Ship->GetShipClass()->GetIdentifier());
	}
	else
	{
		g_TargetLabel->SetString("");
	}
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
			SelectLinkedSystem(0);
			SelectPlanet(0);
			g_PlayerShip->m_Velocity.set(0.0f, 0.0f);
			g_InputFocus->m_Accelerate = false;
			g_InputFocus->m_TurnLeft = false;
			g_InputFocus->m_TurnRight = false;
		}
		else if(EventSource == g_MapDialog)
		{
			if(g_CurrentSystem->IsLinkedToSystem(g_MapDialog->GetSelectedSystem()) == true)
			{
				SelectLinkedSystem(g_MapDialog->GetSelectedSystem());
			}
			g_MapDialog = 0;
			g_Pause = false;
			g_InputFocus->m_Accelerate = false;
			g_InputFocus->m_TurnLeft = false;
			g_InputFocus->m_TurnRight = false;
		}
	}
} g_GlobalDestroyListener;

void LeaveSystem(void)
{
	if(g_CurrentSystem != 0)
	{
		g_CurrentSystem->ClearShips();
		g_CurrentSystem->ClearCargos();
		g_CurrentSystem = 0;
	}
	g_CurrentSystemLabel->SetString("");
	SelectLinkedSystem(0);
	SelectPlanet(0);
}

void EnterSystem(System * NewSystem, System * OldSystem)
{
	g_CurrentSystem = NewSystem;
	if(OldSystem != 0)
	{
		math3d::vector2f Direction(NewSystem->GetPosition() - OldSystem->GetPosition());
		
		Direction.normalize();
		g_PlayerShip->m_Position = Direction * -300.0f;
		g_PlayerShip->m_Velocity = Direction * g_PlayerShip->GetShipClass()->GetMaximumSpeed();
		
		float Radians(acosf(Direction.m_V.m_A[0]));
		
		if(Direction.m_V.m_A[1] >= 0)
		{
			g_PlayerShip->m_AngularPosition = Radians;
		}
		else
		{
			g_PlayerShip->m_AngularPosition = -Radians;
		}
	}
	g_CurrentSystem->AddShip(g_PlayerShip);
	g_CurrentSystemLabel->SetString(g_CurrentSystem->GetName());
	SelectLinkedSystem(0);
	SelectPlanet(0);
	g_PlayerCharacter->GetMapKnowledge()->AddExploredSystem(g_CurrentSystem);
}

void SetTimeWarp(float TimeWarp)
{
	g_TimeWarp = TimeWarp;
	
	std::stringstream TimeWarpString;
	
	TimeWarpString << "Time Warp: " << g_TimeWarp;
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
			g_Camera.ZoomIn();
			
			break;
		}
	case 5: // MouseButton: WHEEL_DOWN
		{
			g_Camera.ZoomOut();
			
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
	case 27: // Key: R
		{
			const std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
			Ship * SelectedShip(dynamic_cast< Ship * >(g_PlayerShip->GetTarget()));
			
			if(SelectedShip == 0)
			{
				if(Ships.size() > 0)
				{
					SelectShip(Ships.front());
				}
			}
			else
			{
				std::list< Ship * >::const_iterator ShipIterator(find(Ships.begin(), Ships.end(), SelectedShip));
				
				if(ShipIterator == Ships.end())
				{
					SelectShip(0);
				}
				else
				{
					++ShipIterator;
					if(ShipIterator == Ships.end())
					{
						SelectShip(0);
					}
					else
					{
						SelectShip(*ShipIterator);
					}
				}
			}
			
			break;
		}
	case 28: // Key: T
		{
			const std::list< Cargo * > & Cargos(g_CurrentSystem->GetCargos());
			Cargo * SelectedCargo(dynamic_cast< Cargo * >(g_PlayerShip->GetTarget()));
			
			if(SelectedCargo == 0)
			{
				if(Cargos.size() > 0)
				{
					SelectCargo(Cargos.front());
				}
			}
			else
			{
				std::list< Cargo * >::const_iterator CargoIterator(find(Cargos.begin(), Cargos.end(), SelectedCargo));
				
				if(CargoIterator == Cargos.end())
				{
					SelectCargo(0);
				}
				else
				{
					++CargoIterator;
					if(CargoIterator == Cargos.end())
					{
						SelectCargo(0);
					}
					else
					{
						SelectCargo(*CargoIterator);
					}
				}
			}
			
			break;
		}
	case 33: // Key: P
		{
			const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
			Planet * SelectedPlanet(dynamic_cast< Planet * >(g_PlayerShip->GetTarget()));
			
			if(SelectedPlanet != 0)
			{
				std::list< Planet * >::const_iterator PlanetIterator(find(Planets.begin(), Planets.end(), SelectedPlanet));
				
				if(PlanetIterator == Planets.end())
				{
					SelectPlanet(0);
				}
				else
				{
					++PlanetIterator;
					if(PlanetIterator == Planets.end())
					{
						SelectPlanet(0);
					}
					else
					{
						SelectPlanet(*PlanetIterator);
					}
				}
			}
			else
			{
				if(Planets.size() > 0)
				{
					SelectPlanet(Planets.front());
				}
			}
			
			break;
		}
	case 35: // Key: ]
		{
			const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
			const Planet * FocusPlanet(dynamic_cast< const Planet * >(g_Camera.GetFocus()));
			
			if(FocusPlanet != 0)
			{
				std::list< Planet * >::const_iterator PlanetIterator(find(Planets.begin(), Planets.end(), FocusPlanet));
				
				if(PlanetIterator == Planets.end())
				{
					g_Camera.SetFocus(g_PlayerShip);
				}
				else
				{
					++PlanetIterator;
					if(PlanetIterator == Planets.end())
					{
						g_Camera.SetFocus(g_PlayerShip);
					}
					else
					{
						g_Camera.SetFocus(*PlanetIterator);
					}
				}
			}
			else
			{
				if(Planets.size() > 0)
				{
					g_Camera.SetFocus(Planets.front());
				}
			}
			
			break;
		}
	case 39: // Key: S
		{
			Cargo * SelectedCargo(dynamic_cast< Cargo * >(g_PlayerShip->GetTarget()));
			
			if(SelectedCargo == 0)
			{
				const std::list< Cargo * > & Cargos(g_CurrentSystem->GetCargos());
				float MinimumDistance(0.0f);
				Cargo * MinimumCargo(0);
				
				for(std::list< Cargo * >::const_iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
				{
					if(MinimumCargo == 0)
					{
						MinimumCargo = *CargoIterator;
						MinimumDistance = (MinimumCargo->GetPosition() - g_PlayerShip->GetPosition()).length_squared();
					}
					else
					{
						float Distance(((*CargoIterator)->GetPosition() - g_PlayerShip->GetPosition()).length_squared());
						
						if(Distance < MinimumDistance)
						{
							MinimumCargo = *CargoIterator;
							MinimumDistance = Distance;
						}
					}
				}
				if(MinimumCargo != 0)
				{
					SelectCargo(MinimumCargo);
				}
			}
			else
			{
				// test distance
				if((SelectedCargo->GetPosition() - g_PlayerShip->GetPosition()).length_squared() <= 5.0f * SelectedCargo->GetRadialSize() * SelectedCargo->GetRadialSize())
				{
					// test speed
					if((g_PlayerShip->GetVelocity() - SelectedCargo->GetVelocity()).length_squared() <= 2.0f)
					{
						g_PlayerShip->AddCommodities(SelectedCargo->GetCommodity(), 1.0f);
						g_CurrentSystem->RemoveCargo(SelectedCargo);
						SelectCargo(0);
						delete SelectedCargo;
					}
					else
					{
						SetMessage("Your relative velocity to the cargo is too high to scoop it up.");
					}
				}
				else
				{
					SetMessage("You are too far away from the cargo to scoop it up.");
				}
			}
			
			break;
		}
	case 44: // Key: J
		{
			if(g_PlayerShip->GetLinkedSystemTarget() != 0)
			{
				switch(WantToJump(g_PlayerShip))
				{
				case OK_TO_JUMP:
					{
						System * OldSystem(g_CurrentSystem);
						System * NewSystem(g_PlayerShip->GetLinkedSystemTarget());
						
						LeaveSystem();
						g_PlayerShip->Jump();
						EnterSystem(NewSystem, OldSystem);
						
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
				}
			}
			
			break;
		}
	case 46: // Key: L
		{
			if(g_PlanetDialog == 0)
			{
				Planet * SelectedPlanet(dynamic_cast< Planet * >(g_PlayerShip->GetTarget()));
				
				if(SelectedPlanet != 0)
				{
					// test distance
					if((SelectedPlanet->GetPosition() - g_PlayerShip->GetPosition()).length_squared() <= SelectedPlanet->GetSize() * SelectedPlanet->GetSize())
					{
						// test speed (should be relative speed but planets have no speed, yet)
						if(g_PlayerShip->GetVelocity().length_squared() <= 2.0f)
						{
							if(g_PlayerCharacter->GetCredits() >= SelectedPlanet->GetLandingFee())
							{
								g_PlayerCharacter->RemoveCredits(SelectedPlanet->GetLandingFee());
								g_Pause = true;
								g_PlanetDialog = new PlanetDialog(g_UserInterface.GetRootWidget(), SelectedPlanet);
								g_PlanetDialog->GrabKeyFocus();
								g_PlanetDialog->AddDestroyListener(&g_GlobalDestroyListener);
							}
							else
							{
								SetMessage("You don't have enough credits to pay the landing fee.");
							}
						}
						else
						{
							SetMessage("You are too fast to land on the planet.");
						}
					}
					else
					{
						SetMessage("You are too far away from the planet to land.");
					}
				}
				else
				{
					const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
					float MinimumDistance(0.0f);
					Planet * MinimumPlanet(0);
					
					for(std::list< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
					{
						if(MinimumPlanet == 0)
						{
							MinimumPlanet = *PlanetIterator;
							MinimumDistance = (MinimumPlanet->GetPosition() - g_PlayerShip->GetPosition()).length_squared();
						}
						else
						{
							float Distance(((*PlanetIterator)->GetPosition() - g_PlayerShip->GetPosition()).length_squared());
							
							if(Distance < MinimumDistance)
							{
								MinimumPlanet = *PlanetIterator;
								MinimumDistance = Distance;
							}
						}
					}
					if(MinimumPlanet != 0)
					{
						SelectPlanet(MinimumPlanet);
					}
				}
			}
			
			break;
		}
	case 54: // Key: C
		{
			SetMessage("Jettison cargo.");
			while(g_PlayerShip->GetCommodities().size() > 0)
			{
				const Commodity * Commodity(g_PlayerShip->GetCommodities().begin()->first);
				Cargo * NewCargo(new Cargo(g_ModelManager.Get("cargo_cube"), Commodity, g_PlayerShip->GetVelocity() * 0.8f + math3d::vector2f(-0.5f + static_cast< float >(random()) / RAND_MAX, -0.5f + static_cast< float >(random()) / RAND_MAX))); // TODO: memory leak
				
				NewCargo->SetObjectIdentifier("::cargo(" + Commodity->GetIdentifier() + ")::" + to_string_cast(reinterpret_cast< void * >(NewCargo)));
				g_PlayerShip->RemoveCommodities(Commodity, 1.0f);
				NewCargo->SetPosition(g_PlayerShip->GetPosition());
				g_CurrentSystem->AddCargo(NewCargo);
			}
			
			break;
		}
	case 57: // Key: N
		{
			const std::list< System * > & LinkedSystems(g_CurrentSystem->GetLinkedSystems());
			
			if(g_PlayerShip->GetLinkedSystemTarget() != 0)
			{
				std::list< System * >::const_iterator SystemIterator(find(LinkedSystems.begin(), LinkedSystems.end(), g_PlayerShip->GetLinkedSystemTarget()));
				
				if(SystemIterator == LinkedSystems.end())
				{
					SelectLinkedSystem(0);
				}
				else
				{
					++SystemIterator;
					if(SystemIterator == LinkedSystems.end())
					{
						SelectLinkedSystem(0);
					}
					else
					{
						SelectLinkedSystem(*SystemIterator);
					}
				}
			}
			else
			{
				if(LinkedSystems.size() > 0)
				{
					SelectLinkedSystem(LinkedSystems.front());
				}
			}
			
			break;
		}
	case 58: // Key: M
		{
			if(g_MapDialog == 0)
			{
				g_Pause = true;
				g_MapDialog = new MapDialog(g_UserInterface.GetRootWidget(), g_CurrentSystem);
				g_MapDialog->GrabKeyFocus();
				g_MapDialog->AddDestroyListener(&g_GlobalDestroyListener);
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
			SetTimeWarp(g_TimeWarp * 0.9f);
			
			break;
		}
	case 65: // Key: SPACE
		{
			g_Camera.SetPosition(0.0f, 0.0f);
			
			break;
		}
	case 67: // Key: F1
		{
			XMLStream XML(std::cout);
			
			XML << element << "save";
			XML << element << "system" << attribute << "identifier" << value << g_CurrentSystem->GetIdentifier() << end;
			XML << element << "character";
			XML << element << "credits" << attribute << "value" << value << g_PlayerCharacter->GetCredits() << end;
			XML << element << "map-knowledge";
			
			const std::set< System * > & ExploredSystems(g_PlayerCharacter->GetMapKnowledge()->GetExploredSystems());
			
			for(std::set< System * >::const_iterator ExploredSystemIterator = ExploredSystems.begin(); ExploredSystemIterator != ExploredSystems.end(); ++ExploredSystemIterator)
			{
				XML << element << "explored-system" << attribute << "identifier" << value << (*ExploredSystemIterator)->GetIdentifier() << end;
			}
			XML << end; // map-knowledge
			XML << end; // character
			XML << element << "ship" << attribute << "class-identifier" << value << g_PlayerShip->GetShipClass()->GetIdentifier();
			XML << element << "fuel" << attribute << "value" << value << g_PlayerShip->GetFuel() << end;
			XML << element << "position" << attribute << "x" << value << g_PlayerShip->GetPosition().m_V.m_A[0] << attribute << "y" << value << g_PlayerShip->GetPosition().m_V.m_A[1] << end;
			XML << element << "angular-position" << attribute << "value" << value << g_PlayerShip->GetAngularPosition() << end;
			XML << element << "velocity" << attribute << "x" << value << g_PlayerShip->GetVelocity().m_V.m_A[0] << attribute << "y" << value << g_PlayerShip->GetVelocity().m_V.m_A[1] << end;
			XML << element << "commodities";
			
			const std::map< const Commodity *, float > & Commodities(g_PlayerShip->GetCommodities());
			
			for(std::map< const Commodity *, float >::const_iterator Commodity = Commodities.begin(); Commodity != Commodities.end(); ++Commodity)
			{
				XML << element << "commodity" << attribute << "identifier" << value << Commodity->first->GetIdentifier() << attribute << "amount" << value << Commodity->second << end;
			}
			XML << end; // commodities
			XML << end; // ship
			XML << element << "camera";
			XML << element << "position" << attribute << "x" << value << g_Camera.GetPosition().m_V.m_A[0] << attribute << "y" << value << g_Camera.GetPosition().m_V.m_A[1] << attribute << "z" << value << g_Camera.GetPosition().m_V.m_A[2] << end;
			if(g_Camera.GetFocus()->GetObjectIdentifier() == "")
			{
				g_Camera.GetFocus()->GenerateObjectIdentifier();
			}
			XML << element << "focus" << attribute << "object-identifier" << value << g_Camera.GetFocus()->GetObjectIdentifier() << end;
			XML << end; // character
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
	case 98: // Key: UP
		{
			g_InputFocus->m_Accelerate = true;
			
			break;
		}
	case 100: // Key: LEFT
		{
			g_InputFocus->m_TurnLeft = true;
			
			break;
		}
	case 102: // Key: RIGHT
		{
			g_InputFocus->m_TurnRight = true;
			
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
	case 98:  // Key: UP
		{
			g_InputFocus->m_Accelerate = false;
			
			break;
		}
	case 100: // Key: LEFT
		{
			g_InputFocus->m_TurnLeft = false;
			
			break;
		}
	case 102: // Key: RIGHT
		{
			g_InputFocus->m_TurnRight = false;
			
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
	WindowAttributes.override_redirect = True;
	// create the window
	g_Window = XCreateWindow(g_Display, RootWindow(g_Display, VisualInfo->screen), 0, 0, static_cast< unsigned int >(g_Width), static_cast< unsigned int >(g_Height), 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect, &WindowAttributes);
	// show the window
	XMapRaised(g_Display, g_Window);
	// grab the keyboard so we get keyboard events and windowmanager can interfere
	XGrabKeyboard(g_Display, g_Window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
	// grab the mouse, so the mouse cursor can't leave our window
	XGrabPointer(g_Display, g_Window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, g_Window, None, CurrentTime);
	// don't allow key repeats
	XAutoRepeatOff(g_Display);
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
	const std::vector< Element * > & SaveChilds(SaveElement->GetChilds());
	std::string System;
	
	for(std::vector< Element * >::const_iterator SaveChild = SaveChilds.begin(); SaveChild != SaveChilds.end(); ++SaveChild)
	{
		if((*SaveChild)->GetName() == "system")
		{
			System = (*SaveChild)->GetAttribute("identifier");
		}
		else if((*SaveChild)->GetName() == "character")
		{
			g_PlayerCharacter = new Character();
			g_PlayerCharacter->SetObjectIdentifier((*SaveChild)->GetAttribute("object-identifier"));
			for(std::vector< Element * >::const_iterator CharacterChild = (*SaveChild)->GetChilds().begin(); CharacterChild != (*SaveChild)->GetChilds().end(); ++CharacterChild)
			{
				if((*CharacterChild)->GetName() == "credits")
				{
					g_PlayerCharacter->SetCredits(from_string_cast< float >((*CharacterChild)->GetAttribute("value")));
				}
				else if((*CharacterChild)->GetName() == "map-knowledge")
				{
					for(std::vector< Element * >::const_iterator MapKnowledgeChild = (*CharacterChild)->GetChilds().begin(); MapKnowledgeChild != (*CharacterChild)->GetChilds().end(); ++MapKnowledgeChild)
					{
						if((*MapKnowledgeChild)->GetName() == "explored-system")
						{
							g_PlayerCharacter->GetMapKnowledge()->AddExploredSystem(g_SystemManager.Get((*MapKnowledgeChild)->GetAttribute("identifier")));
						}
					}
				}
			}
		}
		else if((*SaveChild)->GetName() == "ship")
		{
			g_PlayerShip = new Ship(g_ShipClassManager.Get((*SaveChild)->GetAttribute("class-identifier")));
			g_PlayerShip->SetObjectIdentifier((*SaveChild)->GetAttribute("object-identifier"));
			for(std::vector< Element * >::const_iterator ShipChild = (*SaveChild)->GetChilds().begin(); ShipChild != (*SaveChild)->GetChilds().end(); ++ShipChild)
			{
				if((*ShipChild)->GetName() == "fuel")
				{
					g_PlayerShip->SetFuel(from_string_cast< float >((*ShipChild)->GetAttribute("value")));
				}
				else if((*ShipChild)->GetName() == "position")
				{
					g_PlayerShip->SetPosition(math3d::vector2f(from_string_cast< float >((*ShipChild)->GetAttribute("x")), from_string_cast< float >((*ShipChild)->GetAttribute("y"))));
				}
				else if((*ShipChild)->GetName() == "velocity")
				{
					g_PlayerShip->SetVelocity(math3d::vector2f(from_string_cast< float >((*ShipChild)->GetAttribute("x")), from_string_cast< float >((*ShipChild)->GetAttribute("y"))));
				}
				else if((*ShipChild)->GetName() == "angular-position")
				{
					g_PlayerShip->SetAngularPosition(from_string_cast< float >((*ShipChild)->GetAttribute("value")));
				}
				else if((*ShipChild)->GetName() == "commodities")
				{
					for(std::vector< Element * >::const_iterator CommoditiesChild = (*ShipChild)->GetChilds().begin(); CommoditiesChild != (*ShipChild)->GetChilds().end(); ++CommoditiesChild)
					{
						if((*CommoditiesChild)->GetName() == "commodity")
						{
							g_PlayerShip->SetCommodities(g_CommodityManager.Get((*CommoditiesChild)->GetAttribute("identifier")), from_string_cast< float >((*CommoditiesChild)->GetAttribute("amount")));
						}
					}
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
						g_Camera.SetFocus(g_PlayerShip);
					}
				}
			}
		}
	}
	g_CurrentSystem = g_SystemManager.Get(System);
	g_CurrentSystem->AddShip(g_PlayerShip);
	g_CurrentSystemLabel->SetString(g_CurrentSystem->GetName());
	SelectLinkedSystem(0);
	SelectPlanet(0);
	RealTime::Invalidate();
}

void LoadSavegame(const std::string & LoadSavegameFileName)
{
	std::ifstream InputStream(LoadSavegameFileName.c_str());
	Document SavegameDocument(InputStream);
	
	LoadSavegame(SavegameDocument.GetDocumentElement());
}

int main(int argc, char ** argv)
{
	std::vector< std::string > Arguments(argv, argv + argc);
	std::string LoadSavegameFileName("data/savegame_default.xml");
	
	for(std::vector< std::string >::size_type Index = 0; Index < Arguments.size(); ++Index)
	{
		if((Arguments[Index] == "--load") && (Index + 1 < Arguments.size()))
		{
			LoadSavegameFileName = Arguments[++Index];
		}
	}
	
	// setup the random number generator for everyday use
	srand(time(0));
	// ui setup
	Arxx::URI URI("data/data.arx");
	Arxx::Archive Archive;
	
	if(Archive.bLoad(URI) == false)
	{
		std::cerr << "Could not find or open " << URI << "." << std::endl;
		
		return 1;
	}
	g_MessageLabel = ReadLabel(GetItem(Archive, MESSAGE_LABEL));
	g_TimeWarpLabel = ReadLabel(GetItem(Archive, TIME_WARP_LABEL));
	g_SystemLabel = ReadLabel(GetItem(Archive, SYSTEM_LABEL));
	g_CreditsLabel = ReadLabel(GetItem(Archive, CREDITS_LABEL));
	g_FuelLabel = ReadLabel(GetItem(Archive, FUEL_LABEL));
	g_RadarWidget = ReadWidget(GetItem(Archive, RADAR_WIDGET));
	g_MiniMapWidget = ReadWidget(GetItem(Archive, MINI_MAP_WIDGET));
	g_TargetLabel = ReadLabel(GetItem(Archive, TARGET_LABEL));
	g_CurrentSystemLabel = ReadLabel(GetItem(Archive, CURRENT_SYSTEM_LABEL));
	
	// data reading
	// ARX
	ReadModel(&g_ModelManager, GetItem(Archive, CARGO_CUBE_MODEL));
	ReadModel(&g_ModelManager, GetItem(Archive, SHUTTLE_MODEL));
	ReadShipClass(&g_ShipClassManager, GetItem(Archive, SHUTTLE_SHIP_CLASS));
	ReadCommodity(&g_CommodityManager, GetItem(Archive, FOOD_COMMODITY));
	ReadCommodity(&g_CommodityManager, GetItem(Archive, INDUSTRIAL_COMMODITY));
	ReadCommodity(&g_CommodityManager, GetItem(Archive, MEDICAL_SUPPLIES_COMMODITY));
	ReadCommodity(&g_CommodityManager, GetItem(Archive, LUXURY_GOODS_COMMODITY));
	ReadCommodity(&g_CommodityManager, GetItem(Archive, METAL_COMMODITY));
	ReadCommodity(&g_CommodityManager, GetItem(Archive, EQUIPMENT_COMMODITY));
	ReadSystem(&g_SystemManager, GetItem(Archive, SOL_SYSTEM));
	ReadSystem(&g_SystemManager, GetItem(Archive, RIGEL_SYSTEM));
	ReadSystem(&g_SystemManager, GetItem(Archive, TICHEL_SYSTEM));
	ReadSystem(&g_SystemManager, GetItem(Archive, ALPHA_CENTAURI_SYSTEM));
	ReadSystemLink(&g_SystemManager, GetItem(Archive, SOL_ALPHA_CENTAURI_SYSTEM_LINK));
	ReadSystemLink(&g_SystemManager, GetItem(Archive, SOL_RIGEL_SYSTEM_LINK));
	ReadSystemLink(&g_SystemManager, GetItem(Archive, TICHEL_ALPHA_CENTAURI_SYSTEM_LINK));
	ReadSystemLink(&g_SystemManager, GetItem(Archive, TICHEL_RIGEL_SYSTEM_LINK));
	
	// initialize the player (initial load)
	LoadSavegame(LoadSavegameFileName);
	
	// setting up the player environment
	// TODO: read this from the savegame as well
	SetTimeWarp(1.0f);
	// setting the input focus
	g_InputFocus = g_PlayerShip;
	g_MiniMapCamera.SetPosition(0.0f, 0.0f, 1500.0f);
	g_MiniMapCamera.SetFocus(g_PlayerShip);
	// set first timeout for widget collector, it will reinsert itself on callback
	g_TimeoutNotifications.insert(std::make_pair(RealTime::GetTime() + 5.0f, new FunctionCallback0< void >(CollectWidgets)));
	// setting up the graphical environment
	CreateWindow();
	InitializeOpenGL();
	while(g_Quit == false)
	{
		ProcessEvents();
		Render();
		glXSwapBuffers(g_Display, g_Window);
	}
	DestroyWindow();
	
	return 0;
}

