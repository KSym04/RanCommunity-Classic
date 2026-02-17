
#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

class	CBasicTextBox;
class	CBasicLineBoxEx;
class	CBasicButton;
class	CD3DFontPar;
class	CBasicVarTextBoxExIcon;

const DWORD UIMSG_UIVARTEXT_EX_BUTTON_CLOSE_CLICK = UIMSG_USER1;
const DWORD UIMSG_UIVARTEXT_EX_BUTTON_CLOSE_MOUSEIN = UIMSG_USER2;

class	CBasicVarTextBoxExItemLink : public CUIGroup
{
private:
	static	const	float	fMOUSEPOINT_GAP;

protected:
	enum
	{
		BUTTON_CLOSE = NO_ID + 1,
	};

public:
	CBasicVarTextBoxExItemLink ();
	virtual	~CBasicVarTextBoxExItemLink ();

public:
	void	CreateSubControl ();

public:
	void	ClearText ();
	int		AddTextNoSplit ( CString strText, D3DCOLOR dwColor );
	int		SetTextNoSplit ( CString strText, D3DCOLOR dwColor );
	void	SetText( CString strText, D3DCOLOR	dwColor );
	void	AddString ( int nIndex, const CString& strText, const D3DCOLOR& dwColor );
	int		AddText ( CString strText, D3DCOLOR dwColor );
	void	AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor );
	void	AddStringNoSplit ( int nIndex, const CString& strText, const D3DCOLOR& dwColor );

	void	AddTitleText( CString strText, DWORD dwColor );
	void	SetIcon( SNATIVEID sICONINDEX, const char* szTexture );
public:
	void	SetUseOverColor ( bool bUseColor );
	void	SetOverColor ( const D3DCOLOR& dwColor );
	void	SetLineInterval ( const float fLineInterval );

public:
	int		GetCount ();

	void	RePosControl ( int x, int y );

public:
	void	SetBlockMouseTracking ( bool bBlock );

public:
	void	SetMousePointGap ( D3DXVECTOR2 vGap );

public:
	float	GetLongestLine ();

	void	SetTextAlign ( int nALIGN );

	void	ShowCloseButton( bool bSHOW );

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	bool	m_bBLOCK_MOUSETRACKING;

private:
	CBasicVarTextBoxExIcon*	m_pIconImage;
	CBasicTextBox*		m_pTextBox;
	CBasicTextBox*		m_pTextBoxTitle;
	CBasicLineBoxEx*	m_pLineBox;
	CBasicButton*		m_pCloseButton;
	D3DXVECTOR2			m_vMousePointGap;

private:
	CUIControl*			m_pSelfDummy;

private:
	CD3DFontPar*	m_pFont11;
	CD3DFontPar*	m_pFont;
};