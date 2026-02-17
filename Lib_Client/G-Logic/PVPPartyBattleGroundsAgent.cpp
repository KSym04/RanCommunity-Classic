#include "stdafx.h"
#include "./PVPPartyBattleGroundsAgent.h"

#include "./GLAgentServer.h"
#include "./GLClubMan.h"

#include "./PVPPartyBattleGroundsMsg.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace PVPPBG
{
	ManagerAgent& ManagerAgent::GetInstance()
	{
 		static ManagerAgent cInstance;
 		return cInstance;
	}
 
	ManagerAgent::ManagerAgent()
 		: Manager()
		, m_fTimer(0.0f)
 		, m_fRemain(0.0f)
 		, m_wLastNotifyTime(0)
 		, m_bMoveIn(false)
 		, m_bMoveOut(false)
 		, m_fMoveTimer(0.0f)
 		, m_fMoveTimerTotal(0.0f)
	{
 
	}
 
	ManagerAgent::~ManagerAgent()
	{
 
	}
 
	void ManagerAgent::LoadEvent( const std::string& strFile )
	{
 		if ( !RPARAM::bEventPartyBattleGrounds )
 			return;
 
 		ResetEvent();
 
 		bool bok = DataLoad( strFile );
 		if ( bok )
 		{
 			FindScheduleFirst();
 		}
	}
 
	void ManagerAgent::ResetEvent()
	{
 		m_emState = PVPPBG::EVENT_STATE_ENDED;
 		m_sScheduleNext.Reset();
 		m_vecSchedule.clear();
 
		for( int i=0; i<PVP_PBG_MINI_RANKING_NUM; ++i )
			m_sLastTop[i] = PLAYER_DATA();
 
 		m_vecPlayerMove.clear();
		m_mapPlayerData.clear();
		m_vecPlayerDataQueue.clear();
 
 		m_bMoveIn = false;
 		m_bMoveOut = false;
 
 		m_fMoveTimer = 0.0f;
 		m_fMoveTimerTotal = 0.0f;
 
 		m_mapPlayerHash.clear();
 
	}
 
	bool ManagerAgent::SetMapState ()
	{
 		if( m_sBattleMap.IsValidNativeID() )
 		{
 			SMAPNODE *pMapNode = GLAgentServer::GetInstance().GetMapList()->FindMapNode ( m_sBattleMap );
 			if ( pMapNode )
 				pMapNode->bPVPPBGBattleZone = true;
 
 			GLAGLandMan *pLandMan = GLAgentServer::GetInstance().GetByMapID ( m_sBattleMap );
 			if ( pLandMan )
 				pLandMan->m_bPVPPBGBattleMap = true;
 		}
 
 		if( m_sLobbyMap.IsValidNativeID() )
 		{
 			SMAPNODE *pMapNode = GLAgentServer::GetInstance().GetMapList()->FindMapNode ( m_sLobbyMap );
 			if ( pMapNode )
 				pMapNode->bPVPPBGLobbyZone = true;
 
 			GLAGLandMan *pLandMan = GLAgentServer::GetInstance().GetByMapID ( m_sLobbyMap );
 			if ( pLandMan )
 				pLandMan->m_bPVPPBGLobbyMap = true;
 		}
 
 
 		return true;
	}

	bool ManagerAgent::FrameMove ( float fElapsedTime )
	{
		if ( GLCONST_CHAR::bPKLESS )
			return false;

		if ( m_vecSchedule.empty() )	
			return false;

		if ( IsEnded() )
		{
			if ( m_sScheduleNext.dwIndex != PVP_PBG_SCHEDULE_NONE )
			{
				CTime ctimecur = CTime::GetCurrentTime();
				if ( m_sScheduleNext.sStartSched.wStartDay == (WORD)ctimecur.GetDayOfWeek() &&
					m_sScheduleNext.sStartSched.wStartHour == (WORD)ctimecur.GetHour() &&
					m_sScheduleNext.sStartSched.wStartMinute == (WORD)ctimecur.GetMinute() )
				{
					SetEventState( PVPPBG::EVENT_STATE_REGISTER );
				}
			}
		}


		if ( IsRegister() )
		{
			m_fTimer += fElapsedTime;
			m_fRemain -= fElapsedTime;

			NotifyTimeToStart( m_fRemain );	

			if ( m_fTimer >= (float)m_wRegisterTime )
			{
				SetEventState( PVPPBG::EVENT_STATE_BATTLE );
			}
		}

		if ( IsBattle() )
		{
			m_fTimer += fElapsedTime;
			m_fRemain -= fElapsedTime;

			CheckForceMove( fElapsedTime );

			if ( m_fTimer >= (float)m_wBattleTime )
			{
				SetEventState( PVPPBG::EVENT_STATE_REWARD );
			}
		}

		if ( IsReward() )
		{
			m_fTimer += fElapsedTime;
			m_fRemain -= fElapsedTime;

			if ( m_fTimer >= (float)m_wRewardTime )
			{
				SetEventState( PVPPBG::EVENT_STATE_ENDED );
			}
		}

		if ( m_bMoveIn && IsBattle() )
		{
			m_fMoveTimer += fElapsedTime;
			m_fMoveTimerTotal += fElapsedTime;

			if( m_fMoveTimer >= 0.2f )
			{
				MapMoveInPlayer();
				m_fMoveTimer = 0.0f;
			}

			if ( m_fMoveTimerTotal >= 300.0f )
			{
				m_bMoveIn = false;
				m_fMoveTimer = 0.0f;
				m_fMoveTimerTotal = 0.0f;
			}

			if ( m_vecPlayerMove.empty() )
			{
				m_bMoveIn = false;
				m_fMoveTimer = 0.0f;
				m_fMoveTimerTotal = 0.0f;
			}

		}

		if ( m_bMoveOut && IsEnded() )
		{
			m_fMoveTimer += fElapsedTime;
			m_fMoveTimerTotal += fElapsedTime;

			if( m_fMoveTimer >= 0.2f )
			{
				MapMoveOutPlayer();
				m_fMoveTimer = 0.0f;
			}

			if ( m_fMoveTimerTotal >= 300.0f )
			{
				m_bMoveOut = false;
				m_fMoveTimer = 0.0f;
				m_fMoveTimerTotal = 0.0f;
			}

			if ( m_vecPlayerMove.empty() )
			{
				m_bMoveOut = false;
				m_fMoveTimer = 0.0f;
				m_fMoveTimerTotal = 0.0f;
			}
		}

		return true;
	}


	void ManagerAgent::SetEventState( PVPPBG::EVENT_STATE emState )
	{
		GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "PVPPBG::ManagerAgent::SetEventState '%s'", PVPPBG::STRING_EVENT_STATE[emState].c_str() );
		RLOG::LogFile( "_PVPPBG.txt", "PVPPBG::ManagerAgent::SetEventState '%s'", PVPPBG::STRING_EVENT_STATE[emState].c_str() );

		switch( emState )
		{
		case PVPPBG::EVENT_STATE_ENDED:
			DoStateEnd();
			break;

		case PVPPBG::EVENT_STATE_REGISTER:
			DoStateRegister();
			break;

		case PVPPBG::EVENT_STATE_BATTLE:
			DoStateBattle();
			break;

		case PVPPBG::EVENT_STATE_REWARD:
			DoStateReward();
			break;
		};

		m_emState = emState;
	}


	void ManagerAgent::DoStateRegister()
	{
		m_wLastNotifyTime = 0;
		m_fTimer	= 0.0f;
		m_fRemain	= (float)m_wRegisterTime;

 		m_vecPlayerMove.clear();
		m_mapPlayerData.clear();
		m_vecPlayerDataQueue.clear();

 		m_mapPlayerHash.clear();

		GLMSG::SNET_PVP_PBG_A2FC_STATE_REGISTER netmsgstate;
		netmsgstate.fTime = m_fRemain;
		GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
		GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
	}

	void ManagerAgent::DoStateBattle()
	{
		m_fTimer	= 0.0f;
		m_fRemain	= (float)m_wBattleTime;

		for( int i=0; i<PVP_PBG_MINI_RANKING_NUM; ++i )
			m_sLastTop[i] = PLAYER_DATA();

		ProcessQueueDataAll();

		SendPlayerData();

 		QueueMoveIn();

		GLMSG::SNET_PVP_PBG_A2FC_STATE_BATTLE netmsgstate;
		netmsgstate.fTime = m_fRemain;
		GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
		GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
	}

	void ManagerAgent::DoStateReward()
	{
		m_fTimer	= 0.0f;
		m_fRemain	= (float)m_wRewardTime;


		GLMSG::SNET_PVP_PBG_A2FC_STATE_REWARD netmsgstate;
		netmsgstate.fTime = m_fRemain;
		GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
		GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
	}

	void ManagerAgent::DoStateEnd()
	{
		GLMSG::SNET_PVP_PBG_A2FC_STATE_ENDED netmsgstate;
		GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
		GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );

		if ( FindScheduleNext() )
		{
			GLMSG::SNET_PVP_PBG_A2FC_NEXTSCHED netmsgschedule;
			netmsgschedule.sScheduleNext = m_sScheduleNext;
			GLAgentServer::GetInstance().SENDTOCHANNEL( &netmsgschedule, 0 );
			GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsgschedule );
		}

 		QueueMoveOut();
 
		m_mapPlayerData.clear();
		m_vecPlayerDataQueue.clear();
 
 		m_mapPlayerHash.clear();
	}

	void ManagerAgent::NotifyTimeToStart( float fTime )
	{
		WORD wremain = (WORD)m_fRemain;
		if( m_wLastNotifyTime == wremain )
			return;

		bool bnotify = false;
		if ( wremain > 300 && ( wremain%300 == 0 ) )	
			bnotify = true;
		else if ( wremain > 0 && wremain <= 300 && ( wremain%60 == 0 ) )
			bnotify = true;

		if ( !bnotify )	
			return;

		m_wLastNotifyTime = wremain;

		GLMSG::SNET_PVP_PBG_A2C_TOBATTLE_TIME netmsgclient;
		netmsgclient.wTime = wremain / 60;
		GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgclient );
	}

	REGISTER_DATA* ManagerAgent::PlayerDataGet( DWORD dwCharID )
	{
		if ( dwCharID == PVP_PBG_CHAR_ID_NULL ) 
			return NULL;

		REGISTER_DATA_MAP_ITER iterdata = m_mapPlayerData.find( dwCharID );
		if( iterdata != m_mapPlayerData.end() )
		{
			return &(*iterdata).second;
		}
	
		return NULL;
	};

	REGISTER_DATA_QUEUE* ManagerAgent::PlayerQueueGet( DWORD dwCharID )
	{
		if ( dwCharID == PVP_PBG_CHAR_ID_NULL )
			return NULL;

		for( size_t i=0; i<m_vecPlayerDataQueue.size(); ++i )
		{
			const REGISTER_DATA_QUEUE& sdata = m_vecPlayerDataQueue[i];
			if ( sdata.sPlayerData.dwInfoCharID == dwCharID )
			{
				return &m_vecPlayerDataQueue[i];
			}
		}

		return NULL;
	};

	BOOL ManagerAgent::IsPlayerFull()
	{
		//unlimited team members
		if ( m_wPlayerLimit == 0 )	
			return FALSE;

		if ( m_mapPlayerData.size() >= m_wPlayerLimit )
			return TRUE;

		return FALSE;
	}

	BOOL ManagerAgent::IsPlayerReg( DWORD dwCharID )
	{
		if ( dwCharID == PVP_PBG_CHAR_ID_NULL )
			return FALSE;

		REGISTER_DATA_MAP_ITER iterdata = m_mapPlayerData.find( dwCharID );
		if( iterdata != m_mapPlayerData.end() )
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL ManagerAgent::IsPlayerQueue( DWORD dwCharID )
	{
		if ( dwCharID == PVP_PBG_CHAR_ID_NULL )	
			return FALSE;

		for( size_t i=0; i<m_vecPlayerDataQueue.size(); ++i )
		{
			const REGISTER_DATA_QUEUE sdata = m_vecPlayerDataQueue[i];
			if ( sdata.sPlayerData.dwInfoCharID == dwCharID )
			{
				return TRUE;
				break;
			}
		}

		return FALSE;
	}

	BOOL ManagerAgent::InsertQueue( DWORD dwCharID, const char* szName, WORD wClass, WORD wSchool )
	{
		if ( !szName )							
			return FALSE;

		if ( dwCharID == PVP_PBG_CHAR_ID_NULL )	
			return FALSE;

		if ( wClass >= GLCI_NUM_8CLASS )	
			return FALSE;

		if ( wSchool >= PVPPBG::SCHOOL_SIZE )
			return FALSE;

		REGISTER_DATA_QUEUE sdata;
		sdata.sPlayerData.dwInfoCharID = dwCharID;
		sdata.sPlayerData.wInfoClass = wClass;
		sdata.sPlayerData.wInfoSchool = wSchool;
		sdata.wQueueNum = (WORD)m_vecPlayerDataQueue.size() + 1;
		StringCchCopy( sdata.sPlayerData.szInfoCharName, SCHOOLWARS_CHARNAME_STRING_SIZE, szName );
		m_vecPlayerDataQueue.push_back( sdata );

		return TRUE;
	}

	BOOL ManagerAgent::InsertRegister( DWORD dwCharID, const char* szName, WORD wClass, WORD wSchool )
	{
		if ( !szName )							
			return FALSE;

		if ( dwCharID == PVP_PBG_CHAR_ID_NULL )
			return FALSE;

		if ( wClass >= GLCI_NUM_8CLASS )	
			return FALSE;

		if ( wSchool >= PVPPBG::SCHOOL_SIZE )
			return FALSE;

		REGISTER_DATA sdata;
		sdata.dwInfoCharID = dwCharID;
		sdata.wInfoClass = wClass;
		sdata.wInfoSchool = wSchool;
		StringCchCopy( sdata.szInfoCharName, PVP_PBG_CHAR_NAME_SIZE, szName );
		m_mapPlayerData.insert( std::make_pair( sdata.dwInfoCharID, sdata ) );

		return TRUE;
	}

	BOOL ManagerAgent::DeleteRegister( DWORD dwCharID )
	{
		if ( dwCharID == PVP_PBG_CHAR_ID_NULL )
			return FALSE;

		REGISTER_DATA_MAP_ITER iterdata = m_mapPlayerData.find( dwCharID );
		if( iterdata != m_mapPlayerData.end() )
		{
			m_mapPlayerData.erase( iterdata );
			return TRUE;
		}

		return FALSE;
	}

	BOOL ManagerAgent::DeleteQueue( DWORD dwCharID )
	{
		if ( dwCharID == PVP_PBG_CHAR_ID_NULL )	
			return FALSE;

		for( size_t i=0; i<m_vecPlayerDataQueue.size(); ++i )
		{
			REGISTER_DATA_QUEUE sdata = m_vecPlayerDataQueue[i];
			if ( sdata.sPlayerData.dwInfoCharID == dwCharID )
			{
				m_vecPlayerDataQueue.erase( m_vecPlayerDataQueue.begin() + i );
				return TRUE;
				break;
			}
		}

		return FALSE;
	}

	void ManagerAgent::ProcessQueueData()
	{
		if ( IsPlayerFull() )
			return;

		if ( m_vecPlayerDataQueue.size() <= 0 )	
			return;

		//get data from the very first vector entry
		REGISTER_DATA_QUEUE sdata = m_vecPlayerDataQueue[0];
		PGLCHARAG pcharmove = GLAgentServer::GetInstance().GetCharID( sdata.sPlayerData.dwInfoCharID );
		if ( pcharmove )
		{
			//insert to player data
			m_mapPlayerData.insert( std::make_pair( sdata.sPlayerData.dwInfoCharID, sdata.sPlayerData ) );

			//inform player moved from queue to registered
			GLMSG::SNETPC_PVP_PBG_A2C_QUEUE_MOVED netmsgclient;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pcharmove->m_dwClientID, &netmsgclient );
		}

		//erase the entry from vector
		m_vecPlayerDataQueue.erase(m_vecPlayerDataQueue.begin());
	}

	void ManagerAgent::UpdateQueuePosition()
	{
		if ( m_vecPlayerDataQueue.size() <= 0 )
			return;

		//inform players of their current position in queue
		for( size_t i=0; i<m_vecPlayerDataQueue.size(); ++i )
		{
			REGISTER_DATA_QUEUE &sdata = m_vecPlayerDataQueue[i];
			sdata.wQueueNum = i + 1;

			PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( sdata.sPlayerData.dwInfoCharID );
			if ( !pchar ) 
				continue;

			GLMSG::SNETPC_PVP_PBG_A2C_QUEUE_UPDATE netmsgclient;
			netmsgclient.wQueueNum = sdata.wQueueNum;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgclient );
		}
	}

	void ManagerAgent::ProcessQueueDataAll()
	{
		//try to move the remaining queued players to registered
		if ( !IsPlayerFull() )
		{
			while(1)
			{
				ProcessQueueData();

				if ( IsPlayerFull() )
					break;

				if ( m_vecPlayerDataQueue.size() <= 0 )
					break;

				Sleep(0);
			}
		}

		//remove queued players
		for( size_t i=0; i<m_vecPlayerDataQueue.size(); ++i )
		{
			REGISTER_DATA_QUEUE sdata = m_vecPlayerDataQueue[i];

			PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( sdata.sPlayerData.dwInfoCharID );
			if ( !pchar ) 
				continue;

			GLMSG::SNETPC_PVP_PBG_A2C_REGISTER_FB netmsgfb;
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_CANCEL_REG;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
		}

		//clear
		m_vecPlayerDataQueue.clear();
	}

	void ManagerAgent::SendPlayerData()
	{
		REGISTER_DATA_VEC vecdata;
		vecdata.reserve( m_mapPlayerData.size() );

		REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerData.begin();
		for( ; iterplayer != m_mapPlayerData.end(); ++ iterplayer )
		{
			const REGISTER_DATA& splayerdata = (*iterplayer).second;
			vecdata.push_back( splayerdata );
		}

		WORD wNum = 0;
		GLMSG::SNETPC_PVP_PBG_A2F_PLAYER_DATA net_msg_data;
		for ( size_t i=0; i<vecdata.size(); ++i )
		{
			const REGISTER_DATA& splayer_data = vecdata[i];
			net_msg_data.ADDRANK( splayer_data );

			if ( net_msg_data.wRankNum == SCHOOLWARS_PACKET_PLAYER_DATA_NUM )
			{
				GLAgentServer::GetInstance().SENDTOCHANNEL( &net_msg_data, 0 );
				wNum += net_msg_data.wRankNum;
				net_msg_data.Reset();
			}
		}

		if ( net_msg_data.wRankNum != 0 )
		{
			GLAgentServer::GetInstance().SENDTOCHANNEL( &net_msg_data, 0 );
			wNum += net_msg_data.wRankNum;
			net_msg_data.Reset();
		}

		CDebugSet::ToFileWithTime( "_PVPPBG.txt", "PVPPBG::ManagerAgent Send PlayerData num: %u", wNum );
		GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "PVPPBG::ManagerAgent Send PlayerData num: %u", wNum );
	}

	void ManagerAgent::QueueMoveIn()
	{
		//queue to move players in
		m_vecPlayerMove.clear();
		m_fMoveTimer = 0.0f;
		m_fMoveTimerTotal = 0.0f;

		REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerData.begin();
		for( ; iterplayer != m_mapPlayerData.end(); ++ iterplayer )
		{
			const REGISTER_DATA& splayerdata = (*iterplayer).second;

			PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
			if ( !pchar )	
				continue;

			if ( pchar->m_sCurMapID == m_sLobbyMap )	
				continue;

			m_vecPlayerMove.push_back( splayerdata );
		}

		m_bMoveIn = true;
	}

	void ManagerAgent::QueueMoveOut()
	{
		//queue to move players out
		m_vecPlayerMove.clear();
		m_fMoveTimer = 0.0f;
		m_fMoveTimerTotal = 0.0f;

		REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerData.begin();
		for( ; iterplayer != m_mapPlayerData.end(); ++ iterplayer )
		{
			const REGISTER_DATA& splayerdata = (*iterplayer).second;

			PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
			if ( !pchar )	
				continue;

			if ( pchar->m_sCurMapID == m_sOutMap )	
				continue;

			m_vecPlayerMove.push_back( splayerdata );
		}

		m_bMoveOut = true;
	}

	void ManagerAgent::MapMoveInPlayer()
	{
		if ( m_vecPlayerMove.empty() )
			return;

		REGISTER_DATA sdata = m_vecPlayerMove[0];

		DWORD dwCharID = sdata.dwInfoCharID;
		SNATIVEID sMapid = m_sLobbyMap;
		DWORD dwGateID = m_dwLobbyGate;

		m_vecPlayerMove.erase(m_vecPlayerMove.begin());

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
		if ( pchar )
		{
			GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( sMapid );
			if ( plandman )
			{
				DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
				if ( pGateMan )
				{
					PDXLANDGATE pGate = pGateMan->FindLandGate ( dwGateID );
					if ( pGate )
					{
						D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

						GLMSG::SNETPC_PVP_PBG_A2F_MAP_MOVE net_msg_field;
						net_msg_field.nidMAP = sMapid;
						net_msg_field.vPos = vGenPos;
						net_msg_field.bCurMapCheck = TRUE;
						net_msg_field.dwCharID = pchar->m_dwCharID;
						GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
						return;
					}
				}
			}
		}

		RLOG::LogFile( "_PVPPBG.txt", "_PVPPBG::ManagerAgent::MapMoveInPlayer char:%d failure to move map:[%d~%d] gate%d", dwCharID, sMapid.wMainID, sMapid.wSubID, dwGateID );
	}

	void ManagerAgent::MapMoveOutPlayer()
	{
		if ( m_vecPlayerMove.empty() )
			return;

		REGISTER_DATA sdata = m_vecPlayerMove[0];

		DWORD dwCharID = sdata.dwInfoCharID;
		SNATIVEID sMapid = m_sOutMap;
		DWORD dwGateID = m_dwOutGate;

		m_vecPlayerMove.erase(m_vecPlayerMove.begin());

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
		if ( pchar )
		{
			GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( sMapid );
			if ( plandman )
			{
				DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
				if ( pGateMan )
				{
					PDXLANDGATE pGate = pGateMan->FindLandGate ( dwGateID );
					if ( pGate )
					{
						D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

						GLMSG::SNETPC_PVP_PBG_A2F_MAP_MOVE net_msg_field;
						net_msg_field.nidMAP = sMapid;
						net_msg_field.vPos = vGenPos;
						net_msg_field.bCurMapCheck = TRUE;
						net_msg_field.dwCharID = pchar->m_dwCharID;
						net_msg_field.bSourceMapCheck = TRUE;
						net_msg_field.nidSourceLobbyMAP = m_sLobbyMap;
						net_msg_field.nidSourceBattleMAP = m_sBattleMap;
						GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
						return;
					}
				}
			}
		}

		RLOG::LogFile( "_PVPPBG.txt", "PVPPBG::ManagerAgent::MapMoveOutPlayer char:%d failure to move map:[%d~%d] gate%d", dwCharID, sMapid.wMainID, sMapid.wSubID, dwGateID );
	}

	BOOL ManagerAgent::PCHashCheck( std::string strHash )
	{
		if ( strHash.empty() )	
			return FALSE;

		PLAYER_HASH_DATA_MAP_ITER it = m_mapPlayerHash.find( strHash );
		if ( it != m_mapPlayerHash.end() )
			return TRUE;

		return FALSE;
	}

	void ManagerAgent::PCHashInsert( std::string strHash, DWORD dwCharID )
	{
		PLAYER_HASH_DATA sData;
		sData.dwCharID = dwCharID;
		sData.strHash = strHash;

		m_mapPlayerHash.insert( std::make_pair( sData.strHash, sData ) );
	}

	void ManagerAgent::PCHashDelete( DWORD dwCharID )
	{
		PLAYER_HASH_DATA_MAP_ITER it_b = m_mapPlayerHash.begin();
		PLAYER_HASH_DATA_MAP_ITER it_e = m_mapPlayerHash.end();

		for( ; it_b != it_e; )
		{
			if ( it_b->second.dwCharID == dwCharID )
				m_mapPlayerHash.erase( it_b++ );
			else
				++it_b;
		}
	}

	void ManagerAgent::CheckForceMove( float fElapsedTime )
	{
		REGISTER_DATA_MAP_ITER iterplayer = m_mapPlayerData.begin();
		for( ; iterplayer != m_mapPlayerData.end(); ++ iterplayer )
		{
			REGISTER_DATA& splayerdata = (*iterplayer).second;

			PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( splayerdata.dwInfoCharID );
			if ( !pChar )	
				continue;

			BOOL bMoveAction = FALSE;

			// checks if player is outside the lobby and battle map
			if ( m_bForceMoveIn )
			{
				if ( pChar->m_sCurMapID != m_sLobbyMap && pChar->m_sCurMapID != m_sBattleMap )
				{
					splayerdata.fForceMoveLobbyAreaTime += fElapsedTime;

					if ( splayerdata.fForceMoveLobbyAreaTime >= m_fForceMoveInTime )
					{
						GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sLobbyMap );
						if ( plandman )
						{
							DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
							if ( pGateMan )
							{
								PDXLANDGATE pGate = pGateMan->FindLandGate ( m_dwLobbyGate );
								if ( pGate )
								{
									D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

									// move to lobby map
									GLMSG::SNETPC_PVP_PBG_A2F_MAP_MOVE net_msg_field;
									net_msg_field.nidMAP = m_sLobbyMap;
									net_msg_field.vPos = vGenPos;
									net_msg_field.bCurMapCheck = TRUE;
									net_msg_field.dwCharID = pChar->m_dwCharID;
									GLAgentServer::GetInstance().SENDTOFIELDSVR ( pChar->m_nChannel, pChar->m_dwCurFieldSvr, &net_msg_field );
								}
							}
						}

						bMoveAction = TRUE;
						splayerdata.fForceMoveLobbyAreaTime = 0.0f;
					}
				}
				else
				{
					splayerdata.fForceMoveLobbyAreaTime = 0.0f;
				}
			}

			// a movement has been made
			// other movement action is not performed
			if ( bMoveAction == TRUE )
				continue;

			// checks of player is on battle map and no party
			if ( m_bNoPartyMoveOut )
			{
				if ( pChar->m_sCurMapID == m_sBattleMap && pChar->m_dwPartyID == PARTY_NULL )
				{
					splayerdata.fForceMoveNoPartyTime += fElapsedTime;

					if ( splayerdata.fForceMoveNoPartyTime >= m_fNoPartyMoveOutTime )
					{
						GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sLobbyMap );
						if ( plandman )
						{
							DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
							if ( pGateMan )
							{
								PDXLANDGATE pGate = pGateMan->FindLandGate ( m_dwLobbyGate );
								if ( pGate )
								{
									D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

									// move to lobby map
									GLMSG::SNETPC_PVP_PBG_A2F_MAP_MOVE net_msg_field;
									net_msg_field.nidMAP = m_sLobbyMap;
									net_msg_field.vPos = vGenPos;
									net_msg_field.bCurMapCheck = TRUE;
									net_msg_field.dwCharID = pChar->m_dwCharID;
									GLAgentServer::GetInstance().SENDTOFIELDSVR ( pChar->m_nChannel, pChar->m_dwCurFieldSvr, &net_msg_field );
								}
							}
						}

						bMoveAction = TRUE;
						splayerdata.fForceMoveNoPartyTime = 0.0f;
					}
				}
				else
				{
					splayerdata.fForceMoveNoPartyTime = 0.0f;
				}
			}


			// a movement has been made
			// other movement action is not performed
			if ( bMoveAction == TRUE )
				continue;



		}
	}

	HRESULT ManagerAgent::MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
	{
		if ( !nmg )
			return E_FAIL;

		switch( nmg->nType )
		{
		case NET_MSG_GCTRL_PVP_PBG_F2A_BATTLEINFO_PC:
			{
				GLMSG::SNET_PVP_PBG_F2A_BATTLEINFO_PC* pnetmsg = ( GLMSG::SNET_PVP_PBG_F2A_BATTLEINFO_PC* ) nmg;
				RequestInfoPC( pnetmsg->dwCharID );
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_C2A_REGISTER_REQ:
			{
				GLMSG::SNETPC_PVP_PBG_C2A_REGISTER_REQ* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_C2A_REGISTER_REQ* ) nmg;
				RequestPlayerRegister(
					pnetmsg->dwCharID, 
					pnetmsg->wLevel, 
					pnetmsg->dwContri, 
					pnetmsg->bRegister, 
					pnetmsg->szHASH );
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_C2A_REJOIN_REQ:
			{
				GLMSG::SNETPC_PVP_PBG_C2A_REJOIN_REQ* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_C2A_REJOIN_REQ* ) nmg;
				PlayerRejoinBattle( pnetmsg->dwCharID );
			}
			break;

		case NET_MSG_GCTRL_PVP_PBG_F2A_RANKINFO_PC:
			{
				GLMSG::SNETPC_PVP_PBG_F2A_RANKINFO_PC* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_F2A_RANKINFO_PC* ) nmg;

				for( int i=0; i<PVP_PBG_MINI_RANKING_NUM; ++i )
					m_sLastTop[i] = PLAYER_DATA();

				for( int i=0; i<pnetmsg->wRankNum; ++i )
				{
					if ( pnetmsg->sPlayerData[i].wRank > 0 && pnetmsg->sPlayerData[i].wRank <= PVP_PBG_MINI_RANKING_NUM )
					{
						m_sLastTop[pnetmsg->sPlayerData[i].wRank-1] = pnetmsg->sPlayerData[i];
					}
				}

				GLMSG::SNETPC_PVP_PBG_A2C_RANKINFO_PC netmsgclient;
				for( int i=0; i<PVP_PBG_MINI_RANKING_NUM; ++i )
					netmsgclient.sPlayerData[i] = m_sLastTop[i];

				GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsgclient );

			}break;

		case NET_MSG_GCTRL_PVP_PBG_C2A_MAPEXIT_REQ:
			{
				GLMSG::SNETPC_PVP_PBG_C2A_MAPEXIT_REQ* pnetmsg = ( GLMSG::SNETPC_PVP_PBG_C2A_MAPEXIT_REQ* ) nmg;
				MapExit( pnetmsg->dwCharID );
			}break;

		};


		return S_OK;
	}

	void ManagerAgent::RequestInfoPC( DWORD dwCharID )
	{
		PGLCHARAG pChar = GLAgentServer::GetInstance().GetCharID( dwCharID );
		if ( !pChar )	
			return;

		//event info
		GLMSG::SNET_PVP_PBG_A2C_BATTLEINFO_PC netmsgclient;
		netmsgclient.emState = m_emState;
		netmsgclient.fRemain = m_fRemain;
		netmsgclient.sScheduleNext = m_sScheduleNext;
		netmsgclient.wLevelReq = m_wLevelReq;
		netmsgclient.dwContriReq = m_dwContriReq;
		netmsgclient.wPlayerLimit = m_wPlayerLimit;
		netmsgclient.wBattleTime = m_wBattleTime;

		//register info
		REGISTER_DATA* pPlayerData = PlayerDataGet( dwCharID );
		if ( pPlayerData )
		{
			netmsgclient.bRegistered = TRUE;
		}

		REGISTER_DATA_QUEUE* pPlayerQueue = PlayerQueueGet( dwCharID );
		if ( pPlayerQueue )
		{
			netmsgclient.bRegistered = TRUE;
			netmsgclient.bQueued = TRUE;
			netmsgclient.wQueueNum = pPlayerQueue->wQueueNum;
		}

		GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &netmsgclient );

		GLMSG::SNETPC_PVP_PBG_A2C_RANKINFO_PC netmsgclient2;
		for( int i=0; i<PVP_PBG_MINI_RANKING_NUM; ++i )
			netmsgclient2.sPlayerData[i] = m_sLastTop[i];

		GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsgclient2 );
	}

	void ManagerAgent::RequestPlayerRegister( DWORD dwCharID, WORD wLevel, DWORD dwContri, BOOL bRegister, std::string strHash )
	{	
		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
		if ( !pchar )	
			return;

		GLMSG::SNETPC_PVP_PBG_A2C_REGISTER_FB netmsgfb;

		//invalid charid
		if ( pchar->m_dwCharID != dwCharID )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_INVALID_CHARID;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		//registration not running
		if ( m_emState != PVPPBG::EVENT_STATE_REGISTER )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_NOTPOSSIBLE;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		//register only available in channel 0 
		if ( pchar->m_nChannel != 0 )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_INVALID_CHANNEL;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		//player level too low
		if ( wLevel < m_wLevelReq )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_LEVEL_LOW;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		//contri low
		if ( dwContri < m_dwContriReq )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_CONTRI_LOW;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		if ( bRegister )
		{		
			PlayerRegister( pchar->m_dwCharID, strHash );

			REGISTER_DATA* pdataregchk = PlayerDataGet( dwCharID );
			REGISTER_DATA_QUEUE* pdataqueuechk = PlayerQueueGet( dwCharID );
			if ( !pdataregchk && !pdataqueuechk )
			{
				CDebugSet::ToFileWithTime( "_PVPPBG.txt", "RequestPlayerRegister failed to register [%d]'%s'", 
					pchar->m_dwCharID, pchar->m_szName );
			}
		}
		else
		{
			CancelRegister( pchar->m_dwCharID );

			REGISTER_DATA* pdataregchk = PlayerDataGet( dwCharID );
			REGISTER_DATA_QUEUE* pdataqueuechk = PlayerQueueGet( dwCharID );
			if ( pdataregchk || pdataqueuechk )
			{
				CDebugSet::ToFileWithTime( "_PVPPBG.txt", "RequestPlayerRegister failed to unregister [%d]'%s'", 
					pchar->m_dwCharID, pchar->m_szName );
			}
		}
	}
	
	void ManagerAgent::PlayerRegister( DWORD dwCharID, std::string strHash )
	{
		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
		if ( !pchar )	
			return;

		GLMSG::SNETPC_PVP_PBG_A2C_REGISTER_FB netmsgfb;

 		if ( m_bHashCheck )
 		{
 			//hash check enabled, hash should not be empty
 			if ( strHash.empty() )
 			{
 				netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_DEVICE_INFO_ERROR;
 				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
 				return;
 			}
 
 			//hash already used
 			if ( PCHashCheck( strHash ) )
 			{
 				netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_DEVICE_REGISTERED;
 				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
 				return;
 			}
 		}

		//player already registered
		if ( IsPlayerReg( pchar->m_dwCharID ) )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_ALREADY_REG;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		//player already queued
		if ( IsPlayerQueue( pchar->m_dwCharID ) )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_ALREADY_QUEUE;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		if ( IsPlayerFull() )
		{
			//player map is full insert to queue instead
			BOOL binsert = InsertQueue( pchar->m_dwCharID, pchar->m_szName, CharClassToIndex( pchar->m_emClass ), pchar->m_wSchool );
			if ( binsert )
			{
				netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_QUEUED;
				netmsgfb.wQueueNum = (WORD)m_vecPlayerDataQueue.size();
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

				if ( m_bHashCheck )
					PCHashInsert( strHash, pchar->m_dwCharID );

				return;
			}
		}
		else
		{
			//insert player data
			BOOL binsert = InsertRegister( pchar->m_dwCharID, pchar->m_szName, CharClassToIndex( pchar->m_emClass ), pchar->m_wSchool );
			if ( binsert )
			{
				netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_REGISTERED;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );

				if ( m_bHashCheck )
					PCHashInsert( strHash, pchar->m_dwCharID );

				return;
			}
		}

		netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_FAILED;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
	}

	void ManagerAgent::CancelRegister( DWORD dwCharID )
	{
		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
		if ( !pchar )	return;

		GLMSG::SNETPC_PVP_PBG_A2C_REGISTER_FB netmsgfb;

		//if exist in registered, remove
		BOOL bdelete = DeleteRegister( pchar->m_dwCharID );
		if ( bdelete )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_CANCEL_REG;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			ProcessQueueData();
			UpdateQueuePosition();

			if ( m_bHashCheck )
				PCHashDelete( pchar->m_dwCharID );

			return;
		}

		//not exist in registered, must be in queue vector
		bdelete = DeleteQueue( pchar->m_dwCharID );
		if ( bdelete )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_CANCEL_REG;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			ProcessQueueData();
			UpdateQueuePosition();

			if ( m_bHashCheck )
				PCHashDelete( pchar->m_dwCharID );

			return;
		}

		//fail to delete
		if ( !bdelete )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REGISTER_FB_CANCEL_FAIL;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}
	}

	void ManagerAgent::RequestPlayerCancelRegister( DWORD dwCharID )
	{	
		if ( dwCharID == PVP_PBG_CHAR_ID_NULL )	
			return;

		if ( m_emState != PVPPBG::EVENT_STATE_REGISTER )	
			return;

		REGISTER_DATA* pdatareg = PlayerDataGet( dwCharID );
		if ( pdatareg )
		{
			WORD wplayerschool = pdatareg->wInfoSchool;

			if ( DeleteRegister( dwCharID ) )
			{
				ProcessQueueData();
				UpdateQueuePosition();

				if ( m_bHashCheck )
					PCHashDelete( dwCharID );	
			}
		}

		REGISTER_DATA_QUEUE* pdataqueue = PlayerQueueGet( dwCharID );
		if ( pdataqueue )
		{
			WORD wplayerschool = pdataqueue->sPlayerData.wInfoSchool;

			if ( DeleteQueue( dwCharID ) )
			{
				ProcessQueueData();
				UpdateQueuePosition();

				if ( m_bHashCheck )
					PCHashDelete( dwCharID );
			}
		}


		REGISTER_DATA* pdataregchk = PlayerDataGet( dwCharID );
		if ( pdataregchk )
		{
			CDebugSet::ToFileWithTime( "_PVPPBG.txt", "RequestPlayerCancelRegister failed to delete registered [%d]'%s'", 
				pdataregchk->dwInfoCharID, pdataregchk->szInfoCharName );
		}

		REGISTER_DATA_QUEUE* pdataqueuechk = PlayerQueueGet( dwCharID );
		if ( pdataqueuechk )
		{
			CDebugSet::ToFileWithTime( "_PVPPBG.txt", "RequestPlayerCancelRegister failed to delete queued [%d]'%s'", 
				pdataqueuechk->sPlayerData.dwInfoCharID, pdataqueuechk->sPlayerData.szInfoCharName );
		}

		return;
	}

	void ManagerAgent::PlayerRejoinBattle( DWORD dwCharID )
	{
		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
		if ( !pchar )	return;

		GLMSG::SNETPC_PVP_PBG_A2C_REJOIN_FB netmsgfb;

		//can only rejoin if battle running
		if ( m_emState != PVPPBG::EVENT_STATE_BATTLE )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REJOIN_FB_NOTPOSSIBLE;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		//already inside map no need to rejoin
		if ( pchar->m_sCurMapID == m_sBattleMap || pchar->m_sCurMapID == m_sLobbyMap )
		{
			netmsgfb.emFB = GLMSG::PVP_PBG_REJOIN_FB_INBATTLE;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		REGISTER_DATA* pplayerdata = PlayerDataGet( pchar->m_dwCharID );
		if ( !pplayerdata )
		{	
			//not registered
			netmsgfb.emFB = GLMSG::PVP_PBG_REJOIN_FB_NOT_REGISTERED;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgfb );
			return;
		}

		GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sLobbyMap );
		if ( plandman )
		{
			DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
			if ( pGateMan )
			{
				PDXLANDGATE pGate = pGateMan->FindLandGate ( m_dwLobbyGate );
				if ( pGate )
				{
					D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

					GLMSG::SNETPC_PVP_PBG_A2F_MAP_MOVE net_msg_field;
					net_msg_field.nidMAP = m_sLobbyMap;
					net_msg_field.vPos = vGenPos;
					net_msg_field.bCurMapCheck = TRUE;
					net_msg_field.dwCharID = pchar->m_dwCharID;
					GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
					return;
				}
			}
		}

		RLOG::LogFile( "_PVPPBG.txt", "PlayerRejoinBattle failed  [%d][%s] map[%d~%d] gate[%d]", 
			pplayerdata->dwInfoCharID, 
			pplayerdata->szInfoCharName, 
			m_sLobbyMap.wMainID, 
			m_sLobbyMap.wSubID, 
			m_dwLobbyGate );
	}

	void ManagerAgent::MapExit( DWORD dwCharID )
	{
		if ( m_emState != PVPPBG::EVENT_STATE_REWARD && m_emState != PVPPBG::EVENT_STATE_ENDED )
			return;

		PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
		if ( !pchar )	
			return;

		REGISTER_DATA* pPlayerData = PlayerDataGet( pchar->m_dwCharID );
		if ( !pPlayerData )
			return;

		GLAGLandMan* plandman = GLAgentServer::GetInstance().GetByMapID ( m_sOutMap );
		if ( plandman )
		{
			DxLandGateMan* pGateMan = plandman->GetLandGateMan ();
			if ( pGateMan )
			{
				PDXLANDGATE pGate = pGateMan->FindLandGate ( m_dwOutGate );
				if ( pGate )
				{
					D3DXVECTOR3 vGenPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );

					GLMSG::SNETPC_PVP_PBG_A2F_MAP_MOVE net_msg_field;
					net_msg_field.nidMAP = m_sOutMap;
					net_msg_field.vPos = vGenPos;
					net_msg_field.bCurMapCheck = TRUE;
					net_msg_field.dwCharID = pchar->m_dwCharID;
					GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &net_msg_field );
					return;
				}
			}
		}

		CDebugSet::ToFileWithTime( "_PVPPBG.txt", "PVPPBG::ManagerAgent MapExit failed  [%d][%s] map[%d~%d] gate[%d]", 
			pPlayerData->dwInfoCharID, pPlayerData->szInfoCharName, m_sOutMap.wMainID, m_sOutMap.wSubID, m_dwOutGate );
	}

}