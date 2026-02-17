#include "StdAfx.h"
#include "BossDetailsWindow.h"
#include "BossListSlot.h"
#include "MobPreviewWindowRender.h" // Mob Preview Render
#include "../Lib_ClientUI/Interface/BasicLineBox.h"
#include "../Lib_ClientUI/Interface/BasicTextBoxEx.h"
#include "../Lib_ClientUI/Interface/BasicScrollBarEx.h"
#include "../Lib_ClientUI/Interface/BasicScrollThumbFrame.h"
#include "../Lib_ClientUI/Interface/BasicButton.h"
#include "../Lib_ClientUI/Interface/BasicTextButton.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"

#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_Engine/Core/NSRParam.h"
#include "../Lib_Client/G-Logic/GLMaplist.h"

#include "../Lib_Client/G-Logic/GLCrowData.h"
#include "../Lib_Client/G-Logic/GLItemMan.h"

#include "../Lib_ClientUI/Interface/ItemImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CBossDetailsWindow::CBossDetailsWindow ()
	: m_pListTextBox(NULL)
	, m_pListScrollBar(NULL)
	, m_pTextPage(NULL)
	, m_pTextInfoLevel(NULL)
	, m_pTextInfoHP(NULL)
	, m_pTextInfoAvoid(NULL)
	, m_pTextInfoAttack(NULL)
	, m_pTextInfoGold(NULL)
	, m_nSelectIndexType(-1)
	, m_nSlotNumber(0)
	, m_fClickTimer(10.0f)
	, m_pRender(NULL) // MOB RENDER
	, m_pMobNameText(NULL)
	, m_pButtonLeft(NULL)
	, m_pButtonRight(NULL) // END RENDER
{
	for( int i=0; i<BOSS_LIST_MAX_SLOT; ++i )
		m_pBossListSlot[i] = NULL;

	SecureZeroMemory ( m_pRewardItems, sizeof( m_pRewardItems ) );
}

CBossDetailsWindow::~CBossDetailsWindow ()
{
	
	m_RewardItemMap.clear();
	m_UniqueRewardItemSet.clear();
}

