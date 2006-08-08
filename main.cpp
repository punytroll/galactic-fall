#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include <iostream>
#include <list>
#include <sstream>
#include <vector>

#include <GL/glut.h>
#include <math3d/vector2f.h>
#include <math3d/vector4f.h>

#include "character.h"
#include "color.h"
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
#include "ship.h"
#include "ship_class.h"
#include "ship_class_manager.h"
#include "string_cast.h"
#include "system.h"
#include "system_manager.h"
#include "widget.h"

int g_LastMotionX(-1);
int g_LastMotionY(-1);
int g_MouseButton(-1);
math3d::vector4f g_CameraPosition(0.0f, 0.0f, 200.0f, 0.0f);
float g_GameTime;
ModelManager g_ModelManager;
ShipClassManager g_ShipClassManager(&g_ModelManager);
CommodityManager g_CommodityManager;
SystemManager g_SystemManager(&g_CommodityManager);
Character * g_PlayerCharacter(0);
Ship * g_PlayerShip(0);
Ship * g_InputFocus(0);
Position * g_CameraFocus(0);
float g_Width(0.0f);
float g_Height(0.0f);
Label * g_SystemLabel(0);
Label * g_TimeWarpLabel(0);
Label * g_PlanetLabel(0);
Label * g_CreditsLabel(0);
System * g_CurrentSystem;
System * g_SelectedLinkedSystem(0);
Planet * g_SelectedPlanet(0);
float g_TimeWarp(1.0f);
bool g_Pause(false);
PlanetDialog * g_PlanetDialog;
MapDialog * g_MapDialog;
int g_WidgetCollectorCountDownInitializer(10);
int g_WidgetCollectorCountDown(g_WidgetCollectorCountDownInitializer);

bool CanJump(Ship * Ship)
{
	// only let the ships jump if they are more than 280 clicks from system center
	return Ship->GetPosition().length_squared() > 78400.0f;
}

double GetTime(void)
{
	timeval TimeValue;
	
	gettimeofday(&TimeValue, 0);
	
	return TimeValue.tv_sec + 0.000001 * TimeValue.tv_usec;
}

float CalculateTime(void)
{
	static double LastNowSeconds(GetTime());
	double NowSeconds(GetTime());
	double DeltaSeconds(NowSeconds - LastNowSeconds);
	
	LastNowSeconds = NowSeconds;
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

void DisplayUserInterface(void)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, static_cast< GLfloat >(g_Width), 0.0, static_cast< GLfloat >(g_Height), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(0.0f, -g_Height, 0.0f);
	for(std::list< Widget * >::const_iterator WidgetIterator = Widget::GetTopLevelWidgets().begin(); WidgetIterator != Widget::GetTopLevelWidgets().end(); ++WidgetIterator)
	{
		glPushMatrix();
		glTranslatef((*WidgetIterator)->GetPosition().m_V.m_A[0], (*WidgetIterator)->GetPosition().m_V.m_A[1], 0.0f);
		(*WidgetIterator)->Draw();
		glPopMatrix();
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	if(--g_WidgetCollectorCountDown == 0)
	{
		std::list< Widget * > & DestroyedWidgets(Widget::GetDestroyedWidgets());
		
		while(DestroyedWidgets.size() > 0)
		{
			delete DestroyedWidgets.front();
			DestroyedWidgets.pop_front();
		}
		g_WidgetCollectorCountDown = g_WidgetCollectorCountDownInitializer;
	}
}

void Render(void)
{
	float Seconds(CalculateTime());
	
	if(g_CurrentSystem != 0)
	{
		const std::list< Ship * > Ships(g_CurrentSystem->GetShips());
		
		for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
		{
			(*ShipIterator)->Move(Seconds);
		}
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-g_CameraPosition.m_V.m_A[0], -g_CameraPosition.m_V.m_A[1], -g_CameraPosition.m_V.m_A[2]);
	glLightfv(GL_LIGHT0, GL_POSITION, math3d::vector4f(-50.0f, 50.0f, 100.0f, 0.0f).m_V.m_A);
	if(g_CameraFocus != 0)
	{
		glTranslatef(-g_CameraFocus->m_Position.m_V.m_A[0], -g_CameraFocus->m_Position.m_V.m_A[1], 0.0f);
	}
	if(g_CurrentSystem != 0)
	{
		const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
		const std::list< Ship * > &Ships(g_CurrentSystem->GetShips());
		
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
	}
	// user interface updates
	if((g_SelectedLinkedSystem != 0) && (CanJump(g_PlayerShip) == true))
	{
		g_SystemLabel->GetForegroundColor().Set(1.0f, 1.0f, 1.0f);
	}
	else
	{
		g_SystemLabel->GetForegroundColor().Set(0.5f, 0.5f, 0.5f);
	}
	DisplayUserInterface();
	glutSwapBuffers();
}

void Resize(int Width, int Height)
{
	if(Height == 0)
	{
		Height = 1;
	}
	g_Width = Width;
	g_Height = Height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, Width, Height);
	gluPerspective(45.0f, 1.0 * g_Width / g_Height, 1, 1000);
}

