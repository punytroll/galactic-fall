/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
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

#include <string_cast/string_cast.h>

#include "../asset_class.h"
#include "../battery.h"
#include "../character.h"
#include "../color.h"
#include "../globals.h"
#include "../graphics/camera.h"
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

namespace UI
{
	class ShipDisplay : public UI::ViewDisplay
	{
	public:
		ShipDisplay(UI::Widget * SupWidget, Ship * Ship) :
			UI::ViewDisplay(SupWidget),
			_Ship(Ship),
			_Visualization(nullptr)
		{
			assert(_Ship != nullptr);
			ConnectDestroyingCallback(std::bind(&UI::ShipDisplay::_OnDestroying, this, std::placeholders::_1));
			ConnectUpdatingCallback(std::bind(&UI::ShipDisplay::_OnUpdating, this, std::placeholders::_1, std::placeholders::_2));
			ConnectSizeChangedCallback(std::bind(&UI::ShipDisplay::_OnSizeChanged, this, std::placeholders::_1));
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
			assert(_Ship != nullptr);
			assert(_Ship->GetAspectPhysical() != nullptr);
			
			Graphics::PerspectiveProjection * PerspectiveProjection(new Graphics::PerspectiveProjection());
			float RadialSize(_Ship->GetAspectPhysical()->GetRadialSize());
			float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
			
			PerspectiveProjection->SetFieldOfViewY(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)) * 2.0f);
			PerspectiveProjection->SetAspect(GetSize()[0] / GetSize()[1]);
			PerspectiveProjection->SetNearClippingPlane(1.0f);
			PerspectiveProjection->SetFarClippingPlane(1000.0f);
			
			auto View(new Graphics::View());
			
			g_GraphicsEngine->AddView(View);
			View->SetClearColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
			assert(View->GetCamera() != nullptr);
			View->GetCamera()->SetProjection(PerspectiveProjection);
			View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateFromTranslationVector(Vector3f(0.0f, -2.5f, 1.4f).Normalize() * 4.0f * RadialSize).RotateX(1.05f));
			
			auto Scene(new Graphics::Scene());
			
			Scene->SetDestroyCallback(std::bind(&UI::ShipDisplay::_OnDestroyInScene, this, std::placeholders::_1));
			Scene->ActivateLight();
			assert(Scene->GetLight() != nullptr);
			Scene->GetLight()->SetPosition(-20.0f, -10.0f, 20.0f);
			Scene->GetLight()->SetDiffuseColor(1.0f, 1.0f, 1.0f, 0.0f);
			View->SetScene(Scene);
			
			auto Texture(new Graphics::Texture());
			
			Texture->Create(GetSize()[0], GetSize()[1], 1);
			
			auto RenderTarget(new Graphics::TextureRenderTarget());
			
			RenderTarget->SetTexture(Texture);
			View->SetRenderTarget(RenderTarget);
			
			auto RootNode(new Graphics::Node());
			
			RootNode->SetClearColorBuffer(true);
			RootNode->SetClearDepthBuffer(true);
			RootNode->SetUseLighting(true);
			RootNode->SetUseDepthTest(true);
			Scene->SetRootNode(RootNode);
			assert(_Visualization == nullptr);
			_Visualization = VisualizeObject(_Ship, RootNode);
			_Visualization->SetUpdateOrientation(false);
			_Visualization->SetUpdatePosition(false);
			SetView(View);
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
			SetSize(Vector2f(100.0f, 100.0f));
			
			auto ShipDisplay(new UI::ShipDisplay(this, Ship));
			
			ShipDisplay->SetPosition(Vector2f(1.0f, 1.0f));
			ShipDisplay->SetSize(Vector2f(GetSize()[0] - 2.0f, GetSize()[1] - 2.0f));
			ShipDisplay->SetAnchorBottom(true);
			ShipDisplay->SetAnchorRight(true);
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
					_Border = new UI::Border(this);
					_Border->SetPosition(Vector2f(0.0f, 0.0f));
					_Border->SetSize(GetSize());
					_Border->SetAnchorBottom(true);
					_Border->SetAnchorRight(true);
					_Border->SetWidth(1.0f);
					_Border->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
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

