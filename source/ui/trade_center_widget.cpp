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

#include <string_cast/string_cast.h>

#include "../asset_class.h"
#include "../character.h"
#include "../commodity.h"
#include "../commodity_class.h"
#include "../game_time.h"
#include "../globals.h"
#include "../graphics/camera.h"
#include "../graphics/color_rgbo.h"
#include "../graphics/engine.h"
#include "../graphics/light.h"
#include "../graphics/model.h"
#include "../graphics/node.h"
#include "../graphics/scene.h"
#include "../graphics/perspective_projection.h"
#include "../graphics/texture.h"
#include "../graphics/texture_manager.h"
#include "../graphics/texture_render_target.h"
#include "../graphics/view.h"
#include "../hangar.h"
#include "../object_aspect_accessory.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../object_aspect_outfitting.h"
#include "../object_factory.h"
#include "../planet.h"
#include "../slot.h"
#include "../visualization_prototype.h"
#include "../visualizations.h"
#include "../weapon.h"
#include "../weapon_class.h"
#include "key_event.h"
#include "label.h"
#include "list_box_item.h"
#include "mouse_button_event.h"
#include "scroll_bar.h"
#include "scroll_box.h"
#include "text_button.h"
#include "trade_center_widget.h"
#include "view_display.h"

namespace UI
{
	class TradeCenterAssetClassListWidget : public UI::ListBoxItem
	{
	public:
		TradeCenterAssetClassListWidget(UI::Widget * SupWidget, PlanetAssetClass * PlanetAssetClass, Hangar * Hangar) :
			UI::ListBoxItem(SupWidget),
			_PlanetAssetClass(PlanetAssetClass),
			_Hangar(Hangar)
		{
			assert(_Hangar != nullptr);
			_HangarDestroyingConnection = _Hangar->ConnectDestroyingCallback(std::bind(&UI::TradeCenterAssetClassListWidget::_OnHangarDestroying, this));
			ConnectDestroyingCallback(std::bind(&UI::TradeCenterAssetClassListWidget::_OnDestroying, this, std::placeholders::_1));
			
			auto NameLabel{new UI::Label{this, PlanetAssetClass->GetAssetClass()->GetName()}};
			
			NameLabel->SetLeft(10.0f);
			NameLabel->SetTop(0.0f);
			NameLabel->SetWidth(GetWidth() - 170.0f);
			NameLabel->SetHeight(GetHeight());
			NameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
			NameLabel->SetAnchorBottom(true);
			NameLabel->SetAnchorRight(true);
			
			auto HangarAmountLabel{new UI::Label{this, ""}};
			
			HangarAmountLabel->SetLeft(GetWidth() - 150.0f);
			HangarAmountLabel->SetTop(0.0f);
			HangarAmountLabel->SetWidth(40.0f);
			HangarAmountLabel->SetHeight(GetHeight());
			HangarAmountLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
			HangarAmountLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
			HangarAmountLabel->SetAnchorBottom(true);
			HangarAmountLabel->SetAnchorLeft(false);
			HangarAmountLabel->SetAnchorRight(true);
			HangarAmountLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterAssetClassListWidget::_OnHangarAmountLabelUpdating, this, std::placeholders::_1, std::placeholders::_2, HangarAmountLabel));
			
			auto SizeRequirementLabel{new UI::Label{this, to_string_cast(0.001 * g_ObjectFactory->GetSpaceRequirement(PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier(), PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier()), 3)}};
			
			SizeRequirementLabel->SetLeft(GetWidth() - 100.0f);
			SizeRequirementLabel->SetTop(0.0f);
			SizeRequirementLabel->SetWidth(40.0f);
			SizeRequirementLabel->SetHeight(GetHeight());
			SizeRequirementLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
			SizeRequirementLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
			SizeRequirementLabel->SetAnchorBottom(true);
			SizeRequirementLabel->SetAnchorLeft(false);
			SizeRequirementLabel->SetAnchorRight(true);
			
			auto PriceLabel{new UI::Label{this, to_string_cast(PlanetAssetClass->GetPrice())}};
			
