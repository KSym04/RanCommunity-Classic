#pragma	once

#include "../UIWindowEx.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "../Lib_Client/G-Logic/GLCrowData.h"
#include "../Lib_Client/G-Logic/GLItemMan.h"

#include "../Lib_Client/G-Logic/GLMapList.h"
struct GLCHARLOGIC;

class	CBasicTextButton;
class	CBasicLineBox;
class	CBasicTextBox;
class	CBasicComboBox;
class	CBasicComboBoxRollOver;
class	CCheckBox;

class CPlatinumBuffWindow : public CUIWindowEx
{
private:
	static	const	int	nSTARTLINE;

public:
	CPlatinumBuffWindow ();
	virtual	~CPlatinumBuffWindow ();

public:
	void	CreateSubControl ();
protected: 

	enum
	{
		PLATINUM_BUFF_BUTTON_APPLY = ET_CONTROL_NEXT,
		PLATINUM_BUFF_BUTTON_CLOSE,
		PLATINUM_TYPE_COMBO_BUTTON,
		PLATINUM_TYPE_COMBO_ROLLOVER,
		PLATINUM_COMBO_BUTTON,
		PLATINUM_COMBO_ROLLOVER,
		PLATINUM_WINDOW_MARK_NONE,
		PLATINUM_WINDOW_MARK_BLUE,
		PLATINUM_CHECKBOX_MONEY,
		PLATINUM_CHECKBOX_POINT,
	};

private:

	CBasicTextButton*		m_pApplyButton;
	CBasicTextButton*		m_pCancelButton;
	
	CBasicTextBox*			m_pRemainText;
	CBasicTextBox*			m_pMoneyText;
	CBasicTextBox*			m_pPointText;
	CBasicTextBox*			m_pAccountCurrency;
	CBasicTextBox*			m_pBuffInfo;
	
	CUIControl*				m_pPlatinumMarkNone;
	CUIControl*				m_pPlatinumMarkBlue;
	
	CCheckBox*				m_pOpenType[2];
	
	CBasicComboBox*				m_pComboBoxTypeOpen;
	CBasicComboBoxRollOver*		m_pComboBoxTypeRollOver;
	
	CBasicComboBox*				m_pComboBoxDurationTypeOpen;
	CBasicComboBoxRollOver*		m_pComboBoxDurationTypeRollOver;

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	
public:
	void SelectType( int nSelect );
	void SelectDurationType( int nSelect );
	void Reset();
	void DoPlatinumBuff( int nIndexType, DWORD dwDuration, bool bGold );

public:
	bool m_bGold;

public:
	CBasicLineBox*		CreateUILineBoxWhiteNoBody( char* szBaseControl );
	CBasicLineBox*		CreateUILineBoxQuestList( char* szBaseControl );
	CBasicLineBox*		CreateUILineBoxWhite( char* szBaseControl );
	CBasicTextBox*		CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, CString strText, D3DCOLOR D3DCOLOR, int nAlign );
	CBasicTextBox*		CreateText ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign );
	CUIControl*			CreateControl ( const char* szControl );
};