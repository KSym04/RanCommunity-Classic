#include "stdafx.h"
#include "./s_COdbcManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/* login device check, Juver, 2020/11/06 */
int COdbcManager::UserLoginDeviceCheckVerifyRequest( const TCHAR* szUser, const TCHAR* szPass, int nSvrGrp, int nSvrNum, const TCHAR* szIP, const TCHAR* szPCID_HWID,const TCHAR* szPCID_MAC )
{
	CString strUser(szUser);
	strUser.Trim(_T(" "));
	strUser.Replace(_T("'"), _T("''"));

	CString strPass(szPass);
	strPass.Trim(_T(" "));
	strPass.Replace(_T("'"), _T("''"));

	CString strIP = szIP;
	strIP.Trim(_T(" "));
	strIP.Replace(_T("'"), _T("''"));

	//PCID
	CString strPCID_HWID = szPCID_HWID;
	strPCID_HWID.Trim(_T(" "));
	strPCID_HWID.Replace(_T("'"), _T("''"));

	CString strPCID_MAC = szPCID_MAC;
	strPCID_MAC.Trim(_T(" "));
	strPCID_MAC.Replace(_T("'"), _T("''"));

	TCHAR szTemp[2048] = {0};
	_snprintf( szTemp, 2048, "{call user_login_device_check_verify_request('%s','%s', %d, %d,'%s','%s','%s',?)}", 
		strUser.GetString(),
		strPass.GetString(),
		nSvrGrp,
		nSvrNum,
		strIP.GetString(),
		strPCID_HWID.GetString(),
		strPCID_MAC.GetString()
		);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}