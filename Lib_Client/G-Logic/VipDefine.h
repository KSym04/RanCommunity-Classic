/*!
 * \file VipDefine.h
 *
 * \author Juver
 * \date 2022/04/28
 *
 * 
 */

#ifndef VIPDEFINE_H_INCLUDED__
#define VIPDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum EMVIP_LEVEL
{
	EMVIP_LEVEL_NONE	= 0,
	EMVIP_LEVEL_1		= 1,
	EMVIP_LEVEL_2		= 2,
	EMVIP_LEVEL_3		= 3,
	EMVIP_LEVEL_4		= 4,
	EMVIP_LEVEL_5		= 5,
	EMVIP_LEVEL_6		= 6,
	EMVIP_LEVEL_7		= 7,
	EMVIP_LEVEL_8		= 8,
	EMVIP_LEVEL_9		= 9,
	EMVIP_LEVEL_10		= 10,
	EMVIP_LEVEL_SIZE	= 11,
};

namespace COMMENT
{
	extern std::string CHAR_VIP[EMVIP_LEVEL_SIZE];
};

#endif // VIPDEFINE_H_INCLUDED__
