/*!
 * \file PVPPartyBattleGroundsClient.h
 *
 * \author Juver
 * \date 2023/01/02
 *
 * 
 */

#ifndef PVPPARTYBATTLEGROUNDSCLIENT_H_INCLUDED__
#define PVPPARTYBATTLEGROUNDSCLIENT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./PVPPartyBattleGroundsData.h"
#include "./PVPPartyBattleGroundsDefine.h"

struct NET_MSG_GENERIC;

namespace PVPPBG
{
	class ManagerClient
	{
	private:
		ManagerClient();

	public:
		~ManagerClient();

	public:
		static ManagerClient& GetInstance();

	public:
		PVPPBG::EVENT_STATE m_emState;
		EVENT_SCHEDEDULE_NEXT m_sScheduleNext;

		PVPPBG::PLAYER_DATA m_PlayerData;
		PVPPBG::PLAYER_DATA m_sLastTop[PVP_PBG_MINI_RANKING_NUM];
		PLAYER_DATA_VEC	m_vecRanking;

		float m_fRemain;

		WORD m_wLevelReq;
		WORD m_wPlayerLimit;
		DWORD m_dwContriReq;

		WORD m_wBattleDuration;

		BOOL m_bRegistered;
		BOOL m_bQueued;
		WORD m_wQueueNum;

		BOOL m_bShowStartNotice;

	public:
		bool FrameMove ( float fTime, float fElapsedTime );

		bool IsRegister() { return bool(m_emState == PVPPBG::EVENT_STATE_REGISTER); }
		bool IsEnded()	{ return bool(m_emState == PVPPBG::EVENT_STATE_ENDED); }
		bool IsBattle()	{ return bool(m_emState == PVPPBG::EVENT_STATE_BATTLE); }
		bool IsReward()	{ return bool(m_emState == PVPPBG::EVENT_STATE_REWARD); }

		void SetEventState( PVPPBG::EVENT_STATE emState );

		void DoStateRegister();
		void DoStateBattle();
		void DoStateReward();
		void DoStateEnd();
		void NotifyTimeToStart( WORD wTime );

		PLAYER_DATA* GetOwnRanking();
		int GetRankingSize();
		PLAYER_DATA_VEC& GetRankingVector();

	public:
		HRESULT MsgProcess ( NET_MSG_GENERIC* nmg );

	public:
		void DoRegister();
		void RequestScoreInfo();
		void DoRevive();
		void RequestMapExit();
	};
};


#endif // PVPPARTYBATTLEGROUNDSCLIENT_H_INCLUDED__
