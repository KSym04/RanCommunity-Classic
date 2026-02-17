#include "stdafx.h"

#include "./GLCharDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

WORD school2index ( const EMSCHOOLFLAG emSchool )
{
	switch ( emSchool )
	{
	case GLSCHOOL_00:	return 0;
	case GLSCHOOL_01:	return 1;
	case GLSCHOOL_02:	return 2;
	};

	return 0;
}

EMSCHOOLFLAG index2school ( const WORD wSchool )
{
	switch ( wSchool )
	{
	case 0:	return GLSCHOOL_00;
	case 1:	return GLSCHOOL_01;
	case 2:	return GLSCHOOL_02;
	};

	return GLSCHOOL_00;
}

EMCLASSINDEX CharClassToClassIndex ( const EMCHARCLASS emClass )
{
	switch (emClass)
	{
	case GLCC_BRAWLER_M:
	case GLCC_BRAWLER_W:
	case GLCC_BRAWLER:
		return GLCL_BRAWLER;

	case GLCC_SWORDSMAN_M:
	case GLCC_SWORDSMAN_W:
	case GLCC_SWORDSMAN:
		return GLCL_SWORDSMAN;

	case GLCC_ARCHER_M:
	case GLCC_ARCHER_W:
	case GLCC_ARCHER:
		return GLCL_ARCHER;	

	case GLCC_SHAMAN_M:
	case GLCC_SHAMAN_W:
	case GLCC_SHAMAN:
		return GLCL_SHAMAN;

	case GLCC_EXTREME_M:
	case GLCC_EXTREME_W:
	case GLCC_EXTREME:
		return GLCL_EXTREME;

	case GLCC_GUNNER_M:
	case GLCC_GUNNER_W:
	case GLCC_GUNNER:
		return GLCL_GUNNER;

    case GLCC_ASSASSIN_M:
    case GLCC_ASSASSIN_W:
	case GLCC_ASSASSIN:
		return GLCL_ASSASSIN;

	case GLCC_TRICKER_M:
	case GLCC_TRICKER_W:
	case GLCC_TRICKER:
		return GLCL_TRICKER;

    /*case GLCC_ETC_M:
    case GLCC_ETC_W:
	case GLCC_ETC:
		return GLCL_ETC;

	case GLCC_ACTOR_M:
	case GLCC_ACTOR_W:
	case GLCC_ACTOR:
		return GLCL_ACTOR;*/
	};

	return GLCL_BRAWLER;
}

EMCHARINDEX CharClassToIndex ( const EMCHARCLASS emClass )
{
	switch (emClass)
	{
	case GLCC_BRAWLER_M:	return GLCI_BRAWLER_M;
	case GLCC_SWORDSMAN_M:	return GLCI_SWORDSMAN_M;
	case GLCC_ARCHER_W:		return GLCI_ARCHER_W;
	case GLCC_SHAMAN_W:		return GLCI_SHAMAN_W;
	case GLCC_EXTREME_M:	return GLCI_EXTREME_M;
	case GLCC_EXTREME_W:	return GLCI_EXTREME_W;
	
	case GLCC_BRAWLER_W:	return GLCI_BRAWLER_W;
	case GLCC_SWORDSMAN_W:	return GLCI_SWORDSMAN_W;
	case GLCC_ARCHER_M:		return GLCI_ARCHER_M;
	case GLCC_SHAMAN_M:		return GLCI_SHAMAN_M;

	case GLCC_GUNNER_M:		return GLCI_GUNNER_M;
	case GLCC_GUNNER_W:		return GLCI_GUNNER_W;

	case GLCC_ASSASSIN_M:	return GLCI_ASSASSIN_M;
	case GLCC_ASSASSIN_W:	return GLCI_ASSASSIN_W;
	case GLCC_TRICKER_M:	return GLCI_TRICKER_M;
	case GLCC_TRICKER_W:	return GLCI_TRICKER_W;
	};

	GASSERT(0&&"잘못된 케릭터 클래스입니다.");
	return GLCI_BRAWLER_M;
}

EMCHARCLASS CharIndexToClass ( const EMCHARINDEX emIndex )
{
	switch (emIndex)
	{
	case GLCI_BRAWLER_M:	return GLCC_BRAWLER_M;
	case GLCI_SWORDSMAN_M:	return GLCC_SWORDSMAN_M;
	case GLCI_ARCHER_W:		return GLCC_ARCHER_W;
	case GLCI_SHAMAN_W:		return GLCC_SHAMAN_W;

	case GLCI_EXTREME_M:	return GLCC_EXTREME_M;
	case GLCI_EXTREME_W:	return GLCC_EXTREME_W;

	case GLCI_BRAWLER_W:	return GLCC_BRAWLER_W;
	case GLCI_SWORDSMAN_W:	return GLCC_SWORDSMAN_W;
	case GLCI_ARCHER_M:		return GLCC_ARCHER_M;
	case GLCI_SHAMAN_M:		return GLCC_SHAMAN_M;

	case GLCI_GUNNER_M:		return GLCC_GUNNER_M;
	case GLCI_GUNNER_W:		return GLCC_GUNNER_W;

	case GLCI_ASSASSIN_M:	return GLCC_ASSASSIN_M;
	case GLCI_ASSASSIN_W:	return GLCC_ASSASSIN_W;
	case GLCI_TRICKER_M:	return GLCC_TRICKER_M;
	case GLCI_TRICKER_W:	return GLCC_TRICKER_W;
	};

	GASSERT(0&&"잘못된 케릭터 인덱스입니다.");
	return GLCC_BRAWLER_M;
}


