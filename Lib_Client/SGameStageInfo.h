/*!
 * \file SGameStageInfo.h
 *
 * \author Juver
 * \date 2020/02/02
 *
 * 
 */

#ifndef SGAMESTAGEINFO_H_INCLUDED__
#define SGAMESTAGEINFO_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct SGameStageInfo
{
	DWORD dwFlag;

	bool bCharacterDead;

	SGameStageInfo();
	~SGameStageInfo();
};


#endif // SGAMESTAGEINFO_H_INCLUDED__
