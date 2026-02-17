#include "stdafx.h"
#include "./LargeMapWindow.h"

#include "./MapWindowMark.h"
#include "./BasicTextBox.h"

#include "./InnerInterface.h"
#include "./GameTextControl.h"

#include "../../Lib_Engine/DxCommon/d3dfont.h"
#include "../../Lib_Engine/DxOctree/DxLandGateMan.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLQuestMan.h"
#include "../../Lib_Client/G-Logic/GLQuest.h"

#include "../../Lib_Client/NpcTalk/NpcTalk.h"
#include "../../Lib_Client/NpcTalk/NpcDialogue.h"
#include "../../Lib_Client/NpcTalk/NpcTalkControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CLargeMapWindow::MAP_MARK_RESET()
{
	for ( UINT i=0; i<m_vecMapMark.size(); ++i )
	{
		const SMAP_MARK_DATA& sData = m_vecMapMark[i];
		DeleteControl ( sData.cID, 0 );
	}

	m_vecMapMark.clear();
}

void CLargeMapWindow::MAP_MARK_INIT()
{
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if ( !pLand	)	return;
	if ( !pLand->IsUIMapInfo() )	return;

	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pCharacter ) return;

	DxLandGateMan*	pGateMan = &pLand->GetLandGateMan();
	if ( pGateMan )	
	{
		PDXLANDGATE pLandGate = pGateMan->GetListLandGate();

		while ( pLandGate )
		{	
			if ( pLandGate->GetFlags() & DxLandGate::GATE_OUT )
			{
				/* multi gate out, Juver, 2020/11/17 */
				std::string strName("");

				if ( pLandGate->GetNewSystem() )
				{
					for ( DWORD i = 0; i<pLandGate->GetToMapNum(); ++i )
					{
						SNATIVEID sToMapID = pLandGate->GetToMapID(i);
						SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( sToMapID );
						if ( pMapNode )
						{
							DWORD dwNameINDEX = pLandGate->GetNameINDEX(i);
							std::string strNameKEY = pLandGate->GetNameKEY(i);

							std::string strTEMP("");

							if ( dwNameINDEX != DxLandGate::GATE_OUT_INDEX_NULL && strNameKEY.size() )
								strTEMP = ID2GAMEWORD( strNameKEY.c_str(), dwNameINDEX );

							if ( strTEMP.empty() )
								strTEMP = pMapNode->strMapName.c_str();

							if ( strName.size() )	strName += ",";
							strName += strTEMP;
						}
					}
				}
				else
				{
					SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( pLandGate->GetToMapID( DxLandGate::DEFAULT_GATE_OUT_INDEX ) );
					if ( pMapNode )	
						strName = pMapNode->strMapName;
				}
				

				float fDistX = pLandGate->GetMax().x-pLandGate->GetMin().x;
				float fDistY = pLandGate->GetMax().y-pLandGate->GetMin().y;
				float fDistZ = pLandGate->GetMax().z-pLandGate->GetMin().z;

				float fCenterX = pLandGate->GetMin().x + ( fDistX / 2.0f );
				float fCenterY = pLandGate->GetMin().y + ( fDistY / 2.0f );
				float fCenterZ = pLandGate->GetMin().z + ( fDistZ / 2.0f );

				MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_GATE_MARK", strName.c_str(), D3DXVECTOR3( fCenterX, fCenterY, fCenterZ ));
			}

			pLandGate = pLandGate->m_pNext;
		}
	}

	MOBSCHEDULELIST * GLMobSchList = pLand->GetMobSchMan()->GetMobSchList();
	if ( GLMobSchList	)
	{
		SGLNODE<GLMobSchedule*>* pCur = GLMobSchList->m_pHead;
		while ( pCur )
		{
			SGLNODE<GLMobSchedule*>* pSched = pLand->GetMobSchMan()->FindMobSch( pCur->Data->m_szName );
			PCROWDATA pCROW = GLCrowDataMan::GetInstance().GetCrowData( pCur->Data->m_CrowID );

			/*hide crow name, Juver, 2017/09/01 */
			if ( pSched && pCROW && !pCROW->m_sBasic.m_bHideName )
			{
				D3DXVECTOR3 vPos = pSched->Data->m_pAffineParts->vTrans;

				if ( pCROW->m_sBasic.m_emCrow == CROW_NPC )
				{
					//npc function, quest
					CInnerInterface::INTERFACE_CSTRING_VEC	vec_npc_function;
					CInnerInterface::INTERFACE_CSTRING_VEC	vec_quest_start;
					CInnerInterface::INTERFACE_CSTRING_VEC	vec_quest_step;
					CInnerInterface::INTERFACE_CSTRING_VEC	vec_quest_complete;

					CNpcDialogueSet		m_DialogueSet;
					BOOL bLoad = m_DialogueSet.Load ( pCROW->GetTalkFile() );
					if ( bLoad )
					{
						CInnerInterface::GetInstance().target_information_load_talk_node( &m_DialogueSet, 
																						pCharacter, 
																						1, 
																						vec_npc_function, 
																						vec_quest_start, 
																						vec_quest_step, 
																						vec_quest_complete );
					}


					if ( vec_quest_complete.size() )
					{
						MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_QUEST_FINISH_MARK", pCROW->GetName(), vPos );
					}
					else if ( vec_quest_step.size() )
					{
						MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_QUEST_STEP_MARK", pCROW->GetName(), vPos );
					}
					else if ( vec_quest_start.size() )
					{
						MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_QUEST_START_MARK", pCROW->GetName(), vPos );
					}
					else
					{
						if ( m_DialogueSet.IsBUSSTATION() )
						{
							MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_BUS_MARK", pCROW->GetName(), vPos );
						}
						else if ( m_DialogueSet.IsSTORAGE() || m_DialogueSet.IsCLUBSTORAGE() )
						{
							MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_LOOKER_MARK", pCROW->GetName(), vPos );
						}
						else if ( m_DialogueSet.IsSTARTPOINT() )
						{
							MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_START_POS_MARK", pCROW->GetName(), vPos );
						}
						else if ( m_DialogueSet.IsCLUBNEW() || m_DialogueSet.IsCLUBRANKUP() )
						{
							MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_CLUB_MANAGER_MARK", pCROW->GetName(), vPos );
						}
						else if ( m_DialogueSet.IsMARKET() )
						{
							MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_SEARCH_CHARACTER_MARK", pCROW->GetName(), vPos );
						}
						else
						{
							MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_NORMAL_MARK", pCROW->GetName(), vPos );
						}
					}

	

				}
				else if ( pCROW->m_sBasic.m_emCrow == CROW_ZONE_NAME || pCROW->m_sBasic.m_emCrow == CROW_GATE_NAME )
				{
					MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_LANDMARK_MARK", pCROW->GetName(), vPos );
					MAP_MARK_CREATE_LABEL( pCROW->GetName(), vPos );
				}
				else if ( pCROW->m_sBasic.m_emCrow == CROW_MOB || pCROW->m_sBasic.m_emCrow == CROW_MATERIAL )
				{
					/*
					//quest mob
					GLQuestMan & glQuestMan = GLQuestMan::GetInstance();

					BOOL bDone = FALSE;
					GLQuestPlay::MAPQUEST &mapPROC = pCharacter->m_cQuestPlay.GetQuestProc();
					GLQuestPlay::MAPQUEST_ITER iter = mapPROC.begin();
					GLQuestPlay::MAPQUEST_ITER iter_end = mapPROC.end();
					for ( ; iter!=iter_end; ++iter )
					{
						GLQUESTPROG* pPROG = (*iter).second;
						if ( !pPROG )	continue;

						GLQUEST* pQUEST = glQuestMan.Find ( pPROG->m_sNID.dwID );
						if ( !pQUEST )	continue;

						GLQUEST_STEP* pSTEP = pQUEST->GetSTEP ( pPROG->m_dwSTEP );
						if ( !pSTEP )	continue;

						//mob kill quest
						if ( !bDone && !pPROG->m_sSTEP_PROG.m_bMOBKILL_ALL && pCROW->m_sBasic.sNativeID.dwID == pSTEP->m_dwNID_MOBKILL )
						{
							MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_MONSTER_2", pCROW->GetName(), vPos );
							bDone = TRUE;
						}

						//item gen from mob kill quest
						if ( !bDone && !pPROG->m_sSTEP_PROG.m_bQITEM )
						{
							BOOL have_mob_gen = FALSE;

							GLInventory sTEMP_INVEN;
							sTEMP_INVEN.Assign ( pPROG->m_sINVENTORY );

							GENMOBITEMARRAY_ITER iter = pSTEP->m_vecMOBGEN_QITEM.begin();
							GENMOBITEMARRAY_ITER iter_end = pSTEP->m_vecMOBGEN_QITEM.end();
							for ( ; iter!=iter_end; ++iter )
							{
								SGENQUESTITEM &sGENQITEM = (*iter);

								bool bFOUND = sGENQITEM.Find ( pCROW->m_sBasic.sNativeID.dwID );
								if ( !bFOUND )	continue;

								MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_MONSTER_2", pCROW->GetName(), vPos );
								bDone = TRUE;
							}
						}
					}

					if ( !bDone )
						MAP_MARK_CREATE_MARK( "MAP_WINDOW_SETTING_MONSTER_1", pCROW->GetName(), vPos );

					*/
				}
			}


			pCur = pCur->pNext;
		}
	}
}

