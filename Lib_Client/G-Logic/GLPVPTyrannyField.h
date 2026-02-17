/*!
 * \file GLPVPTyrannyField.h
 *
 * \author Juver
 * \date June 2017
 *
 * 
 */

#pragma once

#include "./GLPVPTyranny.h"

class	GLPVPTyrannyField : public GLPVPTyranny
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
	/*global buffs map setting, Juver, 2018/01/23 */
	void	RequestInfoPC( DWORD dwCharID, BOOL bTyrannyMap, BOOL bDisableSystemBuffs );

	TYRANNY_PLAYER_DATA*		PlayerDataGet( DWORD dwCharID );

private:
	void	InsertPlayerData( TYRANNY_REGISTER_DATA* pPlayerData );
	void	PlayerRevive( DWORD dwGaeaID, DWORD dwCharID );

public:
	void	TowerCapture( SNATIVEID sidMob, WORD wSchool, DWORD dwClubID, DWORD dwClubMarkVer );

public:
	void	ScoreKill( const SNATIVEID& sMapID, DWORD dwCharKilled, DWORD dwCharKiller );
	void	ScoreDamage( const SNATIVEID& sMapID, DWORD dwChar, DWORD dwDamage );
	void	ScoreHeal(const SNATIVEID& sMapID, DWORD dwChar, DWORD dwHeal, DWORD dwCharHealer );
	void	ScoreResu( const SNATIVEID& sMapID, DWORD dwChar );
	void	ScoreCalculate( TYRANNY_PLAYER_DATA_MAP& mapData, WORD wSchoolWinner );

	void	CalculateRankingSchool();
	void	CalculateRankingAll();
	void	RankingSend();
	void	GetRewards( WORD wSchoolWinner );
	void	RewardSend();
	void	PointSend();
	void	CheckRewards();

	void	LogTyrannyResult( WORD wSchoolWinner );
	void	LogTyrannyParticipant( int nSessionID );

	WORD	GetRebirthTime( WORD wSchool )	{ return m_wRebirthTime[wSchool]; }

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
public:
	void AddTyrannyClubScore( DWORD dwKillClub, DWORD dwDeathClub );
	void AddTyrannyClubScoreResu( DWORD dwResuClub );
	STYRANNY_CLUB_RANK_INFO* GetTyrannyClubRankInfo( DWORD dwClubID );
	void UpdateTyrannyClubRanking();

	void AddTyrannyScore( DWORD dwKillChar, DWORD dwDeathChar );
	void AddTyrannyScoreResu( DWORD dwResuChar );
	STYRANNY_RANK_INFO* GetTyrannyRankInfo( DWORD dwCharID );
	void UpdateTyrannyRanking();

	void AddTyrannyResuScore( DWORD dwKillChar, DWORD dwDeathChar );
	void AddTyrannyResuScoreResu( DWORD dwResuChar );
	STYRANNY_RANK_RESU_INFO* GetTyrannyRankResuInfo( DWORD dwCharID );
	void UpdateTyrannyRankingResu();
	
public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

private:
	TYRANNY_PLAYER_DATA_MAP			m_mapPlayerDataSG;
	TYRANNY_PLAYER_DATA_MAP			m_mapPlayerDataMP;
	TYRANNY_PLAYER_DATA_MAP			m_mapPlayerDataPHX;
	TYRANNY_REWARD_MAP				m_mapRewarded;
	BOOL							m_bValidInstance;

	WORD							m_wRebirthTime[TYRANNY_SCHOOL_SIZE];

public:
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	TYRANNY_CLUB_RANK_INFO_MAP		m_mapTyrannyClubScore;
	TYRANNY_RANK_INFO_MAP			m_mapTyrannyScore;
	TYRANNY_RANK_RESU_INFO_MAP		m_mapTyrannyScoreResu;

public:
	GLPVPTyrannyField()
		: m_bValidInstance(FALSE)
	{
		
	}

	~GLPVPTyrannyField()
	{

	}


public:
	static GLPVPTyrannyField& GetInstance();
};
