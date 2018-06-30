/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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

#include "../blueprint_manager.h"
#include "../character.h"
#include "../commodity.h"
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
#include "../planet_assets.h"
#include "../slot.h"
#include "../visualization_prototype.h"
#include "../visualizations.h"
#include "../weapon.h"
#include "key_event.h"
#include "label.h"
#include "list_box.h"
#include "list_box_item.h"
#include "text_button.h"
#include "trade_center_widget.h"
#include "view_display.h"

using namespace Expressions::Operators;

namespace UI
{
	class TradeCenterAssetsListBoxItem : public UI::ListBoxItem
	{
	public:
		TradeCenterAssetsListBoxItem(PlanetAssets * PlanetAssets, Hangar * Hangar) :
			_PlanetAssets(PlanetAssets),
			_Hangar(Hangar)
		{
			assert(_Hangar != nullptr);
			SetHeight(20.0_c);
			_HangarDestroyingConnection = _Hangar->ConnectDestroyingCallback(std::bind(&UI::TradeCenterAssetsListBoxItem::_OnHangarDestroying, this));
			ConnectDestroyingCallback(std::bind(&UI::TradeCenterAssetsListBoxItem::_OnDestroying, this, std::placeholders::_1));
			
			// create components
			auto HangarAmountLabel{new UI::Label{}};
			auto NameLabel{new UI::Label{}};
			auto PriceLabel{new UI::Label{}};
			auto SizeRequirementLabel{new UI::Label{}};
			
			// initialize components
			HangarAmountLabel->SetLeft(left(SizeRequirementLabel) - 5.0_c - width(HangarAmountLabel));
			HangarAmountLabel->SetTop(0.0_c);
			HangarAmountLabel->SetWidth(40.0_c);
			HangarAmountLabel->SetHeight(height(this));
			HangarAmountLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
			HangarAmountLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
			HangarAmountLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterAssetsListBoxItem::_OnHangarAmountLabelUpdating, this, std::placeholders::_1, std::placeholders::_2, HangarAmountLabel));
			NameLabel->SetLeft(5.0_c);
			NameLabel->SetTop(0.0_c);
			NameLabel->SetWidth(left(HangarAmountLabel) - 5.0_c - left(NameLabel));
			NameLabel->SetHeight(height(this));
			NameLabel->SetText(PlanetAssets->GetName());
			NameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
			PriceLabel->SetLeft(width(this) - 5.0_c - width(PriceLabel));
			PriceLabel->SetTop(0.0_c);
			PriceLabel->SetWidth(40.0_c);
			PriceLabel->SetHeight(height(this));
			PriceLabel->SetText(to_string_cast(PlanetAssets->GetPrice()));
			PriceLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
			PriceLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
			SizeRequirementLabel->SetLeft(left(PriceLabel) - 5.0_c - width(SizeRequirementLabel));
			SizeRequirementLabel->SetTop(0.0_c);
			SizeRequirementLabel->SetWidth(40.0_c);
			SizeRequirementLabel->SetHeight(height(this));
			SizeRequirementLabel->SetText(to_string_cast(0.001 * g_BlueprintManager->GetSpaceRequirement(PlanetAssets->GetTypeIdentifier(), PlanetAssets->GetSubTypeIdentifier()), 3));
			SizeRequirementLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
			SizeRequirementLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
			// add components
			AddSubWidget(HangarAmountLabel);
			AddSubWidget(NameLabel);
			AddSubWidget(PriceLabel);
			AddSubWidget(SizeRequirementLabel);
		}
		
		const PlanetAssets * GetPlanetAssets(void) const
		{
			return _PlanetAssets;
		}
	private:
		// event handlers
		void _OnHangarAmountLabelUpdating(float RealTimeSeconds, float GameTimeSeconds, UI::Label * HangarAmountLabel)
		{
			if(_Hangar != nullptr)
			{
				assert(_Hangar->GetAspectObjectContainer() != nullptr);
				HangarAmountLabel->SetVisible(true);
				HangarAmountLabel->SetText(to_string_cast(_Hangar->GetAspectObjectContainer()->GetAmount(_PlanetAssets->GetTypeIdentifier(), _PlanetAssets->GetSubTypeIdentifier())));
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
		
		PlanetAssets * _PlanetAssets;
		Hangar * _Hangar;
		Connection _HangarDestroyingConnection;
	};
}

