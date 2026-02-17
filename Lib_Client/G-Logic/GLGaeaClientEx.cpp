#include "stdafx.h"
#include "./GLGaeaClient.h"

#include "./PVPPartyBattleGroundsClient.h"
#include "./PVPPartyBattleGroundsMsg.h"

#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/LoadingThread.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"
#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/ModalCallerID.h"
#include "../../Lib_ClientUI/Interface/ModalWindow.h"

#include "../../Lib_Engine/DxResponseMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLGaeaClient::PVPClubWarsRankClear()
{
	m_mapClubWarData.clear();

	CInnerInterface::GetInstance().PVPClubWarRankingWindowClose();
}

void GLGaeaClient::PVPClubWarsRankStart( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_CLUB_SCORE_A2C_RANKING_START* pNetMsg = ( GLMSG::SNETPC_CLUB_SCORE_A2C_RANKING_START* ) nmg;

	SPVP_CLUB_WAR_CLIENT_DATA_MAP_ITER it = m_mapClubWarData.find( pNetMsg->dwGuidanceID );
	if ( it != m_mapClubWarData.end() )
	{
		m_mapClubWarData.erase( it );
	}

	SPVP_CLUB_WAR_CLIENT_DATA sData;
	sData.dwClubWarID = pNetMsg->dwGuidanceID;
	StringCchCopy( sData.szClubWarName, PVP_CLUB_WAR_NAME_SIZE, pNetMsg->szGuidanceName );

	sData.dwOwnerGuildID = pNetMsg->dwOwnerGuildID;
	StringCchCopy( sData.szOwnerGuildName, PVP_CLUB_WAR_NAME_SIZE, pNetMsg->szOwnerGuildName );
	sData.dwGuildMarkVer = pNetMsg->dwGuildMarkVer;

	sData.fBonusMain = pNetMsg->fBonusMain;
	sData.fBonusAlly = pNetMsg->fBonusAlly;

	m_mapClubWarData.insert( std::make_pair( sData.dwClubWarID, sData ) );
}

void GLGaeaClient::PVPClubWarsRankUpdate( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_CLUB_SCORE_A2C_RANKING_UPDATE* pNetMsg = ( GLMSG::SNETPC_CLUB_SCORE_A2C_RANKING_UPDATE* ) nmg;

	SPVP_CLUB_WAR_CLIENT_DATA_MAP_ITER it = m_mapClubWarData.find( pNetMsg->dwGuidanceID );
	if ( it != m_mapClubWarData.end() )
	{
		SPVP_CLUB_WAR_CLIENT_DATA& sData = (*it).second;

		for ( WORD i=0; i<pNetMsg->wRankNum; ++i )
		{
			sData.InsertPlayerData( pNetMsg->sPlayerData[i] ); 
		}
	}
}

void GLGaeaClient::PVPClubWarsRankEnd( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_CLUB_SCORE_F2C_RANKING_END* pNetMsg = ( GLMSG::SNETPC_CLUB_SCORE_F2C_RANKING_END* ) nmg;

	SPVP_CLUB_WAR_CLIENT_DATA_MAP_ITER it = m_mapClubWarData.find( pNetMsg->dwGuidanceID );
	if ( it != m_mapClubWarData.end() )
	{
		SPVP_CLUB_WAR_CLIENT_DATA& sData = (*it).second;
		sData.CalculateRank();
		sData.CreateSelfScore( GetCharacter()->m_dwCharID );

		CInnerInterface::GetInstance().PVPClubWarRankingWindowRefresh();
	}
}

bool GLGaeaClient::PVPClubWarsRankValid()
{
	if ( m_mapClubWarData.size() > 0 )	return true;

	return false;
}

void GLGaeaClient::PartyFinderReset()
{
	m_vecPartyFinderResult.clear();
}

