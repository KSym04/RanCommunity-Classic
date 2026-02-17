#include "stdafx.h"

#include "./UserTypeDefine.h"

#include "../DxGlobalStage.h"

#include "./GLCharacter.h"
#include "./GLItemMan.h"
#include "./GLItemMixMan.h"
#include "./GLActivity.h"
#include "./GLCrowData.h"

#include "./RTimerDefine.h"
#include "../../Lib_Engine/Utils/RanFilter.h"

#include "../../Lib_ClientUI/Interface/UITextControl.h"
#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/ModalWindow.h"
#include "../../Lib_ClientUI/Interface/ModalCallerID.h"

#include "../../Lib_ClientUI/Interface/QBoxButton.h"

#include "../../Lib_ClientUI/Interface/BattleRankingDisplay.h"

#include "../../Lib_Engine/Common/StringUtils.h"

#include "./GLGaeaClient.h"

#include "./GLPartyFinder.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSROption.h"
#include "../../Lib_Engine/Core/NSRProfile.h"
#include "../../Lib_Engine/Core/NSRGameGlobal.h"

#include "../../Lib_Engine/Core/NSREngineGlobal.h"

/* chaos machine, Juver, 2021/07/09 */
#include "./GLChaosMachine.h"

/* set item option, Juver, 2021/09/04 */
#include "./GLSetOption.h"

#include "./GLContributionShop.h"


/*12-9-14, Battle Pass - CNDev*/
#include "./GLBattlePassReward.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*itemfood system, Juver, 2017/05/25 */
HRESULT GLCharacter::ReqConsumeFood ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pINVENITEM = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pINVENITEM )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_NOITEM") );
		return E_FAIL;
	}

	wPosX = pINVENITEM->wPosX;
	wPosY = pINVENITEM->wPosY;

	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pITEM )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_NOITEM") );
		return E_FAIL;
	}

	if ( pITEM->sBasicOp.emItemType != ITEM_FOOD )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_NOITEM") );
		return E_FAIL;
	}

	PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( pITEM->sSkillBookOp.sSkill_ID );
	if ( !pSKILL )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_NONDATA") );
		return E_FAIL;
	}

	GLMSG::SNET_INVEN_CONSUME_FOOD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

/*itemfood system, Juver, 2017/05/26 */
HRESULT GLCharacter::ReqUnlockFoodSlot ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pINVENITEM = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pINVENITEM )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMUNLOCK_FOOD_FB_NOITEM") );
		return E_FAIL;
	}

	wPosX = pINVENITEM->wPosX;
	wPosY = pINVENITEM->wPosY;

	SITEM* pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pITEM )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMUNLOCK_FOOD_FB_NOITEM") );
		return E_FAIL;
	}

	if ( pITEM->sBasicOp.emItemType != ITEM_CARD_FOODUNLOCK )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMUNLOCK_FOOD_FB_NOITEM") );
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
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMUNLOCK_FOOD_FB_UNLOCKED") );
		return E_FAIL;
	}

	GLMSG::SNET_INVEN_UNLOCK_FOOD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

/*rightclick wear/unwear, Juver, 2017/06/23 */
HRESULT GLCharacter::ReqInvenToWear ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );

	if ( !pInvenItem )						return E_FAIL;
	if ( m_sCONFTING.IsCONFRONTING() )		return E_FAIL;
	if ( ValidWindowOpen() )				return E_FAIL;	
	if ( VALID_HOLD_ITEM() )				return E_FAIL;
	if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL) )	return E_FAIL;

	SITEM* pITEM = GLItemMan::GetInstance().GetItem( pInvenItem->sItemCustom.sNativeID );
	if ( !pITEM )		return E_FAIL;

	//check accept item
	if( pInvenItem->sItemCustom.nidDISGUISE!=SNATIVEID(false) )
	{
		if ( !ACCEPT_ITEM ( pInvenItem->sItemCustom.sNativeID, pInvenItem->sItemCustom.nidDISGUISE ) )
			return E_FAIL;
	}
	else
	{
		if ( !ACCEPT_ITEM ( pInvenItem->sItemCustom.sNativeID ) )
			return E_FAIL;
	}

	if ( m_fWearDelay < 1.0f )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("INVENTOWEAR_REQUIRE_TIME") );
		return E_FAIL;
	}

	//find compatible slot
	EMSLOT emUseSlot = SLOT_TSIZE;

	switch( pITEM->sSuitOp.emSuit )
	{
	case SUIT_HEADGEAR:	emUseSlot = SLOT_HEADGEAR;	break;
	case SUIT_UPPER:	emUseSlot = SLOT_UPPER;		break;
	case SUIT_LOWER:	emUseSlot = SLOT_LOWER;		break;
	case SUIT_HAND:		emUseSlot = SLOT_HAND;		break;
	case SUIT_FOOT:		emUseSlot = SLOT_FOOT;		break;

	case SUIT_HANDHELD:
		{
			if ( pITEM->sBasicOp.emItemType == ITEM_SUIT )
				emUseSlot = GetCurRHand();
			else emUseSlot = GetCurLHand();
		}break;

	case SUIT_NECK:		emUseSlot = SLOT_NECK;		break;
	case SUIT_WRIST:	emUseSlot = SLOT_WRIST;		break;

	case SUIT_FINGER:	
		emUseSlot = m_bRingSlotFirst? SLOT_LFINGER:SLOT_RFINGER;		
		break;

		/*ep8 puton slots, Juver, 2018/03/29 */
	case SUIT_BELT:			emUseSlot = SLOT_BELT;			break;
	case SUIT_EARRING:		emUseSlot = SLOT_EARRING;		break;
	case SUIT_DECORATION:	emUseSlot = SLOT_DECORATION;	break;
	case SUIT_WING:			emUseSlot = SLOT_WING;			break;

	case SUIT_ACCESSORY:	
		emUseSlot = m_bAccessorySlotFirst? SLOT_ACCESSORY_L:SLOT_ACCESSORY_R;		
		break;
	};

	if ( emUseSlot == SLOT_TSIZE )	return E_FAIL;

	if ( !CHECKSLOT_ITEM ( pITEM->sBasicOp.sNativeID, emUseSlot ) )	return E_FAIL;

	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_EQUIP ) && !( emUseSlot == SLOT_LHAND || emUseSlot == SLOT_LHAND_S ) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PERSONAL_LOCK_MSG_PUTON") );
		return E_FAIL;
	}

	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_INVEN ) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PERSONAL_LOCK_MSG_INVEN") );
		return E_FAIL;
	}

	//check inven free space
	if ( pITEM->sSuitOp.emSuit==SUIT_HANDHELD && pITEM->sSuitOp.IsBOTHHAND() )
	{
		EMSLOT emRHand = GetCurRHand();
		EMSLOT emLHand = GetCurLHand();

		if ( VALID_SLOT_ITEM(emLHand) && VALID_SLOT_ITEM(emRHand) )
		{
			SITEM *pItemLeft = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emLHand).sNativeID);

			WORD wInvenPosX, wInvenPosY;
			BOOL bOk = m_cInventory.FindInsrtable ( pItemLeft->sBasicOp.wInvenSizeX, pItemLeft->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
			if ( !bOk )
			{
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("INVENTOWEAR_NO_SPACE_LEFTITEM") );
				return E_FAIL;
			}
		}
	}

	m_bRingSlotFirst = !m_bRingSlotFirst;

	/*ep8 puton slots, Juver, 2018/03/29 */
	m_bAccessorySlotFirst = !m_bAccessorySlotFirst;

	GLMSG::SNETPC_REQ_INVEN_TO_WEAR NetMsg;
	NetMsg.wPosX = pInvenItem->wPosX;
	NetMsg.wPosY = pInvenItem->wPosY;
	NetMsg.emSlot = emUseSlot;

	NETSENDTOFIELD ( &NetMsg );

	m_fWearDelay = 0.0f;

	return S_OK;
}

/*rightclick wear/unwear, Juver, 2017/06/24 */
HRESULT GLCharacter::ReqWearToInven ( EMSLOT emSLOT )
{
	if ( !VALID_SLOT_ITEM(emSLOT) )	return E_FAIL;
	if ( ValidWindowOpen() )		return E_FAIL;	

	if ( IsACTION(GLAT_ATTACK) || IsACTION(GLAT_SKILL) )
		return E_FAIL;

	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_EQUIP ) && !( emSLOT == SLOT_LHAND || emSLOT == SLOT_LHAND_S ) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PERSONAL_LOCK_MSG_PUTON") );
		return E_FAIL;
	}

	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_INVEN ) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PERSONAL_LOCK_MSG_INVEN") );
		return E_FAIL;
	}

	SITEM *pItemSlot = GLItemMan::GetInstance().GetItem(GET_SLOT_ITEM(emSLOT).sNativeID);
	if ( !pItemSlot )	return E_FAIL;

	WORD wInvenPosX, wInvenPosY;
	BOOL bOk = m_cInventory.FindInsrtable ( pItemSlot->sBasicOp.wInvenSizeX, pItemSlot->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
	if ( !bOk )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("WEARTOINVEN_NO_SPACE") );
		return E_FAIL;
	}

	GLMSG::SNETPC_REQ_WEAR_TO_INVEN NetMsg;
	NetMsg.emSlot = emSLOT;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

/*npc shop, Juver, 2017/07/27 */
HRESULT GLCharacter::ReqNpcShopPurchaseReset()
{
	m_sNPCShopCrow = NATIVEID_NULL();
	m_sNPCShopItem = NATIVEID_NULL();
	m_wNPCShopType = 0;

	return S_OK;
}

/*npc shop, Juver, 2017/07/27 */
HRESULT GLCharacter::ReqNpcShopPurchase( SNATIVEID sidCrow, SNATIVEID sidItem, WORD wShopType )
{
	if ( m_fNPCShopDelay < NPC_SHOP_DELAY )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_DELAY") );
		return E_FAIL;
	}

	SITEM* pitem = GLItemMan::GetInstance().GetItem ( sidItem );
	if ( !pitem )	
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_INVALID_ITEM") );
		return E_FAIL;
	}

	SCROWDATA*	pcrow = GLCrowDataMan::GetInstance().GetCrowData( sidCrow );
	if ( !pcrow )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_INVALID_CROW") );
		return E_FAIL;
	}

	SNPC_SHOP_DATA* pNpcShop = GLCrowDataMan::GetInstance().NPCShopFind( pcrow->m_sAction.m_strShopFile );
	if ( !pNpcShop )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_INVALID_SHOP") );
		return E_FAIL;
	}

	if ( pNpcShop->m_wShopType != wShopType )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_INVALID_SHOPTYPE") );
		return E_FAIL;
	}

	const SNPC_SHOP_ITEM* pshopitem = pNpcShop->FindShopItem( sidItem.dwID );
	if ( !pshopitem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_INVALID_SHOPITEM") );
		return E_FAIL;
	}

	//player money check
	LONGLONG dwPrice = pitem->sBasicOp.dwBuyPrice;
	if ( m_lnMoney < (LONGLONG)dwPrice )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_NOT_MONEY") );
		return E_FAIL;
	}

	/*contribution point, Juver, 2017/08/23 */
	if ( pitem->sBasicOp.dwReqContributionPoint != 0 &&
		m_dwContributionPoint < pitem->sBasicOp.dwReqContributionPoint )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_NOCONTRIBUTIONPOINT") );
		return E_FAIL;
	}

	/*activity point, Juver, 2017/08/23 */
	if ( pitem->sBasicOp.dwReqActivityPoint != 0 && 
		m_dwActivityPoint < pitem->sBasicOp.dwReqActivityPoint )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_NOPOINT") );
		return E_FAIL;
	}

	/* play time system, Juver, 2021/01/27 */
	if ( pitem->sBasicOp.llPlayTimeReq != 0 && m_llPlayTime < pitem->sBasicOp.llPlayTimeReq )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_PLAY_TIME") );
		return E_FAIL;
	}

	WORD winvensizex = pitem->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem->sBasicOp.wInvenSizeY;

	//inventory space check
	BOOL bITEM_SPACE(FALSE);
	if ( pitem->ISPILE() )
	{	
		WORD wPILENUM = pitem->sDrugOp.wPileNum;
		WORD wREQINSRTNUM = ( 1 * pitem->GETAPPLYNUM() );
		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sidItem, wPILENUM, winvensizex, winvensizey );
	}else{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );
	}

	if ( !bITEM_SPACE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_NOT_INVEN_SPACE") );
		return E_FAIL;
	}


	m_sNPCShopCrow = sidCrow;
	m_sNPCShopItem = sidItem;
	m_wNPCShopType = wShopType;

	if ( RGAME_GLOBAL::bNPCShopConfirm )
	{
		CString strQuestion = CInnerInterface::GetInstance().MakeString( ID2GAMEINTEXT("NPCSHOP_PURCHASE_QUESTION"), pitem->GetName() );
		DoModal( strQuestion.GetString(),  MODAL_INFOMATION, YESNO, MODAL_NPCSHOP_PURCHASE );		
	}
	else
	{
		ReqNpcShopDoPurchase();
	}
	
	return S_OK;
}

/*npc shop, Juver, 2017/07/27 */
HRESULT GLCharacter::ReqNpcShopDoPurchase()
{
	if ( m_wNPCShopType == 0 )
	{
		GLMSG::SNETPC_REQ_NPCSHOP_PURCHASE_MONEY NetMsg;
		NetMsg.sidCrow = m_sNPCShopCrow;
		NetMsg.sidItem = m_sNPCShopItem;
		NetMsg.wShopType = m_wNPCShopType;
		NETSENDTOFIELD ( &NetMsg );
	}

	m_fNPCShopDelay = 0.0f;

	return S_OK;
}

/*vehicle booster system, Juver, 2017/08/10 */
void GLCharacter::ReqVehicleEnableBooster( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem ) return;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) return;

	if ( pItem->sBasicOp.emItemType != ITEM_VEHICLE )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM") );
		return;
	}

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold ) return;

	if ( pHold->sBasicOp.emItemType != ITEM_CARD_BIKEBOOST )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD") );
		return;
	}

	if ( pInvenItem->sItemCustom.dwVehicleID == 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM") );
		return;
	}

	if ( pHold->sVehicle.emVehicleType != pItem->sVehicle.emVehicleType )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_TYPE") );
		return;
	}

	if ( pHold->sVehicle.emVehicleSubType != pItem->sVehicle.emVehicleSubType )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_TYPE") );
		return;
	}

	SVEHICLEITEMINFO sVehicle;

	if ( !DxGlobalStage::GetInstance().IsEmulator() )
	{
		VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pInvenItem->sItemCustom.dwVehicleID );
		if ( iter==m_mapVEHICLEItemInfo.end() ) return;
		sVehicle = (*iter).second;
	}
	else
	{
		sVehicle.m_bBooster = m_sVehicle.m_bBooster;
	}

	if ( sVehicle.m_bBooster ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_ALREADY_ENABLED") );
		return;
	}

	GLMSG::SNET_VEHICLE_REQ_ENABLE_BOOSTER NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;

	NETSENDTOFIELD ( &NetMsg );
}

/*item exchange, Juver, 2017/10/13 */
HRESULT GLCharacter::ReqNPCItemExchange( SNATIVEID sidNPC, WORD wExchangeID )
{
	if ( VALID_HOLD_ITEM() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_INVALID_ITEM") );
		return E_FAIL;
	}

	if ( m_fNPCItemExchangeDelay < NPC_ITEM_EXCHANGE_DELAY )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_DELAY") );
		return E_FAIL;
	}

	PCROWDATA pcrow_data = GLCrowDataMan::GetInstance().GetCrowData( sidNPC );
	if ( !pcrow_data )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_INVALID_NPC") );
		return E_FAIL;
	}

	SNPC_ITEM_EXCHANGE* pItemExchange = GLCrowDataMan::GetInstance().NPCItemExchangeFind( pcrow_data->m_sAction.m_strItemExchangeFile );
	if ( !pItemExchange )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_INVALID_DATA") );
		return E_FAIL;
	}

	const SNPC_ITEM_EXCHANGE_DATA* pnpc_item_exchange_data = pItemExchange->FindData( wExchangeID );
	if ( !pnpc_item_exchange_data )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_INVALID_ID") );
		return E_FAIL;
	}

	SITEM* pitem_data_result = GLItemMan::GetInstance().GetItem( pnpc_item_exchange_data->sidItemResult.sidItem );
	if ( !pitem_data_result )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_INVALID_ITEM") );
		return E_FAIL;
	}

	/*contribution point, Juver, 2017/08/23 */
	if ( pitem_data_result->sBasicOp.dwReqContributionPoint != 0 &&
		m_dwContributionPoint < pitem_data_result->sBasicOp.dwReqContributionPoint )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_NOCONTRIBUTIONPOINT") );
		return E_FAIL;
	}

	/*activity point, Juver, 2017/08/23 */
	if ( pitem_data_result->sBasicOp.dwReqActivityPoint != 0 && 
		m_dwActivityPoint < pitem_data_result->sBasicOp.dwReqActivityPoint )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_NOPOINT") );
		return E_FAIL;
	}

	/* play time system, Juver, 2021/01/27 */
	if ( pitem_data_result->sBasicOp.llPlayTimeReq != 0 && m_llPlayTime < pitem_data_result->sBasicOp.llPlayTimeReq )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_NO_PLAY_TIME") );
		return E_FAIL;
	}

	WORD winvensizex = pitem_data_result->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem_data_result->sBasicOp.wInvenSizeY;

	//inventory space check
	BOOL bITEM_SPACE(FALSE);
	if ( pitem_data_result->ISPILE() )
	{	
		WORD wPILENUM = pitem_data_result->sDrugOp.wPileNum;
		WORD wREQINSRTNUM = ( pnpc_item_exchange_data->sidItemResult.wItemNum * pitem_data_result->GETAPPLYNUM() );
		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, pnpc_item_exchange_data->sidItemResult.sidItem, wPILENUM, winvensizex, winvensizey );
	}else{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );
	}

	if ( !bITEM_SPACE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_NOT_INVEN_SPACE") );
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
				DWORD dwNum = pnpc_item_exchange_data->sidItemRequire[i].wItemNum;
				DWORD dwItemNum = m_cInventory.CountTurnItem( pitem_data_require->sBasicOp.sNativeID );
				if ( dwItemNum < dwNum )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_REQUIRE_MISSING") );
					return E_FAIL;
				}
			}
			else
			{
				//not pile just check if exist
				SINVENITEM* pinvenitem = m_cInventory.FindItem( pitem_data_require->sBasicOp.sNativeID );
				if ( !pinvenitem )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_REQUIRE_MISSING") );
					return E_FAIL;
				}
			}
		}
	}
	
	m_fNPCItemExchangeDelay = 0.0f;

	GLMSG::SNETPC_REQ_NPC_ITEM_EXCHANGE_TRADE NetMsg;
	NetMsg.sid_NPC = sidNPC;
	NetMsg.wTradeID = wExchangeID;;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

