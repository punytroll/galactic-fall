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
#include "model.h"
#include "model_manager.h"
#include "planet.h"
#include "planet_dialog.h"
#include "real_time.h"
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
Camera g_RadarCamera;
Camera g_MiniMapCamera;
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
std::multimap< double, Callback0< void > * > g_TimeoutNotifications;
std::list< Mind * > g_ActiveMinds;
std::list< Mind * > g_SuspendedMinds;
Widget * g_ScannerWidget(0);
Widget * g_MiniMapWidget(0);
Display * g_Display;
GLXContext g_GLXContext;
Window g_Window;

enum WantReturnCode
{
	OK,
	NOT_ENOUGH_CREDITS,
	NOT_ENOUGH_FUEL,
	TOO_FAR_AWAY,
	TOO_FAST,
	TOO_NEAR_TO_SYSTEM_CENTER
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
	
	return OK;
}

int WantToLand(Ship * Ship, Planet * Planet)
{
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
	if(g_PlayerCharacter->GetCredits() < Planet->GetLandingFee())
	{
		return NOT_ENOUGH_CREDITS;
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

void CalculatePerspectiveMatrix(float FieldOfView, float Aspect, float NearClippingPlane, float FarClippingPlane, math3d::matrix4f & Matrix)
{
	float Right, Top;
	
	Top = NearClippingPlane * tan(FieldOfView);
	Right = Top * Aspect;
	
	Matrix.m_M[1].m_A[0] = Matrix.m_M[2].m_A[0] = Matrix.m_M[3].m_A[0] = Matrix.m_M[0].m_A[1] = Matrix.m_M[2].m_A[1] = Matrix.m_M[3].m_A[1] = Matrix.m_M[0].m_A[2] = Matrix.m_M[1].m_A[2] = Matrix.m_M[0].m_A[3] = Matrix.m_M[1].m_A[3] = Matrix.m_M[3].m_A[3] = 0.0f;
	Matrix.m_M[0].m_A[0] = NearClippingPlane / Right;
	Matrix.m_M[1].m_A[1] = NearClippingPlane / Top;
	Matrix.m_M[2].m_A[2] = -(FarClippingPlane + NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
	Matrix.m_M[3].m_A[2] = -(2.0f * FarClippingPlane * NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
	Matrix.m_M[2].m_A[3] = -1.0f;
}

void SetMainPerspective(void)
{
	static float CalculatedForFieldOfView(0.0f);
	static float CalculatedForHeight(0.0f);
	static float CalculatedForWidth(0.0f);
	static math3d::matrix4f Matrix;
	static bool Initialized(false);
	
	if((Initialized == false) || (CalculatedForFieldOfView != g_Camera.GetFieldOfView()) || (CalculatedForHeight != g_Height) || (CalculatedForWidth != g_Width))
	{
		/// TODO: Make this configurable
		/// TODO: Also consider that these values may want to change while in game.
		const float FieldOfView(g_Camera.GetFieldOfView());
		const float NearClippingPlane(1.0f);
		const float FarClippingPlane(1000.0f);
		
		CalculatePerspectiveMatrix(FieldOfView, g_Width / g_Height, NearClippingPlane, FarClippingPlane, Matrix);
		Initialized = true;
		CalculatedForFieldOfView = FieldOfView;
		CalculatedForHeight = g_Height;
		CalculatedForWidth = g_Width;
	}
	glLoadMatrixf(Matrix.matrix());
}

void SetRadarPerspective(float RadialSize)
{
	static float CalculatedForRadialSize(0.0f);
	static math3d::matrix4f Matrix;
	static bool Initialized(false);
	
	/// TODO: Make this configurable
	if((Initialized == false) || (RadialSize != CalculatedForRadialSize))
	{
		float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
		
		g_RadarCamera.SetFieldOfView(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)));
		CalculatePerspectiveMatrix(g_RadarCamera.GetFieldOfView(), 1.0f, 1.0f, 1000.0f, Matrix);
		Initialized = true;
		CalculatedForRadialSize = RadialSize;
	}
	glLoadMatrixf(Matrix.matrix());
}

void SetMiniMapPerspective(void)
{
	static float CalculatedForFieldOfView(0.0f);
	static math3d::matrix4f Matrix;
	static bool Initialized(false);
	
	if((Initialized == false) || (CalculatedForFieldOfView != g_MiniMapCamera.GetFieldOfView()))
	{
		/// TODO: Make this configurable
		/// TODO: Also consider that these values may want to change while in game.
		const float FieldOfView(g_MiniMapCamera.GetFieldOfView());
		const float NearClippingPlane(1.0f);
		const float FarClippingPlane(10000.0f);
		
		CalculatePerspectiveMatrix(FieldOfView, 1.0f, NearClippingPlane, FarClippingPlane, Matrix);
		Initialized = true;
		CalculatedForFieldOfView = FieldOfView;
	}
	glLoadMatrixf(Matrix.matrix());
}

class ScannerDisplayWidget : public Widget
{
public:
	ScannerDisplayWidget(void) :
		Widget()
	{
	}
	
	virtual void Draw(void) const
	{
		Widget::Draw();
		// radar
		if((g_InputFocus != 0) && (g_InputFocus->GetTarget() != 0))
		{
			float RadialSize(g_InputFocus->GetTarget()->GetRadialSize());
			
			glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			// TODO: 0.0f is not the real value
			glViewport(static_cast< GLint >(GetGlobalPosition().m_V.m_A[0]), static_cast< GLint >(0.0f), static_cast< GLint >(GetSize().m_V.m_A[0]), static_cast< GLint >(GetSize().m_V.m_A[1]));
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			SetRadarPerspective(RadialSize);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			g_RadarCamera.SetPosition(0.0f, 0.0f, 4.0f * RadialSize);
			g_RadarCamera.SetFocus(g_InputFocus->GetTarget());
			g_RadarCamera.Draw();
			if((g_CurrentSystem != 0) && (g_CurrentSystem->GetStar() != 0))
			{
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				glLightfv(GL_LIGHT0, GL_POSITION, math3d::vector4f(g_CurrentSystem->GetStar()->GetPosition().m_V.m_A[0], g_CurrentSystem->GetStar()->GetPosition().m_V.m_A[1], 100.0f, 0.0f).m_V.m_A);
			}
			glClear(GL_DEPTH_BUFFER_BIT);
			g_InputFocus->GetTarget()->Draw();
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glPopAttrib();
		}
	}
};

class MiniMapWidget : public Widget
{
public:
	MiniMapWidget(void) :
		Widget()
	{
	}
	
	virtual void Draw(void) const
	{
		Widget::Draw();
		// mini map
		if((g_InputFocus != 0) && (g_CurrentSystem != 0))
		{
			glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			// TODO: 0.0f is not the real value
			glViewport(static_cast< GLint >(GetGlobalPosition().m_V.m_A[0]), static_cast< GLint >(0.0f), static_cast< GLint >(GetSize().m_V.m_A[0]), static_cast< GLint >(220.0f));
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			SetMiniMapPerspective();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			g_MiniMapCamera.Draw();
			
			const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
			const std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
			const std::list< Cargo * > & Cargos(g_CurrentSystem->GetCargos());
			
			glBegin(GL_POINTS);
			glColor3f(0.8f, 0.8f, 0.8f);
			for(std::list< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
			{
				if(*PlanetIterator == g_InputFocus->GetTarget())
				{
					glColor3f(0.2f, 1.0f, 0.0f);
				}
				glVertex2f((*PlanetIterator)->GetPosition().m_V.m_A[0], (*PlanetIterator)->GetPosition().m_V.m_A[1]);
				if(*PlanetIterator == g_InputFocus->GetTarget())
				{
					glColor3f(0.8f, 0.8f, 0.8f);
				}
			}
			for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
			{
				if(*ShipIterator == g_InputFocus->GetTarget())
				{
					glColor3f(0.2f, 1.0f, 0.0f);
				}
				glVertex2f((*ShipIterator)->GetPosition().m_V.m_A[0], (*ShipIterator)->GetPosition().m_V.m_A[1]);
				if(*ShipIterator == g_InputFocus->GetTarget())
				{
					glColor3f(0.8f, 0.8f, 0.8f);
				}
			}
			for(std::list< Cargo * >::const_iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
			{
				if(*CargoIterator == g_InputFocus->GetTarget())
				{
					glColor3f(0.2f, 1.0f, 0.0f);
				}
				glVertex2f((*CargoIterator)->GetPosition().m_V.m_A[0], (*CargoIterator)->GetPosition().m_V.m_A[1]);
				if(*CargoIterator == g_InputFocus->GetTarget())
				{
					glColor3f(0.8f, 0.8f, 0.8f);
				}
			}
			glEnd();
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glPopAttrib();
		}
	}
};

void DeleteShipFromSystem(System * System, std::list< Ship * >::iterator ShipIterator)
{
	std::list< Mind * >::iterator MindIterator;
	
	MindIterator = std::find_if(g_ActiveMinds.begin(), g_ActiveMinds.end(), MindWithShip(*ShipIterator));
	if(MindIterator != g_ActiveMinds.end())
	{
		delete (*MindIterator)->GetCharacter();
		delete *MindIterator;
		g_ActiveMinds.erase(MindIterator);
	}
	MindIterator = std::find_if(g_SuspendedMinds.begin(), g_SuspendedMinds.end(), MindWithShip(*ShipIterator));
	if(MindIterator != g_SuspendedMinds.end())
	{
		delete (*MindIterator)->GetCharacter();
		delete *MindIterator;
		g_SuspendedMinds.erase(MindIterator);
	}
	if((g_InputFocus != 0) && (g_InputFocus->GetTarget() == *ShipIterator))
	{
		g_InputFocus->SetTarget(0);
	}
	if(*ShipIterator == g_Camera.GetFocus())
	{
		g_Camera.SetFocus(0);
	}
	delete *ShipIterator;
	System->GetShips().erase(ShipIterator);
}

void CalculateMinds(void)
{
	for(std::list< Mind * >::iterator MindIterator = g_ActiveMinds.begin(); MindIterator != g_ActiveMinds.end(); ++MindIterator)
	{
		(*MindIterator)->Update();
	}
}

void CalculateMovements(void)
{
	float Seconds(CalculateTime());
	
	if(g_CurrentSystem != 0)
	{
		std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
		const std::list< Cargo * > Cargos(g_CurrentSystem->GetCargos());
		std::list< Shot * > & Shots(g_CurrentSystem->GetShots());
		
		for(std::list< Ship * >::iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
		{
			(*ShipIterator)->Update(Seconds);
		}
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
					if((*ShotIterator)->GetShooter() != *ShipIterator)
					{
						if(((*ShotIterator)->GetPosition() - (*ShipIterator)->GetPosition()).length_squared() < ((*ShotIterator)->GetRadialSize() * (*ShotIterator)->GetRadialSize() + (*ShipIterator)->GetRadialSize() * (*ShipIterator)->GetRadialSize()))
						{
							(*ShipIterator)->SetHull((*ShipIterator)->GetHull() - (*ShotIterator)->GetDamage());
							if((*ShipIterator)->GetHull() <= 0.0f)
							{
								if(*ShipIterator == g_PlayerShip)
								{
									g_PlayerShip = 0;
								}
								if(*ShipIterator == g_InputFocus)
								{
									g_InputFocus = 0;
								}
								DeleteShipFromSystem(g_CurrentSystem, ShipIterator);
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

void UpdateUserInterface(void)
{
	// call all real time timeouts
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
	if(g_InputFocus != 0)
	{
		// display fuel
		g_FuelLabel->SetString("Fuel: " + to_string_cast(100.0f * g_InputFocus->GetFuel() / g_InputFocus->GetFuelCapacity(), 2) + "%");
		// display hull
		g_HullLabel->SetString("Hull: " + to_string_cast(g_InputFocus->GetHull(), 2));
		// display credits in every cycle
		g_CreditsLabel->SetString("Credits: " + to_string_cast(g_PlayerCharacter->GetCredits()));
		// set system label color according to jump status
		if((g_InputFocus->GetLinkedSystemTarget() != 0) && (WantToJump(g_InputFocus) == OK))
		{
			g_SystemLabel->GetForegroundColor().Set(0.7f, 0.8f, 1.0f);
		}
		else
		{
			g_SystemLabel->GetForegroundColor().Set(0.4f, 0.4f, 0.4f);
		}
	}
}

void Render(void)
{
	glViewport(0, 0, static_cast< GLsizei >(g_Width), static_cast< GLsizei >(g_Height));
	glMatrixMode(GL_PROJECTION);
	SetMainPerspective();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	if(g_CurrentSystem != 0)
	{
		const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
		const std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
		const std::list< Cargo * > & Cargos(g_CurrentSystem->GetCargos());
		const std::list< Shot * > & Shots(g_CurrentSystem->GetShots());
		
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
	if((g_InputFocus != 0) && (g_InputFocus->GetTarget() != 0))
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		DrawSelection(g_InputFocus->GetTarget(), g_InputFocus->GetTarget()->GetRadialSize());
		
		math3d::vector2f RelativePosition(g_InputFocus->GetTarget()->GetPosition() - g_InputFocus->GetPosition());
		
		RelativePosition.normalize();
		glPushMatrix();
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glTranslatef(g_InputFocus->GetPosition().m_V.m_A[0], g_InputFocus->GetPosition().m_V.m_A[1], 0.0f);
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
	g_UserInterface.GetRootWidget()->SetSize(math3d::vector2f(g_Width, g_Height));
	g_ScannerWidget->SetPosition(math3d::vector2f(0.0f, g_Height - 240.0f));
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

void GameFrame(void)
{
	CalculateMinds();
	CalculateMovements();
	UpdateUserInterface();
	Render();
}

void SelectPhysicalObject(PhysicalObject * Object)
{
	g_InputFocus->SetTarget(Object);
	if(g_InputFocus->GetTarget() != 0)
	{
		g_TargetLabel->SetString(Object->GetName());
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
			SelectPhysicalObject(0);
			g_InputFocus->m_Velocity.set(0.0f, 0.0f);
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
		g_CurrentSystem->ClearShots();
		g_CurrentSystem = 0;
	}
	while(g_ActiveMinds.begin() != g_ActiveMinds.end())
	{
		Character * Character(g_ActiveMinds.front()->GetCharacter());
		Ship * Ship(Character->GetShip());
		
		delete g_ActiveMinds.front();
		g_ActiveMinds.erase(g_ActiveMinds.begin());
		delete Character;
		delete Ship;
	}
	g_CurrentSystemLabel->SetString("");
	SelectLinkedSystem(0);
	SelectPhysicalObject(0);
	g_PlayerShip->SetCurrentSystem(0);
}

void PopulateSystem(void)
{
	int NumberOfShips(GetRandomInteger(3));
	
	std::cout << "Spawning " << NumberOfShips << " ships." << std::endl;
	for(int ShipNumber = 1; ShipNumber <= NumberOfShips; ++ShipNumber)
	{
		std::cout << "  Processing ship " << ShipNumber << std::endl;
		
		std::stringstream IdentifierStream;
		
		IdentifierStream << "::system(" << g_CurrentSystem->GetIdentifier() << ")::created_at(" << std::fixed << RealTime::GetTime() << "[" << ShipNumber << "])::";
		
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
		
		NewShip->SetObjectIdentifier(IdentifierStream.str() + "ship(" + NewShip->GetShipClass()->GetIdentifier() + ")");
		NewShip->SetPosition(math3d::vector2f(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f)));
		NewShip->SetFuel(NewShip->GetFuelCapacity());
		NewShip->SetCurrentSystem(g_CurrentSystem);
		if(ShipClassIdentifier == "fighter")
		{
			NewShip->m_Fire = true;
		}
		
		Character * NewCharacter(new Character());
		
		NewCharacter->SetObjectIdentifier(IdentifierStream.str() + "character(" + NewShip->GetShipClass()->GetIdentifier() + ")");
		NewCharacter->SetShip(NewShip);
		
		StateMachineMind * NewMind(new StateMachineMind());
		
		NewMind->SetObjectIdentifier(IdentifierStream.str() + "mind(state_machine)");
		NewMind->SetCharacter(NewCharacter);
		NewMind->GetStateMachine()->SetState(new SelectSteering(NewShip, NewMind->GetStateMachine()));
		g_ActiveMinds.push_back(NewMind);
		g_CurrentSystem->AddShip(NewShip);
	}
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
	g_PlayerShip->SetCurrentSystem(NewSystem);
	SelectLinkedSystem(0);
	SelectPhysicalObject(0);
	g_PlayerCharacter->GetMapKnowledge()->AddExploredSystem(g_CurrentSystem);
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
	case 27: // Key: R
		{
			if(g_InputFocus != 0)
			{
				const std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
				Ship * SelectedShip(dynamic_cast< Ship * >(g_InputFocus->GetTarget()));
				
				if(SelectedShip == 0)
				{
					if(Ships.size() > 0)
					{
						SelectPhysicalObject(Ships.front());
					}
				}
				else
				{
					std::list< Ship * >::const_iterator ShipIterator(find(Ships.begin(), Ships.end(), SelectedShip));
					
					if(ShipIterator == Ships.end())
					{
						SelectPhysicalObject(0);
					}
					else
					{
						++ShipIterator;
						if(ShipIterator == Ships.end())
						{
							SelectPhysicalObject(0);
						}
						else
						{
							SelectPhysicalObject(*ShipIterator);
						}
					}
				}
			}
			
			break;
		}
	case 28: // Key: T
		{
			if(g_InputFocus != 0)
			{
				const std::list< Cargo * > & Cargos(g_CurrentSystem->GetCargos());
				Cargo * SelectedCargo(dynamic_cast< Cargo * >(g_InputFocus->GetTarget()));
				
				if(SelectedCargo == 0)
				{
					if(Cargos.size() > 0)
					{
						SelectPhysicalObject(Cargos.front());
					}
				}
				else
				{
					std::list< Cargo * >::const_iterator CargoIterator(find(Cargos.begin(), Cargos.end(), SelectedCargo));
					
					if(CargoIterator == Cargos.end())
					{
						SelectPhysicalObject(0);
					}
					else
					{
						++CargoIterator;
						if(CargoIterator == Cargos.end())
						{
							SelectPhysicalObject(0);
						}
						else
						{
							SelectPhysicalObject(*CargoIterator);
						}
					}
				}
			}
			
			break;
		}
	case 33: // Key: P
		{
			if(g_InputFocus != 0)
			{
				const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
				Planet * SelectedPlanet(dynamic_cast< Planet * >(g_InputFocus->GetTarget()));
				
				if(SelectedPlanet == 0)
				{
					if(Planets.size() > 0)
					{
						SelectPhysicalObject(Planets.front());
					}
				}
				else
				{
					std::list< Planet * >::const_iterator PlanetIterator(find(Planets.begin(), Planets.end(), SelectedPlanet));
					
					if(PlanetIterator == Planets.end())
					{
						SelectPhysicalObject(0);
					}
					else
					{
						++PlanetIterator;
						if(PlanetIterator == Planets.end())
						{
							SelectPhysicalObject(0);
						}
						else
						{
							SelectPhysicalObject(*PlanetIterator);
						}
					}
				}
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
			if(g_InputFocus != 0)
			{
				Cargo * SelectedCargo(dynamic_cast< Cargo * >(g_InputFocus->GetTarget()));
				
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
							MinimumDistance = (MinimumCargo->GetPosition() - g_InputFocus->GetPosition()).length_squared();
						}
						else
						{
							float Distance(((*CargoIterator)->GetPosition() - g_InputFocus->GetPosition()).length_squared());
							
							if(Distance < MinimumDistance)
							{
								MinimumCargo = *CargoIterator;
								MinimumDistance = Distance;
							}
						}
					}
					if(MinimumCargo != 0)
					{
						SelectPhysicalObject(MinimumCargo);
					}
				}
				else
				{
					// test distance
					if((SelectedCargo->GetPosition() - g_InputFocus->GetPosition()).length_squared() <= 5.0f * SelectedCargo->GetRadialSize() * SelectedCargo->GetRadialSize())
					{
						// test speed
						if((g_InputFocus->GetVelocity() - SelectedCargo->GetVelocity()).length_squared() <= 2.0f)
						{
							g_InputFocus->AddCommodities(SelectedCargo->GetCommodity(), 1.0f);
							g_CurrentSystem->RemoveCargo(SelectedCargo);
							SelectPhysicalObject(0);
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
			}
			
			break;
		}
	case 44: // Key: J
		{
			if(g_InputFocus != 0)
			{
				if(g_InputFocus->GetLinkedSystemTarget() != 0)
				{
					switch(WantToJump(g_InputFocus))
					{
					case OK:
						{
							System * OldSystem(g_CurrentSystem);
							System * NewSystem(g_InputFocus->GetLinkedSystemTarget());
							
							LeaveSystem();
							g_InputFocus->SetFuel(g_InputFocus->GetFuel() - g_InputFocus->GetShipClass()->GetJumpFuel());
							EnterSystem(NewSystem, OldSystem);
							PopulateSystem();
							
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
				else
				{
					SetMessage("No system selected to jump to.");
				}
			}
			
			break;
		}
	case 46: // Key: L
		{
			if(g_InputFocus != 0)
			{
				Planet * SelectedPlanet(dynamic_cast< Planet * >(g_InputFocus->GetTarget()));
				
				if(SelectedPlanet != 0)
				{
					switch(WantToLand(g_InputFocus, SelectedPlanet))
					{
					case OK:
						{
							// TODO: change to current character
							g_PlayerCharacter->RemoveCredits(SelectedPlanet->GetLandingFee());
							g_Pause = true;
							g_PlanetDialog = new PlanetDialog(g_UserInterface.GetRootWidget(), SelectedPlanet);
							g_PlanetDialog->GrabKeyFocus();
							g_PlanetDialog->AddDestroyListener(&g_GlobalDestroyListener);
							
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
					case NOT_ENOUGH_CREDITS:
						{
							SetMessage("You don't have enough credits to pay the landing fee.");
							
							break;
						}
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
							MinimumDistance = (MinimumPlanet->GetPosition() - g_InputFocus->GetPosition()).length_squared();
						}
						else
						{
							float Distance(((*PlanetIterator)->GetPosition() - g_InputFocus->GetPosition()).length_squared());
							
							if(Distance < MinimumDistance)
							{
								MinimumPlanet = *PlanetIterator;
								MinimumDistance = Distance;
							}
						}
					}
					if(MinimumPlanet != 0)
					{
						SelectPhysicalObject(MinimumPlanet);
					}
				}
			}
			
			break;
		}
	case 54: // Key: C
		{
			if(g_InputFocus != 0)
			{
				SetMessage("Jettison cargo.");
				while(g_InputFocus->GetCommodities().size() > 0)
				{
					const Commodity * Commodity(g_InputFocus->GetCommodities().begin()->first);
					Cargo * NewCargo(new Cargo(g_ModelManager.Get("cargo_cube"), Commodity, g_InputFocus->GetVelocity() * 0.8f + math3d::vector2f(GetRandomFloat(-0.5f, 0.5f), GetRandomFloat(-0.5f, 0.5f)))); // TODO: memory leak
					
					NewCargo->SetObjectIdentifier("::cargo(" + Commodity->GetIdentifier() + ")::" + to_string_cast(reinterpret_cast< void * >(NewCargo)));
					g_InputFocus->RemoveCommodities(Commodity, 1.0f);
					NewCargo->SetPosition(g_InputFocus->GetPosition());
					g_CurrentSystem->AddCargo(NewCargo);
				}
			}
			
			break;
		}
	case 57: // Key: N
		{
			if(g_InputFocus != 0)
			{
				const std::list< System * > & LinkedSystems(g_CurrentSystem->GetLinkedSystems());
				
				if(g_InputFocus->GetLinkedSystemTarget() != 0)
				{
					std::list< System * >::const_iterator SystemIterator(find(LinkedSystems.begin(), LinkedSystems.end(), g_InputFocus->GetLinkedSystemTarget()));
					
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
			SetTimeWarp(g_TimeWarp / 1.1f);
			
			break;
		}
	case 65: // Key: SPACE
		{
			if(g_InputFocus != 0)
			{
				g_InputFocus->m_Fire = true;
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
			XML << element << "credits" << attribute << "value" << value << g_PlayerCharacter->GetCredits() << end;
			XML << element << "map-knowledge";
			
			const std::set< System * > & ExploredSystems(g_PlayerCharacter->GetMapKnowledge()->GetExploredSystems());
			
			for(std::set< System * >::const_iterator ExploredSystemIterator = ExploredSystems.begin(); ExploredSystemIterator != ExploredSystems.end(); ++ExploredSystemIterator)
			{
				XML << element << "explored-system" << attribute << "identifier" << value << (*ExploredSystemIterator)->GetIdentifier() << end;
			}
			XML << end; // map-knowledge
			XML << end; // character
			XML << element << "ship" << attribute << "class-identifier" << value << g_PlayerShip->GetShipClass()->GetIdentifier() << attribute << "object-identifier" << value << g_PlayerShip->GetObjectIdentifier();
			XML << element << "fuel" << attribute << "value" << value << g_PlayerShip->GetFuel() << end;
			XML << element << "hull" << attribute << "value" << value << g_PlayerShip->GetHull() << end;
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
			if(g_InputFocus != 0)
			{
				g_InputFocus->m_Accelerate = true;
			}
			
			break;
		}
	case 100: // Key: LEFT
		{
			if(g_InputFocus != 0)
			{
				g_InputFocus->m_TurnLeft = true;
			}
			
			break;
		}
	case 102: // Key: RIGHT
		{
			if(g_InputFocus != 0)
			{
				g_InputFocus->m_TurnRight = true;
			}
			
			break;
		}
	case 105: // Key: PAGE DOWN
		{
			std::list< Mind * >::iterator MindIterator;
			
			MindIterator = std::find_if(g_SuspendedMinds.begin(), g_SuspendedMinds.end(), MindWithShip(g_InputFocus));
			if(MindIterator != g_SuspendedMinds.end())
			{
				g_SuspendedMinds.erase(MindIterator);
				g_ActiveMinds.push_back(*MindIterator);
			}
			
			std::list< Ship * > & Ships(g_CurrentSystem->GetShips());
			std::list< Ship * >::const_iterator ShipIterator(find(Ships.begin(), Ships.end(), g_InputFocus));
			
			if((ShipIterator == Ships.end()) || (++ShipIterator == Ships.end()))
			{
				g_InputFocus = Ships.front();
				g_Camera.SetFocus(Ships.front());
			}
			else
			{
				g_InputFocus = *ShipIterator;
				g_Camera.SetFocus(*ShipIterator);
			}
			MindIterator = std::find_if(g_ActiveMinds.begin(), g_ActiveMinds.end(), MindWithShip(g_InputFocus));
			if(MindIterator != g_ActiveMinds.end())
			{
				g_ActiveMinds.erase(MindIterator);
				g_SuspendedMinds.push_back(*MindIterator);
			}
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
			if(g_InputFocus != 0)
			{
				g_InputFocus->m_Fire = false;
			}
			
			break;
		}
	case 98:  // Key: UP
		{
			if(g_InputFocus != 0)
			{
				g_InputFocus->m_Accelerate = false;
			}
			
			break;
		}
	case 100: // Key: LEFT
		{
			if(g_InputFocus != 0)
			{
				g_InputFocus->m_TurnLeft = false;
			}
			
			break;
		}
	case 102: // Key: RIGHT
		{
			if(g_InputFocus != 0)
			{
				g_InputFocus->m_TurnRight = false;
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
	XSetStandardProperties(g_Display, g_Window, "escapevelocity 0.2", "TITLE", None, NULL, 0, NULL);
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
				else if((*ShipChild)->GetName() == "hull")
				{
					g_PlayerShip->SetHull(from_string_cast< float >((*ShipChild)->GetAttribute("value")));
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
				else if((*ShipChild)->GetName() == "name")
				{
					g_PlayerShip->SetName((*ShipChild)->GetAttribute("value"));
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
				}
			}
		}
	}
	g_CurrentSystem = g_SystemManager.Get(System);
	g_CurrentSystemLabel->SetString(g_CurrentSystem->GetName());
	if(g_PlayerShip != 0)
	{
		g_PlayerCharacter->SetShip(g_PlayerShip);
		g_CurrentSystem->AddShip(g_PlayerShip);
		g_PlayerShip->SetCurrentSystem(g_CurrentSystem);
	}
	g_InputFocus = g_PlayerShip;
	if(g_InputFocus != 0)
	{
		SelectLinkedSystem(0);
		SelectPhysicalObject(0);
	}
	RealTime::Invalidate();
	PopulateSystem();
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
	std::vector< std::string > Arguments(argv, argv + argc);
	std::string LoadSavegameFileName("data/savegame_default.xml");
	
	for(std::vector< std::string >::size_type Index = 0; Index < Arguments.size(); ++Index)
	{
		if((Arguments[Index].substr(0, 6) == "--load") && (Arguments[Index].length() > 7))
		{
			LoadSavegameFileName = Arguments[Index].substr(7);
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
	g_HullLabel = ReadLabel(GetItem(Archive, HULL_LABEL));
	g_ScannerWidget = ReadWidget(GetItem(Archive, SCANNER_WIDGET));
	ReadWidget(GetItem(Archive, SCANNER_DISPLAY_WIDGET), new ScannerDisplayWidget());
	g_MiniMapWidget = ReadWidget(GetItem(Archive, MINI_MAP_WIDGET), new MiniMapWidget());
	g_TargetLabel = ReadLabel(GetItem(Archive, TARGET_LABEL));
	g_CurrentSystemLabel = ReadLabel(GetItem(Archive, CURRENT_SYSTEM_LABEL));
	
	// data reading
	// ARX
	ReadModel(&g_ModelManager, GetItem(Archive, CARGO_CUBE_MODEL));
	ReadModel(&g_ModelManager, GetItem(Archive, SHUTTLE_MODEL));
	ReadModel(&g_ModelManager, GetItem(Archive, TRANSPORTER_MODEL));
	ReadModel(&g_ModelManager, GetItem(Archive, PLANET_MODEL));
	ReadModel(&g_ModelManager, GetItem(Archive, FIGHTER_MODEL));
	ReadShipClass(&g_ShipClassManager, GetItem(Archive, SHUTTLE_SHIP_CLASS));
	ReadShipClass(&g_ShipClassManager, GetItem(Archive, TRANSPORTER_SHIP_CLASS));
	ReadShipClass(&g_ShipClassManager, GetItem(Archive, FIGHTER_SHIP_CLASS));
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
	if(LoadSavegame(LoadSavegameFileName) == false)
	{
		return 1;
	}
	
	// setting up the player environment
	g_MiniMapCamera.SetPosition(0.0f, 0.0f, 1500.0f);
	if(g_InputFocus != 0)
	{
		g_MiniMapCamera.SetFocus(g_InputFocus);
	}
	g_MiniMapCamera.SetFieldOfView(0.392699082f);
	// set first timeout for widget collector, it will reinsert itself on callback
	g_TimeoutNotifications.insert(std::make_pair(RealTime::GetTime() + 5.0f, new FunctionCallback0< void >(CollectWidgets)));
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

