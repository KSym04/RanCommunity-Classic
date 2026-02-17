#ifndef CHECKWEARDEFINE_H__INCLUDED
#define CHECKWEARDEFINE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum EMCHECK_WEAR
{
	EMCHECK_WEAR_PASS				= 0,
	EMCHECK_WEAR_INVALID_ITEM,
	EMCHECK_WEAR_ERROR_WEAR_SLOT,


	EMCHECK_WEAR_SIZE,
};

struct SCHECK_WEAR_PARAM
{
	bool bCheckWear;

	bool bCheckWearSlotType;


	SCHECK_WEAR_PARAM()
		: bCheckWear(false)
		, bCheckWearSlotType(false)
	{

	};
};

#endif // CHECKWEARDEFINE_H__INCLUDED