/*product item, Juver, 2017/10/17 */
bool GLCharacter::ValidItemCompoundOpen()
{
	if ( CInnerInterface::GetInstance().IsVisibleGroup( PRODUCT_WINDOW ) )
		return true;
	return false;
}

/*product item, Juver, 2017/10/18 */
void GLCharacter::ItemCompound( DWORD dwID )
{
	m_dwProductID = dwID;

	DoModal( ID2GAMEINTEXT("RAN_PPRODUCT_BUTTON_PRODUCE_QUESTION"),  MODAL_INFOMATION, YESNO, MODAL_ITEM_PRODUCT_PRODUCE );		
}

/*product item, Juver, 2017/10/17 */
HRESULT GLCharacter::ReqItemCompound( DWORD dwID )
{
	if ( m_bItemCompoundTask )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_TASK_RUNNING") );
		return E_FAIL;
	}

	const ITEM_MIX* pitem_mix = GLItemMixMan::GetInstance().GetProduct( dwID );
	if ( !pitem_mix )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_INVALID_ID") );
		return E_FAIL;
	}

	if ( VALID_HOLD_ITEM() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_RESULT_ITEM_INVALID") );
		return E_FAIL;
	}

	//money check
	if( m_lnMoney < pitem_mix->dwPrice )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_INVALID_COST") );
		return E_FAIL;
	}

	//level check
	if ( m_wLevel < pitem_mix->wLevelReq )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_INVALID_CHAR_LEVEL") );
		return E_FAIL;
	}

	/* play time system, Juver, 2021/01/27 */
	if ( m_llPlayTime < pitem_mix->llPlayTimeReq )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_INVALID_CHAR_PLAY_TIME") );
		return E_FAIL;
	}

	/* play time system, Juver, 2021/01/27 */
	if ( m_llPlayPoint < pitem_mix->llPlayPointUse )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_INVALID_CHAR_PLAY_POINT") );
		return E_FAIL;
	}

	if ( m_dwContributionPoint < pitem_mix->dwContriUse )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_INVALID_CHAR_CONTRI_POINT") );
		return E_FAIL;
	}

	//result item check
	SITEM* pitem_data_result = GLItemMan::GetInstance().GetItem( pitem_mix->sResultItem.sNID );
	if ( !pitem_data_result )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_RESULT_ITEM_INVALID") );
		return E_FAIL;
	}

	WORD winvensizex = pitem_data_result->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem_data_result->sBasicOp.wInvenSizeY;

	//inventory space check
	BOOL bITEM_SPACE(FALSE);
	if ( pitem_data_result->ISPILE() )
	{	
		WORD wPILENUM = pitem_data_result->sDrugOp.wPileNum;
		WORD wREQINSRTNUM = ( pitem_mix->sResultItem.nNum * pitem_data_result->GETAPPLYNUM() );
		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, pitem_mix->sResultItem.sNID, wPILENUM, winvensizex, winvensizey );
	}else{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );
	}

	if ( !bITEM_SPACE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_RESULT_ITEM_NO_SPACE") );
		return E_FAIL;
	}

	//requirements check
	for ( int i=0; i<ITEMMIX_ITEMNUM; ++i )
	{	
		SITEM* pitem_data_require = GLItemMan::GetInstance().GetItem( pitem_mix->sMeterialItem[i].sNID );
		if ( pitem_data_require )
		{
			if ( pitem_data_require->ISPILE() )
			{
				DWORD dwNum = pitem_mix->sMeterialItem[i].nNum;
				DWORD dwItemNum = m_cInventory.CountTurnItem( pitem_data_require->sBasicOp.sNativeID );
				if ( dwItemNum < dwNum )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_MATERIAL") );
					return E_FAIL;
				}
			}
			else
			{
				//not pile just check if exist
				SINVENITEM* pinvenitem = m_cInventory.FindItem( pitem_data_require->sBasicOp.sNativeID );
				if ( !pinvenitem )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_MATERIAL") );
					return E_FAIL;
				}

				if ( pinvenitem->sItemCustom.nidDISGUISE !=SNATIVEID(false) )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_FAIL_COSTUME") );
					return E_FAIL;
				}
			}
		}
	}

	GLMSG::SNETPC_REQ_ITEM_COMPOUND_START NetMsg;
	NetMsg.dwProductID = dwID;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

/*activity system, Juver, 2017/11/05 */
void GLCharacter::ReqChangeBadge( DWORD dwBadgeID )
{
	if ( m_fCharTitleDelay < CHARACTER_BADGE_TIME )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAR_TITLE_DELAY") );
		return;
	}

	SACTIVITY_FILE_DATA* pactivity_file_data = GLActivity::GetInstance().GetActivity( dwBadgeID );
	if ( pactivity_file_data )
	{
		if ( !pactivity_file_data->bRewardBadge )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAR_TITLE_FAIL") );
			return;
		}

		if ( strcmp(m_szBadge, pactivity_file_data->strBadgeString.c_str() ) == 0 )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAR_TITLE_SAME") );
			return;
		}
	}

	m_fCharTitleDelay = 0.0f;

	GLMSG::SNETPC_REQ_CHARACTER_BADGE_CHANGE NetMsg;
	NetMsg.dwBadgeID = dwBadgeID;
	NETSENDTOFIELD ( &NetMsg );
}

/*charinfoview , Juver, 2017/11/11 */
void GLCharacter::RequestCharacterInfoGaea( DWORD dwGaeaID )
{
	if ( !RPARAM::bViewCharInfo )	return;

	if ( m_fReqCharInfoTimer < RPARAM::fViewCharInfoDelay )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_SHOW_CHARACTER_TIMEDELAY") );
		return;
	}

	m_fReqCharInfoTimer = 0.0f;

	GLMSG::SNETPC_REQ_CHARINFO_GAEA NetMsg;
	NetMsg.dwGaeaID = dwGaeaID;
	NETSEND ( &NetMsg );
}

/* charinfoview, Juver, 2020/03/03 */
void GLCharacter::RequestCharacterInfoCharID( DWORD dwCharID )
{
	if ( !RPARAM::bViewCharInfo )	return;

	if ( m_fReqCharInfoTimer < RPARAM::fViewCharInfoDelay )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_SHOW_CHARACTER_TIMEDELAY") );
		return;
	}

	m_fReqCharInfoTimer = 0.0f;

	GLMSG::SNETPC_REQ_CHARINFO_CHAR_ID NetMsg;
	NetMsg.dwCharID = dwCharID;
	NETSEND ( &NetMsg );
}

/* charinfoview, Juver, 2020/03/03 */
void GLCharacter::RequestCharacterInfoCharName( const char* szName )
{
	if ( !RPARAM::bViewCharInfo )	return;
	if ( !szName )					return;
	if ( strlen( szName) == 0 )		return;

	if ( m_fReqCharInfoTimer < RPARAM::fViewCharInfoDelay )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_SHOW_CHARACTER_TIMEDELAY") );
		return;
	}

	m_fReqCharInfoTimer = 0.0f;

	GLMSG::SNETPC_REQ_CHARINFO_CHAR_NAME NetMsg;
	StringCchCopy( NetMsg.szCharName, CHAR_SZNAME, szName );
	NETSEND( &NetMsg );
}

/*bike color , Juver, 2017/11/12 */
void GLCharacter::ReqVehicleColor( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_CARD_BIKECOLOR )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_CARD") );
		return;
	}

	if ( !m_bVehicle )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_VEHICLE") );
		return;
	}

	SITEM* pVehicle = GET_SLOT_ITEMDATA( SLOT_VEHICLE );
	if ( !pVehicle )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_VEHICLE") );
		return;
	}

	if ( !pVehicle->sVehicle.bUseColor )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_VEHICLE_COLOR") );
		return;
	}

	if ( pVehicle->sVehicle.emVehicleType != VEHICLE_TYPE_BIKE )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_BIKE") );
		return;
	}

	for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
		m_wBikeColor[i] = m_sVehicle.m_wColor[i];

	m_bEnableBikeColor = TRUE;

	CInnerInterface::GetInstance().ShowGroupFocus( BIKE_COLOR_WINDOW );
}

/*bike color , Juver, 2017/11/13 */
void GLCharacter::ReqBikeColorChange()
{
	SINVENITEM*	 pInvenItem = m_cInventory.FindItem( ITEM_CARD_BIKECOLOR );
	if ( !pInvenItem )	return;

	if ( !m_bVehicle )	return;
	if ( !m_bEnableBikeColor )	return;

	GLMSG::SNET_VEHICLE_REQ_CHANGE_COLOR NetMsg;
	for( int i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
		NetMsg.wColor[i] = m_wBikeColor[i];

	NETSENDTOFIELD ( &NetMsg );
}


/*rv card, Juver, 2017/11/25 */
HRESULT GLCharacter::ReqRandomOptionChange ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;

	SINVENITEM* ptarget_inven_item = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !ptarget_inven_item )	
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_INVALID_TARGET_ITEM") );
		return S_FALSE;
	}

	wPosX = ptarget_inven_item->wPosX;
	wPosY = ptarget_inven_item->wPosY;

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( ptarget_inven_item->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_INVALID_TARGET_ITEM") );
		return S_FALSE;
	}

	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pitem_data_hold )	return S_FALSE;

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_RANDOM_OPTION_CARD )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_INVALID_CARD_ITEM") );
		return S_FALSE;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{	
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_NOT_SUIT") );
		return S_FALSE;
	}

	if ( pitem_data_target->sBasicOp.bUseRVCard == FALSE )
	{	
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_INVALID") );
		return S_FALSE;
	}

	if ( !pitem_data_hold->sRvCard.bUseSuit[pitem_data_target->sSuitOp.emSuit] )
	{	
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_SUIT_MISMATCH") );
		return S_FALSE;
	}
	
	if ( pitem_data_hold->sRvCard.bReplaceOpt )
	{
		if ( ptarget_inven_item->sItemCustom.cOptTYPE1 == (BYTE) pitem_data_hold->sRvCard.emOption 
			&& ptarget_inven_item->sItemCustom.nOptVALUE1 >= (short) pitem_data_hold->sRvCard.wValue )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET") );
			return S_FALSE;
		}

		if ( ptarget_inven_item->sItemCustom.cOptTYPE2 == (BYTE) pitem_data_hold->sRvCard.emOption 
			&& ptarget_inven_item->sItemCustom.nOptVALUE2 >= (short) pitem_data_hold->sRvCard.wValue )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET") );
			return S_FALSE;
		}

		if ( ptarget_inven_item->sItemCustom.cOptTYPE3 == (BYTE) pitem_data_hold->sRvCard.emOption 
			&& ptarget_inven_item->sItemCustom.nOptVALUE3 >= (short) pitem_data_hold->sRvCard.wValue )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET") );
			return S_FALSE;
		}

		if ( ptarget_inven_item->sItemCustom.cOptTYPE4 == (BYTE) pitem_data_hold->sRvCard.emOption 
			&& ptarget_inven_item->sItemCustom.nOptVALUE4 >= (short) pitem_data_hold->sRvCard.wValue )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET") );
			return S_FALSE;
		}
	}


	if ( pitem_data_hold->sRvCard.bCheckExist )
	{
		BOOL bfound_option = FALSE;

		if ( ptarget_inven_item->sItemCustom.cOptTYPE1 == (BYTE) pitem_data_hold->sRvCard.emOption )	bfound_option = TRUE;
		if ( ptarget_inven_item->sItemCustom.cOptTYPE2 == (BYTE) pitem_data_hold->sRvCard.emOption )	bfound_option = TRUE;
		if ( ptarget_inven_item->sItemCustom.cOptTYPE3 == (BYTE) pitem_data_hold->sRvCard.emOption )	bfound_option = TRUE;
		if ( ptarget_inven_item->sItemCustom.cOptTYPE4 == (BYTE) pitem_data_hold->sRvCard.emOption )	bfound_option = TRUE;

		if ( !bfound_option )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_NOT_EXIST") );
			return S_FALSE;
		}
	}
	else
	{
		BOOL bfound_free = FALSE;

		if ( ptarget_inven_item->sItemCustom.cOptTYPE1 == EMR_OPT_NULL )	bfound_free = TRUE;
		if ( ptarget_inven_item->sItemCustom.cOptTYPE2 == EMR_OPT_NULL )	bfound_free = TRUE;
		if ( ptarget_inven_item->sItemCustom.cOptTYPE3 == EMR_OPT_NULL )	bfound_free = TRUE;
		if ( ptarget_inven_item->sItemCustom.cOptTYPE4 == EMR_OPT_NULL )	bfound_free = TRUE;

		if ( ptarget_inven_item->sItemCustom.cOptTYPE1 == (BYTE) pitem_data_hold->sRvCard.emOption )	bfound_free = TRUE;
		if ( ptarget_inven_item->sItemCustom.cOptTYPE2 == (BYTE) pitem_data_hold->sRvCard.emOption )	bfound_free = TRUE;
		if ( ptarget_inven_item->sItemCustom.cOptTYPE3 == (BYTE) pitem_data_hold->sRvCard.emOption )	bfound_free = TRUE;
		if ( ptarget_inven_item->sItemCustom.cOptTYPE4 == (BYTE) pitem_data_hold->sRvCard.emOption )	bfound_free = TRUE;

		if ( !bfound_free )
		{
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_NOT_FREE") );
			return S_FALSE;
		}
	}


	GLMSG::SNET_INVEN_RANDOM_OPTION_CHANGE net_msg_field;
	net_msg_field.wPosX = wPosX;
	net_msg_field.wPosY = wPosY;

	NETSENDTOFIELD ( &net_msg_field );

	return S_OK;
}

/*nondrop card, Juver, 2017/11/26 */
HRESULT GLCharacter::ReqItemNonDropCard ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;

	SINVENITEM* ptarget_inven_item = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !ptarget_inven_item )	
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_NONDROP_CARD_INVALID_TARGET_ITEM") );
		return S_FALSE;
	}

	wPosX = ptarget_inven_item->wPosX;
	wPosY = ptarget_inven_item->wPosY;

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( ptarget_inven_item->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_NONDROP_CARD_INVALID_TARGET_ITEM") );
		return S_FALSE;
	}

	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pitem_data_hold )	return S_FALSE;

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_NONDROP_CARD )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_NONDROP_CARD_INVALID_CARD_ITEM") );
		return S_FALSE;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{	
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_NONDROP_CARD_TARGET_NOT_SUIT") );
		return S_FALSE;
	}

	if ( ptarget_inven_item->sItemCustom.IsGM_GENITEM() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_NONDROP_CARD_TARGET_ALREADY") );
		return S_FALSE;
	}
	
	GLMSG::SNET_INVEN_NONDROP_CARD net_msg_field;
	net_msg_field.wPosX = wPosX;
	net_msg_field.wPosY = wPosY;

	NETSENDTOFIELD ( &net_msg_field );

	return S_OK;
}

/*regional chat, Juver, 2017/12/06 */
BOOL GLCharacter::RegionalChatPay ()
{
	if ( !IsValidBody() )		return FALSE;

	DWORD dwCost = RPARAM::dwRegionalChatCost;
	if ( dwCost <= 0 )	return TRUE;
	
	if( m_lnMoney < dwCost )	return FALSE;

	GLMSG::SNET_REGIONAL_CHAT_PAY net_msg_field;
	net_msg_field.dwCost = dwCost;
	NETSENDTOFIELD ( &net_msg_field );

	return TRUE;
}

/*change scale card, Juver, 2018/01/03 */
void GLCharacter::ReqChangeScale( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCALE_NOT_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCALE_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_CHANGE_SCALE_CARD )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCALE_NOT_CARD") );
		return;
	}

	m_fActiveCharScale = m_fScaleRange;
	m_bChangeCharScale = TRUE;

	CInnerInterface::GetInstance().ShowGroupFocus( CHANGE_SCALE_WINDOW );
}

/*change scale card, Juver, 2018/01/03 */
void GLCharacter::ChangeScaleCancel()
{
	m_bChangeCharScale = FALSE;
	m_fActiveCharScale = 1.0f;
}

/*change scale card, Juver, 2018/01/04 */
void GLCharacter::ChangeScaleSave()
{
	SINVENITEM*	 pInvenItem = m_cInventory.FindItem( ITEM_CHANGE_SCALE_CARD );
	if ( !pInvenItem )	return;

	if ( !m_bChangeCharScale )	return;

	GLMSG::SNETPC_INVEN_SCALE_CHANGE NetMsg;
	NetMsg.fScale = m_fActiveCharScale;
	NETSENDTOFIELD ( &NetMsg );
}