EMCHARCLASS CharClassToSex ( const EMCHARCLASS emClass )
{
	switch (emClass)
	{
	case GLCC_BRAWLER_M:	return GLCC_BRAWLER_W;
	case GLCC_SWORDSMAN_M:	return GLCC_SWORDSMAN_W;
	case GLCC_ARCHER_W:		return GLCC_ARCHER_M;
	case GLCC_SHAMAN_W:		return GLCC_SHAMAN_M;
	case GLCC_EXTREME_M:	return GLCC_EXTREME_W;
	case GLCC_EXTREME_W:	return GLCC_EXTREME_M;
	
	case GLCC_BRAWLER_W:	return GLCC_BRAWLER_M;
	case GLCC_SWORDSMAN_W:	return GLCC_SWORDSMAN_M;
	case GLCC_ARCHER_M:		return GLCC_ARCHER_W;
	case GLCC_SHAMAN_M:		return GLCC_SHAMAN_W;

	case GLCC_GUNNER_M:		return GLCC_GUNNER_W;
	case GLCC_GUNNER_W:		return GLCC_GUNNER_M;

	case GLCC_ASSASSIN_M:	return GLCC_ASSASSIN_W;
	case GLCC_ASSASSIN_W:	return GLCC_ASSASSIN_M;
	case GLCC_TRICKER_M:	return GLCC_TRICKER_W;
	case GLCC_TRICKER_W:	return GLCC_TRICKER_M;
	};

	GASSERT(0&&"잘못된 케릭터 클래스입니다.");
	return GLCC_BRAWLER_M;
}

EMCHARINDEX CharIndexToSex ( const EMCHARINDEX emIndex )
{
	switch (emIndex)
	{
	case GLCI_BRAWLER_M:	return GLCI_BRAWLER_W;
	case GLCI_SWORDSMAN_M:	return GLCI_SWORDSMAN_W;
	case GLCI_ARCHER_W:		return GLCI_ARCHER_M;
	case GLCI_SHAMAN_W:		return GLCI_SHAMAN_M;

	case GLCI_EXTREME_M:	return GLCI_EXTREME_W;
	case GLCI_EXTREME_W:	return GLCI_EXTREME_M;

	case GLCI_BRAWLER_W:	return GLCI_SHAMAN_W;
	case GLCI_SWORDSMAN_W:	return GLCI_ARCHER_W;
	case GLCI_ARCHER_M:		return GLCI_SWORDSMAN_M;
	case GLCI_SHAMAN_M:		return GLCI_BRAWLER_M;

	case GLCI_GUNNER_M:		return GLCI_GUNNER_W;
	case GLCI_GUNNER_W:		return GLCI_GUNNER_M;

	case GLCI_ASSASSIN_M:	return GLCI_ASSASSIN_W;
	case GLCI_ASSASSIN_W:	return GLCI_ASSASSIN_M;
	case GLCI_TRICKER_M:	return GLCI_TRICKER_W;
	case GLCI_TRICKER_W:	return GLCI_TRICKER_M;
	};

	GASSERT(0&&"잘못된 케릭터 인덱스입니다.");
	return GLCI_BRAWLER_M;
}

EMCHARGENDER CharClassToGender( const EMCHARCLASS emClass )
{
	switch (emClass)
	{
	case GLCC_BRAWLER_M:
	case GLCC_SWORDSMAN_M:
	case GLCC_ARCHER_M:
	case GLCC_SHAMAN_M:	
	case GLCC_EXTREME_M:
	case GLCC_GUNNER_M:
	case GLCC_ASSASSIN_M:
	case GLCC_TRICKER_M:
		return GLGENDER_M;
		break;

	case GLCC_BRAWLER_W:
	case GLCC_SWORDSMAN_W:
	case GLCC_ARCHER_W:
	case GLCC_SHAMAN_W:
	case GLCC_EXTREME_W:
	case GLCC_GUNNER_W:
	case GLCC_ASSASSIN_W:
	case GLCC_TRICKER_W:
		return GLGENDER_W;
		break;
	};

	GASSERT(0&&"CharClassToGender Invalid emClass.");
	return GLGENDER_M;
}

EMCHARGENDER CharIndexToGender( const EMCHARINDEX emIndex )
{
	switch (emIndex)
	{
	case GLCI_BRAWLER_M:
	case GLCI_SWORDSMAN_M:
	case GLCI_ARCHER_M:
	case GLCI_SHAMAN_M:	
	case GLCI_EXTREME_M:
	case GLCI_GUNNER_M:
	case GLCI_ASSASSIN_M:
	case GLCI_TRICKER_M:
		return GLGENDER_M;
		break;

	case GLCI_BRAWLER_W:
	case GLCI_SWORDSMAN_W:
	case GLCI_ARCHER_W:
	case GLCI_SHAMAN_W:
	case GLCI_EXTREME_W:
	case GLCI_GUNNER_W:
	case GLCI_ASSASSIN_W:
	case GLCI_TRICKER_W:
		return GLGENDER_W;
		break;
	};

	GASSERT(0&&"CharIndexToGender Invalid emIndex.");
	return GLGENDER_M;
}

