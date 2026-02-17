#include "StdAfx.h"
#include "./GLPVPTyrannyClient.h"
#include "./UserTypeDefine.h"

#include "./GLGaeaClient.h"
#include "../DxGlobalStage.h"

#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"
#include "../../Lib_ClientUI/Interface/GameTextControl.h"



#include "../../Lib_Helper/RPCID/RPCID.h"
#include "../../Lib_Helper/RPCID/RPCIDSMBIOS.h"
#include "../../Lib_Helper/RPCID/RPCIDMAC.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLPVPTyrannyClient& GLPVPTyrannyClient::GetInstance()
{
	static GLPVPTyrannyClient cInstance;
	return cInstance;
}

bool GLPVPTyrannyClient::FrameMove ( float fTime, float fElapsedTime  )
{
	if( m_emState == TYRANNY_STATE_REGISTER ||
		m_emState == TYRANNY_STATE_BATTLE ||
		m_emState == TYRANNY_STATE_REWARD ){
		if ( m_fRemain > 0.0f ){
			m_fRemain -= fElapsedTime;
		}
	}
	// PVP Next Battle UI - Jxyyy
	else if( m_emState == TYRANNY_STATE_ENDED )
	{
		m_fRemainTimeToNextBattle -= fElapsedTime;
	}
	return true;
}

void GLPVPTyrannyClient::SetEventState( TYRANNY_STATE emState )
{
	switch( emState )
	{
	case TYRANNY_STATE_ENDED:
		{
			DoStateEnd();
		}break;

	case TYRANNY_STATE_REGISTER:
		{
			DoStateRegister();
		}break;

	case TYRANNY_STATE_BATTLE:
		{
			DoStateBattle();
		}break;

	case TYRANNY_STATE_REWARD:
		{
			DoStateReward();
		}break;
	};

	m_emState = emState;
}

void GLPVPTyrannyClient::DoStateRegister()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_TYRANNY_REG") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_TYRANNY_REG") );
}

void GLPVPTyrannyClient::DoStateBattle()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_TYRANNY_BATTLE") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_TYRANNY_BATTLE") );

	ResetScore();

	m_wLastWinner = TYRANNY_SCHOOL_SIZE;

	for( int i=0; i<TYRANNY_MINI_RANKING_NUM; ++i )
		m_sLastTop[i] = TYRANNY_PLAYER_DATA();
}

void GLPVPTyrannyClient::DoStateReward()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_TYRANNY_REWARD") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_TYRANNY_REWARD") );
}

void GLPVPTyrannyClient::DoStateEnd()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_TYRANNY_ENDED") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_TYRANNY_ENDED") );

	m_fRemain = 0.0f;
	m_bRegistered = false;
	m_bQueued = false;
	m_wQueueNum = 0;
	m_bShowStartNotice = FALSE;

	ResetScore();
}

void GLPVPTyrannyClient::NotifyTimeToStart( WORD wTime )
{
	if ( wTime > 0 ){
		CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_TYRANNY_TOBATTLE"), wTime );
		CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_TYRANNY_TOBATTLE"), wTime );
	}
}

void GLPVPTyrannyClient::DoRegister()
{
	//prevent register when pet was used just a few seconds ago
	if ( GLGaeaClient::GetInstance().GetCharacter()->isPetLastUseTime() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("PET_LAST_USE_ACTIVE") );
		return;
	}

	if ( m_emState != TYRANNY_STATE_REGISTER && m_emState != TYRANNY_STATE_BATTLE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("TYRANNY_REGISTER_FB_NOTPOSSIBLE") );
		return;
	}
	
	if( DxGlobalStage::GetInstance().GetChannel() != 0 ){
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("TYRANNY_REGISTER_FB_INVALID_CHANNEL") );
		return;
	}

	//if ( m_emState == TYRANNY_STATE_BATTLE && !m_bRegistered )
	//{
	//	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("TYRANNY_REGISTER_FB_NOTPOSSIBLE") );
	//	return;
	//}

	if ( m_emState == TYRANNY_STATE_REGISTER )
	{
		GLMSG::SNETPC_TYRANNY_C2A_REGISTER_REQ netmsgregister;
		netmsgregister.bRegister = !m_bRegistered;
		netmsgregister.wLevel = GLGaeaClient::GetInstance().GetCharacter()->m_wLevel;
		netmsgregister.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
		


		/* PVPTyranny Hash Check, Juver, 2020/07/28 */
		SPCID_SMBIOS sPCID_SMBIOS;
		RPCID::getSMBIOS( sPCID_SMBIOS );
		std::string strHashCombined("");
		strHashCombined += RPCID::getHashSMBIOS(sPCID_SMBIOS).c_str();
		strHashCombined += RPCID::getHashWindowsMAC().c_str();
		::StringCchCopy(netmsgregister.szHASH, TYRANNY_PLAYER_HASH_SIZE+1, strHashCombined.c_str() );


		NETSEND ( &netmsgregister );
	}

	if ( m_emState == TYRANNY_STATE_BATTLE )
	{
		if ( GLGaeaClient::GetInstance().GetActiveMap()->m_bPVPTyrannyMap )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("TYRANNY_REJOIN_FB_INBATTLE") );
			return;
		}

		// Guild Check Condition
		if ( (GLGaeaClient::GetInstance().GetCharacter()->m_dwUserLvl < NSUSER_TYPE::USER_TYPE_MASTER) &&
			(GLGaeaClient::GetInstance().GetCharacter()->m_dwGuild == 0) ) 
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, "You must be in a guild to rejoin Tyranny." );
			return;
		}

		if ( m_bRegistered )
		{
			GLMSG::SNETPC_TYRANNY_C2A_REJOIN_REQ netmsgrejoin;
			netmsgrejoin.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
			NETSEND ( &netmsgrejoin );
		}
		else
		{
			GLMSG::SNETPC_TYRANNY_C2A_LATE_JOIN_REQ NetMsgLateJoin;
			NetMsgLateJoin.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
			NetMsgLateJoin.wLevel = GLGaeaClient::GetInstance().GetCharacter()->m_wLevel;

			/* PVPTyranny Hash Check, Juver, 2020/07/28 */
			SPCID_SMBIOS sPCID_SMBIOS;
			RPCID::getSMBIOS( sPCID_SMBIOS );
			std::string strHashCombined("");
			strHashCombined += RPCID::getHashSMBIOS(sPCID_SMBIOS).c_str();
			strHashCombined += RPCID::getHashWindowsMAC().c_str();
			::StringCchCopy(NetMsgLateJoin.szHASH, TYRANNY_PLAYER_HASH_SIZE+1, strHashCombined.c_str() );

			NETSEND ( &NetMsgLateJoin );
		}
	}

	GLGaeaClient::GetInstance().GetCharacter()->resetPetBlockTime();
}

