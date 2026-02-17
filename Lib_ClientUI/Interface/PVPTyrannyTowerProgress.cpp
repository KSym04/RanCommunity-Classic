#include "StdAfx.h"
#include "./PVPTyrannyTowerProgress.h"

#include "./BasicProgressBar.h"
#include "./InnerInterface.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../../Lib_Engine/DxCommon/DxClubMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPTyrannyTowerProgress::CPVPTyrannyTowerProgress ()
	: m_dwClubID(CLUB_NULL)
	, m_dwClubMarkVer(0)
{
	for ( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		m_pTowerStatus[i] = NULL;
		m_pTowerOwner[i] = NULL;
	}
}

CPVPTyrannyTowerProgress::~CPVPTyrannyTowerProgress ()
{
}

void CPVPTyrannyTowerProgress::CreateSubControl ()
{
	std::string strProgress[TYRANNY_SCHOOL_SIZE] = 
	{
		"AUTHENTICATOR_CTF_DISPLAY_IMAGE_SM",
		"AUTHENTICATOR_CTF_DISPLAY_IMAGE_HA",
		"AUTHENTICATOR_CTF_DISPLAY_IMAGE_BH",
	};

	std::string strProgressOver[TYRANNY_SCHOOL_SIZE] = 
	{
		"AUTHENTICATOR_CTF_DISPLAY_IMAGE_SM_OVER",
		"AUTHENTICATOR_CTF_DISPLAY_IMAGE_HA_OVER",
		"AUTHENTICATOR_CTF_DISPLAY_IMAGE_BH_OVER",	
	};

	std::string strOwner[TYRANNY_SCHOOL_SIZE] = 
	{
		"AUTHENTICATOR_CTF_DISPLAY_IMAGE_SM_OWNER",
		"AUTHENTICATOR_CTF_DISPLAY_IMAGE_HA_OWNER",
		"AUTHENTICATOR_CTF_DISPLAY_IMAGE_BH_OWNER",
	};

	for ( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		m_pTowerStatus[i] = new CBasicProgressBar;
		m_pTowerStatus[i]->CreateSub ( this, strProgress[i].c_str() );
		m_pTowerStatus[i]->CreateOverImage ( (char*)strProgressOver[i].c_str() );
		m_pTowerStatus[i]->SetType( CBasicProgressBar::VERTICAL );
		m_pTowerStatus[i]->SetPercent( 0.0f );
		RegisterControl ( m_pTowerStatus[i] );

		m_pTowerOwner[i] = new CUIControl;
		m_pTowerOwner[i]->CreateSub ( this, strOwner[i].c_str() );
		m_pTowerOwner[i]->SetVisibleSingle( FALSE );
		RegisterControl ( m_pTowerOwner[i] );
	}

	m_pImageClub = new CUIControl;
	m_pImageClub->CreateSub ( this, "AUTHENTICATOR_CTF_DISPLAY_IMAGE_CLUB_OWNER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pImageClub->SetUseRender(TRUE);
	m_pImageClub->SetVisibleSingle( FALSE );
	RegisterControl ( m_pImageClub );
}

void CPVPTyrannyTowerProgress::UpdateInfo( WORD wOwner, float* fDamage, DWORD dwClubID, DWORD dwClubMarkVer )
{
	m_dwClubID = dwClubID; 
	m_dwClubMarkVer = dwClubMarkVer; 

	for ( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		if ( m_pTowerStatus[i] )
			m_pTowerStatus[i]->SetPercent( 0.0f );

		if ( m_pTowerOwner[i] )
			m_pTowerOwner[i]->SetVisibleSingle( FALSE );
	}

	
	if ( wOwner < TYRANNY_SCHOOL_SIZE )
	{
		if ( m_pTowerOwner[wOwner] )
			m_pTowerOwner[wOwner]->SetVisibleSingle( TRUE );
	}

	for ( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		float fpercent = fDamage[i];
		if ( m_pTowerStatus[i] && fpercent > 0.0f )
			m_pTowerStatus[i]->SetPercent( fpercent / 100.0f );
	}
}

HRESULT CPVPTyrannyTowerProgress::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	if ( !IsVisible () )											return S_OK;
	
	HRESULT hr = S_OK;
	m_pImageClub->SetVisibleSingle ( FALSE );

	hr = CUIGroup::Render ( pd3dDevice );
	if( FAILED ( hr ) )												return hr;
	if ( m_dwClubID == CLUB_NULL )									return hr;

	int nID = static_cast<int>( m_dwClubID );
	int nServer = static_cast<int>(GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID);
	DWORD dwVer = m_dwClubMarkVer;

	DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nServer, nID, dwVer );

	m_pImageClub->SetTexturePos ( 0, sMarkData.vTex_1_LU );
	m_pImageClub->SetTexturePos ( 1, sMarkData.vTex_2_RU );
	m_pImageClub->SetTexturePos ( 3, sMarkData.vTex_3_LD );
	m_pImageClub->SetTexturePos ( 2, sMarkData.vTex_4_RD );

	m_pImageClub->SetVisibleSingle ( TRUE );
	m_pImageClub->SetTexture ( sMarkData.pddsTexture );
	{
		hr = m_pImageClub->Render ( pd3dDevice );
		if( FAILED ( hr ) ) return hr;
	}

	return S_OK;
}
