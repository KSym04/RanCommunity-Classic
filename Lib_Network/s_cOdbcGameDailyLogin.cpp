#include "stdafx.h"
#include "./s_COdbcManager.h"
#include "./s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int COdbcManager::DailyLoginRead( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{    
	if ( !pChaData2 )	
		return DB_ERROR;

	bool bRead = CombatRecordCheck(SGNum, dwChaNum);
	if (!bRead)
	{
		CombatRecordMake(SGNum, dwChaNum, pChaData2 );
	}
	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn) 
		return DB_ERROR;

	SQLRETURN sReturn = 0;
	int nUserNum = 0;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, 
		"SELECT "
		"ChaDailyLoginStreak "

		"FROM ChaInfo WHERE ChaNum=%u ", pChaData2->m_dwCharID );

	sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS );

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	SQLINTEGER	nChaDailyLoginStreak, cbChaDailyLoginStreak	= SQL_NTS;

	while(true)
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
			::SQLGetData(pConn->hStmt,1, SQL_C_LONG, &nChaDailyLoginStreak, 0, &cbChaDailyLoginStreak );	

			pChaData2->m_dwDayStreakCounter = static_cast<DWORD>(nChaDailyLoginStreak);
			
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

int COdbcManager::DailyLoginWrite( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{
	if ( !pChaData2 )	return DB_ERROR;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, 
		"Update ChaInfo Set "
		"ChaDailyLoginStreak=%u "

		"WHERE ChaNum=%u ", 

		pChaData2->m_dwDayStreakCounter,
		pChaData2->m_dwCharID );

	int nReturn = m_pGameDB->ExecuteSQL(szTemp);

	return nReturn;
}