			PriceLabel->SetLeft(GetWidth() - 50.0f);
			PriceLabel->SetTop(0.0f);
			PriceLabel->SetWidth(40.0f);
			PriceLabel->SetHeight(GetHeight());
			PriceLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
			PriceLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
			PriceLabel->SetAnchorBottom(true);
			PriceLabel->SetAnchorLeft(false);
			PriceLabel->SetAnchorRight(true);
			SetHeight(20.0f);
		}
		
		const PlanetAssetClass * GetPlanetAssetClass(void) const
		{
			return _PlanetAssetClass;
		}
	private:
		// event handlers
		void _OnHangarAmountLabelUpdating(float RealTimeSeconds, float GameTimeSeconds, UI::Label * HangarAmountLabel)
		{
			if(_Hangar != nullptr)
			{
				assert(_Hangar->GetAspectObjectContainer() != nullptr);
				HangarAmountLabel->SetVisible(true);
				HangarAmountLabel->SetText(to_string_cast(_Hangar->GetAspectObjectContainer()->GetAmount(_PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier(), _PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier())));
			}
			else
			{
				HangarAmountLabel->SetVisible(false);
			}
		}
		
		void _OnDestroying(UI::Event & DestroyingEvent)
		{
			if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
			{
				if(_Hangar != nullptr)
				{
					assert(_HangarDestroyingConnection.IsValid() == true);
					_Hangar->DisconnectDestroyingCallback(_HangarDestroyingConnection);
					_Hangar = nullptr;
				}
				assert(_HangarDestroyingConnection.IsValid() == false);
			}
		}
		
		void _OnHangarDestroying(void)
		{
			_Hangar = nullptr;
		}
		
		PlanetAssetClass * _PlanetAssetClass;
		Hangar * _Hangar;
		Connection _HangarDestroyingConnection;
	};
}

