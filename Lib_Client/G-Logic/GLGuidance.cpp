#include "StdAfx.h"

#include <algorithm>
//#include <strstream>
#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "../../Lib_Engine/Common/IniLoader.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"
#include "./GLGuidance.h"

#include "../DbActionLogic.h"
#include "./GLLandMan.h"
#include "./GLGaeaServer.h"
#include "./GLAgentServer.h"
#include "./GLChar.h"
#include "./GLCrow.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLGuidance::GLGuidance () :
	m_dwID(UINT_MAX),
	
	m_dwClubMap(0),
	m_dwCLubMapGate(0),
	m_dwClubHallMap(0),

	m_fItemDropRate(0),
	m_fDefaultCommissionPer(5),

	m_dwGuidanceClub(CLUB_NULL),
	m_fCommissionPercent(0.0f),

	m_dwBattleTime(3600),

	m_bNewCommission(false),
	m_fCommissionTimer(0.0f),
	m_fNewCommissionPercent(0.0f),
	
	m_bBattle(false),
	m_fBattleTimer(0.0f),

	m_dwCERTIFY_CHARID(0),
	m_dwCERTIFY_NPCID(0),
	m_fCERTIFY_TIMER(0),
	m_vCERTIFY_POS(0,0,0),

	m_bBattleEndCheck(false),
	m_fCHECK_TIMER(0.0f),
	m_fCHECK_TIME(0.0f),

	m_fScoreKill(1.0f),
	m_fScoreResu(1.0f),
	m_fPointScore(1.0f),
	m_fPointWinMainBonus(1.0f),
	m_fPointWinAllyBonus(0.5f),
	m_wMinimumScore(0),

	m_fRemainTimer(0),
	m_fTimer(0)
{
	/*clubwar map lock, Juver, 2017/06/27 */
	for( int i=0; i<MAX_MAP_LOCK; ++i )
	{
		m_dwMapLock[i] = NATIVEID_NULL().dwID;
	}
}

GLGuidance& GLGuidance::operator= ( const GLGuidance& value )
{
	m_dwID				= value.m_dwID;
	m_strName			= value.m_strName;
	m_vecMaps			= value.m_vecMaps;

	m_dwClubMap			= value.m_dwClubMap;
	m_dwCLubMapGate		= value.m_dwCLubMapGate;
	m_dwClubHallMap		= value.m_dwClubHallMap;

	m_dwBattleTime		= value.m_dwBattleTime;

	m_vecBattleSchedule = value.m_vecBattleSchedule;

	m_fRemainTimer		= value.m_fRemainTimer;
	m_fTimer			= value.m_fTimer;

	m_vecAwardMob.clear();
	for ( size_t n=0; n<value.m_vecAwardMob.size(); ++n )
	{
		m_vecAwardMob.push_back( value.m_vecAwardMob[n] );
	}
	
	m_fItemDropRate			= value.m_fItemDropRate;
	m_fDefaultCommissionPer = value.m_fDefaultCommissionPer;

	m_dwGuidanceClub		= value.m_dwGuidanceClub;
	m_fCommissionPercent	= value.m_fCommissionPercent;

	m_bNewCommission		= value.m_bNewCommission;
	m_fCommissionTimer		= value.m_fCommissionTimer;
	m_fNewCommissionPercent = value.m_fNewCommissionPercent;

	m_bBattle				= value.m_bBattle;
	m_fBattleTimer			= value.m_fBattleTimer;

	m_dwCERTIFY_CHARID		= value.m_dwCERTIFY_CHARID;
	m_dwCERTIFY_NPCID		= value.m_dwCERTIFY_NPCID;
	m_fCERTIFY_TIMER		= value.m_fCERTIFY_TIMER;
	m_vCERTIFY_POS			= value.m_vCERTIFY_POS;

	m_bBattleEndCheck		= value.m_bBattleEndCheck;
	m_fCHECK_TIMER			= value.m_fCHECK_TIMER;
	m_fCHECK_TIME			= value.m_fCHECK_TIME;

	/*clubwar map lock, Juver, 2017/06/27 */
	for( int i=0; i<MAX_MAP_LOCK; ++i )
	{
		m_dwMapLock[i]		= value.m_dwMapLock[i];
	}

	m_mapPlayerData			= value.m_mapPlayerData;

	m_fScoreKill			= value.m_fScoreKill;
	m_fScoreResu			= value.m_fScoreResu;
	m_fPointScore			= value.m_fPointScore;
	m_fPointWinMainBonus	= value.m_fPointWinMainBonus;
	m_fPointWinAllyBonus	= value.m_fPointWinAllyBonus;
	m_wMinimumScore			= value.m_wMinimumScore;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	m_mapClubWarClubScore	= value.m_mapClubWarClubScore;
	m_mapClubWarScore		= value.m_mapClubWarScore;
	m_mapClubWarScoreResu	= value.m_mapClubWarScoreResu;
	m_sCWRankingReward		= value.m_sCWRankingReward;
	m_sCWRankingResuReward	= value.m_sCWRankingResuReward;
	m_fCWRankingUpdate		= value.m_fCWRankingUpdate;
	return *this;
}

