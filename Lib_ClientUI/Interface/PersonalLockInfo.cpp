#include "StdAfx.h"
#include "./PersonalLockInfo.h"

#include "./BasicLineBoxEx.h"

#include "./UITextControl.h"
#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./PersonalLockImage.h"
#include "../Lib_ClientUI/Interface/ItemShop/ItemShopButton.h"

#include "./PartyFinderIcon.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPersonalLockInfo::CPersonalLockInfo ()
	: m_bFirstGap(FALSE)

	, m_pImageBase(NULL)
	, m_pImageDummy(NULL)

	/* party finder, Juver, 2020/01/24 */
	, m_pPartyFinderIcon(NULL)
	, m_pItemShopButton(NULL) // Item Shop - JX

{
	for ( int i=0; i<IMAGE_SIZE; ++i )
		m_pImage[i] = NULL;
}

CPersonalLockInfo::~CPersonalLockInfo ()
{
}

void CPersonalLockInfo::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	m_pImageBase = new CUIControl;
	m_pImageBase->CreateSub ( this, "PERSONAL_LOCK_IMAGE_BASE" );
	m_pImageBase->SetVisibleSingle( TRUE );
	RegisterControl ( m_pImageBase );

	m_pImageDummy = new CUIControl;
	m_pImageDummy->CreateSub ( this, "PERSONAL_LOCK_IMAGE_DUMMY", UI_FLAG_DEFAULT, IMAGE_DUMMY );
	m_pImageDummy->SetVisibleSingle( TRUE );
	RegisterControl ( m_pImageDummy );


	std::string strImage[IMAGE_SIZE] = 
	{
		"PERSONAL_LOCK_IMAGE_PUTON",
		"PERSONAL_LOCK_IMAGE_INVEN",
		"PERSONAL_LOCK_IMAGE_LOCKER",
	};

	for ( int i=0; i<IMAGE_SIZE; ++i )
	{
		m_pImage[i] = new CPersonalLockImage;
		m_pImage[i]->CreateSub ( this, strImage[i].c_str(), UI_FLAG_DEFAULT, IMAGE_00 + i );
		m_pImage[i]->CreateSubControl();
		RegisterControl ( m_pImage[i] );
	}

	/* party finder, Juver, 2020/01/24 */
	if( RPARAM::bUsePartyFinder )
	{ 
		m_pPartyFinderIcon = new CPartyFinderIcon;
		m_pPartyFinderIcon->CreateSub ( this, "PARTY_FINDER_ICON", UI_FLAG_DEFAULT, PARTY_FINDER_ICON );
		m_pPartyFinderIcon->CreateSubControl ();
		RegisterControl ( m_pPartyFinderIcon );
	}

	if ( RPARAM::bGameItemShop )
	{
		m_pItemShopButton = new CItemShopButton;
		m_pItemShopButton->CreateSub ( this, "ITEM_SHOP_BUTTON", UI_FLAG_DEFAULT, ITEMSHOP_BUTTON );
		m_pItemShopButton->CreateSubControl ();
		RegisterControl ( m_pItemShopButton );
		//ShowGroupBottom(ITEMSHOP_BUTTON);
	}

}


void CPersonalLockInfo::CheckMouseState ()
{
	const DWORD dwMsg = GetMessageEx ();
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		if( UIMSG_LB_DUP & dwMsg )
		{
			AddMessageEx( UIMSG_PERSONAL_LOCK_LBDUP );
			SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
			return;
		}
		
		if ( dwMsg & UIMSG_LB_DOWN )
		{
			SetExclusiveControl();	

			if ( !m_bFirstGap )
			{
				UIRECT rcPos = GetGlobalPos ();
				m_vGap.x = m_PosX - rcPos.left;
				m_vGap.y = m_PosY - rcPos.top;
				m_bFirstGap = TRUE;

				SetDiffuse ( D3DCOLOR_ARGB ( 0xB4, 0xFF, 0xFF, 0xFF ) );
			}
		}
		else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
		{
			ResetExclusiveControl();

			m_bFirstGap = FALSE;				

			SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
		}
	}
	else if ( CHECK_LB_UP_LIKE ( dwMsg ) )		
	{								
		ResetExclusiveControl();
		m_bFirstGap = FALSE;					

		SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
	}
}


void CPersonalLockInfo::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	m_bCHECK_MOUSE_STATE = false;

	m_PosX = x;
	m_PosY = y;

	for ( int i=0; i<IMAGE_SIZE; ++i )
	{
		if ( m_pImage[i] )
			m_pImage[i]->SetImageHover( FALSE );
	}

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( IsExclusiveSelfControl() )
	{
		SetGlobalPos ( D3DXVECTOR2 ( x - m_vGap.x, y - m_vGap.y ) );
	}

	CheckLockStatus();
}

void CPersonalLockInfo::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case IMAGE_DUMMY:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( !m_bCHECK_MOUSE_STATE )
				{
					CheckMouseState ();
					m_bCHECK_MOUSE_STATE = true;
				}
			}
		}break;

	case IMAGE_00:
	case IMAGE_01:
	case IMAGE_02:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - IMAGE_00;
				if ( nSelect < IMAGE_SIZE )
				{
					if ( m_pImage[nSelect] )
						m_pImage[nSelect]->SetImageHover( TRUE );

					CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD( "PERSONAL_LOCK_INFO", nSelect ), NS_UITEXTCOLOR::WHITE );	

					if ( CHECK_LB_UP_LIKE ( dwMsg ) )		
					{	
						EMPERSONAL_LOCK emLock = ImageToEnum( nSelect );
						GLGaeaClient::GetInstance().GetCharacter()->PersonalLockRequest( emLock );
					}
				}
			}
		}break;
	};

	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CPersonalLockInfo::CheckLockStatus()
{
	for ( int i=0; i<IMAGE_SIZE; ++i )
	{
		if ( m_pImage[i] )
		{
			m_pImage[i]->SetImageLock( FALSE );

			EMPERSONAL_LOCK emLock = ImageToEnum( i );
			BOOL bLock = GLGaeaClient::GetInstance().GetCharacter()->isPersonalLock( emLock );
			m_pImage[i]->SetImageLock( bLock );
		}
	}
}

EMPERSONAL_LOCK CPersonalLockInfo::ImageToEnum( int nIndex )
{
	switch ( nIndex )
	{
	case 0: return EMPERSONAL_LOCK_EQUIP;
	case 1: return EMPERSONAL_LOCK_INVEN;
	case 2: return EMPERSONAL_LOCK_LOCKER;

	default: return EMPERSONAL_LOCK_SIZE;
	};
}