EMCHARCLASS charclass_change_gender( const EMCHARCLASS emClass )
{
	switch (emClass)
	{
	case GLCC_BRAWLER_M:	return GLCC_BRAWLER_W;
	case GLCC_SWORDSMAN_M:	return GLCC_SWORDSMAN_W;
	case GLCC_ARCHER_W:		return GLCC_ARCHER_M;
	case GLCC_SHAMAN_W:		return GLCC_SHAMAN_M;

	case GLCC_EXTREME_M:	return GLCC_EXTREME_W;
	case GLCC_EXTREME_W:	return GLCC_EXTREME_M;

	case GLCC_BRAWLER_W:	return GLCC_BRAWLER_M;
	case GLCC_SWORDSMAN_W:	return GLCC_SWORDSMAN_M;
	case GLCC_ARCHER_M:		return GLCC_ARCHER_W;
	case GLCC_SHAMAN_M:		return GLCC_SHAMAN_W;

	case GLCC_GUNNER_M:		return GLCC_GUNNER_W;
	case GLCC_GUNNER_W:		return GLCC_GUNNER_M;

	case GLCC_ASSASSIN_M:	return GLCC_ASSASSIN_W;
	case GLCC_ASSASSIN_W:	return GLCC_ASSASSIN_M;

	case GLCC_TRICKER_M:	return GLCC_TRICKER_W;
	case GLCC_TRICKER_W:	return GLCC_TRICKER_M;
	};

	GASSERT(0&&"charclass_change_gender invalid case");
	return GLCC_BRAWLER_M;
}

VOID GetCharSkillClassIndex( const EMCHARCLASS emClass, WORD & wBeginIndex, WORD & wEndIndex )
{
	switch ( emClass )
	{
	case GLCC_BRAWLER_M:
	case GLCC_BRAWLER_W:
		{
			wBeginIndex = EMSKILL_BRAWLER_01;
			wEndIndex = EMSKILL_BRAWLER_04;
		}
		break;

	case GLCC_SWORDSMAN_M:
	case GLCC_SWORDSMAN_W:
		{
			wBeginIndex = EMSKILL_SWORDSMAN_01;
			wEndIndex = EMSKILL_SWORDSMAN_04;
		}
		break;

	case GLCC_ARCHER_M:
	case GLCC_ARCHER_W:
		{
			wBeginIndex = EMSKILL_ARCHER_01;
			wEndIndex = EMSKILL_ARCHER_04;
		}
		break;

	case GLCC_SHAMAN_M:
	case GLCC_SHAMAN_W:
		{
			wBeginIndex = EMSKILL_SHAMAN_01;
			wEndIndex = EMSKILL_SHAMAN_04;
		}
		break;	

	case GLCC_EXTREME_M:
	case GLCC_EXTREME_W:
		{
			wBeginIndex = EMSKILL_EXTREME_01;
			wEndIndex = EMSKILL_EXTREME_04;
		}
		break;

	case GLCC_GUNNER_M:
	case GLCC_GUNNER_W:
		{
			wBeginIndex = EMSKILL_GUNNER_01;
			wEndIndex = EMSKILL_GUNNER_04;
		}
		break;

	case GLCC_ASSASSIN_M:
	case GLCC_ASSASSIN_W:
		{
			wBeginIndex = EMSKILL_ASSASSIN_01;
			wEndIndex = EMSKILL_ASSASSIN_04;
		}
		break;

	case GLCC_TRICKER_M:
	case GLCC_TRICKER_W:
		{
			wBeginIndex = EMSKILL_TRICKER_01;
			wEndIndex = EMSKILL_TRICKER_04;
		}
		break;

	default:
		GASSERT(0&&"잘못된 케릭터 인덱스입니다.");
		break;
	};
}

namespace COMMENT
{
	std::string ACTIONTYPE[GLAT_SIZE] =
	{
		"GLAT_IDLE",
		"GLAT_MOVE",
		"GLAT_ATTACK",
		"GLAT_SKILL",
		"GLAT_SHOCK",
		"GLAT_PUSHPULL",
		"GLAT_GATHERING",
		"GLAT_TALK",
		"GLAT_CONFT_END",
		"GLAT_SKILL_PUSH_PULL",		/* push skill logic, Juver, 2020/12/14 */
		"GLAT_SKILLDASH",			/* dash skill logic, Juver, 2020/12/15 */
		"GLAT_FALLING",
		"GLAT_DIE",
	};

	std::string TRIBE[TRIBE_NSIZE] = 
	{
		"Human",
		"Monster",
		"Angel",
		"Devil",
		"Fairy",
		"Genie",
		"Dragon"
	};

	std::string MOVETYPE[MOVETYPE_NSIZE] = 
	{
		"Ground Movement",
		"Air Movement",
		"Water Movement"
	};

	std::string ELEMENT[EMELEMENT_MAXNUM2] = 
	{
		"Vital Energy(Spirit)",
		"Fire",
		"Ice",
		"Electric",
		"Poison",
		"Stone",
		"Confusion(Mad)",
		"Fainting(Stun)",
		"Malediction(Curse)",
		"Weapon Element",
	};

