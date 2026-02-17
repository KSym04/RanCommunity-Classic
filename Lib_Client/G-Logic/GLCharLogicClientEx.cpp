#include "stdafx.h"
#include "./GLCharLogicClient.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



void GLCHARLOGIC_CLIENT::StateStunSet( bool bSet )
{
	EnterCriticalSection(&m_STATE_STUN_Lock);
	m_bSTATE_STUN_ORIG = bSet;
	LeaveCriticalSection(&m_STATE_STUN_Lock);
}

void GLCHARLOGIC_CLIENT::StateStunSet2( bool bSet )
{
	EnterCriticalSection(&m_STATE_STUN_Lock);
	m_cSTATE_STUN_CHECK.Set( (int)bSet );
	LeaveCriticalSection(&m_STATE_STUN_Lock);
}

bool GLCHARLOGIC_CLIENT::StateStunGet()
{
	EnterCriticalSection(&m_STATE_STUN_Lock);

	if ( m_bSTATE_STUN_ORIG )
	{
		LeaveCriticalSection(&m_STATE_STUN_Lock);
		return true;
	}

	if ( m_cSTATE_STUN_CHECK.GetInt() != 0 )
	{
		LeaveCriticalSection(&m_STATE_STUN_Lock);
		return true;
	}

	LeaveCriticalSection(&m_STATE_STUN_Lock);

	return false;
}


void GLCHARLOGIC_CLIENT::SumPierceSet( int nVal )
{
	EnterCriticalSection(&m_SUM_PIERCE_Lock);
	m_nSUM_PIERCE_ORIG = nVal;
	LeaveCriticalSection(&m_SUM_PIERCE_Lock);
}

void GLCHARLOGIC_CLIENT::SumPierceSet2( int nVal )
{
	EnterCriticalSection(&m_SUM_PIERCE_Lock);
	m_cSUM_PIERCE_CHECK.Set( (int)nVal );
	LeaveCriticalSection(&m_SUM_PIERCE_Lock);
}


int GLCHARLOGIC_CLIENT::SumPierceGet()
{
	int nVal = 0;
	EnterCriticalSection(&m_SUM_PIERCE_Lock);

	nVal = new_min( m_nSUM_PIERCE_ORIG, m_cSUM_PIERCE_CHECK.GetInt() );
	
	LeaveCriticalSection(&m_SUM_PIERCE_Lock);

	return nVal;
}



void GLCHARLOGIC_CLIENT::SumTargetRangeSet( float fVal )
{
	EnterCriticalSection(&m_SUM_TARGET_RANGE_Lock);
	m_fSUM_TARGET_RANGE_ORIG = fVal;
	LeaveCriticalSection(&m_SUM_TARGET_RANGE_Lock);
}

void GLCHARLOGIC_CLIENT::SumTargetRangeSet2( float fVal )
{
	EnterCriticalSection(&m_SUM_TARGET_RANGE_Lock);
	m_cSUM_TARGET_RANGE_CHECK.Set( (float)fVal );
	LeaveCriticalSection(&m_SUM_TARGET_RANGE_Lock);
}


float GLCHARLOGIC_CLIENT::SumTargetRangeGet()
{
	float fVal = 0;
	EnterCriticalSection(&m_SUM_TARGET_RANGE_Lock);

	fVal = new_min( m_fSUM_TARGET_RANGE_ORIG, m_cSUM_TARGET_RANGE_CHECK.GetFloat() );

	LeaveCriticalSection(&m_SUM_TARGET_RANGE_Lock);

	return fVal;
}



void GLCHARLOGIC_CLIENT::SumAttackRangeSet( float fVal )
{
	EnterCriticalSection(&m_SUM_ATTACK_RANGE_Lock);
	m_fSUM_ATTACK_RANGE_ORIG = fVal;
	LeaveCriticalSection(&m_SUM_ATTACK_RANGE_Lock);
}

void GLCHARLOGIC_CLIENT::SumAttackRangeSet2( float fVal )
{
	EnterCriticalSection(&m_SUM_ATTACK_RANGE_Lock);
	m_cSUM_ATTACK_RANGE_CHECK.Set( (float)fVal );
	LeaveCriticalSection(&m_SUM_ATTACK_RANGE_Lock);
}

float GLCHARLOGIC_CLIENT::SumAttackRangeGet()
{
	float fVal = 0;
	EnterCriticalSection(&m_SUM_ATTACK_RANGE_Lock);

	fVal = new_min( m_fSUM_ATTACK_RANGE_ORIG, m_cSUM_ATTACK_RANGE_CHECK.GetFloat() );

	LeaveCriticalSection(&m_SUM_ATTACK_RANGE_Lock);

	return fVal;
}



void GLCHARLOGIC_CLIENT::SumApplyRangeSet( float fVal )
{
	EnterCriticalSection(&m_SUM_APPLY_RANGE_Lock);
	m_fSUM_APPLY_RANGE_ORIG = fVal;
	LeaveCriticalSection(&m_SUM_APPLY_RANGE_Lock);
}

void GLCHARLOGIC_CLIENT::SumApplyRangeSet2( float fVal )
{
	EnterCriticalSection(&m_SUM_APPLY_RANGE_Lock);
	m_cSUM_APPLY_RANGE_CHECK.Set( (float)fVal );
	LeaveCriticalSection(&m_SUM_APPLY_RANGE_Lock);
}

float GLCHARLOGIC_CLIENT::SumApplyRangeGet()
{
	float fVal = 0;
	EnterCriticalSection(&m_SUM_APPLY_RANGE_Lock);

	fVal = new_min( m_fSUM_APPLY_RANGE_ORIG, m_cSUM_APPLY_RANGE_CHECK.GetFloat() );

	LeaveCriticalSection(&m_SUM_APPLY_RANGE_Lock);

	return fVal;
}
