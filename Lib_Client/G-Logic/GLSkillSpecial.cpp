#include "stdafx.h"
#include "./GLSkillSpecial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace SKILL
{
	BOOL SSPECIAL_SKILL::LOAD_0100( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;
		SFile >> strEffectName;
		SFile >> dwRemainSecond;
		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}
		return TRUE;
	}

	BOOL SSPECIAL_SKILL::LOAD_0101( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;
		SFile >> dwRemainSecond;
		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}

		SFile >> strEffectName;

		return TRUE;
	}

	BOOL SSPECIAL_SKILL::LOAD_0103( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;
		SFile >> strEffectName;
		SFile >> dwRemainSecond;

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}

		for( int i=0; i<MAX_SKILL_LEVEL; ++i )
		{
			SFile >> sSummon[i].sidCrow.dwID;
			SFile >> sSummon[i].fDelay;
			SFile >> sSummon[i].fLife;
			SFile >> sSummon[i].dwMaxNum;
			SFile >> sSummon[i].wSkillLevel;
		}

		return TRUE;
	}

	BOOL SSPECIAL_SKILL::LOAD_0104( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;
		SFile >> strEffectName;
		SFile >> dwRemainSecond;

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}

		for( int i=0; i<MAX_SKILL_LEVEL; ++i )
		{
			SFile >> sSummon[i].sidCrow.dwID;
			SFile >> sSummon[i].fDelay;
			SFile >> sSummon[i].fLife;
			SFile >> sSummon[i].dwMaxNum;
			SFile >> sSummon[i].wSkillLevel;
			SFile >> sSummon[i].unknown_var_1;
			SFile >> sSummon[i].unknown_var_2;
		}

		return TRUE;
	}

	BOOL SSPECIAL_SKILL::LOAD( basestream &SFile )
	{
		DWORD dwDATA;
		SFile >> dwDATA; emSSTYPE = (EMSPECIALSKILLTYPE)dwDATA;
		SFile >> strEffectName;
		SFile >> dwRemainSecond;

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile >> strTransform_Man[i];
			SFile >> strTransform_Woman[i];
		}

		for( int i=0; i<MAX_SKILL_LEVEL; ++i )
		{
			SFile >> sSummon[i].sidCrow.dwID;
			SFile >> sSummon[i].fDelay;
			SFile >> sSummon[i].fLife;
			SFile >> sSummon[i].dwMaxNum;
			SFile >> sSummon[i].wSkillLevel;
			SFile >> sSummon[i].unknown_var_1;
			SFile >> sSummon[i].unknown_var_2;
		}

		return TRUE;
	}

	BOOL SSPECIAL_SKILL::SAVE( CSerialFile &SFile )
	{
		SFile << (DWORD)emSSTYPE;
		SFile << strEffectName;
		SFile << dwRemainSecond;	

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			SFile << strTransform_Man[i];
			SFile << strTransform_Woman[i];
		}

		for( int i=0; i<MAX_SKILL_LEVEL; ++i )
		{
			SFile << sSummon[i].sidCrow.dwID;
			SFile << sSummon[i].fDelay;
			SFile << sSummon[i].fLife;
			SFile << sSummon[i].dwMaxNum;
			SFile << sSummon[i].wSkillLevel;
			SFile << sSummon[i].unknown_var_1;
			SFile << sSummon[i].unknown_var_2;
		}

		return TRUE;
	}

	SSPECIAL_SKILL& SSPECIAL_SKILL::operator= ( const SSPECIAL_SKILL &value )
	{
		emSSTYPE	   = value.emSSTYPE;
		dwRemainSecond = value.dwRemainSecond;
		strEffectName  = value.strEffectName;

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			strTransform_Man[i]		  = value.strTransform_Man[i];
			strTransform_Woman[i]	  = value.strTransform_Woman[i];
		}

		for( int i=0; i<MAX_SKILL_LEVEL; ++i )
			sSummon[i] = value.sSummon[i];

		return *this;
	}

	VOID SSPECIAL_SKILL::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "Special Skill Type" << ",";
		SFile << "Special Skill Effect" << ",";
		SFile << "Effect Remain Second" << ",";

		SFile << "MAN_HEAD_File" << ",";
		SFile << "MAN_FACE_File" << ",";
		SFile << "MAN_UPPER_File" << ",";
		SFile << "MAN_LOWER_File" << ",";
		SFile << "MAN_LHAND_File" << ",";
		SFile << "MAN_RHAND_File" << ",";
		SFile << "MAN_FOOT_File" << ",";
		SFile << "MAN_HAND_File" << ",";
		SFile << "WOMAN_HEAD_File" << ",";
		SFile << "WOMAN_FACE_File" << ",";
		SFile << "WOMAN_UPPER_File" << ",";
		SFile << "WOMAN_LOWER_File" << ",";
		SFile << "WOMAN_LHAND_File" << ",";
		SFile << "WOMAN_RHAND_File" << ",";
		SFile << "WOMAN_FOOT_File" << ",";
		SFile << "WOMAN_HAND_File" << ",";

		for( int i = 1; i < SKILL::MAX_SKILL_LEVEL+1; i++ )
		{
			SFile << "sidCrow MID " << i << ",";
			SFile << "sidCrow SID " << i << ",";
			SFile << "fDelay " << i << ",";
			SFile << "fLife " << i << ",";
			SFile << "dwMaxNum " << i << ",";
			SFile << "wSkillLevel " << i << ",";
			SFile << "unknown_var_1 " << i << ",";
			SFile << "unknown_var_2 " << i << ",";
		}
	}

	VOID SSPECIAL_SKILL::SaveCsv ( std::fstream &SFile )
	{
		SFile << (DWORD)emSSTYPE << ",";
		STRUTIL::OutputStrCsv( SFile, strEffectName );
		SFile << dwRemainSecond << ",";
		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			STRUTIL::OutputStrCsv( SFile, strTransform_Man[i] );
		}
		for(int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			STRUTIL::OutputStrCsv( SFile, strTransform_Woman[i] );
		}

		for( int i = 0; i < SKILL::MAX_SKILL_LEVEL; i++ )
		{
			SFile << sSummon[i].sidCrow.wMainID << ",";
			SFile << sSummon[i].sidCrow.wSubID << ",";
			SFile << sSummon[i].fDelay << ",";
			SFile << sSummon[i].fLife << ",";
			SFile << sSummon[i].dwMaxNum << ",";
			SFile << sSummon[i].wSkillLevel << ",";
			SFile << sSummon[i].unknown_var_1 << ",";
			SFile << sSummon[i].unknown_var_2 << ",";
		}
	}

	VOID SSPECIAL_SKILL::LoadCsv ( CStringArray &StrArray, int &iCsvCur )
	{
		emSSTYPE	= (EMSPECIALSKILLTYPE)atoi( StrArray[ iCsvCur++ ] );
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strEffectName );
		dwRemainSecond = (DWORD)atoi( StrArray[ iCsvCur++ ] );

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTransform_Man[i] );
		}

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTransform_Woman[i] );
		}

		for( int i = 0; i < SKILL::MAX_SKILL_LEVEL; i++ )
		{
			sSummon[i].sidCrow.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].sidCrow.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].fDelay = (float)atof( StrArray[ iCsvCur++ ] );
			sSummon[i].fLife = (float)atof( StrArray[ iCsvCur++ ] );
			sSummon[i].dwMaxNum = (DWORD)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].wSkillLevel = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].unknown_var_1 = (DWORD)atoi( StrArray[ iCsvCur++ ] );
			sSummon[i].unknown_var_2 = (bool)atoi( StrArray[ iCsvCur++ ] );
		}
	}

	void SSPECIAL_SKILL::ReadStringStream ( std::stringstream &sStream )
	{
		sStream << (DWORD)emSSTYPE << ",";
		sStream << strEffectName << ",";
		sStream << dwRemainSecond << ",";

		for( int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			sStream << strTransform_Man[i] << ",";
		}
		for(int i = 0; i < EMTRANSFORM_NSIZE; i++ )
		{
			sStream << strTransform_Woman[i] << ",";
		}

		for( int i = 0; i < SKILL::MAX_SKILL_LEVEL; i++ )
		{
			sStream << sSummon[i].sidCrow.wMainID << ",";
			sStream << sSummon[i].sidCrow.wSubID << ",";
			sStream << sSummon[i].fDelay << ",";
			sStream << sSummon[i].fLife << ",";
			sStream << sSummon[i].dwMaxNum << ",";
			sStream << sSummon[i].wSkillLevel << ",";
			sStream << sSummon[i].unknown_var_1 << ",";
			sStream << sSummon[i].unknown_var_2 << ",";
		}
	}
};

namespace COMMENT
{
	std::string SPECIAL_SKILL_TYPE[SKILL::EMSSTYPE_NSIZE] =
	{
		"None",
		"Transform(CPS)",
		"Summon",
	};
};