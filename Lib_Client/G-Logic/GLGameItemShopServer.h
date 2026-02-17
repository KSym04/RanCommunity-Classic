/*!
 * \file GLGameItemShopServer.h
 *
 * \author Juver
 * \date 2021/11/03
 *
 * 
 */

#ifndef GLGAMEITEMSHOPSERVER_H_INCLUDED__
#define GLGAMEITEMSHOPSERVER_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLGameItemShopDef.h"

class GLGameItemShopServer
{
public:
	CRITICAL_SECTION			m_csItemShop;
	CRITICAL_SECTION			m_csPointInfo;

	SGAME_ITEM_SHOP_DATA_MAP	m_mapItemShop;
	SGAME_ITEM_SHOP_POINT_INFO_MAP	m_mapPointInfo;

	BOOL	m_bReloading;
	BOOL	m_bReloadCountdown;
	float	m_fReloadCountdownTimer;

public:
	void CleanUp();
	void LoadItemShop();

	void AddItem( const SGAME_ITEM_SHOP_DATA& sData );
	const SGAME_ITEM_SHOP_DATA*	GetItem( WORD wIndex );
	DWORD GetItemShopSize();
	void ReduceStock( WORD wIndex );
	
	void RequestPointInfo( DWORD dwUserID );
	void AddPointInfo( DWORD dwUserID, DWORD dwUserPoint, DWORD dwVotePoint );
	void PointInfoUserPointSetUpdate( DWORD dwUserID, BOOL bSET );
	void PointInfoVotePointSetUpdate( DWORD dwUserID, BOOL bSET );
	const SGAME_ITEM_SHOP_POINT_INFO* GetPointInfo( DWORD dwUserID );

	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

	void MsgOpenItemShop( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );
	void MsgDBUpdateUserPoint( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );
	void MsgDBUpdateVotePoint( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );
	void MsgPurchase( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );
	void MsgDBPurchaseFB( NET_MSG_GENERIC* nmg, DWORD dwClientID, DWORD dwGaeaID );

	void FrameMove( float fElapsedTime );
	void RequestShopReload();
	void DoShopReload();

public:
	static GLGameItemShopServer& GetInstance();

	GLGameItemShopServer();
	~GLGameItemShopServer();
};

#endif // GLGAMEITEMSHOPSERVER_H_INCLUDED__
