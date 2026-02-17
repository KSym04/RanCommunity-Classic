#include "StdAfx.h"
#include "./ModernMiniPartyExpeditionPage.h"

#include "./ModernMiniPartySlot.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./BasicTextBox.h"

#include "./PartyBuffInfo.h"

#include "../../Lib_Client/G-Logic/GLPartyClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernMiniPartyExpeditionPage::CModernMiniPartyExpeditionPage () :
	m_pPartyBuffInfo(NULL)
{
	memset( m_pPartySlot, 0, sizeof(m_pPartySlot) );
}

CModernMiniPartyExpeditionPage::~CModernMiniPartyExpeditionPage ()
{
}

void CModernMiniPartyExpeditionPage::CreateSubControl ()
{
	int nTextAlign = TEXT_ALIGN_LEFT;
	D3DCOLOR dwFontColor = NS_UITEXTCOLOR::DEFAULT;

	CBasicTextBox* pTextBox = NULL;

	CString strMemberKeyword[MAXPARTY] = 
	{
		"MODERN_MINI_PARTY_EXPEDITION_INFO_PARTY_SLOT0",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_PARTY_SLOT1",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_PARTY_SLOT2",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_PARTY_SLOT3",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_PARTY_SLOT4",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_PARTY_SLOT5",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_PARTY_SLOT6",
		"MODERN_MINI_PARTY_EXPEDITION_INFO_PARTY_SLOT7",
	};

	for ( int i = 0; i < MAXPARTY; i++ )
	{
		CModernMiniPartySlot* pPartySlot = new CModernMiniPartySlot;
		pPartySlot->CreateSub ( this, strMemberKeyword[i].GetString (), UI_FLAG_DEFAULT, MINIPARTY_SLOT_MEMBER0 + i );
		pPartySlot->CreateSubControl ();
		RegisterControl ( pPartySlot );
		m_pPartySlot[i] = pPartySlot;
	}

	CPartyBuffInfo* pPartyBuffInfo = new CPartyBuffInfo;
	pPartyBuffInfo->CreateSub ( this, "MINIPARTY_BUFF_INFO", UI_FLAG_DEFAULT, MINIPARTY_BUFF_INFO );
	pPartyBuffInfo->CreateSubControl ();
	RegisterControl ( pPartyBuffInfo );
	m_pPartyBuffInfo = pPartyBuffInfo;
}

void CModernMiniPartyExpeditionPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if( !IsVisible() )	return ;

	const UIRECT& rcGlobalPos = GetGlobalPos ();
	m_pPartyBuffInfo->SetLocalPos ( D3DXVECTOR2 ( x + 25 - rcGlobalPos.left, y + 25 - rcGlobalPos.top ) );

	m_bMOUSEIN = false;

	m_pPartyBuffInfo->SetVisibleSingle ( FALSE );

	SetSelect( 0 , true );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	for ( int i = 0; i < MAXPARTY; i++ ) 
	{
		m_pPartySlot[i]->SetVisibleSingle ( FALSE );
	}

	GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
	if( pMaster )
	{
		GLPARTY_CLIENT *pSelf = FindSelfClient ();

		if ( !pSelf ) return ;

		SetSlotMember ( m_pPartySlot[0], pMaster, IsSameMap ( pSelf, pMaster ), 0 );

		DWORD nMEMBER_NUM = GLPartyClient::GetInstance().GetMemberNum();
		if( nMEMBER_NUM > 1 )
		{
			nMEMBER_NUM -= 1;
			for ( DWORD i = 0; i < nMEMBER_NUM; i++ )
			{
				GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember ( i );			
				if ( pMember )
				{				
					const BOOL bONESELF = pMember->ISONESELF ();

					SetSlotMember ( m_pPartySlot[i+1], pMember, IsSameMap ( pSelf, pMember ), i + 1 );
				}
			}
		}
	}
}

void CModernMiniPartyExpeditionPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case MINIPARTY_SLOT_MEMBER0:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_bMOUSEIN = true;
				m_pPartyBuffInfo->SetPartyID ( 0 );
				m_pPartyBuffInfo->SetVisibleSingle ( TRUE );		
				SetSelect( 0 );

				if ( CHECK_MOUSEIN_RBUPLIKE ( dwMsg ) )
				{
					GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
					if ( pMaster )
					{
						STARGETID sTARID(CROW_PC,pMaster->m_dwGaeaID);
						GLGaeaClient::GetInstance().GetCharacter ()->ReqSkillReaction(sTARID);
					}
				}
			}
		}
		break;

	case MINIPARTY_SLOT_MEMBER1:
	case MINIPARTY_SLOT_MEMBER2:
	case MINIPARTY_SLOT_MEMBER3:
	case MINIPARTY_SLOT_MEMBER4:
	case MINIPARTY_SLOT_MEMBER5:
	case MINIPARTY_SLOT_MEMBER6:
	case MINIPARTY_SLOT_MEMBER7:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_bMOUSEIN = true;
				int nIndex = ControlID - MINIPARTY_SLOT_MEMBER0;
				m_pPartyBuffInfo->SetPartyID ( nIndex );
				m_pPartyBuffInfo->SetVisibleSingle ( TRUE );
				SetSelect( nIndex );

				if ( CHECK_MOUSEIN_RBUPLIKE ( dwMsg ) )
				{
					int nIndex = ControlID - MINIPARTY_SLOT_MEMBER1;
					GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
					if ( pMaster )
					{
						GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember( nIndex );
						if ( pMember )
						{
							STARGETID sTARID(CROW_PC,pMember->m_dwGaeaID);
							GLGaeaClient::GetInstance().GetCharacter ()->ReqSkillReaction(sTARID);
						}
					}
				}
			}
		}
		break;
	}
}

GLPARTY_CLIENT*	CModernMiniPartyExpeditionPage::FindSelfClient ()
{
	GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
	if ( pMaster && pMaster->ISONESELF() )
	{
		return pMaster;
	}
	else
	{
		DWORD nMEMBER_NUM = GLPartyClient::GetInstance().GetMemberNum();
		if( nMEMBER_NUM > 1 )
		{
			nMEMBER_NUM -= 1;
			for ( DWORD i = 0; i < nMEMBER_NUM; i++ )
			{
				GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember( i );
				if ( pMember && pMember->ISONESELF () )
				{
					return pMember;				
				}
			}
		}
	}

	return NULL;
}

BOOL CModernMiniPartyExpeditionPage::IsSameMap ( GLPARTY_CLIENT *pSelf, GLPARTY_CLIENT *pMember )
{
	if ( !pSelf || !pMember )
	{
		GASSERT ( 0 && "잘못된 데이타입니다." );
		return FALSE;
	}

	return pSelf->m_sMapID.dwID == pMember->m_sMapID.dwID;
}

void CModernMiniPartyExpeditionPage::SetSlotMember ( CModernMiniPartySlot* pPartySlot, GLPARTY_CLIENT *pMember, BOOL bSameMap, const int nPartySlotID )
{
	if ( !pPartySlot || !pMember )
	{
		GASSERT ( 0 && "CMiniPartyWindow::SetSlotMember()" );
		return ;
	}

	const int nClassType = CharClassToIndex ( pMember->m_emClass );	
	pPartySlot->SetNumber( nPartySlotID );
	pPartySlot->SetPlayerName ( pMember->m_szName );	
	pPartySlot->SetVisibleSingle ( TRUE );
	pPartySlot->SetSchool( pMember->m_wSchool );

	bool bDIE = (bSameMap && pMember->m_sHP.wNow == 0);
	pPartySlot->SetDie( bDIE );

	if( bSameMap )
	{
		float fHPPercent = 0.0f;
		if( pMember->m_sHP.wMax != 0 )
		{
			fHPPercent = float(pMember->m_sHP.wNow) / float(pMember->m_sHP.wMax);
		}

		pPartySlot->SetHP( bSameMap? fHPPercent:100.0f, nClassType );
	}

	if ( bSameMap )	
	{
		CString strTemp;
		strTemp.Format( "Lv%d", pMember->m_wLevel );
		pPartySlot->SetPlayerLevel( strTemp.GetString() );
	}
	else
	{
		pPartySlot->SetPlayerLevel("");
	}
}

void CModernMiniPartyExpeditionPage::SetSelect( int nSelect, bool bNone /*= false*/ )
{
	if ( nSelect >= MAXPARTY )	return;

	for ( int i = 0; i < MAXPARTY; i++ ) 
	{
		m_pPartySlot[i]->SetSelect ( FALSE );
	}

	if ( bNone )	return;

	m_pPartySlot[nSelect]->SetSelect ( TRUE );
}