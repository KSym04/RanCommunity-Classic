#include "stdafx.h"
#include "./s_COdbcManager.h"
#include "./s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*gm command logs, Juver, 2018/08/15 */
int COdbcManager::gm_cmd_log_insert ( DWORD user_id, DWORD user_type, DWORD char_id, char* char_name, char* command_info )
{
	SQLRETURN	sReturn = 0;	

	CString name (char_name);
	name.Trim(_T(" ")); 
	name.Replace(_T("'"), _T("''"));

	CString command (command_info);
	command.Trim(_T(" ")); 
	command.Replace(_T("'"), _T("''"));
	//command.Replace(_T(","), _T(" "));

	if (name.GetLength() >= (GM_CMD_LOG_NAME_TEXT_SIZE+1))			return DB_ERROR;
	if (command.GetLength() >= (GM_CMD_LOG_COMMAND_TEXT_SIZE+1))	return DB_ERROR;
	
	TCHAR szTemp[2048] = {0};
	_snprintf( szTemp, 2048, "{call sp_gm_logs_insert(%u,%u,%u,'%s','%s',?)}", user_id, user_type, char_id, name.GetString(), command.GetString() ); 

	int nReturn = m_pLogDB->ExecuteSpInt(szTemp);
	return nReturn;
}

/* log gm party, Juver, 2021/06/13 */
int COdbcManager::LogGMParty ( DWORD dwGMUserNum, DWORD dwGMChaNum, char* szGMChaName, DWORD dwGMUserType, DWORD dwTargetUserNum, DWORD dwTargetChaNum, char* szTargetChaName, DWORD dwTargetUserType )
{
	CString strGMChaName (szGMChaName);
	strGMChaName.Trim(_T(" ")); 
	strGMChaName.Replace(_T("'"), _T("''"));

	CString strTargetChaName (szTargetChaName);
	strTargetChaName.Trim(_T(" ")); 
	strTargetChaName.Replace(_T("'"), _T("''"));


	TCHAR szTemp[2048] = {0};
	_snprintf( szTemp, 2048, "{call sp_LogGMParty(%u,%u,'%s',%u,%u,%u,'%s',%u,?)}", dwGMUserNum, dwGMChaNum, strGMChaName.GetString(), dwGMUserType, dwTargetUserNum, dwTargetChaNum, strTargetChaName.GetString(), dwTargetUserType ); 

	int nReturn = m_pLogDB->ExecuteSpInt(szTemp);
	return nReturn;
}