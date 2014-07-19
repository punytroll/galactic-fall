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

#include "../asset_class.h"
#include "../character.h"
#include "../color.h"
#include "../commodity.h"
#include "../commodity_class.h"
#include "../game_time.h"
#include "../globals.h"
#include "../graphics/camera.h"
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
#include "../object_aspect_accessory.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../object_factory.h"
#include "../planet.h"
#include "../ship.h"
#include "../storage.h"
#include "../string_cast.h"
#include "../visualization_prototype.h"
#include "../visualizations.h"
#include "../weapon.h"
#include "../weapon_class.h"
#include "image.h"
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
		TradeCenterAssetClassListWidget(UI::Widget * SupWidget, PlanetAssetClass * PlanetAssetClass, Reference< Ship > Ship);
		const PlanetAssetClass * GetPlanetAssetClass(void) const;
	private:
		void _OnCharacterAmountLabelUpdating(float RealTimeSeconds, float GameTimeSeconds, UI::Label * CharacterAmountLabel);
		PlanetAssetClass * _PlanetAssetClass;
		Reference< Ship > _Ship;
	};
}

UI::TradeCenterAssetClassListWidget::TradeCenterAssetClassListWidget(UI::Widget * SupWidget, PlanetAssetClass * PlanetAssetClass, Reference< Ship > Ship) :
	UI::ListBoxItem(SupWidget),
	_PlanetAssetClass(PlanetAssetClass),
	_Ship(Ship)
{
	SetSize(Vector2f(200.0f, 20.0f));
	
	auto NameLabel(new UI::Label(this, PlanetAssetClass->GetAssetClass()->GetName()));
	
	NameLabel->SetPosition(Vector2f(10.0f, 0.0f));
	NameLabel->SetSize(Vector2f(30.0f, 20.0f));
	NameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	NameLabel->SetAnchorRight(true);
	
	auto CharacterAmountLabel(new UI::Label(this, ""));
	
	CharacterAmountLabel->SetPosition(Vector2f(50.0f, 0.0f));
	CharacterAmountLabel->SetSize(Vector2f(40.0f, 20.0f));
	CharacterAmountLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	CharacterAmountLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	CharacterAmountLabel->SetAnchorLeft(false);
	CharacterAmountLabel->SetAnchorRight(true);
	CharacterAmountLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterAssetClassListWidget::_OnCharacterAmountLabelUpdating, this, std::placeholders::_1, std::placeholders::_2, CharacterAmountLabel));
	
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

void UI::TradeCenterAssetClassListWidget::_OnCharacterAmountLabelUpdating(float RealTimeSeconds, float GameTimeSeconds, UI::Label * CharacterAmountLabel)
{
	assert(_Ship.IsValid() == true);
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	CharacterAmountLabel->SetText(to_string_cast(_Ship->GetCargoHold()->GetAspectObjectContainer()->GetAmount(_PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier(), _PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier())));
}

const PlanetAssetClass * UI::TradeCenterAssetClassListWidget::GetPlanetAssetClass(void) const
{
	return _PlanetAssetClass;
}

