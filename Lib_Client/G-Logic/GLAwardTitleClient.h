/*!
 * \file GLAwardTitleClient.h
 *
 * \author Juver
 * \date 2021/02/12
 *
 * 
 */

#ifndef GLAWARDTITLECLIENT_H_INCLUDED__
#define GLAWARDTITLECLIENT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./GLAwardTitle.h"

struct NET_MSG_GENERIC;

class GLAwardTitleClient
{
public:
	bool		m_bReqData;

	MAP_AWARD_TITLE_DATA		m_mapAwardTitle;

public:
	void Clear();
	void RequestData();
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg );

	const TCHAR* GetTitle( DWORD dwID, EMAWARD_TITLE_TYPE emType );

public:
	static GLAwardTitleClient& GetInstance ();

protected:
	GLAwardTitleClient(void);

public:
	~GLAwardTitleClient(void);
};



#endif // GLAWARDTITLECLIENT_H_INCLUDED__
