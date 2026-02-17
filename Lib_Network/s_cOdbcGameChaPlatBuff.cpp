#include "stdafx.h"
#include "./s_COdbcManager.h"
#include "./s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int COdbcManager::PlatBuffRead( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{    
	if ( !pChaData2 )	
		return DB_ERROR;

	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn) 
		return DB_ERROR;

	SQLRETURN sReturn = 0;
	int nUserNum = 0;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, 
		"SELECT "
		"ChaPlatinumUse, ChaPlatinumAttack, "
		"ChaPlatinumDefense, ChaPlatinumMelee, "
		"ChaPlatinumShooting, ChaPlatinumEnergy, "
		"ChaPlatinumHitRate, ChaPlatinumAvoid "

		"FROM ChaInfo WHERE ChaNum=%u ", pChaData2->m_dwCharID );

	sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS );

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	SQLINTEGER  nPlatinumUsing, cbPlatinumUsing = SQL_NTS;
	//TIMESTAMP_STRUCT sPlatinumExpiryTime; SQLINTEGER cbPlatinumExpiryTime = SQL_NTS;

	SQLINTEGER  nPlatinumAttack, cbPlatinumAttack = SQL_NTS;
	SQLINTEGER  nPlatinumDefense, cbPlatinumDefense = SQL_NTS;
	SQLINTEGER  nPlatinumMelee, cbPlatinumMelee = SQL_NTS;
	SQLINTEGER  nPlatinumShooting, cbPlatinumShooting = SQL_NTS;
	SQLINTEGER  nPlatinumEnergy, cbPlatinumEnergy = SQL_NTS;
	SQLINTEGER  nPlatinumHitrate, cbPlatinumHitrate = SQL_NTS;
	SQLINTEGER  nPlatinumAvoid, cbPlatinumAvoid = SQL_NTS;

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
			::SQLGetData(pConn->hStmt,1, SQL_C_LONG, &nPlatinumUsing, 0, &cbPlatinumUsing );
			::SQLGetData(pConn->hStmt,2, SQL_C_LONG, &nPlatinumAttack, 0, &cbPlatinumAttack );
			::SQLGetData(pConn->hStmt,3, SQL_C_LONG, &nPlatinumDefense, 0, &cbPlatinumDefense );
			::SQLGetData(pConn->hStmt,4, SQL_C_LONG, &nPlatinumMelee, 0, &cbPlatinumMelee );
			::SQLGetData(pConn->hStmt,5, SQL_C_LONG, &nPlatinumShooting, 0, &cbPlatinumShooting );
			::SQLGetData(pConn->hStmt,6, SQL_C_LONG, &nPlatinumEnergy, 0, &cbPlatinumEnergy );
			::SQLGetData(pConn->hStmt,7, SQL_C_LONG, &nPlatinumHitrate, 0, &cbPlatinumHitrate );
			::SQLGetData(pConn->hStmt,8, SQL_C_LONG, &nPlatinumAvoid, 0, &cbPlatinumAvoid );

			pChaData2->m_bPlatinumUsing = static_cast<bool>(nPlatinumUsing);
			pChaData2->m_dwPlatinumAttack = static_cast<DWORD>(nPlatinumAttack);
			pChaData2->m_dwPlatinumDefense = static_cast<DWORD>(nPlatinumDefense);
			pChaData2->m_dwPlatinumShooting = static_cast<DWORD>(nPlatinumShooting);
			pChaData2->m_dwPlatinumMelee = static_cast<DWORD>(nPlatinumMelee);
			pChaData2->m_dwPlatinumEnergy = static_cast<DWORD>(nPlatinumEnergy);
			pChaData2->m_dwPlatinumHitrate = static_cast<DWORD>(nPlatinumHitrate);
			pChaData2->m_dwPlatinumAvoidrate = static_cast<DWORD>(nPlatinumAvoid);
			
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

int COdbcManager::PlatBuffWrite( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{
	if ( !pChaData2 )	return DB_ERROR;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, 
		"Update ChaInfo Set "
		"ChaPlatinumUse=%u, ChaPlatinumAttack=%u, ChaPlatinumDefense=%u, "
		"ChaPlatinumMelee=%u, ChaPlatinumShooting=%u, "
		"ChaPlatinumEnergy=%u, "
		"ChaPlatinumHitRate=%u, ChaPlatinumAvoid=%u "
		"WHERE ChaNum=%u ", 
		pChaData2->m_bPlatinumUsing,
		pChaData2->m_dwPlatinumAttack,
		pChaData2->m_dwPlatinumDefense,
		pChaData2->m_dwPlatinumShooting,
		pChaData2->m_dwPlatinumMelee,
		pChaData2->m_dwPlatinumEnergy,
		pChaData2->m_dwPlatinumHitrate,
		pChaData2->m_dwPlatinumAvoidrate,
		pChaData2->m_dwCharID );

	int nReturn = m_pGameDB->ExecuteSQL(szTemp);

	return nReturn;
}