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

#include <map>

#include "../character.h"
#include "../galaxy.h"
#include "../globals.h"
#include "../graphics/callback_node.h"
#include "../graphics/camera.h"
#include "../graphics/color_rgbo.h"
#include "../graphics/drawing.h"
#include "../graphics/engine.h"
#include "../graphics/gl.h"
#include "../graphics/orthogonal_2d_projection.h"
#include "../graphics/render_context.h"
#include "../graphics/scene.h"
#include "../graphics/style.h"
#include "../graphics/texture.h"
#include "../graphics/texture_render_target.h"
#include "../graphics/view.h"
#include "../map_knowledge.h"
#include "../object_aspect_name.h"
#include "../object_aspect_position.h"
#include "../system.h"
#include "mouse_button_event.h"
#include "mouse_move_event.h"
#include "star_map_display.h"
#include "user_interface.h"

UI::StarMapDisplay::StarMapDisplay(Widget * SupWidget, Character * Character) :
	UI::ViewDisplay(SupWidget),
	_Character(Character),
	_CameraWorldPosition(true),
	_Scale(5.0f),
	_SelectedSystem(nullptr)
{
	assert(_Character != nullptr);
	_CharacterDestroyingConnection = _Character->ConnectDestroyingCallback(std::bind(&UI::StarMapDisplay::_OnCharacterDestroying, this));
	
	auto System{_Character->GetSystem()};
	
	assert(System != nullptr);
	assert(System->GetAspectPosition() != nullptr);
	_CameraWorldPosition[0] += System->GetAspectPosition()->GetPosition()[0];
	_CameraWorldPosition[1] += System->GetAspectPosition()->GetPosition()[1];
	SetSize(Vector2f(100.0f, 100.0f));
	ConnectDestroyingCallback(std::bind(&UI::StarMapDisplay::_OnDestroying, this, std::placeholders::_1));
	ConnectMouseButtonCallback(std::bind(&UI::StarMapDisplay::_OnMouseButton, this, std::placeholders::_1));
	ConnectMouseMoveCallback(std::bind(&UI::StarMapDisplay::_OnMouseMove, this, std::placeholders::_1));
	ConnectSizeChangedCallback(std::bind(&UI::StarMapDisplay::_OnSizeChanged, this, std::placeholders::_1));
	_SetupView();
}

void UI::StarMapDisplay::_ClearView(void)
{
	auto OldView(GetView());
	
	if(OldView != nullptr)
	{
		SetView(nullptr);
		assert(OldView->GetScene() != nullptr);
		
		auto Scene(OldView->GetScene());
		
		OldView->SetScene(nullptr);
		delete Scene;
		
		assert(OldView->GetCamera() != nullptr);
		assert(OldView->GetCamera()->GetProjection() != nullptr);
		
		auto Projection(OldView->GetCamera()->GetProjection());
		
		OldView->GetCamera()->SetProjection(nullptr);
		delete Projection;
		
		auto TextureRenderTarget(dynamic_cast< Graphics::TextureRenderTarget * >(OldView->GetRenderTarget()));
		
		assert(TextureRenderTarget != nullptr);
		OldView->SetRenderTarget(nullptr);
		
		auto Texture(TextureRenderTarget->GetTexture());
		
		TextureRenderTarget->SetTexture(nullptr);
		delete TextureRenderTarget;
		delete Texture;
		g_GraphicsEngine->RemoveView(OldView);
		delete OldView;
	}
}

