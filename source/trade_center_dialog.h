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

#include "clicked_listener.h"
#include "key_listener.h"
#include "mouse_button_listener.h"
#include "mouse_motion_listener.h"
#include "window.h"

class Button;
class Character;
class Label;
class Planet;
class TradeCenterAssetClass;

class TradeCenterDialog : public ClickedListener, public KeyListener, virtual public MouseButtonListener, virtual public MouseMotionListener, public WWindow
{
public:
	TradeCenterDialog(Widget * SupWidget, Planet * Planet, Character * Character);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, const KeyEventInformation & KeyEventInformation);
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
private:
	void Buy(const PlanetAssetClass * PlanetAssetClass);
	void Sell(const PlanetAssetClass * PlanetAssetClass);
	void UpdateTraderCredits(void);
	void UpdateTraderAvailableSpace(void);
	Planet * m_Planet;
	Character * m_Character;
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	Button * m_BuyButton;
	Label * m_BuyButtonLabel;
	Button * m_SellButton;
	Label * m_SellButtonLabel;
	Widget * m_AssetClassList;
	Widget * m_AssetClassListItems;
	Button * m_DownButton;
	Button * m_UpButton;
	Label * m_TraderCreditsLabel;
	Label * m_TraderAvailableSpaceLabel;
	TradeCenterAssetClass * m_SelectedTradeCenterAssetClass;
};

#endif
