#include "StdAfx.h"
#include "./GLPVPTyrannyAgent.h"
#include "./GLAgentServer.h"
#include "./GLAGLandMan.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLPVPTyrannyAgent& GLPVPTyrannyAgent::GetInstance()
{
	static GLPVPTyrannyAgent cInstance;
	return cInstance;
}

void GLPVPTyrannyAgent::LoadEvent( std::string strFile )
{
	if ( !RPARAM::bEventTyranny )	return;

	ResetData();

	bool bok = Load( strFile );
	if ( bok )
	{
		FindScheduleFirst();
	}
}

bool GLPVPTyrannyAgent::SetMapState ()
{
	for ( size_t i=0; i<m_vecMapRandom.size(); ++i )
	{
		const SNATIVEID& sMapID = m_vecMapRandom[i];

		if( sMapID.IsValidNativeID() )
		{
			GLAGLandMan *plandman = GLAgentServer::GetInstance().GetByMapID ( sMapID );
			if ( plandman )
				plandman->m_bPVPTyrannyMap = true;
		}
	}

	return true;
}

bool GLPVPTyrannyAgent::FrameMove ( float fElaps )
{
	if ( GLCONST_CHAR::bPKLESS )	return false;
	if ( m_vecSchedule.empty() )	return false;

	if ( IsEnded() )
	{
		if ( m_sScheduleNext.dwIndex != TYRANNY_SCHEDULE_NONE )
		{
			CTime ctimecur = CTime::GetCurrentTime();
			if ( m_sScheduleNext.wStartDay == (WORD)ctimecur.GetDayOfWeek() &&
				m_sScheduleNext.wStartHour == (WORD)ctimecur.GetHour() &&
				m_sScheduleNext.wStartMinute == (WORD)ctimecur.GetMinute() )
			{
				SetEventState( TYRANNY_STATE_REGISTER );
				m_fRemainTimeToNextBattle = 0.0f; // PVP Next Battle UI - Jxyyy
			}
		}
	}
	

	if ( IsRegister() )
	{
		m_fTimer += fElaps;
		m_fRemain -= fElaps;
		m_fRemainTimeToNextBattle = 0.0f; // PVP Next Battle UI - Jxyyy

		NotifyTimeToStart( m_fRemain );	

		if ( m_fTimer >= (float)m_wRegisterTime ){
			SetEventState( TYRANNY_STATE_BATTLE );
		}
	}

	if ( IsBattle() )
	{
		m_fTimer += fElaps;
		m_fRemain -= fElaps;
		m_fRemainTimeToNextBattle = 0.0f; // PVP Next Battle UI - Jxyyy

		if ( m_fTimer >= (float)m_wBattleTime ){
			SetEventState( TYRANNY_STATE_REWARD );
		}
	}

	if ( IsReward() )
	{
		m_fTimer += fElaps;
		m_fRemain -= fElaps;
		m_fRemainTimeToNextBattle = 0.0f; // PVP Next Battle UI - Jxyyy

		if ( m_fTimer >= (float)m_wRewardTime ){
			SetEventState( TYRANNY_STATE_ENDED );
		}
	}

	if ( m_bMoveIn && IsBattle() )
	{
		m_fMoveTimer += fElaps;
		m_fMoveTimerTotal += fElaps;
		m_fRemainTimeToNextBattle = 0.0f; // PVP Next Battle UI - Jxyyy

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
		m_fMoveTimer += fElaps;
		m_fMoveTimerTotal += fElaps;

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

void GLPVPTyrannyAgent::SetEventState( TYRANNY_STATE emState )
{
	std::string streventstate[TYRANNY_STATE_SIZE] = 
	{
		"Event End",
		"Register Start",
		"Battle Start",
		"Award Start"
	};

	GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "GLPVPTyrannyAgent Change State to '%s'", streventstate[emState].c_str() );
	CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyAgent Change State to '%s'", streventstate[emState].c_str() );

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

void GLPVPTyrannyAgent::ResetData()
{
	m_emState = TYRANNY_STATE_ENDED;
	m_sScheduleNext = TYRANNY_SCHED_NEXT();
	m_vecSchedule.clear();
	m_mapBuffData.clear();
	
	for( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
		m_sTowerData[i].wOwner = TYRANNY_SCHOOL_SIZE;

	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		m_sTowerScore[i].Reset();
		m_sTowerScore[i].wSchool = i;

		m_dwGuiID[i] = CLUB_NULL;
		m_dwMarkVer[i] = 0;
	}

	m_wLastWinner = TYRANNY_SCHOOL_SIZE;

	for( int i=0; i<TYRANNY_MINI_RANKING_NUM; ++i )
		m_sLastTop[i] = TYRANNY_PLAYER_DATA();

	m_vecPlayerMove.clear();
	m_mapPlayerDataSG.clear();
	m_mapPlayerDataMP.clear();
	m_mapPlayerDataPHX.clear();
	m_vecPlayerDataQueueSG.clear();
	m_vecPlayerDataQueueMP.clear();
	m_vecPlayerDataQueuePHX.clear();

	m_bMoveIn = false;
	m_bMoveOut = false;

	m_fMoveTimer = 0.0f;
	m_fMoveTimerTotal = 0.0f;

	/*PVPTyranny enemy resu, EJCode, 2018/10/21 */
	m_bDisableOtherSchoolResu = FALSE;

	/*PVPTyranny disable score self heal, EJCode, 2018/11/28 */
	m_bDisableScoreSelfHeal = FALSE;

	m_fContributionPointMultiplier = 1.0f;

	/* PVPTyranny Hash Check, Juver, 2020/07/28 */
	m_mapPlayerHash.clear();
}

void GLPVPTyrannyAgent::DoStateRegister()
{
	m_wLastNotifyTime = 0;
	m_fTimer	= 0.0f;
	m_fRemain	= (float)m_wRegisterTime;

	m_vecPlayerMove.clear();
	m_mapPlayerDataSG.clear();
	m_mapPlayerDataMP.clear();
	m_mapPlayerDataPHX.clear();
	m_vecPlayerDataQueueSG.clear();
	m_vecPlayerDataQueueMP.clear();
	m_vecPlayerDataQueuePHX.clear();

	GLMSG::SNET_TYRANNY_A2FC_STATE_REGISTER netmsgstate;
	netmsgstate.fTime = m_fRemain;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );

	GLAgentServer::GetInstance().TyrannyResetBuffs();

	/*GLMSG::SNET_TYRANNY_A2FC_REWARD_BUFF_INFO netmsgbuff;
	for( int i=0; i<TYRANNY_REWARD_BUFF_NUM; ++i )
		netmsgbuff.sRewardBuff[i] = m_sScheduleNext.m_sRewardBuff[i];

	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &netmsgbuff );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgbuff );*/

	BuffSelect();
	BattleMapSelect();

	GLMSG::SNET_TYRANNY_A2FC_NEXTSCHED netmsgschedule;
	netmsgschedule.sScheduleNext = m_sScheduleNext;
	GLAgentServer::GetInstance().SENDTOCHANNEL( &netmsgschedule, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsgschedule );


	/* PVPTyranny Hash Check, Juver, 2020/07/28 */
	m_mapPlayerHash.clear();
}

void GLPVPTyrannyAgent::DoStateBattle()
{
	m_fTimer	= 0.0f;
	m_fRemain	= (float)m_wBattleTime;

	//reset tower owner
	for( int i=0; i<TYRANNY_TOWER_SIZE; ++i )
		m_sTowerData[i].wOwner = TYRANNY_SCHOOL_SIZE;

	//reset tower scores
	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		m_sTowerScore[i].Reset();
		m_sTowerScore[i].wSchool = i;

		m_dwGuiID[i] = CLUB_NULL;
		m_dwMarkVer[i] = 0;
	}

	//reset the last winner
	m_wLastWinner = TYRANNY_SCHOOL_SIZE;

	for( int i=0; i<TYRANNY_MINI_RANKING_NUM; ++i )
		m_sLastTop[i] = TYRANNY_PLAYER_DATA();

	CalculateRebirthTime();

	ProcessQueueDataAll( m_mapPlayerDataSG, m_vecPlayerDataQueueSG );
	ProcessQueueDataAll( m_mapPlayerDataMP, m_vecPlayerDataQueueMP );
	ProcessQueueDataAll( m_mapPlayerDataPHX, m_vecPlayerDataQueuePHX );

	CalculateBattleBuff();
	SendPlayerData();
	QueueMoveIn();

	GLMSG::SNET_TYRANNY_A2FC_STATE_BATTLE netmsgstate;
	netmsgstate.fTime = m_fRemain;
	netmsgstate.wPlayerNum[TYRANNY_SCHOOL_SG] = (WORD)m_mapPlayerDataSG.size();
	netmsgstate.wPlayerNum[TYRANNY_SCHOOL_MP] = (WORD)m_mapPlayerDataMP.size();
	netmsgstate.wPlayerNum[TYRANNY_SCHOOL_PHX] = (WORD)m_mapPlayerDataPHX.size();
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
}

