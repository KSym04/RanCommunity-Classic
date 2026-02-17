#include "stdafx.h"
#include "./GLChar.h"
#include "./GLGaeaServer.h"
#include "./GLItemMan.h"
#include "./GLSkill.h"
#include "./GLItemMixMan.h"
#include "./GLCrowData.h"
#include "./GLSetOption.h"
/*12-9-14, Battle Pass - CNDev*/
#include "./GLBattlePassReward.h"
#include "../../Lib_Engine/Common/StringUtils.h"
#include "../../Lib_Engine/Utils/RanFilter.h"
#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRLog.h"

/* chaos machine, Juver, 2021/07/10 */
#include "./GLChaosMachine.h"

#include "./MapDropFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*itemfood system, Juver, 2017/05/25 */
HRESULT GLChar::MsgReqConsumeFood( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_CONSUME_FOOD* pNetMsg = (GLMSG::SNET_INVEN_CONSUME_FOOD*)nmg;
	GLMSG::SNET_INVEN_CONSUME_FOOD_FB	NetMsgFB;

	SINVENITEM *pINVENITEM = m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		NetMsgFB.emFB = EMCONSUME_FOOD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pITEM )
	{
		NetMsgFB.emFB = EMCONSUME_FOOD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pITEM->sBasicOp.emItemType != ITEM_FOOD )
	{
		NetMsgFB.emFB = EMCONSUME_FOOD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( m_pLandMan && m_pLandMan->IsLunchBoxForbid() )
	{
		NetMsgFB.emFB = EMCONSUME_FOOD_FB_NONAREA;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		NetMsgFB.emFB = EMCONSUME_FOOD_FB_COOLTIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( pITEM->sSkillBookOp.sSkill_ID );
	if ( !pSKILL )
	{
		NetMsgFB.emFB = EMCONSUME_FOOD_FB_NONDATA;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SNATIVEID _sidSKILL = pSKILL->m_sBASIC.sNATIVEID;
	WORD _wSLOT = ItemFoodFindSlot( _sidSKILL );
	if ( _wSLOT == FITEMFACT_SIZE )
	{
		NetMsgFB.emFB = EMCONSUME_FOOD_FB_NOSLOT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	WORD _wUSESLOT = ItemFoodAdd( _sidSKILL, 0, _wSLOT );
	if ( _wUSESLOT == FITEMFACT_SIZE )
	{
		NetMsgFB.emFB = EMCONSUME_FOOD_FB_NOSLOT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	GLMSG::SNETPC_FITEMFACT_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.sidSkill = _sidSKILL;
	NetMsgBrd.wSLOT = _wUSESLOT;
	NetMsgBrd.sFACT = m_sFITEMFACT[_wUSESLOT];
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgBrd);
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	NetMsgFB.emFB = EMCONSUME_FOOD_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}

/*itemfood system, Juver, 2017/05/26 */
HRESULT GLChar::MsgReqUnlockFood( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_UNLOCK_FOOD* pNetMsg = (GLMSG::SNET_INVEN_UNLOCK_FOOD*)nmg;
	GLMSG::SNET_INVEN_UNLOCK_FOOD_FB	NetMsgFB;

	SINVENITEM *pINVENITEM = m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		NetMsgFB.emFB = EMUNLOCK_FOOD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pITEM )
	{
		NetMsgFB.emFB = EMUNLOCK_FOOD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pITEM->sBasicOp.emItemType != ITEM_CARD_FOODUNLOCK )
	{
		NetMsgFB.emFB = EMUNLOCK_FOOD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	BOOL bUNLOCKED = TRUE;
	for( int i=0; i<FITEMFACT_SIZE; ++i ){
		if ( !m_sFITEMFACT[i].bENABLE ){
			bUNLOCKED = FALSE;
			break;
		}
	}

	if ( bUNLOCKED )
	{
		NetMsgFB.emFB = EMUNLOCK_FOOD_FB_UNLOCKED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	for( int i=0; i<FITEMFACT_SIZE; ++i ){
		m_sFITEMFACT[i].bENABLE = TRUE;
	}

	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	NetMsgFB.emFB = EMUNLOCK_FOOD_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}

/*rightclick wear/unwear, Juver, 2017/06/24 */
HRESULT GLChar::MsgReqInvenToWear ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_INVEN_TO_WEAR *pNetMsgClient = reinterpret_cast<GLMSG::SNETPC_REQ_INVEN_TO_WEAR*> ( nmg );

	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_EQUIP ) && !( pNetMsgClient->emSlot == SLOT_LHAND || pNetMsgClient->emSlot == SLOT_LHAND_S ) )
		return S_FALSE;

	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_INVEN ) )
		return S_FALSE;

	GLMSG::SNETPC_REQ_INVEN_TO_HOLD NetMsgInvenToHold;
	NetMsgInvenToHold.wPosX = pNetMsgClient->wPosX;
	NetMsgInvenToHold.wPosY = pNetMsgClient->wPosY;
	HRESULT hrInvenToHold = MsgReqInvenToHold( (NET_MSG_GENERIC*)&NetMsgInvenToHold );
	if ( hrInvenToHold != S_OK )	return S_FALSE;
	
	BOOL bEMPTY_SLOT = ISEMPTY_SLOT( GET_HOLD_ITEM().sNativeID, pNetMsgClient->emSlot);
	if ( VALID_HOLD_ITEM() && !bEMPTY_SLOT )
	{
		GLMSG::SNETPC_REQ_SLOT_EX_HOLD NetMsgHoldToSlot;
		NetMsgHoldToSlot.emSlot = pNetMsgClient->emSlot;
		HRESULT hrWear = MsgReqSlotExHold( (NET_MSG_GENERIC*)&NetMsgHoldToSlot );
		if ( hrWear != S_OK ) return S_FALSE;
	}
	else if ( VALID_HOLD_ITEM() )
	{
		GLMSG::SNETPC_REQ_HOLD_TO_SLOT NetMsgHoldToSlot;
		NetMsgHoldToSlot.emSlot = pNetMsgClient->emSlot;
		HRESULT hrWear = MsgReqHoldToSlot( (NET_MSG_GENERIC*)&NetMsgHoldToSlot );
		if ( hrWear != S_OK ) return S_FALSE;
	}

	GLMSG::SNETPC_REQ_HOLD_TO_INVEN NetMsgHoldToInven;
	NetMsgHoldToInven.wPosX = pNetMsgClient->wPosX;
	NetMsgHoldToInven.wPosY = pNetMsgClient->wPosY;
	MsgReqHoldToInven( (NET_MSG_GENERIC*)&NetMsgHoldToInven );

	return S_OK;
}

/*rightclick wear/unwear, Juver, 2017/06/24 */
HRESULT GLChar::MsgReqWearToInven ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_WEAR_TO_INVEN *pNetMsgClient = reinterpret_cast<GLMSG::SNETPC_REQ_WEAR_TO_INVEN*> ( nmg );
	if ( pNetMsgClient->emSlot >= SLOT_TSIZE )		return S_FALSE;

	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_EQUIP ) && !( pNetMsgClient->emSlot == SLOT_LHAND || pNetMsgClient->emSlot == SLOT_LHAND_S ) )
		return S_FALSE;

	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_INVEN ) )
		return S_FALSE;

	GLMSG::SNETPC_REQ_SLOT_TO_HOLD NetMsgHoldToSlot;
	NetMsgHoldToSlot.emSlot = pNetMsgClient->emSlot;
	HRESULT hrUnwear = MsgReqSlotToHold( (NET_MSG_GENERIC*)&NetMsgHoldToSlot );
	if ( hrUnwear != S_OK ) return S_FALSE;

	if( !VALID_HOLD_ITEM() )	return S_FALSE;

	SITEM* pITEMHOLD = GLItemMan::GetInstance().GetItem( GET_HOLD_ITEM().sNativeID );
	if ( !pITEMHOLD )	return S_FALSE;

	WORD wInvenPosX, wInvenPosY;
	BOOL bOk = m_cInventory.FindInsrtable ( pITEMHOLD->sBasicOp.wInvenSizeX, pITEMHOLD->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
	if ( !bOk )	return S_FALSE;

	GLMSG::SNETPC_REQ_HOLD_TO_INVEN NetMsgHoldToInven;
	NetMsgHoldToInven.wPosX = wInvenPosX;
	NetMsgHoldToInven.wPosY = wInvenPosY;
	MsgReqHoldToInven( (NET_MSG_GENERIC*)&NetMsgHoldToInven );

	return S_OK;
}

/*npc shop, Juver, 2017/07/27 */
HRESULT GLChar::ReqNpcShopPurchaseMoney ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_NPCSHOP_PURCHASE_MONEY *pnetmsgclient = reinterpret_cast<GLMSG::SNETPC_REQ_NPCSHOP_PURCHASE_MONEY*> ( nmg );
	GLMSG::SNETPC_REQ_NPCSHOP_PURCHASE_MONEY_FB	netmsgfb;

	SITEM* pitem = GLItemMan::GetInstance().GetItem ( pnetmsgclient->sidItem );
	if ( !pitem )	
	{
		netmsgfb.emFB = EMNPCSHOP_PURCHASE_FB_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	SCROWDATA*	pcrow = GLCrowDataMan::GetInstance().GetCrowData( pnetmsgclient->sidCrow );
	if ( !pcrow )
	{
		netmsgfb.emFB = EMNPCSHOP_PURCHASE_FB_INVALID_CROW;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	SNPC_SHOP_DATA* pNpcShop = GLCrowDataMan::GetInstance().NPCShopFind( pcrow->m_sAction.m_strShopFile );
	if ( !pNpcShop )
	{
		netmsgfb.emFB = EMNPCSHOP_PURCHASE_FB_INVALID_SHOP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	if ( pNpcShop->m_wShopType != pnetmsgclient->wShopType )
	{
		netmsgfb.emFB = EMNPCSHOP_PURCHASE_FB_INVALID_SHOPTYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	const SNPC_SHOP_ITEM* pshopitem = pNpcShop->FindShopItem( pnetmsgclient->sidItem.dwID );
	if ( !pshopitem )
	{
		netmsgfb.emFB = EMNPCSHOP_PURCHASE_FB_INVALID_SHOPITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}


	if ( pNpcShop->m_wShopType == 0 )
	{
		//money check
		volatile LONGLONG llPRICE = (LONGLONG) pitem->sBasicOp.dwBuyPrice;
		if ( m_lnMoney < llPRICE )
		{
			netmsgfb.emFB = EMNPCSHOP_PURCHASE_FB_NOT_MONEY;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			return E_FAIL;
		}

		/*contribution point, Juver, 2017/08/23 */
		if ( pitem->sBasicOp.dwReqContributionPoint != 0 &&
			m_dwContributionPoint < pitem->sBasicOp.dwReqContributionPoint )	
		{
			return E_FAIL;
		}

		/*activity point, Juver, 2017/08/23 */
		if ( pitem->sBasicOp.dwReqActivityPoint != 0 &&
			m_dwActivityPoint < pitem->sBasicOp.dwReqActivityPoint )
		{
			return E_FAIL;
		}

		/* play time system, Juver, 2021/01/27 */
		if ( pitem->sBasicOp.llPlayTimeReq != 0 && m_llPlayTime < pitem->sBasicOp.llPlayTimeReq )
		{
			return E_FAIL;
		}

		WORD winvensizex = pitem->sBasicOp.wInvenSizeX;
		WORD winvensizey = pitem->sBasicOp.wInvenSizeY;

		BOOL bITEM_SPACE(FALSE);
		if ( pitem->ISPILE() )
		{
			WORD wPILENUM = pitem->sDrugOp.wPileNum;
			SNATIVEID sNID = pitem->sBasicOp.sNativeID;
			WORD wREQINSRTNUM = ( 1 * pitem->GETAPPLYNUM() );

			bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, winvensizex, winvensizey );

			if ( !bITEM_SPACE )
			{
				netmsgfb.emFB = EMNPCSHOP_PURCHASE_FB_NOT_INVEN_SPACE;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
				return E_FAIL;
			}

			SITEMCUSTOM snew_item(sNID);
			snew_item.cGenType = EMGEN_SHOP;
			snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
			snew_item.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
			snew_item.wTurnNum = wPILENUM;
			snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();

			/*item color, Juver, 2018/01/08 */
			SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
			if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
			{
				snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
				snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
			}

			GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );

			CheckMoneyUpdate( m_lnMoney, llPRICE, FALSE, "Buy From SHOP." );
			m_bMoneyUpdate = TRUE;

			m_lnMoney -= llPRICE;

			if ( llPRICE>EMMONEY_LOG )
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE), EMITEM_ROUTE_DELETE );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
			}

			InsertPileItem ( snew_item, wREQINSRTNUM );

			/*activity system, Juver, 2017/11/03 */
			DoActivityTakeItem( snew_item.sNativeID, wREQINSRTNUM );
			/*12-9-14, Battle Pass - CNDev*/
			DoBattlePassTakeItem( snew_item.sNativeID, wREQINSRTNUM );	
		}
		else
		{
			WORD wInsertPosX(0), wInsertPosY(0);
			bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );

			if ( !bITEM_SPACE )	
			{
				netmsgfb.emFB = EMNPCSHOP_PURCHASE_FB_NOT_INVEN_SPACE;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
				return E_FAIL;
			}

			SITEMCUSTOM snew_item( pitem->sBasicOp.sNativeID );
			snew_item.cDAMAGE = pshopitem->cDAMAGE;
			snew_item.cDEFENSE = pshopitem->cDEFENSE;
			snew_item.cRESIST_FIRE = pshopitem->cRESIST_FIRE;
			snew_item.cRESIST_ICE = pshopitem->cRESIST_ICE;
			snew_item.cRESIST_ELEC = pshopitem->cRESIST_ELEC;
			snew_item.cRESIST_POISON = pshopitem->cRESIST_POISON;
			snew_item.cRESIST_SPIRIT = pshopitem->cRESIST_SPIRIT;

			CTime cTIME = CTime::GetCurrentTime();
			snew_item.tBORNTIME = cTIME.GetTime();
			snew_item.wTurnNum = 1;
			snew_item.cGenType = (BYTE) EMGEN_SHOP;
			snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
			snew_item.cFieldID = (BYTE) GLGaeaServer::GetInstance().GetFieldSvrID();
			snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pitem->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );

			/*item color, Juver, 2018/01/08 */
			SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
			if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
			{
				snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
				snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
			}

			if ( pshopitem->bGenerateRandomValue )
			{
				if( snew_item.GENERATE_RANDOM_OPT () )
				{
					GLITEMLMT::GetInstance().ReqRandomItem( snew_item );
				}

				/* set item option, Juver, 2021/09/01 */
				if ( pitem_data_check )
				{
					snew_item.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_check->sBasicOp.strGenSetOption );
				}
			}

			CheckMoneyUpdate( m_lnMoney, llPRICE, FALSE, "Buy From SHOP." );
			m_bMoneyUpdate = TRUE;

			m_lnMoney -= llPRICE;

			if ( llPRICE>EMMONEY_LOG )
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPRICE), EMITEM_ROUTE_DELETE );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
			}

			BOOL bOK = m_cInventory.InsertItem ( snew_item, wInsertPosX, wInsertPosY );
			if ( !bOK )		return S_OK;

			GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );

			GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
			NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

			/*activity system, Juver, 2017/11/03 */
			DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
			/*12-9-14, Battle Pass - CNDev*/
			DoBattlePassTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
		}

		GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
		NetMsgMoney.lnMoney = m_lnMoney;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgMoney);
	}

	netmsgfb.emFB = EMNPCSHOP_PURCHASE_FB_PURCHASED;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);

	return S_OK;
}

/*item exchange, Juver, 2017/10/13 */
HRESULT GLChar::ReqNpcItemExchangeTrade ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_NPC_ITEM_EXCHANGE_TRADE *pnetmsgclient = reinterpret_cast<GLMSG::SNETPC_REQ_NPC_ITEM_EXCHANGE_TRADE*> ( nmg );
	GLMSG::SNETPC_REQ_NPC_ITEM_EXCHANGE_TRADE_FB	netmsgfb;

	PCROWDATA pcrow_data = GLCrowDataMan::GetInstance().GetCrowData( pnetmsgclient->sid_NPC );
	if ( !pcrow_data )
	{
		netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_INVALID_NPC;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_INVALID_NPC %d~%d", m_dwCharID, m_szName,
			pnetmsgclient->sid_NPC.wMainID, pnetmsgclient->sid_NPC.wSubID );
		return E_FAIL;
	}

	SNPC_ITEM_EXCHANGE* pItemExchange = GLCrowDataMan::GetInstance().NPCItemExchangeFind( pcrow_data->m_sAction.m_strItemExchangeFile );
	if ( !pItemExchange )
	{
		netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_INVALID_DATA;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_INVALID_DATA %d", m_dwCharID, m_szName,
			pnetmsgclient->wTradeID );
		return E_FAIL;
	}

	const SNPC_ITEM_EXCHANGE_DATA* pnpc_item_exchange_data = pItemExchange->FindData( pnetmsgclient->wTradeID );
	if ( !pnpc_item_exchange_data )
	{
		netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_INVALID_ID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_INVALID_ID %d", m_dwCharID, m_szName,
			pnetmsgclient->wTradeID );
		return E_FAIL;
	}

	SITEM* pitem_data_result = GLItemMan::GetInstance().GetItem( pnpc_item_exchange_data->sidItemResult.sidItem );
	if ( !pitem_data_result )
	{
		netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_INVALID_ITEM %d~%d", m_dwCharID, m_szName,
			pnpc_item_exchange_data->sidItemResult.sidItem.wMainID, pnpc_item_exchange_data->sidItemResult.sidItem.wSubID );
		return E_FAIL;
	}

	/*contribution point, Juver, 2017/08/23 */
	if ( pitem_data_result->sBasicOp.dwReqContributionPoint != 0 &&
		m_dwContributionPoint < pitem_data_result->sBasicOp.dwReqContributionPoint )
	{
		netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_NOCONTRIBUTIONPOINT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_NOCONTRIBUTIONPOINT %u~%u", m_dwCharID, m_szName,
			m_dwContributionPoint, pitem_data_result->sBasicOp.dwReqContributionPoint );
		return E_FAIL;
	}

	/* NpcExchange Gold Requirement | JX | 01-11-24*/
	if (pitem_data_result->sBasicOp.dwBuyPrice != 0 &&
	    m_lnMoney < pitem_data_result->sBasicOp.dwBuyPrice)
	{
	    netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_NOMONEY;
	    GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
	    CDebugSet::ToFileWithTime("_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_NOMONEY %d~%d", 
	        m_dwCharID, m_szName, m_lnMoney, pitem_data_result->sBasicOp.dwBuyPrice);
	    return E_FAIL;
	}
	
	// Deduct the gold after confirming enough gold is available
	if (pitem_data_result->sBasicOp.dwBuyPrice > 0)
	{
	    CheckMoneyUpdate(m_lnMoney, pitem_data_result->sBasicOp.dwBuyPrice, FALSE, "NPC Exchange Deduction");
	    m_bMoneyUpdate = TRUE;
	
	    m_lnMoney -= pitem_data_result->sBasicOp.dwBuyPrice;
	
	    GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
	    NetMsgMoney.lnMoney = m_lnMoney;
	    GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);
	}


	/*activity point, Juver, 2017/08/23 */
	if ( pitem_data_result->sBasicOp.dwReqActivityPoint != 0 &&
		m_dwActivityPoint < pitem_data_result->sBasicOp.dwReqActivityPoint )
	{
		netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_NOPOINT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_NOPOINT %u~%u", m_dwCharID, m_szName,
			m_dwActivityPoint, pitem_data_result->sBasicOp.dwReqActivityPoint );
		return E_FAIL;
	}
	

	{
		// Calculate the total contribution points required (similar to your example with llPRICE4)
		volatile LONGLONG llRequiredContriPoints = (LONGLONG)(pitem_data_result->sBasicOp.dwReqContributionPoint);

		// Check if the required contribution points are not zero and the player has enough points
		if ( pitem_data_result->sBasicOp.dwReqContributionPoint != 0 && m_dwContributionPoint < llRequiredContriPoints )
		{
			netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_NOCONTRI; // Change this to a more fitting error message if needed
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
			CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_NOCONTRI %lld~%lld", 
				                       m_dwCharID, m_szName, m_dwContributionPoint, llRequiredContriPoints );
			return E_FAIL;
		}
	}

	/* play time system, Juver, 2021/01/27 */
	if ( pitem_data_result->sBasicOp.llPlayTimeReq != 0 && m_llPlayTime < pitem_data_result->sBasicOp.llPlayTimeReq )
	{
		netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_NO_PLAY_TIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_NO_PLAY_TIME %I64d~%I64d", m_dwCharID, m_szName,
			m_llPlayTime, pitem_data_result->sBasicOp.llPlayTimeReq );
		return E_FAIL;
	}

	//requirements check
	for( int i=0; i<NPC_ITEM_EXCHANGE_MAX_REQ; ++i )
	{
		SITEM* pitem_data_require = GLItemMan::GetInstance().GetItem( pnpc_item_exchange_data->sidItemRequire[i].sidItem );
		if ( pitem_data_require )
		{
			if ( pitem_data_require->ISPILE() )
			{
				WORD wNum = pnpc_item_exchange_data->sidItemRequire[i].wItemNum;
				WORD witem_num = m_cInventory.CountTurnItem( pitem_data_require->sBasicOp.sNativeID );
				if ( witem_num < wNum )
				{
					netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_REQUIRE_MISSING;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
					CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_REQUIRE_MISSING %d~%d %d", m_dwCharID, m_szName,
						pitem_data_require->sBasicOp.sNativeID.wMainID, pitem_data_require->sBasicOp.sNativeID.wSubID, wNum );
					return E_FAIL;
				}

				BOOL bDelete = DeletePileItem( pitem_data_require->sBasicOp.sNativeID, wNum, EMITEM_ROUTE_DELETE );
				if ( !bDelete )
				{
					netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_DELETE_FAIL;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
					CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_DELETE_FAIL %d~%d %d", m_dwCharID, m_szName,
						pitem_data_require->sBasicOp.sNativeID.wMainID, pitem_data_require->sBasicOp.sNativeID.wSubID, wNum );
					return E_FAIL;
				}
			}
			else
			{
				//not pile just check if exist
				SINVENITEM* pinvenitem = m_cInventory.FindItem( pitem_data_require->sBasicOp.sNativeID );
				if ( !pinvenitem )
				{
					netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_REQUIRE_MISSING;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
					CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_REQUIRE_MISSING %d~%d", m_dwCharID, m_szName,
						pitem_data_require->sBasicOp.sNativeID.wMainID, pitem_data_require->sBasicOp.sNativeID.wSubID );
					return E_FAIL;
				}

				WORD wposX = pinvenitem->wPosX;
				WORD wposY = pinvenitem->wPosY;

				GLITEMLMT::GetInstance().ReqItemRoute( pinvenitem->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pinvenitem->sItemCustom.wTurnNum );

				BOOL bDelete = m_cInventory.DeleteItem ( wposX, wposY );
				if ( !bDelete )
				{
					netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_DELETE_FAIL;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
					CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_DELETE_FAIL %d~%d %d~%d", m_dwCharID, m_szName,
						pitem_data_require->sBasicOp.sNativeID.wMainID, pitem_data_require->sBasicOp.sNativeID.wSubID, wposX, wposY );
					return E_FAIL;
				}

				GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
				NetMsg_Inven_Delete.wPosX = wposX;
				NetMsg_Inven_Delete.wPosY = wposY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Delete);
			}
		}
	}

	WORD winvensizex = pitem_data_result->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem_data_result->sBasicOp.wInvenSizeY;

	BOOL bITEM_SPACE(FALSE);
	if ( pitem_data_result->ISPILE() )
	{
		WORD wPILENUM = pitem_data_result->sDrugOp.wPileNum;
		SNATIVEID sNID = pitem_data_result->sBasicOp.sNativeID;
		WORD wREQINSRTNUM = ( pnpc_item_exchange_data->sidItemResult.wItemNum * pitem_data_result->GETAPPLYNUM() );

		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, winvensizex, winvensizey );

		if ( !bITEM_SPACE )
		{
			netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_NOT_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_NOT_INVEN_SPACE", m_dwCharID, m_szName );
			return E_FAIL;
		}

		SITEMCUSTOM snew_item(sNID);
		snew_item.cGenType = EMGEN_NPC;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pitem_data_result->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );
		snew_item.wTurnNum = wPILENUM;
		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		BOOL bOK = InsertPileItem ( snew_item, wREQINSRTNUM, EMITEM_ROUTE_CHAR );
		if ( !bOK )	
		{
			netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_INSERT_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_INSERT_FAIL", m_dwCharID, m_szName );
			return E_FAIL;
		}

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
		/*12-9-14, Battle Pass - CNDev*/
		DoBattlePassTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
	}
	else
	{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );
		if ( !bITEM_SPACE )	
		{
			netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_NOT_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_NOT_INVEN_SPACE", m_dwCharID, m_szName );
			return E_FAIL;
		}

		SITEMCUSTOM snew_item( pitem_data_result->sBasicOp.sNativeID );
		snew_item.cDAMAGE = pnpc_item_exchange_data->cDAMAGE;
		snew_item.cDEFENSE = pnpc_item_exchange_data->cDEFENSE;
		snew_item.cRESIST_FIRE = pnpc_item_exchange_data->cRESIST_FIRE;
		snew_item.cRESIST_ICE = pnpc_item_exchange_data->cRESIST_ICE;
		snew_item.cRESIST_ELEC = pnpc_item_exchange_data->cRESIST_ELEC;
		snew_item.cRESIST_POISON = pnpc_item_exchange_data->cRESIST_POISON;
		snew_item.cRESIST_SPIRIT = pnpc_item_exchange_data->cRESIST_SPIRIT;
		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();
		snew_item.wTurnNum = 1;
		snew_item.cGenType = (BYTE) EMGEN_NPC;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE) GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pitem_data_result->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		if ( pnpc_item_exchange_data->bGenerateRandomValue )
		{
			if( snew_item.GENERATE_RANDOM_OPT () )
			{
				GLITEMLMT::GetInstance().ReqRandomItem( snew_item );
			}

			/* set item option, Juver, 2021/09/01 */
			if ( pitem_data_check )
			{
				snew_item.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_check->sBasicOp.strGenSetOption );
			}
		}
		
		BOOL bOK = m_cInventory.InsertItem ( snew_item, wInsertPosX, wInsertPosY );
		if ( !bOK )	
		{
			netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_INSERT_FAIL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_npcexchange.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_INSERT_FAIL", m_dwCharID, m_szName );
			return E_FAIL;
		}

		/* NpcExchange Gold Requirement | JX | 01-11-24*/
		if ( pitem_data_result->sBasicOp.dwBuyPrice > 0 )
		{
			CheckMoneyUpdate( m_lnMoney, pitem_data_result->sBasicOp.dwBuyPrice, FALSE, "" );
			m_bMoneyUpdate = TRUE;

			m_lnMoney -= pitem_data_result->sBasicOp.dwBuyPrice;

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgMoney );
		}
		

		// Deduct Contribution Points after successful item insert
		if (pitem_data_result->sBasicOp.dwReqContributionPoint > 0)
		{
			volatile LONGLONG llRequiredContriPoints = (LONGLONG)(pitem_data_result->sBasicOp.dwReqContributionPoint);

			CheckMoneyUpdate(m_dwContributionPoint, llRequiredContriPoints, FALSE, "NPC Exchange - Contribution Deduction");
			m_bMoneyUpdate = TRUE;

			m_dwContributionPoint -= llRequiredContriPoints;

			GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT NetMsgContri;
			NetMsgContri.dwPoint = m_dwContributionPoint;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgContri);
		}

		GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );
		
		
		
		GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
		NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
		/*12-9-14, Battle Pass - CNDev*/
		DoBattlePassTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
	}
	

	// Success feedback
	netmsgfb.emFB = EMNPC_ITEM_EXCHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);

	return S_OK;
}