BOOL GLCharacter::InvenUseToPutOn( EMSLOT emSlot )
{
	if ( !IsValidBody() )		return FALSE;
	if ( ValidWindowOpen() )	return FALSE;	
	if ( !VALID_HOLD_ITEM() )	return FALSE;	
	if ( emSlot >= SLOT_TSIZE )	return FALSE;
	
	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA( SLOT_HOLD );
	if ( !pitem_data_hold )	return FALSE;

	SITEM* pitem_data_slot = GET_SLOT_ITEMDATA( emSlot );
	if ( !pitem_data_slot )	return FALSE;

	if ( !SIMPLE_CHECK_ITEM( pitem_data_hold->sBasicOp.sNativeID ) )	return TRUE;
	if ( CheckCoolTime( pitem_data_hold->sBasicOp.sNativeID ) )			return TRUE;

	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_EQUIP ) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PERSONAL_LOCK_MSG_PUTON") );
		return FALSE;
	}

	switch( pitem_data_hold->sBasicOp.emItemType )
	{
	case ITEM_COSTUME_COLOR:
		{
			/*item color, Juver, 2018/01/10 */
			ReqItemColorChange( emSlot );
		}break;

	default:
		{
			return FALSE;
		}break;
	};


	WORD wInvenPosX(0);
	WORD wInvenPosY(0);
	BOOL bOk = m_cInventory.FindInsrtable ( pitem_data_hold->sBasicOp.wInvenSizeX, pitem_data_hold->sBasicOp.wInvenSizeY, wInvenPosX, wInvenPosY );
	if ( !bOk )	return FALSE;

	GLMSG::SNETPC_REQ_HOLD_TO_INVEN NetMsg;
	NetMsg.wPosX = wInvenPosX;
	NetMsg.wPosY = wInvenPosY;
#if defined(VN_PARAM) //vietnamtest%%%
	NetMsg.bUseVietnamInven = GET_HOLD_ITEM().bVietnamGainItem;
#else
	NetMsg.bUseVietnamInven = FALSE;
#endif
	NETSENDTOFIELD ( &NetMsg );

	return TRUE;
}

/*item color, Juver, 2018/01/10 */
void GLCharacter::ReqItemColorChange( EMSLOT emSlot )
{
	if ( emSlot >= SLOT_TSIZE )	return;
	
	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA( SLOT_HOLD );
	if ( !pitem_data_hold )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_NOT_CARD") );
		return;
	}

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_COSTUME_COLOR )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_NOT_CARD") );
		return;
	}

	SITEM* pitem_data_slot = GET_SLOT_ITEMDATA( emSlot );
	if ( !pitem_data_slot )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR") );
		return;
	}

	if ( GET_SLOT_ITEM(emSlot).nidDISGUISE == NATIVEID_NULL() )
	{
		if ( !pitem_data_slot->sBasicOp.bItemColor )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR") );
			return;
		}
	}
	else
	{
		SITEM* pitem_data_disguise = GLItemMan::GetInstance().GetItem( GET_SLOT_ITEM(emSlot).nidDISGUISE );
		if ( !pitem_data_disguise )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR") );
			return;
		}

		if ( !pitem_data_disguise->sBasicOp.bItemColor )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR") );
			return;
		}
	}

	
	m_bEnableItemColor = TRUE;
	m_emSlotItemColor = emSlot;
	m_wItemColor1 = GET_SLOT_ITEM( m_emSlotItemColor ).wColor1;
	m_wItemColor2 = GET_SLOT_ITEM( m_emSlotItemColor ).wColor2;
	CInnerInterface::GetInstance().ShowGroupFocus( ITEM_COLOR_WINDOW );
}

/*item color, Juver, 2018/01/10 */
void GLCharacter::ItemColorEnd()
{
	m_bEnableItemColor = FALSE;
	m_wItemColor1 = ITEMCOLOR_WHITE;
	m_wItemColor2 = ITEMCOLOR_WHITE;
	m_emSlotItemColor = SLOT_TSIZE;

	if ( m_pd3dDevice )
		UpdateSuit();
}

/*item color, Juver, 2018/01/10 */
void GLCharacter::ItemColorUpdate()
{
	EMPIECECHAR empiece = SLOT_2_PIECE ( m_emSlotItemColor );

	if ( empiece >= PIECE_SIZE )	return;

	if ( m_pSkinChar )
	{
		m_pSkinChar->SetColor1( empiece, m_wItemColor1 );
		m_pSkinChar->SetColor2( empiece, m_wItemColor2 );
	}
}

/*item color, Juver, 2018/01/10 */
void GLCharacter::ItemColorChange()
{
	if ( m_emSlotItemColor >= SLOT_TSIZE )	return;

	SITEM* pitem_data_slot = GET_SLOT_ITEMDATA( m_emSlotItemColor );
	if ( !pitem_data_slot )	return;

	if ( GET_SLOT_ITEM(m_emSlotItemColor).nidDISGUISE == NATIVEID_NULL() )
	{
		if ( !pitem_data_slot->sBasicOp.bItemColor )	return;
	}
	else
	{
		SITEM* pitem_data_disguise = GLItemMan::GetInstance().GetItem( GET_SLOT_ITEM(m_emSlotItemColor).nidDISGUISE );
		if ( !pitem_data_disguise )	return;
		if ( !pitem_data_disguise->sBasicOp.bItemColor )	return;
	}

	SINVENITEM*	 pinvenitem = m_cInventory.FindItem( ITEM_COSTUME_COLOR );
	if ( !pinvenitem )	return;

	GLMSG::SNETPC_INVEN_ITEMCOLOR_CHANGE NetMsg;
	NetMsg.emSlot = m_emSlotItemColor;
	NetMsg.wColor1 = m_wItemColor1;
	NetMsg.wColor2 = m_wItemColor2;
	NETSENDTOFIELD ( &NetMsg );
}

/*item wrapper, Juver, 2018/01/11 */
void GLCharacter::ReqItemWrap( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return;
	if ( GLTradeClient::GetInstance().Valid() )	return;

	SINVENITEM* pinven_item_target = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pinven_item_target )	return;

	wPosX = pinven_item_target->wPosX;
	wPosY = pinven_item_target->wPosY;

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_NOT_VALID") );
		return;
	}

	if ( !pinven_item_target->sItemCustom.IsGM_GENITEM() && pitem_data_target->sBasicOp.IsEXCHANGE() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_NO_NEED") );
		return;
	}

	if ( !pitem_data_target->isWrappable() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_NOT_VALID") );
		return;
	}

	if ( pinven_item_target->sItemCustom.nidDISGUISE != NATIVEID_NULL() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_HAVE_DISGUISE") );
		return;
	}

	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pitem_data_hold )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_HOLD_INVALID") );
		return;
	}

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_WRAPPER )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_HOLD_NOT_WRAPPER") );
		return;
	}

	SITEM* phold_item_box = GLItemMan::GetInstance().GetItem( pitem_data_hold->sBasicOp.sidWrapperBox );
	if ( !phold_item_box )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_HOLD_NOT_BOX") );
		return;
	}

	DoModal( ID2GAMEINTEXT("MODAL_WRAP_ITEM"),  MODAL_INFOMATION, YESNO, MODAL_WRAP_ITEM );	

	m_wInvenPosX3 = wPosX;
	m_wInvenPosY3 = wPosY;
}

/*item wrapper, Juver, 2018/01/11 */
void GLCharacter::ReqItemUnwrap( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return;
	if ( GLTradeClient::GetInstance().Valid() )	return;


	SINVENITEM* pinven_item_target = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pinven_item_target )	return;

	wPosX = pinven_item_target->wPosX;
	wPosY = pinven_item_target->wPosY;

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_TARGET_NOT_VALID") );
		return;
	}

	if ( pinven_item_target->sItemCustom.nidDISGUISE == NATIVEID_NULL() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_TARGET_NOT_VALID") );
		return;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_WRAPPER_BOX )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_TARGET_NOT_VALID") );
		return;
	}

	SITEM* pitem_data_disguise = GLItemMan::GetInstance().GetItem( pinven_item_target->sItemCustom.nidDISGUISE );
	if ( !pitem_data_disguise )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_TARGET_NOT_VALID") );
		return;
	}

	DoModal( ID2GAMEINTEXT("MODAL_UNWRAP_ITEM"),  MODAL_INFOMATION, YESNO, MODAL_UNWRAP_ITEM );	

	m_wInvenPosX3 = wPosX;
	m_wInvenPosY3 = wPosY;
}

/*item wrapper, Juver, 2018/01/12 */
void GLCharacter::ReqItemWrapMsg()
{
	if ( !IsValidBody() )						return;
	if ( GLTradeClient::GetInstance().Valid() )	return;

	SINVENITEM* pinven_item_target = m_cInventory.FindPosItem ( m_wInvenPosX3, m_wInvenPosY3 );
	if ( !pinven_item_target )	return;

	m_wInvenPosX3 = pinven_item_target->wPosX;
	m_wInvenPosY3 = pinven_item_target->wPosY;

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_NOT_VALID") );
		return;
	}

	if ( !pinven_item_target->sItemCustom.IsGM_GENITEM() && pitem_data_target->sBasicOp.IsEXCHANGE() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_NO_NEED") );
		return;
	}

	if ( !pitem_data_target->isWrappable() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_NOT_VALID") );
		return;
	}

	if ( pinven_item_target->sItemCustom.nidDISGUISE != NATIVEID_NULL() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_HAVE_DISGUISE") );
		return;
	}

	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pitem_data_hold )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_HOLD_INVALID") );
		return;
	}

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_WRAPPER )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_HOLD_NOT_WRAPPER") );
		return;
	}

	SITEM* phold_item_box = GLItemMan::GetInstance().GetItem( pitem_data_hold->sBasicOp.sidWrapperBox );
	if ( !phold_item_box )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_HOLD_NOT_BOX") );
		return;
	}
	
	GLMSG::SNETPC_INVEN_WRAP NetMsg;
	NetMsg.wPosX = m_wInvenPosX3;
	NetMsg.wPosY = m_wInvenPosY3;
	NETSENDTOFIELD ( &NetMsg );
}

/*item wrapper, Juver, 2018/01/11 */
void GLCharacter::ReqItemUnwrapMsg()
{
	if ( !IsValidBody() )						return;
	if ( GLTradeClient::GetInstance().Valid() )	return;


	SINVENITEM* pinven_item_target = m_cInventory.FindPosItem ( m_wInvenPosX3, m_wInvenPosY3 );
	if ( !pinven_item_target )	return;

	m_wInvenPosX3 = pinven_item_target->wPosX;
	m_wInvenPosY3 = pinven_item_target->wPosY;

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( pinven_item_target->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_TARGET_NOT_VALID") );
		return;
	}

	if ( pinven_item_target->sItemCustom.nidDISGUISE == NATIVEID_NULL() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_TARGET_NOT_VALID") );
		return;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_WRAPPER_BOX )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_TARGET_NOT_VALID") );
		return;
	}

	SITEM* pitem_data_disguise = GLItemMan::GetInstance().GetItem( pinven_item_target->sItemCustom.nidDISGUISE );
	if ( !pitem_data_disguise )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_TARGET_NOT_VALID") );
		return;
	}

	GLMSG::SNETPC_INVEN_UNWRAP NetMsg;
	NetMsg.wPosX = m_wInvenPosX3;
	NetMsg.wPosY = m_wInvenPosY3;
	NETSENDTOFIELD ( &NetMsg );
}

/*change school card, Juver, 2018/01/12 */
void GLCharacter::ReqChangeSchool( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_NOT_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_CHANGE_SCHOOL )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_NOT_CARD") );
		return;
	}

	if ( RPARAM::bChangeSchoolReqCleanQuest && m_cQuestPlay.GetQuestProc().size() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_CLEAN_QUEST") );
		return;
	}

	if (RPARAM::bChangeSchoolReqCleanGuild && m_dwGuild!=CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_CLEAN_GUILD") );
		return;
	}

	if ( m_emClass == GLCC_EXTREME_M || m_emClass == GLCC_EXTREME_W )
	{
		if (RPARAM::bChangeSchoolReqMaxLevel && m_wLevel < GLCONST_CHAR::wMAX_EXTREME_LEVEL )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_MAX_LEVEL") );
			return;
		}
	}
	else
	{
		if (RPARAM::bChangeSchoolReqMaxLevel && m_wLevel < GLCONST_CHAR::wMAX_LEVEL )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_MAX_LEVEL") );
			return;
		}
	}
	
	CInnerInterface::GetInstance().ShowGroupFocus( CHANGE_SCHOOL_WINDOW );
}

/*change school card, Juver, 2018/01/12 */
void GLCharacter::ReqChangeSchoolMsg( WORD wSelected )
{
	if ( wSelected >= GLSCHOOL_NUM )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_NOT_SELECTED") );
		return;
	}

	if ( wSelected == m_wSchool )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_SAME_SCHOOL") );
		return;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_CHANGE_SCHOOL );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_NOT_CARD") );
		return;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_CHANGE_SCHOOL )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_NOT_CARD") );
		return;
	}

	if (RPARAM::bChangeSchoolReqCleanQuest && m_cQuestPlay.GetQuestProc().size() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_CLEAN_QUEST") );
		return;
	}

	if (RPARAM::bChangeSchoolReqCleanGuild && m_dwGuild!=CLUB_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_CLEAN_GUILD") );
		return;
	}

	if ( m_emClass == GLCC_EXTREME_M || m_emClass == GLCC_EXTREME_W )
	{
		if (RPARAM::bChangeSchoolReqMaxLevel && m_wLevel < GLCONST_CHAR::wMAX_EXTREME_LEVEL )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_MAX_LEVEL") );
			return;
		}
	}
	else
	{
		if (RPARAM::bChangeSchoolReqMaxLevel && m_wLevel < GLCONST_CHAR::wMAX_LEVEL )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_MAX_LEVEL") );
			return;
		}
	}


	GLMSG::SNETPC_INVEN_CHANGE_SCHOOL NetMsg;
	NetMsg.wSchool = wSelected;
	NETSENDTOFIELD ( &NetMsg );
}

/*item transfer card, Juver, 2018/01/18 */
void GLCharacter::ReqItemTransferOpen( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_NOT_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_TRANSFER_CARD )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_NOT_CARD") );
		return;
	}

	CInnerInterface::GetInstance().OpenItemTransferWindow();
}

/*item transfer card, Juver, 2018/01/18 */
const SITEMCUSTOM GLCharacter::GetItemTransferItemOld()
{
	SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = NATIVEID_NULL();
	if( !m_sItemTransferPosOld.VALID() )	return sItemCustom;

	SINVENITEM* pResistItem = m_cInventory.GetItem( m_sItemTransferPosOld.wPosX, m_sItemTransferPosOld.wPosY );
	if( !pResistItem )	return sItemCustom;

	sItemCustom = pResistItem->sItemCustom;
	return sItemCustom;
}

/*item transfer card, Juver, 2018/01/18 */
const SITEMCUSTOM GLCharacter::GetItemTransferItemNew()
{
	SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = NATIVEID_NULL();
	if( !m_sItemTransferPosNew.VALID() )	return sItemCustom;

	SINVENITEM* pResistItem = m_cInventory.GetItem( m_sItemTransferPosNew.wPosX, m_sItemTransferPosNew.wPosY );
	if( !pResistItem )	return sItemCustom;

	sItemCustom = pResistItem->sItemCustom;
	return sItemCustom;
}

/*item transfer card, Juver, 2018/01/18 */
void GLCharacter::SetItemTransferMoveItemOld()
{
	if( m_sPreInventoryItem.VALID() )
	{
		if (m_sItemTransferPosOld.wPosX == m_sPreInventoryItem.wPosX &&
			m_sItemTransferPosOld.wPosY == m_sPreInventoryItem.wPosY )
		{
			m_sPreInventoryItem.RESET();
			return;
		}

		if ( m_sItemTransferPosNew.wPosX == m_sPreInventoryItem.wPosX &&
			m_sItemTransferPosNew.wPosY == m_sPreInventoryItem.wPosY )
		{
			m_sPreInventoryItem.RESET();
			return;
		}

		m_sItemTransferPosOld.SET( m_sPreInventoryItem.wPosX, m_sPreInventoryItem.wPosY );
		m_sPreInventoryItem.RESET();
	}
}

/*item transfer card, Juver, 2018/01/18 */
void GLCharacter::SetItemTransferMoveItemNew()
{
	if( m_sPreInventoryItem.VALID() )
	{
		if (m_sItemTransferPosOld.wPosX == m_sPreInventoryItem.wPosX &&
			m_sItemTransferPosOld.wPosY == m_sPreInventoryItem.wPosY )
		{
			m_sPreInventoryItem.RESET();
			return;
		}

		if ( m_sItemTransferPosNew.wPosX == m_sPreInventoryItem.wPosX &&
			m_sItemTransferPosNew.wPosY == m_sPreInventoryItem.wPosY )
		{
			m_sPreInventoryItem.RESET();
			return;
		}

		m_sItemTransferPosNew.SET( m_sPreInventoryItem.wPosX, m_sPreInventoryItem.wPosY );
		m_sPreInventoryItem.RESET();
	}
}

/*item transfer card, Juver, 2018/01/18 */
void GLCharacter::ReSetItemTransferItemOld()
{
	m_sItemTransferPosOld.RESET();
}

/*item transfer card, Juver, 2018/01/18 */
void GLCharacter::ReSetItemTransferItemNew()
{
	m_sItemTransferPosNew.RESET();
}

/*item transfer card, Juver, 2018/01/18 */
void GLCharacter::ResetItemTransferItem()
{
	m_sItemTransferPosOld.RESET();
	m_sItemTransferPosNew.RESET();
	m_sPreInventoryItem.RESET();
}

/*item transfer card, Juver, 2018/01/18 */
bool GLCharacter::ValidItemTransferOpen()
{
	if ( CInnerInterface::GetInstance().IsVisibleGroup( ITEM_TRANSFER_INVEN_WINDOW ) 
		|| CInnerInterface::GetInstance().IsVisibleGroup( ITEM_TRANSFER_WINDOW ) )
		return true;
	return false;
}

