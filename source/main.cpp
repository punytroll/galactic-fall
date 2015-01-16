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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <GL/glx.h>

#include <algebra/matrix4f.h>

#include <string_cast/string_cast.h>

#include <xml_puny_dom/xml_puny_dom.h>

#include <xml_stream/xml_stream.h>

#include "asset_class.h"
#include "battery.h"
#include "battery_class.h"
#include "character.h"
#include "class_manager.h"
#include "color.h"
#include "command_mind.h"
#include "commodity.h"
#include "commodity_class.h"
#include "draw_text.h"
#include "faction.h"
#include "file_handling.h"
#include "galaxy.h"
#include "game_time.h"
#include "generator.h"
#include "generator_class.h"
#include "globals.h"
#include "goals.h"
#include "graphics/camera.h"
#include "graphics/default_render_target.h"
#include "graphics/engine.h"
#include "graphics/gl.h"
#include "graphics/light.h"
#include "graphics/mesh_manager.h"
#include "graphics/model.h"
#include "graphics/model_manager.h"
#include "graphics/orthogonal_2d_projection.h"
#include "graphics/particle_system.h"
#include "graphics/perspective_projection.h"
#include "graphics/scene.h"
#include "graphics/shading_manager.h"
#include "graphics/style.h"
#include "graphics/system_node.h"
#include "graphics/texture.h"
#include "graphics/texture_manager.h"
#include "graphics/view.h"
#include "hangar.h"
#include "map_knowledge.h"
#include "math.h"
#include "message.h"
#include "message_dispatcher.h"
#include "messages.h"
#include "mind.h"
#include "object_aspect_accessory.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "object_factory.h"
#include "output_observer.h"
#include "planet.h"
#include "real_time.h"
#include "resource_reader.h"
#include "scenario.h"
#include "scenario_manager.h"
#include "settings.h"
#include "ship.h"
#include "ship_class.h"
#include "shot.h"
#include "slot.h"
#include "slot_class.h"
#include "star.h"
#include "state_machine.h"
#include "states.h"
#include "storage.h"
#include "system.h"
#include "system_statistics.h"
#include "timeout_notifications.h"
#include "ui/button.h"
#include "ui/heads_up_display.h"
#include "ui/key_event.h"
#include "ui/label.h"
#include "ui/main_menu_window.h"
#include "ui/map_dialog.h"
#include "ui/mouse_button_event.h"
#include "ui/mouse_move_event.h"
#include "ui/object_information_dialog.h"
#include "ui/outfit_ship_dialog.h"
#include "ui/star_map_display.h"
#include "ui/timing_dialog.h"
#include "ui/user_interface.h"
#include "ui/widget.h"
#include "visualization.h"
#include "visualization_prototype.h"
#include "visualizations.h"
#include "weapon.h"
#include "write_to_xml_stream.h"

// these objects are exported via globals.h
ClassManager< AssetClass > * g_AssetClassManager(nullptr);
ClassManager< BatteryClass > * g_BatteryClassManager(0);
ClassManager< CommodityClass > * g_CommodityClassManager(0);
Galaxy * g_Galaxy(0);
ClassManager< GeneratorClass > * g_GeneratorClassManager(0);
Graphics::Engine * g_GraphicsEngine(0);
Graphics::PerspectiveProjection * g_MainProjection(0);
Graphics::View * g_MainView(0);
Graphics::Orthogonal2DProjection * g_UIProjection(0);
Graphics::View * g_UIView(0);
std::vector< Graphics::View * > g_PrerenderedViews;
MessageDispatcher * g_MessageDispatcher(0);
ObjectFactory * g_ObjectFactory(0);
ScenarioManager * g_ScenarioManager(0);
ClassManager< ShipClass > * g_ShipClassManager(0);
ClassManager< SlotClass > * g_SlotClassManager(0);
SystemStatistics * g_SystemStatistics(0);
UI::UserInterface * g_UserInterface(0);

// global dialog pointers
UI::MainMenuWindow * g_MainMenuWindow(0);
UI::MapDialog * g_MapDialog(0);
UI::OutfitShipDialog * g_OutfitShipDialog(0);
UI::TimingDialog * g_TimingDialog(0);

Vector2f g_LastMotion(-1.0f, -1.0f);
UI::MouseButtonEvent::MouseButton g_MouseButton(UI::MouseButtonEvent::MouseButton::Unspecified);
Vector3f g_CameraPosition;
bool g_FirstPersonCameraMode(false);
Reference< CommandMind > g_InputMind;
OutputObserver * g_CharacterObserver;
float g_Width(0.0f);
float g_Height(0.0f);
System * g_CurrentSystem;
float g_TimeWarp(1.0f);
bool g_Quit(false);
bool g_Pause(false);
TimeoutNotificationManager * g_GameTimeTimeoutNotifications;
TimeoutNotification g_SpawnShipTimeoutNotification;
TimeoutNotificationManager * g_RealTimeTimeoutNotifications;
Display * g_Display;
GLXContext g_GLXContext;
Window g_Window;
Colormap g_ColorMap;
bool g_EchoEvents(false);
bool g_EchoResizes(false);
bool g_DumpEndReport(false);
bool g_TakeScreenShot(false);
ResourceReader * g_ResourceReader(0);
Settings * g_Settings(0);
void (*g_KeyboardLookupTable[128][2])(void);

namespace Graphics
{
	class UIRootNode : public Graphics::Node
	{
	public:
		virtual void Draw(Graphics::RenderContext * RenderContext) override
		{
			g_UserInterface->Draw();
		}
	};
}

std::vector< std::string > SplitString(const std::string & String, char Delimiter)
{
	std::vector< std::string > Result;
	size_t Begin(0);
	size_t End(String.find(Delimiter, Begin));
	
	while(End != std::string::npos)
	{
		Result.push_back(String.substr(Begin, End - Begin));
		Begin = End + 1;
		End = String.find(Delimiter, Begin);
	}
	
	return Result;
}

int WantToJump(Ship * Ship, System * TargetSystem)
{
	if(TargetSystem == 0)
	{
		return NO_JUMP_TARGET;
	}
	
	const System * CurrentSystem(dynamic_cast< System * >(Ship->GetContainer()));
	
	assert(CurrentSystem != 0);
	
	// only let the ship jump if it is not near any other stellar object
	const std::vector< Planet * > & Planets(CurrentSystem->GetPlanets());
	
	for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
	{
		if((Ship->GetAspectPosition()->GetPosition() - (*PlanetIterator)->GetAspectPosition()->GetPosition()).SquaredLength() < 4 * (*PlanetIterator)->GetSize() * (*PlanetIterator)->GetSize())
		{
			return TOO_NEAR_TO_STELLAR_OBJECT;
		}
	}
	// only let ships jump if they have enough fuel
	if(Ship->GetFuel() < Ship->GetFuelNeededToJump())
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
	if((Planet->GetAspectPosition()->GetPosition() - Ship->GetAspectPosition()->GetPosition()).SquaredLength() > Planet->GetSize() * Planet->GetSize())
	{
		return TOO_FAR_AWAY;
	}
	// test speed (should be relative speed but planets have no speed, yet)
	if(Ship->GetVelocity().SquaredLength() > 2.0f)
	{
		return TOO_FAST;
	}
	assert(Ship->GetAspectPhysical() !=0);
	// test credits
	if(Character->GetCredits() < (Planet->GetLandingFeePerSpace() * Ship->GetAspectPhysical()->GetSpaceRequirement()))
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
	assert(Commodity->GetAspectPhysical() != 0);
	assert(Commodity->GetAspectPosition() != 0);
	if((Commodity->GetAspectPosition()->GetPosition() - Ship->GetAspectPosition()->GetPosition()).SquaredLength() > 5.0f * Commodity->GetAspectPhysical()->GetRadialSize() * Commodity->GetAspectPhysical()->GetRadialSize())
	{
		return TOO_FAR_AWAY;
	}
	// test speed
	if((Ship->GetVelocity() - Commodity->GetVelocity()).SquaredLength() > 2.0f)
	{
		return TOO_HIGH_RELATIVE_VELOCITY;
	}
	// test storage availability
	if(Ship->GetCargoHold() == 0)
	{
		return NO_STORAGE;
	}
	// test available storage space
	if(Ship->GetCargoHold()->GetSpace() < Commodity->GetAspectPhysical()->GetSpaceRequirement())
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

void SetMessage(const std::string & Message)
{
	auto HeadsUpDisplay(dynamic_cast< UI::HeadsUpDisplay * >(g_UserInterface->GetWidget("/heads_up_display")));
	
	assert(HeadsUpDisplay != nullptr);
	HeadsUpDisplay->SetMessage(Message);
}

void DrawSelection(const Object * Object, float RadialSize, const Color & Color)
{
	static const float OuterFactor(0.9f);
	static const float InnerFactor(1.1f);
	float OuterSize(RadialSize / OuterFactor);
	float InnerSize(RadialSize / InnerFactor);
	
	GLPushMatrix();
	GLPushAttrib(GL_LIGHTING_BIT);
	GLDisable(GL_LIGHTING);
	GLTranslatef(Object->GetAspectPosition()->GetPosition()[0], Object->GetAspectPosition()->GetPosition()[1], 0.0f);
	GLColor4fv(Color.GetColor().GetPointer());
	GLBegin(GL_LINE_STRIP);
	GLVertex2f(-OuterSize, -InnerSize);
	GLVertex2f(-OuterSize, -OuterSize);
	GLVertex2f(-InnerSize, -OuterSize);
	GLEnd();
	GLBegin(GL_LINE_STRIP);
	GLVertex2f(-OuterSize, InnerSize);
	GLVertex2f(-OuterSize, OuterSize);
	GLVertex2f(-InnerSize, OuterSize);
	GLEnd();
	GLBegin(GL_LINE_STRIP);
	GLVertex2f(OuterSize, -InnerSize);
	GLVertex2f(OuterSize, -OuterSize);
	GLVertex2f(InnerSize, -OuterSize);
	GLEnd();
	GLBegin(GL_LINE_STRIP);
	GLVertex2f(OuterSize, InnerSize);
	GLVertex2f(OuterSize, OuterSize);
	GLVertex2f(InnerSize, OuterSize);
	GLEnd();
	GLPopAttrib();
	GLPopMatrix();
}

void CollectWidgets(void)
{
	std::list< UI::Widget * > & DestroyedWidgets(UI::Widget::GetDestroyedWidgets());
	
	for(std::list< UI::Widget * >::iterator DestroyedWidgetIterator = DestroyedWidgets.begin(); DestroyedWidgetIterator != DestroyedWidgets.end(); ++DestroyedWidgetIterator)
	{
		delete *DestroyedWidgetIterator;
	}
	DestroyedWidgets.clear();
}

void CollectWidgetsRecurrent(void)
{
	CollectWidgets();
	/// TODO: Make the 5.0f seconds timeout configurable via the game configuration archive.
	g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 5.0f, CollectWidgetsRecurrent);
}

void DeleteObject(Object * Object)
{
	Object->Destroy();
	delete Object;
}

void CalculateCharacters(void)
{
	for(auto Character : Character::GetCharacters())
	{
		Character->Update();
	}
}

