#pragma	once

#include "UIOuterWindow.h"
class CD3DFontPar;
class CUIEditBox;
class CUIEditBoxMan;
class	CBasicScrollBarEx;

#define MACRO_TEXT_LIMIT 5000


class CAnnouncementWindow : public CUIOuterWindow
{
protected:
	enum
	{
		ANNOUNCEMENT_SCROLLBAR,
		CHAT_MACRO_OK = ET_CONTROL_NEXT + 1,
	};
public:
	CAnnouncementWindow ();
	virtual	~CAnnouncementWindow ();

public:
	void	CreateSubControl ();
	void	AddText ( CString strMessage, D3DCOLOR dwMessageColor );

public:
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	bool	LoadMessage ( const CString& strFileName );
	std::vector<CString>	m_vecMESSAGE;

private:
	CD3DFontPar*	m_pFont;
	CBasicTextBox*		m_pInfo;
	CUIEditBoxMan * m_pEditBoxMan;
	CBasicTextBox*	m_pInfoDisplay;
	CBasicScrollBarEx*		m_pScrollBar;
	CUIControl*		m_pInfoDisplayDummy;

};