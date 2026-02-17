#include "stdafx.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/SkillTrayTab.h"

#include "./GLCharacter.h"
#include "./GLGaeaClient.h"
#include "./GLLandManClient.h"
#include "./DxGlobalStage.h"
#include "./GLAutoPilotC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLAutoPilotC::GLAutoPilotC(GLCharacter* pChar)
	: GLAutoPilot()
	, m_pChar(pChar)
{

}

GLAutoPilotC::~GLAutoPilotC()
{

}

void GLAutoPilotC::Reset()
{
	m_sTargetID.RESET();
	m_bValidTarget = FALSE;

	m_sSkillRun.ResetNativeID();

	GLAutoPilot::Reset();
}

void GLAutoPilotC::FrameMove( float fElapsedTime )
{
	GLAutoPilot::FrameMove(fElapsedTime);

	if ( m_pChar == NULL )
		return;

	if ( m_bActive == FALSE )
		return;

	//map check
	//auto pilot should reset when player moves to another map as it invalidates range and central point for scan
	if ( m_sMap != GLGaeaClient::GetInstance().GetActiveMap()->GetMapID() )
	{
		Reset();

		//send packet to server
		GLMSG::SNETPC_REQ_AUTO_PILOT_RESET netMsg;
		netMsg.bActive = m_bActive;
		NETSENDTOFIELD ( &netMsg );
		return;
	}

	//death check
	//auto pilot should reset when player died
	if ( m_pChar->IsDie() )
	{
		Reset();

		//send packet to server
		GLMSG::SNETPC_REQ_AUTO_PILOT_RESET netMsg;
		netMsg.bActive = m_bActive;
		NETSENDTOFIELD ( &netMsg );
		return;
	}

	if ( m_bValidTarget == TRUE )
	{
		//target validity
		GLCOPY* pCopy = GLGaeaClient::GetInstance().GetCopyActor( m_sTargetID );
		if ( pCopy == NULL )
		{
			m_sTargetID.RESET();
			m_bValidTarget = FALSE;
			return;
		}

		//target crow validity
		if ( pCopy->GetCrow() != CROW_MOB && pCopy->GetCrow() != CROW_SUMMON )
		{
			m_sTargetID.RESET();
			m_bValidTarget = FALSE;
			return;
		}

		//check if target is dead
		if ( pCopy->IsACTION(GLAT_DIE) || pCopy->IsACTION(GLAT_FALLING) )
		{
			m_sTargetID.RESET();
			m_bValidTarget = FALSE;
			return;
		}
	}
	
	//skill check
	EMSKILLCHECK emSkillCheck = m_pChar->CHECHSKILL(m_sSkillRun, 1, m_pChar->IsDefenseSkill(), m_pChar->GetCurRHand(), m_pChar->GetCurLHand() );
	if (emSkillCheck != EMSKILL_OK)
	{
		//reset active skill and try to find one
		m_sSkillRun.ResetNativeID();

		//now this is a bummer, the only place where the active tab is saved is in the ui
		int nActiveTabIndex = CInnerInterface::GetInstance().skill_tray_get_tab_index();
		if ( nActiveTabIndex < 0 && nActiveTabIndex >= 4 )
			return;

		int nRangeStart = nActiveTabIndex*QUICK_SKILL_SLOT_MAX;
		int nRangeEnd = nRangeStart + QUICK_SKILL_SLOT_MAX;

		for ( int i=nRangeStart; i<nRangeEnd; ++ i )
		{
			SNATIVEID sSkillID = m_pChar->GetCharData().m_sSKILLQUICK[i];
			EMSKILLCHECK emSkillCheck = m_pChar->CHECHSKILL(sSkillID, 1, m_pChar->IsDefenseSkill(), m_pChar->GetCurRHand(), m_pChar->GetCurLHand() );
			if (emSkillCheck == EMSKILL_OK)
			{
				//skill is clear for use so we will use this
				m_sSkillRun = sSkillID;
				break;
			}
		}
	}

}

void GLAutoPilotC::Set( BOOL bEnable, float fRange )
{
	if ( m_pChar == NULL )
		return;

	if ( RPARAM::bUseAutoPilot == FALSE )
		return;

	if ( GLGaeaClient::GetInstance().GetInstance().GetActiveMap()->isUseAutoPilot() == FALSE )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_NOT_MAP") );
		return;
	}

	if ( m_fRange <= 0.0f )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_INVALID_VALUE") );
		return;
	}

	if ( m_fRange > RPARAM::fAutoPilotRangeMax )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_INVALID_VALUE") );
		return;
	}

	//send packet to server
	GLMSG::SNETPC_REQ_AUTO_PILOT_SET netMsg;
	netMsg.bActive = bEnable;
	netMsg.fRange = fRange;
	NETSENDTOFIELD ( &netMsg );
}

void GLAutoPilotC::MsgProcess( NET_MSG_GENERIC* nmg )
{
	if ( nmg == NULL )
		return;

	switch ( nmg->nType )
	{
	case NET_MSG_GCTRL_REQ_AUTO_PILOT_SET_FB:
		MsgAutoPilotSetFb(nmg);
		break;

	case NET_MSG_GCTRL_REQ_AUTO_PILOT_RESET_FB:
		MsgAutoPilotResetFb(nmg);
		break;
	};
}

void GLAutoPilotC::MsgAutoPilotSetFb( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_AUTO_PILOT_SET_FB *pNetMsg = (GLMSG::SNETPC_REQ_AUTO_PILOT_SET_FB *) nmg;

	switch ( pNetMsg->emFB )
	{
	case EMREQ_AUTO_PILOT_FB_FAIL:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_FAIL") );
		break;

	case EMREQ_AUTO_PILOT_FB_INVALID_VALUE:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_INVALID_VALUE") );
		break;

	case EMREQ_AUTO_PILOT_FB_DONE:
		{
			m_bActive = pNetMsg->bActive;
			m_fRange = pNetMsg->fRange;

			m_sMap = GLGaeaClient::GetInstance().GetActiveMap()->GetMapID();

			if ( m_pChar )
				m_vPos = m_pChar->GetPosition();

			if ( m_bActive )
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_DONE_ON") );
			else
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_DONE_OFF") );
		}
		break;

	case EMREQ_AUTO_PILOT_FB_NOT_MAP:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_NOT_MAP") );
		break;
	};
}

void GLAutoPilotC::MsgAutoPilotResetFb( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_AUTO_PILOT_RESET_FB *pNetMsg = (GLMSG::SNETPC_REQ_AUTO_PILOT_RESET_FB *) nmg;

	m_bActive = pNetMsg->bActive;

	if ( m_bActive == FALSE )
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_AUTO_PILOT_FB_DONE_OFF") );
}