UI::TradeCenterWidget::TradeCenterWidget(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character) :
	UI::Widget(SupWidget),
	_Character(Character),
	_Planet(Planet),
	_SelectedTradeCenterAssetClassListWidget(nullptr)
{
	SetSize(Vector2f(650.0f, 300.0f));
	ConnectDestroyingCallback(std::bind(&UI::TradeCenterWidget::_OnDestroying, this, std::placeholders::_1));
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
	HeaderRow->SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
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
	_AssetClassScrollBox->ConnectMouseButtonCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassScrollBoxMouseButton, this, std::placeholders::_1));
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(Planet->GetPlanetAssetClasses());
	std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator(PlanetAssetClasses.begin());
	float Top(5.0f);
	
	while(PlanetAssetClassIterator != PlanetAssetClasses.end())
	{
		auto NewTradeCenterAssetClassListWidget(new UI::TradeCenterAssetClassListWidget(_AssetClassScrollBox->GetContent(), *PlanetAssetClassIterator, _Character->GetShip()->GetReference()));
		
		NewTradeCenterAssetClassListWidget->SetPosition(Vector2f(5.0f, Top));
		NewTradeCenterAssetClassListWidget->SetSize(Vector2f(_AssetClassScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
		NewTradeCenterAssetClassListWidget->SetAnchorRight(true);
		NewTradeCenterAssetClassListWidget->ConnectMouseButtonCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassMouseButton, this, std::placeholders::_1, NewTradeCenterAssetClassListWidget));
		Top += 25.0f;
		++PlanetAssetClassIterator;
	}
	_AssetClassScrollBox->GetContent()->SetSize(Vector2f(450.0f, Top));
	_AssetClassScrollBox->GetContent()->SetAnchorRight(true);
	_AssetClassViewDisplay = new UI::ViewDisplay(this);
	_AssetClassViewDisplay->SetPosition(Vector2f(500.0f, 0.0f));
	_AssetClassViewDisplay->SetSize(Vector2f(150.0f, 150.0f));
	_AssetClassViewDisplay->SetAnchorLeft(false);
	_AssetClassViewDisplay->SetAnchorRight(true);
	_AssetClassViewDisplay->SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
	
	auto TraderAvailableSpaceLabel = new UI::Label(this, "");
	
	TraderAvailableSpaceLabel->SetPosition(Vector2f(0.0f, 260.0f));
	TraderAvailableSpaceLabel->SetSize(Vector2f(200.0f, 20.0f));
	TraderAvailableSpaceLabel->SetAnchorBottom(true);
	TraderAvailableSpaceLabel->SetAnchorTop(false);
	TraderAvailableSpaceLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnTraderAvailableSpaceLabelUpdating, this, TraderAvailableSpaceLabel, std::placeholders::_1, std::placeholders::_2));
	
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
	assert(PlanetAssetClass != 0);
	assert(PlanetAssetClass->GetAssetClass() != 0);
	assert(_Character.IsValid() == true);
	if(_Character->RemoveCredits(PlanetAssetClass->GetPrice()) == true)
	{
		assert(_Character->GetShip() != 0);
		assert(_Character->GetShip()->GetCargoHold() != 0);
		if(_Character->GetShip()->GetCargoHold()->GetSpace() >= g_ObjectFactory->GetSpaceRequirement(PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier(), PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier()))
		{
			Object * NewCargo(g_ObjectFactory->Create(PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier(), PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier()));
			
			assert(NewCargo != 0);
			NewCargo->SetObjectIdentifier("::asset(" + PlanetAssetClass->GetAssetClass()->GetIdentifier() + ")::" + PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier() + "(" + PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier() + ")::created_on(" + _Planet->GetObjectIdentifier() + ")::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::bought_by(" + _Character->GetObjectIdentifier() + ")::created_at_address(" + to_string_cast(reinterpret_cast< void * >(NewCargo)) + ")");
			assert(_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer() != 0);
			_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer()->AddContent(NewCargo);
		}
		else
		{
			_Character->AddCredits(PlanetAssetClass->GetPrice());
		}
	}
}