UI::TradeCenterWidget::TradeCenterWidget(UI::Widget * SupWidget, Planet * Planet, Character * Character) :
	UI::Widget(SupWidget),
	_Character(Character),
	_Planet(Planet),
	_SelectedTradeCenterAssetClassListWidget(nullptr)
{
	assert(_Character != nullptr);
	_CharacterDestroyingConnection = _Character->ConnectDestroyingCallback(std::bind(&UI::TradeCenterWidget::_OnCharacterDestroying, this));
	assert(_Planet != nullptr);
	_PlanetDestroyingConnection = _Planet->ConnectDestroyingCallback(std::bind(&UI::TradeCenterWidget::_OnPlanetDestroying, this));
	ConnectDestroyingCallback(std::bind(&UI::TradeCenterWidget::_OnDestroying, this, std::placeholders::_1));
	ConnectKeyCallback(std::bind(&UI::TradeCenterWidget::_OnKey, this, std::placeholders::_1));
	ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnUpdating, this, std::placeholders::_1, std::placeholders::_2));
	
	auto BuyButton{new UI::TextButton{this, "Buy"}};
	
	BuyButton->SetLeft(0.0f);
	BuyButton->SetTop(GetHeight() - 20.0f);
	BuyButton->SetWidth(100.0f);
	BuyButton->SetHeight(20.0f);
	BuyButton->SetAnchorBottom(true);
	BuyButton->SetAnchorTop(false);
	BuyButton->ConnectClickedCallback(std::bind(&UI::TradeCenterWidget::_OnBuyButtonClicked, this));
	BuyButton->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnBuyButtonUpdating, this, BuyButton, std::placeholders::_1, std::placeholders::_2));
	
	auto SellButton{new UI::TextButton{this, "Sell"}};
	
	SellButton->SetLeft(110.0f);
	SellButton->SetTop(GetHeight() - 20.0f);
	SellButton->SetWidth(100.0f);
	SellButton->SetHeight(20.0f);
	SellButton->SetAnchorBottom(true);
	SellButton->SetAnchorTop(false);
	SellButton->ConnectClickedCallback(std::bind(&UI::TradeCenterWidget::_OnSellButtonClicked, this));
	SellButton->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnSellButtonUpdating, this, SellButton, std::placeholders::_1, std::placeholders::_2));
	
	auto HeaderRow{new UI::Widget{this}};
	
	HeaderRow->SetLeft(0.0f);
	HeaderRow->SetTop(0.0f);
	HeaderRow->SetWidth(GetWidth() - 180.0f);
	HeaderRow->SetHeight(20.0f);
	HeaderRow->SetBackgroundColor(Graphics::ColorRGBO(0.3f, 0.3f, 0.3f, 1.0f));
	HeaderRow->SetAnchorRight(true);
	
	auto NameColumnHeader{new UI::Label{HeaderRow, "Name"}};
	
	NameColumnHeader->SetLeft(15.0f);
	NameColumnHeader->SetTop(0.0f);
	NameColumnHeader->SetWidth(HeaderRow->GetWidth() - 180.0f);
	NameColumnHeader->SetHeight(HeaderRow->GetHeight());
	NameColumnHeader->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Left);
	NameColumnHeader->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	NameColumnHeader->SetAnchorBottom(true);
	NameColumnHeader->SetAnchorRight(true);
	
	auto CharacterAmountColumnHeader{new UI::Label{HeaderRow, "Amount"}};
	
	CharacterAmountColumnHeader->SetLeft(HeaderRow->GetWidth() - 155.0f);
	CharacterAmountColumnHeader->SetTop(0.0f);
	CharacterAmountColumnHeader->SetWidth(40.0f);
	CharacterAmountColumnHeader->SetHeight(HeaderRow->GetHeight());
	CharacterAmountColumnHeader->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	CharacterAmountColumnHeader->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	CharacterAmountColumnHeader->SetAnchorBottom(true);
	CharacterAmountColumnHeader->SetAnchorLeft(false);
	CharacterAmountColumnHeader->SetAnchorRight(true);
	
	auto SizeColumnHeader{new UI::Label{HeaderRow, "Size"}};
	
	SizeColumnHeader->SetLeft(HeaderRow->GetWidth() - 105.0f);
	SizeColumnHeader->SetTop(0.0f);
	SizeColumnHeader->SetWidth(40.0f);
	SizeColumnHeader->SetHeight(HeaderRow->GetHeight());
	SizeColumnHeader->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	SizeColumnHeader->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	SizeColumnHeader->SetAnchorBottom(true);
	SizeColumnHeader->SetAnchorLeft(false);
	SizeColumnHeader->SetAnchorRight(true);
	
	auto PriceColumnHeader{new UI::Label{HeaderRow, "Price"}};
	
	PriceColumnHeader->SetLeft(HeaderRow->GetWidth() - 55.0f);
	PriceColumnHeader->SetTop(0.0f);
	PriceColumnHeader->SetWidth(40.0f);
	PriceColumnHeader->SetHeight(HeaderRow->GetHeight());
	PriceColumnHeader->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	PriceColumnHeader->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	PriceColumnHeader->SetAnchorLeft(false);
	PriceColumnHeader->SetAnchorRight(true);
	_AssetClassScrollBox = new UI::ScrollBox{this};
	_AssetClassScrollBox->SetLeft(0.0f);
	_AssetClassScrollBox->SetTop(20.0f);
	_AssetClassScrollBox->SetWidth(GetWidth() - 160.0f);
	_AssetClassScrollBox->SetHeight(GetHeight() - 50.0f);
	_AssetClassScrollBox->SetHorizontalScrollBarVisible(false);
	_AssetClassScrollBox->SetAnchorRight(true);
	_AssetClassScrollBox->SetAnchorBottom(true);
	_AssetClassScrollBox->GetContent()->SetWidth(_AssetClassScrollBox->GetView()->GetWidth());
	_AssetClassScrollBox->GetContent()->SetAnchorRight(true);
	
	auto Hangar(_Planet->GetHangar(_Character));
	
	assert(Hangar != nullptr);
	
	auto Top{5.0f};
	
	for(auto PlanetAssetClass : Planet->GetPlanetAssetClasses())
	{
		auto NewTradeCenterAssetClassListWidget{new UI::TradeCenterAssetClassListWidget{_AssetClassScrollBox->GetContent(), PlanetAssetClass, Hangar}};
		
		NewTradeCenterAssetClassListWidget->SetLeft(5.0f);
		NewTradeCenterAssetClassListWidget->SetTop(Top);
		NewTradeCenterAssetClassListWidget->SetWidth(_AssetClassScrollBox->GetContent()->GetWidth());
		NewTradeCenterAssetClassListWidget->SetAnchorRight(true);
		NewTradeCenterAssetClassListWidget->ConnectMouseButtonCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassMouseButton, this, std::placeholders::_1, NewTradeCenterAssetClassListWidget));
		Top += NewTradeCenterAssetClassListWidget->GetHeight() + 5.0f;
	}
	_AssetClassScrollBox->GetContent()->SetHeight(Top);
	_AssetClassViewDisplay = new UI::ViewDisplay{this};
	_AssetClassViewDisplay->SetLeft(GetWidth() - 150.0f);
	_AssetClassViewDisplay->SetTop(0.0f);
	_AssetClassViewDisplay->SetWidth(150.0f);
	_AssetClassViewDisplay->SetHeight(150.0f);
	_AssetClassViewDisplay->SetAnchorLeft(false);
	_AssetClassViewDisplay->SetAnchorRight(true);
	_AssetClassViewDisplay->SetBackgroundColor(Graphics::ColorRGBO(0.15f, 0.15f, 0.15f, 1.0f));
	
	auto AssetClassPriceCaptionLabel{new UI::Label{this, "Price:"}};
	
	AssetClassPriceCaptionLabel->SetLeft(GetWidth() - 150.0f);
	AssetClassPriceCaptionLabel->SetTop(160.0f);
	AssetClassPriceCaptionLabel->SetWidth(150.0f);
	AssetClassPriceCaptionLabel->SetHeight(20.0f);
	AssetClassPriceCaptionLabel->SetAnchorLeft(false);
	AssetClassPriceCaptionLabel->SetAnchorRight(true);
	AssetClassPriceCaptionLabel->SetVisible(false);
	AssetClassPriceCaptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassPriceCaptionLabelUpdating, this, AssetClassPriceCaptionLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassPriceLabel{new UI::Label{this, ""}};
	
	AssetClassPriceLabel->SetLeft(GetWidth() - 150.0f);
	AssetClassPriceLabel->SetTop(160.0f);
	AssetClassPriceLabel->SetWidth(150.0f);
	AssetClassPriceLabel->SetHeight(20.0f);
	AssetClassPriceLabel->SetAnchorLeft(false);
	AssetClassPriceLabel->SetAnchorRight(true);
	AssetClassPriceLabel->SetVisible(false);
	AssetClassPriceLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	AssetClassPriceLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassPriceLabelUpdating, this, AssetClassPriceLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassSizeCaptionLabel{new UI::Label{this, "Size:"}};
	
	AssetClassSizeCaptionLabel->SetLeft(GetWidth() - 150.0f);
	AssetClassSizeCaptionLabel->SetTop(180.0f);
	AssetClassSizeCaptionLabel->SetWidth(150.0f);
	AssetClassSizeCaptionLabel->SetHeight(20.0f);
	AssetClassSizeCaptionLabel->SetAnchorLeft(false);
	AssetClassSizeCaptionLabel->SetAnchorRight(true);
	AssetClassSizeCaptionLabel->SetVisible(false);
	AssetClassSizeCaptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassSizeCaptionLabelUpdating, this, AssetClassSizeCaptionLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassSizeLabel{new UI::Label{this, ""}};
	
	AssetClassSizeLabel->SetLeft(GetWidth() - 150.0f);
	AssetClassSizeLabel->SetTop(180.0f);
	AssetClassSizeLabel->SetWidth(150.0f);
	AssetClassSizeLabel->SetHeight(20.0f);
	AssetClassSizeLabel->SetAnchorLeft(false);
	AssetClassSizeLabel->SetAnchorRight(true);
	AssetClassSizeLabel->SetVisible(false);
	AssetClassSizeLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	AssetClassSizeLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassSizeLabelUpdating, this, AssetClassSizeLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassDescriptionCaptionLabel{new UI::Label{this, "Description:"}};
	
	AssetClassDescriptionCaptionLabel->SetLeft(GetWidth() - 150.0f);
	AssetClassDescriptionCaptionLabel->SetTop(200.0f);
	AssetClassDescriptionCaptionLabel->SetWidth(150.0f);
	AssetClassDescriptionCaptionLabel->SetHeight(20.0f);
	AssetClassDescriptionCaptionLabel->SetAnchorLeft(false);
	AssetClassDescriptionCaptionLabel->SetAnchorRight(true);
	AssetClassDescriptionCaptionLabel->SetVisible(false);
	AssetClassDescriptionCaptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassDescriptionCaptionLabelUpdating, this, AssetClassDescriptionCaptionLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassDescriptionLabel{new UI::Label{this, ""}};
	
	AssetClassDescriptionLabel->SetLeft(GetWidth() - 150.0f);
	AssetClassDescriptionLabel->SetTop(220.0f);
	AssetClassDescriptionLabel->SetWidth(150.0f);
	AssetClassDescriptionLabel->SetHeight(30.0f);
	AssetClassDescriptionLabel->SetAnchorLeft(false);
	AssetClassDescriptionLabel->SetAnchorRight(true);
	AssetClassDescriptionLabel->SetAnchorBottom(true);
	AssetClassDescriptionLabel->SetWrap(true);
	AssetClassDescriptionLabel->SetWordWrap(true);
	AssetClassDescriptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassDescriptionLabelUpdating, this, AssetClassDescriptionLabel, std::placeholders::_1, std::placeholders::_2));
}

