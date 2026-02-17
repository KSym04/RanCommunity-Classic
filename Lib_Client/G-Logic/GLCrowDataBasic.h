#ifndef GLCROWDATA_BASIC_H_
#define GLCROWDATA_BASIC_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib_Engine/Common/BaseStream.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/STRINGUTILS.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

#include "./GLCharDefine.h"
#include "./GLItemPet.h"
#include "./GLSummonDefine.h"

enum CROW_BASIC_FLAG
{
	CROW_BASIC_FLAG_NO_TRANSFORM	= 0x0001,
};

struct SCROWBASIC_100
{
	SNATIVEID			sNativeID;					//	고유 ID.
	char				m_szName[CHAR_SZNAME];		//	이름.
	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	WORD				m_wLevel;					//	레벨.

	WORD				m_wHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST_101			m_sResist;					//	저항값.
};

struct SCROWBASIC_101
{
	SNATIVEID			sNativeID;					//	고유 ID.
	char				m_szName[CHAR_SZNAME];		//	이름.
	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	WORD				m_wLevel;					//	레벨.

	WORD				m_wHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST				m_sResist;					//	저항값.
};

struct SCROWBASIC_102
{
	SNATIVEID			sNativeID;					//	고유 ID.
	char				m_szName[CHAR_SZNAME];		//	이름.
	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	WORD				m_wLevel;					//	레벨.

	WORD				m_wHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST				m_sResist;					//	저항값.

	WORD				m_wBonusExp;				//	추가 경험치.
};

struct SCROWBASIC_103
{
	SNATIVEID			sNativeID;					//	고유 ID.

	char				m_szName[CHAR_SZNAME];		//	이름.

	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	WORD				m_wLevel;					//	레벨.

	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST				m_sResist;					//	저항값.

	WORD				m_wBonusExp;				//	추가 경험치.
};

struct SCROWBASIC_104
{
	SNATIVEID			sNativeID;					//	고유 ID.

	char				m_szName[CHAR_SZNAME];		//	이름.

	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	// 팻타입 추가
	PETTYPE				m_emPetType;				// 팻타입

	WORD				m_wLevel;					//	레벨.

	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST				m_sResist;					//	저항값.

	WORD				m_wBonusExp;				//	추가 경험치.
};

struct SCROWBASIC_105
{
	SNATIVEID			sNativeID;					//	고유 ID.

	char				m_szName[CHAR_SZNAME];		//	이름.

	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	WORD				m_wLevel;					//	레벨.

	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST				m_sResist;					//	저항값.

	WORD				m_wBonusExp;				//	추가 경험치.

	// 팻타입 추가
	PETTYPE				m_emPetType;				// 팻타입
};

struct SCROWBASIC_106
{
	SNATIVEID			sNativeID;					//	고유 ID.

	char				m_szName[CHAR_SZNAME];		//	이름.

	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	WORD				m_wLevel;					//	레벨.

	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST				m_sResist;					//	저항값.

	WORD				m_wBonusExp;				//	추가 경험치.

	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부

	// 팻타입 추가
	PETTYPE				m_emPetType;				// 팻타입
};

struct SCROWBASIC_108
{
	SNATIVEID			sNativeID;					//	고유 ID.

	char				m_szName[CHAR_SZNAME];		//	이름.

	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	WORD				m_wLevel;					//	레벨.

	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST				m_sResist;					//	저항값.

	DWORD				m_wBonusExp;				//	추가 경험치.

	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부

	// 팻타입 추가
	PETTYPE				m_emPetType;				// 팻타입
};

struct SCROWBASIC_109
{
	SNATIVEID			sNativeID;					//	고유 ID.

	char				m_szName[CHAR_SZNAME];		//	이름.

	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	WORD				m_wLevel;					//	레벨.

	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST				m_sResist;					//	저항값.

	DWORD				m_dwBonusExp;				//	추가 경험치.

	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부

	// 팻타입 추가
	PETTYPE				m_emPetType;				// 팻타입

	DWORD				m_dwGenTime;				// 호출 시간
};

struct SCROWBASIC_110
{
	SNATIVEID			sNativeID;					//	고유 ID.

	char				m_szName[CHAR_SZNAME];		//	이름.

	EMTRIBE				m_emTribe;					//	종족.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMBRIGHT			m_emBright;					//	속성 (광/암)

	WORD				m_wLevel;					//	레벨.

	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP

	//	회복율.
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.

	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.

	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wDefense;					//	방어력.

	SRESIST				m_sResist;					//	저항값.

	DWORD				m_dwBonusExp;				//	추가 경험치.

	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부

	// 팻타입 추가
	PETTYPE				m_emPetType;				// 팻타입

	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
};

struct SCROWBASIC_113
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;					// dummy data
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
};

struct SCROWBASIC_115
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;					// dummy data
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	DWORD				dummy_var_0;
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
};

struct SCROWBASIC_116
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;					// dummy data
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	DWORD				dummy_var_0;
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
};

