#ifndef GLPVPCLUBWARDATA_H__INCLUDED
#define GLPVPCLUBWARDATA_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PVP_CLUB_WAR_PLAYER_DATA_CHARID_NULL		0xFFFFFFFF	//similar to GAEAID_NULL
#define PVP_CLUB_WAR_PLAYER_DATA_CHAR_NAME_SIZE		33			//similar to CHAR_SZNAME
#define PVP_CLUB_WAR_PLAYER_DATA_GUILD_NAME_SIZE	33			//similar to CHAR_SZNAME

#define PVP_CLUB_WAR_PACKET_RANK_NUM				18			//field to client ranking data
#define PVP_CLUB_WAR_NAME_SIZE						33

enum EMPVP_CLUB_WAR_PLAYER_STATE
{
	EMPVP_CLUB_WAR_PLAYER_STATE_PARTICIPANT = 0,
	EMPVP_CLUB_WAR_PLAYER_STATE_WINNER_MAIN = 1,
	EMPVP_CLUB_WAR_PLAYER_STATE_WINNER_ALLY = 2,
};

struct SPVP_CLUB_WAR_PLAYER_DATA
{
	DWORD	dwCharID;
	char	szCharName[PVP_CLUB_WAR_PLAYER_DATA_CHAR_NAME_SIZE];
	WORD	wCharSchool;
	WORD	wCharClass;

	DWORD	dwGuildID;
	char	szGuildName[PVP_CLUB_WAR_PLAYER_DATA_GUILD_NAME_SIZE];
	DWORD	dwGuildMarkVer;

	//rank
	WORD	wRankAll;
	WORD	wRankGuild;

	//score
	WORD	wScoreKill;
	WORD	wScoreDeath;
	WORD	wScoreResu;
	WORD	wScoreTotal;

	WORD	wState;
	WORD	wRewardBase;
	WORD	wRewardBonus;

	SPVP_CLUB_WAR_PLAYER_DATA()
		: dwCharID(PVP_CLUB_WAR_PLAYER_DATA_CHARID_NULL)
		, wCharSchool(0)
		, wCharClass(0)

		, dwGuildID(0)
		, dwGuildMarkVer(0)

		, wRankAll(0)
		, wRankGuild(0)

		, wScoreKill(0)
		, wScoreDeath(0)
		, wScoreResu(0)
		, wScoreTotal(0)

		, wState(EMPVP_CLUB_WAR_PLAYER_STATE_PARTICIPANT)
		, wRewardBase(0)
		, wRewardBonus(0)

	{
		memset (szCharName, 0, sizeof(char) * PVP_CLUB_WAR_PLAYER_DATA_CHAR_NAME_SIZE);		
		memset (szGuildName, 0, sizeof(char) * PVP_CLUB_WAR_PLAYER_DATA_GUILD_NAME_SIZE);		
	};

	bool operator < ( const SPVP_CLUB_WAR_PLAYER_DATA& sData )
	{			
		if ( wScoreTotal > sData.wScoreTotal )
			return true;
		else if ( wScoreTotal == sData.wScoreTotal && wScoreKill > sData.wScoreKill )	
			return true;
		else if ( wScoreTotal == sData.wScoreTotal && wScoreKill == sData.wScoreKill && wScoreDeath < sData.wScoreDeath )	
			return true;

		return  false;
	}

	SPVP_CLUB_WAR_PLAYER_DATA ( const SPVP_CLUB_WAR_PLAYER_DATA &value )
	{
		operator=(value);
	}

	SPVP_CLUB_WAR_PLAYER_DATA& operator = ( const SPVP_CLUB_WAR_PLAYER_DATA& rvalue )
	{
		dwCharID		= rvalue.dwCharID;
		StringCchCopy( szCharName, PVP_CLUB_WAR_PLAYER_DATA_CHAR_NAME_SIZE, rvalue.szCharName );
		wCharSchool		= rvalue.wCharSchool;
		wCharClass		= rvalue.wCharClass;

		dwGuildID		= rvalue.dwGuildID;
		StringCchCopy( szGuildName, PVP_CLUB_WAR_PLAYER_DATA_GUILD_NAME_SIZE, rvalue.szGuildName );
		dwGuildMarkVer	= rvalue.dwGuildMarkVer;

		wRankAll		= rvalue.wRankAll;
		wRankGuild		= rvalue.wRankGuild;

		wScoreKill		= rvalue.wScoreKill;
		wScoreDeath		= rvalue.wScoreDeath;
		wScoreResu		= rvalue.wScoreResu;
		wScoreTotal		= rvalue.wScoreTotal;

		wState			= rvalue.wState;
		wRewardBase		= rvalue.wRewardBase;
		wRewardBonus	= rvalue.wRewardBonus;

		return *this;
	}
};


typedef std::map<DWORD,SPVP_CLUB_WAR_PLAYER_DATA>			SPVP_CLUB_WAR_PLAYER_DATA_MAP;
typedef SPVP_CLUB_WAR_PLAYER_DATA_MAP::iterator				SPVP_CLUB_WAR_PLAYER_DATA_MAP_ITER;
typedef std::vector<SPVP_CLUB_WAR_PLAYER_DATA>				SPVP_CLUB_WAR_PLAYER_DATA_VEC;


struct SPVP_CLUB_WAR_CLIENT_DATA
{
	DWORD	dwClubWarID;
	char	szClubWarName[PVP_CLUB_WAR_NAME_SIZE];

	DWORD	dwOwnerGuildID;
	char	szOwnerGuildName[PVP_CLUB_WAR_PLAYER_DATA_GUILD_NAME_SIZE];
	DWORD	dwGuildMarkVer;

	float	fBonusMain;
	float	fBonusAlly;

	SPVP_CLUB_WAR_PLAYER_DATA_VEC	vecPlayerData;
	SPVP_CLUB_WAR_PLAYER_DATA		sRankSelf;

	SPVP_CLUB_WAR_CLIENT_DATA()
		: dwClubWarID(0)
		, dwOwnerGuildID(0)
		, dwGuildMarkVer(0)
		, fBonusMain(1.0f)
		, fBonusAlly(1.0f)
	{
		memset (szClubWarName, 0, sizeof(char) * PVP_CLUB_WAR_NAME_SIZE);	
		memset (szOwnerGuildName, 0, sizeof(char) * PVP_CLUB_WAR_PLAYER_DATA_GUILD_NAME_SIZE);	
	};

	void InsertPlayerData( const SPVP_CLUB_WAR_PLAYER_DATA& sPlayerData );
	void CalculateRank();
	void CreateSelfScore( DWORD dwCharID );
};

typedef std::map<DWORD,SPVP_CLUB_WAR_CLIENT_DATA>			SPVP_CLUB_WAR_CLIENT_DATA_MAP;
typedef SPVP_CLUB_WAR_CLIENT_DATA_MAP::iterator				SPVP_CLUB_WAR_CLIENT_DATA_MAP_ITER;
typedef std::vector<SPVP_CLUB_WAR_CLIENT_DATA>				SPVP_CLUB_WAR_CLIENT_DATA_VEC;


#endif // GLPVPCLUBWARDATA_H__INCLUDED