Graphics::ParticleSystem * CreateParticleSystem(const std::string & ParticleSystemClassIdentifier)
{
	Graphics::ParticleSystem * ParticleSystem(new Graphics::ParticleSystem());
	
	if(ParticleSystemClassIdentifier == "hit")
	{
		for(int Index = 0; Index < 30; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			Vector2f Velocity(Vector2f::CreateFromMagnitudeAndAngle(GetRandomFloat(0.0f, 0.5f) + GetRandomFloatFromExponentialDistribution(1.0f), GetRandomFloat(0.0f, 2 * M_PI)));
			
			Particle._Position.Set(0.0f, 0.0f, 0.0f);
			Particle._Velocity.Set(Velocity[0], Velocity[1], 0.0f);
			Particle._TimeOfDeath = GameTime::Get() + GetRandomDouble(0.3f, 0.8f);
			Particle._Color.Set(GetRandomFloat(0.4f, 0.5f), GetRandomFloat(0.35f, 0.45f), GetRandomFloat(0.35f, 0.65f), 0.3f);
			Particle._Size = GetRandomFloat(0.25f, 0.4f);
			ParticleSystem->AddParticle(Particle);
		}
		ParticleSystem->AddSystemScriptLine("kill-empty");
		ParticleSystem->AddSystemScriptLine("move");
		ParticleSystem->AddSystemScriptLine("update-particles");
		ParticleSystem->AddParticleScriptLine("kill-old");
		ParticleSystem->AddParticleScriptLine("move");
	}
	else if(ParticleSystemClassIdentifier == "jump")
	{
		// add sparks which fly around fast
		for(int Index = 0; Index < 100; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			Vector2f Position(Vector2f::CreateFromMagnitudeAndAngle(GetRandomFloat(1.0f, 4.0f) + GetRandomFloatFromExponentialDistribution(2.0f), GetRandomFloat(0.0f, 2 * M_PI)));
			Vector2f Velocity(Vector2f::CreateFromMagnitudeAndAngle(GetRandomFloat(15.0f, 20.0f), GetRandomFloat(0.0f, 2 * M_PI)));
			
			Particle._Position.Set(Position[0], Position[1], 0.0f);
			Particle._Velocity.Set(Velocity[0], Velocity[1], 0.0f);
			Particle._TimeOfDeath = GameTime::Get() + GetRandomDouble(0.8f, 1.2f);
			Particle._Color.Set(GetRandomFloat(0.5f, 0.65f), GetRandomFloat(0.6f, 0.65f), 1.0f, 0.5f);
			Particle._Size = GetRandomFloat(0.5f, 0.9f);
			ParticleSystem->AddParticle(Particle);
		}
		// add green cloud which turns counter-clockwise
		for(int Index = 0; Index < 50; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			Vector2f Position(Vector2f::CreateFromMagnitudeAndAngle(GetRandomFloat(0.0f, 8.0f) + GetRandomFloatFromExponentialDistribution(5.0f), GetRandomFloat(0.0f, 2 * M_PI)));
			
			Particle._Position.Set(Position[0], Position[1], 0.0f);
			Particle._Velocity.Set(Position[1], -Position[0], 0.0f);
			Particle._TimeOfDeath = GameTime::Get() + GetRandomDouble(1.5f, 2.5f);
			Particle._Color.Set(GetRandomFloat(0.4f, 0.5f), GetRandomFloat(0.85f, 1.0f), GetRandomFloat(0.75f, 0.95f), 0.05f);
			Particle._Size = GetRandomFloat(10.0f, 12.0f);
			ParticleSystem->AddParticle(Particle);
		}
		
		// add flash
		Graphics::ParticleSystem::Particle Particle;
		
		Particle._Position.Set(0.0f, 0.0f, 0.0f);
		Particle._Velocity.Set(0.0f, 0.0f, 0.0f);
		Particle._TimeOfDeath = GameTime::Get() + GetRandomDouble(0.1f, 0.15f);
		Particle._Color.Set(0.4f, GetRandomFloat(0.4f, 1.0f), GetRandomFloat(0.95f, 1.0f), 0.3f);
		Particle._Size = 200.0f;
		ParticleSystem->AddParticle(Particle);
		ParticleSystem->AddSystemScriptLine("kill-empty");
		ParticleSystem->AddSystemScriptLine("move");
		ParticleSystem->AddSystemScriptLine("update-particles");
		ParticleSystem->AddParticleScriptLine("kill-old");
		ParticleSystem->AddParticleScriptLine("move");
	}
	else if(ParticleSystemClassIdentifier == "explosion")
	{
		// center explosion
		for(int Index = 0; Index < 100; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			float Speed(GetRandomFloat(0.0f, 2.5f) + GetRandomFloatFromExponentialDistribution(2.0f));
			Vector2f Velocity(Vector2f::CreateFromMagnitudeAndAngle(Speed, GetRandomFloat(0.0f, 2 * M_PI)));
			
			Particle._Velocity.Set(Velocity[0], Velocity[1], 0.0f);
			Particle._Position.Set(0.0f, 0.0f, 0.0f);
			Particle._TimeOfDeath = GameTime::Get() + GetRandomDouble(1.0f, 2.5f);
			Particle._Color.Set(GetRandomFloat(0.4f, 0.8f), GetRandomFloat(0.2f, 0.4f), GetRandomFloat(0.05f, 0.15f), 0.5f);
			Particle._Size = 5.0f / abs(Speed);
			ParticleSystem->AddParticle(Particle);
		}
		
		float CloudAngle(GetRandomFloat(0.0f, 2 * M_PI));
		
		// off-center explosion
		for(int Index = 0; Index < 40; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			float Speed(GetRandomFloat(0.0f, 3.0f) + GetRandomFloatFromExponentialDistribution(2.0f));
			Vector2f Velocity(Vector2f::CreateFromMagnitudeAndAngle(Speed, CloudAngle + GetRandomFloat(-0.1f, 0.1f)));
			float Distance(GetRandomFloat(0.0f, 1.0f) + GetRandomFloatFromExponentialDistribution(1.0f));
			Vector2f Position(Vector2f::CreateFromMagnitudeAndAngle(Distance, CloudAngle + GetRandomFloat(-0.3f, 0.3f)));
			
			Particle._Velocity.Set(Velocity[0], Velocity[1], 0.0f);
			Particle._Position.Set(Position[0], Position[1], 0.0f);
			Particle._TimeOfDeath = GameTime::Get() + GetRandomDouble(1.0f, 2.0f);
			Particle._Color.Set(GetRandomFloat(0.6f, 0.9f), GetRandomFloat(0.3f, 0.5f), GetRandomFloat(0.15f, 0.2f), 0.5f);
			Particle._Size = 5.0f / abs(abs(Speed) - 3.0f);
			ParticleSystem->AddParticle(Particle);
		}
		// exhalting explosion cloud
		CloudAngle = GetRandomFloat(0.0f, 2 * M_PI);
		for(int Index = 0; Index < 40; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			float Speed(GetRandomFloat(2.0f, 10.0f) + GetRandomFloatFromExponentialDistribution(2.0f));
			Vector2f Velocity(Vector2f::CreateFromMagnitudeAndAngle(Speed, CloudAngle + GetRandomFloat(-0.1f, 0.1f)));
			
			Particle._Velocity.Set(Velocity[0], Velocity[1], 0.0f);
			Particle._Position.Set(0.0f, 0.0f, 0.0f);
			Particle._TimeOfDeath = GameTime::Get() + GetRandomDouble(0.8f, 1.8f);
			Particle._Color.Set(GetRandomFloat(0.6f, 0.8f), GetRandomFloat(0.3f, 0.4f), GetRandomFloat(0.3f, 0.3f), 0.5f);
			Particle._Size = 3.0f / abs(abs(Speed) - 6.0f);
			ParticleSystem->AddParticle(Particle);
		}
		// flash
		for(int Index = 0; Index < 5; ++Index)
		{
			Graphics::ParticleSystem::Particle Particle;
			Vector2f Position(Vector2f::CreateFromMagnitudeAndAngle(GetRandomFloat(0.0f, 6.0f), GetRandomFloat(0.0f, 2 * M_PI)));
			
			Particle._Position.Set(Position[0], Position[1], 0.0f);
			Particle._Velocity.Set(0.0f, 0.0f, 0.0f);
			Particle._TimeOfDeath = GameTime::Get() + GetRandomDouble(0.1f, 0.22f);
			Particle._Color.Set(0.9f, GetRandomFloat(0.9f, 1.0f), GetRandomFloat(0.95f, 1.0f), 0.15f);
			Particle._Size = 20.0f;
			ParticleSystem->AddParticle(Particle);
		}
		
		Graphics::ParticleSystem::Particle Particle;
		
		Particle._Position.Set(0.0f, 0.0f, 0.0f);
		Particle._Velocity.Set(0.0f, 0.0f, 0.0f);
		Particle._TimeOfDeath = GameTime::Get() + GetRandomDouble(0.1f, 0.15f);
		Particle._Color.Set(0.9f, GetRandomFloat(0.9f, 1.0f), GetRandomFloat(0.95f, 1.0f), 0.3f);
		Particle._Size = 200.0f;
		ParticleSystem->AddParticle(Particle);
		ParticleSystem->AddSystemScriptLine("kill-empty");
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
	assert(g_GraphicsEngine != 0);
	g_GraphicsEngine->AddParticleSystem(ParticleSystem);
	
	return ParticleSystem;
}

void UpdateVisualizations(Galaxy * Galaxy)
{
	std::deque< Object * > ToDo;
	
	ToDo.push_back(Galaxy);
	while(ToDo.empty() == false)
	{
		auto TheObject(ToDo.back());
		
		ToDo.pop_back();
		if(TheObject->GetAspectObjectContainer() != nullptr)
		{
			ToDo.insert(ToDo.end(), TheObject->GetAspectObjectContainer()->GetContent().begin(), TheObject->GetAspectObjectContainer()->GetContent().end());
		}
		if(TheObject->GetAspectVisualization() != nullptr)
		{
			TheObject->GetAspectVisualization()->UpdateVisualizations();
		}
	}
}

void EmptySystem(System * System)
{
	assert(System != nullptr);
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

void EmptyUnwatchedSystems(Galaxy * Galaxy)
{
	assert(Galaxy != nullptr);
	assert(Galaxy->GetAspectObjectContainer() != nullptr);
	for(auto Object : Galaxy->GetAspectObjectContainer()->GetContent())
	{
		if((Object->GetTypeIdentifier() == "system") && (Object != g_CurrentSystem))
		{
			EmptySystem(dynamic_cast< System * >(Object));
		}
	}
}

void UpdateObjects(Galaxy * Galaxy, float Seconds)
{
	assert(Galaxy != nullptr);
	assert(Galaxy->GetAspectObjectContainer() != nullptr);
	
	std::deque< Object * > ToDo;
	
	ToDo.push_back(Galaxy);
	while(ToDo.empty() == false)
	{
		auto TheObject(ToDo.back());
		
		ToDo.pop_back();
		assert(TheObject != nullptr);
		if(TheObject->GetTypeIdentifier() == "ship")
		{
			g_SystemStatistics->SetShipsInCurrentSystemThisFrame(g_SystemStatistics->GetShipsInCurrentSystemThisFrame() + 1);
		}
		else if(TheObject->GetTypeIdentifier() == "commodity")
		{
			g_SystemStatistics->SetCommoditiesInCurrentSystemThisFrame(g_SystemStatistics->GetCommoditiesInCurrentSystemThisFrame() + 1);
		}
		else if(TheObject->GetTypeIdentifier() == "shot")
		{
			g_SystemStatistics->SetShotsInCurrentSystemThisFrame(g_SystemStatistics->GetShotsInCurrentSystemThisFrame() + 1);
		}
		if(TheObject->GetAspectUpdate() != nullptr)
		{
			if(TheObject->GetAspectUpdate()->Update(Seconds) == false)
			{
				DeleteObject(TheObject);
				TheObject = nullptr;
			}
		}
		if((TheObject != nullptr) && (TheObject->GetAspectObjectContainer() != nullptr))
		{
			ToDo.insert(ToDo.end(), TheObject->GetAspectObjectContainer()->GetContent().begin(), TheObject->GetAspectObjectContainer()->GetContent().end());
		}
	}
}

void CollisionDetection(System * System)
{
	std::list< Shot * > Shots(System->GetShots().begin(), System->GetShots().end());
	
	for(auto Shot : Shots)
	{
		assert(Shot != nullptr);
		// test for collisions with ships
		for(auto Ship : System->GetShips())
		{
			assert(Ship != nullptr);
			if((Shot->GetShooter().IsValid() == true) && (Shot->GetShooter().Get() != Ship))
			{
				assert(Ship->GetAspectPhysical() != nullptr);
				assert(Ship->GetAspectPosition() != nullptr);
				assert(Shot->GetAspectPhysical() != nullptr);
				assert(Shot->GetAspectPosition() != nullptr);
				if((Shot->GetAspectPosition()->GetPosition() - Ship->GetAspectPosition()->GetPosition()).SquaredLength() < (Shot->GetAspectPhysical()->GetRadialSize() * Shot->GetAspectPhysical()->GetRadialSize() + Ship->GetAspectPhysical()->GetRadialSize() * Ship->GetAspectPhysical()->GetRadialSize()))
				{
					auto * NewHitParticleSystem(CreateParticleSystem("hit"));
					
					NewHitParticleSystem->SetPosition(Shot->GetAspectPosition()->GetPosition());
					NewHitParticleSystem->SetVelocity((Shot->GetVelocity() * 0.2f) + (Ship->GetVelocity() * 0.8f));
					VisualizeParticleSystem(NewHitParticleSystem, System);
					Ship->SetHull(Ship->GetHull() - Shot->GetDamage());
					assert(Ship->GetAspectObjectContainer() != nullptr);
					for(auto Content : Ship->GetAspectObjectContainer()->GetContent())
					{
						if(Content->GetTypeIdentifier() == "character")
						{
							g_MessageDispatcher->PushMessage(new ThreatMessage(Shot->GetShooter(), Content->GetReference(), Shot->GetDamage()));
						}
					}
					if(Ship->GetHull() <= 0.0f)
					{
						auto NewExplosionParticleSystem(CreateParticleSystem("explosion"));
						
						NewExplosionParticleSystem->SetPosition(Ship->GetAspectPosition()->GetPosition());
						NewExplosionParticleSystem->SetVelocity(Ship->GetVelocity() * 0.5f);
						VisualizeParticleSystem(NewExplosionParticleSystem, System);
						// if the ship has content, drop all of it
						assert(Ship->GetAspectObjectContainer() != nullptr);
						
						const auto & ShipContent(Ship->GetAspectObjectContainer()->GetContent());
						auto ShipContentIterator(ShipContent.begin());
						
						while(ShipContentIterator != ShipContent.end())
						{
							if((*ShipContentIterator)->GetTypeIdentifier() == "storage")
							{
								auto TheStorage(*ShipContentIterator);
								const auto & StorageContent(TheStorage->GetAspectObjectContainer()->GetContent());
								auto StorageContentIterator(StorageContent.begin());
								
								while(StorageContentIterator != StorageContent.end())
								{
									if((*StorageContentIterator)->GetTypeIdentifier() == "commodity")
									{
										auto TheCommodity(dynamic_cast< Commodity * >(*StorageContentIterator));
										
										++StorageContentIterator;
										TheStorage->GetAspectObjectContainer()->RemoveContent(TheCommodity);
										TheCommodity->GetAspectPosition()->SetPosition(Ship->GetAspectPosition()->GetPosition());
										
										Vector2f Velocity(Vector2f::CreateFromMagnitudeAndAngle(GetRandomFloat(0.0f, 1.2f), GetRandomFloat(0.0f, 2 * M_PI)));
										
										TheCommodity->SetVelocity(Vector3f(Ship->GetVelocity()[0] * 0.8f + Velocity[0], Ship->GetVelocity()[1] * 0.8 + Velocity[1], 0.0f));
										
										Vector3f RotationAxis(GetRandomFloat(-1.0f, 1.0f), GetRandomFloat(-1.0f, 1.0f), GetRandomFloat(-1.0f, 1.0f));
										
										RotationAxis.Normalize();
										TheCommodity->SetAngularVelocity(AxisAngle(RotationAxis[0], RotationAxis[1], RotationAxis[2], GetRandomFloat(0.0f, 0.7f)));
										Ship->GetContainer()->GetAspectObjectContainer()->AddContent(TheCommodity);
									}
									else
									{
										++StorageContentIterator;
									}
								}
							}
							++ShipContentIterator;
						}
						DeleteObject(Ship);
					}
					DeleteObject(Shot);
					Shot = nullptr;
					
					break;
				}
			}
		}
		if(Shot != nullptr)
		{
			for(auto Commodity : System->GetCommodities())
			{
				assert(Commodity != nullptr);
				assert(Commodity->GetAspectPhysical() != nullptr);
				assert(Commodity->GetAspectPosition() != nullptr);
				assert(Shot->GetAspectPhysical() != nullptr);
				assert(Shot->GetAspectPosition() != nullptr);
				if((Shot->GetAspectPosition()->GetPosition() - Commodity->GetAspectPosition()->GetPosition()).SquaredLength() < (Shot->GetAspectPhysical()->GetRadialSize() * Shot->GetAspectPhysical()->GetRadialSize() + Commodity->GetAspectPhysical()->GetRadialSize() * Commodity->GetAspectPhysical()->GetRadialSize()))
				{
					auto NewHitParticleSystem(CreateParticleSystem("hit"));
					
					NewHitParticleSystem->SetPosition(Shot->GetAspectPosition()->GetPosition());
					NewHitParticleSystem->SetVelocity((Shot->GetVelocity() * 0.4f) + (Commodity->GetVelocity() * 0.6f));
					VisualizeParticleSystem(NewHitParticleSystem, System);
					Commodity->SetHull(Commodity->GetHull() - Shot->GetDamage());
					if(Commodity->GetHull() <= 0.0f)
					{
						auto NewHitParticleSystem(CreateParticleSystem("hit"));
						
						NewHitParticleSystem->SetPosition(Commodity->GetAspectPosition()->GetPosition());
						NewHitParticleSystem->SetVelocity(Commodity->GetVelocity() * 0.5f);
						VisualizeParticleSystem(NewHitParticleSystem, System);
						DeleteObject(Commodity);
					}
					DeleteObject(Shot);
					Shot = nullptr;
					
					break;
				}
			}
		}
	}
}

void UpdateMainViewCamera(void)
{
	auto SpacialMatrix(Matrix4f::CreateIdentity());

	if((g_CharacterObserver != nullptr) && (g_CharacterObserver->GetObservedCharacter().IsValid() == true) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr))
	{
		auto Focus(g_CharacterObserver->GetObservedCharacter()->GetShip());
		
		assert(Focus->GetAspectPosition() != nullptr);
		SpacialMatrix.Translate(Focus->GetAspectPosition()->GetPosition()[0], Focus->GetAspectPosition()->GetPosition()[1], 0.0f);
		if(g_FirstPersonCameraMode == true)
		{
			SpacialMatrix.RotateZ(-M_PI / 2.0f);
			SpacialMatrix.Rotate(Focus->GetAspectPosition()->GetOrientation());
		}
	}
	SpacialMatrix.Translate(g_CameraPosition);
	assert(g_MainView != 0);
	assert(g_MainView->GetCamera() != 0);
	g_MainView->GetCamera()->SetSpacialMatrix(SpacialMatrix);
}

void DisplayMainView(void)
{
	g_MainView->Render();
	// HUD
	if((g_CharacterObserver->GetObservedCharacter().IsValid() == true) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != 0) && (g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget() != nullptr))
	{
		Color Color(1.0f, 0.0f, 0.0f, 1.0f);
		
		GLClear(GL_DEPTH_BUFFER_BIT);
		assert(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget()->GetAspectPhysical() != 0);
		DrawSelection(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget(), g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget()->GetAspectPhysical()->GetRadialSize(), Color);
		
		assert(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget()->GetAspectPosition() != 0);
		Vector3f RelativePosition(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget()->GetAspectPosition()->GetPosition() - g_CharacterObserver->GetObservedCharacter()->GetShip()->GetAspectPosition()->GetPosition());
		
		RelativePosition.Normalize();
		GLPushMatrix();
		GLPushAttrib(GL_LIGHTING_BIT);
		GLDisable(GL_LIGHTING);
		GLTranslatef(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetAspectPosition()->GetPosition()[0], g_CharacterObserver->GetObservedCharacter()->GetShip()->GetAspectPosition()->GetPosition()[1], 0.0f);
		GLRotatef(GetRadians(Vector2f(RelativePosition[0], RelativePosition[1])) * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
		GLColor3f(0.0f, 0.5f, 0.5f);
		GLBegin(GL_LINES);
		GLVertex2f(20.0f, 0.0f);
		GLVertex2f(30.0f, 0.0f);
		GLVertex2f(26.0f, 4.0f);
		GLVertex2f(30.0f, 0.0f);
		GLVertex2f(26.0f, -4.0f);
		GLVertex2f(30.0f, 0.0f);
		GLEnd();
		GLPopAttrib();
		GLPopMatrix();
	}
	// debug HUD
	if((g_InputMind.IsValid() == false) && (g_CharacterObserver->GetObservedCharacter().IsValid() == true) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != 0))
	{
		Color Color(0.0f, 0.0f, 1.0f, 1.0f);
		
		GLClear(GL_DEPTH_BUFFER_BIT);
		assert(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetAspectPhysical() != 0);
		DrawSelection(g_CharacterObserver->GetObservedCharacter()->GetShip(), g_CharacterObserver->GetObservedCharacter()->GetShip()->GetAspectPhysical()->GetRadialSize(), Color);
	}
}

