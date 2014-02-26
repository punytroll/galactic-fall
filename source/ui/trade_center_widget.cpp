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
#include "../callbacks/callbacks.h"
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
#include "../key_event_information.h"
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
#include "label.h"
#include "scroll_bar.h"
#include "scroll_box.h"
#include "text_button.h"
#include "trade_center_widget.h"
#include "view_display.h"

namespace UI
{
	class TradeCenterAssetClass : public UI::Widget
	{
	public:
		TradeCenterAssetClass(UI::Widget * SupWidget, PlanetAssetClass * PlanetAssetClass, Reference< Ship > Ship);
		const PlanetAssetClass * GetPlanetAssetClass(void) const;
	private:
		void _OnUpdating(float RealTimeSeconds, float GameTimeSeconds);
		PlanetAssetClass * _PlanetAssetClass;
		Reference< Ship > _Ship;
		UI::Label * _CharacterAmountLabel;
	};
}

UI::TradeCenterAssetClass::TradeCenterAssetClass(UI::Widget * SupWidget, PlanetAssetClass * PlanetAssetClass, Reference< Ship > Ship) :
	UI::Widget(SupWidget),
	_PlanetAssetClass(PlanetAssetClass),
	_Ship(Ship)
{
	ConnectUpdatingCallback(Callback(this, &UI::TradeCenterAssetClass::_OnUpdating));
	
	UI::Label * PlanetAssetClassNameLabel(new UI::Label(this, PlanetAssetClass->GetAssetClass()->GetName()));
	
	PlanetAssetClassNameLabel->SetPosition(Vector2f(10.0f, 0.0f));
	PlanetAssetClassNameLabel->SetSize(Vector2f(0.0f, 20.0f));
	PlanetAssetClassNameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	PlanetAssetClassNameLabel->SetAnchorRight(true);
	_CharacterAmountLabel = new UI::Label(this, "");
	_CharacterAmountLabel->SetPosition(Vector2f(-110.0f, 0.0f));
	_CharacterAmountLabel->SetSize(Vector2f(50.0f, 20.0f));
	_CharacterAmountLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	_CharacterAmountLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	_CharacterAmountLabel->SetAnchorLeft(false);
	_CharacterAmountLabel->SetAnchorRight(true);
	
	UI::Label * PlanetAssetClassPriceLabel(new UI::Label(this, to_string_cast(PlanetAssetClass->GetPrice())));
	
	PlanetAssetClassPriceLabel->SetPosition(Vector2f(-60.0f, 0.0f));
	PlanetAssetClassPriceLabel->SetSize(Vector2f(50.0f, 20.0f));
	PlanetAssetClassPriceLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	PlanetAssetClassPriceLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	PlanetAssetClassPriceLabel->SetAnchorLeft(false);
	PlanetAssetClassPriceLabel->SetAnchorRight(true);
}

void UI::TradeCenterAssetClass::_OnUpdating(float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Ship.IsValid() == true);
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	_CharacterAmountLabel->SetText(to_string_cast(_Ship->GetCargoHold()->GetAspectObjectContainer()->GetAmount(_PlanetAssetClass->GetAssetClass()->GetObjectTypeIdentifier(), _PlanetAssetClass->GetAssetClass()->GetObjectClassIdentifier())));
}

const PlanetAssetClass * UI::TradeCenterAssetClass::GetPlanetAssetClass(void) const
{
	return _PlanetAssetClass;
}