/*item transfer card, Juver, 2018/01/18 */
void GLCharacter::ItemTransferStart()
{
	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_TRANSFER_CARD );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_NOT_CARD") );
		return;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_TRANSFER_CARD )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_NOT_CARD") );
		return;
	}

	const SITEMCUSTOM sitemcustom_old = GetItemTransferItemOld();
	const SITEMCUSTOM sitemcustom_new = GetItemTransferItemNew();

	SITEM* pitem_data_old = GLItemMan::GetInstance().GetItem( sitemcustom_old.sNativeID );
	SITEM* pitem_data_new = GLItemMan::GetInstance().GetItem( sitemcustom_new.sNativeID );
	if ( !pitem_data_old || !pitem_data_new )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_INVALID_ITEM") );
		return;
	}

	if ( pitem_data_old->sBasicOp.emItemType != ITEM_SUIT || pitem_data_new->sBasicOp.emItemType != ITEM_SUIT )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_INVALID_ITEM_TYPE") );
		return;
	}

	if ( pitem_data_old->IsTIMELMT() || pitem_data_new->IsTIMELMT() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_TIME_LIMIT") );
		return;
	}

	if ( pitem_data_old->sBasicOp.IsDISGUISE() || pitem_data_new->sBasicOp.IsDISGUISE() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_DISGUISE") );
		return;
	}

	if ( !pitem_data_old->sBasicOp.bItemTransfer || !pitem_data_new->sBasicOp.bItemTransfer )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_NOT_TRANSFER_ITEM") );
		return;
	}

	if ( pitem_data_old->sSuitOp.emSuit != pitem_data_new->sSuitOp.emSuit )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_SUIT_MISMATCH") );
		return;
	}

	if ( pitem_data_old->sSuitOp.emSuit == SUIT_HANDHELD && pitem_data_old->sSuitOp.emAttack != pitem_data_new->sSuitOp.emAttack )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_ATTACK_MISMATCH") );
		return;
	}

	if ( RPARAM::dwItemTransferCost != 0 && m_lnMoney < RPARAM::dwItemTransferCost ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_COST") );
		return;
	}

	if ( sitemcustom_old.nidDISGUISE != NATIVEID_NULL() || sitemcustom_new.nidDISGUISE != NATIVEID_NULL() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_HAVE_DISGUISE") );
		return;
	}

	GLMSG::SNETPC_INVEN_TRANSFER_STATS net_msg;
	net_msg.sInvenPosOLD = m_sItemTransferPosOld;
	net_msg.sInvenPosNEW = m_sItemTransferPosNew;
	NETSENDTOFIELD ( &net_msg );

	ResetItemTransferItem();
}

/* car, cart color, Juver, 2018/02/14 */
void GLCharacter::ReqCarColor( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_INVALID_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_INVALID_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_CARD_CAR_COLOR )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_INVALID_CARD") );
		return;
	}

	if ( !m_bVehicle )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_INACTIVE") );
		return;
	}

	SITEM* pVehicle = GET_SLOT_ITEMDATA( SLOT_VEHICLE );
	if ( !pVehicle )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_INACTIVE") );
		return;
	}

	if ( !pVehicle->sVehicle.bUseColor )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_NOT_COLOR") );
		return;
	}

	if ( pVehicle->sVehicle.emVehicleType != VEHICLE_TYPE_CAR && pVehicle->sVehicle.emVehicleType != VEHICLE_TYPE_CART )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_NOT_CAR") );
		return;
	}

	m_wCarColorA = m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A1];
	m_wCarColorB = m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A2];
	
	m_bEnableCarColor = TRUE;

	CInnerInterface::GetInstance().ShowGroupFocus( CAR_COLOR_WINDOW );
}

/* car, cart color, Juver, 2018/02/14 */
void GLCharacter::CarColorEnd()
{
	m_bEnableCarColor = FALSE;
	m_wCarColorA = ITEMCOLOR_WHITE;
	m_wCarColorB = ITEMCOLOR_WHITE;

	if ( m_pd3dDevice )
		UpdateSuit();
}

/* car, cart color, Juver, 2018/02/14 */
void GLCharacter::CarColorUpdate()
{
	if ( m_bEnableCarColor && m_pSkinChar && m_pSkinChar->m_pVehicle && ( m_sVehicle.m_emTYPE == VEHICLE_TYPE_CAR || m_sVehicle.m_emTYPE == VEHICLE_TYPE_CART ) )
	{
		m_pSkinChar->m_pVehicle->SetColor1( PIECE_SIZE, m_wCarColorA );
		m_pSkinChar->m_pVehicle->SetColor2( PIECE_SIZE, m_wCarColorB );
	}
}

/* car, cart color, Juver, 2018/02/14 */
void GLCharacter::CarColorChange()
{
	SINVENITEM*	 pInvenItem = m_cInventory.FindItem( ITEM_CARD_CAR_COLOR );
	if ( !pInvenItem )	return;

	if ( !m_bVehicle )	return;
	if ( !m_bEnableCarColor )	return;

	GLMSG::SNET_VEHICLE_REQ_CHANGE_CAR_COLOR NetMsg;
	NetMsg.wColorA = m_wCarColorA;
	NetMsg.wColorB = m_wCarColorB;

	NETSENDTOFIELD ( &NetMsg );
}

/* booster all vehicle, Juver, 2018/02/14 */
void GLCharacter::ReqAllVehicleEnableBooster( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem ) return;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) return;

	if ( pItem->sBasicOp.emItemType != ITEM_VEHICLE )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM") );
		return;
	}

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold ) return;

	if ( pHold->sBasicOp.emItemType != ITEM_CARD_VEHICLE_BOOST )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD") );
		return;
	}

	if ( pInvenItem->sItemCustom.dwVehicleID == 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM") );
		return;
	}

	if ( pItem->sVehicle.emVehicleType == VEHICLE_TYPE_BOARD )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_TYPE") );
		return;
	}

	SVEHICLEITEMINFO sVehicle;

	if ( !DxGlobalStage::GetInstance().IsEmulator() )
	{
		VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pInvenItem->sItemCustom.dwVehicleID );
		if ( iter==m_mapVEHICLEItemInfo.end() ) return;
		sVehicle = (*iter).second;
	}
	else
	{
		sVehicle.m_bBooster = m_sVehicle.m_bBooster;
	}

	if ( sVehicle.m_bBooster ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_ALREADY_ENABLED") );
		return;
	}

	GLMSG::SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;

	NETSENDTOFIELD ( &NetMsg );
}
/* qitem inven, 2024/01/07 */
HRESULT GLCharacter::ReqInvenQItem ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem || pItem->sBasicOp.emItemType!=ITEM_QITEM )	return E_FAIL;

	if( CInnerInterface::GetInstance().GetQBoxButton()->GetQBoxEnable() == FALSE )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("QBOX_OPTION_DISABLE_MSG") );
		return E_FAIL;
	}

	PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap ();
	if ( !pLandMan )	
		return E_FAIL;

	if( pLandMan && pItem->sBasicOp.sSubID != SNATIVEID(false) )
	{
		if( pLandMan->GetMapID() != pItem->sBasicOp.sSubID )
		{
			CInnerInterface::GetInstance().PrintConsoleText("Unable to use this item on other map");
			return E_FAIL;
		}
	}

	SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( pLandMan->GetMapID() );
	if ( pMapNode && !pMapNode->bQBoxEnable)
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("QBOX_NOT_USE_AREA") );
		return E_FAIL;
	}

	GLMSG::SNETPC_REQ_USE_INVENQITEM NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}
/* additional gm commands, Juver, 2018/03/01 */
void GLCharacter::GMInvenOpen( std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_INVEN_OPEN_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_INVEN_OPEN_FB_INVALID_PASS") );
		return;
	}

	if ( m_wINVENLINE >= (EM_INVENSIZE_Y - EM_INVEN_DEF_SIZE_Y - EM_INVEN_PREMIUM_SIZE) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_INVEN_OPEN_FB_FAIL") );
		return;
	}

	GLMSG::SNETPC_GM_COMMAND_INVEN_OPEN net_msg;
	StringCchCopy( net_msg.szPass, MAX_PATH, strPass.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/* additional gm commands, Juver, 2018/03/01 */
void GLCharacter::GMGetSkillAll( std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SKILL_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SKILL_FB_INVALID_PASS") );
		return;
	}

	GLMSG::SNETPC_GM_COMMAND_GET_SKILL net_msg;
	net_msg.bALL = TRUE;
	StringCchCopy( net_msg.szPass, MAX_PATH, strPass.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/* additional gm commands, Juver, 2018/03/01 */
void GLCharacter::GMGetSkill( WORD wMID, WORD wSID, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SKILL_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SKILL_FB_INVALID_PASS") );
		return;
	}

	GLSKILL* pskill_data = GLSkillMan::GetInstance().GetData ( wMID, wSID );
	if ( !pskill_data )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SKILL_FB_INVALID_SKILL") );
		return;
	}

	GLMSG::SNETPC_GM_COMMAND_GET_SKILL net_msg;
	net_msg.bALL = FALSE;
	net_msg.sidSkill = pskill_data->m_sBASIC.sNATIVEID;
	StringCchCopy( net_msg.szPass, MAX_PATH, strPass.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/* additional gm commands, Juver, 2018/03/01 */
void GLCharacter::GMGetLevel( WORD wLevel, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_LEVEL_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_LEVEL_FB_INVALID_PASS") );
		return;
	}

	if( m_emClass == GLCC_EXTREME_M || m_emClass == GLCC_EXTREME_W )
	{
		if ( m_wLevel >= GLCONST_CHAR::wMAX_EXTREME_LEVEL )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_LEVEL_FB_MAX_LEVEL") );
			return;
		}
	}
	else
	{
		if ( m_wLevel >= GLCONST_CHAR::wMAX_LEVEL )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_LEVEL_FB_MAX_LEVEL") );
			return;
		}
	}

	GLMSG::SNETPC_GM_COMMAND_GET_LEVEL net_msg;
	net_msg.wLevel = wLevel;
	StringCchCopy( net_msg.szPass, MAX_PATH, strPass.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/* additional gm commands, Juver, 2018/03/01 */
void GLCharacter::GMGetStat( EMSTATS emStat, WORD wStats, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_STATS_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_STATS_FB_INVALID_PASS") );
		return;
	}

	GLMSG::SNETPC_GM_COMMAND_GET_STATS net_msg;
	net_msg.emStat = emStat;
	net_msg.wNum = wStats;
	StringCchCopy( net_msg.szPass, MAX_PATH, strPass.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/* additional gm commands, Juver, 2018/03/01 */
void GLCharacter::GMGetStatP( WORD wNum, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_STATSP_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_STATSP_FB_INVALID_PASS") );
		return;
	}

	GLMSG::SNETPC_GM_COMMAND_GET_STATSP net_msg;
	net_msg.wNum = wNum;
	StringCchCopy( net_msg.szPass, MAX_PATH, strPass.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/* additional gm commands, Juver, 2018/03/01 */
void GLCharacter::GMGetSkillP( WORD wNum, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SKILLP_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SKILLP_FB_INVALID_PASS") );
		return;
	}

	GLMSG::SNETPC_GM_COMMAND_GET_SKILLP net_msg;
	net_msg.wNum = wNum;
	StringCchCopy( net_msg.szPass, MAX_PATH, strPass.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/*gm command send item, Juver, 2019/03/23 */
void GLCharacter::GMSendItem( DWORD dwCharID, WORD wMID, WORD wSID, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SEND_ITEM_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SEND_ITEM_FB_INVALID_PASS") );
		return;
	}

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem ( wMID, wSID );
	if ( !pitem_data )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_SEND_ITEM_FB_INVALID_ITEM") );
		return;
	}

	GLMSG::SNETPC_GM_COMMAND_C2A_SEND_ITEM net_msg;
	net_msg.dwCharID = dwCharID;
	net_msg.sidItem = pitem_data->sBasicOp.sNativeID;
	StringCchCopy( net_msg.szPass, MAX_PATH, strPass.c_str() );
	NETSEND ( &net_msg );
}

/*modern inventory window, Juver, 2018/04/09 */
void GLCharacter::InventoryOpenLocker()
{
	SINVENITEM* sItem = GLGaeaClient::GetInstance().GetCharacter()->m_cInventory.FindItem( ITEM_STORAGE_CONNECT );
	if ( sItem )
	{
		m_wInventoryUsePosX = sItem->wPosX;
		m_wInventoryUsePosY = sItem->wPosY;
		DoModal (  ID2GAMEINTEXT("MODERN_INVENTORY_LOCKER_OPEN_ITEM_REQ"), MODAL_QUESTION, YESNO, MODAL_MODERN_INVENTORY_LOCKER );	
	}else{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("MODERN_INVENTORY_LOCKER_OPEN_FAIL" ) );
	}
}

/*modern inventory window, Juver, 2018/04/09 */
void GLCharacter::InventoryExpand()
{
	SINVENITEM* sItem = GLGaeaClient::GetInstance().GetCharacter()->m_cInventory.FindItem( ITEM_INVEN_CARD );
	if ( sItem )
	{
		m_wInventoryUsePosX = sItem->wPosX;
		m_wInventoryUsePosY = sItem->wPosY;
		DoModal (  ID2GAMEINTEXT("MODERN_INVEN_EXTEND_OPEN_ITEM_REQ"), MODAL_QUESTION, YESNO, MODAL_MODERN_INVENTORY_BACKPACK );	
	}else{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("MODERN_INVENTORY_EXTEND_OPEN_FAIL" ) );
	}
}

/*modern inventory window, Juver, 2018/04/09 */
void GLCharacter::InventoryOpenTrashCan()
{
	DoModal (  ID2GAMEINTEXT("MODERN_INVENTORY_WASTEBASKET_OPEN"), MODAL_QUESTION, YESNO, MODAL_MODERN_INVENTORY_WASTEBASKET );
}

/*modern inventory window, Juver, 2018/04/09 */
/*inventory sort, Juver, 2018/07/07 */
void GLCharacter::InventorySort()
{
	//function disabled
	if ( !RPARAM::use_inven_sort )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_INVEN_SORT_DISABLED") );
		return;
	}

	if ( ValidWindowOpen() || m_sPMarket.IsOpen() || VALID_HOLD_ITEM () )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_INVEN_SORT_FAILED") );
		return;
	}

	if ( m_fInventorySortTimer < REQ_INVENTORY_SORT_DELAY )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_INVEN_SORT_DELAY") );
		return;
	}

	m_fInventorySortTimer = 0.0f;

	GLMSG::SNETPC_INVEN_SORT net_msg;
	net_msg.item_num = m_cInventory.GetNumItems();
	NETSENDTOFIELD ( &net_msg );
}

/*item random option rebuild, Juver, 2018/07/02 */
void GLCharacter::ReqItemRandomOptionRebuildOpen( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_RANDON_OPTION_REBUILD )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_CARD") );
		return;
	}

	CInnerInterface::GetInstance().OpenItemRandomOptionRebuildWindow();
}


/*item random option rebuild, Juver, 2018/07/03 */
void GLCharacter::SetItemRandomOptionRebuildMoveItem()
{
	if( m_sPreInventoryItem.VALID() )
	{
		if (m_sItemRandomOptionRebuildItem.wPosX == m_sPreInventoryItem.wPosX &&
			m_sItemRandomOptionRebuildItem.wPosY == m_sPreInventoryItem.wPosY )
		{
			m_sPreInventoryItem.RESET();
			return;
		}

		if ( m_sItemRandomOptionRebuildStamp.wPosX == m_sPreInventoryItem.wPosX &&
			m_sItemRandomOptionRebuildStamp.wPosY == m_sPreInventoryItem.wPosY )
		{
			m_sPreInventoryItem.RESET();
			return;
		}

		SITEMCUSTOM hold_item = GET_PREHOLD_ITEM();
		SITEM* phold_item = GLItemMan::GetInstance().GetItem( hold_item.sNativeID );
		if( phold_item && phold_item->sBasicOp.emItemType == ITEM_SUIT )
		{
			/*use rebuild flag, Juver, 2019/02/18 */
			if( strlen( phold_item->sRandomOpt.szNAME ) > 3 && phold_item->sBasicOp.IsUseRebuild() )
			{
				m_sItemRandomOptionRebuildItem.SET( m_sPreInventoryItem.wPosX, m_sPreInventoryItem.wPosY );
			}
		}

		m_sPreInventoryItem.RESET();
	}
}

/*item random option rebuild, Juver, 2018/07/03 */
void GLCharacter::SetItemRandomOptionRebuildMoveStamp()
{
	if( m_sPreInventoryItem.VALID() )
	{
		if (m_sItemRandomOptionRebuildItem.wPosX == m_sPreInventoryItem.wPosX &&
			m_sItemRandomOptionRebuildItem.wPosY == m_sPreInventoryItem.wPosY )
		{
			m_sPreInventoryItem.RESET();
			return;
		}

		if ( m_sItemRandomOptionRebuildStamp.wPosX == m_sPreInventoryItem.wPosX &&
			m_sItemRandomOptionRebuildStamp.wPosY == m_sPreInventoryItem.wPosY )
		{
			m_sPreInventoryItem.RESET();
			return;
		}

		SITEMCUSTOM hold_item = GET_PREHOLD_ITEM();
		SITEM* phold_item = GLItemMan::GetInstance().GetItem( hold_item.sNativeID );
		if( phold_item && phold_item->sBasicOp.emItemType == ITEM_SEALED_CARD )
		{
			m_sItemRandomOptionRebuildStamp.SET( m_sPreInventoryItem.wPosX, m_sPreInventoryItem.wPosY );
		}

		m_sPreInventoryItem.RESET();
	}
}

/*item random option rebuild, Juver, 2018/07/03 */
void GLCharacter::ReSetItemRandomOptionRebuildItem()
{
	m_sItemRandomOptionRebuildItem.RESET();
}

/*item random option rebuild, Juver, 2018/07/03 */
void GLCharacter::ReSetItemRandomOptionRebuildStamp()
{
	m_sItemRandomOptionRebuildStamp.RESET();

	m_bItemRandomOptionRebuildOptionLock0 = FALSE;
	m_bItemRandomOptionRebuildOptionLock1 = FALSE;
	m_bItemRandomOptionRebuildOptionLock2 = FALSE;
	m_bItemRandomOptionRebuildOptionLock3 = FALSE;
}