void OnMainSceneNodeDestroy(Graphics::Node * Node)
{
	InvalidateVisualizationReference(Node);
	delete Node;
}

void OnUISceneNodeDestroy(Graphics::Node * Node)
{
	delete Node;
}

void Resize(void)
{
	if(g_EchoResizes == true)
	{
		std::cout << "Resize:         width = " << g_Width << " height = " << g_Height << std::endl;
	}
	if(g_Height == 0)
	{
		g_Height = 1;
	}
	assert(g_MainView != 0);
	
	Graphics::DefaultRenderTarget * MainViewRenderTarget(dynamic_cast< Graphics::DefaultRenderTarget * >(g_MainView->GetRenderTarget()));
	
	assert(MainViewRenderTarget != 0);
	MainViewRenderTarget->SetSize(g_Width, g_Height);
	assert(g_MainProjection != 0);
	g_MainProjection->SetAspect(g_Width / g_Height);
	assert(g_UIProjection != 0);
	g_UIProjection->SetRight(g_Width);
	g_UIProjection->SetBottom(g_Height);
	assert(g_UserInterface != 0);
	assert(g_UserInterface->GetRootWidget() != 0);
	g_UserInterface->GetRootWidget()->SetSize(Vector2f(g_Width, g_Height));
}

void OnMainMenuDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		g_MainMenuWindow = 0;
		g_Pause = false;
	}
}

void OnMapDialogDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if((g_InputMind.IsValid() == true) && (g_InputMind->GetCharacter() != 0) && (g_InputMind->GetCharacter()->GetShip() != 0))
		{
			const System * CurrentSystem(dynamic_cast< System * >(g_InputMind->GetCharacter()->GetShip()->GetContainer()));
			
			assert(CurrentSystem != 0);
			if(CurrentSystem->IsLinkedToSystem(g_MapDialog->GetSelectedSystem()) == true)
			{
				g_InputMind->SelectLinkedSystem(g_MapDialog->GetSelectedSystem());
			}
		}
		g_MapDialog = 0;
	}
}

void OnOutfitShipDialogDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		g_OutfitShipDialog = 0;
	}
}

void OnTimingDialogDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		g_TimingDialog = 0;
	}
}

void SpawnShip(System * System, const std::string & IdentifierSuffix, std::string ShipClassIdentifier = "")
{
	if(ShipClassIdentifier == "")
	{
		double RandomUniform(GetRandomUniform());
		
		if(RandomUniform < 0.2f)
		{
			ShipClassIdentifier = "shuttle";
		}
		else if(RandomUniform < 0.5f)
		{
			ShipClassIdentifier = "transporter";
		}
		else
		{
			ShipClassIdentifier = "fighter";
		}
	}
	
	auto NewShip(dynamic_cast< Ship * >(g_ObjectFactory->Create("ship", ShipClassIdentifier, true)));
	
	NewShip->SetObjectIdentifier("::ship(" + NewShip->GetClassIdentifier() + ")" + IdentifierSuffix);
	
	float Random(GetRandomUniform());
	Faction * Faction(nullptr);
	
	if(Random < 0.2f)
	{
		Faction = g_Galaxy->GetFactions().find("pirates")->second;
	}
	else if(Random < 0.8f)
	{
		Faction = g_Galaxy->GetFactions().find("earth")->second;
	}
	else
	{
		Faction = g_Galaxy->GetFactions().find("neutral")->second;
	}
	NewShip->SetFaction(Faction->GetReference());
	
	auto & PartStyles(NewShip->GetAspectVisualization()->GetVisualizationPrototype()->GetPartStyles());
	
	if(PartStyles.find("faction") != PartStyles.end())
	{
		PartStyles["faction"]->SetDiffuseColor(Faction->GetColor());
	}
	NewShip->GetAspectPosition()->SetPosition(Vector3f(GetRandomFloat(-200.0f, 200.0f), GetRandomFloat(-200.0f, 200.0f), 0.0f));
	NewShip->GetAspectPosition()->SetOrientation(Quaternion::CreateAsRotationZ(GetRandomFloat(0.0f, 2.0f * M_PI)));
	
	Vector2f Velocity(Vector2f::CreateFromMagnitudeAndAngle(GetRandomFloat(0.0f, NewShip->GetMaximumSpeed()), GetRandomFloat(0.0f, 2.0f * M_PI)));
	
	NewShip->SetVelocity(Vector3f(Velocity[0], Velocity[1], 0.0f));
	NewShip->SetFuel(NewShip->GetFuelCapacity());
	
	Object * NewBattery(g_ObjectFactory->Create("battery", "light_battery", true));
	
	NewBattery->SetObjectIdentifier("::battery(" + NewBattery->GetClassIdentifier() + ")::created_for(" + NewShip->GetObjectIdentifier() + ")" + IdentifierSuffix);
	NewShip->GetAspectObjectContainer()->AddContent(NewBattery);
	NewShip->GetAspectOutfitting()->GetSlot("battery")->Mount(NewBattery);
	
	Character * NewCharacter(dynamic_cast< Character * >(g_ObjectFactory->Create("character", "", true)));
	
	NewCharacter->SetObjectIdentifier("::character(" + NewShip->GetClassIdentifier() + ")" + IdentifierSuffix);
	NewCharacter->GetMapKnowledge()->AddExploredSystem(System);
	if(ShipClassIdentifier == "fighter")
	{
		NewCharacter->SetCredits(200 + GetRandomU4Byte(50, 250));
		
		Object * NewWeapon(g_ObjectFactory->Create("weapon", "light_laser", true));
		
		NewWeapon->SetObjectIdentifier("::weapon(" + NewWeapon->GetClassIdentifier() + ")::created_for(" + NewShip->GetObjectIdentifier() + ")" + IdentifierSuffix);
		NewShip->GetAspectObjectContainer()->AddContent(NewWeapon);
		NewShip->GetAspectOutfitting()->GetSlot("front_gun")->Mount(NewWeapon);
	}
	else if((ShipClassIdentifier == "transporter") || (ShipClassIdentifier == "shuttle"))
	{
		NewCharacter->SetCredits(GetRandomU4Byte(500, 2500));
		for(int NumberOfAssetClasses = GetRandomIntegerFromExponentialDistribution(2); NumberOfAssetClasses > 0; --NumberOfAssetClasses)
		{
			const std::map< std::string, AssetClass * > & AssetClasses(g_AssetClassManager->GetClasses());
			std::map< std::string, AssetClass * >::const_iterator AssetClassIterator(AssetClasses.begin());
			
			for(std::map< std::string, AssetClass * >::size_type Choice = GetRandomInteger(AssetClasses.size() - 1); Choice > 0; --Choice)
			{
				++AssetClassIterator;
			}
			
			int AmountOfAssets(GetRandomIntegerFromExponentialDistribution(NewShip->GetCargoHold()->GetSpaceCapacity() / g_ObjectFactory->GetSpaceRequirement(AssetClassIterator->second->GetObjectTypeIdentifier(), AssetClassIterator->second->GetObjectClassIdentifier())));
			
			while((AmountOfAssets > 0) && (NewShip->GetCargoHold()->GetSpace() >= g_ObjectFactory->GetSpaceRequirement(AssetClassIterator->second->GetObjectTypeIdentifier(), AssetClassIterator->second->GetObjectClassIdentifier())))
			{
				Object * NewCommodity(g_ObjectFactory->Create(AssetClassIterator->second->GetObjectTypeIdentifier(), AssetClassIterator->second->GetObjectClassIdentifier(), true));
				
				NewCommodity->SetObjectIdentifier("::" + AssetClassIterator->second->GetObjectTypeIdentifier() + "(" + AssetClassIterator->second->GetIdentifier() + ")::(" + to_string_cast(NumberOfAssetClasses) + "|" + to_string_cast(AmountOfAssets) + ")" + IdentifierSuffix);
				NewShip->GetCargoHold()->GetAspectObjectContainer()->AddContent(NewCommodity);
				--AmountOfAssets;
			}
		}
	}
	if(GetRandomBoolean(0.2) == true)
	{
		NewShip->SetHull(GetRandomFloat(0.1f * NewShip->GetHullCapacity(), 0.9f * NewShip->GetHullCapacity()));
	}
	else
	{
		NewShip->SetHull(NewShip->GetHullCapacity());
	}
	NewShip->SetFuel(GetRandomFloat(0.1f * NewShip->GetFuelCapacity(), 0.8f * NewShip->GetFuelCapacity()));
	if((ShipClassIdentifier == "transporter") || (ShipClassIdentifier == "shuttle"))
	{
		StateMachineMind * NewMind(new StateMachineMind());
		
		NewMind->SetObjectIdentifier("::mind(state_machine)" + IdentifierSuffix);
		NewMind->SetCharacter(NewCharacter);
		NewMind->GetStateMachine()->SetState(new SelectSteering(NewMind));
		NewMind->GetStateMachine()->SetGlobalState(new MonitorFuel(NewMind));
		NewCharacter->GetAspectObjectContainer()->AddContent(NewMind);
	}
	else
	{
		GoalMind * NewMind(new GoalMind());
		
		NewMind->SetObjectIdentifier("::mind(goal)" + IdentifierSuffix);
		NewMind->SetCharacter(NewCharacter);
		NewMind->GetAspectObjectContainer()->AddContent(new GoalFighterThink(NewMind));
		NewCharacter->GetAspectObjectContainer()->AddContent(NewMind);
	}
	NewShip->GetAspectObjectContainer()->AddContent(NewCharacter);
	System->GetAspectObjectContainer()->AddContent(NewShip);
}

void SpawnShipOnTimeout(System * SpawnInSystem)
{
	std::stringstream IdentifierSuffix;
	
	IdentifierSuffix << "::created_at_game_time(" << std::fixed << GameTime::Get() << ")::in_system(" << SpawnInSystem->GetClassIdentifier() << ")";
	
	SpawnShip(SpawnInSystem, IdentifierSuffix.str());
	g_SpawnShipTimeoutNotification = g_GameTimeTimeoutNotifications->Add(GameTime::Get() + GetRandomFloatFromExponentialDistribution(1.0f / SpawnInSystem->GetTrafficDensity()), std::bind(SpawnShipOnTimeout, SpawnInSystem));
}

void PopulateSystem(System * System)
{
	int NumberOfShips(GetRandomInteger(5));
	
	for(int ShipNumber = 1; ShipNumber <= NumberOfShips; ++ShipNumber)
	{
		std::stringstream IdentifierSuffix;
		
		IdentifierSuffix << "::created_at_game_time(" << std::fixed << GameTime::Get() << "[" << ShipNumber << "])::in_system(" << System->GetClassIdentifier() << ")";
		
		SpawnShip(System, IdentifierSuffix.str());
	}
}

System * GetObservedSystem(void)
{
	System * Result(nullptr);
	
	if((g_CharacterObserver->GetObservedCharacter().IsValid() == true) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr))
	{
		Result = g_CharacterObserver->GetObservedCharacter()->GetShip()->GetSystem();
	}
	if(Result == nullptr)
	{
		Result = g_CurrentSystem;
	}
	
	return Result;
}

void OnOutputEnterSystem(System * EnterSystem)
{
	assert(g_SpawnShipTimeoutNotification.IsValid() == false);
	assert(g_MainView != nullptr);
	assert(g_MainView->GetScene() == nullptr);
	
	// build the static setup of the scene
	auto MainScene(new Graphics::Scene());
	
	MainScene->SetDestroyCallback(OnMainSceneNodeDestroy);
	
	auto Star(EnterSystem->GetStar());
	
	if(Star != nullptr)
	{
		MainScene->ActivateLight();
		assert(MainScene->GetLight() != nullptr);
		MainScene->GetLight()->SetType(Graphics::Light::Type::Directional);
		MainScene->GetLight()->SetColor(Star->GetColor());
		assert(Star->GetAspectPosition() != nullptr);
		MainScene->GetLight()->SetDirection(Vector3f(-Star->GetAspectPosition()->GetPosition()[0], -Star->GetAspectPosition()->GetPosition()[1], -100.0f));
	}
	else
	{
		MainScene->DeactivateLight();
	}
	g_MainView->SetScene(MainScene);
	
	auto RootNode(new Graphics::Node());
	
	RootNode->SetClearColorBuffer(true);
	RootNode->SetClearDepthBuffer(true);
	MainScene->SetRootNode(RootNode);
	
	auto Visualization(VisualizeObject(EnterSystem, RootNode));
	
	Visualization->SetUpdateOrientation(false);
	Visualization->SetUpdatePosition(false);
	assert(g_UIView != nullptr);
	assert(g_UIView->GetScene() != nullptr);
	assert(g_UIView->GetScene()->GetRootNode() != nullptr);
	g_UIView->GetScene()->GetRootNode()->SetClearColorBuffer(false);
	g_SpawnShipTimeoutNotification = g_GameTimeTimeoutNotifications->Add(GameTime::Get() + GetRandomFloatFromExponentialDistribution(1.0f / EnterSystem->GetTrafficDensity()), std::bind(SpawnShipOnTimeout, EnterSystem));
	
	auto HeadsUpDisplay(new UI::HeadsUpDisplay(g_UserInterface->GetRootWidget()));
	
	HeadsUpDisplay->SetName("heads_up_display");
	HeadsUpDisplay->SetPosition(Vector2f(0.0f, 0.0f));
	HeadsUpDisplay->SetSize(Vector2f(g_Width, g_Height));
	HeadsUpDisplay->SetAnchorRight(true);
	HeadsUpDisplay->SetAnchorBottom(true);
}

void OnOutputLeaveSystem(System * System)
{
	assert(System != nullptr);
	
	auto HeadsUpDisplay(g_UserInterface->GetWidget("/heads_up_display"));
	
	HeadsUpDisplay->Destroy();
	HeadsUpDisplay = nullptr;
	if(g_SpawnShipTimeoutNotification.IsValid() == true)
	{
		g_SpawnShipTimeoutNotification.Dismiss();
	}
	
	auto MainScene(g_MainView->GetScene());
	
	g_MainView->SetScene(nullptr);
	delete MainScene;
	assert(g_UIView != nullptr);
	assert(g_UIView->GetScene() != nullptr);
	assert(g_UIView->GetScene()->GetRootNode() != nullptr);
	g_UIView->GetScene()->GetRootNode()->SetClearColorBuffer(true);
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
	GLPushAttrib(GL_PIXEL_MODE_BIT);
	GLReadBuffer(GL_FRONT);
	// now read the bits
	GLReadPixels(0, 0, static_cast< GLsizei >(g_Width), static_cast< GLsizei >(g_Height), GL_RGB, GL_UNSIGNED_BYTE, ScreenshotData);
	// revert the read buffer
	GLPopAttrib();
	
	// the file name with the current datetime in the format YYYYMMDD-HHMMSS
	std::string FileName(MakeTimeStampedFileName("screenshot", "tex"));
	std::ofstream OutputFileStream(FileName.c_str());
	
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
		SetMessage("Screenshot written to file \"" + FileName + "\".");
	}
	delete[] ScreenshotData;
}

