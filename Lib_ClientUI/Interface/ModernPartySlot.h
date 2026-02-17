#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"


class	CBasicTextBox;
class	CBasicButton;
class	CBasicTextButton;
class	CBasicProgressBar;
class	CModernPartySlotText;	


class	CModernPartySlot : public CUIGroup
{
protected:
	enum
	{
		 //= NO_ID + 1,
		
	};

public:
	CModernPartySlot ();
	virtual	~CModernPartySlot ();

public:
	void	CreateSubControl ();

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void	SetClass ( int nClassType, BOOL bSameMap );
	void	SetSchool ( int nSchoolType );
	void	SetLevel( CString strLevel );
	void	SetPlayerName ( CString strName );
	void	SetMapName ( CString strMapName );
	void	SetPlayerPos ( CString strPos );
	void	SetMaster( bool bMaster, int nSlotNumber );
	void	SetSelect ( bool bSelect );
	void	SetID( DWORD dwID )	{	m_dwGaeaID = dwID;	}
	DWORD	GetID()	{	return m_dwGaeaID;	}
	void	SetDie( bool bDIE );

protected:
	CUIControl*		CreateControl ( const char* szControl );

private:
	CUIControl*			m_pClassLive[GLCI_NUM_8CLASS];
	CUIControl*			m_pClassDead[GLCI_NUM_8CLASS];
	CUIControl*			m_pClassSelect;
	CUIControl*			m_pClassDIE;

	CModernPartySlotText*	m_pSlotText;
	CUIControl*			m_pSlotMaster;
	CUIControl*			m_pSlotSubMaster;
	CBasicTextBox*		m_pSlotNumText;

private:
	DWORD		m_dwGaeaID;
	CString		m_strName;

};