void GLPVPTyrannyAgent::DoStateReward()
{
	m_fTimer	= 0.0f;
	m_fRemain	= (float)m_wRewardTime;

	//get the winner
	CheckWinner();

	GLMSG::SNET_TYRANNY_A2FC_STATE_REWARD netmsgstate;
	netmsgstate.fTime = m_fRemain;
	netmsgstate.wWinnerSchool = m_wLastWinner;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );
}

void GLPVPTyrannyAgent::DoStateEnd()
{
	GLMSG::SNET_TYRANNY_A2FC_STATE_ENDED netmsgstate;
	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgstate, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgstate );

	if ( FindScheduleNext() )
	{
		CalculateRemainingTimeNexBattle(); // PVP Next Battle UI - Jxyyy
		GLMSG::SNET_TYRANNY_A2FC_NEXTSCHED netmsgschedule;
		netmsgschedule.sScheduleNext = m_sScheduleNext;
		netmsgschedule.fRemainTimeToNextBattle = m_fRemainTimeToNextBattle; // PVP Next Battle UI - Jxyyy
		GLAgentServer::GetInstance().SENDTOCHANNEL( &netmsgschedule, 0 );
		GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsgschedule );
	}

	QueueMoveOut();

	m_mapPlayerDataSG.clear();
	m_mapPlayerDataMP.clear();
	m_mapPlayerDataPHX.clear();
	m_vecPlayerDataQueueSG.clear();
	m_vecPlayerDataQueueMP.clear();
	m_vecPlayerDataQueuePHX.clear();

	if ( m_sScheduleNext.m_bRewardBuff )
		GLAgentServer::GetInstance().TyrannySetRewardBuffs();

	/* PVPTyranny Hash Check, Juver, 2020/07/28 */
	m_mapPlayerHash.clear();
}

