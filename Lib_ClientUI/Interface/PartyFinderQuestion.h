#pragma	once

#include "./UIWindow.h"

#include "../../Lib_Client/G-Logic/GLPartyFinderDef.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

class CBasicTextBox;
class CBasicTextButton;

class	CPartyFinderQuestion : public CUIWindow
{
protected:
	enum
	{
		MODAL_BUTTON_ACCEPT = ET_CONTROL_NEXT,
		MODAL_BUTTON_DECLINE,
	};

public:
	CPartyFinderQuestion ();
	virtual	~CPartyFinderQuestion ();

public:
	void	CreateBaseModal ( char* szWidowKeyword );
	void	CreateSubControl ();

public:
	void	SetCallerID ( UIGUID ControlID )		{ m_CallerID = ControlID; }
	UIGUID	GetCallerID ()							{ return m_CallerID; }
	void	SetQuestionData( SPARTY_FINDER_QUESTION* pData );
	void	ResetQuestionData();

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

protected:
	void	ResizeControl ( char* szWindowKeyword );

public:
	CBasicTextBox*	m_pTextBoxTitle;
	CBasicTextBox*	m_pTextBoxInfo1;
	CBasicTextBox*	m_pTextBoxInfo2;
	CBasicTextBox*	m_pTextBoxInfo3;

	CUIControl*		m_pSchoolImage[GLSCHOOL_NUM];
	CUIControl*		m_pClassImage[GLCI_NUM_8CLASS];

	CBasicTextButton*	m_pButtonAccept;
	CBasicTextButton*	m_pButtonCancel;

private:
    UIGUID	m_CallerID;

public:
	DWORD	m_dwCharID;
	DWORD	m_dwPartyID;

};