void UI::TradeCenterWidget::_ClearAssetClassViewDisplay(void)
{
	Graphics::View * OldView(_AssetClassViewDisplay->GetView());
	
	if(OldView != 0)
	{
		_AssetClassViewDisplay->SetView(0);
		assert(OldView->GetScene() != 0);
		
		Graphics::Scene * Scene(OldView->GetScene());
		
		OldView->SetScene(0);
		delete Scene;
		
		assert(OldView->GetCamera() != 0);
		assert(OldView->GetCamera()->GetProjection() != 0);
		
		Graphics::Projection * Projection(OldView->GetCamera()->GetProjection());
		
		OldView->GetCamera()->SetProjection(0);
		delete Projection;
		
		Graphics::TextureRenderTarget * TextureRenderTarget(dynamic_cast< Graphics::TextureRenderTarget * >(OldView->GetRenderTarget()));
		
		assert(TextureRenderTarget != 0);
		OldView->SetRenderTarget(0);
		
		Graphics::Texture * Texture(TextureRenderTarget->GetTexture());
		
		TextureRenderTarget->SetTexture(0);
		delete TextureRenderTarget;
		g_GraphicsEngine->GetTextureManager()->Destroy(Texture->GetIdentifier());
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
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedTradeCenterAssetClassListWidget != nullptr)
		{
			_SelectedTradeCenterAssetClassListWidget->SetSelected(false);
		}
		_SelectedTradeCenterAssetClassListWidget = TradeCenterAssetClassListWidget;
		_SelectedTradeCenterAssetClassListWidget->SetSelected(true);
		_ClearAssetClassViewDisplay();
		
		const VisualizationPrototype * VisualizationPrototype(g_ObjectFactory->GetVisualizationPrototype(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier(), _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier()));
		
		if(VisualizationPrototype != 0)
		{
			Graphics::Node * VisualizationNode(VisualizePrototype(VisualizationPrototype));
			
			if(VisualizationNode != 0)
			{
				float RadialSize(VisualizationPrototype->GetModel()->GetRadialSize());
				float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
				Graphics::PerspectiveProjection * PerspectiveProjection(new Graphics::PerspectiveProjection());
				
				PerspectiveProjection->SetFieldOfViewY(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)) * 2.0f);
				PerspectiveProjection->SetAspect(_AssetClassViewDisplay->GetSize()[0] / _AssetClassViewDisplay->GetSize()[1]);
				PerspectiveProjection->SetNearClippingPlane(0.1f);
				PerspectiveProjection->SetFarClippingPlane(100.0f);
				
				Graphics::View * View(new Graphics::View());
				
				g_GraphicsEngine->AddView(View);
				View->SetClearColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
				assert(View->GetCamera() != 0);
				View->GetCamera()->SetProjection(PerspectiveProjection);
				View->GetCamera()->SetSpacialMatrix(Matrix4f::CreateFromTranslationVector(Vector3f(0.0f, -2.5f, 1.4f).Normalize() * 4.0f * RadialSize).RotateX(1.05f));
				
				Graphics::Scene * Scene(new Graphics::Scene());
				
				Scene->SetDestroyCallback(std::bind(&UI::TradeCenterWidget::_OnDestroyInScene, this, std::placeholders::_1));
				Scene->ActivateLight();
				assert(Scene->GetLight() != 0);
				Scene->GetLight()->SetPosition(-20.0f, -10.0f, 20.0f);
				Scene->GetLight()->SetDiffuseColor(1.0f, 1.0f, 1.0f, 0.0f);
				View->SetScene(Scene);
				
				Graphics::Texture * Texture(g_GraphicsEngine->GetTextureManager()->Create("trade-center-widget-commodity-view"));
				
				assert(Texture != 0);
				Texture->Create(_AssetClassViewDisplay->GetSize()[0], _AssetClassViewDisplay->GetSize()[1], 1);
				
				Graphics::TextureRenderTarget * RenderTarget(new Graphics::TextureRenderTarget());
				
				RenderTarget->SetTexture(Texture);
				View->SetRenderTarget(RenderTarget);
				Scene->SetRootNode(VisualizationNode);
				VisualizationNode->SetClearColorBuffer(true);
				VisualizationNode->SetClearDepthBuffer(true);
				VisualizationNode->SetUseLighting(true);
				VisualizationNode->SetUseDepthTest(true);
				_AssetClassViewDisplay->SetView(View);
			}
		}
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

void UI::TradeCenterWidget::_OnAssetClassScrollBoxMouseButton(UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::WheelUp) && (MouseButtonEvent.IsDown() == true))
	{
		_AssetClassScrollBox->GetVerticalScrollBar()->StepLess();
		MouseButtonEvent.StopPropagation();
	}
	else if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::WheelDown) && (MouseButtonEvent.IsDown() == true))
	{
		_AssetClassScrollBox->GetVerticalScrollBar()->StepMore();
		MouseButtonEvent.StopPropagation();
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
	if(_SelectedTradeCenterAssetClassListWidget != 0)
	{
		_Buy(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass());
	}
}