void UI::TradeCenterWidget::_Buy(const PlanetAssetClass * PlanetAssetClass)
{
	assert(PlanetAssetClass != nullptr);
	assert(PlanetAssetClass->GetAssetClass() != nullptr);
	assert(_Character != nullptr);
	assert(_Planet != nullptr);
	if(_Character->RemoveCredits(PlanetAssetClass->GetPrice()) == true)
	{
		auto NewCargo(g_ObjectFactory->Create(PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier(), PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier(), true));
		
		assert(NewCargo != nullptr);
		NewCargo->SetObjectIdentifier("::asset(" + PlanetAssetClass->GetAssetClass()->GetIdentifier() + ")::" + PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier() + "(" + PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier() + ")::created_on(" + _Planet->GetObjectIdentifier() + ")::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::bought_by(" + _Character->GetObjectIdentifier() + ")::created_at_address(" + to_string_cast(reinterpret_cast< void * >(NewCargo)) + ")");
		
		auto Hangar(_Planet->GetHangar(_Character));
		
		assert(Hangar != nullptr);
		assert(Hangar->GetAspectObjectContainer() != nullptr);
		Hangar->GetAspectObjectContainer()->AddContent(NewCargo);
	}
}

void UI::TradeCenterWidget::_ClearAssetClassViewDisplay(void)
{
	auto OldView(_AssetClassViewDisplay->GetView());
	
	if(OldView != nullptr)
	{
		_AssetClassViewDisplay->SetView(nullptr);
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

void UI::TradeCenterWidget::_OnAssetClassDescriptionCaptionLabelUpdating(UI::Label * AssetClassDescriptionCaptionLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(AssetClassDescriptionCaptionLabel != nullptr);
	AssetClassDescriptionCaptionLabel->SetVisible(_SelectedTradeCenterAssetClassListWidget != nullptr);
}

void UI::TradeCenterWidget::_OnAssetClassDescriptionLabelUpdating(UI::Label * AssetClassDescriptionLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	if(_SelectedTradeCenterAssetClassListWidget != nullptr)
	{
		AssetClassDescriptionLabel->SetVisible(true);
		AssetClassDescriptionLabel->SetText(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetDescription());
	}
	else
	{
		AssetClassDescriptionLabel->SetVisible(false);
	}
}

void UI::TradeCenterWidget::_OnAssetClassMouseButton(UI::MouseButtonEvent & MouseButtonEvent, TradeCenterAssetClassListWidget * TradeCenterAssetClassListWidget)
{
	if((MouseButtonEvent.GetPhase() == UI::Event::Phase::Bubbling) && (MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedTradeCenterAssetClassListWidget != nullptr)
		{
			_SelectedTradeCenterAssetClassListWidget->SetSelected(false);
		}
		_SelectedTradeCenterAssetClassListWidget = TradeCenterAssetClassListWidget;
		_SelectedTradeCenterAssetClassListWidget->SetSelected(true);
		_ClearAssetClassViewDisplay();
		if((_AssetClassViewDisplay->GetWidth() > 0.0f) && (_AssetClassViewDisplay->GetHeight() > 0.0f))
		{
			auto VisualizationPrototype{g_ObjectFactory->GetVisualizationPrototype(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier(), _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier())};
			
			if(VisualizationPrototype != nullptr)
			{
				auto VisualizationNode(VisualizePrototype(VisualizationPrototype));
				
				if(VisualizationNode != nullptr)
				{
					float RadialSize(VisualizationPrototype->GetModel()->GetRadialSize());
					float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
					Graphics::PerspectiveProjection * PerspectiveProjection(new Graphics::PerspectiveProjection());
					
					PerspectiveProjection->SetFieldOfViewY(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)) * 2.0f);
					PerspectiveProjection->SetAspect(_AssetClassViewDisplay->GetWidth() / _AssetClassViewDisplay->GetHeight());
					PerspectiveProjection->SetNearClippingPlane(0.1f);
					PerspectiveProjection->SetFarClippingPlane(100.0f);
					
					auto View(new Graphics::View());
					
					g_GraphicsEngine->AddView(View);
					View->SetClearColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 0.0f));
					assert(View->GetCamera() != nullptr);
					View->GetCamera()->SetProjection(PerspectiveProjection);
					View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateTranslation(Vector3f::CreateFromComponents(0.0f, -2.5f, 1.4f).Normalize() * 4.0f * RadialSize).RotateX(1.05f));
					
					auto Scene(new Graphics::Scene());
					
					Scene->SetDestroyCallback(std::bind(&UI::TradeCenterWidget::_OnDestroyInScene, this, std::placeholders::_1));
					Scene->ActivateLight();
					assert(Scene->GetLight() != nullptr);
					Scene->GetLight()->SetType(Graphics::Light::Type::Directional);
					Scene->GetLight()->SetDirection(Vector3f::CreateFromComponents(20.0f, 10.0f, -20.0f));
					Scene->GetLight()->SetColor(Graphics::ColorRGB(1.0f, 1.0f, 1.0f));
					View->SetScene(Scene);
					
					auto Texture(new Graphics::Texture());
					
					Texture->Create(_AssetClassViewDisplay->GetWidth(), _AssetClassViewDisplay->GetHeight(), 1);
					
					auto RenderTarget(new Graphics::TextureRenderTarget());
					
					RenderTarget->SetTexture(Texture);
					View->SetRenderTarget(RenderTarget);
					Scene->SetRootNode(VisualizationNode);
					VisualizationNode->SetClearColorBuffer(true);
					VisualizationNode->SetClearDepthBuffer(true);
					VisualizationNode->SetUseDepthTest(true);
					_AssetClassViewDisplay->SetView(View);
				}
			}
		}
		MouseButtonEvent.StopPropagation();
	}
}

