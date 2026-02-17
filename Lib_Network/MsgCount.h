/*!
 * \file MsgCount.h
 *
 * \author Juver
 * \date 2022/04/12
 *
 * 
 */

#ifndef MSGCOUNT_H_INCLUDED__
#define MSGCOUNT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SMSG_COUNT
{
	DWORD dwMsgType;
	DWORD dwMsgCount;
	DWORD dwMsgSize;

	SMSG_COUNT()
		: dwMsgType(0)
		, dwMsgCount(0)
		, dwMsgSize(0)
	{

	}

	//for sorting
	//we sort by number of count
	//this is reverse order where the higher count should go top of the list
	//this is used for printing data later
	bool operator < ( const SMSG_COUNT& sData )
	{			
		if ( dwMsgCount > sData.dwMsgCount )
		{
			return true;
		}

		if ( dwMsgCount == sData.dwMsgCount )
		{
			if ( dwMsgType > sData.dwMsgType )
			{
				return true;
			}
		}

		if ( dwMsgCount == sData.dwMsgCount && dwMsgType == sData.dwMsgType )
		{
			if ( dwMsgSize > sData.dwMsgSize )
			{
				return true;
			}
		}

		return  false;
	}
};

typedef std::map<DWORD, SMSG_COUNT> MAP_MSG_COUNT;
typedef MAP_MSG_COUNT::iterator		MAP_MSG_COUNT_ITER;
typedef std::vector<SMSG_COUNT>		VEC_MSG_COUNT;

class CMsgCount
{
public:
	CMsgCount();
	~CMsgCount();

	static CMsgCount& GetInstance ();

public:
	MAP_MSG_COUNT m_mapData;
	CRITICAL_SECTION m_CS;
	BOOL m_bActive;

public:
	void Start();
	void Stop();

	void Add( DWORD dwType, DWORD dwSize );

};

#endif // MSGCOUNT_H_INCLUDED__