void PrerenderViews(void)
{
	for(auto PrerenderedView : g_PrerenderedViews)
	{
		PrerenderedView->Render();
	}
}

void GameFrame(void)
{
	g_SystemStatistics->NextFrame();
	RealTime::Invalidate();
	
	static double FrameTimeBegin(RealTime::Get());
	double FrameTimeEnd(RealTime::Get());
	double FrameTimeDelta(FrameTimeEnd - FrameTimeBegin);
	
	// now set the next begin time to the current end time
	FrameTimeBegin = FrameTimeEnd;
	
	double FrameProcessingTimeBegin(RealTime::Get());
	
	g_GameTimeTimeoutNotifications->Process(GameTime::Get());
	g_RealTimeTimeoutNotifications->Process(RealTime::Get());
	RealTime::Invalidate();
	
	double MessagingTimeBegin(RealTime::Get());
	
	g_MessageDispatcher->DispatchMessages();
	RealTime::Invalidate();
	
	double MessagingTimeEnd(RealTime::Get());
	double MessagingTimeDelta(MessagingTimeEnd - MessagingTimeBegin);
	double AITimeBegin(RealTime::Get());
	
	CalculateCharacters();
	RealTime::Invalidate();
	
	double AITimeEnd(RealTime::Get());
	double AITimeDelta(AITimeEnd - AITimeBegin);
	double PhysicsTimeBegin(RealTime::Get());
	float Seconds(CalculateTime());
	
	if(g_Galaxy != nullptr)
	{
		auto OldObservedSystem(GetObservedSystem());
		
		UpdateObjects(g_Galaxy, Seconds);
		if(OldObservedSystem != nullptr)
		{
			CollisionDetection(OldObservedSystem);
		}
		
		auto NewObservedSystem(GetObservedSystem());
		
		if(OldObservedSystem != NewObservedSystem)
		{
			OnOutputLeaveSystem(OldObservedSystem);
			g_CurrentSystem = NewObservedSystem;
			assert(g_CurrentSystem != nullptr);
			OnOutputEnterSystem(NewObservedSystem);
			PopulateSystem(NewObservedSystem);
		}
		EmptyUnwatchedSystems(g_Galaxy);
		UpdateVisualizations(g_Galaxy);
	}
	UpdateMainViewCamera();
	RealTime::Invalidate();
	
	double PhysicsTimeEnd(RealTime::Get());
	double PhysicsTimeDelta(PhysicsTimeEnd - PhysicsTimeBegin);
	
	g_UserInterface->Update(FrameTimeDelta, Seconds);
	RealTime::Invalidate();
	
	double GraphicsTimeBegin(RealTime::Get());
	
	assert(g_GraphicsEngine != nullptr);
	g_GraphicsEngine->Update(Seconds);
	PrerenderViews();
	DisplayMainView();
	g_UIView->Render();
	RealTime::Invalidate();
	
	double GraphicsTimeEnd(RealTime::Get());
	double GraphicsTimeDelta(GraphicsTimeEnd - GraphicsTimeBegin);
	
	if(g_TakeScreenShot == true)
	{
		TakeScreenShot();
		g_TakeScreenShot = false;
	}
	RealTime::Invalidate();
	
	double FrameProcessingTimeEnd(RealTime::Get());
	double FrameProcessingTimeDelta(FrameProcessingTimeEnd - FrameProcessingTimeBegin);
	
	g_SystemStatistics->SetAISecondsThisFrame(AITimeDelta);
	g_SystemStatistics->SetGraphicsSecondsThisFrame(GraphicsTimeDelta);
	g_SystemStatistics->SetFrameToFrameSecondsThisFrame(FrameTimeDelta);
	g_SystemStatistics->SetMessagingSecondsThisFrame(MessagingTimeDelta);
	g_SystemStatistics->SetPhysicsSecondsThisFrame(PhysicsTimeDelta);
	g_SystemStatistics->SetProcessingSecondsThisFrame(FrameProcessingTimeDelta);
}

void PurgeGame(void)
{
	if(g_CurrentSystem != nullptr)
	{
		OnOutputLeaveSystem(g_CurrentSystem);
		g_CurrentSystem = nullptr;
	}
	if(g_Galaxy != nullptr)
	{
		g_Galaxy->Destroy();
		delete g_Galaxy;
		g_Galaxy = nullptr;
	}
}

void SetupInitialGalaxyState(void)
{
	assert(g_Galaxy == nullptr);
	// initialize the galaxy object with hard coded properties
	g_Galaxy = new Galaxy();
	g_Galaxy->SetTypeIdentifier("galaxy");
	g_Galaxy->SetClassIdentifier("milky_way");
	g_Galaxy->SetObjectIdentifier("::galaxy");
	g_Galaxy->GetAspectName()->SetName("Milky Way");
	// read the factions from the data archive
	g_ResourceReader->ReadFactions();
	// read the systems from the data archive
	g_ResourceReader->ReadSystems(g_AssetClassManager);
	// read the system links from the data archive
	g_ResourceReader->ReadSystemLinks();
}