UI::TradeCenterWidget::TradeCenterWidget(Planet * Planet, Character * Character) :
	_Character(Character),
	_Planet(Planet)
{
	assert(_Character != nullptr);
	_CharacterDestroyingConnection = _Character->ConnectDestroyingCallback(std::bind(&UI::TradeCenterWidget::_OnCharacterDestroying, this));
	assert(_Planet != nullptr);
	_PlanetDestroyingConnection = _Planet->ConnectDestroyingCallback(std::bind(&UI::TradeCenterWidget::_OnPlanetDestroying, this));
	ConnectDestroyingCallback(std::bind(&UI::TradeCenterWidget::_OnDestroying, this, std::placeholders::_1));
	ConnectKeyCallback(std::bind(&UI::TradeCenterWidget::_OnKey, this, std::placeholders::_1));
	ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnUpdating, this, std::placeholders::_1, std::placeholders::_2));
	
	// create components
	auto AssetClassDescriptionCaptionLabel{new UI::Label{}};
	auto AssetClassDescriptionLabel{new UI::Label{}};
	auto AssetClassPriceCaptionLabel{new UI::Label{}};
	auto AssetClassPriceLabel{new UI::Label{}};
	auto AssetClassSizeCaptionLabel{new UI::Label{}};
	auto AssetClassSizeLabel{new UI::Label{}};
	auto BuyButton{new UI::TextButton{}};
	auto CharacterAmountColumnHeader{new UI::Label{}};
	auto HeaderRow{new UI::Widget{}};
	auto NameColumnHeader{new UI::Label{}};
	auto PriceColumnHeader{new UI::Label{}};
	auto SellButton{new UI::TextButton{}};
	auto SizeColumnHeader{new UI::Label{}};
	
	_AssetClassListBox = new UI::ListBox{};
	_AssetClassViewDisplay = new UI::ViewDisplay{};
	
	// initialize components
	BuyButton->SetLeft(0.0_c);
	BuyButton->SetTop(height(this) - height(BuyButton));
	BuyButton->SetWidth(100.0_c);
	BuyButton->SetHeight(20.0_c);
	BuyButton->SetText("Buy");
	BuyButton->ConnectClickedCallback(std::bind(&UI::TradeCenterWidget::_OnBuyButtonClicked, this));
	BuyButton->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnBuyButtonUpdating, this, BuyButton, std::placeholders::_1, std::placeholders::_2));
	SellButton->SetLeft(width(BuyButton) + 10.0_c);
	SellButton->SetTop(height(this) - height(SellButton));
	SellButton->SetWidth(100.0_c);
	SellButton->SetHeight(20.0_c);
	SellButton->SetText("Sell");
	SellButton->ConnectClickedCallback(std::bind(&UI::TradeCenterWidget::_OnSellButtonClicked, this));
	SellButton->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnSellButtonUpdating, this, SellButton, std::placeholders::_1, std::placeholders::_2));
	HeaderRow->SetLeft(0.0_c);
	HeaderRow->SetTop(0.0_c);
	HeaderRow->SetWidth(width(this) - 180.0_c);
	HeaderRow->SetHeight(20.0_c);
	HeaderRow->SetBackgroundColor(Graphics::ColorRGBO(0.3f, 0.3f, 0.3f, 1.0f));
	PriceColumnHeader->SetLeft(width(HeaderRow) - 2.0_c - 5.0_c - width(PriceColumnHeader));
	PriceColumnHeader->SetTop(0.0_c);
	PriceColumnHeader->SetWidth(40.0_c);
	PriceColumnHeader->SetHeight(height(HeaderRow));
	PriceColumnHeader->SetText("Price");
	PriceColumnHeader->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	PriceColumnHeader->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	SizeColumnHeader->SetLeft(left(PriceColumnHeader) - 5.0_c - width(SizeColumnHeader));
	SizeColumnHeader->SetTop(0.0_c);
	SizeColumnHeader->SetWidth(40.0_c);
	SizeColumnHeader->SetHeight(height(HeaderRow));
	SizeColumnHeader->SetText("Size");
	SizeColumnHeader->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	SizeColumnHeader->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	CharacterAmountColumnHeader->SetLeft(left(SizeColumnHeader) - 5.0_c - width(CharacterAmountColumnHeader));
	CharacterAmountColumnHeader->SetTop(0.0_c);
	CharacterAmountColumnHeader->SetWidth(40.0_c);
	CharacterAmountColumnHeader->SetHeight(height(HeaderRow));
	CharacterAmountColumnHeader->SetText("Amount");
	CharacterAmountColumnHeader->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	CharacterAmountColumnHeader->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	NameColumnHeader->SetLeft(5.0_c + 2.0_c);
	NameColumnHeader->SetTop(0.0_c);
	NameColumnHeader->SetWidth(left(CharacterAmountColumnHeader) - 5.0_c - left(NameColumnHeader));
	NameColumnHeader->SetHeight(height(HeaderRow));
	NameColumnHeader->SetText("Name");
	NameColumnHeader->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Left);
	NameColumnHeader->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_AssetClassViewDisplay->SetLeft(width(this) - width(_AssetClassViewDisplay));
	_AssetClassViewDisplay->SetTop(0.0_c);
	_AssetClassViewDisplay->SetWidth(150.0_c);
	_AssetClassViewDisplay->SetHeight(150.0_c);
	_AssetClassViewDisplay->SetBackgroundColor(Graphics::ColorRGBO(0.15f, 0.15f, 0.15f, 1.0f));
	AssetClassPriceCaptionLabel->SetLeft(left(_AssetClassViewDisplay));
	AssetClassPriceCaptionLabel->SetTop(bottom(_AssetClassViewDisplay) + 10.0_c);
	AssetClassPriceCaptionLabel->SetWidth(width(_AssetClassViewDisplay));
	AssetClassPriceCaptionLabel->SetHeight(20.0_c);
	AssetClassPriceCaptionLabel->SetText("Price:");
	AssetClassPriceCaptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassPriceCaptionLabelUpdating, this, AssetClassPriceCaptionLabel, std::placeholders::_1, std::placeholders::_2));
	AssetClassPriceLabel->SetLeft(left(_AssetClassViewDisplay));
	AssetClassPriceLabel->SetTop(bottom(_AssetClassViewDisplay) + 10.0_c);
	AssetClassPriceLabel->SetWidth(width(_AssetClassViewDisplay));
	AssetClassPriceLabel->SetHeight(20.0_c);
	AssetClassPriceLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	AssetClassPriceLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassPriceLabelUpdating, this, AssetClassPriceLabel, std::placeholders::_1, std::placeholders::_2));
	AssetClassSizeCaptionLabel->SetLeft(left(_AssetClassViewDisplay));
	AssetClassSizeCaptionLabel->SetTop(bottom(AssetClassPriceLabel));
	AssetClassSizeCaptionLabel->SetWidth(width(_AssetClassViewDisplay));
	AssetClassSizeCaptionLabel->SetHeight(20.0_c);
	AssetClassSizeCaptionLabel->SetText("Size:");
	AssetClassSizeCaptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassSizeCaptionLabelUpdating, this, AssetClassSizeCaptionLabel, std::placeholders::_1, std::placeholders::_2));
	AssetClassSizeLabel->SetLeft(left(_AssetClassViewDisplay));
	AssetClassSizeLabel->SetTop(bottom(AssetClassPriceLabel));
	AssetClassSizeLabel->SetWidth(width(_AssetClassViewDisplay));
	AssetClassSizeLabel->SetHeight(20.0_c);
	AssetClassSizeLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	AssetClassSizeLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassSizeLabelUpdating, this, AssetClassSizeLabel, std::placeholders::_1, std::placeholders::_2));
	AssetClassDescriptionCaptionLabel->SetLeft(left(_AssetClassViewDisplay));
	AssetClassDescriptionCaptionLabel->SetTop(bottom(AssetClassSizeLabel));
	AssetClassDescriptionCaptionLabel->SetWidth(width(_AssetClassViewDisplay));
	AssetClassDescriptionCaptionLabel->SetHeight(20.0_c);
	AssetClassDescriptionCaptionLabel->SetText("Description:");
	AssetClassDescriptionCaptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassDescriptionCaptionLabelUpdating, this, AssetClassDescriptionCaptionLabel, std::placeholders::_1, std::placeholders::_2));
	AssetClassDescriptionLabel->SetLeft(left(_AssetClassViewDisplay));
	AssetClassDescriptionLabel->SetTop(bottom(AssetClassDescriptionCaptionLabel));
	AssetClassDescriptionLabel->SetWidth(width(_AssetClassViewDisplay));
	AssetClassDescriptionLabel->SetHeight(height(this) - top(AssetClassDescriptionLabel));
	AssetClassDescriptionLabel->SetWrap(true);
	AssetClassDescriptionLabel->SetWordWrap(true);
	AssetClassDescriptionLabel->ConnectUpdatingCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassDescriptionLabelUpdating, this, AssetClassDescriptionLabel, std::placeholders::_1, std::placeholders::_2));
	_AssetClassListBox->SetLeft(0.0_c);
	_AssetClassListBox->SetTop(bottom(HeaderRow));
	_AssetClassListBox->SetWidth(left(_AssetClassViewDisplay) - 10.0_c);
	_AssetClassListBox->SetHeight(top(BuyButton) - 10.0_c - top(_AssetClassListBox));
	_AssetClassListBox->SetHorizontalScrollBarVisible(false);
	_AssetClassListBox->ConnectSelectedItemChangedCallback(std::bind(&UI::TradeCenterWidget::_OnAssetClassListBoxSelectedItemChanged, this));
	AddSubWidget(_AssetClassListBox);
	
	auto Hangar{_Planet->GetHangar(_Character)};
	
	assert(Hangar != nullptr);
	
	std::vector< PlanetAssets * > SortedPlanetAssets{Planet->GetPlanetAssets()};
	
	std::sort(SortedPlanetAssets.begin(), SortedPlanetAssets.end(), [](PlanetAssets * One, PlanetAssets * Two)
																	{
																		return One->GetName() < Two->GetName();
																	});
	for(auto PlanetAssets : SortedPlanetAssets)
	{
		_AssetClassListBox->GetContent()->AddSubWidget(new UI::TradeCenterAssetsListBoxItem{PlanetAssets, Hangar});
	}
	// add components
	AddSubWidget(AssetClassDescriptionCaptionLabel);
	AddSubWidget(AssetClassDescriptionLabel);
	AddSubWidget(AssetClassPriceCaptionLabel);
	AddSubWidget(AssetClassPriceLabel);
	AddSubWidget(AssetClassSizeCaptionLabel);
	AddSubWidget(AssetClassSizeLabel);
	AddSubWidget(_AssetClassViewDisplay);
	AddSubWidget(BuyButton);
	AddSubWidget(HeaderRow);
	HeaderRow->AddSubWidget(CharacterAmountColumnHeader);
	HeaderRow->AddSubWidget(NameColumnHeader);
	HeaderRow->AddSubWidget(PriceColumnHeader);
	HeaderRow->AddSubWidget(SizeColumnHeader);
	AddSubWidget(SellButton);
}