HRESULT GLChar::ReqItemCompoundStart( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_ITEM_COMPOUND_START* pnetmsgclient = (GLMSG::SNETPC_REQ_ITEM_COMPOUND_START*)nmg;
	GLMSG::SNETPC_REQ_ITEM_COMPOUND_START_FB	netmsgfb;

	if ( !RPARAM::bProduct )
	{
		netmsgfb.emFB = RAN_PRODUCT_FB_FEATURE_OFF;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_FEATURE_OFF %d task:%d", m_dwCharID, m_szName,
		pnetmsgclient->dwProductID, m_dwItemCompoundTaskID );
		return E_FAIL;
	}

	if ( m_bItemCompoundTask )
	{
		netmsgfb.emFB = RAN_PRODUCT_FB_TASK_RUNNING;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_TASK_RUNNING %d task:%d", m_dwCharID, m_szName,
		pnetmsgclient->dwProductID, m_dwItemCompoundTaskID );
		return E_FAIL;
	}

	const ITEM_MIX* pitem_mix = GLItemMixMan::GetInstance().GetProduct( pnetmsgclient->dwProductID );
	if ( !pitem_mix )
	{
		netmsgfb.emFB = RAN_PRODUCT_FB_INVALID_ID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_INVALID_ID %d", m_dwCharID, m_szName,
		pnetmsgclient->dwProductID );
		return E_FAIL;
	}

	//money check
	if( m_lnMoney < pitem_mix->dwPrice )
	{
		netmsgfb.emFB = RAN_PRODUCT_FB_INVALID_COST;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_INVALID_COST %d char money:%I64d", m_dwCharID, m_szName,
		pitem_mix->dwPrice, m_lnMoney );
		return E_FAIL;
	}

	//level check
	if ( m_wLevel < pitem_mix->wLevelReq )
	{
		netmsgfb.emFB = RAN_PRODUCT_FB_INVALID_CHAR_LEVEL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_INVALID_CHAR_LEVEL %d char level:%d", m_dwCharID, m_szName,
		pitem_mix->wLevelReq, m_wLevel );
		return E_FAIL;
	}

	/* play time system, Juver, 2021/01/27 */
	if ( m_llPlayTime < pitem_mix->llPlayTimeReq )
	{
		netmsgfb.emFB = RAN_PRODUCT_FB_INVALID_CHAR_PLAY_TIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_INVALID_CHAR_PLAY_TIME %I64d char play time:%I64d", m_dwCharID, m_szName,
			pitem_mix->llPlayTimeReq, m_llPlayTime );
		return E_FAIL;
	}

	/* play time system, Juver, 2021/01/27 */
	if( m_llPlayPoint < pitem_mix->llPlayPointUse )
	{
		netmsgfb.emFB = RAN_PRODUCT_FB_INVALID_CHAR_PLAY_POINT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_INVALID_CHAR_PLAY_POINT %I64d char play points:%I64d", m_dwCharID, m_szName,
			pitem_mix->llPlayPointUse, m_llPlayPoint );
		return E_FAIL;
	}

	if( m_dwContributionPoint < pitem_mix->dwContriUse )
	{
		netmsgfb.emFB = RAN_PRODUCT_FB_INVALID_CHAR_CONTRI_POINT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_INVALID_CHAR_CONTRI_POINT %I64d char play points:%I64d", m_dwCharID, m_szName,
			pitem_mix->dwContriUse, m_dwContributionPoint );
		return E_FAIL;
	}

	SITEM* pitem_data_result = GLItemMan::GetInstance().GetItem( pitem_mix->sResultItem.sNID );
	if ( !pitem_data_result )
	{
		netmsgfb.emFB = RAN_PRODUCT_FB_RESULT_ITEM_INVALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_RESULT_ITEM_INVALID %d~%d", m_dwCharID, m_szName,
			pitem_mix->sResultItem.sNID.wMainID, pitem_mix->sResultItem.sNID.wSubID );
		return E_FAIL;
	}

	WORD winvensizex = pitem_data_result->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem_data_result->sBasicOp.wInvenSizeY;

	BOOL bITEM_SPACE(FALSE);
	if ( pitem_data_result->ISPILE() )
	{
		WORD wPILENUM = pitem_data_result->sDrugOp.wPileNum;
		SNATIVEID sNID = pitem_data_result->sBasicOp.sNativeID;
		WORD wREQINSRTNUM = ( pitem_mix->sResultItem.nNum * pitem_data_result->GETAPPLYNUM() );

		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, winvensizex, winvensizey );

		if ( !bITEM_SPACE )
		{
			netmsgfb.emFB = RAN_PRODUCT_FB_RESULT_ITEM_NO_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_RESULT_ITEM_NO_SPACE", m_dwCharID, m_szName );
			return E_FAIL;
		}
	}
	else
	{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );
		if ( !bITEM_SPACE )	
		{
			netmsgfb.emFB = RAN_PRODUCT_FB_RESULT_ITEM_NO_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_RESULT_ITEM_NO_SPACE", m_dwCharID, m_szName );
			return E_FAIL;
		}
	}

	//requirements check
	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{	
		SITEM* pitem_data_require = GLItemMan::GetInstance().GetItem( pitem_mix->sMeterialItem[i].sNID );
		if ( pitem_data_require )
		{
			if ( pitem_data_require->ISPILE() )
			{
				WORD wNum = pitem_mix->sMeterialItem[i].nNum;
				WORD witem_num = m_cInventory.CountTurnItem( pitem_data_require->sBasicOp.sNativeID );
				if ( witem_num < wNum )
				{
					netmsgfb.emFB = RAN_PRODUCT_FB_MATERIAL;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
					CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_MATERIAL %d~%d %d", m_dwCharID, m_szName,
					pitem_data_require->sBasicOp.sNativeID.wMainID, pitem_data_require->sBasicOp.sNativeID.wSubID, wNum );
					return E_FAIL;
				}

				BOOL bDelete = DeletePileItem( pitem_data_require->sBasicOp.sNativeID, wNum, EMITEM_ROUTE_DELETE );
				if ( !bDelete )
				{
					netmsgfb.emFB = RAN_PRODUCT_FB_FAIL_DELETE;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
					CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_FAIL_DELETE %d~%d %d", m_dwCharID, m_szName,
					pitem_data_require->sBasicOp.sNativeID.wMainID, pitem_data_require->sBasicOp.sNativeID.wSubID, wNum );
					return E_FAIL;
				}
			}
			else
			{
				//not pile just check if exist
				SINVENITEM* pinvenitem = m_cInventory.FindItem( pitem_data_require->sBasicOp.sNativeID );
				if ( !pinvenitem )
				{
					netmsgfb.emFB = RAN_PRODUCT_FB_MATERIAL;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
					CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_MATERIAL %d~%d", m_dwCharID, m_szName,
					pitem_data_require->sBasicOp.sNativeID.wMainID, pitem_data_require->sBasicOp.sNativeID.wSubID );
					return E_FAIL;
				}

				if ( pinvenitem->sItemCustom.nidDISGUISE !=SNATIVEID(false) )
				{
					netmsgfb.emFB = RAN_PRODUCT_FB_FAIL_COSTUME;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
					CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_FB_MATERIAL %d~%d  %d~%d", m_dwCharID, m_szName,
					pitem_data_require->sBasicOp.sNativeID.wMainID, pitem_data_require->sBasicOp.sNativeID.wSubID,
					pinvenitem->sItemCustom.nidDISGUISE.wMainID, pinvenitem->sItemCustom.nidDISGUISE.wSubID );
					return E_FAIL;
				}

				WORD wposX = pinvenitem->wPosX;
				WORD wposY = pinvenitem->wPosY;

				GLITEMLMT::GetInstance().ReqItemRoute( pinvenitem->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pinvenitem->sItemCustom.wTurnNum );

				BOOL bDelete = m_cInventory.DeleteItem ( wposX, wposY );
				if ( !bDelete )
				{
					netmsgfb.emFB = RAN_PRODUCT_FB_FAIL_DELETE;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
					CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] EMNPC_ITEM_EXCHANGE_FB_DELETE_FAIL %d~%d %d~%d", m_dwCharID, m_szName,
					pitem_data_require->sBasicOp.sNativeID.wMainID, pitem_data_require->sBasicOp.sNativeID.wSubID, wposX, wposY );
					return E_FAIL;
				}

				GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
				NetMsg_Inven_Delete.wPosX = wposX;
				NetMsg_Inven_Delete.wPosY = wposY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Delete);
			}
		}
	}

	//consume gold
	if ( pitem_mix->dwPrice > 0 )
	{
		CheckMoneyUpdate( m_lnMoney, pitem_mix->dwPrice, FALSE, "ITEM_COMPOUND" );
		m_bMoneyUpdate = TRUE;

		m_lnMoney -= pitem_mix->dwPrice;

		GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
		NetMsgMoney.lnMoney = m_lnMoney;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgMoney );

		if ( pitem_mix->dwPrice >=  EMMONEY_LOG )
		{
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(pitem_mix->dwPrice), EMITEM_ROUTE_DELETE );
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
		}
	}

	/* play time system, Juver, 2021/01/27 */
	if ( pitem_mix->llPlayPointUse > 0 && m_llPlayPoint >= pitem_mix->llPlayPointUse )
	{
		m_llPlayPoint -= pitem_mix->llPlayPointUse;

		GLMSG::SNETPC_UPDATE_PLAY_POINT NetMsgPlayPoint;
		NetMsgPlayPoint.llPlayPoint = m_llPlayPoint;
		NetMsgPlayPoint.bNotice = TRUE;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgPlayPoint);
	}

	if ( pitem_mix->dwContriUse > 0 && m_dwContributionPoint >= pitem_mix->dwContriUse )
	{
		m_dwContributionPoint -= pitem_mix->dwContriUse;

		GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT NetMsgContriPoint;
		NetMsgContriPoint.dwPoint = m_dwContributionPoint;
		NetMsgContriPoint.bNotice = FALSE;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgContriPoint);
	}

	m_bItemCompoundTask = TRUE;
	m_dwItemCompoundTaskID = pitem_mix->dwKey;
	m_fItemCompoundTaskTime = pitem_mix->fTime;
	m_fItemCompoundTaskTimer = 0.0f;

	netmsgfb.emFB = RAN_PRODUCT_FB_STARTED;
	netmsgfb.bItemCompoundTask = m_bItemCompoundTask;
	netmsgfb.dwItemCompoundTaskID = m_dwItemCompoundTaskID;
	netmsgfb.fItemCompoundTaskTime = m_fItemCompoundTaskTime;
	netmsgfb.fItemCompoundTaskTimer = m_fItemCompoundTaskTimer;

	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);

	return S_OK;
}

HRESULT GLChar::DoItemCompoundTask()
{
	GLMSG::SNETPC_REQ_ITEM_COMPOUND_PROCESS_FB	netmsgfb;

	const ITEM_MIX* pitem_mix = GLItemMixMan::GetInstance().GetProduct( m_dwItemCompoundTaskID );
	if ( !pitem_mix )
	{
		netmsgfb.emFB = RAN_PRODUCT_PROCESS_FB_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_PROCESS_FB_FAILED %d", m_dwCharID, m_szName,
		m_dwItemCompoundTaskID );
		return E_FAIL;
	}

	float fRate = (float)pitem_mix->dwRate;
	if ( !RANDOM_GEN( fRate ) )
	{
		netmsgfb.emFB = RAN_PRODUCT_PROCESS_FB_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_PROCESS_FB_FAILED %d %g", m_dwCharID, m_szName,
		m_dwItemCompoundTaskID, fRate );
		return E_FAIL;
	}

	SITEM* pitem_data_result = GLItemMan::GetInstance().GetItem( pitem_mix->sResultItem.sNID );
	if ( !pitem_data_result )
	{
		netmsgfb.emFB = RAN_PRODUCT_PROCESS_FB_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_PROCESS_FB_FAILED %d~%d", m_dwCharID, m_szName,
		pitem_mix->sResultItem.sNID.wMainID, pitem_mix->sResultItem.sNID.wSubID );
		return E_FAIL;
	}

	WORD winvensizex = pitem_data_result->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem_data_result->sBasicOp.wInvenSizeY;

	BOOL bITEM_SPACE(FALSE);
	if ( pitem_data_result->ISPILE() )
	{
		WORD wPILENUM = pitem_data_result->sDrugOp.wPileNum;
		SNATIVEID sNID = pitem_data_result->sBasicOp.sNativeID;
		WORD wREQINSRTNUM = ( pitem_mix->sResultItem.nNum * pitem_data_result->GETAPPLYNUM() );

		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, winvensizex, winvensizey );

		if ( !bITEM_SPACE )
		{
			netmsgfb.emFB = RAN_PRODUCT_PROCESS_FB_FULL_INVEN;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_PROCESS_FB_FULL_INVEN", m_dwCharID, m_szName );
			return E_FAIL;
		}

		SITEMCUSTOM snew_item(sNID);
		snew_item.cGenType = EMGEN_ITEMMIX;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pitem_data_result->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );
		snew_item.wTurnNum = wPILENUM;
		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		BOOL bOK = InsertPileItem ( snew_item, wREQINSRTNUM, EMITEM_ROUTE_ITEMMIX );
		if ( !bOK )	
		{
			netmsgfb.emFB = RAN_PRODUCT_PROCESS_FB_FAILED_INSERT;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_PROCESS_FB_FAILED_INSERT", m_dwCharID, m_szName );
			return E_FAIL;
		}

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
		/*12-9-14, Battle Pass - CNDev*/
		DoBattlePassTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
	}
	else
	{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );
		if ( !bITEM_SPACE )	
		{
			netmsgfb.emFB = RAN_PRODUCT_PROCESS_FB_FULL_INVEN;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_PROCESS_FB_FULL_INVEN", m_dwCharID, m_szName );
			return E_FAIL;
		}

		SITEMCUSTOM snew_item( pitem_data_result->sBasicOp.sNativeID );

		if ( pitem_mix->bUseStat )
		{
			snew_item.cDAMAGE = pitem_mix->cDAMAGE;
			snew_item.cDEFENSE = pitem_mix->cDEFENSE;
			snew_item.cRESIST_FIRE = pitem_mix->cRESIST_FIRE;
			snew_item.cRESIST_ICE = pitem_mix->cRESIST_ICE;
			snew_item.cRESIST_ELEC = pitem_mix->cRESIST_ELEC;
			snew_item.cRESIST_POISON = pitem_mix->cRESIST_POISON;
			snew_item.cRESIST_SPIRIT = pitem_mix->cRESIST_SPIRIT;
		}
		else
		{
			//use default upgrade from item data
			snew_item.cDAMAGE = (BYTE)pitem_data_result->sBasicOp.wGradeAttack;
			snew_item.cDEFENSE = (BYTE)pitem_data_result->sBasicOp.wGradeDefense;
		}

		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();
		snew_item.wTurnNum = 1;
		snew_item.cGenType = (BYTE) EMGEN_ITEMMIX;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE) GLGaeaServer::GetInstance().GetFieldSvrID(); 
		snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pitem_data_result->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		if ( pitem_mix->bGenerateRandomValue )
		{
			if( snew_item.GENERATE_RANDOM_OPT () )
			{
				GLITEMLMT::GetInstance().ReqRandomItem( snew_item );
			}

			/* set item option, Juver, 2021/09/01 */
			if ( pitem_data_check )
			{
				snew_item.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_check->sBasicOp.strGenSetOption );
			}
		}
		
		BOOL bOK = m_cInventory.InsertItem ( snew_item, wInsertPosX, wInsertPosY );
		if ( !bOK )	
		{
			netmsgfb.emFB = RAN_PRODUCT_PROCESS_FB_FAILED_INSERT;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			CDebugSet::ToFileWithTime( "_itemcompound.txt", "[%d][%s] RAN_PRODUCT_PROCESS_FB_FAILED_INSERT", m_dwCharID, m_szName );
			return E_FAIL;
		}

		GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_ITEMMIX, snew_item.wTurnNum );

		GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
		NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
		/*12-9-14, Battle Pass - CNDev*/
		DoBattlePassTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
	}

	netmsgfb.emFB = RAN_PRODUCT_PROCESS_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);

	return S_OK;
}