void CBossDetailsWindow::CreateSubControl ()
{

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_SHADOW_FLAG || _DEFAULT_FONT_SHADOW_FLAG ); // Locations

	//Window Result Black LineBox
	CBasicLineBox* pBasicLineBoxBlackResultParty = new CBasicLineBox;
	pBasicLineBoxBlackResultParty->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxBlackResultParty->CreateBaseBoxQuestList ( "BOSS_DETAILS_BLACK_BODY_RESULT_WINDOW_LINE" );
	RegisterControl ( pBasicLineBoxBlackResultParty );
	//Locations Black LineBox
	CBasicLineBox* pBasicLineBoxBlackLocations = new CBasicLineBox;
	pBasicLineBoxBlackLocations->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxBlackLocations->CreateBaseBoxQuestList ( "PARTY_SEARCH_BLACK_BODY_LOCATIONS_WINDOW_LINE" );
	RegisterControl ( pBasicLineBoxBlackLocations );
	//MOB NAME LINEBOX
	CBasicLineBox* pBasicLineBoxBlackMobName = new CBasicLineBox;
	pBasicLineBoxBlackMobName->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxBlackMobName->CreateBaseBoxQuestList ( "MOB_NAME_LINE_BOX_RENDER_JX" );
	RegisterControl ( pBasicLineBoxBlackMobName );
	//MOB NAME WHITE BORDER LINEBOX
	CBasicLineBox* pBasicLineBoxWhiteMobName = new CBasicLineBox;
	pBasicLineBoxWhiteMobName->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxWhiteMobName->CreateBaseBoxOuterBlankWhite ( "MOB_NAME_WHITE_LINE_BOX_RENDER_JX" );
	RegisterControl ( pBasicLineBoxWhiteMobName );
	//MOB RENDER LINEBOX
	CBasicLineBox* pBasicLineBoxBlackMobRender = new CBasicLineBox;
	pBasicLineBoxBlackMobRender->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxBlackMobRender->CreateBaseBoxQuestList ( "MOB_RENDER_LINE_BOX_RENDER_JX" );
	RegisterControl ( pBasicLineBoxBlackMobRender );
	//MOB RENDER WHITE BORDER LINEBOX
	CBasicLineBox* pBasicLineBoxWhiteMobRender = new CBasicLineBox;
	pBasicLineBoxWhiteMobRender->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxWhiteMobRender->CreateBaseBoxOuterBlankWhite ( "MOB_RENDER_WHITE_LINE_BOX_RENDER_JX" );
	RegisterControl ( pBasicLineBoxWhiteMobRender );
	//Locations White LineBox
	CBasicLineBox* pBasicLineBoxWhiteLocations = new CBasicLineBox;
	pBasicLineBoxWhiteLocations->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxWhiteLocations->CreateBaseBoxOuterBlankWhite ( "BOSS_DETAILS_WHITE_BODY_LOCATIONS_WINDOW_LINE" );
	RegisterControl ( pBasicLineBoxWhiteLocations );
	//Map Category White LineBox
	CBasicLineBox* pBasicLineBoxWhiteMapList = new CBasicLineBox;
	pBasicLineBoxWhiteMapList->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxWhiteMapList->CreateBaseBoxOuterBlankWhite ( "PARTY_SEARCH_WHITE_BODY_MAPLIST_WINDOW_LINE" );
	RegisterControl ( pBasicLineBoxWhiteMapList );
	//Search Button White LineBox
	CBasicLineBox* pBasicLineBoxWhiteSearchButton = new CBasicLineBox;
	pBasicLineBoxWhiteSearchButton->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxWhiteSearchButton->CreateBaseBoxOuterBlankWhite ( "PARTY_SEARCH_WHITE_BODY_SEARCH_BUTTON_WINDOW_LINE" );
	RegisterControl ( pBasicLineBoxWhiteSearchButton );
	//Window Result White LineBox
	CBasicLineBox* pBasicLineBoxWhiteResultParty = new CBasicLineBox;
	pBasicLineBoxWhiteResultParty->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxWhiteResultParty->CreateBaseBoxOuterBlankWhite ( "BOSS_DETAILS_WHITE_BODY_RESULT_WINDOW_LINE" );
	RegisterControl ( pBasicLineBoxWhiteResultParty );
	//Prev - Next White LineBox
	CBasicLineBox* pBasicLineBoxWhitePrevNext = new CBasicLineBox;
	pBasicLineBoxWhitePrevNext->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBoxWhitePrevNext->CreateBaseBoxOuterBlankWhite ( "PARTY_SEARCH_WHITE_BODY_PREVNEXT_WINDOW_LINE" );
	RegisterControl ( pBasicLineBoxWhitePrevNext );

	CBasicTextBox* pTextStaticDrop = new CBasicTextBox;
	pTextStaticDrop->CreateSub ( this, "BOSS_DETAILS_DROP_STATIC_TEXT" );
	pTextStaticDrop->SetFont ( pFont10 );
	pTextStaticDrop->SetTextAlign ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pTextStaticDrop->SetText( "LOOTS" );
	RegisterControl ( pTextStaticDrop );

	CBasicTextBox* pTextStaticInfo = new CBasicTextBox;
	pTextStaticInfo->CreateSub ( this, "BOSS_DETAILS_INFO_STATIC_TEXT" );
	pTextStaticInfo->SetFont ( pFont10 );
	pTextStaticInfo->SetTextAlign ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pTextStaticInfo->SetText( "BOSS INFORMATION" );
	RegisterControl ( pTextStaticInfo );

	//Locations White LineBox
	CBasicLineBox* pLineForItems = new CBasicLineBox;
	pLineForItems->CreateSub ( this, "BASIC_LINE_BOX_BODY_OUTER_BLANK_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineForItems->CreateBaseBoxOuterBlankWhite ( "BOSS_DETAILS_WHITE_BODY_ITEMS_WINDOW_LINE" );
	RegisterControl ( pLineForItems );

	//Locations White LineBox
	CBasicLineBox* pLineBoxForStaticDrop = new CBasicLineBox;
	pLineBoxForStaticDrop->CreateSub ( this, "BASIC_LINE_BOX_BODY_OUTER_BLANK_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBoxForStaticDrop->CreateBaseBoxOuterBlankWhite ( "BOSS_DETAILS_WHITE_TITLE_ITEMS_WINDOW_LINE" );
	RegisterControl ( pLineBoxForStaticDrop );

	//Locations White LineBox
	CBasicLineBox* pLineBodyBossInfo = new CBasicLineBox;
	pLineBodyBossInfo->CreateSub ( this, "BASIC_LINE_BOX_BODY_OUTER_BLANK_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBodyBossInfo->CreateBaseBoxOuterBlankWhite ( "BOSS_DETAILS_WHITE_BODY_INFO_WINDOW_LINE" );
	RegisterControl ( pLineBodyBossInfo );

	//Locations White LineBox
	CBasicLineBox* pLineTitleInformation = new CBasicLineBox;
	pLineTitleInformation->CreateSub ( this, "BASIC_LINE_BOX_BODY_OUTER_BLANK_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineTitleInformation->CreateBaseBoxOuterBlankWhite ( "BOSS_DETAILS_WHITE_TITLE_INFO_WINDOW_LINE" );
	RegisterControl ( pLineTitleInformation );

	for (int v=0; v < REWARD_V_LINE_COUNT; ++v)
	{
		for (int h=0; h < REWARD_H_LINE_COUNT; ++h)
		{
			int index = v*REWARD_H_LINE_COUNT + h;  
			CString strFormat;
			strFormat.Format("MAP_WINDOW_INFO_TAP_MONSTER_REWARD%d", index + 1);
			m_pRewardItems[v][h] = CreateItemImage(strFormat.GetString(),REWARD_ITEM1 + index );
		}
	}

	std::string strSlot[BOSS_LIST_MAX_SLOT] =
	{
		"BOSS_LIST_SLOT_NODE_00",
		"BOSS_LIST_SLOT_NODE_01",
		"BOSS_LIST_SLOT_NODE_02",
		"BOSS_LIST_SLOT_NODE_03",
		"BOSS_LIST_SLOT_NODE_04",
		"BOSS_LIST_SLOT_NODE_05",
		"BOSS_LIST_SLOT_NODE_06",
		"BOSS_LIST_SLOT_NODE_07",
	};

	for( int i=0; i<BOSS_LIST_MAX_SLOT; ++i )
	{
		m_pBossListSlot[i] = new CBossListSlot;
		m_pBossListSlot[i]->CreateSub ( this, strSlot[i].c_str(), UI_FLAG_DEFAULT, BOSS_DETAILS_SLOT1 + i );
		m_pBossListSlot[i]->CreateSubControl ();
		m_pBossListSlot[i]->SetVisibleSingle ( TRUE );
		RegisterControl ( m_pBossListSlot[i] );
	}


	m_pTextPage = new CBasicTextBox;
	m_pTextPage->CreateSub ( this, "PARTY_SEARCH_ALLMAP_TEXT" );
	m_pTextPage->SetFont ( pFont10 );
	m_pTextPage->SetTextAlign ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pTextPage->SetText( "Location" );
	RegisterControl ( m_pTextPage );

	m_pTextInfoLevel = new CBasicTextBox;
	m_pTextInfoLevel->CreateSub ( this, "BOSS_DETAILS_INFO_STATIC_LEVEL" );
	m_pTextInfoLevel->SetFont ( pFont9 );
	m_pTextInfoLevel->SetTextAlign ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pTextInfoLevel );

	m_pTextInfoHP = new CBasicTextBox;
	m_pTextInfoHP->CreateSub ( this, "BOSS_DETAILS_INFO_STATIC_HP" );
	m_pTextInfoHP->SetFont ( pFont9 );
	m_pTextInfoHP->SetTextAlign ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pTextInfoHP );

	m_pTextInfoAttack = new CBasicTextBox;
	m_pTextInfoAttack->CreateSub ( this, "BOSS_DETAILS_INFO_STATIC_ATTACK" );
	m_pTextInfoAttack->SetFont ( pFont9 );
	m_pTextInfoAttack->SetTextAlign ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pTextInfoAttack );

	m_pTextInfoAvoid = new CBasicTextBox;
	m_pTextInfoAvoid->CreateSub ( this, "BOSS_DETAILS_INFO_STATIC_AVOID" );
	m_pTextInfoAvoid->SetFont ( pFont9 );
	m_pTextInfoAvoid->SetTextAlign ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pTextInfoAvoid );

	m_pTextInfoGold = new CBasicTextBox;
	m_pTextInfoGold->CreateSub ( this, "BOSS_DETAILS_INFO_STATIC_GOLD" );
	m_pTextInfoGold->SetFont ( pFont9 );
	m_pTextInfoGold->SetTextAlign ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( m_pTextInfoGold );

	/* MOB RENDER*/
	m_pRender = new CMobPreviewWindowRender;
	m_pRender->CreateSub(this, "MOBPREVIEW_WINDOW_MESHRENDER");
	m_pRender->CreateSubControl();
	RegisterControl(m_pRender);

	m_pMobNameText = new CBasicTextBox;
	m_pMobNameText->CreateSub(this, "MOBPREVIEW_WINDOW_MOB_NAME_TEXT", TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
	m_pMobNameText->SetFont(pFont9);
	m_pMobNameText->SetTextAlign(TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
	RegisterControl(m_pMobNameText);

	m_pButtonLeft = new CBasicButton;
	m_pButtonLeft->CreateSub(this, "MOBPREVIEW_WINDOW_ARROW_TURN_LEFT_DEFAULT", UI_FLAG_DEFAULT, BUTTON_LEFT);
	m_pButtonLeft->CreateFlip("MOBPREVIEW_WINDOW_ARROW_TURN_LEFT_CLICK", CBasicButton::CLICK_FLIP);
	m_pButtonLeft->CreateMouseOver("MOBPREVIEW_WINDOW_ARROW_TURN_LEFT_OVER");
	m_pButtonLeft->SetVisibleSingle(TRUE);
	RegisterControl(m_pButtonLeft);

	m_pButtonRight = new CBasicButton;
	m_pButtonRight->CreateSub(this, "MOBPREVIEW_WINDOW_ARROW_TURN_RIGHT_DEFAULT", UI_FLAG_DEFAULT, BUTTON_RIGHT);
	m_pButtonRight->CreateFlip("MOBPREVIEW_WINDOW_ARROW_TURN_RIGHT_CLICK", CBasicButton::CLICK_FLIP);
	m_pButtonRight->CreateMouseOver("MOBPREVIEW_WINDOW_ARROW_TURN_RIGHT_OVER");
	m_pButtonRight->SetVisibleSingle(TRUE);
	RegisterControl(m_pButtonRight);

	/* RENDER END */
	m_pListTextBox = new CBasicTextBoxEx;
	m_pListTextBox->CreateSub ( this, "PARTY_SEARCH_CATEGORY_TEXT", UI_FLAG_DEFAULT, BOSS_DETAILS_MAP_LIST );
	m_pListTextBox->SetFont ( pFont9 );		
	m_pListTextBox->SetLineInterval ( 3.0f );
	m_pListTextBox->SetSensitive ( true );
	m_pListTextBox->SetLimitLine ( 10000 );
	RegisterControl ( m_pListTextBox );

	int nTotalLine = m_pListTextBox->GetVisibleLine ();

	m_pListScrollBar = new CBasicScrollBarEx;
	m_pListScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, BOSS_DETAILS_SCROLL );
	m_pListScrollBar->CreateBaseScrollBar ( "PARTY_SEARCH_LOCATION_CATEGORY_SCROLL" );
	m_pListScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
	RegisterControl ( m_pListScrollBar );

	/*m_pTextPage = new CBasicTextBox;
	m_pTextPage->CreateSub ( this, "PARTY_SEARCH_PAGE_NUM_TEXT" );
	m_pTextPage->SetFont ( pFont9 );
	m_pTextPage->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextPage->SetText( "0" );
	RegisterControl ( m_pTextPage );*/

	LOAD_MAP_LIST();

}

