#include "StdAfx.h"

#include "./ModernSkillPage.h"
#include "./ModernSkillSlot.h"

#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CModernSkillPage::nSTARTLINE = 0;
const int CModernSkillPage::nOUTOFRANGE = -1;

CModernSkillPage::CModernSkillPage () :
	m_pScrollBar ( NULL ),
	m_nCurPos ( -1 )
{
	memset ( m_pSkillSlotArray, 0, sizeof ( CModernSkillSlot* ) * GLSkillMan::MAX_CLASSSKILL );	
	memset ( m_pSkillSlotArrayDummy, 0, sizeof ( CUIControl* ) * nMAX_ONE_VIEW_SLOT );		
}

CModernSkillPage::~CModernSkillPage ()
{
}

CUIControl*	CModernSkillPage::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

void CModernSkillPage::CreateSubControl ( SNATIVEID sNativeIDArray[GLSkillMan::MAX_CLASSSKILL], int nSkillCount )
{	
	if ( nSkillCount < 1 ) return ;

	CString strSlotKeyword[nMAX_ONE_VIEW_SLOT] =
	{
		"RAN_CHARACTER_SKILL_SLOT_0",
		"RAN_CHARACTER_SKILL_SLOT_1",
		"RAN_CHARACTER_SKILL_SLOT_2",
		"RAN_CHARACTER_SKILL_SLOT_3",
		"RAN_CHARACTER_SKILL_SLOT_4",
		"RAN_CHARACTER_SKILL_SLOT_5",
		"RAN_CHARACTER_SKILL_SLOT_6",
		"RAN_CHARACTER_SKILL_SLOT_7",
		"RAN_CHARACTER_SKILL_SLOT_8",
		"RAN_CHARACTER_SKILL_SLOT_9",
		"RAN_CHARACTER_SKILL_SLOT_10",
		"RAN_CHARACTER_SKILL_SLOT_11",
		"RAN_CHARACTER_SKILL_SLOT_12",
		"RAN_CHARACTER_SKILL_SLOT_13",
		"RAN_CHARACTER_SKILL_SLOT_14",
		"RAN_CHARACTER_SKILL_SLOT_15",
		"RAN_CHARACTER_SKILL_SLOT_16",
		"RAN_CHARACTER_SKILL_SLOT_17",
	};

	{
		for ( int i = 0; i < nMAX_ONE_VIEW_SLOT; i++ )
		{
			m_pSkillSlotArrayDummy[i] = CreateControl ( strSlotKeyword[i].GetString () );
		}
	}

	{	
		if ( nSkillCount < nMAX_ONE_VIEW_SLOT )
		{
			nSkillCount = nMAX_ONE_VIEW_SLOT;
		}

		m_vecViewSkill.clear();		

		for ( int i = 0; i < GLSkillMan::MAX_CLASSSKILL; i++ )
		{
			m_pSkillSlotArray[i] = CreateSkillSlot ( strSlotKeyword[0], SKILL_SLOT0 + i, sNativeIDArray[i] );
		}
		m_nMaxSkillCnt = nSkillCount;
	}

	{	
		CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
		pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE );
		pScrollBar->CreateBaseScrollBar ( "RAN_CHARACTER_SKILL_LIST_SCROOL" );
		pScrollBar->GetThumbFrame()->SetState ( nSkillCount, nMAX_ONE_VIEW_SLOT );
		RegisterControl ( pScrollBar );
		m_pScrollBar = pScrollBar;
	}

	SetSkillSlotRender ( nSTARTLINE, nMAX_ONE_VIEW_SLOT );
}

CModernSkillSlot*	CModernSkillPage::CreateSkillSlot ( CString strKeyword, UIGUID ControlID, SNATIVEID sNativeID )
{
	CModernSkillSlot* pSkillSlot = new CModernSkillSlot;
	pSkillSlot->CreateSub ( this, strKeyword.GetString (), UI_FLAG_DEFAULT, ControlID );
	pSkillSlot->CreateSubControl ( sNativeID );
	RegisterControl ( pSkillSlot );
	return pSkillSlot;
}

void CModernSkillPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	m_nSlotIndex = nOUTOFRANGE;
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pScrollBar )
	{
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nViewPerPage = pThumbFrame->GetViewPerPage ();
			int nBeforeSkillCnt = m_vecViewSkill.size();

			m_vecViewSkill.clear();
			for ( int i = 0; i < m_nMaxSkillCnt; i++ )
			{
				SNATIVEID sSkillID = m_pSkillSlotArray[i]->GetNativeSkillID();
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID );
				if( pSkill && pSkill->m_sBASIC.bLearnView && !GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL( sSkillID ) )
				{
				}else{
					m_vecViewSkill.push_back( i );
				}
			}
			if( nBeforeSkillCnt != m_vecViewSkill.size() )
			{
				m_pScrollBar->GetThumbFrame()->SetState ( m_vecViewSkill.size(), nMAX_ONE_VIEW_SLOT );
				ResetAllSkillSlotRender( m_nMaxSkillCnt );
				SetSkillSlotRender ( 0, 0 + nViewPerPage );			
			}
			
		}
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal ();
			if ( nTotal <= nMAX_ONE_VIEW_SLOT ) return ;

			const int nViewPerPage = pThumbFrame->GetViewPerPage ();

			if ( nViewPerPage < nTotal )
			{
				int nCurPos = nSTARTLINE;
				const int nMovableLine = nTotal - nViewPerPage;
				float fPercent = pThumbFrame->GetPercent ();

				nCurPos = (int)floor(fPercent * nMovableLine);
				if ( nCurPos < nSTARTLINE ) nCurPos = nSTARTLINE;

				if ( m_nCurPos == nCurPos ) return;
				if ( nCurPos % 2 == 0 ){}else{nCurPos +=1;}
				m_nCurPos = nCurPos;

				ResetAllSkillSlotRender ( nTotal );
				SetSkillSlotRender ( nCurPos, nCurPos + nViewPerPage );
			}
		}
	}
}

void CModernSkillPage::ResetAllSkillSlotRender ( int nTotal )
{
	if ( nTotal < 0 ) return ;

	for ( int i = 0; i < GLSkillMan::MAX_CLASSSKILL; i++ )
	{
		CModernSkillSlot* pSkillSlot = m_pSkillSlotArray[i];
		if ( pSkillSlot )
		{
			pSkillSlot->SetVisibleSingle ( FALSE );
		}
	}
}

void CModernSkillPage::SetSkillSlotRender ( int nStartIndex, int nTotal )
{
	if ( nTotal < 0 ) return ;

	CDebugSet::ToView( 16, "%d %d", nStartIndex, nTotal );
	const UIRECT& rcParentPos = GetGlobalPos ();

	

	int iViewIndex = 0; // 보여질 위치
	int iSlotNum   = 0;	// 보여지는 SlotNum
	for ( int i = nStartIndex; i < (int)m_vecViewSkill.size(); i++ )
	{
		if( i >= m_nMaxSkillCnt ) break;
		if( i >= nTotal ) break;

		iSlotNum = m_vecViewSkill[i];

		CModernSkillSlot* pSkillSlot = m_pSkillSlotArray[iSlotNum];
		if ( pSkillSlot )
		{
						
			int nAbsoluteIndex = iViewIndex;
			CUIControl* pDummyControl = m_pSkillSlotArrayDummy[nAbsoluteIndex];

			const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos ();
			const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos ();

			pSkillSlot->SetLocalPos ( rcSlotLocalPos );			
			pSkillSlot->SetGlobalPos ( rcSlotPos );

			pSkillSlot->SetVisibleSingle ( TRUE );

			iViewIndex++;
		}
	}

	if( (int)m_vecViewSkill.size() < nTotal )
	{
		int iMaxLackTotal = nTotal - (int)m_vecViewSkill.size();		
		for( int i = 0; i < iMaxLackTotal; i++ )
		{
			iSlotNum++;
			CModernSkillSlot* pSkillSlot = m_pSkillSlotArray[iSlotNum];
			if ( pSkillSlot )
			{

				int nAbsoluteIndex = iViewIndex;
				CUIControl* pDummyControl = m_pSkillSlotArrayDummy[nAbsoluteIndex];

				const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos ();
				const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos ();

				pSkillSlot->SetLocalPos ( rcSlotLocalPos );			
				pSkillSlot->SetGlobalPos ( rcSlotPos );

				pSkillSlot->SetVisibleSingle ( TRUE );

				iViewIndex++;
			}
		}
	}


	
	for( size_t i = nTotal; i < m_vecViewSkill.size(); i++ )
	{
		iSlotNum = m_vecViewSkill[i];

		if( m_pSkillSlotArray[iSlotNum] ) m_pSkillSlotArray[iSlotNum]->SetVisibleSingle( FALSE );
	}
}

SNATIVEID  CModernSkillPage::GetSkillID ( int nIndex )
{	
	return m_pSkillSlotArray[nIndex]->GetNativeSkillID ();
}

void CModernSkillPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	if ( SKILL_SLOT0 <= ControlID && ControlID <= SKILL_SLOT_END )
	{
		int nSelected = ControlID - SKILL_SLOT0;
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			m_nSlotIndex = nSelected;
			m_pSkillSlotArray[ nSelected ]->ShowHover (TRUE);
		}else{
			m_pSkillSlotArray[ nSelected ]->ShowHover (FALSE);
		}
	}
}
