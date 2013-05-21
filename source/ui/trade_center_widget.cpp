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
#include "../commodity.h"
#include "../commodity_class.h"
#include "../game_time.h"
#include "../globals.h"
#include "../graphics/camera.h"
#include "../graphics/engine.h"
#include "../graphics/light.h"
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
#include "button.h"
#include "image.h"
#include "label.h"
#include "scroll_bar.h"
#include "scroll_box.h"
#include "trade_center_widget.h"
#include "view_display.h"

namespace UI
{
	class TradeCenterAssetClass : public UI::Widget
	{
	public:
		TradeCenterAssetClass(UI::Widget * SupWidget, PlanetAssetClass * PlanetAssetClass, Ship * Ship);
		void UpdateCharacterAmount(void);
		const PlanetAssetClass * GetPlanetAssetClass(void) const;
	private:
		PlanetAssetClass * m_PlanetAssetClass;
		Ship * m_Ship;
		UI::Label * m_CharacterAmountLabel;
	};
}

UI::TradeCenterAssetClass::TradeCenterAssetClass(UI::Widget * SupWidget, PlanetAssetClass * PlanetAssetClass, Ship * Ship) :
	UI::Widget(SupWidget),
	m_PlanetAssetClass(PlanetAssetClass),
	m_Ship(Ship)
{
	UI::Label * PlanetAssetClassNameLabel(new UI::Label(this, PlanetAssetClass->GetAssetClass()->GetName()));
	
	PlanetAssetClassNameLabel->SetPosition(Vector2f(10.0f, 0.0f));
	PlanetAssetClassNameLabel->SetSize(Vector2f(0.0f, 20.0f));
	PlanetAssetClassNameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	PlanetAssetClassNameLabel->SetAnchorRight(true);
	m_CharacterAmountLabel = new UI::Label(this, "");
	m_CharacterAmountLabel->SetPosition(Vector2f(-110.0f, 0.0f));
	m_CharacterAmountLabel->SetSize(Vector2f(50.0f, 20.0f));
	m_CharacterAmountLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	m_CharacterAmountLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	m_CharacterAmountLabel->SetAnchorLeft(false);
	m_CharacterAmountLabel->SetAnchorRight(true);
	
	UI::Label * PlanetAssetClassPriceLabel(new UI::Label(this, to_string_cast(PlanetAssetClass->GetPrice())));
	
	PlanetAssetClassPriceLabel->SetPosition(Vector2f(-60.0f, 0.0f));
	PlanetAssetClassPriceLabel->SetSize(Vector2f(50.0f, 20.0f));
	PlanetAssetClassPriceLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	PlanetAssetClassPriceLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	PlanetAssetClassPriceLabel->SetAnchorLeft(false);
	PlanetAssetClassPriceLabel->SetAnchorRight(true);
	UpdateCharacterAmount();
}

void UI::TradeCenterAssetClass::UpdateCharacterAmount(void)
{
	m_CharacterAmountLabel->SetText(to_string_cast(m_Ship->GetCargoHold()->GetAmount(m_PlanetAssetClass->GetAssetClass()->GetObjectType(), m_PlanetAssetClass->GetAssetClass()->GetObjectClass())));
}

const PlanetAssetClass * UI::TradeCenterAssetClass::GetPlanetAssetClass(void) const
{
	return m_PlanetAssetClass;
}

