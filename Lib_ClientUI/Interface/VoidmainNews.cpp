#include "StdAfx.h"
#include "VoidMainNews.h"
#include "BasicTextBox.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "InnerInterface.h"
#include "BasicTextButton.h" 
#include "UIEditBox.h"
#include "./MultiModeButton.h"
#include "../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


VoidMainNews::VoidMainNews()
	:	m_pNewsImage0 ( NULL )
	,	m_pNewsImage1 ( NULL )
	,	m_pNewsImage2 ( NULL )
	,	m_pNewsImage3 ( NULL )
	,	m_pNewsImage4 ( NULL )
	,	m_pNewsImage5 ( NULL )
	,	m_pNewsImage6 ( NULL )
	,	m_pNewsImage7 ( NULL )
	,	m_pNewsImage8 ( NULL )
	,	m_pNewsImage9 ( NULL )
	,	m_pNewsImage10 ( NULL )
	,	m_pNewsImageSwitch ( NULL )
	,	vm_GuideButton ( NULL )
	,	vm_NewsButton ( NULL )
	,	vm_ServerInfoButton ( NULL )
	,	vm_RVButton ( NULL )
	,	vm_DonationButton ( NULL )
	,	vm_GiftCodeButton ( NULL )
	,	vm_Button7 ( NULL )
	,	vm_Button8 ( NULL )
	,	vm_Button9 ( NULL )
	,	vm_Button10 ( NULL )
	,waitfordelay(false)
	,fDelayTime(0.0f)

{

}

VoidMainNews::~VoidMainNews()
{
}

