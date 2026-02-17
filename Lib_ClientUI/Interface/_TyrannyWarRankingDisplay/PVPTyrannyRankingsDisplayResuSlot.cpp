#include "StdAfx.h"
#include "PVPTyrannyRankingsDisplayResuSlot.h"
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

/* 7-13-23 Tyranny War Ranking System - Montage */

CPVPTyrannyRankingsDisplayResuSlot::CPVPTyrannyRankingsDisplayResuSlot ()
: m_pTextRank(NULL)
, m_pTextName(NULL)
, m_pTextScoreKill(NULL)
, m_pTextScoreDeath(NULL)
, m_pTextScoreResu(NULL)
, m_pIconGuild(NULL)
, m_pIconDeath(NULL)
, m_pIconResu(NULL)
{

	for( int i = 0; i < PVP_TYRANNY_RANKING_DISPLAY_ICON_CLASS; ++i )
	{
		m_pIconClass[i] = NULL;
		m_pIconKill[i] = NULL;
	}
	for( int i = 0; i < nSCHOOL_MARK; ++i )
	{
		m_pSchoolMark[i] = NULL;
	}
}

CPVPTyrannyRankingsDisplayResuSlot::~CPVPTyrannyRankingsDisplayResuSlot ()
{
}

void CPVPTyrannyRankingsDisplayResuSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBox = new CBasicLineBox;
	m_pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBox->CreateBaseBoxQuestList ( "PVP_TYRANNY_RANKING_RESU_DISPLAY_SLOT_BASE_LINEBOX" );
	RegisterControl ( m_pLineBox );

	std::string strClassIcon[PVP_TYRANNY_RANKING_DISPLAY_ICON_CLASS] = 
	{
		"PVP_TYRANNY_RANKING_DISPLAY_BRAWLER_MALE",
		"PVP_TYRANNY_RANKING_DISPLAY_SWORDMAN_MALE",
		"PVP_TYRANNY_RANKING_DISPLAY_ARCHER_FEMALE",
		"PVP_TYRANNY_RANKING_DISPLAY_SHAMAN_FEMALE",
		"PVP_TYRANNY_RANKING_DISPLAY_EXTREME_MALE",
		"PVP_TYRANNY_RANKING_DISPLAY_EXTREME_FEMALE",
		"PVP_TYRANNY_RANKING_DISPLAY_BRAWLER_FEMALE",
		"PVP_TYRANNY_RANKING_DISPLAY_SWORDMAN_FEMALE",
		"PVP_TYRANNY_RANKING_DISPLAY_ARCHER_MALE",
		"PVP_TYRANNY_RANKING_DISPLAY_SHAMAN_MALE",
		"PVP_TYRANNY_RANKING_DISPLAY_GUNNER_MALE",
		"PVP_TYRANNY_RANKING_DISPLAY_GUNNER_FEMALE",
		"PVP_TYRANNY_RANKING_DISPLAY_ASSASSIN_MALE",
		"PVP_TYRANNY_RANKING_DISPLAY_ASSASSIN_FEMALE",
		"PVP_TYRANNY_RANKING_DISPLAY_TRICKER_MALE",
		"PVP_TYRANNY_RANKING_DISPLAY_TRICKER_FEMALE",
	};

	for( int i = 0; i < PVP_TYRANNY_RANKING_DISPLAY_ICON_CLASS; ++i )
	{
		m_pIconClass[i] = new CUIControl;
		m_pIconClass[i]->CreateSub ( this, strClassIcon[i].c_str() );	
		m_pIconClass[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pIconClass[i] );
	}

	m_pTextRank = new CBasicTextBox;
	m_pTextRank->CreateSub ( this, "PVP_TYRANNY_RANKING_RESU_DISPLAY_SLOT_TEXT_BASE_RANK" );
	m_pTextRank->SetFont ( pFont8 );
	m_pTextRank->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextRank->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextRank );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "PVP_TYRANNY_RANKING_RESU_DISPLAY_SLOT_TEXT_BASE_NAME" );
	m_pTextName->SetFont ( pFont8 );
	m_pTextName->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextName->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextName );

	m_pTextScoreKill = new CBasicTextBox;
	m_pTextScoreKill->CreateSub ( this, "PVP_TYRANNY_RANKING_RESU_DISPLAY_SLOT_TEXT_BASE_SCORE_KILL" );
	m_pTextScoreKill->SetFont ( pFont8 );
	m_pTextScoreKill->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextScoreKill->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextScoreKill );

	m_pTextScoreDeath = new CBasicTextBox;
	m_pTextScoreDeath->CreateSub ( this, "PVP_TYRANNY_RANKING_RESU_DISPLAY_SLOT_TEXT_BASE_SCORE_DEATH" );
	m_pTextScoreDeath->SetFont ( pFont8 );
	m_pTextScoreDeath->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextScoreDeath->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextScoreDeath );

	m_pTextScoreResu = new CBasicTextBox;
	m_pTextScoreResu->CreateSub ( this, "PVP_TYRANNY_RANKING_RESU_DISPLAY_SLOT_TEXT_BASE_SCORE_RESU" );
	m_pTextScoreResu->SetFont ( pFont8 );
	m_pTextScoreResu->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	m_pTextScoreResu->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextScoreResu );

	CUIControl	TempControl;
	TempControl.Create ( 1, "PVP_TYRANNY_RANKING_SCHOOL_MARK" );
	const UIRECT& rcLocalPos = TempControl.GetLocalPos ();
	std::string strSchoolMark[nSCHOOL_MARK] = 
	{
		"PVP_TYRANNY_RANKING_SCHOOL1",
		"PVP_TYRANNY_RANKING_SCHOOL2",
		"PVP_TYRANNY_RANKING_SCHOOL3"
	};

	for ( int i = 0; i < nSCHOOL_MARK; ++i )
	{
		m_pSchoolMark[i] = new CUIControl;
		m_pSchoolMark[i]->CreateSub ( this, strSchoolMark[i].c_str() );
		m_pSchoolMark[i]->SetLocalPos ( rcLocalPos );
		m_pSchoolMark[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolMark[i] );
	}

	m_pIconGuild = new CUIControl;
	m_pIconGuild->CreateSub(this, "PVP_TYRANNY_RANKING_DISPLAY_SLOT_ICON_GUILD", UI_FLAG_DEFAULT);
	RegisterControl(m_pIconGuild);
	m_pIconGuild->SetUseRender(TRUE);

	std::string strClassKill[PVP_TYRANNY_RANKING_DISPLAY_ICON_CLASS] = 
	{
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_BRAWLER_MALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_SWORDMAN_MALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_ARCHER_FEMALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_SHAMAN_FEMALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_EXTREME_MALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_EXTREME_FEMALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_BRAWLER_FEMALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_SWORDMAN_FEMALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_ARCHER_MALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_SHAMAN_MALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_GUNNER_MALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_GUNNER_FEMALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_ASSASSIN_MALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_ASSASSIN_FEMALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_TRICKER_MALE",
		"PVP_TYRANNY_RANKING_RESU_DISPLAY_KILL_TRICKER_FEMALE",
	};

	for( int i = 0; i < PVP_TYRANNY_RANKING_DISPLAY_ICON_CLASS; ++i )
	{
		m_pIconKill[i] = new CUIControl;
		m_pIconKill[i]->CreateSub ( this, strClassKill[i].c_str() );	
		m_pIconKill[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pIconKill[i] );
	}

	m_pIconDeath = new CUIControl;
	m_pIconDeath->CreateSub(this, "PVP_TYRANNY_RANKING_RESU_DISPLAY_SLOT_ICON_DEATH", UI_FLAG_DEFAULT);
	RegisterControl(m_pIconDeath);


	m_pIconResu = new CUIControl;
	m_pIconResu->CreateSub(this, "PVP_TYRANNY_RANKING_RESU_DISPLAY_SLOT_ICON_RESU", UI_FLAG_DEFAULT);
	RegisterControl(m_pIconResu);
}

