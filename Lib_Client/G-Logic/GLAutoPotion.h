/*!
 * \file GLAutoPotion.h
 *
 * \author Juver
 * \date 2022/03/15
 *
 * 
 */

#ifndef GLAUTOPOTION_H_INCLUDED__
#define GLAUTOPOTION_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLAutoPotionDef.h"
#include "./GLCharData.h"

class GLAutoPotion
{
public:
	GLAutoPotion();
	~GLAutoPotion();

public:
	float m_fHPThreshold;
	float m_fMPThreshold;
	float m_fSPThreshold;

	float m_fCoolDown[AUTO_POTION_COOLDOWN_TYPE_SIZE];

public:
	void Reset();

	void FrameMove( float fElapsedTime );
	BOOL isReady( int nIndex );
	void applyDelay( int nIndex );
};


#endif // GLAUTOPOTION_H_INCLUDED__