HRESULT GLChar::MsgVehicleChangeColor( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_VEHICLE_REQ_CHANGE_COLOR* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_CHANGE_COLOR* ) nmg;

	GLMSG::SNET_VEHICLE_REQ_CHANGE_COLOR_FB NetMsgFB;
	SNATIVEID sNativeID;

	SINVENITEM* pinvencard = m_cInventory.FindItem( ITEM_CARD_BIKECOLOR );
	if ( !pinvencard ) 
	{
		NetMsgFB.emFB = EMREQ_CHANGE_BIKE_COLOR_FB_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pitemcard = GLItemMan::GetInstance().GetItem( pinvencard->sItemCustom.sNativeID );
	if ( !pitemcard )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_BIKE_COLOR_FB_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pitemcard->sBasicOp.emItemType != ITEM_CARD_BIKECOLOR )	
	{
		NetMsgFB.emFB = EMREQ_CHANGE_BIKE_COLOR_FB_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( !m_bVehicle )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_BIKE_COLOR_FB_VEHICLE_INACTIVE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pVehicle = GET_SLOT_ITEMDATA( SLOT_VEHICLE );
	if ( !pVehicle )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_BIKE_COLOR_FB_VEHICLE_INACTIVE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pVehicle->sVehicle.emVehicleType != VEHICLE_TYPE_BIKE )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_BIKE_COLOR_FB_VEHICLE_NOT_BIKE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( !pVehicle->sVehicle.bUseColor )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_BIKE_COLOR_FB_VEHICLE_NOT_COLOR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	BOOL bDelete = DoDrugInvenItem( pinvencard->wPosX, pinvencard->wPosY );
	if ( !bDelete )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_BIKE_COLOR_FB_CHANGE_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	for( WORD i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
	{
		m_sVehicle.m_wColor[i] = pNetMsg->wColor[i];
	}

	NetMsgFB.emFB = EMREQ_CHANGE_BIKE_COLOR_FB_CHANGE_DONE;
	NetMsgFB.dwVehicleID = m_sVehicle.m_dwGUID;

	GLMSG::SNET_VEHICLE_REQ_CHANGE_COLOR_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.dwVehicleID = m_sVehicle.m_dwGUID;

	for( WORD i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
	{
		NetMsgFB.wColor[i] = m_sVehicle.m_wColor[i];
		NetMsgBrd.wColor[i] = m_sVehicle.m_wColor[i];
	}
	
	GLITEMLMT::GetInstance().ReqVehicleAction( m_sVehicle.m_dwGUID, pitemcard->sBasicOp.sNativeID, EMVEHICLE_ACTION_BIKE_CHANGE_COLOR, 0 );

	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
		
	return S_OK;

}

/*rv card, Juver, 2017/11/25 */
HRESULT GLChar::MsgRandomOptionChange ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_RANDOM_OPTION_CHANGE *pnet_msg_client = (GLMSG::SNET_INVEN_RANDOM_OPTION_CHANGE *)nmg;
	GLMSG::SNET_INVEN_RANDOM_OPTION_CHANGE_FB net_msg_fb;

	if ( !VALID_HOLD_ITEM() )	return S_FALSE;

	SITEM* pitem_data_hold = GLItemMan::GetInstance().GetItem ( m_PutOnItems[SLOT_HOLD].sNativeID );
	if ( !pitem_data_hold )		return E_FAIL;

	if ( CheckCoolTime( pitem_data_hold->sBasicOp.sNativeID ) ) return E_FAIL;

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_RANDOM_OPTION_CARD )
	{
		net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_INVALID_CARD_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SINVENITEM* pinven_item_target = m_cInventory.GetItem ( pnet_msg_client->wPosX, pnet_msg_client->wPosY );
	if ( !pinven_item_target )
	{
		net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_INVALID_TARGET_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_INVALID_TARGET_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{	
		net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_TARGET_NOT_SUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pitem_data_target->sBasicOp.bUseRVCard == FALSE )
	{	
		net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_TARGET_NOT_SUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( !pitem_data_hold->sRvCard.bUseSuit[pitem_data_target->sSuitOp.emSuit] )
	{	
		net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_SUIT_MISMATCH;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}


	if ( pitem_data_hold->sRvCard.bReplaceOpt )
	{
		if ( pinven_item_target->sItemCustom.cOptTYPE1 == (BYTE) pitem_data_hold->sRvCard.emOption 
			&& pinven_item_target->sItemCustom.nOptVALUE1 >= (short) pitem_data_hold->sRvCard.wValue )
		{
			net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
			return E_FAIL;
		}

		if ( pinven_item_target->sItemCustom.cOptTYPE2 == (BYTE) pitem_data_hold->sRvCard.emOption 
			&& pinven_item_target->sItemCustom.nOptVALUE2 >= (short) pitem_data_hold->sRvCard.wValue )
		{
			net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
			return E_FAIL;
		}

		if ( pinven_item_target->sItemCustom.cOptTYPE3 == (BYTE) pitem_data_hold->sRvCard.emOption 
			&& pinven_item_target->sItemCustom.nOptVALUE3 >= (short) pitem_data_hold->sRvCard.wValue )
		{
			net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
			return E_FAIL;
		}

		if ( pinven_item_target->sItemCustom.cOptTYPE4 == (BYTE) pitem_data_hold->sRvCard.emOption 
			&& pinven_item_target->sItemCustom.nOptVALUE4 >= (short) pitem_data_hold->sRvCard.wValue )
		{
			net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
			return E_FAIL;
		}
	}

	if ( pitem_data_hold->sRvCard.bCheckExist )
	{
		BOOL bfound_option = FALSE;

		if ( !bfound_option && pinven_item_target->sItemCustom.cOptTYPE1 == (BYTE) pitem_data_hold->sRvCard.emOption )
		{
			pinven_item_target->sItemCustom.cOptTYPE1 = (BYTE) pitem_data_hold->sRvCard.emOption;
			if ( pitem_data_hold->sRvCard.bReplaceOpt )
				pinven_item_target->sItemCustom.nOptVALUE1 = (short) pitem_data_hold->sRvCard.wValue;
			else
				pinven_item_target->sItemCustom.nOptVALUE1 += (short) pitem_data_hold->sRvCard.wValue;
			bfound_option = TRUE;
		}

		if ( !bfound_option && pinven_item_target->sItemCustom.cOptTYPE2 == (BYTE) pitem_data_hold->sRvCard.emOption )
		{
			pinven_item_target->sItemCustom.cOptTYPE2 = (BYTE) pitem_data_hold->sRvCard.emOption;
			if ( pitem_data_hold->sRvCard.bReplaceOpt )
				pinven_item_target->sItemCustom.nOptVALUE2 = (short) pitem_data_hold->sRvCard.wValue;
			else
				pinven_item_target->sItemCustom.nOptVALUE2 += (short) pitem_data_hold->sRvCard.wValue;
			bfound_option = TRUE;
		}

		if ( !bfound_option && pinven_item_target->sItemCustom.cOptTYPE3 == (BYTE) pitem_data_hold->sRvCard.emOption )
		{
			pinven_item_target->sItemCustom.cOptTYPE3 = (BYTE) pitem_data_hold->sRvCard.emOption;
			if ( pitem_data_hold->sRvCard.bReplaceOpt )
				pinven_item_target->sItemCustom.nOptVALUE3 = (short) pitem_data_hold->sRvCard.wValue;
			else
				pinven_item_target->sItemCustom.nOptVALUE3 += (short) pitem_data_hold->sRvCard.wValue;
			bfound_option = TRUE;
		}

		if ( !bfound_option && pinven_item_target->sItemCustom.cOptTYPE4 == (BYTE) pitem_data_hold->sRvCard.emOption )
		{
			pinven_item_target->sItemCustom.cOptTYPE4 = (BYTE) pitem_data_hold->sRvCard.emOption;
			if ( pitem_data_hold->sRvCard.bReplaceOpt )
				pinven_item_target->sItemCustom.nOptVALUE4 = (short) pitem_data_hold->sRvCard.wValue;
			else
				pinven_item_target->sItemCustom.nOptVALUE4 += (short) pitem_data_hold->sRvCard.wValue;
			bfound_option = TRUE;
		}

		if ( !bfound_option )
		{
			net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_NOT_EXIST;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
			return E_FAIL;
		}
		else
		{
			BOOL bErase = DoDrugSlotItem(SLOT_HOLD);
			if ( !bErase )
			{
				SINVENITEM* pinven_item_card = m_cInventory.FindItem( ITEM_RANDOM_OPTION_CARD );
				if ( pinven_item_card )
					bErase = DoDrugInvenItem( pinven_item_card->wPosX, pinven_item_card->wPosY );
			}

			if ( !bErase )
				CDebugSet::ToFileWithTime( "_random_option_change.txt", "[%d] %s cant find option card", m_dwCharID, m_szName );

			GLMSG::SNET_INVEN_ITEM_UPDATE net_msg_client_item_update;
			net_msg_client_item_update.wPosX = pnet_msg_client->wPosX;
			net_msg_client_item_update.wPosY = pnet_msg_client->wPosY;
			net_msg_client_item_update.sItemCustom = pinven_item_target->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_item_update );

			GLITEMLMT::GetInstance().ReqItemConversion ( pinven_item_target->sItemCustom, ID_CHAR, m_dwCharID );

			net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_DONE_CHANGE;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		}
	}
	else
	{
		BOOL bfound_free = FALSE;

		if ( !bfound_free && pinven_item_target->sItemCustom.cOptTYPE1 == (BYTE) pitem_data_hold->sRvCard.emOption )
		{
			pinven_item_target->sItemCustom.cOptTYPE1 = (BYTE) pitem_data_hold->sRvCard.emOption;
			if ( pitem_data_hold->sRvCard.bReplaceOpt )
				pinven_item_target->sItemCustom.nOptVALUE1 = (short) pitem_data_hold->sRvCard.wValue;
			else
				pinven_item_target->sItemCustom.nOptVALUE1 += (short) pitem_data_hold->sRvCard.wValue;
			bfound_free = TRUE;
		}

		if ( !bfound_free && pinven_item_target->sItemCustom.cOptTYPE2 == (BYTE) pitem_data_hold->sRvCard.emOption )
		{
			pinven_item_target->sItemCustom.cOptTYPE2 = (BYTE) pitem_data_hold->sRvCard.emOption;
			if ( pitem_data_hold->sRvCard.bReplaceOpt )
				pinven_item_target->sItemCustom.nOptVALUE2 = (short) pitem_data_hold->sRvCard.wValue;
			else
				pinven_item_target->sItemCustom.nOptVALUE2 += (short) pitem_data_hold->sRvCard.wValue;
			bfound_free = TRUE;
		}

		if ( !bfound_free && pinven_item_target->sItemCustom.cOptTYPE3 == (BYTE) pitem_data_hold->sRvCard.emOption )
		{
			pinven_item_target->sItemCustom.cOptTYPE3 = (BYTE) pitem_data_hold->sRvCard.emOption;
			if ( pitem_data_hold->sRvCard.bReplaceOpt )
				pinven_item_target->sItemCustom.nOptVALUE3 = (short) pitem_data_hold->sRvCard.wValue;
			else
				pinven_item_target->sItemCustom.nOptVALUE3 += (short) pitem_data_hold->sRvCard.wValue;
			bfound_free = TRUE;
		}

		if ( !bfound_free && pinven_item_target->sItemCustom.cOptTYPE4 == (BYTE) pitem_data_hold->sRvCard.emOption )
		{
			pinven_item_target->sItemCustom.cOptTYPE4 = (BYTE) pitem_data_hold->sRvCard.emOption;
			if ( pitem_data_hold->sRvCard.bReplaceOpt )
				pinven_item_target->sItemCustom.nOptVALUE4 = (short) pitem_data_hold->sRvCard.wValue;
			else
				pinven_item_target->sItemCustom.nOptVALUE4 += (short) pitem_data_hold->sRvCard.wValue;
			bfound_free = TRUE;
		}


		if ( !bfound_free && pinven_item_target->sItemCustom.cOptTYPE1 == EMR_OPT_NULL )
		{
			pinven_item_target->sItemCustom.cOptTYPE1 = (BYTE) pitem_data_hold->sRvCard.emOption;
			pinven_item_target->sItemCustom.nOptVALUE1 = (short) pitem_data_hold->sRvCard.wValue;
			bfound_free = TRUE;
		}

		if ( !bfound_free && pinven_item_target->sItemCustom.cOptTYPE2 == EMR_OPT_NULL )
		{
			pinven_item_target->sItemCustom.cOptTYPE2 = (BYTE) pitem_data_hold->sRvCard.emOption;
			pinven_item_target->sItemCustom.nOptVALUE2 = (short) pitem_data_hold->sRvCard.wValue;
			bfound_free = TRUE;
		}

		if ( !bfound_free && pinven_item_target->sItemCustom.cOptTYPE3 == EMR_OPT_NULL )
		{
			pinven_item_target->sItemCustom.cOptTYPE3 = (BYTE) pitem_data_hold->sRvCard.emOption;
			pinven_item_target->sItemCustom.nOptVALUE3 = (short) pitem_data_hold->sRvCard.wValue;
			bfound_free = TRUE;
		}

		if ( !bfound_free && pinven_item_target->sItemCustom.cOptTYPE4 == EMR_OPT_NULL )
		{
			pinven_item_target->sItemCustom.cOptTYPE4 = (BYTE) pitem_data_hold->sRvCard.emOption;
			pinven_item_target->sItemCustom.nOptVALUE4 = (short) pitem_data_hold->sRvCard.wValue;
			bfound_free = TRUE;
		}

		if ( !bfound_free )
		{
			net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_NOT_FREE;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
			return E_FAIL;
		}
		else
		{
			BOOL bErase = DoDrugSlotItem(SLOT_HOLD);
			if ( !bErase )
			{
				SINVENITEM* pinven_item_card = m_cInventory.FindItem( ITEM_RANDOM_OPTION_CARD );
				if ( pinven_item_card )
					bErase = DoDrugInvenItem( pinven_item_card->wPosX, pinven_item_card->wPosY );
			}

			if ( !bErase )
				CDebugSet::ToFileWithTime( "_random_option_change.txt", "[%d] %s cant find option card", m_dwCharID, m_szName );

			GLMSG::SNET_INVEN_ITEM_UPDATE net_msg_client_item_update;
			net_msg_client_item_update.wPosX = pnet_msg_client->wPosX;
			net_msg_client_item_update.wPosY = pnet_msg_client->wPosY;
			net_msg_client_item_update.sItemCustom = pinven_item_target->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_item_update );

			GLITEMLMT::GetInstance().ReqItemConversion ( pinven_item_target->sItemCustom, ID_CHAR, m_dwCharID );

			net_msg_fb.emFB = EM_RANDOM_OPTION_CHANGE_DONE_CHANGE;
			GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		}
	}

	return S_OK;
}

/*nondrop card, Juver, 2017/11/26 */
HRESULT GLChar::MsgNonDropCard ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_NONDROP_CARD *pnet_msg_client = (GLMSG::SNET_INVEN_NONDROP_CARD *)nmg;
	GLMSG::SNET_INVEN_NONDROP_CARD_FB net_msg_fb;

	if ( !VALID_HOLD_ITEM() )	return S_FALSE;

	SITEM* pitem_data_hold = GLItemMan::GetInstance().GetItem ( m_PutOnItems[SLOT_HOLD].sNativeID );
	if ( !pitem_data_hold )		return E_FAIL;

	if ( CheckCoolTime( pitem_data_hold->sBasicOp.sNativeID ) ) return E_FAIL;

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_NONDROP_CARD )
	{
		net_msg_fb.emFB = EM_NONDROP_CARD_INVALID_CARD_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SINVENITEM* pinven_item_target = m_cInventory.GetItem ( pnet_msg_client->wPosX, pnet_msg_client->wPosY );
	if ( !pinven_item_target )
	{
		net_msg_fb.emFB = EM_NONDROP_CARD_INVALID_TARGET_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		net_msg_fb.emFB = EM_NONDROP_CARD_INVALID_TARGET_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{	
		net_msg_fb.emFB = EM_NONDROP_CARD_TARGET_NOT_SUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pinven_item_target->sItemCustom.IsGM_GENITEM() )
	{
		net_msg_fb.emFB = EM_NONDROP_CARD_TARGET_ALREADY;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	pinven_item_target->sItemCustom.SetGM_GEN( GetUserID() );


	BOOL bErase = DoDrugSlotItem(SLOT_HOLD);
	if ( !bErase )
	{
		SINVENITEM* pinven_item_card = m_cInventory.FindItem( ITEM_NONDROP_CARD );
		if ( pinven_item_card )
			bErase = DoDrugInvenItem( pinven_item_card->wPosX, pinven_item_card->wPosY );
	}

	if ( !bErase )
		CDebugSet::ToFileWithTime( "_nondrop_card.txt", "[%d] %s cant find nondrop card", m_dwCharID, m_szName );

	GLMSG::SNET_INVEN_ITEM_UPDATE net_msg_client_item_update;
	net_msg_client_item_update.wPosX = pnet_msg_client->wPosX;
	net_msg_client_item_update.wPosY = pnet_msg_client->wPosY;
	net_msg_client_item_update.sItemCustom = pinven_item_target->sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_item_update );

	GLITEMLMT::GetInstance().ReqItemConversion ( pinven_item_target->sItemCustom, ID_CHAR, m_dwCharID );

	net_msg_fb.emFB = EM_NONDROP_CARD_DONE_CHANGE;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );


	return S_OK;
}

/*regional chat, Juver, 2017/12/06 */
HRESULT GLChar::MsgRegionalChatPay ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_REGIONAL_CHAT_PAY *pnet_msg_client = (GLMSG::SNET_REGIONAL_CHAT_PAY *)nmg;
	
	if( m_lnMoney < pnet_msg_client->dwCost )
	{
		CDebugSet::ToFileWithTime( "_regionalpay.txt", "[%d][%s] not enough money %d char money:%I64d", m_dwCharID, m_szName, pnet_msg_client->dwCost, m_lnMoney );
		return E_FAIL;
	}

	if ( pnet_msg_client->dwCost <= 0 )
	{
		CDebugSet::ToFileWithTime( "_regionalpay.txt", "[%d][%s] invalid cost %d char money:%I64d", m_dwCharID, m_szName, pnet_msg_client->dwCost, m_lnMoney );
		return E_FAIL;
	}

	CheckMoneyUpdate( m_lnMoney, pnet_msg_client->dwCost, FALSE, "Regional Chat Pay." );
	m_bMoneyUpdate = TRUE;

	m_lnMoney -= pnet_msg_client->dwCost;

	if ( pnet_msg_client->dwCost>EMMONEY_LOG )
	{
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(pnet_msg_client->dwCost), EMITEM_ROUTE_DELETE );
		GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
	}

	GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
	NetMsgMoney.lnMoney = m_lnMoney;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgMoney );

	return S_OK;
}

/*dual pet skill, Juver, 2017/12/29 */
HRESULT	GLChar::MsgEnableDualSkill ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPET_REQ_ENABLE_DUAL_SKILL* pclient_msg = ( GLMSG::SNETPET_REQ_ENABLE_DUAL_SKILL* ) nmg;
	GLMSG::SNETPET_REQ_ENABLE_DUAL_SKILL_FB net_msg_fb_client;

	SINVENITEM* pinven_item = m_cInventory.FindPosItem ( pclient_msg->wPosX, pclient_msg->wPosY );
	if ( !pinven_item ) 
	{
		net_msg_fb_client.emFB = EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_fb_client );
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pinven_item->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		net_msg_fb_client.emFB = EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_fb_client );
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PET_CARD )	
	{
		net_msg_fb_client.emFB = EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_fb_client );
		return E_FAIL;
	}

	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pitem_data_hold ) 
	{
		net_msg_fb_client.emFB = EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_fb_client );
		return E_FAIL;
	}

	if ( CheckCoolTime( pitem_data_hold->sBasicOp.sNativeID ) ) return E_FAIL;

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_CARD_DUALPETSKILL )	
	{
		net_msg_fb_client.emFB = EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_fb_client );
		return E_FAIL;
	}

	SITEM* pPutOnItem = GLItemMan::GetInstance().GetItem ( m_PutOnItems[SLOT_HOLD].sNativeID );
	if( pitem_data_hold != pPutOnItem )
	{
		net_msg_fb_client.emFB = EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_fb_client );
		return E_FAIL;
	}

	SNATIVEID sNativeID = pitem_data_hold->sBasicOp.sNativeID;

	PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
	if ( pMyPet && pMyPet->m_dwPetID == pinven_item->sItemCustom.dwPetID )
	{
		if ( pMyPet->m_bDualSkill )
		{
			net_msg_fb_client.emFB = EMPET_REQ_ENABLE_DUAL_SKILL_ALREADY_ENABLED;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_fb_client );
			return E_FAIL;
		}

		pMyPet->m_bDualSkill = TRUE;

		if ( pMyPet->IsValid () )	pMyPet->m_fTIMER = 0.0f;

		BOOL bdelete_item = DoDrugSlotItem ( SLOT_HOLD );
		if ( !bdelete_item )
		{
			SINVENITEM* pinven_item_card = m_cInventory.FindItem ( ITEM_CARD_DUALPETSKILL );
			if ( pinven_item_card )	
				bdelete_item = DoDrugInvenItem ( pinven_item_card->wPosX, pinven_item_card->wPosY );

			if ( !bdelete_item )	
				CDebugSet::ToFileWithTime( "_pet_dual_skill.txt", "[%d]%s pet dual skill delete failed", m_dwCharID, m_szName );
		}

		GLITEMLMT::GetInstance().ReqPetAction( pMyPet->m_dwPetID, 
												pinven_item->sItemCustom.sNativeID, 
												EMPET_ACTION_PET_DUAL_SKILL, 
												pMyPet->m_bDualSkill );

		net_msg_fb_client.emFB	     = EMPET_REQ_ENABLE_DUAL_SKILL_FB_OK;
		net_msg_fb_client.dwPetID    = pMyPet->m_dwPetID;
		net_msg_fb_client.sNativeID  = sNativeID;
		net_msg_fb_client.bDualSkill = pMyPet->m_bDualSkill;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_fb_client );

		CSetPetDualSkill* pDBAction = new CSetPetDualSkill( m_dwCharID, pinven_item->sItemCustom.dwPetID, true );
		GLDBMan* pDBManager = GLGaeaServer::GetInstance().GetDBMan ();
		if ( pDBManager ) pDBManager->AddJob ( pDBAction );
	}
	else
	{
		net_msg_fb_client.emFB = EMPET_REQ_ENABLE_DUAL_SKILL_NOT_ACTIVE;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_fb_client );
		return E_FAIL;
	}

	return S_OK;
}

