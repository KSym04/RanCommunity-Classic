#include "stdafx.h"
#include "./DbActionLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*
* 학교vs학교 파티대련결과저장
*/
CLogPartyMatch::CLogPartyMatch(
	int nSGNum,
	int nSvrNum,
	WORD wWin,
	WORD wLost )
	: m_nSGNum(nSGNum)
	, m_nSvrNum(nSvrNum)
	, m_wWin(wWin)
	, m_wLost(wLost)
{
}

int CLogPartyMatch::Execute(
	CServer* pServer )
{
	m_pDbManager->LogPartyMatch( m_nSGNum, m_nSvrNum, m_wWin, m_wLost );
	return NET_OK;
}

/*
* 아이템 거래 로그
*/
CLogItemExchange::CLogItemExchange(
	const SLOGITEMEXCHANGE &sEXCHANGE )
{
	m_sEXCHANGE = sEXCHANGE;
}

int CLogItemExchange::Execute(
	CServer* pServer )
{
    m_pDbManager->LogItemExchange( m_sEXCHANGE );
	return NET_OK;
}

/*
* 게임머니 거래 로그
*/
CLogMoneyExchange::CLogMoneyExchange(
	const SLOGMONEYEXCHANGE &_sLOG )
{
	m_sLOG = _sLOG;
}

int CLogMoneyExchange::Execute(
	CServer* pServer )
{
	m_pDbManager->LogMoneyExchange( m_sLOG );
	return NET_OK;
}

/*
* 아이템 인첸트 로그
*/
CLogItemConversion::CLogItemConversion(
	const SLOGITEMCONVERSION &_sLOG )
{
	m_sLOG = _sLOG;
}

int CLogItemConversion::Execute(
	CServer* pServer )
{
	m_pDbManager->LogItemConversion( m_sLOG );
	return NET_OK;
}

/**
* 아이템의 랜덤옵션 로그를 남긴다.
* \param _sLOG 랜덤옵션 구조체
*/
CLogRandomItem::CLogRandomItem(
	const SLOGRANDOMOPTION& sLOG )
{
	m_sLOG = sLOG;
}

int CLogRandomItem::Execute(
	CServer* pServer )
{
	m_pDbManager->LogRandomItem( m_sLOG );
	return NET_OK;
}

/**
* 펫의 액션 로그를 남긴다.
* \param _sLOG 펫액션로그 구조체
*/
CLogPetAction::CLogPetAction( const SLOGPETACTION& sLOG )
{
	m_sLOG = sLOG;
}

int CLogPetAction::Execute( CServer* pServer )
{
	m_pDbManager->LogPetAction( m_sLOG );
	return NET_OK;
}


/**
* 탈것의 액션 로그를 남긴다.
* \param _sLOG 탈것액션로그 구조체
*/
CLogVehicleAction::CLogVehicleAction( const SLOGVEHICLEACTION& sLOG )
{
	m_sLOG = sLOG;
}

int CLogVehicleAction::Execute( CServer* pServer )
{
	m_pDbManager->LogVehicleAction( m_sLOG );
	return NET_OK;
}

/**
* 게임내에서 진정이나 신고로 신고한 내용을 DB에 기록한다.
* \param nSGNum 서버그룹
* \param ChaNum 신고한 캐릭터번호
* \param strMsg 신고내용 (최대크기는 CHAT_MSG_SIZE)
*/
CLogAppeal::CLogAppeal(
	int nSGNum,
	int nChaNum,
	CString strMsg )
	: m_nSGNum( nSGNum )
	, m_nChaNum( nChaNum )
{    
    m_strMsg  = strMsg;
}

int CLogAppeal::Execute(
	CServer* pServer )
{
    return m_pDbManager->LogAppeal( m_nSGNum, m_nChaNum, m_strMsg );
}

/**
* 불법프로그램 사용내역을 기록한다.
* \param nUserNum 유저번호
* \param nChaNum 캐릭터번호
* \param nHackProgramNum 해킹프로그램번호
*/
CLogHackProgram::CLogHackProgram(
	int nUserNum,
	int nChaNum,
	int nHackProgramNum,
	TCHAR* szINFO )
	: m_nUserNum( nUserNum )
	, m_nChaNum( nChaNum )
	, m_nHackProgramNum( nHackProgramNum )
{
	StringCchCopy( m_szINFO, CLogHackProgram::MAX_INFO, szINFO );
}

int CLogHackProgram::Execute(
	CServer* pServer )
{
	return m_pDbManager->LogHackProgram(
							pServer->GetServerGroup(),
		                    pServer->GetServerNum(),
							m_nUserNum,
							m_nChaNum,
							m_nHackProgramNum,
							m_szINFO );
}

/**
* 캐릭터 활동 로그를 기록한다.
*/
CLogAction::CLogAction(
	const SLOGACTION &_sLOG )
{
	m_sLOG = _sLOG;
}

int CLogAction::Execute(
	CServer* pServer )
{
	return m_pDbManager->LogAction( m_sLOG );
}

CLogPutOnDrop::CLogPutOnDrop( DWORD dwClientID, DWORD dwCharID, const char* szCharName, const char* szSerial, 
	SNATIVEID sItemID, BYTE cItemGen, SNATIVEID sMapID, SNATIVEID sFactID_A, SNATIVEID sFactID_B, 
	BOOL bPet, SNATIVEID sPetSkill_A, SNATIVEID sPetSkill_B )
{	
	m_dwClient		= dwClientID;

	m_dwCharID		= dwCharID;

	if (szCharName != NULL)
		StringCchCopy(m_szCharName, CHAR_SZNAME, szCharName);

	if (szSerial != NULL)
		StringCchCopy(m_szSerial, LOG_PUTON_SERIAL_SIZE+1, szSerial);

	m_sItemID		= sItemID;
	m_cItemGen		= cItemGen;

	m_sMapID		= sMapID;

	m_sFactID_A		= sFactID_A;
	m_sFactID_B		= sFactID_B;

	m_bPet			= bPet;
	m_sPetSkill_A	= sPetSkill_A;
	m_sPetSkill_B	= sPetSkill_B;
}

int CLogPutOnDrop::Execute (CServer* pServer)
{
	if (pServer == NULL)	return DB_ERROR;

	return m_pDbManager->LogPutonDrop( m_dwCharID, m_szCharName, m_szSerial, m_sItemID, m_cItemGen, m_sMapID, 
										m_sFactID_A, m_sFactID_B, m_bPet, m_sPetSkill_A, m_sPetSkill_B );
}