void VoidMainNews::CreateSubControl ()
{
	CString LineBoxes;
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 11, _DEFAULT_FONT_SHADOW_FLAG );
	
	/*for(int i=0;i<4;i++)
	{
		char* LBoxKey;
		LineBoxes.Format("VOIDMAIN_NEWS_LINEBOX_BUTTON_%d",i);
		LBoxKey =  (LPTSTR) (LPCTSTR)LineBoxes.GetString();
		CreateLineBoxWhite(LBoxKey);
	}*/

	//Buttons
	if ( RPARAM::dwNewsWindowButtons == 10 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);

		vm_GuideButton = CreateTabButton("VOIDMAIN_NEWS_GUIDE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",2) , GUIDE_BUTTON_IMAGE);
		vm_GuideButton->SetVisibleSingle(TRUE);

		vm_ServerInfoButton = CreateTabButton("VOIDMAIN_NEWS_SERVER_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",3), SERVERINFO_BUTTON_IMAGE);
		vm_ServerInfoButton->SetVisibleSingle(TRUE);

		vm_RVButton = CreateTabButton("VOIDMAIN_NEWS_RV_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",4), RV_BUTTON_IMAGE);
		vm_RVButton->SetVisibleSingle(TRUE);

		vm_DonationButton = CreateTabButton("VOIDMAIN_NEWS_DONATE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",5) ,DONATION_BUTTON_IMAGE);
		vm_DonationButton->SetVisibleSingle(TRUE);

		vm_GiftCodeButton = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",6), GC_BUTTON_IMAGE);
		vm_GiftCodeButton->SetVisibleSingle(TRUE);

		vm_Button7 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_7",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",7), JX_NEWS_7);
		vm_Button7->SetVisibleSingle(TRUE);

		vm_Button8 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_8",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",8), JX_NEWS_8);
		vm_Button8->SetVisibleSingle(TRUE);

		vm_Button9 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_9",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",9), JX_NEWS_9);
		vm_Button9->SetVisibleSingle(TRUE);

		vm_Button10 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_10",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",10), JX_NEWS_10);
		vm_Button10->SetVisibleSingle(TRUE);
	}

	if ( RPARAM::dwNewsWindowButtons == 9 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);

		vm_GuideButton = CreateTabButton("VOIDMAIN_NEWS_GUIDE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",2) , GUIDE_BUTTON_IMAGE);
		vm_GuideButton->SetVisibleSingle(TRUE);

		vm_ServerInfoButton = CreateTabButton("VOIDMAIN_NEWS_SERVER_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",3), SERVERINFO_BUTTON_IMAGE);
		vm_ServerInfoButton->SetVisibleSingle(TRUE);

		vm_RVButton = CreateTabButton("VOIDMAIN_NEWS_RV_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",4), RV_BUTTON_IMAGE);
		vm_RVButton->SetVisibleSingle(TRUE);

		vm_DonationButton = CreateTabButton("VOIDMAIN_NEWS_DONATE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",5) ,DONATION_BUTTON_IMAGE);
		vm_DonationButton->SetVisibleSingle(TRUE);

		vm_GiftCodeButton = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",6), GC_BUTTON_IMAGE);
		vm_GiftCodeButton->SetVisibleSingle(TRUE);

		vm_Button7 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_7",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",7), JX_NEWS_7);
		vm_Button7->SetVisibleSingle(TRUE);

		vm_Button8 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_8",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",8), JX_NEWS_8);
		vm_Button8->SetVisibleSingle(TRUE);

		vm_Button9 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_9",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",9), JX_NEWS_9);
		vm_Button9->SetVisibleSingle(TRUE);
	}

	if ( RPARAM::dwNewsWindowButtons == 8 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);

		vm_GuideButton = CreateTabButton("VOIDMAIN_NEWS_GUIDE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",2) , GUIDE_BUTTON_IMAGE);
		vm_GuideButton->SetVisibleSingle(TRUE);

		vm_ServerInfoButton = CreateTabButton("VOIDMAIN_NEWS_SERVER_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",3), SERVERINFO_BUTTON_IMAGE);
		vm_ServerInfoButton->SetVisibleSingle(TRUE);

		vm_RVButton = CreateTabButton("VOIDMAIN_NEWS_RV_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",4), RV_BUTTON_IMAGE);
		vm_RVButton->SetVisibleSingle(TRUE);

		vm_DonationButton = CreateTabButton("VOIDMAIN_NEWS_DONATE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",5) ,DONATION_BUTTON_IMAGE);
		vm_DonationButton->SetVisibleSingle(TRUE);

		vm_GiftCodeButton = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",6), GC_BUTTON_IMAGE);
		vm_GiftCodeButton->SetVisibleSingle(TRUE);

		vm_Button7 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_7",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",7), JX_NEWS_7);
		vm_Button7->SetVisibleSingle(TRUE);

		vm_Button8 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_8",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",8), JX_NEWS_8);
		vm_Button8->SetVisibleSingle(TRUE);
	}

	if ( RPARAM::dwNewsWindowButtons == 7 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);

		vm_GuideButton = CreateTabButton("VOIDMAIN_NEWS_GUIDE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",2) , GUIDE_BUTTON_IMAGE);
		vm_GuideButton->SetVisibleSingle(TRUE);

		vm_ServerInfoButton = CreateTabButton("VOIDMAIN_NEWS_SERVER_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",3), SERVERINFO_BUTTON_IMAGE);
		vm_ServerInfoButton->SetVisibleSingle(TRUE);

		vm_RVButton = CreateTabButton("VOIDMAIN_NEWS_RV_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",4), RV_BUTTON_IMAGE);
		vm_RVButton->SetVisibleSingle(TRUE);

		vm_DonationButton = CreateTabButton("VOIDMAIN_NEWS_DONATE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",5) ,DONATION_BUTTON_IMAGE);
		vm_DonationButton->SetVisibleSingle(TRUE);

		vm_GiftCodeButton = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",6), GC_BUTTON_IMAGE);
		vm_GiftCodeButton->SetVisibleSingle(TRUE);

		vm_Button7 = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON_7",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",7), JX_NEWS_7);
		vm_Button7->SetVisibleSingle(TRUE);
	}

	if ( RPARAM::dwNewsWindowButtons == 6 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);

		vm_GuideButton = CreateTabButton("VOIDMAIN_NEWS_GUIDE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",2) , GUIDE_BUTTON_IMAGE);
		vm_GuideButton->SetVisibleSingle(TRUE);

		vm_ServerInfoButton = CreateTabButton("VOIDMAIN_NEWS_SERVER_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",3), SERVERINFO_BUTTON_IMAGE);
		vm_ServerInfoButton->SetVisibleSingle(TRUE);

		vm_RVButton = CreateTabButton("VOIDMAIN_NEWS_RV_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",4), RV_BUTTON_IMAGE);
		vm_RVButton->SetVisibleSingle(TRUE);

		vm_DonationButton = CreateTabButton("VOIDMAIN_NEWS_DONATE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",5) ,DONATION_BUTTON_IMAGE);
		vm_DonationButton->SetVisibleSingle(TRUE);

		vm_GiftCodeButton = CreateTabButton("VOIDMAIN_NEWS_GC_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",6), GC_BUTTON_IMAGE);
		vm_GiftCodeButton->SetVisibleSingle(TRUE);

	}

	if ( RPARAM::dwNewsWindowButtons == 5 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);

		vm_GuideButton = CreateTabButton("VOIDMAIN_NEWS_GUIDE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",2) , GUIDE_BUTTON_IMAGE);
		vm_GuideButton->SetVisibleSingle(TRUE);

		vm_ServerInfoButton = CreateTabButton("VOIDMAIN_NEWS_SERVER_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",3), SERVERINFO_BUTTON_IMAGE);
		vm_ServerInfoButton->SetVisibleSingle(TRUE);

		vm_RVButton = CreateTabButton("VOIDMAIN_NEWS_RV_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",4), RV_BUTTON_IMAGE);
		vm_RVButton->SetVisibleSingle(TRUE);

		vm_DonationButton = CreateTabButton("VOIDMAIN_NEWS_DONATE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",5) ,DONATION_BUTTON_IMAGE);
		vm_DonationButton->SetVisibleSingle(TRUE);
	}

	if ( RPARAM::dwNewsWindowButtons == 4 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);

		vm_GuideButton = CreateTabButton("VOIDMAIN_NEWS_GUIDE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",2) , GUIDE_BUTTON_IMAGE);
		vm_GuideButton->SetVisibleSingle(TRUE);

		vm_ServerInfoButton = CreateTabButton("VOIDMAIN_NEWS_SERVER_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",3), SERVERINFO_BUTTON_IMAGE);
		vm_ServerInfoButton->SetVisibleSingle(TRUE);

		vm_RVButton = CreateTabButton("VOIDMAIN_NEWS_RV_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",4), RV_BUTTON_IMAGE);
		vm_RVButton->SetVisibleSingle(TRUE);

	}

	if ( RPARAM::dwNewsWindowButtons == 3 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);

		vm_GuideButton = CreateTabButton("VOIDMAIN_NEWS_GUIDE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",2) , GUIDE_BUTTON_IMAGE);
		vm_GuideButton->SetVisibleSingle(TRUE);

		vm_ServerInfoButton = CreateTabButton("VOIDMAIN_NEWS_SERVER_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",3), SERVERINFO_BUTTON_IMAGE);
		vm_ServerInfoButton->SetVisibleSingle(TRUE);

	}

	if ( RPARAM::dwNewsWindowButtons == 2 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);

		vm_GuideButton = CreateTabButton("VOIDMAIN_NEWS_GUIDE_BUTTON",ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",2) , GUIDE_BUTTON_IMAGE);
		vm_GuideButton->SetVisibleSingle(TRUE);

	}

	if ( RPARAM::dwNewsWindowButtons == 1 )
	{
		vm_NewsButton = CreateTabButton ("VOIDMAIN_NEWS_NEWS_BUTTON", ID2GAMEWORD("VOID_MAIN_NEWS_TEXT",1 ) , NEWS_BUTTON_IMAGE );
		vm_NewsButton->SetVisibleSingle(TRUE);
	}


	/*News/Guide  Images VOIDMAIN NRDEV 9/3/2021*/

	m_pNewsImage1 = CreateNewsWindow("VOID_MAIN_NEWS_1");
	m_pNewsImage1->SetVisibleSingle(TRUE);
	m_pNewsImage2 = CreateNewsWindow("VOID_MAIN_NEWS_2");
	m_pNewsImage2->SetVisibleSingle(FALSE);
	m_pNewsImage3 = CreateNewsWindow("VOID_MAIN_NEWS_3");
	m_pNewsImage3->SetVisibleSingle(FALSE);
	m_pNewsImage4 = CreateNewsWindow("VOID_MAIN_NEWS_4");
	m_pNewsImage4->SetVisibleSingle(FALSE);
	m_pNewsImage5 = CreateNewsWindow("VOID_MAIN_NEWS_5");
	m_pNewsImage5->SetVisibleSingle(FALSE);
	m_pNewsImage6 = CreateNewsWindow("VOID_MAIN_NEWS_6");
	m_pNewsImage6->SetVisibleSingle(FALSE);
	m_pNewsImage7 = CreateNewsWindow("VOID_MAIN_NEWS_7");
	m_pNewsImage7->SetVisibleSingle(FALSE);
	m_pNewsImage8 = CreateNewsWindow("VOID_MAIN_NEWS_8");
	m_pNewsImage8->SetVisibleSingle(FALSE);
	m_pNewsImage9 = CreateNewsWindow("VOID_MAIN_NEWS_9");
	m_pNewsImage9->SetVisibleSingle(FALSE);
	m_pNewsImage10 = CreateNewsWindow("VOID_MAIN_NEWS_10");
	m_pNewsImage10->SetVisibleSingle(FALSE);

}