void UI::StarMapDisplay::_SetupView(void)
{
	auto OrthogonalProjection{new Graphics::Orthogonal2DProjection()};
	
	// the orthogonal pojection is designed to place (0.0, 0.0) in the middle of the widget, scale appropriately and make this a game coordinate view (y axis is up)
	OrthogonalProjection->SetLeft(-GetSize()[0] / 2.0f / _Scale);
	OrthogonalProjection->SetTop(GetSize()[1] / 2.0f / _Scale);
	OrthogonalProjection->SetRight(GetSize()[0] / 2.0f / _Scale);
	OrthogonalProjection->SetBottom(-GetSize()[1] / 2.0f / _Scale);
	
	auto View{new Graphics::View()};
	
	View->SetClearColor(Graphics::ColorRGBO(0.0f, 0.0f, 0.0f, 0.0f));
	assert(View->GetCamera() != nullptr);
	View->GetCamera()->SetProjection(OrthogonalProjection);
	View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateTranslation(_CameraWorldPosition[0], _CameraWorldPosition[1], 0.0f));
	assert(g_GraphicsEngine != nullptr);
	g_GraphicsEngine->AddView(View);
	
	auto Scene{new Graphics::Scene()};
	
	Scene->SetDestroyCallback(std::bind(&UI::StarMapDisplay::_OnDestroyInScene, this, std::placeholders::_1));
	View->SetScene(Scene);
	
	auto Texture{new Graphics::Texture()};
	
	Texture->Create(GetSize()[0], GetSize()[1], 1);
	
	auto RenderTarget{new Graphics::TextureRenderTarget()};
	
	RenderTarget->SetTexture(Texture);
	View->SetRenderTarget(RenderTarget);
	
	auto RootNode{new Graphics::CallbackNode()};
	
	RootNode->SetDrawCallback(std::bind(&UI::StarMapDisplay::_OnDraw, this, std::placeholders::_1));
	RootNode->SetClearColorBuffer(true);
	RootNode->SetClearDepthBuffer(true);
	RootNode->SetUseBlending(false);
	RootNode->SetUseDepthTest(false);
	RootNode->SetUseBlending(true);
	RootNode->SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Scene->SetRootNode(RootNode);
	SetView(View);
}