void LoadGameFromElement(const Element * SaveElement)
{
	if(SaveElement->GetName() != "save")
	{
		throw std::runtime_error("The savegame is not rooted at a \"save\" element.");
	}
	
	std::string CurrentSystem;
	std::string InputMindObjectIdentifier;
	std::string ObservedCharacterObjectIdentifier;
	
	// setup the game world
	PurgeGame();
	SetupInitialGalaxyState();
	for(auto SaveChild : SaveElement->GetChilds())
	{
		if(SaveChild->GetName() == "game-time")
		{
			GameTime::Set(from_string_cast< double >(SaveChild->GetAttribute("value")));
		}
		else if(SaveChild->GetName() == "current-system")
		{
			CurrentSystem = SaveChild->GetAttribute("identifier");
		}
		else if(SaveChild->GetName() == "time-warp")
		{
			g_TimeWarp = from_string_cast< float >(SaveChild->GetAttribute("value"));
		}
		else if(SaveChild->GetName() == "input-mind")
		{
			InputMindObjectIdentifier = SaveChild->GetAttribute("object-identifier");
		}
		else if(SaveChild->GetName() == "observed-character")
		{
			ObservedCharacterObjectIdentifier = SaveChild->GetAttribute("object-identifier");
		}
		else if(SaveChild->GetName() == "main-camera")
		{
			for(auto CameraChild : SaveChild->GetChilds())
			{
				if(CameraChild->GetName() == "position")
				{
					g_CameraPosition.Set(from_string_cast< float >(CameraChild->GetAttribute("x")), from_string_cast< float >(CameraChild->GetAttribute("y")), from_string_cast< float >(CameraChild->GetAttribute("z")));
				}
				else if(CameraChild->GetName() == "field-of-view-y")
				{
					if(CameraChild->HasAttribute("radians") == true)
					{
						assert(g_MainProjection != nullptr);
						g_MainProjection->SetFieldOfViewY(from_string_cast< float >(CameraChild->GetAttribute("radians")));
					}
				}
				else
				{
					throw std::runtime_error("The \"main-camera\" element contains an unidentified child element \"" + CameraChild->GetName() + "\".");
				}
			}
		}
		else if(SaveChild->GetName() == "object")
		{
			assert(SaveChild->HasAttribute("object-identifier") == true);
			
			Object * NewObject(Object::GetObject(SaveChild->GetAttribute("object-identifier")));
			
			if(NewObject == nullptr)
			{
				assert(SaveChild->HasAttribute("type-identifier") == true);
				assert(SaveChild->HasAttribute("class-identifier") == true);
				NewObject = g_ObjectFactory->Create(SaveChild->GetAttribute("type-identifier"), SaveChild->GetAttribute("class-identifier"), false);
				NewObject->SetObjectIdentifier(SaveChild->GetAttribute("object-identifier"));
			}
			else
			{
				assert(SaveChild->HasAttribute("type-identifier") == true);
				assert(SaveChild->GetAttribute("type-identifier") == NewObject->GetTypeIdentifier());
				assert(SaveChild->HasAttribute("class-identifier") == true);
				assert(SaveChild->GetAttribute("class-identifier") == NewObject->GetClassIdentifier());
			}
			for(auto ObjectChild : SaveChild->GetChilds())
			{
				if(ObjectChild->GetName() == "aspect-accessory")
				{
					// assert that the object supports the name aspect
					assert(NewObject->GetAspectAccessory() != nullptr);
					// read data related to the name aspect
					for(auto AspectChild : ObjectChild->GetChilds())
					{
						if(AspectChild->GetName() == "slot")
						{
							assert(AspectChild->HasAttribute("slot-identifier") == true);
							// read in third pass
						}
						else
						{
							throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + AspectChild->GetName() + "\".");
						}
					}
				}
				else if(ObjectChild->GetName() == "aspect-name")
				{
					// assert that the object supports the name aspect
					assert(NewObject->GetAspectName() != nullptr);
					// read data related to the name aspect
					for(auto AspectChild : ObjectChild->GetChilds())
					{
						if(AspectChild->GetName() == "name")
						{
							assert(AspectChild->HasAttribute("value") == true);
							NewObject->GetAspectName()->SetName(AspectChild->GetAttribute("value"));
						}
						else
						{
							throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + AspectChild->GetName() + "\".");
						}
					}
				}
				else if(ObjectChild->GetName() == "aspect-object-container")
				{
					// assert that the object supports the object container aspect
					assert(NewObject->GetAspectObjectContainer() != nullptr);
					// read data related to the object container aspect
					for(auto AspectChild : ObjectChild->GetChilds())
					{
						if(AspectChild->GetName() == "content")
						{
							assert(AspectChild->HasAttribute("object-identifier") == true);
							// read in second pass
						}
						else
						{
							throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + AspectChild->GetName() + "\".");
						}
					}
				}
				else if(ObjectChild->GetName() == "aspect-physical")
				{
					// assert that the object supports the physical aspect
					assert(NewObject->GetAspectPhysical() != nullptr);
					// read data related to the physical aspect
					for(auto AspectChild : ObjectChild->GetChilds())
					{
						if(AspectChild->GetName() == "radial-size")
						{
							assert(AspectChild->HasAttribute("value") == true);
							NewObject->GetAspectPhysical()->SetRadialSize(from_string_cast< float >(AspectChild->GetAttribute("value")));
						}
						else if(AspectChild->GetName() == "space-requirement")
						{
							assert(AspectChild->HasAttribute("value") == true);
							NewObject->GetAspectPhysical()->SetSpaceRequirement(from_string_cast< float >(AspectChild->GetAttribute("value")));
						}
						else
						{
							throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + AspectChild->GetName() + "\".");
						}
					}
				}
				else if(ObjectChild->GetName() == "aspect-position")
				{
					// assert that the object supports the position aspect
					assert(NewObject->GetAspectPosition() != nullptr);
					// read data related to the position aspect
					for(auto AspectChild : ObjectChild->GetChilds())
					{
						if(AspectChild->GetName() == "orientation")
						{
							assert(AspectChild->HasAttribute("w") == true);
							assert(AspectChild->HasAttribute("x") == true);
							assert(AspectChild->HasAttribute("y") == true);
							assert(AspectChild->HasAttribute("z") == true);
							NewObject->GetAspectPosition()->SetOrientation(Quaternion(from_string_cast< float >(AspectChild->GetAttribute("w")), from_string_cast< float >(AspectChild->GetAttribute("x")), from_string_cast< float >(AspectChild->GetAttribute("y")), from_string_cast< float >(AspectChild->GetAttribute("z"))));
						}
						else if(AspectChild->GetName() == "position")
						{
							assert(AspectChild->HasAttribute("x") == true);
							assert(AspectChild->HasAttribute("y") == true);
							assert(AspectChild->HasAttribute("z") == true);
							NewObject->GetAspectPosition()->SetPosition(Vector3f(from_string_cast< float >(AspectChild->GetAttribute("x")), from_string_cast< float >(AspectChild->GetAttribute("y")), from_string_cast< float >(AspectChild->GetAttribute("z"))));
						}
						else
						{
							throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + AspectChild->GetName() + "\".");
						}
					}
				}
				else if(ObjectChild->GetName() == "aspect-visualization")
				{
					// assert that the object supports the visualization aspect
					assert(NewObject->GetAspectVisualization() != nullptr);
					// read data related to the visualization aspect
					for(auto AspectChild : ObjectChild->GetChilds())
					{
						if(AspectChild->GetName() == "part")
						{
							assert(AspectChild->HasAttribute("identifier") == true);
							
							auto NewPartStyle(new Graphics::Style());
							
							for(auto PartChild : AspectChild->GetChilds())
							{
								if(PartChild->GetName() == "style-diffuse-color")
								{
									assert(PartChild->HasAttribute("red") == true);
									assert(PartChild->HasAttribute("green") == true);
									assert(PartChild->HasAttribute("blue") == true);
									assert(PartChild->HasAttribute("opacity") == true);
									NewPartStyle->SetDiffuseColor(Color(from_string_cast< float >(PartChild->GetAttribute("red")), from_string_cast< float >(PartChild->GetAttribute("green")), from_string_cast< float >(PartChild->GetAttribute("blue")), from_string_cast< float >(PartChild->GetAttribute("opacity"))));
								}
								else if(PartChild->GetName() == "style-specular-color")
								{
									assert(PartChild->HasAttribute("red") == true);
									assert(PartChild->HasAttribute("green") == true);
									assert(PartChild->HasAttribute("blue") == true);
									assert(PartChild->HasAttribute("opacity") == true);
									NewPartStyle->SetSpecularColor(Color(from_string_cast< float >(PartChild->GetAttribute("red")), from_string_cast< float >(PartChild->GetAttribute("green")), from_string_cast< float >(PartChild->GetAttribute("blue")), from_string_cast< float >(PartChild->GetAttribute("opacity"))));
								}
								else if(PartChild->GetName() == "style-shininess")
								{
									assert(PartChild->HasAttribute("value") == true);
									NewPartStyle->SetShininess(from_string_cast< float >(PartChild->GetAttribute("value")));
								}
								else if(PartChild->GetName() == "style-program-identifier")
								{
									assert(PartChild->HasAttribute("value") == true);
									NewPartStyle->SetProgramIdentifier(PartChild->GetAttribute("value"));
								}
								else
								{
									throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + PartChild->GetName() + "\".");
								}
							}
							NewObject->GetAspectVisualization()->GetVisualizationPrototype()->SetPartStyle(AspectChild->GetAttribute("identifier"), NewPartStyle);
						}
						else
						{
							throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + AspectChild->GetName() + "\".");
						}
					}
				}
				else if(ObjectChild->GetName() == "type-specific")
				{
					if(SaveChild->GetAttribute("type-identifier") == "battery")
					{
						Battery * NewBattery(dynamic_cast< Battery * >(NewObject));
						
						assert(NewBattery != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "energy")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewBattery->SetEnergy(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "energy-capacity")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewBattery->SetEnergyCapacity(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else
							{
								throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + TypeSpecificChild->GetName() + "\".");
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "character")
					{
						Character * NewCharacter(dynamic_cast< Character * >(NewObject));
						
						assert(NewCharacter != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "credits")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewCharacter->SetCredits(from_string_cast< unsigned_numeric >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "map-knowledge")
							{
								for(std::vector< Element * >::const_iterator MapKnowledgeChild = TypeSpecificChild->GetChilds().begin(); MapKnowledgeChild != TypeSpecificChild->GetChilds().end(); ++MapKnowledgeChild)
								{
									if((*MapKnowledgeChild)->GetName() == "explored-system")
									{
										assert((*MapKnowledgeChild)->HasAttribute("object-identifier") == true);
										// red in second pass
									}
									else
									{
										throw std::runtime_error("The \"" + TypeSpecificChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + (*MapKnowledgeChild)->GetName() + "\".");
									}
								}
							}
							else
							{
								throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + TypeSpecificChild->GetName() + "\".");
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "commodity")
					{
						Commodity * NewCommodity(dynamic_cast< Commodity * >(NewObject));
						
						assert(NewCommodity != 0);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "angular-velocity")
							{
								assert(TypeSpecificChild->HasAttribute("axis-x") == true);
								assert(TypeSpecificChild->HasAttribute("axis-y") == true);
								assert(TypeSpecificChild->HasAttribute("axis-z") == true);
								assert(TypeSpecificChild->HasAttribute("angle") == true);
								NewCommodity->SetAngularVelocity(AxisAngle(from_string_cast< float >(TypeSpecificChild->GetAttribute("axis-x")), from_string_cast< float >(TypeSpecificChild->GetAttribute("axis-y")), from_string_cast< float >(TypeSpecificChild->GetAttribute("axis-z")), from_string_cast< float >(TypeSpecificChild->GetAttribute("angle"))));
							}
							else if(TypeSpecificChild->GetName() == "hull")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewCommodity->SetHull(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "velocity")
							{
								assert(TypeSpecificChild->HasAttribute("x") == true);
								assert(TypeSpecificChild->HasAttribute("y") == true);
								assert(TypeSpecificChild->HasAttribute("z") == true);
								NewCommodity->SetVelocity(Vector3f(from_string_cast< float >(TypeSpecificChild->GetAttribute("x")), from_string_cast< float >(TypeSpecificChild->GetAttribute("y")), from_string_cast< float >(TypeSpecificChild->GetAttribute("z"))));
							}
							else
							{
								throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + TypeSpecificChild->GetName() + "\".");
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "generator")
					{
						Generator * NewGenerator(dynamic_cast< Generator * >(NewObject));
						
						assert(NewGenerator != 0);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "energy-provision-per-second")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewGenerator->SetEnergyProvisionPerSecond(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else
							{
								throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + TypeSpecificChild->GetName() + "\".");
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "hangar")
					{
						Hangar * NewHangar(dynamic_cast< Hangar * >(NewObject));
						
						assert(NewHangar != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "character")
							{
								assert(TypeSpecificChild->HasAttribute("object-identifier") == true);
								// read in second pass
							}
							else
							{
								throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + TypeSpecificChild->GetName() + "\".");
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "mind")
					{
						Mind * NewMind(dynamic_cast< Mind * >(NewObject));
						
						assert(NewMind != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "character")
							{
								assert(TypeSpecificChild->HasAttribute("object-identifier") == true);
								// read in second pass
							}
							else
							{
								throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + TypeSpecificChild->GetName() + "\".");
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "ship")
					{
						Ship * NewShip(dynamic_cast< Ship * >(NewObject));
						
						assert(NewShip != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "exhaust-offset")
							{
								assert(TypeSpecificChild->HasAttribute("x") == true);
								assert(TypeSpecificChild->HasAttribute("y") == true);
								assert(TypeSpecificChild->HasAttribute("z") == true);
								NewShip->SetExhaustOffset(Vector3f(from_string_cast< float >(TypeSpecificChild->GetAttribute("x")), from_string_cast< float >(TypeSpecificChild->GetAttribute("y")), from_string_cast< float >(TypeSpecificChild->GetAttribute("z"))));
							}
							else if(TypeSpecificChild->GetName() == "faction")
							{
								assert(TypeSpecificChild->HasAttribute("identifier") == true);
								NewShip->SetFaction(g_Galaxy->GetFaction(TypeSpecificChild->GetAttribute("identifier"))->GetReference());
							}
							else if(TypeSpecificChild->GetName() == "fuel")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewShip->SetFuel(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "fuel-capacity")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewShip->SetFuelCapacity(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "fuel-needed-to-jump")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewShip->SetFuelNeededToJump(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "hull")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewShip->SetHull(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "hull-capacity")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewShip->SetHullCapacity(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "maximum-forward-thrust")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewShip->SetMaximumForwardThrust(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "maximum-speed")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewShip->SetMaximumSpeed(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "maximum-turn-speed")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewShip->SetMaximumTurnSpeed(from_string_cast< float >(TypeSpecificChild->GetAttribute("value")));
							}
							else if(TypeSpecificChild->GetName() == "velocity")
							{
								assert(TypeSpecificChild->HasAttribute("x") == true);
								assert(TypeSpecificChild->HasAttribute("y") == true);
								assert(TypeSpecificChild->HasAttribute("z") == true);
								NewShip->SetVelocity(Vector3f(from_string_cast< float >(TypeSpecificChild->GetAttribute("x")), from_string_cast< float >(TypeSpecificChild->GetAttribute("y")), from_string_cast< float >(TypeSpecificChild->GetAttribute("z"))));
							}
							else
							{
								throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + TypeSpecificChild->GetName() + "\".");
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "storage")
					{
						Storage * NewStorage(dynamic_cast< Storage * >(NewObject));
						
						assert(NewStorage != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "space-capacity")
							{
								assert(TypeSpecificChild->HasAttribute("value") == true);
								NewStorage->SetSpaceCapacity(from_string_cast< unsigned_numeric >(TypeSpecificChild->GetAttribute("value")));
							}
							else
							{
								throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + TypeSpecificChild->GetName() + "\".");
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "weapon")
					{
						Weapon * NewWeapon(dynamic_cast< Weapon * >(NewObject));
						
						assert(NewWeapon != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							throw std::runtime_error("The \"" + ObjectChild->GetName() + "\" element for the object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unknown element \"" + TypeSpecificChild->GetName() + "\".");
						}
					}
					else
					{
						throw std::runtime_error("The object \"" + SaveChild->GetAttribute("object-identifier") + "\" requests a type-specific setup for the unknown type \"" + SaveChild->GetAttribute("type-identifier") + "\".");
					}
				}
				else
				{
					throw std::runtime_error("The object \"" + SaveChild->GetAttribute("object-identifier") + "\" contains an unidentified child element \"" + ObjectChild->GetName() + "\".");
				}
			}
		}
		else
		{
			throw std::runtime_error("The \"save\" element contains an unidentified child element \"" + SaveChild->GetName() + "\".");
		}
	}
	// this is a hack
	assert(CurrentSystem.empty() == false);
	g_CurrentSystem = g_Galaxy->GetSystem(CurrentSystem);
	assert(g_CurrentSystem != nullptr);
	// in the second pass we do a fast skip over the childs to resolve any object references
	// no errors except resolving errors will be displayed
	// at the moment this also resolves back references/containments
	for(auto SaveChild : SaveElement->GetChilds())
	{
		if(SaveChild->GetName() == "object")
		{
			assert(SaveChild->HasAttribute("object-identifier"));
			
			Object * TheObject(Object::GetObject(SaveChild->GetAttribute("object-identifier")));
			
			assert(TheObject != nullptr);
			for(auto ObjectChild : SaveChild->GetChilds())
			{
				if(ObjectChild->GetName() == "aspect-object-container")
				{
					for(auto AspectChild : ObjectChild->GetChilds())
					{
						if(AspectChild->GetName() == "content")
						{
							Object * Content(Object::GetObject(AspectChild->GetAttribute("object-identifier")));
							
							assert(Content != nullptr);
							TheObject->GetAspectObjectContainer()->AddContent(Content);
						}
					}
				}
				else if(ObjectChild->GetName() == "type-specific")
				{
					if(SaveChild->GetAttribute("type-identifier") == "character")
					{
						Character * TheCharacter(dynamic_cast< Character * >(TheObject));
						
						assert(TheCharacter != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "map-knowledge")
							{
								for(auto MapKnowledgeChild : TypeSpecificChild->GetChilds())
								{
									if(MapKnowledgeChild->GetName() == "explored-system")
									{
										Object * ExploredSystem(Object::GetObject(MapKnowledgeChild->GetAttribute("object-identifier")));
										
										assert(ExploredSystem != nullptr);
										assert(dynamic_cast< System * >(ExploredSystem) != nullptr);
										TheCharacter->GetMapKnowledge()->AddExploredSystem(dynamic_cast< System * >(ExploredSystem));
									}
								}
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "hangar")
					{
						Hangar * TheHangar(dynamic_cast< Hangar * >(TheObject));
						
						assert(TheHangar != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "character")
							{
								Object * TheCharacter(Object::GetObject(TypeSpecificChild->GetAttribute("object-identifier")));
								
								assert(TheCharacter != nullptr);
								assert(dynamic_cast< Character * >(TheCharacter) != nullptr);
								TheHangar->SetCharacter(dynamic_cast< Character * >(TheCharacter));
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "mind")
					{
						Mind * TheMind(dynamic_cast< Mind * >(TheObject));
						
						assert(TheMind != nullptr);
						for(auto TypeSpecificChild : ObjectChild->GetChilds())
						{
							if(TypeSpecificChild->GetName() == "character")
							{
								Object * TheCharacter(Object::GetObject(TypeSpecificChild->GetAttribute("object-identifier")));
								
								assert(TheCharacter != nullptr);
								assert(dynamic_cast< Character * >(TheCharacter) != nullptr);
								TheMind->SetCharacter(dynamic_cast< Character * >(TheCharacter));
							}
						}
					}
					else if(SaveChild->GetAttribute("type-identifier") == "ship")
					{
						// the current system has been read in the first pass
						// now we can place all the ships in the current system
						Ship * TheShip(dynamic_cast< Ship * >(TheObject));
						
						assert(TheShip != nullptr);
						if(TheShip->GetContainer() == nullptr)
						{
							g_CurrentSystem->GetAspectObjectContainer()->AddContent(TheShip);
						}
					}
				}
			}
		}
	}
	// in the third pass we do a fast skip over the elements and mount the objects that have an accessory aspect and are mounted
	for(auto SaveChild : SaveElement->GetChilds())
	{
		if(SaveChild->GetName() == "object")
		{
			assert(SaveChild->HasAttribute("object-identifier"));
			
			Object * TheObject(Object::GetObject(SaveChild->GetAttribute("object-identifier")));
			
			assert(TheObject != nullptr);
			for(auto ObjectChild : SaveChild->GetChilds())
			{
				if(ObjectChild->GetName() == "aspect-accessory")
				{
					for(auto AspectChild : ObjectChild->GetChilds())
					{
						if(AspectChild->GetName() == "slot")
						{
							Object * TheShip(TheObject->GetContainer());
							
							assert(TheShip != nullptr);
							assert(TheShip->GetAspectOutfitting() != nullptr);
							TheShip->GetAspectOutfitting()->GetSlot(AspectChild->GetAttribute("slot-identifier"))->Mount(TheObject);
						}
					}
				}
			}
		}
	}
	if(InputMindObjectIdentifier.empty() == false)
	{
		g_InputMind = Object::GetObject(InputMindObjectIdentifier)->GetReference();
	}
	if(ObservedCharacterObjectIdentifier.empty() == false)
	{
		g_CharacterObserver->SetObservedCharacter(Object::GetObject(ObservedCharacterObjectIdentifier)->GetReference());
	}
	OnOutputEnterSystem(g_CurrentSystem);
	RealTime::Invalidate();
	PopulateSystem(g_CurrentSystem);
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
	
	if(!InputFileStream)
	{
		std::cerr << "The savegame file '" << FileName << "' does not exist or is not readable." << std::endl;
		
		return false;
	}
	
	return LoadGameFromInputStream(InputFileStream);
}

bool LoadScenario(const Scenario * Scenario)
{
	assert(Scenario != 0);
	
	std::string SavegameString(g_ResourceReader->ReadSavegameFromScenarioPath(Scenario->GetResourcePath()));
	
	if(SavegameString.empty() == true)
	{
		std::cerr << "The scenario '" << Scenario->GetName() << "' does not exist or is not readable." << std::endl;
		
		return false;
	}
	
	std::stringstream SavegameStringStream(SavegameString);
	
	return LoadGameFromInputStream(SavegameStringStream);
}

bool LoadScenarioFromScenarioIdentifier(const std::string & ScenarioIdentifier)
{
	assert(g_ScenarioManager != 0);
	
	return LoadScenario(g_ScenarioManager->Get(ScenarioIdentifier));
}

void SaveGame(std::ostream & OStream)
{
	XMLStream XML(OStream);
	
	XML << element << "save";
	
	XML << element << "game-time" << attribute << "value" << value << GameTime::Get() << end;
	if(g_CurrentSystem != nullptr)
	{
		XML << element << "current-system" << attribute << "identifier" << value << g_CurrentSystem->GetClassIdentifier() << end;
	}
	XML << element << "time-warp" << attribute << "value" << value << g_TimeWarp << end;
	if(g_InputMind.IsValid() == true)
	{
		XML << element << "input-mind" << attribute << "object-identifier" << value << g_InputMind->GetObjectIdentifier() << end;
	}
	if(g_CharacterObserver->GetObservedCharacter().IsValid() == true)
	{
		XML << element << "observed-character" << attribute << "object-identifier" << value << g_CharacterObserver->GetObservedCharacter()->GetObjectIdentifier() << end;
	}
	// save main camera properties
	XML << element << "main-camera";
	XML << element << "position" << attribute << "x" << value << g_CameraPosition[0] << attribute << "y" << value << g_CameraPosition[1] << attribute << "z" << value << g_CameraPosition[2] << end;
	assert(g_MainProjection != nullptr);
	XML << element << "field-of-view-y" << attribute << "radians" << value << g_MainProjection->GetFieldOfViewY() << end;
	XML << end; // camera
	// now save the impoartant objects
	if(g_InputMind.IsValid() == true)
	{
		// if no character is available
		if(g_InputMind->GetCharacter() == nullptr)
		{
			// only save the input mind
			WriteToXMLStream(XML, g_InputMind.Get(), true);
		}
		else
		{
			// if no ship is available
			if(g_InputMind->GetCharacter()->GetShip() == nullptr)
			{
				// only save the character
				WriteToXMLStream(XML, g_InputMind->GetCharacter(), true);
			}
			else
			{
				// save the complete ship
				WriteToXMLStream(XML, g_InputMind->GetCharacter()->GetShip(), true);
			}
			// save hangars
			for(auto System : g_Galaxy->GetSystems())
			{
				for(auto Planet : System.second->GetPlanets())
				{
					auto Hangar(Planet->GetHangar(g_InputMind->GetCharacter()));
					
					if(Hangar != nullptr)
					{
						assert(Planet->GetAspectObjectContainer() != nullptr);
						XML << element << "object" << attribute << "type-identifier" << value << Planet->GetTypeIdentifier() << attribute << "class-identifier" << value << Planet->GetClassIdentifier() << attribute << "object-identifier" << value << Planet->GetObjectIdentifier();
						XML << element << "aspect-object-container";
						XML << element << "content" << attribute << "object-identifier" << value << Hangar->GetObjectIdentifier() << end;
						XML << end;
						XML << end;
						WriteToXMLStream(XML, Hangar, true);
					}
				}
			}
		}
	}
	XML << end; // save
	OStream << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Actions                                                                                       //
// - Actions must be functions with no arguments and no return values.                           //
// - Actions are called in response to key events.                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
void ActionDecreaseFieldOfView(void)
{
	assert(g_MainProjection != 0);
	g_MainProjection->SetFieldOfViewY(0.9f * g_MainProjection->GetFieldOfViewY());
}

void ActionDecreaseTimeWarp(void)
{
	g_TimeWarp /= 1.1f;
}

void ActionDeleteObservedObject(void)
{
	if(g_InputMind.IsValid() == true)
	{
		DeleteObject(g_InputMind->GetCharacter()->GetShip());
	}
	else if(g_CharacterObserver->GetObservedCharacter().IsValid() == true)
	{
		DeleteObject(g_CharacterObserver->GetObservedCharacter()->GetShip());
	}
}

void ActionDisableAccelerate(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->DisableAccelerate();
	}
}

void ActionDisableFire(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->DisableFire();
	}
}

