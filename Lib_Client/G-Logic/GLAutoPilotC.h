/*!
 * \file GLAutoPilotC.h
 *
 * \author Juver
 * \date 2022/04/21
 *
 * 
 */

#ifndef GLAUTOPILOTC_H_INCLUDED__
#define GLAUTOPILOTC_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLAutoPilot.h"

class GLCharacter;
struct NET_MSG_GENERIC;

class GLAutoPilotC : public GLAutoPilot
{
public:
	GLAutoPilotC(GLCharacter* pChar);
	virtual ~GLAutoPilotC();

public:
	GLCharacter* m_pChar;
	STARGETID m_sTargetID;
	BOOL m_bValidTarget;

	SNATIVEID m_sSkillRun;

public:
	void Reset() override;
	void FrameMove( float fElapsedTime ) override;

	void Set( BOOL bEnable, float fRange );

public:
	void MsgProcess( NET_MSG_GENERIC* nmg );
	void MsgAutoPilotSetFb( NET_MSG_GENERIC* nmg );
	void MsgAutoPilotResetFb( NET_MSG_GENERIC* nmg );

};

#endif // GLAUTOPILOTC_H_INCLUDED__