void UI::StarMapDisplay::_OnDraw(Graphics::RenderContext * RenderContext)
{
	if(_Character != nullptr)
	{
		auto System{_Character->GetSystem()};
		
		assert(System != nullptr);
		assert(_Character->GetMapKnowledge() != nullptr);
		
		auto SystemSize{1.5f};
		auto & UnexploredSystems{_Character->GetMapKnowledge()->GetUnexploredSystems()};
		
		for(auto UnexploredSystem : UnexploredSystems)
		{
			auto Position{UnexploredSystem->GetAspectPosition()->GetPosition()};
			
			if(UnexploredSystem == _SelectedSystem)
			{
				GLColor3f(0.0f, 1.0f, 0.0f);
			}
			else if(UnexploredSystem == System)
			{
				GLColor3f(0.8f, 0.8f, 0.0f);
			}
			else
			{
				GLColor3f(0.5f, 0.5f, 0.5f);
			}
			GLBegin(GL_LINE_LOOP);
			GLVertex2f(Position[0] + SystemSize, Position[1]);
			GLVertex2f(Position[0] + SystemSize * 0.866f, Position[1] + SystemSize * 0.5f);
			GLVertex2f(Position[0] + SystemSize * 0.5f, Position[1] + SystemSize * 0.866f);
			GLVertex2f(Position[0], Position[1] + SystemSize);
			GLVertex2f(Position[0] - SystemSize * 0.5f, Position[1] + SystemSize * 0.866f);
			GLVertex2f(Position[0] - SystemSize * 0.866f, Position[1] + SystemSize * 0.5f);
			GLVertex2f(Position[0] - SystemSize, Position[1]);
			GLVertex2f(Position[0] - SystemSize * 0.866f, Position[1] - SystemSize * 0.5f);
			GLVertex2f(Position[0] - SystemSize * 0.5f, Position[1] - SystemSize * 0.866f);
			GLVertex2f(Position[0], Position[1] - SystemSize);
			GLVertex2f(Position[0] + SystemSize * 0.5f, Position[1] - SystemSize * 0.866f);
			GLVertex2f(Position[0] + SystemSize * 0.866f, Position[1] - SystemSize * 0.5f);
			GLEnd();
		}
		
		auto & ExploredSystems{_Character->GetMapKnowledge()->GetExploredSystems()};
		
		for(auto ExploredSystem : ExploredSystems)
		{
			assert(ExploredSystem != nullptr);
			assert(ExploredSystem->GetAspectPosition() != nullptr);
			
			auto Position{ExploredSystem->GetAspectPosition()->GetPosition()};
			auto & LinkedSystems{ExploredSystem->GetLinkedSystems()};
			
			for(auto LinkedSystem : LinkedSystems)
			{
				assert(LinkedSystem != nullptr);
				if(UnexploredSystems.find(LinkedSystem) == UnexploredSystems.end())
				{
					// system already explored so make it blue
					GLColor3f(0.0f, 0.0f, 0.8f);
				}
				else
				{
					// system still unexplored so make it grey
					GLColor3f(0.5f, 0.5f, 0.5f);
				}
				GLBegin(GL_LINES);
				GLVertex2f(Position[0], Position[1]);
				assert(LinkedSystem->GetAspectPosition() != nullptr);
				
				auto To{LinkedSystem->GetAspectPosition()->GetPosition() - ExploredSystem->GetAspectPosition()->GetPosition()};
				
				GLVertex2f(Position[0] + To[0], Position[1] + To[1]);
				GLEnd();
			}
			if(ExploredSystem == _SelectedSystem)
			{
				GLColor3f(0.0f, 1.0f, 0.0f);
			}
			else if(ExploredSystem == System)
			{
				GLColor3f(0.8f, 0.8f, 0.0f);
			}
			else
			{
				GLColor3f(0.0f, 0.0f, 0.8f);
			}
			GLBegin(GL_LINE_LOOP);
			GLVertex2f(Position[0] + SystemSize, Position[1]);
			GLVertex2f(Position[0] + SystemSize * 0.866f, Position[1] + SystemSize * 0.5f);
			GLVertex2f(Position[0] + SystemSize * 0.5f, Position[1] + SystemSize * 0.866f);
			GLVertex2f(Position[0], Position[1] + SystemSize);
			GLVertex2f(Position[0] - SystemSize * 0.5f, Position[1] + SystemSize * 0.866f);
			GLVertex2f(Position[0] - SystemSize * 0.866f, Position[1] + SystemSize * 0.5f);
			GLVertex2f(Position[0] - SystemSize, Position[1]);
			GLVertex2f(Position[0] - SystemSize * 0.866f, Position[1] - SystemSize * 0.5f);
			GLVertex2f(Position[0] - SystemSize * 0.5f, Position[1] - SystemSize * 0.866f);
			GLVertex2f(Position[0], Position[1] - SystemSize);
			GLVertex2f(Position[0] + SystemSize * 0.5f, Position[1] - SystemSize * 0.866f);
			GLVertex2f(Position[0] + SystemSize * 0.866f, Position[1] - SystemSize * 0.5f);
			GLEnd();
			assert(ExploredSystem->GetAspectName() != nullptr);
			RenderContext->SetColorRGBO(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 1.0f));
			Graphics::Drawing::DrawTextWithoutClippingRightHanded(RenderContext, Position[0] - _CameraWorldPosition[0], Position[1] - _CameraWorldPosition[1] - SystemSize * 1.1f, 12.0f / _Scale, ExploredSystem->GetAspectName()->GetName());
			RenderContext->UnsetColorRGBO();
		}
	}
}

void UI::StarMapDisplay::_OnCharacterDestroying(void)
{
	assert(_Character != nullptr);
	_Character = nullptr;
	assert(_CharacterDestroyingConnection.IsValid() == true);
	_CharacterDestroyingConnection.Disconnect();
	assert(_CharacterDestroyingConnection.IsValid() == false);
}

void UI::StarMapDisplay::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_ClearView();
		if(_Character != nullptr)
		{
			assert(_CharacterDestroyingConnection.IsValid() == true);
			_Character->DisconnectDestroyingCallback(_CharacterDestroyingConnection);
			_Character = nullptr;
		}
		assert(_CharacterDestroyingConnection.IsValid() == false);
		if(_SelectedSystem != nullptr)
		{
			assert(_SelectedSystemDestroyingConnection.IsValid() == true);
			_SelectedSystem->DisconnectDestroyingCallback(_SelectedSystemDestroyingConnection);
			_SelectedSystem = nullptr;
		}
		assert(_SelectedSystemDestroyingConnection.IsValid() == false);
	}
}