void CBossDetailsWindow::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListTextBox->SetUseOverColor ( FALSE );
	if ( m_pListTextBox->GetCount () && 0 <= m_nSelectIndexType )
		m_pListTextBox->SetUseTextColor ( m_nSelectIndexType, FALSE );
	
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if (m_fClickTimer > 0.0f)
	{
		m_fClickTimer-=fElapsedTime;
	}

	if ( m_pListTextBox->GetCount () && 0 <= m_nSelectIndexType )
	{
		m_pListTextBox->SetUseTextColor ( m_nSelectIndexType, TRUE );
		m_pListTextBox->SetTextColor ( m_nSelectIndexType, NS_UITEXTCOLOR::ORANGE );
	}

	CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();

	const int nTotalLine = m_pListTextBox->GetTotalLine ();
	const int nLinePerOneView = m_pListTextBox->GetVisibleLine ();

	pThumbFrame->SetState ( nTotalLine, nLinePerOneView );

	if ( nLinePerOneView < nTotalLine )
	{
		const int nMovableLine = nTotalLine - nLinePerOneView;
		float fPercent = pThumbFrame->GetPercent ();
		int nPos = (int)floor(fPercent * nMovableLine);
		m_pListTextBox->SetCurLine ( nPos );			
	}
}

void CBossDetailsWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	/* RENDER HERE*/
	case BUTTON_RIGHT:
	{
		if (CHECK_MOUSE_IN_LBDOWNLIKE(dwMsg))
		{
			if (m_pRender)	m_pRender->m_vRot.x += 0.1f;
		}
	}break;
	case BUTTON_LEFT:
	{
		if (CHECK_MOUSE_IN_LBDOWNLIKE(dwMsg))
		{
			if (m_pRender)	m_pRender->m_vRot.x -= 0.1f;
		}
	}break;
	/*RENDER END*/
	case BOSS_DETAILS_MAP_LIST:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pListTextBox->GetSelectPos ();

				if (nIndex == m_nSelectIndexType) return;
				if ( m_pListTextBox->GetCount () <= nIndex || nIndex < 0  ) return ;

				if ( UIMSG_LB_UP & dwMsg )
				{
					DWORD dwType = m_pListTextBox->GetTextData ( nIndex );
					m_nSelectIndexType = nIndex;
					SelectMapRequest(dwType);
					m_fClickTimer = 10.0f;
				}

				m_pListTextBox->SetUseOverColor ( TRUE );
				m_pListTextBox->SetOverColor ( nIndex, NS_UITEXTCOLOR::ORANGE );
			}
		}
		break;
	case BOSS_DETAILS_SLOT1:
	case BOSS_DETAILS_SLOT2:
	case BOSS_DETAILS_SLOT3:
	case BOSS_DETAILS_SLOT4:
	case BOSS_DETAILS_SLOT5:
	case BOSS_DETAILS_SLOT6:
	case BOSS_DETAILS_SLOT7:
	case BOSS_DETAILS_SLOT8:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			if (UIMSG_LB_UP & dwMsg)
			{
				for (int i = 0; i < BOSS_LIST_MAX_SLOT; ++i)
				{
					if (m_pBossListSlot[i]->GetWndID() == ControlID)
					{
						// Check if the slot is valid (not empty)
						if (m_pBossListSlot[i]->GetMobID() != NATIVEID_NULL()) // Assuming NATIVEID_NULL() indicates an empty slot
						{
							ResetItems();
							GetDropItem(m_pBossListSlot[i]->GetMobID());
							StartPreviewMob(m_pBossListSlot[i]->GetMobID());
						}
						else
						{
                        // Handle the case where the slot is empty (optional)
                        // Example: Show a message or ignore the click
						}
					}
				}
			}
		}
	} break;
	};

	for (int v=0; v < REWARD_V_LINE_COUNT; ++v)
	{
		for (int h=0; h < REWARD_H_LINE_COUNT; ++h)
		{
			if( m_pRewardItems[v][h]->GetWndID() == ControlID )
			{
				if( CHECK_MOUSE_IN(dwMsg) )
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem( m_pRewardItems[v][h]->GetItemID() );
					if ( pItem )
					{
						SITEMCUSTOM sItemCustom;
						sItemCustom.sNativeID = pItem->sBasicOp.sNativeID;
						sItemCustom.cDAMAGE = pItem->sBasicOp.wGradeAttack;
						sItemCustom.cDEFENSE = pItem->sBasicOp.wGradeDefense;
						sItemCustom.wTurnNum = pItem->sDrugOp.wPileNum;
						CInnerInterface::GetInstance().SHOW_ITEM_INFO( sItemCustom, FALSE, FALSE, FALSE, 0, 0 );
					}
				}
			}
		}
	}
}

