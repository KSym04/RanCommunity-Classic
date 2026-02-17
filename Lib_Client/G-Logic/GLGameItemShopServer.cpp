#include "stdafx.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#include "./GLItemMan.h"

#include "./GLAgentServer.h"
#include "./GLGameItemShopServer.h"

#include "../DbActionLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLGameItemShopServer& GLGameItemShopServer::GetInstance()
{
	static GLGameItemShopServer _instance;
	return _instance;
}

GLGameItemShopServer::GLGameItemShopServer()
	: m_bReloading(false)
	, m_bReloadCountdown(false)
	, m_fReloadCountdownTimer(0.0f)
{
	InitializeCriticalSection( &m_csItemShop );
	InitializeCriticalSection( &m_csPointInfo );
}

GLGameItemShopServer::~GLGameItemShopServer()
{
	DeleteCriticalSection(&m_csItemShop);
	DeleteCriticalSection(&m_csPointInfo);

	m_mapItemShop.clear();
	m_mapPointInfo.clear();
}

void GLGameItemShopServer::CleanUp()
{
	EnterCriticalSection(&m_csItemShop);
	{
		m_mapItemShop.clear();
	}
	LeaveCriticalSection(&m_csItemShop);

	EnterCriticalSection(&m_csPointInfo);
	{
		m_mapPointInfo.clear();
	}
	LeaveCriticalSection(&m_csPointInfo);
}

void GLGameItemShopServer::LoadItemShop()
{
	if ( !RPARAM::bGameItemShop )
		return;

	CleanUp();

	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( !pDbMan )	
		return;

	DWORD StartTime = ::GetTickCount();

	SGAME_ITEM_SHOP_DATA_VEC vecData;

	//juvs.todo
	//this should be converted to pending job to free thread
	//leave it as is for now
	pDbMan->GISGetShopItems( vecData );

	for ( size_t i=0; i<vecData.size(); ++i )
	{
		AddItem( vecData[i] );
	}

	DWORD CheckTime = ::GetTickCount();
	DWORD ElspTime  = CheckTime - StartTime;

	GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "GameItemShop load (%d items) (%d msec)", GetItemShopSize(), ElspTime );

}

void GLGameItemShopServer::AddItem( const SGAME_ITEM_SHOP_DATA& sData )
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem( sData.sItemID );
	if ( pItem == NULL )
		return;

	EnterCriticalSection(&m_csItemShop);
	{
		if ( sData.wIndex != GAME_ITEM_SHOP_INDEX_NULL )	
		{
			SGAME_ITEM_SHOP_DATA_MAP_ITER it = m_mapItemShop.find( sData.wIndex );
			if ( it == m_mapItemShop.end() )
			{
				m_mapItemShop.insert( std::make_pair( sData.wIndex, sData ) );
			}
		}
	}
	LeaveCriticalSection(&m_csItemShop);
}

const SGAME_ITEM_SHOP_DATA* GLGameItemShopServer::GetItem( WORD wIndex )
{
	if ( wIndex == GAME_ITEM_SHOP_INDEX_NULL )	
		return NULL;

	SGAME_ITEM_SHOP_DATA_MAP_CITER it = m_mapItemShop.find( wIndex );
	if ( it != m_mapItemShop.end() )
		return &it->second;

	return NULL;
}

DWORD GLGameItemShopServer::GetItemShopSize()
{
	DWORD dwSize = 0;
	EnterCriticalSection(&m_csItemShop);
	{
		dwSize = m_mapItemShop.size();
	}
	LeaveCriticalSection(&m_csItemShop);
	
	return dwSize;
}

void GLGameItemShopServer::ReduceStock( WORD wIndex )
{
	EnterCriticalSection(&m_csItemShop);

	SGAME_ITEM_SHOP_DATA_MAP_ITER it = m_mapItemShop.find( wIndex );
	if ( it != m_mapItemShop.end() )
	{
		SGAME_ITEM_SHOP_DATA& sData = (*it).second;
		if ( sData.wStock > 0 )
			sData.wStock -= 1;
	}

	LeaveCriticalSection(&m_csItemShop);
}