	std::string BRIGHT[BRIGHT_SIZE] =
	{
		"Light(Good)",
		"Dark(Evil)",
		"Both(Neutral)",
	};

	std::string CHARCLASS[GLCI_NUM_8CLASS] =
	{
		"Brawler Male",
		"Swordsman Male",
		"Archer Female",
		"Shaman Female",
		"Extreme Male",
		"Extreme Female",
		"Brawler Female",
		"Swordsman Female",
		"Archer Male",
		"Shaman Male",
		"Gunner Male",
		"Gunner Female",
		"Assassin Male",
		"Assassin Female",
		"Magician Male",
		"Magician Female",
	};

	std::string CHARCLASS2[GLCI2_NUM_8CLASS+1] =
	{
		"Brawler",
		"Swordsman",
		"Archer",
		"Shaman",
		"Extreme",
		"Gunner",
		"Assassin",
		"Magician",
		"All Class",
	};

	std::string EMSCHOOL[GLSCHOOL_NUM] =
	{
		"Sacred Gate",
		"Mystic Peak",
		"Phoenix",
	};

	std::string EMGENDER[GLGENDER_NUM] =
	{
		"Female",
		"Male",
	};

	std::string CHARCOMMENT[GLCI_NUM_8CLASS] = 
	{
		"", "", "", "", "", "", "", "", "", "",
		"", "", "", "", "", "",
	};

	std::string BLOW[EMBLOW_SIZE] =
	{
		"None",
		"Paralysis(Numb)",
		"Faint(Stun)",
		"Petrochemical(Stone)",
		"Burn",
		"Freeze",
		"Chaos(Mad)",
		"Poison",
		"Curse",
	};

	std::string BLOW_VAR1[EMBLOW_SIZE] =
	{
		"None",
		"Movement Speed",
		"None",
		"Movement Speed",
		"None",
		"Movement Speed",
		"None",
		"None",
		"None",
	};

	float BLOW_VAR1_SCALE[EMBLOW_SIZE] =
	{
		1.0f,
		100.0f,
		1.0f,
		100.0f,
		1.0f,
		100.0f,	
		1.0f,
		1.0f,
		1.0f,
	};

	std::string BLOW_VAR2[EMBLOW_SIZE] =
	{
		"None",
		"DelayRate",
		"None",
		"HP Recovery",
		"HP Recovery",
		"Damage Increase",
		"None",
		"HP Recovery",
		"HP Recovery",
	};

	float BLOW_VAR2_SCALE[EMBLOW_SIZE] =
	{
		1.0f,
		100.0f,
		1.0f,
		1.0f,
		1.0f,
		100.0f,	
		1.0f,
		1.0f,
		1.0f,
	};

	std::string IMPACT_ADDON[EIMPACTA_SIZE] =
	{
		"None",
		"Hit Rate",
		"Avoid Rate",
		"Attack",
		"Defense",
		"HP Recover",
		"MP Recover",
		"SP Recover",
		"HP+MP+SP Recover",
		"Attack Percentage",
		"Defense Percentage",
		"Melee",
		"Missile",
		"Energy",
		"Maximum HP",
		"Maximum MP",
		"Maximum SP",
		"Resistance",
		"Change Stats",
		"HP recovery amount +-",
		"MP recovery amount +-",
		"SP recovery amount +-",
		"CP recovery amount +-",
		"CP Auto +-",
	};

	float IMPACT_ADDON_SCALE[EIMPACTA_SIZE] =
	{
		1.0f,	1.0f,	1.0f,	1.0f,	1.0f,
		100.0f,	100.0f,	100.0f,	100.0f,	100.0f,
		100.0f,	1.0f,	1.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,	1.0f,
	};

	std::string IMPACT_TAR[TAR_SIZE] =
	{
		"Self",
		"Target",
		"From Self To Target",
		"Specific Location",
		"Specified"
	};

	std::string IMPACT_REALM[REALM_SIZE] =
	{
		"Target Self",
		"Target Area",
		"Keep Target Area",
		"Cone Target",
	};

	std::string IMPACT_SIDE[SIDE_SIZE] =
	{
		"Target MySide",
		"Target Enemy",
		"Target Everyone",
	};