void GLPVPTyrannyClient::DoRevive()
{
	if ( !GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
	{
		return;
	}

	if ( m_emState != TYRANNY_STATE_BATTLE && m_emState != TYRANNY_STATE_REWARD )
	{
		GLGaeaClient::GetInstance().GetCharacter()->ReqReBirth();
		CInnerInterface::GetInstance().CloseAllWindow();
		return;
	}

	CInnerInterface::GetInstance().CloseAllWindow();
	GLGaeaClient::GetInstance().GetCharacter()->SetSTATE(EM_ACT_DIE);

	GLMSG::SNETPC_TYRANNY_C2F_REVIVE_REQ netmsgrevive;
	netmsgrevive.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
	NETSENDTOFIELD ( &netmsgrevive );
}

void GLPVPTyrannyClient::RequestTowerInfo()
{
	PLANDMANCLIENT plandclient = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !plandclient )	return;

	if ( !plandclient->m_bPVPTyrannyMap )	return;

	GLMSG::SNETPC_TYRANNY_C2A_TOWER_OWNER_INFO_REQ netmsgtowerinforeq;
	netmsgtowerinforeq.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
	NETSEND ( &netmsgtowerinforeq );
}

void GLPVPTyrannyClient::ResetScore()
{
	for( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
	{
		m_wTowerOwner[i] = TYRANNY_SCHOOL_SIZE;
		m_dwGuiID[i] = CLUB_NULL;
		m_dwMarkVer[i] = 0;
	}

	m_vecRankingAll.clear();
	m_vecRankingSG.clear();
	m_vecRankingMP.clear();
	m_vecRankingPHX.clear();
}

TYRANNY_PLAYER_DATA* GLPVPTyrannyClient::GetOwnRanking()
{
	if ( m_vecRankingAll.empty() )	return NULL;

	DWORD dwcharid = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;

	for( size_t i=0; i<m_vecRankingAll.size(); ++i )
	{
		TYRANNY_PLAYER_DATA& sranking = m_vecRankingAll[i];
		if ( dwcharid == sranking.dwInfoCharID )
		{
			return &sranking;
			break;
		}
	}

	return NULL;
}

int GLPVPTyrannyClient::GetRankingSize( int nType )
{
	switch( nType )
	{
	case TYRANNY_SCHOOL_SG:
		return (int)m_vecRankingSG.size();
		break;

	case TYRANNY_SCHOOL_MP:
		return (int)m_vecRankingMP.size();
		break;

	case TYRANNY_SCHOOL_PHX:
		return (int)m_vecRankingPHX.size();
		break;
	};

	return (int)m_vecRankingAll.size();
}

TYRANNY_PLAYER_DATA_VEC& GLPVPTyrannyClient::GetRankingVector( int nType )
{
	switch( nType )
	{
	case TYRANNY_SCHOOL_SG:
		return m_vecRankingSG;
		break;

	case TYRANNY_SCHOOL_MP:
		return m_vecRankingMP;
		break;

	case TYRANNY_SCHOOL_PHX:
		return m_vecRankingPHX;
		break;
	};

	return m_vecRankingAll;
}

void GLPVPTyrannyClient::RequestMapExit()
{
	GLMSG::SNETPC_TYRANNY_C2A_MAPEXIT_REQ netmsgexit;
	netmsgexit.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
	NETSEND ( &netmsgexit );
}

WORD GLPVPTyrannyClient::GetRebirthTime()
{
	WORD wSchool = GLGaeaClient::GetInstance().GetCharacter()->m_wSchool;

	if ( wSchool >= TYRANNY_SCHOOL_SIZE )	return 20;

	return m_wRebirthTime[wSchool];
}

BOOL GLPVPTyrannyClient::SetContributionPointMultiplier( float fMulti )
{
	m_fContributionPointMultiplier = fMulti;
	return TRUE;
}

float GLPVPTyrannyClient::GetContributionPointMultiplier()
{
	return m_fContributionPointMultiplier;
}