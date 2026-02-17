#include "stdafx.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "./BasicLineBox.h"
#include "./BasicTextBoxEx.h"

#include "./PKMarkSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPKMarkSlot::CPKMarkSlot()
	: m_pTextNameA( NULL )
	, m_pTextNameB( NULL )
	, m_pLineBoxBG( NULL )
	, m_pImageClubMarkA( NULL )
	, m_pImageClubMarkB( NULL )
{
	for( int i = 0; i < PKMARK_SLOT_IMAGE_CLASS; ++i )
	{
		m_pImageClassA[i] = NULL;
		m_pImageClassB[i] = NULL;
	}

	for( int i = 0; i < PKMARK_SLOT_IMAGE_SCHOOL; ++i )
	{
		m_pImageSchoolA[i] = NULL;
		m_pImageSchoolB[i] = NULL;
	}

	for( int i = 0; i < PKMARK_SLOT_IMAGE_WEAPON; ++i )
	{
		m_pWeaponImage[i] = NULL;
	}
}

CPKMarkSlot::~CPKMarkSlot()
{
}

void CPKMarkSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBoxBG = new CBasicLineBox;
	m_pLineBoxBG->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxBG->CreateBaseBoxQuestList ( "PKMARK_SLOT_LINEBOX_BG" );
	RegisterControl ( m_pLineBoxBG );

	std::string strSchoolA[PKMARK_SLOT_IMAGE_SCHOOL] = 
	{
		"PKMARK_SLOT_SCHOOL_A_SM",
		"PKMARK_SLOT_SCHOOL_A_HA",
		"PKMARK_SLOT_SCHOOL_A_BH",
	};

	std::string strSchoolB[PKMARK_SLOT_IMAGE_SCHOOL] = 
	{
		"PKMARK_SLOT_SCHOOL_B_SM",
		"PKMARK_SLOT_SCHOOL_B_HA",
		"PKMARK_SLOT_SCHOOL_B_BH",
	};

	std::string strClassA[PKMARK_SLOT_IMAGE_CLASS] = 
	{
		"PKMARK_SLOT_CLASS_A_BRAWLER_MALE",
		"PKMARK_SLOT_CLASS_A_SWORDMAN_MALE",
		"PKMARK_SLOT_CLASS_A_ARCHER_FEMALE",
		"PKMARK_SLOT_CLASS_A_SHAMAN_FEMALE",
		"PKMARK_SLOT_CLASS_A_EXTREME_MALE",
		"PKMARK_SLOT_CLASS_A_EXTREME_FEMALE",
		"PKMARK_SLOT_CLASS_A_BRAWLER_FEMALE",
		"PKMARK_SLOT_CLASS_A_SWORDMAN_FEMALE",
		"PKMARK_SLOT_CLASS_A_ARCHER_MALE",
		"PKMARK_SLOT_CLASS_A_SHAMAN_MALE",
		"PKMARK_SLOT_CLASS_A_SCIENTIST_MALE",
		"PKMARK_SLOT_CLASS_A_SCIENTIST_FEMALE",
		"PKMARK_SLOT_CLASS_A_ASSASSIN_MALE",
		"PKMARK_SLOT_CLASS_A_ASSASSIN_FEMALE",
		"PKMARK_SLOT_CLASS_A_TRICKER_MALE",
		"PKMARK_SLOT_CLASS_A_TRICKER_FEMALE",
	};

	std::string strClassB[PKMARK_SLOT_IMAGE_CLASS] = 
	{
		"PKMARK_SLOT_CLASS_B_BRAWLER_MALE",
		"PKMARK_SLOT_CLASS_B_SWORDMAN_MALE",
		"PKMARK_SLOT_CLASS_B_ARCHER_FEMALE",
		"PKMARK_SLOT_CLASS_B_SHAMAN_FEMALE",
		"PKMARK_SLOT_CLASS_B_EXTREME_MALE",
		"PKMARK_SLOT_CLASS_B_EXTREME_FEMALE",
		"PKMARK_SLOT_CLASS_B_BRAWLER_FEMALE",
		"PKMARK_SLOT_CLASS_B_SWORDMAN_FEMALE",
		"PKMARK_SLOT_CLASS_B_ARCHER_MALE",
		"PKMARK_SLOT_CLASS_B_SHAMAN_MALE",
		"PKMARK_SLOT_CLASS_B_SCIENTIST_MALE",
		"PKMARK_SLOT_CLASS_B_SCIENTIST_FEMALE",
		"PKMARK_SLOT_CLASS_B_ASSASSIN_MALE",
		"PKMARK_SLOT_CLASS_B_ASSASSIN_FEMALE",
		"PKMARK_SLOT_CLASS_B_TRICKER_MALE",
		"PKMARK_SLOT_CLASS_B_TRICKER_FEMALE",
	};

	std::string strWeapon[PKMARK_SLOT_IMAGE_WEAPON] = 
	{
		"PKMARK_SLOT_KILL_NONE",
		"PKMARK_SLOT_KILL_SWORD",
		"PKMARK_SLOT_KILL_BLADE",
		"PKMARK_SLOT_KILL_DAGGER",
		"PKMARK_SLOT_KILL_SPEAR",
		"PKMARK_SLOT_KILL_STICK",
		"PKMARK_SLOT_KILL_GAUNT",
		"PKMARK_SLOT_KILL_BOW",
		"PKMARK_SLOT_KILL_THROW",
		"PKMARK_SLOT_KILL_GUN",
		"PKMARK_SLOT_KILL_RAIL_GUN",
		"PKMARK_SLOT_KILL_PORTAL_GUN",
		"PKMARK_SLOT_KILL_SCYTHE",
		"PKMARK_SLOT_KILL_DUAL_SPEAR",
		"PKMARK_SLOT_KILL_DUAL_SHURIKEN",
	};


	for( int i = 0; i < PKMARK_SLOT_IMAGE_SCHOOL; ++i )
	{
		m_pImageSchoolA[i] = new CUIControl;
		m_pImageSchoolA[i]->CreateSub ( this, strSchoolA[i].c_str() );	
		m_pImageSchoolA[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pImageSchoolA[i] );

		m_pImageSchoolB[i] = new CUIControl;
		m_pImageSchoolB[i]->CreateSub ( this, strSchoolB[i].c_str() );	
		m_pImageSchoolB[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pImageSchoolB[i] );
	}

	for( int i = 0; i < PKMARK_SLOT_IMAGE_CLASS; ++i )
	{
		m_pImageClassA[i] = new CUIControl;
		m_pImageClassA[i]->CreateSub ( this, strClassA[i].c_str() );	
		m_pImageClassA[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pImageClassA[i] );

		m_pImageClassB[i] = new CUIControl;
		m_pImageClassB[i]->CreateSub ( this, strClassB[i].c_str() );	
		m_pImageClassB[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pImageClassB[i] );
	}

	for( int i = 0; i < PKMARK_SLOT_IMAGE_WEAPON; ++i )
	{
		m_pWeaponImage[i] = new CUIControl;
		m_pWeaponImage[i]->CreateSub ( this, strWeapon[i].c_str() );	
		m_pWeaponImage[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pWeaponImage[i] );
	}

	m_pTextNameA = new CBasicTextBoxEx;
	m_pTextNameA->CreateSub ( this, "PKMARK_SLOT_NAME_A" );
	m_pTextNameA->SetFont ( pFont8 );		
	m_pTextNameA->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextNameA->SetText( "0xBADF00D");
	RegisterControl ( m_pTextNameA );

	m_pTextNameB = new CBasicTextBoxEx;
	m_pTextNameB->CreateSub ( this, "PKMARK_SLOT_NAME_B" );
	m_pTextNameB->SetFont ( pFont8 );		
	m_pTextNameB->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextNameB->SetText( "0xBADF00D");
	RegisterControl ( m_pTextNameB );

	m_pImageClubMarkA = new CUIControl;
	m_pImageClubMarkA->CreateSub ( this, "PKMARK_SLOT_CLUB_A", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pImageClubMarkA->SetUseRender(TRUE);
	m_pImageClubMarkA->SetVisibleSingle( FALSE );
	RegisterControl ( m_pImageClubMarkA );

	m_pImageClubMarkB = new CUIControl;
	m_pImageClubMarkB->CreateSub ( this, "PKMARK_SLOT_CLUB_B", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pImageClubMarkB->SetUseRender(TRUE);
	m_pImageClubMarkB->SetVisibleSingle( FALSE );
	RegisterControl ( m_pImageClubMarkB );
}

void CPKMarkSlot::ResetData()
{
	for ( int i=0; i<PKMARK_SLOT_IMAGE_SCHOOL; ++i )
	{
		if (m_pImageSchoolA[i])
			m_pImageSchoolA[i]->SetVisibleSingle( FALSE );

		if (m_pImageSchoolB[i])
			m_pImageSchoolB[i]->SetVisibleSingle( FALSE );
	}

	for( int i = 0; i < PKMARK_SLOT_IMAGE_CLASS; ++i )
	{
		if (m_pImageClassA[i])
			m_pImageClassA[i]->SetVisibleSingle(FALSE);

		if (m_pImageClassB[i])
			m_pImageClassB[i]->SetVisibleSingle(FALSE);
	}

	for( int i = 0; i < PKMARK_SLOT_IMAGE_WEAPON; ++i )
	{
		if (m_pWeaponImage[i])
			m_pWeaponImage[i]->SetVisibleSingle(FALSE);
	}

	if ( m_pTextNameA )
		m_pTextNameA->ClearText();

	if ( m_pTextNameB )
		m_pTextNameB->ClearText();

	if (m_pImageClubMarkA)
		m_pImageClubMarkA->SetVisibleSingle( FALSE );

	if (m_pImageClubMarkB)
		m_pImageClubMarkB->SetVisibleSingle( FALSE );
}

void CPKMarkSlot::SetData( const SPKMARK_INFO& sData )
{
	m_sMarkData = sData;

	if ( sData.m_wSchoolA < PKMARK_SLOT_IMAGE_SCHOOL && sData.m_dwClubA == CLUB_NULL )
	{
		if (m_pImageSchoolA[sData.m_wSchoolA])
			m_pImageSchoolA[sData.m_wSchoolA]->SetVisibleSingle( TRUE );
	}

	if ( sData.m_wSchoolB < PKMARK_SLOT_IMAGE_SCHOOL && sData.m_dwClubB == CLUB_NULL )
	{
		if (m_pImageSchoolB[sData.m_wSchoolB])
			m_pImageSchoolB[sData.m_wSchoolB]->SetVisibleSingle( TRUE );
	}

	if ( sData.m_dwClubA != CLUB_NULL )
	{
		if (m_pImageClubMarkA)
			m_pImageClubMarkA->SetVisibleSingle( TRUE );
	}

	if ( sData.m_dwClubB != CLUB_NULL )
	{
		if (m_pImageClubMarkB)
			m_pImageClubMarkB->SetVisibleSingle( TRUE );
	}

	if ( sData.m_emClassA < PKMARK_SLOT_IMAGE_CLASS )
	{
		if (m_pImageClassA[sData.m_emClassA])
			m_pImageClassA[sData.m_emClassA]->SetVisibleSingle( TRUE );
	}

	if ( sData.m_emClassB < PKMARK_SLOT_IMAGE_CLASS )
	{
		if (m_pImageClassB[sData.m_emClassB])
			m_pImageClassB[sData.m_emClassB]->SetVisibleSingle( TRUE );
	}

	if ( sData.m_emAttack < PKMARK_SLOT_IMAGE_WEAPON )
	{
		if (m_pWeaponImage[sData.m_emAttack])
			m_pWeaponImage[sData.m_emAttack]->SetVisibleSingle( TRUE );
	}
	else
	{
		if (m_pWeaponImage[0])
			m_pWeaponImage[0]->SetVisibleSingle( TRUE );
	}

	if ( m_pTextNameA )
		m_pTextNameA->SetText( sData.m_szNameA );

	if ( m_pTextNameB )
		m_pTextNameB->SetText( sData.m_szNameB );

	DWORD dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;

	if ( dwCharID == sData.m_dwCharIDB )
	{
		m_pLineBoxBG->SetUseRender ( TRUE );
		m_pLineBoxBG->SetDiffuse( NS_UITEXTCOLOR::DISABLE );
	}
	else if ( dwCharID == sData.m_dwCharIDA )
	{
		m_pLineBoxBG->SetUseRender ( TRUE );
		m_pLineBoxBG->SetDiffuse( NS_UITEXTCOLOR::GREENYELLOW );
	}
	else
	{
		m_pLineBoxBG->SetUseRender ( FALSE );
	}
}

HRESULT CPKMarkSlot::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	if ( !IsVisible () )											return S_OK;
	HRESULT hr = S_OK;

	m_pImageClubMarkA->SetVisibleSingle ( FALSE );
	m_pImageClubMarkB->SetVisibleSingle ( FALSE );
	{
		hr = CUIGroup::Render ( pd3dDevice );
		if( FAILED ( hr ) ) return hr;
	}

	if( m_sMarkData.m_dwClubA != CLUB_NULL )
	{
		int nID = static_cast<int>( m_sMarkData.m_dwClubA );
		int nServer = static_cast<int>(GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID);
		DWORD dwVer = m_sMarkData.m_dwClubMarkVerA;

		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nServer, nID, dwVer );

		m_pImageClubMarkA->SetTexturePos ( 0, sMarkData.vTex_1_LU );
		m_pImageClubMarkA->SetTexturePos ( 1, sMarkData.vTex_2_RU );
		m_pImageClubMarkA->SetTexturePos ( 3, sMarkData.vTex_3_LD );
		m_pImageClubMarkA->SetTexturePos ( 2, sMarkData.vTex_4_RD );
		m_pImageClubMarkA->SetVisibleSingle ( TRUE );

		m_pImageClubMarkA->SetTexture ( sMarkData.pddsTexture );
		{
			hr = m_pImageClubMarkA->Render ( pd3dDevice );
			if( FAILED ( hr ) ) return hr;
		}
	}

	if( m_sMarkData.m_dwClubB != CLUB_NULL )
	{
		int nID = static_cast<int>( m_sMarkData.m_dwClubB );
		int nServer = static_cast<int>(GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID);
		DWORD dwVer = m_sMarkData.m_dwClubMarkVerB;

		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nServer, nID, dwVer );

		m_pImageClubMarkB->SetTexturePos ( 0, sMarkData.vTex_1_LU );
		m_pImageClubMarkB->SetTexturePos ( 1, sMarkData.vTex_2_RU );
		m_pImageClubMarkB->SetTexturePos ( 3, sMarkData.vTex_3_LD );
		m_pImageClubMarkB->SetTexturePos ( 2, sMarkData.vTex_4_RD );
		m_pImageClubMarkB->SetVisibleSingle ( TRUE );

		m_pImageClubMarkB->SetTexture ( sMarkData.pddsTexture );
		{
			hr = m_pImageClubMarkB->Render ( pd3dDevice );
			if( FAILED ( hr ) ) return hr;
		}
	}

	return S_OK;
}