/*item random option rebuild, Juver, 2018/07/03 */
void GLCharacter::ResetItemRandomOptionRebuildItemAll()
{
	m_sItemRandomOptionRebuildItem.RESET();
	m_sItemRandomOptionRebuildStamp.RESET();
	m_sPreInventoryItem.RESET();

	m_bItemRandomOptionRebuildOptionLock0 = FALSE;
	m_bItemRandomOptionRebuildOptionLock1 = FALSE;
	m_bItemRandomOptionRebuildOptionLock2 = FALSE;
	m_bItemRandomOptionRebuildOptionLock3 = FALSE;
}

/*item random option rebuild, Juver, 2018/07/03 */
const SITEMCUSTOM GLCharacter::GetItemRandomOptionRebuildItem()
{
	SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = NATIVEID_NULL();
	if( !m_sItemRandomOptionRebuildItem.VALID() )	return sItemCustom;

	SINVENITEM* pitem = m_cInventory.GetItem( m_sItemRandomOptionRebuildItem.wPosX, m_sItemRandomOptionRebuildItem.wPosY );
	if( !pitem )	return sItemCustom;

	sItemCustom = pitem->sItemCustom;
	return sItemCustom;
}

/*item random option rebuild, Juver, 2018/07/03 */
const SITEMCUSTOM GLCharacter::GetItemRandomOptionRebuildStamp()
{
	SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = NATIVEID_NULL();
	if( !m_sItemRandomOptionRebuildStamp.VALID() )	return sItemCustom;

	SINVENITEM* pitem = m_cInventory.GetItem( m_sItemRandomOptionRebuildStamp.wPosX, m_sItemRandomOptionRebuildStamp.wPosY );
	if( !pitem )	return sItemCustom;

	sItemCustom = pitem->sItemCustom;
	return sItemCustom;
}

/*item random option rebuild, Juver, 2018/07/03 */
bool GLCharacter::ValidItemRandomOptionRebuildOpen()
{
	if ( CInnerInterface::GetInstance().IsVisibleGroup( ITEM_RANDOM_OPTION_REBUILD_INVENTORY_WINDOW ) 
		|| CInnerInterface::GetInstance().IsVisibleGroup( ITEM_RANDOM_OPTION_REBUILD_WINDOW ) )
		return true;
	return false;
}

/*item random option rebuild, Juver, 2018/07/03 */
WORD GLCharacter::GetItemRandomOptionRebuildLockNum()
{
	WORD num(0);
	if ( m_bItemRandomOptionRebuildOptionLock0 )	num++;
	if ( m_bItemRandomOptionRebuildOptionLock1 )	num++;
	if ( m_bItemRandomOptionRebuildOptionLock2 )	num++;
	if ( m_bItemRandomOptionRebuildOptionLock3 )	num++;

	return num;
}

/*item random option rebuild, Juver, 2018/07/03 */
void GLCharacter::SetRandomOptionRebuildLock(WORD windex, BOOL bset)
{
	SINVENITEM* pinven_item = m_cInventory.GetItem( m_sItemRandomOptionRebuildStamp.wPosX, m_sItemRandomOptionRebuildStamp.wPosY );
	if ( !pinven_item )	return;
	
	SITEM* pitem = GLItemMan::GetInstance().GetItem ( pinven_item->sItemCustom.sNativeID );
	if ( !pitem )		return;
	
	if ( pitem->sBasicOp.emItemType != ITEM_SEALED_CARD )		return;
	
	if ( bset && GetItemRandomOptionRebuildLockNum() >= pitem->sSuitOp.wReModelNum )	return;

	switch ( windex )
	{
	case 0:
		m_bItemRandomOptionRebuildOptionLock0 = bset;
		break;

	case 1:
		m_bItemRandomOptionRebuildOptionLock1 = bset;
		break;

	case 2:
		m_bItemRandomOptionRebuildOptionLock2 = bset;
		break;

	case 3:
		m_bItemRandomOptionRebuildOptionLock3 = bset;
		break;
	}
}

/*item random option rebuild, Juver, 2018/07/03 */
void GLCharacter::ItemRandomOptionRebuildStart()
{
	SINVENITEM* pinven_item_card = m_cInventory.FindItem( ITEM_RANDON_OPTION_REBUILD );
	if ( !pinven_item_card )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_CARD") );
		return;
	}

	SITEM* pitem_card = GLItemMan::GetInstance().GetItem ( pinven_item_card->sItemCustom.sNativeID );
	if ( !pitem_card ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_CARD") );
		return;
	}

	if ( pitem_card->sBasicOp.emItemType != ITEM_RANDON_OPTION_REBUILD )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_CARD") );
		return;
	}

	if ( RPARAM::item_random_option_rebuild_use_delay )
	{
		if ( m_item_random_option_rebuild_timer < RPARAM::item_random_option_rebuild_delay_time )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_DELAY_TIME") );
			return;
		}
	}
	
	if ( m_item_random_option_rebuild_active_task )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_TASK_ACTIVE") );
		return;
	}

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem( GetItemRandomOptionRebuildItem().sNativeID );
	if ( !pitem_data_target )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_NOT_TARGET") );
		return;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_TARGET_NOT_SUIT") );
		return;
	}

	/*use rebuild flag, Juver, 2019/02/18 */
	if( strlen( pitem_data_target->sRandomOpt.szNAME ) <= 3 || !pitem_data_target->sBasicOp.IsUseRebuild() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_TARGET_NOT_RANDOM") );
		return;
	}

	/* Item Reform use limit, Juver, 2021/07/23 */
	if ( pitem_data_target->sBasicOp.wMaxItemReform != ITEM_REFORM_INFINITE )
	{
		if ( GetItemRandomOptionRebuildItem().wReformUseCount >= pitem_data_target->sBasicOp.wMaxItemReform )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_REBUILD_TARGET_MAXED") );
			return;
		}
	}

	BOOL use_stamp = FALSE;
	if ( m_sItemRandomOptionRebuildStamp.VALID() )
	{
		SINVENITEM* pinven_item_stamp = m_cInventory.GetItem( m_sItemRandomOptionRebuildStamp.wPosX, m_sItemRandomOptionRebuildStamp.wPosY );
		if ( pinven_item_stamp )
		{
			SITEM* pitem_stamp = GLItemMan::GetInstance().GetItem ( pinven_item_stamp->sItemCustom.sNativeID );
			if ( pitem_stamp && pitem_stamp->sBasicOp.emItemType == ITEM_SEALED_CARD ) 
			{
				use_stamp = TRUE;

				if ( GetItemRandomOptionRebuildLockNum() > pitem_stamp->sSuitOp.wReModelNum )
				{
					use_stamp = FALSE;

					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_RANDOM_OPTION_OPTION_SELECT_ERROR") );
					return;
				}
			}
		}
	}

	if ( !use_stamp )
	{
		m_bItemRandomOptionRebuildOptionLock0 = FALSE;
		m_bItemRandomOptionRebuildOptionLock1 = FALSE;
		m_bItemRandomOptionRebuildOptionLock2 = FALSE;
		m_bItemRandomOptionRebuildOptionLock3 = FALSE;
	}
	

	
	GLMSG::SNETPC_INVEN_ITEM_RANDOM_OPTION_REBUILD net_msg;
	net_msg.sInvenPosTarget = m_sItemRandomOptionRebuildItem;
	net_msg.sInvenPosStamp = m_sItemRandomOptionRebuildStamp;
	net_msg.bOptionLock0 = m_bItemRandomOptionRebuildOptionLock0;
	net_msg.bOptionLock1 = m_bItemRandomOptionRebuildOptionLock1;
	net_msg.bOptionLock2 = m_bItemRandomOptionRebuildOptionLock2;
	net_msg.bOptionLock3 = m_bItemRandomOptionRebuildOptionLock3;

	NETSENDTOFIELD ( &net_msg );

	m_item_random_option_rebuild_timer = 0.0f;
	m_item_random_option_rebuild_active_task = TRUE;
}

/*item random option rebuild, Juver, 2018/07/03 */
bool GLCharacter::item_random_option_rebuild_task_check()
{
	if( m_item_random_option_rebuild_active_task )	return false;

	if (RPARAM::item_random_option_rebuild_use_delay )
	{
		if( m_item_random_option_rebuild_timer < RPARAM::item_random_option_rebuild_delay_time )	return false;
	}

	return true;
}

/*buffs manual remove, Juver, 2018/08/13 */
void GLCharacter::ReqBuffManualRemove( SNATIVEID skill_id )
{
	BOOL buff_exist = FALSE;
	for( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		const SSKILLFACT& skill_fact = m_sSKILLFACT[i];
		if ( skill_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( skill_fact.sNATIVEID == skill_id )	
			buff_exist = TRUE;
	}

	if ( !buff_exist )
	{
		/*manual lunchbox remove, Juver, 2018/09/19 */
		req_lunchbox_remove( skill_id );

		return;
	}

	if ( !RPARAM::allow_buff_remove )	return;

	PGLSKILL pskill = GLSkillMan::GetInstance().GetData( skill_id );
	if ( !pskill )					return;
	if ( !pskill->IsSkillFact() )	return;

	std::string skill_name = pskill->GetName();

	if ( pskill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILLFACT_RESET_FAIL"), skill_name.c_str() );
		return;
	}

	m_buff_remove_id = skill_id;

	CString question;
	question.Format( ID2GAMEINTEXT("SKILLFACT_ASK_RESET"), skill_name.c_str() );
	DoModal( question.GetString(),  MODAL_INFOMATION, YESNO, MODAL_BUFF_MANUAL_REMOVE );	
}

/*buffs manual remove, Juver, 2018/08/14 */
void GLCharacter::BuffManualRemove()
{
	if ( !RPARAM::allow_buff_remove )	return;

	PGLSKILL pskill = GLSkillMan::GetInstance().GetData( m_buff_remove_id );
	if ( !pskill )					return;
	if ( !pskill->IsSkillFact() )	return;

	std::string skill_name = pskill->GetName();

	if ( pskill->m_sBASIC.emIMPACT_SIDE == SIDE_ENEMY )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("SKILLFACT_RESET_FAIL"), skill_name.c_str() );
		return;
	}

	BOOL buff_exist = FALSE;
	for( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		const SSKILLFACT& skill_fact = m_sSKILLFACT[i];
		if ( skill_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( skill_fact.sNATIVEID == m_buff_remove_id )	
			buff_exist = TRUE;
	}

	if ( !buff_exist )	return;

	GLMSG::SNETPC_REQ_BUFF_REMOVE net_msg;
	net_msg.skill_id = m_buff_remove_id;
	NETSENDTOFIELD ( &net_msg );

	m_buff_remove_id = NATIVEID_NULL().dwID;
}

/*item bank question, Juver, 2018/08/22 */
HRESULT GLCharacter::item_bank_retrieve( WORD pos_x, WORD pos_y )
{
	SINVENITEM *pINVENITEM = m_cInvenCharged.GetItem ( pos_x, pos_y );
	if ( !pINVENITEM )						return E_FAIL;

	SNATIVEID nidPOS(pos_x,pos_y);
	MAPSHOP_KEY_ITER iter = m_mapChargedKey.find ( nidPOS.dwID );
	if ( m_mapChargedKey.end()==iter )		return E_FAIL;


	SITEM* pitem_data = GLItemMan::GetInstance().GetItem( pINVENITEM->sItemCustom.sNativeID );
	if ( !pitem_data )	return E_FAIL;

	CString strCombine = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("MODAL_ITEMBANK_GET_ITEM"), pitem_data->GetName() );
	DoModal ( strCombine, MODAL_QUESTION, YESNO, MODAL_ITEM_BANK_RETRIEVE );	
	m_item_bank_pos_x = pos_x;
	m_item_bank_pos_y = pos_y;

	return S_OK;
}

/*item bank question, Juver, 2018/08/22 */
HRESULT GLCharacter::item_bank_retrieve_answer()
{
	/* personal lock system, Juver, 2019/12/06 */
	if ( isPersonalLock( EMPERSONAL_LOCK_LOCKER ) )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PERSONAL_LOCK_MSG_LOCKER") );
		return E_FAIL;
	}

	SINVENITEM *pINVENITEM = m_cInvenCharged.GetItem ( m_item_bank_pos_x, m_item_bank_pos_y );
	if ( !pINVENITEM )						return E_FAIL;

	SNATIVEID nidPOS(m_item_bank_pos_x,m_item_bank_pos_y);
	MAPSHOP_KEY_ITER iter = m_mapChargedKey.find ( nidPOS.dwID );
	if ( m_mapChargedKey.end()==iter )		return E_FAIL;

	std::string strPurKey = (*iter).second;

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem( pINVENITEM->sItemCustom.sNativeID );
	if ( !pitem_data )	return E_FAIL;

	GLMSG::SNET_CHARGED_ITEM_GET NetMsg;
	NetMsg.dwID = nidPOS.dwID;
	StringCchCopy ( NetMsg.szPurKey, PURKEY_LENGTH+1, strPurKey.c_str() );
	NETSENDTOFIELD ( &NetMsg );

	m_item_bank_pos_x = USHRT_MAX;
	m_item_bank_pos_y = USHRT_MAX;

	return S_OK;
}

/*away gesture, Juver, 2018/08/23 */
void GLCharacter::do_away_gesture()
{
	if ( !m_pSkinChar )	return;
	if ( GLCONST_CHAR::nAWAY_GESTURE_NUMBER >= AN_SUB_00_SIZE )				return;
	if ( m_pSkinChar->GETCURSTYPE() == GLCONST_CHAR::nAWAY_GESTURE_NUMBER )	return;

	if ( m_bVehicle )		return;
	if ( !IsValidBody() )	return;

	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	BOOL bPeaceZone = pLand ? pLand->IsPeaceZone() : FALSE;

	if ( !bPeaceZone && !IsSTATE(EM_ACT_PEACEMODE) )
	{
		if ( IsACTION(GLAT_IDLE) )		ReqTogglePeaceMode();
	}

	if ( !bPeaceZone && IsSTATE(EM_ACT_PEACEMODE) )
	{
		bPeaceZone = TRUE;
	}

	if ( !bPeaceZone )		return;
	
	PANIMCONTNODE pNODE = m_pSkinChar->GETANI ( AN_GESTURE, EMANI_SUBTYPE(GLCONST_CHAR::nAWAY_GESTURE_NUMBER) );
	if ( !pNODE )			return;

	m_dwANISUBGESTURE = (DWORD) GLCONST_CHAR::nAWAY_GESTURE_NUMBER;
	TurnAction(GLAT_TALK);

	GLMSG::SNETPC_REQ_GESTURE NetMsg;
	NetMsg.dwID = m_dwANISUBGESTURE;
	NETSENDTOFIELD ( &NetMsg );
}

/*specific item box, Juver, 2018/09/02 */
HRESULT GLCharacter::item_box_specific_open_window ( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pinventory_item = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pinventory_item )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM") );
		return E_FAIL;
	}

	wPosX = pinventory_item->wPosX;
	wPosY = pinventory_item->wPosY;

	SITEM* pitem_data = GLItemMan::GetInstance().GetItem ( pinventory_item->sItemCustom.sNativeID );
	if ( !pitem_data )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM") );
		return E_FAIL;
	}

	if ( pitem_data->sBasicOp.emItemType != ITEM_BOX_SPECIFIC )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM") );
		return E_FAIL;
	}

	if ( !pitem_data->sBox.VALID() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_BOX") );
		return E_FAIL;
	}

	if ( !SIMPLE_CHECK_ITEM( pitem_data->sBasicOp.sNativeID ) ) return E_FAIL;

	for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
	{
		SITEMCUSTOM sCUSTOM;
		sCUSTOM.sNativeID = pitem_data->sBox.sITEMS[i].nidITEM;
		if ( sCUSTOM.sNativeID==SNATIVEID(false) )				continue;

		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
		if ( !pITEM )
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_CONTENTS") );
			return E_FAIL;
		}
	}


	m_item_box_pos_x = wPosX;
	m_item_box_pos_y = wPosY;

	CInnerInterface::GetInstance().item_box_specific_selection_window_show( pitem_data->sBasicOp.sNativeID );

	return S_OK;
}

/*specific item box, Juver, 2018/09/03 */
HRESULT GLCharacter::item_box_specific_select_item ( SNATIVEID item_id, int selected_index, SNATIVEID selected_item_id )
{
	SINVENITEM* pinventory_item = m_cInventory.FindPosItem ( m_item_box_pos_x, m_item_box_pos_y );
	if ( !pinventory_item )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM") );
		return E_FAIL;
	}

	SITEM* pitem_data_main = GLItemMan::GetInstance().GetItem ( pinventory_item->sItemCustom.sNativeID );
	if ( !pitem_data_main )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM") );
		return E_FAIL;
	}

	if ( pitem_data_main->sBasicOp.emItemType != ITEM_BOX_SPECIFIC )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_ITEM") );
		return E_FAIL;
	}

	if ( !pitem_data_main->sBox.VALID() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_BOX") );
		return E_FAIL;
	}

	if ( selected_index < 0 || selected_index >= ITEM::SBOX::ITEM_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_SELECTED") );
		return E_FAIL;
	}

	if ( selected_item_id == NATIVEID_NULL() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_SELECTED") );
		return E_FAIL;
	}

	if ( !SIMPLE_CHECK_ITEM( pitem_data_main->sBasicOp.sNativeID ) ) return E_FAIL;

	SITEM* pitem_data_selected = GLItemMan::GetInstance().GetItem( selected_item_id );
	if ( !pitem_data_selected )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_SELECTED") );
		return E_FAIL;
	}

	if ( pitem_data_main->sBox.sITEMS[selected_index].nidITEM != selected_item_id )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_BAD_SELECTED_EXIST") );
		return E_FAIL;
	}

	WORD found_pos_x, found_pos_y;
	BOOL found_pos = m_cInventory.FindInsrtable ( pitem_data_selected->sBasicOp.wInvenSizeX, pitem_data_selected->sBasicOp.wInvenSizeY, found_pos_x, found_pos_y );
	if ( !found_pos )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ITEM_BOX_SPECIFIC_NO_INVEN_SPACE") );
		return E_FAIL;
	}

	GLMSG::SNET_INVEN_REQ_OPEN_ITEM_BOX_SPECIFIC NetMsg;
	NetMsg.pos_x = m_item_box_pos_x;
	NetMsg.pos_y = m_item_box_pos_y;
	NetMsg.item_id = item_id;
	NetMsg.item_id_selected = selected_item_id;
	NetMsg.item_selected_index = selected_index;
	NetMsg.wLevel = m_wLevel;
	NETSENDTOFIELD ( &NetMsg );

	m_item_box_pos_x = 0;
	m_item_box_pos_y = 0;

	return S_OK;
}

