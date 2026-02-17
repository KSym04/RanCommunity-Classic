#include "stdafx.h"
#include "./GLCharData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void SPASSIVE_SKILL_DATA_CLIENT::Assign( const SPASSIVE_SKILL_DATA_SERVER& rValue )
{
	m_nHP				= rValue.m_nHP;
	m_nMP				= rValue.m_nMP;
	m_nSP				= rValue.m_nSP;

	m_nDAMAGE			= rValue.m_nDAMAGE;
	m_nDEFENSE			= rValue.m_nDEFENSE;

	m_nHIT				= rValue.m_nHIT;
	m_nAVOID			= rValue.m_nAVOID;

	m_nPIERCE			= rValue.m_nPIERCE;
	m_fTARGET_RANGE		= rValue.m_fTARGET_RANGE;

	/*skill range spec logic, Juver, 2017/06/06 */
	m_fATTACK_RANGE		= rValue.m_fATTACK_RANGE;
	m_fAPPLY_RANGE		= rValue.m_fAPPLY_RANGE;

	m_fMOVEVELO			= rValue.m_fMOVEVELO;
	m_fATTVELO			= rValue.m_fATTVELO;
	m_fSKILLDELAY		= rValue.m_fSKILLDELAY;

	m_fINCR_HP			= rValue.m_fINCR_HP;
	m_fINCR_MP			= rValue.m_fINCR_MP;
	m_fINCR_SP			= rValue.m_fINCR_SP;

	m_fDAMAGE_RATE		= rValue.m_fDAMAGE_RATE;
	m_fDEFENSE_RATE		= rValue.m_fDEFENSE_RATE;

	m_sDamageSpec		= rValue.m_sDamageSpec;

	m_nPA				= rValue.m_nPA;
	m_nSA				= rValue.m_nSA;
	m_nMA				= rValue.m_nMA;

	m_fHP_RATE			= rValue.m_fHP_RATE;
	m_fMP_RATE			= rValue.m_fMP_RATE;
	m_fSP_RATE			= rValue.m_fSP_RATE;

	m_sSUMRESIST		= rValue.m_sSUMRESIST;

	/*summon time, Juver, 2017/12/12 */
	m_nSummonTime		= rValue.m_nSummonTime;

};