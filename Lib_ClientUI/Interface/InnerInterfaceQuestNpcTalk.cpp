#include "StdAfx.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "../../Lib_Client/G-Logic/UserTypeDefine.h"

#include "./InnerInterface.h"

#include "./BasicVarTextBox.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLPartyClient.h"

#include "../../Lib_Client/G-Logic/GLQuestMan.h"
#include "../../Lib_Client/G-Logic/GLQuest.h"

#include "../../Lib_Client/NpcTalk/NpcTalk.h"
#include "../../Lib_Client/NpcTalk/NpcDialogue.h"
#include "../../Lib_Client/NpcTalk/NpcTalkControl.h"

#include "./DialogueWindow.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*target information display, Juver, 2018/08/22 */
void CInnerInterface::target_information_set ( STARGETID target_id )
{
	if ( !RPARAM::use_target_information )	return;

	m_target_id_information = target_id;
}

/*target information display, Juver, 2018/08/22 */
void CInnerInterface::target_information_show ()
{
	GLCOPY *pcopy = GLGaeaClient::GetInstance().GetCopyActor ( m_target_id_information );
	if ( !pcopy ) return;

	GLCharacter* pcharacter = GLGaeaClient::GetInstance().GetCharacter();
	if ( !pcharacter ) return;

	if ( IsMouseInControl () ) return;

	if ( IsVisibleGroup ( TARGET_INFORMATION_DISPLAY ) ) return;

	if ( !m_ptarget_information_display )	return;

	m_ptarget_information_display->ClearText();

	switch ( m_target_id_information.emCrow )
	{
	case CROW_PC:
		{
			PGLCHARCLIENT pchar_client = GLGaeaClient::GetInstance().GetChar ( m_target_id_information.dwID );
			if ( pchar_client )
			{
				/*hide gm info, Juver, 2017/11/26 */
				if ( pchar_client->GetCharData().dwUserLevel >= NSUSER_TYPE::USER_TYPE_GM3 ) return;

				CString text;

				//level, name
				text.Format( "Lv.%d %s", pchar_client->GetCharData().wLevel, pchar_client->GetName() );

				DWORD name_color = NS_UITEXTCOLOR::PALEGREEN;
				if ( pchar_client->GetCharData().wLevel >= pcharacter->m_wLevel )
					name_color = NS_UITEXTCOLOR::RED;

				m_ptarget_information_display->AddTextNoSplit ( text.GetString(), name_color );

				//school
				if ( pchar_client->GetCharData().wSchool < GLSCHOOL_NUM )
				{
					DWORD school_color = NS_UITEXTCOLOR::WHITE;
					if ( pchar_client->GetCharData().wSchool != pcharacter->m_wSchool )
						school_color = NS_UITEXTCOLOR::RED;

					m_ptarget_information_display->AddTextNoSplit ( GLCONST_CHAR::strSCHOOLNAME[pchar_client->GetCharData().wSchool].c_str(), school_color );
				}

				//class
				EMCHARINDEX char_index = CharClassToIndex( pchar_client->GetCharData().emClass );
				if ( char_index < GLCI_NUM_8CLASS )
				{
					m_ptarget_information_display->AddTextNoSplit ( COMMENT::CHARCLASS[char_index].c_str(), NS_UITEXTCOLOR::WHITE );
				}

				//party info
				if ( pchar_client->GetCharData().wSchool == pcharacter->m_wSchool || GLCONST_CHAR::bPARTY_2OTHERSCHOOL )
				{
					DWORD party_id = GLPartyClient::GetInstance().GetPartyID();

					if ( pchar_client->IsPartyMem() && pchar_client->IsPartyMaster() )
					{
						if ( party_id != PARTY_NULL && party_id == pchar_client->GETPARTYID() )
							m_ptarget_information_display->AddTextNoSplit( ID2GAMEINTEXT("CURSOR_TARGET_PARTYMASTER"), NS_UITEXTCOLOR::BRIGHTGREEN );
						else
							m_ptarget_information_display->AddTextNoSplit( ID2GAMEINTEXT("CURSOR_TARGET_PARTYMASTER"), NS_UITEXTCOLOR::WHITE );
					}
					else if ( pchar_client->IsPartyMem() )
					{
						if ( party_id != PARTY_NULL && party_id == pchar_client->GETPARTYID() )
							m_ptarget_information_display->AddTextNoSplit( ID2GAMEINTEXT("CURSOR_TARGET_PARTYMEMBER"), NS_UITEXTCOLOR::BRIGHTGREEN );
						else
							m_ptarget_information_display->AddTextNoSplit( ID2GAMEINTEXT("CURSOR_TARGET_PARTYMEMBER"), NS_UITEXTCOLOR::WHITE );
					}
					else
						m_ptarget_information_display->AddTextNoSplit( ID2GAMEINTEXT("CURSOR_TARGET_PARTYNO"), NS_UITEXTCOLOR::DODGERBLUE );
				}

				//pkinfo
				if ( !pcharacter->IsReActionable( m_target_id_information ) )
					m_ptarget_information_display->AddTextNoSplit ( ID2GAMEINTEXT("CURSOR_TARGET_NONFREEPK"), NS_UITEXTCOLOR::WHITE );

				//shift info
				m_ptarget_information_display->AddTextNoSplit ( ID2GAMEINTEXT("CURSOR_TARGET_SHIFTCLICK"), NS_UITEXTCOLOR::YELLOW );

			}
		}break;

	case CROW_MOB:
		{
			PGLCROWCLIENT pcrow_client = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( m_target_id_information.dwID );
			if ( pcrow_client && pcrow_client->m_pCrowData )
			{
				/*hide crow name, Juver, 2017/09/01 */
				if ( pcrow_client->m_pCrowData->m_sBasic.m_bHideName )	return;

				CString text;

				//level, name
				text.Format( "Lv.%d %s", pcrow_client->GETLEVEL(), pcrow_client->GETNAME() );

				DWORD name_color = NS_UITEXTCOLOR::WHITE;
				if ( pcrow_client->GETLEVEL() >= pcharacter->m_wLevel )
					name_color = NS_UITEXTCOLOR::RED;

				m_ptarget_information_display->AddTextNoSplit ( text.GetString(), name_color );

				//crow school
				/*crow school, Juver, 2018/08/02 */
				if ( pcrow_client->m_pCrowData->m_sBasic.m_emCrowSchool != EMCROW_SCHOOL_SIZE && pcrow_client->m_pCrowData->m_sBasic.m_emCrowSchool < GLSCHOOL_NUM )
				{
					DWORD school_color = NS_UITEXTCOLOR::WHITE;
					if ( pcrow_client->m_pCrowData->m_sBasic.m_emCrowSchool != pcharacter->m_wSchool )
						school_color = NS_UITEXTCOLOR::RED;

					m_ptarget_information_display->AddTextNoSplit ( GLCONST_CHAR::strSCHOOLNAME[pcrow_client->m_pCrowData->m_sBasic.m_emCrowSchool].c_str(), school_color );
				}

				//hp
				float fPercent = (float) pcrow_client->GETHP()/pcrow_client->GETMAXHP();
				text.Format( "%d / %d (%d %s)", pcrow_client->GETHP(), pcrow_client->GETMAXHP(), int( fPercent * 100.0f ), "%" );

				m_ptarget_information_display->AddTextNoSplit ( text.GetString(), NS_UITEXTCOLOR::YELLOW );


				//quest mob
				GLQuestMan & glQuestMan = GLQuestMan::GetInstance();

				BOOL have_quest = FALSE;
				GLQuestPlay::MAPQUEST &mapPROC = pcharacter->m_cQuestPlay.GetQuestProc();
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
					if ( !pPROG->m_sSTEP_PROG.m_bMOBKILL_ALL && pcrow_client->m_pCrowData->m_sBasic.sNativeID.dwID == pSTEP->m_dwNID_MOBKILL )
					{
						if ( !have_quest )
						{
							m_ptarget_information_display->AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );
							m_ptarget_information_display->AddTextNoSplit ( ID2GAMEINTEXT("TARGET_INFO_QUEST_STEP"), NS_UITEXTCOLOR::WHITE );
							have_quest = TRUE;
						}

						text.Format( "Quest:%s", pQUEST->GetTITLE() );
						m_ptarget_information_display->AddTextNoSplit ( text.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN );
						text.Format( "Step:%s %d/%d", pSTEP->GetTITLE(), pPROG->m_sSTEP_PROG.m_dwNUM_MOBKILL, pSTEP->m_dwNUM_MOBKILL );
						m_ptarget_information_display->AddTextNoSplit ( text.GetString(), NS_UITEXTCOLOR::YELLOW );
					}

					//item gen from mob kill quest
					if ( !pPROG->m_sSTEP_PROG.m_bQITEM )
					{
						BOOL have_mob_gen = FALSE;

						GLInventory sTEMP_INVEN;
						sTEMP_INVEN.Assign ( pPROG->m_sINVENTORY );

						GENMOBITEMARRAY_ITER iter = pSTEP->m_vecMOBGEN_QITEM.begin();
						GENMOBITEMARRAY_ITER iter_end = pSTEP->m_vecMOBGEN_QITEM.end();
						for ( ; iter!=iter_end; ++iter )
						{
							SGENQUESTITEM &sGENQITEM = (*iter);

							bool bFOUND = sGENQITEM.Find ( pcrow_client->m_pCrowData->m_sBasic.sNativeID.dwID );
							if ( !bFOUND )	continue;

							if ( !have_quest )
							{
								m_ptarget_information_display->AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );
								m_ptarget_information_display->AddTextNoSplit ( ID2GAMEINTEXT("TARGET_INFO_QUEST_STEP"), NS_UITEXTCOLOR::WHITE );
								have_quest = TRUE;
							}

							if ( !have_mob_gen )
							{
								text.Format( "Quest:%s", pQUEST->GetTITLE() );
								m_ptarget_information_display->AddTextNoSplit ( text.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN );

								text.Format( "Step:%s", pSTEP->GetTITLE() );
								m_ptarget_information_display->AddTextNoSplit ( text.GetString(), NS_UITEXTCOLOR::YELLOW );

								have_mob_gen = TRUE;
							}

							if ( sGENQITEM.wNUM > 0 )
							{
								DWORD dwNUM = sTEMP_INVEN.CountTurnItem ( sGENQITEM.sNID );
								text.Format( "%s %d/%d", pSTEP->GetOBJ_MOBGEN_QITEM(), dwNUM, sGENQITEM.wNUM );
								m_ptarget_information_display->AddTextNoSplit ( text.GetString(), NS_UITEXTCOLOR::YELLOW );
							}
							else
							{
								text.Format( "%s %d/%d", pSTEP->GetOBJ_MOBGEN_QITEM(), 0, 1 );
								m_ptarget_information_display->AddTextNoSplit ( text.GetString(), NS_UITEXTCOLOR::YELLOW );
							}
						}
					}
				}
			}
		}break;

	case CROW_NPC:
		{
			PGLCROWCLIENT pcrow_client = GLGaeaClient::GetInstance().GetActiveMap()->GetCrow ( m_target_id_information.dwID );
			if ( pcrow_client && pcrow_client->m_pCrowData )
			{
				/*hide crow name, Juver, 2017/09/01 */
				if ( pcrow_client->m_pCrowData->m_sBasic.m_bHideName )	return;

				//npc name
				CString text;
				text.Format( "%s", pcrow_client->GETNAME() );
				m_ptarget_information_display->AddTextNoSplit ( text.GetString(), NS_UITEXTCOLOR::WHITE );

				//npc function, quest
				INTERFACE_CSTRING_VEC	vec_npc_function;
				INTERFACE_CSTRING_VEC	vec_quest_start;
				INTERFACE_CSTRING_VEC	vec_quest_step;
				INTERFACE_CSTRING_VEC	vec_quest_complete;
				
				CNpcDialogueSet		m_DialogueSet;
				BOOL bLoad = m_DialogueSet.Load ( pcrow_client->m_pCrowData->GetTalkFile() );
				if ( bLoad )
				{
					target_information_load_talk_node( &m_DialogueSet, 
														pcharacter, 
														1, 
														vec_npc_function, 
														vec_quest_start, 
														vec_quest_step,
														vec_quest_complete );
				}

				if ( vec_npc_function.size() )
				{
					for( int i=0; i<(int)vec_npc_function.size(); ++i )
					{
						CString text_info = vec_npc_function[i];
						m_ptarget_information_display->AddTextNoSplit( text_info.GetString(), NS_UITEXTCOLOR::YELLOW );
					}
				}

				if ( vec_quest_start.size() )
				{
					m_ptarget_information_display->AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );
					m_ptarget_information_display->AddTextNoSplit( ID2GAMEINTEXT("CURSOR_TARGET_QUESTSTART"), NS_UITEXTCOLOR::WHITE );

					for( int i=0; i<(int)vec_quest_start.size(); ++i )
					{
						CString text_info = vec_quest_start[i];
						m_ptarget_information_display->AddTextNoSplit( text_info.GetString(), NS_UITEXTCOLOR::DODGERBLUE );
					}
				}

				if ( vec_quest_step.size() )
				{
					m_ptarget_information_display->AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );
					m_ptarget_information_display->AddTextNoSplit( ID2GAMEINTEXT("CURSOR_TARGET_QUESTSTEP"), NS_UITEXTCOLOR::WHITE );

					for( int i=0; i<(int)vec_quest_step.size(); ++i )
					{
						CString text_info = vec_quest_step[i];
						m_ptarget_information_display->AddTextNoSplit( text_info.GetString(), NS_UITEXTCOLOR::BRIGHTGREEN );
					}
				}

				if ( vec_quest_complete.size() )
				{
					m_ptarget_information_display->AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );
					m_ptarget_information_display->AddTextNoSplit( ID2GAMEINTEXT("CURSOR_TARGET_QUESTSTEP"), NS_UITEXTCOLOR::WHITE );

					for( int i=0; i<(int)vec_quest_complete.size(); ++i )
					{
						CString text_info = vec_quest_complete[i];
						m_ptarget_information_display->AddTextNoSplit( text_info.GetString(), NS_UITEXTCOLOR::AQUA );
					}
				}
			}
		}break;
	};

	//add distance display for gm character
	if ( pcharacter->m_dwUserLvl >= NSUSER_TYPE::USER_TYPE_GM3 )
	{
		m_ptarget_information_display->AddTextNoSplit ( " ", NS_UITEXTCOLOR::WHITE );

		const D3DXVECTOR3& vCharacterPos = pcharacter->GetPosition();
		const D3DXVECTOR3& vTargetPos = pcopy->GetPosition();
		const D3DXVECTOR3& vDistance = vCharacterPos - vTargetPos;
		float fDistance = D3DXVec3Length ( &vDistance );

		CString text;
		text.Format( "CharacterPos:%g/%g/%g", vCharacterPos.x, vCharacterPos.y, vCharacterPos.z );
		m_ptarget_information_display->AddTextNoSplit( text.GetString(), NS_UITEXTCOLOR::WHITE );
		text.Format( "TargetPos:%g/%g/%g", vTargetPos.x, vTargetPos.y, vTargetPos.z );
		m_ptarget_information_display->AddTextNoSplit( text.GetString(), NS_UITEXTCOLOR::WHITE );
		text.Format( "Distance:%g", fDistance );
		m_ptarget_information_display->AddTextNoSplit( text.GetString(), NS_UITEXTCOLOR::WHITE );
	}

	ShowGroupTop ( TARGET_INFORMATION_DISPLAY );
}

