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

#ifndef TRADE_CENTER_DIALOG_H
#define TRADE_CENTER_DIALOG_H

#include "mouse_button_listener.h"
#include "window.h"

class Button;
class Character;
class Label;
class Planet;
class ScrollBox;
class TradeCenterAssetClass;

class TradeCenterDialog : virtual public MouseButtonListener, public WWindow
{
public:
	TradeCenterDialog(Widget * SupWidget, Planet * Planet, Character * Character);
protected:
	// listeners
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
private:
	// callbacks
	void OnBuyClicked(void);
	bool OnKey(const KeyEventInformation & KeyEventInformation);
	void OnOKClicked(void);
	void OnSellClicked(void);
	void OnAssetClassMouseEnter(TradeCenterAssetClass * AssetClassWidget);
	void OnAssetClassMouseLeave(TradeCenterAssetClass * AssetClassWidget);
	// helper functions and actions
	void Buy(const PlanetAssetClass * PlanetAssetClass);
	void Sell(const PlanetAssetClass * PlanetAssetClass);
	void UpdateTraderCredits(void);
	void UpdateTraderAvailableSpace(void);
	// member variables
	Planet * m_Planet;
	Character * m_Character;
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	Button * m_BuyButton;
	Label * m_BuyButtonLabel;
	Button * m_SellButton;
	Label * m_SellButtonLabel;
	ScrollBox * m_AssetClassScrollBox;
	Label * m_TraderCreditsLabel;
	Label * m_TraderAvailableSpaceLabel;
	TradeCenterAssetClass * m_SelectedTradeCenterAssetClass;
};

#endif
