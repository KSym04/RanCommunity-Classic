/*!
 * \file GLWeaponSimplify.h
 *
 * \author Juver
 * \date 2022/04/10
 *
 * 
 */

#ifndef GLWEAPONSIMPLIFY_H_INCLUDED__
#define GLWEAPONSIMPLIFY_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLItemDef.h"
#include "./GLCharDefine.h"

namespace NSWEAPON_SIMPLIFY
{
	extern std::string strWeaponPieceRM[ITEMATT_NSIZE];
	extern std::string strWeaponPieceLM[ITEMATT_NSIZE];

	extern std::string strWeaponPieceRW[ITEMATT_NSIZE];
	extern std::string strWeaponPieceLW[ITEMATT_NSIZE];

	std::string getWeaponPiece( bool bR, EMCHARGENDER emGender, GLITEM_ATT emAttack );
};

#endif // GLWEAPONSIMPLIFY_H_INCLUDED__
