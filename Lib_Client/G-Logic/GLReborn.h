/*!
 * \file GLReborn.h
 *
 * \author Juver
 * \date 2022/04/25
 *
 * 
 */

#ifndef GLREBORN_H_INCLUDED__
#define GLREBORN_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "./GLCharDefine.h"

struct SREBORN
{
	WORD		wLevel;
	DWORD		dwPrice;
	WORD		wRemainStats;
	SCHARSTATS	sStatsReplace;
	WORD		wLevelAfter;
	DWORD		wRebornIncrease;

	SREBORN()
		: wLevel(0)
		, dwPrice(0)
		, wRemainStats(0)
		, wLevelAfter(0)
		, wRebornIncrease(0)
	{
	}

	void	Reset()
	{
		wLevel = 0;
		dwPrice = 0;
		wRemainStats = 0;
		wLevelAfter = 0;
		wRebornIncrease = 0;
		sStatsReplace.RESET();
	};
};

class GLReborn
{
public:
	enum
	{
		MAX_ARRAY = 10000
	};

private:
	SREBORN		sReborn[MAX_ARRAY]; //info for 1 to 200 reborn
	SREBORN		sRebornMax; //reborn for after max
	DWORD		dwMaxReborn; //max reborn to use from array

public:
	BOOL	LOAD ( std::string strFILE );
	void	Reset();
	SREBORN* GetRebornData( int nReborn );
	SREBORN* GetRebornDataMax()	{ return &sRebornMax; }
	DWORD	GetMaxReborn()	{ return dwMaxReborn; }

protected:
	GLReborn(void);

public:
	virtual ~GLReborn(void);

public:
	static GLReborn& GetInstance ();

};


#endif // GLREBORN_H_INCLUDED__
