/*!
 * \file GLPostClient.h
 *
 * \author Juver
 * \date 2021/02/05
 *
 * 
 */

#ifndef GLPOSTCLIENT_H_INCLUDED__
#define GLPOSTCLIENT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <list>
#include <vector>
#include <queue>
#include <utility>

#include "./GLPostData.h"
#include "./GLPostDefine.h"
#include "./GLPost.h"
#include "./GLPostControlMsg.h"

struct SAPOSTCLIENT : public SAPOST
{
public:
	enum
	{
		EMSTATE_WAIT_DELETE		= 0x0001, // 삭제 대기중
		EMSTATE_WAIT_RETURN		= 0x0002, // 반환 대기중
	};

public:
	DWORD dwSTATE;

public:
	SAPOSTCLIENT();
	SAPOSTCLIENT( const SAPOST& rValue );
	
	const SAPOSTCLIENT& operator = ( const SAPOST& rValue );

public:
	const BOOL IsUnRead();
	const BOOL IsReturnPost();
	const BOOL IsDeletePost ();
	const __time64_t GetRecentSendDate(); 
	const DWORD GetExpireDay();
};

class GLCharacter;

namespace GLMSG {
	struct SNET_POST_SEND;
}

struct SAPOSTHISTORY : public SAPOSTSTATE
{

public:
	enum
	{
		ID_NULL = -1,
	};

public:
	SAPOSTHISTORY ();
	SAPOSTHISTORY ( const SAPOSTSTATE& rValue );
	const SAPOSTHISTORY& operator = ( const SAPOSTSTATE& rValue );

public:
	void SET ( const SAPOSTCLIENT& sPost );
	void SET( GLCharacter* pCharacter, GLMSG::SNET_POST_SEND& nmg );

public:
	const BOOL IsUnRead();
	const BOOL IsReturnPost();
	const BOOL IsDeletePost();
	const __time64_t GetRecentSendDate();
};


class GLPostClient : public GLPost
{

public:
	struct SPOSTSENDREQ
	{
		DWORD					ReqID;
		GLMSG::SNET_POST_SEND	Data;
	};

	struct SPOSTINFO
	{
		DWORD nNew;
		DWORD nRead;
		DWORD nReportNewPost;
		DWORD nWaiting;
		DWORD nTotal;

		SPOSTINFO ()
			: nNew		( 0 )
			, nRead		( 0 )
			, nWaiting	( 0 )
			, nReportNewPost( 0	)
		{
		}
	};

	typedef std::map<LONGLONG,SAPOSTHISTORY>	SPOSTHISTORYMAP;
	typedef	SPOSTHISTORYMAP::iterator			SPOSTHISTORYMAPITER;
	typedef std::pair<LONGLONG,SAPOSTHISTORY>	SPOSTHISTORYPAIR;

	typedef std::list<SAPOSTHISTORY>	        SPOSTHISTORYLIST;
	typedef	SPOSTHISTORYLIST::iterator			SPOSTHISTORYLISTITER;

	typedef std::vector<SAPOSTHISTORY*>			SPOSTHISTORYVEC;
	typedef SPOSTHISTORYVEC::iterator			SPOSTHISTORYVECITER;
	typedef SPOSTHISTORYVEC::reverse_iterator	SPOSTHISTORYVECRITER;

	struct SPOSTHISTORYINFOSET
	{
		enum SPOSTHISTORYINFOTYPE
		{
			SPOSTHISTORYALLINFO,
			SPOSTHISTORYSENDINFO,
			SPOSTHISTORYRECVINFO,
		};

		SPOSTHISTORYMAP	     HistoryMap;
		SPOSTHISTORYLIST     HistoryVirtualList;
		SPOSTHISTORYVEC	     HistoryVec;
		BOOL                 bReqHistory; // Note : 우편 현황 요청중
		DWORD                dwStateHowManyGet; // Postboxsize = 30
		DWORD			     dwLastHistTime;
		SPOSTHISTORYINFOTYPE spostHistoryType;

