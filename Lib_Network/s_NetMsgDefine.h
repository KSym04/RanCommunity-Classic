/*!
 * \file s_NetMsgDefine.h
 *
 * \author Juver
 * \date 2019/10/26
 *
 * 
 */

#ifndef S_NETMSGDEFINE_H_INCLUDED__
#define S_NETMSGDEFINE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DBACTION_CLIENT_ID 0
#define CLIENTID_NULL      MAXDWORD

///////////////////////////////////////////////////////////////////////////////
// Base Msg
// If you change base message, all message number will be change
// If user find packet order, change NET_MSG_BASE number
// All Message Number must less than 2 byte(65535)


#if defined ( BUILD_CH )

	#if defined ( BUILD_EP9 )
		#define NET_MSG_BASE				1149
	#elif defined ( BUILD_EP7 )
		#define NET_MSG_BASE				1147
	#elif defined ( BUILD_EP6 )
		#define NET_MSG_BASE				1145
	#elif defined ( BUILD_EP4 )
		#define NET_MSG_BASE				1143
	#else
		#define NET_MSG_BASE				1141
	#endif

#elif defined ( BUILD_CHY )

	#if defined ( BUILD_EP9 )
	#define NET_MSG_BASE				1159
	#elif defined ( BUILD_EP7 )
	#define NET_MSG_BASE				1157
	#elif defined ( BUILD_EP6 )
	#define NET_MSG_BASE				1155
	#elif defined ( BUILD_EP4 )
	#define NET_MSG_BASE				1153
	#else
	#define NET_MSG_BASE				1151
	#endif

#elif defined ( BUILD_X )

	#if defined ( BUILD_EP9 )
		#define NET_MSG_BASE				1129
	#elif defined ( BUILD_EP7 )
		#define NET_MSG_BASE				1127
	#elif defined ( BUILD_EP6 )
		#define NET_MSG_BASE				1125
	#elif defined ( BUILD_EP4 )
		#define NET_MSG_BASE				1123
	#else
		#define NET_MSG_BASE				1121
	#endif

#else

	#if defined ( BUILD_EP9 )
		#define NET_MSG_BASE				1109
	#elif defined ( BUILD_EP7 )
		#define NET_MSG_BASE				1107
	#elif defined ( BUILD_EP6 )
		#define NET_MSG_BASE				1105
	#elif defined ( BUILD_EP4 )
		#define NET_MSG_BASE				1103
	#else
		#define NET_MSG_BASE				1101
	#endif

#endif




// Memo : 아래 기본 메세지는 중복이 되지 않도록 정의한다.
//
//#if defined(CH_PARAM)
//#define NET_MSG_BASE				1019				  // 기본 메시지
//#elif defined(HK_PARAM)
//#define NET_MSG_BASE				1007				  // 기본 메시지
//#elif defined(ID_PARAM)
//#define NET_MSG_BASE				977					  // 기본 메시지
//#elif defined(JP_PARAM)
//#define NET_MSG_BASE				1031				  // 기본 메시지
//#elif defined(KR_PARAM)
//#define NET_MSG_BASE				987					  // 기본 메시지
//#elif defined(KRT_PARAM)
//#define NET_MSG_BASE				971					  // 기본 메시지
//#elif defined(MY_PARAM)
//#define NET_MSG_BASE				969					  // 기본 메시지
//#elif defined(MYE_PARAM)
//#define NET_MSG_BASE				1005				  // 기본 메시지
//#elif defined(PH_PARAM)
//#define NET_MSG_BASE				1017				  // 기본 메시지
//#elif defined(VN_PARAM)
//#define NET_MSG_BASE				1021				  // 기본 메시지
//#elif defined(TW_PARAM)
//#define NET_MSG_BASE				997					  // 기본 메시지
//#elif defined(TH_PARAM)
//#define NET_MSG_BASE				989					  // 기본 메시지
//#elif defined(GS_PARAM)
//#define NET_MSG_BASE				1004				  // 기본 메시지
//#else
//#define NET_MSG_BASE				995					  // 기본 메시지
//#endif


/*
#define NET_MSG_LGIN				(NET_MSG_BASE +  507) // 로그인서버 메시지
#define NET_MSG_LOBBY				(NET_MSG_BASE + 1013) // 게임서버 로비
#define NET_MSG_LOBBY_MAX			(NET_MSG_BASE + 1509) // 게임서버 로비 최대값

#define NET_MSG_GCTRL				(NET_MSG_BASE + 2011) // 게임서버 게임플레이
#define NET_MSG_GCTRL_MAX			(NET_MSG_BASE + 4010) // 게임서버 게임플레이 최대값
*/

// 2005-11-04
//#define NET_MSG_LGIN				(NET_MSG_BASE +  413) // 로그인서버 메시지
//#define NET_MSG_LOBBY				(NET_MSG_BASE +  913) // 게임서버 로비
//#define NET_MSG_LOBBY_MAX			(NET_MSG_BASE + 1413) // 게임서버 로비 최대값
//
//#define NET_MSG_GCTRL				(NET_MSG_BASE + 1823) // 게임서버 게임플레이
//#define NET_MSG_GCTRL_MAX			(NET_MSG_BASE + 3823) // 게임서버 게임플레이 최대값

// 2007-04-11
#define NET_MSG_LGIN				(NET_MSG_BASE +  450) // 로그인서버 메시지
#define NET_MSG_LOBBY				(NET_MSG_BASE +  950) // 게임서버 로비
#define NET_MSG_LOBBY_MAX			(NET_MSG_BASE + 1450) // 게임서버 로비 최대값

#define NET_MSG_GCTRL				(NET_MSG_BASE + 1900) // 게임서버 게임플레이
#define NET_MSG_GCTRL_MAX			(NET_MSG_BASE + 3900) // 게임서버 게임플레이 최대값


// Login information
enum EMNET_MSG
{
	NET_MSG_BASE_ZERO           = 0,

	///////////////////////////////////////////////////////////////////////////
	// Version check and downloading new version
	// Do not change version and Crypt key message
	NET_MSG_VERSION_OK			= 100, //< 버전이 일치함
	NET_MSG_VERSION_INFO		= 110, //< 버전 정보를 보냄
	NET_MSG_VERSION_REQ			= 120, //< 버전 정보를 요청함
	NET_MSG_REQ_CRYT_KEY		= 130, //< 클라이언트->게임서버 : 암호키 요청
	NET_MSG_SND_CRYT_KEY		= 140, //< 게임서버->클라이언트 : 암호키 전송	
	NET_MSG_RANDOM_NUM          = 141, //< Agent->Client : 랜덤넘버
	//	NET_MSG_PING_REQUEST		= 150, //< 서버로의 ping 을 요청한다.
	//	NET_MSG_PING_ANSWER		    = 151, //< 서버에서 ping 을 응답한다.	
	NET_MSG_HEARTBEAT_SERVER_REQ= 153, //< SERVER->SERVER Server HeartBeat Request
	NET_MSG_HEARTBEAT_SERVER_ANS= 154, //< SERVER->SERVER Server HeartBeat Answer
	NET_MSG_SVR_DOWN            = 155, //< Server System Down!
	NET_MSG_SVR_CLOSECLIENT     = 156, //< Server Close Client
	NET_MSG_HEARTBEAT_CLIENT_REQ= 160, //< SERVER->CLIENT Client HeartBeat Request
	NET_MSG_HEARTBEAT_CLIENT_ANS= 161, //< CLIENT->SERVER Client HeartBeat Answer

	NET_MSG_CLIENT_CHECK		= 162,
	NET_MSG_CLIENT_CHECK_FB		= 163,

	NET_MSG_COMPRESS            = 170, //< Compressed message	

	NET_MSG_IPFILTER_SERVER_TO_SESSION_BLOCK_ADD    = 180,
	NET_MSG_IPFILTER_SERVER_TO_SESSION_BLOCK_DEL    = 181,
	NET_MSG_IPFILTER_SERVER_TO_SESSION_KNOWN_ADD    = 182,
	NET_MSG_IPFILTER_SERVER_TO_SESSION_KNOWN_DEL    = 183,
	NET_MSG_IPFILTER_SESSION_TO_SERVER_BLOCK_ADD	= 184,
	NET_MSG_IPFILTER_SESSION_TO_SERVER_BLOCK_DEL	= 185,
	NET_MSG_IPFILTER_SESSION_TO_SERVER_KNOWN_ADD	= 186,
	NET_MSG_IPFILTER_SESSION_TO_SERVER_KNOWN_DEL	= 187,
	NET_MSG_IPFILTER_SERVER_TO_SESSION_LIST_SAVE    = 188,


	NET_MSG_WRITE_IP_LOGIN_TO_SESSION	= 190,
	NET_MSG_WRITE_IP_SESSION_TO_SERVER	= 191,

	/*dmk14 whitelist*/
	NET_MSG_IPFILTER_WHITELIST_ADD_SESSION    = 192,
	NET_MSG_IPFILTER_WHITELIST_ADD_SERVERS    = 193,
	NET_MSG_IPFILTER_WHITELIST_DEL_SESSION    = 194,
	NET_MSG_IPFILTER_WHITELIST_DEL_SERVERS    = 195,

	///////////////////////////////////////////////////////////////////////////	
	// Server info message
	NET_MSG_REQ_GAME_SVR		= (NET_MSG_LGIN + 100), //< 클라이언트가 -> 로그인서버 : 게임 서버의 정보를 요청
	NET_MSG_SND_GAME_SVR	    = (NET_MSG_LGIN + 110), //< 로그인서버   -> 클라이언트 : 게임 서버의 정보를 전송
	NET_MSG_SND_GAME_SVR_END	= (NET_MSG_LGIN + 120), //< 로그인서버   -> 클라이언트 : 게임 서버 정보 전달 끝
	NET_MSG_REQ_FULL_SVR_INFO	= (NET_MSG_LGIN + 130), //< 세션서버 -> 로그인서버, 게임서버 : 서버 전체 정보를 요청한다.
	NET_MSG_SND_FULL_SVR_INFO	= (NET_MSG_LGIN + 140), //< 로그인서버, 게임서버 -> 세션서버 : 서버 전체 정보를 Session 서버에 전송한다.
	NET_MSG_REQ_CUR_STATE		= (NET_MSG_LGIN + 150), //< 세션서버 -> 로그인서버, 게임서버 : 현재 서버상태= (간략한),를 요청한다.
	NET_MSG_SND_CUR_STATE		= (NET_MSG_LGIN + 160), //< 로그인서버, 게임서버 -> 세션서버 : 현재 서버상태= (간략한),를 전송한다.
	NET_MSG_SND_CHANNEL_STATE   = (NET_MSG_LGIN + 161), //< Agent->Session : 현재 채널상태를 전송한다.
	NET_MSG_REQ_SVR_INFO		= (NET_MSG_LGIN + 170), //< 로그인서버 -> 세션서버 : 게임서버의 상태정보를 요청한다.
	NET_MSG_SND_SVR_INFO		= (NET_MSG_LGIN + 180), //< 세션서버 -> 로그인서버 : 게임서버의 상태정보를 전송한다.
	NET_MSG_SND_SVR_INFO_RESET	= (NET_MSG_LGIN + 190), //< 세션서버 -> 로그인서버 : 게임서버의 상태정보를 리셋하라고 명령.
	NET_MSG_REQ_ALL_SVR_INFO    = (NET_MSG_LGIN + 191), //< 관리프로그램->세션서버 : 모든 서버의 상태정보를 요청한다.
	NET_MSG_SND_ALL_SVR_INFO    = (NET_MSG_LGIN + 192), //< 세션서버->관리프로그램 : 모든 서버의 상태정보를 전송한다.
	NET_MSG_SND_ALL_SVR_INFO_S  = (NET_MSG_LGIN + 193), //< 세션서버->관리프로그램 : 상태정보전송 시작
	NET_MSG_SND_ALL_SVR_INFO_E	= (NET_MSG_LGIN + 194), //< 세션서버->관리프로그램 : 상태정보를 모두 보냈음을 알린다.
	NET_MSG_RECONNECT_FIELD     = (NET_MSG_LGIN + 195), //< 세션서버->에이전트 : 필드로 다시 접속하라.
	NET_MSG_I_AM_AGENT          = (NET_MSG_LGIN + 196), //< Agent->Field : 자신이 서버통신용 슬롯임을 알림

	/*dmk14 whitelist*/
	NET_MSG_REQ_GAME_SVR_WHILELIST	= (NET_MSG_LGIN + 197),

	NET_MSG_LOGOUT_G_S			= (NET_MSG_LGIN + 200), //< Agent->Session : 로그아웃 정보	
	NET_MSG_LOGOUT_G_S_S		= (NET_MSG_LGIN + 201), //< Agent->Session : 로그아웃 정보. 사용자 ID 만 전송
	DAUM_NET_MSG_LOGOUT_G_S     = (NET_MSG_LGIN + 202), //< Agent->Session : Daum 로그아웃 정보

	//< User Information message
	NET_MSG_REQ_USER_INFO		= (NET_MSG_LGIN + 210), //< 사용자 정보를 요청한다.
	NET_MSG_USER_INFO			= (NET_MSG_LGIN + 220), //< 사용자 정보를 전송한다.

	//< Server control message	
	NET_MSG_SVR_CMD				= (NET_MSG_LGIN + 300),
	NET_MSG_SVR_PAUSE			= (NET_MSG_LGIN + 310), //< 서버를 잠시 멈춘다.
	NET_MSG_SVR_RESUME			= (NET_MSG_LGIN + 311), //< 멈추어진 서버를 다시 가동시킨다.	
	NET_MSG_SVR_RESTART			= (NET_MSG_LGIN + 312), //< 서버를 완전히 멈추고 새롭게 가동시킨다.
	NET_MSG_SVR_STOP			= (NET_MSG_LGIN + 313), //< 서버를 완전히 멈춘다.
	NET_MSG_SND_CHANNEL_FULL_STATE = (NET_MSG_LGIN + 314), //< 서버의 채널 FULL 상태를 LoginServer에 보낸다.

	//<-----------------------------------------------------------------------------------------------------[LOBBY]	
	// NET_MSG_LOGIN				= (NET_MSG_LOBBY + 100), //< 로그인 
	NET_MSG_LOGIN_2 			= (NET_MSG_LOBBY + 107), //< 로그인
	NET_MSG_LOGIN_FB			= (NET_MSG_LOBBY + 108), //< 로그인 결과	
	NET_MSG_LOGIN_FB_SUB_OK_FIELD=(NET_MSG_LOBBY + 109), //< 로그인성공결과를 필드서버로 전송해서 세팅하게 한다.		

