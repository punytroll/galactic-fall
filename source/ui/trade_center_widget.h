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

#ifndef TRADE_CENTER_WIDGET_H
#define TRADE_CENTER_WIDGET_H

#include "widget.h"

class Character;
class Planet;

namespace Graphics
{
	class Node;
}

namespace UI
{
	class Button;
	class Label;
	class ViewDisplay;
	class ScrollBox;
	class TradeCenterAssetClass;

	class TradeCenterWidget : public UI::Widget
	{
	public:
		TradeCenterWidget(UI::Widget * SupWidget, Planet * Planet, Character * Character);
	private:
		// callbacks
		bool OnAssetClassMouseButton(UI::TradeCenterAssetClass * TradeCenterAssetClass, int Button, int State, float X, float Y);
		void OnAssetClassMouseEnter(UI::TradeCenterAssetClass * AssetClassWidget);
		void OnAssetClassMouseLeave(UI::TradeCenterAssetClass * AssetClassWidget);
		bool OnAssetClassScrollBoxMouseButton(int Button, int State, float X, float Y);
		void OnBuyClicked(void);
		bool OnKey(const KeyEventInformation & KeyEventInformation);
		void OnSellClicked(void);
		void _OnDestroying(void);
		void _OnDestroyInScene(Graphics::Node * Node);
		void _OnUpdating(float RealTimeSeconds, float GameTimeSeconds);
		// helper functions and actions
		void Buy(const PlanetAssetClass * PlanetAssetClass);
		void ClearAssetClassViewDisplay(void);
		void Sell(const PlanetAssetClass * PlanetAssetClass);
		void UpdateTraderCredits(void);
		void UpdateTraderAvailableSpace(void);
		// member variables
		Planet * m_Planet;
		Character * m_Character;
		UI::Button * m_BuyButton;
		UI::Button * m_SellButton;
		UI::ScrollBox * m_AssetClassScrollBox;
		UI::ViewDisplay * _AssetClassViewDisplay;
		UI::Label * m_TraderCreditsLabel;
		UI::Label * m_TraderAvailableSpaceLabel;
		UI::TradeCenterAssetClass * m_SelectedTradeCenterAssetClass;
	};
}

#endif