/* pvp club death match, Juver, 2020/11/26 */
HRESULT GLGaeaClient::PVPClubDeathMatchReBirthFB ( NET_MSG_GENERIC* nmg )
{
	if ( m_Pet.IsVALID() )
	{
		m_Pet.DeleteDeviceObjects ();
		m_Character.m_bIsPetActive = TRUE;
	}

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].IsVALID() )
			m_Summon[i].DeleteDeviceObjects();
	}

	if ( m_Character.m_bVehicle )
	{
		m_Character.ReqSetVehicle( false );
		m_Character.SetVehicle ( false );
		m_Character.m_bIsVehicleActive = TRUE;
	}

	HRESULT hr=S_OK;

	DWORD dwColorArg1(NULL),dwOldMod(NULL);
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1,	&dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );

	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,		&dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB *pNetMsg = (GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB *) nmg;

	GLMapList::FIELDMAP MapsList = GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find( pNetMsg->sMapID.dwID );
	if( iter == MapsList.end() )				return E_FAIL;

	const SMAPNODE *pMapNode = &(*iter).second;

	NLOADINGTHREAD::StartThreadLOAD( &m_pd3dDevice, m_hWnd, m_szAppPath, pMapNode->strLoadingImageName.c_str(), pMapNode->strMapName.c_str(), TRUE );
	NLOADINGTHREAD::WaitThread();

	LOADINGSTEP::SETSTEP ( 1 );

	m_pLandMClient->ClearDropObj ();

	LOADINGSTEP::SETSTEP ( 3 );

	//	부활할 Map이 틀릴 경우.
	if ( pNetMsg->sMapID != m_pLandMClient->GetMapID() )
	{
		hr = MoveActiveMap ( pNetMsg->sMapID, pNetMsg->vPos );
		if ( SUCCEEDED(hr) )
		{
			LOADINGSTEP::SETSTEP ( 5 );
			m_Character.MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPos );
		}
	}
	else
	{
		LOADINGSTEP::SETSTEP ( 5 );
		m_Character.SetPosition ( pNetMsg->vPos );
		m_Character.DoActWait ();
	}

	LOADINGSTEP::SETSTEP ( 7 );

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

	LOADINGSTEP::SETSTEP ( 9 );

	//	부활처리.
	m_Character.ReBirth ( pNetMsg->wNowHP, pNetMsg->wNowMP, pNetMsg->wNowSP, pNetMsg->vPos, false );

	LOADINGSTEP::SETSTEP ( 11 );
	NLOADINGTHREAD::EndThread();

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	dwColorArg1 );

	return S_OK;
}

HRESULT GLGaeaClient::PVPPBGReBirthFB ( NET_MSG_GENERIC* nmg )
{
	if ( m_Pet.IsVALID() )
	{
		m_Pet.DeleteDeviceObjects ();
		m_Character.m_bIsPetActive = TRUE;
	}

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].IsVALID() )
			m_Summon[i].DeleteDeviceObjects();
	}

	if ( m_Character.m_bVehicle )
	{
		m_Character.ReqSetVehicle( false );
		m_Character.SetVehicle ( false );
		m_Character.m_bIsVehicleActive = TRUE;
	}

	HRESULT hr=S_OK;

	DWORD dwColorArg1(NULL),dwOldMod(NULL);
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1,	&dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );

	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,		&dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );

	GLMSG::SNETPC_PVP_PBG_F2AC_REVIVE_REQ_FB *pNetMsg = (GLMSG::SNETPC_PVP_PBG_F2AC_REVIVE_REQ_FB *) nmg;

	GLMapList::FIELDMAP MapsList = GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find( pNetMsg->sMapID.dwID );
	if( iter == MapsList.end() )				return E_FAIL;

	const SMAPNODE *pMapNode = &(*iter).second;

	NLOADINGTHREAD::StartThreadLOAD( &m_pd3dDevice, m_hWnd, m_szAppPath, pMapNode->strLoadingImageName.c_str(), pMapNode->strMapName.c_str(), TRUE );
	NLOADINGTHREAD::WaitThread();

	LOADINGSTEP::SETSTEP ( 1 );

	m_pLandMClient->ClearDropObj ();

	LOADINGSTEP::SETSTEP ( 3 );

	//	부활할 Map이 틀릴 경우.
	if ( pNetMsg->sMapID != m_pLandMClient->GetMapID() )
	{
		hr = MoveActiveMap ( pNetMsg->sMapID, pNetMsg->vPos );
		if ( SUCCEEDED(hr) )
		{
			LOADINGSTEP::SETSTEP ( 5 );
			m_Character.MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPos );
		}
	}
	else
	{
		LOADINGSTEP::SETSTEP ( 5 );
		m_Character.SetPosition ( pNetMsg->vPos );
		m_Character.DoActWait ();
	}

	LOADINGSTEP::SETSTEP ( 7 );

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

	LOADINGSTEP::SETSTEP ( 9 );

	//	부활처리.
	m_Character.ReBirth ( pNetMsg->wNowHP, pNetMsg->wNowMP, pNetMsg->wNowSP, pNetMsg->vPos, false );

	LOADINGSTEP::SETSTEP ( 11 );
	NLOADINGTHREAD::EndThread();

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	dwColorArg1 );

	return S_OK;
}