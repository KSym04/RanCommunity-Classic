#include "StdAfx.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Engine/Core/NSRProfile.h"
#include "../../Lib_Engine/Core/NSROption.h"

#include "./ClubWindow.h"
#include "./ConftModalWindow.h"
#include "./DialogueWindow.h"
#include "../../Lib_Client/DxGlobalStage.h"
#include "./FriendWindow.h"
#include "./GameTextControl.h"
#include "../../Lib_Client/G-Logic/GLFriendClient.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./InnerInterface.h"
#include "./InventoryWindow.h"
#include "./MarketWindow.h"
#include "./ModalCallerID.h"
#include "./ModalWindow.h"
#include "./PartyModalWindow.h"
#include "./PartyWindow.h"
#include "./PrivateMarketSellWindow.h"
#include "./PrivateMarketWindow.h"
#include "./PtoPWindow.h"
#include "./QuestWindow.h"

#include "./StorageChargeCard.h"
#include "./StorageWindow.h"
#include "./TradeWindow.h"
#include "./UITextControl.h"
#include "./WaitServerDialogue.h"
#include "./RebirthDialogue.h"
#include "./ItemMixWindow.h"
#include "./ModernQuestWindow.h"
#include "./AdminMessageDisplay.h"
#include "PandoraBox/PandoraBoxWindow.h" // Pandora

/* Add ItemShop, Ssodomain, 11-09-2023, Start */
#include "ItemShop/ItemShopWindow.h"
/* Add ItemShop, Ssodomain, 11-09-2023, End */

/*modern inventory window, Juver, 2018/04/06 */
#include "./ModernInventoryWindow.h"

/*extended inventory window, EJCode, 2018/10/16 */
#include "./EP7InventoryWindow.h"

#include "./CharacterInfoViewWindow.h"

/* modern party window, Juver, 2019/12/20 */
#include "./ModernPartyWindow.h"

/* party finder, Juver, 2020/01/17 */
#include "./PartyFinderQuestion.h"

/* modern dialogue window, Juver, 2020/02/03 */
#include "./ModernDialogueWindow.h"

/* modern market window, Juver, 2020/02/05 */
#include "./ModernMarketWindow.h"

/* modern friend window, Juver, 2020/03/02 */
#include "./ModernFriendWindow.h"

