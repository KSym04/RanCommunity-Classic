#include "stdafx.h"
#include <string>

#include "./GLPVPWoeClient.h"
#include "./GLPVPTyrannyField.h"

#include "./GLGaeaClient.h"
#include "./DxGlobalStage.h"

#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GLPVPWoeClient& GLPVPWoeClient::GetInstance()
{
	static GLPVPWoeClient cInstance;
	return cInstance;
}

bool GLPVPWoeClient::FrameMove ( float fTime, float fElapsedTime  )
{
	if( m_emState == WOE_STATE_REGISTER ||
		m_emState == WOE_STATE_BATTLE ||
		m_emState == WOE_STATE_REWARD ){
		if ( m_fRemain > 0.0f ){
			m_fRemain -= fElapsedTime;
		}
	}

	return true;
}

void GLPVPWoeClient::SetEventState( WOE_STATE emState )
{
	switch( emState )
	{
	case WOE_STATE_ENDED:
		{
			DoStateEnd();
		}break;

	case WOE_STATE_REGISTER:
		{
			DoStateRegister();
		}break;

	case WOE_STATE_BATTLE:
		{
			DoStateBattle();
		}break;

	case WOE_STATE_REWARD:
		{
			DoStateReward();
		}break;
	};

	m_emState = emState;
}

void GLPVPWoeClient::DoStateRegister()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_WOE_REG") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_WOE_REG") );
}

void GLPVPWoeClient::DoStateBattle()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_WOE_BATTLE") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_WOE_BATTLE") );

	ResetScore();
}

void GLPVPWoeClient::DoStateReward()
{
	CInnerInterface::GetInstance().PrintConsoleText(ID2GAMEINTEXT("PVP_WOE_REWARD"));
	CInnerInterface::GetInstance().PVPCompetitionShowNotice(NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_WOE_REWARD"));
}

void GLPVPWoeClient::DoStateEnd()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_WOE_ENDED") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_WOE_ENDED") );

	m_fRemain = 0.0f;
	m_bRegistered = false;
	m_bShowStartNotice = FALSE;

	ResetScore();
}

void GLPVPWoeClient::NotifyTimeToStart( WORD wTime )
{
	if ( wTime > 0 ){
		CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_WOE_TOBATTLE"), wTime );
		CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_WOE_TOBATTLE"), wTime );
	}
}

void GLPVPWoeClient::DoRegister()
{
	if ( m_emState != WOE_STATE_REGISTER && m_emState != WOE_STATE_BATTLE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_NOTPOSSIBLE") );
		return;
	}
	
	if( DxGlobalStage::GetInstance().GetChannel() != 0 ){
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("WOE_REGISTER_FB_INVALID_CHANNEL") );
		return;
	}

	/*if ( m_emState == WOE_STATE_BATTLE && !m_bRegistered )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WOE_REGISTER_FB_NOTPOSSIBLE"));
		return;
	}*/

	if ( /*m_emState == WOE_STATE_REGISTER*/ !m_bRegistered )
	{
		GLMSG::SNETPC_WOE_C2A_REGISTER_REQ netmsgregister;
		netmsgregister.bRegister = !m_bRegistered;
		netmsgregister.wLevel = GLGaeaClient::GetInstance().GetCharacter()->m_wLevel;
		netmsgregister.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
		NETSEND ( &netmsgregister );
	}

	if ( m_emState == WOE_STATE_BATTLE && m_bRegistered )
	{
		if ( GLGaeaClient::GetInstance().GetActiveMap()->m_bPVPWoeMap )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("WOE_REJOIN_FB_INBATTLE") );
			return;
		}

		GLPetClient* pPetClient = GLGaeaClient::GetInstance().GetPetClient();
		if ( pPetClient->IsVALID() )
		{
			char gP4RM[64];
			sprintf_s(gP4RM, sizeof(gP4RM), "Please off your pet.");

			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::NEGATIVE, gP4RM);


			return;
		}
		
		GLMSG::SNETPC_WOE_C2A_REJOIN_REQ netmsgrejoin;
		netmsgrejoin.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
		NETSEND ( &netmsgrejoin );
	}
}

void GLPVPWoeClient::DoRevive()
{
	if ( !GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
	{
		return;
	}

	if ( m_emState != WOE_STATE_BATTLE && m_emState != WOE_STATE_REWARD )
	{
		GLGaeaClient::GetInstance().GetCharacter()->ReqReBirth();
		CInnerInterface::GetInstance().CloseAllWindow();
		return;
	}

	CInnerInterface::GetInstance().CloseAllWindow();
	GLGaeaClient::GetInstance().GetCharacter()->SetSTATE(EM_ACT_DIE);

	GLMSG::SNETPC_WOE_C2F_REVIVE_REQ netmsgrevive;
	netmsgrevive.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
	NETSENDTOFIELD ( &netmsgrevive );
}

void GLPVPWoeClient::RequestEmperiumInfo()
{
	PLANDMANCLIENT plandclient = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !plandclient )	return;

	if ( !plandclient->m_bPVPWoeMap )	return;

	GLMSG::SNETPC_WOE_C2A_TOWER_OWNER_INFO_REQ netmsgtowerinforeq;
	netmsgtowerinforeq.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
	NETSEND ( &netmsgtowerinforeq );
}

void GLPVPWoeClient::ResetScore()
{
	m_dwEmperiumGuildPrev = CLUB_NULL;
	m_dwEmperiumMarkVerPrev = 0;
	strName[0] = '\0';
	strLeader[0] = '\0';
}

void GLPVPWoeClient::RequestMapExit()
{
	GLMSG::SNETPC_WOE_C2A_MAPEXIT_REQ netmsgexit;
	netmsgexit.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
	NETSEND ( &netmsgexit );
}