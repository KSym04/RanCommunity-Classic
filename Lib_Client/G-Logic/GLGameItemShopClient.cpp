#include "stdafx.h"

#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/GameItemShopWindow.h"

#include "./GLGaeaClient.h"
#include "./DxGlobalStage.h"

#include "./GLGameItemShopClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLGameItemShopClient& GLGameItemShopClient::GetInstance()
{
	static GLGameItemShopClient _instance;
	return _instance;
}

GLGameItemShopClient::GLGameItemShopClient()
	: m_bRequestOpen(false)
	, m_bRequestPurchase(false)
	, m_dwUserPoint(0)
	, m_dwVotePoint(0)
{
	InitializeCriticalSection( &m_csItemShop );
}

GLGameItemShopClient::~GLGameItemShopClient()
{
	DeleteCriticalSection(&m_csItemShop);
	m_mapItemShop.clear();
}

void GLGameItemShopClient::CleanUp()
{
	EnterCriticalSection(&m_csItemShop);
	{
		m_mapItemShop.clear();
	}
	LeaveCriticalSection(&m_csItemShop);
}

void GLGameItemShopClient::AddItem( const SGAME_ITEM_SHOP_DATA& sData )
{
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

const SGAME_ITEM_SHOP_DATA* GLGameItemShopClient::GetItem( WORD wIndex )
{
	if ( wIndex == GAME_ITEM_SHOP_INDEX_NULL )	
		return NULL;

	SGAME_ITEM_SHOP_DATA_MAP_CITER it = m_mapItemShop.find( wIndex );
	if ( it != m_mapItemShop.end() )
		return &it->second;

	return NULL;
}

DWORD GLGameItemShopClient::GetItemShopSize()
{
	DWORD dwSize = 0;
	EnterCriticalSection(&m_csItemShop);
	{
		dwSize = m_mapItemShop.size();
	}
	LeaveCriticalSection(&m_csItemShop);

	return dwSize;
}

void GLGameItemShopClient::ReduceStock( WORD wIndex )
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


void GLGameItemShopClient::FrameMove( float fTime, float fElapsedTime )
{
}

void GLGameItemShopClient::OpenItemShop()
{
	//
	BOOL isVisible = CInnerInterface::GetInstance().IsVisibleGroup( GAME_ITEM_SHOP_WINDOW );
	if ( !isVisible )
	{
		if ( m_bRequestOpen )	
			return;

		m_bRequestOpen = true;

		GLMSG::SNETPC_C2A_GIS_REQ_OPEN NetMsg;
		NETSEND(&NetMsg);
	}
}

void GLGameItemShopClient::RequestPurchase( WORD wIndex )
{
	const SGAME_ITEM_SHOP_DATA* pShopItem = GetItem( wIndex );
	if ( !pShopItem )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_ERROR") );
		return;
	}

	if ( m_bRequestPurchase )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_ONGOING") );
		return;
	}

	if ( pShopItem->wType == GAME_ITEM_SHOP_TYPE_POINT )
	{
		if ( m_dwUserPoint < pShopItem->wPrice )
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_NO_USER_POINT") );
			return;
		}
	}

	if ( pShopItem->wType == GAME_ITEM_SHOP_TYPE_VOTE )
	{
		if ( m_dwVotePoint < pShopItem->wPrice )
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_NO_VOTE_POINT") );
			return;
		}
	}

	if ( pShopItem->wType == GAME_ITEM_SHOP_TYPE_NONE )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_NO_TYPE") );
		return;
	}

	if ( pShopItem->wType >= GAME_ITEM_SHOP_TYPE_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_NO_TYPE") );
		return;
	}

	if ( pShopItem->wStock == 0 )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_NO_STOCK") );
		return;
	}

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter )
		return;

	GLMSG::SNETPC_C2A_GIS_REQ_PURCHASE NetMsg;
	NetMsg.wIndex = wIndex;
	NetMsg.dwGaeaID = pCharacter->m_dwGaeaID;
	NetMsg.dwCharID = pCharacter->m_dwCharID;
	NETSEND(&NetMsg);

	m_bRequestPurchase = true;

}