	std::string SPEC_ADDON[EMSPECA_NSIZE] =
	{
		"None",									//EMSPECA_NULL
		"Distract Movement",					//EMSPECA_PUSHPULL
		"Return Damage",						//EMSPECA_REFDAMAGE
		"Revive",								//EMSPECA_REBIRTH
		"Steal HP",								//EMSPECA_HP_GATHER
		"Steal MP",								//EMSPECA_MP_GATHER
		"Steal SP",								//EMSPECA_SP_GATHER
		"Share HP",								//EMSPECA_HP_DIV
		"Share MP",								//EMSPECA_MP_DIV
		"Share SP",								//EMSPECA_SP_DIV
		"Prevent Abnormal Condition",			//EMSPECA_NONBLOW
		"Avoid Abnormal Condition",				//EMSPECA_RECBLOW
		"Pierce",								//EMSPECA_PIERCE
		"Target Range",							//EMSPECA_TARRANGE
		"Movement Speed",						//EMSPECA_MOVEVELO
		"Dash",									//EMSPECA_ONWARD
		"Invisible",							//EMSPECA_INVISIBLE
		"Detect Invisible",						//EMSPECA_RECVISIBLE
		"Attack Speed",							//EMSPECA_ATTACKVELO
		"Skill Delay"							//EMSPECA_SKILLDELAY
		"Crushing Blow",						//EMSPECA_CRUSHING_BLOW
		"Melee Damage Absorb",					//EMSPECA_PSY_DAMAGE_REDUCE
		"Magic Damage Absorb",					//EMSPECA_MAGIC_DAMAGE_REDUCE
		"Return Melee Damage",					//EMSPECA_PSY_DAMAGE_REFLECTION
		"Return Magic Damage",					//EMSPECA_MAGIC_DAMAGE_REFLECTION
		"Remove Buff",							//EMSPECA_BUFF_REMOVE
		"Stun",									//EMSPECA_STUN
		"Active Type Skill",					//EMSPECA_DEFENSE_SKILL_ACTIVE
		"Pull",									//EMSPECA_PULL
		"Push",									//EMSPECA_PUSH
		"Continuous Damage",					//EMSPECA_CONTINUOUS_DAMAGE
		"Curse",								//EMSPECA_CURSE
		"Change Attack Range",					//EMSPECA_CHANGE_ATTACK_RANGE
		"Change Coverage Area",					//EMSPECA_CHANGE_APPLY_RANGE
		"Prohibit Using Potion",				//EMSPECA_PROHIBIT_POTION
		"Prohibit Using Skill",					//EMSPECA_PROHIBIT_SKILL
		"Damage Absorption",					//EMSPECA_IGNORE_DAMAGE
		"Item Drop Rate",						//EMSPECA_ITEMDROP_RATE
		"Gold Drop Rate",						//EMSPECA_MONEYDROP_RATE
		"EXP Rate",								//EMSPECA_GETEXP_RATE
		"Special NPC Dialogue",					//EMSPECA_TALK_TO_NPC
		"Special Item Drop",					//EMSPECA_SPECIAL_ITEM_DROP
		"Teleportation",						//EMSPECA_TELEPORT
		"Position Shift",						//EMSPECA_POSITION_SHIFT
		"Amplify",								//EMSPECA_AMPLIFY
		"Illusion",								//EMSPECA_ILLUSION
		"Vehicle Dismount",						//EMSPECA_VEHICLE_OFF
		"Duration Change",						//EMSPECA_DURATION_CHANGE
		"Stigma",								//EMSPECA_STIGMA
		"Transform",							//EMSPECA_TRANSFORM
		"Effect Release",						//EMSPECA_EFFECT_RELEASE
		"Damage Loop",							//EMSPECA_DAMAGE_LOOP
		"Pet Off",								//EMSPECA_PET_OFF
		"Immunity",								//EMSPECA_IMMUNE
		"Hostile",								//EMSPECA_HOSTILE_ON
		"Hostile Off",							//EMSPECA_HOSTILE_OFF
		"Target Fix",							//EMSPECA_TARGET_FIX
		"Dominate",								//EMSPECA_MANIPULATE
		"Random EXP Rate",						//EMSPECA_RANDOM_EXP_RATE
		"Skill Link",							//EMSPECA_SKILLLINK
	};

	std::string SPEC_ADDON_VAR1[EMSPECA_NSIZE] =
	{
		"None",						//EMSPECA_NULL
		"Distance",					//EMSPECA_PUSHPULL
		"None",						//EMSPECA_REFDAMAGE
		"Recovery Rate",			//EMSPECA_REBIRTH
		"Absorb Rate",				//EMSPECA_HP_GATHER
		"Absorb Rate",				//EMSPECA_MP_GATHER
		"Absorb Rate",				//EMSPECA_SP_GATHER
		"None",						//EMSPECA_HP_DIV
		"None",						//EMSPECA_MP_DIV
		"None",						//EMSPECA_SP_DIV
		"None",						//EMSPECA_NONBLOW
		"None",						//EMSPECA_RECBLOW
		"Pierce",					//EMSPECA_PIERCE
		"Weapon Reach",				//EMSPECA_TARRANGE
		"Movement Speed",			//EMSPECA_MOVEVELO
		"Chance",					//EMSPECA_ONWARD
		"Hiding Lv",				//EMSPECA_INVISIBLE
		"Detect Lv",				//EMSPECA_RECVISIBLE
		"Attack Speed",				//EMSPECA_ATTACKVELO
		"Skill Delay",				//EMSPECA_SKILLDELAY
		"Distance",					//EMSPECA_CRUSHING_BLOW
		"Absorb Rate",				//EMSPECA_PSY_DAMAGE_REDUCE
		"Absorb Rate",				//EMSPECA_MAGIC_DAMAGE_REDUCE
		"Return Rate",				//EMSPECA_PSY_DAMAGE_REFLECTION
		"Return Rate",				//EMSPECA_MAGIC_DAMAGE_REFLECTION
		"None",						//EMSPECA_BUFF_REMOVE
		"None",						//EMSPECA_STUN
		"Rate",						//EMSPECA_DEFENSE_SKILL_ACTIVE
		"Rate", 					//EMSPECA_PULL
		"Rate",						//EMSPECA_PUSH
		"Duration",					//EMSPECA_CONTINUOUS_DAMAGE
		"Reflect Rate",				//EMSPECA_CURSE
		"Range", 					//EMSPECA_CHANGE_ATTACK_RANGE
		"Range", 					//EMSPECA_CHANGE_APPLY_RANGE
		"None", 					//EMSPECA_PROHIBIT_POTION
		"None", 					//EMSPECA_PROHIBIT_SKILL
		"Number of", 				//EMSPECA_IGNORE_DAMAGE
		"Increase Rate", 			//EMSPECA_ITEMDROP_RATE
		"Increase Rate", 			//EMSPECA_MONEYDROP_RATE
		"Increase Rate", 			//EMSPECA_GETEXP_RATE
		"None", 					//EMSPECA_TALK_TO_NPC
		"None", 					//EMSPECA_SPECIAL_ITEM_DROP
		"Rate", 					//EMSPECA_TELEPORT
		"Rate", 					//EMSPECA_POSITION_SHIFT
		"Critical R. Inc.", 		//EMSPECA_AMPLIFY
		"Distance", 				//EMSPECA_ILLUSION
		"Rate", 					//EMSPECA_VEHICLE_OFF
		"Change1", 					//EMSPECA_DURATION_CHANGE
		"Effect Distance", 			//EMSPECA_STIGMA
		"Transform ID", 			//EMSPECA_TRANSFORM
		"Rate", 					//EMSPECA_EFFECT_RELEASE
		"Rate", 					//EMSPECA_DAMAGE_LOOP
		"Rate", 					//EMSPECA_PET_OFF
		"Application type", 		//EMSPECA_IMMUNE
		"Rate", 					//EMSPECA_HOSTILE_ON
		"None", 					//EMSPECA_HOSTILE_OFF
		"Rate", 					//EMSPECA_TARGET_FIX
		"Rate", 					//EMSPECA_MANIPULATE
		"Index", 					//EMSPECA_RANDOM_EXP_RATE
		"None", 					//EMSPECA_SKILLLINK
	};