void UI::TradeCenterWidget::_Buy(const PlanetAssets * PlanetAssets)
{
	assert(PlanetAssets != nullptr);
	assert(_Character != nullptr);
	assert(_Planet != nullptr);
	if(_Character->RemoveCredits(PlanetAssets->GetPrice()) == true)
	{
		auto NewCargo{g_ObjectFactory->Create(PlanetAssets->GetTypeIdentifier(), PlanetAssets->GetSubTypeIdentifier(), true)};
		
		assert(NewCargo != nullptr);
		NewCargo->SetObjectIdentifier("::" + PlanetAssets->GetTypeIdentifier() + "(" + PlanetAssets->GetSubTypeIdentifier() + ")::created_on(" + _Planet->GetObjectIdentifier() + ")::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::bought_by(" + _Character->GetObjectIdentifier() + ")::created_at_address(" + to_string_cast(reinterpret_cast< void * >(NewCargo)) + ")");
		
		auto Hangar{_Planet->GetHangar(_Character)};
		
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
	AssetClassDescriptionCaptionLabel->SetVisible(_AssetClassListBox->GetSelectedItem() != nullptr);
}

void UI::TradeCenterWidget::_OnAssetClassDescriptionLabelUpdating(UI::Label * AssetClassDescriptionLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	auto SelectedAssetClassListBoxItem{dynamic_cast< UI::TradeCenterAssetsListBoxItem * >(_AssetClassListBox->GetSelectedItem())};
	
	if(_AssetClassListBox->GetSelectedItem() != nullptr)
	{
		AssetClassDescriptionLabel->SetText(SelectedAssetClassListBoxItem->GetPlanetAssets()->GetDescription());
		AssetClassDescriptionLabel->SetVisible(true);
	}
	else
	{
		AssetClassDescriptionLabel->SetText("");
		AssetClassDescriptionLabel->SetVisible(false);
	}
}

void UI::TradeCenterWidget::_OnAssetClassListBoxSelectedItemChanged(void)
{
	auto SelectedAssetClassListBoxItem{dynamic_cast< UI::TradeCenterAssetsListBoxItem * >(_AssetClassListBox->GetSelectedItem())};
	
	_ClearAssetClassViewDisplay();
	if((SelectedAssetClassListBoxItem != nullptr) && (_AssetClassViewDisplay->GetWidth() > 0.0f) && (_AssetClassViewDisplay->GetHeight() > 0.0f))
	{
		auto VisualizationPrototype{SelectedAssetClassListBoxItem->GetPlanetAssets()->GetVisualizationPrototype()};
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

void UI::TradeCenterWidget::_OnAssetClassPriceCaptionLabelUpdating(UI::Label * AssetClassPriceCaptionLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(AssetClassPriceCaptionLabel != nullptr);
	AssetClassPriceCaptionLabel->SetVisible(_AssetClassListBox->GetSelectedItem() != nullptr);
}

void UI::TradeCenterWidget::_OnAssetClassPriceLabelUpdating(UI::Label * AssetClassPriceLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	auto SelectedAssetClassListBoxItem{dynamic_cast< UI::TradeCenterAssetsListBoxItem * >(_AssetClassListBox->GetSelectedItem())};
	
	assert(AssetClassPriceLabel != nullptr);
	if(SelectedAssetClassListBoxItem != nullptr)
	{
		AssetClassPriceLabel->SetText(to_string_cast(SelectedAssetClassListBoxItem->GetPlanetAssets()->GetPrice()));
		AssetClassPriceLabel->SetVisible(true);
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
	AssetClassSizeCaptionLabel->SetVisible(_AssetClassListBox->GetSelectedItem() != nullptr);
}

void UI::TradeCenterWidget::_OnAssetClassSizeLabelUpdating(UI::Label * AssetClassSizeLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	auto SelectedAssetClassListBoxItem{dynamic_cast< UI::TradeCenterAssetsListBoxItem * >(_AssetClassListBox->GetSelectedItem())};
	
	assert(AssetClassSizeLabel != nullptr);
	if(SelectedAssetClassListBoxItem != nullptr)
	{
		AssetClassSizeLabel->SetText(to_string_cast(0.001 * SelectedAssetClassListBoxItem->GetPlanetAssets()->GetSpaceRequirement(), 3));
		AssetClassSizeLabel->SetVisible(true);
	}
	else
	{
		AssetClassSizeLabel->SetText("");
		AssetClassSizeLabel->SetVisible(false);
	}
}

void UI::TradeCenterWidget::_OnBuyButtonClicked(void)
{
	auto SelectedAssetClassListBoxItem{dynamic_cast< UI::TradeCenterAssetsListBoxItem * >(_AssetClassListBox->GetSelectedItem())};
	
	if(SelectedAssetClassListBoxItem != nullptr)
	{
		_Buy(SelectedAssetClassListBoxItem->GetPlanetAssets());
	}
}

void UI::TradeCenterWidget::_OnBuyButtonUpdating(UI::Button * BuyButton, float RealTimeSeconds, float GameTimeSeconds)
{
	auto SelectedAssetClassListBoxItem{dynamic_cast< UI::TradeCenterAssetsListBoxItem * >(_AssetClassListBox->GetSelectedItem())};
	
	BuyButton->SetEnabled((SelectedAssetClassListBoxItem != nullptr) && (_Character != nullptr) && (_Character->GetCredits() >= SelectedAssetClassListBoxItem->GetPlanetAssets()->GetPrice()) && (_Planet != nullptr) && (_Planet->GetHangar(_Character) != nullptr));
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
	auto SelectedAssetClassListBoxItem{dynamic_cast< UI::TradeCenterAssetsListBoxItem * >(_AssetClassListBox->GetSelectedItem())};
	
	if((SelectedAssetClassListBoxItem != nullptr) && (KeyEvent.GetPhase() == UI::Event::Phase::Bubbling) && (KeyEvent.IsDown() == true))
	{
		if(KeyEvent.GetKeyCode() == 56 /* B */)
		{
			_Buy(SelectedAssetClassListBoxItem->GetPlanetAssets());
		}
		else if(KeyEvent.GetKeyCode() == 39 /* S */)
		{
			_Sell(SelectedAssetClassListBoxItem->GetPlanetAssets());
		}
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
	auto SelectedAssetClassListBoxItem{dynamic_cast< UI::TradeCenterAssetsListBoxItem * >(_AssetClassListBox->GetSelectedItem())};
	
	if(SelectedAssetClassListBoxItem != nullptr)
	{
		_Sell(SelectedAssetClassListBoxItem->GetPlanetAssets());
	}
}

void UI::TradeCenterWidget::_OnSellButtonUpdating(UI::Button * SellButton, float RealTimeSeconds, float GameTimeSeconds)
{
	auto SelectedAssetClassListBoxItem{dynamic_cast< UI::TradeCenterAssetsListBoxItem * >(_AssetClassListBox->GetSelectedItem())};
	bool Enabled(false);
	
	if((SelectedAssetClassListBoxItem != nullptr) && (_Character != nullptr) && (_Planet != nullptr))
	{
		assert(SelectedAssetClassListBoxItem->GetPlanetAssets() != nullptr);
		
		auto Hangar{_Planet->GetHangar(_Character)};
		
		assert(Hangar != nullptr);
		assert(Hangar->GetAspectObjectContainer() != nullptr);
		for(auto Content : Hangar->GetAspectObjectContainer()->GetContent())
		{
			if((Content->GetTypeIdentifier() == SelectedAssetClassListBoxItem->GetPlanetAssets()->GetTypeIdentifier()) && (Content->GetSubTypeIdentifier() == SelectedAssetClassListBoxItem->GetPlanetAssets()->GetSubTypeIdentifier()))
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

void UI::TradeCenterWidget::_Sell(const PlanetAssets * PlanetAssets)
{
	assert(PlanetAssets != nullptr);
	assert(_Character != nullptr);
	assert(_Planet != nullptr);
	
	auto Hangar{_Planet->GetHangar(_Character)};
	std::list< Object * > ToAdd;
	
	assert(Hangar != nullptr);
	assert(Hangar->GetAspectObjectContainer() != nullptr);
	for(auto Content : Hangar->GetAspectObjectContainer()->GetContent())
	{
		if((Content->GetTypeIdentifier() == PlanetAssets->GetTypeIdentifier()) && (Content->GetSubTypeIdentifier() == PlanetAssets->GetSubTypeIdentifier()))
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
			_Character->AddCredits(PlanetAssets->GetPrice());
			
			break;
		}
	}
	for(auto Object : ToAdd)
	{
		Hangar->GetAspectObjectContainer()->AddContent(Object);
	}
}