/*target information display, Juver, 2018/08/22 */
void CInnerInterface::target_information_reset()
{
	if ( m_ptarget_information_display )
		m_ptarget_information_display->ClearText();

	HideGroup ( TARGET_INFORMATION_DISPLAY );
}

/*target information display, Juver, 2018/08/22 */
void CInnerInterface::target_information_load_talk_node( CNpcDialogueSet* pdialogue_set, 
														GLCharacter* pcharacter, 
														DWORD talk_id, 
														INTERFACE_CSTRING_VEC& vec_npc_function, 
														INTERFACE_CSTRING_VEC& vec_quest_start, 
														INTERFACE_CSTRING_VEC& vec_quest_step, 
														INTERFACE_CSTRING_VEC& vec_quest_complete )
{
	if ( !pdialogue_set )	return;
	if ( !pcharacter )		return;

	GLQuestMan & glQuestMan = GLQuestMan::GetInstance();

	CNpcDialogue* pdialogue = pdialogue_set->GetDialogue ( talk_id );
	if ( !pdialogue )		return;

	//prevent infinite loading of series dialogue
	if ( pdialogue->get_info_done() )	return;
	pdialogue->set_info_done( TRUE );

	DWORD index = pdialogue->FindConditionSatisfied ( pcharacter );
	if ( index==UINT_MAX )	return;

	CNpcDialogueCase* pdialogue_case = pdialogue->GetDlgCase(index);	
	if ( !pdialogue_case )	return;

	CNpcTalkControl* pnpc_talk_control = pdialogue_case->GetTalkControl ();
	if ( !pnpc_talk_control )	return;

	CNpcTalkControl::NPCMAP* ptalk_map = pnpc_talk_control->GetTalkMap ();
	CNpcTalkControl::NPCMAP_IT iter = ptalk_map->begin();
	CNpcTalkControl::NPCMAP_IT iter_end = ptalk_map->end();

	CString text;
	for ( ; iter != iter_end; ++iter )
	{
		SNpcTalk* pnpc_talk = (*iter).second;

		if ( !pnpc_talk )	continue;
		if ( !pnpc_talk->DoTEST(pcharacter) )	continue;

		if ( pnpc_talk->m_nACTION == SNpcTalk::EM_BASIC )
		{
			if ( pnpc_talk->m_dwACTION_NO < SNpcTalk::EM_BASIC_SIZE )
			{
				CString npc_function = SNpcTalk::szBASICDESC[pnpc_talk->m_dwACTION_NO].c_str();

				BOOL exist = FALSE;

				for ( int i=0; i<(int)vec_npc_function.size(); ++i )
				{
					if ( vec_npc_function[i] == npc_function )
						exist = TRUE;
				}

				if ( !exist )
					vec_npc_function.push_back( npc_function );
			}
		}
		else if ( pnpc_talk->m_nACTION == SNpcTalk::EM_QUEST_START )
		{
			GLQUEST* pQUEST = glQuestMan.Find ( pnpc_talk->m_dwQuestStartID[0] );
			if ( !pQUEST )	continue;

			text.Format( "%s", pQUEST->GetTITLE() );
			vec_quest_start.push_back( text );
		}
		else if ( pnpc_talk->m_nACTION == SNpcTalk::EM_QUEST_STEP )
		{
			GLQUEST* pQUEST = glQuestMan.Find ( pnpc_talk->m_dwACTION_PARAM1 );
			if ( !pQUEST )	continue;

			GLQUESTPROG *pQUEST_PROG = pcharacter->m_cQuestPlay.FindProc(pnpc_talk->m_dwACTION_PARAM1);
			if ( !pQUEST_PROG )	continue;

			if ( pQUEST_PROG->m_dwSTEP != pnpc_talk->m_dwACTION_PARAM2 )		continue;

			GLQUEST_STEP* pQuestStep = pQUEST->GetSTEP ( pQUEST_PROG->m_dwSTEP );
			if ( !pQuestStep )	continue;

			text.Format( "%s-%s", pQUEST->GetTITLE(), pQuestStep->GetTITLE() );

			if ( pQUEST_PROG->CheckCOMPLETE() || (pQUEST_PROG->m_dwSTEP == (pQUEST->GetSTEPNUM() - 1)) )
				vec_quest_complete.push_back( text );
			else
				vec_quest_step.push_back( text );	
		}
		else if ( pnpc_talk->m_nACTION == SNpcTalk::EM_PAGE_MOVE )
		{
			target_information_load_talk_node ( pdialogue_set, 
												pcharacter, 
												pnpc_talk->m_dwACTION_NO, 
												vec_npc_function, 
												vec_quest_start, 
												vec_quest_step, 
												vec_quest_complete );
		}
	}
}