void UI::TradeCenterWidget::_OnBuyButtonUpdating(UI::Button * BuyButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	BuyButton->SetEnabled((_SelectedTradeCenterAssetClassListWidget != 0) && (_Character->GetCredits() >= _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetPrice()) && (_Character->GetShip() != 0) && (_Character->GetShip()->GetCargoHold()->GetSpace() >= g_ObjectFactory->GetSpaceRequirement(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier(), _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier())));
}

void UI::TradeCenterWidget::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_ClearAssetClassViewDisplay();
	}
}

void UI::TradeCenterWidget::_OnDestroyInScene(Graphics::Node * Node)
{
	delete Node;
}

void UI::TradeCenterWidget::_OnKey(UI::KeyEvent & KeyEvent)
{
	if((KeyEvent.GetPhase() == UI::Event::Phase::Bubbling) && (KeyEvent.GetKeyCode() == 56 /* B */) && (_SelectedTradeCenterAssetClassListWidget != 0) && (KeyEvent.IsDown() == true))
	{
		_Buy(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass());
	}
	else if((KeyEvent.GetPhase() == UI::Event::Phase::Bubbling) && (KeyEvent.GetKeyCode() == 39 /* S */) && (_SelectedTradeCenterAssetClassListWidget != 0) && (KeyEvent.IsDown() == true))
	{
		_Sell(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass());
	}
}

void UI::TradeCenterWidget::_OnSellButtonClicked(void)
{
	if(_SelectedTradeCenterAssetClassListWidget != 0)
	{
		_Sell(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass());
	}
}

void UI::TradeCenterWidget::_OnSellButtonUpdating(UI::Button * SellButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	
	bool Enabled(false);
	
	if((_SelectedTradeCenterAssetClassListWidget != 0) && (_Character->GetShip() != 0) && (_Character->GetShip()->GetCargoHold() != 0))
	{
		assert(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass() != 0);
		assert(_SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass() != 0);
		assert(_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer() != 0);
		
		const std::set< Object * > & Content(_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer()->GetContent());
		std::set< Object * >::const_iterator ContentIterator(Content.begin());
		
		while(ContentIterator != Content.end())
		{
			Object * Content(*ContentIterator);
			
			if((Content->GetTypeIdentifier() == _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier()) && (Content->GetClassIdentifier() == _SelectedTradeCenterAssetClassListWidget->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier()) && ((Content->GetAspectAccessory() == 0) || (Content->GetAspectAccessory()->GetSlot() == 0)))
			{
				Enabled = true;
				
				break;
			}
			++ContentIterator;
		}
	}
	SellButton->SetEnabled(Enabled);
}

void UI::TradeCenterWidget::_OnTraderAvailableSpaceLabelUpdating(UI::Label * TraderAvailableSpaceLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != nullptr);
	assert(_Character->GetShip()->GetCargoHold() != nullptr);
	assert(TraderAvailableSpaceLabel != nullptr);
	TraderAvailableSpaceLabel->SetText("Available Space: " + to_string_cast(0.001 * _Character->GetShip()->GetCargoHold()->GetSpace(), 3));
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
	assert(PlanetAssetClass != 0);
	assert(PlanetAssetClass->GetAssetClass() != 0);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	assert(_Character->GetShip()->GetCargoHold() != 0);
	assert(_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer() != 0);
	
	const std::set< Object * > & Content(_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer()->GetContent());
	std::set< Object * >::const_iterator ContentIterator(Content.begin());
	
	while(ContentIterator != Content.end())
	{
		Object * Content(*ContentIterator);
		
		if((Content->GetTypeIdentifier() == PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier()) && (Content->GetClassIdentifier() == PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier()) && ((Content->GetAspectAccessory() == 0) || (Content->GetAspectAccessory()->GetSlot() == 0)))
		{
			_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer()->RemoveContent(Content);
			delete Content;
			_Character->AddCredits(PlanetAssetClass->GetPrice());
			
			break;
		}
		++ContentIterator;
	}
}