/*change scale card, Juver, 2018/01/04 */
HRESULT GLChar::MsgChangeScale( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_SCALE_CHANGE* pNetMsg = ( GLMSG::SNETPC_INVEN_SCALE_CHANGE* ) nmg;

	GLMSG::SNETPC_INVEN_SCALE_CHANGE_FB NetMsgFB;
	SNATIVEID sNativeID;

	SINVENITEM* pinvencard = m_cInventory.FindItem( ITEM_CHANGE_SCALE_CARD );
	if ( !pinvencard ) 
	{
		NetMsgFB.emFB = EMFB_CHANGE_SCALE_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pitemcard = GLItemMan::GetInstance().GetItem( pinvencard->sItemCustom.sNativeID );
	if ( pitemcard->sBasicOp.emItemType != ITEM_CHANGE_SCALE_CARD )	
	{
		NetMsgFB.emFB = EMFB_CHANGE_SCALE_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	BOOL bDelete = DoDrugInvenItem( pinvencard->wPosX, pinvencard->wPosY );
	if ( !bDelete )
	{
		NetMsgFB.emFB = EMFB_CHANGE_SCALE_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	m_fScaleRange = pNetMsg->fScale;

	NetMsgFB.emFB	= EMFB_CHANGE_SCALE_OK;
	NetMsgFB.fScale = m_fScaleRange;

	GLMSG::SNETPC_INVENSCALE_CHANGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.fScale = m_fScaleRange;

	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	return S_OK;
}

/*item color, Juver, 2018/01/10 */
HRESULT GLChar::MsgReqInvenItemColorChange( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_ITEMCOLOR_CHANGE* pNetMsg = ( GLMSG::SNETPC_INVEN_ITEMCOLOR_CHANGE* ) nmg;

	GLMSG::SNETPC_INVEN_ITEMCOLOR_CHANGE_FB NetMsgFB;
	SNATIVEID sNativeID;

	SINVENITEM* pinvencard = m_cInventory.FindItem( ITEM_COSTUME_COLOR );
	if ( !pinvencard ) 
	{
		NetMsgFB.emFB = EMFB_ITEM_COLOR_CHANGE_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pitemcard = GLItemMan::GetInstance().GetItem( pinvencard->sItemCustom.sNativeID );
	if ( pitemcard->sBasicOp.emItemType != ITEM_COSTUME_COLOR )	
	{
		NetMsgFB.emFB = EMFB_ITEM_COLOR_CHANGE_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pNetMsg->emSlot >= SLOT_TSIZE )
	{
		NetMsgFB.emFB = EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pitem_data_slot = GET_SLOT_ITEMDATA( pNetMsg->emSlot );
	if ( !pitem_data_slot )
	{
		NetMsgFB.emFB = EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( GET_SLOT_ITEM(pNetMsg->emSlot).nidDISGUISE == NATIVEID_NULL() )
	{
		if ( !pitem_data_slot->sBasicOp.bItemColor )
		{
			NetMsgFB.emFB = EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}
	}
	else
	{
		SITEM* pitem_data_disguise = GLItemMan::GetInstance().GetItem( GET_SLOT_ITEM(pNetMsg->emSlot).nidDISGUISE );
		if ( !pitem_data_disguise )	
		{
			NetMsgFB.emFB = EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}

		if ( !pitem_data_disguise->sBasicOp.bItemColor )
		{
			NetMsgFB.emFB = EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}
	}

	BOOL bDelete = DoDrugInvenItem( pinvencard->wPosX, pinvencard->wPosY );
	if ( !bDelete )
	{
		NetMsgFB.emFB = EMFB_ITEM_COLOR_CHANGE_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	m_PutOnItems[pNetMsg->emSlot].wColor1 = pNetMsg->wColor1;
	m_PutOnItems[pNetMsg->emSlot].wColor2 = pNetMsg->wColor2;

	NetMsgFB.emFB = EMFB_ITEM_COLOR_CHANGE_DONE;
	NetMsgFB.emSlot = pNetMsg->emSlot;
	NetMsgFB.wColor1 = pNetMsg->wColor1;
	NetMsgFB.wColor2 = pNetMsg->wColor2;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	GLMSG::SNETPC_INVEN_ITEMCOLOR_CHANGE_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.emSlot = pNetMsg->emSlot;
	NetMsgBrd.wColor1 = pNetMsg->wColor1;
	NetMsgBrd.wColor2 = pNetMsg->wColor2;
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	return S_OK;
}

/*item wrapper, Juver, 2018/01/11 */
HRESULT GLChar::MsgReqInvenItemWrap( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_WRAP* pnet_msg_client = ( GLMSG::SNETPC_INVEN_WRAP* ) nmg;

	GLMSG::SNETPC_INVEN_WRAP_FB net_msg_fb;

	if ( m_sTrade.Valid() )
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SINVENITEM* pinven_item_target = m_cInventory.FindPosItem ( pnet_msg_client->wPosX, pnet_msg_client->wPosY );
	if ( !pinven_item_target )	
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_TARGET_NOT_VALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( !pinven_item_target->sItemCustom.IsGM_GENITEM() && pitem_data_target->sBasicOp.IsEXCHANGE() )
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_TARGET_NO_NEED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( !pitem_data_target->isWrappable() )
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_TARGET_NOT_VALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( pinven_item_target->sItemCustom.nidDISGUISE != NATIVEID_NULL() )
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_TARGET_HAVE_DISGUISE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pitem_data_hold )
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_HOLD_INVALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_WRAPPER )	
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_HOLD_NOT_WRAPPER;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SITEM* phold_item_box = GLItemMan::GetInstance().GetItem( pitem_data_hold->sBasicOp.sidWrapperBox );
	if ( !phold_item_box )
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_HOLD_NOT_BOX;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	BOOL bdelete_item = DoDrugSlotItem ( SLOT_HOLD );
	if ( !bdelete_item )
	{
		SINVENITEM* pinven_item_card = m_cInventory.FindItem ( ITEM_WRAPPER );
		if ( pinven_item_card )	
			bdelete_item = DoDrugInvenItem ( pinven_item_card->wPosX, pinven_item_card->wPosY );

		if ( !bdelete_item )	
			CDebugSet::ToFileWithTime( "_wrapper.txt", "[%d]%s unable to delete card", m_dwCharID, m_szName );
	}

	SITEMCUSTOM sitem_new = pinven_item_target->sItemCustom;
	sitem_new.nidDISGUISE = sitem_new.sNativeID;
	sitem_new.sNativeID = pitem_data_hold->sBasicOp.sidWrapperBox;
	
	WORD wposX = pinven_item_target->wPosX;
	WORD wposY = pinven_item_target->wPosY;
	BOOL bDelete = m_cInventory.DeleteItem ( wposX, wposY );
	if ( !bDelete )
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		CDebugSet::ToFileWithTime( "_wrapper.txt", "[%d] %s unable to delete target item %d~%d %s", m_dwCharID, m_szName,
			pitem_data_target->sBasicOp.sNativeID.wMainID, pitem_data_target->sBasicOp.sNativeID.wSubID, pitem_data_target->GetName() );
		return E_FAIL;
	}

	GLMSG::SNETPC_INVEN_DELETE net_msg_inven_delete;
	net_msg_inven_delete.wPosX = wposX;
	net_msg_inven_delete.wPosY = wposY;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_inven_delete);

	BOOL bOK = m_cInventory.InsertItem ( sitem_new, wposX, wposY );
	if ( !bOK )	
	{
		net_msg_fb.emFB = EMFB_ITEM_WRAP_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		CDebugSet::ToFileWithTime( "_wrapper.txt", "[%d] %s unable to insert new item %d~%d", m_dwCharID, m_szName,
			sitem_new.sNativeID.wMainID, sitem_new.sNativeID.wSubID );
		return E_FAIL;
	}

	GLITEMLMT::GetInstance().ReqItemRoute ( sitem_new, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, sitem_new.wTurnNum );

	GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
	NetMsgInven.Data = *m_cInventory.GetItem ( wposX, wposY );
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

	net_msg_fb.emFB = EMFB_ITEM_WRAP_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);

	return S_OK;
}

/*item wrapper, Juver, 2018/01/12 */
HRESULT GLChar::MsgReqInvenItemUnWrap( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_UNWRAP* pnet_msg_client = ( GLMSG::SNETPC_INVEN_UNWRAP* ) nmg;

	GLMSG::SNETPC_INVEN_UNWRAP_FB net_msg_fb;

	if ( m_sTrade.Valid() )
	{
		net_msg_fb.emFB = EMFB_ITEM_UNWRAP_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SINVENITEM* pinven_item_target = m_cInventory.FindPosItem ( pnet_msg_client->wPosX, pnet_msg_client->wPosY );
	if ( !pinven_item_target )	
	{
		net_msg_fb.emFB = EMFB_ITEM_UNWRAP_TARGET_NOT_VALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		net_msg_fb.emFB = EMFB_ITEM_UNWRAP_TARGET_NOT_VALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( pinven_item_target->sItemCustom.nidDISGUISE == NATIVEID_NULL() )
	{
		net_msg_fb.emFB = EMFB_ITEM_UNWRAP_TARGET_NOT_VALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_WRAPPER_BOX )
	{
		net_msg_fb.emFB = EMFB_ITEM_UNWRAP_TARGET_NOT_VALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SITEM* pitem_data_disguise = GLItemMan::GetInstance().GetItem( pinven_item_target->sItemCustom.nidDISGUISE );
	if ( !pitem_data_disguise )
	{
		net_msg_fb.emFB = EMFB_ITEM_UNWRAP_TARGET_NOT_VALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SITEMCUSTOM sitem_new = pinven_item_target->sItemCustom;
	sitem_new.sNativeID = sitem_new.nidDISGUISE;
	sitem_new.nidDISGUISE = NATIVEID_NULL();

	WORD wposX = pinven_item_target->wPosX;
	WORD wposY = pinven_item_target->wPosY;
	BOOL bDelete = m_cInventory.DeleteItem ( wposX, wposY );
	if ( !bDelete )
	{
		net_msg_fb.emFB = EMFB_ITEM_UNWRAP_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		CDebugSet::ToFileWithTime( "_wrapperbox.txt", "[%d] %s unable to delete target item %d~%d %s", m_dwCharID, m_szName,
			pitem_data_target->sBasicOp.sNativeID.wMainID, pitem_data_target->sBasicOp.sNativeID.wSubID, pitem_data_target->GetName() );
		return E_FAIL;
	}

	GLMSG::SNETPC_INVEN_DELETE net_msg_inven_delete;
	net_msg_inven_delete.wPosX = wposX;
	net_msg_inven_delete.wPosY = wposY;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_inven_delete);

	BOOL bOK = m_cInventory.InsertItem ( sitem_new, wposX, wposY );
	if ( !bOK )	
	{
		net_msg_fb.emFB = EMFB_ITEM_UNWRAP_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		CDebugSet::ToFileWithTime( "_wrapperbox.txt", "[%d] %s unable to insert new item %d~%d", m_dwCharID, m_szName,
			sitem_new.sNativeID.wMainID, sitem_new.sNativeID.wSubID );
		return E_FAIL;
	}

	GLITEMLMT::GetInstance().ReqItemRoute ( sitem_new, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, sitem_new.wTurnNum );

	GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
	NetMsgInven.Data = *m_cInventory.GetItem ( wposX, wposY );
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

	net_msg_fb.emFB = EMFB_ITEM_UNWRAP_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);

	return S_OK;
}

/*change school card, Juver, 2018/01/12 */
HRESULT GLChar::MsgReqInvenChangeSchool( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_CHANGE_SCHOOL* pnet_msg_client = ( GLMSG::SNETPC_INVEN_CHANGE_SCHOOL* ) nmg;
	GLMSG::SNETPC_INVEN_CHANGE_SCHOOL_FB net_msg_fb;

	if ( pnet_msg_client->wSchool >= GLSCHOOL_NUM )
	{
		net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_NOT_SELECTED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( pnet_msg_client->wSchool == m_wSchool )
	{
		net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_SAME_SCHOOL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_CHANGE_SCHOOL );
	if ( !pInvenItem )
	{
		net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_CHANGE_SCHOOL )	
	{
		net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( RPARAM::bChangeSchoolReqCleanQuest && m_cQuestPlay.GetQuestProc().size() )
	{
		net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_REQ_CLEAN_QUEST;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( RPARAM::bChangeSchoolReqCleanGuild && m_dwGuild!=CLUB_NULL )
	{
		net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_REQ_CLEAN_GUILD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( m_emClass == GLCC_EXTREME_M || m_emClass == GLCC_EXTREME_W )
	{
		if ( RPARAM::bChangeSchoolReqMaxLevel && m_wLevel < GLCONST_CHAR::wMAX_EXTREME_LEVEL )
		{
			net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_REQ_MAX_LEVEL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			return E_FAIL;
		}
	}
	else
	{
		if ( RPARAM::bChangeSchoolReqMaxLevel && m_wLevel < GLCONST_CHAR::wMAX_LEVEL )
		{
			net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_REQ_MAX_LEVEL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			return E_FAIL;
		}
	}

	BOOL bDelete = DoDrugInvenItem( pInvenItem->wPosX, pInvenItem->wPosY );
	if ( !bDelete )
	{
		net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	m_wSchool = pnet_msg_client->wSchool;

	if ( RPARAM::bChangeSchoolWipeQuest )
	{
		m_cQuestPlay.DeleteAll();
	}

	net_msg_fb.emFB = EMFB_CHANGE_SCHOOL_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
	return E_FAIL;

	return S_OK;
}

/*item transfer card, Juver, 2018/01/18 */
HRESULT GLChar::MsgReqInvenItemTransfer( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_TRANSFER_STATS* pnet_msg_client = ( GLMSG::SNETPC_INVEN_TRANSFER_STATS* ) nmg;
	GLMSG::SNETPC_INVEN_TRANSFER_STATS_FB net_msg_fb;

	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_TRANSFER_CARD );
	if ( !pInvenItem )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_TRANSFER_CARD )	
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	SINVENITEM* pinven_item_old =  m_cInventory.GetItem ( pnet_msg_client->sInvenPosOLD.wPosX,pnet_msg_client->sInvenPosOLD.wPosY );
	SINVENITEM* pinven_item_new =  m_cInventory.GetItem ( pnet_msg_client->sInvenPosNEW.wPosX,pnet_msg_client->sInvenPosNEW.wPosY );
	if ( !pinven_item_old || !pinven_item_new )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	SITEM* pitem_data_old = GLItemMan::GetInstance().GetItem( pinven_item_old->sItemCustom.sNativeID );
	SITEM* pitem_data_new = GLItemMan::GetInstance().GetItem( pinven_item_new->sItemCustom.sNativeID );
	if ( !pitem_data_old || !pitem_data_new )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	if ( pitem_data_old->sBasicOp.emItemType != ITEM_SUIT || pitem_data_new->sBasicOp.emItemType != ITEM_SUIT )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_INVALID_ITEM_TYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	if ( pitem_data_old->IsTIMELMT() || pitem_data_new->IsTIMELMT() )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_TIME_LIMIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	if ( pitem_data_old->sBasicOp.IsDISGUISE() || pitem_data_new->sBasicOp.IsDISGUISE() )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_DISGUISE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	if ( !pitem_data_old->sBasicOp.bItemTransfer || !pitem_data_new->sBasicOp.bItemTransfer )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_NOT_TRANSFER_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	if ( pitem_data_old->sSuitOp.emSuit != pitem_data_new->sSuitOp.emSuit )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_SUIT_MISMATCH;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	if ( pitem_data_old->sSuitOp.emSuit == SUIT_HANDHELD && pitem_data_old->sSuitOp.emAttack != pitem_data_new->sSuitOp.emAttack )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_ATTACK_MISMATCH;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	if ( RPARAM::dwItemTransferCost != 0 && m_lnMoney < RPARAM::dwItemTransferCost ) 
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_COST;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	if ( pinven_item_old->sItemCustom.nidDISGUISE != NATIVEID_NULL() || pinven_item_new->sItemCustom.nidDISGUISE != NATIVEID_NULL() )
	{
		net_msg_fb.emFB = EMFB_ITEM_TRANSFER_HAVE_DISGUISE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return S_FALSE;
	}

	BOOL bdelete_item = FALSE;
	SINVENITEM* pinven_item_card = m_cInventory.FindItem ( ITEM_TRANSFER_CARD );
	if ( pinven_item_card )	
		bdelete_item = DoDrugInvenItem ( pinven_item_card->wPosX, pinven_item_card->wPosY );

	if ( !bdelete_item )
	{
		SITEMCUSTOM shold_item = GET_SLOT_ITEM( SLOT_HOLD );
		SITEM* pitem_data_hold = GLItemMan::GetInstance().GetItem( shold_item.sNativeID );
		if ( pitem_data_hold && pitem_data_hold->sBasicOp.emItemType == ITEM_TRANSFER_CARD ) 
		{
			bdelete_item = DoDrugSlotItem ( SLOT_HOLD );
		}
	}

	if ( !bdelete_item )	
		CDebugSet::ToFileWithTime( "_item_transfer_card.txt", "[%d]%s card item delete failed", m_dwCharID, m_szName );

	if ( RPARAM::dwItemTransferCost != 0 )
	{
		CheckMoneyUpdate( m_lnMoney, RPARAM::dwItemTransferCost , FALSE, "Item Transfer Cost." );
		m_bMoneyUpdate = TRUE;

		m_lnMoney -= RPARAM::dwItemTransferCost;

		if ( RPARAM::dwItemTransferCost>EMMONEY_LOG )
		{
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(RPARAM::dwItemTransferCost), EMITEM_ROUTE_DELETE );
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
		}

		GLMSG::SNETPC_UPDATE_MONEY net_msg_money;
		net_msg_money.lnMoney = m_lnMoney;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_money);
	}
	

	SITEMCUSTOM sitem_new = pinven_item_old->sItemCustom;
	sitem_new.sNativeID = pinven_item_new->sItemCustom.sNativeID;


	{
		WORD wposX = pinven_item_old->wPosX;
		WORD wposY = pinven_item_old->wPosY;
		GLITEMLMT::GetInstance().ReqItemRoute ( pinven_item_old->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pinven_item_old->sItemCustom.wTurnNum );
		BOOL bDelete = m_cInventory.DeleteItem ( wposX, wposY );
		if ( !bDelete )
		{
			net_msg_fb.emFB = EMFB_ITEM_TRANSFER_FAILED;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			CDebugSet::ToFileWithTime( "_item_transfer_card.txt", "[%d] %s unable to delete old item %d~%d %s", m_dwCharID, m_szName,
				pitem_data_old->sBasicOp.sNativeID.wMainID, pitem_data_old->sBasicOp.sNativeID.wSubID, pitem_data_old->GetName() );
			return E_FAIL;
		}

		GLMSG::SNETPC_INVEN_DELETE net_msg_inven_delete;
		net_msg_inven_delete.wPosX = wposX;
		net_msg_inven_delete.wPosY = wposY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_inven_delete);
	}
	{
		WORD wposX = pinven_item_new->wPosX;
		WORD wposY = pinven_item_new->wPosY;
		GLITEMLMT::GetInstance().ReqItemRoute ( pinven_item_new->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pinven_item_new->sItemCustom.wTurnNum );
		BOOL bDelete = m_cInventory.DeleteItem ( wposX, wposY );
		if ( !bDelete )
		{
			net_msg_fb.emFB = EMFB_ITEM_TRANSFER_FAILED;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			CDebugSet::ToFileWithTime( "_item_transfer_card.txt", "[%d] %s unable to delete new item %d~%d %s", m_dwCharID, m_szName,
				pitem_data_new->sBasicOp.sNativeID.wMainID, pitem_data_new->sBasicOp.sNativeID.wSubID, pitem_data_new->GetName() );
			return E_FAIL;
		}

		GLMSG::SNETPC_INVEN_DELETE net_msg_inven_delete;
		net_msg_inven_delete.wPosX = wposX;
		net_msg_inven_delete.wPosY = wposY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_inven_delete);
	}

	{
		WORD wposX(0);
		WORD wposY(0);
		BOOL bFindSpace = m_cInventory.FindInsrtable ( pitem_data_new->sBasicOp.wInvenSizeX, pitem_data_new->sBasicOp.wInvenSizeY, wposX, wposY );
		if ( !bFindSpace )
		{
			net_msg_fb.emFB = EMFB_ITEM_TRANSFER_FAILED;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			CDebugSet::ToFileWithTime( "_item_transfer_card.txt", "[%d] %s unable to find space for new item %d~%d", m_dwCharID, m_szName,
				sitem_new.sNativeID.wMainID, sitem_new.sNativeID.wSubID );
			return E_FAIL;
		}
		
		BOOL bInsertNewItem = m_cInventory.InsertItem ( sitem_new, wposX, wposY );
		if ( !bInsertNewItem )	
		{
			net_msg_fb.emFB = EMFB_ITEM_TRANSFER_FAILED;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			CDebugSet::ToFileWithTime( "_item_transfer_card.txt", "[%d] %s unable to insert new item %d~%d", m_dwCharID, m_szName,
				sitem_new.sNativeID.wMainID, sitem_new.sNativeID.wSubID );
			return E_FAIL;
		}

		GLITEMLMT::GetInstance().ReqItemRoute ( sitem_new, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, sitem_new.wTurnNum );

		GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
		NetMsgInven.Data = *m_cInventory.GetItem ( wposX, wposY );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);
	}

	CDebugSet::ToFileWithTime( "_item_transfer_card.txt", "[%d] %s transfer old item [%d~%d %s] new item [%d~%d %s]", m_dwCharID, m_szName,
		pitem_data_old->sBasicOp.sNativeID.wMainID, pitem_data_old->sBasicOp.sNativeID.wSubID, pitem_data_old->GetName(), 
		pitem_data_new->sBasicOp.sNativeID.wMainID, pitem_data_new->sBasicOp.sNativeID.wSubID, pitem_data_new->GetName() );

	net_msg_fb.emFB = EMFB_ITEM_TRANSFER_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);

	return S_OK;
}

/* car, cart color, Juver, 2018/02/14 */
HRESULT GLChar::MsgCarChangeColor( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_VEHICLE_REQ_CHANGE_CAR_COLOR* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_CHANGE_CAR_COLOR* ) nmg;

	GLMSG::SNET_VEHICLE_REQ_CHANGE_CAR_COLOR_FB NetMsgFB;
	SNATIVEID sNativeID;

	SINVENITEM* pinvencard = m_cInventory.FindItem( ITEM_CARD_CAR_COLOR );
	if ( !pinvencard ) 
	{
		NetMsgFB.emFB = EMREQ_CHANGE_CAR_COLOR_FB_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pitemcard = GLItemMan::GetInstance().GetItem( pinvencard->sItemCustom.sNativeID );
	if ( !pitemcard )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_CAR_COLOR_FB_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pitemcard->sBasicOp.emItemType != ITEM_CARD_CAR_COLOR )	
	{
		NetMsgFB.emFB = EMREQ_CHANGE_CAR_COLOR_FB_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( !m_bVehicle )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_INACTIVE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pVehicle = GET_SLOT_ITEMDATA( SLOT_VEHICLE );
	if ( !pVehicle )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_INACTIVE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pVehicle->sVehicle.emVehicleType != VEHICLE_TYPE_CAR && pVehicle->sVehicle.emVehicleType != VEHICLE_TYPE_CART )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_NOT_CAR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( !pVehicle->sVehicle.bUseColor )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_NOT_COLOR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	BOOL bDelete = DoDrugInvenItem( pinvencard->wPosX, pinvencard->wPosY );
	if ( !bDelete )
	{
		NetMsgFB.emFB = EMREQ_CHANGE_CAR_COLOR_FB_CHANGE_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A1] = pNetMsg->wColorA;
	m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A2] = pNetMsg->wColorB;

	GLITEMLMT::GetInstance().ReqVehicleAction( m_sVehicle.m_dwGUID, pitemcard->sBasicOp.sNativeID, EMVEHICLE_ACTION_CAR_CART_CHANGE_COLOR, 0 );

	NetMsgFB.emFB = EMREQ_CHANGE_CAR_COLOR_FB_CHANGE_DONE;
	NetMsgFB.dwVehicleID = m_sVehicle.m_dwGUID;
	NetMsgFB.wColorA = m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A1];
	NetMsgFB.wColorB = m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A2];
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	GLMSG::SNET_VEHICLE_REQ_CHANGE_CAR_COLOR_BRD NetMsgBrd;
	NetMsgBrd.dwGaeaID = m_dwGaeaID;
	NetMsgBrd.dwVehicleID = m_sVehicle.m_dwGUID;
	NetMsgBrd.wColorA = m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A1];
	NetMsgBrd.wColorB = m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A2];
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );

	return S_OK;
}

