/*!
 * \file PVPPartyBattleGroundsAgent.h
 *
 * \author Juver
 * \date 2023/01/02
 *
 * 
 */

#ifndef PVPPARTYBATTLEGROUNDSAGENT_H_INCLUDED__
#define PVPPARTYBATTLEGROUNDSAGENT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./PVPPartyBattleGrounds.h"

struct NET_MSG_GENERIC;

namespace PVPPBG
{
	class ManagerAgent : public Manager
	{
	private:
		ManagerAgent();

	public:
		~ManagerAgent();

	public:
		static ManagerAgent& GetInstance();

	public:
		float	m_fTimer;
		float	m_fRemain;

		WORD	m_wLastNotifyTime;

		bool	m_bMoveIn;
		bool	m_bMoveOut;
		float	m_fMoveTimer;
		float	m_fMoveTimerTotal;
	

		REGISTER_DATA_MAP m_mapPlayerData;
		REGISTER_DATA_VEC m_vecPlayerMove;

		REGISTER_DATA_QUEUE_VEC	m_vecPlayerDataQueue;
		PLAYER_HASH_DATA_MAP	m_mapPlayerHash;

		PLAYER_DATA m_sLastTop[PVP_PBG_MINI_RANKING_NUM];

	public:
		void LoadEvent( const std::string& strFile );
		void ResetEvent();

		bool FrameMove ( float fElapsedTime );
		bool SetMapState ();

		void SetEventState( PVPPBG::EVENT_STATE emState );

		void DoStateRegister();
		void DoStateBattle();
		void DoStateReward();
		void DoStateEnd();

		void NotifyTimeToStart( float fTime );

	public:
		REGISTER_DATA* PlayerDataGet( DWORD dwCharID );
		REGISTER_DATA_QUEUE* PlayerQueueGet( DWORD dwCharID );

		BOOL IsPlayerFull();
		BOOL IsPlayerReg( DWORD dwCharID );
		BOOL IsPlayerQueue( DWORD dwCharID );

		BOOL InsertQueue( DWORD dwCharID, const char* szName, WORD wClass, WORD wSchool );
		BOOL InsertRegister( DWORD dwCharID, const char* szName, WORD wClass, WORD wSchool );

		BOOL DeleteRegister( DWORD dwCharID );
		BOOL DeleteQueue( DWORD dwCharID );

		void ProcessQueueData();
		void UpdateQueuePosition();
		void ProcessQueueDataAll();

		void SendPlayerData();
		void QueueMoveIn();
		void QueueMoveOut();
		void MapMoveInPlayer();
		void MapMoveOutPlayer();

		BOOL PCHashCheck( std::string strHash );
		void PCHashInsert( std::string strHash, DWORD dwCharID );
		void PCHashDelete( DWORD dwCharID );

		void CheckForceMove( float fElapsedTime );

	public:
		HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

		void RequestInfoPC( DWORD dwCharID );
		void RequestPlayerRegister( 
			DWORD dwCharID, 
			WORD wLevel, 
			DWORD dwContri,
			BOOL bRegister, 
			std::string strHash );

		void RequestPlayerCancelRegister( DWORD dwCharID );

		void PlayerRegister( DWORD dwCharID, std::string strHash );
		void CancelRegister( DWORD dwCharID );

		void PlayerRejoinBattle( DWORD dwCharID );
		void MapExit( DWORD dwCharID );
	};

};

#endif // PVPPARTYBATTLEGROUNDSAGENT_H_INCLUDED__