CUIControl* VoidMainNews::CreateNewsWindow(const char* guiKeyword )
{
	CUIControl* m_pNewsImage = new CUIControl;
	m_pNewsImage->CreateSub ( this, guiKeyword, UI_FLAG_DEFAULT );	
	m_pNewsImage->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pNewsImage );
	return m_pNewsImage;
}

CBasicTextBox* VoidMainNews::CreateNewsLabel ( const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	pStaticText->SetVisibleSingle ( FALSE );
	RegisterControl ( pStaticText );

	return pStaticText;
}

CBasicTextButton*  VoidMainNews::CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE22;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText ,_DEFAULT_FONT_SHADOW_FLAG );
	RegisterControl ( pTextButton );
	return pTextButton;
}
CBasicLineBoxEx*  VoidMainNews::CreateLineBoxWhite ( char* szLineBoxKeyWord )
{
	CBasicLineBoxEx* m_pLineBoxes = new CBasicLineBoxEx;
	m_pLineBoxes->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxes->CreateBaseBoxDialogue( szLineBoxKeyWord );
	m_pLineBoxes->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxes );

	return m_pLineBoxes;
}
CBasicTextButton*  VoidMainNews::CreateTextButton ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE19;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText );
	pTextButton->SetFlip(FALSE);
	RegisterControl ( pTextButton );
	return pTextButton;
}