	DAUM_NET_MSG_LOGIN			= (NET_MSG_LOBBY + 110), //< Daum 로그인
	DAUM_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 111), //< Daum 로그인 결과 Agent->Client	

	MET_MSG_SVR_FULL            = (NET_MSG_LOBBY + 112), //< 서버 Full

	CHINA_NET_MSG_LOGIN         = (NET_MSG_LOBBY + 113), //< China 로그인
	CHINA_NET_MSG_LOGIN_FB      = (NET_MSG_LOBBY + 114), //< China 로그인 결과 Agent->Client

	GSP_NET_MSG_LOGIN           = (NET_MSG_LOBBY + 115), //< GSP 로그인
	GSP_NET_MSG_LOGIN_FB        = (NET_MSG_LOBBY + 116), //< GSP 로그인 결과 AGent->Client

	TERRA_NET_MSG_LOGIN         = (NET_MSG_LOBBY + 120), //< Terra 로그인
	TERRA_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 121), //< Terra 로그인 결과 Agent->Client

	TERRA_NET_MSG_PASSCHECK     = (NET_MSG_LOBBY + 122), //< Terra Password Check
	DAUM_NET_MSG_PASSCHECK      = (NET_MSG_LOBBY + 123), //< Daum Password Check
	NET_MSG_PASSCHECK_FB		= (NET_MSG_LOBBY + 124), //< Password Check 결과 Agent->Client
	EXCITE_NET_MSG_PASSCHECK    = (NET_MSG_LOBBY + 125), //< Excite Password Check

	EXCITE_NET_MSG_LOGIN        = (NET_MSG_LOBBY + 130), //< Excite 로그인
	EXCITE_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 131), //< Excite 로그인 결과 Agent->Client

	JAPAN_NET_MSG_LOGIN         = (NET_MSG_LOBBY + 132), //< 일본(Gonzo) 로그인
	JAPAN_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 133), //< 일본(Gonzo) 로그인 결과 Agent->Client
	JAPAN_NET_MSG_UUID			= (NET_MSG_LOBBY + 134), //< 일본(Gonzo) 로그인 UUID

	GS_NET_MSG_LOGIN			= (NET_MSG_LOBBY + 135), //< GS 로그인
	GS_NET_MSG_LOGIN_FB			= (NET_MSG_LOBBY + 136), //< GS 로그인 결과 Agent->Client

	THAI_NET_MSG_LOGIN          = (NET_MSG_LOBBY + 140), //< Thailand 로그인
	THAI_NET_MSG_LOGIN_FB		= (NET_MSG_LOBBY + 141), //< Thailand 로그인 결과 Agent->Client

	NET_MSG_REQ_RAND_KEY        = (NET_MSG_LOBBY + 150), //< 랜덤키 요청 Client->Agent

	NET_MSG_SND_ENCRYPT_KEY		= (NET_MSG_LOBBY + 160), //< 로그인 랜덤키 전송 Login->Session

	//< Character create message
	NET_MSG_CHA_INFO			= (NET_MSG_LOBBY + 200), //< 캐릭터 정보
	NET_MSG_CHA_NEW				= (NET_MSG_LOBBY + 201), //< 새로운 캐릭터 생성
	NET_MSG_CHA_NEW_FB			= (NET_MSG_LOBBY + 202), //< 캐릭터 생성 성공/실패 메시지

	NET_MSG_CHA_DEL				= (NET_MSG_LOBBY + 210), //< Client->Agent : 케릭터 삭제
	NET_MSG_CHA_DEL_FB			= (NET_MSG_LOBBY + 211), //< Agent->Client : 케릭터 삭제 결과
	NET_MSG_CHA_DEL_FB_OK		= (NET_MSG_LOBBY + 212), //< Agent->Client : 케릭터 삭제 성공
	NET_MSG_CHA_DEL_FB_ERROR	= (NET_MSG_LOBBY + 213), //< Agent->Client : 케릭터 삭제 실패
	DAUM_NET_MSG_CHA_DEL		= (NET_MSG_LOBBY + 214), //< Client->Agent : 케릭터 삭제
	NET_MSG_CHA_DEL_FB_CLUB	    = (NET_MSG_LOBBY + 215), //< Agent->Client : 케릭터 삭제 실패(클럽마스터이기 때문에 삭제가 되지 않는다)
	TERRA_NET_MSG_CHA_DEL		= (NET_MSG_LOBBY + 216), //< Client->Agent : 케릭터 삭제
	EXCITE_NET_MSG_CHA_DEL		= (NET_MSG_LOBBY + 217), //< Client->Agent : 케릭터 삭제
	GSP_NET_MSG_CHA_DEL		    = (NET_MSG_LOBBY + 218), //< Client->Agent : 케릭터 삭제
	JAPAN_NET_MSG_CHA_DEL		= (NET_MSG_LOBBY + 219), //< Client->Agent : 케릭터 삭제
	GS_NET_MSG_CHA_DEL			= (NET_MSG_LOBBY + 220), //< Client->Agent : 케릭터 삭제

	NET_MSG_CHA_DECREASE        = (NET_MSG_LOBBY + 240), //< Agent->Session : 캐릭터 생성, 갯수 감소
	NET_MSG_TEST_CHA_DECREASE   = (NET_MSG_LOBBY + 241), //< Agent->Session : 캐릭터 생성, 갯수 감소
	DAUM_NET_MSG_CHA_DECREASE   = (NET_MSG_LOBBY + 242), //< DAUM : Agent->Session : 캐릭터 생성, 갯수 감소
	DAUM_NET_MSG_TEST_CHA_DECREASE=(NET_MSG_LOBBY+ 243), //< DAUM : Agent->Session : 캐릭터 생성, 갯수 감소

	NET_MSG_CHA_INCREASE        = (NET_MSG_LOBBY + 244), //< Agent->Session : 캐릭터 삭제, 갯수 증가
	NET_MSG_TEST_CHA_INCREASE   = (NET_MSG_LOBBY + 245), //< Agent->Session : 캐릭터 삭제, 갯수 증가
	DAUM_NET_MSG_CHA_INCREASE   = (NET_MSG_LOBBY + 246), //< DAUM : Agent->Session : 캐릭터 삭제, 갯수 증가
	DAUM_NET_MSG_TEST_CHA_INCREASE= (NET_MSG_LOBBY+247),//< DAUM : Agent->Session : 캐릭터 삭제, 갯수 증가

	NET_MSG_REQ_CHA_ALL_INFO	= (NET_MSG_LOBBY + 300), //< 모든 캐릭터의 정보를 요청한다.
	NET_MSG_REQ_CHA_INFO		= (NET_MSG_LOBBY + 301), //< 특정 캐릭터의 정보를 요청한다.
	NET_MSG_REQ_CHA_BINFO		= (NET_MSG_LOBBY + 302), //< 특정 캐릭터의 기본= (캐릭터선택화면),정보 요청한다.
	NET_MSG_REQ_CHA_FEEDBACK	= (NET_MSG_LOBBY + 303), //< 캐릭터 정보 피드백데이터
	NET_MSG_CHA_INFO_LOGIN		= (NET_MSG_LOBBY + 304), //< 로그인서버에서 클라이언트에게 넘겨주는 캐릭터 정보
	NET_MSG_REQ_CHA_BAINFO		= (NET_MSG_LOBBY + 305), //< 클라이언트->게임서버 : 캐릭터 기초정보 요청
	NET_MSG_CHA_BAINFO			= (NET_MSG_LOBBY + 306), //< 게임서버->클라이언트 : 캐릭터 기초정보	

	NET_MSG_LOBBY_CHAR_SEL		= (NET_MSG_LOBBY + 390), //< 필드서버->클라이언트 : 캐릭터 선택용 정보.
	NET_MSG_LOBBY_CHAR_JOIN		= (NET_MSG_LOBBY + 391), //< 필드서버->클라이언트 : 선택된 캐릭터 정보를 보냄.
	NET_MSG_LOBBY_CHAR_ITEM		= (NET_MSG_LOBBY + 392), //< 필드서버->클라이언트 : 선택된 캐릭터 인벤토리 아이템을 보냄.
	NET_MSG_LOBBY_CHAR_JOIN_FB	= (NET_MSG_LOBBY + 393), //< 필드서버->클라이언트 : 선택된 캐릭터의 접속참가 실패 메시지.
	NET_MSG_LOBBY_CHAR_SKILL	= (NET_MSG_LOBBY + 394), //<	필드서버->클라이언트 : 선택된 캐릭터가 배운 스킬 정보 전송.
	NET_MSG_LOBBY_QUEST_END		= (NET_MSG_LOBBY + 395), //<	퀘스트 완료 정보.
	NET_MSG_LOBBY_QUEST_PROG	= (NET_MSG_LOBBY + 396), //<	퀘스트 진행 정보.
	NET_MSG_LOBBY_CHARGE_ITEM	= (NET_MSG_LOBBY + 397), //<	구입 아이템 전송.
	NET_MSG_LOBBY_CHAR_PUTON	= (NET_MSG_LOBBY + 398), //< 필드서버->클라이언트 : 선택된 캐릭터 착용 아이템을 보냄.

	NET_MSG_LOBBY_CLUB_INFO		= (NET_MSG_LOBBY + 399), //<	클럽 정보.
	NET_MSG_LOBBY_CLUB_MEMBER	= (NET_MSG_LOBBY + 400), //<	클럽 멤버.
	NET_MSG_LOBBY_CLUB_ALLIANCE	= (NET_MSG_LOBBY + 401), //<	클럽 동맹.
	NET_MSG_LOBBY_CLUB_BATTLE	= (NET_MSG_LOBBY + 402), //<	클럽 배틀.

	NET_MSG_CHARPOS_FROMDB2AGT	= (NET_MSG_LOBBY + 403), //<	캐릭터 위치 DB 저장 결과

	NET_MSG_LOBBY_ITEM_COOLTIME = (NET_MSG_LOBBY + 404), //<	아이템 쿨타임

	NET_MSG_LOBBY_GAME_JOIN			= (NET_MSG_LOBBY + 411), //< 클라이언트  ->이이전트서버 : 로비에서 선택한 캐릭터로 게임접속	
	NET_MSG_LOBBY_GAME_COMPLETE		= (NET_MSG_LOBBY + 412), //< 클라이언트  ->클라이언트   : 게임에 들어갈 정보를 모두 받아서 게임에 들어감.
	NET_MSG_GAME_JOIN_OK			= (NET_MSG_LOBBY + 413), //< 필드서버    ->세션서버     : 해당 캐릭터가 조인성공
	MET_MSG_GAME_JOIN_FIELDSVR		= (NET_MSG_LOBBY + 414), //< 에이전트서버->필드서버     : 해당캐릭터가 게임에 조인요청
	MET_MSG_GAME_JOIN_FIELDSVR_FB	= (NET_MSG_LOBBY + 415), //< 필드서버->에이전트서버:선택된 캐릭터로 조인함

	NET_MSG_CONNECT_CLIENT_FIELD	= (NET_MSG_LOBBY + 416), //<	에이전트서버->클라이언트   : 해당필드서버로 접속 명령
	NET_MSG_JOIN_FIELD_IDENTITY		= (NET_MSG_LOBBY + 417), //<	클라이언트  ->필드서버     : 새로운 Field 서버에 접속후 자신의 인식자 전송.

	NET_MSG_LOBBY_REQ_GAME_JOIN		= (NET_MSG_LOBBY + 421), //< 클라이언트  ->클라이언트   : 인터페이스로 선택된 케릭으로 접속을 명령하기 위해서.

	NET_MSG_AGENT_REQ_JOIN			= (NET_MSG_LOBBY + 422), //< 에이젼트DB  ->에이젼트   : db에서 읽은 char로 접속시도.
	NET_MSG_FIELD_REQ_JOIN			= (NET_MSG_LOBBY + 423), //< 필드DB		->필드서버   : db에서 읽은 char로 접속시도.

	NET_MSG_EVENT_LOTTERY           = (NET_MSG_LOBBY + 430), //< 클라이언트  ->에이전트   : 복권입력
	NET_MSG_EVENT_LOTTERY_FB        = (NET_MSG_LOBBY + 431), //< 에이전트    ->클라이언트 : 복권결과

	NET_MSG_LOBBY_CHINA_ERROR		= (NET_MSG_LOBBY + 432), //< 중국 로그인시 플레이 누적시간이 5시간이 넘고 오프라인 누적시간이 5시간이 되지 않았을 경우 에러 메시지

	NET_MSG_LOBBY_CHAR_VIETNAM_ITEM = (NET_MSG_LOBBY + 433), //< 베트남 인벤토리 아이템 전송 메시지

	NET_MSG_LOBBY_CHAR_PUTON_EX		= (NET_MSG_LOBBY + 434), //< 필드서버->클라이언트 : 선택된 캐릭터 착용 아이템을 보냄. ( 한개씩 보냄 )

	/*itemfood system, Juver, 2017/05/25 */
	NET_MSG_LOBBY_CHAR_ITEMFOOD		= (NET_MSG_LOBBY + 435),

	/*activity system, Juver, 2017/10/30 */
	NET_MSG_LOBBY_CHAR_ACTIVITY_PROG	= (NET_MSG_LOBBY + 436),
	NET_MSG_LOBBY_CHAR_ACTIVITY_DONE	= (NET_MSG_LOBBY + 437),

	/*register page, Juver, 2017/11/18 */
	NET_MSG_REGISTER_INIT				= (NET_MSG_LOBBY + 438),
	NET_MSG_REGISTER_ACTION				= (NET_MSG_LOBBY + 439),
	NET_MSG_REGISTER_ACTION_FB			= (NET_MSG_LOBBY + 440),
	NET_MSG_REGISTER_ACTION_FB2			= (NET_MSG_LOBBY + 441),

	/* personal lock system, Juver, 2019/12/06 */
	NET_MSG_LOBBY_CHAR_INFO_PERSONAL_LOCK	= (NET_MSG_LOBBY + 500),

	/* pk combo, Juver, 2021/02/09 */
	NET_MSG_LOBBY_CHAR_INFO_PK_COMBO_COUNT	= (NET_MSG_LOBBY + 501),

	NET_MSG_LOBBY_CHAR_INFO_COMBAT_RECORD	= (NET_MSG_LOBBY + 502),

	/* user account page, Juver, 2020/11/05 */
	NET_MSG_USER_ACCOUNT_PAGE_INIT			= (NET_MSG_LOBBY + 520),

	/* login device check, Juver, 2020/11/06 */
	NET_MSG_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET		= (NET_MSG_LOBBY + 525),
	NET_MSG_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB		= (NET_MSG_LOBBY + 526),
	NET_MSG_USER_ACCOUNT_PAGE_LOGIN_DEVICE_CHECK_SET_FB2	= (NET_MSG_LOBBY + 527),

	NET_MSG_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_INIT				= (NET_MSG_LOBBY + 530),
	NET_MSG_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION			= (NET_MSG_LOBBY + 531),
	NET_MSG_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB		= (NET_MSG_LOBBY + 532),
	NET_MSG_USER_LOGIN_DEVICE_CHECK_VERIFY_REQUEST_ACTION_FB2		= (NET_MSG_LOBBY + 533),
	
	NET_MSG_LOBBY_CHAR_DAILYLOGIN_DATA								= (NET_MSG_LOBBY + 534),

	/* Codex, Archie 02/16/24 */
	NET_MSG_LOBBY_CHAR_CODEX_PROG									= (NET_MSG_LOBBY + 535),
	NET_MSG_LOBBY_CHAR_CODEX_DONE									= (NET_MSG_LOBBY + 536),

		/*12-9-14, Battle Pass - CNDev*/
	NET_MSG_LOBBY_CHAR_BATTLEPASS_PROG								= (NET_MSG_LOBBY + 537),
	NET_MSG_LOBBY_CHAR_BATTLEPASS_DONE								= (NET_MSG_LOBBY + 538),
	NET_MSG_LOBBY_CHAR_BATTLEPASS_REWARD_DATA						= (NET_MSG_LOBBY + 539),

	/*dmk14 ingame web*/
	NET_MSG_FORGOTPASS					= (NET_MSG_LOBBY + 540),
	NET_MSG_FORGOTPASS_FB				= (NET_MSG_LOBBY + 541),
	NET_MSG_CHANGEPASS					= (NET_MSG_LOBBY + 542),
	NET_MSG_CHANGEPASS_FB				= (NET_MSG_LOBBY + 543),
	NET_MSG_SELCHAR_CHANGEPASS			= (NET_MSG_LOBBY + 544),
	NET_MSG_SELCHAR_CHANGEPASS_FB		= (NET_MSG_LOBBY + 545),
	NET_MSG_SELCHAR_CHANGEPIN			= (NET_MSG_LOBBY + 546),
	NET_MSG_SELCHAR_CHANGEPIN_FB		= (NET_MSG_LOBBY + 547),
	NET_MSG_REQ_GAMETIME_CONVERT		= (NET_MSG_LOBBY + 548),
	NET_MSG_REQ_GAMETIME_CONVERT_FB		= (NET_MSG_LOBBY + 549),
	NET_MSG_SELCHAR_RESETPIN			= (NET_MSG_LOBBY + 550),
	NET_MSG_SELCHAR_RESETPIN_FB			= (NET_MSG_LOBBY + 551),
	NET_MSG_SELCHAR_CHANGEMAIL			= (NET_MSG_LOBBY + 552),
	NET_MSG_SELCHAR_CHANGEMAIL_FB		= (NET_MSG_LOBBY + 553),
	NET_MSG_SELCHAR_TOPUP				= (NET_MSG_LOBBY + 554),
	NET_MSG_SELCHAR_TOPUP_FB			= (NET_MSG_LOBBY + 555),
	//<-----------------------------------------------------------------------------------------------------[GAME CONTRL]

	NET_MSG_COMBINE					= (NET_MSG_GCTRL +  10), //< 병합 메시지. ( 필드 서버 -> 클라이언트 )

	///////////////////////////////////////////////////////////////////////////
	// nProtect
	NET_MSG_GAMEGUARD_AUTH          = (NET_MSG_GCTRL +  20), //< nProtect GameGuard Auth 요청 메시지
	NET_MSG_GAMEGUARD_ANSWER        = (NET_MSG_GCTRL +  21), //< nProtect GameGuard Auth Answer 메시지
	NET_MSG_GAMEGUARD_AUTH_1        = (NET_MSG_GCTRL +  22), //< nProtect GameGuard Auth 요청 메시지
	NET_MSG_GAMEGUARD_ANSWER_1      = (NET_MSG_GCTRL +  23), //< nProtect GameGuard Auth Answer 메시지
	NET_MSG_GAMEGUARD_AUTH_2        = (NET_MSG_GCTRL +  24), //< nProtect GameGuard Auth 요청 메시지
	NET_MSG_GAMEGUARD_ANSWER_2      = (NET_MSG_GCTRL +  25), //< nProtect GameGuard Auth Answer 메시지

	// Apex 적용
	NET_MSG_APEX_DATA				= (NET_MSG_GCTRL +	30), //< Apex Data 메세지
	NET_MSG_APEX_ANSWER				= (NET_MSG_GCTRL +	31), //< Apex Answer 메세지
	//	NET_MSG_APEX_RSAKEY				= (NET_MSG_GCTRL +	32), //< Apex RSA Key 메세지
	// Apex 적용( 홍콩 )
	NET_MSG_APEX_RETURN 			= (NET_MSG_GCTRL +	33), //< Apex Client Return 

	NET_MSG_REQ_FRIENDLIST			= (NET_MSG_GCTRL +  50), //< 친구 목록 전송 요청.
	NET_MSG_REQ_FRIENDADD			= (NET_MSG_GCTRL +  51), //< 친구 추가 요청.
	NET_MSG_REQ_FRIENDADD_LURE		= (NET_MSG_GCTRL +  52), //< 친구 추가 문의.
	NET_MSG_REQ_FRIENDADD_ANS		= (NET_MSG_GCTRL +  53), //< 친구 추가 수락 여부 회신.
	NET_MSG_REQ_FRIENDADD_FB		= (NET_MSG_GCTRL +  54), //< 친구 추가 요청 결과.
	NET_MSG_REQ_FRIENDDEL			= (NET_MSG_GCTRL +  55), //< 친구 삭제 요청.
	NET_MSG_REQ_FRIENDDEL_FB		= (NET_MSG_GCTRL +  56), //< 친구 삭제 결과.

	NET_MSG_FRIENDINFO				= (NET_MSG_GCTRL +  57), //< 친구 정보.
	NET_MSG_FRIENDSTATE				= (NET_MSG_GCTRL +  58), //< 친구 상태.

	NET_MSG_REQ_FRIENDBLOCK			= (NET_MSG_GCTRL +  59), //< 귓속말 블럭 요청.
	NET_MSG_REQ_FRIENDBLOCK_FB		= (NET_MSG_GCTRL +  60), //< 귓속말 블럭 요청 결과.

	NET_MSG_CHAT					= (NET_MSG_GCTRL + 100), //< 채팅메시지 정의 ( 클라이언트->게임서버 )
	NET_MSG_CHAT_FB					= (NET_MSG_GCTRL + 101), //< 채팅메시지 정의 ( 게임서버->클라이언트 )
	NET_MSG_CHAT_PRIVATE_FAIL		= (NET_MSG_GCTRL + 102), //< 채팅, 귓속말 보내기에 실패하였을 경우. ( 게임서버->클라이언트 )	

	NET_MSG_CHAT_LOUDSPEAKER		= (NET_MSG_GCTRL + 103), //< 확성기 메시시.
	NET_MSG_CHAT_LOUDSPEAKER_AGT	= (NET_MSG_GCTRL + 104), //< 확성기 메시지 agt 에 보냄.
	NET_MSG_CHAT_LOUDSPEAKER_FB		= (NET_MSG_GCTRL + 105), //< 확성기 메시지 처리 FB.

	NET_MSG_CHAT_BLOCK				= (NET_MSG_GCTRL + 106), //< 채팅 차단 상태일 경우. ( 게임서버 -> 클라이언트 )

	NET_MSG_FIELDSVR_CHARCHK		= (NET_MSG_GCTRL + 110), //< 필드서버에 케릭터 진입전에 잔존 CHAR 제거 위한 메시지 전송.
	NET_MSG_FIELDSVR_CHARCHK_FB		= (NET_MSG_GCTRL + 111), //< 필드서버에 케릭터 진입전에 잔존 CHAR 제거 위한 메시지 전송.

	NET_MSG_GCTRL_FIELDSVR_OUT		= (NET_MSG_GCTRL + 115), //< 필드서버에서 나오기 위한 예비 처리를 부탁. ( 필드서버 이동시. )
	NET_MSG_GCTRL_FIELDSVR_OUT_FB	= (NET_MSG_GCTRL + 116), //< 필드서버에서 나오기 위한 예비 처리 결과. ( 필드서버 이동시. )

	NET_MSG_GCTRL_REBIRTH_OUT		= (NET_MSG_GCTRL + 117), //< 다른 필드서버에서 부활하기 위한 예비 처리 부탁.
	NET_MSG_GCTRL_REBIRTH_OUT_FB	= (NET_MSG_GCTRL + 118), //< 다른 필드서버에서 부활하기 위한 예비 처리 결과.

	NET_MSG_GCTRL_DROP_ITEM			= (NET_MSG_GCTRL + 120),
	NET_MSG_GCTRL_DROP_MONEY		= (NET_MSG_GCTRL + 121),

	NET_MSG_GCTRL_DROP_PC			= (NET_MSG_GCTRL + 123),
	NET_MSG_GCTRL_DROP_CROW			= (NET_MSG_GCTRL + 124),
	NET_MSG_GCTRL_DROP_MATERIAL		= (NET_MSG_GCTRL + 125),

	NET_MSG_GCTRL_DROP_OUT			= (NET_MSG_GCTRL + 127),
	NET_MSG_GCTRL_DROP_OUT_FORCED	= (NET_MSG_GCTRL + 128),

	NET_MSG_GCTRL_REQ_GATEOUT_REQ	= (NET_MSG_GCTRL + 130),
	NET_MSG_GCTRL_REQ_GATEOUT		= (NET_MSG_GCTRL + 131),
	NET_MSG_GCTRL_REQ_GATEOUT_FB	= (NET_MSG_GCTRL + 132),
	NET_MSG_GCTRL_REQ_LANDIN		= (NET_MSG_GCTRL + 133),

	NET_MSG_GCTRL_REQ_READY			= (NET_MSG_GCTRL + 134),

	NET_MSG_GCTRL_LAND_INFO			= (NET_MSG_GCTRL + 135),
	NET_MSG_GCTRL_SERVER_INFO		= (NET_MSG_GCTRL + 136),
	NET_MSG_GCTRL_SCHOOLFREEPK_FLD	= (NET_MSG_GCTRL + 137),

	NET_MSG_GCTRL_ACTSTATE			= (NET_MSG_GCTRL + 139),
	NET_MSG_GCTRL_MOVESTATE			= (NET_MSG_GCTRL + 140),
	NET_MSG_GCTRL_MOVESTATE_BRD		= (NET_MSG_GCTRL + 141),

	NET_MSG_GCTRL_GOTO				= (NET_MSG_GCTRL + 142),
	NET_MSG_GCTRL_GOTO_BRD			= (NET_MSG_GCTRL + 143),

	NET_MSG_GCTRL_ATTACK			= (NET_MSG_GCTRL + 144),
	NET_MSG_GCTRL_ATTACK_BRD		= (NET_MSG_GCTRL + 145),
	NET_MSG_GCTRL_ATTACK_CANCEL		= (NET_MSG_GCTRL + 146),
	NET_MSG_GCTRL_ATTACK_CANCEL_BRD	= (NET_MSG_GCTRL + 147),
	NET_MSG_GCTRL_ATTACK_AVOID		= (NET_MSG_GCTRL + 149),
	NET_MSG_GCTRL_ATTACK_AVOID_BRD	= (NET_MSG_GCTRL + 150),
	NET_MSG_GCTRL_ATTACK_DAMAGE		= (NET_MSG_GCTRL + 151),
	NET_MSG_GCTRL_ATTACK_DAMAGE_BRD	= (NET_MSG_GCTRL + 152),

	NET_MSG_GCTRL_ACTION_BRD		= (NET_MSG_GCTRL + 153),	//<	행동 변화.
	NET_MSG_GCTRL_UPDATE_STATE		= (NET_MSG_GCTRL + 154),	//<	상태 변화 업대이트.
	NET_MSG_GCTRL_UPDATE_EXP		= (NET_MSG_GCTRL + 155),	//<	경험치 변화시 발생.
	NET_MSG_GCTRL_UPDATE_MONEY		= (NET_MSG_GCTRL + 156),	//<	경험치 변화시 발생.

	NET_MSG_GCTRL_UPDATE_SP			= (NET_MSG_GCTRL + 157),	//<	SP 변화시 발생.
	NET_MSG_GCTRL_UPDATE_LP			= (NET_MSG_GCTRL + 158),	//<	Living Point 변화시 발생.
	NET_MSG_GCTRL_UPDATE_SKP		= (NET_MSG_GCTRL + 160),	//<	스킬 포인트 변화시 발생.

	NET_MSG_GCTRL_UPDATE_STATE_BRD	= (NET_MSG_GCTRL + 161),	//<	상태 변화 업대이트.
	NET_MSG_GCTRL_UPDATE_PASSIVE_BRD= (NET_MSG_GCTRL + 162),	//<	passive skill 상태 변화 업대이트.

	NET_MSG_GCTRL_POSITIONCHK_BRD	= (NET_MSG_GCTRL + 163),	//<	서버에서의 위치를 클라이언트에서 체크하기 위해서.

	NET_MSG_GCTRL_UPDATE_BRIGHT		= (NET_MSG_GCTRL + 164),	//<	bright(속성)치 업대이트.
	NET_MSG_GCTRL_UPDATE_BRIGHT_BRD	= (NET_MSG_GCTRL + 165),	//<	bright(속성)치 업대이트.
	NET_MSG_GCTRL_UPDATE_STATS		= (NET_MSG_GCTRL + 166),	//<	stats 업대이트.

	NET_MSG_GCTRL_UPDATE_FLAGS		= (NET_MSG_GCTRL + 167),	//<	flags 업대이트.

	NET_MSG_GCTRL_PUSHPULL_BRD		= (NET_MSG_GCTRL + 171),	//<	밀거나 당기는 액션 발생시.
	NET_MSG_GCTRL_SKILL_PUSH_BRD	= (NET_MSG_GCTRL + 172),	/* push skill logic, Juver, 2020/12/14 */
	NET_MSG_GCTRL_SKILL_PULL_BRD	= (NET_MSG_GCTRL + 173),	/* pull skill logic, Juver, 2020/12/14 */
	NET_MSG_GCTRL_SKILL_DASH_BRD	= (NET_MSG_GCTRL + 174),	/* dash skill logic, Juver, 2020/12/14 */
	NET_MSG_GCTRL_SKILL_POSITION_SHIFT_BRD	= (NET_MSG_GCTRL + 175),	/* skill position shift, Juver, 2020/12/15 */


	NET_MSG_GCTRL_JUMP_POS_BRD		= (NET_MSG_GCTRL + 182),	//<	케릭터의 위치가 순간적으로 변경되었을 경우. 클라이언트에 반영.

	NET_MSG_GCTRL_INVEN_INSERT		= (NET_MSG_GCTRL + 200),	//<	인밴에 아이탬 삽입됨.
	NET_MSG_GCTRL_INVEN_DELETE		= (NET_MSG_GCTRL + 201),	//<	인밴   아이탬 삭제.
	NET_MSG_GCTRL_INVEN_DEL_INSERT	= (NET_MSG_GCTRL + 202),	//<	인밴에 있던 아이탬 삭제후 아이탬 삽입.

	NET_MSG_GCTRL_PUTON_RELEASE		= (NET_MSG_GCTRL + 203),	//<	슬롯의 아이탬을 제거함.
	NET_MSG_GCTRL_PUTON_RELEASE_BRD	= (NET_MSG_GCTRL + 204),	//<	슬롯의 아이탬을 제거함.
	NET_MSG_GCTRL_PUTON_UPDATE		= (NET_MSG_GCTRL + 205),	//<	슬롯에 아이탬 변경.
	NET_MSG_GCTRL_PUTON_UPDATE_BRD	= (NET_MSG_GCTRL + 206),	//<	[방송] 사용 아이탬이 변경됨.

	NET_MSG_GCTRL_INVEN_DRUG_UPDATE	= (NET_MSG_GCTRL + 207),	//<	소모형 아이탬 소모됨을 업대이트.
	NET_MSG_GCTRL_PUTON_DRUG_UPDATE	= (NET_MSG_GCTRL + 208),	//<	소모형 아이탬 소모됨을 업대이트.

	NET_MSG_GCTRL_INVEN_ITEM_UPDATE	= (NET_MSG_GCTRL + 209),	//<	인밴 아이템 수치 업데이트.

	NET_MSG_GCTRL_PICKUP_MONEY		= (NET_MSG_GCTRL + 221),	//<	돈 습득.
	NET_MSG_GCTRL_PICKUP_ITEM		= (NET_MSG_GCTRL + 222),	//<	아이템 습득 알림.

	NET_MSG_GCTRL_2_FRIEND_REQ		= (NET_MSG_GCTRL + 231),	//<	친구로 가기 요청.
	NET_MSG_GCTRL_2_FRIEND_CK		= (NET_MSG_GCTRL + 232),	//<	친구로 가기 친구가 있는지 검사후 위치 반환.
	NET_MSG_GCTRL_2_FRIEND_AG		= (NET_MSG_GCTRL + 233),	//<	친구로 가기 AG로 회신 ( 친구 위치 정보 ).
	NET_MSG_GCTRL_2_FRIEND_FLD		= (NET_MSG_GCTRL + 234),	//<	친구로 가기 FLD로 이동 명령.
	NET_MSG_GCTRL_2_FRIEND_FB		= (NET_MSG_GCTRL + 235),	//<	친구로 가기 처리 결과 통보.

	/*dmk14 fast teleport fix*/
	NET_MSG_GCTRL_2_FRIEND_REQ_CANCEL = (NET_MSG_GCTRL + 236),

	NET_MSG_GCTRL_REQ_TAKE_FB		= (NET_MSG_GCTRL + 300),	//<	줍기 시도 결과.
	NET_MSG_GCTRL_REQ_FIELD_TO_INVEN= (NET_MSG_GCTRL + 301),	//<	아이탬,돈 줍기 시도.
	NET_MSG_GCTRL_REQ_FIELD_TO_HOLD	= (NET_MSG_GCTRL + 302),	//<	Field에 있는 아이탬을 들기 시도.

	NET_MSG_GCTRL_REQ_INVEN_TO_HOLD	= (NET_MSG_GCTRL + 303),	//<	인밴에 있는 아이탬을 들기 시도.
	NET_MSG_GCTRL_REQ_INVEN_EX_HOLD	= (NET_MSG_GCTRL + 304),	//<	인밴에 있는 아이탬을 들기 시도.

	NET_MSG_GCTRL_REQ_SLOT_TO_HOLD	= (NET_MSG_GCTRL + 305),	//<	아이탬, 슬롯에서 손으로.
	NET_MSG_GCTRL_REQ_SLOT_EX_HOLD	= (NET_MSG_GCTRL + 306),	//<	아이탬, 슬롯에 있는것과 손에 들고 있는것 교환.

	NET_MSG_GCTRL_REQ_HOLD_TO_FIELD	= (NET_MSG_GCTRL + 307),	//<	아이탬을 바닥에 떨어트림.
	NET_MSG_GCTRL_REQ_HOLD_TO_INVEN	= (NET_MSG_GCTRL + 308),	//<	아이탬을 INVEN에 넣음.
	NET_MSG_GCTRL_REQ_HOLD_TO_SLOT	= (NET_MSG_GCTRL + 309),	//<	아이탬, 손에서 슬롯으로.

	NET_MSG_GCTRL_REQ_HOLD_FB		= (NET_MSG_GCTRL + 310),	//<	인밴에 있는 아이탬을 들기 시도시 결과 피드백.
	NET_MSG_GCTRL_REQ_INVEN_TO_SLOT	= (NET_MSG_GCTRL + 311),	//<	인밴에 있는 아이탬을 슬롯에 넣기.

	NET_MSG_GCTRL_REQ_MONEY_TO_FIELD= (NET_MSG_GCTRL + 312),	//<	돈을 필드에 버리기.
	NET_MSG_GCTRL_REQ_INVEN_SPLIT	= (NET_MSG_GCTRL + 313),	//<	인벤에 있는 겹침 아이템 분리.

	NET_MSG_GCTRL_PUTON_CHANGE		= (NET_MSG_GCTRL + 314),	//<	무기 스왑.
	NET_MSG_GCTRL_PUTON_CHANGE_BRD	= (NET_MSG_GCTRL + 315),	//<	[방송] 무기 스왑 알림.
	NET_MSG_GCTRL_PUTON_CHANGE_AG	= (NET_MSG_GCTRL + 316),	//<	Agent로 무기 스왑 알림.
	NET_MSG_GCTRL_REQ_SLOT_CHANGE	= (NET_MSG_GCTRL + 317),	//<	무기 슬롯 체인지.

	NET_MSG_GCTRL_REQ_BUY_FROM_NPC	= (NET_MSG_GCTRL + 350),	//<	NPC에게 물품 판매 시도.
	NET_MSG_GCTRL_REQ_SALE_TO_NPC	= (NET_MSG_GCTRL + 351),	//<	NPC에게 물품 판매 시도.

	NET_MSG_GCTRL_REQ_REBIRTH		= (NET_MSG_GCTRL + 380),	//<	부활 요청.
	NET_MSG_GCTRL_REQ_REBIRTH_FB	= (NET_MSG_GCTRL + 381),	//<	부활 요청 응답.

	NET_MSG_GCTRL_REQ_LEVELUP		= (NET_MSG_GCTRL + 382),	//<	랩업 요청.
	NET_MSG_GCTRL_REQ_LEVELUP_FB	= (NET_MSG_GCTRL + 383),	//<	랩업 요청 응답.
	NET_MSG_GCTRL_REQ_LEVELUP_BRD	= (NET_MSG_GCTRL + 384),	//<	랩업 효과.

	NET_MSG_GCTRL_REQ_STATSUP		= (NET_MSG_GCTRL + 385),	//<	STATS UP
	NET_MSG_GCTRL_REQ_STATSUP_FB	= (NET_MSG_GCTRL + 386),	//<	STATS UP 응답.

	NET_MSG_GCTRL_REQ_INVENDRUG		= (NET_MSG_GCTRL + 400),	//<	인밴 약품 먹기.
	NET_MSG_GCTRL_REQ_LEARNSKILL	= (NET_MSG_GCTRL + 401),	//<	인밴 스킬 습득 요청.
	NET_MSG_GCTRL_REQ_LEARNSKILL_FB	= (NET_MSG_GCTRL + 402),	//<	인밴 스킬 습득 응답.
	NET_MSG_GCTRL_REQ_SKILLUP		= (NET_MSG_GCTRL + 403),	//<	스킬 업그래이드 요청.
	NET_MSG_GCTRL_REQ_SKILLUP_FB	= (NET_MSG_GCTRL + 404),	//<	스킬 업그래이드 응답.

	NET_MSG_GCTRL_REQ_RECALL		= (NET_MSG_GCTRL + 405),	//<	귀환 아이템 사용 요청.
	NET_MSG_GCTRL_REQ_RECALL_FB		= (NET_MSG_GCTRL + 406),	//<	귀환 아이템 사용 응답.
	NET_MSG_GCTRL_REQ_RECALL_AG		= (NET_MSG_GCTRL + 407),	//<	귀환을 에이젼트 서버에 요청.

	NET_MSG_GCTRL_REQ_BUS			= (NET_MSG_GCTRL + 408),	//<	버스 승차 요청.
	NET_MSG_GCTRL_REQ_BUS_FB		= (NET_MSG_GCTRL + 409),	//<	버스 승차 요청 응답.

	NET_MSG_GCTRL_SKILL_EXPIRE_FB	= (NET_MSG_GCTRL + 410),

	NET_MSG_GCTRL_REQ_SKILL			= (NET_MSG_GCTRL + 411),	//<	스킬 발동 요청.
	NET_MSG_GCTRL_REQ_SKILL_FB		= (NET_MSG_GCTRL + 412),	//<	스킬 발동 요청 응답.
	NET_MSG_GCTRL_REQ_SKILL_BRD		= (NET_MSG_GCTRL + 413),	//<	스킬 발동.
	NET_MSG_GCTRL_SKILLFACT_BRD		= (NET_MSG_GCTRL + 414),	//<	스킬 적용시 체력 변화.
	NET_MSG_GCTRL_SKILLHOLD_BRD		= (NET_MSG_GCTRL + 415),	//<	스킬 적용시 지속 효과.
	NET_MSG_GCTRL_SKILLCONSUME_FB	= (NET_MSG_GCTRL + 416),	//<	스킬 발동후 소모되는 수치값.

	NET_MSG_GCTRL_SKILL_CANCEL		= (NET_MSG_GCTRL + 417),	//<	스킬 취소.
	NET_MSG_GCTRL_SKILL_CANCEL_BRD	= (NET_MSG_GCTRL + 418),	//<	스킬 취소 클라이언트에 반영.

	NET_MSG_GCTRL_SKILLHOLD_RS_BRD	= (NET_MSG_GCTRL + 419),	//<	스킬 지속 효과 제거.
	NET_MSG_GCTRL_SKILLHOLDEX_BRD	= (NET_MSG_GCTRL + 420),	//<	스킬 적용시 지속 효과.

	NET_MSG_GCTRL_STATEBLOW_BRD		= (NET_MSG_GCTRL + 426),	//<	상태이상.
	NET_MSG_GCTRL_CURESTATEBLOW_BRD	= (NET_MSG_GCTRL + 427),	//<	상태이상.
	NET_MSG_GCTRL_LANDEFFECT		= (NET_MSG_GCTRL + 428),	//< 지형에 관련된 상태 이상

	NET_MSG_GCTRL_REQ_SKILLQ_ACTIVE	= (NET_MSG_GCTRL + 430),	//<	스킬 퀵슬롯의 액티브 스킬.
	NET_MSG_GCTRL_REQ_SKILLQ_SET	= (NET_MSG_GCTRL + 431),	//<	스킬 퀵슬롯 지정 요청.
	NET_MSG_GCTRL_REQ_SKILLQ_RESET	= (NET_MSG_GCTRL + 432),	//<	스킬 퀵슬롯 해지 요청.
	NET_MSG_GCTRL_REQ_SKILLQ_FB		= (NET_MSG_GCTRL + 433),	//<	스킬 퀵슬롯 요청에 따른 변경사항 클라이언트에 통지.

	NET_MSG_GCTRL_REQ_ACTIONQ_SET	= (NET_MSG_GCTRL + 435),	//<	엑션슬롯 지정 요청.
	NET_MSG_GCTRL_REQ_ACTIONQ_RESET	= (NET_MSG_GCTRL + 436),	//<	엑션슬롯 해지 요청.
	NET_MSG_GCTRL_REQ_ACTIONQ_FB	= (NET_MSG_GCTRL + 437),	//<	엑션슬롯 요청에 따른 변경사항 클라이언트에 통지.

	NET_MSG_GCTRL_REQ_GETSTORAGE		= (NET_MSG_GCTRL + 441),	//<	창고 정보 가져오기.
	NET_MSG_GCTRL_REQ_GETSTORAGE_FB		= (NET_MSG_GCTRL + 442),	//<	창고 정보 가져오기.
	NET_MSG_GCTRL_REQ_GETSTORAGE_ITEM	= (NET_MSG_GCTRL + 443),	//<	창고 정보 가져오기.

	NET_MSG_GCTRL_REQ_STORAGEDRUG		= (NET_MSG_GCTRL + 444),	//<	창고 약품 먹기.
	NET_MSG_GCTRL_REQ_STORAGESKILL		= (NET_MSG_GCTRL + 445),	//<	창고 스킬 먹기.

	NET_MSG_GCTRL_REQ_STORAGE_TO_HOLD	= (NET_MSG_GCTRL + 446),	//<	창고에 있는 아이탬을 들기 시도.
	NET_MSG_GCTRL_REQ_STORAGE_EX_HOLD	= (NET_MSG_GCTRL + 447),	//<	창고에 있는 아이탬을 들기 시도.
	NET_MSG_GCTRL_REQ_HOLD_TO_STORAGE	= (NET_MSG_GCTRL + 448),	//<	아이탬을 창고에 넣음.

	NET_MSG_GCTRL_STORAGE_INSERT		= (NET_MSG_GCTRL + 449),	//<	창고에 아이탬 삽입됨.
	NET_MSG_GCTRL_STORAGE_DELETE		= (NET_MSG_GCTRL + 450),	//<	창고 아이탬 삭제.
	NET_MSG_GCTRL_STORAGE_DEL_INSERT	= (NET_MSG_GCTRL + 451),	//<	창고 아이탬 삭제 및 삽입.
	NET_MSG_GCTRL_STORAGE_DRUG_UPDATE	= (NET_MSG_GCTRL + 452),	//<	창고 소모품 사용횟수 파악.

	NET_MSG_GCTRL_STORAGE_SAVE_MONEY	= (NET_MSG_GCTRL + 453),	//<	창고에 돈을 저장.
	NET_MSG_GCTRL_STORAGE_DRAW_MONEY	= (NET_MSG_GCTRL + 454),	//<	창고에서 돈을 빼냄.
	NET_MSG_GCTRL_STORAGE_UPDATE_MONEY	= (NET_MSG_GCTRL + 455),	//<	창고의 돈 액수 변화.

	NET_MSG_GCTRL_REQ_STORAGE_SPLIT		= (NET_MSG_GCTRL + 456),	//<	창고에 있는 겹침 아이템 분리.
	NET_MSG_GCTRL_STORAGE_STATE			= (NET_MSG_GCTRL + 457),	//<	창고 사용가능 속성.
	NET_MSG_GCTRL_PREMIUM_STATE			= (NET_MSG_GCTRL + 458),	//<	프리미엄 속성.

	NET_MSG_GCTRL_PARTY_LURE			= (NET_MSG_GCTRL + 460),	//<	파티 가입 요청.			클라이언트 -> 서버.
	NET_MSG_GCTRL_PARTY_LURE_FB			= (NET_MSG_GCTRL + 461),	//<	파티 가입 요청의 결과.	클라이언트 -> 서버.
	NET_MSG_GCTRL_PARTY_LURE_TAR		= (NET_MSG_GCTRL + 462),	//<	파티 가입 요청.			서버 -> 클라이언트.
	NET_MSG_GCTRL_PARTY_LURE_TAR_ANS	= (NET_MSG_GCTRL + 463),	//<	파티 가입 요청 응답.	클라이언트 -> 서버.

	NET_MSG_GCTRL_PARTY_SECEDE			= (NET_MSG_GCTRL + 464),	//<	파티 탈퇴 요청.

	NET_MSG_GCTRL_PARTY_ADD				= (NET_MSG_GCTRL + 465),	//<	파티원 추가.
	NET_MSG_GCTRL_PARTY_ADD_FB			= (NET_MSG_GCTRL + 466),	//<	파티원 추가 FB.

	NET_MSG_GCTRL_PARTY_DEL				= (NET_MSG_GCTRL + 467),	//<	파티원 제거.
	NET_MSG_GCTRL_PARTY_DEL_FB			= (NET_MSG_GCTRL + 468),	//<	파티원 제거 FB

	NET_MSG_GCTRL_PARTY_AUTHORITY		= (NET_MSG_GCTRL + 469),	//< 파티장 위임

	NET_MSG_GCTRL_PARTY_DISSOLVE		= (NET_MSG_GCTRL + 470),	//<	파티 해체.

	NET_MSG_GCTRL_PARTY_FNEW			= (NET_MSG_GCTRL + 471),	//<	파티 생성. (field).
	NET_MSG_GCTRL_PARTY_MBR_DETAIL		= (NET_MSG_GCTRL + 472),	//<	파티원의 세부 정보.
	NET_MSG_GCTRL_PARTY_MBR_MOVEMAP		= (NET_MSG_GCTRL + 473),	//<	파티원의 map 이동.
	NET_MSG_GCTRL_PARTY_MBR_POINT		= (NET_MSG_GCTRL + 474),	//<	파티원의 POINT 변화.
	NET_MSG_GCTRL_PARTY_MBR_POS			= (NET_MSG_GCTRL + 475),	//<	파티원의 위치 변화.
	NET_MSG_GCTRL_PARTY_MBR_PICKUP_BRD	= (NET_MSG_GCTRL + 476),	//<	파티원의 아이템 습득을 알림.

	NET_MSG_GCTRL_PARTY_BRD				= (NET_MSG_GCTRL + 477),	//<	케릭터의 파티 소속 변화를 주변 클라이언트들에 알림.
	NET_MSG_GCTRL_PARTY_MASTER_RENEW	= (NET_MSG_GCTRL + 478),	//< 파티마스터 재설정.

	NET_MSG_GCTRL_TRADE					= (NET_MSG_GCTRL + 480),	//<	트래이드 요청. ( 클라이언트1 -> 서버 )
	NET_MSG_GCTRL_TRADE_FB				= (NET_MSG_GCTRL + 481),	//<	트래이드 요청 응답. ( 서버 -> 클라이언트1 )
	NET_MSG_GCTRL_TRADE_TAR				= (NET_MSG_GCTRL + 482),	//<	트래이드 상대에게 요청. ( 서버 -> 클라이언트2 )
	NET_MSG_GCTRL_TRADE_TAR_ANS			= (NET_MSG_GCTRL + 483),	//<	트래이드 상대의 응답. ( 클라이언트2 -> 서버 )

	NET_MSG_GCTRL_TRADE_AGREE			= (NET_MSG_GCTRL + 484),	//<	트래이드 승인 지정.

	NET_MSG_GCTRL_TRADE_AGREE_TAR		= (NET_MSG_GCTRL + 486),	//<	트래이드 승인 지정 상대방에게 알림. ( 상대방에게. )

	NET_MSG_GCTRL_TRADE_ITEM_REGIST		= (NET_MSG_GCTRL + 487),	//<	트래이드할 아이템 등록.
	NET_MSG_GCTRL_TRADE_ITEM_REGIST_TAR	= (NET_MSG_GCTRL + 488),	//<	트래이드할 아이템 등록. ( 상대방에게. )
	NET_MSG_GCTRL_TRADE_ITEM_REMOVE		= (NET_MSG_GCTRL + 489),	//<	트래이드할 아이템 목록에서 제거.
	NET_MSG_GCTRL_TRADE_ITEM_REMOVE_TAR	= (NET_MSG_GCTRL + 490),	//<	트래이드할 아이템 목록에서 제거. ( 상대방에게. )

	NET_MSG_GCTRL_TRADE_MONEY			= (NET_MSG_GCTRL + 491),	//<	트래이드할 금액.
	NET_MSG_GCTRL_TRADE_MONEY_TAR		= (NET_MSG_GCTRL + 492),	//<	트래이드할 금액을 상대방에게 알림.

	NET_MSG_GCTRL_TRADE_COMPLETE_TAR	= (NET_MSG_GCTRL + 493),	//<	트래이드 완료.

	NET_MSG_GCTRL_TRADE_CANCEL			= (NET_MSG_GCTRL + 494),	//<	트래이드 취소.
	NET_MSG_GCTRL_TRADE_CANCEL_TAR		= (NET_MSG_GCTRL + 495),	//<	트래이드 취소. ( 상대방에게. )

	NET_MSG_GCTRL_INVEN_BOXOPEN			= (NET_MSG_GCTRL + 496),	//<	box open 요청.
	NET_MSG_GCTRL_INVEN_BOXOPEN_FB		= (NET_MSG_GCTRL + 497),	//<	box open 요청 FB.

	NET_MSG_GCTRL_INVEN_DISGUISE		= (NET_MSG_GCTRL + 498),	//<	복장에 코스춤을 덧씌움.
	NET_MSG_GCTRL_INVEN_DISGUISE_FB		= (NET_MSG_GCTRL + 499),	//<	복장에 코스춤을 덧씌움 FB.

	//Item Card Extend Costume Expiration
	NET_MSG_GCTRL_INVEN_DISGUISE_EXTEND = (NET_MSG_GCTRL + 500),

	NET_MSG_GCTRL_INVEN_GRINDING		= (NET_MSG_GCTRL + 501),	//<	INVEN 아이템 연마 시도.
	NET_MSG_GCTRL_INVEN_GRINDING_FB		= (NET_MSG_GCTRL + 502),	//<	INVEN 아이템 연마 시도.

	NET_MSG_GCTRL_REGEN_GATE			= (NET_MSG_GCTRL + 503),	//<	부활 위치 지정.
	NET_MSG_GCTRL_REGEN_GATE_FB			= (NET_MSG_GCTRL + 504),	//<	부활 위치 지정 FB.

	NET_MSG_GCTRL_CURE					= (NET_MSG_GCTRL + 505),	//<	치료 요청.
	NET_MSG_GCTRL_CURE_FB				= (NET_MSG_GCTRL + 506),	//<	치료 요청 FB.

	NET_MSG_GCTRL_CHARRESET				= (NET_MSG_GCTRL + 507),	//<	케릭터 정보 리셋 요청. ( stats, skill )
	NET_MSG_GCTRL_CHARRESET_FB			= (NET_MSG_GCTRL + 508),	//<	케릭터 정보 리셋 요청 FB. ( stats, skill )

	NET_MSG_GCTRL_NPC_ITEM_TRADE		= (NET_MSG_GCTRL + 509),	//<	아이템 교환 요청.
	NET_MSG_GCTRL_NPC_ITEM_TRADE_FB		= (NET_MSG_GCTRL + 510),	//<	아이템 교환 요청 FB.

	NET_MSG_GCTRL_CONFRONT				= (NET_MSG_GCTRL + 511),	//<	대련 요청 to (서버)
	NET_MSG_GCTRL_CONFRONT_TAR			= (NET_MSG_GCTRL + 512),	//<	대련 요청 to (상대방)
	NET_MSG_GCTRL_CONFRONT_ANS			= (NET_MSG_GCTRL + 513),	//<	대련 요청 응답 to (서버)
	NET_MSG_GCTRL_CONFRONT_FB			= (NET_MSG_GCTRL + 514),	//<	대련 요청 응답 to (요청자)

	NET_MSG_GCTRL_CONFRONT_START2_FLD	= (NET_MSG_GCTRL + 515),	//<	대련 시작 to (필드)
	NET_MSG_GCTRL_CONFRONT_START2_CLT	= (NET_MSG_GCTRL + 516),	//<	대련 시작 to (클라이언트)
	NET_MSG_GCTRL_CONFRONT_FIGHT2_CLT	= (NET_MSG_GCTRL + 517),	//<	대련 FIGHT to (클라이언트)

	NET_MSG_GCTRL_CONFRONT_END2_FLD		= (NET_MSG_GCTRL + 518),	//<	대련 종료 to (필드).
	NET_MSG_GCTRL_CONFRONT_END2_CLT		= (NET_MSG_GCTRL + 519),	//<	대련 종료 to (클라이언트).
	NET_MSG_GCTRL_CONFRONT_END2_CLT_BRD	= (NET_MSG_GCTRL + 520),	//<	대련 종료 to (주변 클라이언트).
	NET_MSG_GCTRL_CONFRONT_END2_AGT		= (NET_MSG_GCTRL + 521),	//<	대련 종료 to (에이전트).
	NET_MSG_GCTRL_CONFRONT_END2_CLT_MBR	= (NET_MSG_GCTRL + 522),	//<	대련 종료 to (대련 당사자 클라이언트).

	NET_MSG_GCTRL_CONFRONT_RECOVE		= (NET_MSG_GCTRL + 523),	//<	대련옵션 회복약 사용 횟수 to (클라이언트)

	NET_MSG_GCTRL_CONFRONTPTY_START2_FLD= (NET_MSG_GCTRL + 525),	//<	(파티) 대련 시작 to (필드).
	NET_MSG_GCTRL_CONFRONTPTY_START2_CLT= (NET_MSG_GCTRL + 526),	//<	(파티) 대련 시작 to (클라이언트)

	NET_MSG_GCTRL_CONFRONTPTY_END2_FLD	= (NET_MSG_GCTRL + 527),	//<	(파티) 대련 종료 to (필드).
	NET_MSG_GCTRL_CONFRONTPTY_END2_CLT	= (NET_MSG_GCTRL + 528),	//<	(파티) 대련 종료 to (클라이언트).

	NET_MSG_GCTRL_CONFRONTPTY_CHECKMBR_FLD = (NET_MSG_GCTRL + 529),	//<	(파티) 대련에 참가할 멤버 검사 to (필드).
	NET_MSG_GCTRL_CONFRONTPTY_CHECKMBR_AGT = (NET_MSG_GCTRL + 529),	//<	(파티) 대련에 참가할 멤버 to (에이젼트).

	NET_MSG_GCTRL_CONFRONTSPTY_EXP_FLD	= (NET_MSG_GCTRL + 530),	//<	학원 파티 대련 승리시 경험치 전체 유저에 exp 부여.

	NET_MSG_GCTRL_CONFRONTCLB_START2_FLD= (NET_MSG_GCTRL + 531),	//<	(클럽) 대련 시작 to (필드).
	NET_MSG_GCTRL_CONFRONTCLB_START2_CLT= (NET_MSG_GCTRL + 532),	//<	(클럽) 대련 시작 to (클라이언트)

	NET_MSG_GCTRL_CONFRONTCLB_END2_FLD	= (NET_MSG_GCTRL + 533),	//<	(클럽) 대련 종료 to (필드).
	NET_MSG_GCTRL_CONFRONTCLB_END2_CLT	= (NET_MSG_GCTRL + 534),	//<	(클럽) 대련 종료 to (클라이언트).

	NET_MSG_GCTRL_CONFRONTCLB_CHECKMBR_FLD = (NET_MSG_GCTRL + 535),	//<	(클럽) 대련에 참가할 멤버 검사 to (필드).
	NET_MSG_GCTRL_CONFRONTCLB_CHECKMBR_AGT = (NET_MSG_GCTRL + 536),	//<	(클럽) 대련에 참가할 멤버 to (에이젼트).

	NET_MSG_GCTRL_SERVERTEST			= (NET_MSG_GCTRL + 537),	// Purpose for field server test
	NET_MSG_GCTRL_SERVERTEST_FB			= (NET_MSG_GCTRL + 538),

	NET_MSG_GCTRL_REQ_GENITEM_AGT		= (NET_MSG_GCTRL + 541),	//<	아이템 생성 질의 to (에이전트).
	NET_MSG_GCTRL_REQ_GENITEM_FLD_FB	= (NET_MSG_GCTRL + 542),	//<	아이템 생성 답변 to (필드).

	NET_MSG_GCTRL_REQ_GESTURE			= (NET_MSG_GCTRL + 543),	//<	제스쳐 동작.
	NET_MSG_GCTRL_REQ_GESTURE_BRD		= (NET_MSG_GCTRL + 544),	//<	제스쳐 동작 BRD.

	NET_MSG_GCTRL_QITEMFACT_BRD			= (NET_MSG_GCTRL + 545),	//<	?아이템 수치 BRD.
	NET_MSG_GCTRL_QITEMFACT_END_BRD		= (NET_MSG_GCTRL + 546),	//<	?아이템 종료 BRD.

	NET_MSG_GCTRL_EVENTFACT_BRD			= (NET_MSG_GCTRL + 547),	//<	?아이템 이벤트 시작 BRD.
	NET_MSG_GCTRL_EVENTFACT_END_BRD		= (NET_MSG_GCTRL + 548),	//<	?아이템 이벤트 종료 BRD.
	NET_MSG_GCTRL_EVENTFACT_INFO		= (NET_MSG_GCTRL + 549),	//<	?아이템 이벤트 BRD.

	NET_MSG_GCTRL_INVEN_CLEANSER		= (NET_MSG_GCTRL + 551),	//<	망각의 세제를 사용하여 변환된 형상을 되돌림.
	NET_MSG_GCTRL_INVEN_CLEANSER_FB		= (NET_MSG_GCTRL + 552),	//<	망각의 세제를 사용하여 변환된 형상을 되돌림 FB.

	NET_MSG_GCTRL_INVEN_DEL_ITEM_TIMELMT= (NET_MSG_GCTRL + 553),	//<	제한 시간 만료로 아이템 삭제.

	NET_MSG_GCTRL_INVEN_RESET_SKST		= (NET_MSG_GCTRL + 554),	//<	스텟 스킬 리셋 요청.
	NET_MSG_GCTRL_INVEN_RESET_SKST_FB	= (NET_MSG_GCTRL + 555),	//<	스텟 스킬 리셋 응답.

	NET_MSG_GCTRL_INVEN_CHARCARD		= (NET_MSG_GCTRL + 560),	//<	케릭터 추가 카드 사용 요청.
	NET_MSG_GCTRL_INVEN_CHARCARD_FB		= (NET_MSG_GCTRL + 561),	//<	케릭터 추가 카드 사용 요청 FB.

	NET_MSG_GCTRL_INVEN_INVENLINE		= (NET_MSG_GCTRL + 562),	//<	인벤라인 추가 카드 사용 요청.
	NET_MSG_GCTRL_INVEN_INVENLINE_FB	= (NET_MSG_GCTRL + 563),	//<	인벤라인 추가 카드 사용 요청 FB.

	NET_MSG_GCTRL_INVEN_STORAGECARD		= (NET_MSG_GCTRL + 564),	//<	창고 추가 카드 사용 요청.
	NET_MSG_GCTRL_INVEN_STORAGECARD_FB	= (NET_MSG_GCTRL + 565),	//<	창고 추가 카드 사용 요청 FB.

	NET_MSG_GCTRL_INVEN_STORAGEOPEN		= (NET_MSG_GCTRL + 566),	//<	긴급 창고 이용 카드 사용 요청.
	NET_MSG_GCTRL_INVEN_STORAGEOPEN_FB	= (NET_MSG_GCTRL + 567),	//<	긴급 창고 이용 카드 사용 요청 FB.
	NET_MSG_GCTRL_INVEN_STORAGECLOSE	= (NET_MSG_GCTRL + 568),	//<	긴급 창고 이용 카드 사용 완료.

	NET_MSG_GCTRL_INVEN_PREMIUMSET		= (NET_MSG_GCTRL + 569),	//<	프리미엄셋 사용 요청.
	NET_MSG_GCTRL_INVEN_PREMIUMSET_FB	= (NET_MSG_GCTRL + 570),	//<	프리미엄셋 사용 요청 FB.

	NET_MSG_GCTRL_FIRECRACKER			= (NET_MSG_GCTRL + 571),	//<	폭죽 사용 요청.
	NET_MSG_GCTRL_FIRECRACKER_FB		= (NET_MSG_GCTRL + 572),	//<	폭죽 사용 요청 FB.
	NET_MSG_GCTRL_FIRECRACKER_BRD		= (NET_MSG_GCTRL + 573),	//<	폭죽 사용.

	NET_MSG_GCTRL_REVIVE				= (NET_MSG_GCTRL + 574),	//<	귀혼주 사용
	NET_MSG_GCTRL_REVIVE_FB				= (NET_MSG_GCTRL + 575),	//<	귀혼주 사용 FB.

	NET_MSG_GCTRL_PLAYERKILLING_ADD		= (NET_MSG_GCTRL + 576),	//<	player killing 당사자 추가.
	NET_MSG_GCTRL_PLAYERKILLING_DEL		= (NET_MSG_GCTRL + 577),	//<	player killing 당사자 제거.

	NET_MSG_GCTRL_GET_CHARGEDITEM_FROMDB	= (NET_MSG_GCTRL + 579),//<	구입한 아이템 정보 DB에서 가져오기 요청
	NET_MSG_GCTRL_GET_CHARGEDITEM_FROMDB_FB = (NET_MSG_GCTRL + 580),//<	구입한 아이템 정보 DB에서 가져오기 요청 FB
	NET_MSG_GCTRL_CHARGED_ITEM_GET		= (NET_MSG_GCTRL + 581),	//<	구입한 아이템 가져오기 요청.
	NET_MSG_GCTRL_CHARGED_ITEM_GET_FB	= (NET_MSG_GCTRL + 582),	//<	구입한 아이템 가져오기 요청 FB.
	NET_MSG_GCTRL_CHARGED_ITEM_DEL		= (NET_MSG_GCTRL + 583),	//<	구입한 아이템 리스트에서 삭제.
	NET_MSG_GCTRL_CHARGED_ITEM2_INVEN	= (NET_MSG_GCTRL + 584),	//<	구입한 아이템 인벤에 넣기 요청.

	NET_MSG_GCTRL_INVEN_RANDOMBOXOPEN	= (NET_MSG_GCTRL + 585),	//<	random box open 요청.
	NET_MSG_GCTRL_INVEN_RANDOMBOXOPEN_FB= (NET_MSG_GCTRL + 586),	//<	random box open 요청 FB.

	NET_MSG_GCTRL_INVEN_DISJUNCTION		= (NET_MSG_GCTRL + 587),	//<	코스툼 분리 요청.
	NET_MSG_GCTRL_INVEN_DISJUNCTION_FB	= (NET_MSG_GCTRL + 588),	//<	코스툼 분리 요청 FB.

	NET_MSG_GCTRL_INVEN_HAIR_CHANGE		= (NET_MSG_GCTRL + 591),	//<	헤어스타일 변경 요청.
	NET_MSG_GCTRL_INVEN_HAIR_CHANGE_FB	= (NET_MSG_GCTRL + 592),	//<	헤어스타일 변경 결과 FB.
	NET_MSG_GCTRL_INVEN_HAIR_CHANGE_BRD	= (NET_MSG_GCTRL + 593),	//<	헤어스타일 변경 결과 BRD.

	NET_MSG_GCTRL_INVEN_HAIRSTYLE_CHANGE		= (NET_MSG_GCTRL + 594),	//<	헤어스타일 변경 요청.
	NET_MSG_GCTRL_INVEN_HAIRSTYLE_CHANGE_FB		= (NET_MSG_GCTRL + 595),	//<	헤어스타일 변경 결과 FB.
	NET_MSG_GCTRL_INVEN_HAIRSTYLE_CHANGE_BRD	= (NET_MSG_GCTRL + 596),	//<	헤어스타일 변경 결과 BRD.

	NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE		= (NET_MSG_GCTRL + 597),	//<	헤어컬러 변경 요청.
	NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE_FB		= (NET_MSG_GCTRL + 598),	//<	헤어컬러 변경 결과 FB.
	NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE_BRD	= (NET_MSG_GCTRL + 599),	//<	헤어컬러 변경 결과 BRD.

	NET_MSG_GCTRL_INVEN_FACE_CHANGE		= (NET_MSG_GCTRL + 601),	//<	얼굴스타일 변경 요청.
	NET_MSG_GCTRL_INVEN_FACE_CHANGE_FB	= (NET_MSG_GCTRL + 602),	//<	얼굴스타일 변경 결과 FB.
	NET_MSG_GCTRL_INVEN_FACE_CHANGE_BRD	= (NET_MSG_GCTRL + 603),	//<	얼굴스타일 변경 결과 BRD.

	NET_MSG_GCTRL_INVEN_REMODELOPEN		= (NET_MSG_GCTRL + 604),	//<	개조 기능 이용 카드 사용 요청.
	NET_MSG_GCTRL_INVEN_REMODELOPEN_FB	= (NET_MSG_GCTRL + 605),	//<	개조 기능 이용 카드 사용 요청 FB.

	NET_MSG_GCTRL_INVEN_GARBAGEOPEN		= (NET_MSG_GCTRL + 606),	//<	휴지통 기능 이용 카드 사용 요청
	NET_MSG_GCTRL_INVEN_GARBAGEOPEN_FB	= (NET_MSG_GCTRL + 607),	//<	휴지통 기능 이용 카드 사용 요청 FB.
	NET_MSG_GCTRL_GARBAGE_RESULT		= (NET_MSG_GCTRL + 608),	//<	아이템 삭제 요청
	NET_MSG_GCTRL_GARBAGE_RESULT_FB		= (NET_MSG_GCTRL + 609),	//<	아이템 삭제 요청 FB.

	NET_MSG_GCTRL_CROW_MOVETO			= (NET_MSG_GCTRL + 611),
	NET_MSG_GCTRL_CROW_ATTACK			= (NET_MSG_GCTRL + 612),
	NET_MSG_GCTRL_CROW_AVOID			= (NET_MSG_GCTRL + 613),
	NET_MSG_GCTRL_CROW_DAMAGE			= (NET_MSG_GCTRL + 614),
	NET_MSG_GCTRL_CROW_SKILL			= (NET_MSG_GCTRL + 615),

	NET_MSG_GCTRL_INVEN_RENAME			= (NET_MSG_GCTRL + 620),	//<	이름 변경 요청.
	NET_MSG_GCTRL_INVEN_RENAME_FB		= (NET_MSG_GCTRL + 621),	//<	이름 변경 결과 FB.
	NET_MSG_GCTRL_INVEN_RENAME_FROM_DB	= (NET_MSG_GCTRL + 622),	//<	DB 저장 결과 FB.
	NET_MSG_GCTRL_INVEN_RENAME_AGTBRD	= (NET_MSG_GCTRL + 623),	//<	Agent에 모든채널 BRD 요청.
	NET_MSG_GCTRL_INVEN_RENAME_BRD		= (NET_MSG_GCTRL + 624),	//<	이름 변경 결과 BRD.

	NET_MSG_GCTRL_PARTY_MBR_RENAME_CLT	= (NET_MSG_GCTRL + 626),	//< 이름 변경을 파티멤버에게 알림.
	NET_MSG_GCTRL_CLUB_MBR_RENAME_CLT	= (NET_MSG_GCTRL + 627),	//< 이름 변경을 클럽멤버에게 알림.
	NET_MSG_GCTRL_FRIEND_RENAME_CLT		= (NET_MSG_GCTRL + 628),	//< 이름 변경을 친구들에게 알림.
	NET_MSG_GCTRL_FRIEND_PHONENUMBER_CLT= (NET_MSG_GCTRL + 629),	//< 폰번호 변경을 친구들에게 알림.
	NET_MSG_GCTRL_PARTY_MBR_RENAME_FLD	= (NET_MSG_GCTRL + 630),	//< 이름 변경을 FieldServer에 알림.
	NET_MSG_GCTRL_CLUB_MBR_RENAME_FLD	= (NET_MSG_GCTRL + 631),	//< 이름 변경을 FieldServer에 알림.
	NET_MSG_GCTRL_FRIEND_RENAME_FLD		= (NET_MSG_GCTRL + 632),	//< 이름 변경을 FieldServer에 알림.
	NET_MSG_GCTRL_CHANGE_NAMEMAP	 	= (NET_MSG_GCTRL + 633),	//< GLLandMan 의 NameMap 변경

	/* modern party window, Juver, 2019/12/20 */
	NET_MSG_GCTRL_PARTY_CHANGEOPTION	= (NET_MSG_GCTRL + 634),
	NET_MSG_GCTRL_PARTY_OPTIONTOPARTY	= (NET_MSG_GCTRL + 635),

	NET_MSG_GCTRL_REQ_QUEST_START		= (NET_MSG_GCTRL + 646),	//<	퀘스트 시작을 요청.
	NET_MSG_GCTRL_REQ_QUEST_START_FB	= (NET_MSG_GCTRL + 647),	//<	퀘스트 시작을 요청 FB.
	NET_MSG_GCTRL_REQ_QUEST_TALK		= (NET_MSG_GCTRL + 648),	//<	퀘스트 대화 진행 요청.
	NET_MSG_GCTRL_REQ_QUEST_TALK_FB		= (NET_MSG_GCTRL + 649),	//<	퀘스트 대화 진행 요청 FB.

	NET_MSG_GCTRL_QUEST_PROG_GIVEUP		= (NET_MSG_GCTRL + 651),	//<	퀘스트 진행 포기 요청.
	NET_MSG_GCTRL_QUEST_PROG_DEL		= (NET_MSG_GCTRL + 652),	//<	퀘스트 진행 리스트에서 삭제.
	NET_MSG_GCTRL_QUEST_END_DEL			= (NET_MSG_GCTRL + 653),	//<	퀘스트 완료 리스트에서 삭제.
	NET_MSG_GCTRL_QUEST_END_STREAM		= (NET_MSG_GCTRL + 654),	//<	퀘스트 완료 정보.

	NET_MSG_GCTRL_QUEST_PROG_STREAM		= (NET_MSG_GCTRL + 655),	//<	퀘스트 진행 정보.
	NET_MSG_GCTRL_QUEST_PROG_STEP_STREAM= (NET_MSG_GCTRL + 656),	//<	퀘스트 진행 step.
	NET_MSG_GCTRL_QUEST_PROG_INVEN		= (NET_MSG_GCTRL + 657),	//<	퀘스트 INVENTORY.

	NET_MSG_GCTRL_QUEST_PROG_NPCTALK	= (NET_MSG_GCTRL + 661),	//<	퀘스트 진행 npc 대화.
	NET_MSG_GCTRL_QUEST_PROG_NPCTALK_FB	= (NET_MSG_GCTRL + 662),	//<	퀘스트 진행 npc 대화.

	NET_MSG_GCTRL_QUEST_PROG_MOBKILL		= (NET_MSG_GCTRL + 663),	//<	퀘스트 진행 mob 제거.
	NET_MSG_GCTRL_QUEST_PARTY_PROG_MOBKILL	= (NET_MSG_GCTRL + 664),	//<	퀘스트 진행 mob 제거.( 파티퀘스트 완료시 )
	NET_MSG_GCTRL_QUEST_PROG_QITEM			= (NET_MSG_GCTRL + 665),	//<	퀘스트 진행 퀘스트 아이템 습득.
	NET_MSG_GCTRL_QUEST_PARTY_PROG_QITEM	= (NET_MSG_GCTRL + 666),	//<	퀘스트 진행 퀘스트 아이템 습득.( 파티퀘스트 완료시 )
	NET_MSG_GCTRL_QUEST_PROG_REACHZONE		= (NET_MSG_GCTRL + 667),	//<	퀘스트 진행 위치 도달.
	NET_MSG_GCTRL_QUEST_PROG_READ			= (NET_MSG_GCTRL + 668),	//<	퀘스트 읽기 요청 리셋.
	NET_MSG_GCTRL_QUEST_PROG_TIME			= (NET_MSG_GCTRL + 669),	//<	퀘스트 경과 시간.
	NET_MSG_GCTRL_QUEST_PROG_TIMEOVER		= (NET_MSG_GCTRL + 670),	//<	퀘스트 시간 제한 초과됨.
	NET_MSG_GCTRL_QUEST_PROG_NONDIE			= (NET_MSG_GCTRL + 671),	//<	퀘스트 사망 제한.
	NET_MSG_GCTRL_QUEST_PROG_LEVEL			= (NET_MSG_GCTRL + 672),	//<	퀘스트 레벨 도달.

	NET_MSG_GCTRL_QUEST_COMPLETE_FB		= (NET_MSG_GCTRL + 673),	//<	퀘스트가 완료되었으나 보상 물품을 주지 못한상태.
	NET_MSG_GCTRL_REQ_QUEST_COMPLETE	= (NET_MSG_GCTRL + 674),	//<	퀘스트 완료 요청 ( 보상물품 습득 ).

	NET_MSG_GCTRL_QUEST_PROG_INVEN_TURN	= (NET_MSG_GCTRL + 681),	//<	퀘스트 인밴 수량 변경.
	NET_MSG_GCTRL_QUEST_PROG_INVEN_PICKUP= (NET_MSG_GCTRL + 682),	//<	퀘스트 인밴로 아이템 습득.

	NET_MSG_GCTRL_QUEST_PROG_INVEN_INSERT= (NET_MSG_GCTRL + 683),	//<	퀘스트 인밴 아이탬 삽입.
	NET_MSG_GCTRL_QUEST_PROG_INVEN_DELETE= (NET_MSG_GCTRL + 684),	//<	퀘스트 인밴 아이탬 삭제.


	NET_MSG_GCTRL_PMARKET_TITLE			= (NET_MSG_GCTRL + 701),	//<	개인 상점의 타이틀 설정.
	NET_MSG_GCTRL_PMARKET_TITLE_FB		= (NET_MSG_GCTRL + 702),	//<	개인 상점의 타이틀 설정.

	NET_MSG_GCTRL_PMARKET_REGITEM		= (NET_MSG_GCTRL + 703),	//<	개인 상점에 판매 아이템 등록.
	NET_MSG_GCTRL_PMARKET_REGITEM_FB	= (NET_MSG_GCTRL + 704),	//<	개인 상점에 판매 아이템 등록 FB.

	NET_MSG_GCTRL_PMARKET_DISITEM		= (NET_MSG_GCTRL + 705),	//<	개인 상점에서 아이템 제거.
	NET_MSG_GCTRL_PMARKET_DISITEM_FB	= (NET_MSG_GCTRL + 706),	//<	개인 상점에서 아이템 제거 FB.
	NET_MSG_GCTRL_PMARKET_OPEN			= (NET_MSG_GCTRL + 707),	//<	개인 상점 오픈.
	NET_MSG_GCTRL_PMARKET_OPEN_FB		= (NET_MSG_GCTRL + 708),	//<	개인 상점 오픈 FB.
	NET_MSG_GCTRL_PMARKET_OPEN_BRD		= (NET_MSG_GCTRL + 709),	//<	개인 상점 오픈 BRD.

	NET_MSG_GCTRL_PMARKET_CLOSE			= (NET_MSG_GCTRL + 710),	//<	개인 상점 종료.
	NET_MSG_GCTRL_PMARKET_CLOSE_FB		= (NET_MSG_GCTRL + 711),	//<	개인 상점 종료 FB.
	NET_MSG_GCTRL_PMARKET_CLOSE_BRD		= (NET_MSG_GCTRL + 712),	//<	개인 상점 종료 BRD.

	NET_MSG_GCTRL_PMARKET_BUY			= (NET_MSG_GCTRL + 713),	//<	개인 상점에서 구입.
	NET_MSG_GCTRL_PMARKET_BUY_FB		= (NET_MSG_GCTRL + 714),	//<	개인 상점에서 구입 FB.

	NET_MSG_GCTRL_PMARKET_ITEM_INFO		= (NET_MSG_GCTRL + 715),	//<	개인 상점의 판매 아이템 정보 요청.
	NET_MSG_GCTRL_PMARKET_ITEM_INFO_BRD	= (NET_MSG_GCTRL + 716),	//<	개인 상점의 판매 아이템 BRD.
	NET_MSG_GCTRL_PMARKET_ITEM_UPDATE_BRD= (NET_MSG_GCTRL + 717),	//<	개인 상점의 판매 아이템 Update BRD.

	//offline vend
	NET_MSG_GCTRL_PMARKET_OPEN_AGT		= (NET_MSG_GCTRL + 718),

	NET_MSG_GCTRL_CLUB_NEW				= (NET_MSG_GCTRL + 720),	//<	클럽 생성.
	NET_MSG_GCTRL_CLUB_NEW_2AGT			= (NET_MSG_GCTRL + 721),	//<	클럽 생성 to AGENT.
	NET_MSG_GCTRL_CLUB_NEW_DB2AGT		= (NET_MSG_GCTRL + 722),	//<	클럽 생성 DB to AGENT.
	NET_MSG_GCTRL_CLUB_NEW_2FLD			= (NET_MSG_GCTRL + 723),	//<	클럽 생성 to FIELD.
	NET_MSG_GCTRL_CLUB_NEW_FB			= (NET_MSG_GCTRL + 724),	//<	클럽 생성 FB.

	NET_MSG_GCTRL_CLUB_DISSOLUTION		= (NET_MSG_GCTRL + 725),	//<	클럽 해체 요청.
	NET_MSG_GCTRL_CLUB_DISSOLUTION_2FLD	= (NET_MSG_GCTRL + 726),	//<	클럽 해체 요청.
	NET_MSG_GCTRL_CLUB_DISSOLUTION_FB	= (NET_MSG_GCTRL + 727),	//<	클럽 해체 요청 FB.
	NET_MSG_GCTRL_CLUB_DEL_2FLD			= (NET_MSG_GCTRL + 728),	//<	클럽 해체 to FIELD.
	NET_MSG_GCTRL_CLUB_DEL_2CLT			= (NET_MSG_GCTRL + 729),	//<	클럽 삭제 to CLIENT. ( 해체 및 탈퇴 )
	NET_MSG_GCTRL_CLUB_DEL_BRD			= (NET_MSG_GCTRL + 730),	//<	클럽 삭제 BRD. ( 해체 및 탈퇴 )

	NET_MSG_GCTRL_CLUB_INFO_2FLD		= (NET_MSG_GCTRL + 731),	//<	클럽 정보 to FLIED.
	NET_MSG_GCTRL_CLUB_INFO_2CLT		= (NET_MSG_GCTRL + 732),	//<	클럽 정보 to CLIENT.
	NET_MSG_GCTRL_CLUB_INFO_DISSOLUTION	= (NET_MSG_GCTRL + 733),	//<	클럽 해체예약 정보 to CLIENT.
	NET_MSG_GCTRL_CLUB_MEMBER_2CLT		= (NET_MSG_GCTRL + 734),	//<	클럽 멤버 to CLIENT.

	NET_MSG_GCTRL_CLUB_INFO_BRD			= (NET_MSG_GCTRL + 735),	//<	클럽 정보 BRD.
	NET_MSG_GCTRL_CLUB_INFO_MARK_BRD	= (NET_MSG_GCTRL + 736),	//<	클럽 마크 정보 BRD.
	NET_MSG_GCTRL_CLUB_INFO_NICK_BRD	= (NET_MSG_GCTRL + 737),	//<	클럽 닉네임 정보 BRD.

	NET_MSG_GCTRL_CLUB_MARK_INFO		= (NET_MSG_GCTRL + 740),	//<	클럽 마크 요청.
	NET_MSG_GCTRL_CLUB_MARK_INFO_FB		= (NET_MSG_GCTRL + 741),	//<	클럽 마크 반환.

	NET_MSG_GCTRL_CLUB_MARK_CHANGE		= (NET_MSG_GCTRL + 742),	//<	클럽 마크 변경 요청.
	NET_MSG_GCTRL_CLUB_MARK_CHANGE_2FLD	= (NET_MSG_GCTRL + 743),	//<	클럽 마크 변경 요청.
	NET_MSG_GCTRL_CLUB_MARK_CHANGE_2CLT	= (NET_MSG_GCTRL + 744),	//<	클럽 마크 변경 요청.
	NET_MSG_GCTRL_CLUB_MARK_CHANGE_FB	= (NET_MSG_GCTRL + 745),	//<	클럽 마크 변경 FB.

	NET_MSG_GCTRL_CLUB_RANK				= (NET_MSG_GCTRL + 750),	//<	클럽 랭크 변경.
	NET_MSG_GCTRL_CLUB_RANK_2AGT		= (NET_MSG_GCTRL + 751),	//<	클럽 랭크 변경.
	NET_MSG_GCTRL_CLUB_RANK_2FLD		= (NET_MSG_GCTRL + 752),	//<	클럽 랭크 변경.
	NET_MSG_GCTRL_CLUB_RANK_2CLT		= (NET_MSG_GCTRL + 753),	//<	클럽 랭크 변경.
	NET_MSG_GCTRL_CLUB_RANK_FB			= (NET_MSG_GCTRL + 754),	//<	클럽 랭크 변경 FB.

	NET_MSG_GCTRL_CLUB_MEMBER_REQ		= (NET_MSG_GCTRL + 755),	//<	클럽 참가 요청.
	NET_MSG_GCTRL_CLUB_MEMBER_REQ_ASK	= (NET_MSG_GCTRL + 756),	//<	클럽 참가 질의.
	NET_MSG_GCTRL_CLUB_MEMBER_REQ_ANS	= (NET_MSG_GCTRL + 757),	//<	클럽 참가 답변.
	NET_MSG_GCTRL_CLUB_MEMBER_REQ_2AGT	= (NET_MSG_GCTRL + 758),	//<	클럽 참가 답변.
	NET_MSG_GCTRL_CLUB_MEMBER_REQ_FB	= (NET_MSG_GCTRL + 759),	//<	클럽 참가 요청 FB.

	NET_MSG_GCTRL_CLUB_MEMBER_ADD_2FLD	= (NET_MSG_GCTRL + 760),	//<	클럽 멤버 추가 to FIELD.

	NET_MSG_GCTRL_CLUB_MEMBER_DEL		= (NET_MSG_GCTRL + 761),	//<	클럽 멤버 제명 요청.
	NET_MSG_GCTRL_CLUB_MEMBER_DEL_2FLD	= (NET_MSG_GCTRL + 762),	//<	클럽 멤버 제명 요청 to FIELD.
	NET_MSG_GCTRL_CLUB_MEMBER_DEL_2CLT	= (NET_MSG_GCTRL + 763),	//<	클럽 멤버 제명 요청 결과 to CLIENT.
	NET_MSG_GCTRL_CLUB_MEMBER_DEL_FB	= (NET_MSG_GCTRL + 764),	//<	클럽 멤버 제명 요청 결과 FB.

	NET_MSG_GCTRL_CLUB_MEMBER_NICK		= (NET_MSG_GCTRL + 765),	//<	클럽 멤버 별명.
	NET_MSG_GCTRL_CLUB_MEMBER_NICK_FB	= (NET_MSG_GCTRL + 766),	//<	클럽 멤버 별명 FB.

	NET_MSG_GCTRL_CLUB_MEMBER_STATE		= (NET_MSG_GCTRL + 767),	//<	클럽 멤버 상태.
	NET_MSG_GCTRL_CLUB_MEMBER_POS		= (NET_MSG_GCTRL + 768),	//<	클럽 멤버 위치.

	NET_MSG_GCTRL_CLUB_MEMBER_SECEDE	= (NET_MSG_GCTRL + 769),	//<	클럽 멤버 탈퇴.
	NET_MSG_GCTRL_CLUB_MEMBER_SECEDE_FB	= (NET_MSG_GCTRL + 770),	//<	클럽 멤버 탈퇴 FB.

	NET_MSG_GCTRL_CLUB_MEMBER_DB2DEL	= (NET_MSG_GCTRL + 771),	//<	클럽 멤버 제명 요청.

	NET_MSG_GCTRL_CLUB_BATTLE_REMAIN_BRD = (NET_MSG_GCTRL + 775),	//<	선도 클럽 결정전 종료.
	NET_MSG_GCTRL_CLUB_BATTLE_START_FLD	= (NET_MSG_GCTRL + 776),	//<	선도 클럽 결정전 시작.
	NET_MSG_GCTRL_CLUB_BATTLE_START_BRD	= (NET_MSG_GCTRL + 777),	//<	선도 클럽 결정전 시작.

	NET_MSG_GCTRL_CLUB_BATTLE_END_FLD	= (NET_MSG_GCTRL + 778),	//<	선도 클럽 결정전 종료.
	NET_MSG_GCTRL_CLUB_BATTLE_END_BRD	= (NET_MSG_GCTRL + 779),	//<	선도 클럽 결정전 종료.

	NET_MSG_GCTRL_CLUB_CD_CERTIFY		  = (NET_MSG_GCTRL + 780),	//<	cd 인증.
	NET_MSG_GCTRL_CLUB_CD_CERTIFY_FB      = (NET_MSG_GCTRL + 780),	//<	cd FB
	NET_MSG_GCTRL_CLUB_CD_CERTIFY_BRD	  = (NET_MSG_GCTRL + 781),	//<	cd 인증 BRD.
	NET_MSG_GCTRL_CLUB_CD_CERTIFY_ING_BRD = (NET_MSG_GCTRL + 782),	//<	cd 인증 결과 BRD

	NET_MSG_GCTRL_CLUB_CERTIFIED_AGT	= (NET_MSG_GCTRL + 787),	//<	cd 인증 완료 ( agent에 알림 )
	NET_MSG_GCTRL_CLUB_CERTIFIED_FLD	= (NET_MSG_GCTRL + 788),	//<	cd 인증 완료 ( field에 알림 )
	NET_MSG_GCTRL_CLUB_CERTIFIED_CLT	= (NET_MSG_GCTRL + 789),	//<	cd 인증 완료 ( field에 알림 )

	NET_MSG_GCTRL_CLUB_COMMISSION		= (NET_MSG_GCTRL + 790),	//<	선도 지역 수수료 설정.
	NET_MSG_GCTRL_CLUB_COMMISSION_FB	= (NET_MSG_GCTRL + 791),	//<	선도 지역 수수료 설정 FB.
	NET_MSG_GCTRL_CLUB_COMMISSION_FLD	= (NET_MSG_GCTRL + 792),	//<	선도 지역 수수료 설정 FLD.
	NET_MSG_GCTRL_CLUB_COMMISSION_BRD	= (NET_MSG_GCTRL + 793),	//<	선도 지역 수수료 설정 BRD.
	NET_MSG_GCTRL_CLUB_INCOME_MONEY_AGT	= (NET_MSG_GCTRL + 794),	//<	클럽에 수익 발생 AGT.

	NET_MSG_GCTRL_CLUB_GETSTORAGE		= (NET_MSG_GCTRL + 795),	//<	창고 정보 요청.
	NET_MSG_GCTRL_CLUB_GETSTORAGE_FB	= (NET_MSG_GCTRL + 796),	//<	창고 정보 요청.
	NET_MSG_GCTRL_CLUB_GETSTORAGE_ITEM	= (NET_MSG_GCTRL + 797),	//<	창고 정보 회신.

	NET_MSG_GCTRL_CLUB_STORAGE_TO_HOLD	= (NET_MSG_GCTRL + 798),	//<	창고 아이템 들어올리기.
	NET_MSG_GCTRL_CLUB_STORAGE_EX_HOLD	= (NET_MSG_GCTRL + 799),	//<	창고 아이템과 손에 들린 아이템 교환.
	NET_MSG_GCTRL_CLUB_HOLD_TO_STORAGE	= (NET_MSG_GCTRL + 800),	//<	손에 들린 아이템 창고에 넣기.
	NET_MSG_GCTRL_CLUB_STORAGE_SPLIT	= (NET_MSG_GCTRL + 801),	//<	창고의 아이템 분할하기.
	NET_MSG_GCTRL_CLUB_STORAGE_SAVE_MONEY= (NET_MSG_GCTRL + 802),	//<	돈을 저장.
	NET_MSG_GCTRL_CLUB_STORAGE_DRAW_MONEY= (NET_MSG_GCTRL + 803),	//<	돈을 인출.

	NET_MSG_GCTRL_CLUB_STORAGE_INSERT	= (NET_MSG_GCTRL + 804),	//<	창고에 넣기.
	NET_MSG_GCTRL_CLUB_STORAGE_DELETE	= (NET_MSG_GCTRL + 805),	//<	창고에서 삭제.
	NET_MSG_GCTRL_CLUB_STORAGE_DEL_INS	= (NET_MSG_GCTRL + 806),	//<	삭제와 넣기 동시에 하기.
	NET_MSG_GCTRL_CLUB_STORAGE_UPDATE_ITEM= (NET_MSG_GCTRL + 807),	//<	창고에 있는 아이템의 정보 갱신.
	NET_MSG_GCTRL_CLUB_STORAGE_UPDATE_MONEY= (NET_MSG_GCTRL + 808),	//<	창고의 저장금액 갱신.

	NET_MSG_GCTRL_CLUB_COMMISSION_RV_BRD= (NET_MSG_GCTRL + 809),	//<	선도 지역 수수료 설정 예약 BRD.

	NET_MSG_GCTRL_CLUB_STORAGE_RESET	= (NET_MSG_GCTRL + 811),	//<	클럽 창고 정보를 리샛.
	NET_MSG_GCTRL_CLUB_STORAGE_GET_DB	= (NET_MSG_GCTRL + 812),	//<	클럽 창고 정보를 db에서 수신.

	NET_MSG_GCTRL_CLUB_INCOME_RENEW		= (NET_MSG_GCTRL + 815),	//<	클럽 수익금 업뎃 요청 ( client->field ).
	NET_MSG_GCTRL_CLUB_INCOME_UP		= (NET_MSG_GCTRL + 816),	//<	클럽 수익금 업뎃 요청 ( field->agent ).
	NET_MSG_GCTRL_CLUB_INCOME_DN		= (NET_MSG_GCTRL + 817),	//<	클럽 수익금 업뎃 회신 ( agent->field ).
	NET_MSG_GCTRL_CLUB_INCOME_FB		= (NET_MSG_GCTRL + 818),	//<	클럽 수익금 업뎃 결과 ( field->agent ).

	NET_MSG_GCTRL_CLUB_NOTICE_REQ		= (NET_MSG_GCTRL + 821),	//<	클럽 공지 수정 요청. ( client->agent )
	NET_MSG_GCTRL_CLUB_NOTICE_FB		= (NET_MSG_GCTRL + 822),	//<	클럽 공지 수정 결과. ( agent->client )
	NET_MSG_GCTRL_CLUB_NOTICE_FLD		= (NET_MSG_GCTRL + 823),	//<	클럽 공지 필드에 알림. ( agent->field )
	NET_MSG_GCTRL_CLUB_NOTICE_CLT		= (NET_MSG_GCTRL + 824),	//<	클럽 공지 클라이언트에 알림. ( field->client )

	NET_MSG_GCTRL_CLUB_SUBMASTER		= (NET_MSG_GCTRL + 825),	//<	클럽 부마 설정.
	NET_MSG_GCTRL_CLUB_SUBMASTER_FB		= (NET_MSG_GCTRL + 826),	//<	클럽 부마 설정 FB.
	NET_MSG_GCTRL_CLUB_SUBMASTER_BRD	= (NET_MSG_GCTRL + 827),	//<	클럽 부마 설정 알림 CLT.
	NET_MSG_GCTRL_CLUB_SUBMASTER_FLD	= (NET_MSG_GCTRL + 828),	//<	클럽 부마 설정 알림 FLD.

	NET_MSG_GCTRL_CLUB_ALLIANCE_REQ		= (NET_MSG_GCTRL + 831),	//<	클럽 동맹 요청.
	NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_ASK	= (NET_MSG_GCTRL + 832),	//<	클럽 동맹 요청 ASK.
	NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_ANS	= (NET_MSG_GCTRL + 833),	//<	클럽 동맹 요청 ANS.
	NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_FB	= (NET_MSG_GCTRL + 834),	//<	클럽 동맹 요청 결과 FB.

	NET_MSG_GCTRL_CLUB_ALLIANCE_ADD_FLD	= (NET_MSG_GCTRL + 837),	//<	클럽 동맹 클럽 ADD FLD.
	NET_MSG_GCTRL_CLUB_ALLIANCE_ADD_CLT	= (NET_MSG_GCTRL + 838),	//<	클럽 동맹 클럽 ADD CLT.

	NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_REQ	= (NET_MSG_GCTRL + 839),	//<	클럽 동맹 클럽 제명 요청.
	NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_FB	= (NET_MSG_GCTRL + 840),	//<	클럽 동맹 클럽 제명 요청 FB.

	NET_MSG_GCTRL_CLUB_ALLIANCE_SEC_REQ	= (NET_MSG_GCTRL + 841),	//<	클럽 동맹 탈퇴 요청.
	NET_MSG_GCTRL_CLUB_ALLIANCE_SEC_FB	= (NET_MSG_GCTRL + 842),	//<	클럽 동맹 탈퇴 요청 FB.

	NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_FLD	= (NET_MSG_GCTRL + 843),	//<	클럽 동맹 클럽 DEL FLD.
	NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_CLT	= (NET_MSG_GCTRL + 844),	//<	클럽 동맹 클럽 DEL CLT.

	NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_REQ	= (NET_MSG_GCTRL + 845),	//<	클럽 동맹 해체 FLD.
	NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_FB	= (NET_MSG_GCTRL + 846),	//<	클럽 동맹 해체 FLD.

	NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_FLD	= (NET_MSG_GCTRL + 847),	//<	클럽 동맹 해체 FLD.
	NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_CLT	= (NET_MSG_GCTRL + 848),	//<	클럽 동맹 해체 CLT.


	NET_MSG_GCTRL_CLUB_BATTLE_REQ		= (NET_MSG_GCTRL + 849),	//<	클럽 배틀 요청 
	NET_MSG_GCTRL_CLUB_BATTLE_REQ_ASK	= (NET_MSG_GCTRL + 850),	//<	클럽 배틀 요청 ASK.
	NET_MSG_GCTRL_CLUB_BATTLE_REQ_ANS	= (NET_MSG_GCTRL + 851),	//<	클럽 배틀 요청 ANS.
	NET_MSG_GCTRL_CLUB_BATTLE_REQ_FB	= (NET_MSG_GCTRL + 852),	//<	클럽 배틀 요청 결과 FB.

	NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_FLD	= (NET_MSG_GCTRL + 853),	//<	클럽 배틀 시작 FLD.
	NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_CLT	= (NET_MSG_GCTRL + 854),	//<	클럽 배틀 시작 CLT(대기).
	NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_CLT2= (NET_MSG_GCTRL + 855),	//<	클럽 배틀 시작 CLT2(진짜시작).
	NET_MSG_GCTRL_CLUB_BATTLE_OVER_FLD	= (NET_MSG_GCTRL + 856),	//<	클럽 배틀 종료 FLD.
	NET_MSG_GCTRL_CLUB_BATTLE_OVER_CLT	= (NET_MSG_GCTRL + 857),	//<	클럽 배틀 종료 CLT.

	NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ		= (NET_MSG_GCTRL + 858),	//<	클럽 배틀 휴전 요청.
	NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_ASK	= (NET_MSG_GCTRL + 859),	//<	클럽 배틀 휴전 요청 ASK.
	NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_ANS	= (NET_MSG_GCTRL + 860),	//<	클럽 배틀 휴전 요청 ANS.
	NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_FB	= (NET_MSG_GCTRL + 861),	//<	클럽 배틀 휴전 요청 결과 FB.

	NET_MSG_GCTRL_CLUB_BATTLE_SUBMISSION_REQ	= (NET_MSG_GCTRL + 862),	//<	클럽 배틀 항복 요청.
	NET_MSG_GCTRL_CLUB_BATTLE_SUBMISSION_REQ_FB	= (NET_MSG_GCTRL + 863),	//<	클럽 배틀 항복 요청 결과 FB.

	NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE_AGT	= (NET_MSG_GCTRL + 864),	//<	클럽 배틀 킬수 UPDATE Field -> Agent
	NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE_FLD	= (NET_MSG_GCTRL + 865),	//<	클럽 배틀 킬수 UPDATE Agent -> Field
	NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE		= (NET_MSG_GCTRL + 866),	//<	클럽 배틀 킬수 UPDATE 클라이언트

	NET_MSG_GCTRL_CLUB_BATTLE_POINT_UPDATE		= (NET_MSG_GCTRL + 867),	//<	클럽 배틀 킬/데스수 증가

	NET_MSG_GCTRL_CLUB_BATTLE_LAST_KILL_UPDATE_FLD	= (NET_MSG_GCTRL + 868),	//<	클럽 배틀 종료시 Field정보 조합 Agent -> Field
	NET_MSG_GCTRL_CLUB_BATTLE_LAST_KILL_UPDATE_AGT	= (NET_MSG_GCTRL + 869),	//<	클럽 배틀 종료시 Field정보 조합 결과 Field -> Agent


	NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ		= (NET_MSG_GCTRL + 870),	//<	동맹 배틀 요청 
	NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_ASK	= (NET_MSG_GCTRL + 871),	//<	동맹 배틀 요청 ASK.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_ANS	= (NET_MSG_GCTRL + 872),	//<	동맹 배틀 요청 ANS.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_FB	= (NET_MSG_GCTRL + 873),	//<	동맹 배틀 요청 결과 FB.

	NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ		= (NET_MSG_GCTRL + 874),	//<	동맹 배틀 휴전 요청.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_ASK	= (NET_MSG_GCTRL + 875),	//<	동맹 배틀 휴전 요청 ASK.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_ANS	= (NET_MSG_GCTRL + 876),	//<	동맹 배틀 휴전 요청 ANS.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_FB	= (NET_MSG_GCTRL + 877),	//<	동맹 배틀 휴전 요청 결과 FB.

	NET_MSG_GCTRL_ALLIANCE_BATTLE_SUBMISSION_REQ	= (NET_MSG_GCTRL + 878),	//<	동맹 배틀 항복 요청.
	NET_MSG_GCTRL_ALLIANCE_BATTLE_SUBMISSION_REQ_FB	= (NET_MSG_GCTRL + 879),	//<	동맹 배틀 항복 요청 결과 FB.





	NET_MSG_GCTRL_LEVEL_EVENT_END_FLD		= (NET_MSG_GCTRL + 896),	//<	레벨 이벤트 종료.
	NET_MSG_GCTRL_LEVEL_EVENT_WARNING_FLD	= (NET_MSG_GCTRL + 897),	//< 레벨 이벤트 경고 메세지.
	NET_MSG_GCTRL_LEVEL_EVENT_COUNTDOWN_FLD = (NET_MSG_GCTRL + 898),	//< 레벨 이벤트 카운트 메세지.

	NET_MSG_GM_EVENT_EX					= (NET_MSG_GCTRL + 899),	//<	EX이벤트 시작.
	NET_MSG_GM_EVENT_EX_END				= (NET_MSG_GCTRL + 900),	//<	EX이벤트 종료.

	NET_MSG_GM_EVENT_EXP				= (NET_MSG_GCTRL + 901),	//<	경험치 이벤트 시작.
	NET_MSG_GM_EVENT_EXP_FB				= (NET_MSG_GCTRL + 902),
	NET_MSG_GM_EVENT_EXP_END			= (NET_MSG_GCTRL + 903),	//<	경험치 이벤트 종료.
	NET_MSG_GM_EVENT_EXP_END_FB			= (NET_MSG_GCTRL + 904),	//<	

	NET_MSG_GM_EVENT_ITEM_GEN			= (NET_MSG_GCTRL + 905),	//<	아이탬 발생율 이벤트.
	NET_MSG_GM_EVENT_ITEM_GEN_FB		= (NET_MSG_GCTRL + 906),
	NET_MSG_GM_EVENT_ITEM_GEN_END		= (NET_MSG_GCTRL + 907),	//<	아이탬 발생율 이벤트 종료.
	NET_MSG_GM_EVENT_ITEM_GEN_END_FB	= (NET_MSG_GCTRL + 908),	//

	NET_MSG_GM_EVENT_MONEY_GEN			= (NET_MSG_GCTRL + 909),	//<	돈 발생율 이벤트.
	NET_MSG_GM_EVENT_MONEY_GEN_FB		= (NET_MSG_GCTRL + 910),
	NET_MSG_GM_EVENT_MONEY_GEN_END		= (NET_MSG_GCTRL + 911),	//<	돈 발생율 이벤트 종료.
	NET_MSG_GM_EVENT_MONEY_GEN_END_FB	= (NET_MSG_GCTRL + 912),	

	NET_MSG_GCTRL_PERIOD				= (NET_MSG_GCTRL + 921),
	NET_MSG_GCTRL_WEATHER				= (NET_MSG_GCTRL + 922),
	NET_MSG_GCTRL_WHIMSICAL				= (NET_MSG_GCTRL + 923),
	NET_MSG_GCTRL_FIELDINFO_RESET		= (NET_MSG_GCTRL + 924),

	NET_MSG_SERVER_CTRL_WEATHER			= (NET_MSG_GCTRL + 931),	//<	서버의 날씨 변경.
	NET_MSG_SERVER_CTRL_TIME			= (NET_MSG_GCTRL + 932),	//<	서버의 시간 변경.
	NET_MSG_SERVER_CTRL_MONTH			= (NET_MSG_GCTRL + 933),	//<	서버의 월 변경.
	NET_MSG_SERVER_CTRL_GENITEMHOLD		= (NET_MSG_GCTRL + 934),	//<	서버의 아이템 독점권 변경.
	NET_MSG_SERVER_PLAYERKILLING_MODE	= (NET_MSG_GCTRL + 935),	//<	player killing on/off.

	NET_MSG_GM_MOVE2GATE				= (NET_MSG_GCTRL + 936),	//<	케릭터를 gate no 로 이동.
	NET_MSG_GM_MOVE2GATE_FLD			= (NET_MSG_GCTRL + 937),	//<	케릭터를 gate no 로 이동 Agent -> Field
	NET_MSG_GM_MOVE2GATE_FB				= (NET_MSG_GCTRL + 938),	//<	케릭터를 gate no 로 이동.
	NET_MSG_GM_KICK_USER				= (NET_MSG_GCTRL + 940),	//<	특정 플레이어를 내보냄.
	NET_MSG_GM_KICK_USER_PROC			= (NET_MSG_GCTRL + 941),	//<	특정 플레이어를 내보냄.
	NET_MSG_GM_KICK_USER_PROC_FLD		= (NET_MSG_GCTRL + 942),    //<  특정 플레이어 강제 접속종료 Agent -> Field
	NET_MSG_GM_MOVE2MAPPOS				= (NET_MSG_GCTRL + 944),	//<	캐릭터를 특정맵의 특정위치로 이동.
	NET_MSG_GM_MOVE2MAPPOS_FLD			= (NET_MSG_GCTRL + 945),	//<	캐릭터를 특정맵의 특정위치로 이동. Agent -> Field

	NET_MSG_BLOCK_DETECTED				= (NET_MSG_GCTRL + 946),	//<	케릭터를 gate no 로 이동.
	NET_MSG_SERVER_GENERALCHAT			= (NET_MSG_GCTRL + 947),	//<	서버 알림 메시지 발생시.

	NET_MSG_GM_WHERE_NPC				= (NET_MSG_GCTRL + 948),	//<	NPC가 어디 위치에 있는지 검사.
	NET_MSG_GM_WHERE_NPC_FLD			= (NET_MSG_GCTRL + 949),	//<	NPC가 어디 위치에 있는지 검사. Agent -> Field
	NET_MSG_GM_WHERE_NPC_FB				= (NET_MSG_GCTRL + 950),	//<	NPC가 어디 위치에 있는지 검사.

	NET_MSG_GM_WHERE_PC_MAP				= (NET_MSG_GCTRL + 951),	//<	PC가 어느 멥에 있는지 검사.
	NET_MSG_GM_WHERE_PC_MAP_FB			= (NET_MSG_GCTRL + 952),	//<	PC가 어느 멥에 있는지 검사 FB.

	NET_MSG_GM_WHERE_PC_POS				= (NET_MSG_GCTRL + 953),	//<	PC가 어디 위치에 있는지 검사.
	NET_MSG_GM_WHERE_PC_POS_FB			= (NET_MSG_GCTRL + 954),	//<	PC가 어디 위치에 있는지 검사 FB.

	NET_MSG_GM_MOVE2CHAR				= (NET_MSG_GCTRL + 955),	//<	캐릭터를 특정캐릭터로 이동 요청.
	NET_MSG_GM_MOVE2CHAR_POS			= (NET_MSG_GCTRL + 956),	//<	캐릭터를 특정캐릭터로 이동 위치 확인.
	NET_MSG_GM_MOVE2CHAR_AG				= (NET_MSG_GCTRL + 957),	//<	캐릭터를 특정캐릭터로 이동 위치 확인 응답.
	NET_MSG_GM_MOVE2CHAR_FLD			= (NET_MSG_GCTRL + 958),	//<	캐릭터를 특정캐릭터로 이동 요청 실행.
	NET_MSG_GM_MOVE2CHAR_FB				= (NET_MSG_GCTRL + 959),	//<	캐릭터를 특정캐릭터로 이동 실행 결과.

	NET_MSG_GM_BIGHEAD					= (NET_MSG_GCTRL + 961),	//<	BIG HEAD 모드 on/off.
	NET_MSG_GM_BIGHEAD_BRD				= (NET_MSG_GCTRL + 962),	//<	BIG HEAD 모드 on/off.

	NET_MSG_GM_BIGHAND					= (NET_MSG_GCTRL + 963),	//<	BIG HAND 모드 on/off.
	NET_MSG_GM_BIGHAND_BRD				= (NET_MSG_GCTRL + 964),	//<	BIG HAND 모드 on/off.

	NET_MSG_GM_MOB_GEN					= (NET_MSG_GCTRL + 965),	//<	몹 발생.
	NET_MSG_GM_MOB_GEN_FLD				= (NET_MSG_GCTRL + 966),	//<	몹 발생. Agent->Field
	NET_MSG_GM_MOB_DEL					= (NET_MSG_GCTRL + 967),	//<	몹 제거.
	NET_MSG_GM_MOB_DEL_FLD				= (NET_MSG_GCTRL + 968),	//<	몹 제거. Agent->Field
	NET_MSG_GM_MOB_LEVEL				= (NET_MSG_GCTRL + 969),	//<	몹(LEVEL FILE) 발생.
	NET_MSG_GM_MOB_LEVEL_CLEAR			= (NET_MSG_GCTRL + 970),	//<	몹(LEVEL FILE) 발생 제거.

	NET_MSG_GM_CHAT_BLOCK_UACCOUNT		= (NET_MSG_GCTRL + 971),	//<	채팅 블록 (유저이름)
	NET_MSG_GM_CHAT_BLOCK_CHARNAME		= (NET_MSG_GCTRL + 972),	//<	채팅 블록 (캐릭이름)
	NET_MSG_GM_CHAT_BLOCK_CHARID		= (NET_MSG_GCTRL + 973),	//<	채팅 블록 (캐릭번호)
	NET_MSG_GM_CHAT_BLOCK_FB			= (NET_MSG_GCTRL + 974),	//<	채팅 블록 FB
	NET_MSG_GM_CHAT_BLOCK_FLD			= (NET_MSG_GCTRL + 975),	//<	채팅 블록 FLD

	//vacant 978~980

	// Contribution Item - JX
	NET_MSG_GCTRL_CARD_CONTRIBUTION						= (NET_MSG_GCTRL + 976),
	NET_MSG_GCTRL_CARD_CONTRIBUTION_FB					= (NET_MSG_GCTRL + 977),

	NET_MSG_GM_CHAR_INFO_4NAME			= (NET_MSG_GCTRL + 981),	//<	캐릭터의 정보 출력 (캐릭이름).
	NET_MSG_GM_CHAR_INFO_4CHARID		= (NET_MSG_GCTRL + 982),	//<	캐릭터의 정보 출력 (캐릭번호).
	NET_MSG_GM_CHAR_INFO_AGT_FB			= (NET_MSG_GCTRL + 983),	//<	캐릭터의 정보 출력 agt FB.
	NET_MSG_GM_CHAR_INFO_FLD_FB			= (NET_MSG_GCTRL + 984),	//<	캐릭터의 정보 출력 fld FB.
	NET_MSG_GM_CHAR_INFO_FLD			= (NET_MSG_GCTRL + 985),	//<	캐릭터의 정보 출력 FLD.


	//-----------------------------------------------------------------------------------------------------------

	NET_MSG_GM_FREEPK					  = (NET_MSG_GCTRL + 986),	//<	자유 피케이
	NET_MSG_GM_FREEPK_BRD				  = (NET_MSG_GCTRL + 987),	//<	자유 피케이

	NET_MSG_GM_WARNING_MSG				  = (NET_MSG_GCTRL + 989),	//<	자유 피케이
	NET_MSG_GM_WARNING_MSG_FLD			  = (NET_MSG_GCTRL + 990),	//<	자유 피케이
	NET_MSG_GM_WARNING_MSG_BRD			  = (NET_MSG_GCTRL + 991),	//<	자유 피케이

	NET_MSG_GM_VIEWALLPLAYER			  = (NET_MSG_GCTRL + 992),	//<  맵의 모든 플레이어 표시
	NET_MSG_GM_VIEWALLPLAYER_FLD_REQ	  = (NET_MSG_GCTRL + 993),	//<  필드서버에 요청
	NET_MSG_GM_VIEWALLPLAYER_FLD_FB 	  = (NET_MSG_GCTRL + 994),	//<  필드서버 처리결과 회신

	NET_MSG_GM_GENCHAR					  = (NET_MSG_GCTRL + 995),	//<  케릭터 소환 요청
	NET_MSG_GM_GENCHAR_POS				  = (NET_MSG_GCTRL + 996),	//<  케릭터 소환 요청
	NET_MSG_GM_GENCHAR_AG				  = (NET_MSG_GCTRL + 997),	//<  케릭터 소환 요청
	NET_MSG_GM_GENCHAR_FLD				  = (NET_MSG_GCTRL + 998),	//<  케릭터 소환 요청
	NET_MSG_GM_GENCHAR_FB				  = (NET_MSG_GCTRL + 999),	//<  케릭터 소환 요청

	NET_MSG_GCTRL_SERVER_BRIGHTEVENT_INFO = (NET_MSG_GCTRL + 1001),	// 성향이벤트
	NET_MSG_GCTRL_SERVER_CLUB_BATTLE_INFO = (NET_MSG_GCTRL + 1002),	// 선도전 시작 유무
	NET_MSG_GCTRL_REQ_SERVER_CLUB_BATTLE_REMAIN_AG = (NET_MSG_GCTRL + 1003), // 선도전 남은시간 요청(Field->Agent->Client)

	NET_MSG_GCTRL_UPDATE_LASTCALL		= (NET_MSG_GCTRL + 1010),	// 직전 귀환 위치 클라이언트로 전송
	NET_MSG_GCTRL_UPDATE_STARTCALL		= (NET_MSG_GCTRL + 1011),	// 시작 귀환 위치 클라이언트로 전송

	NET_MSG_GM_COUNTDOWN_MSG_BRD		= (NET_MSG_GCTRL + 1020),	// 자동레벨 카운트 메세지
	NET_MSG_GM_SHOWMETHEMONEY			= (NET_MSG_GCTRL + 1021),	//<  캐릭터로 돈 저장
	NET_MSG_GM_SHOWMETHEMONEY_FLD		= (NET_MSG_GCTRL + 1022),	//<  캐릭터로 돈 저장

	//-----------------------------------------------------------------------------------------------------------

	NET_MSG_REBUILD_RESULT				= (NET_MSG_GCTRL + 1025),	// 아이템 개조 - 시작, 종료 결과	// ITEMREBUILD_MARK
	NET_MSG_REBUILD_MOVE_ITEM			= (NET_MSG_GCTRL + 1026),	// 아이템 개조 - 아이템 이동
	NET_MSG_REBUILD_COST_MONEY			= (NET_MSG_GCTRL + 1027),	// 아이템 개조 - 개조 비용
	NET_MSG_REBUILD_INPUT_MONEY			= (NET_MSG_GCTRL + 1028),	// 아이템 개조 - 유저 입력 금액

	//-----------------------------------------------------------------------------------------------------------

	NET_MSG_PET_DROPPET					= (NET_MSG_GCTRL + 1030),	// ANYPET 생성
	NET_MSG_CREATE_ANYPET				= (NET_MSG_GCTRL + 1031),	// ANYPET 생성
	NET_MSG_PET_REQ_USECARD				= (NET_MSG_GCTRL + 1032),	// 팻카드 사용
	NET_MSG_PET_REQ_USECARD_FB			= (NET_MSG_GCTRL + 1033),
	NET_MSG_PET_REQ_UNUSECARD			= (NET_MSG_GCTRL + 1034),	// 팻 소멸
	NET_MSG_PET_REQ_UNUSECARD_FB		= (NET_MSG_GCTRL + 1035),
	NET_MSG_PET_REQ_DISAPPEAR			= (NET_MSG_GCTRL + 1036),	// 강제로 소멸(맵이동시, 게임종료등...)

	NET_MSG_CREATE_PET_FROMDB_FB		= (NET_MSG_GCTRL + 1037),	// 팻생성(DB)
	NET_MSG_GET_PET_FROMDB_FB			= (NET_MSG_GCTRL + 1038),	// 팻생성(DB)
	NET_MSG_GET_PET_FROMDB_ERROR		= (NET_MSG_GCTRL + 1039),	// 팻생성(DB) 에러

	NET_MSG_PET_REQ_UPDATE_MOVE_STATE	 = (NET_MSG_GCTRL + 1040),  // 움직임 상태 변경
	NET_MSG_PET_REQ_UPDATE_MOVE_STATE_FB = (NET_MSG_GCTRL + 1041),
	NET_MSG_PET_REQ_UPDATE_MOVE_STATE_BRD = (NET_MSG_GCTRL + 1042),

	/*dual pet skill, Juver, 2017/12/27 */
	NET_MSG_PET_REQ_SKILLCHANGE_A		= (NET_MSG_GCTRL + 1143),	// 스킬변경
	NET_MSG_PET_REQ_SKILLCHANGE_A_FB	= (NET_MSG_GCTRL + 1144),	// 스킬변경
	NET_MSG_PET_REQ_SKILLCHANGE_A_BRD	= (NET_MSG_GCTRL + 1145),	// 스킬변경
	NET_MSG_PET_ADD_SKILLFACT_A			= (NET_MSG_GCTRL + 1146),	// SkillFact 추가
	NET_MSG_PET_REMOVE_SKILLFACT_A		= (NET_MSG_GCTRL + 1147),	// SkillFact 추가

	/*dual pet skill, Juver, 2017/12/27 */
	NET_MSG_PET_REQ_SKILLCHANGE_B		= (NET_MSG_GCTRL + 1148),	// 스킬변경
	NET_MSG_PET_REQ_SKILLCHANGE_B_FB	= (NET_MSG_GCTRL + 1149),	// 스킬변경
	NET_MSG_PET_REQ_SKILLCHANGE_B_BRD	= (NET_MSG_GCTRL + 1150),	// 스킬변경
	NET_MSG_PET_ADD_SKILLFACT_B			= (NET_MSG_GCTRL + 1151),	// SkillFact 추가
	NET_MSG_PET_REMOVE_SKILLFACT_B		= (NET_MSG_GCTRL + 1152),	// SkillFact 추가

	NET_MSG_PET_GETRIGHTOFITEM_A		= (NET_MSG_GCTRL + 1153),	// 소유권한이 있는 아이템정보 요청
	NET_MSG_PET_GETRIGHTOFITEM_A_FB		= (NET_MSG_GCTRL + 1154),	// 소유권한이 있는 아이템정보 요청
	NET_MSG_PET_GETRIGHTOFITEM_B		= (NET_MSG_GCTRL + 1155),	// 소유권한이 있는 아이템정보 요청
	NET_MSG_PET_GETRIGHTOFITEM_B_FB		= (NET_MSG_GCTRL + 1156),	// 소유권한이 있는 아이템정보 요청

	NET_MSG_PET_REQ_GOTO				= (NET_MSG_GCTRL + 1053),	// 이동처리
	NET_MSG_PET_REQ_STOP				= (NET_MSG_GCTRL + 1054),	// 이동처리
	NET_MSG_PET_REQ_GOTO_FB				= (NET_MSG_GCTRL + 1055),	// 이동처리
	NET_MSG_PET_GOTO_BRD				= (NET_MSG_GCTRL + 1056),	// 이동처리
	NET_MSG_PET_STOP_BRD				= (NET_MSG_GCTRL + 1057),	// 이동처리

	NET_MSG_PET_REQ_GIVEFOOD			= (NET_MSG_GCTRL + 1058),	// 사료주기
	NET_MSG_PET_REQ_GIVEFOOD_FB			= (NET_MSG_GCTRL + 1059),	// 사료주기
	NET_MSG_PET_REQ_GETFULL_FROMDB_FB	= (NET_MSG_GCTRL + 1060),	// 사료주기

	/*dual pet skill, Juver, 2017/12/29 */
	NET_MSG_PET_REQ_ENABLE_DUAL_SKILL		= (NET_MSG_GCTRL + 1061),
	NET_MSG_PET_REQ_ENABLE_DUAL_SKILL_FB	= (NET_MSG_GCTRL + 1062),

	//NET_MSG_PET_GETRIGHTOFITEM			= (NET_MSG_GCTRL + 1070),	// 소유권한이 있는 아이템정보 요청
	//NET_MSG_PET_GETRIGHTOFITEM_FB		= (NET_MSG_GCTRL + 1071),	// 소유권한이 있는 아이템정보 요청
	NET_MSG_PET_NOTENOUGHINVEN			= (NET_MSG_GCTRL + 1072),	// 인벤토리가 부족하여 팻을 사라지게 함

	NET_MSG_PET_REQ_RENAME				= (NET_MSG_GCTRL + 1075),	// 이름변경
	NET_MSG_PET_REQ_RENAME_FB			= (NET_MSG_GCTRL + 1076),	// 이름변경
	NET_MSG_PET_REQ_RENAME_BRD			= (NET_MSG_GCTRL + 1077),	// 이름변경
	NET_MSG_PET_REQ_RENAME_FROMDB_FB	= (NET_MSG_GCTRL + 1078),	// 이름변경

	NET_MSG_PET_REQ_CHANGE_COLOR		= (NET_MSG_GCTRL + 1080),	// 컬러변경
	NET_MSG_PET_REQ_CHANGE_COLOR_FB		= (NET_MSG_GCTRL + 1081),	// 컬러변경
	NET_MSG_PET_REQ_CHANGE_COLOR_BRD	= (NET_MSG_GCTRL + 1082),	// 컬러변경

	NET_MSG_PET_REQ_CHANGE_STYLE		= (NET_MSG_GCTRL + 1085),	// 스타일변경
	NET_MSG_PET_REQ_CHANGE_STYLE_FB		= (NET_MSG_GCTRL + 1086),	// 스타일변경
	NET_MSG_PET_REQ_CHANGE_STYLE_BRD	= (NET_MSG_GCTRL + 1087),	// 스타일변경

	// 클라이언트 상태 변경 (Field -> Client)
	NET_MSG_PET_UPDATECLIENT_FULL		= (NET_MSG_GCTRL + 1090),	// 팻포만감 갱신

	NET_MSG_PET_REQ_SLOT_EX_HOLD		= (NET_MSG_GCTRL + 1100),	// 아이템 탈착(SLOT<->HOLD)
	NET_MSG_PET_REQ_SLOT_EX_HOLD_FB		= (NET_MSG_GCTRL + 1101),	// 아이템 탈착(SLOT<->HOLD)
	NET_MSG_PET_REQ_SLOT_EX_HOLD_BRD	= (NET_MSG_GCTRL + 1102),	// 아이템 탈착(SLOT<->HOLD)
	NET_MSG_PET_REQ_SLOT_TO_HOLD		= (NET_MSG_GCTRL + 1103),	// 아이템 탈착(SLOT->HOLD)
	NET_MSG_PET_REQ_SLOT_TO_HOLD_FB		= (NET_MSG_GCTRL + 1104),	// 아이템 탈착(SLOT->HOLD)
	NET_MSG_PET_REQ_SLOT_TO_HOLD_BRD	= (NET_MSG_GCTRL + 1105),	// 아이템 탈착(SLOT->HOLD)
	NET_MSG_PET_REQ_HOLD_TO_SLOT		= (NET_MSG_GCTRL + 1106),	// 아이템 탈착(HOLD->SLOT)
	NET_MSG_PET_REQ_HOLD_TO_SLOT_FB		= (NET_MSG_GCTRL + 1107),	// 아이템 탈착(HOLD->SLOT)
	NET_MSG_PET_REQ_HOLD_TO_SLOT_BRD	= (NET_MSG_GCTRL + 1108),	// 아이템 탈착(HOLD->SLOT)

	NET_MSG_PET_REQ_LEARNSKILL			= (NET_MSG_GCTRL + 1110),	// 팻스킬 배우기
	NET_MSG_PET_REQ_LEARNSKILL_FB		= (NET_MSG_GCTRL + 1111),	// 팻스킬 배우기
	NET_MSG_PET_REQ_LEARNSKILL_BRD		= (NET_MSG_GCTRL + 1112),	// 팻스킬 배우기

	NET_MSG_PET_REQ_FUNNY				= (NET_MSG_GCTRL + 1115),	// Funny Gesture
	NET_MSG_PET_REQ_FUNNY_BRD			= (NET_MSG_GCTRL + 1116),	// Funny Gesture

	NET_MSG_PET_REMOVE_SLOTITEM			= (NET_MSG_GCTRL + 1118),	// 팻슬롯아이템 제거
	NET_MSG_PET_REMOVE_SLOTITEM_FB		= (NET_MSG_GCTRL + 1119),	// 팻슬롯아이템 제거
	NET_MSG_PET_REMOVE_SLOTITEM_BRD		= (NET_MSG_GCTRL + 1120),	// 팻슬롯아이템 제거

	NET_MSG_PET_ATTACK					= (NET_MSG_GCTRL + 1125),	// 공격모션
	NET_MSG_PET_ATTACK_BRD				= (NET_MSG_GCTRL + 1126),	// 공격모션

	NET_MSG_PET_SAD						= (NET_MSG_GCTRL + 1127),	// Sad모션
	NET_MSG_PET_SAD_BRD					= (NET_MSG_GCTRL + 1128),	// Sad모션

	NET_MSG_PET_REQ_PETCARDINFO			= (NET_MSG_GCTRL + 1131),	// 팻카드 정보
	NET_MSG_PET_REQ_PETCARDINFO_FB		= (NET_MSG_GCTRL + 1132),	// 팻카드 정보

	NET_MSG_PET_REQ_PETREVIVEINFO		= (NET_MSG_GCTRL + 1133),	// 팻부활 정보
	NET_MSG_PET_REQ_PETREVIVEINFO_FB	= (NET_MSG_GCTRL + 1134),	// 팻부활 정보

	NET_MSG_PET_REQ_REVIVE				= (NET_MSG_GCTRL + 1135),	// 삭제된 팻 부활
	NET_MSG_PET_REQ_REVIVE_FB			= (NET_MSG_GCTRL + 1136),	// 삭제된 팻 부활
	NET_MSG_PET_REQ_REVIVE_FROMDB_FB	= (NET_MSG_GCTRL + 1137),	// 삭제된 팻 부활

	NET_MSG_PET_ACCESSORY_DELETE		= (NET_MSG_GCTRL + 1140),	// 팻악세사리 삭제
	NET_MSG_PET_ACCESSORY_DELETE_BRD	= (NET_MSG_GCTRL + 1141),	// 팻악세사리 삭제

	NET_MSG_SMS_PHONE_NUMBER			= (NET_MSG_GCTRL + 1200),	// 캐릭터 폰번호 저장/업데이트 요청
	NET_MSG_SMS_PHONE_NUMBER_FB			= (NET_MSG_GCTRL + 1201),	//<
	NET_MSG_SMS_PHONE_NUMBER_FROM_DB	= (NET_MSG_GCTRL + 1202),	//<	DB 저장 결과 FB.
	NET_MSG_SMS_PHONE_NUMBER_AGTBRD		= (NET_MSG_GCTRL + 1203),	//<	Agent에 모든채널 BRD 요청.

	NET_MSG_SMS_SEND					= (NET_MSG_GCTRL + 1204),
	NET_MSG_SMS_SEND_FB					= (NET_MSG_GCTRL + 1205),
	NET_MSG_SMS_SEND_FROM_DB			= (NET_MSG_GCTRL + 1206),

	NET_MSG_GM_MOB_GEN_EX				= (NET_MSG_GCTRL + 1210),   // 몹소환(확장기능추가)
	NET_MSG_GM_MOB_GEN_EX_FLD			= (NET_MSG_GCTRL + 1211),   // 몹소환(확장기능추가)
	NET_MSG_GM_MOB_DEL_EX				= (NET_MSG_GCTRL + 1212),   // 몹제거(확장기능추가)
	NET_MSG_GM_MOB_DEL_EX_FLD			= (NET_MSG_GCTRL + 1213),   // 몹제거(확장기능추가)

	NET_MSG_MGAME_ODDEVEN				= (NET_MSG_GCTRL + 1220),	// 미니 게임 - 홀짝(클라이언트->필드서버)
	NET_MSG_MGAME_ODDEVEN_FB			= (NET_MSG_GCTRL + 1221),	// 미니 게임 - 홀짝(필드서버->클라이언트)	

	NET_MSG_SERVERTIME_BRD				= (NET_MSG_GCTRL + 1230),	// 서버 현재시간 BroadCasting

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	
	NET_MSG_GCTRL_CLUBWAR_MYRANK_RESU_UPDATE		= (NET_MSG_GCTRL + 1231),
	NET_MSG_GCTRL_CLUBWAR_RANKING_RESU_UPDATE		= (NET_MSG_GCTRL + 1232),
	NET_MSG_GCTRL_CLUBWAR_RANKING_RESU_REQ			= (NET_MSG_GCTRL + 1233),

	NET_MSG_GCTRL_CLUBWAR_CLUB_RANK_POINT_UPDATE	= (NET_MSG_GCTRL + 1234),
	NET_MSG_GCTRL_CLUBWAR_CLUB_MYRANK_UPDATE		= (NET_MSG_GCTRL + 1235),
	NET_MSG_GCTRL_CLUBWAR_CLUB_RANKING_UPDATE		= (NET_MSG_GCTRL + 1236),
	NET_MSG_GCTRL_CLUBWAR_CLUB_RANKING_REQ			= (NET_MSG_GCTRL + 1237),
	
	//vacant 1240 ~ 1259

	NET_MSG_GCTRL_STORAGE_ITEM_UPDATE	= (NET_MSG_GCTRL + 1260 ),	//< 락커 아이템 업데이트
	NET_MSG_REQ_FRIEND_CLUB_OPEN		= (NET_MSG_GCTRL + 1270 ),	//< 친구,클럽창 열림(클라이언트->에이젼트)

	NET_MSG_GM_GETWHISPERMSG		    = (NET_MSG_GCTRL + 1402),	//<	캐릭터의 귓속말을 엿듣는다.
	NET_MSG_CHINA_GAINTYPE  		    = (NET_MSG_GCTRL + 1403),	//<	중국 등급별 수익

	NET_MSG_GM_LIMIT_EVENT_BEGIN		= (NET_MSG_GCTRL + 1404),	//<	이벤트 시작.
	NET_MSG_GM_LIMIT_EVENT_END			= (NET_MSG_GCTRL + 1405),	//<	이벤트 종료.

	NET_MSG_GM_LIMIT_EVENT_RESTART		= (NET_MSG_GCTRL + 1406),	//<	이벤트 다시 시작.

	NET_MSG_GM_LIMIT_EVENT_APPLY_START	= (NET_MSG_GCTRL + 1407),	//<	클라이언트에 이벤트 적용 시작했음을 알림
	NET_MSG_GM_LIMIT_EVENT_APPLY_END 	= (NET_MSG_GCTRL + 1408),	//<	클라이언트에 이벤트 적용 끝났음을 알림

	NET_MSG_GM_LIMIT_EVENT_BEGIN_FB		= (NET_MSG_GCTRL + 1409),   //<	클라이언트에 이벤트가 시작 되었음을 알림
	NET_MSG_GM_LIMIT_EVENT_END_FB		= (NET_MSG_GCTRL + 1410),   //<	클라이언트에 이벤트가 종료 되었음을 알림

	NET_MSG_GM_LIMIT_EVENT_TIME_REQ		= (NET_MSG_GCTRL + 1411),   //<	클라이언트가 서버에 이벤트 정보를 요청
	NET_MSG_GM_LIMIT_EVENT_TIME_REQ_FB	= (NET_MSG_GCTRL + 1412),   //<	서버가 클라이언트에 이벤트 정보를 보냄
	NET_MSG_GM_LIMIT_EVENT_TIME_RESET	= (NET_MSG_GCTRL + 1413),   //<	AGENT서버가 FIELD서버에게 로그인 시간을 RESET하라고 보냄

	NET_MSG_GM_PRINT_CROWLIST			= (NET_MSG_GCTRL + 1414),   //<	자신이 있는 맵의 몹 리스트를 프린트함.
	NET_MSG_GM_PRINT_CROWLIST_FLD		= (NET_MSG_GCTRL + 1415),   //<	자신이 있는 맵의 몹 리스트를 프린트함.
	NET_MSG_GM_PRINT_CROWLIST_FB		= (NET_MSG_GCTRL + 1416),   //<	자신이 있는 맵의 몹 리스트를 프린트함.

	NET_MSG_SERVER_CTRL_WEATHER2		= (NET_MSG_GCTRL + 1417),	//<	서버의 날씨 변경. 새로운 메시지
	NET_MSG_GCTRL_MAPWEATHER			= (NET_MSG_GCTRL + 1418),	
	NET_MSG_GCTRL_MAPWHIMSICAL			= (NET_MSG_GCTRL + 1419),

	NET_MSG_GCTRL_ACTIVE_VEHICLE			= (NET_MSG_GCTRL + 1420),	//< 클라이언트가 탈것을 활성/비활성 요청
	NET_MSG_GCTRL_ACTIVE_VEHICLE_FB			= (NET_MSG_GCTRL + 1421),	//< 클라이언트에 탈것을 활성/비활성 알림
	NET_MSG_GCTRL_ACTIVE_VEHICLE_BRD		= (NET_MSG_GCTRL + 1422),	//< 주위 클라이언트에 탈것을 활성/비활성 알림

	NET_MSG_GCTRL_GET_VEHICLE				= (NET_MSG_GCTRL + 1423),	//< 클라이언트가 탈것의 정보 요청
	NET_MSG_GCTRL_GET_VEHICLE_FB			= (NET_MSG_GCTRL + 1424),	//< 클라이언트에 탈것의 정보 받음
	NET_MSG_GCTRL_GET_VEHICLE_BRD			= (NET_MSG_GCTRL + 1425),	//< 주위 클라이언트에 탈것 정보 전송
	NET_MSG_GCTRL_UNGET_VEHICLE_FB			= (NET_MSG_GCTRL + 1426),	//< 탈것 정보 비활성화
	NET_MSG_GCTRL_UNGET_VEHICLE_BRD			= (NET_MSG_GCTRL + 1427),	//< 탈것 정보 비활성화

	NET_MSG_VEHICLE_CREATE_FROMDB_FB		= (NET_MSG_GCTRL + 1428),	//< 최초 탈것 생성 ( DB ) 
	NET_MSG_VEHICLE_GET_FROMDB_FB			= (NET_MSG_GCTRL + 1429),	//< 탈것 생성 ( DB )
	NET_MSG_VEHICLE_GET_FROMDB_ERROR		= (NET_MSG_GCTRL + 1430),	//< 탈것 생성 ( DB에러 )

	NET_MSG_VEHICLE_ACCESSORY_DELETE		= (NET_MSG_GCTRL + 1431),	//< 탈것의 악세서리 삭제
	NET_MSG_VEHICLE_ACCESSORY_DELETE_BRD	= (NET_MSG_GCTRL + 1432),	// 탈것의 악세사리 삭제 알림

	NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD		= (NET_MSG_GCTRL + 1433 ),	// 탈것 slot ex hold
	NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD_FB		= (NET_MSG_GCTRL + 1434 ),	// 탈것 slot ex hold
	NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD_BRD	= (NET_MSG_GCTRL + 1435 ),	// 탈것 slot ex hold

	NET_MSG_VEHICLE_REQ_HOLD_TO_SLOT		= (NET_MSG_GCTRL + 1436 ),	// 탈것 hold to slot
	NET_MSG_VEHICLE_REQ_HOLD_TO_SLOT_FB		= (NET_MSG_GCTRL + 1437 ),	// 탈것 hold to slot

	NET_MSG_VEHICLE_REQ_SLOT_TO_HOLD		= (NET_MSG_GCTRL + 1438 ),	// 탈것 slot to hold
	NET_MSG_VEHICLE_REQ_SLOT_TO_HOLD_FB		= (NET_MSG_GCTRL + 1439 ),	// 탈것 slot to hold

	NET_MSG_VEHICLE_REMOVE_SLOTITEM			= (NET_MSG_GCTRL + 1440 ),	// item slot 해제
	NET_MSG_VEHICLE_REMOVE_SLOTITEM_FB		= (NET_MSG_GCTRL + 1441 ),	// item slot 해제
	NET_MSG_VEHICLE_REMOVE_SLOTITEM_BRD		= (NET_MSG_GCTRL + 1442 ),	// item slot 해제

	NET_MSG_VEHICLE_REQ_GIVE_BATTERY		= (NET_MSG_GCTRL + 1443 ),	// 배터리 충전
	NET_MSG_VEHICLE_REQ_GIVE_BATTERY_FB		= (NET_MSG_GCTRL + 1444 ),	// 배터리 충전
	NET_MSG_VEHICLE_REQ_GETFULL_FROMDB_FB	= (NET_MSG_GCTRL + 1445 ),	// 배터리 충전
	NET_MSG_VEHICLE_UPDATE_CLIENT_BATTERY	= (NET_MSG_GCTRL + 1446 ),	// 배터리 갱신
	NET_MSG_VEHICLE_REQ_GET_BATTERY_FROMDB_FB	= (NET_MSG_GCTRL + 1447 ),	// 배터리 가져오기 ( 내부 )

	NET_MSG_VEHICLE_REQ_ITEM_INFO			= (NET_MSG_GCTRL + 1448),	// 탈것아이템 정보
	NET_MSG_VEHICLE_REQ_ITEM_INFO_FB		= (NET_MSG_GCTRL + 1449),	// 탈것아이템 정보

	NET_MSG_VIETNAM_GAINTYPE				= (NET_MSG_GCTRL + 1450),	// 베트남 탐닉 방지 시스템 현재 수익 타입
	NET_MSG_VIETNAM_TIME_REQ				= (NET_MSG_GCTRL + 1451),	// 베트남 탐닉 방지 시스템 시간 요청
	NET_MSG_VIETNAM_TIME_REQ_FB				= (NET_MSG_GCTRL + 1452),	// 베트남 탐닉 방지 시스템 시간 요청
	NET_MSG_VIETNAM_GAIN_EXP				= (NET_MSG_GCTRL + 1453),	// 베트남 탐닉 방지 시스템 경험치
	NET_MSG_VIETNAM_GAIN_MONEY				= (NET_MSG_GCTRL + 1454),	// 베트남 탐닉 방지 시스템 돈
	NET_MSG_GCTRL_REQ_VNGAIN_TO_HOLD		= (NET_MSG_GCTRL + 1455),   //<	탐닉방지 인밴에 있는 아이탬을 들기 시도.
	NET_MSG_GCTRL_REQ_VNGAIN_EX_HOLD		= (NET_MSG_GCTRL + 1456),	//<	탐닉방지 인밴에 있는 아이탬을 들기 시도.
	NET_MSG_GCTRL_REQ_HOLD_TO_VNGAIN		= (NET_MSG_GCTRL + 1457),	//<	아이템을 탐닉방지 인밴에 넣음
	NET_MSG_GCTRL_REQ_VNGAIN_INVEN_RESET	= (NET_MSG_GCTRL + 1458),	//< 탐닉 방지 인밴 전체 초기화
	NET_MSG_GCTRL_REQ_VNINVEN_TO_INVEN      = (NET_MSG_GCTRL + 1459),	//< 오른쪽 버튼으로 탐닉 방지 인밴에서 인밴으로 아이템 옮길때
	NET_MSG_GCTRL_REQ_VNINVEN_TO_INVEN_FB   = (NET_MSG_GCTRL + 1460),	//< 오른쪽 버튼으로 탐닉 방지 인밴에서 인밴으로 아이템 옮길때
	NET_MSG_GCTRL_INVEN_VIETNAM_INVENGET	= (NET_MSG_GCTRL + 1461),	//<	베트남 탐닉 방지 아이템 사용시 메시지
	NET_MSG_GCTRL_INVEN_VIETNAM_EXPGET_FB	= (NET_MSG_GCTRL + 1462),	//<	베트남 탐닉 방지 아이템 사용시 메시지
	NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGET_FB	= (NET_MSG_GCTRL + 1463),	//<	베트남 탐닉 방지 아이템 사용시 메시지
	NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGETNUM_UPDATE = (NET_MSG_GCTRL + 1464),	//<	베트남 탐닉 방지 아이템 사용시 메시지 
	NET_MSG_VIETNAM_ALLINITTIME				= (NET_MSG_GCTRL + 1465),	//<	베트남 탐닉 방지시스템 12시가 지나서 초기화 되었을때

	NET_MSG_GCTRL_INVEN_GENDER_CHANGE		= (NET_MSG_GCTRL + 1466),	// 성별 변경
	NET_MSG_GCTRL_INVEN_GENDER_CHANGE_FB	= (NET_MSG_GCTRL + 1467),	// 성별 변경

	NET_MSG_MARKET_CHECKTIME				= (NET_MSG_GCTRL + 1468),	// 상점이 열려있을 때 10분마다 메시지를 보내 튕김 현상을 방지한다.

	NET_MSG_CHAT_CTRL_FB					= (NET_MSG_GCTRL + 1469), //< 관리자용 채팅메시지 정의 ( 게임서버->클라이언트 )

	NET_NON_REBIRTH_REQ						= (NET_MSG_GCTRL + 1470), //< 부활 스킬 금지 사용 여부

	NET_MSG_REQ_SKILL_REVIVEL_FAILED		= (NET_MSG_GCTRL + 1471), //< 부활 스킬 금지 되었을 경우 당사자에게 메시지를 보낸다.

	NET_QBOX_OPTION_REQ_AG					= (NET_MSG_GCTRL + 1472), //< QBox On/Off 옵션 마스터 -> 에이전트 서버
	NET_QBOX_OPTION_REQ_FLD					= (NET_MSG_GCTRL + 1473), //< QBox On/Off 옵션 에이전트 서버 -> 필드서버
	NET_QBOX_OPTION_MEMBER					= (NET_MSG_GCTRL + 1474), //< QBox On/Off 옵션 필드 서버 -> 멤버에게..

	NET_MSG_UPDATE_TRACING_ALL				= (NET_MSG_GCTRL + 1475), //< 추적할 캐릭터를 전부 업데이트한다.
	NET_MSG_UPDATE_TRACING_CHARACTER		= (NET_MSG_GCTRL + 1476), //< 추적할 캐릭터를 특정 캐릭터만 업데이트한다.
	NET_MSG_LOG_UPDATE_TRACING_CHARACTER	= (NET_MSG_GCTRL + 1477), //< // 추적중인 유저의 로그를 agent서버에 보낸다.

	NET_MSG_TRACING_CHARACTER				= (NET_MSG_GCTRL + 1478), //< 필드->클라이언트 추적 유저인지 아닌지를 보낸다.

	NET_MSG_GCTRL_REQ_TELEPORT				= (NET_MSG_GCTRL + 1479),	//<	소환서 아이템 사용 요청.
	NET_MSG_GCTRL_REQ_TELEPORT_FB			= (NET_MSG_GCTRL + 1480),	//<	소환서 아이템 사용 응답.
	NET_MSG_GCTRL_REQ_TELEPORT_AG			= (NET_MSG_GCTRL + 1481),	//<	소환서 에이젼트 서버에 요청.

	NET_MSG_GCTRL_GETEXP_RECOVERY			= (NET_MSG_GCTRL + 1482),	// 경험치 회복 데이터 요청
	NET_MSG_GCTRL_GETEXP_RECOVERY_FB		= (NET_MSG_GCTRL + 1483),	// 경험치 회복 데이터 요청 FB
	NET_MSG_GCTRL_RECOVERY					= (NET_MSG_GCTRL + 1484),	// 경험치 회복 사용
	NET_MSG_GCTRL_RECOVERY_FB				= (NET_MSG_GCTRL + 1485),	// 경험치 회복 사용 FB
	NET_MSG_GCTRL_GETEXP_RECOVERY_NPC		= (NET_MSG_GCTRL + 1486),	// 경험치 회복 데이터 요청 (NPC)
	NET_MSG_GCTRL_GETEXP_RECOVERY_NPC_FB	= (NET_MSG_GCTRL + 1487),	// 경험치 회복 데이터 요청 FB (NPC)
	NET_MSG_GCTRL_RECOVERY_NPC				= (NET_MSG_GCTRL + 1488),	// 경험치 회복 사용 (NPC)
	NET_MSG_GCTRL_RECOVERY_NPC_FB			= (NET_MSG_GCTRL + 1489),	// 경험치 회복 사용 FB (NPC)
	NET_MSG_GCTRL_QUEST_PROG_LEAVEMAP		= (NET_MSG_GCTRL + 1490),	//<	퀘스트 맵이동 제한.
	//vacant 1491

	NET_MSG_VIETNAM_TIME_RESET				= (NET_MSG_GCTRL + 1492),	// 베트남 시간 리셋 //vietnamtest%%% && vietnamtest2

	NET_MSG_GCTRL_ITEMSHOPOPEN				= (NET_MSG_GCTRL + 1493),	//<	ItemShopOpen ( Japan ) 
	NET_MSG_GCTRL_ITEMSHOPOPEN_BRD			= (NET_MSG_GCTRL + 1494),	//<	ItemShopOpen ( Japan ) 

	NET_MSG_GM_VIEWWORKEVENT			    = (NET_MSG_GCTRL + 1495),	//< 현재 agent서버에 적용된 모든 이벤트 보기
	NET_MSG_GM_VIEWWORKEVENT_FB			    = (NET_MSG_GCTRL + 1496),	//< 현재 agent서버에 적용된 모든 이벤트 보기 회신

	NET_MSG_PET_PETSKINPACKOPEN				= (NET_MSG_GCTRL + 1497),	//< 펫 스킨 팩 아이템 사용
	NET_MSG_PET_PETSKINPACKOPEN_FB			= (NET_MSG_GCTRL + 1498),	//< 펫 스킨 팩 아이템 사용
	NET_MSG_PET_PETSKINPACKOPEN_BRD			= (NET_MSG_GCTRL + 1499),	//< 펫 스킨 팩 아이템 사용

	NET_MSG_GCTRL_PMARKET_SEARCH_ITEM		= (NET_MSG_GCTRL + 1500),	//< 개인상점의 아이템 검색
	NET_MSG_GCTRL_PMARKET_SEARCH_ITEM_RESULT = (NET_MSG_GCTRL + 1501),	//< 개인상점의 아이템 검색 결과
	NET_MSG_GCTRL_PMARKET_SEARCH_ITEM_RESULT_REQ = (NET_MSG_GCTRL + 1502),	//< 개인상점의 아이템 검색 결과 요청

	NET_MSG_GCTRL_DEFENSE_SKILL_ACTIVE		= (NET_MSG_GCTRL + 1503),	//< 피격시 스킬 발동

	NET_MSG_REQ_ATTENDLIST					= (NET_MSG_GCTRL + 1504),	//< 출석정보 리스트 요청
	NET_MSG_REQ_ATTENDLIST_FB				= (NET_MSG_GCTRL + 1505),	//< 출석정보 리스트 결과
	NET_MSG_REQ_ATTENDANCE					= (NET_MSG_GCTRL + 1506),	//< 출석 체크
	NET_MSG_REQ_ATTENDANCE_FB				= (NET_MSG_GCTRL + 1507),	//< 출석 체크 답변
	NET_MSG_REQ_ATTEND_REWARD_FLD			= (NET_MSG_GCTRL + 1508),	//< 출석 체크 이벤트 아이템 지급 ( 필드용 )
	NET_MSG_REQ_ATTEND_REWARD_CLT			= (NET_MSG_GCTRL + 1509),	//< 출석 체크 이벤트 아이템 지급 ( 클라용 )

	//NET_MSG_REQ_USE_SUMMON					= (NET_MSG_GCTRL + 1510),	//< 소환을 요청
	NET_MSG_REQ_USE_SUMMON_FB				= (NET_MSG_GCTRL + 1511),	//< 소환을 요청
	NET_MSG_REQ_USE_SUMMON_DEL				= (NET_MSG_GCTRL + 1512),	//< 소환수를 지움
	NET_MSG_SUMMON_REQ_GOTO					= (NET_MSG_GCTRL + 1513),	//< 소환수 이동
	NET_MSG_SUMMON_GOTO_BRD					= (NET_MSG_GCTRL + 1514),	//< 소환수 이동	
	NET_MSG_SUMMON_REQ_STOP					= (NET_MSG_GCTRL + 1515),	//< 소환수 멈춤
	NET_MSG_SUMMON_REQ_STOP_BRD				= (NET_MSG_GCTRL + 1516),	//< 소환수 멈춤
	NET_MSG_SUMMON_REQ_UPDATE_MOVE_STATE_BRD = (NET_MSG_GCTRL + 1517),	//< 소환수 이동상태 업데이트
	NET_MSG_SUMMON_REQ_UPDATE_MOVE_STATE	= (NET_MSG_GCTRL + 1518),	//< 소환수 이동상태 업데이트
	NET_MSG_CREATE_ANYSUMMON				= (NET_MSG_GCTRL + 1519),	//< 다른 클라의 소환수 생성
	NET_MSG_DROP_ANYSUMMON					= (NET_MSG_GCTRL + 1520),	//< 다른 클라의 소환수 삭제
	NET_MSG_SUMMON_ATTACK					= (NET_MSG_GCTRL + 1521),	//< 소환수 공격
	NET_MSG_SUMMON_ATTACK_BRD				= (NET_MSG_GCTRL + 1522),	//< 소환수 공격
	NET_MSG_GCTRL_SUMMON_ATTACK_AVOID		= (NET_MSG_GCTRL + 1523),	//< 소환수 공격 회피
	NET_MSG_GCTRL_SUMMON_ATTACK_DAMAGE		= (NET_MSG_GCTRL + 1524),	//< 소환수 공격 데미지

	/*skill summon, Juver, 2017/10/09 */
	NET_MSG_SUMMON_END_REST					= (NET_MSG_GCTRL + 1525),	//
	NET_MSG_SUMMON_END_REST_BRD				= (NET_MSG_GCTRL + 1526),	//
	NET_MSG_SUMMON_END_LIFE					= (NET_MSG_GCTRL + 1527),	//
	NET_MSG_SUMMON_END_LIFE_BRD				= (NET_MSG_GCTRL + 1528),	//
	NET_MSG_SUMMON_RESET_TARGET				= (NET_MSG_GCTRL + 1529),	//
	NET_MSG_SUMMON_RESET_TARGET_BRD			= (NET_MSG_GCTRL + 1530),	//

	//vacant 1525~ 1532


	NET_MSG_GCTRL_INVEN_FACESTYLE_CHANGE	= (NET_MSG_GCTRL + 1541),	//< 얼굴 스타일 변경 요청
	NET_MSG_GCTRL_INVEN_FACESTYLE_CHANGE_FB	= (NET_MSG_GCTRL + 1542),	//< 얼굴 스타일 변경 결과
	NET_MSG_GCTRL_INVEN_FACESTYLE_CHANGE_BRD= (NET_MSG_GCTRL + 1543),	//< 얼굴 스타일 변경 결과

	NET_MSG_REQ_MUST_LEAVE_MAP		= (NET_MSG_GCTRL + 1544),	//< 레벨업시 조건이 맞지않아 맵을 떠나야 할 때
	NET_MSG_REQ_MUST_LEAVE_MAP_FB	= (NET_MSG_GCTRL + 1545),	//< 레벨업시 조건이 맞지않아 맵을 떠나야 할 때
	NET_MSG_REQ_MUST_LEAVE_MAP_AG	= (NET_MSG_GCTRL + 1546),	//< 레벨업시 조건이 맞지않아 맵을 떠나야 할 때

	NET_MSG_CYBERCAFECLASS_UPDATE			= (NET_MSG_GCTRL + 1547),	//< 태국 사이버 카페 등급 업데이트

	NET_MSG_GCTRL_REQ_TAXI					= (NET_MSG_GCTRL + 1548),	//< 택시 탑승 요청
	NET_MSG_GCTRL_REQ_TAXI_FB				= (NET_MSG_GCTRL + 1549),	//< 택시 탑승 요청 결과
	NET_MSG_GCTRL_REQ_TAXI_NPCPOS			= (NET_MSG_GCTRL + 1550),	//< 택시 탑승시 NPC좌표 요청
	NET_MSG_GCTRL_REQ_TAXI_NPCPOS_FB		= (NET_MSG_GCTRL + 1551),	//< 택시 탑승시 NPC좌표 요청 결과

	NET_MSG_GCTRL_ETNRY_FAILED				= (NET_MSG_GCTRL + 1552),	//< 진입 실패시 메시지

	NET_MSG_GCTRL_ITEM_COOLTIME_UPDATE		= (NET_MSG_GCTRL + 1553),	//< 쿨타임 업데이트
	NET_MSG_GCTRL_ITEM_COOLTIME_ERROR		= (NET_MSG_GCTRL + 1554),	//< 쿨타임에 걸려서 사용못함

	NET_MSG_GCTRL_NPC_RECALL				= (NET_MSG_GCTRL + 1555),	//< NPC 호출.
	NET_MSG_GCTRL_NPC_RECALL_FB				= (NET_MSG_GCTRL + 1556),	//< NPC 호출 결과.

	NET_MSG_GCTRL_NPC_COMMISSION			= (NET_MSG_GCTRL + 1557),	//< NPC 호출 COMMISION 업데이트 요청 ( Field->Agent ) 
	NET_MSG_GCTRL_NPC_COMMISSION_FB			= (NET_MSG_GCTRL + 1558),	//< NPC 호출 COMMISION 결과 ( Agent -> Field ) 

	NET_MSG_GCTRL_CREATE_INSTANT_MAP_FLD	= (NET_MSG_GCTRL + 1559),	//< 인스턴스 던젼 생성 AGENT -> FIELD
	NET_MSG_GCTRL_CREATE_INSTANT_MAP_FB		= (NET_MSG_GCTRL + 1560),	//< 인스턴스 던젼 생성 요청 피드백
	NET_MSG_GCTRL_CREATE_INSTANT_MAP_REQ	= (NET_MSG_GCTRL + 1561),	//< 인스턴스 던젼 요청 CLIENT -> FIELD -> AGENT
	NET_MSG_GCTRL_CREATE_INSTANT_MAP_DEL	= (NET_MSG_GCTRL + 1562),	//< 인스턴스 던젼 삭제 

	NET_MSG_GM_SHOP_INFO_REQ				= (NET_MSG_GCTRL + 1563),	//< 상점의 정보 요청(GM명령어)
	NET_MSG_GM_SHOP_INFO_FB					= (NET_MSG_GCTRL + 1564),	//< 상점의 정보 응답(GM명령어)

	NET_MSG_GCTRL_INVEN_ITEM_MIX			= (NET_MSG_GCTRL + 1565),	//< 아이템 조합 요청
	NET_MSG_GCTRL_INVEN_ITEM_MIX_FB			= (NET_MSG_GCTRL + 1566),	//< 아이템 조합 요청 결과

	NET_MSG_GCTRL_LIMITTIME_OVER			= (NET_MSG_GCTRL + 1567),	//< 진입 실패시 메시지

	NET_MSG_REQ_GATHERING					= (NET_MSG_GCTRL + 1570),	//< 채집 요청
	NET_MSG_REQ_GATHERING_FB				= (NET_MSG_GCTRL + 1571),	//< 채집 요청 결과
	NET_MSG_REQ_GATHERING_BRD				= (NET_MSG_GCTRL + 1572),	//< 채집 요청 결과
	NET_MSG_REQ_GATHERING_RESULT			= (NET_MSG_GCTRL + 1573),	//< 채집 결과
	NET_MSG_REQ_GATHERING_RESULT_BRD		= (NET_MSG_GCTRL + 1574),	//< 채집 결과
	NET_MSG_REQ_GATHERING_CANCEL			= (NET_MSG_GCTRL + 1575),	//< 채집중 중지
	NET_MSG_REQ_GATHERING_CANCEL_BRD		= (NET_MSG_GCTRL + 1576),	//< 채집중 중지

	NET_MSG_GCTRL_CLUB_AUTHORITY_REQ		= (NET_MSG_GCTRL + 1577),	//<	클럽마스터 권한 위임
	NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_ASK	= (NET_MSG_GCTRL + 1578),	//<	클럽마스터 권한 위임 질문
	NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_ANS	= (NET_MSG_GCTRL + 1579),	//<	클럽마스터 권한 위임 답변
	NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_FB		= (NET_MSG_GCTRL + 1580),	//<	클럽마스터 권한 위임 결과
	NET_MSG_GCTRL_CLUB_AUTHORITY_CLT		= (NET_MSG_GCTRL + 1581),	//<	클럽마스터 변경 내용
	NET_MSG_GCTRL_CLUB_AUTHORITY_BRD		= (NET_MSG_GCTRL + 1582),	//<	클럽마스터 변경 내용
	NET_MSG_GCTRL_CLUB_AUTHORITY_FLD		= (NET_MSG_GCTRL + 1583),	//<	클럽마스터 변경 내용 (필드)

	NET_MSG_GCTRL_CLUB_DEATHMATCH_REMAIN_BRD	= (NET_MSG_GCTRL + 1584),	//<	클럽데스매치 남은시간.
	NET_MSG_GCTRL_CLUB_DEATHMATCH_READY_FLD		= (NET_MSG_GCTRL + 1585),	//<	클럽데스매치 준비.
	NET_MSG_GCTRL_CLUB_DEATHMATCH_START_FLD		= (NET_MSG_GCTRL + 1586),	//<	클럽데스매치 시작.
	NET_MSG_GCTRL_CLUB_DEATHMATCH_START_BRD		= (NET_MSG_GCTRL + 1587),	//<	클럽데스매치 시작.

	NET_MSG_GCTRL_CLUB_DEATHMATCH_END_FLD		= (NET_MSG_GCTRL + 1588),	//<	클럽데스매치 종료.
	NET_MSG_GCTRL_CLUB_DEATHMATCH_END_BRD		= (NET_MSG_GCTRL + 1589),	//<	클럽데스매치 종료.

	NET_MSG_GCTRL_SERVER_CLUB_DEATHMATCH_INFO	= (NET_MSG_GCTRL + 1590),	//<	클럽데스매치 정보 전송 ( 접속시 ) 
	NET_MSG_GCTRL_REQ_SERVER_CLUB_DEATHMATCH_REMAIN_AG	= (NET_MSG_GCTRL + 1591),	//<	클럽데스매치 정보 전송 ( Agent에 남은시간 요청 )

	NET_MSG_GCTRL_CLUB_DEATHMATCH_POINT_UPDATE		= (NET_MSG_GCTRL + 1592),	//<	클럽데스매치 Kill/Death 상황 ( 자기만 )
	NET_MSG_GCTRL_CLUB_DEATHMATCH_MYRANK_UPDATE		= (NET_MSG_GCTRL + 1593),	//<	클럽데스매치 Kill/Death 상황 ( 자기만 )
	NET_MSG_GCTRL_CLUB_DEATHMATCH_RANKING_UPDATE	= (NET_MSG_GCTRL + 1594),	//<	클럽데스매치 랭킹 상황
	NET_MSG_GCTRL_CLUB_DEATHMATCH_RANKING_REQ		= (NET_MSG_GCTRL + 1595),	//<	클럽데스매치 상황 요청

	/*itemfood system, Juver, 2017/05/25 */
	NET_MSG_GCTRL_INVEN_CONSUME_FOOD				= (NET_MSG_GCTRL + 1596),
	NET_MSG_GCTRL_INVEN_CONSUME_FOOD_FB				= (NET_MSG_GCTRL + 1597),
	NET_MSG_GCTRL_FITEMFACT_BRD						= (NET_MSG_GCTRL + 1598),

	/*itemfood system, Juver, 2017/05/26 */
	NET_MSG_GCTRL_INVEN_UNLOCK_FOOD					= (NET_MSG_GCTRL + 1599),
	NET_MSG_GCTRL_INVEN_UNLOCK_FOOD_FB				= (NET_MSG_GCTRL + 1600),

	/*combatpoint logic, Juver, 2017/05/28 */
	NET_MSG_GCTRL_UPDATE_CP							= (NET_MSG_GCTRL + 1601),



	/*rightclick wear/unwear, Juver, 2017/06/24 */
	NET_MSG_GCTRL_REQ_INVEN_TO_WEAR					= (NET_MSG_GCTRL + 1604),
	NET_MSG_GCTRL_REQ_WEAR_TO_INVEN					= (NET_MSG_GCTRL + 1605),

	/*npc shop, Juver, 2017/07/27 */
	NET_MSG_GCTRL_NPCSHOP_PURCHASE_MONEY			= (NET_MSG_GCTRL + 1606),
	NET_MSG_GCTRL_NPCSHOP_PURCHASE_MONEY_FB			= (NET_MSG_GCTRL + 1607),

	/*vehicle booster system, Juver, 2017/08/10 */
	NET_MSG_VEHICLE_REQ_ENABLE_BOOSTER				= (NET_MSG_GCTRL + 1608),
	NET_MSG_VEHICLE_REQ_ENABLE_BOOSTER_FB			= (NET_MSG_GCTRL + 1609),
	NET_MSG_GCTRL_REQ_VEHICLE_BOOSTER				= (NET_MSG_GCTRL + 1610),

	NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_CHARGE		= (NET_MSG_GCTRL + 1611),
	NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_START		= (NET_MSG_GCTRL + 1612),
	NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_RESET		= (NET_MSG_GCTRL + 1613),

	NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_START_BRD	= (NET_MSG_GCTRL + 1614),
	NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_RESET_BRD	= (NET_MSG_GCTRL + 1615),

	/*contribution point, Juver, 2017/08/23 */
	NET_MSG_GCTRL_UPDATE_CONTRIBUTION_POINT			= (NET_MSG_GCTRL + 1616),

	/*activity point, Juver, 2017/08/23 */
	NET_MSG_GCTRL_UPDATE_ACTIVITY_POINT				= (NET_MSG_GCTRL + 1617 ),

	/*event map move, Juver, 2017/08/25 */
	NET_MSG_GCTRL_REQ_EVENT_MOVEMAP_FB				= (NET_MSG_GCTRL + 1620),
	NET_MSG_GCTRL_REQ_EVENT_MOVEMAP_FLD				= (NET_MSG_GCTRL + 1621),
	NET_MSG_GM_MOVE2_MAPPOS							= (NET_MSG_GCTRL + 1622),

	/*system buffs, Juver, 2017/09/05 */
	NET_MSG_GCTRL_SYSTEM_BUFF_BRD					= (NET_MSG_GCTRL + 1623),

	/*pvp tyranny, Juver, 2017/08/25 */
	NET_MSG_GCTRL_TYRANNY_A2FC_STATE_REGISTER		= (NET_MSG_GCTRL + 1630),
	NET_MSG_GCTRL_TYRANNY_A2FC_STATE_BATTLE			= (NET_MSG_GCTRL + 1631),
	NET_MSG_GCTRL_TYRANNY_A2FC_STATE_REWARD			= (NET_MSG_GCTRL + 1632),
	NET_MSG_GCTRL_TYRANNY_A2FC_STATE_ENDED			= (NET_MSG_GCTRL + 1633),
	NET_MSG_GCTRL_TYRANNY_A2C_TOBATTLE_TIME			= (NET_MSG_GCTRL + 1634),
	NET_MSG_GCTRL_TYRANNY_A2FC_NEXTSCHED			= (NET_MSG_GCTRL + 1635),
	NET_MSG_GCTRL_TYRANNY_F2A_BATTLEINFO_PC			= (NET_MSG_GCTRL + 1636),
	NET_MSG_GCTRL_TYRANNY_A2C_BATTLEINFO_PC			= (NET_MSG_GCTRL + 1637),
	NET_MSG_GCTRL_TYRANNY_C2A_REGISTER_REQ			= (NET_MSG_GCTRL + 1638),
	NET_MSG_GCTRL_TYRANNY_A2C_REGISTER_FB			= (NET_MSG_GCTRL + 1639),
	NET_MSG_GCTRL_TYRANNY_A2C_QUEUE_MOVED			= (NET_MSG_GCTRL + 1640),
	NET_MSG_GCTRL_TYRANNY_A2C_QUEUE_UPDATE			= (NET_MSG_GCTRL + 1641),
	NET_MSG_GCTRL_TYRANNY_A2F_MAP_MOVE				= (NET_MSG_GCTRL + 1642),
	NET_MSG_GCTRL_TYRANNY_C2A_REJOIN_REQ			= (NET_MSG_GCTRL + 1643),
	NET_MSG_GCTRL_TYRANNY_A2C_REJOIN_FB				= (NET_MSG_GCTRL + 1644),
	NET_MSG_GCTRL_TYRANNY_C2A_MAPEXIT_REQ			= (NET_MSG_GCTRL + 1645),
	NET_MSG_GCTRL_TYRANNY_A2F_PLAYER_DATA			= (NET_MSG_GCTRL + 1646),
	NET_MSG_GCTRL_TYRANNY_F2A_TOWER_CAPTURE			= (NET_MSG_GCTRL + 1647),
	NET_MSG_GCTRL_TYRANNY_A2F_TOWER_CAPTURE			= (NET_MSG_GCTRL + 1648),
	NET_MSG_GCTRL_TYRANNY_F2C_TOWER_CAPTURE			= (NET_MSG_GCTRL + 1649),
	NET_MSG_GCTRL_TYRANNY_C2A_TOWER_INFO_REQ		= (NET_MSG_GCTRL + 1650),
	NET_MSG_GCTRL_TYRANNY_A2C_TOWER_INFO			= (NET_MSG_GCTRL + 1651),
	NET_MSG_GCTRL_TYRANNY_F2A_TOWER_SCORE			= (NET_MSG_GCTRL + 1652),
	NET_MSG_GCTRL_TYRANNY_A2F_TOWER_WINNER			= (NET_MSG_GCTRL + 1653),
	NET_MSG_GCTRL_TYRANNY_F2C_TOWER_WINNER			= (NET_MSG_GCTRL + 1654),
	NET_MSG_GCTRL_TYRANNY_C2F_REVIVE_REQ			= (NET_MSG_GCTRL + 1655),
	NET_MSG_GCTRL_TYRANNY_F2C_RANKING_UPDATE		= (NET_MSG_GCTRL + 1656),
	NET_MSG_GCTRL_TYRANNY_F2C_RANKING_END			= (NET_MSG_GCTRL + 1657),
	NET_MSG_GCTRL_TYRANNY_F2A_RANKINFO_PC			= (NET_MSG_GCTRL + 1658),
	NET_MSG_GCTRL_TYRANNY_A2C_RANKINFO_PC			= (NET_MSG_GCTRL + 1659),
	NET_MSG_GCTRL_TYRANNY_A2F_BUFF_RESET			= (NET_MSG_GCTRL + 1660),
	NET_MSG_GCTRL_TYRANNY_A2F_BUFF_REWARD			= (NET_MSG_GCTRL + 1661),
	NET_MSG_GCTRL_TYRANNY_A2F_BUFF_BATTLE			= (NET_MSG_GCTRL + 1662),
	//NET_MSG_GCTRL_TYRANNY_A2FC_REWARD_BUFF_INFO		= (NET_MSG_GCTRL + 1663),
	NET_MSG_GCTRL_TYRANNY_A2FC_SET_REBIRTHTIME		= (NET_MSG_GCTRL + 1663),
	NET_MSG_GCTRL_TYRANNY_C2A_LATE_JOIN_REQ			= (NET_MSG_GCTRL + 1664),
	NET_MSG_GCTRL_TYRANNY_A2C_LATE_JOIN_FB			= (NET_MSG_GCTRL + 1665),

	/*pvp tyranny, Juver, 2017/08/25 */
	NET_MSG_GCTRL_TYRANNY_CROW_OWNER				= (NET_MSG_GCTRL + 1670),
	NET_MSG_GCTRL_TYRANNY_CROW_DAMAGE				= (NET_MSG_GCTRL + 1671),

	/*item exchange, Juver, 2017/10/13 */
	NET_MSG_GCTRL_NPC_ITEM_EXCHANGE_TRADE			= (NET_MSG_GCTRL + 1672),
	NET_MSG_GCTRL_NPC_ITEM_EXCHANGE_TRADE_FB		= (NET_MSG_GCTRL + 1673),

	/*product item, Juver, 2017/10/18 */
	NET_MSG_GCTRL_ITEM_COMPOUND_START				= (NET_MSG_GCTRL + 1674),
	NET_MSG_GCTRL_ITEM_COMPOUND_START_FB			= (NET_MSG_GCTRL + 1675),
	NET_MSG_GCTRL_ITEM_COMPOUND_PROCESS_FB			= (NET_MSG_GCTRL + 1676),

	/*activity system, Juver, 2017/10/30 */
	NET_MSG_GCTRL_ACTIVITY_COMPLETE_BRD				= (NET_MSG_GCTRL + 1677 ),
	NET_MSG_GCTRL_ACTIVITY_UPDATE					= (NET_MSG_GCTRL + 1678 ),
	NET_MSG_GCTRL_ACTIVITY_COMPLETE					= (NET_MSG_GCTRL + 1679 ),

	NET_MSG_GCTRL_ACTIVITY_NOTIFY_AGENT				= (NET_MSG_GCTRL + 1680 ),
	NET_MSG_GCTRL_ACTIVITY_NOTIFY_CLIENT			= (NET_MSG_GCTRL + 1681 ),

	/*activity system, Juver, 2017/11/05 */
	NET_MSG_GCTRL_CHARACTER_BADGE_CHANGE			= (NET_MSG_GCTRL + 1682 ),
	NET_MSG_GCTRL_CHARACTER_BADGE_CHANGE_FB			= (NET_MSG_GCTRL + 1683 ),
	NET_MSG_GCTRL_CHARACTER_BADGE_CHANGE_BRD		= (NET_MSG_GCTRL + 1684 ),

	/*bike color , Juver, 2017/11/13 */
	NET_MSG_VEHICLE_REQ_CHANGE_COLOR				= (NET_MSG_GCTRL + 1688 ),
	NET_MSG_VEHICLE_REQ_CHANGE_COLOR_FB				= (NET_MSG_GCTRL + 1689 ),
	NET_MSG_VEHICLE_REQ_CHANGE_COLOR_BRD			= (NET_MSG_GCTRL + 1690 ),

	/*pk info, Juver, 2017/11/17 */
	NET_MSG_GCTRL_UPDATE_PK_KILL					= (NET_MSG_GCTRL + 1691 ),
	NET_MSG_GCTRL_UPDATE_PK_DEATH					= (NET_MSG_GCTRL + 1692 ),

	/*rv card, Juver, 2017/11/25 */
	NET_MSG_GCTRL_INVEN_RANDOM_OPTION_CHANGE		= (NET_MSG_GCTRL + 1693),
	NET_MSG_GCTRL_INVEN_RANDOM_OPTION_CHANGE_FB		= (NET_MSG_GCTRL + 1694),

	/*nondrop card, Juver, 2017/11/26 */
	NET_MSG_GCTRL_INVEN_NONDROP_CARD				= (NET_MSG_GCTRL + 1695),
	NET_MSG_GCTRL_INVEN_NONDROP_CARD_FB				= (NET_MSG_GCTRL + 1696),

	/*regional chat, Juver, 2017/12/06 */
	NET_MSG_GCTRL_REGIONAL_CHAT_PAY					= (NET_MSG_GCTRL + 1697),

	/*trade lock, Juver, 2018/01/02 */
	NET_MSG_GCTRL_TRADE_LOCK						= (NET_MSG_GCTRL + 1698),
	NET_MSG_GCTRL_TRADE_LOCK_TAR					= (NET_MSG_GCTRL + 1699),

	/*private market set, Juver, 2018/01/02 */
	NET_MSG_GM_SET_PRIVATE_MARKET					= (NET_MSG_GCTRL + 1700),
	NET_MSG_GM_SET_PRIVATE_MARKET_BRD				= (NET_MSG_GCTRL + 1701),

	/*megaphone set, Juver, 2018/01/02 */
	NET_MSG_GM_SET_MEGAPHONE						= (NET_MSG_GCTRL + 1702),
	NET_MSG_GM_SET_MEGAPHONE_BRD					= (NET_MSG_GCTRL + 1703),

	/*change scale card, Juver, 2018/01/04 */
	NET_MSG_GCTRL_INVEN_SCALE_CHANGE				= (NET_MSG_GCTRL + 1704),
	NET_MSG_GCTRL_INVEN_SCALE_CHANGE_FB				= (NET_MSG_GCTRL + 1705),
	NET_MSG_GCTRL_INVEN_SCALE_CHANGE_BRD			= (NET_MSG_GCTRL + 1706),

	/*item color, Juver, 2018/01/10 */
	NET_MSG_GCTRL_INVEN_ITEMCOLOR_CHANGE			= (NET_MSG_GCTRL + 1707),
	NET_MSG_GCTRL_INVEN_ITEMCOLOR_CHANGE_FB			= (NET_MSG_GCTRL + 1708),
	NET_MSG_GCTRL_INVEN_ITEMCOLOR_CHANGE_BRD		= (NET_MSG_GCTRL + 1709),

	/*item wrapper, Juver, 2018/01/11 */
	NET_MSG_GCTRL_INVEN_WRAP						= (NET_MSG_GCTRL + 1710),
	NET_MSG_GCTRL_INVEN_WRAP_FB						= (NET_MSG_GCTRL + 1711),
	NET_MSG_GCTRL_INVEN_UNWRAP						= (NET_MSG_GCTRL + 1712),
	NET_MSG_GCTRL_INVEN_UNWRAP_FB					= (NET_MSG_GCTRL + 1713),

	/*change school card, Juver, 2018/01/12 */
	NET_MSG_GCTRL_INVEN_CHANGE_SCHOOL				= (NET_MSG_GCTRL + 1714),
	NET_MSG_GCTRL_INVEN_CHANGE_SCHOOL_FB			= (NET_MSG_GCTRL + 1715),
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	NET_MSG_GCTRL_TYRANNY_RANK_POINT_UPDATE			= (NET_MSG_GCTRL + 1716),
	NET_MSG_GCTRL_TYRANNY_MYRANK_UPDATE				= (NET_MSG_GCTRL + 1717),
	NET_MSG_GCTRL_TYRANNY_RANKING_UPDATE			= (NET_MSG_GCTRL + 1718),
	NET_MSG_GCTRL_TYRANNY_RANKING_REQ				= (NET_MSG_GCTRL + 1719),

	NET_MSG_GCTRL_TYRANNY_MYRANK_RESU_UPDATE		= (NET_MSG_GCTRL + 1720),
	NET_MSG_GCTRL_TYRANNY_RANKING_RESU_UPDATE		= (NET_MSG_GCTRL + 1721),
	NET_MSG_GCTRL_TYRANNY_RANKING_RESU_REQ			= (NET_MSG_GCTRL + 1722),

	NET_MSG_GCTRL_TYRANNY_CLUB_RANK_POINT_UPDATE	= (NET_MSG_GCTRL + 1723),
	NET_MSG_GCTRL_TYRANNY_CLUB_MYRANK_UPDATE		= (NET_MSG_GCTRL + 1724),
	NET_MSG_GCTRL_TYRANNY_CLUB_RANKING_UPDATE		= (NET_MSG_GCTRL + 1725),
	NET_MSG_GCTRL_TYRANNY_CLUB_RANKING_REQ			= (NET_MSG_GCTRL + 1726),

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	NET_MSG_GCTRL_CLUBWAR_RANK_POINT_UPDATE			= (NET_MSG_GCTRL + 1727),
	NET_MSG_GCTRL_CLUBWAR_MYRANK_UPDATE				= (NET_MSG_GCTRL + 1728),
	NET_MSG_GCTRL_CLUBWAR_RANKING_UPDATE			= (NET_MSG_GCTRL + 1729),
	NET_MSG_GCTRL_CLUBWAR_RANKING_REQ				= (NET_MSG_GCTRL + 1730),


	/*item transfer card, Juver, 2018/01/18 */
	NET_MSG_GCTRL_INVEN_ITEM_TRANSFER			= (NET_MSG_GCTRL + 1731),
	NET_MSG_GCTRL_INVEN_ITEM_TRANSFER_FB		= (NET_MSG_GCTRL + 1732),

	/*school wars, Juver, 2018/01/19 */
	NET_MSG_GCTRL_SCHOOLWARS_A2FC_STATE_REGISTER		= (NET_MSG_GCTRL + 1740),
	NET_MSG_GCTRL_SCHOOLWARS_A2FC_STATE_BATTLE			= (NET_MSG_GCTRL + 1741),
	NET_MSG_GCTRL_SCHOOLWARS_A2FC_STATE_REWARD			= (NET_MSG_GCTRL + 1742),
	NET_MSG_GCTRL_SCHOOLWARS_A2FC_STATE_ENDED			= (NET_MSG_GCTRL + 1743),
	NET_MSG_GCTRL_SCHOOLWARS_A2C_TOBATTLE_TIME			= (NET_MSG_GCTRL + 1744),
	NET_MSG_GCTRL_SCHOOLWARS_A2FC_NEXTSCHED				= (NET_MSG_GCTRL + 1745),
	NET_MSG_GCTRL_SCHOOLWARS_F2A_BATTLEINFO_PC			= (NET_MSG_GCTRL + 1746),
	NET_MSG_GCTRL_SCHOOLWARS_A2C_BATTLEINFO_PC			= (NET_MSG_GCTRL + 1747),
	NET_MSG_GCTRL_SCHOOLWARS_C2A_REGISTER_REQ			= (NET_MSG_GCTRL + 1748),
	NET_MSG_GCTRL_SCHOOLWARS_A2C_REGISTER_FB			= (NET_MSG_GCTRL + 1749),
	NET_MSG_GCTRL_SCHOOLWARS_A2C_QUEUE_MOVED			= (NET_MSG_GCTRL + 1750),
	NET_MSG_GCTRL_SCHOOLWARS_A2C_QUEUE_UPDATE			= (NET_MSG_GCTRL + 1751),
	NET_MSG_GCTRL_SCHOOLWARS_C2A_REJOIN_REQ				= (NET_MSG_GCTRL + 1752),
	NET_MSG_GCTRL_SCHOOLWARS_A2C_REJOIN_FB				= (NET_MSG_GCTRL + 1753),
	NET_MSG_GCTRL_SCHOOLWARS_A2F_MAP_MOVE				= (NET_MSG_GCTRL + 1754),
	NET_MSG_GCTRL_SCHOOLWARS_C2A_MAPEXIT_REQ			= (NET_MSG_GCTRL + 1755),
	NET_MSG_GCTRL_SCHOOLWARS_A2F_PLAYER_DATA			= (NET_MSG_GCTRL + 1756),
	NET_MSG_GCTRL_SCHOOLWARS_C2A_SCORE_INFO_REQ			= (NET_MSG_GCTRL + 1757),
	NET_MSG_GCTRL_SCHOOLWARS_A2C_SCORE_INFO				= (NET_MSG_GCTRL + 1758),
	NET_MSG_GCTRL_SCHOOLWARS_F2A_SCORE_UPDATE			= (NET_MSG_GCTRL + 1759),
	NET_MSG_GCTRL_SCHOOLWARS_A2F_SCORE_UPDATE			= (NET_MSG_GCTRL + 1760),
	NET_MSG_GCTRL_SCHOOLWARS_F2C_SCORE_UPDATE			= (NET_MSG_GCTRL + 1761),
	NET_MSG_GCTRL_SCHOOLWARS_C2F_REVIVE_REQ				= (NET_MSG_GCTRL + 1762),
	NET_MSG_GCTRL_SCHOOLWARS_A2C_RANKINFO_PC			= (NET_MSG_GCTRL + 1763),
	NET_MSG_GCTRL_SCHOOLWARS_A2F_SCORE_WINNER			= (NET_MSG_GCTRL + 1764),
	NET_MSG_GCTRL_SCHOOLWARS_F2C_SCORE_WINNER			= (NET_MSG_GCTRL + 1765),
	NET_MSG_GCTRL_SCHOOLWARS_F2C_RANKING_UPDATE			= (NET_MSG_GCTRL + 1766),
	NET_MSG_GCTRL_SCHOOLWARS_F2C_RANKING_END			= (NET_MSG_GCTRL + 1767),
	NET_MSG_GCTRL_SCHOOLWARS_F2A_RANKINFO_PC			= (NET_MSG_GCTRL + 1768),
	NET_MSG_GCTRL_SCHOOLWARS_A2C_SCORE_UPDATE			= (NET_MSG_GCTRL + 1769),
	NET_MSG_GCTRL_SCHOOLWARS_F2A_MOB_KILL_UPDATE		= (NET_MSG_GCTRL + 1770),
	NET_MSG_GCTRL_SCHOOLWARS_A2F_MOB_KILL_NOTICE		= (NET_MSG_GCTRL + 1771),
	NET_MSG_GCTRL_SCHOOLWARS_F2C_MOB_KILL_NOTICE		= (NET_MSG_GCTRL + 1772),


	/*pvp capture the flag, Juver, 2018/01/24 */
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2FC_STATE_REGISTER	= (NET_MSG_GCTRL + 1780),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2FC_STATE_BATTLE	= (NET_MSG_GCTRL + 1781),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2FC_STATE_REWARD	= (NET_MSG_GCTRL + 1782),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2FC_STATE_ENDED		= (NET_MSG_GCTRL + 1783),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2C_TOBATTLE_TIME	= (NET_MSG_GCTRL + 1784),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2FC_NEXTSCHED		= (NET_MSG_GCTRL + 1785),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_BATTLEINFO_PC	= (NET_MSG_GCTRL + 1786),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2C_BATTLEINFO_PC	= (NET_MSG_GCTRL + 1787),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_REGISTER_REQ		= (NET_MSG_GCTRL + 1788),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2C_REGISTER_FB		= (NET_MSG_GCTRL + 1789),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2C_PLAYER_NUM		= (NET_MSG_GCTRL + 1790),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2F_PLAYER_TEAM		= (NET_MSG_GCTRL + 1791),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_PLAYER_TEAM_BRD	= (NET_MSG_GCTRL + 1792),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_REJOIN_REQ		= (NET_MSG_GCTRL + 1793),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2C_REJOIN_FB		= (NET_MSG_GCTRL + 1794),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2F_MAP_MOVE			= (NET_MSG_GCTRL + 1795),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_MAPEXIT_REQ		= (NET_MSG_GCTRL + 1796),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2F_PLAYER_DATA		= (NET_MSG_GCTRL + 1797),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2F_REVIVE_REQ		= (NET_MSG_GCTRL + 1798),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2C_RANKINFO_PC		= (NET_MSG_GCTRL + 1799),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_RANKING_UPDATE	= (NET_MSG_GCTRL + 1800),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_RANKING_END		= (NET_MSG_GCTRL + 1801),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_RANKINFO_PC		= (NET_MSG_GCTRL + 1802),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2A_SCORE_INFO_REQ	= (NET_MSG_GCTRL + 1803),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2C_SCORE_INFO		= (NET_MSG_GCTRL + 1804),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_KILL_UPDATE		= (NET_MSG_GCTRL + 1805),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_CAPTURE_UPDATE	= (NET_MSG_GCTRL + 1807),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2F_CAPTURE_UPDATE	= (NET_MSG_GCTRL + 1808),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_CAPTURE_UPDATE	= (NET_MSG_GCTRL + 1809),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_A2F_WINNER			= (NET_MSG_GCTRL + 1810),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_WINNER			= (NET_MSG_GCTRL + 1811),

	NET_MSG_GCTRL_CAPTURE_THE_FLAG_C2F_FLAG_ACTION_CAPTURE	= (NET_MSG_GCTRL + 1812),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2A_FLAG_HOLD			= (NET_MSG_GCTRL + 1813),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_FLAG_HOLD			= (NET_MSG_GCTRL + 1814),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_FLAG_HOLD_BRD		= (NET_MSG_GCTRL + 1815),
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_FLAG_POSITION		= (NET_MSG_GCTRL + 1816),

	NET_MSG_GCTRL_CAPTURE_THE_FLAG_CROW_HIDE		= (NET_MSG_GCTRL + 1820),

	/* car, cart color, Juver, 2018/02/14 */
	NET_MSG_VEHICLE_REQ_CHANGE_CAR_COLOR			= (NET_MSG_GCTRL + 1821),
	NET_MSG_VEHICLE_REQ_CHANGE_CAR_COLOR_FB			= (NET_MSG_GCTRL + 1822),
	NET_MSG_VEHICLE_REQ_CHANGE_CAR_COLOR_BRD		= (NET_MSG_GCTRL + 1823),

	/* booster all vehicle, Juver, 2018/02/14 */
	NET_MSG_ALLVEHICLE_REQ_ENABLE_BOOSTER			= (NET_MSG_GCTRL + 1824),
	NET_MSG_ALLVEHICLE_REQ_ENABLE_BOOSTER_FB		= (NET_MSG_GCTRL + 1825),

	/* additional gm commands, Juver, 2018/03/01 */
	//NET_MSG_GCTRL_GM_COMMAND_GET_ITEM				= (NET_MSG_GCTRL + 1826),
	//NET_MSG_GCTRL_GM_COMMAND_GET_ITEM_FB			= (NET_MSG_GCTRL + 1827),
	NET_MSG_GCTRL_GM_COMMAND_INVEN_OPEN				= (NET_MSG_GCTRL + 1828),
	NET_MSG_GCTRL_GM_COMMAND_INVEN_OPEN_FB			= (NET_MSG_GCTRL + 1829),
	NET_MSG_GCTRL_GM_COMMAND_GET_SKILL				= (NET_MSG_GCTRL + 1830),
	NET_MSG_GCTRL_GM_COMMAND_GET_SKILL_FB			= (NET_MSG_GCTRL + 1831),
	NET_MSG_GCTRL_GM_COMMAND_GET_SKILL2_FB			= (NET_MSG_GCTRL + 1832),
	NET_MSG_GCTRL_GM_COMMAND_GET_LEVEL				= (NET_MSG_GCTRL + 1833),
	NET_MSG_GCTRL_GM_COMMAND_GET_LEVEL_FB			= (NET_MSG_GCTRL + 1834),
	NET_MSG_GCTRL_GM_COMMAND_GET_STATS				= (NET_MSG_GCTRL + 1835),
	NET_MSG_GCTRL_GM_COMMAND_GET_STATS_FB			= (NET_MSG_GCTRL + 1836),
	NET_MSG_GCTRL_GM_COMMAND_GET_STATSP				= (NET_MSG_GCTRL + 1837),
	NET_MSG_GCTRL_GM_COMMAND_GET_STATSP_FB			= (NET_MSG_GCTRL + 1838),
	NET_MSG_GCTRL_GM_COMMAND_GET_SKILLP				= (NET_MSG_GCTRL + 1839),
	NET_MSG_GCTRL_GM_COMMAND_GET_SKILLP_FB			= (NET_MSG_GCTRL + 1840),

	/*modern character window, Juver, 2018/03/17 */
	NET_MSG_GCTRL_REQ_STATSUP_MODERN				= (NET_MSG_GCTRL + 1841),
	NET_MSG_GCTRL_REQ_STATSUP_MODERN_FB				= (NET_MSG_GCTRL + 1842),

	/*hackshield implementation, Juver, 2018/06/19 */
	NET_MSG_GCTRL_HS_CALLBACK						= (NET_MSG_GCTRL + 1843),
	NET_MSG_GCTRL_HS_CLIENT_CLOSE					= (NET_MSG_GCTRL + 1844),

	//fastrevive
	NET_MSG_GCTRL_DEATHTIMER_FB_FLD					= (NET_MSG_GCTRL + 1845),
	NET_MSG_GCTRL_DEATHTIMER_FB_AG					= (NET_MSG_GCTRL + 1846),
	NET_MSG_GCTRL_DEATHTIMER_AG						= (NET_MSG_GCTRL + 1847),

	/*item random option rebuild, Juver, 2018/07/04 */
	NET_MSG_GCTRL_INVEN_ITEM_RANDOM_OPTION_REBUILD			= (NET_MSG_GCTRL + 1850),
	NET_MSG_GCTRL_INVEN_ITEM_RANDOM_OPTION_REBUILD_FB		= (NET_MSG_GCTRL + 1851),

	/*inventory sort, Juver, 2018/07/07 */
	NET_MSG_GCTRL_INVEN_ITEM_SORT					= (NET_MSG_GCTRL + 1852),
	NET_MSG_GCTRL_INVEN_ITEM_SORT_FB				= (NET_MSG_GCTRL + 1853),
	NET_MSG_GCTRL_INVEN_ITEM_SORT_UPDATE			= (NET_MSG_GCTRL + 1854),

	/*buffs manual remove, Juver, 2018/08/14 */
	NET_MSG_GCTRL_BUFF_REMOVE						= (NET_MSG_GCTRL + 1855),

	/*specific item box, Juver, 2018/09/03 */
	NET_MSG_GCTRL_INVEN_REQ_OPEN_ITEM_BOX_SPECIFIC			= (NET_MSG_GCTRL + 1856),
	NET_MSG_GCTRL_INVEN_REQ_OPEN_ITEM_BOX_SPECIFIC_FB		= (NET_MSG_GCTRL + 1857),

	/*skill teleport, Juver, 2018/09/06 */
	NET_MSG_GCTRL_SKILL_TELEPORT_BRD						= (NET_MSG_GCTRL + 1858),

	/*extreme auto tab, Juver, 2018/09/10 */
	NET_MSG_GCTRL_REQ_SKILL_TAB_CHANGE						= (NET_MSG_GCTRL + 1859),
	NET_MSG_GCTRL_REQ_SKILL_TAB_CHANGE_FB					= (NET_MSG_GCTRL + 1860),
	NET_MSG_GCTRL_REQ_SKILL_TAB_CHANGE_BRD					= (NET_MSG_GCTRL + 1861),

	/*manual lunchbox remove, Juver, 2018/09/19 */
	NET_MSG_GCTRL_LUNCHBOX_MANUAL_REMOVE					= (NET_MSG_GCTRL + 1862),
	NET_MSG_GCTRL_LUNCHBOX_MANUAL_REMOVE_BRD				= (NET_MSG_GCTRL + 1863),

	/*ecall function Arc Development 08-16-2024*/
	NET_MSG_CALL_GUILD_MEM						= (NET_MSG_GCTRL + 1864),
	NET_MSG_CALL_MOVE2CHAR_POS					= (NET_MSG_GCTRL + 1865),
	NET_MSG_CALL_MOVE2CHAR_AG					= (NET_MSG_GCTRL + 1866),

	/*character disconnect function, EJCode, 2018/11/25 */
	NET_MSG_GCTRL_CHARACTER_DISCONNECT_FIELD_TO_AGENT		= (NET_MSG_GCTRL + 1867),
	NET_MSG_GCTRL_CHARACTER_DISCONNECT_AGENT_TO_CHAR		= (NET_MSG_GCTRL + 1868),
	NET_MSG_GCTRL_CHARACTER_DISCONNECT_AGENT_TO_FIELD		= (NET_MSG_GCTRL + 1869),

	/*client tick count check, EJCode, 2018/11/26 */
	NET_MSG_GCTRL_CLIENT_TICK_COUNT_CHECK_REQUEST			= (NET_MSG_GCTRL + 1870 ),
	NET_MSG_GCTRL_CLIENT_TICK_COUNT_CHECK_RESULT			= (NET_MSG_GCTRL + 1871 ),

	/*gm command send item, Juver, 2019/03/23 */
	NET_MSG_GCTRL_GM_COMMAND_C2A_SEND_ITEM					= (NET_MSG_GCTRL + 1872),
	NET_MSG_GCTRL_GM_COMMAND_A2C_SEND_ITEM_FB				= (NET_MSG_GCTRL + 1873),
	NET_MSG_GCTRL_GM_COMMAND_A2F_SEND_ITEM					= (NET_MSG_GCTRL + 1874),
	NET_MSG_GCTRL_GM_COMMAND_A2C_SEND_ITEM_NOTICE			= (NET_MSG_GCTRL + 1875),
	NET_MSG_GCTRL_GM_COMMAND_F2A_SEND_ITEM_FB				= (NET_MSG_GCTRL + 1876),


	NET_MSG_GCTRL_CAPTURE_PUTON_DROP						= (NET_MSG_GCTRL + 1877),

	NET_MSG_GCTRL_GM_COMMAND_GET_CROW_TIME					= (NET_MSG_GCTRL + 1878),
	NET_MSG_GCTRL_GM_COMMAND_GET_CROW_TIME_FB				= (NET_MSG_GCTRL + 1879),


	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY				= (NET_MSG_GCTRL + 1881),
	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY_FB			= (NET_MSG_GCTRL + 1882),
	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY_BRD			= (NET_MSG_GCTRL + 1883),
	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY_FLD			= (NET_MSG_GCTRL + 1884),

	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR			= (NET_MSG_GCTRL + 1886),
	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR_FB			= (NET_MSG_GCTRL + 1887),
	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR_BRD		= (NET_MSG_GCTRL + 1888),
	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR_FLD		= (NET_MSG_GCTRL + 1889),

	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG		= (NET_MSG_GCTRL + 1891),
	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG_FB	= (NET_MSG_GCTRL + 1892),
	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG_BRD	= (NET_MSG_GCTRL + 1893),
	NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG_FLD	= (NET_MSG_GCTRL + 1894),



	NET_MSG_GCTRL_CLUB_SCORE_KILL_FLD_2_AGT						= (NET_MSG_GCTRL + 1900),
	NET_MSG_GCTRL_CLUB_SCORE_RESU_FLD_2_AGT						= (NET_MSG_GCTRL + 1901),

	NET_MSG_GCTRL_CLUB_SCORE_RANKING_START_A2C					= (NET_MSG_GCTRL + 1902),
	NET_MSG_GCTRL_CLUB_SCORE_RANKING_UPDATE_A2C					= (NET_MSG_GCTRL + 1903),
	NET_MSG_GCTRL_CLUB_SCORE_RANKING_END_A2C					= (NET_MSG_GCTRL + 1904),
	NET_MSG_GCTRL_CLUB_SCORE_SEND_SCORE_A2F						= (NET_MSG_GCTRL + 1905),

	NET_MSG_GCTRL_UPDATE_WAR_CHIPS								= (NET_MSG_GCTRL + 1920),
	NET_MSG_GCTRL_UPDATE_GAME_POINTS							= (NET_MSG_GCTRL + 1921),


	NET_MSG_GCTRL_CURRENCY_SHOP_PURCHASE						= (NET_MSG_GCTRL + 1925),
	NET_MSG_GCTRL_CURRENCY_SHOP_PURCHASE_FB						= (NET_MSG_GCTRL + 1926),

	NET_MSG_GCTRL_GM_COMMAND_KICKOUT							= (NET_MSG_GCTRL + 1930),
	NET_MSG_GCTRL_GM_COMMAND_KICKOUT_FB							= (NET_MSG_GCTRL + 1931),

	/* personal lock system, Juver, 2019/12/06 */
	NET_MSG_GCTRL_PERSONAL_LOCK_REQUEST							= (NET_MSG_GCTRL + 1935),
	NET_MSG_GCTRL_PERSONAL_LOCK_REQUEST_FB						= (NET_MSG_GCTRL + 1936),
	NET_MSG_GCTRL_PERSONAL_LOCK_CREATE_PIN						= (NET_MSG_GCTRL + 1937),
	NET_MSG_GCTRL_PERSONAL_LOCK_CREATE_PIN_FB					= (NET_MSG_GCTRL + 1938),
	NET_MSG_GCTRL_PERSONAL_LOCK_INPUT_PIN						= (NET_MSG_GCTRL + 1939),
	NET_MSG_GCTRL_PERSONAL_LOCK_INPUT_PIN_FB					= (NET_MSG_GCTRL + 1940),
	NET_MSG_GCTRL_PERSONAL_LOCK_RESET_PIN						= (NET_MSG_GCTRL + 1941),
	NET_MSG_GCTRL_PERSONAL_LOCK_RESET_PIN_FB					= (NET_MSG_GCTRL + 1942),
	NET_MSG_GCTRL_PERSONAL_LOCK_CHANGE_PIN						= (NET_MSG_GCTRL + 1943),
	NET_MSG_GCTRL_PERSONAL_LOCK_CHANGE_PIN_FB					= (NET_MSG_GCTRL + 1944),
	NET_MSG_GCTRL_PERSONAL_LOCK_RECOVER_PIN						= (NET_MSG_GCTRL + 1945),
	NET_MSG_GCTRL_PERSONAL_LOCK_RECOVER_PIN_FB					= (NET_MSG_GCTRL + 1946),


	/* party finder, Juver, 2020/01/03 */
	NET_MSG_GCTRL_PARTY_FINDER_SEARCH							= (NET_MSG_GCTRL + 1950),	
	NET_MSG_GCTRL_PARTY_FINDER_SEARCH_FB						= (NET_MSG_GCTRL + 1951),	
	NET_MSG_GCTRL_PARTY_FINDER_RESULT_UPDATE					= (NET_MSG_GCTRL + 1952),	
	NET_MSG_GCTRL_PARTY_FINDER_RESULT_DONE						= (NET_MSG_GCTRL + 1953),
	NET_MSG_GCTRL_PARTY_FINDER_JOIN								= (NET_MSG_GCTRL + 1954),	
	NET_MSG_GCTRL_PARTY_FINDER_JOIN_FB							= (NET_MSG_GCTRL + 1955),	
	NET_MSG_GCTRL_PARTY_FINDER_QUESTION							= (NET_MSG_GCTRL + 1956),	
	NET_MSG_GCTRL_PARTY_FINDER_REPLY							= (NET_MSG_GCTRL + 1957),	

	/* party charid/charname, Juver, 2020/03/02 */
	NET_MSG_GCTRL_PARTY_LURE_CHAR_ID							= (NET_MSG_GCTRL + 1965),
	NET_MSG_GCTRL_PARTY_LURE_CHAR_NAME							= (NET_MSG_GCTRL + 1966),

	/* charinfoview, Juver, 2020/03/03 */
	NET_MSG_GCTRL_REQ_CHARINFO_GAEA								= (NET_MSG_GCTRL + 1970),
	NET_MSG_GCTRL_REQ_CHARINFO_CHAR_ID							= (NET_MSG_GCTRL + 1971),
	NET_MSG_GCTRL_REQ_CHARINFO_CHAR_NAME						= (NET_MSG_GCTRL + 1972),
	NET_MSG_GCTRL_REQ_CHARINFO_FB								= (NET_MSG_GCTRL + 1973),
	NET_MSG_GCTRL_REQ_CHARINFO_FLD								= (NET_MSG_GCTRL + 1974),
	NET_MSG_GCTRL_REQ_CHARINFO_AGT								= (NET_MSG_GCTRL + 1975),
	NET_PRIVATE_STATS_REQ										= (NET_MSG_GCTRL + 1976),
	
	NET_MSG_GCTRL_GM_PCID										= (NET_MSG_GCTRL + 1980),
	NET_MSG_GCTRL_GM_PCID_FB									= (NET_MSG_GCTRL + 1981),
	NET_MSG_GCTRL_GM_PCID_FB2									= (NET_MSG_GCTRL + 1982),
	NET_MSG_GCTRL_GM_PCID_TO_CHAR								= (NET_MSG_GCTRL + 1983),
	NET_MSG_GCTRL_GM_PCID_REPLY									= (NET_MSG_GCTRL + 1984),
	NET_MSG_GCTRL_GM_PCID_REPLY2								= (NET_MSG_GCTRL + 1985),

	/* user flag restricted, Juver, 2020/04/21 */
	NET_MSG_GCTRL_GM_USER_RESTRICT								= (NET_MSG_GCTRL + 1990),
	NET_MSG_GCTRL_GM_USER_RESTRICT_FB							= (NET_MSG_GCTRL + 1991),
	NET_MSG_GCTRL_GM_USER_RESTRICT_FLD							= (NET_MSG_GCTRL + 1992),
	NET_MSG_GCTRL_GM_USER_RESTRICT_UPDATE						= (NET_MSG_GCTRL + 1993),

	/* gm command inven clear, Juver, 2020/05/09 */
	NET_MSG_GCTRL_GM_INVEN_CLEAR								= (NET_MSG_GCTRL + 1996),
	NET_MSG_GCTRL_GM_INVEN_CLEAR_FB								= (NET_MSG_GCTRL + 1997),

	/* pvp club death match, Juver, 2020/11/10 */
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REGISTER		= (NET_MSG_GCTRL + 2000),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_BATTLE		= (NET_MSG_GCTRL + 2001),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_REWARD		= (NET_MSG_GCTRL + 2002),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_STATE_ENDED			= (NET_MSG_GCTRL + 2003),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_TOBATTLE_TIME		= (NET_MSG_GCTRL + 2004),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2FC_NEXTSCHED			= (NET_MSG_GCTRL + 2005),

	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2A_BATTLEINFO_PC		= (NET_MSG_GCTRL + 2006),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_BATTLEINFO_PC		= (NET_MSG_GCTRL + 2007),

	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_PLAYER_NUM			= (NET_MSG_GCTRL + 2008),

	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_REGISTER_REQ			= (NET_MSG_GCTRL + 2009),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_REGISTER_FB			= (NET_MSG_GCTRL + 2010),

	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2F_MAP_MOVE				= (NET_MSG_GCTRL + 2011),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_REJOIN_REQ			= (NET_MSG_GCTRL + 2012),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_REJOIN_FB			= (NET_MSG_GCTRL + 2013),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2A_MAPEXIT_REQ			= (NET_MSG_GCTRL + 2014),

	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2F_CLUB_DATA			= (NET_MSG_GCTRL + 2015),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ			= (NET_MSG_GCTRL + 2016),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB		= (NET_MSG_GCTRL + 2017),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2F_FIELD_OUT			= (NET_MSG_GCTRL + 2018),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2A_FIELD_OUT_FB			= (NET_MSG_GCTRL + 2019),

	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_UPDATE	= (NET_MSG_GCTRL + 2020),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_CLUB_DATA_RANK_END		= (NET_MSG_GCTRL + 2021),

	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2C_CLUB_DATA_SCORE_UPDATE	= (NET_MSG_GCTRL + 2022),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_C2F_CLUB_DATA_SCORE_INFO_REQ	= (NET_MSG_GCTRL + 2023),

	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_A2C_TOP_RANK_INFO			= (NET_MSG_GCTRL + 2024),
	NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2C_DOUBLE_POINT_UPDATE		= (NET_MSG_GCTRL + 2025),

	/* genchar pt, Juver, 2020/12/03 */
	NET_MSG_GM_GENCHAR_PARTY										= (NET_MSG_GCTRL + 2050),

	/* skill delay addition, Juver, 2020/12/09 */
	NET_MSG_GCTRL_F2C_SKILL_DELAY_ADJUSTMENT						= (NET_MSG_GCTRL + 2051),
	NET_MSG_GCTRL_F2C_SKILL_DELAY_ADJUSTMENT_MULTI					= (NET_MSG_GCTRL + 2052),

	/* skill buff duration change, Juver, 2020/12/17 */
	NET_MSG_GCTRL_F2C_SKILL_BUFF_DURATION_CHANGE_TIME_MULTIPLE_BRD	= (NET_MSG_GCTRL + 2053),

	/* force auto revive, Juver, 2020/12/24 */
	NET_MSG_GCTRL_F2A_FORCE_AUTO_REVIVE								= (NET_MSG_GCTRL + 2060),

	/* play time system, Juver, 2021/01/26 */
	NET_MSG_GCTRL_UPDATE_PLAY_TIME									= (NET_MSG_GCTRL + 2061),
	NET_MSG_GCTRL_UPDATE_PLAY_POINT									= (NET_MSG_GCTRL + 2062),
	NET_MSG_GCTRL_UPDATE_PLAY_TIME_A2C								= (NET_MSG_GCTRL + 2063),

	/* pk combo, Juver, 2021/02/09 */
	NET_MSG_GCTRL_UPDATE_PK_COMBO									= (NET_MSG_GCTRL + 2070),

	/* award title, Juver, 2021/02/12 */
	NET_MSG_GCTRL_C2A_AWARD_TITLE_REQUEST_DATA						= (NET_MSG_GCTRL + 2074),
	NET_MSG_GCTRL_A2C_AWARD_TITLE_UPDATE_DATA						= (NET_MSG_GCTRL + 2075),
	NET_MSG_GCTRL_A2C_AWARD_TITLE_UPDATE_DONE						= (NET_MSG_GCTRL + 2076),

	/* post system, Juver, 2021/02/05 */
	NET_MSG_POST_REQ_CHANUM											= (NET_MSG_GCTRL + 2080),
	NET_MSG_POST_REQ_CHANUM_FB										= (NET_MSG_GCTRL + 2081),
	NET_MSG_POST_SEND												= (NET_MSG_GCTRL + 2082),
	NET_MSG_POST_SEND_FB											= (NET_MSG_GCTRL + 2083),

	/* game notice, Juver, 2021/06/12 */
	NET_MSG_GCTRL_GM_GAME_NOTICE_RELOAD								= (NET_MSG_GCTRL + 2200),
	NET_MSG_GCTRL_GM_GAME_NOTICE_RELOAD_FB							= (NET_MSG_GCTRL + 2201),

