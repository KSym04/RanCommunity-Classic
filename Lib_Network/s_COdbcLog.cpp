#include "stdafx.h"
#include "./s_COdbcManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int COdbcManager::AddLogServerState(
	int nSGNum,
	int nSvrNum,
	int nUserNum,
	int nUserMax )
{
/*
	std::strstream strTemp;
	strTemp << "{call log_serverstate(";
	strTemp << nUserNum << ",";
	strTemp << nUserMax << ",";
	strTemp << nSvrNum << ",";	
	strTemp << nSGNum << ")}";
	strTemp << std::ends;
*/
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call log_serverstate(%d,%d,%d,%d)}", nUserNum, nUserMax, nSvrNum, nSGNum ); 

	int nReturn = m_pLogDB->ExecuteSp(szTemp);
//	strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
	
	return nReturn;
}

int COdbcManager::LogRandomItem(
	const SLOGRANDOMOPTION &_sLOG )
{
/*
	std::strstream strTemp;
	strTemp << "{call sp_LogItemRandom_Insert(";
	strTemp << _sLOG.nNIDMain << ",";
	strTemp << _sLOG.nNIDSub << ",";
	strTemp << _sLOG.nSGNum << ",";	
	strTemp << _sLOG.nSvrNum << ",";
	strTemp << _sLOG.nFldNum << ",";
	strTemp << _sLOG.nMakeType << ",";
	strTemp << _sLOG.lnMakeNum << ",";

	strTemp << (short) _sLOG.cOptTYPE1 << ",";
	strTemp << _sLOG.nOptVALUE1 << ",";

	strTemp << (short) _sLOG.cOptTYPE2 << ",";
	strTemp << _sLOG.nOptVALUE2 << ",";

	strTemp << (short) _sLOG.cOptTYPE3 << ",";
	strTemp << _sLOG.nOptVALUE3 << ",";

	strTemp << (short) _sLOG.cOptTYPE4 << ",";
	strTemp << _sLOG.nOptVALUE4 << ",?)}";

	strTemp << std::ends;
*/

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call sp_LogItemRandom_Insert(%d,%d,%d,%d,%d,%d,%I64d,%d,%d,%d,%d,%d,%d,%d,%d,?)}", 
									_sLOG.nNIDMain, _sLOG.nNIDSub, _sLOG.nSGNum, _sLOG.nSvrNum,
									_sLOG.nFldNum, _sLOG.nMakeType, _sLOG.lnMakeNum, (short) _sLOG.cOptTYPE1,
									_sLOG.nOptVALUE1, (short) _sLOG.cOptTYPE2, _sLOG.nOptVALUE2,
									(short) _sLOG.cOptTYPE3, _sLOG.nOptVALUE3, (short) _sLOG.cOptTYPE4,
									_sLOG.nOptVALUE4); 

	int nReturn = m_pLogDB->ExecuteSpInt(szTemp);
//	strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.

	return nReturn;
}

int COdbcManager::LogPetAction( const SLOGPETACTION &_sLOG )
{
/*
	std::strstream strTemp;
	strTemp << "{call sp_LogPetAction_Insert(";
	strTemp << _sLOG.nPetNum << ",";
	strTemp << _sLOG.nItemMID << ",";
	strTemp << _sLOG.nItemSID << ",";	
	strTemp << _sLOG.nActionType << ",";
	strTemp << _sLOG.nPetFull << ",?)}";
	strTemp << std::ends;
*/

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call sp_LogPetAction_Insert(%d,%d,%d,%d,%d,?)}", _sLOG.nPetNum, _sLOG.nItemMID, 
													_sLOG.nItemSID, _sLOG.nActionType, _sLOG.nPetFull); 

	int nReturn = m_pLogDB->ExecuteSpInt(szTemp);
//	strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.

	return nReturn;
}

int COdbcManager::LogVehicleAction( const SLOGVEHICLEACTION &_sLOG )
{
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call sp_LogVehicleAction_Insert(%d,%d,%d,%d,%d,?)}", _sLOG.nVehicleNum, _sLOG.nItemMID, 
		_sLOG.nItemSID, _sLOG.nActionType, _sLOG.nVehicleFull); 

	int nReturn = m_pLogDB->ExecuteSpInt(szTemp);

	return nReturn;
}