	float SPEC_ADDON_VAR1_SCALE[EMSPECA_NSIZE] =
	{
		1.0f,		//EMSPECA_NULL
		0.10f,		//EMSPECA_PUSHPULL	
		1.0f,		//EMSPECA_REFDAMAGE	
		100.0f,		//EMSPECA_REBIRTH	
		100.0f,		//EMSPECA_HP_GATHER
		100.0f,		//EMSPECA_MP_GATHER
		100.0f,		//EMSPECA_SP_GATHER
		1.0f,		//EMSPECA_HP_DIV
		1.0f,		//EMSPECA_MP_DIV
		1.0f,		//EMSPECA_SP_DIV
		1.0f,		//EMSPECA_NONBLOW
		1.0f,		//EMSPECA_RECBLOW
		1.0f,		//EMSPECA_PIERCE
		1.0f,		//EMSPECA_TARRANGE
		100.0f,		//EMSPECA_MOVEVELO
		100.0f,		//EMSPECA_ONWARD
		1.0f,		//EMSPECA_INVISIBLE
		1.0f,		//EMSPECA_RECVISIBLE
		100.0f,		//EMSPECA_ATTACKVELO
		100.0f,		//EMSPECA_SKILLDELAY
		0.10f,		//EMSPECA_CRUSHING_BLOW
		100.0f,		//EMSPECA_PSY_DAMAGE_REDUCE
		100.0f,		//EMSPECA_MAGIC_DAMAGE_REDUCE
		100.0f,		//EMSPECA_PSY_DAMAGE_REFLECTION
		100.0f,		//EMSPECA_MAGIC_DAMAGE_REFLECTION
		1.0f,		//EMSPECA_BUFF_REMOVE
		1.0f,		//EMSPECA_STUN
		100.0f, 	//EMSPECA_DEFENSE_SKILL_ACTIVE
		100.0f, 	//EMSPECA_PULL
		100.0f,		//EMSPECA_PUSH
		1.0f,		//EMSPECA_CONTINUOUS_DAMAGE
		100.0f,		//EMSPECA_CURSE
		1.0f,		//EMSPECA_CHANGE_ATTACK_RANGE
		1.0f,		//EMSPECA_CHANGE_APPLY_RANGE
		1.0f,		//EMSPECA_PROHIBIT_POTION
		1.0f,		//EMSPECA_PROHIBIT_SKILL
		1.0f,		//EMSPECA_IGNORE_DAMAGE
		100.0f, 	//EMSPECA_ITEMDROP_RATE
		100.0f, 	//EMSPECA_MONEYDROP_RATE
		100.0f,		//EMSPECA_GETEXP_RATE
		1.0f,		//EMSPECA_TALK_TO_NPC
		1.0f,		//EMSPECA_SPECIAL_ITEM_DROP
		1.0f,		//EMSPECA_TELEPORT
		100.0f,		//EMSPECA_POSITION_SHIFT
		100.0f,		//EMSPECA_AMPLIFY
		1.0f,		//EMSPECA_ILLUSION
		100.0f,		//EMSPECA_VEHICLE_OFF
		1.0f,		//EMSPECA_DURATION_CHANGE
		1.0f,		//EMSPECA_STIGMA
		1.0f,		//EMSPECA_TRANSFORM
		100.0f,		//EMSPECA_EFFECT_RELEASE
		1.0f,		//EMSPECA_DAMAGE_LOOP
		100.0f,		//EMSPECA_PET_OFF
		1.0f,		//EMSPECA_IMMUNE
		100.0f,		//EMSPECA_HOSTILE_ON
		1.0f,		//EMSPECA_HOSTILE_OFF
		1.0f,		//EMSPECA_TARGET_FIX
		100.0f,		//EMSPECA_MANIPULATE
		1.0f,		//EMSPECA_RANDOM_EXP_RATE
		1.0f,		//EMSPECA_SKILLLINK
	};