void GLPVPTyrannyAgent::NotifyTimeToStart( float fTime )
{
	WORD wremain = (WORD)m_fRemain;
	if( m_wLastNotifyTime == wremain )	return;

	bool bnotify = false;
	if ( wremain > 300 && ( wremain%300 == 0 ) )	
		bnotify = true;
	else if ( wremain > 0 && wremain <= 300 && ( wremain%60 == 0 ) )
		bnotify = true;

	if ( !bnotify )	return;

	m_wLastNotifyTime = wremain;

	GLMSG::SNET_TYRANNY_A2C_TOBATTLE_TIME netmsgclient;
	netmsgclient.wTime = wremain / 60;
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &netmsgclient );
}

/*global buffs map setting, Juver, 2018/01/23 */
void GLPVPTyrannyAgent::RequestInfoPC( DWORD dwCharID, BOOL bTyrannyMap, BOOL bDisableSystemBuffs )
{
	PGLCHARAG pchar = GLAgentServer::GetInstance().GetCharID( dwCharID );
	if ( !pchar )	return;
	// PVP Next Battle UI - Jxyyy
	CalculateRemainingTimeNexBattle();
	GLMSG::SNET_TYRANNY_A2FC_NEXTSCHED netmsgschedule;
	netmsgschedule.sScheduleNext = m_sScheduleNext;
	netmsgschedule.fRemainTimeToNextBattle = m_fRemainTimeToNextBattle;
	GLAgentServer::GetInstance().SENDTOCHANNEL( &netmsgschedule, 0 );
	GLAgentServer::GetInstance().SENDTOALLCLIENT( &netmsgschedule );

	//event info
	GLMSG::SNET_TYRANNY_A2C_BATTLEINFO_PC netmsgclient;
	netmsgclient.emState = m_emState;
	netmsgclient.fRemain = m_fRemain;
	netmsgclient.sScheduleNext = m_sScheduleNext;
	netmsgclient.wLevelReq = m_wLevelReq;
	netmsgclient.wPlayerLimit = m_wPlayerLimit;

	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
		netmsgclient.wRebirthTime[i] = m_wRebirthTime[i];
	
	//register info
	TYRANNY_REGISTER_DATA* pplayerdata = PlayerDataGet( dwCharID );
	if ( pplayerdata ){
		netmsgclient.bRegistered = true;
	}

	TYRANNY_REGISTER_DATA_QUEUE* pplayerqueue = PlayerQueueGet( dwCharID );
	if ( pplayerqueue ){
		netmsgclient.bRegistered = true;
		netmsgclient.bQueued = true;
		netmsgclient.wQueueNum = pplayerqueue->wQueueNum;
	}

	/*pvp reward info, Juver, 2018/07/24 */
	netmsgclient.sItemRewardWin = m_sItemRewardWin;
	netmsgclient.sItemRewardLoss = m_sItemRewardLost;
	netmsgclient.wRewardLimitScoreWin = m_wRewardLimitScoreWin;
	netmsgclient.wRewardLimitScoreLost = m_wRewardLimitScoreLost;
	netmsgclient.fContributionPointMultiplier = m_fContributionPointMultiplier;
	
	GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsgclient );

	//last rankinfo
	GLMSG::SNETPC_TYRANNY_A2C_RANKINFO_PC netmsglastrank;
	netmsglastrank.wLastWinner = m_wLastWinner;
	for( int i=0; i<TYRANNY_MINI_RANKING_NUM; ++i )
		netmsglastrank.sPlayerData[i] = m_sLastTop[i];

	//for( int i=0; i<TYRANNY_REWARD_BUFF_NUM; ++i )
	//	netmsglastrank.sRewardBuff[i] = m_sScheduleNext.m_sRewardBuff[i].sidSkill;

	GLAgentServer::GetInstance().SENDTOCLIENT ( pchar->m_dwClientID, &netmsglastrank );

	

	if ( bTyrannyMap )
	{
		if ( m_sScheduleNext.m_bBattleBuff )
		{
			GLMSG::SNETPC_TYRANNY_A2F_BUFF_BATTLE netmsgbuff;
			netmsgbuff.dwCharID = pchar->m_dwCharID;

			for ( int i=0; i<TYRANNY_BATTLE_BUFF_NUM; ++i )
			{
				WORD _wLevel = m_wBattleBuffLevelIndex[pchar->m_wSchool];
				netmsgbuff.sBattleBuff[i].sidSkill = m_sScheduleNext.m_sBattleBuff[i].sidSkill;
				netmsgbuff.sBattleBuff[i].wLevel = m_sScheduleNext.m_sBattleBuff[i].wLevel[_wLevel];
			}

			GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &netmsgbuff );
		}
	}
	else
	{
		/*global buffs map setting, Juver, 2018/01/23 */
		if ( m_sScheduleNext.m_bRewardBuff && !bDisableSystemBuffs )
		{
			BOOL bEnableBuff = TRUE;
			if ( pchar->m_wSchool != m_wLastWinner )	bEnableBuff = FALSE;
			if ( m_emState != TYRANNY_STATE_ENDED )		bEnableBuff = FALSE;
			if ( m_wLastWinner == TYRANNY_SCHOOL_SIZE ) bEnableBuff = FALSE;

			if ( bEnableBuff )
			{
				GLMSG::SNETPC_TYRANNY_A2F_BUFF_REWARD netmsgbuff;
				netmsgbuff.dwCharID = pchar->m_dwCharID;
				netmsgbuff.sRewardBuffMain = m_sScheduleNext.m_sRewardBuffMain;

				for ( int i=0; i<TYRANNY_REWARD_BUFF_NUM; ++i )
					netmsgbuff.sRewardBuff[i] = m_sScheduleNext.m_sRewardBuff[i];

				GLAgentServer::GetInstance().SENDTOFIELDSVR ( pchar->m_nChannel, pchar->m_dwCurFieldSvr, &netmsgbuff );
			}
		}
	}
}

