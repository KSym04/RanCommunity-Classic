#include "StdAfx.h"
#include "./PVPCaptureTheFlagRewardInfo.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./ItemImage.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "./InnerInterface.h"
#include "../../Lib_Client/G-Logic/GLPVPCaptureTheFlagClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPCaptureTheFlagRewardInfo::CPVPCaptureTheFlagRewardInfo ()	
	: m_pTitleWin(NULL)
	, m_pTitleLoss(NULL)
	, m_pInfoWin(NULL)
	, m_pInfoLoss(NULL)
	, m_pItemWin(NULL)
	, m_pItemLoss(NULL)
	, m_pItemInfoWin(NULL)
	, m_pItemInfoLoss(NULL)
{
}

CPVPCaptureTheFlagRewardInfo::~CPVPCaptureTheFlagRewardInfo ()
{
}

void CPVPCaptureTheFlagRewardInfo::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CAPTURE_THE_FLAG_REWARD_INFO_LINEBOX_WIN" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CAPTURE_THE_FLAG_REWARD_INFO_LINEBOX_TITLE_WIN" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CAPTURE_THE_FLAG_REWARD_INFO_LINEBOX_LOSS" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CAPTURE_THE_FLAG_REWARD_INFO_LINEBOX_TITLE_LOSS" );
	RegisterControl ( pLineBox );

	m_pTitleWin = new CBasicTextBox;
	m_pTitleWin->CreateSub ( this, "PVP_CAPTURE_THE_FLAG_REWARD_INFO_TITLE_WIN" );
	m_pTitleWin->SetFont ( pFont9 );
	m_pTitleWin->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTitleWin->SetText( ID2GAMEWORD("PVP_CAPTURE_THE_FLAG_REWARD_INFO", 1 ), NS_UITEXTCOLOR::GREENYELLOW );
	RegisterControl ( m_pTitleWin );

	m_pTitleLoss = new CBasicTextBox;
	m_pTitleLoss->CreateSub ( this, "PVP_CAPTURE_THE_FLAG_REWARD_INFO_TITLE_LOSS" );
	m_pTitleLoss->SetFont ( pFont9 );
	m_pTitleLoss->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTitleLoss->SetText( ID2GAMEWORD("PVP_CAPTURE_THE_FLAG_REWARD_INFO", 2 ), NS_UITEXTCOLOR::RED );
	RegisterControl ( m_pTitleLoss );

	m_pInfoWin = new CBasicTextBox;
	m_pInfoWin->CreateSub ( this, "PVP_CAPTURE_THE_FLAG_REWARD_INFO_TEXT_WIN" );
	m_pInfoWin->SetFont ( pFont9 );
	m_pInfoWin->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pInfoWin );

	m_pInfoLoss = new CBasicTextBox;
	m_pInfoLoss->CreateSub ( this, "PVP_CAPTURE_THE_FLAG_REWARD_INFO_TEXT_LOSS" );
	m_pInfoLoss->SetFont ( pFont9 );
	m_pInfoLoss->SetTextAlign ( TEXT_ALIGN_LEFT );
	RegisterControl ( m_pInfoLoss );

	m_pItemWin = new CItemImage;
	m_pItemWin->CreateSub ( this, "PVP_CAPTURE_THE_FLAG_REWARD_ITEM_WIN", UI_FLAG_DEFAULT, CAPTURE_THE_FLAG_REWARD_INFO_ITEM_WIN );
	m_pItemWin->CreateSubControl ();
	m_pItemWin->SetUseRender ( TRUE );
	m_pItemWin->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pItemWin );

	m_pItemLoss = new CItemImage;
	m_pItemLoss->CreateSub ( this, "PVP_CAPTURE_THE_FLAG_REWARD_ITEM_LOSS", UI_FLAG_DEFAULT, CAPTURE_THE_FLAG_REWARD_INFO_ITEM_LOSS );
	m_pItemLoss->CreateSubControl ();
	m_pItemLoss->SetUseRender ( TRUE );
	m_pItemLoss->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pItemLoss );

	m_pItemInfoWin = new CBasicTextBox;
	m_pItemInfoWin->CreateSub ( this, "PVP_CAPTURE_THE_FLAG_REWARD_ITEM_WIN_INFO" );
	m_pItemInfoWin->SetFont ( pFont9 );
	m_pItemInfoWin->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pItemInfoWin );

	m_pItemInfoLoss = new CBasicTextBox;
	m_pItemInfoLoss->CreateSub ( this, "PVP_CAPTURE_THE_FLAG_REWARD_ITEM_LOSS_INFO" );
	m_pItemInfoLoss->SetFont ( pFont9 );
	m_pItemInfoLoss->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pItemInfoLoss );
}

void CPVPCaptureTheFlagRewardInfo::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		ShowInfo();
	}
}

