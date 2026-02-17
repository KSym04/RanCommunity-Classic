/*!
 * \file GLAutoPilotS.h
 *
 * \author Juver
 * \date 2022/04/21
 *
 * 
 */

#ifndef GLAUTOPILOTS_H_INCLUDED__
#define GLAUTOPILOTS_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLAutoPilot.h"

class GLChar;
struct NET_MSG_GENERIC;

class GLAutoPilotS : public GLAutoPilot
{
public:
	GLAutoPilotS(GLChar* pChar);
	virtual ~GLAutoPilotS();

public:
	GLChar* m_pChar;

public:
	void Reset() override;

public:
	void MsgProcess( NET_MSG_GENERIC* nmg );
	void MsgAutoPilotSet( NET_MSG_GENERIC* nmg );
	void MsgAutoPilotReset( NET_MSG_GENERIC* nmg );

};

#endif // GLAUTOPILOTS_H_INCLUDED__
