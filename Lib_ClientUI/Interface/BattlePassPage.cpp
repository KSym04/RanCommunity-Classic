#include "StdAfx.h"
#include "./BattlePassPage.h"

#include "./BattlePassList.h"
#include "./BattlePassReward.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextBoxEx.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicButton.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLBattlePass.h"

#include "./InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-9-14, Battle Pass - CNDev*/

CBattlePassPage::CBattlePassPage ()	
	: m_pTextMissionTitle(NULL)
	, m_pTextRewardTitle(NULL)
	, m_pBattlePassList(NULL)
	, m_pBattlePassReward(NULL)
{
}

CBattlePassPage::~CBattlePassPage ()
{
}

void CBattlePassPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_STUDENTRECORD_OBJECT_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhite ( "RAN_STUDENTRECORD_OBJECT_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_BATTLEPASS_LINEBOX_MISSION_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_BATTLEPASS_LINEBOX_MISSION_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_BATTLEPASS_LINEBOX_MISSION_CONTENT" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_BATTLEPASS_LINEBOX_MISSION_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_BATTLEPASS_LINEBOX_MISSION_CONTENT" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_BATTLEPASS_LINEBOX_REWARD_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_BATTLEPASS_LINEBOX_REWARD_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_BATTLEPASS_LINEBOX_REWARD_CONTENT" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_BATTLEPASS_LINEBOX_REWARD_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_BATTLEPASS_LINEBOX_REWARD_CONTENT" );
	RegisterControl ( pLineBox );

	m_pTextMissionTitle = new CBasicTextBox;
	m_pTextMissionTitle->CreateSub ( this, "RAN_BATTLEPASS_STATIC_MISSION_TITLE" );
	m_pTextMissionTitle->SetFont ( pFont9 );
	m_pTextMissionTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextMissionTitle->SetText( ID2GAMEWORD( "RAN_BATTLEPASS_WINDOW_MISSION", 1 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextMissionTitle );

	m_pTextRewardTitle = new CBasicTextBox;
	m_pTextRewardTitle->CreateSub ( this, "RAN_BATTLEPASS_STATIC_REWARD_TITLE" );
	m_pTextRewardTitle->SetFont ( pFont9 );
	m_pTextRewardTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRewardTitle->SetText( ID2GAMEWORD( "RAN_BATTLEPASS_WINDOW_MISSION", 2 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextRewardTitle );

	m_pBattlePassList = new CBattlePassList;
	m_pBattlePassList->CreateSub( this, "RAN_BATTLEPASS_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE, BATTLEPASS_LIST );
	m_pBattlePassList->CreateSubControl ();
	RegisterControl ( m_pBattlePassList );

	m_pBattlePassReward = new CBattlePassReward;
	m_pBattlePassReward->CreateSub( this, "RAN_BATTLEPASS_REWARD", UI_FLAG_XSIZE | UI_FLAG_YSIZE, BATTLEPASS_REWARD );
	m_pBattlePassReward->CreateSubControl ();
	RegisterControl ( m_pBattlePassReward );
}

void CBattlePassPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	default:
		break;
	};
}

void CBattlePassPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CBattlePassPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		LoadData();
	}
}

void CBattlePassPage::LoadData()
{
	if ( m_pBattlePassList )	m_pBattlePassList->ResetData();
	if ( m_pBattlePassReward )	m_pBattlePassReward->ResetData();

	m_pBattlePassList->LoadData();
	m_pBattlePassReward->LoadData();
}