void GLGameItemShopServer::RequestPointInfo( DWORD dwUserID )
{
	PGLCHARAG pCharAG = GLAgentServer::GetInstance().GetCharUID( dwUserID );
	if ( !pCharAG )
		return;

	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( !pDbMan )	
		return;

	//cache the data
	AddPointInfo( dwUserID, 0, 0 );

	CGISGetUserPoints *pDBAction1 = new CGISGetUserPoints( dwUserID, FALSE );
	pDbMan->AddJob ( pDBAction1 );

	CGISGetVotePoints *pDBAction2 = new CGISGetVotePoints( dwUserID, FALSE );
	pDbMan->AddJob ( pDBAction2 );
}

void GLGameItemShopServer::AddPointInfo( DWORD dwUserID, DWORD dwUserPoint, DWORD dwVotePoint )
{
	EnterCriticalSection(&m_csItemShop);
	{
		SGAME_ITEM_SHOP_POINT_INFO_MAP_ITER it = m_mapPointInfo.find( dwUserID );
		if ( it != m_mapPointInfo.end() )
		{
			SGAME_ITEM_SHOP_POINT_INFO& sInfo = (*it).second;
			sInfo.dwUserPoint = dwUserPoint;
			sInfo.dwVotePoint = dwVotePoint;
			sInfo.bUserPointUpdate = TRUE;
			sInfo.bVotePointUpdate = TRUE;
		}
		else
		{
			SGAME_ITEM_SHOP_POINT_INFO sInfo;
			sInfo.dwUserID = dwUserID;
			sInfo.dwUserPoint = dwUserPoint;
			sInfo.dwVotePoint = dwVotePoint;
			sInfo.bUserPointUpdate = TRUE;
			sInfo.bVotePointUpdate = TRUE;

			m_mapPointInfo.insert( std::make_pair( sInfo.dwUserID, sInfo ) );
		}
	}
	LeaveCriticalSection(&m_csItemShop);
}

void GLGameItemShopServer::PointInfoUserPointSetUpdate( DWORD dwUserID, BOOL bSET )
{
	EnterCriticalSection(&m_csItemShop);
	{
		SGAME_ITEM_SHOP_POINT_INFO_MAP_ITER it = m_mapPointInfo.find( dwUserID );
		if ( it != m_mapPointInfo.end() )
		{
			SGAME_ITEM_SHOP_POINT_INFO& sInfo = (*it).second;
			sInfo.bUserPointUpdate = bSET;
		}
	}
	LeaveCriticalSection(&m_csItemShop);
}

void GLGameItemShopServer::PointInfoVotePointSetUpdate( DWORD dwUserID, BOOL bSET )
{
	EnterCriticalSection(&m_csItemShop);
	{
		SGAME_ITEM_SHOP_POINT_INFO_MAP_ITER it = m_mapPointInfo.find( dwUserID );
		if ( it != m_mapPointInfo.end() )
		{
			SGAME_ITEM_SHOP_POINT_INFO& sInfo = (*it).second;
			sInfo.bVotePointUpdate = bSET;
		}
	}
	LeaveCriticalSection(&m_csItemShop);
}

const SGAME_ITEM_SHOP_POINT_INFO* GLGameItemShopServer::GetPointInfo( DWORD dwUserID )
{
	SGAME_ITEM_SHOP_POINT_INFO_MAP_CITER it = m_mapPointInfo.find( dwUserID );
	if ( it != m_mapPointInfo.end() )
		return &it->second;

	return NULL;
}