bool GLGuidance::Load ( std::string strFile )
{
	if( strFile.empty() )	return FALSE;

	std::string strPath;
	strPath = GLOGIC::GetServerPath ();
	strPath += strFile;

	CIniLoader cFILE;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_SERVER_ZIPFILE );

	if( !cFILE.open ( strPath, true ) )
	{
		CDebugSet::ToLogFile ( "ERROR : GLGuidance::Load(), File Open %s", strFile.c_str() );
		return false;
	}
	
	cFILE.getflag( "GUIDANCE", "ID", 0, 1, m_dwID );
	cFILE.getflag( "GUIDANCE", "NAME", 0, 1, m_strName );

	SNATIVEID nidMAP;
	cFILE.getflag( "GUIDANCE", "CLUB_MAP", 0, 2, nidMAP.wMainID );
	cFILE.getflag( "GUIDANCE", "CLUB_MAP", 1, 2, nidMAP.wSubID );
	m_dwClubMap = nidMAP.dwID;

	cFILE.getflag( "GUIDANCE", "CLUB_GATE", 0, 1, m_dwCLubMapGate );

	cFILE.getflag( "GUIDANCE", "CLUB_HALL_MAP", 0, 2, nidMAP.wMainID );
	cFILE.getflag( "GUIDANCE", "CLUB_HALL_MAP", 1, 2, nidMAP.wSubID );
	m_dwClubHallMap = nidMAP.dwID;

	cFILE.getflag( "GUIDANCE", "COMMISSION", 0, 1, m_fCommissionPercent );
	cFILE.getflag( "GUIDANCE", "ITEM_DROPRATE", 0, 1, m_fItemDropRate );

	DWORD dwNUM = cFILE.GetKeySize( "GUIDANCE", "BATTLE_TIME" );
	if( dwNUM > MAX_CLUB_WAR_SCHEDULE )
	{
		CDebugSet::ToLogFile( "ERROR : GLGuidance::Load(), %s, It was wrong size of BATTLE_TIME", strFile.c_str() );
		return false;
	}

	DWORD dwNumSchedule = cFILE.GetKeySize( "GUIDANCE", "BATTLE_TIME" );
	for( DWORD i=0; i<dwNumSchedule; ++i )
	{
		SBATTLE_SCHEDULE sSchedule;
		cFILE.getflag( i, "GUIDANCE", "BATTLE_TIME", 0, 3, sSchedule.nDay );
		cFILE.getflag( i, "GUIDANCE", "BATTLE_TIME", 1, 3, sSchedule.nHour );
		cFILE.getflag( i, "GUIDANCE", "BATTLE_TIME", 2, 3, sSchedule.nMinute );
		m_vecBattleSchedule.push_back( sSchedule );
	}

	/*clubwar map lock, Juver, 2017/06/27 */
	DWORD dwnummaplock = cFILE.GetKeySize( "GUIDANCE", "MAP_LOCK" );
	for( DWORD i=0; i<dwnummaplock; ++i )
	{
		SNATIVEID sidmaplock;
		cFILE.getflag( i, "GUIDANCE", "MAP_LOCK", 0, 2, sidmaplock.wMainID );
		cFILE.getflag( i, "GUIDANCE", "MAP_LOCK", 1, 2, sidmaplock.wSubID );

		if ( i >= MAX_MAP_LOCK )	continue;
		m_dwMapLock[i] = sidmaplock.dwID;
	}


	cFILE.getflag( "GUIDANCE", "BATTLE_THE_TIME", 0, 1, m_dwBattleTime );

	m_vecMaps.clear ();

	dwNUM = cFILE.GetKeySize( "GUIDANCE", "GUIDANCE_MAP" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		cFILE.getflag( i, "GUIDANCE", "GUIDANCE_MAP", 0, 2, nidMAP.wMainID );
		cFILE.getflag( i, "GUIDANCE", "GUIDANCE_MAP", 1, 2, nidMAP.wSubID );

		m_vecMaps.push_back ( nidMAP.dwID );
	}

	m_vecAwardMob.clear();

	dwNUM = cFILE.GetKeySize( "GUIDANCE", "AWARD_MOB" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		SAWARD_MOB sAwardMob;

		cFILE.getflag( i, "GUIDANCE", "AWARD_MOB", 0, 4, sAwardMob.nidGenMob.wMainID );
		cFILE.getflag( i, "GUIDANCE", "AWARD_MOB", 1, 4, sAwardMob.nidGenMob.wSubID );
		cFILE.getflag( i, "GUIDANCE", "AWARD_MOB", 2, 4, sAwardMob.wGenPosX );
		cFILE.getflag( i, "GUIDANCE", "AWARD_MOB", 3, 4, sAwardMob.wGenPosY );

		m_vecAwardMob.push_back( sAwardMob );
	}

	cFILE.getflag( "GUIDANCE", "SCORE_KILL", 0, 1, m_fScoreKill );
	cFILE.getflag( "GUIDANCE", "SCORE_RESU", 0, 1, m_fScoreResu );
	cFILE.getflag( "GUIDANCE", "POINT_SCORE", 0, 1, m_fPointScore );
	cFILE.getflag( "GUIDANCE", "POINT_WIN_MAIN_BONUS", 0, 1, m_fPointWinMainBonus );
	cFILE.getflag( "GUIDANCE", "POINT_WIN_ALLY_BONUS", 0, 1, m_fPointWinAllyBonus );
	cFILE.getflag( "GUIDANCE", "MIN_SCORE", 0, 1, m_wMinimumScore );

	return true;
}

bool GLGuidance::IsNewCommission ()
{
	return m_bNewCommission;
}

bool GLGuidance::IsGuidClub ( DWORD dwClubID )
{
	if ( m_dwGuidanceClub==CLUB_NULL )
	{
		return false;
	}

	if (m_dwGuidanceClub==dwClubID)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GLGuidance::IsBattle ()
{
	return m_bBattle;
}

SBATTLE_SCHEDULE* GLGuidance::GetStartBattle(int nDayOfWeek, int nHour, int nMinute)
{
	for ( size_t i=0; i<m_vecBattleSchedule.size(); ++i )
	{
		//finds the matching schedule
		if ( m_vecBattleSchedule[i].nDay == nDayOfWeek && 
			m_vecBattleSchedule[i].nHour == nHour && 
			m_vecBattleSchedule[i].nMinute == nMinute )
		{
			return &m_vecBattleSchedule[i];
		}
	}

	return NULL;
}

// 선도클럽 결정전 남은 시간을 클라이언트에게 알린다.
void GLGuidance::UpdateNotifyBattle ()
{
	CTime cCurTime = CTime::GetCurrentTime();
	int nCurDay = cCurTime.GetDayOfWeek();
	int nCurHour = cCurTime.GetHour();
	int nCurMinute = cCurTime.GetMinute();
	
	for( size_t i=0; i<m_vecBattleSchedule.size(); ++i )
	{
		SBATTLE_SCHEDULE& sBattleSchedule = m_vecBattleSchedule[i];

		if ( sBattleSchedule.nDay == 0 )
			continue;

		int nDayIterval = sBattleSchedule.nDay - nCurDay;
		if ( nDayIterval < 0 )
			nDayIterval += 7; //if current day is more than the schedule day then the schedule is next week

		int nHourInterval = sBattleSchedule.nHour - nCurHour;
		int nMinuteInterval = sBattleSchedule.nMinute - nCurMinute;


		int nInterval = nDayIterval * 24 * 60;
		nInterval += nHourInterval * 60;
		nInterval += nMinuteInterval;



		if ( nInterval == 60 )
		{
			if ( sBattleSchedule.bNotify1Hour == false )
			{
				sBattleSchedule.bNotify1Hour = true;

				GLMSG::SNET_CLUB_BATTLE_START_BRD NetMsgBrd;
				NetMsgBrd.dwID = m_dwID;
				NetMsgBrd.nTIME = 60;
				StringCchCopy ( NetMsgBrd.szName, GLMSG::SNET_CLUB_BATTLE_START_BRD::TEXT_LEN, m_strName.c_str() );
				GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );

				//CDebugSet::ToFileWithTime( "_cw_test.txt", "broadcast upcoming battle id:%d min:%d d:%d h:%d m:%d", 
				//	m_dwID, nInterval, sBattleSchedule.nDay, sBattleSchedule.nHour, sBattleSchedule.nMinute );
			}
		}
		else if ( nInterval == 30 )
		{
			if ( sBattleSchedule.bNotifyHalfHour == false )
			{
				sBattleSchedule.bNotifyHalfHour = true;

				GLMSG::SNET_CLUB_BATTLE_START_BRD NetMsgBrd;
				NetMsgBrd.dwID = m_dwID;
				NetMsgBrd.nTIME = 30;
				StringCchCopy ( NetMsgBrd.szName, GLMSG::SNET_CLUB_BATTLE_START_BRD::TEXT_LEN, m_strName.c_str() );
				GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );

				//CDebugSet::ToFileWithTime( "_cw_test.txt", "broadcast upcoming battle id:%d min:%d d:%d h:%d m:%d", 
				//	m_dwID, nInterval, sBattleSchedule.nDay, sBattleSchedule.nHour, sBattleSchedule.nMinute );
			}
		}
		else if ( nInterval == 10 )
		{
			if ( sBattleSchedule.bNotify10Minute == false )
			{
				sBattleSchedule.bNotify10Minute = true;

				GLMSG::SNET_CLUB_BATTLE_START_BRD NetMsgBrd;
				NetMsgBrd.dwID = m_dwID;
				NetMsgBrd.nTIME = 10;
				StringCchCopy ( NetMsgBrd.szName, GLMSG::SNET_CLUB_BATTLE_START_BRD::TEXT_LEN, m_strName.c_str() );
				GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );

				//CDebugSet::ToFileWithTime( "_cw_test.txt", "broadcast upcoming battle id:%d min:%d d:%d h:%d m:%d", 
				//	m_dwID, nInterval, sBattleSchedule.nDay, sBattleSchedule.nHour, sBattleSchedule.nMinute );
			}
		}
	}
}

