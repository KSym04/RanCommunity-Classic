#include "stdafx.h"

#include "../../Lib_Engine/Common/StringUtils.h"

#include "./GLSetOptionDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



SSET_OPTION_DATA::SSET_OPTION_DATA()
	: wID( SET_OPTION_NULL )
	, strName("")
{
	for ( int i=0; i<EMSET_OPTION_SLOT_SIZE; ++i )
	{
		bUseParts[i] = FALSE;
	}

	for ( int i=0; i<VAR_STAGES; ++i )
	{
		emOptionType[i] = EMSET_OPTION_VAR_NONE;
		fOptionVar[i] = 0.0f;
	}
}

void SSET_OPTION_DATA::Load( basestream &SFile )
{
	DWORD dwVer( 0 ), dwSize( 0 );
	SFile >> dwVer;
	SFile >> dwSize;

	switch( dwVer )
	{
	case VERSION:
		{
			Load_0100( SFile );
		}break;

	default:
		{
			CDebugSet::ErrorVersion( "SSET_OPTION::Load", dwVer );
			SFile.SetOffSet( SFile.GetfTell() + dwSize );
		}break;
	};
}

void SSET_OPTION_DATA::Load_0100( basestream &SFile )
{
	SFile >> wID;
	SFile >> strName;

	for ( int i=0; i<EMSET_OPTION_SLOT_SIZE; ++i )
	{
		SFile >> bUseParts[i];
	}

	for ( int i=0; i<VAR_STAGES; ++i )
	{
		DWORD dwType(0);
		SFile >> dwType;	emOptionType[i] = (EMSET_OPTION_VAR)dwType;;

		SFile >> fOptionVar[i];
	}
}

void SSET_OPTION_DATA::Save( CSerialFile &SFile )
{
	SFile << static_cast<int>( SSET_OPTION_DATA::VERSION );
	SFile.BeginBlock(); 
	{
		SFile << wID;
		SFile << strName;

		for ( int i=0; i<EMSET_OPTION_SLOT_SIZE; ++i )
		{
			SFile << bUseParts[i];
		}

		for ( int i=0; i<VAR_STAGES; ++i )
		{
			SFile << (DWORD)emOptionType[i]; 
			SFile << fOptionVar[i];
		}
	}
	SFile.EndBlock();
}

VOID SSET_OPTION_DATA::SaveCsvHead ( std::fstream &SFile )
{
	SFile << "wID" << ",";
	SFile << "strName" << ",";	

	for ( int i=0; i<EMSET_OPTION_SLOT_SIZE; ++i )
	{
		SFile << "bUseParts" << i << ",";	
	}

	for ( int i=0; i<SSET_OPTION_DATA::VAR_STAGES; ++i )
	{
		SFile << "emOptionType" << i << ",";	
		SFile << "fOptionVar" << i << ",";	
	}

	SFile << std::endl;
}

VOID SSET_OPTION_DATA::SaveCsv ( std::fstream &SFile )
{
	SFile << wID << ",";

	STRUTIL::OutputStrCsv( SFile, strName );

	for ( int i=0; i<EMSET_OPTION_SLOT_SIZE; ++i )
	{
		SFile << bUseParts[i] << ",";
	}

	for ( int i=0; i<SSET_OPTION_DATA::VAR_STAGES; ++i )
	{
		SFile << (DWORD)emOptionType[i] << ",";
		SFile << fOptionVar[i] << ",";
	}

	SFile << std::endl;
}

VOID SSET_OPTION_DATA::LoadCsv ( CStringArray &StrArray )	
{
	int iCsvCur = 0;

	wID = (WORD)atoi( StrArray[ iCsvCur++ ] );

	STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strName );

	for ( int i=0; i<EMSET_OPTION_SLOT_SIZE; ++i )
	{
		BOOL bSlot = (BOOL)atoi( StrArray[ iCsvCur++ ] );
		bUseParts[i] = bSlot? true:false;
	}

	for ( int i=0; i<SSET_OPTION_DATA::VAR_STAGES; ++i )
	{
		emOptionType[i] = (EMSET_OPTION_VAR)atoi( StrArray[ iCsvCur++ ] );
		fOptionVar[i] = (float)atof( StrArray[ iCsvCur++ ] );
	}

	
}


SSET_OPTION_GEN_CHANCE::SSET_OPTION_GEN_CHANCE()
	: wGenID(SET_OPTION_NULL)
	, fGenChance(0.0f)
{

}


SSET_OPTION_GEN::SSET_OPTION_GEN()
	: strGenName("")
	, fChance(0.0f)
{
}

void SSET_OPTION_GEN::Load( basestream &SFile )
{
	DWORD dwVer( 0 ), dwSize( 0 );
	SFile >> dwVer;
	SFile >> dwSize;

	switch( dwVer )
	{
	case VERSION:
		{
			Load_0100( SFile );
		}break;

	default:
		{
			CDebugSet::ErrorVersion( "SSET_OPTION_GEN::Load", dwVer );
			SFile.SetOffSet( SFile.GetfTell() + dwSize );
		}break;
	};
}