#if defined( BUILD_CH ) || defined( BUILD_CHY )
	/* wtf, Juver, 2021/06/24 */
	NET_MSG_GCTRL_WTF_FLD											= (NET_MSG_GCTRL + 2205),
#endif

	/* variable check, Juver, 2021/07/02 */
	NET_MSG_GCTRL_VARIABLE_CHECK_CLIENT								= (NET_MSG_GCTRL + 2210),
	NET_MSG_GCTRL_VARIABLE_CHECK_RESULT								= (NET_MSG_GCTRL + 2211),

	NET_MSG_GCTRL_INVEN_CHAOS_MACHINE								= (NET_MSG_GCTRL + 2220),
	NET_MSG_GCTRL_INVEN_CHAOS_MACHINE_FB							= (NET_MSG_GCTRL + 2221),


	/*game stats, Juver, 2017/06/21 */
	NET_MSG_GCTRL_PING_PACKET_AGENT									= (NET_MSG_GCTRL + 2230),
	NET_MSG_GCTRL_PING_PACKET_AGENT_FB								= (NET_MSG_GCTRL + 2231),
	NET_MSG_GCTRL_PING_PACKET_FIELD									= (NET_MSG_GCTRL + 2232),
	NET_MSG_GCTRL_PING_PACKET_FIELD_FB								= (NET_MSG_GCTRL + 2233),

	NET_MSG_GCTRL_REQ_SKILL_TARGET_FB								= (NET_MSG_GCTRL + 2235),
	NET_MSG_GCTRL_REQ_SKILL_GO_TO_IDLE_BRD							= (NET_MSG_GCTRL + 2236),


	/* max level notice, Juver, 2021/07/26 */
	NET_MSG_GCTRL_MAX_LEVEL_NOTICE_AGENT							= (NET_MSG_GCTRL + 2240 ),
	NET_MSG_GCTRL_MAX_LEVEL_NOTICE_CLIENT							= (NET_MSG_GCTRL + 2241 ),

	/* tyranny AFK disconnect, Juver, 2021/08/07 */
	NET_MSG_GCTRL_TYRANNY_AFK_DISCONNECT							= (NET_MSG_GCTRL + 2245),

	/* set item option, Juver, 2021/09/04 */
	NET_MSG_GCTRL_INVEN_SET_OPTION_INSERT							= (NET_MSG_GCTRL + 2250),
	NET_MSG_GCTRL_INVEN_SET_OPTION_INSERT_FB						= (NET_MSG_GCTRL + 2251),
	NET_MSG_GCTRL_INVEN_SET_OPTION_REROLL							= (NET_MSG_GCTRL + 2252),
	NET_MSG_GCTRL_INVEN_SET_OPTION_REROLL_FB						= (NET_MSG_GCTRL + 2253),

	/* tyranny result log, Juver, 2021/09/09 */
	NET_MSG_GCTRL_TYRANNY_LOG_RESULT_FROM_DB						= (NET_MSG_GCTRL + 2260),

	// Vacant
	
	NET_MSG_GCTRL_CAPTURE_THE_FLAG_AFK_DISCONNECT					= (NET_MSG_GCTRL + 2340),
	
	NET_MSG_GTRL_PKMARK_BRD											= (NET_MSG_GCTRL + 2341),

	NET_MSG_GCTRL_NPC_CONTRIBUTION_SHOP_PURCHASE					= (NET_MSG_GCTRL + 2345),
	NET_MSG_GCTRL_NPC_CONTRIBUTION_SHOP_PURCHASE_FB					= (NET_MSG_GCTRL + 2346),


	NET_MSG_GCTRL_REQ_MAXRV_PREVIEW									= (NET_MSG_GCTRL + 2350),
	NET_MSG_GCTRL_REQ_MAXRV_PREVIEW_FB								= (NET_MSG_GCTRL + 2351),
	NET_MSG_GCTRL_REQ_MAXRV_PREVIEW_DATA_INIT						= (NET_MSG_GCTRL + 2352),
	NET_MSG_GCTRL_REQ_MAXRV_PREVIEW_DATA_MOB_GEN					= (NET_MSG_GCTRL + 2353),
	NET_MSG_GCTRL_REQ_MAXRV_PREVIEW_DATA_REBUILD					= (NET_MSG_GCTRL + 2354),


	NET_MSG_GCTRL_GM_GIS_RELOAD										= (NET_MSG_GCTRL + 2360),
	NET_MSG_GCTRL_GM_GIS_RELOAD_FB									= (NET_MSG_GCTRL + 2361),
	NET_MSG_GCTRL_GM_GIS_RELOAD_CLOSE								= (NET_MSG_GCTRL + 2362),
	NET_MSG_GCTRL_GM_GIS_RELOAD_DONE								= (NET_MSG_GCTRL + 2363),

	NET_MSG_GCTRL_REQ_PURCHASE_SKILL								= (NET_MSG_GCTRL + 2366),	
	NET_MSG_GCTRL_REQ_PURCHASE_SKILL_FB								= (NET_MSG_GCTRL + 2367),	

	NET_MSG_ENCHANT_NOTIFY_AGT										= (NET_MSG_GCTRL + 2370),
	NET_MSG_ENCHANT_NOTIFY_CLIENT									= (NET_MSG_GCTRL + 2371),

	NET_MSG_COMBO_WHORE_NOTIFY_AGT									= (NET_MSG_GCTRL + 2372),
	NET_MSG_COMBO_WHORE_NOTIFY_CLIENT								= (NET_MSG_GCTRL + 2373),

	NET_MSG_GCTRL_REQ_AUTO_POTION									= (NET_MSG_GCTRL + 2380),
	NET_MSG_GCTRL_REQ_AUTO_POTION_FB								= (NET_MSG_GCTRL + 2381),

	NET_MSG_GCTRL_REQ_AUTO_PILOT_SET								= (NET_MSG_GCTRL + 2385),
	NET_MSG_GCTRL_REQ_AUTO_PILOT_SET_FB								= (NET_MSG_GCTRL + 2386),
	NET_MSG_GCTRL_REQ_AUTO_PILOT_RESET								= (NET_MSG_GCTRL + 2387),
	NET_MSG_GCTRL_REQ_AUTO_PILOT_RESET_FB							= (NET_MSG_GCTRL + 2388),

	NET_MSG_GCTRL_CARD_REBORN_A										= (NET_MSG_GCTRL + 2390),
	NET_MSG_GCTRL_CARD_REBORN_A_FB									= (NET_MSG_GCTRL + 2391),

	NET_MSG_GCTRL_CARD_REBORN_B										= (NET_MSG_GCTRL + 2395),
	NET_MSG_GCTRL_CARD_REBORN_B_FB									= (NET_MSG_GCTRL + 2396),

	NET_MSG_GCTRL_REQ_EXCHANGE_ITEM									= (NET_MSG_GCTRL + 2400),
	NET_MSG_GCTRL_REQ_EXCHANGE_ITEM_FB								= (NET_MSG_GCTRL + 2401),

	NET_MSG_GCTRL_UPDATE_CHANGE_ITEM_POINTS							= (NET_MSG_GCTRL + 2410),

	NET_MSG_GCTRL_REQ_EXCHANGE_ITEM_CARD							= (NET_MSG_GCTRL + 2412),
	NET_MSG_GCTRL_REQ_EXCHANGE_ITEM_CARD_FB							= (NET_MSG_GCTRL + 2413),

	NET_MSG_GCTRL_UPDATE_COMBAT_RECORD								= (NET_MSG_GCTRL + 2415),

	NET_MSG_GCTRL_CAPTCHA_UPDATE_SHOW_AC							= (NET_MSG_GCTRL + 2420),
	NET_MSG_GCTRL_CAPTCHA_UPDATE_INPUT_CA							= (NET_MSG_GCTRL + 2421),
	NET_MSG_GCTRL_CAPTCHA_UPDATE_INPUT_AC							= (NET_MSG_GCTRL + 2422),

	NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_REGISTER						= (NET_MSG_GCTRL + 2500),
	NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_BATTLE							= (NET_MSG_GCTRL + 2501),
	NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_REWARD							= (NET_MSG_GCTRL + 2502),
	NET_MSG_GCTRL_PVP_PBG_A2FC_STATE_ENDED							= (NET_MSG_GCTRL + 2503),
	NET_MSG_GCTRL_PVP_PBG_A2C_TOBATTLE_TIME							= (NET_MSG_GCTRL + 2504),
	NET_MSG_GCTRL_PVP_PBG_A2FC_NEXTSCHED							= (NET_MSG_GCTRL + 2505),

	NET_MSG_GCTRL_PVP_PBG_F2A_BATTLEINFO_PC							= (NET_MSG_GCTRL + 2506),
	NET_MSG_GCTRL_PVP_PBG_A2C_BATTLEINFO_PC							= (NET_MSG_GCTRL + 2507),
	NET_MSG_GCTRL_PVP_PBG_C2A_REGISTER_REQ							= (NET_MSG_GCTRL + 2508),
	NET_MSG_GCTRL_PVP_PBG_A2C_REGISTER_FB							= (NET_MSG_GCTRL + 2509),
	NET_MSG_GCTRL_PVP_PBG_C2A_REJOIN_REQ							= (NET_MSG_GCTRL + 2510),
	NET_MSG_GCTRL_PVP_PBG_A2C_REJOIN_FB								= (NET_MSG_GCTRL + 2511),
	NET_MSG_GCTRL_PVP_PBG_A2C_QUEUE_MOVED							= (NET_MSG_GCTRL + 2512),
	NET_MSG_GCTRL_PVP_PBG_A2C_QUEUE_UPDATE							= (NET_MSG_GCTRL + 2513),
	NET_MSG_GCTRL_PVP_PBG_A2F_PLAYER_DATA							= (NET_MSG_GCTRL + 2514),
	NET_MSG_GCTRL_PVP_PBG_A2F_MAP_MOVE								= (NET_MSG_GCTRL + 2515),
	NET_MSG_GCTRL_PVP_PBG_F2C_PLAYER_SCORE_UPDATE					= (NET_MSG_GCTRL + 2516),
	NET_MSG_GCTRL_PVP_PBG_C2F_PLAYER_SCORE_INFO_REQ					= (NET_MSG_GCTRL + 2517),
	NET_MSG_GCTRL_PVP_PBG_C2AF_REVIVE_REQ							= (NET_MSG_GCTRL + 2518),
	NET_MSG_GCTRL_PVP_PBG_F2AC_REVIVE_REQ_FB						= (NET_MSG_GCTRL + 2519),
	NET_MSG_GCTRL_PVP_PBG_A2F_FIELD_OUT								= (NET_MSG_GCTRL + 2520),
	NET_MSG_GCTRL_PVP_PBG_F2A_FIELD_OUT_FB							= (NET_MSG_GCTRL + 2521),
	NET_MSG_GCTRL_PVP_PBG_F2C_RANKING_UPDATE						= (NET_MSG_GCTRL + 2522),
	NET_MSG_GCTRL_PVP_PBG_F2C_RANKING_END							= (NET_MSG_GCTRL + 2523),
	NET_MSG_GCTRL_PVP_PBG_F2A_RANKINFO_PC							= (NET_MSG_GCTRL + 2524),
	NET_MSG_GCTRL_PVP_PBG_A2C_RANKINFO_PC							= (NET_MSG_GCTRL + 2525),
	NET_MSG_GCTRL_PVP_PBG_C2A_MAPEXIT_REQ							= (NET_MSG_GCTRL + 2526),
	
	NET_MSG_CHAT_GENERAL_FAIL								= (NET_MSG_GCTRL + 2600),

	/* qitem inven, 2024/01/07 */
	NET_MSG_GCTRL_REQ_USE_INVENQITEM						= (NET_MSG_GCTRL + 2620),

	/* PK Streak Sigaw - Jxyy */
	NET_MSG_GCTRL_PK_STREAK								= (NET_MSG_GCTRL + 2621),
	NET_MSG_GCTRL_PK_STREAK_KILL						= (NET_MSG_GCTRL + 2622),


	// vacant 2625 - 2702
	/*Top Level*/
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_LEVEL_UPDATE		= (NET_MSG_GCTRL + 2623),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_LEVEL_AGT			= (NET_MSG_GCTRL + 2624),

	/*dmk14 freepk*/
	NET_MSG_GCTRL_GM_FREEPK_F2A_BATTLEINFO_PC				= (NET_MSG_GCTRL + 2703),
	NET_MSG_GCTRL_GM_FREEPK_A2C_BATTLEINFO_PC				= (NET_MSG_GCTRL + 2704),

	NET_MSG_GCTRL_REQ_BATTLE_RANKING							= (NET_MSG_GCTRL + 2705),
	NET_MSG_GCTRL_REQ_MY_BATTLE_RANKING							= (NET_MSG_GCTRL + 2706),
	NET_MSG_GCTRL_REQ_BATTLE_RANKING_FB							= (NET_MSG_GCTRL + 2707),
	NET_MSG_GCTRL_REQ_MY_BATTLE_RANKING_FB						= (NET_MSG_GCTRL + 2708),
	NET_MSG_GCTRL_REQ_CLUB_BATTLE_RANKING_FB					= (NET_MSG_GCTRL + 2709),
	NET_MSG_GCTRL_REQ_MY_CLUB_BATTLE_RANKING_FB					= (NET_MSG_GCTRL + 2710),

	/*dmk14 send notif less data*/
	NET_MSG_GCTRL_REQ_RECMSG_NOTIF									= (NET_MSG_GCTRL + 2718),

	NET_MSG_GCTRL_TYRANNY_F2A_GUILD_TOWER_CAPTURE					= (NET_MSG_GCTRL + 2719),
	NET_MSG_GCTRL_TYRANNY_A2F_GUILD_TOWER_CAPTURE					= (NET_MSG_GCTRL + 2720),
	NET_MSG_GCTRL_TYRANNY_F2C_GUILD_TOWER_CAPTURE					= (NET_MSG_GCTRL + 2721),

	//vacant 2722

	/* Reset Stats Item */
	NET_MSG_GCTRL_INVEN_RESET_STATS									= (NET_MSG_GCTRL + 2723),
	NET_MSG_GCTRL_INVEN_RESET_STATS_FB								= (NET_MSG_GCTRL + 2724),

	/* Gacha System, MontageDev 7/10/24 */
	NET_MSG_GCTRL_OPEN_GACHA										= (NET_MSG_GCTRL + 2725 ),
	NET_MSG_GCTRL_UPDATE_CHA_PROMISE_POINTS							= (NET_MSG_GCTRL + 2726 ),
	
	/*12-16-24, daily login - CNDev*/
	NET_MSG_GCTRL_DAILYLOGIN_UPDATE									= (NET_MSG_GCTRL + 2727),
	NET_MSG_GCTRL_DAILYLOGIN_COMPLETE								= (NET_MSG_GCTRL + 2728),
	NET_MSG_GCTRL_DAILYLOGIN_COMPLETE_BRD							= (NET_MSG_GCTRL + 2729),

	NET_MSG_GCTRL_DAILYLOGIN_NOTIFY_CLIENT							= (NET_MSG_GCTRL + 2730),

	NET_MSG_GCTRL_REQ_DAILYLOGIN_REWARD_CLAIM						= (NET_MSG_GCTRL + 2731),
	NET_MSG_GCTRL_REQ_DAILYLOGIN_REWARD_CLAIM_FB					= (NET_MSG_GCTRL + 2732),

	/* Boss Spawn Viewer, Review000 */
	NET_MSG_GCTRL_BOSS_DETAIL_REQ						= (NET_MSG_GCTRL + 2733),
	NET_MSG_GCTRL_BOSS_DETAIL_FB						= (NET_MSG_GCTRL + 2734),

	/* Codex, Archie 02/16/24 */
	NET_MSG_GCTRL_CODEX_COMPLETE_BRD				= (NET_MSG_GCTRL + 2735),
	NET_MSG_GCTRL_CODEX_UPDATE						= (NET_MSG_GCTRL + 2736),
	NET_MSG_GCTRL_CODEX_COMPLETE					= (NET_MSG_GCTRL + 2737),

	NET_MSG_GCTRL_CODEX_NOTIFY_AGENT				= (NET_MSG_GCTRL + 2738),
	NET_MSG_GCTRL_CODEX_NOTIFY_CLIENT				= (NET_MSG_GCTRL + 2739),
	NET_MSG_GCTRL_REGISTER_CODEX					= (NET_MSG_GCTRL + 2740),
	NET_MSG_GCTRL_REGISTER_CODEX_FB					= (NET_MSG_GCTRL + 2741),
	
	/*12-9-14, Battle Pass - CNDev*/
	NET_MSG_GCTRL_BATTLEPASS_UPDATE						= (NET_MSG_GCTRL + 2742),
	NET_MSG_GCTRL_BATTLEPASS_COMPLETE					= (NET_MSG_GCTRL + 2743),
	NET_MSG_GCTRL_BATTLEPASS_COMPLETE_BRD				= (NET_MSG_GCTRL + 2744),

	NET_MSG_GCTRL_BATTLEPASS_NOTIFY_CLIENT				= (NET_MSG_GCTRL + 2745),

	NET_MSG_GCTRL_UPDATE_BATTLEPASS_EXP					= (NET_MSG_GCTRL + 2746),
	NET_MSG_GCTRL_UPDATE_BATTLEPASS_LEVEL				= (NET_MSG_GCTRL + 2747),

	NET_MSG_GCTRL_REQ_BATTLEPASS_REWARD_DATA			= (NET_MSG_GCTRL + 2748),

	NET_MSG_GCTRL_REQ_BATTLEPASS_REWARD_CLAIM			= (NET_MSG_GCTRL + 2749),
	NET_MSG_GCTRL_REQ_BATTLEPASS_REWARD_CLAIM_FB		= (NET_MSG_GCTRL + 2750),

	NET_MSG_GCTRL_REQ_BATTLEPASS_PREMIUM_CARD			= (NET_MSG_GCTRL + 2751),
	NET_MSG_GCTRL_REQ_BATTLEPASS_PREMIUM_CARD_FB		= (NET_MSG_GCTRL + 2752),

	NET_MSG_GCTRL_REQ_BATTLEPASS_LEVELUP_CARD			= (NET_MSG_GCTRL + 2753),
	NET_MSG_GCTRL_REQ_BATTLEPASS_LEVELUP_CARD_FB		= (NET_MSG_GCTRL + 2754),

	// pandora kuno
    NET_MSG_PANDORA_BOX_SETTING_INFO 						= (NET_MSG_GCTRL + 2755),
    NET_MSG_PANDORA_BOX_OPEN_REQ 							= (NET_MSG_GCTRL + 2756),
    NET_MSG_PANDORA_BOX_OPEN_FB 							= (NET_MSG_GCTRL + 2757),
    NET_MSG_PANDORA_BOX_RESULT 								= (NET_MSG_GCTRL + 2758),
    NET_MSG_PANDORA_BOX_BUY_ITEM 							= (NET_MSG_GCTRL + 2759),
    NET_MSG_PANDORA_BOX_BUY_ITEM_FB 						= (NET_MSG_GCTRL + 2760),
    NET_MSG_PANDORA_BOX_REWARD_RARE_BRD 					= (NET_MSG_GCTRL + 2761),
    NET_MSG_PANDORA_BOX_CLEAR_RESULTS_REQ 					= (NET_MSG_GCTRL + 2762),
    NET_MSG_PANDORA_BOX_CLEAR_RESULTS_FB 					= (NET_MSG_GCTRL + 2763),

	/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	NET_MSG_GCTRL_GET_ITEMSHOP_FROMDB_FB					= (NET_MSG_GCTRL + 2764),
	NET_MSG_GCTRL_BUY_ITEMSHOP								= (NET_MSG_GCTRL + 2765),
	NET_MSG_GCTRL_BUY_ITEMSHOP_ITEM							= (NET_MSG_GCTRL + 2766),
	/* Add ItemShop, Ssodomain, 11-09-2023, End */
	NET_MSG_RETRIEVE_POINTS									= (NET_MSG_GCTRL + 2767),
	NET_MSG_RETRIEVE_POINTS_FB								= (NET_MSG_GCTRL + 2768),

	//SetPoints Function
	NET_MSG_GCTRL_SET_POINTS								= (NET_MSG_GCTRL + 2769),  //! ???????? ???o??? ?????? ??? ????
	NET_MSG_GCTRL_SET_POINTS_FB								= (NET_MSG_GCTRL + 2770),  //! ???????? ???o??? ?????? ??? ????
	NET_MSG_GCTRL_UPDATE_EPOINT								= (NET_MSG_GCTRL + 2771),

	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING					= (NET_MSG_GCTRL + 2772),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_FB					= (NET_MSG_GCTRL + 2773),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_KILL_UPDATE		= (NET_MSG_GCTRL + 2774),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_KILL_AGT			= (NET_MSG_GCTRL + 2775),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RICH_UPDATE		= (NET_MSG_GCTRL + 2776),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RICH_AGT			= (NET_MSG_GCTRL + 2777),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_GUILD_AGT			= (NET_MSG_GCTRL + 2778),
	/////////////////////////////////////////////////////////////////////////////

	/*re-coded LG-7 Global Ranking by DevArt22*/
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_BR_UPDATE			= (NET_MSG_GCTRL + 2779),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_BR_AGT				= (NET_MSG_GCTRL + 2780),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SW_UPDATE			= (NET_MSG_GCTRL + 2781),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SW_AGT				= (NET_MSG_GCTRL + 2782),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AR_UPDATE			= (NET_MSG_GCTRL + 2783),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AR_AGT				= (NET_MSG_GCTRL + 2784),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SH_UPDATE			= (NET_MSG_GCTRL + 2785),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SH_AGT				= (NET_MSG_GCTRL + 2786),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_EX_UPDATE			= (NET_MSG_GCTRL + 2787),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_EX_AGT				= (NET_MSG_GCTRL + 2788),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SC_UPDATE			= (NET_MSG_GCTRL + 2789),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SC_AGT				= (NET_MSG_GCTRL + 2790),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AS_UPDATE			= (NET_MSG_GCTRL + 2791),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AS_AGT				= (NET_MSG_GCTRL + 2792),

	/*Top MMR, Mhundz */
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_MMR_UPDATE			= (NET_MSG_GCTRL + 2793),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_MMR_AGT			= (NET_MSG_GCTRL + 2794),
	NET_MSG_GCTRL_UPDATE_MMR_SCORE						= (NET_MSG_GCTRL + 2795),

	///////////////////////////////////////////////////////////////////////////////////////
	//MMR Rank
	NET_MSG_GCTRL_REQ_RANKMARK_FB						= (NET_MSG_GCTRL + 2796),
	NET_MSG_GCTRL_REQ_RANKMARK_BRD						= (NET_MSG_GCTRL + 2797),
	/////////////////////////////////////////////////////////////////////////////
	// PK GLOBAL CharView
	NET_MSG_GCTRL_REQ_CHAR_VIEW							= (NET_MSG_GCTRL + 2798),
	NET_MSG_GCTRL_REQ_CHAR_VIEW_FB						= (NET_MSG_GCTRL + 2799),
	NET_MSG_GCTRL_REQ_CHAR_VIEW_AGT						= (NET_MSG_GCTRL + 2800),
	NET_MSG_GCTRL_REQ_CHAR_VIEW_FLD						= (NET_MSG_GCTRL + 2801),
	////////////////////////////////////////////////////////////////////////////
	/*Top Resu, Mhundz */
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RESU_UPDATE		= (NET_MSG_GCTRL + 2802),
	NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RESU_AGT			= (NET_MSG_GCTRL + 2803),
	NET_MSG_GCTRL_UPDATE_RESU_SCORE2					= (NET_MSG_GCTRL + 2804),

	NET_MSG_GCTRL_REQ_RANKMARK							= (NET_MSG_GCTRL + 2805),
	NET_MSG_GCTRL_REQ_RANKNAME							= (NET_MSG_GCTRL + 2806),
	NET_MSG_GCTRL_REQ_RANKNAME_FB						= (NET_MSG_GCTRL + 2807),
	NET_MSG_GCTRL_REQ_RANKNAME_BRD						= (NET_MSG_GCTRL + 2808),

	/*self buff via NPC, DevArt22, 2023/03/18 */
	NET_MSG_GCTRL_SELFBUFF_BA_ONLY						= (NET_MSG_GCTRL + 2809),
	NET_MSG_GCTRL_SELFBUFF_BA_ONLY_FB					= (NET_MSG_GCTRL + 2810),
	NET_MSG_GCTRL_SELFBUFF_FULL							= (NET_MSG_GCTRL + 2811),
	NET_MSG_GCTRL_SELFBUFF_FULL_FB						= (NET_MSG_GCTRL + 2812),
	NET_MSG_GCTRL_SELFBUFF_FULL_NO_BA					= (NET_MSG_GCTRL + 2813),
	NET_MSG_GCTRL_SELFBUFF_FULL_NO_BA_FB				= (NET_MSG_GCTRL + 2814),

	// Vacant 2815 - 3193
	/* security 2025 */
	NET_DETECT_CHEAT_A2C						=   (NET_MSG_GCTRL + 3191),
	NET_DETECT_CHEAT_C2A						=	(NET_MSG_GCTRL + 3192),

	/*woe Arc Development 08-06-2024*/
	NET_MSG_GCTRL_WOE_A2FC_STATE_REGISTER		= (NET_MSG_GCTRL + 3194),
	NET_MSG_GCTRL_WOE_A2FC_STATE_BATTLE			= (NET_MSG_GCTRL + 3195),
	NET_MSG_GCTRL_WOE_A2FC_STATE_REWARD			= (NET_MSG_GCTRL + 3196),
	NET_MSG_GCTRL_WOE_A2FC_STATE_ENDED			= (NET_MSG_GCTRL + 3197),
	NET_MSG_GCTRL_WOE_A2C_TOBATTLE_TIME			= (NET_MSG_GCTRL + 3198),
	NET_MSG_GCTRL_WOE_A2FC_NEXTSCHED			= (NET_MSG_GCTRL + 3199),
	NET_MSG_GCTRL_WOE_F2A_BATTLEINFO_PC			= (NET_MSG_GCTRL + 3200),
	NET_MSG_GCTRL_WOE_A2C_BATTLEINFO_PC			= (NET_MSG_GCTRL + 3201),
	NET_MSG_GCTRL_WOE_C2A_REGISTER_REQ			= (NET_MSG_GCTRL + 3202),
	NET_MSG_GCTRL_WOE_A2C_REGISTER_FB			= (NET_MSG_GCTRL + 3203),
	NET_MSG_GCTRL_WOE_A2F_MAP_MOVE				= (NET_MSG_GCTRL + 3206),
	NET_MSG_GCTRL_WOE_C2A_REJOIN_REQ			= (NET_MSG_GCTRL + 3207),
	NET_MSG_GCTRL_WOE_A2C_REJOIN_FB				= (NET_MSG_GCTRL + 3208),
	NET_MSG_GCTRL_WOE_C2A_MAPEXIT_REQ			= (NET_MSG_GCTRL + 3209),
	NET_MSG_GCTRL_WOE_A2F_PLAYER_DATA			= (NET_MSG_GCTRL + 3210),
	NET_MSG_GCTRL_WOE_F2A_TOWER_CAPTURE			= (NET_MSG_GCTRL + 3211),
	NET_MSG_GCTRL_WOE_A2F_TOWER_CAPTURE			= (NET_MSG_GCTRL + 3212),
	NET_MSG_GCTRL_WOE_F2C_TOWER_CAPTURE			= (NET_MSG_GCTRL + 3213),
	NET_MSG_GCTRL_WOE_C2A_TOWER_INFO_REQ		= (NET_MSG_GCTRL + 3214),
	NET_MSG_GCTRL_WOE_A2C_TOWER_INFO			= (NET_MSG_GCTRL + 3215),
	NET_MSG_GCTRL_WOE_F2A_TOWER_SCORE			= (NET_MSG_GCTRL + 3216),
	NET_MSG_GCTRL_WOE_A2F_TOWER_WINNER			= (NET_MSG_GCTRL + 3217),
	NET_MSG_GCTRL_WOE_F2C_TOWER_WINNER			= (NET_MSG_GCTRL + 3218),
	NET_MSG_GCTRL_WOE_C2F_REVIVE_REQ			= (NET_MSG_GCTRL + 3219),
	NET_MSG_WOE_A2C_TOWER_WINNER				= (NET_MSG_GCTRL + 3220),
	NET_MSG_GCTRL_WOE_F2A_RANKINFO_PC			= (NET_MSG_GCTRL + 3222),
	NET_MSG_GCTRL_WOE_A2C_RANKINFO_PC			= (NET_MSG_GCTRL + 3223),
	NET_MSG_GCTRL_WOE_CROW_OWNER				= (NET_MSG_GCTRL + 3227),
	NET_MSG_GCTRL_WOE_CROW_DAMAGE				= (NET_MSG_GCTRL + 3228),
	NET_MSG_WOE_CAPTURE							= (NET_MSG_GCTRL + 3241),
	NET_MSG_WOE_CAPTURE_FLD						= (NET_MSG_GCTRL + 3242),
	NET_MSG_WOE_CAPTURE_AGT						= (NET_MSG_GCTRL + 3243),
	NET_MSG_WOE_END_BATTLE						= (NET_MSG_GCTRL + 3244),
	NET_MSG_WOE_CHECK_WINNER					= (NET_MSG_GCTRL + 3245),
	NET_MSG_WOE_FIRST_CAPTURE					= (NET_MSG_GCTRL + 3246),
	NET_MSG_WOE_RANKING_FLD						= (NET_MSG_GCTRL + 3247),
	NET_MSG_WOE_RANKING_AGT						= (NET_MSG_GCTRL + 3248),
	NET_MSG_WOE_RANKING_CLT						= (NET_MSG_GCTRL + 3249),
	NET_MSG_GCTRL_WOE_GUILD_POINT_UPDATE		= (NET_MSG_GCTRL + 3252),
	NET_MSG_GCTRL_WOE_GUILD_MYRANK_UPDATE		= (NET_MSG_GCTRL + 3253),
	NET_MSG_GCTRL_WOE_GUILD_RANKING_UPDATE		= (NET_MSG_GCTRL + 3254),
	NET_MSG_GCTRL_WOE_GUILD_RANKING_REQ			= (NET_MSG_GCTRL + 3255),
	NET_MSG_GCTRL_WOE_PLAYER_MYRANK_UPDATE		= (NET_MSG_GCTRL + 3256),
	NET_MSG_GCTRL_WOE_PLAYER_RANKING_UPDATE		= (NET_MSG_GCTRL + 3257),
	NET_MSG_GCTRL_WOE_PLAYER_RANKING_REQ		= (NET_MSG_GCTRL + 3258),
	NET_MSG_GCTRL_WOE_RESU_MYRANK_UPDATE		= (NET_MSG_GCTRL + 3259),
	NET_MSG_GCTRL_WOE_RESU_RANKING_UPDATE		= (NET_MSG_GCTRL + 3260),
	NET_MSG_GCTRL_WOE_RESU_RANKING_REQ			= (NET_MSG_GCTRL + 3261),
	NET_MSG_WOE_A2F_DECLARE_WINNER				= (NET_MSG_GCTRL + 3264),
	NET_MSG_WOE_F2C_DECLARE_WINNER				= (NET_MSG_GCTRL + 3265),
	NET_MSG_WOE_A2C_DECLARE_WINNER				= (NET_MSG_GCTRL + 3266),
	NET_MSG_GCTRL_RESU_STREAK,

};



#endif // S_NETMSGDEFINE_H_INCLUDED__
