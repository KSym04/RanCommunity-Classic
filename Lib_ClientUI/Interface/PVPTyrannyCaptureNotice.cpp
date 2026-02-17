#include "StdAfx.h"
#include "./PVPTyrannyCaptureNotice.h"
#include "../../Lib_Client/G-Logic/GLPVPTyrannyDefine.h"
#include "./InnerInterface.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPTyrannyCaptureNotice::CPVPTyrannyCaptureNotice ()
	: m_pImage(NULL)
	, m_pSchoolImage(NULL)
	, m_pImageClub(NULL)
	, m_pSchoolText(NULL)
	, m_fTimer(0.0f)
	, m_dwClubID(CLUB_NULL)
	, m_dwClubMarkVer(0)
{
}

CPVPTyrannyCaptureNotice::~CPVPTyrannyCaptureNotice ()
{
}

void CPVPTyrannyCaptureNotice::CreateSubControl ( int nType )
{
	if ( nType < 0 || nType >= TYRANNY_SCHOOL_SIZE )	return;

	std::string strSchooImage[TYRANNY_SCHOOL_SIZE] = 
	{
		"PROGRESS_DISPLAY_CTF_SCHOOL_0",
		"PROGRESS_DISPLAY_CTF_SCHOOL_1",
		"PROGRESS_DISPLAY_CTF_SCHOOL_2",
	};

	std::string strSchoolText[TYRANNY_SCHOOL_SIZE] = 
	{
		"PROGRESS_DISPLAY_CTF_TEXT_CAPTURED_BY_0",
		"PROGRESS_DISPLAY_CTF_TEXT_CAPTURED_BY_1",
		"PROGRESS_DISPLAY_CTF_TEXT_CAPTURED_BY_2",
	};

	
	m_pImage = new CUIControl;
	m_pImage->CreateSub ( this, "PROGRESS_DISPLAY_CTF_BACK_1" );
	RegisterControl ( m_pImage );

	m_pSchoolImage = new CUIControl;
	m_pSchoolImage->CreateSub ( this, strSchooImage[nType].c_str() );
	RegisterControl ( m_pSchoolImage );

	m_pSchoolText = new CUIControl;
	m_pSchoolText->CreateSub ( this, strSchoolText[nType].c_str() );
	RegisterControl ( m_pSchoolText );	

	m_pImageClub = new CUIControl;
	m_pImageClub->CreateSub ( this, "PROGRESS_DISPLAY_CTF_CLUB_ICO", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pImageClub->SetUseRender(TRUE);
	m_pImageClub->SetVisibleSingle( FALSE );
	RegisterControl ( m_pImageClub );
}

void CPVPTyrannyCaptureNotice::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fTimer += fElapsedTime;
	if ( m_fTimer >= 5.0f )
	{
		m_fTimer = 0.0f;
		CInnerInterface::GetInstance().HideGroup( GetWndID() );
	}
}

HRESULT CPVPTyrannyCaptureNotice::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
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

void CPVPTyrannyCaptureNotice::SetData( DWORD dwClubID, DWORD dwClubMarkVer )		
{ 
	m_dwClubID = dwClubID; 
	m_dwClubMarkVer = dwClubMarkVer; 
}