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
	class TradeCenterAssetClassListWidget;

	class TradeCenterWidget : public UI::Widget
	{
	public:
		TradeCenterWidget(UI::Widget * SupWidget, Planet * Planet, Character * Character);
	private:
		// callbacks
		void _OnAssetClassDescriptionCaptionLabelUpdating(UI::Label * AssetClassDescriptionCaptionLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _OnAssetClassDescriptionLabelUpdating(UI::Label * AssetClassDescriptionLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _OnAssetClassMouseButton(UI::MouseButtonEvent & MouseButtonEvent, UI::TradeCenterAssetClassListWidget * TradeCenterAssetClassListWidget);
		void _OnAssetClassMouseEnter(UI::Event & MouseEnterEvent, UI::TradeCenterAssetClassListWidget * TradeCenterAssetClassListWidget);
		void _OnAssetClassMouseLeave(UI::Event & MouseEnterEvent, UI::TradeCenterAssetClassListWidget * TradeCenterAssetClassListWidget);
		void _OnAssetClassPriceCaptionLabelUpdating(UI::Label * AssetClassPriceCaptionLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _OnAssetClassPriceLabelUpdating(UI::Label * AssetClassPriceLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _OnAssetClassScrollBoxMouseButton(UI::MouseButtonEvent & MouseButtonEvent);
		void _OnAssetClassSizeCaptionLabelUpdating(UI::Label * AssetClassSizeCaptionLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _OnAssetClassSizeLabelUpdating(UI::Label * AssetClassSizeLabel, float RealTimeSeconds, float GameTimeSeconds);
		void _OnBuyButtonClicked(void);
		void _OnBuyButtonUpdating(UI::Button * BuyButton, float RealTimeSeconds, float GameTimeSeconds);
		void _OnCharacterDestroying(void);
		void _OnDestroying(UI::Event & DestroyingEvent);
		void _OnDestroyInScene(Graphics::Node * Node);
		void _OnKey(UI::KeyEvent & KeyEvent);
		void _OnPlanetDestroying(void);
		void _OnSellButtonClicked(void);
		void _OnSellButtonUpdating(UI::Button * BuyButton, float RealTimeSeconds, float GameTimeSeconds);
		void _OnUpdating(float RealTimeSeconds, float GameTimeSeconds);
		// helper functions and actions
		void _Buy(const PlanetAssetClass * PlanetAssetClass);
		void _ClearAssetClassViewDisplay(void);
		void _Sell(const PlanetAssetClass * PlanetAssetClass);
		// member variables
		UI::ScrollBox * _AssetClassScrollBox;
		UI::ViewDisplay * _AssetClassViewDisplay;
		Character * _Character;
		Connection _CharacterDestroyingConnection;
		Planet * _Planet;
		Connection _PlanetDestroyingConnection;
		UI::TradeCenterAssetClassListWidget * _SelectedTradeCenterAssetClassListWidget;
	};
}

#endif