HRESULT GLGameItemShopClient::MsgProcess( NET_MSG_GENERIC* nmg )
{
	switch ( nmg->nType )
	{
	case NET_MSG_GCTRL_A2C_GIS_REQ_OPEN_FB:
		MsgOpenItemShopFB( nmg );
		break;
	
	case NET_MSG_GCTRL_A2C_GIS_REQ_OPEN_UPDATE:
		MsgOpenItemShopUpdate( nmg );
		break;

	case NET_MSG_GCTRL_A2C_GIS_USER_POINT_UPDATE:
		MsgUserPointUpdate(nmg);
		break;

	case NET_MSG_GCTRL_A2C_GIS_VOTE_POINT_UPDATE:
		MsgVotePointUpdate(nmg);
		break;

	case NET_MSG_GCTRL_C2A_GIS_REQ_PURCHASE_FB:
		MsgPurchaseFB(nmg);
		break;

	default:
		CDebugSet::ToListView ( "GLGameItemShopClient::MsgProcess() not classified message : TYPE[%d]", nmg->nType );
		break;
	};

	return S_OK;
}

void GLGameItemShopClient::MsgOpenItemShopFB( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_A2C_GIS_REQ_OPEN_FB *pNetMsg = (GLMSG::SNETPC_A2C_GIS_REQ_OPEN_FB *) nmg;
	switch( pNetMsg->emFB )
	{
	case EMFB_GAME_ITEM_SHOP_OPEN_ERROR:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_OPEN_ERROR") );
			m_bRequestOpen = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_OPEN_COOLTIME:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_OPEN_COOLTIME"), GAME_ITEM_SHOP_OPEN_COOLTIME );
			m_bRequestOpen = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_OPEN_START:
		{
			CleanUp();
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::BRIGHTGREEN, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_OPEN_START") );
		}
		break;

	case EMFB_GAME_ITEM_SHOP_OPEN_END:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::BRIGHTGREEN, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_OPEN_END"), GetItemShopSize() );

			CInnerInterface::GetInstance().ShowGroupFocus( GAME_ITEM_SHOP_WINDOW );
			CInnerInterface::GetInstance().GetGameItemShopWindow()->ReloadShop();

			m_bRequestOpen = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_OPEN_RELOADING:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_OPEN_RELOADING") );
			m_bRequestOpen = false;
		}break;
	};
}

void GLGameItemShopClient::MsgOpenItemShopUpdate( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_A2C_GIS_REQ_UPDATE *pNetMsg = (GLMSG::SNETPC_A2C_GIS_REQ_UPDATE *) nmg;
	for ( WORD i=0; i<pNetMsg->wDataNum; ++i )
	{
		AddItem( pNetMsg->sData[i] );
	}
}

void GLGameItemShopClient::MsgUserPointUpdate( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_A2C_GIS_USER_POINT_UPDATE *pNetMsg = (GLMSG::SNETPC_A2C_GIS_USER_POINT_UPDATE *) nmg;
	m_dwUserPoint = pNetMsg->dwUserPoint;
}

void GLGameItemShopClient::MsgVotePointUpdate( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_A2C_GIS_VOTE_POINT_UPDATE *pNetMsg = (GLMSG::SNETPC_A2C_GIS_VOTE_POINT_UPDATE *) nmg;
	m_dwVotePoint = pNetMsg->dwVotePoint;
}

void GLGameItemShopClient::MsgPurchaseFB( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_C2A_GIS_REQ_PURCHASE_FB *pNetMsg = (GLMSG::SNETPC_C2A_GIS_REQ_PURCHASE_FB *) nmg;


	switch( pNetMsg->emFB )
	{
	case EMFB_GAME_ITEM_SHOP_PURCHASE_ERROR:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_ERROR") );
			m_bRequestPurchase = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_PURCHASE_ONGOING:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_ONGOING") );
			m_bRequestPurchase = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_PURCHASE_POINT_PENDING:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_POINT_PENDING") );
			m_bRequestPurchase = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_PURCHASE_NO_USER_POINT:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_NO_USER_POINT") );
			m_bRequestPurchase = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_PURCHASE_NO_VOTE_POINT:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_NO_VOTE_POINT") );
			m_bRequestPurchase = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_PURCHASE_NO_TYPE:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_NO_TYPE") );
			m_bRequestPurchase = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_PURCHASE_NO_STOCK:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_NO_STOCK") );
			m_bRequestPurchase = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_PURCHASE_COOLDOWN:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_COOLDOWN") );
			m_bRequestPurchase = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_PURCHASE_RELOADING:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_RELOADING") );
			m_bRequestPurchase = false;
		}
		break;

	case EMFB_GAME_ITEM_SHOP_PURCHASE_DONE:
		{
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::BRIGHTGREEN, ID2GAMEINTEXT("EMFB_GAME_ITEM_SHOP_PURCHASE_DONE") );
			m_bRequestPurchase = false;

			ReduceStock( pNetMsg->wIndex );
			

		}break;
	};

}