void CBossDetailsWindow::SelectMapRequest(DWORD dwType) 
{
	GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
	GLMapList::FIELDMAP_ITER iter = MapsList.find ( dwType );

	m_nSlotNumber = 0;

	for( int i=0; i<BOSS_LIST_MAX_SLOT; ++i )
		m_pBossListSlot[i]->ItemReset();


	ResetItems ();

	for (int v=0; v < REWARD_V_LINE_COUNT; ++v)
	{
		for (int h=0; h < REWARD_H_LINE_COUNT; ++h)
		{
			m_pRewardItems[v][h]->ResetItem();
			m_pRewardItems[v][h]->SetVisibleSingle(FALSE);
		}
	}


	const SMAPNODE *pMapNode = &(*iter).second;
	GLGaeaClient::GetInstance().GetCharacter()->ReqBossDetails(0, pMapNode->sNativeID, NATIVEID_NULL());
}

void CBossDetailsWindow::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}else{
		ClearRender(); //RENDER HERE
	}
}

void CBossDetailsWindow::ClearRender()
{
	if (m_pRender)
		m_pRender->ClearRender();
}

void CBossDetailsWindow::StartPreviewMob(SNATIVEID sMobID)
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if (!pCharacter)		return;

	if (!m_pRender)	return;
	//////////////////////////////////////////////////////////////////////////////
	//--load & insert drop item list
	//m_pMobDBItemList->Reset();

	/////////////////////////////////////////////////
	//--version 1, each map code to each crow in every map
	SCROWDATA* pCROW = GLCrowDataMan::GetInstance().GetCrowData(sMobID);

	if (pCROW)
	{

		CString strTmp;
		strTmp.Format("< %s >", pCROW->GetName());
		m_pMobNameText->SetOneLineText(strTmp, NS_UITEXTCOLOR::CHARTREUSE);
	}
	ClearRender();
	m_pRender->m_bReady = true;
	m_pRender->m_sMobID = sMobID;
}

