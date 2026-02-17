#include "stdafx.h"
#include "./s_COdbcManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int COdbcManager::PCIDBlock (DWORD dwUserID, DWORD dwCharID, const char* szCharName, DWORD dwType, const char* szHWID, const char* szMAC )
{
	CString strReason("none");
	strReason.Format( "block user:%u char:%u %s", dwUserID, dwCharID, szCharName );

	CString strHWID (szHWID);
	strHWID.Trim(_T(" ")); 
	strHWID.Replace(_T("'"), _T("''"));

	CString strMAC (szMAC);
	strMAC.Trim(_T(" ")); 
	strMAC.Replace(_T("'"), _T("''"));

	TCHAR szTemp[2048] = {0};
	_snprintf( szTemp, 2048, "{call sp_PCIDBlock('%s', '%s', '%s', %u, ?)}", strReason.GetString(), strHWID.GetString(), strMAC.GetString(), dwType ); 

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);
	return nReturn;
}

/* user flag restricted, Juver, 2020/04/21 */
int COdbcManager::UserRestrict (DWORD dwUserID, bool bUserRestrict)
{
	TCHAR szTemp[256] = {0};
	_snprintf( szTemp, 256, "{call sp_UserRestrict(%u, %d, ?)}", dwUserID, bUserRestrict ); 

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);
	return nReturn;
}

/* HWID watchlist, Juver, 2020/05/05 */
int COdbcManager::getUserWatchlist( DWORD dwUserID, const TCHAR* szHWID, const TCHAR* szMAC )
{
	TCHAR szTemp[256] = {0};
	_snprintf( szTemp, 256, "{call HWIDWatchlistCheck('%s', '%s', ?)}", szHWID, szMAC ); 

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);
	return nReturn;
}