HRESULT GLGameItemShopServer::MsgProcess( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	switch ( nmg->nType )
	{
	case NET_MSG_GCTRL_C2A_GIS_REQ_OPEN:
		MsgOpenItemShop( nmg, dwClientID, dwGaeaID );
		break;

	case NET_MSG_GCTRL_DB2A_GIS_GET_USER_POINT:
		MsgDBUpdateUserPoint( nmg, dwClientID, dwGaeaID );
		break;

	case NET_MSG_GCTRL_DB2A_GIS_GET_VOTE_POINT:
		MsgDBUpdateVotePoint( nmg, dwClientID, dwGaeaID );
		break;

	case NET_MSG_GCTRL_C2A_GIS_REQ_PURCHASE:
		MsgPurchase( nmg, dwClientID, dwGaeaID );
		break;

	case NET_MSG_GCTRL_DB2A_GIS_PURCHASE_FB:
		MsgDBPurchaseFB( nmg, dwClientID, dwGaeaID );
		break;

	default:
		CDebugSet::ToListView ( "GLGameItemShopServer::MsgProcess() not classified message : TYPE[%d]", nmg->nType );
		break;
	};

	return S_OK;
}

void GLGameItemShopServer::MsgOpenItemShop( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !RPARAM::bGameItemShop )
		return;

	GLMSG::SNETPC_C2A_GIS_REQ_OPEN *pNetMsg = (GLMSG::SNETPC_C2A_GIS_REQ_OPEN *) nmg;

	PGLCHARAG pCharAG = GLAgentServer::GetInstance().GetChar( dwGaeaID );
	if ( !pCharAG )	
		return;

	GLMSG::SNETPC_A2C_GIS_REQ_OPEN_FB NetMsgFB;

	//cooldown
	if ( pCharAG->m_fGameItemShopOpenTimer < GAME_ITEM_SHOP_OPEN_COOLTIME )
	{
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_OPEN_COOLTIME;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	//shop reloading
	if ( m_bReloading )
	{
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_OPEN_RELOADING;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	//start
	NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_OPEN_START;
	GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );

	EnterCriticalSection(&m_csItemShop);
	{
		GLMSG::SNETPC_A2C_GIS_REQ_UPDATE	NetMsgUpdate;

		SGAME_ITEM_SHOP_DATA_MAP_CITER it_b = m_mapItemShop.begin();
		SGAME_ITEM_SHOP_DATA_MAP_CITER it_e = m_mapItemShop.end();
		for ( ; it_b != it_e; ++it_b )
		{
			if ( m_bReloading )
				break;

			const SGAME_ITEM_SHOP_DATA& sData = (*it_b).second;

			NetMsgUpdate.ADD( sData );
			if ( NetMsgUpdate.wDataNum == GAME_ITEM_SHOP_UPDATE_NUM )
			{
				GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgUpdate );
				NetMsgUpdate.Reset();
			}
		}

		if ( NetMsgUpdate.wDataNum != 0 )
		{
			GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgUpdate );
			NetMsgUpdate.Reset();
		}

		//end
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_OPEN_END;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );

	}
	LeaveCriticalSection(&m_csItemShop);

	pCharAG->m_fGameItemShopOpenTimer = 0.0f;



	EnterCriticalSection(&m_csItemShop);
	{
		SGAME_ITEM_SHOP_POINT_INFO_MAP_ITER it = m_mapPointInfo.find( pCharAG->m_dwUserID );
		if ( it != m_mapPointInfo.end() )
		{
			const SGAME_ITEM_SHOP_POINT_INFO& sInfo = (*it).second;

			GLMSG::SNETPC_A2C_GIS_USER_POINT_UPDATE	NetMsgUpdate1;
			NetMsgUpdate1.dwUserPoint = sInfo.dwUserPoint;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgUpdate1 );

			GLMSG::SNETPC_A2C_GIS_VOTE_POINT_UPDATE	NetMsgUpdate2;
			NetMsgUpdate2.dwVotePoint = sInfo.dwVotePoint;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgUpdate2 );
		}
	}
	LeaveCriticalSection(&m_csItemShop);

}

