/*!
 * \file GLAutoPilot.h
 *
 * \author Juver
 * \date 2022/04/15
 *
 * 
 */

#ifndef GLAUTOPILOT_H_INCLUDED__
#define GLAUTOPILOT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLAutoPilotDef.h"

class GLAutoPilot
{
public:
	GLAutoPilot();
	virtual ~GLAutoPilot();

public:
	BOOL m_bActive;
	float m_fRange;

	SNATIVEID m_sMap;
	D3DXVECTOR3 m_vPos;

public:
	virtual void Reset();

	void SetActive( BOOL bActive ) { m_bActive = bActive; }
	inline BOOL isActive() { return m_bActive; }
	virtual void FrameMove( float fElapsedTime );
};

#endif // GLAUTOPILOT_H_INCLUDED__
