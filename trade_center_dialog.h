#ifndef TRADE_CENTER_DIALOG_H
#define TRADE_CENTER_DIALOG_H

#include "clicked_listener.h"
#include "key_listener.h"
#include "mouse_button_listener.h"
#include "mouse_motion_listener.h"
#include "window.h"

class Button;
class Character;
class Label;
class Planet;
class TradeCenterCommodity;

class TradeCenterDialog : public ClickedListener, public KeyListener, virtual public MouseButtonListener, virtual public MouseMotionListener, public WWindow
{
public:
	TradeCenterDialog(Widget * SupWidget, Planet * Planet, Character * Character);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
private:
	void UpdateTraderCredits(void);
	void UpdateTraderFreeCargoHoldSize(void);
	Planet * m_Planet;
	Character * m_Character;
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	Button * m_BuyButton;
	Label * m_BuyButtonLabel;
	Button * m_SellButton;
	Label * m_SellButtonLabel;
	Label * m_TraderCreditsLabel;
	Label * m_TraderFreeCargoHoldSizeLabel;
	TradeCenterCommodity * m_SelectedTradeCenterCommodity;
};

#endif