UI::HangarWidget::HangarWidget(UI::Widget * SupWidget, Planet * Planet, Character * Character) :
	UI::Widget(SupWidget),
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
	SetSize(Vector2f(600.0f, 350.0f));
	_ShipScrollBox = new UI::ScrollBox(this);
	_ShipScrollBox->SetPosition(Vector2f(0.0f, 0.0f));
	_ShipScrollBox->SetSize(Vector2f(600.0f, 130.0f));
	_ShipScrollBox->SetAnchorRight(true);
	_ShipScrollBox->SetVerticalScrollBarVisible(false);
	_ShipScrollBox->GetContent()->ConnectSubWidgetAddedCallback(std::bind(&UI::HangarWidget::_OnShipScrollBoxSubWidgetAdded, this, std::placeholders::_1));
	_ShipScrollBox->GetContent()->ConnectSubWidgetRemovedCallback(std::bind(&UI::HangarWidget::_OnShipScrollBoxSubWidgetRemoved, this, std::placeholders::_1));
	_ShipScrollBox->GetContent()->SetSize(Vector2f(600.0f, 0.0f));
	_ShipScrollBox->GetContent()->SetAnchorRight(true);
	
	auto Hangar(_Planet->GetHangar(_Character));
	
	assert(Hangar != nullptr);
	assert(Hangar->GetAspectObjectContainer() != nullptr);
	_HangarContentAddedConnection = Hangar->GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&UI::HangarWidget::_OnHangarContentAdded, this, std::placeholders::_1));
	_HangarContentRemovedConnection = Hangar->GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&UI::HangarWidget::_OnHangarContentRemoved, this, std::placeholders::_1));
	for(auto Content : Hangar->GetAspectObjectContainer()->GetContent())
	{
		if(Content->GetTypeIdentifier() == "ship")
		{
			auto TheShip(dynamic_cast< Ship * >(Content));
			
			assert(TheShip != nullptr);
			
			auto NewShipListItem(new UI::ShipListItem(TheShip));
			
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
	
	UI::Button * BoardButton(new UI::TextButton(this, "Board"));
	
	BoardButton->SetPosition(Vector2f(0.0f, 140.0f));
	BoardButton->SetSize(Vector2f(180.0f, 20.0f));
	BoardButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnBoardButtonClicked, this));
	BoardButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnBoardButtonUpdating, this, BoardButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * LoadButton(new UI::TextButton(this, "Load"));
	
	LoadButton->SetPosition(Vector2f(0.0f, 170.0f));
	LoadButton->SetSize(Vector2f(180.0f, 20.0f));
	LoadButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnLoadButtonClicked, this));
	LoadButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnLoadButtonUpdating, this, LoadButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * OutfitButton(new UI::TextButton(this, "Outfit"));
	
	OutfitButton->SetPosition(Vector2f(0.0f, 200.0f));
	OutfitButton->SetSize(Vector2f(180.0f, 20.0f));
	OutfitButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnOutfitButtonClicked, this));
	OutfitButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnOutfitButtonUpdating, this, OutfitButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * RepairButton(new UI::TextButton(this, "Repair"));
	
	RepairButton->SetPosition(Vector2f(0.0f, 230.0f));
	RepairButton->SetSize(Vector2f(180.0f, 20.0f));
	RepairButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnRepairButtonClicked, this));
	RepairButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnRepairButtonUpdating, this, RepairButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::ProgressBar * HullStateProgressBar(new UI::ProgressBar(this));
	
	HullStateProgressBar->SetPosition(Vector2f(190.0f, 230.0f));
	HullStateProgressBar->SetSize(Vector2f(410.0f, 20.0f));
	HullStateProgressBar->SetAnchorRight(true);
	HullStateProgressBar->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnHullStateProgressBarUpdating, this, HullStateProgressBar, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * RechargeButton(new UI::TextButton(this, "Recharge"));
	
	RechargeButton->SetPosition(Vector2f(0.0f, 260.0f));
	RechargeButton->SetSize(Vector2f(180.0f, 20.0f));
	RechargeButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnRechargeButtonClicked, this));
	RechargeButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnRechargeButtonUpdating, this, RechargeButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::ProgressBar * EnergyStateProgressBar(new UI::ProgressBar(this));
	
	EnergyStateProgressBar->SetPosition(Vector2f(190.0f, 260.0f));
	EnergyStateProgressBar->SetSize(Vector2f(410.0f, 20.0f));
	EnergyStateProgressBar->SetAnchorRight(true);
	EnergyStateProgressBar->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnEnergyStateProgressBarUpdating, this, EnergyStateProgressBar, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * RefuelButton(new UI::TextButton(this, "Refuel"));
	
	RefuelButton->SetPosition(Vector2f(0.0f, 290.0f));
	RefuelButton->SetSize(Vector2f(180.0f, 20.0f));
	RefuelButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnRefuelButtonClicked, this));
	RefuelButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnRefuelButtonUpdating, this, RefuelButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::ProgressBar * FuelStateProgressBar(new UI::ProgressBar(this));
	
	FuelStateProgressBar->SetPosition(Vector2f(190.0f, 290.0f));
	FuelStateProgressBar->SetSize(Vector2f(410.0f, 20.0f));
	FuelStateProgressBar->SetAnchorRight(true);
	FuelStateProgressBar->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnFuelStateProgressBarUpdating, this, FuelStateProgressBar, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * TakeOffButton(new UI::TextButton(this, "Take Off"));
	
	TakeOffButton->SetPosition(Vector2f(0.0f, 320.0f));
	TakeOffButton->SetSize(Vector2f(180.0f, 20.0f));
	TakeOffButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnTakeOffButtonClicked, this));
	TakeOffButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnTakeOffButtonUpdating, this, TakeOffButton, std::placeholders::_1, std::placeholders::_2));
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
	float EnergyPercentage(0.0f);
	
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
	EnergyStateProgressBar->SetColor(Color(1.0f - EnergyPercentage, EnergyPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnFuelStateProgressBarUpdating(UI::ProgressBar * FuelStateProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(FuelStateProgressBar != nullptr);
	
	float FuelPercentage(0.0f);
	
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
	FuelStateProgressBar->SetColor(Color(1.0f - FuelPercentage, FuelPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnHangarContentAdded(Object * Content)
{
	if(Content->GetTypeIdentifier() == "ship")
	{
		auto TheShip(dynamic_cast< Ship * >(Content));
		
		assert(TheShip != nullptr);
		assert(_ShipScrollBox != nullptr);
		assert(_ShipScrollBox->GetContent() != nullptr);
		
		_ShipScrollBox->GetContent()->AddSubWidget(new ShipListItem(TheShip));
	}
}

void UI::HangarWidget::_OnHangarContentRemoved(Object * Content)
{
	if(Content->GetTypeIdentifier() == "ship")
	{
		auto TheShip(dynamic_cast< Ship * >(Content));
		
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
	
	float HullPercentage(0.0f);
	
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
	HullStateProgressBar->SetColor(Color(1.0f - HullPercentage, HullPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnLoadButtonClicked(void)
{
	if(_LoadShipWindow == nullptr)
	{
		assert(_Planet != nullptr);
		assert(_SelectedShipListItem != nullptr);
		assert(_SelectedShipListItem->GetShip() != nullptr);
		_LoadShipWindow = new UI::LoadShipWindow(GetRootWidget(), _Planet->GetHangar(_Character), _SelectedShipListItem->GetShip());
		_LoadShipWindow->ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnLoadShipWindowDestroying, this, std::placeholders::_1));
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
		_OutfitShipDialog = new UI::OutfitShipDialog(GetRootWidget(), _SelectedShipListItem->GetShip()->GetReference());
		_OutfitShipDialog->ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnOutfitShipDialogDestroying, this, std::placeholders::_1));
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
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(_Planet->GetPlanetAssetClasses());
	bool OffersRefueling(false);
	
	for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			OffersRefueling = true;
			
			break;
		}
	}
	if(_SelectedShipListItem != nullptr)
	{
		assert(_SelectedShipListItem->GetShip() != nullptr);
	}
	RefuelButton->SetEnabled((OffersRefueling == true) && (_SelectedShipListItem != nullptr) && (_SelectedShipListItem->GetShip()->GetFuel() < _SelectedShipListItem->GetShip()->GetFuelCapacity()));
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
		auto NewItem(dynamic_cast< UI::ShipListItem * >(SubWidgetEvent.GetSubWidget()));
		
		assert(NewItem != nullptr);
		
		float Left(0.0f);
		
		for(auto OldWidget : _ShipScrollBox->GetContent()->GetSubWidgets())
		{
			if(OldWidget != NewItem)
			{
				Left = std::max(Left, OldWidget->GetPosition()[0] + OldWidget->GetSize()[0]);
			}
		}
		Left += 5.0f;
		NewItem->SetPosition(Vector2f(Left, 5.0f));
		NewItem->SetSize(Vector2f(100.0f, 100.0f));
		NewItem->ConnectMouseButtonCallback(std::bind(&UI::HangarWidget::_OnShipListItemMouseButton, this, std::placeholders::_1, NewItem));
		NewItem->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnShipListItemUpdating, this, std::placeholders::_1, std::placeholders::_2, NewItem));
		_ShipScrollBox->GetContent()->SetSize(Vector2f(std::max(NewItem->GetPosition()[0] + NewItem->GetSize()[0], _ShipScrollBox->GetView()->GetSize()[0]), _ShipScrollBox->GetView()->GetSize()[1]));
	}
}

void UI::HangarWidget::_OnShipScrollBoxSubWidgetRemoved(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		float NextLeft;
		UI::ShipListItem * NextSelectedListItem(nullptr);
		float Left(0.0f);
		
		for(auto OldSubWidget : _ShipScrollBox->GetContent()->GetSubWidgets())
		{
			if(OldSubWidget->GetPosition()[0] > SubWidgetEvent.GetSubWidget()->GetPosition()[0])
			{
				if((NextSelectedListItem == nullptr) || (OldSubWidget->GetPosition()[0] < NextLeft))
				{
					NextSelectedListItem = dynamic_cast< UI::ShipListItem * >(OldSubWidget);
					NextLeft = OldSubWidget->GetPosition()[0];
				}
				OldSubWidget->SetPosition(Vector2f(OldSubWidget->GetPosition()[0] - (SubWidgetEvent.GetSubWidget()->GetSize()[0] + 5.0f), OldSubWidget->GetPosition()[1]));
			}
			else
			{
				if((NextSelectedListItem == nullptr) || (OldSubWidget->GetPosition()[0] > NextLeft))
				{
					NextSelectedListItem = dynamic_cast< UI::ShipListItem * >(OldSubWidget);
					NextLeft = OldSubWidget->GetPosition()[0];
				}
			}
			Left = std::max(Left, OldSubWidget->GetPosition()[0] + OldSubWidget->GetSize()[0]);
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
		_ShipScrollBox->GetContent()->SetSize(Vector2f(std::max(Left + 5.0f, _ShipScrollBox->GetView()->GetSize()[0]), _ShipScrollBox->GetContent()->GetSize()[1]));
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
