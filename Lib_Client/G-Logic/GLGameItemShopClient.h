/*!
 * \file GLGameItemShopClient.h
 *
 * \author Juver
 * \date 2021/11/03
 *
 * 
 */

#ifndef GLGAMEITEMSHOPCLIENT_H_INCLUDED__
#define GLGAMEITEMSHOPCLIENT_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLGameItemShopDef.h"

struct NET_MSG_GENERIC;

class GLGameItemShopClient
{
public:
	CRITICAL_SECTION			m_csItemShop;
	SGAME_ITEM_SHOP_DATA_MAP	m_mapItemShop;

	bool	m_bRequestOpen;
	bool	m_bRequestPurchase;

	DWORD	m_dwUserPoint;
	DWORD	m_dwVotePoint;

public:
	void CleanUp();
	void AddItem( const SGAME_ITEM_SHOP_DATA& sData );
	const SGAME_ITEM_SHOP_DATA*	GetItem( WORD wIndex );
	DWORD GetItemShopSize();
	void ReduceStock( WORD wIndex );

	void FrameMove( float fTime, float fElapsedTime );

	void OpenItemShop();
	void RequestPurchase ( WORD wIndex );

	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg );

	void MsgOpenItemShopFB( NET_MSG_GENERIC* nmg );
	void MsgOpenItemShopUpdate( NET_MSG_GENERIC* nmg );
	void MsgUserPointUpdate( NET_MSG_GENERIC* nmg );
	void MsgVotePointUpdate( NET_MSG_GENERIC* nmg );
	void MsgPurchaseFB( NET_MSG_GENERIC* nmg );

public:
	static GLGameItemShopClient& GetInstance();

	GLGameItemShopClient();
	~GLGameItemShopClient();
};


#endif // GLGAMEITEMSHOPCLIENT_H_INCLUDED__