/* booster all vehicle, Juver, 2018/02/14 */
HRESULT GLChar::MsgAllVehicleEnableBooster( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER* pNetMsg = ( GLMSG::SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER* ) nmg;

	GLMSG::SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER_FB NetMsgFB;
	SNATIVEID sNativeID;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem ) 
	{
		NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_VEHICLE )	
	{
		NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold ) 
	{
		NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pHold->sBasicOp.emItemType != ITEM_CARD_VEHICLE_BOOST )	
	{
		NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pPutOnItem = GLItemMan::GetInstance().GetItem ( m_PutOnItems[SLOT_HOLD].sNativeID );
	if( pHold != pPutOnItem )
	{
		NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pItem->sVehicle.emVehicleType == VEHICLE_TYPE_BOARD )	
	{
		NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_TYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	DWORD dwVehicleID = pInvenItem->sItemCustom.dwVehicleID;
	if (  dwVehicleID == 0 )
	{
		NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( m_sVehicle.m_dwGUID == dwVehicleID && m_sVehicle.IsBooster() )	
	{
		NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_ALREADY_ENABLED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	CSetVehicleBooster* pDbAction = new CSetVehicleBooster( m_dwClientID, m_dwCharID, dwVehicleID, TRUE );
	GLDBMan* pDbMan = GLGaeaServer::GetInstance().GetDBMan ();
	if ( pDbMan ) pDbMan->AddJob ( pDbAction );

	DoDrugSlotItem ( SLOT_HOLD );

	if ( m_sVehicle.m_dwGUID == dwVehicleID )	
	{
		m_sVehicle.SetBooster( TRUE );
	}

	GLITEMLMT::GetInstance().ReqVehicleAction( dwVehicleID, pHold->sBasicOp.sNativeID, EMVEHICLE_ACTION_BOOSTER_ENABLE, m_sVehicle.m_bBooster );


	NetMsgFB.emFB = EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_OK;
	NetMsgFB.dwVehicleID = dwVehicleID;
	NetMsgFB.sCardID = pHold->sBasicOp.sNativeID;
	NetMsgFB.sItemID = pItem->sBasicOp.sNativeID;
	GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgFB );

	return S_OK;

}
/* qitem inven, 2024/01/07 */
HRESULT GLChar::MsgReqUseInvenQitem ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )												return E_FAIL;
	if ( m_pLandMan && m_pLandMan->IsPeaceZone() )						return E_FAIL;

	/*qbox check, Juver, 2017/12/05 */
	if ( !m_pLandMan->IsQBoxEnabled() )
	{
		CDebugSet::ToFileWithTime( "_qbox_check.txt", "[%d]%s attempt to use qbox in non qbox map", m_dwCharID, m_szName );
		return S_FALSE;
	}

	GLMSG::SNETPC_REQ_USE_INVENQITEM *pNetMsg = (GLMSG::SNETPC_REQ_USE_INVENQITEM *) nmg;

	SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pInvenItem )	return E_FAIL;

	if ( CheckCoolTime( pInvenItem->sItemCustom.sNativeID ) )	return S_FALSE;

	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItemData || pItemData->sBasicOp.emItemType!=ITEM_QITEM )	return E_FAIL;

	SNATIVEID sMapID = m_sMapID;
	if( m_pLandMan->IsInstantMap() )	sMapID.wSubID = 0;
	bool bFilterDrop = CMapDropFilter::GetInstance().Find( sMapID, pItemData->sBasicOp.sNativeID );
	if (bFilterDrop)
		return FALSE;

	if( m_pLandMan->GetMapID() != pItemData->sBasicOp.sSubID && 
		pItemData->sBasicOp.sSubID != SNATIVEID(false) )	return FALSE;

	const ITEM::SQUESTIONITEM &sQUESTIONITEM = pItemData->sQuestionItem;
	switch ( sQUESTIONITEM.emType )
	{
	case QUESTION_SPEED_UP:
	case QUESTION_CRAZY:
	case QUESTION_ATTACK_UP:
	case QUESTION_EXP_UP:
	case QUESTION_LUCKY:
	case QUESTION_SPEED_UP_M:
	case QUESTION_MADNESS:
	case QUESTION_ATTACK_UP_M:
		{
			SNATIVEID sNativeID = pItemData->sBasicOp.sNativeID;
			GLPARTY_FIELD* pParty = m_pGLGaeaServer->GetParty(m_dwPartyID);

			if ( pParty )
			{
				GLPARTY_FIELD::MEMBER_ITER pos = pParty->m_cMEMBER.begin();
				GLPARTY_FIELD::MEMBER_ITER end = pParty->m_cMEMBER.end();
				for ( ; pos!=end; ++pos )
				{
					PGLCHAR pCHAR = m_pGLGaeaServer->GetChar ( (*pos).first );
					if ( !pCHAR )	continue;
					if ( pCHAR->m_sMapID!=m_sMapID )	continue;

					D3DXVECTOR3 vDist = pCHAR->m_vPos - m_vPos;
					float fDist = D3DXVec3Length ( &vDist );
					if ( MAX_VIEWRANGE < fDist )		continue;
					pCHAR->RecieveQItemFact ( sNativeID,FALSE );
				}
			}
			else
			{
				RecieveQItemFact ( sNativeID,FALSE );
			}
		}
		break;
	};

	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY, true );

	return S_OK;
}
/*item random option rebuild, Juver, 2018/07/04 */
HRESULT GLChar::MsgReqInvenItemRandomOptionRebuild( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_ITEM_RANDOM_OPTION_REBUILD* pnet_msg_client = ( GLMSG::SNETPC_INVEN_ITEM_RANDOM_OPTION_REBUILD* ) nmg;

	GLMSG::SNETPC_INVEN_ITEM_RANDOM_OPTION_REBUILD_FB net_msg_fb;


	SINVENITEM* pinven_item_card = m_cInventory.FindItem( ITEM_RANDON_OPTION_REBUILD );
	if ( !pinven_item_card )
	{
		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SITEM* pitem_card = GLItemMan::GetInstance().GetItem ( pinven_item_card->sItemCustom.sNativeID );
	if ( !pitem_card ) 
	{
		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( pitem_card->sBasicOp.emItemType != ITEM_RANDON_OPTION_REBUILD )	
	{
		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( RPARAM::item_random_option_rebuild_use_delay )
	{
		if ( m_item_random_option_rebuild_timer < RPARAM::item_random_option_rebuild_delay_time )
		{
			net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_DELAY_TIME;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			return E_FAIL;
		}
	}

	if ( m_item_random_option_rebuild_active_task )
	{
		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_TASK_ACTIVE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	m_item_random_option_rebuild_active_task = TRUE;

	SINVENITEM* pinven_item_target = m_cInventory.GetItem( pnet_msg_client->sInvenPosTarget.wPosX, pnet_msg_client->sInvenPosTarget.wPosY );
	if ( !pinven_item_target )
	{
		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_TARGET;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_TARGET;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{
		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_TARGET_NOT_SUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	/*use rebuild flag, Juver, 2019/02/18 */
	if( strlen( pitem_data_target->sRandomOpt.szNAME ) <= 3 || !pitem_data_target->sBasicOp.IsUseRebuild() )
	{
		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_TARGET_NOT_RANDOM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}

	/* Item Reform use limit, Juver, 2021/07/23 */
	if ( pitem_data_target->sBasicOp.wMaxItemReform != ITEM_REFORM_INFINITE )
	{
		if ( pinven_item_target->sItemCustom.wReformUseCount >= pitem_data_target->sBasicOp.wMaxItemReform )
		{
			net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_TARGET_MAXED;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			return E_FAIL;
		}
	}
	
	SRANDOM_OPTION_GEN* pRANDOM_SET = GLItemMan::GetInstance().GetItemRandomOpt( pinven_item_target->sItemCustom.sNativeID );
	if( !pRANDOM_SET )
	{
		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_TARGET_NOT_RANDOM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return E_FAIL;
	}


	BOOL bdelete_item = DoDrugInvenItem ( pinven_item_card->wPosX, pinven_item_card->wPosY );
	if ( !bdelete_item )
	{
		SITEMCUSTOM shold_item = GET_SLOT_ITEM( SLOT_HOLD );
		SITEM* pitem_data_hold = GLItemMan::GetInstance().GetItem( shold_item.sNativeID );
		if ( pitem_data_hold && pitem_data_hold->sBasicOp.emItemType == ITEM_RANDON_OPTION_REBUILD ) 
		{
			bdelete_item = DoDrugSlotItem ( SLOT_HOLD );
		}
	}

	if ( !bdelete_item )	
		CDebugSet::ToFileWithTime( "_item_random_option_rebuild.txt", "[%d]%s card item delete failed", m_dwCharID, m_szName );

	BOOL bItemRandomOptionRebuildOptionLock0 = pnet_msg_client->bOptionLock0;
	BOOL bItemRandomOptionRebuildOptionLock1 = pnet_msg_client->bOptionLock1;
	BOOL bItemRandomOptionRebuildOptionLock2 = pnet_msg_client->bOptionLock2;
	BOOL bItemRandomOptionRebuildOptionLock3 = pnet_msg_client->bOptionLock3;

	WORD locknum(0);
	if ( bItemRandomOptionRebuildOptionLock0 )	locknum++;
	if ( bItemRandomOptionRebuildOptionLock1 )	locknum++;
	if ( bItemRandomOptionRebuildOptionLock2 )	locknum++;
	if ( bItemRandomOptionRebuildOptionLock3 )	locknum++;

	BOOL use_stamp(FALSE);
	if ( pnet_msg_client->sInvenPosStamp.VALID() )
	{
		SINVENITEM* pinven_item_stamp = m_cInventory.GetItem( pnet_msg_client->sInvenPosStamp.wPosX, pnet_msg_client->sInvenPosStamp.wPosY );
		if ( pinven_item_stamp )
		{
			SITEM* pitem_stamp = GLItemMan::GetInstance().GetItem ( pinven_item_stamp->sItemCustom.sNativeID );
			if ( pitem_stamp && pitem_stamp->sBasicOp.emItemType == ITEM_SEALED_CARD ) 
			{
				use_stamp = TRUE;

				if ( locknum > pitem_stamp->sSuitOp.wReModelNum )
				{
					use_stamp = FALSE;

					net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_SELECT_ERROR;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
					return E_FAIL;
				}
				
				if ( locknum == 0 )
				{
					use_stamp = FALSE;
				}
			}
		}
	}

	if ( use_stamp )
	{
		BOOL bdelete_item = DoDrugInvenItem ( pnet_msg_client->sInvenPosStamp.wPosX, pnet_msg_client->sInvenPosStamp.wPosY );
		if ( !bdelete_item )
		{
			SITEMCUSTOM shold_item = GET_SLOT_ITEM( SLOT_HOLD );
			SITEM* pitem_data_hold = GLItemMan::GetInstance().GetItem( shold_item.sNativeID );
			if ( pitem_data_hold && pitem_data_hold->sBasicOp.emItemType == ITEM_SEALED_CARD ) 
			{
				bdelete_item = DoDrugSlotItem ( SLOT_HOLD );
			}
		}

		if ( !bdelete_item )	
			CDebugSet::ToFileWithTime( "_item_random_option_rebuild.txt", "[%d]%s stamp item delete failed", m_dwCharID, m_szName );
	}
	else
	{
		bItemRandomOptionRebuildOptionLock0 = FALSE;
		bItemRandomOptionRebuildOptionLock1 = FALSE;
		bItemRandomOptionRebuildOptionLock2 = FALSE;
		bItemRandomOptionRebuildOptionLock3 = FALSE;
	}

	float fDestroyRate = seqrandom::getpercent();
	if( fDestroyRate < pRANDOM_SET->fD_point )
	{
		GLITEMLMT::GetInstance().ReqItemRoute( pinven_item_target->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, m_dwCharID, EMITEM_ROUTE_DELETE, pinven_item_target->sItemCustom.wTurnNum );

		m_cInventory.DeleteItem( pnet_msg_client->sInvenPosTarget.wPosX, pnet_msg_client->sInvenPosTarget.wPosY );

		GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
		NetMsg_Inven_Delete.wPosX = pnet_msg_client->sInvenPosTarget.wPosX;
		NetMsg_Inven_Delete.wPosY = pnet_msg_client->sInvenPosTarget.wPosY;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsg_Inven_Delete );

		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_TARGET_DESTROYED;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
	}
	else
	{
		pinven_item_target->sItemCustom.GENERATE_RANDOM_OPT( false,
			bItemRandomOptionRebuildOptionLock0, bItemRandomOptionRebuildOptionLock1, bItemRandomOptionRebuildOptionLock2, bItemRandomOptionRebuildOptionLock3 );

		GLITEMLMT::GetInstance().ReqRandomItem( pinven_item_target->sItemCustom );

		/* Item Reform use limit, Juver, 2021/07/23 */
		pinven_item_target->sItemCustom.wReformUseCount ++;

		GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
		NetItemUpdate.wPosX = pnet_msg_client->sInvenPosTarget.wPosX;
		NetItemUpdate.wPosY = pnet_msg_client->sInvenPosTarget.wPosY;
		NetItemUpdate.sItemCustom = pinven_item_target->sItemCustom;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetItemUpdate );

		net_msg_fb.emFB = EMFB_ITEM_RANDOM_OPTION_REBUILD_SUCCESS;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
	}

	m_item_random_option_rebuild_timer = 0.0f;
	m_item_random_option_rebuild_active_task = FALSE;

	return S_OK;
}

/*inventory sort, Juver, 2018/07/07 */
HRESULT GLChar::MsgReqInvenItemSort ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	if ( m_fInventorySortTimer < REQ_INVENTORY_SORT_DELAY )	return E_FAIL;

	std::vector<SITEMCUSTOM>	vecPILE;
	std::vector<SNATIVEID>	vecPILEDEL;
	std::vector<SITEMCUSTOM>	vecDEL;
	std::vector<SNATIVEID>	vecDEL2;
	GLInventory::CELL_MAP &ItemMap = *m_cInventory.GetItemList();
	GLInventory::CELL_MAP_ITER iter = ItemMap.begin();
	for ( ; iter!=ItemMap.end(); ++iter )
	{
		if (!(*iter).second ) continue;
		SINVENITEM &sINVENITEM = *(*iter).second;
		SITEMCUSTOM &sITEMCUSTOM = sINVENITEM.sItemCustom;
		SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( sITEMCUSTOM.sNativeID );
		if ( !pHoldData ) continue;
		if( pHoldData && pHoldData->ISPILE() )  
		{
			vecPILE.push_back ( sITEMCUSTOM ); 
			vecPILEDEL.push_back ( SNATIVEID(sINVENITEM.wPosX,sINVENITEM.wPosY) );
		}
		else
		{
			vecDEL.push_back ( sITEMCUSTOM );
			vecDEL2.push_back ( SNATIVEID(sINVENITEM.wPosX,sINVENITEM.wPosY) );
		}
		
	}

	if ( !vecDEL2.empty() )
	for ( DWORD i=0; i<vecDEL2.size(); ++i )
	{
		m_cInventory.DeleteItem ( vecDEL2[i].wMainID, vecDEL2[i].wSubID );
		GLMSG::SNETPC_INVEN_DELETE NetMsgDelete;
		NetMsgDelete.wPosX = vecDEL2[i].wMainID;
		NetMsgDelete.wPosY = vecDEL2[i].wSubID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgDelete);
	}

	if ( !vecPILEDEL.empty() )
	for ( DWORD i=0; i<vecPILEDEL.size(); ++i )
	{
		m_cInventory.DeleteItem ( vecPILEDEL[i].wMainID, vecPILEDEL[i].wSubID );
		GLMSG::SNETPC_INVEN_DELETE NetMsgDelete;
		NetMsgDelete.wPosX = vecPILEDEL[i].wMainID;
		NetMsgDelete.wPosY = vecPILEDEL[i].wSubID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgDelete);
	}

	if ( !vecDEL.empty() )
	{
		for ( DWORD b=0; b<vecDEL.size(); ++b )
		for ( DWORD c=0; c<vecDEL.size()-1; ++c )
		{
			SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( vecDEL[c].sNativeID );
			if ( !pHoldData )
			{
				//MessageBox(NULL,"if ( !pHoldData )","vecDEL",NULL);
				continue;
			}
			SITEM* pHoldData2 = GLItemMan::GetInstance().GetItem ( vecDEL[c+1].sNativeID );
			if ( !pHoldData2 )
			{
			//	MessageBox(NULL,"if ( !pHoldData2 )","vecDEL",NULL);
				continue;
			}
			BOOL bCOMPARE = FALSE;
			if ( pHoldData->sBasicOp.emItemType > pHoldData2->sBasicOp.emItemType ) bCOMPARE = TRUE;
			else if ( pHoldData->sBasicOp.emItemType == pHoldData2->sBasicOp.emItemType )
			{
				if ( pHoldData->sBasicOp.emItemType == ITEM_SUIT )
				{
					if ( pHoldData->sSuitOp.emSuit == pHoldData2->sSuitOp.emSuit )
					{
						if ( pHoldData->sSuitOp.emSuit == SUIT_HANDHELD )
						{
							if ( pHoldData->sSuitOp.emAttack > pHoldData2->sSuitOp.emAttack ) bCOMPARE = TRUE;
						}
						else bCOMPARE = TRUE;
					}
					else if ( pHoldData->sSuitOp.emSuit > pHoldData2->sSuitOp.emSuit ) bCOMPARE = TRUE;
				}
				else bCOMPARE = TRUE;
			}

			if ( bCOMPARE ) 
			{
				SITEMCUSTOM sCUSTOM = vecDEL[c+1];
				vecDEL[c+1] = vecDEL[c];
				vecDEL[c] = sCUSTOM;
			}
		}

		WORD _X = 0, _Y = 0;
		for( int i = 0; i < vecDEL.size(); i++ )
		{		
			SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( vecDEL[i].sNativeID );
			if ( !pHoldData )
			{
				//MessageBox(NULL,"if ( !pHoldData )","",NULL);
				continue;
			}
			BOOL bOk = m_cInventory.IsInsertable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, _X, _Y );
			if ( !bOk )
			{
				bOk = m_cInventory.FindInsrtable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, _X, _Y );
				if ( !bOk )
				{
					//MessageBox(NULL,"bOk = m_cInventory.FindInsrtable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, _X, _Y );","",NULL);
					continue;
				}
			}
			bOk = m_cInventory.InsertItem ( vecDEL[i], _X, _Y );
			if ( !bOk )	
			{
				//MessageBox(NULL,"!bOk = m_cInventory.InsertItem ( vecDEL[i], _X, _Y );","",NULL);
				continue;
			}
			
					
			SINVENITEM sINVENITEM = *m_cInventory.FindPosItem ( _X, _Y );
					
			GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven_Insert;
			NetMsg_Inven_Insert.Data = sINVENITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Insert);

			SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.sNativeID );
			if ( pITEM && pITEM->IsTIMELMT() )
			{
				if ( pITEM->sDrugOp.tTIME_LMT != 0 )
				{
					const CTime cTIME_CUR = CTime::GetCurrentTime();

					CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
					CTime cTIME_LMT(sINVENITEM.sItemCustom.tBORNTIME);
					cTIME_LMT += cSPAN;

					if ( cTIME_CUR > cTIME_LMT )
					{
						if ( pITEM->sBasicOp.emItemType == ITEM_VEHICLE && sINVENITEM.sItemCustom.dwVehicleID != 0 )
						{
							GLVEHICLE* pNewVehicle = new GLVEHICLE();
							CGetVehicle* pGetVehicle = new CGetVehicle ( pNewVehicle, 
														sINVENITEM.sItemCustom.dwVehicleID, 
														m_dwClientID, 
														m_dwCharID, 
														true );
							GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
							if ( pDBMan ) pDBMan->AddJob ( pGetVehicle );
						}

						if ( pITEM->sBasicOp.emItemType == ITEM_PET_CARD && sINVENITEM.sItemCustom.dwPetID != 0 )
						{
							CDeletePet* pDbAction = new CDeletePet ( m_dwCharID, sINVENITEM.sItemCustom.dwPetID );
							GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
							if ( pDBMan ) pDBMan->AddJob ( pDbAction );

							PGLPETFIELD pMyPet = GLGaeaServer::GetInstance().GetPET ( m_dwPetGUID );
							if ( pMyPet && sINVENITEM.sItemCustom.dwPetID == pMyPet->m_dwPetID )
							{
								pMyPet->UpdateTimeLmtItem ( this );

								for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
								{
									CItemDrop cDropItem;
									cDropItem.sItemCustom = pMyPet->m_PutOnItems[i];
									if ( IsInsertToInven ( &cDropItem ) ) InsertToInven ( &cDropItem );
									else
									{
										if ( m_pLandMan )
										{
											m_pLandMan->DropItem ( m_vPos, 
																&(cDropItem.sItemCustom), 
																EMGROUP_ONE, 
																m_dwGaeaID );
										}
									}
								}

								GLGaeaServer::GetInstance().ReserveDropOutPet ( SDROPOUTPETINFO(pMyPet->m_dwGUID,true,false) );

								CGetRestorePetList *pDbAction = new CGetRestorePetList ( m_dwCharID, m_dwClientID );
								if ( pDBMan ) pDBMan->AddJob ( pDbAction );
							}
							else
							{
								GLPET* pNewPet = new GLPET ();
								CGetPet* pGetPet = new CGetPet ( pNewPet, 
																sINVENITEM.sItemCustom.dwPetID, 
																m_dwClientID, 
																m_dwCharID,
																sINVENITEM.wPosX,
																sINVENITEM.wPosY,
																true );
								GLDBMan* pDBMan = GLGaeaServer::GetInstance().GetDBMan ();
								if ( pDBMan ) pDBMan->AddJob ( pGetPet );
							}
						}

						GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
						NetMsgInvenDel.wPosX = sINVENITEM.wPosX;
						NetMsgInvenDel.wPosY = sINVENITEM.wPosY;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);

						GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
						NetMsgInvenDelTimeLmt.nidITEM = sINVENITEM.sItemCustom.sNativeID;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

						m_cInventory.DeleteItem ( sINVENITEM.wPosX, sINVENITEM.wPosY );
					
						if ( sINVENITEM.sItemCustom.nidDISGUISE!=SNATIVEID(false) )
						{
							SITEM *pONE = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.nidDISGUISE );
							if ( !pONE )		continue;

							SITEMCUSTOM sITEM_NEW;
							sITEM_NEW.sNativeID = sINVENITEM.sItemCustom.nidDISGUISE;
							CTime cTIME = CTime::GetCurrentTime();
							if ( sINVENITEM.sItemCustom.tDISGUISE!=0 && pONE->sDrugOp.tTIME_LMT!= 0 )
							{
								cTIME = CTime(sINVENITEM.sItemCustom.tDISGUISE);

								CTimeSpan tLMT(pONE->sDrugOp.tTIME_LMT);
								cTIME -= tLMT;
							}

							sITEM_NEW.tBORNTIME = cTIME.GetTime();
							sITEM_NEW.wTurnNum = 1;
							sITEM_NEW.cGenType = EMGEN_BILLING;
							sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
							sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
							sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

							BOOL bOk = m_cInventory.IsInsertable ( pONE->sBasicOp.wInvenSizeX, pONE->sBasicOp.wInvenSizeY, _X, _Y );
							if ( !bOk )			continue;

							m_cInventory.InsertItem ( sITEM_NEW, _X, _Y );
							SINVENITEM *pINSERT_ITEM = m_cInventory.GetItem ( _X, _Y );

							GLMSG::SNETPC_INVEN_INSERT NetItemInsert;
							NetItemInsert.Data = *pINSERT_ITEM;
							GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemInsert);

							GLITEMLMT::GetInstance().ReqItemRoute ( pINSERT_ITEM->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pINSERT_ITEM->sItemCustom.wTurnNum );
						}
					}
				}
			}
			
			if ( _X < 5 ) _X++;
			else
			{
				_X = 0;
				_Y++;
			}
		}
	}

	if ( !vecPILE.empty() )
	{
		for ( DWORD b=0; b<vecPILE.size(); ++b )
		for ( DWORD c=0; c<vecPILE.size()-1; ++c )
		{
			SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( vecPILE[c].sNativeID );
			if ( !pHoldData )	continue;
			
			SITEM* pHoldData2 = GLItemMan::GetInstance().GetItem ( vecPILE[c+1].sNativeID );
			if ( !pHoldData2 )	continue;
			
			BOOL bCOMPARE = FALSE;
			
			if ( pHoldData->sBasicOp.sNativeID.dwID > pHoldData2->sBasicOp.sNativeID.dwID ) bCOMPARE = TRUE;
			else if ( pHoldData->sBasicOp.sNativeID == pHoldData2->sBasicOp.sNativeID && vecPILE[c].wTurnNum != vecPILE[c+1].wTurnNum ) bCOMPARE = TRUE;
			
			if ( bCOMPARE ) 
			{
				SITEMCUSTOM sCUSTOM = vecPILE[c+1];
				vecPILE[c+1] = vecPILE[c];
				vecPILE[c] = sCUSTOM;
			}
		}

		WORD _X = 0, _Y = 0;
		for( int i = 0; i < vecPILE.size(); i++ )
		{		
			SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( vecPILE[i].sNativeID );
			if ( !pHoldData )
			{
				//MessageBox(NULL,"if ( !pHoldData )",NULL,NULL);
				continue;
			}
			BOOL bOk = m_cInventory.IsInsertable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, _X, _Y );
			if ( !bOk )
			{
				bOk = m_cInventory.FindInsrtable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, _X, _Y );
				if ( !bOk )
				{
					//MessageBox(NULL,"bOk = m_cInventory.FindInsrtable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, _X, _Y );","",NULL);
					continue;
				}
			}
			bOk = m_cInventory.InsertItem ( vecPILE[i], _X, _Y );
			if ( !bOk )	
			{
				//MessageBox(NULL,"!bOk = m_cInventory.InsertItem ( vecPILE[i], _X, _Y );","",NULL);
				continue;
			}
			
			SINVENITEM sINVENITEM = *m_cInventory.FindPosItem ( _X, _Y );
			
			GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven_Insert;
			NetMsg_Inven_Insert.Data = sINVENITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Insert);

			SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.sNativeID );
			if ( pITEM && pITEM->IsTIMELMT() )
			{
				if ( pITEM->sDrugOp.tTIME_LMT != 0 )
				{
					const CTime cTIME_CUR = CTime::GetCurrentTime();

					CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
					CTime cTIME_LMT(sINVENITEM.sItemCustom.tBORNTIME);
					cTIME_LMT += cSPAN;

					if ( cTIME_CUR > cTIME_LMT )
					{
						GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
						NetMsgInvenDel.wPosX = sINVENITEM.wPosX;
						NetMsgInvenDel.wPosY = sINVENITEM.wPosY;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);

						GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
						NetMsgInvenDelTimeLmt.nidITEM = sINVENITEM.sItemCustom.sNativeID;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

						m_cInventory.DeleteItem ( sINVENITEM.wPosX, sINVENITEM.wPosY );
					
					}
				}
			}

			if ( _X < 5 ) _X++;
			else
			{
				_X = 0;
				_Y++;
			}
		}
				GLInventory::CELL_MAP &ItemMapLoop = *m_cInventory.GetItemList();
				GLInventory::CELL_MAP_ITER iter_Loop = ItemMapLoop.begin();
				for ( ; iter_Loop!=ItemMapLoop.end(); ++iter_Loop )
				{
					if (!(*iter_Loop).second ) continue;
					GLInventory::CELL_MAP &ItemMapComp = *m_cInventory.GetItemList();
					GLInventory::CELL_MAP_ITER iter_comp = ItemMapComp.begin();
					for ( ; iter_comp!=ItemMapComp.end(); ++iter_comp )
					{
						if (!(*iter_comp).second ) continue;
						SINVENITEM* sINVENITEM = (*iter_comp).second;
						if ( sINVENITEM )
						{
						SITEMCUSTOM& sItemCustom = sINVENITEM->sItemCustom;
						SITEM *pITEMc = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
						if ( pITEMc && pITEMc->ISPILE() )
						{
							GLInventory::CELL_MAP &ItemMap = *m_cInventory.GetItemList();
							GLInventory::CELL_MAP_ITER iter = ItemMap.begin();
							for ( ; iter!=ItemMap.end(); ++iter )
							{
								if (!(*iter).second ) continue;
								SINVENITEM* pInvenItem = (*iter).second;
								if ( pInvenItem )
								{
								SITEMCUSTOM& sItemCustoms = pInvenItem->sItemCustom;
								SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );

								if ( pITEM && pITEM->ISPILE() )
								{
									if ( pITEM->sBasicOp.sNativeID == pITEMc->sBasicOp.sNativeID )
									{
										if( sINVENITEM != pInvenItem ) // compare two
										{
											WORD wINVENX = pITEMc->sBasicOp.wInvenSizeX;
											WORD wINVENY = pITEMc->sBasicOp.wInvenSizeY;

											WORD wPILENUM = pITEMc->sDrugOp.wPileNum;
											SNATIVEID sNID = pITEMc->sBasicOp.sNativeID;

											WORD wREQINSRTNUM = ( sItemCustom.wTurnNum );
											
											if ( sItemCustoms.sNativeID != sItemCustom.sNativeID )	continue;
											if ( sItemCustoms.wTurnNum>=wPILENUM )		continue;
											if ( sItemCustom.wTurnNum>=wPILENUM )		continue;
											WORD wSURPLUSNUM = wPILENUM - sItemCustoms.wTurnNum;

											if ( wREQINSRTNUM > wSURPLUSNUM )
											{
												//MessageBox(NULL,"if ( wREQINSRTNUM > wSURPLUSNUM )",NULL,NULL);
											
												sItemCustoms.wTurnNum = wPILENUM;

												GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
												NetMsg.wPosX = pInvenItem->wPosX;
												NetMsg.wPosY = pInvenItem->wPosY;
												NetMsg.wTurnNum = sItemCustoms.wTurnNum;
												GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

												wREQINSRTNUM -= wSURPLUSNUM;
											}
											else
											{
												sItemCustoms.wTurnNum += wREQINSRTNUM;

												GLMSG::SNETPC_INVEN_DRUG_UPDATE NetMsg;
												NetMsg.wPosX = pInvenItem->wPosX;
												NetMsg.wPosY = pInvenItem->wPosY;
												NetMsg.wTurnNum = sItemCustoms.wTurnNum;
												GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg);

												GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
												NetMsgMoney.lnMoney = m_lnMoney;
												GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgMoney);

												m_cInventory.DeleteItem ( sINVENITEM->wPosX, sINVENITEM->wPosY );
												GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
												NetMsgInvenDel.wPosX = sINVENITEM->wPosX;
												NetMsgInvenDel.wPosY = sINVENITEM->wPosY;
												GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);
												
												break;
											}
											
											WORD wONENUM = wREQINSRTNUM / wPILENUM;
											WORD wITEMNUM = wONENUM;

											//	여분겹침아이템의 겹침수.
											WORD wSPLITNUM = wREQINSRTNUM % wPILENUM;
											if ( wSPLITNUM > 0 )				wITEMNUM += 1;
											if ( wSPLITNUM==0 && wITEMNUM>=1 )	wSPLITNUM = wPILENUM;

											for ( WORD i=0; i<wITEMNUM; ++i )
											{
												WORD wInsertPosX(0), wInsertPosY(0);
												BOOL bITEM_SPACE = m_cInventory.FindInsrtable ( wINVENX, wINVENY, wInsertPosX, wInsertPosY );
												GASSERT(bITEM_SPACE&&"넣을 공간을 미리 체크를 하고 아이템을 넣었으나 공간이 부족함.");

												//	Note : 새로운 아이템을 넣어줌.
												//
												SITEMCUSTOM sITEMCUSTOM(sItemCustom.sNativeID);
												sITEMCUSTOM.tBORNTIME = sItemCustom.tBORNTIME;

												sITEMCUSTOM.cGenType = sItemCustom.cGenType;
												sITEMCUSTOM.cChnID = sItemCustom.cChnID;
												sITEMCUSTOM.cFieldID = sItemCustom.cFieldID;

												if ( wITEMNUM==(i+1) )	sITEMCUSTOM.wTurnNum = wSPLITNUM;	//	마지막 아이템은 잔여량.
												else					sITEMCUSTOM.wTurnNum = wPILENUM;	//	아닐 경우는 꽉찬량.

												m_cInventory.InsertItem ( sITEMCUSTOM, wInsertPosX, wInsertPosY );	//	인밴토리에 넣습니다.

												//	인밴에 아이탬 넣어주는 메시지.
												GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
												NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
												GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);
												
												m_cInventory.DeleteItem ( sINVENITEM->wPosX, sINVENITEM->wPosY );
												GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
												NetMsgInvenDel.wPosX = sINVENITEM->wPosX;
												NetMsgInvenDel.wPosY = sINVENITEM->wPosY;
												GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);
											}
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	{
		vecPILEDEL.clear();
		vecPILE.clear();
		
		GLInventory::CELL_MAP &ItemMap = *m_cInventory.GetItemList();
		GLInventory::CELL_MAP_ITER iter = ItemMap.begin();
		for ( ; iter!=ItemMap.end(); ++iter )
		{
			if (!(*iter).second ) continue;
			SINVENITEM &sINVENITEM = *(*iter).second;
			SITEMCUSTOM &sITEMCUSTOM = sINVENITEM.sItemCustom;
			SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( sITEMCUSTOM.sNativeID );
			if ( !pHoldData ) continue;
			if( pHoldData && pHoldData->ISPILE() )  
			{
				vecPILE.push_back ( sITEMCUSTOM ); 
				vecPILEDEL.push_back ( SNATIVEID(sINVENITEM.wPosX,sINVENITEM.wPosY) );
			}
		}

		if ( !vecPILEDEL.empty() )
		for ( DWORD i=0; i<vecPILEDEL.size(); ++i )
		{
			m_cInventory.DeleteItem ( vecPILEDEL[i].wMainID, vecPILEDEL[i].wSubID );
			GLMSG::SNETPC_INVEN_DELETE NetMsgDelete;
			NetMsgDelete.wPosX = vecPILEDEL[i].wMainID;
			NetMsgDelete.wPosY = vecPILEDEL[i].wSubID;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgDelete);
		}

		for ( DWORD b=0; b<vecPILE.size(); ++b )
		for ( DWORD c=0; c<vecPILE.size()-1; ++c )
		{
			SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( vecPILE[c].sNativeID );
			if ( !pHoldData )	continue;
			
			SITEM* pHoldData2 = GLItemMan::GetInstance().GetItem ( vecPILE[c+1].sNativeID );
			if ( !pHoldData2 )	continue;
			
			BOOL bCOMPARE = FALSE;
			
			if ( pHoldData->sBasicOp.sNativeID.dwID > pHoldData2->sBasicOp.sNativeID.dwID ) bCOMPARE = TRUE;
			else if ( pHoldData->sBasicOp.sNativeID == pHoldData2->sBasicOp.sNativeID && vecPILE[c].wTurnNum != vecPILE[c+1].wTurnNum ) bCOMPARE = TRUE;
			
			if ( bCOMPARE ) 
			{
				SITEMCUSTOM sCUSTOM = vecPILE[c+1];
				vecPILE[c+1] = vecPILE[c];
				vecPILE[c] = sCUSTOM;
			}
		}

		WORD _X = 0, _Y = 0;
		for( int i = 0; i < vecPILE.size(); i++ )
		{		
			SITEM* pHoldData = GLItemMan::GetInstance().GetItem ( vecPILE[i].sNativeID );
			if ( !pHoldData )
			{
				//MessageBox(NULL,"if ( !pHoldData )",NULL,NULL);
				continue;
			}
			BOOL bOk = m_cInventory.IsInsertable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, _X, _Y );
			if ( !bOk )
			{
				bOk = m_cInventory.FindInsrtable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, _X, _Y );
				if ( !bOk )
				{
					//MessageBox(NULL,"bOk = m_cInventory.FindInsrtable ( pHoldData->sBasicOp.wInvenSizeX, pHoldData->sBasicOp.wInvenSizeY, _X, _Y );","",NULL);
					continue;
				}
			}
			bOk = m_cInventory.InsertItem ( vecPILE[i], _X, _Y );
			if ( !bOk )	
			{
				//MessageBox(NULL,"!bOk = m_cInventory.InsertItem ( vecPILE[i], _X, _Y );","",NULL);
				continue;
			}
			
			SINVENITEM sINVENITEM = *m_cInventory.FindPosItem ( _X, _Y );
			
			GLMSG::SNETPC_INVEN_INSERT NetMsg_Inven_Insert;
			NetMsg_Inven_Insert.Data = sINVENITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Insert);

			SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sINVENITEM.sItemCustom.sNativeID );
			if ( pITEM && pITEM->IsTIMELMT() )
			{
				if ( pITEM->sDrugOp.tTIME_LMT != 0 )
				{
					const CTime cTIME_CUR = CTime::GetCurrentTime();

					CTimeSpan cSPAN(pITEM->sDrugOp.tTIME_LMT);
					CTime cTIME_LMT(sINVENITEM.sItemCustom.tBORNTIME);
					cTIME_LMT += cSPAN;

					if ( cTIME_CUR > cTIME_LMT )
					{
						GLMSG::SNETPC_INVEN_DELETE NetMsgInvenDel;
						NetMsgInvenDel.wPosX = sINVENITEM.wPosX;
						NetMsgInvenDel.wPosY = sINVENITEM.wPosY;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDel);

						GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT NetMsgInvenDelTimeLmt;
						NetMsgInvenDelTimeLmt.nidITEM = sINVENITEM.sItemCustom.sNativeID;
						GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInvenDelTimeLmt);

						m_cInventory.DeleteItem ( sINVENITEM.wPosX, sINVENITEM.wPosY );
					
					}
				}
			}

			if ( _X < 5 ) _X++;
			else
			{
				_X = 0;
				_Y++;
			}
		}
	}

	vecPILEDEL.clear();
	vecPILE.clear();
	vecDEL.clear();
	vecDEL2.clear();

	m_fInventorySortTimer = 0.0f;

	return S_OK;
}