/* modern rebirth dialogue, Juver, 2020/03/31 */
#include "./ModernRebirthDialogue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CInnerInterface::MsgProcess ( LPVOID msgBuffer )
{
	if( !msgBuffer) return ;

	NET_MSG_GENERIC *nmg = (NET_MSG_GENERIC*) msgBuffer;

	switch ( nmg->nType )
	{
		//pandora
	case NET_MSG_PANDORA_BOX_RESULT:
		{
			GLMSG::SNET_PANDORA_BOX_RESULT* pNetMsg = ( GLMSG::SNET_PANDORA_BOX_RESULT* ) nmg;
			m_pPandoraBoxWindow->InitWindow();
		}
		break;
	case NET_MSG_PANDORA_BOX_BUY_ITEM_FB:
		{
			GLMSG::SNET_PANDORA_BOX_BUY_ITEM_FB* pNetMsg = ( GLMSG::SNET_PANDORA_BOX_BUY_ITEM_FB* ) nmg;
			m_pPandoraBoxWindow->InitWindow();
		}
		break;
	case NET_MSG_EVENT_LOTTERY_FB:  
		{
			//	煎傳
			NET_EVENT_LOTTERY_FB* pNetMsg = (NET_EVENT_LOTTERY_FB*)nmg;
			const int nResult = pNetMsg->nResult;

			enum
			{
				LOTTERY_FB_NO1 = 1,
				LOTTERY_FB_NO2,
				LOTTERY_FB_NO3,
				LOTTERY_FB_NO4,
				LOTTERY_FB_NO5,
				LOTTERY_FB_NO6,
				LOTTERY_FB_ALREADY_INPUTED,
				LOTTERY_FB_NOT_PCROOM,
				LOTTERY_FB_ALREADY_USED,
				LOTTERY_FB_ERROR = 10,
			};

			switch ( nResult )
			{
			case LOTTERY_FB_NO1:
			case LOTTERY_FB_NO2:
			case LOTTERY_FB_NO3:
			case LOTTERY_FB_NO4:
			case LOTTERY_FB_NO5:
			case LOTTERY_FB_NO6:
				{
					CString strTemp = MakeString ( ID2GAMEINTEXT("LOTTERY_FB_NO1_6"), nResult );
					DoModal ( strTemp );
				}
				break;

			case LOTTERY_FB_ALREADY_INPUTED:
				{
					DoModal ( ID2GAMEINTEXT("LOTTERY_FB_ALREADY_INPUTED") );
				}
				break;
			case LOTTERY_FB_NOT_PCROOM:
				{
					DoModal ( ID2GAMEINTEXT("LOTTERY_FB_NOT_PCROOM") );
				}
				break;
			case LOTTERY_FB_ALREADY_USED:
				{
					DoModal ( ID2GAMEINTEXT("LOTTERY_FB_ALREADY_USED") );
				}
				break;
			case LOTTERY_FB_ERROR:
				{
					DoModal ( ID2GAMEINTEXT("LOTTERY_FB_ERROR") );
				}
				break;
			}
		}
		break;
	
	case NET_MSG_APEX_ANSWER: 
		{
			NET_APEX_ANSWER* pNetMsg = (NET_APEX_ANSWER*) nmg;
			switch( pNetMsg->wAction )
			{
			case 1: // 擊 餌辨朝匙擊 嫦唯擊陽 嶸盪纔 唳堅蒂 輿朝 詭撮雖
				{
					CString strTemp( ID2GAMEINTEXT( "APEX_ACTION_MESSAGE_WARNING" ) );
					PrintConsoleText( strTemp.GetString() );
					DoModal( strTemp, MODAL_INFOMATION, OK, NO_ID );
				}
				break;
			case 2: // 渠 旎雖
				{
					CString strTemp;
					strTemp.Format( ID2GAMEINTEXT( "APEX_ACTION_MESSAGE_CHATBLOCK" ), (pNetMsg->wHour*60) );
					PrintConsoleText( strTemp.GetString() );
					DoModal( strTemp, MODAL_INFOMATION, OK, NO_ID );
				}
				break;
			case 3: // 嶸盪 鑄
			case 4: // 嶸盪 綰煙
				{
					CString strTemp( ID2GAMEINTEXT( "APEX_ACTION_MESSAGE_KICK" ) );
					PrintConsoleText( strTemp.GetString() );
					WAITSERVER_DIALOGUE_OPEN( strTemp, WAITSERVER_CLOSEGAME, 30.0f );
				}
				break;
			default:
				GASSERT( 0 && "ERROR : DxGameStage::MsgProcess, NET_MSG_APEX_ANSER" );
				break;
			}
		}
		break;
	
	case NET_MSG_CHAT_CTRL_FB:			// 婦葬濠辨 瓣詭衛雖
		{
			NET_CHAT_CTRL_FB* pNc = (NET_CHAT_CTRL_FB*) nmg;

			/*item link, Juver, 2017/07/31 */
			/*staff name in chat, Juver, 2017/12/26 */
			/* Chat Color, Mhundz 02/22/25 */
			if ( RPARAM::bChatShowStaffName )
				DisplayChatMessage ( pNc->emType, pNc->szName, pNc->szChatMsg, pNc->sItemLink, pNc->bStaff );
			else
				DisplayChatMessage ( pNc->emType, NULL, pNc->szChatMsg, pNc->sItemLink, false );
		}break;

	case NET_MSG_CHAT_FB: // Ã¤ÆÃ¸Þ½ÃÁö  
		{
			NET_CHAT_FB* pNc = (NET_CHAT_FB*) nmg;

			/*item link, Juver, 2017/07/31 */
			/* Chat Color, Mhundz 02/22/25 */
			DisplayChatMessage ( pNc->emType, pNc->szName, pNc->szChatMsg, pNc->sItemLink, pNc->bStaff );
		}break;

	case NET_MSG_CHAT_PRIVATE_FAIL: 
		{
			GLMSG::SNETPC_CHAT_PRIVATE_FAIL *pNetMsg = (GLMSG::SNETPC_CHAT_PRIVATE_FAIL *) nmg;
			PrintConsoleText ( ID2GAMEINTEXT("CHAT_PRIVATE_FAIL"), pNetMsg->szName );
		}
		break;

	case NET_MSG_CHAT_GENERAL_FAIL: 
		{
			GLMSG::SNETPC_CHAT_GENERAL_FAIL *pNetMsg = (GLMSG::SNETPC_CHAT_GENERAL_FAIL *) nmg;
			PrintConsoleText ( ID2GAMEINTEXT("CHAT_GENERAL_FAIL") );
		}
		break;
	
	case NET_MSG_CHAT_BLOCK: 
		{
#ifndef CH_PARAM
			GLMSG::SNETPC_CHAT_BLOCK *pNetMsg = (GLMSG::SNETPC_CHAT_BLOCK *) nmg;
			CTime cTime( pNetMsg->tChatBlock ); // 憮幗縑憮 0檜 剩橫螃雖 彊朝棻堅 陛薑棻.
			PrintConsoleText( "[%s]:%02d/%02d/%02d %02d:%02d%s", 
															ID2GAMEWORD("NOTIFY_NAME"),
															cTime.GetYear()%2000, cTime.GetMonth(), cTime.GetDay(),
															cTime.GetHour(), cTime.GetMinute(),
															ID2GAMEINTEXT("CHAT_BLOCK_MSG") );
#endif
		}
		break;

	case NET_MSG_GCTRL_DROP_OUT_FORCED:
		{
			GLMSG::SNET_DROP_OUT_FORCED *pNetMsg = (GLMSG::SNET_DROP_OUT_FORCED *) msgBuffer;

			//CDebugSet::ToLogFile ( "error : overlapping login " );

			if ( pNetMsg->emForced==EMDROPOUT_REQLOGIN )
			{
				DoModal ( ID2GAMEINTEXT ("PLAYINTERFACE_8"), MODAL_INFOMATION, OK, MODAL_CLOSEGAME );
			}
			else
			{
				DoModal ( ID2GAMEINTEXT ("PLAYINTERFACE_9"), MODAL_INFOMATION, OK, MODAL_CLOSEGAME );
			}

			DxGlobalStage::GetInstance().GetNetClient()->CloseConnect();
		}
		break;

	case NET_MSG_GCTRL_REQ_TAKE_FB:  
		{
			GLMSG::SNETPC_REQ_TAKE_FB *pNetMsg = (GLMSG::SNETPC_REQ_TAKE_FB *)nmg;

			switch ( pNetMsg->emTakeFB )
			{
			case EMTAKE_FB_OFF:
				//pNetMsg->emCrow 餌塭颶. (GUI)
				switch ( pNetMsg->emCrow)
				{
				case CROW_ITEM:
					{
						//PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_10") );
					}
					break;
				case CROW_MONEY:
					{
						//PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_11") );
					}
					break;
				}
				break;
			case EMTAKE_FB_RESERVED:
				{
					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_12") );
				}
				break;

			case EMTAKE_FB_INVEN_ERROR:
				{
					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_13") );
				}
				break;
			case EMTAKE_FB_TOO_EVENT_ITEM:
				{
					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_14") );
				}
				break;
			case EMTAKE_FB_DISTANCE:
				{
//					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAKE_FB_DISTANCE") );
				}
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_PARTY_LURE_TAR:  
		{
			GLMSG::SNET_PARTY_LURE_TAR *pNetMsg = (GLMSG::SNET_PARTY_LURE_TAR *) nmg;
			
			if ( IsVisibleGroup ( PARTY_MODAL_WINDOW ) )
			{
				//	Note : 嬴霜 褐羶縑 渠 港滲擊 雖 彊擎 唳辦
				//		   譆蟾 檜曖 韓僥擎 賅舒 剪瞰棻.
				GLMSG::SNET_PARTY_LURE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = pNetMsg->dwMasterID;
				NetMsg.dwPartyID = pNetMsg->dwPartyID;
				NetMsg.emAnswer = EMPARTY_LURE_REFUSE;
				NETSEND ( &NetMsg );
			}

			//	Note :  濠翕 剪睡.
			//
			if ( RPROFILE::bDIS_PARTY )
			{
				//	 剪瞰 籀葬
				GLMSG::SNET_PARTY_LURE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = pNetMsg->dwMasterID;
				NetMsg.dwPartyID = pNetMsg->dwPartyID;
				NetMsg.emAnswer = EMPARTY_LURE_REFUSE;
				NETSEND ( &NetMsg );

				PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PARTY_LURE_REFUSE_AUTO"), pNetMsg->szCharName );
				break;
			}

			//	Note : 蹂羶  褫暮.
			//
			m_sReqPartyOption = pNetMsg->sOption;

			GetPartyWindow()->SetPartyInfo ( pNetMsg->dwPartyID, pNetMsg->dwMasterID );

			CString strTemp;
			strTemp = MakeString ( ID2GAMEINTEXT("PARTY_TAR"), pNetMsg->szCharName );
			DoPartyModal ( strTemp, MODAL_PARTY_TAR );	
		}
		break;

	case NET_MSG_GCTRL_TRADE_TAR: 
		{
			GLMSG::SNET_TRADE_TAR *pNetMsg = (GLMSG::SNET_TRADE_TAR *) nmg;
			PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
			if( !pLand ) break;

			PGLCHARCLIENT pChar = pLand->GetChar( pNetMsg->dwMasterID );
			if( !pChar ) break;
			
			CString strName( pChar->GetName() );

			//	Note : 剪楚 濠翕 剪睡.
			//
			bool bPMARKETOPEN = GLGaeaClient::GetInstance().GetCharacter()->m_sPMarket.IsOpen();
			if (RPROFILE::bDIS_TRADE || bPMARKETOPEN )
			{
				//	P2P 剪楚 剪瞰 籀葬
				GLMSG::SNET_TRADE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = pNetMsg->dwMasterID;
				NetMsg.emAns = EMTRADE_CANCEL;
				NETSEND ( &NetMsg );

				PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("TRADE_FB_CANCEL_AUTO"), strName );
				break;
			}

			CString strTemp;
			strTemp = MakeString ( ID2GAMEINTEXT("TRADE_TAR"), strName );
			DoModal ( strTemp, MODAL_QUESTION, OKCANCEL, MODAL_TRADE_REQ );

			GetTradeWindow()->SetTradeInfo ( strName, pNetMsg->dwMasterID );
		}
		break;

	case NET_MSG_GCTRL_CONFRONT_TAR:  
		{
			GLMSG::SNETPC_REQ_CONFRONT_TAR *pNetMsg = (GLMSG::SNETPC_REQ_CONFRONT_TAR *)nmg;
			
			PGLCHARCLIENT pChar = GLGaeaClient::GetInstance().GetChar ( pNetMsg->dwID );
			if( !pChar ) break;
			
			if ( IsVisibleGroup ( CONFT_MODAL_WINDOW ) )
			{
				//	Note : 嬴霜 渠溼褐羶縑 渠 港滲擊 雖 彊擎 唳辦
				//		   譆蟾 檜曖 韓僥擎 賅舒 剪瞰棻.
				GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
				NetMsgAns.emTYPE = pNetMsg->emTYPE;
				NetMsgAns.dwID = pNetMsg->dwID;
				NetMsgAns.emFB = EMCONFRONT_REFUSE;
				NetMsgAns.sOption = pNetMsg->sOption;
				NETSEND ( &NetMsgAns );
			}
			
			//	Note : 渠溼 濠翕 剪睡.
			//
			if (RPROFILE::bDIS_CONFT )
			{
				//	Note : 渠溼 剪瞰衛縑 詭衛雖 瞪歎.
				GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
				NetMsgAns.emTYPE = pNetMsg->emTYPE;
				NetMsgAns.dwID = pNetMsg->dwID;
				NetMsgAns.emFB = EMCONFRONT_REFUSE;
				NetMsgAns.sOption = pNetMsg->sOption;
				NETSEND ( &NetMsgAns );

				PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_REFUSE_AUTO"), pChar->GetName() );
				break;
			}

			const char *szReqName = pChar->GetName();
			m_ConflictReqID = pNetMsg->dwID;
			m_emConflictReqType = pNetMsg->emTYPE;
			m_sReqConflictOption = pNetMsg->sOption;
			UIGUID MODAL_CONFT_TYPE = NO_ID;

			CString strTemp;
			switch (pNetMsg->emTYPE)
			{
			case EMCONFT_NONE:
//				GASSERT ( 0 && "螃盟" );
				return ;
				break;
			case EMCONFT_ONE:
				strTemp = MakeString ( ID2GAMEINTEXT("CONFLICT_ONE_QUESTION_TAR"), szReqName );
				MODAL_CONFT_TYPE = MODAL_CONFLICT_ONE_TAR;
				break;
			case EMCONFT_PARTY:
				strTemp = MakeString ( ID2GAMEINTEXT("CONFLICT_PARTY_QUESTION_TAR"), szReqName );
				MODAL_CONFT_TYPE = MODAL_CONFLICT_PARTY_TAR;
				break;
			case EMCONFT_GUILD:
				strTemp = MakeString ( ID2GAMEINTEXT("CONFLICT_CLUB_QUESTION_TAR"), szReqName );
				MODAL_CONFT_TYPE = MODAL_CONFLICT_GUILD_TAR;
				break;
			};

			CString strMyName = GLGaeaClient::GetInstance().GetCharacter ()->GetName ();
			DoConftModal ( strTemp, MODAL_CONFT_TYPE, szReqName, strMyName );
		}
		break;
	case NET_MSG_SERVER_GENERALCHAT: 
		{
			GLMSG::SNET_SERVER_GENERALCHAT* pNetMsg = (GLMSG::SNET_SERVER_GENERALCHAT*) nmg;

			if (pNetMsg->bNotify) 
			{
				switch(pNetMsg->iColor)
				{
				case 1: // WHITE ANNOUNEMENT
						if ( ROPTION::bHideAnnouncements )
						{
							PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, pNetMsg->szTEXT );
						}
					break;

				case 2: // WHITE ANNOUNEMENT WITH CHAT
						PrintMsgText ( NS_UITEXTCOLOR::WHITE, pNetMsg->szTEXT );
						if ( ROPTION::bHideAnnouncements )
						{
							PVPCompetitionShowNotice( NS_UITEXTCOLOR::WHITE, pNetMsg->szTEXT );
						}
					break;

				case 3: // GREEN ANNOUNCEMENT
						if ( ROPTION::bHideAnnouncements )
						{
							PVPCompetitionShowNotice( NS_UITEXTCOLOR::BRIGHTGREEN, pNetMsg->szTEXT );
						}
					break;

				case 4: // GREEN ANNOUNCEMENT WITH CHAT
						PrintMsgText ( NS_UITEXTCOLOR::BRIGHTGREEN, pNetMsg->szTEXT );
						if ( ROPTION::bHideAnnouncements )
						{
							PVPCompetitionShowNotice( NS_UITEXTCOLOR::BRIGHTGREEN, pNetMsg->szTEXT );
						}
					break;

				case 5: // BLUE ANNOUNCEMENT
						PrintMsgText ( NS_UITEXTCOLOR::AQUAMARINE, pNetMsg->szTEXT );
						if ( ROPTION::bHideAnnouncements )
						{
							PVPCompetitionShowNotice( NS_UITEXTCOLOR::AQUAMARINE, pNetMsg->szTEXT );
						}
					break;

				case 6: // BLUE ANNOUNCEMENT WITH CHAT
						PrintMsgText ( NS_UITEXTCOLOR::AQUAMARINE, pNetMsg->szTEXT );
						if ( ROPTION::bHideAnnouncements )
						{
							PVPCompetitionShowNotice( NS_UITEXTCOLOR::AQUAMARINE, pNetMsg->szTEXT );
						}
					break;

				case 7: // RED ANNOUNCEMENT
					if ( ROPTION::bHideAnnouncements )
					{
						PVPCompetitionShowNotice( NS_UITEXTCOLOR::AQUAMARINE, pNetMsg->szTEXT );
					}
					break;

				case 8: // RED ANNOUNCEMENT WITH CHAT
						PrintMsgText ( NS_UITEXTCOLOR::DISABLE, pNetMsg->szTEXT );
						if ( ROPTION::bHideAnnouncements )
						{
							PVPCompetitionShowNotice( NS_UITEXTCOLOR::DISABLE, pNetMsg->szTEXT );
						}
					break;

				case 9: // ORANGE ANNOUNCEMENT
					if ( ROPTION::bHideAnnouncements )
					{
						PVPCompetitionShowNotice( NS_UITEXTCOLOR::ORNAGERED, pNetMsg->szTEXT );
					}
					break;

				case 10: // ORANGE ANNOUNCEMENT WITH CHAT
						PrintMsgText ( NS_UITEXTCOLOR::ORNAGERED, pNetMsg->szTEXT );
						if ( ROPTION::bHideAnnouncements )
						{
							PVPCompetitionShowNotice( NS_UITEXTCOLOR::ORNAGERED, pNetMsg->szTEXT );
						}
					break;

				case 11: // YELLOW ANNOUNCEMENT
					if ( ROPTION::bHideAnnouncements )
					{
						PVPCompetitionShowNotice( NS_UITEXTCOLOR::YELLOW, pNetMsg->szTEXT );
					}
					break;

				case 12: // YELLOW ANNOUNCEMENT WITH CHAT
						PrintMsgText ( NS_UITEXTCOLOR::YELLOW, pNetMsg->szTEXT );
						if ( ROPTION::bHideAnnouncements )
						{
							PVPCompetitionShowNotice( NS_UITEXTCOLOR::YELLOW, pNetMsg->szTEXT );
						}
					break;
				case 13: // MMR Rank UP
					PrintMsgText ( NS_UITEXTCOLOR::AQUAMARINE, pNetMsg->szTEXT );
					if ( ROPTION::bHideAnnouncements )
					{
						ShowPKStreakShutdownNotice ( NS_UITEXTCOLOR::AQUAMARINE, pNetMsg->szTEXT );
					}

				default:
						DisplayChatMessage ( CHAT_TYPE_CTRL_GLOBAL, "", pNetMsg->szTEXT );
					break;
				}
			}
			else{
				DisplayChatMessage ( CHAT_TYPE_CTRL_GLOBAL, "", pNetMsg->szTEXT );
			}


			/*if ( pNetMsg->bUseAdminDisplay )
			{
				if ( m_pAdminMessageDisplay )
				{
					CString strAdminText;
					strAdminText.Format( "%s", pNetMsg->szTEXT );
					m_pAdminMessageDisplay->AddText ( strAdminText, NS_UITEXTCOLOR::ADMIN_COLOR );

					ShowGroupTop ( ADMIN_MESSAGE_DISPLAY );	
				}
			}*/
		}
		break;

//		NOTE
//			CLOSING MESSAGE 籀葬
//	case BLAH_BLAH:
//		{
//			SetWaitServerDialogueClose ();
//		}
//		break;
	};
}

