#include "stdafx.h"
#include "./GLLandMan.h"
#include "./GLMobSchedule.h"

#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/StringUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD	SMOBACTION::VERSION = 0x0100;
const DWORD GLMobScheduleMan::VERSION = 0x0100;

BOOL GLMobSchedule::Load ( basestream &SFile )
{
	DWORD dwVer, dwSize;
	SFile >> dwVer;
	dwSize = SFile.ReadBlockSize ();

	switch ( dwVer )
	{
	case 0x0100:	Load_0100( SFile );	break;
	case 0x0101:	Load_0101( SFile );	break;
	case 0x0102:	Load_0102( SFile );	break;
	case 0x0103:	Load_0103( SFile );	break;
	case 0x0104:	Load_0104( SFile );	break;
	case 0x0105:	Load_0105( SFile );	break;
	case 0x0106:	Load_0106( SFile );	break;
	case 0x0107:	Load_0107( SFile );	break;
	case 0x0108:	Load_0108( SFile );	break;
	case 0x0109:	Load_0109( SFile );	break;
	case 0x0110:	Load_0110( SFile );	break;
	case 0x0111:	Load_0111( SFile );	break;
	case 0x0112:	Load_0112( SFile );	break;
	case 0x0113:	Load_0113( SFile );	break;

	case 0x0200:	Load_0200( SFile );	break;
	case 0x0201:	Load_0201( SFile );	break;
	case 0x0202:	Load_0202( SFile );	break;
	case VERSION:	Load_0203( SFile );	break;
	default:
		{
			CDebugSet::ErrorVersion( "GLMobSchedule::Load", dwVer );	
		}break;
	};	

	//	박스의 크기를 계산.
	CalculateBox ();

	if ( m_bRendGenPos )
		std::random_shuffle ( m_vectorRendGenPos.begin(), m_vectorRendGenPos.end() );

	//action stay type check
	//check for invalid stay time
	SGLNODE<SMOBACTION>* pCur = m_sMobActList.m_pHead;
	while ( pCur )
	{
		SMOBACTION &sMobAction = pCur->Data;
		if ( sMobAction.emAction == EMACTION_ACTSTAY )
		{
			if ( sMobAction.fLife <= 0.0f )
				sMobAction.fLife = 1.0f;
		}

		pCur = pCur->pNext;
	}

	SCROWDATA* pcrow_data = GLCrowDataMan::GetInstance().GetCrowData( m_CrowID );
	if ( !pcrow_data )
		CDebugSet::ErrorCode( "GLMobSchedule::Load", "%s invalid crow: %d %d (%s)", SFile.GetFileName(), m_CrowID.wMainID, m_CrowID.wSubID, m_szName );

	return TRUE;
}

