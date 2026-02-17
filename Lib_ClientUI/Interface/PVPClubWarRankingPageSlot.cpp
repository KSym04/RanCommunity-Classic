#include "stdafx.h"
#include "./PVPClubWarRankingPageSlot.h"

#include "../../Lib_ClientUI/Interface/BasicLineBox.h"
#include "../../Lib_ClientUI/Interface/BasicLineBoxEx.h"
#include "../../Lib_ClientUI/Interface/MultiModeButton.h"
#include "../../Lib_ClientUI/Interface/BasicTextBox.h"
#include "../../Lib_ClientUI/Interface/BasicTextButton.h"

#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubWarRankingPageSlot::CPVPClubWarRankingPageSlot ()
	: m_bSelf(FALSE)
	, m_pData(NULL)
	, m_pTextRank(NULL)
	, m_pTextGuild(NULL)
	, m_pTextName(NULL)

	, m_pTextKill(NULL)
	, m_pTextDeath(NULL)
	, m_pTextResu(NULL)
	, m_pTextScore(NULL)
	, m_pTextReward(NULL)
	, m_pGuildImage(NULL)
	, m_pImageWarChips(NULL)
{
	for( int i=0; i<PVP_CLUB_WAR_RANKING_PAGE_SLOT_SCHOOL_SIZE; ++i )
		m_pSchoolImage[i] = NULL;

	for( int i=0; i<GLCI_NUM_8CLASS; ++i )
		m_pClassImage[i] = NULL;
}

CPVPClubWarRankingPageSlot::~CPVPClubWarRankingPageSlot ()
{
}