///////////////////////////////////////////////////////////////////////////////
// 케릭터 활동 로그를 기록한다.
int COdbcManager::LogAction(
	const SLOGACTION &_sLOG )
{
	/*
	sp_LogAction_Insert
	_sLOG.m_nCHARID
	_sLOG.m_nTYPE 
	_sLOG.m_nTARID 
	_sLOG.m_nTARTYPE 
	_sLOG.m_nEXP 
	_sLOG.m_nBRIGHT_POINT 
	_sLOG.m_nLIFE_POINT 
	_sLOG.m_nMONEY 
	*/
/*	
	std::strstream strTemp;
	strTemp << "{call sp_LogAction_Insert(";
	strTemp << _sLOG.m_nCHARID << ",";
	strTemp << _sLOG.m_nTYPE << ",";
	strTemp << _sLOG.m_nTARID << ",";	
	strTemp << _sLOG.m_nTARTYPE << ",";
	strTemp << _sLOG.m_nEXP << ",";
	strTemp << _sLOG.m_nBRIGHT_POINT << ",";
	strTemp << _sLOG.m_nLIFE_POINT << ",";
	strTemp << _sLOG.m_nMONEY << ")}";
	strTemp << std::ends;
*/
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "{call sp_LogAction_Insert(%d,%d,%d,%d,%I64d,%d,%d,%d)}", _sLOG.m_nCHARID,
													_sLOG.m_nTYPE, _sLOG.m_nTARID, _sLOG.m_nTARTYPE, 
													_sLOG.m_nEXP, _sLOG.m_nBRIGHT_POINT, _sLOG.m_nLIFE_POINT,
													_sLOG.m_nMONEY );

	int nReturn = m_pLogDB->ExecuteSp(szTemp);
//	strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.

	return nReturn;
}

/**
 * 게임내에서 진정이나 신고로 신고한 내용을 DB에 기록한다.
 * \param nSGNum 서버그룹
 * \param nChaNum 신고한 캐릭터번호
 * \param strMsg 신고내용 (최대크기는 CHAT_MSG_SIZE)  
 * \return 
 */
int COdbcManager::LogAppeal(
	int nSGNum,
	int nChaNum,
	CString strMsg )
{
    if ((strMsg.GetLength() > CHAT_MSG_SIZE) || (nSGNum < 0) || (nChaNum < 0))
    {
        return DB_ERROR;
    }

	strMsg.Trim(_T(" ")); // 앞뒤 공백제거
	strMsg.Replace("'", "''"); // ' -> ''
/*
    std::strstream strTemp;
	strTemp << "INSERT INTO LogAppeal (SGNum, ChaNum, ApNote) VALUES (";
    strTemp << nSGNum << ",";
    strTemp << nChaNum << ",";
    strTemp << "'" << strMsg.GetString() << "')";
	strTemp << std::ends;
*/
	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "INSERT INTO LogAppeal (SGNum, ChaNum, ApNote) VALUES (%d, %d)",
													 nSGNum, nChaNum);

	int nReturn = m_pLogDB->ExecuteSQL(szTemp);
//	strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.

    return nReturn;
}

/**
 * 불법프로그램 사용내역을 기록한다.
 * \param nSGNum 서버그룹
 * \param nSvrNum 서버번호
 * \param nUserNum 유저번호
 * \param nChaNum 캐릭터번호
 * \param nHackProgramNum 해킹프로그램번호
 * \param szComment 콤멘트
 * \return 
 */
int COdbcManager::LogHackProgram(
	int nSGNum, 
	int nSvrNum, 
	int nUserNum, 
	int nChaNum,
	int nHackProgramNum,
	const TCHAR* szComment )
{
	if (szComment == NULL) return DB_ERROR;
	
	CString strComment(szComment);
	strComment.Trim(_T(" ")); // 앞뒤 공백제거
	strComment.Replace("'", "''"); // ' -> ''	

/*
	std::strstream strTemp;
	strTemp << "{call InsertLogHackProgram(";
	strTemp << nSGNum << ",";
	strTemp << nSvrNum << ",";
	strTemp << nUserNum << ",";
    strTemp << nChaNum << ",";
	strTemp << nHackProgramNum << ",";
	strTemp << "'" << strComment.GetString() << "',?)}";
	strTemp << std::ends;
*/
	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "{call InsertLogHackProgram(%d,%d,%d,%d,%d,'%s',?)}", 
												nSGNum, nSvrNum, nUserNum, nChaNum, nHackProgramNum,
												strComment.GetString());

	int nReturn = m_pLogDB->ExecuteSpInt(szTemp);
//	strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.

    return nReturn;
}

/**
 * 캐릭터 삭제 기록
 * \param nSGNum 서버그룹
 * \param nSvrNum 서버번호 (채널번호)
 * \param nUserNum 사용자번호
 * \param nChaNum 캐릭터번호
 * \return 
 */
int COdbcManager::LogChaDeleted(
	int nSGNum,
	int nSvrNum,
	int nUserNum,
	int nChaNum )
{
	/*
	std::strstream strTemp;
	strTemp << "Insert Into LogChaDelete (SGNum, SvrNum, nUserNum, nChaNum) Values (";
	strTemp << nSGNum << ",";
	strTemp << nSvrNum << ",";
	strTemp << nUserNum << ",";
	strTemp << nChaNum << ")";
	strTemp << std::ends;

	return m_pLogDB->ExecuteSQL(strTemp);

	strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
	*/
	return DB_OK;
}