BOOL GLMobSchedule::Load_0100( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	return TRUE;
}
BOOL GLMobSchedule::Load_0101( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_bRendGenPos;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0102( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bRendGenPos;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0103( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenMin;
	SFile >> m_nRegenHour;

	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );
	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bRendGenPos;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0104( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenMin;
	SFile >> m_nRegenHour;

	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );
	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0105( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenMin;
	SFile >> m_nRegenHour;

	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );
	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0106( basestream &SFile )
{
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenMin;
	SFile >> m_nRegenHour;

	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );
	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_bLeaderMonster;
	SFile >> m_dummy_var_1; //unknown data
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	return TRUE;
}

BOOL GLMobSchedule::Load_0107( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dummy_var_1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;

	return TRUE;
}

BOOL GLMobSchedule::Load_0108( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dummy_var_1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_dummy_var_2; //unknown data

	return TRUE;
}

BOOL GLMobSchedule::Load_0109( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dummy_var_3; //unknown data
	SFile >> m_dummy_var_1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_dummy_var_2; //unknown data

	return TRUE;
}

BOOL GLMobSchedule::Load_0110( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dummy_var_3; //unknown data
	SFile >> m_dummy_var_1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_dummy_var_2; //unknown data
	SFile >> m_dummy_var_4; //unknown data

	return TRUE;
}

BOOL GLMobSchedule::Load_0111( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_dummy_var_5;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dummy_var_3; //unknown data
	SFile >> m_dummy_var_1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_dummy_var_2; //unknown data
	SFile >> m_dummy_var_4; //unknown data

	return TRUE;
}

BOOL GLMobSchedule::Load_0112( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_dummy_var_5;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dummy_var_3; //unknown data
	SFile >> m_dummy_var_1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_dummy_var_2; //unknown data
	SFile >> m_dummy_var_4; //unknown data
	SFile >> m_dummy_var_6;

	return TRUE;
}

BOOL GLMobSchedule::Load_0113( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_dummy_var_5;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_dummy_var_3; //unknown data
	SFile >> m_dummy_var_1; //unknown data
	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;
	SFile >> m_dummy_var_2; //unknown data
	SFile >> m_dummy_var_4; //unknown data
	SFile >> m_dummy_var_6;
	SFile >> m_dummy_var_7;

	return TRUE;
}

BOOL GLMobSchedule::Load_0200( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;

	return TRUE;
}

BOOL GLMobSchedule::Load_0201( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;

	//unknown data
	SFile >> m_dummy_var_1;
	SFile >> m_dummy_var_3;
	SFile >> m_dummy_var_2;

	return TRUE;
}

BOOL GLMobSchedule::Load_0202( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;

	//unknown data
	SFile >> m_dummy_var_1;
	SFile >> m_dummy_var_2;
	SFile >> m_dummy_var_3;
	SFile >> m_dummy_var_4;

	return TRUE;
}

BOOL GLMobSchedule::Load_0203( basestream &SFile )
{
	SFile.ReadBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.ReadBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.ReadBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile >> m_fReGenTime;
	SFile >> m_nRegenHour;
	SFile >> m_nRegenMin;

	SFile.ReadBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );
	DXAFFINEMATRIX::SetAffineValue ( DXAFFINEMATRIX::m_pAffineParts );

	BOOL bExist(FALSE);
	SFile >> bExist;
	while ( bExist )
	{
		DWORD dwVer(0);
		DWORD dwSize(0);
		SMOBACTION sMobAction;

		SFile >> dwVer;
		SFile >> dwSize;
		SFile.ReadBuffer ( &sMobAction, sizeof(SMOBACTION) );
		m_sMobActList.ADDTAIL ( sMobAction );

		SFile >> bExist;
	}

	SFile >> m_bLeaderMonster;
	SFile >> m_strGroupName;
	SFile >> m_bRendGenPos;
	SFile >> m_wRendGenPosNum;
	SFile >> m_wRendGenPosDist;

	DWORD dwNum(0);
	SFile >> dwNum;
	m_vectorRendGenPos.clear();

	for ( DWORD i=0; i<dwNum; ++i )
	{
		D3DXVECTOR3 vPos(0.0f,0.0f,0.0f);
		SFile >> vPos;
		m_vectorRendGenPos.push_back ( vPos );
	}

	SFile >> m_dwPC_REGEN_GATEID;
	SFile >> m_strBUSLOCATION;

	//unknown data
	SFile >> m_dummy_var_1;
	SFile >> m_dummy_var_2;
	SFile >> m_dummy_var_3;
	SFile >> m_dummy_var_4;

	SFile >> m_dummy_var_5;
	SFile >> m_dummy_var_6;
	SFile >> m_dummy_var_7;

	return TRUE;
}

BOOL GLMobSchedule::Save ( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION;

	SFile.BeginBlock(EMBLOCK_02);

	SFile.WriteBuffer ( &m_CrowID, sizeof(SNATIVEID) );
	SFile.WriteBuffer ( m_szName, sizeof(char)*EM_MAXSZNAME );
	SFile.WriteBuffer ( m_bDayOfWeek, sizeof(m_bDayOfWeek) );

	SFile << m_fReGenTime;
	SFile << m_nRegenHour;
	SFile << m_nRegenMin;

	SFile.WriteBuffer ( DXAFFINEMATRIX::m_pAffineParts, sizeof(DXAFFINEPARTS) );

	SGLNODE<SMOBACTION>* pCur = m_sMobActList.m_pHead;
	if ( pCur )	SFile << (BOOL) TRUE;
	else		SFile << (BOOL) FALSE;

	while ( pCur )
	{
		SMOBACTION &sMobAction = pCur->Data;

		SFile << (DWORD) SMOBACTION::VERSION;
		SFile << (DWORD) sizeof(SMOBACTION);
		SFile.WriteBuffer ( &sMobAction, sizeof(SMOBACTION) );

		if ( pCur->pNext )	SFile << (BOOL) TRUE;
		else				SFile << (BOOL) FALSE;

		pCur = pCur->pNext;
	}

	SFile << m_bLeaderMonster;
	SFile << m_strGroupName;
	SFile << m_bRendGenPos;
	SFile << m_wRendGenPosNum;
	SFile << m_wRendGenPosDist;

	SFile << static_cast<DWORD> ( m_vectorRendGenPos.size() );
	VEC3ARRAY_ITER iter = m_vectorRendGenPos.begin ();
	VEC3ARRAY_ITER iter_end = m_vectorRendGenPos.end ();
	for ( ; iter!=iter_end; ++iter )
	{
		SFile <<  (*iter);
	}

	SFile << m_dwPC_REGEN_GATEID;
	SFile << m_strBUSLOCATION;

	//unknown data
	SFile << m_dummy_var_1;
	SFile << m_dummy_var_2;
	SFile << m_dummy_var_3;
	SFile << m_dummy_var_4;

	SFile << m_dummy_var_5;
	SFile << m_dummy_var_6;
	SFile << m_dummy_var_7;

	SFile.EndBlock (EMBLOCK_02);

	return TRUE;
}

