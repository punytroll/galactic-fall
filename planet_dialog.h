#ifndef PLANET_DIALOG_H
#define PLANET_DIALOG_H

#include "clicked_listener.h"
#include "key_listener.h"
#include "window.h"

class Button;
class Character;
class Label;
class Planet;
class TradeCenterDialog;

class PlanetDialog : public ClickedListener, public KeyListener, public WWindow
{
public:
	PlanetDialog(Widget * SupWidget, Planet * Planet, Character * Character);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual void OnDestroy(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
private:
	Planet * m_Planet;
	Character * m_Character;
	Label * m_DescriptionLabel;
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	Button * m_TradeCenterButton;
	Label * m_TradeCenterLabel;
	Button * m_RefuelButton;
	Label * m_RefuelButtonLabel;
	TradeCenterDialog * m_TradeCenterDialog;
};

#endif