void CBossDetailsWindow::LOAD_MAP_LIST()
{
	GLMapList::FIELDMAP MapsList = GLGaeaClient::GetInstance().GetMapList ();
	GLMapList::FIELDMAP_ITER iter_start = MapsList.begin();
	GLMapList::FIELDMAP_ITER iter_end = MapsList.end();

	for( ; iter_start != iter_end; ++iter_start )
	{
		const SMAPNODE *pMapNode = &(*iter_start).second;
		if ( pMapNode )
		{
			if ( pMapNode->bBossMap )
			{
				SMAPDATA_LIST sMAP;
				sMAP.sMAPID = pMapNode->sNativeID;
				sMAP.strNAME = pMapNode->strMapName.c_str();

				int nIndex = m_pListTextBox->AddText ( pMapNode->strMapName.c_str(), NS_UITEXTCOLOR::WHITE );
				m_pListTextBox->SetTextData ( nIndex, sMAP.sMAPID.dwID );
			}
		}	
	}
}

void CBossDetailsWindow::SetSlotItem ( SNATIVEID sMobID, float fTimer, BOOL isAlive ) 
{
	if (m_nSlotNumber >= BOSS_LIST_MAX_SLOT) return;

	if( m_pBossListSlot[m_nSlotNumber] )
	{
		PCROWDATA pCROW = GLCrowDataMan::GetInstance().GetCrowData( sMobID );
		if(pCROW)
		{
			m_pBossListSlot[m_nSlotNumber]->ItemSet( pCROW->GetName(), fTimer, isAlive );
			m_pBossListSlot[m_nSlotNumber]->SetMobID( sMobID );
			m_nSlotNumber++;
		}
	}
}

