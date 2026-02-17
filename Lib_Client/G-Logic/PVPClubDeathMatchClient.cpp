#include "stdafx.h"
#include "./PVPClubDeathMatchClient.h"

#include "./GLGaeaClient.h"
#include "../DxGlobalStage.h"

#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"

#include "../../Lib_Helper/RPCID/RPCID.h"
#include "../../Lib_Helper/RPCID/RPCIDSMBIOS.h"
#include "../../Lib_Helper/RPCID/RPCIDMAC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PVPClubDeathMatchClient& PVPClubDeathMatchClient::GetInstance()
{
	static PVPClubDeathMatchClient cInstance;
	return cInstance;
}

PVPClubDeathMatchClient::PVPClubDeathMatchClient()
	: m_emState(PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED)
	, m_fRemain(0.0f)

	, m_wLevelReq(1)
	, m_wClubLevelLimit(1)
	, m_wClubPlayerMin(1)
	, m_wClubPlayerMax(1)
	, m_bClubPlayerReqCDMRights(true)

	, m_wBattleDuration(1)

	, m_bRegistered(false)
	, m_wClubParticipants(0)
	, m_bShowStartNotice(FALSE)

	, m_bDoublePoint(FALSE)
{
	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
	{
		m_sReward[i] = PVP_CLUB_DEATH_MATCH_REWARD_DATA();
	}
}

PVPClubDeathMatchClient::~PVPClubDeathMatchClient()
{

}

bool PVPClubDeathMatchClient::FrameMove ( float fTime, float fElapsedTime )
{
	if( m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER ||
		m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE ||
		m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD )
	{
			if ( m_fRemain > 0.0f )
			{
				m_fRemain -= fElapsedTime;
			}
	}

	if ( m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE )
	{
	}

	return true;
}

void PVPClubDeathMatchClient::SetEventState( PVP_CLUB_DEATH_MATCH_EVENT_STATE emState )
{
	switch( emState )
	{
	case PVP_CLUB_DEATH_MATCH_EVENT_STATE_ENDED:
		{
			DoStateEnd();
		}break;

	case PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER:
		{
			DoStateRegister();
		}break;

	case PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE:
		{
			DoStateBattle();
		}break;

	case PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD:
		{
			DoStateReward();
		}break;
	};

	m_emState = emState;
}

void PVPClubDeathMatchClient::DoStateRegister()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REG") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REG") );

	ProcessClubCleanUp();
}

void PVPClubDeathMatchClient::DoStateBattle()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_BATTLE") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_BATTLE") );

	m_sFirstClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();
	m_sMyClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();
	m_vecRanking.clear();

	for( int i=0; i<PVP_CLUB_DEATH_MATCH_RANK_TOP; ++i )
		m_sClubTop[i] = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();

	m_bDoublePoint = FALSE;
}

void PVPClubDeathMatchClient::DoStateReward()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REWARD") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REWARD") );
}

void PVPClubDeathMatchClient::DoStateEnd()
{
	CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_ENDED") );
	CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_ENDED") );

	m_fRemain = 0.0f;
	m_bRegistered = false;
	
	m_bShowStartNotice = FALSE;

	m_sFirstClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();
	m_sMyClubRank = PVP_CLUB_DEATH_MATCH_CLUB_DATA_RANK();
	m_vecRanking.clear();

	m_bDoublePoint = FALSE;

	ProcessClubCleanUp();
}