/*
int COdbcManager::LogServerState(int nSGNum, int nSvrNum, int nUserNum, int nUserMax)
{
	SQLRETURN sReturn=0;
	ODBC_STMT* pConn = m_pLogDB->GetConnection();
	if (!pConn)	return DB_ERROR;

	std::strstream strTemp;
	strTemp << "{call log_serverstate(";
	strTemp << nUserNum << ",";
	strTemp << nUserMax << ",";
	strTemp << nSvrNum << ",";	
	strTemp << nSGNum << ")}";
	strTemp << std::ends;

	::SQLPrepare(pConn->hStmt,(SQLCHAR*) strTemp.str(), SQL_NTS);

	sReturn = ::SQLExecute(pConn->hStmt);	
	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
        Print(strTemp.str());		
		Print(GetErrorString(pConn->hStmt));
		m_pLogDB->FreeConnection(pConn);

		strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		return DB_ERROR;
	}
	else
	{
		m_pLogDB->FreeConnection(pConn);

		strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		return DB_OK;
	}
}
*/

int COdbcManager::LogPutonDrop( DWORD dwCharID, char* szCharName, char* szSerial, 
	SNATIVEID sItemID, BYTE cItemGen, SNATIVEID sMapID, SNATIVEID sFactID_A, SNATIVEID sFactID_B, 
	BOOL bPet, SNATIVEID sPetSkill_A, SNATIVEID sPetSkill_B )
{
	SQLRETURN	sReturn = 0;	

	CString strChaName (szCharName);
	strChaName.Trim(_T(" ")); 
	strChaName.Replace(_T("'"), _T("''"));

	CString strSerial (szSerial);
	strSerial.Trim(_T(" ")); 
	strSerial.Replace(_T("'"), _T("''"));

	TCHAR szTemp[2048] = {0};
	_snprintf( szTemp, 2048, "{call sp_LogPutOnDrop(%u,'%s','%s',%u,%u,%u,%u,%u,%u,%u,%u,%u,%d,%u,%u,%u,%u,?)}", dwCharID, strChaName.GetString(), strSerial.GetString(), 
			sItemID.wMainID, sItemID.wSubID, cItemGen, sMapID.wMainID, sMapID.wSubID, sFactID_A.wMainID, sFactID_A.wSubID, sFactID_B.wMainID, sFactID_B.wSubID, 
			bPet, sPetSkill_A.wMainID, sPetSkill_A.wSubID, sPetSkill_B.wMainID, sPetSkill_B.wSubID ); 

	int nReturn = m_pLogDB->ExecuteSpInt(szTemp);

	return nReturn;
}


/* max level notice, Juver, 2021/07/26 */
int COdbcManager::LogMaxLevel ( DWORD dwUserID, DWORD dwCharID, char* szName, WORD wLevel )
{
	int nReturn = DB_ERROR;

	CString strChaName (szName);
	strChaName.Trim(_T(" ")); 
	strChaName.Replace(_T("'"), _T("''"));

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "{call sp_LogMaxLevel(%u,%u,'%s',%u,?)}", dwUserID, dwCharID, strChaName.GetString(), wLevel ); 

	nReturn = m_pLogDB->ExecuteSpInt(szTemp);

	return nReturn;
}

/* tyranny result log, Juver, 2021/09/09 */
int COdbcManager::LogTyrannyResult ( WORD wTyrannyWinner, DWORD dwParticipantSG, DWORD dwParticipantMP, DWORD dwParticipantPHX )
{
	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "{call sp_LogTyrannyResult(%u,%u,%u,%u,?)}", wTyrannyWinner, dwParticipantSG, dwParticipantMP, dwParticipantPHX ); 

	int nReturn = m_pLogDB->ExecuteSpInt(szTemp);

	return nReturn;
}

/* tyranny result log, Juver, 2021/09/09 */
int COdbcManager::LogTyrannyParticipant ( 
	int nSessionID,
	DWORD dwChaNum,
	const char* szName, 
	WORD wSchool, 
	WORD wClass, 
	WORD wRankAll,
	WORD wRankSchool,
	WORD wScoreKill,
	WORD wScoreDeath,
	DWORD dwScoreDamage,
	DWORD dwScoreHeal,
	WORD wScoreResu,
	DWORD dwScoreTotal,
	WORD wRewardPoint )
{

	CString strChaName (szName);
	strChaName.Trim(_T(" ")); 
	strChaName.Replace(_T("'"), _T("''"));

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "{call sp_LogTyrannyParticipant(%d,%u,'%s',%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,?)}", 
		nSessionID, 
		dwChaNum, 
		strChaName.GetString(), 
		wSchool, 
		wClass, 
		wRankAll,
		wRankSchool, 
		wScoreKill, 
		wScoreDeath, 
		dwScoreDamage,
		dwScoreHeal,
		wScoreResu,
		dwScoreTotal, 
		wRewardPoint ); 

	int nReturn = m_pLogDB->ExecuteSpInt(szTemp);

	return nReturn;
}