void CInnerInterface::ModalMsgProcess ( UIGUID nCallerID, DWORD dwMsg )
{
	switch ( nCallerID )
	{
	case MODAL_CLOSEGAME:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_CLOSEGAME );
			}
		}
		break;
	
	case MODAL_TEST_MODAL:
		break;	

	case MODAL_INVENTORY_MONEY_TO_FIELD:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnDropMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_lnMoney;
					if ( lnMoney < lnDropMoney ) lnDropMoney = lnMoney;

					pCharacter->ReqMoneyToField ( lnDropMoney );
				}
			}
		}
		break;

	case MODAL_PARTY_TAR:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				//	 熱塊 籀葬
				GLMSG::SNET_PARTY_LURE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = GetPartyWindow()->GetMasterID ();
				NetMsg.dwPartyID = GetPartyWindow()->GetPartyID ();
				NetMsg.emAnswer = EMPARTY_LURE_OK;
				NetMsg.sOption = m_sReqPartyOption;
				NETSEND ( &NetMsg );
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				//	 剪瞰 籀葬
				GLMSG::SNET_PARTY_LURE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = GetPartyWindow()->GetMasterID ();
				NetMsg.dwPartyID = GetPartyWindow()->GetPartyID ();
				NetMsg.emAnswer = EMPARTY_LURE_REFUSE;
				NetMsg.sOption = m_sReqPartyOption;
				NETSEND ( &NetMsg );
			}
		}
		break;

	case MODAL_PARTY_REQ:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();

				SPARTY_OPT sOption;
				sOption.emGET_ITEM = (EMPARTY_ITEM_OPT)RPROFILE::dwPARTY_GET_ITEM;
				sOption.emGET_MONEY = (EMPARTY_MONEY_OPT)RPROFILE::dwPARTY_GET_MONEY;
				sOption.bPrivateParty = RPROFILE::bPARTY_PRIVATE;

				//	Note : 憮幗縑 鼻渠婁曖  蹂羶.
				//
				GLMSG::SNET_PARTY_LURE NetMsg;
				NetMsg.dwGaeaID = dwGaeaID;
				NetMsg.sOption = sOption;
				NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
			}
		}
		break;

		/*charinfoview , Juver, 2017/11/11 */
	case MODAL_PARTY_REQ_VIEW_CHAR_INFO:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				DWORD dwGaeaID = CInnerInterface::GetInstance().GetCharInfoViewWindow()->GetCharacterData()->dwGaeaID;

				SPARTY_OPT sOption;
				sOption.emGET_ITEM = (EMPARTY_ITEM_OPT)RPROFILE::dwPARTY_GET_ITEM;
				sOption.emGET_MONEY = (EMPARTY_MONEY_OPT)RPROFILE::dwPARTY_GET_MONEY;
				sOption.bPrivateParty = RPROFILE::bPARTY_PRIVATE;

				//	Note : 憮幗縑 鼻渠婁曖  蹂羶.
				//
				GLMSG::SNET_PARTY_LURE NetMsg;
				NetMsg.dwGaeaID = dwGaeaID;
				NetMsg.sOption = sOption;
				NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
			}
		}break;
	
		/* modern friend window, Juver, 2020/03/02 */
	case MODAL_PARTY_REQ_PARTY_WINDOW:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				CString strName = m_pFriendWindow->GET_FRIEND_NAME();
				SFRIEND* pFRIEND = GLFriendClient::GetInstance().GetFriend ( strName.GetString () );
				if ( pFRIEND )
				{
					SPARTY_OPT sOption;
					sOption.emGET_ITEM = (EMPARTY_ITEM_OPT)RPROFILE::dwPARTY_GET_ITEM;
					sOption.emGET_MONEY = (EMPARTY_MONEY_OPT)RPROFILE::dwPARTY_GET_MONEY;
					sOption.bPrivateParty = RPROFILE::bPARTY_PRIVATE;

					/* party charid/charname, Juver, 2020/03/02 */
					GLMSG::SNET_PARTY_LURE_CHAR_ID NetMsg;
					NetMsg.dwCharID = (DWORD)pFRIEND->nCharID;
					NetMsg.sOption = sOption;
					NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
				}
			}
		}break;

		/* party charid/charname, Juver, 2020/03/02 */
	case MODAL_PARTY_REQ_GUILD_WINDOW:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				if ( m_pClubWindow )
				{
					const DWORD dwCharID = m_pClubWindow->GetClubMemberID ();				

					SPARTY_OPT sOption;
					sOption.emGET_ITEM = (EMPARTY_ITEM_OPT)RPROFILE::dwPARTY_GET_ITEM;
					sOption.emGET_MONEY = (EMPARTY_MONEY_OPT)RPROFILE::dwPARTY_GET_MONEY;
					sOption.bPrivateParty = RPROFILE::bPARTY_PRIVATE;

					GLMSG::SNET_PARTY_LURE_CHAR_ID NetMsg;
					NetMsg.dwCharID = dwCharID;
					NetMsg.sOption = sOption;
					NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
				}
			}
		}break;

	case MODAL_TRADE_REQ:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				//	P2P 剪楚 熱塊 籀葬
				GLMSG::SNET_TRADE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = GetTradeWindow()->GetMasterID ();
				NetMsg.emAns = EMTRADE_OK;
				NETSEND ( &NetMsg );
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				//	P2P 剪楚 剪瞰 籀葬
				GLMSG::SNET_TRADE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = GetTradeWindow()->GetMasterID ();
				NetMsg.emAns = EMTRADE_CANCEL;
				NETSEND ( &NetMsg );
			}
		}
		break;
	
	case MODAL_REBIRTH:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				if ( GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
				{
					// 睡晦
					GLGaeaClient::GetInstance().GetCharacter()->ReqReBirth();
					
					// 翮溥霞 璽菟 殘晦
					CloseAllWindow ();				
				}
			}

			if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				if ( GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
				{
					if ( m_pRebirthDialogue->IsRecoveryAction() )
					{	
						GLGaeaClient::GetInstance().GetCharacter()->ReqGetReExp ();
					}
					else
					{
						// 敝輿 餌辨
						GLGaeaClient::GetInstance().GetCharacter()->ReqReGenRevive ();
						// 翮溥霞 璽菟 殘晦
						CloseAllWindow ();						
					}
				}
			}
		}
		break;
	case MODAL_RECOVERY_EXP:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				if ( GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqRecoveryExp();				
				}
			}
			if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				if ( GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
				{
					// 睡晦
					GLGaeaClient::GetInstance().GetCharacter()->ReqReBirth();
					
					// 翮溥霞 璽菟 殘晦
					CloseAllWindow ();				
				}
			}
		}
		break;
	case MODAL_RECOVERY_NPC_EXP:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				int NpcID = 0;
				int DummyData = 0;
				GetModalWindow()->GetModalData( &NpcID, &DummyData );

				GLGaeaClient::GetInstance().GetCharacter()->ReqRecoveryExpNpc( NpcID );				
			}			
		}
		break;


	//case MODAL_DROPMONEY:
	//	{
	//		if ( dwMsg & UIMSG_MODAL_OK )
	//		{
	//			GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	//			if ( pCharacter )
	//			{
	//				CString strTemp = GetModalWindow()->GetEditString ();
	//				LONGLONG lnDropMoney = _ttoi64 ( strTemp.GetString () );
	//				LONGLONG lnMoney = pCharacter->m_lnMoney;
	//				if ( lnMoney < lnDropMoney ) lnDropMoney = lnMoney;

	//				pCharacter->ReqMoneyToField ( lnDropMoney );
	//			}
	//		}
	//	}
	//	break;

	case MODAL_TRADEMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				CString strTemp = GetModalWindow()->GetEditString ();
				LONGLONG lnMoney = _ttoi64 ( strTemp.GetString () );
				GLTradeClient::GetInstance().ReqTradeMoney ( lnMoney );
			}
		}
		break;

	case MODAL_SAVEMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnSaveMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_lnMoney;
					if ( lnMoney < lnSaveMoney ) lnSaveMoney = lnMoney;

					pCharacter->ReqStorageSaveMoney ( lnSaveMoney );
				}
			}
		}
		break;

	case MODAL_CLUB_SAVEMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnSaveMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_lnMoney;
					if ( lnMoney < lnSaveMoney ) lnSaveMoney = lnMoney;

					pCharacter->ReqClubStorageSaveMoney ( lnSaveMoney );
				}
			}
		}
		break;

	case MODAL_LOADMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnLoadMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_lnStorageMoney;
					if ( lnMoney < lnLoadMoney ) lnLoadMoney = lnMoney;

					pCharacter->ReqStorageDrawMoney ( lnLoadMoney );
				}
			}
		}
		break;

	case MODAL_CLUB_LOADMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnLoadMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_sCLUB.m_lnStorageMoney;
					if ( lnMoney < lnLoadMoney ) lnLoadMoney = lnMoney;

					pCharacter->ReqClubStorageDrawMoney ( lnLoadMoney );
				}
			}
		}
		break;

	case MODAL_SPLIT_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					WORD wSplitNum = (WORD) atoi ( strTemp.GetString () );

					WORD wPosX, wPosY;
					GetInventoryWindow ()->GetSplitPos ( &wPosX, &wPosY );
					pCharacter->ReqInvenSplit ( wPosX, wPosY, wSplitNum );
				}
			}
		}
		break;

	case MODAL_STORAGE_SPLIT_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					WORD wSplitNum = (WORD) atoi ( strTemp.GetString () );

					WORD wPosX, wPosY;
					int nChannel = m_pStorageWindow->GetChannel ();
					m_pStorageWindow->GetSplitPos ( &wPosX, &wPosY );					
					pCharacter->ReqStorageSplit ( nChannel, wPosX, wPosY, wSplitNum );
				}
			}
		}
		break;

	case MODAL_CLUB_STORAGE_SPLIT_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					WORD wSplitNum = (WORD) atoi ( strTemp.GetString () );

					WORD wPosX, wPosY;
					int nChannel = m_pClubStorageWindow->GetChannel ();
					m_pClubStorageWindow->GetSplitPos ( &wPosX, &wPosY );
					pCharacter->ReqClubStorageSplit ( nChannel, wPosX, wPosY, wSplitNum );
				}
			}
		}
		break;

	case MODAL_CONFLICT_ONE_TAR:
	case MODAL_CONFLICT_PARTY_TAR:
	case MODAL_CONFLICT_GUILD_TAR:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				//block using of pet for few seconds when duel is accepted
				GLGaeaClient::GetInstance().GetCharacter()->resetPetBlockTime();

				//	Note : 渠溼 熱塊衛縑 詭衛雖 瞪歎.
				GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
				NetMsgAns.emTYPE = m_emConflictReqType;
				NetMsgAns.dwID = m_ConflictReqID;
				NetMsgAns.emFB = EMCONFRONT_AGREE;
				NetMsgAns.sOption = m_sReqConflictOption;
				NETSEND ( &NetMsgAns );
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				//	Note : 渠溼 剪瞰衛縑 詭衛雖 瞪歎.
				GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
				NetMsgAns.emTYPE = m_emConflictReqType;
				NetMsgAns.dwID = m_ConflictReqID;
				NetMsgAns.emFB = EMCONFRONT_REFUSE;
				NetMsgAns.sOption = m_sReqConflictOption;
				NETSEND ( &NetMsgAns );
			}
		}
		break;

	case MODAL_CONFLICT_ONE_REQ:
	case MODAL_CONFLICT_PARTY_REQ:
	case MODAL_CONFLICT_GUILD_REQ:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();

				SCONFT_OPTION sOption;
				sOption.bBOUND = RPROFILE::bCONFT_BOUND!=FALSE;
				sOption.wRECOVER = RPROFILE::wCONFT_RECOVER;
				sOption.fHP_RATE = RPROFILE::fCONFT_HP_RATE;
				sOption.fTAR_HP_RATE = RPROFILE::fCONFT_TAR_HP_RATE;

				//block using of pet for few seconds when you invite someone to duel
				GLGaeaClient::GetInstance().GetCharacter()->resetPetBlockTime();

				GLGaeaClient::GetInstance().GetCharacter()->ReqConflict ( dwGaeaID, sOption );
			}
		}
		break;

	case MODAL_PILE_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				CString strTemp = GetModalWindow()->GetEditString ();
				DWORD dwCount = (DWORD) atoi ( strTemp.GetString () );
				if ( dwCount )
				{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
					CMarketWindow* pMarketWindow = GetMarketWindow ();
#else
					/* modern market window, Juver, 2020/02/05 */
					CModernMarketWindow* pMarketWindow = GetMarketWindow ();
#endif
					SNATIVEID sMarketID = pMarketWindow->GetMarketID ();
					int	nPageIndex = pMarketWindow->GetPageIndex ();

					int nPosX = nOUTOFRANGE;
					int nPosY = nOUTOFRANGE;
					pMarketWindow->GetItemIndex ( &nPosX, &nPosY );
					if ( nPosX < 0 || nPosY < 0 ) return ;

					LONGLONG lnMoney = GLGaeaClient::GetInstance().GetCharacter ()->m_lnMoney;
					SINVENITEM& sInvenItem = pMarketWindow->GetItem ( nPosX, nPosY );
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sInvenItem.sItemCustom.sNativeID );
					if ( !pItem ) return ;

					float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();
					float fSHOP_RATE_C = fSHOP_RATE * 0.01f;
					DWORD dwPrice = static_cast<DWORD>( pItem->sBasicOp.dwBuyPrice * fSHOP_RATE_C );
					DWORD dwMaxCount = (DWORD)(lnMoney / (LONGLONG)dwPrice);
					if ( dwMaxCount < 1 )
					{					
						PrintMsgTextDlg ( NS_UITEXTCOLOR::ORANGE, ID2GAMEINTEXT("BUYITEM_NOMONEY") );
						return ;
					}
					//	幗 虜菟橫雖朝渠煎 厥晦
					//	譆渠 偃熱煎 餌晦
					if ( dwMaxCount < dwCount )
					{												
						PrintMsgText ( NS_UITEXTCOLOR::ORANGE,
							MakeString ( ID2GAMEINTEXT("BUYITEM_MAXIMUM"),
							pItem->GetName(), dwMaxCount )
							);
						dwCount = dwMaxCount;
					}
					GLGaeaClient::GetInstance().GetCharacter()->ReqNpcTo ( sMarketID, nPageIndex, nPosX, nPosY, (WORD)dwCount );
				}
			}
		}
		break;

	case MODAL_SERVER_STAGE:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_TOLOBY );
			}
		}
		break;

	case MODAL_BUY_ITEM:
	case MODAL_SELL_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
