#include "stdafx.h"

#include "../Lib_Client/G-Logic/GLCharData.h"

#include "./s_COdbcManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int COdbcManager::LoadUserDBEtcCharData( SCHARDATA2* pCharData2 )
{
	if ( pCharData2 == NULL )
		return DB_ERROR;

	SQLRETURN sReturn = 0;
	ODBC_STMT* pConn = m_pUserDB->GetConnection();
	if (!pConn) 
		return DB_ERROR;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "SELECT ExchangeItemPoints, UserVIP FROM UserInfo WHERE UserNum=%d", pCharData2->GetUserID() );

	sReturn = ::SQLExecDirect(pConn->hStmt,(SQLCHAR*)szTemp,SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);		
		Print(GetErrorString(pConn->hStmt));
		m_pUserDB->FreeConnection(pConn);
		return DB_ERROR;
	}

	SQLINTEGER nExchangeItemPoints = 0, cbExchangeItemPoints = SQL_NTS;
	SQLINTEGER nUserVIP = 0, cbUserVIP = SQL_NTS;

	while (true)
	{
		sReturn = ::SQLFetch(pConn->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
		{
			Print(szTemp);		
			Print(GetErrorString(pConn->hStmt));
			m_pUserDB->FreeConnection(pConn);
			return DB_ERROR;
		}

		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{			
			::SQLGetData(pConn->hStmt, 1, SQL_C_LONG, &nExchangeItemPoints, 0, &cbExchangeItemPoints);
			::SQLGetData(pConn->hStmt, 2, SQL_C_LONG, &nUserVIP, 0, &cbUserVIP);

			pCharData2->m_wExchangeItemPoints = (WORD)nExchangeItemPoints;
			pCharData2->m_emVIPLevel = static_cast<EMVIP_LEVEL>(nUserVIP);
		}
		else
		{
			break;
		}

		Sleep( 0 );
	}

	m_pUserDB->FreeConnection(pConn);

	return DB_OK;
}

int COdbcManager::SaveUserDBEtcCharData( SCHARDATA2* pCharData2 )
{
	if ( pCharData2 == NULL )
		return DB_ERROR;

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "UPDATE UserInfo SET ExchangeItemPoints=%d, UserVIP=%d WHERE UserNum=%u"
		, (int)pCharData2->m_wExchangeItemPoints
		, (int)pCharData2->m_emVIPLevel
		, pCharData2->GetUserID() );

	if ( m_pUserDB->ExecuteSQL(szTemp) != DB_OK )
		return DB_ERROR;
	else
		return DB_OK;
}