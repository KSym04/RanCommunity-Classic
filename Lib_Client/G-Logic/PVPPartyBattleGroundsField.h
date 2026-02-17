/*!
 * \file PVPPartyBattleGroundsField.h
 *
 * \author Juver
 * \date 2023/01/02
 *
 * 
 */

#ifndef PVPPARTYBATTLEGROUNDSFIELD_H_INCLUDED__
#define PVPPARTYBATTLEGROUNDSFIELD_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./PVPPartyBattleGrounds.h"

struct NET_MSG_GENERIC;

namespace PVPPBG
{
	class ManagerField : public Manager
	{
	private:
		ManagerField();

	public:
		~ManagerField();

	public:
		static ManagerField& GetInstance();

	private:
		bool m_bValidInstance;
		PLAYER_DATA_MAP m_mapPlayerData;

		float m_fScoreUpdate;

	public:
		void LoadEvent( std::string strFile );
		void ResetEvent();
		bool SetMapState ();

		bool FrameMove ( float fElaps );

		void DoStateRegister();
		void DoStateBattle();
		void DoStateReward();
		void DoStateEnd();

	public:
		PLAYER_DATA* PlayerDataGet( DWORD dwCharID );

	private:
		void InsertPlayerData( REGISTER_DATA* pPlayerData );

	public:
		void RequestInfoPC( DWORD dwCharID );
		void ScoreKill( DWORD dwCharKilled, DWORD dwCharKiller );
		void ScoreResu( DWORD dwChar );
		void ScoreHeal( DWORD dwChar, DWORD dwHeal, DWORD dwCharHealer );

		void ScoreUpdate();

		void ScoreCalculate();
		void CalculateRanking();
		void CalculateRewardPoint();
		void RankingSend();
		void SendRewards();

		void RequestScoreInfo( DWORD dwCharID );
	public:
		HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

		
	};
};

#endif // PVPPARTYBATTLEGROUNDSFIELD_H_INCLUDED__
