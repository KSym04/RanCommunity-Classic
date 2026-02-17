#include "stdafx.h"
#include "./DbActionLogic.h"
#include "./G-Logic/GLGaeaServer.h"
#include "./G-Logic/GLChar.h"

#include "../Lib_Network/s_CSessionServer.h"
#include "../Lib_Network/s_CFieldServer.h"
#include "../Lib_Network/s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PCIDBlock::PCIDBlock( 
	DWORD dwClientID, 
	DWORD dwUserID, 
	DWORD dwCharID, 
	const char* szCharName, 
	DWORD dwType, 
	const char* szHWID, 
	const char* szMAC )
{	
	m_dwClient = dwClientID;

	m_dwUserID = dwUserID;
	m_dwCharID = dwCharID;

	StringCchCopy(m_szName, CHAR_SZNAME+1, szCharName);

	m_dwType = dwType;
	StringCchCopy(m_szHWID, PCID_LENGTH, szHWID);
	StringCchCopy(m_szMAC, PCID_LENGTH, szMAC);
}

int PCIDBlock::Execute (CServer* pServer)
{
	if (pServer == NULL)	return DB_ERROR;

	return m_pDbManager->PCIDBlock( m_dwUserID, m_dwCharID, m_szName, m_dwType, m_szHWID, m_szMAC );
}

/* user flag restricted, Juver, 2020/04/21 */
UserRestrict::UserRestrict( DWORD dwClientID, DWORD dwUserID, bool bUserRestrict )
{	
	m_dwClient = dwClientID;

	m_dwUserID = dwUserID;
	m_bUserRestrict = bUserRestrict;
}

/* user flag restricted, Juver, 2020/04/21 */
int UserRestrict::Execute (CServer* pServer)
{
	if (pServer == NULL)	return DB_ERROR;

	return m_pDbManager->UserRestrict( m_dwUserID, m_bUserRestrict );
}



/* max level notice, Juver, 2021/07/26 */
LogMaxLevel::LogMaxLevel( DWORD dwClientID, DWORD dwUserID, DWORD dwCharID, const char* szName, WORD wLevel )
{	
	m_dwClient = dwClientID;

	m_dwUserID = dwUserID;
	m_dwCharID = dwCharID;

	StringCchCopy( m_szName, CHAR_SZNAME+1, szName);
	
	m_wLevel = wLevel;
}

/* max level notice, Juver, 2021/07/26 */
int LogMaxLevel::Execute (CServer* pServer)
{
	if (pServer == NULL)	return DB_ERROR;

	return m_pDbManager->LogMaxLevel( m_dwUserID, m_dwCharID, m_szName, m_wLevel );
}


/* tyranny result log, Juver, 2021/09/09 */
CLogTyrannyResult::CLogTyrannyResult( 
	WORD wTyrannyWinner, 
	DWORD dwParticipantSG, 
	DWORD dwParticipantMP, 
	DWORD dwParticipantPHX )
{	
	m_wTyrannyWinner = wTyrannyWinner;
	m_dwParticipantSG = dwParticipantSG;
	m_dwParticipantMP = dwParticipantMP;
	m_dwParticipantPHX = dwParticipantPHX;
}

/* tyranny result log, Juver, 2021/09/09 */
int CLogTyrannyResult::Execute (CServer* pServer)
{
	if (pServer == NULL)	return DB_ERROR;

	int nSessionID  = m_pDbManager->LogTyrannyResult( m_wTyrannyWinner, m_dwParticipantSG, m_dwParticipantMP, m_dwParticipantPHX );
	if ( nSessionID != DB_ERROR )
	{
		GLMSG::SNETPC_TYRANNY_LOG_RESULT_FROM_DB NetMsg;
		NetMsg.nSessionID = nSessionID; 
		CFieldServer* pFieldServer = reinterpret_cast<CFieldServer*> (pServer);
		pFieldServer->InsertMsg ( DBACTION_CLIENT_ID, (char*) &NetMsg );
	}

	return DB_OK;
}


/* tyranny result log, Juver, 2021/09/09 */
CLogTyrannyParticipant::CLogTyrannyParticipant( 
	int nSessionID,
	DWORD dwChaNum,
	const char* szName, 
	WORD wSchool, 
	WORD wClass, 
	WORD wRankAll,
	WORD wRankSchool,
	WORD wScoreKill,
	WORD wScoreDeath,
	DWORD dwScoreDamage,
	DWORD dwScoreHeal,
	WORD wScoreResu,
	DWORD dwScoreTotal,
	WORD wRewardPoint )
{	
	m_nSessionID	= nSessionID;

	m_dwChaNum		= dwChaNum;
	StringCchCopy( m_szName, CHAR_SZNAME+1, szName );
	m_wSchool		= wSchool;
	m_wClass		= wClass;

	m_wRankAll		= wRankAll;
	m_wRankSchool	= wRankSchool;

	m_wScoreKill	= wScoreKill;
	m_wScoreDeath	= wScoreDeath;
	m_dwScoreDamage = dwScoreDamage;
	m_dwScoreHeal	= dwScoreHeal;
	m_wScoreResu	= wScoreResu;
	m_dwScoreTotal	= dwScoreTotal;
	m_wRewardPoint	= wRewardPoint;
}

/* tyranny result log, Juver, 2021/09/09 */
int CLogTyrannyParticipant::Execute (CServer* pServer)
{
	if (pServer == NULL)	return DB_ERROR;

	return m_pDbManager->LogTyrannyParticipant( 
		m_nSessionID, 
		m_dwChaNum, 
		m_szName, 
		m_wSchool, 
		m_wClass, 
		m_wRankAll, 
		m_wRankSchool, 
		m_wScoreKill,
		m_wScoreDeath,
		m_dwScoreDamage, 
		m_dwScoreHeal,
		m_wScoreResu,
		m_dwScoreTotal, 
		m_wRewardPoint );
}