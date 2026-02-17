#include "stdafx.h"

#include "./s_COdbcManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/* login device check, Juver, 2020/11/06 */
int COdbcManager::UserAccountPageLoginDeviceCheckSet( 
	DWORD dwUserNum, 
	const TCHAR* szUser, 
	const TCHAR* szPass, 
	bool bUserLoginDeviceCheck,
	int nSvrGrp, 
	int nSvrNum
	)
{
	CString strUser(szUser);
	strUser.Trim(_T(" "));
	strUser.Replace(_T("'"), _T("''"));

	CString strPass(szPass);
	strPass.Trim(_T(" "));
	strPass.Replace(_T("'"), _T("''"));

	TCHAR szTemp[2048] = {0};
	_snprintf( szTemp, 2048, "{call user_account_page_login_device_check_set( %d,'%s','%s',%d,%d,%d,?)}", 
		dwUserNum,
		strUser.GetString(),
		strPass.GetString(),
		bUserLoginDeviceCheck,
		nSvrGrp,
		nSvrNum );

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}