void UI::TradeCenterWidget::_OnAssetClassPriceCaptionLabelUpdating(UI::Label * AssetClassPriceCaptionLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(AssetClassPriceCaptionLabel != nullptr);
	AssetClassPriceCaptionLabel->SetVisible(_SelectedTradeCenterAssetClassListWidget != nullptr);
}

void UI::TradeCenterWidget::_OnAssetClassPriceLabelUpdating(UI::Label * AssetClassPriceLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(AssetClassPriceLabel != nullptr);
	if(_SelectedTradeCenterAssetClassListWidget != nullptr)
	{
		AssetClassPriceLabel->SetVisible(true);
		AssetClassPriceLabel->SetText(to_string_cast(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetPrice()));
	}
	else
	{
		AssetClassPriceLabel->SetText("");
		AssetClassPriceLabel->SetVisible(false);
	}
}

void UI::TradeCenterWidget::_OnAssetClassSizeCaptionLabelUpdating(UI::Label * AssetClassSizeCaptionLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(AssetClassSizeCaptionLabel != nullptr);
	AssetClassSizeCaptionLabel->SetVisible(_SelectedTradeCenterAssetClassListWidget != nullptr);
}

void UI::TradeCenterWidget::_OnAssetClassSizeLabelUpdating(UI::Label * AssetClassSizeLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(AssetClassSizeLabel != nullptr);
	if(_SelectedTradeCenterAssetClassListWidget != nullptr)
	{
		AssetClassSizeLabel->SetVisible(true);
		AssetClassSizeLabel->SetText(to_string_cast(0.001 * g_ObjectFactory->GetSpaceRequirement(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier(), _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier()), 3));
	}
	else
	{
		AssetClassSizeLabel->SetText("");
		AssetClassSizeLabel->SetVisible(false);
	}
}

