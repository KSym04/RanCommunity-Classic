/*!
 * \file GLSummonDefine.h
 *
 * \author Juver
 * \date 2020/04/29
 *
 * 
 */

#ifndef GLSUMMONDEFINE_H_INCLUDED__
#define GLSUMMONDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#define  SKILL_SUMMON_SCHOOL_NULL		10
#define  SKILL_SUMMON_EFFECT_TIME		10.0f			//maximum effect time in client to prevent stuck
#define  SKILL_SUMMON_MAX_CLIENT_NUM	10				//maximum summon per char	
#define  SUMMON_VALID_CHECK_TIME		2.0f			//check for summon/owner validity

enum EMSUMMONACTIONTYPE
{
	SUMMONAT_IDLE		= 0,
	SUMMONAT_WALK		= 1,
	SUMMONAT_RUN		= 2,
	SUMMONAT_ATTACK		= 3,
	SUMMONAT_NONE		= 4,
};

enum EMSUMMONACTSTATE
{
	EM_SUMMONACT_WAITING		  = 0x00000001,
	EM_SUMMONACT_MOVE			  = 0x00000002,		// 움직임
	EM_SUMMONACT_WAIT			  = 0x00000004,		// 잠시대기
	EM_SUMMONACT_STOP			  = 0x00000008,		// 완전정지
	EM_SUMMONACT_RUN			  = 0x00000010,		// 뛰기모드
	EM_SUMMONACT_WALK			  = 0x00000020,		// 걷기모드
	EM_SUMMONACT_RUN_CLOSELY	  = 0x00000040,		// 근접거리까지 뛰기모드
	EM_SUMMONACT_ATTACK			  = 0x00000080,		// Attack모드
	EM_SUMMONACT_TRACING		  = 0x00000100,		// 추적모드
};

enum SUMMON_TYPE
{
	SUMMON_TYPE_NONE    = -1,
	SUMMON_TYPE_A		= 0,
	SUMMON_TYPE_B		= 1,
	SUMMON_TYPE_C		= 2,
	SUMMON_TYPE_D		= 3,
	SUMMON_TYPE_SIZE    = 4,
};

namespace COMMENT
{
	extern std::string SUMMON_TYPE[SUMMON_TYPE_SIZE];
};

#endif // GLSUMMONDEFINE_H_INCLUDED__
