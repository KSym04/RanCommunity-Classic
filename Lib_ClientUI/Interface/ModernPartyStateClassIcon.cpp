#include "StdAfx.h"
#include "./ModernPartyStateClassIcon.h"

#include "./BasicTextBox.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernPartyStateClassIcon::CModernPartyStateClassIcon () :
	m_pDIE( NULL ),
	m_pSlotMaster( NULL ),
	m_pSlotSubMaster( NULL )
{
	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		m_pClassLive[i] = NULL;
		m_pClassDead[i] = NULL;
	}
}

CModernPartyStateClassIcon::~CModernPartyStateClassIcon ()
{
}

void CModernPartyStateClassIcon::CreateSubControl ()
{
	CString strClassLive[GLCI_NUM_8CLASS] = 
	{
		"MODERN_MEMBER_STATE_BRAWLER_MALE_LIVE",
		"MODERN_MEMBER_STATE_SWORDSMAN_MALE_LIVE",
		"MODERN_MEMBER_STATE_ARCHER_FEMALE_LIVE",
		"MODERN_MEMBER_STATE_SHAMAN_FEMALE_LIVE",
		"MODERN_MEMBER_STATE_EXTREME_MALE_LIVE",
		"MODERN_MEMBER_STATE_EXTREME_FEMALE_LIVE",
		"MODERN_MEMBER_STATE_BRAWLER_FEMALE_LIVE",
		"MODERN_MEMBER_STATE_SWORDSMAN_FEMALE_LIVE",
		"MODERN_MEMBER_STATE_ARCHER_MALE_LIVE",
		"MODERN_MEMBER_STATE_SHAMAN_MALE_LIVE",
		"MODERN_MEMBER_STATE_SCIENTIST_MALE_LIVE",
		"MODERN_MEMBER_STATE_SCIENTIST_FEMALE_LIVE",
		"MODERN_MEMBER_STATE_ASSASSIN_MALE_LIVE",
		"MODERN_MEMBER_STATE_ASSASSIN_FEMALE_LIVE",
		"MODERN_MEMBER_STATE_TRICKER_MALE_LIVE",
		"MODERN_MEMBER_STATE_TRICKER_FEMALE_LIVE",
	};

	CString strClassDead[GLCI_NUM_8CLASS] = 
	{
		"MODERN_MEMBER_STATE_BRAWLER_MALE_DIE",
		"MODERN_MEMBER_STATE_SWORDSMAN_MALE_DIE",
		"MODERN_MEMBER_STATE_ARCHER_FEMALE_DIE",
		"MODERN_MEMBER_STATE_SHAMAN_FEMALE_DIE",
		"MODERN_MEMBER_STATE_EXTREME_MALE_DIE",
		"MODERN_MEMBER_STATE_EXTREME_FEMALE_DIE",
		"MODERN_MEMBER_STATE_BRAWLER_FEMALE_DIE",
		"MODERN_MEMBER_STATE_SWORDSMAN_FEMALE_DIE",
		"MODERN_MEMBER_STATE_ARCHER_MALE_DIE",
		"MODERN_MEMBER_STATE_SHAMAN_MALE_DIE",
		"MODERN_MEMBER_STATE_SCIENTIST_MALE_DIE",
		"MODERN_MEMBER_STATE_SCIENTIST_FEMALE_DIE",
		"MODERN_MEMBER_STATE_ASSASSIN_MALE_DIE",
		"MODERN_MEMBER_STATE_ASSASSIN_FEMALE_DIE",
		"MODERN_MEMBER_STATE_TRICKER_MALE_DIE",
		"MODERN_MEMBER_STATE_TRICKER_FEMALE_DIE",
	};

	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		m_pClassLive[i] = CreateControl ( strClassLive[i].GetString() );
		m_pClassDead[i] = CreateControl ( strClassDead[i].GetString() );
	}

	m_pDIE = CreateControl ( "MODERN_MEMBER_STATE_DIE" );
	m_pSlotMaster = CreateControl ( "MODERN_MEMBER_STATE_SLOT_MASTER" );
	m_pSlotSubMaster = CreateControl ( "MODERN_MEMBER_STATE_SLOT_SUB_MASTER" );
	m_pDIE->SetVisibleSingle( FALSE );
	m_pSlotMaster->SetVisibleSingle( FALSE );
	m_pSlotSubMaster->SetVisibleSingle( FALSE );
}

CUIControl*	CModernPartyStateClassIcon::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

void CModernPartyStateClassIcon::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernPartyStateClassIcon::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

void CModernPartyStateClassIcon::SetClass ( int nClassType, BOOL bSameMap )
{
	if ( nClassType < GLCI_BRAWLER_M || nClassType > GLCI_TRICKER_W )
	{
		return ;
	}

	for ( int i = 0; i < GLCI_NUM_8CLASS; i++ )
	{
		m_pClassLive[i]->SetVisibleSingle ( FALSE );
		m_pClassDead[i]->SetVisibleSingle ( FALSE );
	}

	m_pClassLive[nClassType]->SetVisibleSingle ( bSameMap );
	m_pClassDead[nClassType]->SetVisibleSingle ( !bSameMap );
}

void CModernPartyStateClassIcon::SetDie( bool bDIE )
{
	m_pDIE->SetVisibleSingle( bDIE );
}

void CModernPartyStateClassIcon::SetMaster( bool bMaster )
{
	m_pSlotMaster->SetVisibleSingle( bMaster );
}