	std::string SPEC_ADDON_VAR2[EMSPECA_NSIZE] =
	{
		"None",					//EMSPECA_NULL
		"Chance",				//EMSPECA_PUSHPULL
		"None",					//EMSPECA_REFDAMAGE
		"None",					//EMSPECA_REBIRTH
		"None",					//EMSPECA_HP_GATHER
		"None",					//EMSPECA_MP_GATHER
		"None",					//EMSPECA_SP_GATHER
		"None",					//EMSPECA_HP_DIV
		"None",					//EMSPECA_MP_DIV
		"None",					//EMSPECA_SP_DIV
		"None",					//EMSPECA_NONBLOW
		"None",					//EMSPECA_RECBLOW
		"None",					//EMSPECA_PIERCE
		"None",					//EMSPECA_TARRANGE
		"None",					//EMSPECA_MOVEVELO
		"Speed",				//EMSPECA_ONWARD
		"Animation",			//EMSPECA_INVISIBLE
		"Range",				//EMSPECA_RECVISIBLE
		"None",					//EMSPECA_ATTACKVELO
		"Rate",					//EMSPECA_SKILLDELAY
		"Chance",				//EMSPECA_CRUSHING_BLOW
		"None",					//EMSPECA_PSY_DAMAGE_REDUCE
		"None",					//EMSPECA_MAGIC_DAMAGE_REDUCE
		"Chance",				//EMSPECA_PSY_DAMAGE_REFLECTION
		"Chance",				//EMSPECA_MAGIC_DAMAGE_REFLECTION
		"None",					//EMSPECA_BUFF_REMOVE
		"None",					//EMSPECA_STUN
		"None",					//EMSPECA_DEFENSE_SKILL_ACTIVE
		"Moving Speed", 		//EMSPECA_PULL
		"Range", 				//EMSPECA_PUSH
		"Damage", 				//EMSPECA_CONTINUOUS_DAMAGE
		"None",					//EMSPECA_CURSE
		"None",					//EMSPECA_CHANGE_ATTACK_RANGE
		"None",					//EMSPECA_CHANGE_APPLY_RANGE
		"None",					//EMSPECA_PROHIBIT_POTION
		"None",					//EMSPECA_PROHIBIT_SKILL
		"Absorption", 			//EMSPECA_IGNORE_DAMAGE
		"None",					//EMSPECA_ITEMDROP_RATE
		"None",					//EMSPECA_MONEYDROP_RATE
		"None",					//EMSPECA_GETEXP_RATE
		"None",					//EMSPECA_TALK_TO_NPC
		"None",					//EMSPECA_SPECIAL_ITEM_DROP
		"None",					//EMSPECA_TELEPORT
		"Speed",				//EMSPECA_POSITION_SHIFT
		"Damage Inc.",			//EMSPECA_AMPLIFY
		"Number of times",		//EMSPECA_ILLUSION
		"Duration",				//EMSPECA_VEHICLE_OFF
		"Change2",				//EMSPECA_DURATION_CHANGE
		"Value",				//EMSPECA_STIGMA
		"Emoticon ID",			//EMSPECA_TRANSFORM
		"Release Condition",	//EMSPECA_EFFECT_RELEASE
		"None",					//EMSPECA_DAMAGE_LOOP
		"None",					//EMSPECA_PET_OFF
		"Behavior type",		//EMSPECA_IMMUNE
		"Duration",				//EMSPECA_HOSTILE_ON
		"None",					//EMSPECA_HOSTILE_OFF
		"Distance",				//EMSPECA_TARGET_FIX
		"Range",				//EMSPECA_MANIPULATE
		"Overlaps",				//EMSPECA_RANDOM_EXP_RATE
		"None",					//EMSPECA_SKILLLINK
	};