void GLGuidance::UpdateCommission ( float fElaps )
{
	m_fCommissionTimer += fElaps;
	
	if ( m_fCommissionTimer > 600.0f )
	{
		//	Note : 수수료 변경.
		ChangeCommission ( m_fNewCommissionPercent );

		//	Note : 수수료 변경 필드에 알림.
		GLMSG::SNET_CLUB_GUID_COMMISSION_FLD NetMsgCommissionFld;
		NetMsgCommissionFld.dwID = m_dwID;
		NetMsgCommissionFld.fCommission = m_fCommissionPercent;
		GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgCommissionFld );

		//	Note : 클라이언트들에게 알림.
		GLMSG::SNET_CLUB_GUID_COMMISSION_BRD NetMsgBrd;
		NetMsgBrd.fCommission = m_fCommissionPercent;

		std::vector<DWORD>::size_type nSize = m_vecMaps.size();
		for ( std::vector<DWORD>::size_type i=0; i<nSize; ++i )
		{
			SNATIVEID nidMAP = (m_vecMaps[i]);
			GLAGLandMan *pLand = GLAgentServer::GetInstance().GetByMapID ( nidMAP );
			if ( !pLand )	continue;

			pLand->SENDTOALLCLIENT ( &NetMsgBrd );
		}

		//	Note : 리셋.
		m_bNewCommission = false;
		m_fCommissionTimer = 0;
		m_fNewCommissionPercent = 0;
	}
}

// 선도클럽 결정전을 시작한다.
void GLGuidance::DoBattleStart ()
{
	//	Note : 결정전을 시작시킴.
	m_bBattle = true;

	//	Note : 선도 클럽 변경을 필드서버에 알림.
	//
	GLMSG::SNET_CLUB_CERTIFIED_FLD NetMsgGuidFld;
	NetMsgGuidFld.dwID = m_dwID;
	NetMsgGuidFld.dwCLUBID = m_dwGuidanceClub;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgGuidFld );

	//	Note : 수수료 변경 필드에 알림.
	GLMSG::SNET_CLUB_GUID_COMMISSION_FLD NetMsgCommissionFld;
	NetMsgCommissionFld.dwID = m_dwID;
	NetMsgCommissionFld.fCommission = m_fCommissionPercent;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgCommissionFld );

	//	Note : 클라이언트들에게 알림.
	GLMSG::SNET_CLUB_GUID_COMMISSION_BRD NetMsgCmBrd;
	NetMsgCmBrd.fCommission = m_fCommissionPercent;

	std::vector<DWORD>::size_type nSize = m_vecMaps.size();
	for ( std::vector<DWORD>::size_type i=0; i<nSize; ++i )
	{
		SNATIVEID nidMAP = (m_vecMaps[i]);
		GLAGLandMan *pLand = GLAgentServer::GetInstance().GetByMapID ( nidMAP );
		if ( !pLand )	continue;

		pLand->SENDTOALLCLIENT ( &NetMsgCmBrd );
	}

	//	Note : 전투 시작을 알림.
	GLMSG::SNET_CLUB_BATTLE_START_FLD NetMsgFld;
	NetMsgFld.dwID = m_dwID;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &NetMsgFld, 0 );

	//	Note : 모든 클라이언트에 알림.
	GLMSG::SNET_CLUB_BATTLE_START_BRD NetMsgBrd;
	NetMsgBrd.nTIME = 0;
	NetMsgBrd.dwID = m_dwID;
	StringCchCopy ( NetMsgBrd.szName, GLMSG::SNET_CLUB_BATTLE_START_BRD::TEXT_LEN, m_strName.c_str() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );


	/*clubwar map lock, Juver, 2017/06/27 */
	//lock maps at event start (agent server)
	for( int i=0; i<GLGuidance::MAX_MAP_LOCK; ++i )
	{
		if ( m_dwMapLock[i] != NATIVEID_NULL().dwID )
		{
			GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID( SNATIVEID(m_dwMapLock[i]) );
			if ( pLandMan )
				pLandMan->m_bClubWarLock = true;
		}
	}

	m_mapPlayerData.clear();
}

