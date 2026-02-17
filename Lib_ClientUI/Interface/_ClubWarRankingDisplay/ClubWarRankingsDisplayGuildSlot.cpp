#include "StdAfx.h"
#include "ClubWarRankingsDisplayGuildSlot.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Engine/DxCommon/DxClubMan.h"
#include "../BasicLineBox.h"
#include "../BasicTextBox.h"
#include "../GameTextControl.h"
#include "../UITextControl.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../InnerInterface.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* 7-13-23 ClubWar War Ranking System - Montage */

CClubWarRankingsDisplayGuildSlot::CClubWarRankingsDisplayGuildSlot ()
: m_pTextRank(NULL)
, m_pTextClub(NULL)
, m_pTextScoreKill(NULL)
, m_pTextScoreDeath(NULL)
, m_pTextScoreResu(NULL)
, m_pIconGuild(NULL)
, m_pIconKill(NULL)
, m_pIconDeath(NULL)
, m_pIconResu(NULL)
{
}

CClubWarRankingsDisplayGuildSlot::~CClubWarRankingsDisplayGuildSlot ()
{
}

void CClubWarRankingsDisplayGuildSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_BASE_LINEBOX" );
	RegisterControl ( pLineBox );

	m_pLineBox = new CBasicLineBox;
	m_pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBox->CreateBaseBoxQuestList ( "PVP_TYRANNY_CLUB_RANKING_DISPLAY_SLOT_BASE_LINEBOX" );
	//RegisterControl ( m_pLineBox );

	m_pTextRank = new CBasicTextBox;
	m_pTextRank->CreateSub ( this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_TEXT_BASE_RANK" );
	m_pTextRank->SetFont ( pFont8 );
	m_pTextRank->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextRank->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextRank );

	m_pTextClub = new CBasicTextBox;
	m_pTextClub->CreateSub ( this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_TEXT_BASE_CLUB" );
	m_pTextClub->SetFont ( pFont8 );
	m_pTextClub->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextClub->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextClub );

	m_pTextScoreKill = new CBasicTextBox;
	m_pTextScoreKill->CreateSub ( this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_TEXT_BASE_SCORE_KILL" );
	m_pTextScoreKill->SetFont ( pFont8 );
	m_pTextScoreKill->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextScoreKill->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextScoreKill );

	m_pTextScoreDeath = new CBasicTextBox;
	m_pTextScoreDeath->CreateSub ( this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_TEXT_BASE_SCORE_DEATH" );
	m_pTextScoreDeath->SetFont ( pFont8 );
	m_pTextScoreDeath->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextScoreDeath->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextScoreDeath );

	m_pTextScoreResu = new CBasicTextBox;
	m_pTextScoreResu->CreateSub ( this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_TEXT_BASE_SCORE_RESU" );
	m_pTextScoreResu->SetFont ( pFont8 );
	m_pTextScoreResu->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextScoreResu->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextScoreResu );

	m_pIconGuild = new CUIControl;
	m_pIconGuild->CreateSub(this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_ICON_GUILD", UI_FLAG_DEFAULT);
	RegisterControl(m_pIconGuild);
	m_pIconGuild->SetUseRender(TRUE);

	m_pIconKill = new CUIControl;
	m_pIconKill->CreateSub(this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_ICON_KILL", UI_FLAG_DEFAULT);
	RegisterControl(m_pIconKill);

	m_pIconDeath = new CUIControl;
	m_pIconDeath->CreateSub(this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_ICON_DEATH", UI_FLAG_DEFAULT);
	RegisterControl(m_pIconDeath);

	m_pIconResu = new CUIControl;
	m_pIconResu->CreateSub(this, "PVP_TYRANNY_CLUB_RANKING_DISPLAY_ICON_RESU", UI_FLAG_DEFAULT);
	RegisterControl(m_pIconResu);
}

void CClubWarRankingsDisplayGuildSlot::ResetData()
{
	m_sClubWarClubRank	= SCLUBWAR_CLUB_RANK();

	if ( m_pTextRank )		m_pTextRank->ClearText();
	if ( m_pTextClub )		m_pTextClub->ClearText();
	if ( m_pTextScoreKill )		m_pTextScoreKill->ClearText();
	if ( m_pTextScoreDeath )		m_pTextScoreDeath->ClearText();
	if ( m_pTextScoreResu )		m_pTextScoreResu->ClearText();

	if ( m_pIconGuild )		m_pIconGuild->SetVisibleSingle(FALSE);

	if ( m_pIconKill )		m_pIconKill->SetVisibleSingle(FALSE);
	if ( m_pIconDeath )		m_pIconDeath->SetVisibleSingle(FALSE);
	if ( m_pIconResu )		m_pIconResu->SetVisibleSingle(FALSE);

}

HRESULT CClubWarRankingsDisplayGuildSlot::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
	if (!CInnerInterface::GetInstance().IsVisibleGroup(CLUB_WAR_RANKING_DISPLAY))
	{
		m_pIconGuild->SetVisibleSingle(FALSE);
		return CUIGroup::Render(pd3dDevice);
	}

	HRESULT hr = S_OK;

	m_pIconGuild->SetVisibleSingle(FALSE);

	{
		hr = CUIGroup::Render(pd3dDevice);
		if (FAILED(hr)) return hr;
	}

	GLCLUB& sCLUB = GLGaeaClient::GetInstance().GetCharacter ()->m_sCLUB;

	BOOL bGuild = m_sClubWarClubRank.wGuNum ? TRUE : FALSE;
	if (bGuild && m_sClubWarClubRank.wGuNum > 0)
	{
		m_pIconGuild->SetVisibleSingle(TRUE);
		const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sClubWarClubRank.dwClubID, m_sClubWarClubRank.wGuMarkVer);

		m_pIconGuild->SetTexturePos(0, sMarkData.vTex_1_LU);
		m_pIconGuild->SetTexturePos(1, sMarkData.vTex_2_RU);
		m_pIconGuild->SetTexturePos(3, sMarkData.vTex_3_LD);
		m_pIconGuild->SetTexturePos(2, sMarkData.vTex_4_RD);

		m_pIconGuild->SetTexture(sMarkData.pddsTexture);
		m_pIconGuild->Render(pd3dDevice);

	}

	return hr;
}

