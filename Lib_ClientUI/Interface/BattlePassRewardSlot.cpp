#include "StdAfx.h"
#include "./BattlePassRewardSlot.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./ItemImage.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./BasicTextButton.h"

#include "./BasicLineBoxSmart.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLItem.h"
#include "../../Lib_Client/G-Logic/GLCharacter.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-9-14, Battle Pass - CNDev*/

CBattlePassRewardSlot::CBattlePassRewardSlot ()
	: m_pNormalRewardImage(NULL)
	, m_pPremiumRewardImage(NULL)
	, m_pNormalLock(NULL)
	, m_pPremiumLock(NULL)
	, m_pTextLevel(NULL)
	, m_sNormalID(false)
	, m_sPremiumID(false)
	, m_dwLevel(0)
	, m_pButtonNormalClaim(NULL)
	, m_pButtonPremiumClaim(NULL)
{
}

CBattlePassRewardSlot::~CBattlePassRewardSlot ()
{
}

void CBattlePassRewardSlot::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBoxSmart* pLineBoxSmart = new CBasicLineBoxSmart();
	pLineBoxSmart->CreateSub( this, "RAN_BATTLEPASS_REWARD_SLOT_BACK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBoxSmart->CreateSubControl( "RAN_BATTLEPASS_REWARD_SLOT_BACK_LINE" );
	RegisterControl( pLineBoxSmart );

	m_pTextLevel = new CBasicTextBox;
	m_pTextLevel->CreateSub ( this, "RAN_BATTLEPASS_REWARD_SLOT_LEVEL_TEXT" );
	m_pTextLevel->SetFont ( pFont );
	m_pTextLevel->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextLevel->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextLevel );

	m_pNormalRewardImage = new CItemImage;
	m_pNormalRewardImage->CreateSub ( this, "RAN_BATTLEPASS_REWARD_SLOT_NORMAL_ITEM_IMAGE", UI_FLAG_DEFAULT, ITEM_IMAGE_NORMAL );
	m_pNormalRewardImage->CreateSubControl ();
	RegisterControl ( m_pNormalRewardImage );

	m_pNormalLock = new CUIControl;
	m_pNormalLock->CreateSub ( this, "RAN_BATTLEPASS_REWARD_SLOT_NORMAL_ITEM_IMAGE_LOCK", UI_FLAG_DEFAULT );
	RegisterControl ( m_pNormalLock );

	m_pPremiumRewardImage = new CItemImage;
	m_pPremiumRewardImage->CreateSub ( this, "RAN_BATTLEPASS_REWARD_SLOT_PREMIUM_ITEM_IMAGE", UI_FLAG_DEFAULT, ITEM_IMAGE_PREMIUM );
	m_pPremiumRewardImage->CreateSubControl ();
	RegisterControl ( m_pPremiumRewardImage );

	m_pPremiumLock = new CUIControl;
	m_pPremiumLock->CreateSub ( this, "RAN_BATTLEPASS_REWARD_SLOT_PREMIUM_ITEM_IMAGE_LOCK", UI_FLAG_DEFAULT );	
	RegisterControl ( m_pPremiumLock );

	m_pButtonNormalClaim = new CBasicTextButton;
	m_pButtonNormalClaim->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, BUTTON_NORMAL_CLAIM );
	m_pButtonNormalClaim->CreateBaseButton ( "RAN_BATTLEPASS_REWARD_SLOT_NORMAL_BUTTON_CLAIM", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("RAN_BATTLEPASS_REWARD_TEXT",2) );
	RegisterControl ( m_pButtonNormalClaim );

	m_pButtonPremiumClaim = new CBasicTextButton;
	m_pButtonPremiumClaim->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, BUTTON_PREMIUM_CLAIM );
	m_pButtonPremiumClaim->CreateBaseButton ( "RAN_BATTLEPASS_REWARD_SLOT_PREMIUM_BUTTON_CLAIM", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("RAN_BATTLEPASS_REWARD_TEXT",2) );
	RegisterControl ( m_pButtonPremiumClaim );

}

void CBattlePassRewardSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ITEM_IMAGE_NORMAL:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sNormalID;

				CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE, 0 );

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

	case ITEM_IMAGE_PREMIUM:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = m_sPremiumID;

				CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE, 0 );

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

	case BUTTON_NORMAL_CLAIM:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				ClaimNormalReward();
			}
		}break;

	case BUTTON_PREMIUM_CLAIM:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE(dwMsg) )
			{
				ClaimPremiumReward();
			}
		}break;
	};
}

