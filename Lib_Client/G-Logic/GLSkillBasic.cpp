#include "stdafx.h"
#include "./GLSkillBasic.h"
#include "./GLItemSuit.h"
#include "./GLStringTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace SKILL
{
	GLSKILL_ATT assign_att ( gloldskill_102::GLSKILL_ATT_102 emOld )
	{
		GLSKILL_ATT emAttack;
		switch ( emOld )
		{
		case gloldskill_102::SKILLATT_NOTHING:	emAttack = SKILLATT_NOTHING;	break;
		case gloldskill_102::SKILLATT_SWORD:	emAttack = SKILLATT_SWORD;	break;
		case gloldskill_102::SKILLATT_REV00:	emAttack = SKILLATT_SWORD;	break;
		case gloldskill_102::SKILLATT_DAGGER:	emAttack = SKILLATT_DAGGER;	break;
		case gloldskill_102::SKILLATT_SPEAR:	emAttack = SKILLATT_SPEAR;	break;
		case gloldskill_102::SKILLATT_BOW:		emAttack = SKILLATT_BOW;	break;
		case gloldskill_102::SKILLATT_THROW:	emAttack = SKILLATT_THROW;	break;
		case gloldskill_102::SKILLATT_NOCARE:	emAttack = SKILLATT_NOCARE;	break;
		default:	emAttack = SKILLATT_NOTHING;	break;
		};

		return emAttack;
	}

	GLSKILL_ATT assign_att ( gloldskill_103::GLSKILL_ATT_103 emOld )
	{
		GLSKILL_ATT emAttack;
		switch ( emOld )
		{
		case gloldskill_103::SKILLATT_NOTHING:	emAttack = SKILLATT_NOTHING;	break;
		case gloldskill_103::SKILLATT_SWORD:	emAttack = SKILLATT_SWORD;	break;
		case gloldskill_103::SKILLATT_BLADE:	emAttack = SKILLATT_BLADE;	break;
		case gloldskill_103::SKILLATT_DAGGER:	emAttack = SKILLATT_DAGGER;	break;
		case gloldskill_103::SKILLATT_SPEAR:	emAttack = SKILLATT_SPEAR;	break;
		case gloldskill_103::SKILLATT_STICK:	emAttack = SKILLATT_STICK;	break;
		case gloldskill_103::SKILLATT_BOW:		emAttack = SKILLATT_BOW;	break;
		case gloldskill_103::SKILLATT_THROW:	emAttack = SKILLATT_THROW;	break;
		case gloldskill_103::SKILLATT_NOCARE:	emAttack = SKILLATT_NOCARE;	break;
		default:	emAttack = SKILLATT_NOTHING;		break;
		};

		return emAttack;
	}

	GLSKILL_ATT assign_att ( gloldskill_108::GLSKILL_ATT_108 emOld )
	{
		GLSKILL_ATT emAttack;
		switch ( emOld )
		{
		case gloldskill_108::SKILLATT_NOTHING:	emAttack = SKILLATT_NOTHING;	break;
		case gloldskill_108::SKILLATT_SWORD:	emAttack = SKILLATT_SWORD;	break;
		case gloldskill_108::SKILLATT_BLADE:	emAttack = SKILLATT_BLADE;	break;
		case gloldskill_108::SKILLATT_SWORDBLADE:	emAttack = SKILLATT_SWORDBLADE;	break;
		case gloldskill_108::SKILLATT_DAGGER:	emAttack = SKILLATT_DAGGER;	break;
		case gloldskill_108::SKILLATT_SPEAR:	emAttack = SKILLATT_SPEAR;	break;
		case gloldskill_108::SKILLATT_STICK:	emAttack = SKILLATT_STICK;	break;
		case gloldskill_108::SKILLATT_GAUNT:	emAttack = SKILLATT_GAUNT;	break;
		case gloldskill_108::SKILLATT_BOW:		emAttack = SKILLATT_BOW;	break;
		case gloldskill_108::SKILLATT_THROW:	emAttack = SKILLATT_THROW;	break;
		case gloldskill_108::SKILLATT_NOCARE:	emAttack = SKILLATT_NOCARE;	break;
		default:	emAttack = SKILLATT_NOTHING;		break;
		};

		return emAttack;
	}

	GLSKILL_ATT assign_att ( gloldskill_110::GLSKILL_ATT_110 emOld )
	{
		GLSKILL_ATT emAttack;
		switch ( emOld )
		{
		case gloldskill_110::SKILLATT_NOTHING:	emAttack = SKILLATT_NOTHING;	break;
		case gloldskill_110::SKILLATT_SWORD:	emAttack = SKILLATT_SWORD;	break;
		case gloldskill_110::SKILLATT_BLADE:	emAttack = SKILLATT_BLADE;	break;
		case gloldskill_110::SKILLATT_SWORDBLADE:	emAttack = SKILLATT_SWORDBLADE;	break;
		case gloldskill_110::SKILLATT_DAGGER:	emAttack = SKILLATT_DAGGER;	break;
		case gloldskill_110::SKILLATT_SPEAR:	emAttack = SKILLATT_SPEAR;	break;
		case gloldskill_110::SKILLATT_STICK:	emAttack = SKILLATT_STICK;	break;
		case gloldskill_110::SKILLATT_GAUNT:	emAttack = SKILLATT_GAUNT;	break;
		case gloldskill_110::SKILLATT_BOW:		emAttack = SKILLATT_BOW;	break;
		case gloldskill_110::SKILLATT_THROW:	emAttack = SKILLATT_THROW;	break;

		case gloldskill_110::SKILLATT_GUN:				emAttack = SKILLATT_GUN;		break;
		case gloldskill_110::SKILLATT_RAILGUN:			emAttack = SKILLATT_RAILGUN;	break;
		case gloldskill_110::SKILLATT_PORTALGUN:		emAttack = SKILLATT_PORTALGUN;	break;
		case gloldskill_110::SKILLATT_SHOTGUN:			emAttack = SKILLATT_SHOTGUN;	break;
		case gloldskill_110::SKILLATT_RAILPORTALGUN:	emAttack = SKILLATT_RAILPORTALGUN;	break;

		case gloldskill_110::SKILLATT_NOCARE:	emAttack = SKILLATT_NOCARE;	break;
		default:	emAttack = SKILLATT_NOTHING;		break;
		};

		return emAttack;
	}

	GLSKILL_ATT assign_att ( gloldskill_111::GLSKILL_ATT_111 emOld )
	{
		GLSKILL_ATT emAttack;
		switch ( emOld )
		{
		case gloldskill_111::SKILLATT_NOTHING:	emAttack = SKILLATT_NOTHING;	break;
		case gloldskill_111::SKILLATT_SWORD:	emAttack = SKILLATT_SWORD;	break;
		case gloldskill_111::SKILLATT_BLADE:	emAttack = SKILLATT_BLADE;	break;
		case gloldskill_111::SKILLATT_SWORDBLADE:	emAttack = SKILLATT_SWORDBLADE;	break;
		case gloldskill_111::SKILLATT_DAGGER:	emAttack = SKILLATT_DAGGER;	break;
		case gloldskill_111::SKILLATT_SPEAR:	emAttack = SKILLATT_SPEAR;	break;
		case gloldskill_111::SKILLATT_STICK:	emAttack = SKILLATT_STICK;	break;
		case gloldskill_111::SKILLATT_GAUNT:	emAttack = SKILLATT_GAUNT;	break;
		case gloldskill_111::SKILLATT_BOW:		emAttack = SKILLATT_BOW;	break;
		case gloldskill_111::SKILLATT_THROW:	emAttack = SKILLATT_THROW;	break;

		case gloldskill_111::SKILLATT_GUN:				emAttack = SKILLATT_GUN;		break;
		case gloldskill_111::SKILLATT_RAILGUN:			emAttack = SKILLATT_RAILGUN;	break;
		case gloldskill_111::SKILLATT_PORTALGUN:		emAttack = SKILLATT_PORTALGUN;	break;
		case gloldskill_111::SKILLATT_SHOTGUN:			emAttack = SKILLATT_SHOTGUN;	break;
		case gloldskill_111::SKILLATT_RAILPORTALGUN:	emAttack = SKILLATT_RAILPORTALGUN;	break;

		case gloldskill_111::SKILLATT_SCYTHE:		emAttack = SKILLATT_SCYTHE;		break;
		case gloldskill_111::SKILLATT_DUALSPEAR:	emAttack = SKILLATT_DUALSPEAR;	break;
		case gloldskill_111::SKILLATT_SHURIKEN:		emAttack = SKILLATT_SHURIKEN;	break;

		case gloldskill_111::SKILLATT_NOCARE:		emAttack = SKILLATT_NOCARE;	break;
		default:	emAttack = SKILLATT_NOTHING;		break;
		};

		return emAttack;
	}

	GLSKILL_ATT assign_att ( gloldskill_113::GLSKILL_ATT_113 emOld )
	{
		GLSKILL_ATT emAttack;
		switch ( emOld )
		{
		case gloldskill_113::SKILLATT_NOTHING:	emAttack = SKILLATT_NOTHING;	break;
		case gloldskill_113::SKILLATT_SWORD:	emAttack = SKILLATT_SWORD;	break;
		case gloldskill_113::SKILLATT_BLADE:	emAttack = SKILLATT_BLADE;	break;
		case gloldskill_113::SKILLATT_SWORDBLADE:	emAttack = SKILLATT_SWORDBLADE;	break;
		case gloldskill_113::SKILLATT_DAGGER:	emAttack = SKILLATT_DAGGER;	break;
		case gloldskill_113::SKILLATT_SPEAR:	emAttack = SKILLATT_SPEAR;	break;
		case gloldskill_113::SKILLATT_STICK:	emAttack = SKILLATT_STICK;	break;
		case gloldskill_113::SKILLATT_GAUNT:	emAttack = SKILLATT_GAUNT;	break;
		case gloldskill_113::SKILLATT_BOW:		emAttack = SKILLATT_BOW;	break;
		case gloldskill_113::SKILLATT_THROW:	emAttack = SKILLATT_THROW;	break;

		case gloldskill_113::SKILLATT_GUN:				emAttack = SKILLATT_GUN;		break;
		case gloldskill_113::SKILLATT_RAILGUN:			emAttack = SKILLATT_RAILGUN;	break;
		case gloldskill_113::SKILLATT_PORTALGUN:		emAttack = SKILLATT_PORTALGUN;	break;
		case gloldskill_113::SKILLATT_SHOTGUN:			emAttack = SKILLATT_SHOTGUN;	break;
		case gloldskill_113::SKILLATT_RAILPORTALGUN:	emAttack = SKILLATT_RAILPORTALGUN;	break;

		case gloldskill_113::SKILLATT_SCYTHE:		emAttack = SKILLATT_SCYTHE;		break;
		case gloldskill_113::SKILLATT_DUALSPEAR:	emAttack = SKILLATT_DUALSPEAR;	break;
		case gloldskill_113::SKILLATT_SHURIKEN:		emAttack = SKILLATT_SHURIKEN;	break;

		case gloldskill_113::SKILLATT_EXTREME_FIST:	emAttack = SKILLATT_EXTREME_FIST;	break;

		case gloldskill_113::SKILLATT_NOCARE:		emAttack = SKILLATT_NOCARE;	break;
		default:	emAttack = SKILLATT_NOTHING;		break;
		};

		return emAttack;
	}

	GLSKILL_ATT assign_att ( gloldskill_114::GLSKILL_ATT_114 emOld )
	{
		GLSKILL_ATT emAttack;
		switch ( emOld )
		{
		case gloldskill_114::SKILLATT_NOTHING:	emAttack = SKILLATT_NOTHING;	break;
		case gloldskill_114::SKILLATT_SWORD:	emAttack = SKILLATT_SWORD;	break;
		case gloldskill_114::SKILLATT_BLADE:	emAttack = SKILLATT_BLADE;	break;
		case gloldskill_114::SKILLATT_SWORDBLADE:	emAttack = SKILLATT_SWORDBLADE;	break;
		case gloldskill_114::SKILLATT_DAGGER:	emAttack = SKILLATT_DAGGER;	break;
		case gloldskill_114::SKILLATT_SPEAR:	emAttack = SKILLATT_SPEAR;	break;
		case gloldskill_114::SKILLATT_STICK:	emAttack = SKILLATT_STICK;	break;
		case gloldskill_114::SKILLATT_GAUNT:	emAttack = SKILLATT_GAUNT;	break;
		case gloldskill_114::SKILLATT_BOW:		emAttack = SKILLATT_BOW;	break;
		case gloldskill_114::SKILLATT_THROW:	emAttack = SKILLATT_THROW;	break;

		case gloldskill_114::SKILLATT_GUN:				emAttack = SKILLATT_GUN;		break;
		case gloldskill_114::SKILLATT_RAILGUN:			emAttack = SKILLATT_RAILGUN;	break;
		case gloldskill_114::SKILLATT_PORTALGUN:		emAttack = SKILLATT_PORTALGUN;	break;
		case gloldskill_114::SKILLATT_SHOTGUN:			emAttack = SKILLATT_SHOTGUN;	break;
		case gloldskill_114::SKILLATT_RAILPORTALGUN:	emAttack = SKILLATT_RAILPORTALGUN;	break;

		case gloldskill_114::SKILLATT_SCYTHE:		emAttack = SKILLATT_SCYTHE;		break;
		case gloldskill_114::SKILLATT_DUALSPEAR:	emAttack = SKILLATT_DUALSPEAR;	break;
		case gloldskill_114::SKILLATT_SHURIKEN:		emAttack = SKILLATT_SHURIKEN;	break;

		case gloldskill_114::SKILLATT_EXTREME_FIST:	emAttack = SKILLATT_EXTREME_FIST;	break;

		case gloldskill_114::SKILLATT_WAND:			emAttack = SKILLATT_WAND;	break;
		case gloldskill_114::SKILLATT_CUBE:			emAttack = SKILLATT_CUBE;	break;
		case gloldskill_114::SKILLATT_ROPE:			emAttack = SKILLATT_ROPE;	break;

		case gloldskill_114::SKILLATT_NOCARE:		emAttack = SKILLATT_NOCARE;	break;
		default:	emAttack = SKILLATT_NOTHING;		break;
		};

		return emAttack;
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_100 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		
		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= assign_att ( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att ( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_101 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		
		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= assign_att ( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att ( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_102 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		
		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= assign_att ( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att ( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_103 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		
		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_105 &sOldData )
	{
		sNATIVEID				= sOldData.sNATIVEID;

		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		dwGRADE					= sOldData.dwGRADE;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;

		emROLE					= sOldData.emROLE;
		emAPPLY					= sOldData.emAPPLY;

		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;

		wTARRANGE				= sOldData.wTARRANGE;

		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_106 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_107 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_108 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );

		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		unknown_var_1			= sOldData.unknown_var_1;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		emACTION				= sOldData.emACTION;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_109 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		unknown_var_1			= sOldData.unknown_var_1;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		emACTION				= sOldData.emACTION;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_110 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		unknown_var_1			= sOldData.unknown_var_1;
		unknown_var_2			= sOldData.unknown_var_2;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		emACTION				= sOldData.emACTION;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_111 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		unknown_var_1			= sOldData.unknown_var_1;
		unknown_var_2			= sOldData.unknown_var_2;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		emACTION				= sOldData.emACTION;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_112 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		unknown_var_1			= sOldData.unknown_var_1;
		unknown_var_2			= sOldData.unknown_var_2;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		emACTION				= sOldData.emACTION;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
		unknown_var_3			= sOldData.unknown_var_3;
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_113 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		unknown_var_1			= sOldData.unknown_var_1;
		unknown_var_2			= sOldData.unknown_var_2;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		emACTION				= sOldData.emACTION;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
		unknown_var_3			= sOldData.unknown_var_3;
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_114 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		unknown_var_1			= sOldData.unknown_var_1;
		unknown_var_2			= sOldData.unknown_var_2;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		emACTION				= sOldData.emACTION;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= assign_att( sOldData.emUSE_LITEM );
		emUSE_RITEM				= assign_att( sOldData.emUSE_RITEM );
		unknown_var_3			= sOldData.unknown_var_3;
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_115 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		unknown_var_1			= sOldData.unknown_var_1;
		unknown_var_2			= sOldData.unknown_var_2;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		emACTION				= sOldData.emACTION;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= sOldData.emUSE_LITEM;
		emUSE_RITEM				= sOldData.emUSE_RITEM;
		unknown_var_3			= sOldData.unknown_var_3;
	}

	void SSKILLBASIC::Assign ( SSKILLBASIC_200 &sOldData )
	{
		StringCchCopy ( szNAME, MAX_SZNAME, sOldData.szNAME );
		sNATIVEID				= sOldData.sNATIVEID;
		bLearnView				= sOldData.bLearnView;
		unknown_var_1			= sOldData.unknown_var_1;
		unknown_var_2			= sOldData.unknown_var_2;
		dwMAXLEVEL				= sOldData.dwMAXLEVEL;
		dwGRADE					= sOldData.dwGRADE;
		emAPPLY					= sOldData.emAPPLY;
		emROLE					= sOldData.emROLE;
		emACTION				= sOldData.emACTION;
		wTARRANGE				= sOldData.wTARRANGE;
		emIMPACT_SIDE			= sOldData.emIMPACT_SIDE;
		emIMPACT_TAR			= sOldData.emIMPACT_TAR;
		emIMPACT_REALM			= sOldData.emIMPACT_REALM;
		emUSE_LITEM				= sOldData.emUSE_LITEM;
		emUSE_RITEM				= sOldData.emUSE_RITEM;
		unknown_var_3			= sOldData.unknown_var_3;
	}

	VOID SSKILLBASIC::SaveCsvHead ( std::fstream &SFile )
	{
		SFile << "sNATIVEID wMainID" << ",";
		SFile << "sNATIVEID wSubID" << ",";

		SFile << "szNAME" << ",";

//		±âÈ¹ÆÀ ¿äÃ»À¸·Î Á¦°ÅÇÔ
		SFile << "szNAME_FULL" << ",";

		SFile << "dwGRADE" << ",";
		SFile << "dwMAXLEVEL" << ",";

		SFile << "emROLE" << ",";
		SFile << "emAPPLY" << ",";
		SFile << "emACTION" << ",";


		SFile << "emIMPACT_TAR" << ",";
		SFile << "emIMPACT_REALM" << ",";
		SFile << "emIMPACT_SIDE" << ",";

		SFile << "wTARRANGE" << ",";

		SFile << "emUSE_LITEM" << ",";
		SFile << "emUSE_RITEM" << ",";

		SFile << "bLearnView" << ",";

		SFile << "unknown_var_1" << ",";
		SFile << "unknown_var_2" << ",";
		SFile << "unknown_var_3" << ",";

		SFile << "dwSkillFlag" << ",";
	}

	VOID SSKILLBASIC::SaveCsv ( std::fstream &SFile )
	{
		SFile << sNATIVEID.wMainID << ",";
		SFile << sNATIVEID.wSubID << ",";

		std::string str = szNAME;
		STRUTIL::OutputStrCsv( SFile, str );

//		±âÈ¹ÆÀ ¿äÃ»À¸·Î Á¦°ÅÇÔ

		if( szNAME )
		{
			const char* szpLongName = GLStringTable::GetInstance().GetString( szNAME, GLStringTable::SKILL );
			if( szpLongName )
				str = szpLongName;
		}
		STRUTIL::OutputStrCsv( SFile, str );


		SFile << dwGRADE << ",";
		SFile << dwMAXLEVEL << ",";

		SFile << emROLE << ",";
		SFile << emAPPLY << ",";
		SFile << emACTION << ",";

		SFile << emIMPACT_TAR << ",";
		SFile << emIMPACT_REALM << ",";
		SFile << emIMPACT_SIDE << ",";

		SFile << wTARRANGE << ",";

		SFile << emUSE_LITEM << ",";
		SFile << emUSE_RITEM << ",";

		SFile << bLearnView << ",";

		SFile << unknown_var_1 << ",";
		SFile << unknown_var_2 << ",";
		SFile << unknown_var_3 << ",";

		SFile << dwSkillFlag << ",";
	}

	VOID SSKILLBASIC::LoadCsv ( CStringArray &StrArray, int &iCsvCur )
	{
		sNATIVEID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sNATIVEID.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );

		std::string str;
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], str );
		memset( szNAME, 0, sizeof(char) * MAX_SZNAME );
		StringCchCopy( szNAME, MAX_SZNAME, str.c_str() );

//		±âÈ¹ÆÀ ¿äÃ»À¸·Î Á¦°ÅÇÔ
		++iCsvCur;	// Item Name

		dwGRADE = (DWORD)atol( StrArray[ iCsvCur++ ] );
		dwMAXLEVEL = (DWORD)atol( StrArray[ iCsvCur++ ] );

		emROLE = (EMROLE)atoi( StrArray[ iCsvCur++ ] );
		emAPPLY = (EMAPPLY)atoi( StrArray[ iCsvCur++ ] );
		emACTION = (EMACTION)atoi( StrArray[ iCsvCur++ ] );

		emIMPACT_TAR = (EMIMPACT_TAR)atoi( StrArray[ iCsvCur++ ] );
		emIMPACT_REALM = (EMIMPACT_REALM)atoi( StrArray[ iCsvCur++ ] );
		emIMPACT_SIDE = (EMIMPACT_SIDE)atoi( StrArray[ iCsvCur++ ] );

		wTARRANGE = (WORD)atoi( StrArray[ iCsvCur++ ] );

		emUSE_LITEM = (GLSKILL_ATT)atoi( StrArray[ iCsvCur++ ] );
		emUSE_RITEM = (GLSKILL_ATT)atoi( StrArray[ iCsvCur++ ] );

		bLearnView = (bool)atoi( StrArray[ iCsvCur++ ] );

		unknown_var_1 = (bool)atoi( StrArray[ iCsvCur++ ] );
		unknown_var_2 = (bool)atoi( StrArray[ iCsvCur++ ] );
		unknown_var_3 = (DWORD)atoi( StrArray[ iCsvCur++ ] );

		dwSkillFlag = (DWORD)atoi( StrArray[ iCsvCur++ ] );
	}

	VOID SSKILLBASIC::ReadStringStream ( std::stringstream &sStream )
	{
		sStream << sNATIVEID.wMainID << ",";
		sStream << sNATIVEID.wSubID << ",";

		sStream << szNAME << ",";

		sStream << dwGRADE << ",";
		sStream << dwMAXLEVEL << ",";

		sStream << emROLE << ",";
		sStream << emAPPLY << ",";
		sStream << emACTION << ",";

		sStream << emIMPACT_TAR << ",";
		sStream << emIMPACT_REALM << ",";
		sStream << emIMPACT_SIDE << ",";

		sStream << wTARRANGE << ",";

		sStream << emUSE_LITEM << ",";
		sStream << emUSE_RITEM << ",";

		sStream << bLearnView << ",";

		sStream << unknown_var_1 << ",";
		sStream << unknown_var_2 << ",";
		sStream << unknown_var_3 << ",";

		sStream << dwSkillFlag << ",";
	}
};

namespace COMMENT
{
	std::string SKILL_ROLE[SKILL::EMROLE_NSIZE] =
	{
		"Active",
		"Passive"
	};

	std::string SKILL_APPLY[SKILL::EMAPPLY_NSIZE] =
	{
		"Melee",
		"Missile",
		"Magic"
	};

	std::string SKILLATTACK[SKILLATT_NSIZE+1] =
	{
		"Nothing",
		"Sword",
		"Blade",
		"Sword/Blade",
		"Dagger",
		"Spear",
		"Stick",
		"Gaunt",
		"Bow",
		"Throw",
		"Gun",
		"Rail Gun",
		"Portal Gun",
		"Shotgun",
		"Rail-gun/Portal-gun",
		"Scythe",
		"DualSpear",
		"Shuriken",
		"ExtremeFist",
		"Wand",
		"Cube",
		"Rope",
		"Shield",
		"Hammer",
		"Umbrella",
		"No Weapon Requirements",
	};

	std::string SKILL_ACTION_TYPE[SKILL::EMACTION_NSIZE] = 
	{
		"Normal Type",
		"Buff Type",
		"Debuff Type",
		"Limit Type",
	};
};