void CClubWarRankingsDisplayGuildSlot::SetData( SCLUBWAR_CLUB_RANK sClubWarClubRank, int nRank, std::string strClub, DWORD dwClubID, WORD wGuMarkVer, WORD wKill, WORD wDeath , WORD wResu )
{
	ResetData();

	m_sClubWarClubRank = sClubWarClubRank;

	CString strText;

	if ( m_pTextRank )
	{
		strText.Format( "%d",nRank );
		m_pTextRank->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pIconKill )		m_pIconKill->SetVisibleSingle(TRUE);
	if ( m_pIconDeath )		m_pIconDeath->SetVisibleSingle(TRUE);
	if ( m_pIconResu )		m_pIconResu->SetVisibleSingle(TRUE);

	if ( m_pTextClub )
	{
		m_pTextClub->AddText( strClub.c_str(), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextScoreDeath )
	{
		strText.Format( "%u", wDeath );
		m_pTextScoreDeath->AddText( strText.GetString(), NS_UITEXTCOLOR::ORNAGERED );
	}

	if ( m_pTextScoreResu )
	{
		strText.Format( "%u", wResu );
		m_pTextScoreResu->AddText( strText.GetString(), NS_UITEXTCOLOR::LIGHTYELLOW );
	}

	if ( m_pTextScoreKill )
	{
		strText.Format( "%u", wKill );
		m_pTextScoreKill->AddText( strText.GetString(), NS_UITEXTCOLOR::GREENYELLOW );
	}

	if ( dwClubID != CLUB_NULL )
	{
		DWORD dwCLUBID = GLGaeaClient::GetInstance().GetCharacter()->m_dwGuild;

		BOOL bSelfOwn = ( dwCLUBID == dwClubID );

		m_pLineBox->SetUseRender ( bSelfOwn );
		m_pLineBox->SetDiffuse( NS_UITEXTCOLOR::GREENYELLOW );
	}
}