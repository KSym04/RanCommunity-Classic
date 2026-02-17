#include "StdAfx.h"
#include "./BattlePassListSlot.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"

#include "./BasicLineBoxSmart.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLBattlePass.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-9-14, Battle Pass - CNDev*/

CBattlePassListSlot::CBattlePassListSlot ()	
	: m_pTextComplete(NULL)
	, m_pTextTitle(NULL)
	, m_pTextDesc(NULL)
	, m_pTextProgress(NULL)
	, m_pImagePoint(NULL)
	, m_pTextBadge(NULL)
	, m_pTextPoint(NULL)
	, m_fUpdateTime(0.0f)
	, m_dwBattlePassID(UINT_MAX)
{
}

CBattlePassListSlot::~CBattlePassListSlot ()
{
}

void CBattlePassListSlot::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBoxSmart* pLineBoxSmart = new CBasicLineBoxSmart();
	pLineBoxSmart->CreateSub( this, "RAN_BATTLEPASS_MISSION_SLOT_BACK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBoxSmart->CreateSubControl( "RAN_BATTLEPASS_MISSION_SLOT_BACK_LINE" );
	RegisterControl( pLineBoxSmart );

	m_pImagePoint = new CUIControl;
	m_pImagePoint->CreateSub ( this, "RAN_BATTLEPASS_MISSION_SLOT_REWARD_POINT_IMAGE",  UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pImagePoint->SetVisibleSingle( TRUE );
	RegisterControl ( m_pImagePoint );

	m_pTextComplete = new CBasicTextBox;
	m_pTextComplete->CreateSub ( this, "RAN_BATTLEPASS_MISSION_SLOT_COMP_TEXT" );
	m_pTextComplete->SetFont ( pFont );
	m_pTextComplete->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pTextComplete->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextComplete );

	m_pTextTitle = new CBasicTextBox;
	m_pTextTitle->CreateSub ( this, "RAN_BATTLEPASS_MISSION_SLOT_TITLE_TEXT" );
	m_pTextTitle->SetFont ( pFont );
	m_pTextTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextTitle->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextTitle );

	m_pTextDesc = new CBasicTextBox;
	m_pTextDesc->CreateSub ( this, "RAN_BATTLEPASS_MISSION_SLOT_DESC_TEXT" );
	m_pTextDesc->SetFont ( pFont );
	m_pTextDesc->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	m_pTextDesc->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextDesc );

	m_pTextProgress = new CBasicTextBox;
	m_pTextProgress->CreateSub ( this, "RAN_BATTLEPASS_MISSION_SLOT_PROGRESS_TEXT" );
	m_pTextProgress->SetFont ( pFont );
	m_pTextProgress->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	m_pTextProgress->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextProgress );

	m_pTextPoint = new CBasicTextBox;
	m_pTextPoint->CreateSub ( this, "RAN_BATTLEPASS_MISSION_SLOT_REWARD_POINT_TEXT" );
	m_pTextPoint->SetFont ( pFont );
	m_pTextPoint->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pTextPoint->SetText( "--" , NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextPoint );
}

void CBattlePassListSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CBattlePassListSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.2f )
	{
		UpdateInfo();
	}
}

void CBattlePassListSlot::ResetData()
{
	m_fUpdateTime = 0.0f;
	m_dwBattlePassID = UINT_MAX;

	if ( m_pImagePoint )		m_pImagePoint->SetVisibleSingle( FALSE );

	if ( m_pTextComplete )		m_pTextComplete->ClearText();
	if ( m_pTextTitle )			m_pTextTitle->ClearText();
	if ( m_pTextDesc )			m_pTextDesc->ClearText();
	if ( m_pTextProgress )		m_pTextProgress->ClearText();
	if ( m_pTextPoint )			m_pTextPoint->ClearText();
}

void CBattlePassListSlot::SetData( DWORD dwID )
{
	ResetData();

	m_dwBattlePassID = dwID;

	UpdateInfo();
}

void CBattlePassListSlot::UpdateInfo()
{
	if ( m_pImagePoint )		m_pImagePoint->SetVisibleSingle( FALSE );

	if ( m_pTextComplete )		m_pTextComplete->ClearText();
	if ( m_pTextTitle )			m_pTextTitle->ClearText();
	if ( m_pTextDesc )			m_pTextDesc->ClearText();
	if ( m_pTextProgress )		m_pTextProgress->ClearText();
	if ( m_pTextPoint )			m_pTextPoint->ClearText();

	CString strTemp;

	SBATTLEPASS_FILE_DATA* pbattlepass_data = GLBattlePass::GetInstance().GetBattlePass( m_dwBattlePassID );
	if ( pbattlepass_data )
	{
		if ( m_pTextTitle )
		{
			strTemp.Format( "< %s >", pbattlepass_data->strBattlePassTitle.c_str() );
			m_pTextTitle->AddText( strTemp.GetString(), NS_UITEXTCOLOR::ORANGE );
		}

		if ( m_pTextDesc )			m_pTextDesc->AddText( pbattlepass_data->strBattlePassDescription.c_str(), NS_UITEXTCOLOR::WHITE );


		if ( pbattlepass_data->dwRewardPoint )
		{
			strTemp.Format( "%u", pbattlepass_data->dwRewardPoint );
			if ( m_pImagePoint )		m_pImagePoint->SetVisibleSingle( TRUE );
			if ( m_pTextPoint )			m_pTextPoint->AddText( strTemp.GetString(), NS_UITEXTCOLOR::GOLD );
		}


		SBATTLEPASS_CHAR_DATA* pbattlepass_data_done = GLGaeaClient::GetInstance().GetCharacter()->GetBattlePassDone( m_dwBattlePassID );
		if ( pbattlepass_data_done )
		{
			strTemp.Format( "%u/%u", pbattlepass_data_done->dwProgressMax, pbattlepass_data_done->dwProgressMax );
			if ( m_pTextProgress )		m_pTextProgress->AddText( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );

			if ( m_pTextComplete )		m_pTextComplete->AddText( ID2GAMEWORD( "RAN_BATTLEPASS_PROGRESS_TEXT", 1 ), NS_UITEXTCOLOR::PRIVATE );

			return;
		}

		SBATTLEPASS_CHAR_DATA* pbattlepass_data_prog = GLGaeaClient::GetInstance().GetCharacter()->GetBattlePassProg( m_dwBattlePassID );
		if ( pbattlepass_data_prog )
		{
			strTemp.Format( "%u/%u", pbattlepass_data_prog->dwProgressNow, pbattlepass_data_prog->dwProgressMax );

			if ( m_pTextProgress )		m_pTextProgress->AddText( strTemp.GetString(), NS_UITEXTCOLOR::WHITE );
			if ( m_pTextComplete )		m_pTextComplete->AddText( ID2GAMEWORD( "RAN_BATTLEPASS_PROGRESS_TEXT", 2 ), NS_UITEXTCOLOR::BRIGHTGREEN );
		}
	}
}