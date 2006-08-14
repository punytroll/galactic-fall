#ifndef TRADE_CENTER_DIALOG_H
#define TRADE_CENTER_DIALOG_H

#include "clicked_listener.h"
#include "key_listener.h"
#include "mouse_button_listener.h"
#include "widget.h"

class Button;
class Label;
class Planet;
class TradeCenterCommodity;

class TradeCenterDialog : public ClickedListener, public KeyListener, public MouseButtonListener, public Widget
{
public:
	TradeCenterDialog(Widget * SupWidget, Planet * Planet);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
private:
	void UpdateTraderCredits(void);
	void UpdateTraderFreeCargoHoldSize(void);
	Planet * m_Planet;
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