#if defined( BUILD_EP4 ) || defined( BUILD_EP6 ) || defined( BUILD_EP7 )
				CMarketWindow* pMarketWindow = GetMarketWindow ();
#else
				/* modern market window, Juver, 2020/02/05 */
				CModernMarketWindow* pMarketWindow = GetMarketWindow ();
#endif
				SNATIVEID sMarketID = pMarketWindow->GetMarketID ();
				int	nPageIndex = pMarketWindow->GetPageIndex ();

				int nPosX = nOUTOFRANGE;
				int nPosY = nOUTOFRANGE;
				if ( nCallerID == MODAL_BUY_ITEM )
				{
					pMarketWindow->GetItemIndex ( &nPosX, &nPosY );
					if ( nPosX < 0 || nPosY < 0 ) return ;
				}
				CDebugSet::ToView ( 1, 3, "餌朝 嬴檜蠱 : %d %d", nPosX, nPosY );

				GLGaeaClient::GetInstance().GetCharacter()->ReqNpcTo ( sMarketID, nPageIndex, nPosX, nPosY );
			}
		}
		break;

	case MODAL_QUEST_GIVEUP:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				//	晦
				
				DWORD dwQuestID = GetQuestWindow()->GetQuestID ();
				GLGaeaClient::GetInstance().GetCharacter ()->ReqQuestGiveUp ( dwQuestID );
			}
		}
		break;

	case MODAL_FRIEND_ADD:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_FRIEND_NAME ();
				GLFriendClient::GetInstance().ReqFriendAdd ( strName.GetString () );
			}
		}
		break;

	case MODAL_FRIEND_DEL:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_FRIEND_NAME ();
				GLFriendClient::GetInstance().ReqFriendDel ( strName.GetString () );
			}
		}
		break;

	case MODAL_FRIEND_MOVE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{				
				SetFriendName ( m_pFriendWindow->GET_FRIEND_NAME () );
				/*dmk14 fast teleport fix*/

				GLGaeaClient::GetInstance().GetCharacter()->Req2Friend ( m_pFriendWindow->GET_FRIEND_NAME () );
				//WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_TOFRIEND, 10.0f );
			}
		}
		break;

	case MODAL_ADD_FRIEND_LURE_ANS:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				if ( !m_strFriendName.GetLength () )
				{
					GASSERT ( 0 && "檜葷檜 蛔煙腎雖 彊懊蝗棲棻." );
					return ;
				}

				GLFriendClient::GetInstance ().ReqFriendLureAns ( m_strFriendName.GetString(), true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				if ( !m_strFriendName.GetLength () )
				{
					GASSERT ( 0 && "檜葷檜 蛔煙腎雖 彊懊蝗棲棻." );
					return ;
				}

				GLFriendClient::GetInstance ().ReqFriendLureAns ( m_strFriendName.GetString(), false );
			}
		}
		break;

	case MODAL_DEFAULT_POS:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				MOVE_DEFAULT_POSITION ();
			}
		}
		break;

	case MODAL_LOTTO:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strLotto = GetModalWindow()->GetEditString ();
				int nLength = strLotto.GetLength ();
				int nGAP = 10 - nLength;
				if ( nGAP )
				{
					strLotto += "0";
				}
				DxGlobalStage::GetInstance().NetSendEventLottery ( strLotto );
			}
		}
		break;

	case MODAL_STORAGE_CHARGE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				WORD wPosX, wPosY, wSTORAGE;
				m_pStorageChargeCard->GetSelectedChargeCard ( wPosX, wPosY, wSTORAGE );

				GLGaeaClient::GetInstance().GetCharacter()->ReqStorageCard ( wPosX, wPosY, wSTORAGE );
			}
		}
		break;

	case MODAL_PRIVATE_MARKET_SELL:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				WORD wPosX, wPosY;
				m_pPrivateMarketSellWindow->GetItemIndex ( wPosX, wPosY );

				DWORD dwNum = m_pPrivateMarketSellWindow->GetItemCount ();
				LONGLONG llMoney = m_pPrivateMarketSellWindow->GetItemCost ();

				GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketRegItem ( wPosX, wPosY, llMoney, dwNum );
			}
		}
		break;

	case MODAL_PRIVATE_MARKET_BUY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{	
				DWORD dwMarketID = m_pPrivateMarketWindow->GetPrivateMarketID ();
//				DWORD dwNum = static_cast<DWORD>(atoi(GetModalWindow()->GetEditString ()));

				int nPosX = nOUTOFRANGE;
				int nPosY = nOUTOFRANGE;

				GetModalWindow()->GetModalData ( &nPosX, &nPosY );
				if ( nPosX < 0 || nPosY < 0 ) return ;

				//CDebugSet::ToView ( 1, 3, "餌朝 嬴檜蠱 : %d %d", nPosX, nPosY );

				GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketBuy ( dwMarketID, nPosX, nPosY, 1 );
			}
		}
		break;

	case MODAL_PRIVATE_MARKET_PILE:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				CString strTemp = GetModalWindow()->GetEditString ();
				DWORD dwCount = (DWORD) atoi ( strTemp.GetString () );
				if ( dwCount )
				{
//					CMarketWindow* pMarketWindow = GetMarketWindow ();
					DWORD dwMarketID = m_pPrivateMarketWindow->GetPrivateMarketID ();
//					int	nPageIndex = pMarketWindow->GetPageIndex ();

					int nPosX = nOUTOFRANGE;
					int nPosY = nOUTOFRANGE;
					GetModalWindow()->GetModalData ( &nPosX, &nPosY );
					if ( nPosX < 0 || nPosY < 0 ) return ;

					LONGLONG lnMoney = GLGaeaClient::GetInstance().GetCharacter ()->m_lnMoney;
					SINVENITEM& sInvenItem = m_pPrivateMarketWindow->GetItem ( nPosX, nPosY );
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sInvenItem.sItemCustom.sNativeID );
					if ( !pItem ) return ;

					DWORD dwPrice = pItem->sBasicOp.dwBuyPrice;
