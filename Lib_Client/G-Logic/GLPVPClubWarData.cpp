#include "stdafx.h"
#include "./GLPVPClubWarData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void SPVP_CLUB_WAR_CLIENT_DATA::InsertPlayerData( const SPVP_CLUB_WAR_PLAYER_DATA& sPlayerData )
{
	//exist
	//should not happen as data is unique for each player
	/*for ( size_t i=0; i<vecPlayerData.size(); ++i )
	{
	if ( sPlayerData.dwCharID == vecPlayerData[i].dwCharID )
	{
	break;
	return;
	}
	}*/

	vecPlayerData.push_back( sPlayerData );
};

void SPVP_CLUB_WAR_CLIENT_DATA::CalculateRank()
{
	std::sort( vecPlayerData.begin(), vecPlayerData.end() );	
};

void SPVP_CLUB_WAR_CLIENT_DATA::CreateSelfScore( DWORD dwCharID )
{
	sRankSelf = SPVP_CLUB_WAR_PLAYER_DATA();

	for ( size_t i=0; i<vecPlayerData.size(); ++i )
	{
		if ( dwCharID == vecPlayerData[i].dwCharID )
		{
			sRankSelf = vecPlayerData[i];
		}
	}
}