struct SCROWBASIC_117
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;					// dummy data
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	DWORD				dummy_var_0;
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;

	bool				dummy_var_1;
	bool				dummy_var_2;
};

struct SCROWBASIC_120
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;					// dummy data
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	DWORD				dummy_var_0;
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;

	bool				dummy_var_1;
	bool				dummy_var_2;
	float				dummy_var_3;
	bool				dummy_var_4;
	WORD				dummy_var_5;
};

struct SCROWBASIC_121
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;					// dummy data
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	DWORD				dummy_var_0;
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;

	bool				dummy_var_1;
	bool				dummy_var_2;
	float				dummy_var_3;
	bool				dummy_var_4;
	WORD				dummy_var_5;
	float				dummy_var_6;
	float				dummy_var_7;
};

struct SCROWBASIC_122
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;					// dummy data
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	DWORD				dummy_var_0;
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;

	bool				dummy_var_1;
	bool				dummy_var_2;
	float				dummy_var_3;
	bool				dummy_var_4;
	WORD				dummy_var_5;
	float				dummy_var_6;
	float				dummy_var_7;
	float				dummy_var_8;
};

struct SCROWBASIC_200
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
};

struct SCROWBASIC_201
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
};

struct SCROWBASIC_202
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
};

struct SCROWBASIC_203
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
	DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */
};

struct SCROWBASIC_204
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
	DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */

	DWORD				dummy_var_0;
	bool				dummy_var_1;
	bool				dummy_var_2;
};

struct SCROWBASIC_205
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
	DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */
	DWORD				dummy_var_0;
	bool				dummy_var_1;
	bool				dummy_var_2;
	EMNPC_CTF_FLAG_TYPE	m_emCTFType;				/*pvp capture the flag, Juver, 2018/02/03 */

};

struct SCROWBASIC_206
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
	DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */
	DWORD				dummy_var_0;
	bool				dummy_var_1;
	bool				dummy_var_2;
	float				dummy_var_3;
	bool				dummy_var_4;
	EMNPC_CTF_FLAG_TYPE	m_emCTFType;				/*pvp capture the flag, Juver, 2018/02/03 */
};

struct SCROWBASIC_207
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
	DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */
	EMNPC_CTF_FLAG_TYPE	m_emCTFType;				/*pvp capture the flag, Juver, 2018/02/03 */


	DWORD				dummy_var_0;
	bool				dummy_var_1;
	bool				dummy_var_2;
	float				dummy_var_3;
	bool				dummy_var_4;
	WORD				dummy_var_5;
	float				dummy_var_6;
	float				dummy_var_7;
	float				dummy_var_8;
};

struct SCROWBASIC_208
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
	DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */
	EMNPC_CTF_FLAG_TYPE	m_emCTFType;				/*pvp capture the flag, Juver, 2018/02/03 */
	EMCROW_SCHOOL		m_emCrowSchool;				/*crow school, Juver, 2018/08/02 */

	DWORD				dummy_var_0;
	bool				dummy_var_1;
	bool				dummy_var_2;
	float				dummy_var_3;
	bool				dummy_var_4;
	WORD				dummy_var_5;
	float				dummy_var_6;
	float				dummy_var_7;
	float				dummy_var_8;
};

struct SCROWBASIC_209
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
	DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */
	EMNPC_CTF_FLAG_TYPE	m_emCTFType;				/*pvp capture the flag, Juver, 2018/02/03 */
	EMCROW_SCHOOL		m_emCrowSchool;				/*crow school, Juver, 2018/08/02 */
	bool				m_bBlockPushPull;			/*block crow push/pull, Juver, 2019/02/14 */

	DWORD				dummy_var_0;
	bool				dummy_var_1;
	bool				dummy_var_2;
	float				dummy_var_3;
	bool				dummy_var_4;
	WORD				dummy_var_5;
	float				dummy_var_6;
	float				dummy_var_7;
	float				dummy_var_8;
};

struct SCROWBASIC_210
{
	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
	DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */
	EMNPC_CTF_FLAG_TYPE	m_emCTFType;				/*pvp capture the flag, Juver, 2018/02/03 */
	EMCROW_SCHOOL		m_emCrowSchool;				/*crow school, Juver, 2018/08/02 */
	bool				m_bBlockPushPull;			/*block crow push/pull, Juver, 2019/02/14 */

	DWORD				dummy_var_0;
	bool				dummy_var_1;
	bool				dummy_var_2;
	float				dummy_var_3;
	bool				dummy_var_4;
	WORD				dummy_var_5;
	float				dummy_var_6;
	float				dummy_var_7;
	float				dummy_var_8;

	DWORD				m_dwCrowFlag;
};

struct SCROWBASIC
{
	enum { VERSION = 0x0211, };