//					float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();					
//					DWORD dwPrice = static_cast<DWORD>(pItem->sBasicOp.dwBuyPrice*fSHOP_RATE/100.0f+0.1f);

					DWORD dwMaxCount = (DWORD)(lnMoney / (LONGLONG)dwPrice);
					if ( dwMaxCount < 1 )
					{					
						PrintMsgTextDlg ( NS_UITEXTCOLOR::ORANGE, ID2GAMEINTEXT("BUYITEM_NOMONEY") );
						return ;
					}
					//	幗 虜菟橫雖朝渠煎 厥晦
					//	譆渠 偃熱煎 餌晦
					if ( dwMaxCount < dwCount )
					{												
						PrintMsgText ( NS_UITEXTCOLOR::ORANGE,
							MakeString ( ID2GAMEINTEXT("BUYITEM_MAXIMUM"),
								pItem->GetName(), dwMaxCount )
							);
						dwCount = dwMaxCount;
					}
					GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketBuy ( dwMarketID, nPosX, nPosY, (WORD)dwCount );
				}
			}
		}
		break;

	case MODAL_PRIVATE_MARKET_QUIT:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				SetPrivateMarketClose ();
			}
		}
		break;

	case MODAL_CLUB_KICK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				const DWORD dwCharID = m_pClubWindow->GetClubMemberID ();				
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubMemberDel ( dwCharID );
			}
		}
		break;

	case MODAL_CLUB_AUTHORITY:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				const DWORD dwCharID = m_pClubWindow->GetClubMemberID ();				
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubAuthority ( dwCharID );
			}
		}
		break;
	case MODAL_CLUB_AUTHORITY_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAuthorityAns( true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAuthorityAns( false );
			}
		}
		break;
	case MODAL_CLUB_BREAKUP:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubDissolution ();
			}
		}
		break;

	case MODAL_CLUB_CHANGE_MARK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strFileName = m_pModalWindow->GetEditString ();
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubMarkChange ( strFileName.GetString() );
			}
		}
		break;

	case MODAL_CLUB_NICK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strNickName = m_pModalWindow->GetEditString ();				
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubNick ( strNickName.GetString() );
			}
		}
		break;

	case MODAL_CLUB_JOIN_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubJoinAns ( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubJoinAns ( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_CLUB_JOIN:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubJoin ( dwGaeaID );
			}
		}
		break;

	case MODAL_CLUB_BREAKUP_CANCEL:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubDissolutionCancel ();
			}
		}
		break;

	case MODAL_CLUB_SELFQUIT:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				//	驍黴 Req
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubSecede ();
			}
		}
		break;

	case MODAL_SKILL_LEVEL_UP_CONFIRM:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				SNATIVEID sNativeID ( GetSkillUpID () );
				GLGaeaClient::GetInstance().GetCharacter()->ReqSkillUp ( sNativeID );
			}
		}
		break;

	case MODAL_COMMISSION:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strCommission = m_pModalWindow->GetEditString ();
				float fCommission = (float) atof ( strCommission.GetString () );
				STARGETID sCrowID = m_pDialogueWindow->GetCrowID ();

				GLGaeaClient::GetInstance().GetCharacter()->ReqGuidCommission ( sCrowID.dwID, fCommission );
			}
		}
		break;

	case MODAL_FRIEND_BLOCK_ADD_N:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_FRIEND_NAME();
				GLFriendClient::GetInstance().ReqFriendBlock ( strName.GetString (), true );
			}
		}
		break;
        
	case MODAL_FRIEND_BLOCK_ADD_B:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_BLOCK_NAME();
				GLFriendClient::GetInstance().ReqFriendBlock ( strName.GetString (), true );
			}
		}
		break;

	case MODAL_FRIEND_BLOCK_DEL:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_BLOCK_NAME ();
				GLFriendClient::GetInstance().ReqFriendBlock ( strName.GetString (), false );
			}
		}
		break;

	case MODAL_UNION_JOIN_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAllianceAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAllianceAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_UNION_JOIN:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAlliance( dwGaeaID );
			}
		}
		break;

	case MODAL_CLUB_BATTLE_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_ALLIANCE_BATTLE_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_CLUB_BATTLE_ARMISTICE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwClubID = m_pClubWindow->GetBattleClubID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleArmistice( dwClubID );
			}
		}
		break;

	case MODAL_ALLIANCE_BATTLE_ARMISTICE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwClubID = m_pClubWindow->GetBattleClubID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleArmistice( dwClubID );
			}
		}
		break;

	case MODAL_CLUB_BATTLE_ARMISTICE_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleArmisticeAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleArmisticeAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_ALLIANCE_BATTLE_ARMISTICE_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleArmisticeAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleArmisticeAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_CLUB_BATTLE_SUBMISSION:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwClubID = m_pClubWindow->GetBattleClubID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleSubmission( dwClubID );
			}
		}
		break;

	case MODAL_ALLIANCE_BATTLE_SUBMISSION:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwClubID = m_pClubWindow->GetBattleClubID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleSubmission( dwClubID );
			}
		}
		break;

	case MODAL_UNION_KICK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				const DWORD dwClubID = m_pClubWindow->GetUnionMemberID();
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAllianceDel( dwClubID );
			}
		}
		break;

	case MODAL_UNION_LEAVE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				//	驍黴 Req
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubAllianceSec();
			}
		}
		break;

	case MODAL_UNION_BREAKUP:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubAllianceDis();
			}
		}
		break;

	case MODAL_CHAR_RENAME:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pModalWindow->GetEditString ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqInvenRename( strName.GetString() );
			}
		}
		break;

	case MODAL_PET_RENAME:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pModalWindow->GetEditString ();
				GLGaeaClient::GetInstance().GetPetClient()->ReqRename( strName.GetString() );
			}
		}
		break;

	// Contribution Item - JX
	case MODAL_ADD_CONTRIBUTION:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqContributionPointCard();
			}
		}
		break;

	case MODAL_RESET_SKILLSTATS:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqResetSkillStats( );
			}

		}
		break;
	case MODAL_ITEMREBUILD_MONEY:	// ITEMREBUILD_MARK
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				CString strInputMoney = GetModalWindow()->GetEditString();
				LONGLONG i64InputMoney = _ttoi64( strInputMoney.GetString() );
				GLGaeaClient::GetInstance().GetCharacter()->ReqRebuildInputMoney( i64InputMoney );
			}
		}
		break;

	case MODAL_ITEMREBUILD_OKQUESTION:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqRebuildResult();
			}
		}
		break;

	case MODAL_ITEMGARBAGE_OKQUESTION:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqGarbageResult();
			}
		}
		break;

	case MODAL_HAIRCOLOR_INFO:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				ShowGroupFocus( HAIRCOLORCARD_WINDOW );
			}
		}
		break;

	case MODAL_PETCOLOR_INFO:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				ShowGroupFocus( PETCOLORCARD_WINDOW );
			}
		}
		break;

	case MODAL_MOBILE_NUMBER:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				CString strInputNumber = GetModalWindow()->GetEditString();
				GLGaeaClient::GetInstance().GetCharacter()->ReqSetPhoneNumber( strInputNumber );
			}
		}
		break;

	case MODAL_GENDER_CHANGE:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				ShowGroupFocus( GENDER_CHANGE_WINDOW );
			}
		}
		break;
	case MODAL_GENDER_CHANGE_END:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_TOLOBY );
			}
		}
		break;

	case MODAL_ITEMMIX_OKQUESTION:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				m_pItemMixWindow->SetAnimation();
			}
		}
		break;

		/*npc shop, Juver, 2017/07/27 */
	case MODAL_NPCSHOP_PURCHASE:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				if ( dwMsg & UIMSG_MODAL_OK )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqNpcShopDoPurchase();
				}
				else if ( dwMsg & UIMSG_MODAL_CANCEL )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqNpcShopPurchaseReset();
				}
			}
		}break;

		/*product item, Juver, 2017/10/18 */
	case MODAL_ITEM_PRODUCT_PRODUCE:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				if ( dwMsg & UIMSG_MODAL_OK )
				{
					DWORD dwid = GLGaeaClient::GetInstance().GetCharacter()->GetItemCompoundID();
					GLGaeaClient::GetInstance().GetCharacter()->ReqItemCompound( dwid );
				}
			}
		}break;

		/*item wrapper, Juver, 2018/01/12 */
	case MODAL_WRAP_ITEM:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				if ( dwMsg & UIMSG_MODAL_OK )
					GLGaeaClient::GetInstance().GetCharacter()->ReqItemWrapMsg();
			}
		}break;

		/*item wrapper, Juver, 2018/01/12 */
	case MODAL_UNWRAP_ITEM:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				if ( dwMsg & UIMSG_MODAL_OK )
					GLGaeaClient::GetInstance().GetCharacter()->ReqItemUnwrapMsg();
			}
		}break;

		/* Reset Stats Item */
	case MODAL_RESET_STATS:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ResetStatsReq();
			}

		}
		break;


		/*modern inventory window, Juver, 2018/04/09 */
	case MODAL_MODERN_INVENTORY_LOCKER:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				WORD wX = GLGaeaClient::GetInstance().GetCharacter()->m_wInventoryUsePosX;
				WORD wY = GLGaeaClient::GetInstance().GetCharacter()->m_wInventoryUsePosY;
				GLGaeaClient::GetInstance().GetCharacter()->ReqStorageOpenCard ( wX, wY );
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				GLGaeaClient::GetInstance().GetCharacter()->m_wInventoryUsePosX = USHRT_MAX;
				GLGaeaClient::GetInstance().GetCharacter()->m_wInventoryUsePosY = USHRT_MAX;
			}
		}break;

		/*modern inventory window, Juver, 2018/04/09 */
	case MODAL_MODERN_INVENTORY_BACKPACK:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				WORD wX = GLGaeaClient::GetInstance().GetCharacter()->m_wInventoryUsePosX;
				WORD wY = GLGaeaClient::GetInstance().GetCharacter()->m_wInventoryUsePosY;
				GLGaeaClient::GetInstance().GetCharacter()->ReqInvenLineCard ( wX, wY );
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				GLGaeaClient::GetInstance().GetCharacter()->m_wInventoryUsePosX = USHRT_MAX;
				GLGaeaClient::GetInstance().GetCharacter()->m_wInventoryUsePosY = USHRT_MAX;
			}
		}break;

		/*modern inventory window, Juver, 2018/04/09 */
	case MODAL_MODERN_INVENTORY_WASTEBASKET:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				OpenItemGarbageWindow( FALSE );
			}
		}break;

		/*buffs manual remove, Juver, 2018/08/14 */
	case MODAL_BUFF_MANUAL_REMOVE:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLGaeaClient::GetInstance().GetCharacter()->BuffManualRemove();;
			}
		}break;

		/*item drop question, Juver, 2018/08/22 */
	case MODAL_INVEN_ITEM_DROP:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLGaeaClient::GetInstance().GetCharacter()->hold_to_field_answer();
			}
		}break;

		/*item bank question, Juver, 2018/08/22 */
	case MODAL_ITEM_BANK_RETRIEVE:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLGaeaClient::GetInstance().GetCharacter()->item_bank_retrieve_answer();
			}
		}break;

		/*manual lunchbox remove, Juver, 2018/09/19 */
	case MODAL_LUNCHBOX_MANUAL_REMOVE:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLGaeaClient::GetInstance().GetCharacter()->lunchbox_manual_remove();
			}
		}break;

		/*item random option rebuild, Juver, 2018/07/04 */
	case ITEM_RANDOM_OPTION_REBUILD_START_QUESTION:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ItemRandomOptionRebuildStart();
			}
		}break;

	case PET_CARD_SUMMON_QUESTION:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqPetCardSummon();
			}
		}break;
			
	case PET_CARD_DISMISS_QUESTION:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqPetCardDismiss();
			}
		}break;

		/* party finder, Juver, 2020/01/17 */
	case MODAL_PARTY_FINDER_JOIN_QUESTION:
		{
			
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				if ( m_pPartyFinderQuestion )
				{
					DWORD dwPartyID = m_pPartyFinderQuestion->m_dwPartyID;
					DWORD dwCharID = m_pPartyFinderQuestion->m_dwCharID;
					GLGaeaClient::GetInstance().GetCharacter()->PartyFinderQuestionAccept( dwPartyID, dwCharID );
				}
				
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				if ( m_pPartyFinderQuestion )
				{
					DWORD dwPartyID = m_pPartyFinderQuestion->m_dwPartyID;
					DWORD dwCharID = m_pPartyFinderQuestion->m_dwCharID;
					GLGaeaClient::GetInstance().GetCharacter()->PartyFinderQuestionDecline( dwPartyID, dwCharID );
				}
			}
		}break;
		
		/* Codex, Archie 02/16/24 */
	case MODAL_REGISTER_CODEX:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				if ( dwMsg & UIMSG_MODAL_OK )
				{
					SNATIVEID msidItem;
					msidItem.wMainID = 0;
					msidItem.wSubID = 0;
					DWORD dwActivityID = 0;
					WORD wGrade = 0;
					WORD wQuantity = 0;

					GetModalWindow()->GetCodexModalData ( &msidItem, &dwActivityID, &wGrade, &wQuantity );
					GLGaeaClient::GetInstance().GetCharacter()->ReqCodexRegister(msidItem, dwActivityID, wGrade, wQuantity);
				}
			}
		}break;
		
		/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	case MODAL_BUYITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CItemShopWindow* pItemShopWindow = GetItemShop ();
					if ( pItemShopWindow )
					{
						pCharacter->ReqBuyItem( pItemShopWindow->GetPurKey() );
					}
				}
			}

		}break;
		/* Add ItemShop, Ssodomain, 11-09-2023, End */


	case MODAL_NPC_CONTRIBUTION_SHOP_PURCHASE:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				if ( dwMsg & UIMSG_MODAL_OK )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqNpcContributionShopDoPurchase();
				}
				else if ( dwMsg & UIMSG_MODAL_CANCEL )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqNpcContributionShopPurchaseReset();
				}
			}
		}break;

	case MODAL_SKILL_PURCHASE_CONFIRM:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				SNATIVEID sNativeID ( GetSkillUpID () );
				GLGaeaClient::GetInstance().GetCharacter()->ReqPurchaseSkill ( sNativeID );
				SetSkillUpID( NATIVEID_NULL().dwID );
			}
		}
		break;

	
	/* Gacha System, MontageDev 7/10/24 */
	case MODAL_OPEN_GACHA:
		{
			if( UIMSG_MODAL_OK & dwMsg )
				{
					if ( dwMsg & UIMSG_MODAL_OK )
					{
						GLGaeaClient::GetInstance().GetCharacter()->ReqOpenGacha(GLCONST_CHAR::nidGachaItem, false);
					}
				}
		}break;
	}
}