#include "StdAfx.h"
#include "./PVPClubDeathMatchRewardInfo.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./ItemImage.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/PVPClubDeathMatchClient.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubDeathMatchRewardInfo::CPVPClubDeathMatchRewardInfo ()	
	: m_pTextHeadRank(NULL)
	, m_pTextHeadItem(NULL)
	, m_pTextHeadContri(NULL)

{
	for ( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		m_pTextRank[i] = NULL;
		m_pTextItem[i] = NULL;
		m_pTextContri[i] = NULL;
		m_pItemImage[i] = NULL;
	}
}

CPVPClubDeathMatchRewardInfo::~CPVPClubDeathMatchRewardInfo ()
{
}

void CPVPClubDeathMatchRewardInfo::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CLUB_DEATH_MATCH_REWARD_INFO_LINEBOX" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody( "PVP_CLUB_DEATH_MATCH_REWARD_INFO_LINEBOX_HEAD" );
	RegisterControl ( pLineBox );	

	m_pTextHeadRank = new CBasicTextBox;
	m_pTextHeadRank->CreateSub ( this, "PVP_CLUB_DEATH_MATCH_REWARD_INFO_TEXT_HEAD_RANK" );
	m_pTextHeadRank->SetFont ( pFont9 );
	m_pTextHeadRank->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextHeadRank->SetText( ID2GAMEWORD("PVP_CLUB_DEATH_MATCH_REWARD_INFO", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextHeadRank );

	m_pTextHeadItem = new CBasicTextBox;
	m_pTextHeadItem->CreateSub ( this, "PVP_CLUB_DEATH_MATCH_REWARD_INFO_TEXT_HEAD_ITEM" );
	m_pTextHeadItem->SetFont ( pFont9 );
	m_pTextHeadItem->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextHeadItem->SetText( ID2GAMEWORD("PVP_CLUB_DEATH_MATCH_REWARD_INFO", 2 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextHeadItem );

	m_pTextHeadContri = new CBasicTextBox;
	m_pTextHeadContri->CreateSub ( this, "PVP_CLUB_DEATH_MATCH_REWARD_INFO_TEXT_HEAD_CONTRI" );
	m_pTextHeadContri->SetFont ( pFont9 );
	m_pTextHeadContri->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextHeadContri->SetText( ID2GAMEWORD("PVP_CLUB_DEATH_MATCH_REWARD_INFO", 3 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextHeadContri );

	std::string strRankText[PVP_CLUB_DEATH_MATCH_RANK_TOP] = 
	{
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_00",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_01",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_02",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_03",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_04",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_05",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_06",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_07",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_08",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_RANK_TEXT_09",
	};

	std::string strItemImage[PVP_CLUB_DEATH_MATCH_RANK_TOP] = 
	{
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_00",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_01",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_02",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_03",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_04",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_05",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_06",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_07",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_08",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_IMAGE_09",
	};

	std::string strItemName[PVP_CLUB_DEATH_MATCH_RANK_TOP] = 
	{
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_00",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_01",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_02",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_03",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_04",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_05",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_06",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_07",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_08",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_TEXT_09",
	};

	std::string strContriText[PVP_CLUB_DEATH_MATCH_RANK_TOP] = 
	{
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_00",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_01",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_02",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_03",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_04",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_05",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_06",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_07",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_08",
		"PVP_CLUB_DEATH_MATCH_REWARD_INFO_CONTRI_TEXT_09",
	};

	for ( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		m_pTextRank[i] = new CBasicTextBox;
		m_pTextRank[i]->CreateSub ( this, strRankText[i].c_str() );
		m_pTextRank[i]->SetFont ( pFont9 );
		m_pTextRank[i]->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
		m_pTextRank[i]->SetText( "--", NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pTextRank[i] );

		m_pTextItem[i] = new CBasicTextBox;
		m_pTextItem[i]->CreateSub ( this, strItemName[i].c_str() );
		m_pTextItem[i]->SetFont ( pFont9 );
		m_pTextItem[i]->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
		m_pTextItem[i]->SetText( "--", NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pTextItem[i] );

		m_pTextContri[i] = new CBasicTextBox;
		m_pTextContri[i]->CreateSub ( this, strContriText[i].c_str() );
		m_pTextContri[i]->SetFont ( pFont9 );
		m_pTextContri[i]->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
		m_pTextContri[i]->SetText( "--", NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pTextContri[i] );

		m_pItemImage[i] = new CItemImage;
		m_pItemImage[i]->CreateSub ( this, strItemImage[i].c_str(), UI_FLAG_DEFAULT, PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_00 + i );
		m_pItemImage[i]->CreateSubControl ();
		m_pItemImage[i]->CreateTextBoxDownRight();
		m_pItemImage[i]->SetUseRender ( TRUE );
		m_pItemImage[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pItemImage[i] );
	}
}

void CPVPClubDeathMatchRewardInfo::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		ShowInfo();
	}
}

