#pragma once

#include "../../Lib_Engine/G-Logic/GLDefine.h"

#include "./UIWindowEx.h"

class CBasicTextButton;
class CBasicTextBox;
struct GLCHARLOGIC_CLIENT;

class CCharacterRebornAWindow : public CUIWindowEx
{
	enum
	{
		REBORN_BUTTON_OK = ET_CONTROL_NEXT,
		REBORN_BUTTON_CANCEL,
	};

public:
	CCharacterRebornAWindow(void);
	virtual ~CCharacterRebornAWindow(void);

public:
	void CreateSubControl();

	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	CUIControl* m_pBackGround;
	CBasicTextButton* m_pButtonReborn;
	CBasicTextButton* m_pButtonCancel;
	CBasicTextBox*	m_pTextBefore;
	CBasicTextBox*	m_pTextAfter;

public:
	int m_nX;
	int m_nY;

public:
	void SetCard ( int nPosX, int nPosY );
	void SetData ( const GLCHARLOGIC_CLIENT& sCharData );
	void ShowRebornData( const GLCHARLOGIC_CLIENT& sCharData );
	void CheckCard();
};