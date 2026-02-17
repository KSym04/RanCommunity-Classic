#include "stdafx.h"
#include "./s_COdbcManager.h"
#include "./s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int COdbcManager::ChaGameRead( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{    
	if ( !pChaData2 )	
		return DB_ERROR;

	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn) 
		return DB_ERROR;

	SQLRETURN sReturn = 0;
	int nUserNum = 0;

	TCHAR szTemp[256] = {0};
	_snprintf( szTemp, 256, 
		"SELECT "
		"ChaReborn "
		"ChaPlayTime, ChaPlayPoint "

		"FROM ChaInfo WHERE ChaNum=%u ", pChaData2->m_dwCharID );

	sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS );

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	LONGLONG   llChaPlayTime = 0;
	SQLINTEGER cbChaPlayTime=SQL_NTS;
	LONGLONG   llChaPlayPoint = 0;
	SQLINTEGER cbChaPlayPoint=SQL_NTS;
	SQLINTEGER nChaReborn = 0, cbChaReborn=SQL_NTS;

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
			::SQLGetData(pConn->hStmt,1, SQL_C_SBIGINT, &llChaPlayTime, 0, &cbChaPlayTime );	
			::SQLGetData(pConn->hStmt,2, SQL_C_SBIGINT, &llChaPlayPoint, 0, &cbChaPlayPoint );	
			::SQLGetData(pConn->hStmt,3, SQL_C_LONG, &nChaReborn,      0, &cbChaReborn);

			pChaData2->m_llPlayTime = static_cast<LONGLONG>(llChaPlayTime);
			pChaData2->m_llPlayPoint = static_cast<LONGLONG>(llChaPlayPoint);
			pChaData2->m_dwReborn = static_cast<DWORD>(nChaReborn);
			
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

int COdbcManager::ChaGameSave( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{
	if ( !pChaData2 )	return DB_ERROR;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, 
		"Update ChaInfo Set "
		"ChaPlayTime=%I64d, ChaPlayPoint=%I64d, ChaReborn=%u "
		"WHERE ChaNum=%u ", 
		pChaData2->m_llPlayTime,
		pChaData2->m_llPlayPoint,
		pChaData2->m_dwReborn,
		pChaData2->m_dwCharID );

	int nReturn = m_pGameDB->ExecuteSQL(szTemp);

	return nReturn;
}