/*manual lunchbox remove, Juver, 2018/09/19 */
void GLCharacter::req_lunchbox_remove( SNATIVEID skill_id )
{
	BOOL exist = FALSE;
	for( int i=0; i<FITEMFACT_SIZE; ++i )
	{
		const SFITEMFACT& food_fact = m_sFITEMFACT[i];
		if ( food_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( food_fact.sNATIVEID == skill_id )	
			exist = TRUE;
	}

	if ( !exist )	return;

	if ( !RPARAM::allow_lunchbox_remove )	return;

	PGLSKILL pskill = GLSkillMan::GetInstance().GetData( skill_id );
	if ( !pskill )					return;
	if ( !pskill->IsSkillFact() )	return;

	std::string skill_name = pskill->GetName();
	m_lunchbox_remove_id = skill_id;

	CString question;
	question.Format( ID2GAMEINTEXT("SKILLFACT_ASK_RESET"), skill_name.c_str() );
	DoModal( question.GetString(),  MODAL_INFOMATION, YESNO, MODAL_LUNCHBOX_MANUAL_REMOVE );	
}

/*manual lunchbox remove, Juver, 2018/09/19 */
void GLCharacter::lunchbox_manual_remove()
{
	if ( !RPARAM::allow_lunchbox_remove )	return;

	PGLSKILL pskill = GLSkillMan::GetInstance().GetData( m_lunchbox_remove_id );
	if ( !pskill )					return;
	if ( !pskill->IsSkillFact() )	return;

	std::string skill_name = pskill->GetName();

	BOOL exist = FALSE;
	for( int i=0; i<FITEMFACT_SIZE; ++i )
	{
		const SFITEMFACT& food_fact = m_sFITEMFACT[i];
		if ( food_fact.sNATIVEID == NATIVEID_NULL() ) 
			continue;

		if ( food_fact.sNATIVEID == m_lunchbox_remove_id )	
			exist = TRUE;
	}

	if ( !exist )	return;

	GLMSG::SNETPC_REQ_LUNCHBOX_MANUAL_REMOVE net_msg;
	net_msg.skill_id = m_lunchbox_remove_id;
	NETSENDTOFIELD ( &net_msg );

	m_lunchbox_remove_id = NATIVEID_NULL().dwID;
}

///*hide costume, EJCode, 2018/11/18 */
//void GLCharacter::hide_costume_set()
//{
//	if ( !RPARAM::hide_costume_use )		return;
//
//	if ( m_hide_costume_timer < RPARAM::hide_costume_delay_time )
//	{
//		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_HIDE_COSTUME_DELAY_TIME") );
//		return;
//	}
//
//	GLMSG::SNETPC_HIDE_COSTUME	net_msg_server;
//	net_msg_server.hide_costume = !m_hide_costume;
//	NETSENDTOFIELD( &net_msg_server );
//
//	m_hide_costume_timer = 0.0f;
//
//	return;
//}

void GLCharacter::GMGetCrowTime( WORD wMobMID, WORD wMobSID, WORD wMapMID, WORD wMapSID, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_GET_CROW_TIME_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_GET_CROW_TIME_FB_INVALID_PASS") );
		return;
	}

	SCROWDATA* pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( wMobMID, wMobSID );
	if ( !pCrowData )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_GET_CROW_TIME_FB_INVALID_CROW") );
		return;
	}

	SMAPNODE* pMapNode = GLGaeaClient::GetInstance().FindMapNode( SNATIVEID( wMapMID, wMapSID ) );
	if ( !pMapNode )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_GET_CROW_TIME_FB_INVALID_MAP") );
		return;
	}

	GLMSG::SNETPC_GM_COMMAND_GET_CROW_TIME NetMsg;
	NetMsg.sMobID = pCrowData->m_sBasic.sNativeID;
	NetMsg.sMapID = pMapNode->sNativeID;
	StringCchCopy( NetMsg.szPass, MAX_PATH, strPass.c_str() );
	NETSENDTOFIELD ( &NetMsg );
}

void GLCharacter::GMEventContriTyranny( float fRate, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_INVALID_PASS") );
		return;
	}

	if ( fRate < 1.0f )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_INVALID_RATE") );
		return;
	}

	GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY NetMsg;
	NetMsg.fRate = fRate;
	StringCchCopy( NetMsg.szPass, MAX_PATH, strPass.c_str() );
	NETSEND(&NetMsg);
}

void GLCharacter::GMEventContriSchoolWar( float fRate, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_INVALID_PASS") );
		return;
	}

	if ( fRate < 1.0f )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_INVALID_RATE") );
		return;
	}

	GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR NetMsg;
	NetMsg.fRate = fRate;
	StringCchCopy( NetMsg.szPass, MAX_PATH, strPass.c_str() );
	NETSEND(&NetMsg);
}

void GLCharacter::GMEventContrinCaptureTheFlag( float fRate, std::string strPass )
{
	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_FAIL") );
		return;
	}

	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_INVALID_PASS") );
		return;
	}

	if ( fRate < 1.0f )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_INVALID_RATE") );
		return;
	}

	GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG NetMsg;
	NetMsg.fRate = fRate;
	StringCchCopy( NetMsg.szPass, MAX_PATH, strPass.c_str() );
	NETSEND(&NetMsg);
}

HRESULT GLCharacter::ReqCurrencyShopPurchase( SNATIVEID sNpcID, SNATIVEID sItemID )
{
	PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData( sNpcID );
	if ( !pCrowData )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CURRENCY_SHOP_PURCHASE_INVALID_CROW") );
		return E_FAIL;
	}

	SCURRENCY_SHOP_DATA* pShop = GLCrowDataMan::GetInstance().CurrencyShopFind( pCrowData->m_sAction.m_strCurrencyShop );
	if ( !pShop )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CURRENCY_SHOP_PURCHASE_INVALID_SHOP") );
		return E_FAIL;
	}

	SCURRENCY_SHOP_ITEM* pShopItem = pShop->FindData( sItemID );
	if ( !pShopItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CURRENCY_SHOP_PURCHASE_INVALID_SHOP_ITEM") );
		return E_FAIL;
	}

	SITEM* pItemData = GLItemMan::GetInstance().GetItem( pShopItem->sItemID );
	if ( !pItemData )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CURRENCY_SHOP_PURCHASE_INVALID_ITEM_DATA") );
		return E_FAIL;
	}

	volatile LONGLONG llPriceMoney = pShopItem->llPriceMoney;
	if ( m_lnMoney < llPriceMoney )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CURRENCY_SHOP_PURCHASE_NOT_ENOUGH_MONEY") );
		return E_FAIL;
	}

	volatile DWORD dwPriceGamePoints = pShopItem->dwPriceGamePoints;
	if ( m_dwGamePoints < dwPriceGamePoints )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CURRENCY_SHOP_PURCHASE_NOT_ENOUGH_GAME_POINTS") );
		return E_FAIL;
	}

	volatile DWORD dwPriceWarChips = pShopItem->dwPriceWarChips;
	if ( m_dwWarChips < dwPriceWarChips )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CURRENCY_SHOP_PURCHASE_NOT_ENOUGH_WAR_CHIPS") );
		return E_FAIL;
	}

	if ( m_fCurrencyShopDelayTimer < RPARAM::fCurrencyShopPurchaseDelayTime )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CURRENCY_SHOP_PURCHASE_DELAY") );
		return E_FAIL;
	}

	m_fCurrencyShopDelayTimer = 0.0f;

	WORD wInvenSizeX = pItemData->sBasicOp.wInvenSizeX;
	WORD wInvenSizeY = pItemData->sBasicOp.wInvenSizeY;

	//inventory space check
	BOOL bITEM_SPACE(FALSE);
	if ( pItemData->ISPILE() )
	{	
		WORD wPILENUM = pItemData->sDrugOp.wPileNum;
		WORD wREQINSRTNUM = ( 1 * pItemData->GETAPPLYNUM() );
		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, pItemData->sBasicOp.sNativeID, wPILENUM, wInvenSizeX, wInvenSizeY );
	}
	else
	{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( wInvenSizeX, wInvenSizeY, wInsertPosX, wInsertPosY );
	}

	if ( !bITEM_SPACE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CURRENCY_SHOP_PURCHASE_NOT_ENOUGH_INVEN_SPACE") );
		return E_FAIL;
	}

	GLMSG::SNETPC_REQ_CURRENCY_SHOP_PURCHASE NetMsg;
	NetMsg.sCrowID = sNpcID;
	NetMsg.sItemID = sItemID;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

void GLCharacter::GMKickOut( DWORD dwUserNum, float fTime, std::string strPass )
{
	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_KICKOUT_FB_WRONG_PASS") );
		return;
	}

	if ( fTime != 0.0f && fTime < RPARAM::fKickOutMinTime )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_KICKOUT_FB_TIME_LOW"), RPARAM::fKickOutMinTime );
		return;
	}

	if ( fTime > RPARAM::fKickOutMaxTime )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_KICKOUT_FB_TIME_HIGH"), RPARAM::fKickOutMaxTime );
		return;
	}

	GLMSG::SNETPC_GM_KICKOUT NetMsgAgent;
	NetMsgAgent.dwUserNum = dwUserNum;
	NetMsgAgent.fTime = fTime;
	StringCchCopy( NetMsgAgent.szPass, MAX_PATH, strPass.c_str() );
	NETSEND( &NetMsgAgent );
}

/* personal lock system, Juver, 2019/12/12 */
void GLCharacter::PersonalLockRequest( EMPERSONAL_LOCK emLock )
{
	if ( !RPARAM::bUsePersonalLock )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_PERSONAL_LOCK_REQUEST_FUNCTION_DISABLED") );
		return;
	}

	if ( emLock < 0 || emLock >= EMPERSONAL_LOCK_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_PERSONAL_LOCK_REQUEST_INVALID_LOCK_TYPE") );
		return;
	}

	GLMSG::SNETPC_PERSONAL_LOCK_REQUEST NetMsg;
	NetMsg.emLock = emLock;
	NETSENDTOFIELD ( &NetMsg );
}

/* personal lock system, Juver, 2020/01/27 */
void GLCharacter::PersonalLockCreatePin( EMPERSONAL_LOCK emLock, std::string strPin1, std::string strPin2 )
{
	if ( !RPARAM::bUsePersonalLock )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_FUNCTION_DISABLED") );
		return;
	}

	if ( emLock < 0 || emLock >= EMPERSONAL_LOCK_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_INVALID_LOCK_TYPE") );
		return;
	}

	if ( strPin1.empty() || strPin2.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_EMPTY") );
		return;
	}

	if ( strPin1.size() < PERSONAL_LOCK_PIN_SIZE || strPin1.size() > PERSONAL_LOCK_PIN_SIZE || 
		strPin2.size() < PERSONAL_LOCK_PIN_SIZE || strPin2.size() > PERSONAL_LOCK_PIN_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_SIZE") );
		return;
	}

	if ( strcmp( strPin1.c_str(), strPin2.c_str() ) != 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_MISMATCH") );
		return;
	}

	if ( strPin1.find_first_not_of( "0123456789" ) != std::string::npos || strPin2.find_first_not_of( "0123456789" ) != std::string::npos )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_NUMBER_ONLY") );
		return;
	}

	CString strTEMP1( strPin1.c_str() );
	CString strTEMP2( strPin2.c_str() );

	if ( STRUTIL::CheckString( strTEMP1 ) || STRUTIL::CheckString( strTEMP2 ) || 
		CRanFilter::GetInstance().NameFilter( strTEMP1 ) || CRanFilter::GetInstance().NameFilter( strTEMP2 ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_NUMBER_ONLY") );
		return;
	}

	GLMSG::SNETPC_PERSONAL_LOCK_CREATE_PIN net_msg;
	net_msg.emLock = emLock;
	StringCchCopy ( net_msg.szPin1, PERSONAL_LOCK_PIN_SIZE+1, strPin1.c_str() );
	StringCchCopy ( net_msg.szPin2, PERSONAL_LOCK_PIN_SIZE+1, strPin2.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/* personal lock system, Juver, 2020/01/27 */
void GLCharacter::PersonalLockInputPin( EMPERSONAL_LOCK emLock, std::string strPin )
{
	if ( !RPARAM::bUsePersonalLock )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_FUNCTION_DISABLED") );
		return;
	}

	if ( emLock < 0 || emLock >= EMPERSONAL_LOCK_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_INVALID_LOCK_TYPE") );
		return;
	}

	if ( strPin.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_EMPTY") );
		return;
	}

	if ( strPin.size() < PERSONAL_LOCK_PIN_SIZE || strPin.size() > PERSONAL_LOCK_PIN_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_SIZE") );
		return;
	}

	if ( strPin.find_first_not_of( "0123456789" ) != std::string::npos )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_NUMBER_ONLY") );
		return;
	}

	CString strTEMP( strPin.c_str() );

	if ( STRUTIL::CheckString( strTEMP ) || CRanFilter::GetInstance().NameFilter( strTEMP ) )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_NUMBER_ONLY") );
		return;
	}

	GLMSG::SNETPC_PERSONAL_LOCK_INPUT_PIN net_msg;
	net_msg.emLock = emLock;
	StringCchCopy ( net_msg.szPin, PERSONAL_LOCK_PIN_SIZE+1, strPin.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/* personal lock system, Juver, 2020/01/27 */
void GLCharacter::PersonalLockPinResetOpen( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RESET_NOT_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RESET_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PERSONAL_LOCK_RESET )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RESET_NOT_CARD") );
		return;
	}

	CInnerInterface::GetInstance().PersonalLockPinReset();
}

/* personal lock system, Juver, 2020/01/27 */
void GLCharacter::PersonalLockPinReset( EMPERSONAL_LOCK emLock )
{
	if ( !RPARAM::bUsePersonalLock )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RESET_FUNCTION_DISABLED") );
		return;
	}

	if ( emLock < 0 || emLock > EMPERSONAL_LOCK_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RESET_INVALID_LOCK_TYPE") );
		return;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_PERSONAL_LOCK_RESET );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RESET_NOT_CARD") );
		return;
	}

	GLMSG::SNETPC_PERSONAL_LOCK_PIN_RESET net_msg;
	net_msg.emLock = emLock;
	NETSENDTOFIELD ( &net_msg );
}

/* personal lock system, Juver, 2020/01/24 */
void GLCharacter::PersonalLockPinChangeOpen( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_NOT_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PERSONAL_LOCK_CHANGE_PIN )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_NOT_CARD") );
		return;
	}

	CInnerInterface::GetInstance().PersonalLockPinChange();
}


/* personal lock system, Juver, 2020/01/27 */
void GLCharacter::PersonalLockPinChange( EMPERSONAL_LOCK emLock, std::string strPinOld, std::string strPinNew, std::string strPinNew2 )
{
	if ( !RPARAM::bUsePersonalLock )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_FUNCTION_DISABLED") );
		return;
	}

	if ( emLock < 0 || emLock >= EMPERSONAL_LOCK_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_INVALID_LOCK_TYPE") );
		return;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_PERSONAL_LOCK_CHANGE_PIN );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_NOT_CARD") );
		return;
	}

	if ( strPinOld.empty() || strPinNew.empty() || strPinNew2.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_EMPTY") );
		return;
	}

	if ( strPinOld.size() < PERSONAL_LOCK_PIN_SIZE || strPinOld.size() > PERSONAL_LOCK_PIN_SIZE ||
		strPinNew.size() < PERSONAL_LOCK_PIN_SIZE || strPinNew.size() > PERSONAL_LOCK_PIN_SIZE ||
		strPinNew2.size() < PERSONAL_LOCK_PIN_SIZE || strPinNew2.size() > PERSONAL_LOCK_PIN_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_SIZE") );
		return;
	}

	if ( strPinOld.find_first_not_of( "0123456789" ) != std::string::npos ||
		strPinNew.find_first_not_of( "0123456789" ) != std::string::npos ||
		strPinNew2.find_first_not_of( "0123456789" ) != std::string::npos )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_NUMBER_ONLY") );
		return;
	}

	if ( strcmp( strPinNew.c_str(), strPinNew2.c_str() ) != 0 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_MISMATCH") );
		return;
	}

	GLMSG::SNETPC_PERSONAL_LOCK_PIN_CHANGE net_msg;
	net_msg.emLock = emLock;
	StringCchCopy ( net_msg.szPinOld, PERSONAL_LOCK_PIN_SIZE+1, strPinOld.c_str() );
	StringCchCopy ( net_msg.szPinNew, PERSONAL_LOCK_PIN_SIZE+1, strPinNew.c_str() );
	StringCchCopy ( net_msg.szPinNew2, PERSONAL_LOCK_PIN_SIZE+1, strPinNew2.c_str() );
	NETSENDTOFIELD ( &net_msg );
}

/* personal lock system, Juver, 2020/01/31 */
void GLCharacter::PersonalLockPinRecoverOpen( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RECOVER_NOT_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RECOVER_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PERSONAL_LOCK_RECOVER_PIN )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RECOVER_NOT_CARD") );
		return;
	}

	CInnerInterface::GetInstance().PersonalLockPinRecover();
}