void GLGameItemShopServer::MsgDBUpdateUserPoint( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNET_GIS_GET_USER_POINTS_DB2A *pNetMsg = (GLMSG::SNET_GIS_GET_USER_POINTS_DB2A *) nmg;

	PGLCHARAG pCharAG = GLAgentServer::GetInstance().GetCharUID( pNetMsg->dwUserNum );
	if ( !pCharAG )
		return;

	//this check is made to make sure the message is coming from db action and not from packet flood
	if ( pNetMsg->dwMSGKey1 != GAME_ITEM_SHOP_MSG_KEY1 )
		return;

	//its very unlikely for this to be wrong
	if ( pNetMsg->dwMSGKey2 != GAME_ITEM_SHOP_MSG_KEY2 )
		return;

	EnterCriticalSection(&m_csItemShop);
	{
		SGAME_ITEM_SHOP_POINT_INFO_MAP_ITER it = m_mapPointInfo.find( pNetMsg->dwUserNum );
		if ( it != m_mapPointInfo.end() )
		{
			SGAME_ITEM_SHOP_POINT_INFO& sInfo = (*it).second;
			sInfo.dwUserPoint = pNetMsg->dwUserPoint;
			sInfo.bUserPointUpdate = FALSE;

			if ( pNetMsg->bMSG )
			{
				GLMSG::SNETPC_A2C_GIS_USER_POINT_UPDATE	NetMsgUpdate;
				NetMsgUpdate.dwUserPoint = sInfo.dwUserPoint;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgUpdate );
			}
		}
	}
	LeaveCriticalSection(&m_csItemShop);

}

void GLGameItemShopServer::MsgDBUpdateVotePoint( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	GLMSG::SNET_GIS_GET_VOTE_POINTS_DB2A *pNetMsg = (GLMSG::SNET_GIS_GET_VOTE_POINTS_DB2A *) nmg;

	PGLCHARAG pCharAG = GLAgentServer::GetInstance().GetCharUID( pNetMsg->dwUserNum );
	if ( !pCharAG )
		return;

	//this check is made to make sure the message is coming from db action and not from packet flood
	if ( pNetMsg->dwMSGKey1 != GAME_ITEM_SHOP_MSG_KEY1 )
		return;

	//its very unlikely for this to be wrong
	if ( pNetMsg->dwMSGKey2 != GAME_ITEM_SHOP_MSG_KEY2 )
		return;

	EnterCriticalSection(&m_csItemShop);
	{
		SGAME_ITEM_SHOP_POINT_INFO_MAP_ITER it = m_mapPointInfo.find( pNetMsg->dwUserNum );
		if ( it != m_mapPointInfo.end() )
		{
			SGAME_ITEM_SHOP_POINT_INFO& sInfo = (*it).second;
			sInfo.dwVotePoint = pNetMsg->dwVotePoint;
			sInfo.bVotePointUpdate = FALSE;

			if ( pNetMsg->bMSG )
			{
				GLMSG::SNETPC_A2C_GIS_VOTE_POINT_UPDATE	NetMsgUpdate;
				NetMsgUpdate.dwVotePoint = sInfo.dwVotePoint;
				GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgUpdate );
			}
		}
	}
	LeaveCriticalSection(&m_csItemShop);
}