void CPVPClubWarRankingPageSlot::CreateSubControl( BOOL bSelf )
{
	m_bSelf = bSelf;

	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
	if ( m_bSelf )	dwTextColor = NS_UITEXTCOLOR::GREENYELLOW;

	m_pTextRank = new CBasicTextBox;
	m_pTextRank->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_RANK" );
	m_pTextRank->SetFont ( pFont8 );
	m_pTextRank->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRank->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextRank );

	m_pTextGuild = new CBasicTextBox;
	m_pTextGuild->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_GUILD" );
	m_pTextGuild->SetFont ( pFont8 );
	m_pTextGuild->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextGuild->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextGuild );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_NAME" );
	m_pTextName->SetFont ( pFont8 );
	m_pTextName->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextName->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextName );

	
	m_pTextKill = new CBasicTextBox;
	m_pTextKill->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_KILL" );
	m_pTextKill->SetFont ( pFont8 );
	m_pTextKill->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextKill->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextKill );

	m_pTextDeath = new CBasicTextBox;
	m_pTextDeath->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_DEATH" );
	m_pTextDeath->SetFont ( pFont8 );
	m_pTextDeath->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextDeath->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextDeath );

	m_pTextResu = new CBasicTextBox;
	m_pTextResu->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_REBIRTH" );
	m_pTextResu->SetFont ( pFont8 );
	m_pTextResu->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextResu->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextResu );

	m_pTextScore = new CBasicTextBox;
	m_pTextScore->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_SCORE" );
	m_pTextScore->SetFont ( pFont8 );
	m_pTextScore->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextScore->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextScore );

	m_pTextReward = new CBasicTextBox;
	m_pTextReward->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_TEXT_BASE_REWARD" );
	m_pTextReward->SetFont ( pFont8 );
	m_pTextReward->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextReward->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextReward );

	std::string strschool_image[TYRANNY_SCHOOL_SIZE] = 
	{
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_SCHOOL0",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_SCHOOL1",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_SCHOOL2",
	};

	std::string strclass_image[GLCI_NUM_8CLASS] = 
	{
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_BRAWLER_MALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_SWORDMAN_MALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_ARCHER_FEMALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_SHAMAN_FEMALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_EXTREME_MALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_EXTREME_FEMALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_BRAWLER_FEMALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_SWORDMAN_FEMALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_ARCHER_MALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_SHAMAN_MALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_SCIENTIST_MALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_SCIENTIST_FEMALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_ASSASSIN_MALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_ASSASSIN_FEMALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_TRICKER_MALE",
		"PVP_CLUB_WAR_RANKING_PAGE_RANKSLOT_IMAGE_TRICKER_FEMALE",
	};

	for( int i=0; i<PVP_CLUB_WAR_RANKING_PAGE_SLOT_SCHOOL_SIZE; ++i )
	{
		m_pSchoolImage[i] = new CUIControl;
		m_pSchoolImage[i]->CreateSub ( this, strschool_image[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pSchoolImage[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pSchoolImage[i] );
	}

	for( int i=0; i<GLCI_NUM_8CLASS; ++i )
	{
		m_pClassImage[i] = new CUIControl;
		m_pClassImage[i]->CreateSub ( this, strclass_image[i].c_str(), UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClassImage[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pClassImage[i] );
	}

	m_pGuildImage = new CUIControl;
	m_pGuildImage->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_IMAGE_BASE_GUILD", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pGuildImage->SetVisibleSingle( FALSE );
	m_pGuildImage->SetUseRender ( TRUE );
	RegisterControl ( m_pGuildImage );

	m_pImageWarChips = new CUIControl;
	m_pImageWarChips->CreateSub ( this, "PVP_CLUB_WAR_RANKING_PAGE_IMAGE_BASE_WAR_CHIPS", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pImageWarChips->SetVisibleSingle( FALSE );
	RegisterControl ( m_pImageWarChips );
}

void CPVPClubWarRankingPageSlot::ResetData()
{
	if ( m_pTextRank )		m_pTextRank->ClearText();
	if ( m_pTextGuild )		m_pTextGuild->ClearText();
	if ( m_pTextName )		m_pTextName->ClearText();
	
	if ( m_pTextKill )		m_pTextKill->ClearText();
	if ( m_pTextDeath )		m_pTextDeath->ClearText();
	if ( m_pTextResu )		m_pTextResu->ClearText();
	if ( m_pTextScore )		m_pTextScore->ClearText();
	if ( m_pTextReward )	m_pTextReward->ClearText();

	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		if ( m_pSchoolImage[i] )
			m_pSchoolImage[i]->SetVisibleSingle( FALSE );
	}

	for( int i=0; i<GLCI_NUM_8CLASS; ++i )
	{
		if ( m_pClassImage[i] )
			m_pClassImage[i]->SetVisibleSingle( FALSE );
	}

	if ( m_pGuildImage )
		m_pGuildImage->SetVisibleSingle( FALSE );

	if ( m_pImageWarChips )
		m_pImageWarChips->SetVisibleSingle( FALSE );

	m_pData = NULL;
}

void CPVPClubWarRankingPageSlot::SetData( SPVP_CLUB_WAR_PLAYER_DATA* pData )
{
	ResetData();

	if ( !pData )	return;

	m_pData = pData;

	CString strText;

	DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;

	if ( pData->wState == EMPVP_CLUB_WAR_PLAYER_STATE_WINNER_MAIN )	
		dwTextColor = NS_UITEXTCOLOR::GREENYELLOW;

	if ( pData->wState == EMPVP_CLUB_WAR_PLAYER_STATE_WINNER_ALLY )	
		dwTextColor = NS_UITEXTCOLOR::YELLOW;

	if ( m_pTextRank )
	{
		strText.Format( "%u", pData->wRankAll );
		m_pTextRank->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextGuild )
	{
		strText.Format( "%s", pData->szGuildName );
		m_pTextGuild->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextName )
	{
		strText.Format( "%s", pData->szCharName );
		m_pTextName->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextKill )
	{
		strText.Format( "%u", pData->wScoreKill );
		m_pTextKill->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextDeath )
	{
		strText.Format( "%u", pData->wScoreDeath );
		m_pTextDeath->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextResu )
	{
		strText.Format( "%u", pData->wScoreResu );
		m_pTextResu->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextScore )
	{
		strText.Format( "%u", pData->wScoreTotal );
		m_pTextScore->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextReward )
	{
		strText.Format( "%u", pData->wRewardBase );

		if ( pData->wRewardBonus > 0 )
			strText.Format( "%u(+%u)", pData->wRewardBase, pData->wRewardBonus );

		m_pTextReward->AddText( strText.GetString(), dwTextColor );
	}

	if ( pData->wCharSchool < PVP_CLUB_WAR_RANKING_PAGE_SLOT_SCHOOL_SIZE )
	{
		if ( m_pSchoolImage[pData->wCharSchool] )
			m_pSchoolImage[pData->wCharSchool]->SetVisibleSingle( TRUE );
	}

	if ( pData->wCharClass < GLCI_NUM_8CLASS )
	{
		if ( m_pClassImage[pData->wCharClass] )
			m_pClassImage[pData->wCharClass]->SetVisibleSingle( TRUE );
	}

	if ( m_pImageWarChips )
		m_pImageWarChips->SetVisibleSingle( TRUE );
}

HRESULT CPVPClubWarRankingPageSlot::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	if ( !IsVisible () ) return S_OK;

	HRESULT hr = S_OK;

	m_pGuildImage->SetVisibleSingle ( FALSE );
	{
		hr = CUIGroup::Render ( pd3dDevice );
		if ( FAILED ( hr ) ) return hr;
	}
	m_pGuildImage->SetVisibleSingle ( TRUE );

	if ( m_pData )
	{
		const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nSERVER, m_pData->dwGuildID, m_pData->dwGuildMarkVer );

		m_pGuildImage->SetTexturePos ( 0, sMarkData.vTex_1_LU );
		m_pGuildImage->SetTexturePos ( 1, sMarkData.vTex_2_RU );
		m_pGuildImage->SetTexturePos ( 3, sMarkData.vTex_3_LD );
		m_pGuildImage->SetTexturePos ( 2, sMarkData.vTex_4_RD );

		const UIRECT& rcGlobalPos = m_pGuildImage->GetGlobalPos ();
		m_pGuildImage->SetGlobalPos( UIRECT( ceil(rcGlobalPos.left), ceil(rcGlobalPos.top), ceil(rcGlobalPos.sizeX), ceil(rcGlobalPos.sizeY) ) );

		m_pGuildImage->SetTexture ( sMarkData.pddsTexture );
		hr = m_pGuildImage->Render ( pd3dDevice );
	}
	

	return hr;
}