/* personal lock system, Juver, 2020/01/31 */
void GLCharacter::PersonalLockPinRecover( EMPERSONAL_LOCK emLock )
{
	if ( !RPARAM::bUsePersonalLock )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RECOVER_FUNCTION_DISABLED") );
		return;
	}

	if ( emLock < 0 || emLock > EMPERSONAL_LOCK_SIZE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RECOVER_INVALID_LOCK_TYPE") );
		return;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_PERSONAL_LOCK_RECOVER_PIN );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_PERSONAL_LOCK_PIN_RECOVER_NOT_CARD") );
		return;
	}

	GLMSG::SNETPC_PERSONAL_LOCK_PIN_RECOVER net_msg;
	net_msg.emLock = emLock;
	NETSENDTOFIELD ( &net_msg );
}

/* party finder, Juver, 2020/01/15 */
void GLCharacter::PartyFinderSearch( DWORD dwIndex )
{
	if ( !RPARAM::bUsePartyFinder )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_FUNCTION_DISABLED") );
		return;
	}

	if ( dwIndex == SPARTY_FINDER_SEARCH_INDEX_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_INVALID_TYPE") );
		return;
	}

	SPARTY_FINDER_MAP_DATA* pData = GLPartyFinder::GetInstance().GetData( dwIndex );
	if ( !pData )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_INVALID_TYPE") );
		return;
	}

	if ( m_fPartyFinderSearchDelayTimer < RPARAM::fPartyFinderSearchDelay )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_REQUIRE_TIME") );
		return;
	}

	if ( IsDie() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_IS_DEAD") );
		return;
	}

	m_fPartyFinderSearchDelayTimer = 0.0f;

	GLMSG::SNETPC_PARTY_FINDER_SEARCH NetMsg;
	NetMsg.dwIndex = dwIndex;
	NETSEND ( &NetMsg);
}

/* party finder, Juver, 2020/01/15 */
void GLCharacter::PartyFinderJoinRequest( DWORD dwPartyID )
{
	if ( !RPARAM::bUsePartyFinder )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_FUNCTION_DISABLED") );
		return;
	}

	if ( dwPartyID == PARTY_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_INVALID_PARTY") );
		return;
	}

	if ( m_fPartyFinderJoinDelayTimer < RPARAM::fPartyFinderJoinDelay )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_REQUIRE_TIME") );
		return;
	}

	if ( IsDie() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_IS_DEAD") );
		return;
	}

	m_fPartyFinderJoinDelayTimer = 0.0f;

	GLMSG::SNETPC_PARTY_FINDER_JOIN NetMsg;
	NetMsg.dwPartyID = dwPartyID;
	NetMsg.wLevel = m_wLevel;
	NETSEND ( &NetMsg);
}

void GLCharacter::PartyFinderQuestionAccept( DWORD dwPartyID, DWORD dwCharID )
{
	GLMSG::SNETPC_PARTY_FINDER_REPLY NetMsg;
	NetMsg.dwPartyID = dwPartyID;
	NetMsg.dwCharID = dwCharID;
	NetMsg.bAccept = TRUE;
	NETSEND ( &NetMsg);
}

void GLCharacter::PartyFinderQuestionDecline( DWORD dwPartyID, DWORD dwCharID )
{
	GLMSG::SNETPC_PARTY_FINDER_REPLY NetMsg;
	NetMsg.dwPartyID = dwPartyID;
	NetMsg.dwCharID = dwCharID;
	NetMsg.bAccept = FALSE;
	NETSEND ( &NetMsg);
}

void GLCharacter::GMPCIDGet( DWORD dwType, DWORD dwCharID, std::string strPass )
{
	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_WRONG_PASS") );
		return;
	}

	GLMSG::SNETPC_GM_PCID NetMsgAgent;
	NetMsgAgent.dwType = dwType;
	NetMsgAgent.dwCharID = dwCharID;
	StringCchCopy( NetMsgAgent.szPass, MAX_PATH, strPass.c_str() );
	NETSEND( &NetMsgAgent );
}

/* user flag restricted, Juver, 2020/04/21 */
void GLCharacter::GMUserRestrict( DWORD dwCharID, bool bRestrict, std::string strPass )
{
	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_USER_RESTRICT_FB_WRONG_PASS") );
		return;
	}

	GLMSG::SNETPC_GM_USER_RESTRICT NetMsgAgent;
	NetMsgAgent.dwCharID = dwCharID;
	NetMsgAgent.bRestrict = bRestrict;
	StringCchCopy( NetMsgAgent.szPass, MAX_PATH, strPass.c_str() );
	NETSEND( &NetMsgAgent );
}

/* gm command inven clear, Juver, 2020/05/09 */
void GLCharacter::GMInvenClear( std::string strPass )
{
	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_INVEN_CLEAR_FB_INVALID_PASS") );
		return;
	}

	GLMSG::SNETPC_GM_INVEN_CLEAR NetMsgField;
	StringCchCopy( NetMsgField.szPass, MAX_PATH, strPass.c_str() );
	NETSENDTOFIELD( &NetMsgField );
}

/* camera zoom gm command, Juver, 2020/06/10 */
void GLCharacter::GMCameraZoom( float fValue )
{
	

	if( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_CZ_FB_NOT_GM") );
		return;
	}

	if( fValue < 0.0f )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_CZ_FB_INVALID_VALUE") );
		return;
	}

	if ( fValue == 0.0f )
		RENGINE_GLOBAL::CameraZoomMultiplierReset();
	else
		RENGINE_GLOBAL::CameraZoomMultiplierSet( fValue );
	
	if( m_bVehicle )
		DxViewPort::GetInstance().SetVehicleCamera();
	else
		DxViewPort::GetInstance().SetGameCamera();

	DxFogMan::GetInstance().SetRangeMultiplier( RENGINE_GLOBAL::fCameraZoomMultiplier );

	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_CZ_FB_DONE") );

	
}

/* game notice, Juver, 2021/06/12 */
void GLCharacter::GMGameNoticeReload( std::string strPass )
{
	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_GAME_NOTICE_RELOAD_FB_INVALID_PASS") );
		return;
	}

	GLMSG::SNETPC_GM_GAME_NOTICE_RELOAD NetMsgAgent;
	StringCchCopy( NetMsgAgent.szPass, MAX_PATH, strPass.c_str() );
	NETSEND( &NetMsgAgent );
}

/* variable check, Juver, 2021/07/02 */
void GLCharacter::VariableCheck()
{
	if ( IsValidBody() == FALSE )	return;

#ifdef _DEBUG
	//CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, "variable check" );
#endif // _DEBUG
	

	GLMSG::SNET_VARIABLE_CHECK_RESULT NetMsgField;
	NetMsgField.emCheck = EMVARIABLE_CHECK_NONE;

	//stun state
	EnterCriticalSection(&m_STATE_STUN_Lock);

	if ( ( m_cSTATE_STUN_CHECK.GetInt() >= 1 && m_bSTATE_STUN_ORIG == false ) || 
		( m_cSTATE_STUN_CHECK.GetInt() == 0 && m_bSTATE_STUN_ORIG == true ) )
	{
		NetMsgField.emCheck = EMVARIABLE_CHECK_STUN;
		NetMsgField.fVar1 = float( m_bSTATE_STUN_ORIG );
		NetMsgField.fVar2 = float( m_cSTATE_STUN_CHECK.GetInt() );
		NETSENDTOFIELD( &NetMsgField );
	}

	LeaveCriticalSection(&m_STATE_STUN_Lock);

	//pierce state
	EnterCriticalSection(&m_SUM_PIERCE_Lock);

	if ( m_nSUM_PIERCE_ORIG != m_cSUM_PIERCE_CHECK.GetInt() )
	{
		NetMsgField.emCheck = EMVARIABLE_CHECK_PIERCE;
		NetMsgField.fVar1 = float( m_nSUM_PIERCE_ORIG );
		NetMsgField.fVar2 = float( m_cSUM_PIERCE_CHECK.GetInt() );
		NETSENDTOFIELD( &NetMsgField );
	}

	LeaveCriticalSection(&m_SUM_PIERCE_Lock);


	//target range state
	EnterCriticalSection(&m_SUM_TARGET_RANGE_Lock);

	//difference more than 1
	if ( std::abs( m_fSUM_TARGET_RANGE_ORIG - m_cSUM_TARGET_RANGE_CHECK.GetFloat() ) >= 1.0f )
	{
		NetMsgField.emCheck = EMVARIABLE_CHECK_TARGET_RANGE;
		NetMsgField.fVar1 = float( m_fSUM_TARGET_RANGE_ORIG );
		NetMsgField.fVar2 = float( m_cSUM_TARGET_RANGE_CHECK.GetFloat() );
		NETSENDTOFIELD( &NetMsgField );
	}

	LeaveCriticalSection(&m_SUM_TARGET_RANGE_Lock);

	
	//attack range state
	EnterCriticalSection(&m_SUM_ATTACK_RANGE_Lock);

	//difference more than 1
	if ( std::abs( m_fSUM_ATTACK_RANGE_ORIG - m_cSUM_ATTACK_RANGE_CHECK.GetFloat() ) >= 1.0f )
	{
		NetMsgField.emCheck = EMVARIABLE_CHECK_ATTACK_RANGE;
		NetMsgField.fVar1 = float( m_fSUM_ATTACK_RANGE_ORIG );
		NetMsgField.fVar2 = float( m_cSUM_ATTACK_RANGE_CHECK.GetFloat() );
		NETSENDTOFIELD( &NetMsgField );
	}

	LeaveCriticalSection(&m_SUM_ATTACK_RANGE_Lock);


	//apply range state
	EnterCriticalSection(&m_SUM_APPLY_RANGE_Lock);

	//difference more than 1
	if ( std::abs( m_fSUM_APPLY_RANGE_ORIG - m_cSUM_APPLY_RANGE_CHECK.GetFloat() ) >= 1.0f )
	{
		NetMsgField.emCheck = EMVARIABLE_CHECK_APPLY_RANGE;
		NetMsgField.fVar1 = float( m_fSUM_APPLY_RANGE_ORIG );
		NetMsgField.fVar2 = float( m_cSUM_APPLY_RANGE_CHECK.GetFloat() );
		NETSENDTOFIELD( &NetMsgField );
	}

	LeaveCriticalSection(&m_SUM_APPLY_RANGE_Lock);
}

/* chaos machine, Juver, 2021/07/08 */
bool GLCharacter::ValidChaosMachineOpen()
{
	if ( CInnerInterface::GetInstance().IsVisibleGroup( CHAOS_MACHINE_INVEN_WINDOW ) 
		|| CInnerInterface::GetInstance().IsVisibleGroup( CHAOS_MACHINE_WINDOW ) )
		return true;
	return false;
}

/* chaos machine, Juver, 2021/07/08 */
void GLCharacter::ResetChaosMachineItem()
{
	m_sChaosMachineItemPos.RESET();
	m_sPreInventoryItem.RESET();

	m_dwChaosMachineNPCID = 0;
}

/* chaos machine, Juver, 2021/07/09 */
void GLCharacter::SetChaosItemMoveMainItem()
{
	if( m_sPreInventoryItem.VALID() )
	{
		if (m_sChaosMachineItemPos.wPosX == m_sPreInventoryItem.wPosX &&
			m_sChaosMachineItemPos.wPosY == m_sPreInventoryItem.wPosY )
		{
			m_sPreInventoryItem.RESET();
			return;
		}

		m_sChaosMachineItemPos.SET( m_sPreInventoryItem.wPosX, m_sPreInventoryItem.wPosY );
		m_sPreInventoryItem.RESET();
	}
}

void GLCharacter::ResetChaosItemMoveMainItem()
{
	m_sChaosMachineItemPos.RESET();
}

/* chaos machine, Juver, 2021/07/09 */
const SITEMCUSTOM GLCharacter::GetChaosMachineItem()
{
	SITEMCUSTOM sItemCustom;
	sItemCustom.sNativeID = NATIVEID_NULL();

	if( !m_sChaosMachineItemPos.VALID() )	return sItemCustom;

	SINVENITEM* pRegistItem = m_cInventory.GetItem( m_sChaosMachineItemPos.wPosX, m_sChaosMachineItemPos.wPosY );
	if( !pRegistItem )	return sItemCustom;

	sItemCustom = pRegistItem->sItemCustom;
	return sItemCustom;
}

/* chaos machine, Juver, 2021/07/09 */
void GLCharacter::ChaosMachineStart()
{
	const SITEMCUSTOM sItemCustom = GetChaosMachineItem();

	SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCustom.sNativeID );
	if ( !pItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_INVALID_ITEM") );
		return;
	}

	SCHAOS_MACHINE_DATA* pChaosMachineData = GLChaosMachine::GetInstance().GetData( pItem->sBasicOp.dwChaosMachineID );
	if ( !pChaosMachineData )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_INVALID_ITEM") );
		return;
	}

	if ( sItemCustom.IsGM_GENITEM() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_NON_DROP") );
		return;
	}

	if ( pChaosMachineData->llCost != 0 && m_lnMoney < pChaosMachineData->llCost ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_COST") );
		return;
	}


	WORD winvensizex = pItem->sBasicOp.wInvenSizeX;
	WORD winvensizey = pItem->sBasicOp.wInvenSizeY;

	//inventory space check
	BOOL bITEM_SPACE(FALSE);
	if ( pItem->ISPILE() )
	{	
		WORD wPILENUM = pItem->sDrugOp.wPileNum;
		WORD wREQINSRTNUM = pItem->GETAPPLYNUM();
		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, pItem->sBasicOp.sNativeID, wPILENUM, winvensizex, winvensizey );
	}
	else
	{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );
	}

	if ( !bITEM_SPACE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_NO_INVEN_SPACE") );
		return;
	}

	//requirements check
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
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_NO_MATERIAL") );
					return;
				}
			}
			else
			{
				//not pile just check if exist
				SINVENITEM* pinvenitem = m_cInventory.FindItem( pItemReq->sBasicOp.sNativeID );
				if ( !pinvenitem )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAOS_MACHINE_NO_MATERIAL") );
					return;
				}
			}
		}
	}

	GLMSG::SNETPC_INVEN_CHAOS_MACHINE net_msg;
	net_msg.sInvenItem = m_sChaosMachineItemPos;
	net_msg.dwNPCID = m_dwChaosMachineNPCID;
	NETSENDTOFIELD ( &net_msg );

	//ResetChaosMachineItem();
}

/* set item option, Juver, 2021/09/04 */
void GLCharacter::ReqSetOptionInsert ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return;
	if ( GLTradeClient::GetInstance().Valid() )	return;

	SINVENITEM* ptarget_inven_item = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !ptarget_inven_item )	
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_INSERT_INVALID_TARGET_ITEM") );
		return;
	}

	wPosX = ptarget_inven_item->wPosX;
	wPosY = ptarget_inven_item->wPosY;

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( ptarget_inven_item->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_INSERT_INVALID_TARGET_ITEM") );
		return;
	}

	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pitem_data_hold )	
		return;

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_SET_OPTION_INSERT )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_INSERT_INVALID_CARD_ITEM") );
		return;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{	
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_INSERT_TARGET_NOT_SUIT") );
		return;
	}

	if ( pitem_data_target->sBasicOp.strGenSetOption.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_INSERT_TARGET_NOT_SET_OPTION") );
		return;
	}

	if ( ptarget_inven_item->sItemCustom.wSetItemID != SET_OPTION_NULL )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_INSERT_TARGET_ALREADY") );
		return;
	}

	GLMSG::SNET_INVEN_SET_OPTION_INSERT net_msg_field;
	net_msg_field.wPosX = wPosX;
	net_msg_field.wPosY = wPosY;

	NETSENDTOFIELD ( &net_msg_field );
}

/* set item option, Juver, 2021/09/04 */
void GLCharacter::ReqSetOptionReroll ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return;
	if ( GLTradeClient::GetInstance().Valid() )	return;

	SINVENITEM* ptarget_inven_item = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !ptarget_inven_item )	
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_REROLL_INVALID_TARGET_ITEM") );
		return;
	}

	wPosX = ptarget_inven_item->wPosX;
	wPosY = ptarget_inven_item->wPosY;

	SITEM* pitem_data_target = GLItemMan::GetInstance().GetItem ( ptarget_inven_item->sItemCustom.sNativeID );
	if ( !pitem_data_target )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_REROLL_INVALID_TARGET_ITEM") );
		return;
	}

	SITEM* pitem_data_hold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pitem_data_hold )	
		return;

	if ( pitem_data_hold->sBasicOp.emItemType != ITEM_SET_OPTION_REROLL )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_REROLL_INVALID_CARD_ITEM") );
		return;
	}

	if ( pitem_data_target->sBasicOp.emItemType != ITEM_SUIT )
	{	
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_REROLL_TARGET_NOT_SUIT") );
		return;
	}

	if ( pitem_data_target->sBasicOp.strGenSetOption.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SET_OPTION_REROLL_TARGET_NOT_SET_OPTION") );
		return;
	}

	GLMSG::SNET_INVEN_SET_OPTION_REROLL net_msg_field;
	net_msg_field.wPosX = wPosX;
	net_msg_field.wPosY = wPosY;

	NETSENDTOFIELD ( &net_msg_field );
}

HRESULT GLCharacter::ReqNpcContributionShopPurchaseReset()
{
	m_sNPCContributionShopCrow = NATIVEID_NULL();
	m_sNPCContributionShopItem = NATIVEID_NULL();

	return S_OK;
}