void ActionDisableTurnLeft(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->DisableTurnLeft();
	}
}

void ActionDisableTurnRight(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->DisableTurnRight();
	}
}

void ActionDumpObjectReport(void)
{
	std::string FileName(MakeTimeStampedFileName("object-report", "xml"));
	std::ofstream OutputFileStream(FileName.c_str());
	XMLStream Out(OutputFileStream);
	
	Object::Dump(Out);
	OutputFileStream << std::endl;
	SetMessage("Object report written to file \"" + FileName + "\".");
}

void ActionEnableAccelerate(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->EnableAccelerate();
	}
}

void ActionEnableFire(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->EnableFire();
	}
}

void ActionEnableTurnLeft(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->EnableTurnLeft();
	}
}

void ActionEnableTurnRight(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->EnableTurnRight();
	}
}

void ActionIncreaseFieldOfView(void)
{
	assert(g_MainProjection != 0);
	g_MainProjection->SetFieldOfViewY(1.1f * g_MainProjection->GetFieldOfViewY());
}

void ActionIncreaseTimeWarp(void)
{
	g_TimeWarp *= 1.1f;
}

void ActionJettisonCargo(void)
{
	if(g_InputMind.IsValid() == true)
	{
		SetMessage("Jettison cargo.");
		g_InputMind->Jettison();
	}
}

void ActionJump(void)
{
	if(g_InputMind.IsValid() == true)
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

void ActionLand(void)
{
	if(g_InputMind.IsValid() == true)
	{
		const Planet * SelectedPlanet(dynamic_cast< const Planet * >(g_InputMind->GetCharacter()->GetShip()->GetTarget()));
		
		switch(WantToLand(g_InputMind->GetCharacter(), g_InputMind->GetCharacter()->GetShip(), SelectedPlanet))
		{
		case OK:
			{
				g_InputMind->GetCharacter()->RemoveCredits(SelectedPlanet->GetLandingFeePerSpace() * g_InputMind->GetCharacter()->GetShip()->GetAspectPhysical()->GetSpaceRequirement());
				g_InputMind->Land();
				
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

void ActionObserveNextCharacter(void)
{
	if(g_InputMind.IsValid() == false)
	{
		std::set< Character * > Characters(Character::GetCharacters());
		
		if(g_CharacterObserver->GetObservedCharacter().IsValid() == false)
		{
			if(Characters.empty() == false)
			{
				g_CharacterObserver->SetObservedCharacter((*(Characters.begin()))->GetReference());
			}
		}
		else
		{
			std::set< Character * >::iterator CharacterIterator(Characters.find(g_CharacterObserver->GetObservedCharacter().Get()));
			
			assert(CharacterIterator != Characters.end());
			++CharacterIterator;
			if(CharacterIterator == Characters.end())
			{
				g_CharacterObserver->SetObservedCharacter(Reference< Character >());
			}
			else
			{
				g_CharacterObserver->SetObservedCharacter((*CharacterIterator)->GetReference());
			}
		}
		if(g_CharacterObserver->GetObservedCharacter().IsValid() == true)
		{
			g_CameraPosition[0] = 0.0f;
			g_CameraPosition[1] = 0.0f;
		}
	}
}

void ActionObservePreviousCharacter(void)
{
	if(g_InputMind.IsValid() == false)
	{
		std::set< Character * > Characters(Character::GetCharacters());
		
		if(g_CharacterObserver->GetObservedCharacter().IsValid() == false)
		{
			if(Characters.empty() == false)
			{
				g_CharacterObserver->SetObservedCharacter((*(Characters.rbegin()))->GetReference());
			}
		}
		else
		{
			std::set< Character * >::iterator CharacterIterator(Characters.find(g_CharacterObserver->GetObservedCharacter().Get()));
			
			assert(CharacterIterator != Characters.end());
			if(CharacterIterator == Characters.begin())
			{
				g_CharacterObserver->SetObservedCharacter(Reference< Character >());
			}
			else
			{
				--CharacterIterator;
				g_CharacterObserver->SetObservedCharacter((*CharacterIterator)->GetReference());
			}
		}
		if(g_CharacterObserver->GetObservedCharacter().IsValid() == true)
		{
			g_CameraPosition[0] = 0.0f;
			g_CameraPosition[1] = 0.0f;
		}
	}
}

void ActionOpenMainMenuWindow(void)
{
	if(g_MainMenuWindow == 0)
	{
		g_Pause = true;
		g_MainMenuWindow = new UI::MainMenuWindow(g_UserInterface->GetRootWidget(), g_ScenarioManager);
		// crude heuristic: if we are not in a system, no game is running
		if(g_CurrentSystem == 0)
		{
			g_MainMenuWindow->GetResumeGameButton()->SetEnabled(false);
			g_MainMenuWindow->GetSaveGameButton()->SetEnabled(false);
		}
		g_MainMenuWindow->SetPosition(Vector2f((g_UserInterface->GetRootWidget()->GetSize()[0] - g_MainMenuWindow->GetSize()[0]) / 2.0f, (g_UserInterface->GetRootWidget()->GetSize()[1] - g_MainMenuWindow->GetSize()[1]) / 2.0f));
		g_MainMenuWindow->GrabKeyFocus();
		g_MainMenuWindow->ConnectDestroyingCallback(OnMainMenuDestroying);
	}
}

void ActionOpenMapDialog(void)
{
	if((g_MapDialog == 0) && (g_CharacterObserver->GetObservedCharacter().IsValid() == true))
	{
		System * CurrentSystem(dynamic_cast< System * >(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetContainer()));
		
		assert(CurrentSystem != 0);
		g_MapDialog = new UI::MapDialog(g_UserInterface->GetRootWidget(), CurrentSystem, g_CharacterObserver->GetObservedCharacter().Get());
		g_MapDialog->GrabKeyFocus();
		g_MapDialog->ConnectDestroyingCallback(OnMapDialogDestroying);
		if(g_InputMind.IsValid() == true)
		{
			g_InputMind->DisableAccelerate();
			g_InputMind->DisableTurnLeft();
			g_InputMind->DisableTurnRight();
		}
	}
}

void ActionOpenObjectInformationDialog(void)
{
	UI::ObjectInformationDialog * Dialog(new UI::ObjectInformationDialog(g_UserInterface->GetRootWidget(), g_Galaxy->GetReference()));
	
	Dialog->GrabKeyFocus();
}

void ActionOpenOutfitShipDialog(void)
{
	if((g_OutfitShipDialog == 0) && (g_CharacterObserver->GetObservedCharacter().IsValid() == true))
	{
		g_OutfitShipDialog = new UI::OutfitShipDialog(g_UserInterface->GetRootWidget(), g_CharacterObserver->GetObservedCharacter()->GetShip()->GetReference());
		g_OutfitShipDialog->GrabKeyFocus();
		g_OutfitShipDialog->ConnectDestroyingCallback(OnOutfitShipDialogDestroying);
	}
}

void ActionPurgeGame(void)
{
	PurgeGame();
}

void ActionQuitGameLoop(void)
{
	g_Quit = true;
}

void ActionQuitGameAndDumpObjectReport(void)
{
	g_DumpEndReport = true;
	g_Quit = true;
}

void ActionRefuel(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->Refuel();
	}
}

void ActionResetCameraPosition(void)
{
	g_CameraPosition[0] = 0.0f;
	g_CameraPosition[1] = 0.0f;
}

void ActionResetTimeWarp(void)
{
	g_TimeWarp = 1.0f;
}

void ActionScoop(void)
{
	if(g_InputMind.IsValid() == true)
	{
		switch(WantToScoop(g_InputMind->GetCharacter()->GetShip(), dynamic_cast< const Commodity * >(g_InputMind->GetCharacter()->GetShip()->GetTarget())))
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

void ActionSelectNextLinkedSystem(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->SelectNextLinkedSystem();
	}
}

void ActionSpawnFighter(void)
{
	std::stringstream IdentifierPrefix;
	
	IdentifierPrefix << "::system(" << g_CurrentSystem->GetClassIdentifier() << ")::created_at_game_time(" << std::fixed << GameTime::Get() << ")";
	SpawnShip(g_CurrentSystem, IdentifierPrefix.str(), "fighter");
}

void ActionSpawnRandomShip(void)
{
	assert(g_CurrentSystem != nullptr);
	
	std::stringstream IdentifierPrefix;
	
	IdentifierPrefix << "::system(" << g_CurrentSystem->GetClassIdentifier() << ")::created_at_game_time(" << std::fixed << GameTime::Get() << ")";
	SpawnShip(g_CurrentSystem, IdentifierPrefix.str());
}

void ActionTakeScreenShot(void)
{
	g_TakeScreenShot = true;
}

void ActionTargetNearestCargo(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->TargetNearestCargo();
	}
}

void ActionTargetNextCargo(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->TargetNextCargo();
	}
}

void ActionTargetNextPlanet(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->TargetNextPlanet();
	}
}

void ActionTargetNextShip(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->TargetNextShip();
	}
}

void ActionTargetPreviousCargo(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->TargetPreviousCargo();
	}
}

void ActionTargetPreviousPlanet(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->TargetPreviousPlanet();
	}
}

void ActionTargetPreviousShip(void)
{
	if(g_InputMind.IsValid() == true)
	{
		g_InputMind->TargetPreviousShip();
	}
}

void ActionToggleFirstPersonCameraMode(void)
{
	g_FirstPersonCameraMode = !g_FirstPersonCameraMode;
}