void SelectLinkedSystem(System * LinkedSystem)
{
	g_SelectedLinkedSystem = LinkedSystem;
	if(g_SelectedLinkedSystem != 0)
	{
		const std::set< System * > UnexploredSystems(g_PlayerCharacter->GetMapKnowledge()->GetUnexploredSystems());
		
		if(UnexploredSystems.find(g_SelectedLinkedSystem) == UnexploredSystems.end())
		{
			g_SystemLabel->SetString(LinkedSystem->GetName());
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
	g_SelectedPlanet = Planet;
	if(g_SelectedPlanet != 0)
	{
		g_PlanetLabel->SetString(Planet->GetName());
	}
	else
	{
		g_PlanetLabel->SetString("");
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
			g_CreditsLabel->SetString("Credits: " + to_string_cast(g_PlayerCharacter->GetCredits()));
		}
		else if(EventSource == g_MapDialog)
		{
			if(g_CurrentSystem->IsLinkedToSystem(g_MapDialog->GetSelectedSystem()) == true)
			{
				SelectLinkedSystem(g_MapDialog->GetSelectedSystem());
			}
			g_MapDialog = 0;
			g_Pause = false;
		}
	}
} g_GlobalDestroyListener;

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

void LeaveSystem(void)
{
	g_CurrentSystem->ClearShips();
	g_CurrentSystem = 0;
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
		g_PlayerShip->m_Velocity = Direction * g_PlayerShip->GetShipClass()->GetMaximumVelocity();
		
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
	else
	{
		g_PlayerShip->m_Position.set(0.0f, 0.0f);
		g_PlayerShip->m_Velocity.set(0.0f, 0.0f);
		g_PlayerShip->m_AngularPosition = 0.0f;
	}
	g_CurrentSystem->AddShip(g_PlayerShip);
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

void Mouse(int Button, int State, int X, int Y)
{
	for(std::list< Widget * >::const_iterator WidgetIterator = Widget::GetTopLevelWidgets().begin(); WidgetIterator != Widget::GetTopLevelWidgets().end(); ++WidgetIterator)
	{
		const math3d::vector2f & LeftTopCorner((*WidgetIterator)->GetPosition());
		math3d::vector2f RightBottomCorner(LeftTopCorner + (*WidgetIterator)->GetSize());
		
		if((X >= LeftTopCorner.m_V.m_A[0]) && (X < RightBottomCorner.m_V.m_A[0]) && (Y >= LeftTopCorner.m_V.m_A[1]) && (Y < RightBottomCorner.m_V.m_A[1]))
		{
			(*WidgetIterator)->MouseButton(Button, State, X - LeftTopCorner.m_V.m_A[0], Y - LeftTopCorner.m_V.m_A[1]);
			
			return;
		}
	}
	switch(Button)
	{
	case GLUT_WHEEL_UP:
		{
			g_CameraPosition.m_V.m_A[2] *= 0.95f;
			
			break;
		}
	case GLUT_WHEEL_DOWN:
		{
			g_CameraPosition.m_V.m_A[2] *= 1.05f;
			
			break;
		}
	case GLUT_MIDDLE_BUTTON:
		{
			if(State == GLUT_DOWN)
			{
				g_LastMotionX = X;
				g_LastMotionY = Y;
				g_MouseButton = Button;
			}
			else
			{
				g_MouseButton = -1;
			}
			
			break;
		}
	}
}

void MouseMotion(int X, int Y)
{
	int DeltaX(X - g_LastMotionX);
	int DeltaY(Y - g_LastMotionY);
	
	g_LastMotionX = X;
	g_LastMotionY = Y;
	if(g_MouseButton == GLUT_MIDDLE_BUTTON)
	{
		g_CameraPosition.m_V.m_A[0] -= static_cast< float >(DeltaX) * 0.0008f * g_CameraPosition.m_V.m_A[2];
		g_CameraPosition.m_V.m_A[1] += static_cast< float >(DeltaY) * 0.0008f * g_CameraPosition.m_V.m_A[2];
	}
}

void Key(unsigned char Key, int X, int Y)
{
	switch(Key)
	{
	case 27:
	case 'q':
		{
			glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
			exit(0);
		}
	case 'j':
		{
			if(g_SelectedLinkedSystem != 0)
			{
				if(CanJump(g_PlayerShip) == true)
				{
					System * OldSystem(g_CurrentSystem);
					System * NewSystem(g_SelectedLinkedSystem);
					
					LeaveSystem();
					EnterSystem(NewSystem, OldSystem);
				}
			}
			
			break;
		}
	case 'l':
		{
			if(g_PlanetDialog == 0)
			{
				if(g_SelectedPlanet != 0)
				{
					// test distance and speed (should be relative speed but planets have no speed, yet)
					if(((g_SelectedPlanet->GetPosition() - g_PlayerShip->GetPosition()).length_squared() <= g_SelectedPlanet->GetSize() * g_SelectedPlanet->GetSize()) && (g_PlayerShip->GetVelocity().length_squared() < 2.0f))
					{
						g_Pause = true;
						g_PlanetDialog = new PlanetDialog(0, g_SelectedPlanet);
						g_PlanetDialog->AddDestroyListener(&g_GlobalDestroyListener);
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
	case 'm':
		{
			if(g_MapDialog == 0)
			{
				g_Pause = true;
				g_MapDialog = new MapDialog(0, g_CurrentSystem);
				g_MapDialog->AddDestroyListener(&g_GlobalDestroyListener);
			}
			
			break;
		}
	case 'n':
		{
			const std::list< System * > & LinkedSystems(g_CurrentSystem->GetLinkedSystems());
			
			if(g_SelectedLinkedSystem != 0)
			{
				std::list< System * >::const_iterator SystemIterator(find(LinkedSystems.begin(), LinkedSystems.end(), g_SelectedLinkedSystem));
				
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
	case 'p':
		{
			const std::list< Planet * > & Planets(g_CurrentSystem->GetPlanets());
			
			if(g_SelectedPlanet != 0)
			{
				std::list< Planet * >::const_iterator PlanetIterator(find(Planets.begin(), Planets.end(), g_SelectedPlanet));
				
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
	case '+':
		{
			SetTimeWarp(g_TimeWarp * 1.1f);
			
			break;
		}
	case '-':
		{
			SetTimeWarp(g_TimeWarp * 0.9f);
			
			break;
		}
	}
}

void SpecialKey(int Key, int X, int Y)
{
	switch(Key)
	{
	case GLUT_KEY_LEFT:
		{
			g_InputFocus->m_TurnLeft = true;
			
			break;
		}
	case GLUT_KEY_RIGHT:
		{
			g_InputFocus->m_TurnRight = true;
			
			break;
		}
	case GLUT_KEY_UP:
		{
			g_InputFocus->m_Accelerate = true;
			
			break;
		}
	}
}

void SpecialKeyUp(int Key, int X, int Y)
{
	switch(Key)
	{
	case GLUT_KEY_LEFT:
		{
			g_InputFocus->m_TurnLeft = false;
			
			break;
		}
	case GLUT_KEY_RIGHT:
		{
			g_InputFocus->m_TurnRight = false;
			
			break;
		}
	case GLUT_KEY_UP:
		{
			g_InputFocus->m_Accelerate = false;
			
			break;
		}
	}
}

int main(int argc, char **argv)
{
	// ui setup
	g_TimeWarpLabel = new Label(0);
	g_TimeWarpLabel->SetForegroundColor(Color(1.0f, 1.0f, 0.8f));
	g_TimeWarpLabel->SetPosition(math3d::vector2f(0.0f, 00.0f));
	g_SystemLabel = new Label(0);
	g_SystemLabel->SetForegroundColor(Color(1.0f, 1.0f, 0.8f));
	g_SystemLabel->SetPosition(math3d::vector2f(0.0f, 20.0f));
	g_PlanetLabel = new Label(0);
	g_PlanetLabel->SetForegroundColor(Color(1.0f, 1.0f, 0.8f));
	g_PlanetLabel->SetPosition(math3d::vector2f(0.0f, 40.0f));
	g_CreditsLabel = new Label(0);
	g_CreditsLabel->SetForegroundColor(Color(1.0f, 1.0f, 0.8f));
	g_CreditsLabel->SetPosition(math3d::vector2f(0.0f, 60.0f));
	
	// data reading
	LoadModelsFromFile(&g_ModelManager, "data/shuttlecraft.xml");
	LoadShipClassesFromFile(&g_ShipClassManager, "data/shuttlecraft.xml");
	LoadCommoditiesFromFile(&g_CommodityManager, "data/universe.xml");
	LoadSystemsFromFile(&g_SystemManager, "data/universe.xml");
	LoadSystemLinksFromFile(&g_SystemManager, "data/universe.xml");
	
	ShipClass * ShuttleCraftShipClass(g_ShipClassManager.Get("shuttlecraft"));
	
	if(ShuttleCraftShipClass == 0)
	{
		std::cerr << "Loading the \"shuttlecraft\" ship-class failed." << std::endl;
		
		return 1;
	}
	g_PlayerCharacter = new Character();
	g_PlayerCharacter->AddCredits(1000.0f);
	g_PlayerShip = new Ship(ShuttleCraftShipClass);
	g_InputFocus = g_PlayerShip;
	g_CameraFocus = g_PlayerShip;
	EnterSystem(g_SystemManager.Get("sol"), 0);
	SetTimeWarp(1.0f);
	g_CreditsLabel->SetString("Credits: " + to_string_cast(g_PlayerCharacter->GetCredits()));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Escape Velocity");
	glutFullScreen();
	glutDisplayFunc(Render);
	glutReshapeFunc(Resize);
	glutIdleFunc(Render);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_DIFFUSE, math3d::vector4f(1.0f, 1.0f, 1.0f, 0.0f).m_V.m_A);
	glutMainLoop();
}

