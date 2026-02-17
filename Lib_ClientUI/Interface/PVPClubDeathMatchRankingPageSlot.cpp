#include "StdAfx.h"
#include "./PVPClubDeathMatchRankingPageSlot.h"


#include "./BasicTextBox.h"
#include "./BasicButton.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubDeathMatchRankingPageSlot::CPVPClubDeathMatchRankingPageSlot ()
	: m_pTextRank(NULL)
	, m_pTextName(NULL)
	, m_pTextMaster(NULL)
	, m_pTextPoint(NULL)
	, m_pTextKill(NULL)
	, m_pTextDeath(NULL)
	, m_pTextResu(NULL)
	, m_pImageClub(NULL)
	, m_pInfoButton(NULL)
	, m_bSelf(FALSE)
{
}

CPVPClubDeathMatchRankingPageSlot::~CPVPClubDeathMatchRankingPageSlot ()
{
}

void CPVPClubDeathMatchRankingPageSlot::CreateSubControl( BOOL bSelf )
{
	m_bSelf = bSelf;

	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
	if ( m_bSelf )	dwTextColor = NS_UITEXTCOLOR::GREENYELLOW;

	m_pTextRank = new CBasicTextBox;
	m_pTextRank->CreateSub ( this, "RNCDM_RANK_TEXTBOX" );
	m_pTextRank->SetFont ( pFont8 );
	m_pTextRank->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRank->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextRank );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "RNCDM_CLUB_NAME_TEXTBOX" );
	m_pTextName->SetFont ( pFont8 );
	m_pTextName->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextName->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextName );

	m_pTextMaster = new CBasicTextBox;
	m_pTextMaster->CreateSub ( this, "RNCDM_CLUB_MASTER_TEXTBOX" );
	m_pTextMaster->SetFont ( pFont8 );
	m_pTextMaster->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextMaster->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextMaster );

	m_pTextPoint = new CBasicTextBox;
	m_pTextPoint->CreateSub ( this, "RNCDM_GET_POINT_TEXTBOX" );
	m_pTextPoint->SetFont ( pFont8 );
	m_pTextPoint->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPoint->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextPoint );

	m_pTextKill = new CBasicTextBox;
	m_pTextKill->CreateSub ( this, "RNCDM_KINN_NUM_TEXTBOX" );
	m_pTextKill->SetFont ( pFont8 );
	m_pTextKill->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextKill->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextKill );

	m_pTextDeath = new CBasicTextBox;
	m_pTextDeath->CreateSub ( this, "RNCDM_DEATH_NUM_TEXTBOX" );
	m_pTextDeath->SetFont ( pFont8 );
	m_pTextDeath->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextDeath->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextDeath );

	m_pTextResu = new CBasicTextBox;
	m_pTextResu->CreateSub ( this, "RNCDM_REBIRTH_TEXTBOX" );
	m_pTextResu->SetFont ( pFont8 );
	m_pTextResu->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextResu->SetText( "--", dwTextColor );
	RegisterControl ( m_pTextResu );

	m_pImageClub = new CUIControl;
	m_pImageClub->CreateSub ( this, "RN_CDM_RANKSLOT_IMAGE_CLUB_MARK_POS", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pImageClub->SetUseRender( TRUE );
	RegisterControl ( m_pImageClub );

	m_pInfoButton = new CBasicButton;
	m_pInfoButton->CreateSub( this, "PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_INFO_BUTTON", UI_FLAG_DEFAULT, PVP_CLUB_DEATH_MATCH_RANKING_PAGE_SLOT_BUTTON_INFO );
	m_pInfoButton->CreateFlip( "PVP_CLUB_DEATH_MATCH_RESULT_RANKSLOT_INFO_BUTTON_FLIP", CBasicButton::MOUSEIN_FLIP );
	RegisterControl( m_pInfoButton );
	
}

void CPVPClubDeathMatchRankingPageSlot::ResetData()
{
	if ( m_pTextRank )		m_pTextRank->ClearText();
	if ( m_pTextName )		m_pTextName->ClearText();
	if ( m_pTextMaster )	m_pTextMaster->ClearText();
	if ( m_pTextPoint )		m_pTextPoint->ClearText();
	if ( m_pTextKill )		m_pTextKill->ClearText();
	if ( m_pTextDeath )		m_pTextDeath->ClearText();
	if ( m_pTextResu )		m_pTextResu->ClearText();
	if ( m_pTextPoint )		m_pTextPoint->ClearText();

	if ( m_pInfoButton )	m_pInfoButton->SetVisibleSingle( FALSE );

	m_dwClubID = PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL;
	m_dwClubMarkVer = 0;
}

void CPVPClubDeathMatchRankingPageSlot::SetData( PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK* pData )
{
	ResetData();

	if ( !pData )	return;
	CString strText;

	DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
	if ( m_bSelf )	dwTextColor = NS_UITEXTCOLOR::GREENYELLOW;

	if ( m_pTextRank )
	{
		strText.Format( "%u", pData->wRank );
		m_pTextRank->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextName )
	{
		strText.Format( "%s", pData->szClubName );
		m_pTextName->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextMaster )
	{
		strText.Format( "%s", pData->szMasterName );
		m_pTextMaster->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextPoint )
	{
		strText.Format( "%u", pData->wPoints );
		m_pTextPoint->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextKill )
	{
		strText.Format( "%u", pData->wKills );
		m_pTextKill->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextDeath )
	{
		strText.Format( "%u", pData->wDeaths );
		m_pTextDeath->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pTextResu )
	{
		strText.Format( "%u", pData->wResu );
		m_pTextResu->AddText( strText.GetString(), dwTextColor );
	}

	if ( m_pInfoButton )	m_pInfoButton->SetVisibleSingle( TRUE );

	m_dwClubID = pData->dwClubID;
	m_dwClubMarkVer = pData->dwCLUB_MARK_VER;
}

HRESULT CPVPClubDeathMatchRankingPageSlot::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	if ( !IsVisible () ) return S_OK;

	HRESULT hr = S_OK;

	m_pImageClub->SetVisibleSingle ( FALSE );
	{
		hr = CUIGroup::Render ( pd3dDevice );
		if ( FAILED ( hr ) ) return hr;
	}
	m_pImageClub->SetVisibleSingle ( TRUE );

	if ( m_dwClubID != PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )
	{
		const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nSERVER, m_dwClubID, m_dwClubMarkVer );

		m_pImageClub->SetTexturePos ( 0, sMarkData.vTex_1_LU );
		m_pImageClub->SetTexturePos ( 1, sMarkData.vTex_2_RU );
		m_pImageClub->SetTexturePos ( 3, sMarkData.vTex_3_LD );
		m_pImageClub->SetTexturePos ( 2, sMarkData.vTex_4_RD );

		const UIRECT& rcGlobalPos = m_pImageClub->GetGlobalPos ();
		m_pImageClub->SetGlobalPos( UIRECT( ceil(rcGlobalPos.left), ceil(rcGlobalPos.top), ceil(rcGlobalPos.sizeX), ceil(rcGlobalPos.sizeY) ) );

		m_pImageClub->SetTexture ( sMarkData.pddsTexture );
		hr = m_pImageClub->Render ( pd3dDevice );
	}
	else
	{
		m_pImageClub->SetVisibleSingle ( FALSE );
	}

	return hr;
}

void CPVPClubDeathMatchRankingPageSlot::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case PVP_CLUB_DEATH_MATCH_RANKING_PAGE_SLOT_BUTTON_INFO:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
			}
		}break;

	};
}