void ActionToggleTimingDialog(void)
{
	if(g_TimingDialog == 0)
	{
		g_TimingDialog = new UI::TimingDialog(g_UserInterface->GetRootWidget());
		g_TimingDialog->GrabKeyFocus();
		g_TimingDialog->ConnectDestroyingCallback(OnTimingDialogDestroying);
	}
	else
	{
		g_TimingDialog->Destroy();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//  input events for the main view                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
void MainViewKeyEvent(UI::KeyEvent & KeyEvent)
{
	if((KeyEvent.GetPhase() == UI::Event::Phase::Target) && (KeyEvent.GetKeyCode() >= 0) && (KeyEvent.GetKeyCode() < 127))
	{
		int EventIndex = -1;
		
		if(KeyEvent.IsDown() == true)
		{
			EventIndex = 0;
		}
		else
		{
			EventIndex = 1;
		}
		if(g_KeyboardLookupTable[KeyEvent.GetKeyCode()][EventIndex] != 0)
		{
			g_KeyboardLookupTable[KeyEvent.GetKeyCode()][EventIndex]();
		}
	}
}

void MainViewMouseButtonEvent(UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetPhase() == UI::Event::Phase::Target) || (MouseButtonEvent.GetPhase() == UI::Event::Phase::Bubbling))
	{
		switch(MouseButtonEvent.GetMouseButton())
		{
		case UI::MouseButtonEvent::MouseButton::Middle:
			{
				if(MouseButtonEvent.IsDown() == true)
				{
					g_LastMotion = MouseButtonEvent.GetPosition();
					g_MouseButton = UI::MouseButtonEvent::MouseButton::Middle;
				}
				else
				{
					g_MouseButton = UI::MouseButtonEvent::MouseButton::Unspecified;
				}
				
				break;
			}
		case UI::MouseButtonEvent::MouseButton::WheelUp:
			{
				if(MouseButtonEvent.IsDown() == true)
				{
					g_CameraPosition[2] *= 0.95f;
				}
				
				break;
			}
		case UI::MouseButtonEvent::MouseButton::WheelDown:
			{
				if(MouseButtonEvent.IsDown() == true)
				{
					g_CameraPosition[2] *= 1.05f;
				}
				
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void MainViewMouseMove(UI::MouseMoveEvent & MouseMoveEvent)
{
	if(MouseMoveEvent.GetPhase() == UI::Event::Phase::Target)
	{
		auto Delta(MouseMoveEvent.GetPosition() - g_LastMotion);
		
		g_LastMotion = MouseMoveEvent.GetPosition();
		if(g_MouseButton == UI::MouseButtonEvent::MouseButton::Middle)
		{
			g_CameraPosition[0] = g_CameraPosition[0] - Delta[0] * 0.0011f * g_CameraPosition[2];
			g_CameraPosition[1] = g_CameraPosition[1] + Delta[1] * 0.0011f * g_CameraPosition[2];
		}
	}
}

typedef GLXContext (* CreateContextWithAttributesFunction)(Display * Display, GLXFBConfig FBConfiguration, GLXContext ShareContext, bool DirectRendering, const int * AttributeList);

void CreateWindow(void)
{
	ON_DEBUG(std::cout << "Opening display." << std::endl);
	g_Display = XOpenDisplay(0);
	if(g_Display == 0)
	{
		std::cerr << "Coud not open the default display." << std::endl;
		exit(1);
	}
	ON_DEBUG(std::cout << "Checking for GLX availability." << std::endl);
	
	int ErrorBase(0);
	int EventBase(0);
	
	if(glXQueryExtension(g_Display, &ErrorBase, &EventBase) == true)
	{
		ON_DEBUG(std::cout << "  The GLX extension is available." << std::endl);
	}
	else
	{
		std::cerr << "The GLX extension is not available." << std::endl;
		exit(1);
	}
	ON_DEBUG(std::cout << "Checking for GLX version." << std::endl);
	
	int MajorVersionNumber(0);
	int MinorVersionNumber(0);
	
	if(glXQueryVersion(g_Display, &MajorVersionNumber, &MinorVersionNumber) == false)
	{
		std::cerr << "Checking the GLX version failed." << std::endl;
		exit(1);
	}
	else
	{
		ON_DEBUG(std::cout << "  The GLX version is " << MajorVersionNumber << "." << MinorVersionNumber << "." << std::endl);
		if((MajorVersionNumber < 1) || ((MajorVersionNumber == 1) && (MinorVersionNumber < 4)))
		{
			std::cerr << "The GLX version " << MajorVersionNumber << "." << MinorVersionNumber << " is below the minimal required version number 1.4." << std::endl;
			exit(1);
		}
	}
	ON_DEBUG(std::cout << "Getting default screen." << std::endl);
	
	int ScreenNumber(DefaultScreen(g_Display));
	
	ON_DEBUG(std::cout << "Getting the GLX server properties." << std::endl);
	
	const char * GLXServerVendor(glXQueryServerString(g_Display, ScreenNumber, GLX_VENDOR));
	
	if(GLXServerVendor != 0)
	{
		ON_DEBUG(std::cout << "  GLX server vendor: \"" << GLXServerVendor << "\"" << std::endl);
	}
	else
	{
		std::cerr << "The GLX server vendor string is invalid." << std::endl;
		exit(1);
	}
	
	const char * GLXServerVersion(glXQueryServerString(g_Display, ScreenNumber, GLX_VERSION));
	
	if(GLXServerVersion != 0)
	{
		ON_DEBUG(std::cout << "  GLX server version: \"" << GLXServerVersion << "\"" << std::endl);
	}
	else
	{
		std::cerr << "The GLX server version string is invalid." << std::endl;
		exit(1);
	}
	
	const char * GLXServerExtensionsString(glXQueryServerString(g_Display, ScreenNumber, GLX_EXTENSIONS));
	
	if(GLXServerExtensionsString != 0)
	{
		std::vector< std::string > GLXServerExtensions(SplitString(GLXServerExtensionsString, ' '));
		
		ON_DEBUG(std::cout << "  GLX server extensions:" << std::endl);
		for(std::vector< std::string >::iterator Iterator = GLXServerExtensions.begin(); Iterator != GLXServerExtensions.end(); ++Iterator)
		{
			ON_DEBUG(std::cout << "    \"" << *Iterator << '"' << std::endl);
		}
	}
	else
	{
		std::cerr << "The GLX server extensions string is invalid." << std::endl;
		exit(1);
	}
	ON_DEBUG(std::cout << "Getting the GLX client properties." << std::endl);
	
	const char * GLXClientVendor(glXGetClientString(g_Display, GLX_VENDOR));
	
	if(GLXClientVendor != 0)
	{
		ON_DEBUG(std::cout << "  GLX client vendor: \"" << GLXClientVendor << "\"" << std::endl);
	}
	else
	{
		std::cerr << "The GLX client vendor string is invalid." << std::endl;
		exit(1);
	}
	
	const char * GLXClientVersion(glXGetClientString(g_Display, GLX_VERSION));
	
	if(GLXClientVersion != 0)
	{
		ON_DEBUG(std::cout << "  GLX client version: \"" << GLXClientVersion << "\"" << std::endl);
	}
	else
	{
		std::cerr << "The GLX client version string is invalid." << std::endl;
		exit(1);
	}
	
	const char * GLXClientExtensionsString(glXGetClientString(g_Display, GLX_EXTENSIONS));
	
	if(GLXClientExtensionsString != 0)
	{
		std::vector< std::string > GLXClientExtensions(SplitString(GLXClientExtensionsString, ' '));
		
		ON_DEBUG(std::cout << "  GLX client extensions:" << std::endl);
		for(std::vector< std::string >::iterator Iterator = GLXClientExtensions.begin(); Iterator != GLXClientExtensions.end(); ++Iterator)
		{
			ON_DEBUG(std::cout << "    \"" << *Iterator << '"' << std::endl);
		}
	}
	else
	{
		std::cerr << "The GLX client extensions string is invalid." << std::endl;
		exit(1);
	}
	ON_DEBUG(std::cout << "Getting the GLX connection extensions." << std::endl);
	
	const char * GLXExtensionsString(glXQueryExtensionsString(g_Display, ScreenNumber));
	std::vector< std::string > ExtensionStrings(SplitString(GLXExtensionsString, ' '));
	bool GLX_ARB_create_context_Available(false);
	
	ON_DEBUG(std::cout << "  GLX connection extensions:" << std::endl);
	for(std::vector< std::string >::iterator Iterator = ExtensionStrings.begin(); Iterator != ExtensionStrings.end(); ++Iterator)
	{
		if(*Iterator == "GLX_ARB_create_context")
		{
			GLX_ARB_create_context_Available = true;
		}
		ON_DEBUG(std::cout << "    \"" << *Iterator << '"' << std::endl);
	}
	if(GLX_ARB_create_context_Available == false)
	{
		std::cerr << "Your GLX connection does not support the required extension GLX_ARB_create_context." << std::endl;
		exit(1);
	}
	
	int NumberOfConfigurations(0);
	int RequestedAttributes[] =
	{
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_X_RENDERABLE, True,
		GLX_BUFFER_SIZE, 32,
		GLX_DOUBLEBUFFER, True,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		0
	};
	ON_DEBUG(std::cout << "Choosing a FB configuration." << std::endl);
	GLXFBConfig * Configurations(glXChooseFBConfig(g_Display, ScreenNumber, RequestedAttributes, &NumberOfConfigurations));
	
	if(Configurations == 0)
	{
		std::cerr << "Sorry, could not find a suitable FB configuration." << std::endl;
		exit(1);
	}
	assert(g_Settings != 0);
	if(g_Settings->GetWindowDimensions() == 0)
	{
		ON_DEBUG(std::cout << "Getting display dimensions." << std::endl);
		g_Width = DisplayWidth(g_Display, ScreenNumber);
		g_Height = DisplayHeight(g_Display, ScreenNumber);
	}
	else
	{
		g_Width = g_Settings->GetWindowDimensions()->operator[](0);
		g_Height = g_Settings->GetWindowDimensions()->operator[](1);
	}
	ON_DEBUG(std::cout << "Getting visualization information." << std::endl);
	
	XVisualInfo * VisualizationInformation(glXGetVisualFromFBConfig(g_Display, Configurations[0]));
	
	ON_DEBUG(std::cout << "Creating an X colormap." << std::endl);
	g_ColorMap = XCreateColormap(g_Display, RootWindow(g_Display, VisualizationInformation->screen), VisualizationInformation->visual, AllocNone);
	
	XSetWindowAttributes WindowAttributes;
	
	WindowAttributes.colormap = g_ColorMap;
	WindowAttributes.border_pixel = 0;
	WindowAttributes.event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | PointerMotionMask;
	ON_DEBUG(std::cout << "Creating X window." << std::endl);
	g_Window = XCreateWindow(g_Display, RootWindow(g_Display, VisualizationInformation->screen), 0, 0, static_cast< unsigned int >(g_Width), static_cast< unsigned int >(g_Height), 0, VisualizationInformation->depth, InputOutput, VisualizationInformation->visual, CWBorderPixel | CWColormap | CWEventMask/* | CWOverrideRedirect*/, &WindowAttributes);
	if(g_Window == 0)
	{
		std::cerr << "Failed to create a window with the specified visualization information." << std::endl;
		exit(1);
	}
	XFree(VisualizationInformation);
	
	Atom wmDelete = XInternAtom(g_Display, "WM_DELETE_WINDOW", True);
	
	XSetWMProtocols(g_Display, g_Window, &wmDelete, 1);
	XStoreName(g_Display, g_Window, "galactic-fall 0.2");
	XMapWindow(g_Display, g_Window);
	ON_DEBUG(std::cout << "Getting function \"glXCreateContextAttribsARB\"." << std::endl);
	
	CreateContextWithAttributesFunction CreateContextWithAttributes(reinterpret_cast< CreateContextWithAttributesFunction >(glXGetProcAddressARB(reinterpret_cast< const GLubyte * >("glXCreateContextAttribsARB"))));
	
	if(CreateContextWithAttributes == 0)
	{
		std::cerr << "Failed to get the function \"glXCreateContextAttribsARB\"." << std::endl;
		exit(1);
	}
	
	int RequestedContextAttributes[] =
	{
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 1,
// 		GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		None
	};
	
	ON_DEBUG(std::cout << "Creating GLX context." << std::endl);
	ON_DEBUG(std::cout << "  Asking for GLX context with version " << RequestedContextAttributes[1] << '.' << RequestedContextAttributes[3] << "." << std::endl);
	g_GLXContext = CreateContextWithAttributes(g_Display, Configurations[0], 0, true, RequestedContextAttributes);
	if(glXIsDirect(g_Display, g_GLXContext) == false)
	{
		std::cerr << "Failed to acquire direct rendering context." << std::endl;
		exit(1);
	}
	else
	{
		ON_DEBUG(std::cout << "  Direct rendering context acquired." << std::endl);
	}
	ON_DEBUG(std::cout << "Activating context." << std::endl);
	glXMakeCurrent(g_Display, g_Window, g_GLXContext);
	XFree(Configurations);
}

void InitializeOpenGL(void)
{
	ON_DEBUG(std::cout << "Loading OpenGL functions." << std::endl);
	LoadOpenGLFunction(glAttachShader);
	LoadOpenGLFunction(glBegin);
	LoadOpenGLFunction(glBindBuffer);
	LoadOpenGLFunction(glBindFramebuffer);
	LoadOpenGLFunction(glBindRenderbuffer);
	LoadOpenGLFunction(glBindTexture);
	LoadOpenGLFunction(glBindVertexArray);
	LoadOpenGLFunction(glBitmap);
	LoadOpenGLFunction(glBlendFunc);
	LoadOpenGLFunction(glBufferData);
	LoadOpenGLFunction(glCallLists);
	LoadOpenGLFunction(glCheckFramebufferStatus);
	LoadOpenGLFunction(glClear);
	LoadOpenGLFunction(glClearColor);
	LoadOpenGLFunction(glClipPlane);
	LoadOpenGLFunction(glColor3f);
	LoadOpenGLFunction(glColor4f);
	LoadOpenGLFunction(glColor4fv);
	LoadOpenGLFunction(glCompileShader);
	LoadOpenGLFunction(glCreateProgram);
	LoadOpenGLFunction(glCreateShader);
	LoadOpenGLFunction(glDeleteFramebuffers);
	LoadOpenGLFunction(glDeleteLists);
	LoadOpenGLFunction(glDeleteProgram);
	LoadOpenGLFunction(glDeleteRenderbuffers);
	LoadOpenGLFunction(glDeleteShader);
	LoadOpenGLFunction(glDeleteTextures);
	LoadOpenGLFunction(glDisable);
	LoadOpenGLFunction(glDrawArrays);
	LoadOpenGLFunction(glEnable);
	LoadOpenGLFunction(glEnableVertexAttribArray);
	LoadOpenGLFunction(glEnd);
	LoadOpenGLFunction(glEndList);
	LoadOpenGLFunction(glFramebufferRenderbuffer);
	LoadOpenGLFunction(glFramebufferTexture);
	LoadOpenGLFunction(glGenBuffers);
	LoadOpenGLFunction(glGenFramebuffers);
	LoadOpenGLFunction(glGenLists);
	LoadOpenGLFunction(glGenRenderbuffers);
	LoadOpenGLFunction(glGenTextures);
	LoadOpenGLFunction(glGenVertexArrays);
	LoadOpenGLFunction(glGetActiveAttrib);
	LoadOpenGLFunction(glGetActiveUniform);
	LoadOpenGLFunction(glGetAttribLocation);
	LoadOpenGLFunction(glGetIntegerv);
	LoadOpenGLFunction(glGetProgramiv);
	LoadOpenGLFunction(glGetShaderiv);
	LoadOpenGLFunction(glGetString);
	LoadOpenGLFunction(glGetUniformLocation);
	LoadOpenGLFunction(glLightfv);
	LoadOpenGLFunction(glLightModelfv);
	LoadOpenGLFunction(glLinkProgram);
	LoadOpenGLFunction(glListBase);
	LoadOpenGLFunction(glLoadIdentity);
	LoadOpenGLFunction(glLoadMatrixf);
	LoadOpenGLFunction(glMaterialf);
	LoadOpenGLFunction(glMaterialfv);
	LoadOpenGLFunction(glMatrixMode);
	LoadOpenGLFunction(glMultMatrixf);
	LoadOpenGLFunction(glNewList);
	LoadOpenGLFunction(glNormal3fv);
	LoadOpenGLFunction(glOrtho);
	LoadOpenGLFunction(glPixelStorei);
	LoadOpenGLFunction(glPopAttrib);
	LoadOpenGLFunction(glPopMatrix);
	LoadOpenGLFunction(glPushAttrib);
	LoadOpenGLFunction(glPushMatrix);
	LoadOpenGLFunction(glRasterPos2i);
	LoadOpenGLFunction(glReadBuffer);
	LoadOpenGLFunction(glReadPixels);
	LoadOpenGLFunction(glRenderbufferStorage);
	LoadOpenGLFunction(glRotatef);
	LoadOpenGLFunction(glScalef);
	LoadOpenGLFunction(glShaderSource);
	LoadOpenGLFunction(glTexCoord2f);
	LoadOpenGLFunction(glTexImage2D);
	LoadOpenGLFunction(glTexParameteri);
	LoadOpenGLFunction(glTranslatef);
	LoadOpenGLFunction(glUniform1f);
	LoadOpenGLFunction(glUniform3fv);
	LoadOpenGLFunction(glUniform4fv);
	LoadOpenGLFunction(glUniformMatrix3fv);
	LoadOpenGLFunction(glUniformMatrix4fv);
	LoadOpenGLFunction(glUseProgram);
	LoadOpenGLFunction(glVertex2f);
	LoadOpenGLFunction(glVertex2fv);
	LoadOpenGLFunction(glVertex3f);
	LoadOpenGLFunction(glVertex3fv);
	LoadOpenGLFunction(glVertexAttribPointer);
	LoadOpenGLFunction(glViewport);
	
	ON_DEBUG(std::cout << "OpenGL capabilities:" << std::endl);
	
	const unsigned char * ShadingLanguageVersion(0);
	
	ShadingLanguageVersion = GLGetString(GL_SHADING_LANGUAGE_VERSION);
	ON_DEBUG(std::cout << "  Shading Language Version: " << ShadingLanguageVersion << std::endl);
	
	int MaximalNumberOfLights(0);
	
	GLGetIntegerv(GL_MAX_LIGHTS, &MaximalNumberOfLights);
	ON_DEBUG(std::cout << "  Maximal number of lights: " << MaximalNumberOfLights << std::endl);
	
	int MaximalNumberOfUniformComponentsForVertexShaders(0);
	
	GLGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &MaximalNumberOfUniformComponentsForVertexShaders);
	ON_DEBUG(std::cout << "  Maximal number of uniform components for vertex shaders: " << MaximalNumberOfUniformComponentsForVertexShaders << std::endl);
	
	int MaximalNumberOfUniformComponentsForFragmentShaders(0);
	
	GLGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &MaximalNumberOfUniformComponentsForFragmentShaders);
	ON_DEBUG(std::cout << "  Maximal number of uniform components for fragment shaders: " << MaximalNumberOfUniformComponentsForFragmentShaders << std::endl);
	
	int MaximalNumberOfVertexAtributes(0);
	
	GLGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MaximalNumberOfVertexAtributes);
	ON_DEBUG(std::cout << "  Maximal number of vertex attributes: " << MaximalNumberOfVertexAtributes << std::endl);
	GLEnable(GL_CULL_FACE);
	GLEnable(GL_DEPTH_TEST);
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
				UI::MouseMoveEvent MouseMoveEvent;
				
				MouseMoveEvent.SetButtonEvent(&Event.xbutton);
				if(g_EchoEvents == true)
				{
					std::cout << "Motion:         x=" << MouseMoveEvent.GetPosition()[0] << "   y=" << MouseMoveEvent.GetPosition()[1] << std::endl;
				}
				g_UserInterface->DispatchMouseMoveEvent(MouseMoveEvent);
				
				break;
			}
		case ButtonPress:
		case ButtonRelease:
			{
				UI::MouseButtonEvent MouseButtonEvent;
				
				MouseButtonEvent.SetButtonEvent(&Event.xbutton);
				if(g_EchoEvents == true)
				{
					std::cout << "ButtonEvent:    state=" << ((MouseButtonEvent.IsDown() == true) ? ("down") : ((MouseButtonEvent.IsUp() == true) ? ("up  ") : ("unknown"))) << "   button=" << Event.xbutton.button << "   x=" << Event.xbutton.x << "   y=" << Event.xbutton.y << std::endl;
				}
				g_UserInterface->DispatchMouseButtonEvent(MouseButtonEvent);
				
				break;
			}
		case KeyPress:
		case KeyRelease:
			{
				UI::KeyEvent KeyEvent;
				
				KeyEvent.SetKeyEvent(&Event.xkey);
				if(g_EchoEvents == true)
				{
					std::cout << "KeyEvent:       key_state=" << ((KeyEvent.IsDown() == true) ? ("down") : ((KeyEvent.IsUp() == true) ? ("up  ") : ("unknown"))) << "   modifier_state=" << Event.xkey.state << "   key_code=" << KeyEvent.GetKeyCode() << "   string='" << KeyEvent.GetString() << "'  string.length=" << KeyEvent.GetString().length() << "   key_symbol=" << KeyEvent.GetKeySymbol() << std::endl;
				}
				g_UserInterface->DispatchKeyEvent(KeyEvent);
				
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
	XDestroyWindow(g_Display, g_Window);
	g_Window = 0;
	XFreeColormap(g_Display, g_ColorMap);
	g_ColorMap = 0;
    XCloseDisplay(g_Display);
	g_Display = 0;
}

void LoadKeyboardLookupTable(const std::list< Settings::KeyBinding > * KeyBindings)
{
	assert(KeyBindings != 0);
	assert(g_KeyboardLookupTable != 0);
	for(int Index = 0; Index < 128; ++Index)
	{
		g_KeyboardLookupTable[Index][0] = 0;
		g_KeyboardLookupTable[Index][1] = 0;
	}
	for(std::list< Settings::KeyBinding >::const_iterator KeyBindingIterator = KeyBindings->begin(); KeyBindingIterator != KeyBindings->end(); ++KeyBindingIterator)
	{
		if((KeyBindingIterator->Code >= 0) && (KeyBindingIterator->Code < 128))
		{
			int EventIndex = -1;
			
			if(KeyBindingIterator->Event == "down")
			{
				EventIndex = 0;
			}
			else if(KeyBindingIterator->Event == "up")
			{
				EventIndex = 1;
			}
			if(EventIndex != -1)
			{
				if(KeyBindingIterator->Action == "decrease_field_of_view")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionDecreaseFieldOfView;
				}
				else if(KeyBindingIterator->Action == "decrease_time_warp")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionDecreaseTimeWarp;
				}
				else if(KeyBindingIterator->Action == "delete_observed_object")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionDeleteObservedObject;
				}
				else if(KeyBindingIterator->Action == "disable_accelerate")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionDisableAccelerate;
				}
				else if(KeyBindingIterator->Action == "disable_fire")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionDisableFire;
				}
				else if(KeyBindingIterator->Action == "disable_turn_left")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionDisableTurnLeft;
				}
				else if(KeyBindingIterator->Action == "disable_turn_right")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionDisableTurnRight;
				}
				else if(KeyBindingIterator->Action == "dump_object_report")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionDumpObjectReport;
				}
				else if(KeyBindingIterator->Action == "enable_accelerate")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionEnableAccelerate;
				}
				else if(KeyBindingIterator->Action == "enable_fire")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionEnableFire;
				}
				else if(KeyBindingIterator->Action == "enable_turn_left")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionEnableTurnLeft;
				}
				else if(KeyBindingIterator->Action == "enable_turn_right")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionEnableTurnRight;
				}
				else if(KeyBindingIterator->Action == "increase_field_of_view")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionIncreaseFieldOfView;
				}
				else if(KeyBindingIterator->Action == "increase_time_warp")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionIncreaseTimeWarp;
				}
				else if(KeyBindingIterator->Action == "jettison_cargo")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionJettisonCargo;
				}
				else if(KeyBindingIterator->Action == "jump")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionJump;
				}
				else if(KeyBindingIterator->Action == "land")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionLand;
				}
				else if(KeyBindingIterator->Action == "observe_next_character")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionObserveNextCharacter;
				}
				else if(KeyBindingIterator->Action == "observe_previous_character")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionObservePreviousCharacter;
				}
				else if(KeyBindingIterator->Action == "open_main_menu_window")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionOpenMainMenuWindow;
				}
				else if(KeyBindingIterator->Action == "open_map_dialog")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionOpenMapDialog;
				}
				else if(KeyBindingIterator->Action == "open_object_information_dialog")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionOpenObjectInformationDialog;
				}
				else if(KeyBindingIterator->Action == "open_outfit_ship_dialog")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionOpenOutfitShipDialog;
				}
				else if(KeyBindingIterator->Action == "purge_game")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionPurgeGame;
				}
				else if(KeyBindingIterator->Action == "quit_game_and_dump_object_report")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionQuitGameAndDumpObjectReport;
				}
				else if(KeyBindingIterator->Action == "refuel")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionRefuel;
				}
				else if(KeyBindingIterator->Action == "reset_camera_position")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionResetCameraPosition;
				}
				else if(KeyBindingIterator->Action == "reset_time_warp")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionResetTimeWarp;
				}
				else if(KeyBindingIterator->Action == "scoop")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionScoop;
				}
				else if(KeyBindingIterator->Action == "spawn_fighter")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionSpawnFighter;
				}
				else if(KeyBindingIterator->Action == "take_screen_shot")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionTakeScreenShot;
				}
				else if(KeyBindingIterator->Action == "target_nearest_cargo")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionTargetNearestCargo;
				}
				else if(KeyBindingIterator->Action == "target_next_cargo")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionTargetNextCargo;
				}
				else if(KeyBindingIterator->Action == "target_next_planet")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionTargetNextPlanet;
				}
				else if(KeyBindingIterator->Action == "target_next_ship")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionTargetNextShip;
				}
				else if(KeyBindingIterator->Action == "target_previous_cargo")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionTargetPreviousCargo;
				}
				else if(KeyBindingIterator->Action == "target_previous_planet")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionTargetPreviousPlanet;
				}
				else if(KeyBindingIterator->Action == "target_previous_ship")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionTargetPreviousShip;
				}
				else if(KeyBindingIterator->Action == "toggle_first_person_camera_mode")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionToggleFirstPersonCameraMode;
				}
				else if(KeyBindingIterator->Action == "toggle_timing_dialog")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionToggleTimingDialog;
				}
				else if(KeyBindingIterator->Action == "select_next_linked_system")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionSelectNextLinkedSystem;
				}
				else if(KeyBindingIterator->Action == "spawn_random_ship")
				{
					g_KeyboardLookupTable[KeyBindingIterator->Code][EventIndex] = ActionSpawnRandomShip;
				}
				else
				{
					throw std::runtime_error("The action '" + KeyBindingIterator->Action + "' is unknown.");
				}
			}
		}
	}
}

