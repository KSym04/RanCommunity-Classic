#include "StdAfx.h"
#include "./BasicVarTextBoxExIcon.h"
#include "./SkillFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicVarTextBoxExIcon::CBasicVarTextBoxExIcon () 
	: m_pIcon( NULL )
	, m_pd3dDevice ( NULL )
	, m_sICONINDEX ( NATIVEID_NULL () )
{
}

CBasicVarTextBoxExIcon::~CBasicVarTextBoxExIcon ()
{
}

HRESULT CBasicVarTextBoxExIcon::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	m_pd3dDevice = pd3dDevice;

	return S_OK;
}

void CBasicVarTextBoxExIcon::CreateSubControl ()
{
	m_pIcon = new CUIControl;
	m_pIcon->CreateSub ( this, "ITEM_IMAGE" );	
	m_pIcon->SetUseRender ( TRUE );
	m_pIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pIcon );
}

void CBasicVarTextBoxExIcon::Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CBasicVarTextBoxExIcon::SetIcon( SNATIVEID sICONINDEX, const char* szTexture )
{	
	if ( !m_pIcon )	
	{
		return;
	}

	if ( (sICONINDEX == m_sICONINDEX) && (szTexture == m_pIcon->GetTextureName ()) )	
	{
		return ;
	}

	ResetIcon();

	if ( m_pIcon )
	{
		m_pIcon->SetTexturePos ( NS_SKILL::GetIconTexurePos ( sICONINDEX ) );
		m_pIcon->SetTextureName ( szTexture );	

		if ( m_pd3dDevice )
		{	
			m_pIcon->InitDeviceObjects ( m_pd3dDevice );
			m_pIcon->SetVisibleSingle ( TRUE );

			m_sICONINDEX = sICONINDEX;
			m_strTextureName = szTexture;
		}
	}
}

void CBasicVarTextBoxExIcon::ResetIcon()
{
	if ( m_pIcon )
	{
		if ( m_sICONINDEX != NATIVEID_NULL () )
		{
			m_pIcon->DeleteDeviceObjects ();
			m_pIcon->SetTextureName ( NULL );
			m_pIcon->SetVisibleSingle ( FALSE );

			m_sICONINDEX = NATIVEID_NULL ();
			m_strTextureName.Empty ();
		}		
	}
}

