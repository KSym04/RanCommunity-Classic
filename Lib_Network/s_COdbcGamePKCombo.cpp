#include "stdafx.h"
#include "./s_COdbcManager.h"
#include "./s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool COdbcManager::PKComboCheck(int SGNum, DWORD dwChaNum)
{
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call PKComboCheck(%u,?)}", dwChaNum );

	if (m_pGameDB->ExecuteSpInt(szTemp) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int COdbcManager::PKComboMake(int SGNum, DWORD dwChaNum )
{	
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call PKComboMake(%d,%u,?)}", SGNum, dwChaNum );

	int nReturn = m_pGameDB->ExecuteSpInt(szTemp);

	return nReturn;
}


int COdbcManager::PKComboRead( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{    
	if ( !pChaData2 )	return DB_ERROR;

	bool bRead = PKComboCheck(SGNum, dwChaNum);
	if (!bRead)
	{
		PKComboMake(SGNum, dwChaNum );
	}

	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn) return DB_ERROR;

	SQLRETURN sReturn = 0;
	int nUserNum = 0;

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "SELECT PKCombo00, PKCombo01, PKCombo02, PKCombo03, PKCombo04, PKCombo05, PKCombo06, PKCombo07 FROM ChaPKCombo WHERE ChaNum=%u ", pChaData2->m_dwCharID );

	sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS );

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	SQLINTEGER  nPKCombo00, cbPKCombo00 = SQL_NTS;
	SQLINTEGER  nPKCombo01, cbPKCombo01 = SQL_NTS;
	SQLINTEGER  nPKCombo02, cbPKCombo02 = SQL_NTS;
	SQLINTEGER  nPKCombo03, cbPKCombo03 = SQL_NTS;
	SQLINTEGER  nPKCombo04, cbPKCombo04 = SQL_NTS;
	SQLINTEGER  nPKCombo05, cbPKCombo05 = SQL_NTS;
	SQLINTEGER  nPKCombo06, cbPKCombo06 = SQL_NTS;
	SQLINTEGER  nPKCombo07, cbPKCombo07 = SQL_NTS;

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
			::SQLGetData(pConn->hStmt,1, SQL_C_LONG, &nPKCombo00, 0, &cbPKCombo00 );	
			::SQLGetData(pConn->hStmt,2, SQL_C_LONG, &nPKCombo01, 0, &cbPKCombo01 );	
			::SQLGetData(pConn->hStmt,3, SQL_C_LONG, &nPKCombo02, 0, &cbPKCombo02 );	
			::SQLGetData(pConn->hStmt,4, SQL_C_LONG, &nPKCombo03, 0, &cbPKCombo03 );	
			::SQLGetData(pConn->hStmt,5, SQL_C_LONG, &nPKCombo04, 0, &cbPKCombo04 );	
			::SQLGetData(pConn->hStmt,6, SQL_C_LONG, &nPKCombo05, 0, &cbPKCombo05 );	
			::SQLGetData(pConn->hStmt,7, SQL_C_LONG, &nPKCombo06, 0, &cbPKCombo06 );	
			::SQLGetData(pConn->hStmt,8, SQL_C_LONG, &nPKCombo07, 0, &cbPKCombo07 );	

			pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_00] = (DWORD)nPKCombo00;
			pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_01] = (DWORD)nPKCombo01;
			pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_02] = (DWORD)nPKCombo02;
			pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_03] = (DWORD)nPKCombo03;
			pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_04] = (DWORD)nPKCombo04;
			pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_05] = (DWORD)nPKCombo05;
			pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_06] = (DWORD)nPKCombo06;
			pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_07] = (DWORD)nPKCombo07;
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

int COdbcManager::PKComboWrite( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{
	if ( !pChaData2 )	return DB_ERROR;

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "Update ChaPKCombo Set PKCombo00=%u, PKCombo01=%u, PKCombo02=%u, PKCombo03=%u, PKCombo04=%u, PKCombo05=%u, PKCombo06=%u, PKCombo07=%u  WHERE ChaNum=%u ", 
		pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_00],
		pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_01],
		pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_02],
		pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_03],
		pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_04],
		pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_05],
		pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_06],
		pChaData2->m_sPKComboCount.dwCount[EMPKCOMBO_COUNT_07],
		pChaData2->m_dwCharID );

	int nReturn = m_pGameDB->ExecuteSQL(szTemp);

	return nReturn;
}