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
		TradeCenterWidget(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character);
	private:
		// callbacks
		bool _OnAssetClassMouseButton(UI::TradeCenterAssetClass * TradeCenterAssetClass, int Button, int State, float X, float Y);
		void _OnAssetClassMouseEnter(UI::TradeCenterAssetClass * AssetClassWidget);
		void _OnAssetClassMouseLeave(UI::TradeCenterAssetClass * AssetClassWidget);
		bool _OnAssetClassScrollBoxMouseButton(int Button, int State, float X, float Y);
		void _OnBuyButtonClicked(void);
		void _OnDestroying(void);
		void _OnDestroyInScene(Graphics::Node * Node);
		bool _OnKey(const KeyEventInformation & KeyEventInformation);
		void _OnSellButtonClicked(void);
		void _OnUpdating(float RealTimeSeconds, float GameTimeSeconds);
		// helper functions and actions
		void _Buy(const PlanetAssetClass * PlanetAssetClass);
		void _ClearAssetClassViewDisplay(void);
		void _Sell(const PlanetAssetClass * PlanetAssetClass);
		// member variables
		UI::ScrollBox * _AssetClassScrollBox;
		UI::ViewDisplay * _AssetClassViewDisplay;
		UI::Button * _BuyButton;
		Reference< Character > _Character;
		Reference< Planet > _Planet;
		UI::TradeCenterAssetClass * _SelectedTradeCenterAssetClass;
		UI::Button * _SellButton;
		UI::Label * _TraderAvailableSpaceLabel;
		UI::Label * _TraderCreditsLabel;
	};
}

#endif
