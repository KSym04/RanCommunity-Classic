#include "StdAfx.h"
#include "./ModernPartyState.h"

#include "./ModernPartyStateSlot.h"
#include "./ModernPartyStateSkillEffect.h"

#include "./BasicTextBox.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/GLPartyClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRProfile.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernPartyState::CModernPartyState ()
	: m_bFirstGap ( FALSE )

{
	for ( int i = 0; i < MAXPARTY; i++ ) 
	{
		m_pSlot[i] = NULL;
		m_pSkill[i] = NULL;
	}
}

CModernPartyState::~CModernPartyState ()
{
}

void CModernPartyState::CreateSubControl ()
{
	CString strSlot[MAXPARTY] = 
	{
		"MODERN_MEMBER_STATE_SLOT0",
		"MODERN_MEMBER_STATE_SLOT1",
		"MODERN_MEMBER_STATE_SLOT2",
		"MODERN_MEMBER_STATE_SLOT3",
		"MODERN_MEMBER_STATE_SLOT4",
		"MODERN_MEMBER_STATE_SLOT5",
		"MODERN_MEMBER_STATE_SLOT6",
		"MODERN_MEMBER_STATE_SLOT7",
	};

	CString strBuff[MAXPARTY] =
	{
		"MODERN_MEMBER_STATE_SKILLEFFECT_SLOT0",
		"MODERN_MEMBER_STATE_SKILLEFFECT_SLOT1",
		"MODERN_MEMBER_STATE_SKILLEFFECT_SLOT2",
		"MODERN_MEMBER_STATE_SKILLEFFECT_SLOT3",
		"MODERN_MEMBER_STATE_SKILLEFFECT_SLOT4",
		"MODERN_MEMBER_STATE_SKILLEFFECT_SLOT5",
		"MODERN_MEMBER_STATE_SKILLEFFECT_SLOT6",
		"MODERN_MEMBER_STATE_SKILLEFFECT_SLOT7",
	};

	for ( int i = 0; i < MAXPARTY; i++ ) 
	{
		CModernPartyStateSlot* pPartySlot = new CModernPartyStateSlot;
		pPartySlot->CreateSub ( this, strSlot[i].GetString (), UI_FLAG_DEFAULT, PARTY_STATE_MEMBER0 + i );
		pPartySlot->CreateSubControl ();
		pPartySlot->SetVisibleSingle( FALSE );
		RegisterControl ( pPartySlot );
		m_pSlot[i] = pPartySlot;

		CModernPartyStateSkillEffect* pPartySkillEffect = new CModernPartyStateSkillEffect;
		pPartySkillEffect->CreateSub ( this, strBuff[i].GetString () );
		pPartySkillEffect->CreateSubControl ();
		pPartySkillEffect->SetVisibleSingle( FALSE );
		RegisterControl ( pPartySkillEffect );
		m_pSkill[i] = pPartySkillEffect;
	}
}

void CModernPartyState::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_bCHECK_MOUSE_STATE = false;
	m_PosX = x;
	m_PosY = y;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( IsExclusiveSelfControl() )
	{
		SetGlobalPos ( D3DXVECTOR2 ( x - m_vGap.x, y - m_vGap.y ) );
	}

	for ( int i = 0; i < MAXPARTY; i++ ) 
	{
		m_pSlot[i]->SetVisibleSingle ( FALSE );
		m_pSkill[i]->SetVisibleSingle ( FALSE );
	}

	GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();	
	if ( pMaster )
	{
		GLPARTY_CLIENT *pSelf = FindSelfClient ();

		if ( !pSelf ) return ;

		SetSlotMember ( m_pSlot[0], m_pSkill[0], pMaster, IsSameMap ( pSelf, pMaster ) );
		m_pSlot[0]->SetMaster ( TRUE );
		m_pSkill[0]->SetPartyID( 0 );

		DWORD nMEMBER_NUM = GLPartyClient::GetInstance().GetMemberNum();
		if( nMEMBER_NUM > 1 )
		{
			nMEMBER_NUM -= 1; 
			for ( DWORD i = 0; i < nMEMBER_NUM; i++ )
			{
				GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember ( i );			
				if ( pMember )
				{				
					m_pSlot[i+1]->SetMaster ( FALSE );
					m_pSkill[i+1]->SetPartyID( i+1 );
					SetSlotMember ( m_pSlot[i+1], m_pSkill[i+1],pMember, IsSameMap ( pSelf, pMember ) );
				}
			}
		}
	}	
}