void CLargeMapWindow::MAP_MARK_UPDATE()
{
	for ( UINT i=0; i<m_vecMapMark.size(); ++i )
	{
		const SMAP_MARK_DATA& sData = m_vecMapMark[i];
		CUIControl* pControl = FindControl( sData.cID );
		if ( pControl )
		{
			pControl->SetVisibleSingle( FALSE );
			UPDATE_CHAR_POS( pControl, sData.vPos, sData.bTextBox );
		}
	}
}

bool CLargeMapWindow::MAP_MARK_CREATE_MARK( std::string strMark, std::string strLabel, const D3DXVECTOR3& vPos )
{
	m_NEWID++;
	if ( MAP_MARK_END <= m_NEWID ) m_NEWID = MAP_MARK;

	CMapWindowMark* pMark = new CMapWindowMark;
	pMark->CreateSub ( this, strMark.c_str(), UI_FLAG_DEFAULT, m_NEWID );
	pMark->CreateSubControl();
	pMark->InitDeviceObjects ( m_pd3dDevice );
	pMark->RestoreDeviceObjects ( m_pd3dDevice );	
	pMark->strTEXT = strLabel.c_str();
	pMark->SetVisibleSingle ( FALSE );
	RegisterControl ( pMark );	
	
	SMAP_MARK_DATA sMapMarkData;
	sMapMarkData.cID = m_NEWID;
	sMapMarkData.vPos = vPos;
	sMapMarkData.bTextBox = false;
	m_vecMapMark.push_back( sMapMarkData );

	return true;
}

