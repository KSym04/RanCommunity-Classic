#include "stdafx.h"
#include "./s_COdbcManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool COdbcManager::CombatRecordCheck(int SGNum, DWORD dwChaNum)
{
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call sp_CombatRecordCheck(%u,?)}", dwChaNum );

	if (m_pGameDB->ExecuteSpInt(szTemp) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int COdbcManager::CombatRecordMake(int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{	
	//when creating new table row, insert the previous PKScore and PKDeath
	//this is to migrate previous data
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call sp_CombatRecordMake(%d,%u,%u,%u,?)}", SGNum, dwChaNum, pChaData2->m_dwPKScorePREV, pChaData2->m_dwPKDeathPREV );

	int nReturn = m_pGameDB->ExecuteSpInt(szTemp);

	return nReturn;
}

int COdbcManager::CombatRecordRead( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
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

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, 
		"SELECT "
		"PVPKills, PVPDeaths, PVPResu, "
		"TyrannyKills, TyrannyDeaths, TyrannyResu, "
		"ClubWarKills, ClubWarDeaths, ClubWarResu, "
		"CaptureTheFlagKills, CaptureTheFlagDeaths, CaptureTheFlagResu, "
		"SchoolWarsKills, SchoolWarsDeaths, SchoolWarsResu, "
		"ClubDeathMatchKills, ClubDeathMatchDeaths, ClubDeathMatchResu, "
		"PKMapKills, PKMapDeaths, PKMapResu, "
		"PVEKills, PVEDeaths, PVEMoneyGain "
		"FROM ChaCombatRecord WHERE ChaNum=%u ", pChaData2->m_dwCharID );

	sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS );

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	SQLINTEGER  nPVPKills, cbPVPKills = SQL_NTS;
	SQLINTEGER  nPVPDeaths, cbPVPDeaths = SQL_NTS;
	SQLINTEGER  nPVPResu, cbPVPResu = SQL_NTS;
	
	SQLINTEGER  nTyrannyKills, cbTyrannyKills = SQL_NTS;
	SQLINTEGER  nTyrannyDeaths, cbTyrannyDeaths = SQL_NTS;
	SQLINTEGER  nTyrannyResu, cbTyrannyResu = SQL_NTS;

	SQLINTEGER  nClubWarKills, cbClubWarKills = SQL_NTS;
	SQLINTEGER  nClubWarDeaths, cbClubWarDeaths = SQL_NTS;
	SQLINTEGER  nClubWarResu, cbClubWarResu = SQL_NTS;

	SQLINTEGER  nCaptureTheFlagKills, cbCaptureTheFlagKills = SQL_NTS;
	SQLINTEGER  nCaptureTheFlagDeaths, cbCaptureTheFlagDeaths = SQL_NTS;
	SQLINTEGER  nCaptureTheFlagResu, cbCaptureTheFlagResu = SQL_NTS;

	SQLINTEGER  nSchoolWarsKills, cbSchoolWarsKills = SQL_NTS;
	SQLINTEGER  nSchoolWarsDeaths, cbSchoolWarsDeaths = SQL_NTS;
	SQLINTEGER  nSchoolWarsResu, cbSchoolWarsResu = SQL_NTS;

	SQLINTEGER  nClubDeathMatchKills, cbClubDeathMatchKills = SQL_NTS;
	SQLINTEGER  nClubDeathMatchDeaths, cbClubDeathMatchDeaths = SQL_NTS;
	SQLINTEGER  nClubDeathMatchResu, cbClubDeathMatchResu = SQL_NTS;

	SQLINTEGER  nPKMapKills, cbPKMapKills = SQL_NTS;
	SQLINTEGER  nPKMapDeaths, cbPKMapDeaths = SQL_NTS;
	SQLINTEGER  nPKMapResu, cbPKMapResu = SQL_NTS;

	SQLINTEGER  nPVEKills, cbPVEKills = SQL_NTS;
	SQLINTEGER  nPVEDeaths, cbPVEDeaths = SQL_NTS;

	LONGLONG   llPVEMoneyGain = 0;
	SQLINTEGER cbPVEMoneyGain=SQL_NTS; 

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
			::SQLGetData(pConn->hStmt,1, SQL_C_LONG, &nPVPKills, 0, &cbPVPKills );	
			::SQLGetData(pConn->hStmt,2, SQL_C_LONG, &nPVPDeaths, 0, &cbPVPDeaths );	
			::SQLGetData(pConn->hStmt,3, SQL_C_LONG, &nPVPResu, 0, &cbPVPResu );	

			::SQLGetData(pConn->hStmt,4, SQL_C_LONG, &nTyrannyKills, 0, &cbTyrannyKills );	
			::SQLGetData(pConn->hStmt,5, SQL_C_LONG, &nTyrannyDeaths, 0, &cbTyrannyDeaths );	
			::SQLGetData(pConn->hStmt,6, SQL_C_LONG, &nTyrannyResu, 0, &cbTyrannyResu );	

			::SQLGetData(pConn->hStmt,7, SQL_C_LONG, &nClubWarKills, 0, &cbClubWarKills );	
			::SQLGetData(pConn->hStmt,8, SQL_C_LONG, &nClubWarDeaths, 0, &cbClubWarDeaths );	
			::SQLGetData(pConn->hStmt,9, SQL_C_LONG, &nClubWarResu, 0, &cbClubWarResu );	

			::SQLGetData(pConn->hStmt,10, SQL_C_LONG, &nCaptureTheFlagKills, 0, &cbCaptureTheFlagKills );	
			::SQLGetData(pConn->hStmt,11, SQL_C_LONG, &nCaptureTheFlagDeaths, 0, &cbCaptureTheFlagDeaths );	
			::SQLGetData(pConn->hStmt,12, SQL_C_LONG, &nCaptureTheFlagResu, 0, &cbCaptureTheFlagResu );	

			::SQLGetData(pConn->hStmt,13, SQL_C_LONG, &nSchoolWarsKills, 0, &cbSchoolWarsKills );	
			::SQLGetData(pConn->hStmt,14, SQL_C_LONG, &nSchoolWarsDeaths, 0, &cbSchoolWarsDeaths );	
			::SQLGetData(pConn->hStmt,15, SQL_C_LONG, &nSchoolWarsResu, 0, &cbSchoolWarsResu );	

			::SQLGetData(pConn->hStmt,16, SQL_C_LONG, &nClubDeathMatchKills, 0, &cbClubDeathMatchKills );	
			::SQLGetData(pConn->hStmt,17, SQL_C_LONG, &nClubDeathMatchDeaths, 0, &cbClubDeathMatchDeaths );	
			::SQLGetData(pConn->hStmt,18, SQL_C_LONG, &nClubDeathMatchResu, 0, &cbClubDeathMatchResu );	

			::SQLGetData(pConn->hStmt,19, SQL_C_LONG, &nPKMapKills, 0, &cbPKMapKills );	
			::SQLGetData(pConn->hStmt,20, SQL_C_LONG, &nPKMapDeaths, 0, &cbPKMapDeaths );	
			::SQLGetData(pConn->hStmt,21, SQL_C_LONG, &nPKMapResu, 0, &cbPKMapResu );	

			::SQLGetData(pConn->hStmt,22, SQL_C_LONG, &nPVEKills, 0, &cbPVEKills );	
			::SQLGetData(pConn->hStmt,23, SQL_C_LONG, &nPVEDeaths, 0, &cbPVEDeaths );	
			::SQLGetData(pConn->hStmt,24, SQL_C_SBIGINT, &llPVEMoneyGain, 0, &cbPVEMoneyGain);

			pChaData2->m_sCombatRecord.dwPVPKills = static_cast<DWORD>(nPVPKills);
			pChaData2->m_sCombatRecord.dwPVPDeaths = static_cast<DWORD>(nPVPDeaths);
			pChaData2->m_sCombatRecord.dwPVPResu = static_cast<DWORD>(nPVPResu);

			pChaData2->m_sCombatRecord.dwTyrannyKills = static_cast<DWORD>(nTyrannyKills);
			pChaData2->m_sCombatRecord.dwTyrannyDeaths = static_cast<DWORD>(nTyrannyDeaths);
			pChaData2->m_sCombatRecord.dwTyrannyResu = static_cast<DWORD>(nTyrannyResu);

			pChaData2->m_sCombatRecord.dwClubWarKills = static_cast<DWORD>(nClubWarKills);
			pChaData2->m_sCombatRecord.dwClubWarDeaths = static_cast<DWORD>(nClubWarDeaths);
			pChaData2->m_sCombatRecord.dwClubWarResu = static_cast<DWORD>(nClubWarResu);

			pChaData2->m_sCombatRecord.dwCaptureTheFlagKills = static_cast<DWORD>(nCaptureTheFlagKills);
			pChaData2->m_sCombatRecord.dwCaptureTheFlagDeaths = static_cast<DWORD>(nCaptureTheFlagDeaths);
			pChaData2->m_sCombatRecord.dwCaptureTheFlagResu = static_cast<DWORD>(nCaptureTheFlagResu);

			pChaData2->m_sCombatRecord.dwSchoolWarsKills = static_cast<DWORD>(nSchoolWarsKills);
			pChaData2->m_sCombatRecord.dwSchoolWarsDeaths = static_cast<DWORD>(nSchoolWarsDeaths);
			pChaData2->m_sCombatRecord.dwSchoolWarsResu = static_cast<DWORD>(nSchoolWarsResu);

			pChaData2->m_sCombatRecord.dwClubDeathMatchKills = static_cast<DWORD>(nClubDeathMatchKills);
			pChaData2->m_sCombatRecord.dwClubDeathMatchDeaths = static_cast<DWORD>(nClubDeathMatchDeaths);
			pChaData2->m_sCombatRecord.dwClubDeathMatchResu = static_cast<DWORD>(nClubDeathMatchResu);

			pChaData2->m_sCombatRecord.dwPKMapKills = static_cast<DWORD>(nPKMapKills);
			pChaData2->m_sCombatRecord.dwPKMapDeaths = static_cast<DWORD>(nPKMapDeaths);
			pChaData2->m_sCombatRecord.dwPKMapResu = static_cast<DWORD>(nPKMapResu);

			pChaData2->m_sCombatRecord.dwPVEKills = static_cast<DWORD>(nPVEKills);
			pChaData2->m_sCombatRecord.dwPVEDeaths = static_cast<DWORD>(nPVEDeaths);
			pChaData2->m_sCombatRecord.llPVEMoneyGain = static_cast<LONGLONG>(llPVEMoneyGain);
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