void UI::StarMapDisplay::_OnDestroyInScene(Graphics::Node * Node)
{
	delete Node;
}

void UI::StarMapDisplay::_OnMouseButton(UI::MouseButtonEvent & MouseButtonEvent)
{
	if(MouseButtonEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::WheelUp) && (MouseButtonEvent.IsDown() == true))
		{
			_Scale *= 1.1f;
			MouseButtonEvent.StopPropagation();
			_ClearView();
			_SetupView();
		}
		else if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::WheelDown) && (MouseButtonEvent.IsDown() == true))
		{
			_Scale *= 0.9f;
			MouseButtonEvent.StopPropagation();
			_ClearView();
			_SetupView();
		}
		else if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
		{
			if(_Character != nullptr)
			{
				auto ClickWorldPosition{(MouseButtonEvent.GetPosition() - GetSize() / 2.0f) / _Scale};
				
				ClickWorldPosition[1] = -ClickWorldPosition[1];
				ClickWorldPosition += _CameraWorldPosition;
				for(auto SystemPair : g_Galaxy->GetSystems())
				{
					assert(SystemPair.second != nullptr);
					assert(SystemPair.second->GetAspectPosition() != nullptr);
					
					auto SystemWorldPosition{SystemPair.second->GetAspectPosition()->GetPosition()};
					
					if((Vector2f(SystemWorldPosition[0], SystemWorldPosition[1]) - ClickWorldPosition).SquaredLength() < 2.25f)
					{
						SetSelectedSystem(SystemPair.second);
						
						break;
					}
				}
			}
		}
		else if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Right) && (MouseButtonEvent.IsDown() == true))
		{
			SetSelectedSystem(nullptr);
		}
		else if(MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Middle)
		{
			if(MouseButtonEvent.IsDown() == true)
			{
				_GrabPosition = MouseButtonEvent.GetPosition();
				g_UserInterface->SetCaptureWidget(this);
			}
			else if(MouseButtonEvent.IsUp() == true)
			{
				g_UserInterface->ReleaseCaptureWidget();
			}
		}
	}
}

void UI::StarMapDisplay::_OnMouseMove(UI::MouseMoveEvent & MouseMoveEvent)
{
	if((MouseMoveEvent.GetPhase() == UI::Event::Phase::Target) && (g_UserInterface->GetCaptureWidget() == this))
	{
		_CameraWorldPosition[0] -= (MouseMoveEvent.GetPosition()[0] - _GrabPosition[0]) / _Scale;
		_CameraWorldPosition[1] += (MouseMoveEvent.GetPosition()[1] - _GrabPosition[1]) / _Scale;
		_GrabPosition = MouseMoveEvent.GetPosition();
		_ClearView();
		_SetupView();
	}
}

void UI::StarMapDisplay::_OnSelectedSystemDestroying(void)
{
	assert(_SelectedSystem != nullptr);
	_SelectedSystem = nullptr;
	assert(_SelectedSystemDestroyingConnection.IsValid() == true);
	_SelectedSystemDestroyingConnection.Disconnect();
	assert(_SelectedSystemDestroyingConnection.IsValid() == false);
}

void UI::StarMapDisplay::SetSelectedSystem(System * SelectedSystem)
{
	if(_SelectedSystem != nullptr)
	{
		_SelectedSystem = nullptr;
		assert(_SelectedSystemDestroyingConnection.IsValid() == true);
		_SelectedSystemDestroyingConnection.Disconnect();
	}
	assert(_SelectedSystemDestroyingConnection.IsValid() == false);
	if(SelectedSystem != nullptr)
	{
		_SelectedSystem = SelectedSystem;
		_SelectedSystemDestroyingConnection = _SelectedSystem->ConnectDestroyingCallback(std::bind(&UI::StarMapDisplay::_OnSelectedSystemDestroying, this));
	}
}

void UI::StarMapDisplay::_OnSizeChanged(UI::Event & SizeChangedEvent)
{
	if(SizeChangedEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_ClearView();
		_SetupView();
	}
}