HRESULT GLCharacter::ReqNpcContributionShopPurchase( SNATIVEID sidCrow, SNATIVEID sidItem )
{
	if ( m_fNPCContributionShopDelay < NPC_SHOP_DELAY )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_DELAY") );
		return E_FAIL;
	}

	SITEM* pitem = GLItemMan::GetInstance().GetItem ( sidItem );
	if ( !pitem )	
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_INVALID_ITEM") );
		return E_FAIL;
	}

	SCROWDATA*	pcrow = GLCrowDataMan::GetInstance().GetCrowData( sidCrow );
	if ( !pcrow )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_INVALID_CROW") );
		return E_FAIL;
	}

	SNPC_CONTRIBUTION_SHOP_DATA* pNpcShop = GLCrowDataMan::GetInstance().NPCContributionShopFind( pcrow->m_sAction.m_strContributionShop );
	if ( !pNpcShop )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_INVALID_SHOP") );
		return E_FAIL;
	}

	const SNPC_CONTRIBUTION_SHOP_ITEM* pshopitem = pNpcShop->FindItem( sidItem.dwID );
	if ( !pshopitem )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_INVALID_SHOPITEM") );
		return E_FAIL;
	}

	//player contribution check
	if ( m_dwContributionPoint < pshopitem->dwContriPrice )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_NOT_CONTRIBUTION") );
		return E_FAIL;
	}

	//gold check
	volatile DWORD dwGoldPrice = pshopitem->dwGoldPrice;
	if ( m_lnMoney < dwGoldPrice )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_NOT_GOLD") );
		return E_FAIL;
	}

	/*contribution point, Juver, 2017/08/23 */
	if ( pitem->sBasicOp.dwReqContributionPoint != 0 &&
		m_dwContributionPoint < pitem->sBasicOp.dwReqContributionPoint )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_NOCONTRIBUTIONPOINT") );
		return E_FAIL;
	}

	/*activity point, Juver, 2017/08/23 */
	if ( pitem->sBasicOp.dwReqActivityPoint != 0 && 
		m_dwActivityPoint < pitem->sBasicOp.dwReqActivityPoint )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_NOPOINT") );
		return E_FAIL;
	}

	/* play time system, Juver, 2021/01/27 */
	if ( pitem->sBasicOp.llPlayTimeReq != 0 && m_llPlayTime < pitem->sBasicOp.llPlayTimeReq )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BUYITEM_PLAY_TIME") );
		return E_FAIL;
	}

	WORD winvensizex = pitem->sBasicOp.wInvenSizeX;
	WORD winvensizey = pitem->sBasicOp.wInvenSizeY;

	//inventory space check
	BOOL bITEM_SPACE(FALSE);
	if ( pitem->ISPILE() )
	{	
		WORD wPILENUM = pitem->sDrugOp.wPileNum;
		WORD wREQINSRTNUM = ( 1 * pitem->GETAPPLYNUM() );
		bITEM_SPACE = m_cInventory.ValidPileInsrt ( wREQINSRTNUM, sidItem, wPILENUM, winvensizex, winvensizey );
	}else{
		WORD wInsertPosX(0), wInsertPosY(0);
		bITEM_SPACE = m_cInventory.FindInsrtable ( winvensizex, winvensizey, wInsertPosX, wInsertPosY );
	}

	if ( !bITEM_SPACE )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_CONTRIBUTION_SHOP_PURCHASE_FB_NOT_INVEN_SPACE") );
		return E_FAIL;
	}


	m_sNPCContributionShopCrow = sidCrow;
	m_sNPCContributionShopItem = sidItem;

	if ( RGAME_GLOBAL::bNPCContributionShopConfirm )
	{
		CString strQuestion = CInnerInterface::GetInstance().MakeString( ID2GAMEINTEXT("NPC_CONTRIBUTION_SHOP_PURCHASE_QUESTION"), pitem->GetName() );
		DoModal( strQuestion.GetString(),  MODAL_INFOMATION, YESNO, MODAL_NPC_CONTRIBUTION_SHOP_PURCHASE );		
	}
	else
	{
		ReqNpcContributionShopDoPurchase();
	}

	return S_OK;
}

HRESULT GLCharacter::ReqNpcContributionShopDoPurchase()
{
	GLMSG::SNETPC_REQ_NPC_CONTRIBUTION_SHOP_PURCHASE NetMsg;
	NetMsg.sidCrow = m_sNPCContributionShopCrow;
	NetMsg.sidItem = m_sNPCContributionShopItem;
	NETSENDTOFIELD ( &NetMsg );

	m_fNPCContributionShopDelay = 0.0f;

	return S_OK;
}

void GLCharacter::ReqMaxRVShow(const SITEMCUSTOM& sItemCustom)
{
	if ( RPARAM::bMaxRvPreviewEnable == FALSE )
		return;

	SITEM* pItem = GLItemMan::GetInstance().GetItem(sItemCustom.sNativeID);
	if ( !pItem )
	{
		//invalid item
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMMAXRV_PREVIEW_FB_INVALID_ITEM") );
		return;
	}

	if ( std::string(pItem->GetRandomOptFile()).empty() )
	{
		//item does not have valid random option file
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMMAXRV_PREVIEW_FB_INVALID_ITEM_RV") );
		return;
	}

	if ( m_fMaxRVPreviewTimer < MAX_RV_PREVIEW_COOLTIME )
	{
		//preview in cooldown
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMMAXRV_PREVIEW_FB_COOLTIME") );
		return;
	}

	GLMSG::SNETPC_REQ_MAXRV_PREVIEW NetMsg;
	NetMsg.sID = sItemCustom.sNativeID;
	NETSENDTOFIELD ( &NetMsg );

	m_sMaxRvPreview.Reset();
	m_sMaxRvPreview.m_sItemCustom = sItemCustom;

	m_fMaxRVPreviewTimer = 0.0f;
}

void GLCharacter::GMGISReload( std::string strPass )
{
	if ( strPass.empty() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_GIS_RELOAD_FB_INVALID_PASS") );
		return;
	}

	GLMSG::SNETPC_GM_GIS_RELOAD NetMsgAgent;
	StringCchCopy( NetMsgAgent.szPass, MAX_PATH, strPass.c_str() );
	NETSEND( &NetMsgAgent );
}

HRESULT GLCharacter::InvenUseCardRebornA( WORD wPosX, WORD wPosY )
{
	if ( RPARAM::bRBSystem == FALSE )
		return E_FAIL;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	
		return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )	
		return E_FAIL;

	CInnerInterface::GetInstance().OpenCardRebornA( pInvenItem->wPosX, pInvenItem->wPosY );

	return S_OK;
}

HRESULT GLCharacter::InvenUseCardRebornB( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REBORN_CARD_POS_ERROR") );	
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )	
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REBORN_CARD_ITEM_ERROR") );	
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_REBORD_CARD_B )		
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REBORN_CARD_ITEMTYPE_ERROR") );	
		return E_FAIL;
	}

	GLMSG::SNETPC_CARD_REBORN_B NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}
/*self buff via NPC, DevArt22, 2023/03/18 */
HRESULT GLCharacter::ReqSelfBuffBAOnly ( DWORD dwNpcID, DWORD dwGlobalID )
{
	GLMSG::SNETPC_REQ_SELFBUFF_BA_ONLY NetMsg;

	PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap ();
	PGLCROWCLIENT pCrow = pLandMan->GetCrow ( dwNpcID );

	if ( !pCrow )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SELFBUFF_FAIL") );
		return E_FAIL;
	}

	if ( pCrow->GETCROW() != CROW_NPC )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SELFBUFF_FAIL") );
		return E_FAIL;
	}

	//	Note : ?? ??.
	NetMsg.dwNpcID = dwNpcID;
	NetMsg.dwGlobalID = dwGlobalID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

/*self buff via NPC, DevArt22, 2023/03/18 */
HRESULT GLCharacter::ReqSelfBuffFull ( DWORD dwNpcID, DWORD dwGlobalID )
{
	GLMSG::SNETPC_REQ_SELFBUFF_FULL NetMsg;

	PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap ();
	PGLCROWCLIENT pCrow = pLandMan->GetCrow ( dwNpcID );

	if ( !pCrow )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SELFBUFF_FAIL") );
		return E_FAIL;
	}

	if ( pCrow->GETCROW() != CROW_NPC )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SELFBUFF_FAIL") );
		return E_FAIL;
	}

	//	Note : ?? ??.
	NetMsg.dwNpcID = dwNpcID;
	NetMsg.dwGlobalID = dwGlobalID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

/*self buff via NPC, DevArt22, 2023/03/18 */
HRESULT GLCharacter::ReqSelfBuffFullNoBA ( DWORD dwNpcID, DWORD dwGlobalID )
{
	GLMSG::SNETPC_REQ_SELFBUFF_FULL_NO_BA NetMsg;

	PLANDMANCLIENT pLandMan = GLGaeaClient::GetInstance().GetActiveMap ();
	PGLCROWCLIENT pCrow = pLandMan->GetCrow ( dwNpcID );

	if ( !pCrow )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SELFBUFF_FAIL") );
		return E_FAIL;
	}

	if ( pCrow->GETCROW() != CROW_NPC )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_SELFBUFF_FAIL") );
		return E_FAIL;
	}

	//	Note : ?? ??.
	NetMsg.dwNpcID = dwNpcID;
	NetMsg.dwGlobalID = dwGlobalID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}
HRESULT GLCharacter::ReqRebornA( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REBORN_CARD_POS_ERROR") );	
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )	
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REBORN_CARD_ITEM_ERROR") );	
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_REBORD_CARD_A )		
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REBORN_CARD_ITEMTYPE_ERROR") );	
		return E_FAIL;
	}

	GLMSG::SNETPC_CARD_REBORN_A NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

HRESULT GLCharacter::InvenUseCardExchangeItem( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	
		return E_FAIL;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )	
		return E_FAIL;

	CInnerInterface::GetInstance().OpenExchangeItemWindow( pInvenItem->wPosX, pInvenItem->wPosY );

	return S_OK;
}

HRESULT GLCharacter::InvenUseCardExchangeItemPoint( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEXCHANGE_ITEM_POINT_CARD_ERROR") );	
		return E_FAIL;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )	
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEXCHANGE_ITEM_POINT_CARD_ERROR") );	
		return E_FAIL;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_EXCHANGE_ITEM_POINT )		
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEXCHANGE_ITEM_POINT_CARD_ERROR") );	
		return E_FAIL;
	}

	if ( pItem->sSuitOp.wReModelNum == 0 )
	{
		CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEXCHANGE_ITEM_POINT_CARD_ERROR") );	
		return E_FAIL;
	}

	GLMSG::SNETPC_REQ_EXCHANGE_ITEM_CARD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

void GLCharacter::CaptchaInput(DWORD dwCaptcha)
{
	GLMSG::SNET_CAPTCHA_UPDATE_INPUT_CA NetMsgAgent;
	NetMsgAgent.dwCaptcha = dwCaptcha;
	NETSEND( &NetMsgAgent );
}

void GLCharacter::CaptchaReset()
{
	//reset captcha related variables
	m_bCaptchaWait = false;
	m_fCaptchaWaitTimer = 0.0f;
}

HRESULT GLCharacter::ReqBattleRanking()
{
	GLMSG::SNET_REQ_BATTLE_RANKING NetMsg;
	NetMsg.nPage = CInnerInterface::GetInstance().GetBattleRankingDisplay()->GetCurrentTabPage();
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}
HRESULT GLCharacter::ReqMyBattleRanking()
{
	GLMSG::SNET_REQ_MY_BATTLE_RANKING NetMsg;
	NetMsg.nPage = CInnerInterface::GetInstance().GetBattleRankingDisplay()->GetCurrentTabPage();
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}


/*dmk14 send notif less data*/
HRESULT GLCharacter::ReqTogglePlayerKillDisp( BOOL bDisplay )
{
	HRESULT hr = S_OK;
	
	GLMSG::SNETPC_REQ_RECMSG_NOTIF NetMsg;
	NetMsg.bRecMsg = bDisplay;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

//Item Card Extend Costume Expiration
HRESULT GLCharacter::ReqDisguiseExtend ( WORD wPosX, WORD wPosY )
{
	if ( !IsValidBody() )						return E_FAIL;
	if ( GLTradeClient::GetInstance().Valid() )	return E_FAIL;

	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem )			return E_FAIL;

	SITEM* pHold = GET_SLOT_ITEMDATA ( SLOT_HOLD );
	if ( !pHold )	return S_FALSE;

	if ( pHold->sBasicOp.emItemType != ITEM_CARD_COSTUME_EXTENDER )	return S_FALSE;

	if ( !pItem->sBasicOp.IsDISGUISE() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_EXTEND_FB_NOT_COSTUME") );
		return S_FALSE;
	}

	if ( !pItem->IsTIMELMT() )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_EXTEND_FB_NOT_TIMELIMIT") );
		return S_FALSE;
	}

	GLMSG::SNET_INVEN_DISGUISE_EXTEND	NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );

	return S_OK;
}

/*12-9-14, Battle Pass - CNDev*/
void GLCharacter::ReqBattlePassPremiumCard( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_PREMIUM_CARD_FAIL") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_PREMIUM_CARD_INVALID") );
		return;
	}

	if ( m_bBattlePassPremium )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_PREMIUM_CARD_ALREADY") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_BATTLEPASS_PREMIUM_CARD )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_PREMIUM_CARD_NOTCARD") );
		return;
	}

	if ( m_fBattlePassDelay < BATTLEPASS_DELAY )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BATTLEPASS_DELAY") );
		return;
	}

	GLMSG::SNETPC_REQ_BATTLEPASS_PREMIUM_CARD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );
}

void GLCharacter::ReqBattlePassLevelUpCard( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_FAIL") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_INVALID") );
		return;
	}

	/*if ( !m_bBattlePassPremium )
	{
	CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_NOTPREMIUM") );
	return;
	}*/

	if ( m_dwBattlePassLevel >= BATTLEPASS_LEVEL_MAX )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_MAX") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_BATTLEPASS_LEVELUP_CARD )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_LEVELUP_CARD_NOTCARD") );
		return;
	}
	
	if ( m_fBattlePassDelay < BATTLEPASS_DELAY )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BATTLEPASS_DELAY") );
		return;
	}

	GLMSG::SNETPC_REQ_BATTLEPASS_LEVELUP_CARD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD ( &NetMsg );
}

HRESULT GLCharacter::ReqBattlePassRewardClaim( DWORD dwLevel, bool bPremium )
{
	BATTLEPASS_LEVEL_REWARD_MAP_ITER rewardIter = GLBattlePassReward::GetInstance().m_mapBattlePassLevelRewardMap.find(dwLevel);
	if (rewardIter == GLBattlePassReward::GetInstance().m_mapBattlePassLevelRewardMap.end()) 
	{
		CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_REWARD_CLAIM_FAIL"));
		return E_FAIL;
	}

	SBATTLEPASS_LEVEL_REWARD& sRewardData = rewardIter->second;

	if (bPremium && !m_bBattlePassPremium) 
	{
		CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_REWARD_CLAIM_NOTPREMIUM"));
		return E_FAIL;
	}

	SNATIVEID sRewardID = bPremium ? sRewardData.sPremiumRewardID : sRewardData.sNormalRewardID;

	SITEM* pItemReward = GLItemMan::GetInstance().GetItem( sRewardID );
	if ( !pItemReward )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_BATTLEPASS_REWARD_CLAIM_NOTREWARD") );
		return E_FAIL;
	}

	if ( m_fBattlePassDelay < BATTLEPASS_DELAY )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("BATTLEPASS_DELAY") );
		return E_FAIL;
	}

	GLMSG::SNETPC_REQ_BATTLEPASS_REWARD_CLAIM NetMsg;
	NetMsg.dwLevel = dwLevel;
	NetMsg.bPremium = bPremium;
	NETSEND ( &NetMsg );

	return S_OK;
}
/*12-16-24, daily login - CNDev*/
HRESULT GLCharacter::ReqDailyLoginRewardClaim( DWORD dwDay )
{
	DAILYLOGIN_REWARD_MAP_ITER rewardIter = GLDailyLogin::GetInstance().m_mapDailyLoginReward.find(dwDay);
	if (rewardIter == GLDailyLogin::GetInstance().m_mapDailyLoginReward.end()) 
	{
		CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_DAILYLOGIN_REWARD_CLAIM_FAIL"));
		return E_FAIL;
	}

	SDAILYLOGIN_REWARD& sRewardData = rewardIter->second;
	SITEM* pItemReward = GLItemMan::GetInstance().GetItem( sRewardData.sRewardID );
	if ( !pItemReward )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_REQ_DAILYLOGIN_REWARD_CLAIM_NOTREWARD") );
		return E_FAIL;
	}

	if ( m_fDailyLoginDelay < DAILYLOGIN_DELAY )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("DAILYLOGIN_DELAY") );
		return E_FAIL;
	}

	GLMSG::SNETPC_REQ_DAILYLOGIN_REWARD_CLAIM NetMsg;
	NetMsg.dwDay = dwDay;
	NETSEND ( &NetMsg );

	return S_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Boss Spawn Viewer, Review000 */
HRESULT GLCharacter::ReqBossDetails(DWORD dwGlobID, SNATIVEID sMapID, SNATIVEID sMobID)
{
	GLMSG::SNETPC_BOSS_DETAILS_REQ NetMsg;
	NetMsg.dwGlobID = dwGlobID;
	NetMsg.sMapID = sMapID;
	NetMsg.sMobID = sMobID;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

void GLCharacter::ReqEnableEquipmentLock( WORD wPosX, WORD wPosY )
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem ( wPosX, wPosY );
	if ( !pInvenItem )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_NOT_CARD") );
		return;
	}

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem ( pInvenItem->sItemCustom.sNativeID );
	if ( !pItem ) 
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_NOT_CARD") );
		return;
	}

	if ( pItem->sBasicOp.emItemType != ITEM_PERSONAL_LOCK_ENABLE )	
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_NOT_CARD") );
		return;
	}

	EMPERSONAL_LOCK emLock = EMPERSONAL_LOCK_EQUIP;
	CInnerInterface::GetInstance().PersonalLockPinCreate(emLock);
	//CInnerInterface::GetInstance().ShowGroupFocus( PERSONAL_LOCK_PIN_CREATE );
}