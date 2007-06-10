#include "button.h"
#include "character.h"
#include "commodity.h"
#include "globals.h"
#include "label.h"
#include "planet.h"
#include "ship.h"
#include "string_cast.h"
#include "trade_center_dialog.h"

class TradeCenterCommodity : public Widget
{
public:
	TradeCenterCommodity(Widget * SupWidget, PlanetCommodity * PlanetCommodity, Ship * Ship);
	void UpdateCharacterAmount(void);
	const PlanetCommodity * GetPlanetCommodity(void) const;
private:
	PlanetCommodity * m_PlanetCommodity;
	Ship * m_Ship;
	Label * m_CharacterAmountLabel;
};

TradeCenterCommodity::TradeCenterCommodity(Widget * SupWidget, PlanetCommodity * PlanetCommodity, Ship * Ship) :
	Widget(SupWidget),
	m_PlanetCommodity(PlanetCommodity),
	m_Ship(Ship)
{
	Label * CommodityNameLabel(new Label(this, PlanetCommodity->GetCommodity()->GetName()));
	
	CommodityNameLabel->SetPosition(math3d::vector2f(10.0f, 0.0f));
	CommodityNameLabel->SetSize(math3d::vector2f(340.0f, 20.0f));
	CommodityNameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_CharacterAmountLabel = new Label(this, "");
	m_CharacterAmountLabel->SetPosition(math3d::vector2f(360.0f, 0.0f));
	m_CharacterAmountLabel->SetSize(math3d::vector2f(50.0f, 20.0f));
	m_CharacterAmountLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	
	Label * CommodityPriceLabel(new Label(this, to_string_cast(PlanetCommodity->GetPrice())));
	
	CommodityPriceLabel->SetPosition(math3d::vector2f(420.0f, 0.0f));
	CommodityPriceLabel->SetSize(math3d::vector2f(50.0f, 20.0f));
	CommodityPriceLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	CommodityPriceLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	UpdateCharacterAmount();
}

void TradeCenterCommodity::UpdateCharacterAmount(void)
{
	m_CharacterAmountLabel->SetString(to_string_cast(m_Ship->GetCommodityAmount(m_PlanetCommodity->GetCommodity())));
}

const PlanetCommodity * TradeCenterCommodity::GetPlanetCommodity(void) const
{
	return m_PlanetCommodity;
}

TradeCenterDialog::TradeCenterDialog(Widget * SupWidget, Planet * Planet, Character * Character) :
	WWindow(SupWidget, "Trade Center: " + Planet->GetName()),
	m_Planet(Planet),
	m_Character(Character),
	m_SelectedTradeCenterCommodity(0)
{
	SetPosition(math3d::vector2f(600.0f, 100.0f));
	SetSize(math3d::vector2f(500.0f, 330.0f));
	AddKeyListener(this);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(math3d::vector2f(390.0f, 300.0f));
	m_OKButton->SetSize(math3d::vector2f(100.0f, 20.0f));
	m_OKButton->AddClickedListener(this);
	m_OKButtonLabel = new Label(m_OKButton, "OK");
	m_OKButtonLabel->SetPosition(math3d::vector2f(0.0f, 0.0f));
	m_OKButtonLabel->SetSize(m_OKButton->GetSize());
	m_OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_BuyButton = new Button(this);
	m_BuyButton->SetPosition(math3d::vector2f(10.0f, 300.0f));
	m_BuyButton->SetSize(math3d::vector2f(100.0f, 20.0f));
	m_BuyButton->AddClickedListener(this);
	m_BuyButtonLabel = new Label(m_BuyButton, "Buy");
	m_BuyButtonLabel->SetPosition(math3d::vector2f(0.0f, 0.0f));
	m_BuyButtonLabel->SetSize(m_BuyButton->GetSize());
	m_BuyButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_BuyButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_SellButton = new Button(this);
	m_SellButton->SetPosition(math3d::vector2f(120.0f, 300.0f));
	m_SellButton->SetSize(math3d::vector2f(100.0f, 20.0f));
	m_SellButton->AddClickedListener(this);
	m_SellButtonLabel = new Label(m_SellButton, "Sell");
	m_SellButtonLabel->SetPosition(math3d::vector2f(0.0f, 0.0f));
	m_SellButtonLabel->SetSize(m_SellButton->GetSize());
	m_SellButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_SellButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	
	const std::vector< PlanetCommodity * > & PlanetCommodities(Planet->GetCommodities());
	std::vector< PlanetCommodity * >::const_iterator PlanetCommodityIterator(PlanetCommodities.begin());
	float Top(40.0f);
	
	while(PlanetCommodityIterator != PlanetCommodities.end())
	{
		TradeCenterCommodity * NewTradeCenterCommodity(new TradeCenterCommodity(this, *PlanetCommodityIterator, m_Character->GetShip()));
		
		NewTradeCenterCommodity->SetPosition(math3d::vector2f(10.0f, Top));
		NewTradeCenterCommodity->SetSize(math3d::vector2f(480.0f, 20.0f));
		NewTradeCenterCommodity->AddMouseButtonListener(this);
		NewTradeCenterCommodity->AddMouseMotionListener(this);
		Top += 20.0f;
		++PlanetCommodityIterator;
	}
	m_TraderCreditsLabel = new Label(this, "");
	m_TraderCreditsLabel->SetPosition(math3d::vector2f(10.0f, Top + 20.0f));
	m_TraderCreditsLabel->SetSize(math3d::vector2f(200.0f, 20.0f));
	m_TraderFreeCargoHoldSizeLabel = new Label(this, "");
	m_TraderFreeCargoHoldSizeLabel->SetPosition(math3d::vector2f(10.0f, Top + 40.0f));
	m_TraderFreeCargoHoldSizeLabel->SetSize(math3d::vector2f(200.0f, 20.0f));
	UpdateTraderCredits();
	UpdateTraderFreeCargoHoldSize();
}