void PVPClubDeathMatchClient::NotifyTimeToStart( WORD wTime )
{
	if ( wTime > 0 ){
		if ( wTime <= 1 )
		{
			CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_TOBATTLE2"), wTime );
			CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_TOBATTLE2"), wTime );
		}
		else
		{
			CInnerInterface::GetInstance().PrintConsoleText( ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_TOBATTLE"), wTime );
			CInnerInterface::GetInstance().PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_TOBATTLE"), wTime );
		}

	}
}

void PVPClubDeathMatchClient::DoRegister()
{
	//prevent register when pet was used just a few seconds ago
	if ( GLGaeaClient::GetInstance().GetCharacter()->isPetLastUseTime() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("PET_LAST_USE_ACTIVE") );
		return;
	}

	if ( m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER && m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_NOTPOSSIBLE") );
		return;
	}

	if( DxGlobalStage::GetInstance().GetChannel() != 0 )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CHANNEL") );
		return;
	}

	if ( m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE && !m_bRegistered )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_NOTPOSSIBLE") );
		return;
	}

	if ( m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_REGISTER )
	{
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		if ( pCharacter )
		{
			if ( pCharacter->m_dwGuild==PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_INVALID_CLUB") );
				return;
			}

			if ( pCharacter->m_wLevel < m_wLevelReq )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_LEVEL_LOW") );
				return;
			}

			if ( pCharacter->m_sCLUB.m_dwRank < m_wClubLevelLimit )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_CLUB_LEVEL_LOW") );
				return;
			}

			if ( m_bClubPlayerReqCDMRights )
			{
				if ( !pCharacter->m_sCLUB.IsMemberFlgCDM(pCharacter->m_dwCharID) )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REGISTER_FB_REQUIRE_RIGHTS") );
					return;
				}
			}
		}
		GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REGISTER_REQ netmsgregister;
		netmsgregister.bRegister = !m_bRegistered;
		netmsgregister.wLevel = pCharacter->m_wLevel;
		netmsgregister.dwCharID = pCharacter->m_dwCharID;

		/* PVPClubDeathMatch hash check, Juver, 2021/07/18 */
		SPCID_SMBIOS sPCID_SMBIOS;
		RPCID::getSMBIOS( sPCID_SMBIOS );
		std::string strHashCombined("");
		strHashCombined += RPCID::getHashSMBIOS(sPCID_SMBIOS).c_str();
		strHashCombined += RPCID::getHashWindowsMAC().c_str();
		::StringCchCopy(netmsgregister.szHASH, SCHOOLWARS_PLAYER_HASH_SIZE+1, strHashCombined.c_str() );

		NETSEND ( &netmsgregister );
	}

	if ( m_emState == PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE && m_bRegistered )
	{
		if ( GLGaeaClient::GetInstance().GetActiveMap()->m_bPVPClubDeathMatchLobbyMap ||
			GLGaeaClient::GetInstance().GetActiveMap()->m_bPVPClubDeathMatchBattleMap )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PVP_CLUB_DEATH_MATCH_REJOIN_FB_INBATTLE") );
			return;
		}

		GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2A_REJOIN_REQ netmsgrejoin;
		netmsgrejoin.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
		NETSEND ( &netmsgrejoin );
	}

	GLGaeaClient::GetInstance().GetCharacter()->resetPetBlockTime();
}

//remove all club data
void PVPClubDeathMatchClient::ProcessClubCleanUp()
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_b = m_mapClubData.begin();
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it_e = m_mapClubData.end();

	for ( ; it_b != it_e; ++it_b )
	{
		PVP_CLUB_DEATH_MATCH_CLUB_DATA& sData = (*it_b).second;

		//call cleanup
		sData.CleanUp();
	}

	//erase all data
	m_mapClubData.clear();
}

PVP_CLUB_DEATH_MATCH_CLUB_DATA* PVPClubDeathMatchClient::GetClubData( DWORD dwClubID )
{
	PVP_CLUB_DEATH_MATCH_CLUB_DATA_MAP_ITER it = m_mapClubData.find( dwClubID );
	if ( it != m_mapClubData.end() )
	{
		return &(*it).second;
	}

	return NULL;
}

PVP_CLUB_DEATH_MATCH_CLUB_DATA* PVPClubDeathMatchClient::CreateClubData( DWORD dwClubID, const TCHAR* szClubName, const TCHAR* szMasterName, DWORD dwClubMarkVer )
{
	if ( dwClubID == PVP_CLUB_DEATH_MATCH_CLUB_ID_NULL ) return NULL;
	if ( !szClubName )		return NULL;
	if ( !szMasterName )	return NULL;

	PVP_CLUB_DEATH_MATCH_CLUB_DATA sData;
	sData.dwClubID = dwClubID;
	StringCchCopy( sData.szClubName, PVP_CLUB_DEATH_MATCH_CLUB_NAME_SIZE, szClubName );
	StringCchCopy( sData.szMasterName, PVP_CLUB_DEATH_MATCH_CHAR_NAME_SIZE, szMasterName );
	sData.dwCLUB_MARK_VER = dwClubMarkVer;

	m_mapClubData.insert( std::make_pair( sData.dwClubID, sData ) );

	return GetClubData( dwClubID );
}


void PVPClubDeathMatchClient::DoRevive()
{
	if ( !GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
	{
		return;
	}

	if ( m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_BATTLE && m_emState != PVP_CLUB_DEATH_MATCH_EVENT_STATE_REWARD )
	{
		GLGaeaClient::GetInstance().GetCharacter()->ReqReBirth();
		CInnerInterface::GetInstance().CloseAllWindow();
		return;
	}

	CInnerInterface::GetInstance().CloseAllWindow();
	GLGaeaClient::GetInstance().GetCharacter()->SetSTATE(EM_ACT_DIE);

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ netmsgrevive;
	netmsgrevive.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
	NETSEND ( &netmsgrevive );
}

void PVPClubDeathMatchClient::RequestScoreInfo()
{
	PLANDMANCLIENT plandclient = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !plandclient )	return;

	if ( !plandclient->m_bPVPClubDeathMatchLobbyMap && 
		!plandclient->m_bPVPClubDeathMatchBattleMap )	return;

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2F_CLUB_DATA_SCORE_INFO_REQ netmsginforeq;
	netmsginforeq.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
	NETSENDTOFIELD ( &netmsginforeq );
}

void PVPClubDeathMatchClient::RequestMapExit()
{
	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2A_MAPEXIT_REQ netmsgexit;
	netmsgexit.dwCharID = GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;
	NETSEND ( &netmsgexit );
}