void SSET_OPTION_GEN::Load_0100( basestream &SFile )
{
	SFile >> strGenName;
	SFile >> fChance;

	DWORD dwSize(0);
	SFile >> dwSize;

	for ( DWORD i=0; i<dwSize; ++i )
	{
		SSET_OPTION_GEN_CHANCE sGenChance;
		SFile >> sGenChance.wGenID; 
		SFile >> sGenChance.fGenChance;

		if ( vecGen.size() < MAX_ID )
		{
			vecGen.push_back( sGenChance );
		}
	}

	std::sort( vecGen.begin(), vecGen.end() );
}

void SSET_OPTION_GEN::Save( CSerialFile &SFile )
{
	SFile << static_cast<int>( SSET_OPTION_GEN::VERSION );
	SFile.BeginBlock(); 
	{
		SFile << strGenName;
		SFile << fChance;

		DWORD dwSize = (DWORD)vecGen.size();
		SFile << dwSize;

		for ( DWORD i=0; i<dwSize; ++i )
		{
			SFile << vecGen[i].wGenID; 
			SFile << vecGen[i].fGenChance;
		}
	}
	SFile.EndBlock();
}

VOID SSET_OPTION_GEN::SaveCsvHead ( std::fstream &SFile )
{
	SFile << "strGenName" << ",";	
	SFile << "fChance" << ",";	

	for ( DWORD i=0; i<MAX_ID; ++i )
	{
		SFile << "wGenID" << i << ",";	
		SFile << "fGenChance" << i << ",";	
	}

	SFile << std::endl;
}

VOID SSET_OPTION_GEN::SaveCsv ( std::fstream &SFile )
{
	STRUTIL::OutputStrCsv( SFile, strGenName );
	SFile << fChance << ",";

	for ( DWORD i=0; i<MAX_ID; ++i )
	{
		if ( i < vecGen.size() )
		{
			SFile << vecGen[i].wGenID << ",";
			SFile << vecGen[i].fGenChance << ",";
		}
		else
		{
			SSET_OPTION_GEN_CHANCE sGenChance;
			SFile << sGenChance.wGenID << ",";
			SFile << sGenChance.fGenChance << ",";
		}
	}

	SFile << std::endl;
}

VOID SSET_OPTION_GEN::LoadCsv ( CStringArray &StrArray )	
{
	int iCsvCur = 0;

	STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strGenName );
	fChance = (float)atof( StrArray[ iCsvCur++ ] );

	for ( DWORD i=0; i<MAX_ID; ++i )
	{
		SSET_OPTION_GEN_CHANCE sGenChance;
		sGenChance.wGenID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sGenChance.fGenChance = (float)atof( StrArray[ iCsvCur++ ] );

		if ( sGenChance.wGenID != SET_OPTION_NULL &&
			sGenChance.fGenChance != 0.0f )
		{
			vecGen.push_back( sGenChance );
		}
	}

	std::sort( vecGen.begin(), vecGen.end() );
}



namespace COMMENT
{
	std::string SET_OPTION_SLOT[EMSET_OPTION_SLOT_SIZE] = 
	{
		"Hats",				//EMSET_OPTION_SLOT_HEADGEAR
		"Top",				//EMSET_OPTION_SLOT_UPPER
		"Bottom",			//EMSET_OPTION_SLOT_LOWER
		"Gloves",			//EMSET_OPTION_SLOT_HAND
		"Shoes",			//EMSET_OPTION_SLOT_FOOT
		"Weapon",			//EMSET_OPTION_SLOT_WEAPON
		"Necklace",			//EMSET_OPTION_SLOT_NECK
		"Bracelet",			//EMSET_OPTION_SLOT_WRIST
		"Ring",				//EMSET_OPTION_SLOT_FINGER
		"Belt",				//EMSET_OPTION_SLOT_BELT
		"Earring",			//EMSET_OPTION_SLOT_EARRING
		"Accessory",		//EMSET_OPTION_SLOT_ACCESSORY
		"Decoration",		//EMSET_OPTION_SLOT_DECORATION
	};

