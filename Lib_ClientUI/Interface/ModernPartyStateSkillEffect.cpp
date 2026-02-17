#include "StdAfx.h"
#include "./ModernPartyStateSkillEffect.h"

#include "./PartyBuffInfoUnit.h"

#include "../../Lib_Client/G-Logic/GLPartyClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CModernPartyStateSkillEffect::CModernPartyStateSkillEffect () :
	m_NEWID ( PARTY_STATE_SKILLEFFECT_UNIT )
{
}

CModernPartyStateSkillEffect::~CModernPartyStateSkillEffect ()
{
}

void CModernPartyStateSkillEffect::CreateSubControl ()
{
	const CString strKeyword = "MODERN_SKILLEFFECT_SLOT";
	CString strCombine;

	for ( int i = 0; i < SKILLFACT_SIZE; i++ )
	{
		strCombine.Format ( "%s%d", strKeyword, i );
		CUIControl* pDummyControl = new CUIControl;
		pDummyControl->CreateSub ( this, strCombine.GetString(), UI_FLAG_DEFAULT, PARTY_STATE_SKILLEFFECT_DUMMY_BASE + i );
		RegisterControl ( pDummyControl );
		m_pSkillUnitDummy[i] = pDummyControl;
	}
}

void CModernPartyStateSkillEffect::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !m_SkillPlayList.empty () )
	{
		SKILLTIME_DISPLAY_LIST_ITER iter = m_SkillPlayList.begin ();
		SKILLTIME_DISPLAY_LIST_ITER iter_end = m_SkillPlayList.end ();

		for ( ; iter != iter_end; )
		{
			const SKILLTIME_PAIR& skill_pair = (*iter);
			const DWORD& dwID = skill_pair.first;
			const UIGUID& cID = skill_pair.second;
			
			SSKILLFACT sSKILLFACT;
			if ( !GET_PLAY_SKILLFACT ( dwID, sSKILLFACT ) )
			{				
				DeleteControl ( cID, 0 );
				iter = m_SkillPlayList.erase ( iter );
			}
			else ++iter;
		}
	}

	{
		GLPARTY_CLIENT *pMember = NULL;
		if ( m_nPartyID == 0 )
			pMember = GLPartyClient::GetInstance().GetMaster ();
		else
			pMember = GLPartyClient::GetInstance().GetMember ( m_nPartyID-1 );

		if ( pMember )	
		{
			bool bSELF = false;
			if ( pMember == FindSelfClient () ) bSELF = true;

			PGLCHARCLIENT pCHAR = NULL;
			if ( !bSELF )
			{
				pCHAR = GLGaeaClient::GetInstance().GetChar ( pMember->m_dwGaeaID );
				if ( pCHAR )
				{
					for ( int i = 0; i < SKILLFACT_SIZE; i++ )
					{
						const SSKILLFACT* pSKILLFACT = pCHAR->GET_SKILLFACT ( i );
						if ( !pSKILLFACT ) continue;
						const DWORD& dwID = pSKILLFACT->sNATIVEID.dwID;		

						if ( dwID == NATIVEID_NULL().dwID ) continue;

						UIGUID cID = GET_EXIST_PLAY_SKILL_CONTROL ( dwID );
						if ( cID == NO_ID )	NEW_PLAY_SKILL ( dwID, pSKILLFACT->wLEVEL );
					}
				}
			}
			else
			{
				for ( int i = 0; i < SKILLFACT_SIZE; i++ )
				{
					const SSKILLFACT* pSKILLFACT = &(GLGaeaClient::GetInstance().GetCharacter()->m_sSKILLFACT[i]);
					if ( !pSKILLFACT ) continue;
					const DWORD& dwID = pSKILLFACT->sNATIVEID.dwID;		

					if ( dwID == NATIVEID_NULL().dwID ) continue;

					UIGUID cID = GET_EXIST_PLAY_SKILL_CONTROL ( dwID );
					if ( cID == NO_ID )	NEW_PLAY_SKILL ( dwID, pSKILLFACT->wLEVEL );
				}
			}
		}
	}

	if ( m_SkillPlayList.empty () )
	{
		SetVisibleSingle ( FALSE );
		return ;
	}

	int nPLAY_SKILL = 0;
	if ( !m_SkillPlayList.empty () )
	{		
		SKILLTIME_DISPLAY_LIST_ITER iter = m_SkillPlayList.begin ();
		SKILLTIME_DISPLAY_LIST_ITER iter_end = m_SkillPlayList.end ();

		for ( ; iter != iter_end; ++iter )
		{
			const SKILLTIME_PAIR& skill_pair = (*iter);
			const DWORD& dwID = skill_pair.first;
			const UIGUID& cID = skill_pair.second;			

			CPartyBuffInfoUnit* pControl = (CPartyBuffInfoUnit*) FindControl ( cID );
			if ( !pControl )
			{
				GASSERT ( 0 && "컨트롤 찾기 실패" );
				continue;
			}
			
			SSKILLFACT sSKILLFACT;
			if ( !GET_PLAY_SKILLFACT ( dwID, sSKILLFACT ) )
			{
				GASSERT ( 0 && "스킬 데이타 찾기 실패" );
				continue;
			}

			const float& fAGE = sSKILLFACT.fAGE;

			pControl->SetGlobalPos ( m_pSkillUnitDummy[nPLAY_SKILL]->GetGlobalPos () );
			pControl->SetLocalPos ( m_pSkillUnitDummy[nPLAY_SKILL]->GetLocalPos () );
			pControl->SetLeftTime ( fAGE );

			nPLAY_SKILL++;
		}
	}

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CModernPartyStateSkillEffect::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