void GLGuidance::DoBattleEnd()
{
	//	Note : 결정전이 종료.
	m_bBattle = false;

	m_fBattleTimer = 0.0f;

	//	Note : 결정전 종료를 알림.
	//
	GLCLUB *pCLUB = GLAgentServer::GetInstance().GetClubMan().GetClub(m_dwGuidanceClub);

	std::string strClubName;
	if ( pCLUB )	strClubName = pCLUB->m_szName;

	//	Note : 전투 종료를 알림.
	GLMSG::SNET_CLUB_BATTLE_END_FLD NetMsgFld;
	NetMsgFld.dwID = m_dwID;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &NetMsgFld, 0 );

	//	Note : 모든 클라이언트에 알림.
	GLMSG::SNET_CLUB_BATTLE_END_BRD NetMsgBrd;
	NetMsgBrd.dwID = m_dwID;
	StringCchCopy ( NetMsgBrd.szName, GLMSG::SNET_CLUB_BATTLE_END_BRD::TEXT_LEN, m_strName.c_str() );
	StringCchCopy ( NetMsgBrd.szClubName, CHAR_SZNAME, strClubName.c_str() );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );


	/*clubwar map lock, Juver, 2017/06/27 */
	//unlock maps at event end (agent server)
	for( int i=0; i<GLGuidance::MAX_MAP_LOCK; ++i )
	{
		if ( m_dwMapLock[i] != NATIVEID_NULL().dwID )
		{
			GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID( SNATIVEID(m_dwMapLock[i]) );
			if ( pLandMan )
				pLandMan->m_bClubWarLock = false;
		}
	}

	if ( RPARAM::bClubWarRankings )
	{
		CalculateScore();
		SendRewardPoint();
	}
}

void GLGuidance::DoCertifyEnd()
{
	m_dwCERTIFY_CHARID = 0;
	m_dwCERTIFY_NPCID = 0;
	m_fCERTIFY_TIMER = 0.0f;
	m_vCERTIFY_POS = D3DXVECTOR3(0,0,0);
}

void GLGuidance::CheckExtraGuild ( float fElaps )
{
	m_fCHECK_TIMER += fElaps;
	if ( m_fCHECK_TIMER > m_fCHECK_TIME )
	{
		if ( m_fCHECK_TIME >= 30.0f )
		{
			m_bBattleEndCheck = false;
			m_fCHECK_TIMER = 0.0f;
		}
		GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( m_dwClubMap );
		if ( pLandMan )
		{
			pLandMan->DoGateOutPCAll ( m_dwGuidanceClub, m_dwCLubMapGate, DxLandGate::DEFAULT_GATE_OUT_INDEX );
		}
		m_fCHECK_TIME += 10.0f;
	}
}

bool GLGuidance::ChangeCommission ( float fRate )
{
	if ( 0 > fRate )								fRate = 0.0f;
	if ( GLCONST_CHAR::fMAX_COMMISSION < fRate )	fRate = GLCONST_CHAR::fMAX_COMMISSION;

	m_fCommissionPercent = fRate;

	//	Note : DB에 반영 필요.
	CSetClubRegion *pDbAction = new CSetClubRegion ( m_dwID, m_dwGuidanceClub, m_fCommissionPercent );
	GLAgentServer::GetInstance().GetDBMan()->AddJob ( pDbAction );

	return true;
}

bool GLGuidance::ChangeGuidClub ( DWORD dwClubID )
{
	m_dwGuidanceClub = dwClubID;
	m_fCommissionPercent = m_fDefaultCommissionPer;

	//	Note : DB에 반영 필요.
	CSetClubRegion *pDbAction = new CSetClubRegion ( m_dwID, m_dwGuidanceClub, m_fCommissionPercent );
	GLAgentServer::GetInstance().GetDBMan()->AddJob ( pDbAction );

	return true;
}

bool GLGuidanceMan::Load ( std::vector<std::string> &vecFiles )
{
	std::vector<std::string>::size_type i = 0, j = vecFiles.size();
	for( ; i < j; ++i )
	{
		GLGuidance sGuidance;
		bool bOK = sGuidance.Load( vecFiles[i] );
		if( !bOK )
		{
			CDebugSet::ToLogFile ( "sGuidance.Load() fail, %s", vecFiles[i].c_str() );
		}

		m_vecGuidance.push_back ( sGuidance );
	}

	std::sort( m_vecGuidance.begin(), m_vecGuidance.end() );

	return true;
}

GLGuidance* GLGuidanceMan::Find ( DWORD dwID )
{
	GLGuidance cObj;
	cObj.m_dwID = dwID;

	GUIDANCE_VEC_ITER pos = std::lower_bound ( m_vecGuidance.begin(), m_vecGuidance.end(), cObj );
	if ( pos==m_vecGuidance.end() )
	{
		return NULL;
	}
	else
	{
		return &(*pos);
	}
}

bool GLGuidanceMan::SetState ( VECGUID_DB &vecGUID )
{
	for ( VECGUID_DB::size_type i=0; i<vecGUID.size(); ++i )
	{
		const GLGUID_DB &sGUID = vecGUID[i];
		
		// 선도클럽 데이터를 DB 에서 가져와서 선도클럽 맵이 존재하는지 검사한다.
		GLGuidance* pGuid = Find ( sGUID.m_dwID );
		if ( !pGuid )
		{			
			//std::strstream strSTREAM;
			//strSTREAM << "GLGuidance::Find() fail, " << sGUID.m_dwID << std::ends;

			TCHAR szTemp[128] = {0};
			_snprintf( szTemp, 128, "GLGuidance::Find() fail, %u", sGUID.m_dwID );

			MessageBox ( NULL, szTemp, "ERROR", MB_OK );
			//strSTREAM.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
			continue;
		}

		pGuid->m_dwGuidanceClub = sGUID.m_dwCLUBID;
		pGuid->m_fCommissionPercent = sGUID.m_fCommissionPer;
	}

	return true;
}

bool GLGuidanceMan::IsBattle ()
{
	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];
	
		if ( sGuidance.m_bBattle )		return true;
	}

	return false;
}

bool GLGuidanceMan::IsCheckExtraGuild ()
{
	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];
	
		if ( sGuidance.m_bBattleEndCheck )		return true;
	}

	return false;
}

GLGuidanceAgentMan& GLGuidanceAgentMan::GetInstance()
{
	static GLGuidanceAgentMan cInstance;
	return cInstance;
}

GLGuidance* GLGuidanceAgentMan::FindByClubID ( DWORD dwClubID )
{
	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];

		if ( sGuidance.m_dwGuidanceClub == dwClubID )	return &sGuidance;
	}

	return NULL;
}

DWORD GLGuidanceAgentMan::GetGuidID ( DWORD dwClubID )
{
	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];

		if ( sGuidance.m_dwGuidanceClub == dwClubID )		return sGuidance.m_dwID;
	}

	return UINT_MAX;
}

