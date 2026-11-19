/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__TRADE_CENTER_WIDGET_H
#define GALACTIC_FALL__SOURCE__UI__TRADE_CENTER_WIDGET_H

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
    class ListBox;

    class TradeCenterWidget : public UI::Widget
    {
    public:
        TradeCenterWidget(Planet * Planet, Character * Character);
    private:
        // callbacks
        void _OnAssetClassDescriptionCaptionLabelUpdating(UI::Label * AssetClassDescriptionCaptionLabel, float RealTimeSeconds, float GameTimeSeconds);
        void _OnAssetClassDescriptionLabelUpdating(UI::Label * AssetClassDescriptionLabel, float RealTimeSeconds, float GameTimeSeconds);
        void _OnAssetClassListBoxSelectedItemChanged(void);
        void _OnAssetClassPriceCaptionLabelUpdating(UI::Label * AssetClassPriceCaptionLabel, float RealTimeSeconds, float GameTimeSeconds);
        void _OnAssetClassPriceLabelUpdating(UI::Label * AssetClassPriceLabel, float RealTimeSeconds, float GameTimeSeconds);
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
        void _Buy(const PlanetAssets * PlanetAssets);
        void _ClearAssetClassViewDisplay(void);
        void _Sell(const PlanetAssets * PlanetAssets);
        // member variables
        UI::ListBox * _AssetClassListBox;
        UI::ViewDisplay * _AssetClassViewDisplay;
        Character * _Character;
        Signals::Connection m_CharacterDestroyingConnection;
        Planet * _Planet;
        Signals::Connection m_PlanetDestroyingConnection;
    };
}

#endif