/*buffs manual remove, Juver, 2018/08/14 */
HRESULT GLChar::MsgReqBuffRemove( NET_MSG_GENERIC* nmg )
{
	if ( !RPARAM::allow_buff_remove )	return E_FAIL;

	GLMSG::SNETPC_REQ_BUFF_REMOVE* pnet_msg_client = ( GLMSG::SNETPC_REQ_BUFF_REMOVE* ) nmg;

	SNATIVEID skill_id = pnet_msg_client->skill_id;
	PGLSKILL pskill = GLSkillMan::GetInstance().GetData( skill_id );
	if ( !pskill )					return E_FAIL;
	if ( !pskill->IsSkillFact() )	return E_FAIL;
	if ( pskill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )		return E_FAIL;

	BOOL buff_exist = FALSE;
	for( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		const SSKILLFACT& skill_fact = m_sSKILLFACT[i];
		if ( skill_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( skill_fact.sNATIVEID == skill_id )	
			buff_exist = TRUE;
	}

	if ( !buff_exist )	return E_FAIL;

	GLMSG::SNETPC_SKILLHOLD_RS_BRD NetMsgSkillBrd;
	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
		if ( m_sSKILLFACT[i].sNATIVEID != skill_id )		continue;

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[i].sNATIVEID );
		if ( !pSkill ) continue;
		
		DISABLESKEFF( i );
		NetMsgSkillBrd.bRESET[i] = true;
	}

	NetMsgSkillBrd.dwID = m_dwGaeaID;
	NetMsgSkillBrd.emCrow = CROW_PC;

	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgSkillBrd );
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &NetMsgSkillBrd );

	return S_OK;
}

/*specific item box, Juver, 2018/09/03 */
HRESULT GLChar::msg_req_inven_open_item_box_specific( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_REQ_OPEN_ITEM_BOX_SPECIFIC* pnet_msg_client = ( GLMSG::SNET_INVEN_REQ_OPEN_ITEM_BOX_SPECIFIC* ) nmg;
	GLMSG::SNET_INVEN_REQ_OPEN_ITEM_BOX_SPECIFIC_FB net_msg_fb;

	if ( m_wLevel != pnet_msg_client->wLevel )
	{
		//CDebugSet::ToFileWithTime( "openbox.txt", "msg_req_inven_open_item_box_specific mismatch level server:%u, client:%u", m_wLevel, pnet_msg_client->wLevel );
		return E_FAIL;
	}

	SINVENITEM* pinventory_item = m_cInventory.FindPosItem ( pnet_msg_client->pos_x, pnet_msg_client->pos_y );
	if ( !pinventory_item )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SITEM* pitem_data_main = GLItemMan::GetInstance().GetItem ( pinventory_item->sItemCustom.sNativeID );
	if ( !pitem_data_main )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pitem_data_main->sBasicOp.emItemType != ITEM_BOX_SPECIFIC )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( !pitem_data_main->sBox.VALID() )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_BAD_BOX;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( !SIMPLE_CHECK_ITEM( pitem_data_main->sBasicOp.sNativeID, true ) )
	{		
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pnet_msg_client->item_selected_index < 0 || pnet_msg_client->item_selected_index >= ITEM::SBOX::ITEM_SIZE )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_BAD_SELECTED;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pnet_msg_client->item_id_selected == NATIVEID_NULL() )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_BAD_SELECTED;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SITEM* pitem_data_selected = GLItemMan::GetInstance().GetItem( pnet_msg_client->item_id_selected );
	if ( !pitem_data_selected )	
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_BAD_SELECTED;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pitem_data_main->sBox.sITEMS[pnet_msg_client->item_selected_index].nidITEM != pnet_msg_client->item_id_selected )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_BAD_SELECTED_EXIST;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	WORD found_pos_x, found_pos_y;
	BOOL bOk = m_cInventory.FindInsrtable ( pitem_data_selected->sBasicOp.wInvenSizeX, pitem_data_selected->sBasicOp.wInvenSizeY, found_pos_x, found_pos_y );
	if ( !bOk )	
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_NO_INVEN_SPACE;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SITEMCUSTOM sITEM_NEW;
	sITEM_NEW.sNativeID = pitem_data_main->sBox.sITEMS[pnet_msg_client->item_selected_index].nidITEM;
	DWORD dwAMOUNT = pitem_data_main->sBox.sITEMS[pnet_msg_client->item_selected_index].dwAMOUNT;
	
	CTime cTIME = CTime::GetCurrentTime();
	sITEM_NEW.tBORNTIME = cTIME.GetTime();

	sITEM_NEW.wTurnNum = (WORD) dwAMOUNT;
	sITEM_NEW.cGenType = pinventory_item->sItemCustom.cGenType;
	sITEM_NEW.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
	sITEM_NEW.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
	sITEM_NEW.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( sITEM_NEW.sNativeID, (EMITEMGEN)sITEM_NEW.cGenType );

	sITEM_NEW.cDAMAGE = (BYTE)pitem_data_selected->sBasicOp.wGradeAttack;
	sITEM_NEW.cDEFENSE = (BYTE)pitem_data_selected->sBasicOp.wGradeDefense;

	/*item color, Juver, 2018/01/08 */
	SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( sITEM_NEW.sNativeID );
	if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
	{
		sITEM_NEW.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
		sITEM_NEW.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
	}

	//	랜덤 옵션 생성.
	if( sITEM_NEW.GENERATE_RANDOM_OPT() )
	{
		GLITEMLMT::GetInstance().ReqRandomItem( sITEM_NEW );
	}

	/* set item option, Juver, 2021/09/01 */
	if ( pitem_data_check )
	{
		sITEM_NEW.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_check->sBasicOp.strGenSetOption );
	}

	//	생성한 아이탬 인밴에 넣음.
	BOOL item_inserted = m_cInventory.InsertItem ( sITEM_NEW, found_pos_x, found_pos_y );
	if ( !item_inserted )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_NO_INVEN_SPACE;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );

		CDebugSet::ToFileWithTime( "item_box_specific.txt", "[%d]%d selected item failed to insert! item:[%u/%u] %s pos: [%u/%u]", m_dwCharID, m_szName, 
			pitem_data_selected->sBasicOp.sNativeID.wMainID, pitem_data_selected->sBasicOp.sNativeID.wSubID, pitem_data_selected->GetName(), found_pos_x, found_pos_y );

		return E_FAIL;
	}

	SINVENITEM *pinventory_item_inserted = m_cInventory.GetItem ( found_pos_x, found_pos_y );
	if ( !pinventory_item_inserted )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_NO_INVEN_SPACE;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );

		CDebugSet::ToFileWithTime( "item_box_specific.txt", "[%d]%d inserted item failed to get! item:[%u/%u] %s pos: [%u/%u]", m_dwCharID, m_szName, 
			pitem_data_selected->sBasicOp.sNativeID.wMainID, pitem_data_selected->sBasicOp.sNativeID.wSubID, pitem_data_selected->GetName(), found_pos_x, found_pos_y );

		return E_FAIL;
	}

	GLITEMLMT::GetInstance().ReqItemRoute ( pinventory_item_inserted->sItemCustom, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, pinventory_item_inserted->sItemCustom.wTurnNum );

	//	[자신에게] 메시지 발생.
	GLMSG::SNETPC_INVEN_INSERT net_msg_inven_insert;
	net_msg_inven_insert.Data = *pinventory_item_inserted;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_inven_insert);

	/*activity system, Juver, 2017/11/03 */
	DoActivityTakeItem( sITEM_NEW.sNativeID, sITEM_NEW.wTurnNum );


	GLITEMLMT::GetInstance().ReqItemRoute ( pinventory_item->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pinventory_item->sItemCustom.wTurnNum );

	BOOL delete_box = m_cInventory.DeleteItem ( pnet_msg_client->pos_x, pnet_msg_client->pos_y );
	if ( !delete_box )
	{
		net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );

		CDebugSet::ToFileWithTime( "item_box_specific.txt", "[%d]%d box item failed to delete! item duplicated! item:[%u/%u] %s pos: [%u/%u]", m_dwCharID, m_szName, 
			pitem_data_main->sBasicOp.sNativeID.wMainID, pitem_data_main->sBasicOp.sNativeID.wSubID, pitem_data_main->GetName(), pnet_msg_client->pos_x, pnet_msg_client->pos_y );

		return E_FAIL;
	}
	
	GLMSG::SNETPC_INVEN_DELETE net_msg_inven_delete;
	net_msg_inven_delete.wPosX = pnet_msg_client->pos_x;
	net_msg_inven_delete.wPosY = pnet_msg_client->pos_y;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_inven_delete);

	net_msg_fb.emFB = EMREQ_ITEM_BOX_SPECIFIC_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);

	return S_OK;
};