bool GLGuidanceAgentMan::SetMapState ()
{
	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];

		//	Note : 선도 클럽이 관리하는 멥.
		for ( std::vector<DWORD>::size_type m=0; m<sGuidance.m_vecMaps.size(); ++m )
		{
			SNATIVEID nidMAP ( sGuidance.m_vecMaps[m] );
			GLAGLandMan *pLandMan = GLAgentServer::GetInstance().GetByMapID ( nidMAP );
			if ( !pLandMan )	continue;

			pLandMan->m_dwClubMapID = sGuidance.m_dwID;
		}

		//	Note : 선도 클럽 전투 입구존.
		SNATIVEID nidHallMAP ( sGuidance.m_dwClubHallMap );
		GLAGLandMan *pLandMan = GLAgentServer::GetInstance().GetByMapID ( nidHallMAP );
		if ( pLandMan )
		{
			pLandMan->m_dwClubMapID = sGuidance.m_dwID;
		}

		//	Note : 선도 클럽 전투존.
		SNATIVEID nidMAP ( sGuidance.m_dwClubMap );
		pLandMan = GLAgentServer::GetInstance().GetByMapID ( nidMAP );
		if ( pLandMan )
		{
			pLandMan->m_dwClubMapID = sGuidance.m_dwID;
			pLandMan->m_bGuidBattleMap = true;
		}
	}

	return true;
}

bool GLGuidanceAgentMan::ChangeGuidClub ( DWORD dwID, DWORD dwClubID )
{
	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )				return false;

	pGuid->ChangeGuidClub ( dwClubID );

	return true;
}

bool GLGuidanceAgentMan::ChangeCommission ( DWORD dwID, float fRate )
{
	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )				return false;

	pGuid->ChangeCommission ( fRate );

	return true;
}

bool GLGuidanceAgentMan::FrameMove ( float fElapsedAppTime )
{
	// 선도전을 시작하지 않는다.
	if ( GLCONST_CHAR::bPKLESS ) return false;

	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];

		if ( sGuidance.IsNewCommission() )
		{
			sGuidance.UpdateCommission ( fElapsedAppTime );
		}

		CTime cCurTime = CTime::GetCurrentTime();
		int nDayOfWeek = cCurTime.GetDayOfWeek ();
		int nDay = cCurTime.GetDay ();
		int nHour = cCurTime.GetHour();
		int nMinute = cCurTime.GetMinute ();

		//broadcast upcoming schedule
		sGuidance.UpdateNotifyBattle();

		//battle is running
		//check of ending battle
		if ( sGuidance.IsBattle() )
		{
			sGuidance.m_fBattleTimer += fElapsedAppTime;
			if ( sGuidance.m_fBattleTimer > float(sGuidance.m_dwBattleTime) )
			{
				sGuidance.DoBattleEnd();

				//CDebugSet::ToFileWithTime( "_cw_test.txt", "end battle %d", sGuidance.m_dwID );
			}
		}
		//no active battle, search for battle to start
		else
		{
			SBATTLE_SCHEDULE* pSchedule = sGuidance.GetStartBattle(nDayOfWeek, nHour, nMinute);
			if ( pSchedule )
			{
				//CDebugSet::ToFileWithTime( "_cw_test.txt", "start battle id:%d d:%d h:%d m:%d", 
				//	sGuidance.m_dwID, pSchedule->nDay, pSchedule->nHour, pSchedule->nMinute );

				//reset the notify state
				pSchedule->bNotify1Hour = false;
				pSchedule->bNotifyHalfHour = false;
				pSchedule->bNotify10Minute = false;

				//	Note : 클럽 결정전 시작.
				sGuidance.DoBattleStart ();
				sGuidance.m_fRemainTimer = (float)m_vecGuidance[0].m_dwBattleTime;
				sGuidance.m_fTimer = 0.0f;
			}
		}

		//remain timer broadcast
		if ( sGuidance.IsBattle() )
		{
			GLMSG::SNET_CLUB_BATTLE_REMAIN_BRD NetMsgBrd;
			NetMsgBrd.dwId = sGuidance.m_dwID;
			StringCchCopy ( NetMsgBrd.szName, GLMSG::SNET_CLUB_BATTLE_REMAIN_BRD::TEXT_LEN, sGuidance.m_strName.c_str() );
			NetMsgBrd.bBattle = TRUE;

			// 시작하면 통지
			if ( sGuidance.m_fRemainTimer == sGuidance.m_dwBattleTime )
			{
				NetMsgBrd.dwTime = (DWORD)sGuidance.m_fRemainTimer;
				GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );
			}

			sGuidance.m_fTimer += fElapsedAppTime;
			sGuidance.m_fRemainTimer -= fElapsedAppTime;

			// 이후 10분만다 통지
			//lowered to 5 minutes
			if ( sGuidance.m_fTimer > 300.0f )
			{
				NetMsgBrd.dwTime = (DWORD)sGuidance.m_fRemainTimer;
				GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsgBrd );
				sGuidance.m_fTimer = 0.0f;
			}
		}
	}


	return true;
}

void GLGuidanceAgentMan::RemainBattleTimeReq( DWORD dwGaeaId )
{
	PGLCHARAG pChar = GLAgentServer::GetInstance().GetChar ( dwGaeaId );
	if ( pChar )
	{
		for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
		{
			GLGuidance &sGuidance = m_vecGuidance[i];

			GLMSG::SNET_CLUB_BATTLE_REMAIN_BRD NetMsg;
			NetMsg.dwId = sGuidance.m_dwID;
			StringCchCopy ( NetMsg.szName, GLMSG::SNET_CLUB_BATTLE_REMAIN_BRD::TEXT_LEN, sGuidance.m_strName.c_str() );
			NetMsg.bBattle = sGuidance.IsBattle();

			NetMsg.dwTime = (DWORD)sGuidance.m_fRemainTimer;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pChar->m_dwClientID, &NetMsg );
		}	
	}
}

GLGuidanceFieldMan& GLGuidanceFieldMan::GetInstance()
{
	static GLGuidanceFieldMan cInstance;
	return cInstance;
}

