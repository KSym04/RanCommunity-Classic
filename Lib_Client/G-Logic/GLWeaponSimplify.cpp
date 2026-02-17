#include "stdafx.h"
#include "./GLWeaponSimplify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace NSWEAPON_SIMPLIFY
{
	std::string strWeaponPieceRM[ITEMATT_NSIZE] = 
	{
		"",							//ITEMATT_NOTHING
		"m_simple_sword.cps",		//ITEMATT_SWORD
		"m_simple_blade.cps",		//ITEMATT_BLADE
		"",							//ITEMATT_DAGGER
		"m_simple_spear.cps",		//ITEMATT_SPEAR
		"",							//ITEMATT_STICK
		"m_simple_gaunt.cps",		//ITEMATT_GAUNT
		"m_simple_bow.cps",			//ITEMATT_BOW
		"",							//ITEMATT_THROW

		"m_simple_gun_r.abl",		//ITEMATT_GUN
		"m_simple_rail_gun.abl",	//ITEMATT_RAILGUN
		"m_simple_portal_gun.abl",	//ITEMATT_PORTALGUN

		"m_simple_scythe.cps",		//ITEMATT_SCYTHE
		"m_simple_dual_spear_r.cps",//ITEMATT_DUALSPEAR
		"m_simple_shuriken.cps",	//ITEMATT_SHURIKEN
	};

	std::string strWeaponPieceLM[ITEMATT_NSIZE] =
	{
		"",							//ITEMATT_NOTHING
		"",							//ITEMATT_SWORD
		"",							//ITEMATT_BLADE
		"",							//ITEMATT_DAGGER
		"",							//ITEMATT_SPEAR
		"",							//ITEMATT_STICK
		"",							//ITEMATT_GAUNT
		"",							//ITEMATT_BOW
		"",							//ITEMATT_THROW

		"m_simple_gun_l.abl",		//ITEMATT_GUN
		"",							//ITEMATT_RAILGUN
		"",							//ITEMATT_PORTALGUN

		"",							//ITEMATT_SCYTHE
		"m_simple_dual_spear_l.cps",//ITEMATT_DUALSPEAR
		"",							//ITEMATT_SHURIKEN
	};

	std::string strWeaponPieceRW[ITEMATT_NSIZE] = 
	{
		"",							//ITEMATT_NOTHING
		"w_simple_sword.cps",		//ITEMATT_SWORD
		"w_simple_blade.cps",		//ITEMATT_BLADE
		"",							//ITEMATT_DAGGER
		"w_simple_spear.cps",		//ITEMATT_SPEAR
		"",							//ITEMATT_STICK
		"w_simple_gaunt.cps",		//ITEMATT_GAUNT
		"w_simple_bow.cps",			//ITEMATT_BOW
		"",							//ITEMATT_THROW

		"w_simple_gun_r.abl",		//ITEMATT_GUN
		"w_simple_rail_gun.abl",	//ITEMATT_RAILGUN
		"w_simple_portal_gun.abl",	//ITEMATT_PORTALGUN

		"w_simple_scythe.cps",		//ITEMATT_SCYTHE
		"w_simple_dual_spear_r.cps",//ITEMATT_DUALSPEAR
		"w_simple_shuriken.cps",	//ITEMATT_SHURIKEN
	};

	std::string strWeaponPieceLW[ITEMATT_NSIZE] =
	{
		"",							//ITEMATT_NOTHING
		"",							//ITEMATT_SWORD
		"",							//ITEMATT_BLADE
		"",							//ITEMATT_DAGGER
		"",							//ITEMATT_SPEAR
		"",							//ITEMATT_STICK
		"",							//ITEMATT_GAUNT
		"",							//ITEMATT_BOW
		"",							//ITEMATT_THROW

		"w_simple_gun_l.abl",		//ITEMATT_GUN
		"",							//ITEMATT_RAILGUN
		"",							//ITEMATT_PORTALGUN

		"",							//ITEMATT_SCYTHE
		"w_simple_dual_spear_l.cps",//ITEMATT_DUALSPEAR
		"",							//ITEMATT_SHURIKEN
	};


	//cant go any faster than this
	//switch cases by EMCHARINDEX is the only reliable identification of whether its male or female
	std::string getWeaponPiece( bool bR, EMCHARGENDER emGender, GLITEM_ATT emAttack )
	{
		if ( emAttack >= ITEMATT_NSIZE )
			return "";

		if ( emGender == GLGENDER_W )
		{
			if ( bR )
				return strWeaponPieceRW[emAttack];
			else
				return strWeaponPieceLW[emAttack];
		}
		else if ( emGender == GLGENDER_M )
		{
			if ( bR )
				return strWeaponPieceRM[emAttack];
			else
				return strWeaponPieceLM[emAttack];
		}
		
		return "";
	};

};
