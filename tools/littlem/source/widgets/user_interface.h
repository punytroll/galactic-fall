#ifndef LITTLEM_WIDGETS_USER_INTERFACE_H
#define LITTLEM_WIDGETS_USER_INTERFACE_H

#include <vector>

#include "key_acceptor.h"

class Widget;

class UserInterface : public KeyAcceptor
{
public:
	UserInterface(void);
	virtual ~UserInterface(void);
	
	/**
	 * @brief Overloaded from KeyAcceptor::bAcceptKey this function is the top entry point for all key commands.
	 **/
	virtual bool bAcceptKey(int iKeyCode);
	void vGrabKeyFocus(KeyAcceptor * pKeyAcceptor);
	void vDelete(Widget * pWidget);
	void vSetSize(const Boxes::Size & Size);
	void vDraw(void);
	void vSetRootWidget(Widget * pWidget);
	Widget * pGetRootWidget(void);
private:
	bool bAcceptKeyAtKeyAcceptor(KeyAcceptor * pKeyAcceptor, int iKeyCode);
	Widget * m_pRootWidget;
	Boxes::Size m_Size;
};

#endif
