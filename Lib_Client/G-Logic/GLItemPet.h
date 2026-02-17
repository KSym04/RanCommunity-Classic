#ifndef GLITEM_PET_H_
#define GLITEM_PET_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib_Engine/Common/BaseStream.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/SerialFile.h"
#include "../../Lib_Engine/Common/STRINGUTILS.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

#include "./GLItemDef.h"

enum PETTYPE
{
	PET_TYPE_NONE   = -1,
	PET_TYPE_A		= 0,
	PET_TYPE_B		= 1,
	PET_TYPE_C		= 2,
	PET_TYPE_D		= 3,
	PET_TYPE_E		= 4,
	PET_TYPE_F		= 5,

	PET_TYPE_G		= 6,
	PET_TYPE_H		= 7,
	PET_TYPE_I		= 8,
	PET_TYPE_J		= 9,
	PET_TYPE_K		= 10,
	PET_TYPE_L		= 11,
	PET_TYPE_M		= 12,
	PET_TYPE_N		= 13,

	/* new pet types, Juver, 2020/09/15 */
	PET_TYPE_014		= 14, //Pet Type_Phoenix
	PET_TYPE_015		= 15, //Pet Type_Fafnir
	PET_TYPE_016		= 16, //Pet Type_Jormungandr
	PET_TYPE_017		= 17, //Pet Type_Sphinx
	PET_TYPE_018		= 18, //Pet Type_Behemoth
	PET_TYPE_019		= 19, //Pet Type_Gaia
	PET_TYPE_020		= 20, //Pet Type_Dragon
	PET_TYPE_021		= 21, //Pet Type_Love Choco
	PET_TYPE_022		= 22, //Pet Type_Lonely Pop
	PET_TYPE_023		= 23, //Pet Type_Vampire
	PET_TYPE_024		= 24, //Pet Type_Chloe
	PET_TYPE_025		= 25, //Pet Type_Firebird
	PET_TYPE_026		= 26, //Pet Type_Mummy
	PET_TYPE_027		= 27, //Pet Type_Minty

	PET_TYPE_SIZE		= 28,
};

enum PET_ACCESSORY_TYPE
{
	PET_ACCETYPE_A		= 0,	// 赣府, 个烹
	PET_ACCETYPE_B		= 1,	// 迫, 促府, 部府
	PET_ACCETYPE_SIZE	= 2,
};

namespace ITEM
{
	struct SPET_102
	{
		DWORD				dwPetID;   // 绊蜡ID
		PET_ACCESSORY_TYPE	emType;    // 厩技辑府TYPE
		PETTYPE				emPetType; // TYPE
		SNATIVEID			sPetID;    // CROWID
	};

	struct SPET	// PetData
	{
		enum { VERSION = 0x0103 };

		PETTYPE				emPetType; // TYPE
		DWORD				dwPetID;   // 绊蜡ID
		PET_ACCESSORY_TYPE	emType;    // 厩技辑府TYPE
		SNATIVEID			sPetID;    // CROWID

		SPET() 
			: emPetType(PET_TYPE_SIZE)
			, dwPetID(0)
			, emType(PET_ACCETYPE_SIZE)
			, sPetID(NATIVEID_NULL())
		{
		}

		BOOL LOAD ( basestream &SFile );
		BOOL SAVE ( CSerialFile &SFile );

		static VOID SaveCsvHead ( std::fstream &SFile );
		VOID SaveCsv ( std::fstream &SFile );
		VOID LoadCsv ( CStringArray &StrArray, int &iCsvCur );
	};
};

namespace COMMENT
{
	extern std::string ITEMSLOT_PET[PET_ACCETYPE_SIZE];
};

#endif // GLITEM_PET_H_