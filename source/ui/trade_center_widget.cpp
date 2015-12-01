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
			SetSize(Vector2f(200.0f, 20.0f));
			
			auto NameLabel(new UI::Label(this, PlanetAssetClass->GetAssetClass()->GetName()));
			
			NameLabel->SetPosition(Vector2f(10.0f, 0.0f));
			NameLabel->SetSize(Vector2f(30.0f, 20.0f));
			NameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
			NameLabel->SetAnchorRight(true);
			
			auto HangarAmountLabel(new UI::Label(this, ""));
			
			HangarAmountLabel->SetPosition(Vector2f(50.0f, 0.0f));
			HangarAmountLabel->SetSize(Vector2f(40.0f, 20.0f));
			HangarAmountLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
			HangarAmountLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
			HangarAmountLabel->SetAnchorLeft(false);
			HangarAmountLabel->SetAnchorRight(true);
			HangarAmountLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterAssetClassListWidget::_OnHangarAmountLabelUpdating, this, std::placeholders::_1, std::placeholders::_2, HangarAmountLabel));
			
			auto SizeRequirementLabel(new UI::Label(this, to_string_cast(0.001 * g_ObjectFactory->GetSpaceRequirement(PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier(), PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier()), 3)));
			
			SizeRequirementLabel->SetPosition(Vector2f(100.0f, 0.0f));
			SizeRequirementLabel->SetSize(Vector2f(40.0f, 20.0f));
			SizeRequirementLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
			SizeRequirementLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
			SizeRequirementLabel->SetAnchorLeft(false);
			SizeRequirementLabel->SetAnchorRight(true);
			
			auto PriceLabel(new UI::Label(this, to_string_cast(PlanetAssetClass->GetPrice())));
			
			PriceLabel->SetPosition(Vector2f(150.0f, 0.0f));
			PriceLabel->SetSize(Vector2f(40.0f, 20.0f));
			PriceLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
			PriceLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
			PriceLabel->SetAnchorLeft(false);
			PriceLabel->SetAnchorRight(true);
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
	SetSize(Vector2f(650.0f, 300.0f));
	ConnectKeyCallback(std::bind(&UI::TradeCenterWidget::_OnKey, this, std::placeholders::_1));
	ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnUpdating, this, std::placeholders::_1, std::placeholders::_2));
	
	auto BuyButton(new UI::TextButton(this, "Buy"));
	
	BuyButton->SetPosition(Vector2f(0.0f, 280.0f));
	BuyButton->SetSize(Vector2f(100.0f, 20.0f));
	BuyButton->SetAnchorBottom(true);
	BuyButton->SetAnchorTop(false);
	BuyButton->ConnectClickedCallback(std::bind(&UI::TradeCenterWidget::_OnBuyButtonClicked, this));
	BuyButton->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnBuyButtonUpdating, this, BuyButton, std::placeholders::_1, std::placeholders::_2));
	
	auto SellButton(new UI::TextButton(this, "Sell"));
	
	SellButton->SetPosition(Vector2f(110.0f, 280.0f));
	SellButton->SetSize(Vector2f(100.0f, 20.0f));
	SellButton->SetAnchorBottom(true);
	SellButton->SetAnchorTop(false);
	SellButton->ConnectClickedCallback(std::bind(&UI::TradeCenterWidget::_OnSellButtonClicked, this));
	SellButton->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnSellButtonUpdating, this, SellButton, std::placeholders::_1, std::placeholders::_2));
	
	auto HeaderRow(new UI::Widget(this));
	
	HeaderRow->SetPosition(Vector2f(0.0f, 0.0f));
	HeaderRow->SetSize(Vector2f(470.0f, 20.0f));
	HeaderRow->SetBackgroundColor(Graphics::ColorRGBO(0.3f, 0.3f, 0.3f, 1.0f));
	HeaderRow->SetAnchorRight(true);
	
	auto NameColumnHeader(new UI::Label(HeaderRow, "Name"));
	
	NameColumnHeader->SetPosition(Vector2f(15.0f, 0.0f));
	NameColumnHeader->SetSize(Vector2f(290.0f, 20.0f));
	NameColumnHeader->SetHorizontalAlignment(UI::Label::ALIGN_LEFT);
	NameColumnHeader->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	NameColumnHeader->SetAnchorRight(true);
	
	auto CharacterAmountColumnHeader(new UI::Label(HeaderRow, "Amount"));
	
	CharacterAmountColumnHeader->SetPosition(Vector2f(315.0f, 0.0f));
	CharacterAmountColumnHeader->SetSize(Vector2f(40.0f, 20.0f));
	CharacterAmountColumnHeader->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	CharacterAmountColumnHeader->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	CharacterAmountColumnHeader->SetAnchorLeft(false);
	CharacterAmountColumnHeader->SetAnchorRight(true);
	
	auto SizeColumnHeader(new UI::Label(HeaderRow, "Size"));
	
	SizeColumnHeader->SetPosition(Vector2f(365.0f, 0.0f));
	SizeColumnHeader->SetSize(Vector2f(40.0f, 20.0f));
	SizeColumnHeader->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	SizeColumnHeader->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	SizeColumnHeader->SetAnchorLeft(false);
	SizeColumnHeader->SetAnchorRight(true);
	
	auto PriceColumnHeader(new UI::Label(HeaderRow, "Price"));
	
	PriceColumnHeader->SetPosition(Vector2f(415.0f, 0.0f));
	PriceColumnHeader->SetSize(Vector2f(40.0f, 20.0f));
	PriceColumnHeader->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	PriceColumnHeader->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	PriceColumnHeader->SetAnchorLeft(false);
	PriceColumnHeader->SetAnchorRight(true);
	_AssetClassScrollBox = new UI::ScrollBox(this);
	_AssetClassScrollBox->SetPosition(Vector2f(0.0f, 20.0f));
	_AssetClassScrollBox->SetSize(Vector2f(490.0f, 230.0f));
	_AssetClassScrollBox->SetHorizontalScrollBarVisible(false);
	_AssetClassScrollBox->SetAnchorRight(true);
	_AssetClassScrollBox->SetAnchorBottom(true);
	
	auto Hangar(_Planet->GetHangar(_Character));
	
	assert(Hangar != nullptr);
	
	float Top(5.0f);
	
	for(auto PlanetAssetClass : Planet->GetPlanetAssetClasses())
	{
		auto NewTradeCenterAssetClassListWidget(new UI::TradeCenterAssetClassListWidget(_AssetClassScrollBox->GetContent(), PlanetAssetClass, Hangar));
		
		NewTradeCenterAssetClassListWidget->SetPosition(Vector2f(5.0f, Top));
		NewTradeCenterAssetClassListWidget->SetSize(Vector2f(_AssetClassScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
		NewTradeCenterAssetClassListWidget->SetAnchorRight(true);
		NewTradeCenterAssetClassListWidget->ConnectMouseButtonCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassMouseButton, this, std::placeholders::_1, NewTradeCenterAssetClassListWidget));
		Top += 25.0f;
	}
	_AssetClassScrollBox->GetContent()->SetSize(Vector2f(450.0f, Top));
	_AssetClassScrollBox->GetContent()->SetAnchorRight(true);
	_AssetClassViewDisplay = new UI::ViewDisplay(this);
	_AssetClassViewDisplay->SetPosition(Vector2f(500.0f, 0.0f));
	_AssetClassViewDisplay->SetSize(Vector2f(150.0f, 150.0f));
	_AssetClassViewDisplay->SetAnchorLeft(false);
	_AssetClassViewDisplay->SetAnchorRight(true);
	_AssetClassViewDisplay->SetBackgroundColor(Graphics::ColorRGBO(0.15f, 0.15f, 0.15f, 1.0f));
	
	auto AssetClassPriceCaptionLabel(new UI::Label(this, "Price:"));
	
	AssetClassPriceCaptionLabel->SetPosition(Vector2f(500.0f, 160.0f));
	AssetClassPriceCaptionLabel->SetSize(Vector2f(150.0f, 20.0f));
	AssetClassPriceCaptionLabel->SetAnchorLeft(false);
	AssetClassPriceCaptionLabel->SetAnchorRight(true);
	AssetClassPriceCaptionLabel->SetVisible(false);
	AssetClassPriceCaptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassPriceCaptionLabelUpdating, this, AssetClassPriceCaptionLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassPriceLabel(new UI::Label(this, ""));
	
	AssetClassPriceLabel->SetPosition(Vector2f(500.0f, 160.0f));
	AssetClassPriceLabel->SetSize(Vector2f(150.0f, 20.0f));
	AssetClassPriceLabel->SetAnchorLeft(false);
	AssetClassPriceLabel->SetAnchorRight(true);
	AssetClassPriceLabel->SetVisible(false);
	AssetClassPriceLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::ALIGN_RIGHT);
	AssetClassPriceLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassPriceLabelUpdating, this, AssetClassPriceLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassSizeCaptionLabel(new UI::Label(this, "Size:"));
	
	AssetClassSizeCaptionLabel->SetPosition(Vector2f(500.0f, 180.0f));
	AssetClassSizeCaptionLabel->SetSize(Vector2f(150.0f, 20.0f));
	AssetClassSizeCaptionLabel->SetAnchorLeft(false);
	AssetClassSizeCaptionLabel->SetAnchorRight(true);
	AssetClassSizeCaptionLabel->SetVisible(false);
	AssetClassSizeCaptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassSizeCaptionLabelUpdating, this, AssetClassSizeCaptionLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassSizeLabel(new UI::Label(this, ""));
	
	AssetClassSizeLabel->SetPosition(Vector2f(500.0f, 180.0f));
	AssetClassSizeLabel->SetSize(Vector2f(150.0f, 20.0f));
	AssetClassSizeLabel->SetAnchorLeft(false);
	AssetClassSizeLabel->SetAnchorRight(true);
	AssetClassSizeLabel->SetVisible(false);
	AssetClassSizeLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::ALIGN_RIGHT);
	AssetClassSizeLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassSizeLabelUpdating, this, AssetClassSizeLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassDescriptionCaptionLabel(new UI::Label(this, "Description:"));
	
	AssetClassDescriptionCaptionLabel->SetPosition(Vector2f(500.0f, 200.0f));
	AssetClassDescriptionCaptionLabel->SetSize(Vector2f(150.0f, 20.0f));
	AssetClassDescriptionCaptionLabel->SetAnchorLeft(false);
	AssetClassDescriptionCaptionLabel->SetAnchorRight(true);
	AssetClassDescriptionCaptionLabel->SetVisible(false);
	AssetClassDescriptionCaptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassDescriptionCaptionLabelUpdating, this, AssetClassDescriptionCaptionLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto AssetClassDescriptionLabel(new UI::Label(this, ""));
	
	AssetClassDescriptionLabel->SetPosition(Vector2f(500.0f, 220.0f));
	AssetClassDescriptionLabel->SetSize(Vector2f(150.0f, 30.0f));
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
		
		const VisualizationPrototype * VisualizationPrototype(g_ObjectFactory->GetVisualizationPrototype(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier(), _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier()));
		
		if(VisualizationPrototype != nullptr)
		{
			auto VisualizationNode(VisualizePrototype(VisualizationPrototype));
			
			if(VisualizationNode != nullptr)
			{
				float RadialSize(VisualizationPrototype->GetModel()->GetRadialSize());
				float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
				Graphics::PerspectiveProjection * PerspectiveProjection(new Graphics::PerspectiveProjection());
				
				PerspectiveProjection->SetFieldOfViewY(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)) * 2.0f);
				PerspectiveProjection->SetAspect(_AssetClassViewDisplay->GetSize()[0] / _AssetClassViewDisplay->GetSize()[1]);
				PerspectiveProjection->SetNearClippingPlane(0.1f);
				PerspectiveProjection->SetFarClippingPlane(100.0f);
				
				auto View(new Graphics::View());
				
				g_GraphicsEngine->AddView(View);
				View->SetClearColor(Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 0.0f));
				assert(View->GetCamera() != 0);
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
				
				Texture->Create(_AssetClassViewDisplay->GetSize()[0], _AssetClassViewDisplay->GetSize()[1], 1);
				
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
	
	auto Hangar(_Planet->GetHangar(_Character));
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
					auto InnerContent(*(Content->GetAspectObjectContainer()->GetContent().begin()));
					
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
