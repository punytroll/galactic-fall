#ifndef PLANET_DIALOG_H
#define PLANET_DIALOG_H

#include "clicked_listener.h"
#include "destroy_listener.h"
#include "key_listener.h"
#include "widget.h"

class Button;
class Label;
class Planet;
class TradeCenterDialog;

class PlanetDialog : public ClickedListener, public DestroyListener, public KeyListener, public Widget
{
public:
	PlanetDialog(Widget * SupWidget, Planet * Planet);
protected:
	virtual bool OnClicked(Widget * EventSource);
	virtual void OnDestroy(Widget * EventSource);
	virtual bool OnKey(Widget * EventSource, int Key, int State);
private:
	Planet * m_Planet;
	Label * m_NameLabel;
	Label * m_DescriptionLabel;
	Button * m_OKButton;
	Label * m_OKButtonLabel;
	Button * m_TradeCenterButton;
	Label * m_TradeCenterLabel;
	TradeCenterDialog * m_TradeCenterDialog;
};

#endif