int COdbcManager::CombatRecordWrite( int SGNum, DWORD dwChaNum, SCHARDATA2* pChaData2 )
{
	if ( !pChaData2 )	return DB_ERROR;

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, 
		"Update ChaCombatRecord Set "
		"PVPKills=%u, PVPDeaths=%u, PVPResu=%u, "
		"TyrannyKills=%u, TyrannyDeaths=%u, TyrannyResu=%u, "
		"ClubWarKills=%u, ClubWarDeaths=%u, ClubWarResu=%u, "
		"CaptureTheFlagKills=%u, CaptureTheFlagDeaths=%u, CaptureTheFlagResu=%u, "
		"SchoolWarsKills=%u, SchoolWarsDeaths=%u, SchoolWarsResu=%u, "
		"ClubDeathMatchKills=%u, ClubDeathMatchDeaths=%u, ClubDeathMatchResu=%u, "
		"PKMapKills=%u, PKMapDeaths=%u, PKMapResu=%u, "
		"PVEKills=%u, PVEDeaths=%u, PVEMoneyGain=%I64d "   
		"WHERE ChaNum=%u ", 
		pChaData2->m_sCombatRecord.dwPVPKills, pChaData2->m_sCombatRecord.dwPVPDeaths, pChaData2->m_sCombatRecord.dwPVPResu,
		pChaData2->m_sCombatRecord.dwTyrannyKills, pChaData2->m_sCombatRecord.dwTyrannyDeaths, pChaData2->m_sCombatRecord.dwTyrannyResu,
		pChaData2->m_sCombatRecord.dwClubWarKills, pChaData2->m_sCombatRecord.dwClubWarDeaths, pChaData2->m_sCombatRecord.dwClubWarResu,
		pChaData2->m_sCombatRecord.dwCaptureTheFlagKills, pChaData2->m_sCombatRecord.dwCaptureTheFlagDeaths, pChaData2->m_sCombatRecord.dwCaptureTheFlagResu,
		pChaData2->m_sCombatRecord.dwSchoolWarsKills, pChaData2->m_sCombatRecord.dwSchoolWarsDeaths, pChaData2->m_sCombatRecord.dwSchoolWarsResu,
		pChaData2->m_sCombatRecord.dwClubDeathMatchKills, pChaData2->m_sCombatRecord.dwClubDeathMatchDeaths, pChaData2->m_sCombatRecord.dwClubDeathMatchResu,
		pChaData2->m_sCombatRecord.dwPKMapKills, pChaData2->m_sCombatRecord.dwPKMapDeaths, pChaData2->m_sCombatRecord.dwPKMapResu,
		pChaData2->m_sCombatRecord.dwPVEKills, pChaData2->m_sCombatRecord.dwPVEDeaths, pChaData2->m_sCombatRecord.llPVEMoneyGain,
		pChaData2->m_dwCharID );

	int nReturn = m_pGameDB->ExecuteSQL(szTemp);

	return nReturn;
}