	float SPEC_ADDON_VAR2_SCALE[EMSPECA_NSIZE] =
	{
		1.0f,		//EMSPECA_NULL
		100.0f,		//EMSPECA_PUSHPULL
		1.0f,		//EMSPECA_REFDAMAGE
		1.0f,		//EMSPECA_REBIRTH
		1.0f,		//EMSPECA_HP_GATHER
		1.0f,		//EMSPECA_MP_GATHER
		1.0f,		//EMSPECA_SP_GATHER
		1.0f,		//EMSPECA_HP_DIV
		1.0f,		//EMSPECA_MP_DIV
		1.0f,		//EMSPECA_SP_DIV
		1.0f,		//EMSPECA_NONBLOW
		1.0f,		//EMSPECA_RECBLOW
		1.0f,		//EMSPECA_PIERCE
		1.0f,		//EMSPECA_TARRANGE
		100.0f,		//EMSPECA_MOVEVELO
		1.0f,		//EMSPECA_ONWARD
		1.0f,		//EMSPECA_INVISIBLE
		1.0f,		//EMSPECA_RECVISIBLE
		1.0f,		//EMSPECA_ATTACKVELO
		1.0f,		//EMSPECA_SKILLDELAY
		100.0f,		//EMSPECA_CRUSHING_BLOW
		1.0f,		//EMSPECA_PSY_DAMAGE_REDUCE
		1.0f,		//EMSPECA_MAGIC_DAMAGE_REDUCE
		100.0f,		//EMSPECA_PSY_DAMAGE_REFLECTION
		100.0f,		//EMSPECA_MAGIC_DAMAGE_REFLECTION
		1.0f,		//EMSPECA_BUFF_REMOVE
		1.0f,		//EMSPECA_STUN
		1.0f,		//EMSPECA_DEFENSE_SKILL_ACTIVE
		1.0f,		//EMSPECA_PULL
		0.10f,		//EMSPECA_PUSH
		1.0f,		//EMSPECA_CONTINUOUS_DAMAGE
		1.0f,		//EMSPECA_CURSE
		1.0f,		//EMSPECA_CHANGE_ATTACK_RANGE
		1.0f,		//EMSPECA_CHANGE_APPLY_RANGE
		1.0f,		//EMSPECA_PROHIBIT_POTION
		1.0f,		//EMSPECA_PROHIBIT_SKILL
		1.0f,		//EMSPECA_IGNORE_DAMAGE
		1.0f,		//EMSPECA_ITEMDROP_RATE
		1.0f,		//EMSPECA_MONEYDROP_RATE
		1.0f,		//EMSPECA_GETEXP_RATE
		1.0f,		//EMSPECA_TALK_TO_NPC
		1.0f,		//EMSPECA_SPECIAL_ITEM_DROP
		1.0f,		//EMSPECA_TELEPORT
		2.0f,		//EMSPECA_POSITION_SHIFT
		100.0f,		//EMSPECA_AMPLIFY
		1.0f,		//EMSPECA_ILLUSION
		1.0f,		//EMSPECA_VEHICLE_OFF
		1.0f,		//EMSPECA_DURATION_CHANGE
		100.0f,		//EMSPECA_STIGMA
		1.0f,		//EMSPECA_TRANSFORM
		1.0f,		//EMSPECA_EFFECT_RELEASE
		1.0f,		//EMSPECA_DAMAGE_LOOP
		1.0f,		//EMSPECA_PET_OFF
		1.0f,		//EMSPECA_IMMUNE
		1.0f,		//EMSPECA_HOSTILE_ON
		1.0f,		//EMSPECA_HOSTILE_OFF
		1.0f,		//EMSPECA_TARGET_FIX
		1.0f,		//EMSPECA_MANIPULATE
		1.0f,		//EMSPECA_RANDOM_EXP_RATE
		1.0f,		//EMSPECA_SKILLLINK
	};

	std::string SKILLCLASS[EMSKILLCLASS_NSIZE] =
	{
		"Brawler-class1",
		"Brawler-class2",
		"Brawler-class3",
		"Brawler-class4",
		"Swordsman-class1",
		"Swordsman-class2",
		"Swordsman-class3",
		"Swordsman-class4",
		"Archer-class1",
		"Archer-class2",
		"Archer-class3",
		"Archer-class4",
		"Shaman-class1",
		"Shaman-class2",
		"Shaman-class3",
		"Shaman-class4",
		"NPC_01",
		"NPC_02",
		"NPC_03",
		"NPC_04",
		"NPC_05",
		"NPC_06",
		"NPC_07",
		"NPC_08",
		"NPC_09",
		"NPC_10",
		"NPC_11(Pet SKill)",
		"NPC_12",
		"NPC_13",
		"NPC_14",
		"Extreme-class1",
		"Extreme-class2",
		"Extreme-class3",
		"Extreme-class4",
		"NPC_19",
		"NPC_20",
		"Gunner-class1",
		"Gunner-class2",
		"Gunner-class3",
		"Gunner-class4",
		"Food",
		"Summon",
		"Magician-Animal",
		"Assassin-class1",
		"Assassin-class2",
		"Assassin-class3",
		"Assassin-class4",
		"ETC-class1",
		"ETC-class2",
		"ETC-class3",
		"ETC-class4",
		"CaptureTheField",
		"Event",
		"ETC",
		"Test",
		"Magician-multi",
		"Magician-class1",
		"Magician-class2",
		"Magician-class3",
		"Magician-class4",
		"Shaper-Additional Skills",
		"Shaper-DEX",
		"Shaper-POW",
		"Shaper-INT",
		"Shaper-Basic",
		"Shaper-Disguised DEX",
		"Shaper-Disguised POW",
		"Shaper-Disguised INT",
		"Shaper-Disguised Basic",
	};

	std::string LANDEFFECT_TYPE[EMLANDEFFECT_SIZE] =
	{
		"Attack Speed",
		"Movement Speed",
		"Maximum HP",
		"Maximum MP",
		"HP+MP+SP Recover",
		"Attack Rate",
		"Defense Rate",
		"Resistance",
		"Remove All Buffs",
		"SafeZone",
		"Non vend area"
	};
};