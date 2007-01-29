#include "button.h"
#include "character.h"
#include "color.h"
#include "globals.h"
#include "label.h"
#include "planet.h"
#include "planet_dialog.h"
#include "ship.h"
#include "trade_center_dialog.h"

PlanetDialog::PlanetDialog(Widget * SupWidget, Planet * Planet) :
	Widget(SupWidget),
	m_Planet(Planet),
	m_TradeCenterDialog(0)
{
	SetPosition(math3d::vector2f(50.0f, 50.0f));
	SetSize(math3d::vector2f(500.0f, 330.0f));
	SetBackgroundColor(Color(0.2f, 0.2f, 0.2f));
	AddKeyListener(this);
	
	Label * TitleLabel(new Label(this, "Planet: " + m_Planet->GetName()));
	
	TitleLabel->SetPosition(math3d::vector2f(10.0f, 10.0f));
	TitleLabel->SetSize(math3d::vector2f(480.0f, 20.0f));
	TitleLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	TitleLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	TitleLabel->SetBackgroundColor(Color(0.2f, 0.2f, 0.4f));
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
		// fixed fuel price as of now: 130.0f
		float CanBuy(g_PlayerCharacter->GetCredits() / 130.0f);
		float Need(g_PlayerShip->GetFuelCapacity() - g_PlayerShip->GetFuel());
		float Buy((CanBuy > Need) ? (Need) : (CanBuy));
		
		g_PlayerShip->Refuel(Buy);
		g_PlayerCharacter->RemoveCredits(Buy * 130.0f);
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
	Widget::OnDestroy(EventSource);
	if(EventSource == m_TradeCenterDialog)
	{
		m_TradeCenterDialog = 0;
		GrabKeyFocus();
	}
}

bool PlanetDialog::OnKey(Widget * EventSource, int KeyCode, int State)
{
	if(((KeyCode == 9 /* ESCAPE */) || (KeyCode == 36 /* RETURN */) || (KeyCode == 46 /* L */)) && (State == EV_DOWN))
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