void GLGameItemShopServer::MsgPurchase( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !RPARAM::bGameItemShop )
		return;

	PGLCHARAG pCharAG1 = GLAgentServer::GetInstance().GetChar( dwGaeaID );
	if ( !pCharAG1 )
		return;

	GLMSG::SNETPC_C2A_GIS_REQ_PURCHASE *pNetMsg = (GLMSG::SNETPC_C2A_GIS_REQ_PURCHASE *) nmg;

	PGLCHARAG pCharAG2 = GLAgentServer::GetInstance().GetChar( pNetMsg->dwGaeaID );
	if ( !pCharAG2 )
		return;

	PGLCHARAG pCharAG = GLAgentServer::GetInstance().GetCharID( pNetMsg->dwCharID );
	if ( !pCharAG )
		return;


	//paranoid check
	if ( pCharAG1->m_dwGaeaID != pCharAG2->m_dwGaeaID )
		return;

	if ( pCharAG2->m_dwGaeaID != pCharAG->m_dwGaeaID )
		return;

	if ( pCharAG1->m_dwUserID != pCharAG2->m_dwUserID )
		return;

	if ( pCharAG2->m_dwUserID != pCharAG->m_dwUserID )
		return;

	if ( pCharAG1->m_dwCharID != pCharAG2->m_dwCharID )
		return;

	if ( pCharAG2->m_dwCharID != pCharAG->m_dwCharID )
		return;


	GLMSG::SNETPC_C2A_GIS_REQ_PURCHASE_FB NetMsgFB;
	NetMsgFB.wIndex = pNetMsg->wIndex;

	//purchase ongoing , point update ongoing
	if ( pCharAG->m_bGameItemShopRequestPurchase )
	{
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_ONGOING;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	//cooldown
	if ( pCharAG->m_fGameItemShopPurchaseTimer < GAME_ITEM_SHOP_PURCHASE_COOLTIME )
	{
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_COOLDOWN;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	//shop reloading
	if ( m_bReloading )
	{
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_RELOADING;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	const SGAME_ITEM_SHOP_DATA* pShopItem = GetItem( pNetMsg->wIndex );
	if ( !pShopItem )
	{
		//invalid item
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_ERROR;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	const SGAME_ITEM_SHOP_POINT_INFO* pPointInfo = GetPointInfo( pCharAG->m_dwUserID );
	if ( !pPointInfo )
	{
		//no point info
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_ERROR;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	if ( pPointInfo->bUserPointUpdate || pPointInfo->bVotePointUpdate )
	{
		//pending point update
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_POINT_PENDING;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}
	


	//userpoint check
	if ( pShopItem->wType == GAME_ITEM_SHOP_TYPE_POINT )
	{
		if ( pPointInfo->dwUserPoint < pShopItem->wPrice )
		{
			//no point info
			NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_NO_USER_POINT;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
			return;
		}
	}

	//vote point check
	if ( pShopItem->wType == GAME_ITEM_SHOP_TYPE_VOTE )
	{
		if ( pPointInfo->dwVotePoint < pShopItem->wPrice )
		{
			//no point info
			NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_NO_VOTE_POINT;
			GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
			return;
		}
	}

	//type check
	if ( pShopItem->wType == GAME_ITEM_SHOP_TYPE_NONE )
	{
		//no point info
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_NO_TYPE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	if ( pShopItem->wType >= GAME_ITEM_SHOP_TYPE_SIZE )
	{
		//no point info
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_NO_TYPE;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	if ( pShopItem->wStock == 0 )
	{
		//stocks
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_NO_STOCK;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
		return;
	}

	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( !pDbMan )	
		return;

	//add flag if purchase is ongoing and add cooldown timer
	//double protection i guess?
	//what could go wrong
	pCharAG->m_bGameItemShopRequestPurchase = true;
	pCharAG->m_fGameItemShopPurchaseTimer = 0.0f;

	CGISDoPurchase *pDBActionPurchase = new CGISDoPurchase( pCharAG->m_dwUserID, pCharAG->m_szUserName, pShopItem->wIndex, pShopItem->sItemID.wMainID, pShopItem->sItemID.wSubID );
	pDbMan->AddJob ( pDBActionPurchase );
}


void GLGameItemShopServer::MsgDBPurchaseFB( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID )
{
	if ( !RPARAM::bGameItemShop )
		return;

	GLMSG::SNET_DB2A_GIS_PURCHASE *pNetMsg = (GLMSG::SNET_DB2A_GIS_PURCHASE *) nmg;

	//this check is made to make sure the message is coming from db action and not from packet flood
	if ( pNetMsg->dwMSGKey1 != GAME_ITEM_SHOP_MSG_KEY1 )
		return;

	//its very unlikely for this to be wrong
	if ( pNetMsg->dwMSGKey2 != GAME_ITEM_SHOP_MSG_KEY2 )
		return;

	GLDBMan* pDbMan = GLAgentServer::GetInstance().GetDBMan();
	if ( !pDbMan )	
		return;

	const SGAME_ITEM_SHOP_DATA* pShopItem = GetItem( pNetMsg->wIndex );
	if ( !pShopItem )
		return;

	if ( pNetMsg->nRet == 0 )
	{
		ReduceStock( pNetMsg->wIndex );

		//reduce point
		//this has to happen even without checking if pCharAG is valid or not
		//the user could have logged out at this point so if we check pCharAG here the point cannot be deducted
	
		if ( pShopItem->wType == GAME_ITEM_SHOP_TYPE_POINT )
		{
			CGISConsumeUserPoints *pDBAction = new CGISConsumeUserPoints( pNetMsg->dwUserID, pShopItem->wPrice );
			pDbMan->AddJob ( pDBAction );
			
			PointInfoUserPointSetUpdate( pNetMsg->dwUserID, TRUE );
		}
		else if ( pShopItem->wType == GAME_ITEM_SHOP_TYPE_VOTE )
		{
			CGISConsumeVotePoints *pDBAction = new CGISConsumeVotePoints( pNetMsg->dwUserID, pShopItem->wPrice );
			pDbMan->AddJob ( pDBAction );

			PointInfoVotePointSetUpdate( pNetMsg->dwUserID, TRUE );
		}

	}


	GLMSG::SNETPC_C2A_GIS_REQ_PURCHASE_FB NetMsgFB;
	NetMsgFB.wIndex = pNetMsg->wIndex;

	switch ( pNetMsg->nRet )
	{
	case -1: //insert failed
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_ERROR;
		break;

	case 1: //item not found
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_ERROR;
		break;

	case 2: //no stocks
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_NO_STOCK;
		break;

	case 3: //product type is zero, not for purchase
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_NO_TYPE;
		break;

	case 0: //all goods
		NetMsgFB.emFB = EMFB_GAME_ITEM_SHOP_PURCHASE_DONE;
		break;
	};

	PGLCHARAG pCharAG = GLAgentServer::GetInstance().GetCharUID( pNetMsg->dwUserID );
	if ( pCharAG )
	{
		pCharAG->m_bGameItemShopRequestPurchase = false;
		GLAgentServer::GetInstance().SENDTOCLIENT ( pCharAG->m_dwClientID, &NetMsgFB );
	}

	
}

void GLGameItemShopServer::FrameMove( float fElapsedTime )
{
	if ( m_bReloadCountdown )
	{
		m_fReloadCountdownTimer += fElapsedTime;

		if ( m_fReloadCountdownTimer >= GAME_ITEM_SHOP_RELOAD_START_TIMER )
		{
			//turn off countdown
			m_bReloadCountdown = FALSE;
			m_fReloadCountdownTimer = 0.0f;
			DoShopReload();
		}
	}
}

void GLGameItemShopServer::RequestShopReload()
{
	//close shop in all clients
	GLMSG::SNETPC_GM_GIS_RELOAD_CLOSE NetMsg;
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

	m_bReloading = TRUE;

	//add countdown timer before starting reload
	//this will allow previous transactions to be completed first
	m_bReloadCountdown = TRUE;
	m_fReloadCountdownTimer = 0.0f;
}

void GLGameItemShopServer::DoShopReload()
{
	if ( m_bReloading == FALSE )
		return;

	LoadItemShop();

	GLMSG::SNETPC_GM_GIS_RELOAD_DONE NetMsg;
	GLAgentServer::GetInstance().SENDTOALLCLIENT ( &NetMsg );

	m_bReloading = FALSE;
}