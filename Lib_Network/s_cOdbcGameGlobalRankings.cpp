#include "stdafx.h"
#include "./s_COdbcManager.h"
#include "./s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int COdbcManager::GlobalRankingRead( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
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
		"ChaPKScore, ChaPKDeath, ChaMMR, "
		"ChaResu "

		"FROM ChaInfo WHERE ChaNum=%u ", pChaData2->m_dwCharID );

	sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS );

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	SQLINTEGER	nChaPKScore, cbChaPKScore	= SQL_NTS;
	SQLINTEGER	nChaPKDeath, cbChaPKDeath = SQL_NTS;
	SQLINTEGER	nChaMMR, cbChaMMR	= SQL_NTS;
	SQLINTEGER  nChaResu, cbChaResu	= SQL_NTS;

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
			::SQLGetData(pConn->hStmt,1, SQL_C_LONG, &nChaPKScore, 0, &cbChaPKScore );	
			::SQLGetData(pConn->hStmt,2, SQL_C_LONG, &nChaPKDeath, 0, &cbChaPKDeath );	
			::SQLGetData(pConn->hStmt,3, SQL_C_LONG, &nChaMMR, 0, &cbChaMMR );	
			::SQLGetData(pConn->hStmt,4, SQL_C_LONG, &nChaResu, 0, &cbChaResu );	

			pChaData2->m_dwPKScore = static_cast<DWORD>(nChaPKScore);
			pChaData2->m_dwPKDeath = static_cast<DWORD>(nChaPKDeath);
			pChaData2->m_dwMMR = static_cast<DWORD>(nChaMMR);
			pChaData2->m_dwResu = static_cast<DWORD>(nChaResu);
			
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

int COdbcManager::GlobalRankingWrite( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{
	if ( !pChaData2 )	return DB_ERROR;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, 
		"Update ChaInfo Set "
		"ChaPKScore=%u, ChaPKDeath=%u, ChaMMR=%u, "
		"ChaResu=%u  " 
		"WHERE ChaNum=%u ", 
		pChaData2->m_dwPKScore,
		pChaData2->m_dwPKDeath,
		pChaData2->m_dwMMR,
		pChaData2->m_dwResu,
		pChaData2->m_dwCharID );

	int nReturn = m_pGameDB->ExecuteSQL(szTemp);

	return nReturn;
}