bool CLargeMapWindow::MAP_MARK_CREATE_LABEL ( std::string strLabel, const D3DXVECTOR3& vPos )
{
	m_NEWID++;
	if ( MAP_MARK_END <= m_NEWID ) m_NEWID = MAP_MARK;

	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox*	pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, "MAP_WINDOW_SETTING_LABEL", UI_FLAG_XSIZE | UI_FLAG_YSIZE, m_NEWID );
	pTextBox->SetFont ( pFont );
	pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	pTextBox->AddText( strLabel.c_str(), NS_UITEXTCOLOR::DARKORANGE );
	pTextBox->SetVisibleSingle ( FALSE );
	pTextBox->SetNoUpdate(TRUE);
	RegisterControl ( pTextBox );

	const UIRECT& rcTextBoxLocalPos = pTextBox->GetLocalPos ();
	const UIRECT& rcLocalPosDummy = m_pMarkDummy->GetLocalPos ();
	pTextBox->AlignSubControl ( rcTextBoxLocalPos, rcLocalPosDummy );

	int nTotalLine = pTextBox->GetTotalLine ();
	const float fAllLine = pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
	const float fTextBoxGapX = (float)pTextBox->GetLongestLine () - rcTextBoxLocalPos.sizeX;
	const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;

	UIRECT rcLocalNewPos = rcTextBoxLocalPos;
	rcLocalNewPos.sizeX += fTextBoxGapX;
	rcLocalNewPos.sizeY += fTextBoxGapY;
	pTextBox->AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );
	pTextBox->SetGlobalPos ( rcLocalNewPos );		

	pTextBox->SetAlignFlag( UI_FLAG_DEFAULT );

	SMAP_MARK_DATA sMapMarkData;
	sMapMarkData.cID = m_NEWID;
	sMapMarkData.vPos = vPos;
	sMapMarkData.bTextBox = true;
	m_vecMapMark.push_back( sMapMarkData );

	return true;
}