VOID GLMobSchedule::SaveCsvHead ( std::fstream &SFile )
{
	SFile<< "m_szName"								<< ",";

	SFile<< "m_CrowID.wMID"							<< ",";
	SFile<< "m_CrowID.wSID"							<< ",";
	SFile<< "m_CrowID.Name"							<< ",";

	SFile<< "m_fReGenTime"							<< ",";
	SFile<< "m_nRegenHour"							<< ",";
	SFile<< "m_nRegenMin"							<< ",";

	SFile<< "m_bDayOfWeek[0]"						<< ",";
	SFile<< "m_bDayOfWeek[1]"						<< ",";
	SFile<< "m_bDayOfWeek[2]"						<< ",";
	SFile<< "m_bDayOfWeek[3]"						<< ",";
	SFile<< "m_bDayOfWeek[4]"						<< ",";
	SFile<< "m_bDayOfWeek[5]"						<< ",";
	SFile<< "m_bDayOfWeek[6]"						<< ",";

	SFile<< "m_bLeaderMonster"						<< ",";
	SFile<< "m_strGroupName"						<< ",";

	SFile<< "m_bRendGenPos"							<< ",";
	SFile<< "m_wRendGenPosNum"						<< ",";
	SFile<< "m_wRendGenPosDist"						<< ",";

	//m_vectorRendGenPos

	SFile<< "m_dwPC_REGEN_GATEID"					<< ",";
	SFile<< "m_strBUSLOCATION"						<< ",";


	SFile<< "vTrans.x"								<< ",";
	SFile<< "vTrans.y"								<< ",";
	SFile<< "vTrans.z"								<< ",";

	SFile<< "vRotate.x"								<< ",";
	SFile<< "vRotate.y"								<< ",";
	SFile<< "vRotate.z"								<< ",";

	SFile<< "vScale.x"								<< ",";
	SFile<< "vScale.y"								<< ",";
	SFile<< "vScale.z"								<< ",";


	const int nMax = 30;
	for ( int i=0; i<30; ++i )
	{
		SFile << "m_sMobActList " << i << " emAction" << ",";
		SFile << "m_sMobActList " << i << " vPos.x" << ",";
		SFile << "m_sMobActList " << i << " vPos.y" << ",";
		SFile << "m_sMobActList " << i << " vPos.z" << ",";
		SFile << "m_sMobActList " << i << " fLife" << ",";
	}

	SFile << std::endl;
}

