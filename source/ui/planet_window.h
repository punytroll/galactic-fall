/**
 * galactic-fall
 * Copyright (C) 2013-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__PLANET_WINDOW_H
#define GALACTIC_FALL__SOURCE__UI__PLANET_WINDOW_H

#include "window.h"

class Character;
class Planet;

namespace UI
{
    class Label;
    class HangarWidget;
    class ProgressBar;
    class TradeCenterWidget;
    
    class PlanetWindow : public UI::Window
    {
    public:
        PlanetWindow(Planet * Planet, Character * Character);
        
    private:
        void _OnCharacterDestroying(void);
        void _OnDestroying(UI::Event & DestroyingEvent);
        void _OnHangarButtonClicked(void);
        void _OnHomeButtonClicked(void);
        void _OnKey(UI::KeyEvent & KeyEvent);
        void _OnPlanetDestroying(void);
        void _OnTradeCenterButtonClicked(void);
        void _OnUpdateCreditsLabel(UI::Label * CreditsLabel, float RealTimeSeconds, float GameTimeSeconds);
        void _OnUpdateCycleProgressBar(UI::ProgressBar * CycleProgressBar, float RealTimeSeconds, float GameTimeSeconds);
        void _OpenHangar(void);
        void _OpenHomeScreen(void);
        void _OpenTradeCenter(void);
        
        Character * _Character;
        Connection _CharacterDestroyingConnection;
        UI::Label * _DescriptionLabel;
        UI::HangarWidget * _HangarWidget;
        Planet * _Planet;
        Connection _PlanetDestroyingConnection;
        UI::TradeCenterWidget * _TradeCenterWidget;
    };
}

#endif
