#include "stdafx.h"

#include "./s_COdbcManager.h"

#include "../Lib_Engine/Core/NSRParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*register page, Juver, 2017/11/18 */ 
int COdbcManager::UserRegister( 
	const TCHAR* szUser, 
	const TCHAR* szPass, 
	const TCHAR* szPass2, 
	const TCHAR* szSA, 
	const TCHAR* szMail, 
	const TCHAR* szFullName, 
	int nSvrGrp, 
	int nSvrNum,

	const TCHAR* szPCID_HWID,	//PCID
	const TCHAR* szPCID_MAC,	//PCID
	BOOL bRegisterPCIDCheck,	//PCID
	BOOL bRegisterEmailCheck,	//PCID

	DWORD dwReferralUserNum,
	EMREFERRAL_REGISTER_TYPE emRegisterReferral )
{
	CString strUser(szUser);
	strUser.Trim(_T(" "));
	strUser.Replace(_T("'"), _T("''"));

	CString strPass(szPass);
	strPass.Trim(_T(" "));
	strPass.Replace(_T("'"), _T("''"));

	CString strPass2(szPass2);
	strPass2.Trim(_T(" "));
	strPass2.Replace(_T("'"), _T("''"));

	CString strSA(szSA);
	strSA.Trim(_T(" "));
	strSA.Replace(_T("'"), _T("''"));

	CString strMail(szMail);
	strMail.Trim(_T(" "));
	strMail.Replace(_T("'"), _T("''"));

	CString strFullName(szFullName);
	strFullName.Trim(_T(" "));
	strFullName.Replace(_T("'"), _T("''"));

	//PCID
	CString strPCID_HWID = szPCID_HWID;
	strPCID_HWID.Trim(_T(" "));
	strPCID_HWID.Replace(_T("'"), _T("''"));

	CString strPCID_MAC = szPCID_MAC;
	strPCID_MAC.Trim(_T(" "));
	strPCID_MAC.Replace(_T("'"), _T("''"));

	TCHAR szTemp[2048] = {0};

	_snprintf( szTemp, 2048, "{call user_register('%s','%s','%s','%s','%s','%s', %d, %d,'%s','%s', %d, %d, %d, %d,?)}", 
		strUser.GetString(),
		strPass.GetString(),
		strPass2.GetString(),
		strSA.GetString(),
		strMail.GetString(),
		strFullName.GetString(),
		nSvrGrp,
		nSvrNum,

		//PCID
		strPCID_HWID.GetString(),
		strPCID_MAC.GetString(),
		bRegisterPCIDCheck,
		bRegisterEmailCheck,

		emRegisterReferral,
		dwReferralUserNum
		);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}