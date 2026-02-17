#include "stdafx.h"
#include "./PVPClubWarRankingPage.h"

#include "./PVPClubWarRankingPageSlot.h"

#include "../../Lib_ClientUI/Interface/BasicLineBox.h"
#include "../../Lib_ClientUI/Interface/BasicLineBoxEx.h"
#include "../../Lib_ClientUI/Interface/BasicTextBox.h"
#include "../../Lib_ClientUI/Interface/BasicTextButton.h"
#include "../../Lib_ClientUI/Interface/BasicScrollBarEx.h"
#include "../../Lib_ClientUI/Interface/BasicScrollThumbFrame.h"
#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPVPClubWarRankingPage::CPVPClubWarRankingPage ()
	: m_nType(0)
	, m_nStart(0)
	, m_nTotal(0)
	, m_pData(NULL)
	, m_pTextRank(NULL)
	, m_pTextSchool(NULL)
	, m_pTextClass(NULL)
	, m_pTextGuild(NULL)
	, m_pTextName(NULL)
	, m_pTextKill(NULL)
	, m_pTextDeath(NULL)
	, m_pTextResu(NULL)
	, m_pTextScore(NULL)
	, m_pTextPoint(NULL)
	, m_pSlotRankSelf(NULL)
	, m_pScrollBar(NULL)
	, m_pTextGuildOwner(NULL)
	, m_pImageGuildOwner(NULL)
	, m_pTextWinnerBonus(NULL)
{
	for ( int i=0; i<PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT; ++i )
	{
		m_pSlotRank[i] = NULL;
	}
}

CPVPClubWarRankingPage::~CPVPClubWarRankingPage ()
{
}

CBasicTextBox* CPVPClubWarRankingPage::CreateUITextBox( const char* szControl, CD3DFontPar* pFont, int nTextAlign )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szControl );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( nTextAlign );
	RegisterControl ( pTextBox );
	return pTextBox;
}

void CPVPClubWarRankingPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CLUB_WAR_RANKING_PAGE_LINEBOX" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CLUB_WAR_RANKING_PAGE_MYRANK_LINE_HEAD" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CLUB_WAR_RANKING_PAGE_MYRANK_LINE_TAIL" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_CLUB_WAR_RANKING_PAGE_RANK_SCROLLBAR_BACKGROUND" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "PVP_CLUB_WAR_RANKING_PAGE_LINEBOX" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "PVP_CLUB_WAR_RANKING_PAGE_LINEBOX_HEAD" );
	RegisterControl ( pLineBox );

	m_pTextRank = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_RANK", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextSchool = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_SCHOOL", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextClass = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_CLASS", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextGuild = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_GUILD", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextName = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_NAME", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	
	m_pTextKill = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_KILL", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextDeath = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_DEATH", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextResu = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_REBIRTH", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextScore = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_SCORE", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPoint = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_REWARD", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	m_pTextRank->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 0 ), NS_UITEXTCOLOR::WHITE );
	m_pTextSchool->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 1 ), NS_UITEXTCOLOR::WHITE );
	m_pTextClass->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 2 ), NS_UITEXTCOLOR::WHITE );
	m_pTextGuild->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 3 ), NS_UITEXTCOLOR::WHITE );
	m_pTextName->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 4 ), NS_UITEXTCOLOR::WHITE );
	
	m_pTextKill->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 5 ), NS_UITEXTCOLOR::WHITE );
	m_pTextDeath->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 6 ), NS_UITEXTCOLOR::WHITE );
	m_pTextResu->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 7 ), NS_UITEXTCOLOR::WHITE );
	m_pTextScore->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 8 ), NS_UITEXTCOLOR::WHITE );
	m_pTextPoint->AddText( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_BASE", 9 ), NS_UITEXTCOLOR::WHITE );

	std::string strSlot[PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT] = 
	{
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_0",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_1",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_2",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_3",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_4",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_5",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_6",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_7",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_8",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_9",
	};

	for( int i=0; i<PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT; ++i )
	{
		m_pSlotRank[i] = new CPVPClubWarRankingPageSlot;
		m_pSlotRank[i]->CreateSub ( this, strSlot[i].c_str() );
		m_pSlotRank[i]->CreateSubControl ( FALSE );
		m_pSlotRank[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pSlotRank[i] );	
	}

	m_pSlotRankSelf = new CPVPClubWarRankingPageSlot;
	m_pSlotRankSelf->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_SLOT_10" );
	m_pSlotRankSelf->CreateSubControl ( TRUE );
	m_pSlotRankSelf->SetVisibleSingle( FALSE );
	RegisterControl ( m_pSlotRankSelf );	

	m_pScrollBar = new CBasicScrollBarEx;
	m_pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, PVP_CLUB_WAR_RANKING_PAGE_SCROLLBAR );
	m_pScrollBar->CreateBaseScrollBar ( "PVP_CLUB_WAR_RANKING_PAGE_RANK_SCROLLBAR" );
	m_pScrollBar->GetThumbFrame()->SetState ( 1, PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT );
	RegisterControl ( m_pScrollBar );


	m_pTextGuildOwner = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_INFO_GUILD_OWNER", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextWinnerBonus = CreateUITextBox( "PVP_CLUB_WAR_RANKING_PAGE_INFO_REWARD", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	m_pImageGuildOwner = new CUIControl;
	m_pImageGuildOwner->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_INFO_GUILD_OWNER_BADGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pImageGuildOwner->SetVisibleSingle( FALSE );
	m_pImageGuildOwner->SetUseRender ( TRUE );
	RegisterControl ( m_pImageGuildOwner );
}

void CPVPClubWarRankingPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_nTotal = 0;
	if ( m_pData )
		m_nTotal = m_pData->vecPlayerData.size();

	if ( m_pScrollBar )
	{
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal();

			if( nTotal != m_nTotal )
				m_pScrollBar->GetThumbFrame()->SetState ( m_nTotal, PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT );	
		}
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal ();
			if ( nTotal <= PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT ) return ;
			const int nViewPerPage = pThumbFrame->GetViewPerPage ();

			if ( nViewPerPage < nTotal )
			{
				int nCurPos = 0;
				const int nMovableLine = nTotal - nViewPerPage;
				float fPercent = pThumbFrame->GetPercent ();
				nCurPos = (int)floor(fPercent * nMovableLine);
				if ( nCurPos < 0 ) nCurPos = 0;
				if ( m_nStart == nCurPos ) return;
				m_nStart = nCurPos;
				RenderView();
			}
		}
	}
}

HRESULT CPVPClubWarRankingPage::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	if ( !IsVisible () ) return S_OK;

	HRESULT hr = S_OK;

	m_pImageGuildOwner->SetVisibleSingle ( FALSE );
	{
		hr = CUIGroup::Render ( pd3dDevice );
		if ( FAILED ( hr ) ) return hr;
	}
	m_pImageGuildOwner->SetVisibleSingle ( TRUE );

	if ( m_pData )
	{
		const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nSERVER, m_pData->dwOwnerGuildID, m_pData->dwGuildMarkVer );

		m_pImageGuildOwner->SetTexturePos ( 0, sMarkData.vTex_1_LU );
		m_pImageGuildOwner->SetTexturePos ( 1, sMarkData.vTex_2_RU );
		m_pImageGuildOwner->SetTexturePos ( 3, sMarkData.vTex_3_LD );
		m_pImageGuildOwner->SetTexturePos ( 2, sMarkData.vTex_4_RD );

		const UIRECT& rcGlobalPos = m_pImageGuildOwner->GetGlobalPos ();
		m_pImageGuildOwner->SetGlobalPos( UIRECT( ceil(rcGlobalPos.left), ceil(rcGlobalPos.top), ceil(rcGlobalPos.sizeX), ceil(rcGlobalPos.sizeY) ) );

		m_pImageGuildOwner->SetTexture ( sMarkData.pddsTexture );
		hr = m_pImageGuildOwner->Render ( pd3dDevice );
	}

	return hr;
}

void CPVPClubWarRankingPage::SetData( SPVP_CLUB_WAR_CLIENT_DATA* pData )
{
	m_pData = pData;

	RefreshRanking();
}

void CPVPClubWarRankingPage::RefreshRanking()
{
	m_nTotal = 0;

	if ( m_pSlotRankSelf )
	{
		m_pSlotRankSelf->SetVisibleSingle( FALSE );
		m_pSlotRankSelf->SetData( NULL );
	}

	if ( m_pTextGuildOwner )
		m_pTextGuildOwner->ClearText();

	if ( m_pTextWinnerBonus )
		m_pTextWinnerBonus->ClearText();

	if ( m_pData )
	{
		m_nTotal = m_pData->vecPlayerData.size();

		if ( m_pSlotRankSelf )
		{
			if ( m_pData->sRankSelf.dwCharID != PVP_CLUB_WAR_PLAYER_DATA_CHARID_NULL )
			{
				m_pSlotRankSelf->SetVisibleSingle( TRUE );
				m_pSlotRankSelf->SetData( &m_pData->sRankSelf );
			}
		}

		if ( m_pTextGuildOwner )
			m_pTextGuildOwner->AddText( m_pData->szOwnerGuildName, NS_UITEXTCOLOR::BRIGHTGREEN );

		if ( m_pTextWinnerBonus )
		{
			CString strText;
			strText.Format( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_INFO", 0 ), m_pData->fBonusMain * 100.0f );
			int nIndex = m_pTextWinnerBonus->AddText( strText.GetString(), NS_UITEXTCOLOR::GREENYELLOW );
			strText.Format( ID2GAMEWORD( "PVP_CLUB_WAR_RANKING_WINDOW_PAGE_TEXT_INFO", 1 ), m_pData->fBonusAlly * 100.0f );
			m_pTextWinnerBonus->AddString( nIndex, strText.GetString(), NS_UITEXTCOLOR::YELLOW );
		}
	}
	
	RenderView();
}

void CPVPClubWarRankingPage::RenderView()
{
	for ( int i=0; i < PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT; ++ i )
	{
		if ( m_pSlotRank[i] )
		{
			m_pSlotRank[i]->SetVisibleSingle( FALSE );
			m_pSlotRank[i]->SetData( NULL );
		}
	}

	if ( m_pData )
	{
		SPVP_CLUB_WAR_PLAYER_DATA_VEC& vecRank = m_pData->vecPlayerData;

		int nEnd = m_nStart + PVP_CLUB_WAR_RANKING_PAGE_MAX_SLOT;
		int nSlot = 0;

		for ( int i = m_nStart; i < nEnd; ++ i )
		{
			if ( i >= (int)vecRank.size() )	continue;
			if ( m_pSlotRank[nSlot] )
			{
				SPVP_CLUB_WAR_PLAYER_DATA &sRANK = vecRank[i];
				m_pSlotRank[nSlot]->SetVisibleSingle( TRUE );
				m_pSlotRank[nSlot]->SetData( &sRANK );
				nSlot ++ ;
			}
		}
	}
	
}
