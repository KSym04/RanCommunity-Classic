#pragma	once

#include "UIWindowEx.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"

class	CBasicTextButton;
class	CBasicTextBox;
class	CBasicTextBoxEx;
class	CBasicScrollBarEx;
class	CItemImage;

class CExchangeItemWindow : public CUIWindowEx
{
	enum
	{
		BUTTON_OK = ET_CONTROL_NEXT,
		BUTTON_CANCEL,
		ITEM_SLOT_IN,
		ITEM_SLOT_OUT,
		ITEM_LIST_TEXTBOX,
		ITEM_LIST_SCROLLBAR
	};

public:
	CExchangeItemWindow ();
	virtual	~CExchangeItemWindow ();

public:
	void CreateSubControl();

	virtual	void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle( BOOL bVisible );

public:
	CBasicTextButton*		m_pButtonOk;
	CBasicTextButton*		m_pButtonCancel;
	CItemImage*				m_pItemImage_In;
	CItemImage*				m_pItemImage_Out;
	CBasicTextBoxEx*		m_pListText;
	CBasicScrollBarEx*		m_pListScrollBar;
	CBasicTextBox*			m_pTextBefore;
	CBasicTextBox*			m_pTextAfter;
	CBasicTextBox*			m_pTextCIP;
	int	m_nX;
	int m_nY;
	int	m_nSelectIndex;
	SNATIVEID m_sIDOut;

public:
	void CheckCard();
	void SetCard ( int nPosX, int nPosY );
	void Refresh();
	BOOL CheckValidItem( SNATIVEID sID );
	BOOL CheckValidCIP( SNATIVEID sID );
};