void CModernPartyState::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case PARTY_STATE_MEMBER0:
	case PARTY_STATE_MEMBER1:
	case PARTY_STATE_MEMBER2:
	case PARTY_STATE_MEMBER3:
	case PARTY_STATE_MEMBER4:
	case PARTY_STATE_MEMBER5:
	case PARTY_STATE_MEMBER6:
	case PARTY_STATE_MEMBER7:
		{
			if ( dwMsg & UIMSG_MOUSEIN_ICON )
			{
				AddMessageEx ( UIMSG_MOUSEIN_TOMOVE );

				if ( !m_bCHECK_MOUSE_STATE )
				{
					CheckMouseState ();
					m_bCHECK_MOUSE_STATE = true;
				}
			}

			if ( CHECK_MOUSEIN_RBUPLIKE ( dwMsg ) )
			{
				int nIndex = ControlID - PARTY_STATE_MEMBER0;
				if ( nIndex == 0 )
				{
					GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
					if ( pMaster )
					{
						STARGETID sTARID(CROW_PC,pMaster->m_dwGaeaID);
						GLGaeaClient::GetInstance().GetCharacter ()->ReqSkillReaction(sTARID);
					}
				}
				else if ( nIndex > 0 )
				{
					GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
					if ( pMaster )
					{
						GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember( nIndex-1 );
						if ( pMember )
						{
							STARGETID sTARID(CROW_PC,pMember->m_dwGaeaID);
							GLGaeaClient::GetInstance().GetCharacter ()->ReqSkillReaction(sTARID);
						}
					}
				}
			}
		}break;
	};
}

void CModernPartyState::SetSlotMember ( CModernPartyStateSlot* pSlot, CModernPartyStateSkillEffect* pEff, GLPARTY_CLIENT *pMember, BOOL bSameMap )
{
	if ( !pSlot || !pEff)
	{
		return ;
	}

	if ( !pMember )
	{
		return ;
	}

	const int nClassType = CharClassToIndex ( pMember->m_emClass );	
	const CString strName = pMember->m_szName;

	pSlot->SetSchool( pMember->m_wSchool );
	pSlot->SetClass ( nClassType, bSameMap );
	pSlot->SetPlayerName ( strName );	

	bool bDIE = (bSameMap && pMember->m_sHP.wNow == 0);
	pSlot->SetDie( bDIE );

	if ( bSameMap )
	{
		float fHPPercent = 0.0f;
		if ( pMember->m_sHP.wMax )
			fHPPercent = float(pMember->m_sHP.wNow) / float(pMember->m_sHP.wMax);

		pSlot->SetHP ( fHPPercent, nClassType );

		CString strHP;
		strHP.Format( "%d/%d", pMember->m_sHP.wNow,pMember->m_sHP.wMax );
		pSlot->SetHP ( strHP );
	}
	else
	{
		pSlot->SetHP ( 100.0f, nClassType );
		pSlot->SetHP ( "" );
	}

	pSlot->SetVisibleSingle ( TRUE );

	if ( RPROFILE::bPartyWindowShowBuff )
		pEff->SetVisibleSingle ( TRUE );
}

BOOL CModernPartyState::IsSameMap ( GLPARTY_CLIENT *pSelf, GLPARTY_CLIENT *pMember )
{
	if ( !pSelf || !pMember )
	{
		return FALSE;
	}

	return pSelf->m_sMapID.dwID == pMember->m_sMapID.dwID;
}

GLPARTY_CLIENT*	CModernPartyState::FindSelfClient ()
{
	GLPARTY_CLIENT *pMaster = GLPartyClient::GetInstance().GetMaster();
	if ( pMaster && pMaster->ISONESELF() )
	{
		return pMaster;
	}
	else
	{
		for ( int i = 0; i < 7; i++ )
		{
			GLPARTY_CLIENT *pMember = GLPartyClient::GetInstance().GetMember( i );
			if ( pMember && pMember->ISONESELF () )
			{
				return pMember;				
			}
		}
	}

	return NULL;
}

void CModernPartyState::CheckMouseState ()
{
	const DWORD dwMsg = GetMessageEx ();
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		if( UIMSG_LB_DUP & dwMsg )
		{
			return ;
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
			}
		}
		else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
		{
			ResetExclusiveControl();
			m_bFirstGap = FALSE;				
		}
	}
	else if ( CHECK_LB_UP_LIKE ( dwMsg ) )		
	{								
		ResetExclusiveControl();
		m_bFirstGap = FALSE;					
	}
}