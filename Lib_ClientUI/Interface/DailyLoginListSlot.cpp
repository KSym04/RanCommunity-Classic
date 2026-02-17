#include "StdAfx.h"
#include "./DailyLoginListSlot.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextButton.h"

#include "./BasicLineBoxSmart.h"

#include "./ItemImage.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/GLDailyLogin.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-9-14, Battle Pass - CNDev*/

CDailyLoginListSlot::CDailyLoginListSlot ()	
	: m_dwDay(UINT_MAX)
	, m_pRewardImage(NULL)
	, m_sRewardID(false)
	, m_pTextDay(NULL)
	, m_pTextClaimed(NULL)
	, m_pButtonClaim(NULL)
	, m_pLockImage(NULL)
{
}

CDailyLoginListSlot::~CDailyLoginListSlot ()
{
}

void CDailyLoginListSlot::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont11 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 11, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont12 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 12, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBoxSmart* pLineBoxSmart = new CBasicLineBoxSmart();
	pLineBoxSmart->CreateSub( this, "RAN_DAILYLOGIN_SLOT_BACK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBoxSmart->CreateSubControl( "RAN_DAILYLOGIN_SLOT_BACK_LINE" );
	RegisterControl( pLineBoxSmart );

	m_pRewardImage = new CItemImage;
	m_pRewardImage->CreateSub ( this, "RAN_DAILYLOGIN_SLOT_ITEM_IMAGE", UI_FLAG_DEFAULT, ITEM_IMAGE );
	m_pRewardImage->CreateSubControl ();
	RegisterControl ( m_pRewardImage );

	m_pTextDay = new CBasicTextBox;
	m_pTextDay->CreateSub ( this, "RAN_DAILYLOGIN_SLOT_TEXT_DAY" );
	m_pTextDay->SetFont ( pFont11 );
	m_pTextDay->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextDay->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextDay );

	m_pTextClaimed = new CBasicTextBox;
	m_pTextClaimed->CreateSub ( this, "RAN_DAILYLOGIN_SLOT_TEXT_CLAIMED" );
	m_pTextClaimed->SetFont ( pFont11 );
	m_pTextClaimed->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextClaimed->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextClaimed );

	m_pButtonClaim = new CBasicTextButton;
	m_pButtonClaim->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, BUTTON_CLAIM );
	m_pButtonClaim->CreateBaseButton ( "RAN_DAILYLOGIN_SLOT_BUTTON_CLAIM", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("RAN_DAILYLOGIN_DAY_TEXT",2) );
	RegisterControl ( m_pButtonClaim );

	m_pLockImage = new CUIControl;
	m_pLockImage->CreateSub ( this, "RAN_DAILYLOGIN_SLOT_LOCK_IMAGE", UI_FLAG_DEFAULT );
	RegisterControl ( m_pLockImage );
}

void CDailyLoginListSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ITEM_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sRewardID;

				if( sitemcustom.sNativeID != NATIVEID_NULL() )
					CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, 0, 0 );
				/*item preview, Juver, 2017/07/27 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().PreviewItem( sitemcustom );
						return;
					}
				}

				/*box contents, Juver, 2017/08/30 */
				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_RB_UP )
					{
						CInnerInterface::GetInstance().ShowBoxContents( sitemcustom.sNativeID );
						return;
					}
				}
			}
		}break;

	case BUTTON_CLAIM:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				ClaimReward();
			}
		}break;
	}
}

void CDailyLoginListSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

}

void CDailyLoginListSlot::ResetData()
{
	m_dwDay = UINT_MAX;

	m_sRewardID = false;

	if ( m_pRewardImage )		m_pRewardImage->ResetItem();

	if ( m_pTextDay )			m_pTextDay->ClearText();

	if ( m_pLockImage )			m_pLockImage->SetVisibleSingle(FALSE);

	if ( m_pTextClaimed )		m_pTextClaimed->ClearText();
}

void CDailyLoginListSlot::SetData(DWORD dwDay, SNATIVEID sRewardID)
{
	ResetData();

	GLCharacter* pcharacter = GLGaeaClient::GetInstance().GetCharacter();
	if (!pcharacter) return;

	CString strText;
	if (m_pTextDay)
	{
		strText.Format("%s %d", ID2GAMEWORD("RAN_DAILYLOGIN_DAY_TEXT", 0), dwDay);
		m_pTextDay->AddText(strText.GetString(), NS_UITEXTCOLOR::WHITE);
	}

	DWORD dwCharacterDay = 0;
	bool bRewardClaimed = false;

	SDAILYLOGIN_CHAR_DATA_MAP_ITER claimIter = pcharacter->m_mapDailyLogin.find(dwDay);
	if (claimIter != pcharacter->m_mapDailyLogin.end())
	{
		SDAILYLOGIN_CHAR_DATA& sDailyLogin = claimIter->second;
		dwCharacterDay = sDailyLogin.dwDay;
		bRewardClaimed = sDailyLogin.bRewardClaimed;
	}

	SITEMCUSTOM sitemcustom;
	sitemcustom.sNativeID = sRewardID;

	if( sitemcustom.sNativeID == NATIVEID_NULL() ) return;

	SITEM* pItemNormal = GLItemMan::GetInstance().GetItem( sRewardID );
	if ( pItemNormal )
	{
		if ( m_pRewardImage )		
		{
			m_pRewardImage->SetItem( pItemNormal->sBasicOp.sICONID, pItemNormal->GetInventoryFile() );

			if ( bRewardClaimed )
			{
				m_pRewardImage->SetItemImageCheck();

				m_pTextClaimed->AddText((char*)ID2GAMEWORD("RAN_DAILYLOGIN_DAY_TEXT",1), NS_UITEXTCOLOR::WHITE );
			}
		}

		if (m_pLockImage)		m_pLockImage->SetVisibleSingle(pcharacter->m_dwDayStreakCounter < dwDay);

		if (m_pButtonClaim)		m_pButtonClaim->SetVisibleSingle(pcharacter->m_dwDayStreakCounter == dwDay && !bRewardClaimed );

		m_sRewardID = sRewardID;
	}

	m_dwDay = dwDay;
}



void CDailyLoginListSlot::ClaimReward()
{
	GLCharacter* pcharacter = GLGaeaClient::GetInstance().GetCharacter();
	if( !pcharacter )	return;

	pcharacter->ReqDailyLoginRewardClaim(m_dwDay);
}