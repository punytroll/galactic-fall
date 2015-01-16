#ifndef LITTLEM_WIDGETS_KEYACCEPTOR_H
#define LITTLEM_WIDGETS_KEYACCEPTOR_H

#include <list>
#include <string>

class KeyAcceptor
{
public:
	KeyAcceptor(void);
	virtual ~KeyAcceptor(void);
	virtual bool AcceptKey(int iKeyCode, bool IsDown);
	KeyAcceptor * pGetKeyAcceptor(void);
	void vAddKeyAcceptor(KeyAcceptor * pKeyAcceptor);
	void vRemoveKeyAcceptor(KeyAcceptor * pKeyAcceptor);
	void vSetKeyAcceptor(KeyAcceptor * pKeyAcceptor);
private:
	KeyAcceptor * m_pParentKeyAcceptor;
	KeyAcceptor * m_pKeyAcceptor;
	std::list< KeyAcceptor * > m_KeyAcceptors;
};

#endif
