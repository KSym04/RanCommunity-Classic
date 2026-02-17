#include "stdafx.h"
#include "./s_COdbcManager.h"
#include "./s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool COdbcManager::PersonalLockCheck(int SGNum, DWORD dwUserNum)
{
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call PersonalLockCheck(%u,?)}", dwUserNum );

	if (m_pGameDB->ExecuteSpInt(szTemp) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int COdbcManager::PersonalLockMake(int SGNum, DWORD dwUserNum)
{	
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call PersonalLockMake(%d,%u,?)}", SGNum, dwUserNum );

	int nReturn = m_pGameDB->ExecuteSpInt(szTemp);

	return nReturn;
}

int COdbcManager::PersonalLockRead( int SGNum, DWORD dwUserNum, SCHARDATA2* pCharData2 )
{    
	bool bRead = PersonalLockCheck(SGNum, dwUserNum);
	if (!bRead)
	{
		PersonalLockMake(SGNum, dwUserNum);
	}

	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn) return DB_ERROR;

	SQLRETURN sReturn = 0;
	int nUserNum = 0;

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "SELECT UserLockPuton, UserLockInven, UserLockLocker, UserPinPuton, UserPinInven, UserPinLocker FROM UserPersonalLock WHERE UserNum=%u", pCharData2->GetUserID() );

	sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS );

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	SQLINTEGER  nChaLockPuton, cbChaLockPuton = SQL_NTS;
	SQLINTEGER  nChaLockInven, cbChaLockInven = SQL_NTS;
	SQLINTEGER  nChaLockLocker, cbChaLockLocker = SQL_NTS;

	SQLCHAR    szChaPinPuton[PERSONAL_LOCK_PIN_SIZE+1] = {0}; SQLINTEGER cbChaPinPuton = SQL_NTS;
	SQLCHAR    szChaPinInven[PERSONAL_LOCK_PIN_SIZE+1] = {0}; SQLINTEGER cbChaPinInven = SQL_NTS;
	SQLCHAR    szChaPinLocker[PERSONAL_LOCK_PIN_SIZE+1] = {0}; SQLINTEGER cbChaPinLocker = SQL_NTS;

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
			::SQLGetData(pConn->hStmt,1, SQL_C_LONG, &nChaLockPuton, 0, &cbChaLockPuton );	
			::SQLGetData(pConn->hStmt,2, SQL_C_LONG, &nChaLockInven, 0, &cbChaLockInven );	
			::SQLGetData(pConn->hStmt,3, SQL_C_LONG, &nChaLockLocker, 0, &cbChaLockLocker );	
			::SQLGetData(pConn->hStmt,4, SQL_C_CHAR, szChaPinPuton, PERSONAL_LOCK_PIN_SIZE+1, &cbChaPinPuton ); 
			::SQLGetData(pConn->hStmt,5, SQL_C_CHAR, szChaPinInven, PERSONAL_LOCK_PIN_SIZE+1, &cbChaPinInven ); 
			::SQLGetData(pConn->hStmt,6, SQL_C_CHAR, szChaPinLocker, PERSONAL_LOCK_PIN_SIZE+1, &cbChaPinLocker ); 

			pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_EQUIP].bLock = cast_bool((BOOL)nChaLockPuton);
			pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_INVEN].bLock = cast_bool((BOOL)nChaLockInven);
			pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_LOCKER].bLock = cast_bool((BOOL)nChaLockLocker);

			if (cbChaPinPuton != 0 && cbChaPinPuton != -1) 
				::StringCchCopy(pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_EQUIP].szPin, PERSONAL_LOCK_PIN_SIZE+1, (const char*) szChaPinPuton);

			if (cbChaPinInven != 0 && cbChaPinInven != -1) 
				::StringCchCopy(pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_INVEN].szPin, PERSONAL_LOCK_PIN_SIZE+1, (const char*) szChaPinInven);

			if (cbChaPinLocker != 0 && cbChaPinLocker != -1) 
				::StringCchCopy(pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_LOCKER].szPin, PERSONAL_LOCK_PIN_SIZE+1, (const char*) szChaPinLocker);
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

int COdbcManager::PersonalLockWrite( int SGNum, DWORD dwUserNum, SCHARDATA2* pCharData2 )
{
	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "Update UserPersonalLock Set UserLockPuton=%u, UserLockInven=%u, UserLockLocker=%u, UserPinPuton='%s', UserPinInven='%s', UserPinLocker='%s' WHERE UserNum=%d", 
		pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_EQUIP].bLock,
		pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_INVEN].bLock,
		pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_LOCKER].bLock,
		pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_EQUIP].szPin,
		pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_INVEN].szPin,
		pCharData2->m_sPersonalLock[EMPERSONAL_LOCK_LOCKER].szPin,
		dwUserNum );

	int nReturn = m_pGameDB->ExecuteSQL(szTemp);

	return nReturn;
}