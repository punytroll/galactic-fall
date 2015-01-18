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
	virtual bool AcceptKey(int iKeyCode, bool IsDown) override;
	void vGrabKeyFocus(KeyAcceptor * pKeyAcceptor);
private:
	bool _AcceptKeyAtKeyAcceptor(KeyAcceptor * pKeyAcceptor, int iKeyCode, bool IsDown);
};

#endif