void CPVPTyrannyRankingsDisplayResuSlot::ResetData()
{
	m_sTyrannyRank	= STYRANNY_RANK_RESU();

	if ( m_pTextRank )		m_pTextRank->ClearText();
	if ( m_pTextName )		m_pTextName->ClearText();
	if ( m_pTextScoreKill )	m_pTextScoreKill->ClearText();
	if ( m_pTextScoreDeath )	m_pTextScoreDeath->ClearText();
	if ( m_pTextScoreResu )	m_pTextScoreResu->ClearText();

	for( int i = 0; i < PVP_TYRANNY_RANKING_DISPLAY_ICON_CLASS; ++i )
	{
		m_pIconClass[i]->SetVisibleSingle ( FALSE );	
		m_pIconKill[i]->SetVisibleSingle( FALSE );
	}

	for( int i = 0; i < nSCHOOL_MARK; i++ )
	{
		m_pSchoolMark[i]->SetVisibleSingle(FALSE);
	}

	if ( m_pIconGuild )		m_pIconGuild->SetVisibleSingle(FALSE);

	if ( m_pIconDeath )		m_pIconDeath->SetVisibleSingle(FALSE);
	if ( m_pIconResu )		m_pIconResu->SetVisibleSingle(FALSE);

}

HRESULT CPVPTyrannyRankingsDisplayResuSlot::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
	if (!CInnerInterface::GetInstance().IsVisibleGroup(PVP_TYRANNY_RANKING_DISPLAY))
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

	BOOL bGuild = m_sTyrannyRank.wGuNum ? TRUE : FALSE;
	if (bGuild && m_sTyrannyRank.wGuNum > 0)
	{
		m_pIconGuild->SetVisibleSingle(TRUE);
		const int nSERVER					= GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData	= DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sTyrannyRank.wGuNum, m_sTyrannyRank.wGuMarkVer);

		m_pIconGuild->SetTexturePos(0, sMarkData.vTex_1_LU);
		m_pIconGuild->SetTexturePos(1, sMarkData.vTex_2_RU);
		m_pIconGuild->SetTexturePos(3, sMarkData.vTex_3_LD);
		m_pIconGuild->SetTexturePos(2, sMarkData.vTex_4_RD);

		m_pIconGuild->SetTexture(sMarkData.pddsTexture);
		m_pIconGuild->Render(pd3dDevice);

	}

	return hr;
}