void VoidMainNews::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	fDelayTime+=fElapsedTime;
	if(waitfordelay)
	{		
		if(fDelayTime > 8.3f)
		{
			waitfordelay=false;
			fDelayTime=0.0f;
		}
	}

	
	
}
void VoidMainNews::ResetControls()
{
	if ( RPARAM::dwNewsWindowButtons == 10 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
		vm_GuideButton->DoImageFlip(FALSE);
		vm_ServerInfoButton->DoImageFlip(FALSE);
		vm_RVButton->DoImageFlip(FALSE);
		vm_DonationButton->DoImageFlip(FALSE);
		vm_GiftCodeButton->DoImageFlip(FALSE);
		vm_Button7->DoImageFlip(FALSE);
		vm_Button8->DoImageFlip(FALSE);
		vm_Button9->DoImageFlip(FALSE);
		vm_Button10->DoImageFlip(FALSE);
	}

	if ( RPARAM::dwNewsWindowButtons == 9 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
		vm_GuideButton->DoImageFlip(FALSE);
		vm_ServerInfoButton->DoImageFlip(FALSE);
		vm_RVButton->DoImageFlip(FALSE);
		vm_DonationButton->DoImageFlip(FALSE);
		vm_GiftCodeButton->DoImageFlip(FALSE);
		vm_Button7->DoImageFlip(FALSE);
		vm_Button8->DoImageFlip(FALSE);
		vm_Button9->DoImageFlip(FALSE);
	}

	if ( RPARAM::dwNewsWindowButtons == 8 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
		vm_GuideButton->DoImageFlip(FALSE);
		vm_ServerInfoButton->DoImageFlip(FALSE);
		vm_RVButton->DoImageFlip(FALSE);
		vm_DonationButton->DoImageFlip(FALSE);
		vm_GiftCodeButton->DoImageFlip(FALSE);
		vm_Button7->DoImageFlip(FALSE);
		vm_Button8->DoImageFlip(FALSE);
	}

	if ( RPARAM::dwNewsWindowButtons == 7 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
		vm_GuideButton->DoImageFlip(FALSE);
		vm_ServerInfoButton->DoImageFlip(FALSE);
		vm_RVButton->DoImageFlip(FALSE);
		vm_DonationButton->DoImageFlip(FALSE);
		vm_GiftCodeButton->DoImageFlip(FALSE);
		vm_Button7->DoImageFlip(FALSE);
	}

	if ( RPARAM::dwNewsWindowButtons == 6 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
		vm_GuideButton->DoImageFlip(FALSE);
		vm_ServerInfoButton->DoImageFlip(FALSE);
		vm_RVButton->DoImageFlip(FALSE);
		vm_DonationButton->DoImageFlip(FALSE);
		vm_GiftCodeButton->DoImageFlip(FALSE);
	}

	if ( RPARAM::dwNewsWindowButtons == 5 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
		vm_GuideButton->DoImageFlip(FALSE);
		vm_ServerInfoButton->DoImageFlip(FALSE);
		vm_RVButton->DoImageFlip(FALSE);
		vm_DonationButton->DoImageFlip(FALSE);
	}

	if ( RPARAM::dwNewsWindowButtons == 4 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
		vm_GuideButton->DoImageFlip(FALSE);
		vm_ServerInfoButton->DoImageFlip(FALSE);
		vm_RVButton->DoImageFlip(FALSE);
	}

	if ( RPARAM::dwNewsWindowButtons == 3 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
		vm_GuideButton->DoImageFlip(FALSE);
		vm_ServerInfoButton->DoImageFlip(FALSE);
	}

	if ( RPARAM::dwNewsWindowButtons == 2 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
		vm_GuideButton->DoImageFlip(FALSE);
	}

	if ( RPARAM::dwNewsWindowButtons == 1 )
	{
		vm_NewsButton->DoImageFlip(FALSE);
	}
	
	
	m_pNewsImage1->SetVisibleSingle(FALSE);	
	m_pNewsImage2->SetVisibleSingle(FALSE);	
	m_pNewsImage3->SetVisibleSingle(FALSE);	
	m_pNewsImage4->SetVisibleSingle(FALSE);	
	m_pNewsImage5->SetVisibleSingle(FALSE);
	m_pNewsImage6->SetVisibleSingle(FALSE);

	m_pNewsImage7->SetVisibleSingle(FALSE);	
	m_pNewsImage8->SetVisibleSingle(FALSE);	
	m_pNewsImage9->SetVisibleSingle(FALSE);	
	m_pNewsImage10->SetVisibleSingle(FALSE);	

}
void VoidMainNews::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case NEWS_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ))
			{
				ResetControls();
//				vm_NewsButton->SetFlip(true);
				vm_NewsButton->DoImageFlip(TRUE);
				m_pNewsImage1->SetVisibleSingle(TRUE);
				
			}
			
		}break;
		case GUIDE_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg )  )
				{
					ResetControls();
					//vm_GuideButton->SetFlip(true);
					vm_GuideButton->DoImageFlip(TRUE);
					m_pNewsImage2->SetVisibleSingle(TRUE);
				}
			
			
		}break;
		case SERVERINFO_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg )  )
				{
					ResetControls();
					vm_ServerInfoButton->DoImageFlip(TRUE);
					m_pNewsImage3->SetVisibleSingle(TRUE);
				}
			
			
		}break;

		case RV_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg )  )
				{
					ResetControls();
					vm_RVButton->DoImageFlip(TRUE);
					m_pNewsImage4->SetVisibleSingle(TRUE);
				}
			
			
		}break;

		case DONATION_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg )  )	
				{
					ResetControls();
					vm_DonationButton->DoImageFlip(TRUE);
					m_pNewsImage5->SetVisibleSingle(TRUE);
				}
			
			
		}break;
		case GC_BUTTON_IMAGE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg )  )	
				{
					ResetControls();
					vm_GiftCodeButton->DoImageFlip(TRUE);
					m_pNewsImage6->SetVisibleSingle(TRUE);
				}
			
			
		}break;

		case JX_NEWS_7:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg )  )	
				{
					ResetControls();
					vm_Button7->DoImageFlip(TRUE);
					m_pNewsImage7->SetVisibleSingle(TRUE);
				}
			
			
		}break;

		case JX_NEWS_8:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg )  )	
				{
					ResetControls();
					vm_Button8->DoImageFlip(TRUE);
					m_pNewsImage8->SetVisibleSingle(TRUE);
				}
			
			
		}break;

		case JX_NEWS_9:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg )  )	
				{
					ResetControls();
					vm_Button9->DoImageFlip(TRUE);
					m_pNewsImage9->SetVisibleSingle(TRUE);
				}
			
			
		}break;

		case JX_NEWS_10:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg )  )	
				{
					ResetControls();
					vm_Button10->DoImageFlip(TRUE);
					m_pNewsImage10->SetVisibleSingle(TRUE);
				}
			
			
		}break;
	}
}
CMultiModeButton* VoidMainNews::CreateTabButton( const char* szButtonBox, const char* szButtonText, UIGUID ControlID )
{
	CMultiModeButton* pButton = new CMultiModeButton;
	pButton->CreateSub ( this, "TAB_BUTTON_WHITE", UI_FLAG_XSIZE, ControlID );
	pButton->CreateImageBaseMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT", "TAB_BUTTON_WHITE_IMAGE_CENTER", "TAB_BUTTON_WHITE_IMAGE_RIGHT" );
	pButton->CreateImageFlipMulti( "TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT_F", "TAB_BUTTON_WHITE_IMAGE_CENTER_F", "TAB_BUTTON_WHITE_IMAGE_RIGHT_F" );
	pButton->CreateTextBox( "TAB_BUTTON_WHITE_IMAGE_TEXTBOX", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG );
	pButton->AlignToControl( szButtonBox );
	pButton->SetOneLineText( szButtonText, NS_UITEXTCOLOR::WHITE );
	pButton->SetTextColor( NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pButton );
	return pButton;
}