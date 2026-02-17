#include "stdafx.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#include "./GLChar.h"
#include "./GLGaeaServer.h"
#include "./GLAutoPilotS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLAutoPilotS::GLAutoPilotS(GLChar* pChar)
	: GLAutoPilot()
	, m_pChar(pChar)
{

}

GLAutoPilotS::~GLAutoPilotS()
{

}

void GLAutoPilotS::Reset()
{
	GLAutoPilot::Reset();
}

void GLAutoPilotS::MsgProcess( NET_MSG_GENERIC* nmg )
{
	if ( nmg == NULL )
		return;

	switch ( nmg->nType )
	{
	case NET_MSG_GCTRL_REQ_AUTO_PILOT_SET:
		MsgAutoPilotSet(nmg);
		break;

	case NET_MSG_GCTRL_REQ_AUTO_PILOT_RESET:
		MsgAutoPilotReset(nmg);
		break;
	};

}

void GLAutoPilotS::MsgAutoPilotSet( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_AUTO_PILOT_SET *pNetMsg = (GLMSG::SNETPC_REQ_AUTO_PILOT_SET *) nmg;
	GLMSG::SNETPC_REQ_AUTO_PILOT_SET_FB NetMsgFb;

	if ( m_pChar == NULL )
		return;

	if ( RPARAM::bUseAutoPilot == FALSE )
	{
		NetMsgFb.emFB = EMREQ_AUTO_PILOT_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_pChar->m_dwClientID, &NetMsgFb);
		return;
	}

	if ( m_pChar->m_pLandMan == NULL )
	{
		NetMsgFb.emFB = EMREQ_AUTO_PILOT_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_pChar->m_dwClientID, &NetMsgFb);
		return;
	}

	if ( m_pChar->m_pLandMan->isUseAutoPilot() == FALSE )
	{
		NetMsgFb.emFB = EMREQ_AUTO_PILOT_FB_NOT_MAP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_pChar->m_dwClientID, &NetMsgFb);
		return;
	}

	if ( pNetMsg->fRange <= 0.0f || pNetMsg->fRange > RPARAM::fAutoPilotRangeMax )
	{
		NetMsgFb.emFB = EMREQ_AUTO_PILOT_FB_INVALID_VALUE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_pChar->m_dwClientID, &NetMsgFb);
		return;
	}
	
	m_bActive = pNetMsg->bActive;
	m_fRange = pNetMsg->fRange;

	NetMsgFb.emFB = EMREQ_AUTO_PILOT_FB_DONE;
	NetMsgFb.bActive = m_bActive;
	NetMsgFb.fRange = m_fRange;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_pChar->m_dwClientID, &NetMsgFb);
}

void GLAutoPilotS::MsgAutoPilotReset( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_AUTO_PILOT_RESET *pNetMsg = (GLMSG::SNETPC_REQ_AUTO_PILOT_RESET *) nmg;
	GLMSG::SNETPC_REQ_AUTO_PILOT_RESET_FB NetMsgFb;

	if ( m_pChar == NULL )
		return;

	m_bActive = pNetMsg->bActive;

	NetMsgFb.bActive = m_bActive;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_pChar->m_dwClientID, &NetMsgFb);
}