HRESULT CModernPartyStateSkillEffect::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	m_pd3dDevice = pd3dDevice;
	return CUIGroup::InitDeviceObjects ( pd3dDevice );
}

UIGUID CModernPartyStateSkillEffect::GET_EXIST_PLAY_SKILL_CONTROL ( const DWORD& dwSkillID )
{
	SKILLTIME_DISPLAY_LIST_ITER iter = m_SkillPlayList.begin ();
	SKILLTIME_DISPLAY_LIST_ITER iter_end = m_SkillPlayList.end ();
	for ( ; iter != iter_end; ++iter )
	{
		const SKILLTIME_PAIR& pair = (*iter);
		const DWORD& dwID = pair.first;
		const UIGUID& cID = pair.second;
		if ( dwID == dwSkillID ) return cID;
	}

	return NO_ID;
}

bool CModernPartyStateSkillEffect::GET_PLAY_SKILLFACT ( const DWORD& dwSkillID, SSKILLFACT& sSKILLFACT )
{
	GLPARTY_CLIENT *pMember = NULL;
	if ( m_nPartyID == 0 )
		pMember = GLPartyClient::GetInstance().GetMaster ();
	else
		pMember = GLPartyClient::GetInstance().GetMember ( m_nPartyID-1 );
	if ( !pMember ) return false;

	bool bSELF = false;
	if ( pMember == FindSelfClient () ) bSELF = true;

	PGLCHARCLIENT pCHAR = NULL;
	if ( !bSELF )
	{
		pCHAR = GLGaeaClient::GetInstance().GetChar ( pMember->m_dwGaeaID );
		if ( !pCHAR ) return false;
	}

	for ( int i = 0; i < SKILLFACT_SIZE; i++ )
	{
		SSKILLFACT* pSkillFactIter = NULL;
		if ( bSELF )
			pSkillFactIter = &(GLGaeaClient::GetInstance().GetCharacter()->m_sSKILLFACT[i]);
		else
			pSkillFactIter = pCHAR->GET_SKILLFACT ( i );

		if ( !pSkillFactIter ) continue;

		if ( pSkillFactIter->sNATIVEID == dwSkillID )
		{
			sSKILLFACT = *pSkillFactIter;
			return true;
		}
	}

	return false;
}

bool CModernPartyStateSkillEffect::NEW_PLAY_SKILL ( const DWORD& dwSkillID, const WORD wLEVEL )
{
	m_NEWID++;
	if ( PARTY_STATE_SKILLEFFECT_UNIT_END <= m_NEWID ) m_NEWID = PARTY_STATE_SKILLEFFECT_UNIT;

	float fLifeTime = 0.0f;
	if ( !GET_SKILL_LIFE_TIME ( dwSkillID, wLEVEL, fLifeTime ) ) return false;

	SNATIVEID sNativeID ( dwSkillID );
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sNativeID );
	if ( !pSkill ) return false;

	CPartyBuffInfoUnit* pSkillTimeUnit = new CPartyBuffInfoUnit;
	pSkillTimeUnit->CreateSub ( this, "PARTYBUFF_UNIT", UI_FLAG_DEFAULT, m_NEWID );
	pSkillTimeUnit->CreateSubControl ();
	pSkillTimeUnit->InitDeviceObjects ( m_pd3dDevice );
	pSkillTimeUnit->RestoreDeviceObjects ( m_pd3dDevice );	
	pSkillTimeUnit->SetSkill ( dwSkillID, fLifeTime, pSkill->GetName() );
	RegisterControl ( pSkillTimeUnit );	

	SKILLTIME_PAIR pair (dwSkillID,m_NEWID);
	m_SkillPlayList.push_back ( pair );

	return true;
}

bool CModernPartyStateSkillEffect::GET_SKILL_LIFE_TIME ( const DWORD& dwSkillID, const WORD wLEVEL, float& fLifeTime )
{
	SNATIVEID sNativeID ( dwSkillID );

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sNativeID );
	if ( !pSkill ) return false;

	fLifeTime = pSkill->m_sAPPLY.sDATA_LVL[wLEVEL].fLIFE;
	fLifeTime = (fLifeTime)?fLifeTime:1.0f;

	return true;
}

GLPARTY_CLIENT*	CModernPartyStateSkillEffect::FindSelfClient ()
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

void CModernPartyStateSkillEffect::SetPartyID ( const int nPartyID )
{
	m_nPartyID = nPartyID;
}