	std::string SET_OPTION_VAR[EMSET_OPTION_VAR_SIZE] = 
	{
		"None",							//EMSET_OPTION_VAR_NONE
		"Hit Rate",						//EMSET_OPTION_VAR_HITRATE
		"Avoid Rate",					//EMSET_OPTION_VARD_AVOIDRATE
		"Attack",						//EMSET_OPTION_VAR_DAMAGE
		"Defense",						//EMSET_OPTION_VAR_DEFENSE
		"Maximum HP",					//EMSET_OPTION_VAR_HP
		"Maximum MP",					//EMSET_OPTION_VAR_MP
		"Maximum SP",					//EMSET_OPTION_VAR_SP
		"Stats POW",					//EMSET_OPTION_VAR_STATS_POW
		"Stats STR",					//EMSET_OPTION_VAR_STATS_STR
		"Stats SPI",					//EMSET_OPTION_VAR_STATS_SPI
		"Stats DEX",					//EMSET_OPTION_VAR_STATS_DEX
		"Stats INT",					//EMSET_OPTION_VAR_STATS_INT
		"Stats STM",					//EMSET_OPTION_VAR_STATS_STA
		"Melee",						//EMSET_OPTION_VAR_PA
		"Missile",						//EMSET_OPTION_VAR_SA
		"Energy",						//EMSET_OPTION_VAR_MA
		"Max CP",						//EMSET_OPTION_VAR_MAX_CP

		"HP Recover",					//EMSET_OPTION_VAR_REC_HP
		"MP Recover",					//EMSET_OPTION_VAR_REC_MP
		"SP Recover",					//EMSET_OPTION_VAR_REC_SP
		"HP+MP+SP Recover",				//EMSET_OPTION_VAR_REC_AP
		"Movement Speed",				//EMSET_OPTION_VAR_MOVE_SPEED
		"Attack Speed",					//EMSET_OPTION_VAR_ATTACK_SPEED
		"Critical Rate",				//EMSET_OPTION_VAR_CRITICAL_RATE
		"CrushingBlow Rate",			//EMSET_OPTION_VAR_CRUSHING_BLOW
	};

	bool bSET_OPTION_VAR_PERCENT[EMSET_OPTION_VAR_SIZE] = 
	{
		false,							//EMSET_OPTION_VAR_NONE
		false,							//EMSET_OPTION_VAR_HITRATE
		false,							//EMSET_OPTION_VARD_AVOIDRATE
		false,							//EMSET_OPTION_VAR_DAMAGE
		false,							//EMSET_OPTION_VAR_DEFENSE
		false,							//EMSET_OPTION_VAR_HP
		false,							//EMSET_OPTION_VAR_MP
		false,							//EMSET_OPTION_VAR_SP
		false,							//EMSET_OPTION_VAR_STATS_POW
		false,							//EMSET_OPTION_VAR_STATS_STR
		false,							//EMSET_OPTION_VAR_STATS_SPI
		false,							//EMSET_OPTION_VAR_STATS_DEX
		false,							//EMSET_OPTION_VAR_STATS_INT
		false,							//EMSET_OPTION_VAR_STATS_STA
		false,							//EMSET_OPTION_VAR_PA
		false,							//EMSET_OPTION_VAR_SA
		false,							//EMSET_OPTION_VAR_MA
		false,							//EMSET_OPTION_VAR_MAX_CP
		true,							//EMSET_OPTION_VAR_REC_HP
		true,							//EMSET_OPTION_VAR_REC_MP
		true,							//EMSET_OPTION_VAR_REC_SP
		true,							//EMSET_OPTION_VAR_REC_AP
		true,							//EMSET_OPTION_VAR_MOVE_SPEED
		true,							//EMSET_OPTION_VAR_ATTACK_SPEED
		true,							//EMSET_OPTION_VAR_CRITICAL_RATE
		true,							//EMSET_OPTION_VAR_CRUSHING_BLOW
	};

	float fSET_OPTION_VAR_MULTIPLIER[EMSET_OPTION_VAR_SIZE] = 
	{
		1.0f,							//EMSET_OPTION_VAR_NONE
		1.0f,							//EMSET_OPTION_VAR_HITRATE
		1.0f,							//EMSET_OPTION_VARD_AVOIDRATE
		1.0f,							//EMSET_OPTION_VAR_DAMAGE
		1.0f,							//EMSET_OPTION_VAR_DEFENSE
		1.0f,							//EMSET_OPTION_VAR_HP
		1.0f,							//EMSET_OPTION_VAR_MP
		1.0f,							//EMSET_OPTION_VAR_SP
		1.0f,							//EMSET_OPTION_VAR_STATS_POW
		1.0f,							//EMSET_OPTION_VAR_STATS_STR
		1.0f,							//EMSET_OPTION_VAR_STATS_SPI
		1.0f,							//EMSET_OPTION_VAR_STATS_DEX
		1.0f,							//EMSET_OPTION_VAR_STATS_INT
		1.0f,							//EMSET_OPTION_VAR_STATS_STA
		1.0f,							//EMSET_OPTION_VAR_PA
		1.0f,							//EMSET_OPTION_VAR_SA
		1.0f,							//EMSET_OPTION_VAR_MA
		1.0f,							//EMSET_OPTION_VAR_MAX_CP
		100.0f,							//EMSET_OPTION_VAR_REC_HP
		100.0f,							//EMSET_OPTION_VAR_REC_MP
		100.0f,							//EMSET_OPTION_VAR_REC_SP
		100.0f,							//EMSET_OPTION_VAR_REC_AP
		100.0f,							//EMSET_OPTION_VAR_MOVE_SPEED
		100.0f,							//EMSET_OPTION_VAR_ATTACK_SPEED
		100.0f,							//EMSET_OPTION_VAR_CRITICAL_RATE
		100.0f,							//EMSET_OPTION_VAR_CRUSHING_BLOW
	};




};// namespace COMMENT