void UI::TradeCenterWidget::_OnBuyButtonClicked(void)
{
	if(_SelectedTradeCenterAssetClassListWidget != nullptr)
	{
		_Buy(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass());
	}
}

void UI::TradeCenterWidget::_OnBuyButtonUpdating(UI::Button * BuyButton, float RealTimeSeconds, float GameTimeSeconds)
{
	BuyButton->SetEnabled((_SelectedTradeCenterAssetClassListWidget != nullptr) && (_Character != nullptr) && (_Character->GetCredits() >= _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetPrice()) && (_Planet != nullptr) && (_Planet->GetHangar(_Character) != nullptr));
}

void UI::TradeCenterWidget::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_ClearAssetClassViewDisplay();
		if(_Planet != nullptr)
		{
			assert(_PlanetDestroyingConnection.IsValid() == true);
			_Planet->DisconnectDestroyingCallback(_PlanetDestroyingConnection);
			_Planet = nullptr;
		}
		assert(_PlanetDestroyingConnection.IsValid() == false);
		if(_Character != nullptr)
		{
			assert(_CharacterDestroyingConnection.IsValid() == true);
			_Character->DisconnectDestroyingCallback(_CharacterDestroyingConnection);
			_Character = nullptr;
		}
		assert(_CharacterDestroyingConnection.IsValid() == false);
	}
}