void GLPVPTyrannyAgent::CalculateBattleBuff()
{
	//reset buff level
	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
		m_wBattleBuffLevelIndex[i] = 0;

	if ( m_wPlayerLimit > 0 )
	{
		{	//sg
			int nPlayerCnt = (int)m_mapPlayerDataSG.size();
			float fPercent = ((float)nPlayerCnt/(float)m_wPlayerLimit) * 10.0f; //1~10 %
			m_wBattleBuffLevelIndex[0] = static_cast <WORD> (std::floor(fPercent) );
		}

		{	//mp
			int nPlayerCnt = (int)m_mapPlayerDataMP.size();
			float fPercent = ((float)nPlayerCnt/(float)m_wPlayerLimit) * 10.0f; //1~10 %
			m_wBattleBuffLevelIndex[1] = static_cast <WORD> (std::floor(fPercent) );
		}

		{	//phx
			int nPlayerCnt = (int)m_mapPlayerDataPHX.size();
			float fPercent = ((float)nPlayerCnt/(float)m_wPlayerLimit) * 10.0f; //1~10 %
			m_wBattleBuffLevelIndex[2] = static_cast <WORD> (std::floor(fPercent) );
		}
	}

	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
	{
		if( m_wBattleBuffLevelIndex[i] >= 0 && m_wBattleBuffLevelIndex[i] < TYRANNY_BATTLE_BUFF_LEVEL_NUM )
		{
			CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyAgent player buff levels index:%d level:%d", i, m_wBattleBuffLevelIndex[i] );
		}
		else
		{
			WORD wLast = m_wBattleBuffLevelIndex[i];
			m_wBattleBuffLevelIndex[i] = TYRANNY_BATTLE_BUFF_LEVEL_NUM -1 ;
			CDebugSet::ToFileWithTime( "_pvptyranny.txt", "GLPVPTyrannyAgent player wrong buff levels index:%d level:%d corrected to level:%d", i, wLast, m_wBattleBuffLevelIndex[i] );
		}
	}
}