void CPVPCaptureTheFlagRewardInfo::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case CAPTURE_THE_FLAG_REWARD_INFO_ITEM_WIN:
		{
			if ( CHECK_MOUSE_IN(dwMsg) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = GLPVPCaptureTheFlagClient::GetInstance().m_sItemRewardWin;

				if ( sitemcustom.sNativeID != NATIVEID_NULL () )
				{
					SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
					if ( pitem_data && pitem_data->ISPILE() )	sitemcustom.wTurnNum = pitem_data->GETAPPLYNUM();

					CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE );
				}

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

				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().ReqMaxRVShow( sitemcustom );
						return;
					}
				}
			}
		}break;

	case CAPTURE_THE_FLAG_REWARD_INFO_ITEM_LOSS:
		{
			if ( CHECK_MOUSE_IN(dwMsg) )
			{
				SITEMCUSTOM sitemcustom;
				sitemcustom.sNativeID = GLPVPCaptureTheFlagClient::GetInstance().m_sItemRewardLoss;

				if ( sitemcustom.sNativeID != NATIVEID_NULL () )
				{
					SITEM* pitem_data = GLItemMan::GetInstance().GetItem( sitemcustom.sNativeID );
					if ( pitem_data && pitem_data->ISPILE() )	sitemcustom.wTurnNum = pitem_data->GETAPPLYNUM();

					CInnerInterface::GetInstance().SHOW_ITEM_INFO( sitemcustom, FALSE, FALSE, FALSE, NULL, NULL, NATIVEID_NULL(), FALSE );
				}

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

				if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LCONTROL  ) & DXKEY_DOWNED )
				{
					if ( dwMsg & UIMSG_LB_UP )
					{
						CInnerInterface::GetInstance().ReqMaxRVShow( sitemcustom );
						return;
					}
				}
			}
		}break;
	}
}

void CPVPCaptureTheFlagRewardInfo::ShowInfo()
{
	m_pInfoWin->ClearText();
	m_pInfoLoss->ClearText();

	m_pItemWin->ResetItem();
	m_pItemWin->SetVisibleSingle(FALSE);
	m_pItemLoss->ResetItem();
	m_pItemLoss->SetVisibleSingle(FALSE);

	m_pItemInfoWin->ClearText();
	m_pItemInfoLoss->ClearText();

	CString text;

	float fContriMultiplier = GLPVPCaptureTheFlagClient::GetInstance().GetContributionPointMultiplier();

	if ( fContriMultiplier > 1.0f )
	{
		m_pInfoWin->AddText( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_TEXT", 0 ), NS_UITEXTCOLOR::GREENYELLOW );

		int nIndex1 = m_pInfoWin->AddText( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_TEXT", 2 ), NS_UITEXTCOLOR::GREENYELLOW );
		m_pInfoWin->AddString( nIndex1, ",", NS_UITEXTCOLOR::WHITE ); 
		CString strContri1;
		strContri1.Format( "(%g x)", fContriMultiplier );
		m_pInfoWin->AddString( nIndex1, strContri1.GetString(), NS_UITEXTCOLOR::ORANGE ); 

		m_pInfoLoss->AddText( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_TEXT", 1 ), NS_UITEXTCOLOR::RED );

		int nIndex2 = m_pInfoLoss->AddText( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_TEXT", 2 ), NS_UITEXTCOLOR::RED );
		m_pInfoLoss->AddString( nIndex2, ",", NS_UITEXTCOLOR::WHITE ); 
		CString strContri2;
		strContri2.Format( "(%g x)", fContriMultiplier );
		m_pInfoLoss->AddString( nIndex2, strContri2.GetString(), NS_UITEXTCOLOR::ORANGE ); 
	}
	else
	{
		m_pInfoWin->AddText( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_TEXT", 0 ), NS_UITEXTCOLOR::GREENYELLOW );
		m_pInfoWin->AddText( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_TEXT", 2 ), NS_UITEXTCOLOR::GREENYELLOW );

		m_pInfoLoss->AddText( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_TEXT", 1 ), NS_UITEXTCOLOR::RED );
		m_pInfoLoss->AddText( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_TEXT", 2 ), NS_UITEXTCOLOR::RED );
	}

	SNATIVEID item_win = GLPVPCaptureTheFlagClient::GetInstance().m_sItemRewardWin;
	SNATIVEID item_loss = GLPVPCaptureTheFlagClient::GetInstance().m_sItemRewardLoss;
	WORD item_win_req = GLPVPCaptureTheFlagClient::GetInstance().m_wRewardLimitScoreWin;
	WORD item_loss_req = GLPVPCaptureTheFlagClient::GetInstance().m_wRewardLimitScoreLost;
	
	SITEM* pitem_win = GLItemMan::GetInstance().GetItem( item_win );
	if ( pitem_win )
	{
		m_pItemWin->SetVisibleSingle( TRUE );
		m_pItemWin->SetItem( pitem_win->sBasicOp.sICONID, pitem_win->GetInventoryFile() );

		if ( item_win_req )
		{
			text.Format( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_INFO", 3 ), item_win_req );
			m_pItemInfoWin->AddText( text.GetString(), NS_UITEXTCOLOR::WHITE );
		}
	}

	SITEM* pitem_loss = GLItemMan::GetInstance().GetItem( item_loss );
	if ( pitem_loss )
	{
		m_pItemLoss->SetVisibleSingle( TRUE );
		m_pItemLoss->SetItem( pitem_loss->sBasicOp.sICONID, pitem_loss->GetInventoryFile() );

		if ( item_loss_req )
		{
			text.Format( ID2GAMEWORD( "PVP_CAPTURE_THE_FLAG_REWARD_INFO", 3 ), item_loss_req );
			m_pItemInfoLoss->AddText( text.GetString(), NS_UITEXTCOLOR::WHITE );
		}
	}
}