UI::TradeCenterWidget::TradeCenterWidget(UI::Widget * SupWidget, Planet * Planet, Character * Character) :
	UI::Widget(SupWidget),
	m_Planet(Planet),
	m_Character(Character),
	m_SelectedTradeCenterAssetClass(0)
{
	SetSize(Vector2f(600.0f, 300.0f));
	ConnectDestroyingCallback(Callback(this, &TradeCenterWidget::_OnDestroying));
	ConnectKeyCallback(Callback(this, &TradeCenterWidget::OnKey));
	ConnectUpdatingCallback(Callback(this, &TradeCenterWidget::_OnUpdating));
	m_BuyButton = new UI::Button(this);
	m_BuyButton->SetPosition(Vector2f(0.0f, 280.0f));
	m_BuyButton->SetSize(Vector2f(100.0f, 20.0f));
	m_BuyButton->SetAnchorBottom(true);
	m_BuyButton->SetAnchorTop(false);
	m_BuyButton->ConnectClickedCallback(Callback(this, &TradeCenterWidget::OnBuyClicked));
	
	UI::Label * BuyButtonLabel(new UI::Label(m_BuyButton, "Buy"));
	
	BuyButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	BuyButtonLabel->SetSize(m_BuyButton->GetSize());
	BuyButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	BuyButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	m_SellButton = new UI::Button(this);
	m_SellButton->SetPosition(Vector2f(110.0f, 280.0f));
	m_SellButton->SetSize(Vector2f(100.0f, 20.0f));
	m_SellButton->SetAnchorBottom(true);
	m_SellButton->SetAnchorTop(false);
	m_SellButton->ConnectClickedCallback(Callback(this, &TradeCenterWidget::OnSellClicked));
	
	UI::Label * SellButtonLabel(new UI::Label(m_SellButton, "Sell"));
	
	SellButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	SellButtonLabel->SetSize(m_SellButton->GetSize());
	SellButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	SellButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	m_AssetClassScrollBox = new UI::ScrollBox(this);
	m_AssetClassScrollBox->SetPosition(Vector2f(0.0f, 0.0f));
	m_AssetClassScrollBox->SetSize(Vector2f(490.0f, 210.0f));
	m_AssetClassScrollBox->SetHorizontalScrollBarVisible(false);
	m_AssetClassScrollBox->SetAnchorRight(true);
	m_AssetClassScrollBox->SetAnchorBottom(true);
	m_AssetClassScrollBox->ConnectMouseButtonCallback(Callback(this, &TradeCenterWidget::OnAssetClassScrollBoxMouseButton));
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(Planet->GetPlanetAssetClasses());
	std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator(PlanetAssetClasses.begin());
	float Top(5.0f);
	
	while(PlanetAssetClassIterator != PlanetAssetClasses.end())
	{
		TradeCenterAssetClass * NewTradeCenterAssetClass(new TradeCenterAssetClass(m_AssetClassScrollBox->GetContent(), *PlanetAssetClassIterator, m_Character->GetShip()));
		
		NewTradeCenterAssetClass->SetPosition(Vector2f(5.0f, Top));
		NewTradeCenterAssetClass->SetSize(Vector2f(m_AssetClassScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
		NewTradeCenterAssetClass->SetAnchorRight(true);
		NewTradeCenterAssetClass->ConnectMouseButtonCallback(Bind1(Callback(this, &TradeCenterWidget::OnAssetClassMouseButton), NewTradeCenterAssetClass));
		NewTradeCenterAssetClass->ConnectMouseEnterCallback(Bind1(Callback(this, &TradeCenterWidget::OnAssetClassMouseEnter), NewTradeCenterAssetClass));
		NewTradeCenterAssetClass->ConnectMouseLeaveCallback(Bind1(Callback(this, &TradeCenterWidget::OnAssetClassMouseLeave), NewTradeCenterAssetClass));
		Top += 25.0f;
		++PlanetAssetClassIterator;
	}
	m_AssetClassScrollBox->GetContent()->SetSize(Vector2f(450.0f, Top));
	m_AssetClassScrollBox->GetContent()->SetAnchorRight(true);
	_AssetClassViewDisplay = new UI::ViewDisplay(this);
	_AssetClassViewDisplay->SetPosition(Vector2f(500.0f, 0.0f));
	_AssetClassViewDisplay->SetSize(Vector2f(100.0f, 100.0f));
	_AssetClassViewDisplay->SetAnchorLeft(false);
	_AssetClassViewDisplay->SetAnchorRight(true);
	m_TraderCreditsLabel = new UI::Label(this, "");
	m_TraderCreditsLabel->SetPosition(Vector2f(0.0f, 230.0f));
	m_TraderCreditsLabel->SetSize(Vector2f(200.0f, 20.0f));
	m_TraderCreditsLabel->SetAnchorBottom(true);
	m_TraderCreditsLabel->SetAnchorTop(false);
	m_TraderAvailableSpaceLabel = new UI::Label(this, "");
	m_TraderAvailableSpaceLabel->SetPosition(Vector2f(0.0f, 250.0f));
	m_TraderAvailableSpaceLabel->SetSize(Vector2f(200.0f, 20.0f));
	m_TraderAvailableSpaceLabel->SetAnchorBottom(true);
	m_TraderAvailableSpaceLabel->SetAnchorTop(false);
	UpdateTraderCredits();
	UpdateTraderAvailableSpace();
}

void UI::TradeCenterWidget::UpdateTraderCredits(void)
{
	m_TraderCreditsLabel->SetText("Credits: " + to_string_cast(m_Character->GetCredits()));
}

void UI::TradeCenterWidget::UpdateTraderAvailableSpace(void)
{
	m_TraderAvailableSpaceLabel->SetText("Available Space: " + to_string_cast(0.001 * m_Character->GetShip()->GetCargoHold()->GetSpace(), 3));
}

void UI::TradeCenterWidget::Buy(const PlanetAssetClass * PlanetAssetClass)
{
	u4byte Price(PlanetAssetClass->GetPrice());
	
	if(m_Character->RemoveCredits(Price) == true)
	{
		if(m_Character->GetShip()->GetCargoHold()->GetSpace() >= g_ObjectFactory->GetSpaceRequirement(PlanetAssetClass->GetAssetClass()->GetObjectType(), PlanetAssetClass->GetAssetClass()->GetObjectClass()))
		{
			Object * NewCargo(g_ObjectFactory->Create(PlanetAssetClass->GetAssetClass()->GetObjectType(), PlanetAssetClass->GetAssetClass()->GetObjectClass()));
			
			NewCargo->SetObjectIdentifier("::asset(" + PlanetAssetClass->GetAssetClass()->GetIdentifier() + ")::" + PlanetAssetClass->GetAssetClass()->GetObjectType() + "(" + PlanetAssetClass->GetAssetClass()->GetObjectClass() + ")::created_on(" + m_Planet->GetObjectIdentifier() + ")::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::bought_by(" + m_Character->GetObjectIdentifier() + ")::created_at_address(" + to_string_cast(reinterpret_cast< void * >(NewCargo)) + ")");
			assert(m_Character->GetShip()->GetAspectObjectContainer() != 0);
			m_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer()->AddContent(NewCargo);
			UpdateTraderCredits();
			UpdateTraderAvailableSpace();
		}
		else
		{
			m_Character->AddCredits(Price);
		}
	}
}

void UI::TradeCenterWidget::ClearAssetClassViewDisplay(void)
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

void UI::TradeCenterWidget::Sell(const PlanetAssetClass * PlanetAssetClass)
{
	assert(m_Character != 0);
	assert(m_Character->GetShip() != 0);
	assert(m_Character->GetShip()->GetCargoHold() != 0);
	assert(m_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer() != 0);
	
	const std::set< Object * > & Content(m_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer()->GetContent());
	std::set< Object * >::const_iterator ContentIterator(Content.begin());
	
	while(ContentIterator != Content.end())
	{
		Object * Content(*ContentIterator);
		
		if((Content->GetTypeIdentifier() == PlanetAssetClass->GetAssetClass()->GetObjectType()) && (Content->GetClassIdentifier() == PlanetAssetClass->GetAssetClass()->GetObjectClass()) && ((Content->GetAspectAccessory() == 0) || (Content->GetAspectAccessory()->GetSlot() == 0)))
		{
			m_Character->GetShip()->GetCargoHold()->GetAspectObjectContainer()->RemoveContent(Content);
			delete Content;
			m_Character->AddCredits(PlanetAssetClass->GetPrice());
			UpdateTraderCredits();
			UpdateTraderAvailableSpace();
			
			break;
		}
		++ContentIterator;
	}
}

void UI::TradeCenterWidget::OnBuyClicked(void)
{
	if(m_SelectedTradeCenterAssetClass != 0)
	{
		Buy(m_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
		m_SelectedTradeCenterAssetClass->UpdateCharacterAmount();
	}
}

void UI::TradeCenterWidget::OnSellClicked(void)
{
	if(m_SelectedTradeCenterAssetClass != 0)
	{
		Sell(m_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
		m_SelectedTradeCenterAssetClass->UpdateCharacterAmount();
	}
}

bool UI::TradeCenterWidget::OnKey(const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 56 /* B */) && (m_SelectedTradeCenterAssetClass != 0) && (KeyEventInformation.IsDown() == true))
	{
		Buy(m_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
		m_SelectedTradeCenterAssetClass->UpdateCharacterAmount();
	}
	else if((KeyEventInformation.GetKeyCode() == 39 /* S */) && (m_SelectedTradeCenterAssetClass != 0) && (KeyEventInformation.IsDown() == true))
	{
		Sell(m_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
		m_SelectedTradeCenterAssetClass->UpdateCharacterAmount();
	}
	
	return false;
}

bool UI::TradeCenterWidget::OnAssetClassMouseButton(TradeCenterAssetClass * TradeCenterAssetClass, int Button, int State, float X, float Y)
{
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		if(m_SelectedTradeCenterAssetClass != 0)
		{
			m_SelectedTradeCenterAssetClass->UnsetBackgroundColor();
		}
		m_SelectedTradeCenterAssetClass = TradeCenterAssetClass;
		m_SelectedTradeCenterAssetClass->SetBackgroundColor(Color(0.4f, 0.1f, 0.1f, 1.0f));
		ClearAssetClassViewDisplay();
		
		const VisualizationPrototype * VisualizationPrototype(g_ObjectFactory->GetVisualizationPrototype(m_SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetAssetClass()->GetObjectType(), m_SelectedTradeCenterAssetClass->GetPlanetAssetClass()->GetAssetClass()->GetObjectClass()));
		
		if(VisualizationPrototype != 0)
		{
			Graphics::Node * VisualizationNode(VisualizePrototype(VisualizationPrototype));
			
			if(VisualizationNode != 0)
			{
				Graphics::PerspectiveProjection * PerspectiveProjection(new Graphics::PerspectiveProjection());
				
				PerspectiveProjection->SetFieldOfView(0.32);
				PerspectiveProjection->SetAspect(_AssetClassViewDisplay->GetSize()[0] / _AssetClassViewDisplay->GetSize()[1]);
				PerspectiveProjection->SetNearClippingPlane(0.1f);
				PerspectiveProjection->SetFarClippingPlane(100.0f);
				
				Graphics::View * View(new Graphics::View());
				
				g_GraphicsEngine->AddView(View);
				assert(View->GetCamera() != 0);
				View->GetCamera()->SetProjection(PerspectiveProjection);
				
				Matrix4f SpacialMatrix(0.0f, -2.5f, 1.4f);
				
				SpacialMatrix.Transform(Quaternion(1.05f, Quaternion::InitializeRotationX));
				View->GetCamera()->SetSpacialMatrix(SpacialMatrix);
				
				Graphics::Scene * Scene(new Graphics::Scene());
				
				Scene->SetDestroyCallback(Callback(this, &UI::TradeCenterWidget::_OnDestroyInScene));
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

bool UI::TradeCenterWidget::OnAssetClassScrollBoxMouseButton(int Button, int State, float X, float Y)
{
	if((Button == 4 /* WHEEL_UP */) && (State == EV_DOWN))
	{
		m_AssetClassScrollBox->GetVerticalScrollBar()->StepLess();
		
		return true;
	}
	else if((Button == 5 /* WHEEL_DOWN */) && (State == EV_DOWN))
	{
		m_AssetClassScrollBox->GetVerticalScrollBar()->StepMore();
		
		return true;
	}
	
	return false;
}

void UI::TradeCenterWidget::OnAssetClassMouseEnter(TradeCenterAssetClass * AssetClassWidget)
{
	if(AssetClassWidget != m_SelectedTradeCenterAssetClass)
	{
		AssetClassWidget->SetBackgroundColor(Color(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void UI::TradeCenterWidget::OnAssetClassMouseLeave(TradeCenterAssetClass * AssetClassWidget)
{
	if(AssetClassWidget != m_SelectedTradeCenterAssetClass)
	{
		AssetClassWidget->UnsetBackgroundColor();
	}
}

void UI::TradeCenterWidget::_OnDestroying(void)
{
	ClearAssetClassViewDisplay();
}

void UI::TradeCenterWidget::_OnDestroyInScene(Graphics::Node * Node)
{
	delete Node;
}

void UI::TradeCenterWidget::_OnUpdating(float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_AssetClassViewDisplay != 0);
	if(_AssetClassViewDisplay->GetView() != 0)
	{
		assert(_AssetClassViewDisplay->GetView()->GetScene() != 0);
		assert(_AssetClassViewDisplay->GetView()->GetScene()->GetRootNode() != 0);
		_AssetClassViewDisplay->GetView()->GetScene()->GetRootNode()->SetOrientation(_AssetClassViewDisplay->GetView()->GetScene()->GetRootNode()->GetOrientation() * Quaternion(-RealTimeSeconds * M_PI / 4.0f, Quaternion::InitializeRotationZ));
	}
}