bool GLGuidanceFieldMan::SetMapState ()
{
//	GLLANDMANLIST& cLandList = GLGaeaServer::GetInstance().GetLandManList();

	/*GLLANDMANNODE* pLandManNode = cLandList.m_pHead;
	for ( ; pLandManNode; pLandManNode = pLandManNode->pNext )
	{
		PGLLANDMAN pLand = pLandManNode->Data;

		pLand->m_bGuidBattleMap = false;
		pLand->m_bGuidBattleMapHall = false;
		
		SMAPNODE *pMAPNODE = GLGaeaServer::GetInstance().FindMapNode ( pLand->GetMapID() );
		if ( pMAPNODE && pMAPNODE->bCommission )
		{
			pLand->m_fCommissionRate = GLCONST_CHAR::fDEFAULT_COMMISSION;
		}
		else
		{
			pLand->m_fCommissionRate = 0.0f;
		}
	}*/

	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];

		//	Note : 선도 클럽이 관리하는 멥.
		for ( std::vector<DWORD>::size_type m=0; m<sGuidance.m_vecMaps.size(); ++m )
		{
			SNATIVEID nidMAP ( sGuidance.m_vecMaps[m] );

			GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( nidMAP );
			if ( !pLandMan )	continue;

			pLandMan->m_dwClubMapID = sGuidance.m_dwID;
			pLandMan->m_pGuidance = GLGuidanceFieldMan::GetInstance().Find ( sGuidance.m_dwID );

			pLandMan->m_dwGuidClubID = sGuidance.m_dwGuidanceClub;
			pLandMan->m_fCommissionRate = sGuidance.m_fCommissionPercent;
		}

		//	Note : 선도 클럽 전투 입구존.
		SNATIVEID nidHallMAP ( sGuidance.m_dwClubHallMap );
		GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( nidHallMAP );
		if ( pLandMan )
		{
			pLandMan->m_dwClubMapID = sGuidance.m_dwID;
			pLandMan->m_pGuidance = GLGuidanceFieldMan::GetInstance().Find ( sGuidance.m_dwID );

			pLandMan->m_bGuidBattleMapHall = true;
			pLandMan->m_dwGuidClubID = sGuidance.m_dwGuidanceClub;
			pLandMan->m_fCommissionRate = sGuidance.m_fCommissionPercent;	
		}

		//	Note : 선도 클럽 전투존.
		SNATIVEID nidMAP ( sGuidance.m_dwClubMap );
		SMAPNODE *pMAPNODE = GLGaeaServer::GetInstance().FindMapNode ( nidMAP );
		if ( pMAPNODE )
		{
			pMAPNODE->bBattleZone = TRUE;
		}

		pLandMan = GLGaeaServer::GetInstance().GetByMapID ( nidMAP );
		if ( pLandMan )
		{
			pLandMan->m_dwClubMapID = sGuidance.m_dwID;
			pLandMan->m_pGuidance = GLGuidanceFieldMan::GetInstance().Find ( sGuidance.m_dwID );

			pLandMan->m_bGuidBattleMap = true;
			pLandMan->m_dwGuidClubID = sGuidance.m_dwGuidanceClub;
			pLandMan->m_fCommissionRate = sGuidance.m_fCommissionPercent;	
		}
	}

	return true;
}

bool GLGuidanceFieldMan::BeginBattle ( DWORD dwID )
{
	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )	return false;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	pGuid->m_mapClubWarClubScore.clear();
	pGuid->m_mapClubWarScore.clear();
	pGuid->m_mapClubWarScoreResu.clear();

	pGuid->m_bBattle = true;

	/*clubwar map lock, Juver, 2017/06/27 */
	//lock maps at event start (field server)
	for( int i=0; i<GLGuidance::MAX_MAP_LOCK; ++i )
	{
		if ( pGuid->m_dwMapLock[i] != NATIVEID_NULL().dwID )
		{
			GLLandMan* pLandMan = GLGaeaServer::GetInstance().GetByMapID( SNATIVEID(pGuid->m_dwMapLock[i]) );
			if ( pLandMan )
				pLandMan->m_bClubWarLock = true;
		}
	}

	//	Note : 멥 설정 변경.

	return true;
}

bool GLGuidanceFieldMan::EndBattle ( DWORD dwID )
{
	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )	return false;


	pGuid->m_bBattle = false;

	// 체크시작
	pGuid->m_bBattleEndCheck = true;
	pGuid->m_fCHECK_TIME = 10.0f;

	GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( pGuid->m_dwClubMap );
	if ( pLandMan )
	{
		pLandMan->DoGateOutPCAll ( pGuid->m_dwGuidanceClub, pGuid->m_dwCLubMapGate, DxLandGate::DEFAULT_GATE_OUT_INDEX );

		//	Note : 보상 몹 소환.
		for ( size_t n=0; n < pGuid->m_vecAwardMob.size(); ++n )
		{
			pLandMan->DropCrow( pGuid->m_vecAwardMob[n].nidGenMob, 
								pGuid->m_vecAwardMob[n].wGenPosX, 
								pGuid->m_vecAwardMob[n].wGenPosY );
		}
	}

	/*clubwar map lock, Juver, 2017/06/27 */
	//unlock maps at event end (field server)
	for( int i=0; i<GLGuidance::MAX_MAP_LOCK; ++i )
	{
		if ( pGuid->m_dwMapLock[i] != NATIVEID_NULL().dwID )
		{
			GLLandMan* pLandMan = GLGaeaServer::GetInstance().GetByMapID( SNATIVEID(pGuid->m_dwMapLock[i]) );
			if ( pLandMan )
				pLandMan->m_bClubWarLock = false;
		}
	}

	return true;
}

bool GLGuidanceFieldMan::ChangeGuidClub ( DWORD dwID, DWORD dwClubID )
{
	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )	return false;

	pGuid->m_dwGuidanceClub = dwClubID;
	pGuid->m_fCommissionPercent = pGuid->m_fDefaultCommissionPer;

	//	Note : 선도 클럽이 관리하는 멥.
	for ( std::vector<DWORD>::size_type m=0; m<pGuid->m_vecMaps.size(); ++m )
	{
		SNATIVEID nidMAP ( pGuid->m_vecMaps[m] );
		GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( nidMAP );
		if ( !pLandMan )	continue;

		pLandMan->m_dwGuidClubID = pGuid->m_dwGuidanceClub;
		pLandMan->m_fCommissionRate = pGuid->m_fCommissionPercent;
	}

	//	Note : 선도 클럽 전투 입구존.
	SNATIVEID nidHallMAP ( pGuid->m_dwClubHallMap );
	GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( nidHallMAP );
	if ( pLandMan )
	{
		pLandMan->m_dwGuidClubID = pGuid->m_dwGuidanceClub;
		pLandMan->m_fCommissionRate = pGuid->m_fCommissionPercent;	
	}

	//	Note : 선도 클럽 전투존.
	SNATIVEID nidMAP ( pGuid->m_dwClubMap );
	pLandMan = GLGaeaServer::GetInstance().GetByMapID ( nidMAP );
	if ( pLandMan )
	{
		pLandMan->m_dwGuidClubID = pGuid->m_dwGuidanceClub;
		pLandMan->m_fCommissionRate = pGuid->m_fCommissionPercent;	
	}

	return true;
}

