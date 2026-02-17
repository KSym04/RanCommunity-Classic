/*!
 * \file GLGameNotice.h
 *
 * \author Juver
 * \date 2021/06/08
 *
 * 
 */

#ifndef GLGAMENOTICE_H_INCLUDED__
#define GLGAMENOTICE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GAME_NOTICE_MSG_SIZE 1024
#define GAME_NOTICE_UPDATE_TIME 60.0f

enum GAME_NOTICE_TYPE
{
	GAME_NOTICE_TYPE_DEFAULT	= 0,
	GAME_NOTICE_TYPE_PER_HOUR	= 1,
};

//notice structure
struct SGAME_NOTICE
{
	DWORD	dwNoticeID;
	char	szNoticeMessage[GAME_NOTICE_MSG_SIZE];

	bool	bDayOfWeek[7];

	GAME_NOTICE_TYPE	emType;
	int		nHour;
	int		nMinute;

	SGAME_NOTICE()
		: dwNoticeID(0)
		, emType(GAME_NOTICE_TYPE_DEFAULT)

		, nHour(0)
		, nMinute(0)
	{
		memset( szNoticeMessage, 0, sizeof(char)*GAME_NOTICE_MSG_SIZE );
		memset( bDayOfWeek, 0, sizeof(bool)*7 );
		
	}
};

typedef std::map<DWORD, SGAME_NOTICE >	GAME_NOTICE_MAP;
typedef GAME_NOTICE_MAP::iterator		GAME_NOTICE_MAP_ITER;
typedef std::vector<SGAME_NOTICE>		GAME_NOTICE_VEC;

class GLGameNotice
{
public:
	GLGameNotice();
	~GLGameNotice();

public:
	GAME_NOTICE_MAP	m_mapNotice;
	float m_fUpdateTimer;

public:
	void ClearData();
	BOOL AddData( const SGAME_NOTICE& sData );

	void FrameMove( float fTime, float fElapsedTime );
	void Update();
};

#endif // GLGAMENOTICE_H_INCLUDED__
