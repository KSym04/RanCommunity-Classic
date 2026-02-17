#include "StdAfx.h"
#include "./PVPClubDeathMatchTopRankingsSlot.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPClubDeathMatchTopRankingsSlot::CPVPClubDeathMatchTopRankingsSlot ()
	: m_pTextRank(NULL)
	, m_pTextName(NULL)
	, m_pImageClub(NULL)
	, m_pTextPoint(NULL)
	, m_dwClubID(PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL)
	, m_dwClubMarkVer(0)
{
}

CPVPClubDeathMatchTopRankingsSlot::~CPVPClubDeathMatchTopRankingsSlot ()
{
}

void CPVPClubDeathMatchTopRankingsSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pTextRank = new CBasicTextBox;
	m_pTextRank->CreateSub ( this, "PVP_CLUB_DEATH_MATCH_TOP_RANKING_TEXT_BASE_RANK" );
	m_pTextRank->SetFont ( pFont8 );
	m_pTextRank->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextRank->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextRank );

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub ( this, "PVP_CLUB_DEATH_MATCH_TOP_RANKING_TEXT_BASE_CLUB_NAME" );
	m_pTextName->SetFont ( pFont8 );
	m_pTextName->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextName->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextName );

	m_pTextPoint = new CBasicTextBox;
	m_pTextPoint->CreateSub ( this, "PVP_CLUB_DEATH_MATCH_TOP_RANKING_TEXT_BASE_POINTS" );
	m_pTextPoint->SetFont ( pFont8 );
	m_pTextPoint->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPoint->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextPoint );

	m_pImageClub = new CUIControl;
	m_pImageClub->CreateSub ( this, "PVP_CLUB_DEATH_MATCH_TOP_RANKING_TEXT_BASE_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pImageClub->SetUseRender ( TRUE );
	RegisterControl ( m_pImageClub );
}

void CPVPClubDeathMatchTopRankingsSlot::ResetData()
{
	if ( m_pTextRank )		m_pTextRank->ClearText();
	if ( m_pTextName )		m_pTextName->ClearText();
	if ( m_pTextPoint )		m_pTextPoint->ClearText();

	m_dwClubID = PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL;
	m_dwClubMarkVer = 0;
}

void CPVPClubDeathMatchTopRankingsSlot::SetData( PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK* pData )
{
	ResetData();

	if ( !pData )	return;
	CString strText;

	if ( m_pTextRank )
	{
		strText.Format( "%u", pData->wRank );
		m_pTextRank->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextName )
	{
		strText.Format( "%s", pData->szClubName );
		m_pTextName->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	if ( m_pTextPoint )
	{
		strText.Format( "%u", pData->wPoints );
		m_pTextPoint->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	m_dwClubID = pData->dwClubID;
	m_dwClubMarkVer = pData->dwCLUB_MARK_VER;
}

HRESULT CPVPClubDeathMatchTopRankingsSlot::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
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