void CPVPClubDeathMatchRewardInfo::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_00:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_01:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_02:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_03:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_04:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_05:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_06:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_07:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_08:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_09:
	case PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_10:
		{
			if ( CHECK_MOUSE_IN(dwMsg) )
			{
				int nIndex = cID - PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_00;
				if ( nIndex < PVP_CLUB_DEATH_MATCH_RANK_TOP )
				{
					SITEMCUSTOM sItemCustom;
					sItemCustom.sNativeID = PVPClubDeathMatchClient::GetInstance().m_sReward[nIndex].sItemID;

					if ( sItemCustom.sNativeID != NATIVEID_NULL () )
					{
						SITEM* pItemData = GLItemMan::GetInstance().GetItem( sItemCustom.sNativeID );
						if ( pItemData && pItemData->ISPILE() )	sItemCustom.wTurnNum = pItemData->GETAPPLYNUM();

						CInnerInterface::GetInstance().SHOW_ITEM_INFO( sItemCustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE );
					}

					/*item preview, Juver, 2017/07/27 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().PreviewItem( sItemCustom );
							return;
						}
					}

					/*box contents, Juver, 2017/08/30 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_RB_UP )
						{
							CInnerInterface::GetInstance().ShowBoxContents( sItemCustom.sNativeID );
							return;
						}
					}

					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().ReqMaxRVShow( sItemCustom );
							return;
						}
					}
				}
			}
		}break;
	};
}

void CPVPClubDeathMatchRewardInfo::ShowInfo()
{
	PVPClubDeathMatchClient& sClient = PVPClubDeathMatchClient::GetInstance();

	for ( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		CString strTemp;

		if ( m_pTextRank[i] )
		{
			strTemp.Format( "%d %s", i+1, ID2GAMEWORD( "RN_CDM_RANKING_TEXT", 0 ) );		
			m_pTextRank[i]->SetText( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
		}

		SITEM* pItem = GLItemMan::GetInstance().GetItem( sClient.m_sReward[i].sItemID );
		if ( pItem )
		{
			if ( m_pItemImage[i] )
			{
				m_pItemImage[i]->SetVisibleSingle( TRUE );
				m_pItemImage[i]->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );

				if ( pItem->ISPILE() )
				{
					strTemp.Format( "%d", pItem->GETAPPLYNUM() );
					m_pItemImage[i]->SetTextDownRight( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );	
				}
						
			}
			
			if ( m_pTextItem[i] )
			{
				m_pTextItem[i]->SetText( pItem->GetName(), COMMENT::ITEMCOLOR[pItem->sBasicOp.emLevel] );
			}
		}
		else
		{
			if ( m_pItemImage[i] )
			{
				m_pItemImage[i]->ResetItem();
				m_pItemImage[i]->SetTextDownRight( "", NS_UITEXTCOLOR::WHITE );
				m_pItemImage[i]->SetVisibleSingle(FALSE);
			}

			if ( m_pTextItem[i] )
			{
				m_pTextItem[i]->ClearText();
			}
		}

		if ( m_pTextContri[i] )
		{
			strTemp.Format( "%d", sClient.m_sReward[i].wContributionPoint );
			m_pTextContri[i]->SetText( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
		}
	}
}