void CPVPTyrannyRankingsDisplayResuSlot::SetData( STYRANNY_RANK_RESU sTyrannyRank, int nRank, std::string strName, DWORD dwCharID, int nClass,
											 WORD wGuNum, WORD wGuMarkVer, WORD wKill, WORD wDeath , WORD wResu, WORD wSchool )
{
	ResetData();

	m_sTyrannyRank = sTyrannyRank;

	CString strText;

	if ( m_pTextRank )
	{
		strText.Format( "%d",nRank );
		m_pTextRank->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextName )
	{
		m_pTextName->AddText( strName.c_str(), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pIconDeath )		m_pIconDeath->SetVisibleSingle(TRUE);
	if ( m_pIconResu )		m_pIconResu->SetVisibleSingle(TRUE);

	m_pIconClass[nClass]->SetVisibleSingle ( TRUE );	
	m_pIconKill[nClass]->SetVisibleSingle ( TRUE );	
	m_pSchoolMark[wSchool]->SetVisibleSingle ( TRUE );

	if ( m_pTextScoreKill )
	{
		strText.Format( "%u", wKill );
		m_pTextScoreKill->AddText( strText.GetString(), NS_UITEXTCOLOR::GREENYELLOW );
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

	if ( dwCharID != UINT_MAX )
	{
		DWORD dwCHARID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;

		BOOL bSelfOwn = ( dwCharID == dwCHARID );

		m_pLineBox->SetUseRender ( bSelfOwn );
		m_pLineBox->SetDiffuse( NS_UITEXTCOLOR::GREENYELLOW );
	}
}