VOID GLMobSchedule::SaveCsv ( std::fstream &SFile )	const
{
	if( strlen( m_szName ) < 1 )
		SFile << " " << ",";
	else
		SFile << m_szName << ",";

	SFile << m_CrowID.wMainID << ",";
	SFile << m_CrowID.wSubID  << ",";

	SCROWDATA* pCrowData = GLCrowDataMan::GetInstance().GetCrowData( m_CrowID );
	if ( pCrowData )
		SFile << pCrowData->GetName() << ",";
	else
		SFile << " " << ",";

	SFile << m_fReGenTime  << ",";
	SFile << m_nRegenHour  << ",";
	SFile << m_nRegenMin  << ",";

	SFile << m_bDayOfWeek[0]  << ",";
	SFile << m_bDayOfWeek[1]  << ",";
	SFile << m_bDayOfWeek[2]  << ",";
	SFile << m_bDayOfWeek[3]  << ",";
	SFile << m_bDayOfWeek[4]  << ",";
	SFile << m_bDayOfWeek[5]  << ",";
	SFile << m_bDayOfWeek[6]  << ",";

	SFile << m_bLeaderMonster  << ",";
	STRUTIL::OutputStrCsv( SFile, std::string(m_strGroupName) );

	SFile << m_bRendGenPos  << ",";
	SFile << m_wRendGenPosNum  << ",";
	SFile << m_wRendGenPosDist  << ",";

	SFile << m_dwPC_REGEN_GATEID  << ",";
	STRUTIL::OutputStrCsv( SFile, std::string(m_strBUSLOCATION) );

	SFile << m_pAffineParts->vTrans.x  << ",";
	SFile << m_pAffineParts->vTrans.y  << ",";
	SFile << m_pAffineParts->vTrans.z  << ",";

	SFile << m_pAffineParts->vRotate.x  << ",";
	SFile << m_pAffineParts->vRotate.y  << ",";
	SFile << m_pAffineParts->vRotate.z  << ",";

	SFile << m_pAffineParts->vScale.x  << ",";
	SFile << m_pAffineParts->vScale.y  << ",";
	SFile << m_pAffineParts->vScale.z  << ",";

	std::vector<SMOBACTION> vec;
	SGLNODE<SMOBACTION>* pCur = m_sMobActList.m_pHead;
	while ( pCur )
	{
		SMOBACTION &sMobAction = pCur->Data;

		vec.push_back( sMobAction );

		pCur = pCur->pNext;
	}

	const int nMax = 30;
	for ( int i=0; i<30; ++i )
	{
		if ( i < (int)vec.size() )
		{
			SFile << vec[i].emAction  << ",";

			if ( vec[i].emAction == EMACTION_ACTSTAY )
			{
				SFile << (float) 0.0f  << ",";
				SFile << (float) 0.0f  << ",";
				SFile << (float) 0.0f  << ",";
				SFile << vec[i].fLife  << ",";
			}
			else
			{
				SFile << vec[i].vPos.x  << ",";
				SFile << vec[i].vPos.y  << ",";
				SFile << vec[i].vPos.z << ",";
				SFile << (float) 0.0f  << ",";
			}
		}
		else
		{
			SFile << (int)EMACTION_SIZE  << ",";
			SFile << (float) 0.0f  << ",";
			SFile << (float) 0.0f  << ",";
			SFile << (float) 0.0f  << ",";
			SFile << (float) 0.0f  << ",";
		}
		
	}

	SFile << std::endl;
}