void CBattlePassRewardSlot::ItemSet( SNATIVEID sNormalID, SNATIVEID sPremiumID, DWORD dwLevel )
{
	ItemReset();

	GLCharacter* pcharacter = GLGaeaClient::GetInstance().GetCharacter();
	if( !pcharacter )	return;

	SBATTLEPASS_REWARD_CHAR_DATA_MAP_ITER claimIter = pcharacter->m_mapBattlePassReward.find(dwLevel);
	bool bNormalClaimed = false;
	bool bPremiumClaimed = false;

	if (claimIter != pcharacter->m_mapBattlePassReward.end())
	{
		bNormalClaimed = claimIter->second.bNormalClaimed;
		bPremiumClaimed = claimIter->second.bPremiumClaimed;
	}

	SITEM* pItemNormal = GLItemMan::GetInstance().GetItem( sNormalID );
	if ( pItemNormal )
	{
		if ( m_pNormalRewardImage )		
		{
//			m_pNormalRewardImage->SetItem( sNormalID );
			m_pNormalRewardImage->SetItem( pItemNormal->sBasicOp.sICONID, pItemNormal->GetInventoryFile() );

			if ( bNormalClaimed )
				m_pNormalRewardImage->SetItemImageCheck();
			else
				m_pNormalRewardImage->SetItemImageNone();
		}

		if ( m_pNormalLock ) m_pNormalLock->SetVisibleSingle(pcharacter->m_dwBattlePassLevel < dwLevel);

		m_sNormalID = sNormalID;

		if ( m_pButtonNormalClaim ) m_pButtonNormalClaim->SetVisibleSingle(pcharacter->m_dwBattlePassLevel >= dwLevel && !bNormalClaimed);
	}

	SITEM* pItemPremium = GLItemMan::GetInstance().GetItem( sPremiumID );
	if ( pItemPremium )
	{
		if ( m_pPremiumRewardImage )		
		{
			//m_pPremiumRewardImage->SetItem( sPremiumID );
			m_pPremiumRewardImage->SetItem( pItemPremium->sBasicOp.sICONID, pItemPremium->GetInventoryFile() );

			if ( bPremiumClaimed )
				m_pPremiumRewardImage->SetItemImageCheck();
			else
				m_pPremiumRewardImage->SetItemImageNone();
		}

		if ( m_pPremiumLock ) m_pPremiumLock->SetVisibleSingle(pcharacter->m_bBattlePassPremium ? (pcharacter->m_dwBattlePassLevel < dwLevel) : true);

		if ( m_pButtonPremiumClaim ) m_pButtonPremiumClaim->SetVisibleSingle(pcharacter->m_bBattlePassPremium && pcharacter->m_dwBattlePassLevel >= dwLevel && !bPremiumClaimed);

		m_sPremiumID = sPremiumID;
	}

	m_dwLevel = dwLevel;

	if ( m_pTextLevel )
	{
		CString strText;
		strText.Format("%s:%d", ID2GAMEWORD("RAN_BATTLEPASS_REWARD_TEXT",1),dwLevel);
		m_pTextLevel->SetText( strText , NS_UITEXTCOLOR::WHITE );
	}
}

void CBattlePassRewardSlot::ItemReset()
{
	if ( m_pNormalRewardImage )	m_pNormalRewardImage->ResetItem();
	if ( m_pPremiumRewardImage )	m_pPremiumRewardImage->ResetItem();
	if ( m_pNormalLock ) m_pNormalLock->SetVisibleSingle(FALSE);
	if ( m_pPremiumLock )	m_pPremiumLock->SetVisibleSingle(FALSE);
	if ( m_pTextLevel )	m_pTextLevel->ClearText();

	if ( m_pButtonNormalClaim ) m_pButtonNormalClaim->SetVisibleSingle(FALSE);
	if ( m_pButtonPremiumClaim )	m_pButtonPremiumClaim->SetVisibleSingle(FALSE);

	m_sNormalID = false;
	m_sPremiumID = false;

	m_dwLevel = 0;
}

void CBattlePassRewardSlot::ClaimNormalReward()
{
	GLCharacter* pcharacter = GLGaeaClient::GetInstance().GetCharacter();
	if( !pcharacter )	return;

	pcharacter->ReqBattlePassRewardClaim(m_dwLevel);
}


void CBattlePassRewardSlot::ClaimPremiumReward()
{
	GLCharacter* pcharacter = GLGaeaClient::GetInstance().GetCharacter();
	if( !pcharacter )	return;

	pcharacter->ReqBattlePassRewardClaim(m_dwLevel, true);
}