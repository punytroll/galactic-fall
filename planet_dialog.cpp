#include "button.h"
#include "character.h"
#include "color.h"
#include "globals.h"
#include "label.h"
#include "planet.h"
#include "planet_dialog.h"
#include "ship.h"
#include "string_cast.h"
#include "trade_center_dialog.h"

PlanetDialog::PlanetDialog(Widget * SupWidget, Planet * Planet) :
	WWindow(SupWidget, "Planet: " + Planet->GetName()),
	m_Planet(Planet),
	m_TradeCenterDialog(0)
{
	SetPosition(math3d::vector2f(50.0f, 50.0f));
	SetSize(math3d::vector2f(500.0f, 330.0f));
	AddKeyListener(this);
	m_DescriptionLabel = new Label(this, m_Planet->GetDescription());
	m_DescriptionLabel->SetPosition(math3d::vector2f(120.0f, 40.0f));
	m_DescriptionLabel->SetSize(math3d::vector2f(360.0f, 100.0f));
	m_DescriptionLabel->SetWrap(true);
	m_DescriptionLabel->SetWordWrap(true);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(math3d::vector2f(390.0f, 300.0f));
	m_OKButton->SetSize(math3d::vector2f(100.0f, 20.0f));
	m_OKButton->AddClickedListener(this);
	m_OKButtonLabel = new Label(m_OKButton, "OK");
	m_OKButtonLabel->SetPosition(math3d::vector2f(0.0f, 0.0f));
	m_OKButtonLabel->SetSize(m_OKButton->GetSize());
	m_OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_TradeCenterButton = new Button(this);
	m_TradeCenterButton->SetPosition(math3d::vector2f(10.0f, 40.0f));
	m_TradeCenterButton->SetSize(math3d::vector2f(100.0f, 20.0f));
	m_TradeCenterButton->AddClickedListener(this);
	m_TradeCenterLabel = new Label(m_TradeCenterButton, "Trade Center");
	m_TradeCenterLabel->SetPosition(math3d::vector2f(0.0f, 0.0f));
	m_TradeCenterLabel->SetSize(m_TradeCenterButton->GetSize());
	m_TradeCenterLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_TradeCenterLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	if(m_Planet->GetAllowRefuelling() == true)
	{
		m_RefuelButton = new Button(this);
		m_RefuelButton->SetPosition(math3d::vector2f(10.0f, 70.0f));
		m_RefuelButton->SetSize(math3d::vector2f(100.0f, 20.0f));
		m_RefuelButton->AddClickedListener(this);
		m_RefuelButtonLabel = new Label(m_RefuelButton, "Refuel");
		m_RefuelButtonLabel->SetPosition(math3d::vector2f(0.0f, 0.0f));
		m_RefuelButtonLabel->SetSize(m_TradeCenterButton->GetSize());
		m_RefuelButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
		m_RefuelButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
		
		Label * FuelPriceLabel = new Label(this, "Local fuel price is: " + to_string_cast(m_Planet->GetFuelPrice()) + " credits/unit.");
		
		FuelPriceLabel->SetPosition(math3d::vector2f(10.0f, 300.0f));
	}
}

bool PlanetDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		if(m_TradeCenterDialog == 0)
		{
			Destroy();
		}
		
		return true;
	}
	else if(EventSource == m_RefuelButton)
	{
		float FuelPrice(m_Planet->GetFuelPrice());
		float CanBuy(g_PlayerCharacter->GetCredits() / FuelPrice);
		float Need(g_PlayerShip->GetFuelCapacity() - g_PlayerShip->GetFuel());
		float Buy((CanBuy > Need) ? (Need) : (CanBuy));
		
		g_PlayerShip->SetFuel(g_PlayerShip->GetFuel() + Buy);
		g_PlayerCharacter->RemoveCredits(Buy * FuelPrice);
	}
	else if(EventSource == m_TradeCenterButton)
	{
		if(m_TradeCenterDialog == 0)
		{
			m_TradeCenterDialog = new TradeCenterDialog(GetRootWidget(), m_Planet);
			m_TradeCenterDialog->GrabKeyFocus();
			m_TradeCenterDialog->AddDestroyListener(this);
		}
		
		return true;
	}
	
	return false;
}

void PlanetDialog::OnDestroy(Widget * EventSource)
{
	WWindow::OnDestroy(EventSource);
	if(EventSource == m_TradeCenterDialog)
	{
		m_TradeCenterDialog = 0;
		GrabKeyFocus();
	}
}

bool PlanetDialog::OnKey(Widget * EventSource, int KeyCode, int State)
{
	if(((KeyCode == 9 /* ESCAPE */) || (KeyCode == 36 /* RETURN */)) && (State == EV_DOWN))
	{
		Destroy();
	}
	else if((KeyCode == 28 /* T */) && (State == EV_DOWN))
	{
		if(m_TradeCenterDialog == 0)
		{
			m_TradeCenterDialog = new TradeCenterDialog(GetRootWidget(), m_Planet);
			m_TradeCenterDialog->GrabKeyFocus();
			m_TradeCenterDialog->AddDestroyListener(this);
		}
	}
	// eat up all other keys
	return true;
}
