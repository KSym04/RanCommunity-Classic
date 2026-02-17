

#include "stdafx.h"
#include "FileCryptGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace FILE_CRYPT_GLOBAL
{
	std::string file_crypt_type[EMCRYPT_FILETYPE_SIZE] = 
	{
		"TextFiles",
		"StreamFiles",
		"StreamFiles(Ran2)",
		"StreamFiles(Orig)",
		"XML Gui Files",
		"Texture Files",
		"X Files",
		"Animation Bin Files",
		"EGP->NEW",
		"Ran2 EGP->NEW",
		"CPS->NEW",
		"Ran2 CPS->NEW",
		"TextFiles GS",
		"TextFiles GS V4",
	};

	std::string file_crypt_bypass_type[EMCRYPT_BYPASSHEADER_SIZE] = 
	{
		"No Bypass",
		"Bypass 128 Bytes",
		"Bypass 132 Bytes",
		"Bypass 136 Bytes",
	};

	std::string byte_crypt_type[EMRCRYPT_BYTE_TYPE_SIZE] =
	{
		"NONE",
		"OLD",
		"RAN2",

		"ITEM_VER_1",
		"ITEM_VER_2",
		"ITEM_VER_3",

		"SKILL_VER_1",
		"SKILL_VER_2",
		"SKILL_VER_3",

		"CROW_VER_1",
		"CROW_VER_2",
		"CROW_VER_3",

		"LEVEL_VER_1",
		"LEVEL_VER_2",
		"LEVEL_VER_3",

		"NPC_TALK_VER_1",
		"NPC_TALK_VER_2",
		"NPC_TALK_VER_3",

		"QUEST_VER_1",

		"MAPS_LIST_VER_1",
		"MAPS_LIST_VER_2",

		"ITEM_MIX_VER_1",

		"TAXI_STATION_VER_1",

		"RANDOM_OPTION_FILE_VER_1",
		"RANDOM_OPTION_FILE_VER_2",

		"ACTIVITY_FILE_VER_1",
		"ACTIVITY_FILE_VER_2",

		"SKIN_CONTAINER_VER_1",
		"SKIN_CONTAINER_VER_2",

		"SKIN_PIECE_VER_1",

		"ANI_CFG_VER_1",

		"ANI_BIN_VER_1",

		"EFFECT_EGP_VER_1",

		"MAP_SHADER_VER_1",

		"MAP_MATERIAL_VER_1",

		"CLASS_CONST_VER_1",

		"WLD_1_VER_1",
		"WLD_1_VER_2",
		"WLD_1_VER_3",
		"WLD_1_VER_4",

		"WLD_2_VER_1",
		"WLD_2_VER_2",
		"WLD_2_VER_3",
		"WLD_2_VER_4",

		"WLD_3_VER_1",
		"WLD_3_VER_2",
		"WLD_3_VER_3",

		"MAP_PIE_VER_1",
		"MAP_PIE_VER_2",
		"MAP_PIE_VER_3",

		"LAUNCHER_PATCH_LIST_VER_1",

		"SERVER_CFG_VER_1",
		"SERVER_CFG_VER_2",

		"2021_ITEM_V1",
		"2021_SKILL_V1",
		"2021_CROW_V1",
		"2021_LEVEL_V1",
		"2021_NPC_TALK_V1",
		"2021_MAPS_LIST_V1",
		"2021_ITEM_MIX_V1",
		"2021_TAXI_STATION_V1",
		"2021_RANDOM_OPTION_V1",
		"2021_ACTIVITY_V1",
		"2021_CLASS_CONST_V1",
		"2021_SKIN_PIECE_V1",
		"2021_SKIN_CHAR_V1",
		"2021_ATT_BONE_DATA_V1",
		"2021_ATT_BONE_LINK_V1",
		"2021_ATT_VEHICLE_DATA_V1",
		"2021_ATT_EFFECT_GROUP_V1",
		"2021_ATT_ANI_CFG_V1",
		"2021_ATT_ANI_BIN_V1",
		"2021_ATT_WLD1_V1",
		"2021_ATT_WLD2_V1",
		"2021_ATT_WLD3_V1",
		"2021_ATT_PIE_V1",
		"2021_SERVER_CFG_V1",
		"2021_SET_OPTION_V1",
	};
}