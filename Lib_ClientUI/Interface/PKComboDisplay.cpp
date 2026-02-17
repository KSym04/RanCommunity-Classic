#include "StdAfx.h"
#include "./PKComboDisplay.h"
#include "./PKComboImageType.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./UITextControl.h"

#include "./InnerInterface.h"
#include "../../Lib_Engine/DxCommon/DxViewPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const float fKeepTime = 3.0f;

CPKComboDisplay::CPKComboDisplay ()
{
}

CPKComboDisplay::~CPKComboDisplay ()
{
}

void CPKComboDisplay::CreateSubControl ()
{	
	CString strKeyword[PK_COMBO_IMAGE_MAX] = 
	{
		"PK_COMBO_IMAGE_00",
		"PK_COMBO_IMAGE_01",
		"PK_COMBO_IMAGE_02",
		"PK_COMBO_IMAGE_03",
		"PK_COMBO_IMAGE_04",
		"PK_COMBO_IMAGE_05",
		"PK_COMBO_IMAGE_06",
		"PK_COMBO_IMAGE_07",
		"PK_COMBO_IMAGE_08",
		"PK_COMBO_IMAGE_09",
		"PK_COMBO_IMAGE_10",
		"PK_COMBO_IMAGE_11",
		"PK_COMBO_IMAGE_12",
		"PK_COMBO_IMAGE_13",
		"PK_COMBO_IMAGE_14",
		"PK_COMBO_IMAGE_15",
		"PK_COMBO_IMAGE_16",
		"PK_COMBO_IMAGE_17",
		"PK_COMBO_IMAGE_18",
		"PK_COMBO_IMAGE_19",
	};
   
	for ( int i = 0; i < PK_COMBO_IMAGE_MAX; ++i )
	{
		m_pTYPE[i] = new CPKComboImageType;
		m_pTYPE[i]->CreateSub ( this, strKeyword[i].GetString(), UI_FLAG_DEFAULT, PK_COMBO_IMAGE_00 + i );
		m_pTYPE[i]->CreateSubControl ( strKeyword[i] );
		m_pTYPE[i]->SetVisibleSingle ( FALSE );
		m_pTYPE[i]->SetUseRender ( FALSE );
		m_pTYPE[i]->STOP ();
		m_pTYPE[i]->RESET ();		
		RegisterControl ( m_pTYPE[i] );
	}

	m_pPositionControl = new CUIControl;
	m_pPositionControl->CreateSub ( this, "PK_COMBO_POSITION" );
	m_pPositionControl->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pPositionControl );	
}

bool	CPKComboDisplay::START ( UIGUID cID )
{
	if ( cID < PK_COMBO_IMAGE_00 || PK_COMBO_IMAGE_19 < cID ) return false;
	
	int nIndex = cID - PK_COMBO_IMAGE_00;

	m_pTYPE[nIndex]->SetVisibleSingle ( TRUE );
	m_pTYPE[nIndex]->START ();

	return true;
}

bool	CPKComboDisplay::RESET ( UIGUID cID )
{
	if ( cID < PK_COMBO_IMAGE_00 || PK_COMBO_IMAGE_19 < cID ) return false;

	int nIndex = cID - PK_COMBO_IMAGE_00;	
	m_pTYPE[nIndex]->RESET ();

    return true;
}

bool	CPKComboDisplay::STOP ( UIGUID cID )
{
	if ( cID < PK_COMBO_IMAGE_00 || PK_COMBO_IMAGE_19 < cID ) return false;

	int nIndex = cID - PK_COMBO_IMAGE_00;	
	m_pTYPE[nIndex]->STOP ();
	m_pTYPE[nIndex]->SetVisibleSingle ( FALSE );

	return true;
}

void CPKComboDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	D3DXVECTOR3 vPos = GLGaeaClient::GetInstance().GetCharacter()->GetPosBodyHeight();

	static D3DXVECTOR3 vScreenBack;
	D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );

	if( abs( vScreenBack.x - vScreen.x ) < 1.0f )
	{
		vScreen.x = vScreenBack.x;
	}
	
	bool bPLAYING( false );
	
	float fTime = 0.0f;

	for ( int i = 0; i < PK_COMBO_IMAGE_MAX; ++i )
	{
		const UIRECT& rcOriginPos = m_pTYPE[i]->GetGlobalPos ();

		D3DXVECTOR2 vPos;
		vPos.x = floor(vScreen.x - ( rcOriginPos.sizeX * 0.5f ));
		vPos.y = m_pPositionControl->GetGlobalPos().top;

		if ( m_pTYPE[i]->ISPLAYING () )
		{
			m_pTYPE[i]->SetGlobalPos ( vPos );

			bPLAYING = true;
		}
		else
		{
			STOP ( PK_COMBO_IMAGE_00 + i );
		}
	}

	vScreenBack = vScreen;

	if ( !bPLAYING ) CInnerInterface::GetInstance().HideGroup ( GetWndID () );
}

void CPKComboDisplay::ALLSTOP ()
{
	for ( int i = 0; i < PK_COMBO_IMAGE_MAX; ++i )
	{
		m_pTYPE[i]->STOP ();
		m_pTYPE[i]->RESET ();
		m_pTYPE[i]->SetVisibleSingle ( FALSE );
	}

	CInnerInterface::GetInstance().HideGroup ( GetWndID () );
}