bool GLGuidanceFieldMan::ChangeCommission ( DWORD dwID, float fRate )
{
	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )	return false;

	if ( 0 > fRate )								fRate = 0.0f;
	if ( GLCONST_CHAR::fMAX_COMMISSION < fRate )	fRate = GLCONST_CHAR::fMAX_COMMISSION;

	pGuid->m_fCommissionPercent = fRate;

	//	Note : 선도 클럽이 관리하는 멥.
	for ( std::vector<DWORD>::size_type m=0; m<pGuid->m_vecMaps.size(); ++m )
	{
		SNATIVEID nidMAP ( pGuid->m_vecMaps[m] );
		GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( nidMAP );
		if ( !pLandMan )	continue;

		pLandMan->m_fCommissionRate = pGuid->m_fCommissionPercent;
	}

	//	Note : 선도 클럽 전투 입구존.
	SNATIVEID nidHallMAP ( pGuid->m_dwClubHallMap );
	GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( nidHallMAP );
	if ( pLandMan )
	{
		pLandMan->m_fCommissionRate = pGuid->m_fCommissionPercent;	
	}

	//	Note : 선도 클럽 전투존.
	SNATIVEID nidMAP ( pGuid->m_dwClubMap );
	pLandMan = GLGaeaServer::GetInstance().GetByMapID ( nidMAP );
	if ( pLandMan )
	{
		pLandMan->m_fCommissionRate = pGuid->m_fCommissionPercent;	
	}

	return true;
}

bool GLGuidanceFieldMan::DoCertify ( DWORD dwID, DWORD dwCHARID, DWORD dwNPCID, const D3DXVECTOR3 &vPOS )
{
	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )	return false;

	pGuid->m_dwCERTIFY_CHARID = dwCHARID;
	pGuid->m_dwCERTIFY_NPCID = dwNPCID;
	pGuid->m_fCERTIFY_TIMER = 0.0f;
	pGuid->m_vCERTIFY_POS = vPOS;

	return true;
}

EMCHECKCERTIFY GLGuidanceFieldMan::CheckCertify ( DWORD dwID, DWORD dwCHARID )
{
	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )				return EMCHECKCERTIFY_FAIL;

	if ( pGuid->IsCertify() )
	{
		if ( pGuid->m_dwCERTIFY_CHARID==dwCHARID )	return EMCHECKCERTIFY_DOING;
		else										return EMCHECKCERTIFY_OTHERDOING;
	}

	if ( !pGuid->m_bBattle )	return EMCHECKCERTIFY_NOTBATTLE;

	return EMCHECKCERTIFY_OK;
}

const std::string GLGuidanceFieldMan::GetName ( DWORD dwID )
{
	GLGuidance *pGuid = Find ( dwID );
	if ( !pGuid )				return "";

	return pGuid->m_strName;
}

DWORD GLGuidanceFieldMan::GetGuidID ( DWORD dwClubID )
{
	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];

		if ( sGuidance.m_dwGuidanceClub == dwClubID )		return sGuidance.m_dwID;
	}

	return UINT_MAX;
}