UI::TradeCenterWidget::TradeCenterWidget(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character) :
	UI::Widget(SupWidget),
	_Character(Character),
	_Planet(Planet),
	_SelectedTradeCenterAssetClass(0)
{
	SetSize(Vector2f(600.0f, 300.0f));
	ConnectDestroyingCallback(Callback(this, &UI::TradeCenterWidget::_OnDestroying));
	ConnectKeyCallback(Callback(this, &UI::TradeCenterWidget::_OnKey));
	ConnectUpdatingCallback(Callback(this, &UI::TradeCenterWidget::_OnUpdating));
	
	UI::Button * BuyButton(new UI::TextButton(this, "Buy"));
	
	BuyButton->SetPosition(Vector2f(0.0f, 280.0f));
	BuyButton->SetSize(Vector2f(100.0f, 20.0f));
	BuyButton->SetAnchorBottom(true);
	BuyButton->SetAnchorTop(false);
	BuyButton->ConnectClickedCallback(Callback(this, &UI::TradeCenterWidget::_OnBuyButtonClicked));
	BuyButton->ConnectUpdatingCallback(Bind1(Callback(this, &UI::TradeCenterWidget::_OnBuyButtonUpdating), BuyButton));
	
	UI::Button * SellButton(new UI::TextButton(this, "Sell"));
	
	SellButton->SetPosition(Vector2f(110.0f, 280.0f));
	SellButton->SetSize(Vector2f(100.0f, 20.0f));
	SellButton->SetAnchorBottom(true);
	SellButton->SetAnchorTop(false);
	SellButton->ConnectClickedCallback(Callback(this, &UI::TradeCenterWidget::_OnSellButtonClicked));
	SellButton->ConnectUpdatingCallback(Bind1(Callback(this, &UI::TradeCenterWidget::_OnSellButtonUpdating), SellButton));
	_AssetClassScrollBox = new UI::ScrollBox(this);
	_AssetClassScrollBox->SetPosition(Vector2f(0.0f, 0.0f));
	_AssetClassScrollBox->SetSize(Vector2f(490.0f, 210.0f));
	_AssetClassScrollBox->SetHorizontalScrollBarVisible(false);
	_AssetClassScrollBox->SetAnchorRight(true);
	_AssetClassScrollBox->SetAnchorBottom(true);
	_AssetClassScrollBox->ConnectMouseButtonCallback(Callback(this, &UI::TradeCenterWidget::_OnAssetClassScrollBoxMouseButton));
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(Planet->GetPlanetAssetClasses());
	std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator(PlanetAssetClasses.begin());
	float Top(5.0f);
	
	while(PlanetAssetClassIterator != PlanetAssetClasses.end())
	{
		TradeCenterAssetClass * NewTradeCenterAssetClass(new UI::TradeCenterAssetClass(_AssetClassScrollBox->GetContent(), *PlanetAssetClassIterator, _Character->GetShip()->GetReference()));
		
		NewTradeCenterAssetClass->SetPosition(Vector2f(5.0f, Top));
		NewTradeCenterAssetClass->SetSize(Vector2f(_AssetClassScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
		NewTradeCenterAssetClass->SetAnchorRight(true);
		NewTradeCenterAssetClass->ConnectMouseButtonCallback(Bind1(Callback(this, &UI::TradeCenterWidget::_OnAssetClassMouseButton), NewTradeCenterAssetClass));
		NewTradeCenterAssetClass->ConnectMouseEnterCallback(Bind1(Callback(this, &UI::TradeCenterWidget::_OnAssetClassMouseEnter), NewTradeCenterAssetClass));
		NewTradeCenterAssetClass->ConnectMouseLeaveCallback(Bind1(Callback(this, &UI::TradeCenterWidget::_OnAssetClassMouseLeave), NewTradeCenterAssetClass));
		Top += 25.0f;
		++PlanetAssetClassIterator;
	}
	_AssetClassScrollBox->GetContent()->SetSize(Vector2f(450.0f, Top));
	_AssetClassScrollBox->GetContent()->SetAnchorRight(true);
	_AssetClassViewDisplay = new UI::ViewDisplay(this);
	_AssetClassViewDisplay->SetPosition(Vector2f(500.0f, 0.0f));
	_AssetClassViewDisplay->SetSize(Vector2f(100.0f, 100.0f));
	_AssetClassViewDisplay->SetAnchorLeft(false);
	_AssetClassViewDisplay->SetAnchorRight(true);
	_AssetClassViewDisplay->SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
	_TraderCreditsLabel = new UI::Label(this, "");
	_TraderCreditsLabel->SetPosition(Vector2f(0.0f, 230.0f));
	_TraderCreditsLabel->SetSize(Vector2f(200.0f, 20.0f));
	_TraderCreditsLabel->SetAnchorBottom(true);
	_TraderCreditsLabel->SetAnchorTop(false);
	_TraderAvailableSpaceLabel = new UI::Label(this, "");
	_TraderAvailableSpaceLabel->SetPosition(Vector2f(0.0f, 250.0f));
	_TraderAvailableSpaceLabel->SetSize(Vector2f(200.0f, 20.0f));
	_TraderAvailableSpaceLabel->SetAnchorBottom(true);
	_TraderAvailableSpaceLabel->SetAnchorTop(false);
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

bool UI::TradeCenterWidget::_OnAssetClassMouseButton(TradeCenterAssetClass * TradeCenterAssetClass, int Button, int State, float X, float Y)
{
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		if(_SelectedTradeCenterAssetClass != 0)
		{
			_SelectedTradeCenterAssetClass->UnsetBackgroundColor();
		}
		_SelectedTradeCenterAssetClass = TradeCenterAssetClass;
		_SelectedTradeCenterAssetClass->SetBackgroundColor(Color(0.4f, 0.1f, 0.1f, 1.0f));
		_ClearAssetClassViewDisplay();
		
		const VisualizationPrototype * VisualizationPrototype(g_ObjectFactory->GetVisualizationPrototype(_SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier(), _SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier()));
		
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
		
		return true;
	}
	
	return false;
}

void UI::TradeCenterWidget::_OnAssetClassMouseEnter(TradeCenterAssetClass * AssetClassWidget)
{
	if(AssetClassWidget != _SelectedTradeCenterAssetClass)
	{
		AssetClassWidget->SetBackgroundColor(Color(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void UI::TradeCenterWidget::_OnAssetClassMouseLeave(TradeCenterAssetClass * AssetClassWidget)
{
	if(AssetClassWidget != _SelectedTradeCenterAssetClass)
	{
		AssetClassWidget->UnsetBackgroundColor();
	}
}

bool UI::TradeCenterWidget::_OnAssetClassScrollBoxMouseButton(int Button, int State, float X, float Y)
{
	if((Button == 4 /* WHEEL_UP */) && (State == EV_DOWN))
	{
		_AssetClassScrollBox->GetVerticalScrollBar()->StepLess();
		
		return true;
	}
	else if((Button == 5 /* WHEEL_DOWN */) && (State == EV_DOWN))
	{
		_AssetClassScrollBox->GetVerticalScrollBar()->StepMore();
		
		return true;
	}
	
	return false;
}

void UI::TradeCenterWidget::_OnBuyButtonClicked(void)
{
	if(_SelectedTradeCenterAssetClass != 0)
	{
		_Buy(_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
	}
}

void UI::TradeCenterWidget::_OnBuyButtonUpdating(UI::Button * BuyButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	BuyButton->SetEnabled((_SelectedTradeCenterAssetClass != 0) && (_Character->GetCredits() >= _SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetPrice()) && (_Character->GetShip() != 0) && (_Character->GetShip()->GetCargoHold()->GetSpace() >= g_ObjectFactory->GetSpaceRequirement(_SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier(), _SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier())));
}

void UI::TradeCenterWidget::_OnDestroying(void)
{
	_ClearAssetClassViewDisplay();
}

void UI::TradeCenterWidget::_OnDestroyInScene(Graphics::Node * Node)
{
	delete Node;
}

bool UI::TradeCenterWidget::_OnKey(const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 56 /* B */) && (_SelectedTradeCenterAssetClass != 0) && (KeyEventInformation.IsDown() == true))
	{
		_Buy(_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
	}
	else if((KeyEventInformation.GetKeyCode() == 39 /* S */) && (_SelectedTradeCenterAssetClass != 0) && (KeyEventInformation.IsDown() == true))
	{
		_Sell(_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
	}
	
	return false;
}

void UI::TradeCenterWidget::_OnSellButtonClicked(void)
{
	if(_SelectedTradeCenterAssetClass != 0)
	{
		_Sell(_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
	}
}

void UI::TradeCenterWidget::_OnSellButtonUpdating(UI::Button * SellButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	
	bool Enabled(false);
	
	if((_SelectedTradeCenterAssetClass != 0) && (_Character->GetShip() != 0) && (_Character->GetShip()->GetCargoHold() != 0))
	{
		assert(_SelectedTradeCenterAssetClass->GetPlanetAssetClass() != 0);
		assert(_SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetAssetClass() != 0);
		assert(_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer() != 0);
		
		const std::set< Object * > & Content(_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer()->GetContent());
		std::set< Object * >::const_iterator ContentIterator(Content.begin());
		
		while(ContentIterator != Content.end())
		{
			Object * Content(*ContentIterator);
			
			if((Content->GetTypeIdentifier() == _SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetAssetClass()->GetObjectTypeIdentifier()) && (Content->GetClassIdentifier() == _SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetAssetClass()->GetObjectClassIdentifier()) && ((Content->GetAspectAccessory() == 0) || (Content->GetAspectAccessory()->GetSlot() == 0)))
			{
				Enabled = true;
				
				break;
			}
			++ContentIterator;
		}
	}
	SellButton->SetEnabled(Enabled);
}

void UI::TradeCenterWidget::_OnUpdating(float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_TraderCreditsLabel != 0);
	_TraderCreditsLabel->SetText("Credits: " + to_string_cast(_Character->GetCredits()));
	assert(_Character->GetShip() != 0);
	assert(_Character->GetShip()->GetCargoHold() != 0);
	assert(_TraderAvailableSpaceLabel != 0);
	_TraderAvailableSpaceLabel->SetText("Available Space: " + to_string_cast(0.001 * _Character->GetShip()->GetCargoHold()->GetSpace(), 3));
	assert(_AssetClassViewDisplay != 0);
	if(_AssetClassViewDisplay->GetView() != 0)
	{
		assert(_AssetClassViewDisplay->GetView()->GetScene() != 0);
		assert(_AssetClassViewDisplay->GetView()->GetScene()->GetRootNode() != 0);
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