void CBossDetailsWindow::GetDropItem (SNATIVEID sMobID)
{
	SCROWDATA* pCROW = GLCrowDataMan::GetInstance().GetCrowData ( sMobID );
	if ( pCROW )
	{
		SetBossInformation( pCROW );
		MOB_DROP_SPECID_MAP_ITER iter_MDSID = pCROW->m_mapMobDropSpecID.begin ();
		MOB_DROP_SPECID_MAP_ITER iter_MDSID_end = pCROW->m_mapMobDropSpecID.end ();
		
		for( ; iter_MDSID != iter_MDSID_end; ++iter_MDSID )
		{
			ITEMDBDROP_RANGE sRANGE;
			sRANGE = GLItemMan::GetInstance().m_mapItemDBDropKey.equal_range ( iter_MDSID->second );
			
			ITEMDBDROP_KEYMAP_ITER iter_RANGE = sRANGE.first;
			ITEMDBDROP_KEYMAP_ITER iter_RANGE_end = sRANGE.second;
			
			for ( ; iter_RANGE != iter_RANGE_end; ++iter_RANGE )
			{
				SITEM* pItem = GLItemMan::GetInstance().GetItem( iter_RANGE->second.sItemID );
				if( !pItem )						continue;

				REWARDITEM_MAP_ITER it = m_RewardItemMap.find( iter_RANGE->second.sItemID.dwID );
				if( it != m_RewardItemMap.end() )	continue;
				
				m_RewardItemMap.insert( std::make_pair( pItem->sBasicOp.sNativeID.dwID, pItem->sBasicOp.sNativeID ) );
			}
		}			
	}

	for( REWARDITEM_MAP_ITER iter = m_RewardItemMap.begin();
		iter != m_RewardItemMap.end(); ++iter )
	{
		const SNATIVEID& sID = (*iter).second;
		m_UniqueRewardItemSet.insert( sID.dwID );
	}

	for (int v=0; v < REWARD_V_LINE_COUNT; ++v)
	{
		for (int h=0; h < REWARD_H_LINE_COUNT; ++h)
		{
			size_t index = (v)*REWARD_H_LINE_COUNT + h;  
			if( index >= m_UniqueRewardItemSet.size())
			{
				m_pRewardItems[v][h]->SetVisibleSingle(FALSE);
				continue;
			}
			
			std::set<DWORD>::iterator it = m_UniqueRewardItemSet.begin();
			std::advance(it,index); 

			SITEM* pItem = GLItemMan::GetInstance().GetItem( SNATIVEID(*it) );
			if( !pItem )	continue;

			m_pRewardItems[v][h]->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile(), pItem->sBasicOp.sNativeID );		
			m_pRewardItems[v][h]->SetVisibleSingle(TRUE);
		}
	}
}
void CBossDetailsWindow::SetBossInformation( SCROWDATA* pCROW )
{
	if( m_pTextInfoLevel )
	{
		CString level;
		level.Format("Level: %d", pCROW->m_wLevel);

		m_pTextInfoLevel->SetText(level);
	}

	if( m_pTextInfoHP )
	{
		CString hp;
		hp.Format("HP: %d", pCROW->m_dwHP);

		m_pTextInfoHP->SetText(hp);
	}

	if ( m_pTextInfoAttack )
	{
		CString attack;
		attack.Format("Attack: %d ~ %d", pCROW->m_sCrowAttack->sDamage.wLow, pCROW->m_sCrowAttack->sDamage.wHigh);

		m_pTextInfoAttack->SetText(attack);
	}

	if ( m_pTextInfoAvoid )
	{
		CString avoid;
		avoid.Format("Avoid: %d", pCROW->m_wAvoidRate);

		m_pTextInfoAvoid->SetText(avoid);
	}

	if ( m_pTextInfoGold )
	{
		CString gold;
		gold.Format("Gold: %d", pCROW->m_sGenerate.m_dwGenMoney);

		m_pTextInfoGold->SetText(gold);
	}
}
void CBossDetailsWindow::ResetItems () 
{
	m_RewardItemMap.clear();
	m_UniqueRewardItemSet.clear();

	if( m_pTextInfoLevel )
	{
		m_pTextInfoLevel->ClearText();
	}

	if( m_pTextInfoHP )
	{
		m_pTextInfoHP->ClearText();
	}

	if ( m_pTextInfoAttack )
	{
		m_pTextInfoAttack->ClearText();
	}

	if ( m_pTextInfoAvoid )
	{
		m_pTextInfoAvoid->ClearText();
	}

	if ( m_pTextInfoGold )
	{
		m_pTextInfoGold->ClearText();
	}
}
CItemImage*	CBossDetailsWindow::CreateItemImage ( const char* szControl, UIGUID ControlID )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );

	return pItemImage;
}