/*manual lunchbox remove, Juver, 2018/09/19 */
HRESULT GLChar::msg_req_lunchbox_remove ( NET_MSG_GENERIC* nmg )
{
	if ( !RPARAM::allow_lunchbox_remove )	return E_FAIL;

	GLMSG::SNETPC_REQ_LUNCHBOX_MANUAL_REMOVE* pnet_msg_client = ( GLMSG::SNETPC_REQ_LUNCHBOX_MANUAL_REMOVE* ) nmg;

	SNATIVEID skill_id = pnet_msg_client->skill_id;
	PGLSKILL pskill = GLSkillMan::GetInstance().GetData( skill_id );
	if ( !pskill )					return E_FAIL;
	if ( !pskill->IsSkillFact() )	return E_FAIL;
	if ( pskill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )		return E_FAIL;

	BOOL exist = FALSE;
	for( int i=0; i<FITEMFACT_SIZE; ++i )
	{
		const SFITEMFACT& food_fact = m_sFITEMFACT[i];
		if ( food_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( food_fact.sNATIVEID == skill_id )	
			exist = TRUE;
	}

	if ( !exist )	return E_FAIL;

	GLMSG::SNETPC_LUNCHBOX_REMOVE_BRD net_msg_brd;
	for ( int i=0; i<FITEMFACT_SIZE; ++i )
	{
		if ( m_sFITEMFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
		if ( m_sFITEMFACT[i].sNATIVEID != skill_id )		continue;

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sFITEMFACT[i].sNATIVEID );
		if ( !pSkill ) continue;

		m_sFITEMFACT[i].RESET();

		net_msg_brd.bRESET[i] = true;
	}

	net_msg_brd.dwID = m_dwGaeaID;
	net_msg_brd.emCrow = CROW_PC;

	SendMsgViewAround ( (NET_MSG_GENERIC*) &net_msg_brd );
	m_pGLGaeaServer->SENDTOCLIENT ( m_dwClientID, &net_msg_brd );

	return S_OK;
}

void GLChar::MsgReqCurrencyShopPurchase( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_CURRENCY_SHOP_PURCHASE *pNetMsgClient = (GLMSG::SNETPC_REQ_CURRENCY_SHOP_PURCHASE *) nmg;
	GLMSG::SNETPC_REQ_CURRENCY_SHOP_PURCHASE_FB NetMsgFB;

	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData( pNetMsgClient->sCrowID );
	if ( !pCrowData )
	{
		NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_INVALID_CROW;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SCURRENCY_SHOP_DATA* pShop = GLCrowDataMan::GetInstance().CurrencyShopFind( pCrowData->m_sAction.m_strCurrencyShop );
	if ( !pShop )
	{
		NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_INVALID_SHOP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SCURRENCY_SHOP_ITEM* pShopItem = pShop->FindData( pNetMsgClient->sItemID );
	if ( !pShopItem )
	{
		NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_INVALID_SHOP_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SITEM* pItemData = GLItemMan::GetInstance().GetItem( pShopItem->sItemID );
	if ( !pItemData )
	{
		NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_INVALID_ITEM_DATA;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	volatile LONGLONG llPriceMoney = pShopItem->llPriceMoney;
	if ( m_lnMoney < llPriceMoney )
	{
		NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_NOT_ENOUGH_MONEY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	volatile DWORD dwPriceGamePoints = pShopItem->dwPriceGamePoints;
	if ( m_dwGamePoints < dwPriceGamePoints )
	{
		NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_NOT_ENOUGH_GAME_POINTS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	volatile DWORD dwPriceWarChips = pShopItem->dwPriceWarChips;
	if ( m_dwWarChips < dwPriceWarChips )
	{
		NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_NOT_ENOUGH_WAR_CHIPS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( llPriceMoney == 0 && dwPriceGamePoints == 0 && dwPriceWarChips == 0 )
	{
		NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_ERROR_COST;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( m_fCurrencyShopDelayTimer < RPARAM::fCurrencyShopPurchaseDelayTime )
	{
		NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_DELAY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	m_fCurrencyShopDelayTimer = 0.0f;

	WORD winvensizex = pItemData->sBasicOp.wInvenSizeX;
	WORD winvensizey = pItemData->sBasicOp.wInvenSizeY;

	BOOL bITEM_SPACE(FALSE);
	if ( pItemData->ISPILE() )
	{
		WORD wPILENUM = pItemData->sDrugOp.wPileNum;
		SNATIVEID sNID = pItemData->sBasicOp.sNativeID;
		WORD wREQINSRTNUM = ( 1 * pItemData->GETAPPLYNUM() );

		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, winvensizex, winvensizey );

		if ( !bITEM_SPACE )
		{
			NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_NOT_ENOUGH_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return;
		}

		SITEMCUSTOM snew_item(sNID);
		snew_item.cGenType = EMGEN_SHOP;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.wTurnNum = wPILENUM;
		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		if ( llPriceMoney > 0 )
		{
			CheckMoneyUpdate( m_lnMoney, llPriceMoney, FALSE, "Buy From SHOP." );
			m_bMoneyUpdate = TRUE;

			m_lnMoney -= llPriceMoney;

			if ( llPriceMoney>EMMONEY_LOG )
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPriceMoney), EMITEM_ROUTE_DELETE );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
			}

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgMoney);
		}

		if ( dwPriceGamePoints > 0 )
		{
			m_dwGamePoints -= dwPriceGamePoints;

			GLMSG::SNETPC_UPDATE_GAME_POINTS NetMsgClient;
			NetMsgClient.dwGamePoints = m_dwGamePoints;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgClient);
		}

		if ( dwPriceWarChips > 0 )
		{
			m_dwWarChips -= dwPriceWarChips;

			GLMSG::SNETPC_UPDATE_WAR_CHIPS NetMsgClient;
			NetMsgClient.dwWarChips = m_dwWarChips;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgClient);
		}

		InsertPileItem ( snew_item, wREQINSRTNUM, EMITEM_ROUTE_CHAR );

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, wREQINSRTNUM );
	}
	else
	{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );

		if ( !bITEM_SPACE )	
		{
			NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_NOT_ENOUGH_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return;
		}

		SITEMCUSTOM snew_item( pItemData->sBasicOp.sNativeID );
		snew_item.cDAMAGE = pShopItem->cDAMAGE;
		snew_item.cDEFENSE = pShopItem->cDEFENSE;
		snew_item.cRESIST_FIRE = pShopItem->cRESIST_FIRE;
		snew_item.cRESIST_ICE = pShopItem->cRESIST_ICE;
		snew_item.cRESIST_ELEC = pShopItem->cRESIST_ELEC;
		snew_item.cRESIST_POISON = pShopItem->cRESIST_POISON;
		snew_item.cRESIST_SPIRIT = pShopItem->cRESIST_SPIRIT;

		CTime cTIME = CTime::GetCurrentTime();
		snew_item.tBORNTIME = cTIME.GetTime();
		snew_item.wTurnNum = 1;
		snew_item.cGenType = (BYTE) EMGEN_SHOP;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE) GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pItemData->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		if ( pShopItem->bGenerateRandomValue )
		{
			if( snew_item.GENERATE_RANDOM_OPT () )
			{
				GLITEMLMT::GetInstance().ReqRandomItem( snew_item );
			}

			/* set item option, Juver, 2021/09/01 */
			if ( pitem_data_check )
			{
				snew_item.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_check->sBasicOp.strGenSetOption );
			}
		}

		if ( llPriceMoney > 0 )
		{
			CheckMoneyUpdate( m_lnMoney, llPriceMoney, FALSE, "Buy From SHOP." );
			m_bMoneyUpdate = TRUE;

			m_lnMoney -= llPriceMoney;

			if ( llPriceMoney>EMMONEY_LOG )
			{
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(llPriceMoney), EMITEM_ROUTE_DELETE );
				GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
			}

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgMoney);
		}

		if ( dwPriceGamePoints > 0 )
		{
			m_dwGamePoints -= dwPriceGamePoints;

			GLMSG::SNETPC_UPDATE_GAME_POINTS NetMsgClient;
			NetMsgClient.dwGamePoints = m_dwGamePoints;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgClient);
		}

		if ( dwPriceWarChips > 0 )
		{
			m_dwWarChips -= dwPriceWarChips;

			GLMSG::SNETPC_UPDATE_WAR_CHIPS NetMsgClient;
			NetMsgClient.dwWarChips = m_dwWarChips;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgClient);
		}

		BOOL bOK = m_cInventory.InsertItem ( snew_item, wInsertPosX, wInsertPosY );
		if ( !bOK )
		{
			NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_ERROR_INSERT;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return;
		}

		GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );

		GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
		NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
	}

	NetMsgFB.emFB = EMREQ_CURRENCY_SHOP_PURCHASE_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
}

/* chaos machine, Juver, 2021/07/09 */
void GLChar::MsgInvenChaosMachine( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_INVEN_CHAOS_MACHINE *pNetMsgClient = (GLMSG::SNETPC_INVEN_CHAOS_MACHINE *) nmg;
	GLMSG::SNETPC_INVEN_CHAOS_MACHINE_FB NetMsgFB;

	PGLCROW pCrow = m_pLandMan->GetCrow ( pNetMsgClient->dwNPCID );
	if ( !pCrow )
	{
		NetMsgFB.emFB = EMFB_CHAOS_MACHINE_INVALID_NPC;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	float fDist = D3DXVec3Length ( &D3DXVECTOR3(m_vPos-pCrow->GetPosition()) );
	float fTalkRange = (float) (pCrow->GetBodyRadius() + GETBODYRADIUS() + 30);
	float fTalkableDis = fTalkRange + 20;

	if ( fDist>fTalkableDis )
	{
		NetMsgFB.emFB = EMFB_CHAOS_MACHINE_INVALID_NPC;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsgClient->sInvenItem.wPosX, pNetMsgClient->sInvenItem.wPosY );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMFB_CHAOS_MACHINE_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SITEM* pItemMain = GLItemMan::GetInstance().GetItem( pInvenItem->sItemCustom.sNativeID );
	if ( !pItemMain )
	{
		NetMsgFB.emFB = EMFB_CHAOS_MACHINE_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SCHAOS_MACHINE_DATA* pChaosMachineData = GLChaosMachine::GetInstance().GetData( pItemMain->sBasicOp.dwChaosMachineID );
	if ( !pChaosMachineData )
	{
		NetMsgFB.emFB = EMFB_CHAOS_MACHINE_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( pInvenItem->sItemCustom.IsGM_GENITEM() )
	{
		NetMsgFB.emFB = EMFB_CHAOS_MACHINE_NON_DROP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( pChaosMachineData->llCost != 0 && m_lnMoney < pChaosMachineData->llCost ) 
	{
		NetMsgFB.emFB = EMFB_CHAOS_MACHINE_COST;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}


	//requirements check
	//consume materials
	for( int i=0; i<CHAOS_MACHINE_MATERIAL_NUM; ++i )
	{
		SITEM* pItemReq = GLItemMan::GetInstance().GetItem( pChaosMachineData->sMaterial[i].sItemID );
		if ( pItemReq )
		{
			DWORD dwReqCount = pChaosMachineData->sMaterial[i].wNum;

			if ( pItemReq->ISPILE() )
			{
				DWORD dwItemNum = m_cInventory.CountTurnItem( pItemReq->sBasicOp.sNativeID );
				if ( dwItemNum < dwReqCount )
				{
					NetMsgFB.emFB = EMFB_CHAOS_MACHINE_NO_MATERIAL;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
					return;
				}

				BOOL bDelete = DeletePileItem( pItemReq->sBasicOp.sNativeID, dwReqCount, EMITEM_ROUTE_DELETE );
				if ( !bDelete )
				{
					NetMsgFB.emFB = EMFB_CHAOS_MACHINE_FAILED_DELETE;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
					return;
				}
			}
			else
			{
				//not pile just check if exist
				SINVENITEM* pinvenitem = m_cInventory.FindItem( pItemReq->sBasicOp.sNativeID );
				if ( !pinvenitem )
				{
					NetMsgFB.emFB = EMFB_CHAOS_MACHINE_NO_MATERIAL;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
					return;
				}

				WORD wposX = pinvenitem->wPosX;
				WORD wposY = pinvenitem->wPosY;

				GLITEMLMT::GetInstance().ReqItemRoute( pinvenitem->sItemCustom, ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, pinvenitem->sItemCustom.wTurnNum );

				BOOL bDelete = m_cInventory.DeleteItem ( wposX, wposY );
				if ( !bDelete )
				{
					NetMsgFB.emFB = EMFB_CHAOS_MACHINE_FAILED_DELETE;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
					return;
				}

				GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
				NetMsg_Inven_Delete.wPosX = wposX;
				NetMsg_Inven_Delete.wPosY = wposY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Delete);
			}
		}
	}

	//consume gold
	if ( pChaosMachineData->llCost > 0 )
	{
		CheckMoneyUpdate( m_lnMoney, pChaosMachineData->llCost, FALSE, "CHAOS_MACHINE" );
		m_bMoneyUpdate = TRUE;

		m_lnMoney -= pChaosMachineData->llCost;

		GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
		NetMsgMoney.lnMoney = m_lnMoney;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &NetMsgMoney );

		if ( pChaosMachineData->llCost >=  EMMONEY_LOG )
		{
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, -LONGLONG(pChaosMachineData->llCost), EMITEM_ROUTE_DELETE );
			GLITEMLMT::GetInstance().ReqMoneyExc ( ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnMoney, EMITEM_ROUTE_CHAR );
		}
	}

	//consume main item
	if ( pChaosMachineData->bConsumeMainItem )
	{
		if ( pItemMain->ISPILE() )
		{
			bool bDelete = DoDrugInvenItem( pNetMsgClient->sInvenItem.wPosX, pNetMsgClient->sInvenItem.wPosY );
			if ( !bDelete )
			{
				NetMsgFB.emFB = EMFB_CHAOS_MACHINE_FAILED_DELETE;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
				return;
			}
		}
		else
		{
			BOOL bDelete = m_cInventory.DeleteItem ( pNetMsgClient->sInvenItem.wPosX, pNetMsgClient->sInvenItem.wPosY );
			if ( !bDelete )
			{
				NetMsgFB.emFB = EMFB_CHAOS_MACHINE_FAILED_DELETE;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
				return;
			}

			GLMSG::SNETPC_INVEN_DELETE NetMsg_Inven_Delete;
			NetMsg_Inven_Delete.wPosX = pNetMsgClient->sInvenItem.wPosX;
			NetMsg_Inven_Delete.wPosY = pNetMsgClient->sInvenItem.wPosY;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_Inven_Delete);
		}
	}

	//fail or pass?
	if ( !RANDOM_GEN( pChaosMachineData->fChance ) )
	{
		NetMsgFB.emFB = EMFB_CHAOS_MACHINE_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}


	//get the result item
	SNATIVEID sResultItemID = pChaosMachineData->getResultItem();

	SITEM* pItemResult = GLItemMan::GetInstance().GetItem( sResultItemID );
	if ( !pItemResult )
	{
		//result item bugged?
		NetMsgFB.emFB = EMFB_CHAOS_MACHINE_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

		CDebugSet::ToFileWithTime( "_ChaosMachine.txt", "Result Item Failed: %d/%d %d", sResultItemID.wMainID, sResultItemID.wSubID, pChaosMachineData->getCurrentResultIndex() );
		return;
	}


	//insert new item
	WORD winvensizex = pItemResult->sBasicOp.wInvenSizeX;
	WORD winvensizey = pItemResult->sBasicOp.wInvenSizeY;

	BOOL bITEM_SPACE(FALSE);
	if ( pItemResult->ISPILE() )
	{
		WORD wPILENUM = pItemResult->sDrugOp.wPileNum;
		SNATIVEID sNID = pItemResult->sBasicOp.sNativeID;
		WORD wREQINSRTNUM = ( pItemResult->GETAPPLYNUM() );

		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, winvensizex, winvensizey );

		if ( !bITEM_SPACE )
		{
			NetMsgFB.emFB = EMFB_CHAOS_MACHINE_NO_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return;
		}

		SITEMCUSTOM snew_item(sNID);
		snew_item.cGenType = EMGEN_NPC;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pItemResult->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );
		snew_item.wTurnNum = wPILENUM;
		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		BOOL bOK = InsertPileItem ( snew_item, wREQINSRTNUM, EMITEM_ROUTE_CHAR );
		if ( !bOK )	
		{
			NetMsgFB.emFB = EMFB_CHAOS_MACHINE_NO_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return;
		}

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
	}
	else
	{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );
		if ( !bITEM_SPACE )	
		{
			NetMsgFB.emFB = EMFB_CHAOS_MACHINE_NO_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return;
		}

		SITEMCUSTOM snew_item( pItemResult->sBasicOp.sNativeID );
		snew_item.cDAMAGE = (BYTE)pItemResult->sBasicOp.wGradeAttack;
		snew_item.cDEFENSE = (BYTE)pItemResult->sBasicOp.wGradeDefense;
		/*snew_item.cRESIST_FIRE = pnpc_item_exchange_data->cRESIST_FIRE;
		snew_item.cRESIST_ICE = pnpc_item_exchange_data->cRESIST_ICE;
		snew_item.cRESIST_ELEC = pnpc_item_exchange_data->cRESIST_ELEC;
		snew_item.cRESIST_POISON = pnpc_item_exchange_data->cRESIST_POISON;
		snew_item.cRESIST_SPIRIT = pnpc_item_exchange_data->cRESIST_SPIRIT;*/
		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();
		snew_item.wTurnNum = 1;
		snew_item.cGenType = (BYTE) EMGEN_NPC;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE) GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pItemResult->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		if( snew_item.GENERATE_RANDOM_OPT () )
		{
			GLITEMLMT::GetInstance().ReqRandomItem( snew_item );
		}

		/* set item option, Juver, 2021/09/01 */
		if ( pitem_data_check )
		{
			snew_item.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_check->sBasicOp.strGenSetOption );
		}

		BOOL bOK = m_cInventory.InsertItem ( snew_item, wInsertPosX, wInsertPosY );
		if ( !bOK )	
		{
			NetMsgFB.emFB = EMFB_CHAOS_MACHINE_NO_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return;
		}

		GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );

		GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
		NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
	}

	NetMsgFB.emFB = EMFB_CHAOS_MACHINE_SUCCESS;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

}

/* set item option, Juver, 2021/09/04 */
HRESULT GLChar::MsgSetOptionInsert ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_SET_OPTION_INSERT *pnet_msg_client = (GLMSG::SNET_INVEN_SET_OPTION_INSERT *)nmg;
	GLMSG::SNET_INVEN_SET_OPTION_INSERT_FB net_msg_fb;

	if ( !VALID_HOLD_ITEM() )	return S_FALSE;

	SITEM* pitem_data_hold = GLItemMan::GetInstance().GetItem ( m_PutOnItems[SLOT_HOLD].sNativeID );
	if ( !pitem_data_hold )		return E_FAIL;

	if ( CheckCoolTime( pitem_data_hold->sBasicOp.sNativeID ) ) return E_FAIL;

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_SET_OPTION_INSERT)
	{
		net_msg_fb.emFB = EM_SET_OPTION_INSERT_INVALID_CARD_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SINVENITEM* pinven_item_target = m_cInventory.GetItem ( pnet_msg_client->wPosX, pnet_msg_client->wPosY );
	if ( !pinven_item_target )
	{
		net_msg_fb.emFB = EM_SET_OPTION_INSERT_INVALID_TARGET_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		net_msg_fb.emFB = EM_SET_OPTION_INSERT_INVALID_TARGET_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{	
		net_msg_fb.emFB = EM_SET_OPTION_INSERT_TARGET_NOT_SUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pitem_data_target->sBasicOp.strGenSetOption.empty() )
	{
		net_msg_fb.emFB = EM_SET_OPTION_INSERT_TARGET_NOT_SET_OPTION;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pinven_item_target->sItemCustom.wSetItemID != SET_OPTION_NULL )
	{
		net_msg_fb.emFB = EM_SET_OPTION_INSERT_TARGET_ALREADY;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	pinven_item_target->sItemCustom.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_target->sBasicOp.strGenSetOption );

	BOOL bErase = DoDrugSlotItem(SLOT_HOLD);
	if ( !bErase )
	{
		SINVENITEM* pinven_item_card = m_cInventory.FindItem( ITEM_SET_OPTION_INSERT );
		if ( pinven_item_card )
			bErase = DoDrugInvenItem( pinven_item_card->wPosX, pinven_item_card->wPosY );
	}

	if ( !bErase )
		CDebugSet::ToFileWithTime( "_set_option_insert.txt", "[%d] %s cant find card", m_dwCharID, m_szName );

	GLMSG::SNET_INVEN_ITEM_UPDATE net_msg_client_item_update;
	net_msg_client_item_update.wPosX = pnet_msg_client->wPosX;
	net_msg_client_item_update.wPosY = pnet_msg_client->wPosY;
	net_msg_client_item_update.sItemCustom = pinven_item_target->sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_item_update );

	GLITEMLMT::GetInstance().ReqItemConversion ( pinven_item_target->sItemCustom, ID_CHAR, m_dwCharID );

	net_msg_fb.emFB = EM_SET_OPTION_INSERT_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );


	return S_OK;
}


/* set item option, Juver, 2021/09/04 */
HRESULT GLChar::MsgSetOptionReroll ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_INVEN_SET_OPTION_REROLL *pnet_msg_client = (GLMSG::SNET_INVEN_SET_OPTION_REROLL *)nmg;
	GLMSG::SNET_INVEN_SET_OPTION_REROLL_FB net_msg_fb;

	if ( !VALID_HOLD_ITEM() )	return S_FALSE;

	SITEM* pitem_data_hold = GLItemMan::GetInstance().GetItem ( m_PutOnItems[SLOT_HOLD].sNativeID );
	if ( !pitem_data_hold )		return E_FAIL;

	if ( CheckCoolTime( pitem_data_hold->sBasicOp.sNativeID ) ) return E_FAIL;

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_SET_OPTION_REROLL )
	{
		net_msg_fb.emFB = EM_SET_OPTION_REROLL_INVALID_CARD_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SINVENITEM* pinven_item_target = m_cInventory.GetItem ( pnet_msg_client->wPosX, pnet_msg_client->wPosY );
	if ( !pinven_item_target )
	{
		net_msg_fb.emFB = EM_SET_OPTION_REROLL_INVALID_TARGET_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		net_msg_fb.emFB = EM_SET_OPTION_REROLL_INVALID_TARGET_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{	
		net_msg_fb.emFB = EM_SET_OPTION_REROLL_TARGET_NOT_SUIT;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	if ( pitem_data_target->sBasicOp.strGenSetOption.empty() )
	{
		net_msg_fb.emFB = EM_SET_OPTION_REROLL_TARGET_NOT_SET_OPTION;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
		return E_FAIL;
	}

	pinven_item_target->sItemCustom.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_target->sBasicOp.strGenSetOption );

	BOOL bErase = DoDrugSlotItem(SLOT_HOLD);
	if ( !bErase )
	{
		SINVENITEM* pinven_item_card = m_cInventory.FindItem( ITEM_SET_OPTION_REROLL );
		if ( pinven_item_card )
			bErase = DoDrugInvenItem( pinven_item_card->wPosX, pinven_item_card->wPosY );
	}

	if ( !bErase )
		CDebugSet::ToFileWithTime( "_set_option_reroll.txt", "[%d] %s cant find card", m_dwCharID, m_szName );

	GLMSG::SNET_INVEN_ITEM_UPDATE net_msg_client_item_update;
	net_msg_client_item_update.wPosX = pnet_msg_client->wPosX;
	net_msg_client_item_update.wPosY = pnet_msg_client->wPosY;
	net_msg_client_item_update.sItemCustom = pinven_item_target->sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_item_update );

	GLITEMLMT::GetInstance().ReqItemConversion ( pinven_item_target->sItemCustom, ID_CHAR, m_dwCharID );

	net_msg_fb.emFB = EM_SET_OPTION_REROLL_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );


	return S_OK;
}