VOID GLMobSchedule::LoadCsv ( CStringArray &StrArray )
{
	int iCsvCur = 0;

	CString strName = StrArray[ iCsvCur++ ];
	if( strlen( strName ) > 1 && strName.GetAt( 0 ) != ' ' )
		StringCchCopy( m_szName, EM_MAXSZNAME, strName );

	m_CrowID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
	m_CrowID.wSubID  = (WORD)atoi( StrArray[ iCsvCur++ ] );

	iCsvCur++; //crow name

	m_fReGenTime = (float)atof( StrArray[ iCsvCur++ ] );
	m_nRegenHour = (int)atoi( StrArray[ iCsvCur++ ] );
	m_nRegenMin = (int)atoi( StrArray[ iCsvCur++ ] );

	m_bDayOfWeek[0] = (0!=atoi( StrArray[ iCsvCur++ ] ));
	m_bDayOfWeek[1] = (0!=atoi( StrArray[ iCsvCur++ ] ));
	m_bDayOfWeek[2] = (0!=atoi( StrArray[ iCsvCur++ ] ));
	m_bDayOfWeek[3] = (0!=atoi( StrArray[ iCsvCur++ ] ));
	m_bDayOfWeek[4] = (0!=atoi( StrArray[ iCsvCur++ ] ));
	m_bDayOfWeek[5] = (0!=atoi( StrArray[ iCsvCur++ ] ));
	m_bDayOfWeek[6] = (0!=atoi( StrArray[ iCsvCur++ ] ));

	m_bLeaderMonster = (0!=atoi( StrArray[ iCsvCur++ ] ));

	std::string strTemp;
	STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTemp );
	m_strGroupName = strTemp;

	m_bRendGenPos = (0!=atoi( StrArray[ iCsvCur++ ] ));
	m_wRendGenPosNum  = (WORD)atoi( StrArray[ iCsvCur++ ] );
	m_wRendGenPosDist  = (WORD)atoi( StrArray[ iCsvCur++ ] );

	m_dwPC_REGEN_GATEID  = (DWORD)_atoi64( StrArray[ iCsvCur++ ] );

	STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTemp );
	m_strBUSLOCATION = strTemp;


	DXAFFINEPARTS f;
	f.vTrans.x = (float)atof( StrArray[ iCsvCur++ ] );
	f.vTrans.y = (float)atof( StrArray[ iCsvCur++ ] );
	f.vTrans.z = (float)atof( StrArray[ iCsvCur++ ] );

	f.vRotate.x = (float)atof( StrArray[ iCsvCur++ ] );
	f.vRotate.y = (float)atof( StrArray[ iCsvCur++ ] );
	f.vRotate.z = (float)atof( StrArray[ iCsvCur++ ] );

	f.vScale.x = (float)atof( StrArray[ iCsvCur++ ] );
	f.vScale.y = (float)atof( StrArray[ iCsvCur++ ] );
	f.vScale.z = (float)atof( StrArray[ iCsvCur++ ] );

	SetAffineValue( &f );

	m_sMobActList.DELALL();

	const int nMax = 30;
	for ( int i=0; i<30; ++i )
	{
		SMOBACTION sMobAction;
		sMobAction.emAction = (EMMOBACTIONS)atoi( StrArray[ iCsvCur++ ] );

		if ( sMobAction.emAction == EMACTION_ACTSTAY )
		{
			float fx = (float)atof( StrArray[ iCsvCur++ ] );
			float fy = (float)atof( StrArray[ iCsvCur++ ] );
			float fz = (float)atof( StrArray[ iCsvCur++ ] );
			sMobAction.fLife = (float)atof( StrArray[ iCsvCur++ ] );
		}
		else
		{
			sMobAction.vPos.x = (float)atof( StrArray[ iCsvCur++ ] );
			sMobAction.vPos.y = (float)atof( StrArray[ iCsvCur++ ] );
			sMobAction.vPos.z = (float)atof( StrArray[ iCsvCur++ ] );
			float fLife = (float)atof( StrArray[ iCsvCur++ ] );
		}
		
		if ( sMobAction.emAction < EMACTION_SIZE )
		{
			m_sMobActList.ADDTAIL( sMobAction );
		}
	}

}

BOOL GLMobScheduleMan::LoadFile_106 ( basestream &SFile, DWORD dwGroupNum )
{
	m_setMobDesc.clear ();

	for( DWORD i = 0; i<dwGroupNum; ++i )
	{
		BOOL bExist;
		DWORD dwVer, dwSize;

		SFile >> dwVer;
		dwSize = SFile.ReadBlockSize ();

		SFile >> bExist;
		while ( bExist )
		{
			GLMobSchedule* pMobSch = new GLMobSchedule;

			pMobSch->Load ( SFile );
			m_GLMobSchList.ADDTAIL ( pMobSch );

			AddMobSchGroub ( pMobSch );
			m_setMobDesc.insert ( pMobSch->m_CrowID.dwID );

			SFile >> bExist;
		}
	}

	return TRUE;
}

BOOL GLMobScheduleMan::LoadFile ( basestream &SFile )
{
	m_setMobDesc.clear ();

	BOOL bExist;
	DWORD dwVer, dwSize;

	SFile >> dwVer;
	dwSize = SFile.ReadBlockSize ();

	SFile >> bExist;
	while ( bExist )
	{
		GLMobSchedule* pMobSch = new GLMobSchedule;

		pMobSch->Load ( SFile );
		m_GLMobSchList.ADDTAIL ( pMobSch );

		AddMobSchGroub ( pMobSch );
		m_setMobDesc.insert ( pMobSch->m_CrowID.dwID );

		SFile >> bExist;
	}

	return TRUE;
}

BOOL GLMobScheduleMan::SaveFile ( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION;
	SFile.BeginBlock (EMBLOCK_01);

	SGLNODE<GLMobSchedule*>* pCur = m_GLMobSchList.m_pHead;
	SFile << BOOL ( pCur!=NULL );

	while ( pCur )
	{
		GLMobSchedule* pMobSch = pCur->Data;

		pMobSch->Save ( SFile );

		pCur = pCur->pNext;
		SFile << BOOL ( pCur!=NULL );
	}

	SFile.EndBlock (EMBLOCK_01);

	return TRUE;
}



