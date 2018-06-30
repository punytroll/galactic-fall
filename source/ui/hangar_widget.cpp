/**
 * galactic-fall
 * Copyright (C) 2014-2018  Hagen Möbius
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

#include <algorithm>

#include <expressions/operators.h>

#include <string_cast/string_cast.h>

#include "../battery.h"
#include "../character.h"
#include "../globals.h"
#include "../graphics/camera.h"
#include "../graphics/color_rgbo.h"
#include "../graphics/engine.h"
#include "../graphics/light.h"
#include "../graphics/node.h"
#include "../graphics/perspective_projection.h"
#include "../graphics/scene.h"
#include "../graphics/texture.h"
#include "../graphics/texture_render_target.h"
#include "../graphics/view.h"
#include "../hangar.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../object_aspect_physical.h"
#include "../object_aspect_position.h"
#include "../planet.h"
#include "../ship.h"
#include "../visualization.h"
#include "../visualizations.h"
#include "border.h"
#include "event.h"
#include "hangar_widget.h"
#include "label.h"
#include "list_box_item.h"
#include "load_ship_window.h"
#include "mouse_button_event.h"
#include "outfit_ship_dialog.h"
#include "progress_bar.h"
#include "scroll_box.h"
#include "sub_widget_event.h"
#include "text_button.h"
#include "view_display.h"

using namespace Expressions::Operators;

namespace UI
{
	class ShipDisplay : public UI::ViewDisplay
	{
	public:
		ShipDisplay(Ship * Ship) :
			_Ship(Ship),
			_Visualization(nullptr)
		{
			assert(_Ship != nullptr);
			ConnectDestroyingCallback(std::bind(&UI::ShipDisplay::_OnDestroying, this, std::placeholders::_1));
			ConnectUpdatingCallback(std::bind(&UI::ShipDisplay::_OnUpdating, this, std::placeholders::_1, std::placeholders::_2));
			ConnectHeightChangedCallback(std::bind(&UI::ShipDisplay::_OnSizeChanged, this, std::placeholders::_1));
			ConnectWidthChangedCallback(std::bind(&UI::ShipDisplay::_OnSizeChanged, this, std::placeholders::_1));
			_ShipDestroyingConnection = _Ship->ConnectDestroyingCallback(std::bind(&UI::ShipDisplay::_OnShipDestroying, this));
		}
	private:
		void _Clear(void)
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
		
		void _OnDestroying(UI::Event & DestroyingEvent)
		{
			if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
			{
				_ShipDestroyingConnection.Disconnect();
				_Clear();
				assert(_Visualization == nullptr);
			}
		}
		
		void _OnDestroyInScene(Graphics::Node * Node)
		{
			if((_Visualization != nullptr) && (Node == _Visualization->GetGraphics()))
			{
				_Visualization = nullptr;
			}
			InvalidateVisualizationReference(Node);
			delete Node;
		}
		
		void _OnUpdating(float RealTimeSeconds, float GameTimeSeconds)
		{
			if(IsHovered() == true)
			{
				assert(_Visualization != nullptr);
				assert(_Visualization->GetGraphics() != nullptr);
				_Visualization->GetGraphics()->SetOrientation(_Visualization->GetGraphics()->GetOrientation().RotatedZ(-1.0f * RealTimeSeconds));
			}
		}
		
		void _OnShipDestroying(void)
		{
			_Clear();
			_Ship = nullptr;
		}
		
		void _OnSizeChanged(UI::Event & SizeChangedEvent)
		{
			if(SizeChangedEvent.GetPhase() == UI::Event::Phase::Target)
			{
				_Clear();
				_Setup();
			}
		}
		
		void _Setup(void)
		{
			if((GetWidth() > 0.0f) && (GetHeight() > 0.0f))
			{
				assert(_Ship != nullptr);
				assert(_Ship->GetAspectPhysical() != nullptr);
				
				auto PerspectiveProjection{new Graphics::PerspectiveProjection{}};
				float RadialSize{_Ship->GetAspectPhysical()->GetRadialSize()};
				float ExtendedRadialSize{(5.0f / 4.0f) * RadialSize};
				
				PerspectiveProjection->SetFieldOfViewY(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)) * 2.0f);
				PerspectiveProjection->SetAspect(GetWidth() / GetHeight());
				PerspectiveProjection->SetNearClippingPlane(1.0f);
				PerspectiveProjection->SetFarClippingPlane(1000.0f);
				
				auto View{new Graphics::View{}};
				
				g_GraphicsEngine->AddView(View);
				View->SetClearColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 0.0f));
				assert(View->GetCamera() != nullptr);
				View->GetCamera()->SetProjection(PerspectiveProjection);
				View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateTranslation(Vector3f::CreateFromComponents(0.0f, -2.5f, 1.4f).Normalize() * 4.0f * RadialSize).RotateX(1.05f));
				
				auto Scene{new Graphics::Scene{}};
				
				Scene->SetDestroyCallback(std::bind(&UI::ShipDisplay::_OnDestroyInScene, this, std::placeholders::_1));
				Scene->ActivateLight();
				assert(Scene->GetLight() != nullptr);
				Scene->GetLight()->SetType(Graphics::Light::Type::Directional);
				Scene->GetLight()->SetDirection(Vector3f::CreateFromComponents(-15.0f, 10.0f, -20.0f));
				Scene->GetLight()->SetColor(Graphics::ColorRGB(1.0f, 1.0f, 1.0f));
				View->SetScene(Scene);
				
				auto Texture{new Graphics::Texture{}};
				
				Texture->Create(GetWidth(), GetHeight(), 1);
				
				auto RenderTarget{new Graphics::TextureRenderTarget{}};
				
				RenderTarget->SetTexture(Texture);
				View->SetRenderTarget(RenderTarget);
				
				auto RootNode{new Graphics::Node{}};
				
				RootNode->SetClearColorBuffer(true);
				RootNode->SetClearDepthBuffer(true);
				RootNode->SetUseDepthTest(true);
				Scene->SetRootNode(RootNode);
				assert(_Visualization == nullptr);
				_Visualization = VisualizeObject(_Ship, RootNode);
				_Visualization->GetGraphics()->SetOrientation(Quaternion::CreateAsRotationZ(-2.4f));
				_Visualization->SetUpdateOrientation(false);
				_Visualization->SetUpdatePosition(false);
				SetView(View);
			}
		}
		
		Ship * _Ship;
		Connection _ShipDestroyingConnection;
		Visualization * _Visualization;
	};
	
	class ShipListItem : public UI::ListBoxItem
	{
	public:
		ShipListItem(Ship * Ship) :
			UI::ListBoxItem(),
			_Border(nullptr),
			_Ship(Ship)
		{
			assert(_Ship != nullptr);
			_ShipDestroyingConnection = _Ship->ConnectDestroyingCallback(std::bind(&UI::ShipListItem::_OnShipDestroying, this));
			ConnectDestroyingCallback(std::bind(&UI::ShipListItem::_OnDestroying, this, std::placeholders::_1));
			
			// create components
			auto ShipDisplay{new UI::ShipDisplay{Ship}};
			
			// initialize components
			ShipDisplay->SetLeft(1.0_c);
			ShipDisplay->SetTop(1.0_c);
			ShipDisplay->SetWidth(width(this) - 2.0_c);
			ShipDisplay->SetHeight(height(this) - 2.0_c);
			// add components
			AddSubWidget(ShipDisplay);
		}
		
		Ship * GetShip(void)
		{
			return _Ship;
		}
		
		void ShowBorder(bool Show)
		{
			if(Show == true)
			{
				if(_Border == nullptr)
				{
					_Border = new UI::Border{};
					_Border->SetLeft(0.0_c);
					_Border->SetTop(0.0_c);
					_Border->SetWidth(width(this));
					_Border->SetHeight(height(this));
					_Border->SetLineWidth(1.0f);
					_Border->SetColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 1.0f));
					AddSubWidget(_Border);
					LowerSubWidget(_Border);
				}
			}
			else
			{
				if(_Border != nullptr)
				{
					_Border->Destroy();
					_Border = nullptr;
				}
			}
		}
	private:
		// event handlers
		void _OnDestroying(UI::Event & DestroyingEvent)
		{
			if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
			{
				assert(_Ship != nullptr);
				_ShipDestroyingConnection.Disconnect();
				_Ship = nullptr;
			}
		}
		
		void _OnShipDestroying(void)
		{
			_Ship = nullptr;
			Destroy();
		}
		
		UI::Border * _Border;
		Ship * _Ship;
		Connection _ShipDestroyingConnection;
	};
}

UI::HangarWidget::HangarWidget(Planet * Planet, Character * Character) :
	_Character(Character),
	_LoadShipWindow(nullptr),
	_OutfitShipDialog(nullptr),
	_Planet(Planet),
	_SelectedShipListItem(nullptr)
{
	ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnDestroying, this, std::placeholders::_1));
	assert(_Character != nullptr);
	_CharacterDestroyingConnection = _Character->ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnCharacterDestroying, this));
	assert(_Planet != nullptr);
	_PlanetDestroyingConnection = _Planet->ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnPlanetDestroying, this));
	
	// create components
	auto BoardButton{new UI::TextButton{}};
	auto EnergyStateProgressBar{new UI::ProgressBar{}};
	auto FuelStateProgressBar{new UI::ProgressBar{}};
	auto HullStateProgressBar{new UI::ProgressBar{}};
	auto LoadButton{new UI::TextButton{}};
	auto OutfitButton{new UI::TextButton{}};
	auto RechargeButton{new UI::TextButton{}};
	auto RefuelButton{new UI::TextButton{}};
	auto RepairButton{new UI::TextButton{}};
	auto TakeOffButton{new UI::TextButton{}};
	
	_ShipScrollBox = new UI::ScrollBox{};
	//initialize components
	_ShipScrollBox->SetLeft(0.0_c);
	_ShipScrollBox->SetTop(0.0_c);
	_ShipScrollBox->SetWidth(width(this));
	_ShipScrollBox->SetHeight(130.0_c);
	_ShipScrollBox->SetVerticalScrollBarVisible(false);
	_ShipScrollBox->GetContent()->ConnectSubWidgetAddedCallback(std::bind(&UI::HangarWidget::_OnShipScrollBoxSubWidgetAdded, this, std::placeholders::_1));
	_ShipScrollBox->GetContent()->ConnectSubWidgetRemovedCallback(std::bind(&UI::HangarWidget::_OnShipScrollBoxSubWidgetRemoved, this, std::placeholders::_1));
	_ShipScrollBox->GetContent()->SetHeight(constant(_ShipScrollBox->GetView()->GetHeight()));
	
	auto Hangar{_Planet->GetHangar(_Character)};
	
	assert(Hangar != nullptr);
	assert(Hangar->GetAspectObjectContainer() != nullptr);
	_HangarContentAddedConnection = Hangar->GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&UI::HangarWidget::_OnHangarContentAdded, this, std::placeholders::_1));
	_HangarContentRemovedConnection = Hangar->GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&UI::HangarWidget::_OnHangarContentRemoved, this, std::placeholders::_1));
	for(auto Content : Hangar->GetAspectObjectContainer()->GetContent())
	{
		if(Content->GetTypeIdentifier() == "ship")
		{
			auto TheShip{dynamic_cast< Ship * >(Content)};
			
			assert(TheShip != nullptr);
			
			auto NewShipListItem{new UI::ShipListItem{TheShip}};
			
			_ShipScrollBox->GetContent()->AddSubWidget(NewShipListItem);
			if(TheShip == _Character->GetShip())
			{
				_SelectedShipListItem = NewShipListItem;
			}
		}
	}
	if(_SelectedShipListItem != nullptr)
	{
		_SelectedShipListItem->SetSelected(true);
	}
	BoardButton->SetLeft(0.0_c);
	BoardButton->SetTop(140.0_c);
	BoardButton->SetWidth(180.0_c);
	BoardButton->SetHeight(20.0_c);
	BoardButton->SetText("Board");
	BoardButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnBoardButtonClicked, this));
	BoardButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnBoardButtonUpdating, this, BoardButton, std::placeholders::_1, std::placeholders::_2));
	LoadButton->SetLeft(0.0_c);
	LoadButton->SetTop(170.0_c);
	LoadButton->SetWidth(180.0_c);
	LoadButton->SetHeight(20.0_c);
	LoadButton->SetText("Load");
	LoadButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnLoadButtonClicked, this));
	LoadButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnLoadButtonUpdating, this, LoadButton, std::placeholders::_1, std::placeholders::_2));
	OutfitButton->SetLeft(0.0_c);
	OutfitButton->SetTop(200.0_c);
	OutfitButton->SetWidth(180.0_c);
	OutfitButton->SetHeight(20.0_c);
	OutfitButton->SetText("Outfit");
	OutfitButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnOutfitButtonClicked, this));
	OutfitButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnOutfitButtonUpdating, this, OutfitButton, std::placeholders::_1, std::placeholders::_2));
	RepairButton->SetLeft(0.0_c);
	RepairButton->SetTop(230.0_c);
	RepairButton->SetWidth(180.0_c);
	RepairButton->SetHeight(20.0_c);
	RepairButton->SetText("Repair");
	RepairButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnRepairButtonClicked, this));
	RepairButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnRepairButtonUpdating, this, RepairButton, std::placeholders::_1, std::placeholders::_2));
	HullStateProgressBar->SetLeft(190.0_c);
	HullStateProgressBar->SetTop(230.0_c);
	HullStateProgressBar->SetWidth(width(this) - left(HullStateProgressBar));
	HullStateProgressBar->SetHeight(20.0_c);
	HullStateProgressBar->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnHullStateProgressBarUpdating, this, HullStateProgressBar, std::placeholders::_1, std::placeholders::_2));
	RechargeButton->SetLeft(0.0_c);
	RechargeButton->SetTop(260.0_c);
	RechargeButton->SetWidth(180.0_c);
	RechargeButton->SetHeight(20.0_c);
	RechargeButton->SetText("Recharge");
	RechargeButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnRechargeButtonClicked, this));
	RechargeButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnRechargeButtonUpdating, this, RechargeButton, std::placeholders::_1, std::placeholders::_2));
	EnergyStateProgressBar->SetLeft(190.0_c);
	EnergyStateProgressBar->SetTop(260.0_c);
	EnergyStateProgressBar->SetWidth(width(this) - left(EnergyStateProgressBar));
	EnergyStateProgressBar->SetHeight(20.0_c);
	EnergyStateProgressBar->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnEnergyStateProgressBarUpdating, this, EnergyStateProgressBar, std::placeholders::_1, std::placeholders::_2));
	RefuelButton->SetLeft(0.0_c);
	RefuelButton->SetTop(290.0_c);
	RefuelButton->SetWidth(180.0_c);
	RefuelButton->SetHeight(20.0_c);
	RefuelButton->SetText("Refuel");
	RefuelButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnRefuelButtonClicked, this));
	RefuelButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnRefuelButtonUpdating, this, RefuelButton, std::placeholders::_1, std::placeholders::_2));
	FuelStateProgressBar->SetLeft(190.0_c);
	FuelStateProgressBar->SetTop(290.0_c);
	FuelStateProgressBar->SetWidth(width(this) - left(FuelStateProgressBar));
	FuelStateProgressBar->SetHeight(20.0_c);
	FuelStateProgressBar->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnFuelStateProgressBarUpdating, this, FuelStateProgressBar, std::placeholders::_1, std::placeholders::_2));
	TakeOffButton->SetLeft(0.0_c);
	TakeOffButton->SetTop(320.0_c);
	TakeOffButton->SetWidth(180.0_c);
	TakeOffButton->SetHeight(20.0_c);
	TakeOffButton->SetText("Take Off");
	TakeOffButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnTakeOffButtonClicked, this));
	TakeOffButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnTakeOffButtonUpdating, this, TakeOffButton, std::placeholders::_1, std::placeholders::_2));
	// add components
	AddSubWidget(BoardButton);
	AddSubWidget(EnergyStateProgressBar);
	AddSubWidget(FuelStateProgressBar);
	AddSubWidget(HullStateProgressBar);
	AddSubWidget(LoadButton);
	AddSubWidget(OutfitButton);
	AddSubWidget(RechargeButton);
	AddSubWidget(RefuelButton);
	AddSubWidget(RepairButton);
	AddSubWidget(_ShipScrollBox);
	AddSubWidget(TakeOffButton);
}

void UI::HangarWidget::_OnBoardButtonClicked(void)
{
	assert(_Character != nullptr);
	assert(_SelectedShipListItem != nullptr);
	assert(_SelectedShipListItem->GetShip() != nullptr);
	assert(_Character->GetShip() != _SelectedShipListItem->GetShip());
	if(_Character->GetContainer() != nullptr)
	{
		assert(_Character->GetContainer()->GetAspectObjectContainer() != nullptr);
		_Character->GetContainer()->GetAspectObjectContainer()->RemoveContent(_Character);
	}
	assert(_SelectedShipListItem->GetShip()->GetAspectObjectContainer() != nullptr);
	_SelectedShipListItem->GetShip()->GetAspectObjectContainer()->AddContent(_Character);
}

void UI::HangarWidget::_OnBoardButtonUpdating(UI::Button * BoardButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(BoardButton != nullptr);
	if(_SelectedShipListItem != nullptr)
	{
		assert(_SelectedShipListItem->GetShip() != nullptr);
	}
	assert(_Character != nullptr);
	BoardButton->SetEnabled((_SelectedShipListItem != nullptr) && (_SelectedShipListItem->GetShip() != _Character->GetShip()));
}

void UI::HangarWidget::_OnCharacterDestroying(void)
{
	assert(false);
}

void UI::HangarWidget::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if(_HangarContentAddedConnection.IsValid() == true)
		{
			_HangarContentAddedConnection.Disconnect();
		}
		if(_HangarContentRemovedConnection.IsValid() == true)
		{
			_HangarContentRemovedConnection.Disconnect();
		}
		assert(_Character != nullptr);
		_CharacterDestroyingConnection.Disconnect();
		_Character = nullptr;
		assert(_Planet != nullptr);
		_PlanetDestroyingConnection.Disconnect();
		_Planet = nullptr;
		if(_OutfitShipDialog != nullptr)
		{
			_OutfitShipDialog->Destroy();
			assert(_OutfitShipDialog == nullptr);
		}
		if(_LoadShipWindow != nullptr)
		{
			_LoadShipWindow->Destroy();
			assert(_OutfitShipDialog == nullptr);
		}
	}
}

void UI::HangarWidget::_OnEnergyStateProgressBarUpdating(UI::ProgressBar * EnergyStateProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(EnergyStateProgressBar != nullptr);
	
	std::string Text("n/a");
	float EnergyPercentage{0.0f};
	
	if(_SelectedShipListItem != nullptr)
	{
		assert(_SelectedShipListItem->GetShip() != nullptr);
		if(_SelectedShipListItem->GetShip()->GetBattery() != nullptr)
		{
			Text = to_string_cast(_SelectedShipListItem->GetShip()->GetBattery()->GetEnergy(), 2);
			EnergyPercentage = _SelectedShipListItem->GetShip()->GetBattery()->GetEnergy() / _SelectedShipListItem->GetShip()->GetBattery()->GetEnergyCapacity();
		}
	}
	EnergyStateProgressBar->SetText(Text);
	EnergyStateProgressBar->SetFillLevel(EnergyPercentage);
	EnergyStateProgressBar->SetColor(Graphics::ColorRGBO(1.0f - EnergyPercentage, EnergyPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnFuelStateProgressBarUpdating(UI::ProgressBar * FuelStateProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(FuelStateProgressBar != nullptr);
	
	float FuelPercentage{0.0f};
	
	if(_SelectedShipListItem != nullptr)
	{
		assert(_SelectedShipListItem->GetShip() != nullptr);
		FuelStateProgressBar->SetText(to_string_cast(_SelectedShipListItem->GetShip()->GetFuel(), 2));
		FuelPercentage = _SelectedShipListItem->GetShip()->GetFuel() / _SelectedShipListItem->GetShip()->GetFuelCapacity();
	}
	else
	{
		FuelStateProgressBar->SetText("n/a");
	}
	FuelStateProgressBar->SetFillLevel(FuelPercentage);
	FuelStateProgressBar->SetColor(Graphics::ColorRGBO(1.0f - FuelPercentage, FuelPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnHangarContentAdded(Object * Content)
{
	if(Content->GetTypeIdentifier() == "ship")
	{
		auto TheShip{dynamic_cast< Ship * >(Content)};
		
		assert(TheShip != nullptr);
		assert(_ShipScrollBox != nullptr);
		assert(_ShipScrollBox->GetContent() != nullptr);
		
		_ShipScrollBox->GetContent()->AddSubWidget(new ShipListItem{TheShip});
	}
}

void UI::HangarWidget::_OnHangarContentRemoved(Object * Content)
{
	if(Content->GetTypeIdentifier() == "ship")
	{
		auto TheShip{dynamic_cast< Ship * >(Content)};
		
		assert(TheShip != nullptr);
		assert(_ShipScrollBox != nullptr);
		assert(_ShipScrollBox->GetContent() != nullptr);
		
		auto ShipWidgetIterator(std::find_if(_ShipScrollBox->GetContent()->GetSubWidgets().begin(), _ShipScrollBox->GetContent()->GetSubWidgets().end(), [TheShip](UI::Widget * Widget) { return dynamic_cast< UI::ShipListItem * >(Widget)->GetShip() == TheShip; }));
		
		if(ShipWidgetIterator != _ShipScrollBox->GetContent()->GetSubWidgets().end())
		{
			(*ShipWidgetIterator)->Destroy();
		}
	}
}

void UI::HangarWidget::_OnHullStateProgressBarUpdating(UI::ProgressBar * HullStateProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(HullStateProgressBar != nullptr);
	
	float HullPercentage{0.0f};
	
	if(_SelectedShipListItem != nullptr)
	{
		assert(_SelectedShipListItem->GetShip() != nullptr);
		HullStateProgressBar->SetText(to_string_cast(_SelectedShipListItem->GetShip()->GetHull(), 2));
		HullPercentage = _SelectedShipListItem->GetShip()->GetHull() / _SelectedShipListItem->GetShip()->GetHullCapacity();
	}
	else
	{
		HullStateProgressBar->SetText("n/a");
	}
	HullStateProgressBar->SetFillLevel(HullPercentage);
	HullStateProgressBar->SetColor(Graphics::ColorRGBO(1.0f - HullPercentage, HullPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnLoadButtonClicked(void)
{
	if(_LoadShipWindow == nullptr)
	{
		assert(_Planet != nullptr);
		assert(_SelectedShipListItem != nullptr);
		assert(_SelectedShipListItem->GetShip() != nullptr);
		_LoadShipWindow = new UI::LoadShipWindow{_Planet->GetHangar(_Character), _SelectedShipListItem->GetShip()};
		_LoadShipWindow->SetLeft(100.0_c);
		_LoadShipWindow->SetTop(100.0_c);
		_LoadShipWindow->SetWidth(600.0_c);
		_LoadShipWindow->SetHeight(400.0_c);
		_LoadShipWindow->ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnLoadShipWindowDestroying, this, std::placeholders::_1));
		GetRootWidget()->AddSubWidget(_LoadShipWindow);
		_LoadShipWindow->GrabKeyFocus();
	}
	else
	{
		GetRootWidget()->RaiseSubWidget(_LoadShipWindow);
	}
}

void UI::HangarWidget::_OnLoadButtonUpdating(UI::Button * LoadButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(LoadButton != nullptr);
	LoadButton->SetEnabled(_SelectedShipListItem != nullptr);
}

void UI::HangarWidget::_OnLoadShipWindowDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_LoadShipWindow = nullptr;
	}
}

void UI::HangarWidget::_OnOutfitButtonClicked(void)
{
	if(_OutfitShipDialog == nullptr)
	{
		assert(_SelectedShipListItem != nullptr);
		assert(_SelectedShipListItem->GetShip() != nullptr);
		_OutfitShipDialog = new UI::OutfitShipDialog(_SelectedShipListItem->GetShip());
		_OutfitShipDialog->SetLeft(70.0_c);
		_OutfitShipDialog->SetTop(280.0_c);
		_OutfitShipDialog->SetWidth(600.0_c);
		_OutfitShipDialog->SetHeight(400.0_c);
		_OutfitShipDialog->ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnOutfitShipDialogDestroying, this, std::placeholders::_1));
		GetRootWidget()->AddSubWidget(_OutfitShipDialog);
		_OutfitShipDialog->GrabKeyFocus();
	}
	else
	{
		GetRootWidget()->RaiseSubWidget(_OutfitShipDialog);
	}
}

void UI::HangarWidget::_OnOutfitButtonUpdating(UI::Button * OutfitButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(OutfitButton != nullptr);
	OutfitButton->SetEnabled(_SelectedShipListItem != nullptr);
}

void UI::HangarWidget::_OnOutfitShipDialogDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_OutfitShipDialog = nullptr;
	}
}

void UI::HangarWidget::_OnPlanetDestroying(void)
{
	assert(false);
}

void UI::HangarWidget::_OnRechargeButtonClicked(void)
{
	assert(_Planet != nullptr);
	assert(_SelectedShipListItem != nullptr);
	assert(_SelectedShipListItem->GetShip() != nullptr);
	_Planet->Recharge(_SelectedShipListItem->GetShip(), _Character);
}

void UI::HangarWidget::_OnRechargeButtonUpdating(UI::Button * RechargeButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Planet != nullptr);
	if(_SelectedShipListItem != nullptr)
	{
		assert(_SelectedShipListItem->GetShip() != nullptr);
	}
	RechargeButton->SetEnabled((_Planet->GetOffersRecharging() == true) && (_SelectedShipListItem != nullptr) && (_SelectedShipListItem->GetShip()->GetBattery() != nullptr) && (_SelectedShipListItem->GetShip()->GetBattery()->GetEnergy() < _SelectedShipListItem->GetShip()->GetBattery()->GetEnergyCapacity()));
}

void UI::HangarWidget::_OnRefuelButtonClicked(void)
{
	assert(_Planet != nullptr);
	assert(_SelectedShipListItem != nullptr);
	assert(_SelectedShipListItem->GetShip() != nullptr);
	_Planet->Refuel(_SelectedShipListItem->GetShip(), _Character);
}

void UI::HangarWidget::_OnRefuelButtonUpdating(UI::Button * RefuelButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Planet != nullptr);
	
	auto FuelPlanetAssets{_Planet->GetPlanetAssets("commodity", "fuel")};
	
	if(_SelectedShipListItem != nullptr)
	{
		assert(_SelectedShipListItem->GetShip() != nullptr);
	}
	RefuelButton->SetEnabled((FuelPlanetAssets != nullptr) && (_SelectedShipListItem != nullptr) && (_SelectedShipListItem->GetShip()->GetFuel() < _SelectedShipListItem->GetShip()->GetFuelCapacity()));
}

void UI::HangarWidget::_OnRepairButtonClicked(void)
{
	assert(_Planet != nullptr);
	assert(_SelectedShipListItem != nullptr);
	assert(_SelectedShipListItem->GetShip() != nullptr);
	_Planet->Repair(_SelectedShipListItem->GetShip(), _Character);
}

void UI::HangarWidget::_OnRepairButtonUpdating(UI::Button * RepairButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Planet != nullptr);
	if(_SelectedShipListItem != nullptr)
	{
		assert(_SelectedShipListItem->GetShip() != nullptr);
	}
	RepairButton->SetEnabled((_Planet->GetOffersRepairing() == true) && (_SelectedShipListItem != nullptr) && (_SelectedShipListItem->GetShip()->GetHull() < _SelectedShipListItem->GetShip()->GetHullCapacity()));
}

void UI::HangarWidget::_OnShipListItemMouseButton(UI::MouseButtonEvent & MouseButtonEvent, UI::ShipListItem * ShipListItem)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedShipListItem != nullptr)
		{
			_SelectedShipListItem->SetSelected(false);
		}
		_SelectedShipListItem = ShipListItem;
		_SelectedShipListItem->SetSelected(true);
	}
}

void UI::HangarWidget::_OnShipListItemUpdating(float RealTimeSeconds, float GameTimeSeconds, UI::ShipListItem * ShipListItem)
{
	assert(_Character != nullptr);
	assert(ShipListItem != nullptr);
	ShipListItem->ShowBorder(ShipListItem->GetShip() == _Character->GetShip());
}

void UI::HangarWidget::_OnShipScrollBoxSubWidgetAdded(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		auto NewItem{dynamic_cast< UI::ShipListItem * >(SubWidgetEvent.GetSubWidget())};
		
		assert(NewItem != nullptr);
		
		float Left(0.0f);
		
		for(auto OldWidget : _ShipScrollBox->GetContent()->GetSubWidgets())
		{
			if(OldWidget != NewItem)
			{
				Left = std::max(Left, OldWidget->GetRight());
			}
		}
		Left += 5.0f;
		NewItem->SetLeft(constant(Left));
		NewItem->SetTop(5.0_c);
		NewItem->SetWidth(100.0_c);
		NewItem->SetHeight(100.0_c);
		NewItem->ConnectMouseButtonCallback(std::bind(&UI::HangarWidget::_OnShipListItemMouseButton, this, std::placeholders::_1, NewItem));
		NewItem->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnShipListItemUpdating, this, std::placeholders::_1, std::placeholders::_2, NewItem));
		_ShipScrollBox->GetContent()->SetWidth(constant(NewItem->GetRight() + 5.0f));
	}
}

void UI::HangarWidget::_OnShipScrollBoxSubWidgetRemoved(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		float NextLeft;
		UI::ShipListItem * NextSelectedListItem{nullptr};
		float Left{0.0f};
		
		for(auto OldSubWidget : _ShipScrollBox->GetContent()->GetSubWidgets())
		{
			if(OldSubWidget->GetLeft() > SubWidgetEvent.GetSubWidget()->GetLeft())
			{
				if((NextSelectedListItem == nullptr) || (OldSubWidget->GetLeft() < NextLeft))
				{
					NextSelectedListItem = dynamic_cast< UI::ShipListItem * >(OldSubWidget);
					NextLeft = OldSubWidget->GetLeft();
				}
				OldSubWidget->SetLeft(constant(OldSubWidget->GetLeft() - (SubWidgetEvent.GetSubWidget()->GetWidth() + 5.0f)));
			}
			else
			{
				if((NextSelectedListItem == nullptr) || (OldSubWidget->GetLeft() > NextLeft))
				{
					NextSelectedListItem = dynamic_cast< UI::ShipListItem * >(OldSubWidget);
					NextLeft = OldSubWidget->GetLeft();
				}
			}
			Left = std::max(Left, OldSubWidget->GetRight());
		}
		if(SubWidgetEvent.GetSubWidget() == _SelectedShipListItem)
		{
			_SelectedShipListItem->SetSelected(false);
			if(NextSelectedListItem != nullptr)
			{
				NextSelectedListItem->SetSelected(true);
				_SelectedShipListItem = NextSelectedListItem;
			}
			else
			{
				_SelectedShipListItem = nullptr;
			}
		}
		_ShipScrollBox->GetContent()->SetWidth(constant(Left + 5.0f));
	}
}

void UI::HangarWidget::_OnTakeOffButtonClicked(void)
{
	assert(_SelectedShipListItem != nullptr);
	assert(_SelectedShipListItem->GetShip() != nullptr);
	assert(_Character != nullptr);
	assert(_SelectedShipListItem->GetShip() == _Character->GetShip());
	_SelectedShipListItem->GetShip()->SetTakeOff(true);
}

void UI::HangarWidget::_OnTakeOffButtonUpdating(UI::Button * TakeOffButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(TakeOffButton != nullptr);
	assert(_Character != nullptr);
	if(_SelectedShipListItem != nullptr)
	{
		assert(_SelectedShipListItem->GetShip() != nullptr);
	}
	TakeOffButton->SetEnabled((_SelectedShipListItem != nullptr) && (_SelectedShipListItem->GetShip() == _Character->GetShip()));
}
