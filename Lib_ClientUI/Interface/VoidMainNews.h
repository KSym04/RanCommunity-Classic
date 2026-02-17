#pragma	once

#include "UIWindowEx.h"
#include "BasicLineBoxEx.h"

class	CBasicTextBox;
class	CBasicTextButton;
class	CUIEditBox;
class CMultiModeButton;

class VoidMainNews : public CUIWindowEx
{
protected:
	enum
	{
		GUIDE_BUTTON_IMAGE = ET_CONTROL_NEXT,
		NEWS_BUTTON_IMAGE,
		SERVERINFO_BUTTON_IMAGE,
		RV_BUTTON_IMAGE,
		DONATION_BUTTON_IMAGE,
		GC_BUTTON_IMAGE,
		JX_NEWS_7,
		JX_NEWS_8,
		JX_NEWS_9,
		JX_NEWS_10
	};

private:
	CString imageUpdate;
	CUIControl*				m_pNewsImage0;
	CUIControl*				m_pNewsImage1;
	CUIControl*				m_pNewsImage2;
	CUIControl*				m_pNewsImage3;
	CUIControl*				m_pNewsImage4;
	CUIControl*				m_pNewsImage5;
	CUIControl*				m_pNewsImage6;
	CUIControl*				m_pNewsImage7;
	CUIControl*				m_pNewsImage8;
	CUIControl*				m_pNewsImage9;
	CUIControl*				m_pNewsImage10;
	CUIControl*				m_pNewsImageSwitch;
	CMultiModeButton*		vm_GuideButton;
	CMultiModeButton*		vm_NewsButton;
	CMultiModeButton*		vm_ServerInfoButton;
	CMultiModeButton*		vm_RVButton;
	CMultiModeButton*		vm_DonationButton;
	CMultiModeButton*		vm_GiftCodeButton;

	CMultiModeButton*		vm_Button7;
	CMultiModeButton*		vm_Button8;
	CMultiModeButton*		vm_Button9;
	CMultiModeButton*		vm_Button10;
	


	CBasicTextBox*			NewsLabel;
	float					fDelayTime;
	bool					waitfordelay;
	CUIEditBox* pEditBox;

public:
	VoidMainNews();
	~VoidMainNews();

	void	CreateSubControl ();
	CMultiModeButton*	CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID );
	CUIControl* CreateNewsWindow(const char* guiKeyword );
	CBasicTextButton*	CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText );
	CBasicLineBoxEx*  CreateLineBoxWhite ( char* szLineBoxKeyWord );
	CBasicTextButton*	CreateTextButton ( const char* szButton, UIGUID ControlID, const char* szText );
	CBasicTextBox*  CreateNewsLabel ( const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	void ResetControls();

};