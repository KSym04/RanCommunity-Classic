/*!
 * \file GLPKComboDefine.h
 *
 * \author Juver
 * \date 2021/02/09
 *
 * 
 */

#ifndef GLPKCOMBODEFINE_H_INCLUDED__
#define GLPKCOMBODEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum EMPK_COMBO
{
	EMPK_COMBO_TYPE_00			= 0,	//first blood
	EMPK_COMBO_TYPE_01			= 1,	//double kill
	EMPK_COMBO_TYPE_02			= 2,	//tripple kill
	EMPK_COMBO_TYPE_03			= 3,	//ultra kill
	EMPK_COMBO_TYPE_04			= 4,	//rampage
	EMPK_COMBO_TYPE_05			= 5,	//killing spree
	EMPK_COMBO_TYPE_06			= 6,	//dominating
	EMPK_COMBO_TYPE_07			= 7,	//mega kill
	EMPK_COMBO_TYPE_08			= 8,	//unstoppable
	EMPK_COMBO_TYPE_09			= 9,	//wicked sick
	EMPK_COMBO_TYPE_10			= 10,	//monster kill
	EMPK_COMBO_TYPE_11			= 11,	//god like
	EMPK_COMBO_TYPE_12			= 12,	//holy shit
	EMPK_COMBO_TYPE_13			= 13,	//ownage
	EMPK_COMBO_TYPE_14			= 14,	//combo whore
	EMPK_COMBO_TYPE_15			= 15,	//combo whore
	EMPK_COMBO_TYPE_16			= 16,	//combo whore
	EMPK_COMBO_TYPE_17			= 17,	//combo whore
	EMPK_COMBO_TYPE_18			= 18,	//combo whore
	EMPK_COMBO_TYPE_19			= 19,	//combo whore
	EMPK_COMBO_TYPE_SIZE		= 20
};

enum EMPKCOMBO_COUNT
{
	EMPKCOMBO_COUNT_00		= 0,	//mega kill
	EMPKCOMBO_COUNT_01		= 1,	//unstoppable
	EMPKCOMBO_COUNT_02		= 2,	//wicked sick
	EMPKCOMBO_COUNT_03		= 3,	//monster kill
	EMPKCOMBO_COUNT_04		= 4,	//godlike
	EMPKCOMBO_COUNT_05		= 5,	//holy shit
	EMPKCOMBO_COUNT_06		= 6,	//ownage
	EMPKCOMBO_COUNT_07		= 7,	//combo whore
	EMPKCOMBO_COUNT_SIZE	= 8,
};

struct SPKCOMBO_COUNT
{
	DWORD dwCount[EMPKCOMBO_COUNT_SIZE];

	SPKCOMBO_COUNT()
	{
		for ( int i=0; i<EMPKCOMBO_COUNT_SIZE; ++i )
		{
			dwCount[i] = 0;
		}
	}
};

struct SPKCOMBO_STATE
{
	WORD wState;
	float fTimer;

	SPKCOMBO_STATE()
		: wState(0)
		, fTimer(0.0f)
	{
	};

	void Reset()
	{
		wState = 0;
		fTimer = 0.0f;
	}
};

#endif // GLPKCOMBODEFINE_H_INCLUDED__
