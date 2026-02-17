/*!
 * \file GLSetOptionDefine.h
 *
 * \author Juver
 * \date 2021/07/30
 *
 * 
 */

#ifndef GLSETOPTIONDEFINE_H_INCLUDED__
#define GLSETOPTIONDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>

#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/basestream.h"

#define SET_OPTION_NULL 0

enum EMSET_OPTION_SLOT
{
	EMSET_OPTION_SLOT_HEADGEAR		= 0,
	EMSET_OPTION_SLOT_UPPER			= 1,
	EMSET_OPTION_SLOT_LOWER			= 2,
	EMSET_OPTION_SLOT_HAND			= 3,
	EMSET_OPTION_SLOT_FOOT			= 4,
	EMSET_OPTION_SLOT_WEAPON		= 5,
	EMSET_OPTION_SLOT_NECK			= 6,
	EMSET_OPTION_SLOT_WRIST			= 7,
	EMSET_OPTION_SLOT_FINGER		= 8,
	EMSET_OPTION_SLOT_BELT			= 9,
	EMSET_OPTION_SLOT_EARRING		= 10,
	EMSET_OPTION_SLOT_ACCESSORY		= 11,
	EMSET_OPTION_SLOT_DECORATION	= 12,

	//warning changing this require new data version!
	EMSET_OPTION_SLOT_SIZE			= 13,
};

enum EMSET_OPTION_VAR
{
	EMSET_OPTION_VAR_NONE		= 0, //???

	EMSET_OPTION_VAR_HITRATE	= 1,
	EMSET_OPTION_VARD_AVOIDRATE	= 2,

	EMSET_OPTION_VAR_DAMAGE		= 3,
	EMSET_OPTION_VAR_DEFENSE	= 4,

	EMSET_OPTION_VAR_HP			= 5,
	EMSET_OPTION_VAR_MP			= 6,
	EMSET_OPTION_VAR_SP			= 7,

	EMSET_OPTION_VAR_STATS_POW	= 8,
	EMSET_OPTION_VAR_STATS_STR	= 9,
	EMSET_OPTION_VAR_STATS_SPI	= 10,
	EMSET_OPTION_VAR_STATS_DEX	= 11,
	EMSET_OPTION_VAR_STATS_INT	= 12,
	EMSET_OPTION_VAR_STATS_STA	= 13,	

	EMSET_OPTION_VAR_PA			= 14,
	EMSET_OPTION_VAR_SA			= 15,	
	EMSET_OPTION_VAR_MA			= 16,

	EMSET_OPTION_VAR_MAX_CP		= 17,

	EMSET_OPTION_VAR_REC_HP			= 18,
	EMSET_OPTION_VAR_REC_MP			= 19,
	EMSET_OPTION_VAR_REC_SP			= 20,
	EMSET_OPTION_VAR_REC_AP			= 21,
	EMSET_OPTION_VAR_MOVE_SPEED		= 22,
	EMSET_OPTION_VAR_ATTACK_SPEED	= 23,
	EMSET_OPTION_VAR_CRITICAL_RATE	= 24,
	EMSET_OPTION_VAR_CRUSHING_BLOW	= 25, 


	EMSET_OPTION_VAR_SIZE		= 26,
};


struct SSET_OPTION_DATA
{
	enum 
	{ 
		VERSION = 0x100, 
		VAR_STAGES = EMSET_OPTION_SLOT_SIZE - 1,
	};

	WORD			wID;
	std::string		strName;

	bool		bUseParts[EMSET_OPTION_SLOT_SIZE];

	EMSET_OPTION_VAR	emOptionType[VAR_STAGES];
	float				fOptionVar[VAR_STAGES];

	SSET_OPTION_DATA();

	void Load( basestream &SFile );
	void Load_0100( basestream &SFile );
	void Save( CSerialFile &SFile );

	static VOID SaveCsvHead ( std::fstream &SFile );
	VOID SaveCsv ( std::fstream &SFile );
	VOID LoadCsv ( CStringArray &StrArray );
};


typedef std::map<WORD, SSET_OPTION_DATA>	SET_OPTION_DATA_MAP;
typedef SET_OPTION_DATA_MAP::iterator		SET_OPTION_DATA_MAP_ITER;


struct SSET_OPTION_GEN_CHANCE
{
	WORD wGenID;
	float fGenChance;

	SSET_OPTION_GEN_CHANCE();

	//override < operator for sorting
	bool operator < ( const SSET_OPTION_GEN_CHANCE& rVal )
	{
		if ( fGenChance < rVal.fGenChance )	return true;

		return false;
	}

};

typedef std::vector<SSET_OPTION_GEN_CHANCE> VEC_SET_OPTION_GEN_CHANCE;

struct SSET_OPTION_GEN
{
	enum
	{
		VERSION = 0x100,
		MAX_ID	= 20,
	};

	std::string strGenName;

	float fChance;

	VEC_SET_OPTION_GEN_CHANCE vecGen;

	SSET_OPTION_GEN();


	void Load( basestream &SFile );
	void Load_0100( basestream &SFile );
	void Save( CSerialFile &SFile );

	static VOID SaveCsvHead ( std::fstream &SFile );
	VOID SaveCsv ( std::fstream &SFile );
	VOID LoadCsv ( CStringArray &StrArray );
};

typedef std::map<std::string, SSET_OPTION_GEN>		SET_OPTION_GEN_MAP;
typedef SET_OPTION_GEN_MAP::iterator				SET_OPTION_GEN_MAP_ITER;

namespace COMMENT
{
	extern std::string SET_OPTION_SLOT[EMSET_OPTION_SLOT_SIZE];
	extern std::string SET_OPTION_VAR[EMSET_OPTION_VAR_SIZE];
	extern bool bSET_OPTION_VAR_PERCENT[EMSET_OPTION_VAR_SIZE];
	extern float fSET_OPTION_VAR_MULTIPLIER[EMSET_OPTION_VAR_SIZE];

};// namespace COMMENT


#endif // GLSETOPTIONDEFINE_H_INCLUDED__
