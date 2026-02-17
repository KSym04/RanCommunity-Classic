#include "stdafx.h"
#include "./GLChar.h"
#include "./GLGaeaServer.h"
#include "./GLItemMan.h"
#include "./GLBattlePass.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*12-9-14, Battle Pass - CNDev*/

void GLChar::BattlePassComplete( SBATTLEPASS_CHAR_DATA& sbattlepass_char_data )
{
	SBATTLEPASS_FILE_DATA* pbattlepass_file_data = GLBattlePass::GetInstance().GetBattlePass( sbattlepass_char_data.dwBattlePassID );
	if ( !pbattlepass_file_data )	return;

	m_mapBattlePassDone.insert( std::make_pair( sbattlepass_char_data.dwBattlePassID, sbattlepass_char_data ) );

	GLMSG::SNETPC_BATTLEPASS_COMPLETE net_msg_client;
	net_msg_client.sData = sbattlepass_char_data;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );

	GLMSG::SNETPC_BATTLEPASS_COMPLETE_BRD net_msg_client_brd;
	net_msg_client_brd.dwGaeaID = m_dwGaeaID;
	SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&net_msg_client_brd) );

	GLMSG::SNETPC_BATTLEPASS_NOTIFY_CLIENT net_msg_client_notify;
	StringCchCopy( net_msg_client_notify.szCharacterName, CHAR_SZNAME+1, m_szName );
	StringCchCopy( net_msg_client_notify.szBattlePassTitle, BATTLEPASS_TITLE_MSG_SIZE+1, pbattlepass_file_data->strBattlePassTitle.c_str() );
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_notify );

	m_dwBattlePassEXP += pbattlepass_file_data->dwRewardPoint;

	GLMSG::SNETPC_UPDATE_BATTLEPASS_EXP net_msg_client_update;
	net_msg_client_update.dwEXP = m_dwBattlePassEXP;
	net_msg_client_update.bNotice = TRUE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_update );

	while (m_dwBattlePassLevel < BATTLEPASS_LEVEL_MAX - 1 && m_dwBattlePassEXP >= GLCONST_CHAR::dwBattlePassEXP[m_dwBattlePassLevel])
	{
		m_dwBattlePassLevel++;

		GLMSG::SNETPC_UPDATE_BATTLEPASS_LEVEL net_msg_client_level;
		net_msg_client_level.dwLevel = m_dwBattlePassLevel;
		net_msg_client_level.bNotice = TRUE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_level );
	}
}

void GLChar::DoBattlePassMobKill( SNATIVEID sidMob )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_KILL_MOB )	continue;
		if ( sbattlepass_char_data.sidProgress != sidMob )				continue;

		sbattlepass_char_data.dwProgressNow++;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassMapKill( SNATIVEID sidMap )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_KILL_PLAYER )	continue;
		if ( sbattlepass_char_data.sidProgress != sidMap )					continue;

		sbattlepass_char_data.dwProgressNow++;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassMapReach( SNATIVEID sidMap )
{
	if ( !RPARAM::bFeatureBattlePass )	return;
	if ( sidMap == NATIVEID_NULL() )	return;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_REACH_MAP )	continue;
		if ( sbattlepass_char_data.sidProgress != sidMap )				continue;

		sbattlepass_char_data.dwProgressNow++;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassTakeItem( SNATIVEID sidItem, WORD wTurnNum )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	if ( wTurnNum == 0 )	wTurnNum = 1;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_TAKE_ITEM )	continue;
		if ( sbattlepass_char_data.sidProgress != sidItem )				continue;

		sbattlepass_char_data.dwProgressNow += wTurnNum;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassUseItem( SNATIVEID sidItem, WORD wUseNum )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	if ( wUseNum == 0 )	wUseNum = 1;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_USE_ITEM )	continue;
		if ( sbattlepass_char_data.sidProgress != sidItem )				continue;

		sbattlepass_char_data.dwProgressNow += wUseNum;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassTradePlayer( WORD wNum )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	if ( wNum == 0 )	wNum = 1;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_TRADE_PLAYER )	continue;

		sbattlepass_char_data.dwProgressNow += wNum;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassWhisperPlayer( WORD wNum )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	if ( wNum == 0 )	wNum = 1;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_WHISPER_PLAYER )	continue;

		sbattlepass_char_data.dwProgressNow += wNum;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassPrivateMarket( WORD wNum )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	if ( wNum == 0 )	wNum = 1;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_PRIVATE_MARKET )	continue;

		sbattlepass_char_data.dwProgressNow += wNum;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassGainPlayTime( WORD wNum )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	if ( wNum == 0 )	wNum = 1;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_GAIN_PLAYTIME )	continue;

		sbattlepass_char_data.dwProgressNow += wNum;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassGainGold( WORD wNum )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	if ( wNum == 0 )	wNum = 1;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_GAIN_GOLD )	continue;

		sbattlepass_char_data.dwProgressNow += wNum;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}

void GLChar::DoBattlePassGainContributionPoint( WORD wNum )
{
	if ( !RPARAM::bFeatureBattlePass )	return;

	if ( wNum == 0 )	wNum = 1;

	std::vector<DWORD> vecComplete;

	for( SBATTLEPASS_CHAR_DATA_MAP_ITER iter_battlepass_prog = m_mapBattlePassProg.begin();
		iter_battlepass_prog != m_mapBattlePassProg.end(); ++ iter_battlepass_prog )
	{
		SBATTLEPASS_CHAR_DATA& sbattlepass_char_data = (*iter_battlepass_prog).second;
		if ( sbattlepass_char_data.emType != EMBATTLEPASS_TYPE_GAIN_CONTRIPOINT )	continue;

		sbattlepass_char_data.dwProgressNow += wNum;

		if ( sbattlepass_char_data.dwProgressNow >= sbattlepass_char_data.dwProgressMax )
		{
			sbattlepass_char_data.dwProgressNow = sbattlepass_char_data.dwProgressMax;
			vecComplete.push_back( sbattlepass_char_data.dwBattlePassID );
		}
		else
		{
			GLMSG::SNETPC_BATTLEPASS_UPDATE net_msg_client;
			net_msg_client.sData = sbattlepass_char_data;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client );
		}
	}

	for( int i=0; i<(int)vecComplete.size(); ++i )
	{
		DWORD dwID = vecComplete[i];

		SBATTLEPASS_CHAR_DATA_MAP_ITER pos = m_mapBattlePassProg.find( dwID );
		if ( pos != m_mapBattlePassProg.end() )
		{
			SBATTLEPASS_CHAR_DATA sbattlepass_char_data = (*pos).second;

			BattlePassComplete( sbattlepass_char_data );

			m_mapBattlePassProg.erase ( pos );
		}
	}
}