void UI::TradeCenterWidget::_OnCharacterDestroying(void)
{
	assert(_CharacterDestroyingConnection.IsValid() == true);
	assert(_Character != nullptr);
	_Character->DisconnectDestroyingCallback(_CharacterDestroyingConnection);
	assert(_CharacterDestroyingConnection.IsValid() == false);
	_Character = nullptr;
}

void UI::TradeCenterWidget::_OnDestroyInScene(Graphics::Node * Node)
{
	delete Node;
}

void UI::TradeCenterWidget::_OnKey(UI::KeyEvent & KeyEvent)
{
	if((KeyEvent.GetPhase() == UI::Event::Phase::Bubbling) && (KeyEvent.GetKeyCode() == 56 /* B */) && (_SelectedTradeCenterAssetClassListWidget != nullptr) && (KeyEvent.IsDown() == true))
	{
		_Buy(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass());
	}
	else if((KeyEvent.GetPhase() == UI::Event::Phase::Bubbling) && (KeyEvent.GetKeyCode() == 39 /* S */) && (_SelectedTradeCenterAssetClassListWidget != nullptr) && (KeyEvent.IsDown() == true))
	{
		_Sell(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass());
	}
}

void UI::TradeCenterWidget::_OnPlanetDestroying(void)
{
	assert(_PlanetDestroyingConnection.IsValid() == true);
	assert(_Planet != nullptr);
	_Planet->DisconnectDestroyingCallback(_PlanetDestroyingConnection);
	assert(_PlanetDestroyingConnection.IsValid() == false);
	_Planet = nullptr;
}