HRESULT GLChar::ReqNpcContributionShopPurchase ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_NPC_CONTRIBUTION_SHOP_PURCHASE *pnetmsgclient = reinterpret_cast<GLMSG::SNETPC_REQ_NPC_CONTRIBUTION_SHOP_PURCHASE*> ( nmg );
	GLMSG::SNETPC_REQ_NPC_CONTRIBUTION_SHOP_PURCHASE_FB	netmsgfb;

	SITEM* pitem = GLItemMan::GetInstance().GetItem ( pnetmsgclient->sidItem );
	if ( !pitem )	
	{
		netmsgfb.emFB = EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	SCROWDATA*	pcrow = GLCrowDataMan::GetInstance().GetCrowData( pnetmsgclient->sidCrow );
	if ( !pcrow )
	{
		netmsgfb.emFB = EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_INVALID_CROW;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	SNPC_CONTRIBUTION_SHOP_DATA* pNpcShop = GLCrowDataMan::GetInstance().NPCContributionShopFind( pcrow->m_sAction.m_strContributionShop );
	if ( !pNpcShop )
	{
		netmsgfb.emFB = EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_INVALID_SHOP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	const SNPC_CONTRIBUTION_SHOP_ITEM* pshopitem = pNpcShop->FindItem( pnetmsgclient->sidItem.dwID );
	if ( !pshopitem )
	{
		netmsgfb.emFB = EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_INVALID_SHOPITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}


	//contribution check
	volatile DWORD dwContriPrice = pshopitem->dwContriPrice;
	if ( m_dwContributionPoint < dwContriPrice )
	{
		netmsgfb.emFB = EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_NOT_CONTRIBUTION;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	//gold check
	volatile DWORD dwGoldPrice = pshopitem->dwGoldPrice;
	if ( m_lnMoney < dwGoldPrice )
	{
		netmsgfb.emFB = EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_NOT_GOLD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	/*contribution point, Juver, 2017/08/23 */
	if ( pitem->sBasicOp.dwReqContributionPoint != 0 &&
		m_dwContributionPoint < pitem->sBasicOp.dwReqContributionPoint )	
	{
		return E_FAIL;
	}

	/*activity point, Juver, 2017/08/23 */
	if ( pitem->sBasicOp.dwReqActivityPoint != 0 &&
		m_dwActivityPoint < pitem->sBasicOp.dwReqActivityPoint )
	{
		return E_FAIL;
	}

	/* play time system, Juver, 2021/01/27 */
	if ( pitem->sBasicOp.llPlayTimeReq != 0 && m_llPlayTime < pitem->sBasicOp.llPlayTimeReq )
	{
		return E_FAIL;
	}

	WORD winvensizex = pitem->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem->sBasicOp.wInvenSizeY;

	BOOL bITEM_SPACE(FALSE);
	if ( pitem->ISPILE() )
	{
		WORD wPILENUM = pitem->sDrugOp.wPileNum;
		SNATIVEID sNID = pitem->sBasicOp.sNativeID;
		WORD wREQINSRTNUM = ( 1 * pitem->GETAPPLYNUM() );

		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sNID, wPILENUM, winvensizex, winvensizey );

		if ( !bITEM_SPACE )
		{
			netmsgfb.emFB = EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_NOT_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			return E_FAIL;
		}

		SITEMCUSTOM snew_item(sNID);
		snew_item.cGenType = EMGEN_SHOP;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE)GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.wTurnNum = wPILENUM;
		snew_item.tBORNTIME = CTime::GetCurrentTime().GetTime();

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );

		m_dwContributionPoint -= dwContriPrice;

		GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT netMsgContriUpdate;
		netMsgContriUpdate.dwPoint = m_dwContributionPoint;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &netMsgContriUpdate );

		m_lnMoney -= dwGoldPrice;

		GLMSG::SNETPC_UPDATE_MONEY netMsgMoneyUpdate;
		netMsgMoneyUpdate.lnMoney = m_lnMoney;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &netMsgMoneyUpdate );

		//juvs.todo
		//log contribution point consume

		InsertPileItem ( snew_item, wREQINSRTNUM );

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, wREQINSRTNUM );
	}
	else
	{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );

		if ( !bITEM_SPACE )	
		{
			netmsgfb.emFB = EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_NOT_INVEN_SPACE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
			return E_FAIL;
		}

		SITEMCUSTOM snew_item( pitem->sBasicOp.sNativeID );
		snew_item.cDAMAGE = pshopitem->cDAMAGE;
		snew_item.cDEFENSE = pshopitem->cDEFENSE;
		snew_item.cRESIST_FIRE = pshopitem->cRESIST_FIRE;
		snew_item.cRESIST_ICE = pshopitem->cRESIST_ICE;
		snew_item.cRESIST_ELEC = pshopitem->cRESIST_ELEC;
		snew_item.cRESIST_POISON = pshopitem->cRESIST_POISON;
		snew_item.cRESIST_SPIRIT = pshopitem->cRESIST_SPIRIT;

		CTime cTIME = CTime::GetCurrentTime();
		snew_item.tBORNTIME = cTIME.GetTime();
		snew_item.wTurnNum = 1;
		snew_item.cGenType = (BYTE) EMGEN_SHOP;
		snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
		snew_item.cFieldID = (BYTE) GLGaeaServer::GetInstance().GetFieldSvrID();
		snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pitem->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );

		/*item color, Juver, 2018/01/08 */
		SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
		if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
		{
			snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
			snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
		}

		if ( pshopitem->bGenerateRandomValue )
		{
			if( snew_item.GENERATE_RANDOM_OPT () )
			{
				GLITEMLMT::GetInstance().ReqRandomItem( snew_item );
			}

			/* set item option, Juver, 2021/09/01 */
			if ( pitem_data_check )
			{
				snew_item.wSetItemID = GLSetOptionGen::GetInstance().GenerateSetOptionID( pitem_data_check->sBasicOp.strGenSetOption );
			}
		}

		m_dwContributionPoint -= dwContriPrice;

		GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT netMsgContriUpdate;
		netMsgContriUpdate.dwPoint = m_dwContributionPoint;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &netMsgContriUpdate );

		m_lnMoney -= dwGoldPrice;

		GLMSG::SNETPC_UPDATE_MONEY netMsgMoneyUpdate;
		netMsgMoneyUpdate.lnMoney = m_lnMoney;
		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &netMsgMoneyUpdate );

		//juvs.todo
		//log contribution point consume

		BOOL bOK = m_cInventory.InsertItem ( snew_item, wInsertPosX, wInsertPosY );
		if ( !bOK )		return S_OK;

		GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );

		GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
		NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

		/*activity system, Juver, 2017/11/03 */
		DoActivityTakeItem( snew_item.sNativeID, snew_item.wTurnNum );
	}

	netmsgfb.emFB = EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_PURCHASED;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);

	return S_OK;
}

HRESULT GLChar::ReqMaxRVPreview ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_MAXRV_PREVIEW *pnetmsgclient = reinterpret_cast<GLMSG::SNETPC_REQ_MAXRV_PREVIEW*> ( nmg );

	if ( RPARAM::bMaxRvPreviewEnable == FALSE )
		return E_FAIL;

	GLMSG::SNETPC_REQ_MAXRV_PREVIEW_FB	netmsgfb;

	SITEM* pItem = GLItemMan::GetInstance().GetItem(pnetmsgclient->sID);
	if ( !pItem )
	{
		//invalid item
		netmsgfb.emFB = EMMAXRV_PREVIEW_FB_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	if ( std::string(pItem->GetRandomOptFile()).empty() )
	{
		//item does not have valid random option file
		netmsgfb.emFB = EMMAXRV_PREVIEW_FB_INVALID_ITEM_RV;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	if ( m_fMaxRVPreviewTimer < MAX_RV_PREVIEW_COOLTIME )
	{
		//preview in cooldown
		netmsgfb.emFB = EMMAXRV_PREVIEW_FB_COOLTIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	const SRANDOM_OPTION_GEN* pRandomGen = GLRandomOptionManager::GetInstance().GetRandomOption( pItem->GetRandomOptFile() );
	if ( !pRandomGen )
	{
		//item does not have valid random option data
		netmsgfb.emFB = EMMAXRV_PREVIEW_FB_INVALID_ITEM_RV;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}


	//preview start
	netmsgfb.emFB = EMMAXRV_PREVIEW_FB_START;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);

	{ 
		//init
		GLMSG::SNETPC_REQ_MAXRV_PREVIEW_DATA_INIT	netmsgInit;
		netmsgInit.sID = pnetmsgclient->sID;

		if ( RPARAM::bMaxRvPreviewMobGen )
			netmsgInit.dwMaxRandomMobGen = pRandomGen->mapSET.size();

		if ( RPARAM::bMaxRvPreviewRebuild )
			netmsgInit.dwMaxRandomRebuild = pRandomGen->mapSETR.size();

		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgInit);
	}
	

	if ( RPARAM::bMaxRvPreviewMobGen )
	{
		SRANDOM_OPTION_SET_MAP_CITER it_b = pRandomGen->mapSET.begin();
		SRANDOM_OPTION_SET_MAP_CITER it_e = pRandomGen->mapSET.end();

		for( ; it_b != it_e; ++it_b )
		{
			const SRANDOM_OPTION_SET& sSet = (*it_b).second;

			GLMSG::SNETPC_REQ_MAXRV_PREVIEW_DATA_MOB_GEN	netMsgData;
			netMsgData.sOption.m_emOption = sSet.emTYPE;
			netMsgData.sOption.m_fMin = sSet.fLOW;
			netMsgData.sOption.m_fMax = sSet.fHIGH;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netMsgData);
		}
	}

	if ( RPARAM::bMaxRvPreviewRebuild )
	{
		SRANDOM_OPTION_SET_MAP_CITER it_b = pRandomGen->mapSETR.begin();
		SRANDOM_OPTION_SET_MAP_CITER it_e = pRandomGen->mapSETR.end();

		for( ; it_b != it_e; ++it_b )
		{
			const SRANDOM_OPTION_SET& sSet = (*it_b).second;

			GLMSG::SNETPC_REQ_MAXRV_PREVIEW_DATA_REBUILD	netMsgData;
			netMsgData.sOption.m_emOption = sSet.emTYPE;
			netMsgData.sOption.m_fMin = sSet.fLOW;
			netMsgData.sOption.m_fMax = sSet.fHIGH;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netMsgData);
		}
	}

	m_fMaxRVPreviewTimer = 0.0f;

	return S_OK;
}

//Item Card Extend Costume Expiration
HRESULT GLChar::MsgReqInvenDisguiseExtend ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNET_INVEN_DISGUISE_EXTEND *pNetMsg = (GLMSG::SNET_INVEN_DISGUISE_EXTEND *) nmg;

	WORD wPosX = pNetMsg->wPosX;
	WORD wPosY = pNetMsg->wPosY;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )														return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )															return E_FAIL;

	const SITEMCUSTOM& sITEM_HOLD = GET_SLOT_ITEM(SLOT_HOLD);
	if ( sITEM_HOLD.sNativeID==SNATIVEID(false) )							return S_FALSE;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )															return S_FALSE;

	if ( pHold->sBasicOp.emItemType != ITEM_CARD_COSTUME_EXTENDER )			return S_FALSE;

	if ( !pItem->IsTIMELMT() || !pItem->sBasicOp.IsDISGUISE() )				return S_FALSE;

	if ( pHold->sDrugOp.tTIME_LMT != 0 )
	{
		__time64_t &tTIME = pInvenItem->sItemCustom.tBORNTIME;
		tTIME += pHold->sDrugOp.tTIME_LMT;
	}

	GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
	NetItemUpdate.wPosX = pNetMsg->wPosX;
	NetItemUpdate.wPosY = pNetMsg->wPosY;
	NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemUpdate);

	GLITEMLMT::GetInstance().ReqItemConversion ( pInvenItem->sItemCustom, ID_CHAR, m_dwCharID );

	GLITEMLMT::GetInstance().ReqItemRoute ( GET_HOLD_ITEM(), ID_CHAR, m_dwCharID, ID_CHAR, 0, EMITEM_ROUTE_DELETE, GET_HOLD_ITEM().wTurnNum );

	RELEASE_SLOT_ITEM ( SLOT_HOLD );

	GLMSG::SNETPC_PUTON_RELEASE NetMsg_PutOn_Release(SLOT_HOLD);
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsg_PutOn_Release);

	return S_OK;
}

HRESULT GLChar::MsgReqInvenBattlePassPremiumCard ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_BATTLEPASS_PREMIUM_CARD* pNetMsg = ( GLMSG::SNETPC_REQ_BATTLEPASS_PREMIUM_CARD* ) nmg;
	GLMSG::SNETPC_REQ_BATTLEPASS_PREMIUM_CARD_FB NetMsgFB;

	SINVENITEM *pINVENITEM = m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_PREMIUM_CARD_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}


	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pITEM )
	{
		NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_PREMIUM_CARD_INVALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pITEM->sBasicOp.emItemType != ITEM_BATTLEPASS_PREMIUM_CARD )
	{
		NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_PREMIUM_CARD_NOTCARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( m_bBattlePassPremium )
	{
		NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_PREMIUM_CARD_ALREADY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	m_bBattlePassPremium = true;

	NetMsgFB.bPremium = m_bBattlePassPremium;
	NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_PREMIUM_CARD_SUCCESS;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}


HRESULT GLChar::MsgReqInvenBattlePassLevelUpCard ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_BATTLEPASS_LEVELUP_CARD* pNetMsg = ( GLMSG::SNETPC_REQ_BATTLEPASS_LEVELUP_CARD* ) nmg;
	GLMSG::SNETPC_REQ_BATTLEPASS_LEVELUP_CARD_FB NetMsgFB;

	SINVENITEM *pINVENITEM = m_cInventory.FindPosItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}


	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pITEM )
	{
		NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_INVALID;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if ( pITEM->sBasicOp.emItemType != ITEM_BATTLEPASS_LEVELUP_CARD )
	{
		NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_NOTCARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	/*if ( !m_bBattlePassPremium )
	{
	NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_NOTPREMIUM;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
	return E_FAIL;
	}*/

	if ( m_dwBattlePassLevel >= BATTLEPASS_LEVEL_MAX )
	{
		NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_MAX;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	if (m_dwBattlePassLevel < BATTLEPASS_LEVEL_MAX) 
	{
		DWORD dwNextLevelEXP = GLCONST_CHAR::dwBattlePassEXP[m_dwBattlePassLevel];
		DWORD dwEXPNeeded = dwNextLevelEXP - m_dwBattlePassEXP;

		m_dwBattlePassEXP += dwEXPNeeded;

		GLMSG::SNETPC_UPDATE_BATTLEPASS_EXP net_msg_client_update;
		net_msg_client_update.dwEXP = m_dwBattlePassEXP;
		net_msg_client_update.bNotice = TRUE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_update);

		if (m_dwBattlePassLevel < BATTLEPASS_LEVEL_MAX ) 
		{
			m_dwBattlePassLevel++;
			GLMSG::SNETPC_UPDATE_BATTLEPASS_LEVEL net_msg_client_level;
			net_msg_client_level.dwLevel = m_dwBattlePassLevel;
			net_msg_client_level.bNotice = TRUE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client_level);
		}
	}

	NetMsgFB.emFB = EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_SUCCESS;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	return S_OK;
}

HRESULT GLChar::MsgReqBattlePassRewardClaim( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_BATTLEPASS_REWARD_CLAIM* pNetMsg = ( GLMSG::SNETPC_REQ_BATTLEPASS_REWARD_CLAIM* ) nmg;
	GLMSG::SNETPC_REQ_BATTLEPASS_REWARD_CLAIM_FB netmsgfb;

	DWORD dwLevel = pNetMsg->dwLevel;
	bool bPremium = pNetMsg->bPremium;

	BATTLEPASS_LEVEL_REWARD_MAP_ITER rewardIter = GLBattlePassReward::GetInstance().m_mapBattlePassLevelRewardMap.find(dwLevel);
	if (rewardIter == GLBattlePassReward::GetInstance().m_mapBattlePassLevelRewardMap.end()) 
	{
		netmsgfb.emFB = EMINVEN_REQ_BATTLEPASS_REWARD_CLAIM_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
		return E_FAIL;
	}

	SBATTLEPASS_LEVEL_REWARD& sRewardData = rewardIter->second;

	if (bPremium && !m_bBattlePassPremium) 
	{
		netmsgfb.emFB = EMINVEN_REQ_BATTLEPASS_REWARD_CLAIM_NOTPREMIUM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
		return E_FAIL;
	}

	SNATIVEID sRewardID = bPremium ? sRewardData.sPremiumRewardID : sRewardData.sNormalRewardID;

	SITEM* pitem = GLItemMan::GetInstance().GetItem(sRewardID);
	if (!pitem)
	{
		netmsgfb.emFB = EMINVEN_REQ_BATTLEPASS_REWARD_CLAIM_NOTREWARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
		return E_FAIL;
	}

	SBATTLEPASS_REWARD_CHAR_DATA_MAP_ITER claimIter = m_mapBattlePassReward.find(dwLevel);
	if (claimIter != m_mapBattlePassReward.end()) 
	{
		SBATTLEPASS_REWARD_CHAR_DATA& claimData = claimIter->second;
		if ((bPremium && claimData.bPremiumClaimed) || (!bPremium && claimData.bNormalClaimed)) 
		{
			netmsgfb.emFB = EMINVEN_REQ_BATTLEPASS_REWARD_CLAIM_ALREADY;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
			return E_FAIL;
		}
	}

	WORD winvensizex = pitem->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem->sBasicOp.wInvenSizeY;

	BOOL bITEM_SPACE(FALSE);

	WORD wInsertPosX(0), wInsertPosY(0);
	bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );

	if ( !bITEM_SPACE )	
	{
		netmsgfb.emFB = EMINVEN_REQ_BATTLEPASS_REWARD_CLAIM_NOTINVEN;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	SITEMCUSTOM snew_item( pitem->sBasicOp.sNativeID );
	CTime cTIME = CTime::GetCurrentTime();
	snew_item.tBORNTIME = cTIME.GetTime();
	snew_item.wTurnNum = 1;
	snew_item.cGenType = (BYTE) EMGEN_SHOP;
	snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
	snew_item.cFieldID = (BYTE) GLGaeaServer::GetInstance().GetFieldSvrID();
	snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pitem->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );

	/*item color, Juver, 2018/01/08 */
	SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
	if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
	{
		snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
		snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
	}

	if( snew_item.GENERATE_RANDOM_OPT () )
	{
		GLITEMLMT::GetInstance().ReqRandomItem( snew_item );
	}

	BOOL bOK = m_cInventory.InsertItem ( snew_item, wInsertPosX, wInsertPosY );
	if ( !bOK )		return S_OK;

	GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );

	GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
	NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

	GLMSG::SNETPC_REQ_BATTLEPASS_REWARD_DATA NetMsgData;
	if (claimIter != m_mapBattlePassReward.end()) 
	{
		SBATTLEPASS_REWARD_CHAR_DATA& claimData = claimIter->second;
		if (bPremium) 
			claimData.bPremiumClaimed = true;
		else
			claimData.bNormalClaimed = true;

		NetMsgData.sRewardData.dwLevel = dwLevel;
		NetMsgData.sRewardData.bNormalClaimed = claimData.bNormalClaimed;
		NetMsgData.sRewardData.bPremiumClaimed = claimData.bPremiumClaimed;
	} 
	else
	{
		SBATTLEPASS_REWARD_CHAR_DATA claimData;
		claimData.dwLevel = dwLevel;
		if (bPremium)
			claimData.bPremiumClaimed = true;
		else
			claimData.bNormalClaimed = true;

		m_mapBattlePassReward.insert(std::make_pair(dwLevel, claimData));

		NetMsgData.sRewardData.dwLevel = dwLevel;
		NetMsgData.sRewardData.bNormalClaimed = claimData.bNormalClaimed;
		NetMsgData.sRewardData.bPremiumClaimed = claimData.bPremiumClaimed;
	}
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgData);

	netmsgfb.emFB = EMINVEN_REQ_BATTLEPASS_REWARD_CLAIM_SUCCESS;
	netmsgfb.sItemID = snew_item.sNativeID;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);

	return S_OK;
}
/*12-16-24, daily login - CNDev*/
HRESULT GLChar::MsgReqDailyLoginRewardClaim( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_DAILYLOGIN_REWARD_CLAIM* pNetMsg = ( GLMSG::SNETPC_REQ_DAILYLOGIN_REWARD_CLAIM* ) nmg;
	GLMSG::SNETPC_REQ_DAILYLOGIN_REWARD_CLAIM_FB netmsgfb;

	DWORD dwDay = pNetMsg->dwDay;

	DAILYLOGIN_REWARD_MAP_ITER rewardIter = GLDailyLogin::GetInstance().m_mapDailyLoginReward.find(dwDay);
	if (rewardIter == GLDailyLogin::GetInstance().m_mapDailyLoginReward.end())
	{
		netmsgfb.emFB = EMINVEN_REQ_DAILYLOGIN_REWARD_CLAIM_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
		return E_FAIL;
	}

	SITEM* pitem = GLItemMan::GetInstance().GetItem(rewardIter->second.sRewardID);
	if (!pitem)
	{
		netmsgfb.emFB = EMINVEN_REQ_DAILYLOGIN_REWARD_CLAIM_NOTREWARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
		return E_FAIL;
	}

	if (m_dwDayStreakCounter != dwDay)
	{
		netmsgfb.emFB = EMINVEN_REQ_DAILYLOGIN_REWARD_CLAIM_NOTDAY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
		return E_FAIL;
	}

	SDAILYLOGIN_CHAR_DATA_MAP_ITER claimIter = m_mapDailyLogin.find(dwDay);
	if (claimIter != m_mapDailyLogin.end())
	{
		SDAILYLOGIN_CHAR_DATA& claimData = claimIter->second;
		if (claimData.bRewardClaimed)
		{
			netmsgfb.emFB = EMINVEN_REQ_DAILYLOGIN_REWARD_CLAIM_ALREADY;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
			return E_FAIL;
		}
	}

	CTime cCurrentTime = CTime::GetCurrentTime();
	int nHour = cCurrentTime.GetHour();
	int nMinute = cCurrentTime.GetMinute();

	if (nHour == 23 || (nHour == 0 && nMinute == 0))
	{
		netmsgfb.emFB = EMINVEN_REQ_DAILYLOGIN_REWARD_CLAIM_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &netmsgfb);
		return E_FAIL;
	}


	WORD winvensizex = pitem->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem->sBasicOp.wInvenSizeY;

	BOOL bITEM_SPACE(FALSE);

	WORD wInsertPosX(0), wInsertPosY(0);
	bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );

	if ( !bITEM_SPACE )	
	{
		netmsgfb.emFB = EMINVEN_REQ_DAILYLOGIN_REWARD_CLAIM_NOTINVEN;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);
		return E_FAIL;
	}

	SITEMCUSTOM snew_item( pitem->sBasicOp.sNativeID );
	CTime cTIME = CTime::GetCurrentTime();
	snew_item.tBORNTIME = cTIME.GetTime();
	snew_item.wTurnNum = 1;
	snew_item.cGenType = (BYTE) EMGEN_SHOP;
	snew_item.cChnID = (BYTE)GLGaeaServer::GetInstance().GetServerChannel();
	snew_item.cFieldID = (BYTE) GLGaeaServer::GetInstance().GetFieldSvrID();
	snew_item.lnGenNum = GLITEMLMT::GetInstance().RegItemGen ( pitem->sBasicOp.sNativeID, (EMITEMGEN)snew_item.cGenType );

	/*item color, Juver, 2018/01/08 */
	SITEM* pitem_data_check = GLItemMan::GetInstance().GetItem( snew_item.sNativeID );
	if ( pitem_data_check && pitem_data_check->sBasicOp.bItemColor )
	{
		snew_item.wColor1 = pitem_data_check->sBasicOp.wItemColor1;
		snew_item.wColor2 = pitem_data_check->sBasicOp.wItemColor2;
	}

	if( snew_item.GENERATE_RANDOM_OPT () )
	{
		GLITEMLMT::GetInstance().ReqRandomItem( snew_item );
	}

	BOOL bOK = m_cInventory.InsertItem ( snew_item, wInsertPosX, wInsertPosY );
	if ( !bOK )		return S_OK;

	GLITEMLMT::GetInstance().ReqItemRoute ( snew_item, ID_CHAR, 0, ID_CHAR, m_dwCharID, EMITEM_ROUTE_CHAR, snew_item.wTurnNum );

	GLMSG::SNETPC_INVEN_INSERT NetMsgInven;
	NetMsgInven.Data = *m_cInventory.GetItem ( wInsertPosX, wInsertPosY );
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgInven);

	GLMSG::SNETPC_DAILYLOGIN_COMPLETE NetMsgData;
	if (claimIter != m_mapDailyLogin.end()) 
	{
		SDAILYLOGIN_CHAR_DATA& claimData = claimIter->second;
		claimData.bRewardClaimed = true;

		NetMsgData.sData.dwDay = dwDay;
		NetMsgData.sData.bRewardClaimed = claimData.bRewardClaimed;
	} 
	else
	{
		SDAILYLOGIN_CHAR_DATA claimData;
		claimData.dwDay = dwDay;
		claimData.bRewardClaimed = true;

		m_mapDailyLogin.insert(std::make_pair(dwDay, claimData));

		NetMsgData.sData.dwDay = dwDay;
		NetMsgData.sData.bRewardClaimed = claimData.bRewardClaimed;
	}
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgData);

	netmsgfb.emFB = EMINVEN_REQ_DAILYLOGIN_REWARD_CLAIM_SUCCESS;
	netmsgfb.sItemID = snew_item.sNativeID;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&netmsgfb);

	return S_OK;
}