		SPOSTHISTORYINFOSET(SPOSTHISTORYINFOTYPE spostHistoryType)
			: bReqHistory( FALSE ),
			dwLastHistTime( 0 ),
			spostHistoryType( spostHistoryType )
		{
			switch( spostHistoryType )
			{
			case SPOSTHISTORYALLINFO:
				dwStateHowManyGet = GLCONST_POST::dwPOST_STATE_HOW_MANY_GET;
				break;

			case SPOSTHISTORYSENDINFO:
				dwStateHowManyGet = GLCONST_POST::dwPOST_SEND_STATE_HOW_MANY_GET;
				break;

			case SPOSTHISTORYRECVINFO:
				dwStateHowManyGet = GLCONST_POST::dwPOST_RECV_STATE_HOW_MANY_GET;
				break;
			}
		}

		void ClearHistory()
		{
			if( !HistoryMap.empty() )
			{
				HistoryMap.clear();
			}

			if( !HistoryVirtualList.empty() )
			{
				HistoryVirtualList.clear();
			}

			if (!HistoryVec.empty() )
			{
				HistoryVec.clear();
			}
		}

		DWORD GetMaxHistory()
		{
			return HistoryVec.size();
		}

		BOOL IsHistCoolTime()
		{
			DWORD dwCurrentPostHist = ::GetTickCount();	

			if ( dwCurrentPostHist > dwLastHistTime + GLCONST_POST::dwPOST_RECV_COOL_TIME_MS )
			{				
				return FALSE;
			}

			return TRUE;
		}
	};

public:
	typedef std::map<LONGLONG,SAPOSTCLIENT>		SPOSTRCVMAP;
	typedef	SPOSTRCVMAP::iterator				SPOSTRCVMAPITER;
	typedef std::pair<LONGLONG,SAPOSTCLIENT>	SPOSTRCVPAIR;

	typedef std::vector<SAPOSTCLIENT*>			SPOSTRCVVEC;
	typedef SPOSTRCVVEC::iterator				SPOSTRCVVECITER;
	typedef SPOSTRCVVEC::reverse_iterator		SPOSTRCVVECRITER;

public:
	typedef std::queue<SPOSTSENDREQ>			SPOSTSENDQUEUE;

public:    
	GLPostClient();
	virtual ~GLPostClient(void);

public:
	static GLPostClient& GetInstance();

protected:
	SPOSTRCVMAP		 m_ReceiveMap;
	SPOSTRCVVEC		 m_ReceiveVec;

	SPOSTHISTORYINFOSET m_HistoryInfoSet;
	SPOSTHISTORYINFOSET m_SendHistoryInfoSet;
	SPOSTHISTORYINFOSET m_RecvHistoryInfoSet;

	SPOSTINFO		 m_sPostInfo;

protected:
	BOOL			 m_bReqPost;		// Note : 우편 받기 요청중
	BOOL			 m_bReqSend;		// Note : 우편 전송 요청중

protected:
	SPOSTSENDQUEUE	 m_SendQueue;

protected:
	DWORD			 m_dwLastSendTime;
	DWORD			 m_dwLastRecvTime;


public:
	void			 ClearReceivePost();

public:
	const BOOL		 IsRequirePost() { return m_bReqPost;	}
	const BOOL		 IsRequireSend() { return m_bReqSend;	}

public:
	SPOSTRCVMAP&	 GetReceiveMap () { return m_ReceiveMap; }
	SPOSTRCVVEC&	 GetReceiveVec () { return m_ReceiveVec; }

	const SAPOSTCLIENT*	 GetSAPOSTCLIENT_ReceiveVec( DWORD dwIndex ) const;

	SPOSTHISTORYINFOSET& GetHistoryInfoSet () { return m_HistoryInfoSet; }
	SPOSTHISTORYINFOSET& GetSendHistoryInfoSet () { return m_SendHistoryInfoSet; }
	SPOSTHISTORYINFOSET& GetRecvHistoryInfoSet () { return m_RecvHistoryInfoSet; }

public:
	HRESULT			 FrameMove ( float fTime, float fElapsedTime );

public:
	const DWORD		 GetMaxPost();
	const DWORD		 GetNewPost()		{ return m_sPostInfo.nNew;	}
	const DWORD		 GetWaitingPost()	{ return m_sPostInfo.nWaiting;	}
	const DWORD		 GetTotalPost()		{ return m_sPostInfo.nTotal;	}
	const DWORD		 GetReadPost()		{ return m_sPostInfo.nRead;	}
};



#endif // GLPOSTCLIENT_H_INCLUDED__