void TradeCenterDialog::UpdateTraderCredits(void)
{
	m_TraderCreditsLabel->SetString("Credits: " + to_string_cast(m_Character->GetCredits()));
}

void TradeCenterDialog::UpdateTraderFreeCargoHoldSize(void)
{
	m_TraderFreeCargoHoldSizeLabel->SetString("Free Cargo Hold: " + to_string_cast(m_Character->GetShip()->GetFreeCargoHoldSize()));
}

bool TradeCenterDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		Destroy();
		
		return true;
	}
	else if(EventSource == m_BuyButton)
	{
		if(m_SelectedTradeCenterCommodity != 0)
		{
			float Price(m_SelectedTradeCenterCommodity->GetPlanetCommodity()->GetPrice());
			
			if(m_Character->RemoveCredits(Price) == true)
			{
				if(m_Character->GetShip()->AddCommodities(m_SelectedTradeCenterCommodity->GetPlanetCommodity()->GetCommodity(), 1.0f) == false)
				{
					m_Character->AddCredits(Price);
				}
				else
				{
					m_SelectedTradeCenterCommodity->UpdateCharacterAmount();
					UpdateTraderCredits();
					UpdateTraderFreeCargoHoldSize();
				}
			}
		}
	}
	else if(EventSource == m_SellButton)
	{
		if(m_SelectedTradeCenterCommodity != 0)
		{
			if(m_Character->GetShip()->RemoveCommodities(m_SelectedTradeCenterCommodity->GetPlanetCommodity()->GetCommodity(), 1.0f) == true)
			{
				m_Character->AddCredits(m_SelectedTradeCenterCommodity->GetPlanetCommodity()->GetPrice());
				m_SelectedTradeCenterCommodity->UpdateCharacterAmount();
				UpdateTraderCredits();
				UpdateTraderFreeCargoHoldSize();
			}
		}
	}
	
	return false;
}

bool TradeCenterDialog::OnKey(Widget * EventSource, int Key, int State)
{
	if(((Key == 9 /* ESCAPE */) || (Key == 36 /* RETURN */) || (Key == 28 /* T */)) && (State == EV_DOWN))
	{
		Destroy();
	}
	else if((Key == 56 /* B */) && (m_SelectedTradeCenterCommodity != 0) && (State == EV_DOWN))
	{
		float Price(m_SelectedTradeCenterCommodity->GetPlanetCommodity()->GetPrice());
		
		if(m_Character->RemoveCredits(Price) == true)
		{
			if(m_Character->GetShip()->AddCommodities(m_SelectedTradeCenterCommodity->GetPlanetCommodity()->GetCommodity(), 1.0f) == false)
			{
				m_Character->AddCredits(Price);
			}
			else
			{
				m_SelectedTradeCenterCommodity->UpdateCharacterAmount();
				UpdateTraderCredits();
				UpdateTraderFreeCargoHoldSize();
			}
		}
	}
	else if((Key == 39 /* S */) && (m_SelectedTradeCenterCommodity != 0) && (State == EV_DOWN))
	{
		if(m_Character->GetShip()->RemoveCommodities(m_SelectedTradeCenterCommodity->GetPlanetCommodity()->GetCommodity(), 1.0f) == true)
		{
			m_Character->AddCredits(m_SelectedTradeCenterCommodity->GetPlanetCommodity()->GetPrice());
			m_SelectedTradeCenterCommodity->UpdateCharacterAmount();
			UpdateTraderCredits();
			UpdateTraderFreeCargoHoldSize();
		}
	}
	
	return true;
}

bool TradeCenterDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if(WWindow::OnMouseButton(EventSource, Button, State, X, Y) == true)
	{
		return true;
	}
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		TradeCenterCommodity * SelectedTradeCenterCommodity(dynamic_cast< TradeCenterCommodity * >(EventSource));
		
		if(SelectedTradeCenterCommodity != 0)
		{
			if(m_SelectedTradeCenterCommodity != 0)
			{
				m_SelectedTradeCenterCommodity->SetBackgroundColor(Color(0.2f, 0.2f, 0.2f));
			}
			m_SelectedTradeCenterCommodity = SelectedTradeCenterCommodity;
			SelectedTradeCenterCommodity->SetBackgroundColor(Color(0.4f, 0.1f, 0.1f));
			
			return true;
		}
	}
	
	return false;
}

void TradeCenterDialog::OnMouseEnter(Widget * EventSource)
{
	WWindow::OnMouseEnter(EventSource);
	
	TradeCenterCommodity * EnteredTradeCenterCommodity(dynamic_cast< TradeCenterCommodity * >(EventSource));
	
	if((EnteredTradeCenterCommodity != 0) && (EnteredTradeCenterCommodity != m_SelectedTradeCenterCommodity))
	{
		EnteredTradeCenterCommodity->SetBackgroundColor(Color(0.3f, 0.2f, 0.2f));
	}
}

void TradeCenterDialog::OnMouseLeave(Widget * EventSource)
{
	WWindow::OnMouseLeave(EventSource);
	
	TradeCenterCommodity * LeftTradeCenterCommodity(dynamic_cast< TradeCenterCommodity * >(EventSource));
	
	if((LeftTradeCenterCommodity != 0) && (LeftTradeCenterCommodity != m_SelectedTradeCenterCommodity))
	{
		LeftTradeCenterCommodity->SetBackgroundColor(Color(0.2f, 0.2f, 0.2f));
	}
}