	SNATIVEID			sNativeID;					//	고유 ID.
	EMCROW				m_emCrow;					//	Npc, Mob.
	EMTRIBE				m_emTribe;					//	종족.
	EMBRIGHT			m_emBright;					//	속성 (광/암)
	char				m_szName[CHAR_SZNAME];		//	이름.
	WORD				m_wLevel;					//	레벨.
	DWORD				m_dwHP;						//	HP
	WORD				m_wMP;						//	MP
	WORD				m_wSP;						//	SP
	float				m_fIncHP;					//	HP 회복율.
	float				m_fIncMP;					//	MP 회복율.
	float				m_fIncSP;					//	SP 회복율.
	WORD				m_wDefense;					//	방어력.
	WORD				m_wViewRange;				//	시야 거리.
	WORD				m_wAvoidRate;				//	회피율.
	WORD				m_wHitRate;					//	명중율.
	DWORD				m_dwBonusExp;				//	추가 경험치.
	WORD				m_wBonusCP;
	SRESIST				m_sResist;					//	저항값.
	bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
	DWORD				m_dwGenTime;				// 호출 시간
	WORD				m_wGatherTimeLow;			// 채집시간(Low)
	WORD				m_wGatherTimeHigh;			// 채집시간(High)
	float				m_fGatherRate;				// 채집확률
	WORD				m_wGatherAnimation;			// 애니메이션
	PETTYPE				m_emPetType;				// 팻타입
	SUMMON_TYPE			m_emSummonType;
	EMNPC_TYPE			m_emNPCType;
	bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
	DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */
	EMNPC_CTF_FLAG_TYPE	m_emCTFType;				/*pvp capture the flag, Juver, 2018/02/03 */
	EMCROW_SCHOOL		m_emCrowSchool;				/*crow school, Juver, 2018/08/02 */
	bool				m_bBlockPushPull;			/*block crow push/pull, Juver, 2019/02/14 */
	bool				m_bBlockStun;

	DWORD				dummy_var_0;
	bool				dummy_var_1;
	bool				dummy_var_2;
	float				dummy_var_3;
	bool				dummy_var_4;
	WORD				dummy_var_5;
	float				dummy_var_6;
	float				dummy_var_7;
	float				dummy_var_8;

	DWORD				m_dwCrowFlag;
	
	SCROWBASIC (void) :
		m_emCrow(CROW_MOB),
		m_emTribe(TRIBE_HUMAN),
		m_emBright(BRIGHT_LIGHT),
		m_wLevel(1),
		m_dwHP(70),
		m_wMP(40),
		m_wSP(20),
		m_fIncHP(0.3f),
		m_fIncMP(0.3f),
		m_fIncSP(0.5f),
		m_wDefense(0),
		m_wViewRange(80),
		m_wAvoidRate(2),
		m_wHitRate(4),
		m_dwBonusExp(0),
		m_wBonusCP(0),
		m_bOverlapAttack(FALSE),
		m_dwGenTime(0),
		m_wGatherTimeLow(0),
		m_wGatherTimeHigh(0),
		m_fGatherRate(0.0f),
		m_wGatherAnimation(0),
		m_emPetType(PET_TYPE_A),
		m_emSummonType(SUMMON_TYPE_A),
		m_emNPCType(EMNPC_TYPE_NORMAL),
		m_bHideName(false),						/*hide crow name, Juver, 2017/09/01 */
		m_dwNotice(0),							/*crow notice, Juver, 2017/11/25 */
		m_emCTFType(EMNPC_CTF_FLAG_TYPE_NONE),	/*pvp capture the flag, Juver, 2018/02/03 */
		m_emCrowSchool(EMCROW_SCHOOL_SIZE),		/*crow school, Juver, 2018/08/02 */
		m_bBlockPushPull(false),				/*block crow push/pull, Juver, 2019/02/14 */
		m_bBlockStun(false),
		dummy_var_0(0),
		dummy_var_1(false),
		dummy_var_2(false),
		dummy_var_3(1.0f),
		dummy_var_4(false),
		dummy_var_5(100),
		dummy_var_6(50.f),
		dummy_var_7(5.f),
		dummy_var_8(-1.f),

		m_dwCrowFlag(0)
	{
		SecureZeroMemory ( m_szName, sizeof(m_szName) );
	}

	SCROWBASIC& operator = ( const SCROWBASIC_100 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_101 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_102 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_103 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_104 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_105 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_106 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_108 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_109 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_110 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_113 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_115 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_116 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_117 &sCrowBasic );

	SCROWBASIC& operator = ( const SCROWBASIC_120 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_121 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_122 &sCrowBasic );

	SCROWBASIC& operator = ( const SCROWBASIC_200 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_201 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_202 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_203 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_204 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_205 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_206 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_207 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_208 &sCrowBasic );

	SCROWBASIC& operator = ( const SCROWBASIC_209 &sCrowBasic );
	SCROWBASIC& operator = ( const SCROWBASIC_210 &sCrowBasic );

	static VOID SaveCsvHead ( std::fstream &SFile );
	VOID SaveCsv ( std::fstream &SFile );
	VOID LoadCsv ( CStringArray &StrArray, int& iCsvCur );

	BOOL IsNoTransform() { return (m_dwCrowFlag&CROW_BASIC_FLAG_NO_TRANSFORM)!=NULL; }
};

#endif // GLCROWDATA_BASIC_H_