void UI::TradeCenterWidget::_OnSellButtonClicked(void)
{
	if(_SelectedTradeCenterAssetClassListWidget != nullptr)
	{
		_Sell(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass());
	}
}

void UI::TradeCenterWidget::_OnSellButtonUpdating(UI::Button * SellButton, float RealTimeSeconds, float GameTimeSeconds)
{
	bool Enabled(false);
	
	if((_SelectedTradeCenterAssetClassListWidget != nullptr) && (_Character != nullptr) && (_Planet != nullptr))
	{
		assert(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass() != nullptr);
		assert(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass() != nullptr);
		
		auto Hangar(_Planet->GetHangar(_Character));
		
		assert(Hangar != nullptr);
		assert(Hangar->GetAspectObjectContainer() != nullptr);
		for(auto Content : Hangar->GetAspectObjectContainer()->GetContent())
		{
			if((Content->GetTypeIdentifier() == _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier()) && (Content->GetClassIdentifier() == _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier()))
			{
				Enabled = true;
				
				break;
			}
		}
	}
	SellButton->SetEnabled(Enabled);
}

void UI::TradeCenterWidget::_OnUpdating(float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_AssetClassViewDisplay != nullptr);
	if(_AssetClassViewDisplay->GetView() != nullptr)
	{
		assert(_AssetClassViewDisplay->GetView()->GetScene() != nullptr);
		assert(_AssetClassViewDisplay->GetView()->GetScene()->GetRootNode() != nullptr);
		_AssetClassViewDisplay->GetView()->GetScene()->GetRootNode()->SetOrientation(_AssetClassViewDisplay->GetView()->GetScene()->GetRootNode()->GetOrientation().RotatedZ(-RealTimeSeconds * M_PI / 4.0f));
	}
}

void UI::TradeCenterWidget::_Sell(const PlanetAssetClass * PlanetAssetClass)
{
	assert(PlanetAssetClass != nullptr);
	assert(PlanetAssetClass->GetAssetClass() != nullptr);
	assert(_Character != nullptr);
	assert(_Planet != nullptr);
	
	auto Hangar{_Planet->GetHangar(_Character)};
	std::list< Object * > ToAdd;
	
	assert(Hangar != nullptr);
	assert(Hangar->GetAspectObjectContainer() != nullptr);
	for(auto Content : Hangar->GetAspectObjectContainer()->GetContent())
	{
		if((Content->GetTypeIdentifier() == PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier()) && (Content->GetClassIdentifier() == PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier()))
		{
			if(Content->GetAspectOutfitting() != nullptr)
			{
				for(auto Slot : Content->GetAspectOutfitting()->GetSlots())
				{
					if(Slot.second->GetMountedObject() != nullptr)
					{
						Slot.second->Unmount();
					}
				}
			}
			if(Content->GetAspectObjectContainer() != nullptr)
			{
				while(Content->GetAspectObjectContainer()->GetContent().empty() == false)
				{
					auto InnerContent{*(Content->GetAspectObjectContainer()->GetContent().begin())};
					
					Content->GetAspectObjectContainer()->RemoveContent(InnerContent);
					if(InnerContent->GetTypeIdentifier() != "storage")
					{
						ToAdd.push_back(InnerContent);
					}
					else
					{
						assert(InnerContent->GetAspectObjectContainer() != nullptr);
						while(InnerContent->GetAspectObjectContainer()->GetContent().empty() == false)
						{
							auto StorageContent(*(InnerContent->GetAspectObjectContainer()->GetContent().begin()));
							
							InnerContent->GetAspectObjectContainer()->RemoveContent(StorageContent);
							ToAdd.push_back(StorageContent);
						}
					}
				}
			}
			Content->Destroy();
			delete Content;
			assert(_Character != nullptr);
			_Character->AddCredits(PlanetAssetClass->GetPrice());
			
			break;
		}
	}
	for(auto Object : ToAdd)
	{
		Hangar->GetAspectObjectContainer()->AddContent(Object);
	}
}