void PrintSettings(void)
{
	std::cout << "  - Window Dimensions = " << g_Settings->GetWindowDimensions()->operator[](0) << " x " << g_Settings->GetWindowDimensions()->operator[](1) << std::endl;
	std::cout << "  - Key Binding Profile =" << std::endl;
	for(std::list< Settings::KeyBinding >::const_iterator KeyBindingIterator = g_Settings->GetKeyBindings()->begin(); KeyBindingIterator != g_Settings->GetKeyBindings()->end(); ++KeyBindingIterator)
	{
		std::cout << "      Code=" << KeyBindingIterator->Code << "  Event=" << KeyBindingIterator->Event << "  Action=" << KeyBindingIterator->Action << std::endl;
	}
}

int main(int argc, char ** argv)
{
	// setup the random number generator for everyday use
	srand(time(0));
	// parse command line
	ON_DEBUG(std::cout << "Parsing command line." << std::endl);
	
	std::vector< std::string > Arguments(argv, argv + argc);
	std::string LoadSavegameFileName;
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
		else if(Arguments[Index] == "--echo-resizes")
		{
			g_EchoResizes = true;
		}
	}
	
	// try loading the game data archive
	ON_DEBUG(std::cout << "Loading game archive." << std::endl);
	g_ResourceReader = new ResourceReader(DataFileName.substr(0, DataFileName.rfind('/')));
	if(g_ResourceReader->LoadArchive(DataFileName) == false)
	{
		return 1;
	}
	
	// read the settings from the archive
	ON_DEBUG(std::cout << "Reading the settings from the game archive." << std::endl);
	g_Settings = new Settings();
	g_ResourceReader->ReadSettings(g_Settings);
	ON_DEBUG(PrintSettings());
	
	// set up the timeout notification manager
	g_GameTimeTimeoutNotifications = new TimeoutNotificationManager();
	g_RealTimeTimeoutNotifications = new TimeoutNotificationManager();
	
	// setting up the graphical environment
	ON_DEBUG(std::cout << "Setting up the window." << std::endl);
	CreateWindow();
	ON_DEBUG(std::cout << "Initializing OpenGL." << std::endl);
	InitializeOpenGL();
	
	// create managers and global objects
	ON_DEBUG(std::cout << "Creating global managers and objects." << std::endl);
	g_Galaxy = 0;
	g_GraphicsEngine = new Graphics::Engine();
	// UI view
	g_UIProjection = new Graphics::Orthogonal2DProjection();
	g_UIProjection->SetLeft(0.0f);
	g_UIProjection->SetTop(0.0f);
	g_UIView = new Graphics::View();
	g_UIView->SetClearColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
	assert(g_UIView->GetCamera() != nullptr);
	g_UIView->GetCamera()->SetProjection(g_UIProjection);
	g_UIView->GetCamera()->SetSpacialMatrix(Matrix4f::CreateIdentity());
	g_GraphicsEngine->AddView(g_UIView);
	
	Graphics::Scene * UIScene(new Graphics::Scene());
	
	UIScene->SetDestroyCallback(OnUISceneNodeDestroy);
	g_UIView->SetScene(UIScene);
	
	Graphics::UIRootNode * UIRootNode(new Graphics::UIRootNode());
	
	UIRootNode->SetClearColorBuffer(true);
	UIRootNode->SetUseDepthTest(false);
	UIRootNode->SetUseBlending(true);
	UIRootNode->SetUseClipPlane0(true);
	UIRootNode->SetUseClipPlane1(true);
	UIRootNode->SetUseClipPlane2(true);
	UIRootNode->SetUseClipPlane3(true);
	UIRootNode->SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	UIScene->SetRootNode(UIRootNode);
	// main view
	g_MainProjection = new Graphics::PerspectiveProjection();
	g_MainProjection->SetNearClippingPlane(1.0f);
	g_MainProjection->SetFarClippingPlane(1000.0f);
	g_MainView = new Graphics::View();
	g_MainView->SetClearColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
	assert(g_MainView->GetCamera() != nullptr);
	g_MainView->GetCamera()->SetProjection(g_MainProjection);
	
	Graphics::DefaultRenderTarget * MainViewRenderTarget(new Graphics::DefaultRenderTarget());
	
	g_MainView->SetRenderTarget(MainViewRenderTarget);
	g_GraphicsEngine->AddView(g_MainView);
	
	// set first timeout for widget collector, it will reinsert itself on callback
	g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 5.0f, CollectWidgetsRecurrent);
	// user interface
	g_UserInterface = new UI::UserInterface();
	g_UserInterface->GetRootWidget()->SetSize(Vector2f(g_Width, g_Height));
	g_UserInterface->GetRootWidget()->ConnectKeyCallback(MainViewKeyEvent);
	g_UserInterface->GetRootWidget()->ConnectMouseButtonCallback(MainViewMouseButtonEvent);
	g_UserInterface->GetRootWidget()->ConnectMouseMoveCallback(MainViewMouseMove);
	// resize here after the graphics have been set up
	Resize();
	g_ScenarioManager = new ScenarioManager();
	g_MessageDispatcher = new MessageDispatcher();
	g_ObjectFactory = new ObjectFactory();
	g_GeneratorClassManager = new ClassManager< GeneratorClass >();
	g_ShipClassManager = new ClassManager< ShipClass >();
	g_SlotClassManager = new ClassManager< SlotClass >();
	g_AssetClassManager = new ClassManager< AssetClass >();
	g_BatteryClassManager = new ClassManager< BatteryClass >();
	g_CommodityClassManager = new ClassManager< CommodityClass >();
	g_SystemStatistics = new SystemStatistics();
	g_CharacterObserver = new OutputObserver();
	
	// read the data from the archive
	ON_DEBUG(std::cout << "Reading the data objects from the game archive." << std::endl);
	g_ResourceReader->ReadMeshes();
	g_ResourceReader->ReadModels();
	g_ResourceReader->ReadAssetClasses(g_AssetClassManager);
	g_ResourceReader->ReadBatteryClasses();
	g_ResourceReader->ReadCommodityClasses();
	g_ResourceReader->ReadGeneratorClasses();
	g_ResourceReader->ReadSlotClasses();
	g_ResourceReader->ReadShipClasses();
	g_ResourceReader->ReadWeaponClasses(g_ObjectFactory->GetWeaponClassManager());
	g_ResourceReader->ReadScenarios(g_ScenarioManager);
	// reading shaders and programs could be done earlier and without OpenGL, but initializing them requires OpenGL
	g_ResourceReader->ReadShadersAndPrograms(g_GraphicsEngine->GetShadingManager());
	g_GraphicsEngine->GetShadingManager()->BuildPrograms();
	// since reading the textures already creates them we have to do this after initializing OpenGL
	ON_DEBUG(std::cout << "Reading textures from game archive." << std::endl);
	g_ResourceReader->ReadTextures();
	// since initializing the font uses a texture, we have to do it after reading the textures
	ON_DEBUG(std::cout << "Initializing font." << std::endl);
	InitializeFont();
	// load the specified savegame
	if(LoadSavegameFileName.empty() == false)
	{
		ON_DEBUG(std::cout << "Loading save game file '" << LoadSavegameFileName << "'." << std::endl);
		if(LoadGameFromFileName(LoadSavegameFileName) == false)
		{
			return 1;
		}
	}
	else
	{
		assert(g_MainMenuWindow == nullptr);
		ActionOpenMainMenuWindow();
		g_MainMenuWindow->SetDestroyOnESCAPEKey(false);
	}
	
	// transform the key binding profile from the settings into a lookup table for fast execution
	LoadKeyboardLookupTable(g_Settings->GetKeyBindings());
	
	// main loop
	ON_DEBUG(std::cout << "Entering game loop." << std::endl);
	while(g_Quit == false)
	{
		ProcessEvents();
		GameFrame();
		glXSwapBuffers(g_Display, g_Window);
	}
	ON_DEBUG(std::cout << "Left game loop." << std::endl);
	// cleanup
	ON_DEBUG(std::cout << "Purging game." << std::endl);
	PurgeGame();
	ON_DEBUG(std::cout << "Deinitializing font." << std::endl);
	DeinitializeFont();
	ON_DEBUG(std::cout << "Destroying window." << std::endl);
	DestroyWindow();
	// if requested print some final debugging information
	if(g_DumpEndReport == true)
	{
		XMLStream Out(std::cout);
		
		Object::Dump(Out);
		std::cout << std::endl;
	}
	if(g_ScenarioManager != nullptr)
	{
		delete g_ScenarioManager;
		g_ScenarioManager = nullptr;
	}
	// destroying global variables in reverse order
	delete g_CharacterObserver;
	delete g_SystemStatistics;
	delete g_CommodityClassManager;
	delete g_BatteryClassManager;
	delete g_AssetClassManager;
	delete g_SlotClassManager;
	delete g_ShipClassManager;
	delete g_GeneratorClassManager;
	delete g_ObjectFactory;
	delete g_MessageDispatcher;
	delete g_ScenarioManager;
	delete g_UserInterface;
	// main view
	assert(g_MainView != nullptr);
	assert(g_MainView->GetRenderTarget() == MainViewRenderTarget);
	assert(MainViewRenderTarget != nullptr);
	g_MainView->SetRenderTarget(nullptr);
	delete MainViewRenderTarget;
	assert(g_MainView->GetCamera() != nullptr);
	assert(g_MainView->GetCamera()->GetProjection() == g_MainProjection);
	assert(g_MainProjection != nullptr);
	g_MainView->GetCamera()->SetProjection(nullptr);
	delete g_MainProjection;
	g_GraphicsEngine->RemoveView(g_MainView);
	delete g_MainView;
	// ui view
	assert(g_UIView != nullptr);
	assert(g_UIView->GetCamera() != nullptr);
	assert(g_UIView->GetCamera()->GetProjection() == g_UIProjection);
	assert(g_UIProjection != nullptr);
	g_UIView->GetCamera()->SetProjection(nullptr);
	delete g_UIProjection;
	assert(g_UIView->GetScene() == UIScene);
	assert(UIScene != nullptr);
	g_UIView->SetScene(nullptr);
	delete UIScene;
	g_GraphicsEngine->RemoveView(g_UIView);
	delete g_UIView;
	delete g_GraphicsEngine;
	// these are the first things hat got created
	delete g_RealTimeTimeoutNotifications;
	delete g_GameTimeTimeoutNotifications;
	delete g_Settings;
	delete g_ResourceReader;
	CollectWidgets();
	
	return 0;
}
