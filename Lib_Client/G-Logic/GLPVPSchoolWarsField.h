/*!
 * \file GLPVPSchoolWarsField.h
 *
 * \author Juver
 * \date January 2018
 *
 * 
 */

#pragma once

#include "./GLPVPSchoolWars.h"

class	GLPVPSchoolWarsField : public GLPVPSchoolWars
{

public:
	void	LoadEvent( std::string strFile );
	bool	FrameMove ( float fElaps );
	bool	SetMapState ();

	void	ResetData();
	void	DoStateRegister();
	void	DoStateBattle();
	void	DoStateReward( WORD wSchoolWinner );
	void	DoStateEnd();
	void	RequestInfoPC( DWORD dwCharID );

	SCHOOLWARS_PLAYER_DATA*		PlayerDataGet( DWORD dwCharID );

private:
	void	InsertPlayerData( SCHOOLWARS_REGISTER_DATA* pPlayerData );
	void	PlayerRevive( DWORD dwGaeaID, DWORD dwCharID );

public:
	void	ScoreKill( DWORD dwCharKilled, DWORD dwCharKiller );
	void	ScoreHeal( DWORD dwChar, DWORD dwHeal, DWORD dwCharHealer );
	void	ScoreResu( DWORD dwChar );
	void	ScoreMobKill( SNATIVEID idcrow, DWORD idchar );
	void	ScoreMobKillAll( WORD school, WORD score );
	void	ScoreCalculate( SCHOOLWARS_PLAYER_DATA_MAP& mapData, WORD wSchoolWinner );
	
	void	CalculateRankingSchool();
	void	CalculateRankingAll();
	void	RankingSend();
	void	GetRewards( WORD wSchoolWinner );
	void	RewardSend();
	void	PointSend();
	void	CheckRewards();

	void	AddBattleRanking(DWORD dwKillID, DWORD dwDeathID);
	void	UpdateBattleRanking();
	void	ReqBattleRanking(DWORD dwClientID, int nPage);
	void	ReqMyBattleRanking(DWORD dwClientID, DWORD dwCharID, int nPage);

public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

private:
	SCHOOLWARS_PLAYER_DATA_MAP			m_mapPlayerDataSG;
	SCHOOLWARS_PLAYER_DATA_MAP			m_mapPlayerDataMP;
	SCHOOLWARS_PLAYER_DATA_MAP			m_mapPlayerDataPHX;
	SCHOOLWARS_REWARD_MAP				m_mapRewarded;
	BOOL								m_bValidInstance;

	MAP_BATTLE_RANKING				m_mapBattleRankingP;
	VEC_BATTLE_RANKING				m_vecBattleRankingP;
	MAP_BATTLE_RANKING				m_mapBattleRankingC;
	VEC_BATTLE_RANKING				m_vecBattleRankingC;

public:
	GLPVPSchoolWarsField()
		: m_bValidInstance(FALSE)
	{
		
	}

	~GLPVPSchoolWarsField()
	{

	}


public:
	static GLPVPSchoolWarsField& GetInstance();
};