bool GLGuidanceFieldMan::FrameMove ( float fElaps )
{
	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];

		sGuidance.m_fCERTIFY_TIMER += fElaps;

		if ( sGuidance.IsCertify() )
		{
			// 인증도중 게임 종료
			PGLCHAR pCHAR = GLGaeaServer::GetInstance().GetCharID ( sGuidance.m_dwCERTIFY_CHARID );
			if ( !pCHAR )
			{
				sGuidance.DoCertifyEnd();
				continue;
			}

			// 일반 오류..
			if ( !pCHAR->m_pLandMan )
			{
				sGuidance.DoCertifyEnd();
				pCHAR->SetClubWarVerify( FALSE ); /*clubwar verify non push or pull, Juver, 2017/12/13 */
				continue;
			}

			// 일반 오류 ( 인증도중 클럽을 탈퇴할 경우 )
			if ( pCHAR->m_dwGuild == CLUB_NULL )
			{
				GLMSG::SNET_CLUB_CD_CERTIFY_FB NetMsgFb;
				NetMsgFb.emFB = EMCDCERTIFY_FAIL;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFb );

				sGuidance.DoCertifyEnd();
				pCHAR->SetClubWarVerify( FALSE ); /*clubwar verify non push or pull, Juver, 2017/12/13 */
				continue;
			}

			// 장소이동 카드를 이용한 경우(친카, 귀환카드류)
			if ( pCHAR->m_sMapID.dwID!=sGuidance.m_dwClubMap )
			{
				//	Note : 인증도중 장소 이탈.
				// 자기자신에게
				GLMSG::SNET_CLUB_CD_CERTIFY_FB NetMsgFb;
				NetMsgFb.emFB = EMCDCERTIFY_ING_DISTANCE;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFb );

				// 모두에게
				GLMSG::SNET_CLUB_CD_CERTIFY_ING_BRD NetMsgBrd;
				NetMsgBrd.emFB = EMCDCERTIFY_ING_DISTANCE;
				std::string strZone = GetName ( pCHAR->m_pLandMan->m_dwClubMapID );
				GLCLUB* pCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
				if ( pCLUB->IsAlliance() )
				{
					GLCLUB* pCHIEFCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pCLUB->m_dwAlliance );
					StringCchCopy ( NetMsgBrd.szAlliance, CHAR_SZNAME, pCHIEFCLUB->m_szName );
				}
					
				StringCchCopy ( NetMsgBrd.szZone, GLMSG::SNET_CLUB_CD_CERTIFY_BRD::TEXT_LEN, strZone.c_str() );
				StringCchCopy ( NetMsgBrd.szClub, CHAR_SZNAME, pCLUB->m_szName );
				StringCchCopy ( NetMsgBrd.szName, CHAR_SZNAME, pCHAR->m_szName );
				GLGaeaServer::GetInstance().SENDTOAGENT ( &NetMsgBrd );

				sGuidance.DoCertifyEnd();
				pCHAR->SetClubWarVerify( FALSE ); /*clubwar verify non push or pull, Juver, 2017/12/13 */
				continue;
			}
			
			// 인증도중 사망
			if ( !pCHAR->IsValidBody() )
			{
				// 자기자신에게
				GLMSG::SNET_CLUB_CD_CERTIFY_FB NetMsgFb;
				NetMsgFb.emFB = EMCDCERTIFY_ING_DIE;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFb );

				// 모두에게

				GLMSG::SNET_CLUB_CD_CERTIFY_ING_BRD NetMsgBrd;
				NetMsgBrd.emFB = EMCDCERTIFY_ING_DIE;
				std::string strZone = GetName ( pCHAR->m_pLandMan->m_dwClubMapID );
				GLCLUB* pCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
				if ( pCLUB->IsAlliance() )
				{
					GLCLUB* pCHIEFCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pCLUB->m_dwAlliance );
					StringCchCopy ( NetMsgBrd.szAlliance, CHAR_SZNAME, pCHIEFCLUB->m_szName );
				}
					
				StringCchCopy ( NetMsgBrd.szZone, GLMSG::SNET_CLUB_CD_CERTIFY_BRD::TEXT_LEN, strZone.c_str() );
				StringCchCopy ( NetMsgBrd.szClub, CHAR_SZNAME, pCLUB->m_szName );
				StringCchCopy ( NetMsgBrd.szName, CHAR_SZNAME, pCHAR->m_szName );
				GLGaeaServer::GetInstance().SENDTOAGENT ( &NetMsgBrd );

				sGuidance.DoCertifyEnd();
				pCHAR->SetClubWarVerify( FALSE ); /*clubwar verify non push or pull, Juver, 2017/12/13 */
				continue;
			}

			PGLCROW pCROW = pCHAR->m_pLandMan->GetCrow ( sGuidance.m_dwCERTIFY_NPCID );

			D3DXVECTOR3 vDist1 = pCROW->GetPosition() - pCHAR->GetPosition();
			float fDist1 = D3DXVec3Length ( &vDist1 );

			D3DXVECTOR3 vDist2 = pCHAR->m_vPos - sGuidance.m_vCERTIFY_POS;
			float fDist2 = D3DXVec3Length ( &vDist2 );

			if ( fDist1 > GLCONST_CHAR::fCDCERTIFY_DIST || fDist2 > GLCONST_CHAR::fCDCERTIFY_DIST2 )
			{
				//	Note : 인증도중 장소 이탈.
				// 자기자신에게
				GLMSG::SNET_CLUB_CD_CERTIFY_FB NetMsgFb;
				NetMsgFb.emFB = EMCDCERTIFY_ING_DISTANCE;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFb );

				// 모두에게
				GLMSG::SNET_CLUB_CD_CERTIFY_ING_BRD NetMsgBrd;
				NetMsgBrd.emFB = EMCDCERTIFY_ING_DISTANCE;
				std::string strZone = GetName ( pCHAR->m_pLandMan->m_dwClubMapID );
				GLCLUB* pCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
				if ( pCLUB->IsAlliance() )
				{
					GLCLUB* pCHIEFCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pCLUB->m_dwAlliance );
					StringCchCopy ( NetMsgBrd.szAlliance, CHAR_SZNAME, pCHIEFCLUB->m_szName );
				}
					
				StringCchCopy ( NetMsgBrd.szZone, GLMSG::SNET_CLUB_CD_CERTIFY_BRD::TEXT_LEN, strZone.c_str() );
				StringCchCopy ( NetMsgBrd.szClub, CHAR_SZNAME, pCLUB->m_szName );
				StringCchCopy ( NetMsgBrd.szName, CHAR_SZNAME, pCHAR->m_szName );
				GLGaeaServer::GetInstance().SENDTOAGENT ( &NetMsgBrd );

				sGuidance.DoCertifyEnd();
				pCHAR->SetClubWarVerify( FALSE ); /*clubwar verify non push or pull, Juver, 2017/12/13 */
				continue;
			}

			if ( !sGuidance.m_bBattle )
			{
				//	Note : 인증도중 시간 초과.
				// 자기자신에게
				GLMSG::SNET_CLUB_CD_CERTIFY_FB NetMsgFb;
				NetMsgFb.emFB = EMCDCERTIFY_ING_TIMEOUT;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFb );

				// 모두에게
				GLMSG::SNET_CLUB_CD_CERTIFY_ING_BRD NetMsgBrd;
				NetMsgBrd.emFB = EMCDCERTIFY_ING_TIMEOUT;
				std::string strZone = GetName ( pCHAR->m_pLandMan->m_dwClubMapID );
				GLCLUB* pCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pCHAR->m_dwGuild );
				if ( pCLUB->IsAlliance() )
				{
					GLCLUB* pCHIEFCLUB = GLGaeaServer::GetInstance().GetClubMan().GetClub ( pCLUB->m_dwAlliance );
					StringCchCopy ( NetMsgBrd.szAlliance, CHAR_SZNAME, pCHIEFCLUB->m_szName );
				}
					
				StringCchCopy ( NetMsgBrd.szZone, GLMSG::SNET_CLUB_CD_CERTIFY_BRD::TEXT_LEN, strZone.c_str() );
				StringCchCopy ( NetMsgBrd.szClub, CHAR_SZNAME, pCLUB->m_szName );
				StringCchCopy ( NetMsgBrd.szName, CHAR_SZNAME, pCHAR->m_szName );
				GLGaeaServer::GetInstance().SENDTOAGENT ( &NetMsgBrd );

				sGuidance.DoCertifyEnd();
				pCHAR->SetClubWarVerify( FALSE ); /*clubwar verify non push or pull, Juver, 2017/12/13 */
				continue;
			}

			if ( sGuidance.m_fCERTIFY_TIMER >= GLCONST_CHAR::fCDCERTIFY_TIME )
			{
				//	Note : 인증 완료.
				GLMSG::SNET_CLUB_CD_CERTIFY_FB NetMsgFB;
				NetMsgFB.emFB = EMCDCERTIFY_COMPLETE;
				GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsgFB );

				//	Note : 에이젼트에 알림.
				GLMSG::SNET_CLUB_CERTIFIED_AGT NetMsg;
				NetMsg.dwID = sGuidance.m_dwID;
				NetMsg.dwCLUBID = pCHAR->m_dwGuild;
				GLGaeaServer::GetInstance().SENDTOAGENT ( &NetMsg );

				//	Note : 멥 정리.
				GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( sGuidance.m_dwClubMap );
				if ( pLandMan )
				{
					pLandMan->DoGateOutPCAll ( pCHAR->m_dwGuild, sGuidance.m_dwCLubMapGate, DxLandGate::DEFAULT_GATE_OUT_INDEX );
				}

				sGuidance.DoCertifyEnd();
				pCHAR->SetClubWarVerify( FALSE ); /*clubwar verify non push or pull, Juver, 2017/12/13 */
			}
		}

		/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
		if (sGuidance.IsBattle())
		{
			sGuidance.m_fCWRankingUpdate += fElaps;

			if (sGuidance.m_fCWRankingUpdate > 30.0f)
			{
				sGuidance.UpdateClubWarClubRanking();
				sGuidance.UpdateClubWarRanking();
				sGuidance.UpdateClubWarRankingResu();
				sGuidance.m_fCWRankingUpdate = 0.0f;
			}
		}
	}

	return true;
}

void GLGuidanceFieldMan::CheckExtraGuild ( float fElaps )
{
	for ( GUIDANCE_VEC::size_type i=0; i<m_vecGuidance.size(); ++i )
	{
		GLGuidance &sGuidance = m_vecGuidance[i];
		if ( sGuidance.m_bBattleEndCheck )
		{
			sGuidance.CheckExtraGuild( fElaps );
		}
	}
}