void GLPVPTyrannyAgent::CalculateRebirthTime()
{
	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
		m_wRebirthTime[i] = m_wRebirthTimeBase;

	m_wRebirthTime[TYRANNY_SCHOOL_SG] += WORD( m_mapPlayerDataSG.size() * m_fPerCharRebirthTimeAdd );
	m_wRebirthTime[TYRANNY_SCHOOL_MP] += WORD( m_mapPlayerDataMP.size() * m_fPerCharRebirthTimeAdd );
	m_wRebirthTime[TYRANNY_SCHOOL_PHX] += WORD( m_mapPlayerDataPHX.size() * m_fPerCharRebirthTimeAdd );

	CDebugSet::ToFileWithTime( "_pvptyranny_rebirth_time.txt", "GLPVPTyrannyAgent CalculateRebirthTime SG:%d(%u) MP:%d(%u) PHX:%d(%u)", 
		m_wRebirthTime[TYRANNY_SCHOOL_SG], m_mapPlayerDataSG.size(),
		m_wRebirthTime[TYRANNY_SCHOOL_MP], m_mapPlayerDataMP.size(), 
		m_wRebirthTime[TYRANNY_SCHOOL_PHX], m_mapPlayerDataPHX.size() );

	

	GLMSG::SNET_TYRANNY_A2F_SET_REBIRTHTIME netmsgtime;

	for( int i=0; i<TYRANNY_SCHOOL_SIZE; ++i )
		netmsgtime.wRebirthTime[i] = m_wRebirthTime[i];

	GLAgentServer::GetInstance().SENDTOCHANNEL ( &netmsgtime, 0 );
}
// PVP Next Battle UI - Jxyyy
void GLPVPTyrannyAgent::CalculateRemainingTimeNexBattle()
{
	CTime ctimecur = CTime::GetCurrentTime();
	float day = abs(m_sScheduleNext.wStartDay - (WORD)ctimecur.GetDayOfWeek()) * 86400;
	if ( m_sScheduleNext.wStartDay == 1 && (WORD)ctimecur.GetDayOfWeek() == 7) day = 86400.0f;
	float hour = abs(m_sScheduleNext.wStartHour - (WORD)ctimecur.GetHour()) * 3600;
	float minute = abs(m_sScheduleNext.wStartMinute - (WORD)ctimecur.GetMinute()) * 60;
	float sec = (WORD) ctimecur.GetSecond();
	float total = 0.0f;
	if ( day != 0 ) total = day;
	if ( hour != 0 ) total -= hour;
	if ( minute != 0 ) total = abs(total) - minute;
	if ( sec != 0 ) total = abs(total) - sec;

	m_fRemainTimeToNextBattle = total;
}