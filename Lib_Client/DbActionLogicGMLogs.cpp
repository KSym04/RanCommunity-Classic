#include "stdafx.h"
#include "./DbActionLogic.h"
#include "./G-Logic/GLGaeaServer.h"
#include "./G-Logic/GLChar.h"

#include "../Lib_Network/s_CSessionServer.h"
#include "../Lib_Network/s_CFieldServer.h"
#include "../Lib_Network/s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*gm command logs, Juver, 2018/08/15 */
gm_cmd_logs::gm_cmd_logs( DWORD client_id, DWORD user_id, DWORD user_type, DWORD char_id, const char* char_name, const char* command_info )
{	
	m_dwClient = client_id;

	m_user_id = user_id;
	m_user_type = user_type;

	m_char_id = char_id;

	memset(m_char_name, 0, sizeof(char) * GM_CMD_LOG_NAME_TEXT_SIZE+1);
	memset(m_command_info, 0, sizeof(char) * GM_CMD_LOG_COMMAND_TEXT_SIZE+1);

	if (char_name != NULL)
		StringCchCopy(m_char_name, GM_CMD_LOG_NAME_TEXT_SIZE+1, char_name);

	if (command_info != NULL)
		StringCchCopy(m_command_info, GM_CMD_LOG_COMMAND_TEXT_SIZE+1, command_info);

}

/*gm command logs, Juver, 2018/08/15 */
int gm_cmd_logs::Execute (CServer* pServer)
{
	if (pServer == NULL)	return DB_ERROR;

	return m_pDbManager->gm_cmd_log_insert( m_user_id, m_user_type, m_char_id, m_char_name, m_command_info );
}


/* log gm party, Juver, 2021/06/13 */
LogGMParty::LogGMParty( DWORD dwClientID, DWORD dwGMUserNum, DWORD dwGMChaNum, char* szGMChaName, DWORD dwGMUserType, DWORD dwTargetUserNum, DWORD dwTargetChaNum, char* szTargetChaName, DWORD dwTargetUserType )
{	
	m_dwClient = dwClientID;

	m_dwGMUserNum = dwGMUserNum;
	m_dwGMChaNum = dwGMChaNum;
	m_dwGMUserType = dwGMUserType;

	m_dwTargetUserNum = dwTargetUserNum;
	m_dwTargetChaNum = dwTargetChaNum;
	m_dwTargetUserType = dwTargetUserType;

	memset(m_szGMChaName, 0, sizeof(char) * CHAR_SZNAME+1);
	memset(m_szTargetChaName, 0, sizeof(char) * CHAR_SZNAME+1);

	if (szGMChaName != NULL)
		StringCchCopy(m_szGMChaName, CHAR_SZNAME+1, szGMChaName);

	if (szTargetChaName != NULL)
		StringCchCopy(m_szTargetChaName, CHAR_SZNAME+1, szTargetChaName);

}

/* log gm party, Juver, 2021/06/13 */
int LogGMParty::Execute (CServer* pServer)
{
	if (pServer == NULL)	return DB_ERROR;

	return m_pDbManager->LogGMParty( m_dwGMUserNum, m_dwGMChaNum, m_szGMChaName, m_dwGMUserType, m_dwTargetUserNum, m_dwTargetChaNum, m_szTargetChaName, m_dwTargetUserType );
}
