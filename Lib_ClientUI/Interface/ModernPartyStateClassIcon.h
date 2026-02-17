#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

class	CBasicTextBox;

class	CModernPartyStateClassIcon : public CUIGroup
{
protected:


public:
	CModernPartyStateClassIcon ();
	virtual	~CModernPartyStateClassIcon ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetClass ( int nClassType, BOOL bSameMap );
	void	SetDie( bool bDIE );
	void	SetMaster( bool bMaster );

protected:
	CUIControl*		CreateControl ( const char* szControl );

private:
	CUIControl*			m_pClassLive[GLCI_NUM_8CLASS];
	CUIControl*			m_pClassDead[GLCI_NUM_8CLASS];
	CUIControl*			m_pSlotMaster;
	CUIControl*			m_pSlotSubMaster;
	CUIControl*			m_pDIE;

private:

};