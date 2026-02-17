#include "stdafx.h"

#include "./UserTypeDefine.h"

#include "./GLChar.h"
#include "./GLGaeaServer.h"
#include "./GLItemMan.h"
#include "../../Lib_Engine/Common/StringUtils.h"
#include "../../Lib_Engine/Utils/RanFilter.h"

#include "../DbActionLogic.h"

#include "../../Lib_Engine/Core/NSRParam.h"

#include "./GMCommandPass.h"
#include "./GLSetOption.h"

/* pvp club death match, Juver, 2020/11/26 */
#include "./PVPClubDeathMatchField.h"

#include "./GLAutoPotion.h"

#include "./GLReborn.h"

#include "./GLExchangeItem.h"

#include "./PVPPartyBattleGroundsField.h"
#include "./PVPPartyBattleGroundsMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* additional gm commands, Juver, 2018/03/01 */
void GLChar::GMInvenOpen( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_GM_COMMAND_INVEN_OPEN *pnet_msg_client = (GLMSG::SNETPC_GM_COMMAND_INVEN_OPEN *) nmg;
	GLMSG::SNETPC_GM_COMMAND_INVEN_OPEN_FB net_msg_fb;

	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		net_msg_fb.emFB = EMREQ_GM_INVEN_OPEN_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( strcmp( pnet_msg_client->szPass, CGMCommandPass::GetInstance().strPassGetInven.c_str() ) != 0 )
	{
		net_msg_fb.emFB = EMREQ_GM_INVEN_OPEN_FB_INVALID_PASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( m_wINVENLINE >= (EM_INVENSIZE_Y - EM_INVEN_DEF_SIZE_Y - EM_INVEN_PREMIUM_SIZE) )
	{
		net_msg_fb.emFB = EMREQ_GM_INVEN_OPEN_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	m_wINVENLINE = (EM_INVENSIZE_Y - EM_INVEN_DEF_SIZE_Y - EM_INVEN_PREMIUM_SIZE);
	m_cInventory.SetAddLine ( GetOnINVENLINE(), true );

	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		CSetChaInvenNum *pDbAction = new CSetChaInvenNum ( m_dwCharID, m_wINVENLINE );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pDbAction );
	}

	net_msg_fb.wInvenLine = m_wINVENLINE;
	net_msg_fb.emFB = EMREQ_GM_INVEN_OPEN_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);


	/*gm command logs, Juver, 2018/08/18 */
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		char command[1024];
		sprintf_s( command, 1024, "%s", "get_inven_open" );
		gm_cmd_logs *pdb_action = new gm_cmd_logs( m_dwClientID, m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
	}
}

/* additional gm commands, Juver, 2018/03/01 */
void GLChar::GMGetSkill( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_GM_COMMAND_GET_SKILL *pnet_msg_client = (GLMSG::SNETPC_GM_COMMAND_GET_SKILL *) nmg;
	GLMSG::SNETPC_GM_COMMAND_GET_SKILL_FB net_msg_fb;

	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		net_msg_fb.emFB = EMREQ_GM_SKILL_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( strcmp( pnet_msg_client->szPass, CGMCommandPass::GetInstance().strPassGetSkill.c_str() ) != 0 )
	{
		net_msg_fb.emFB = EMREQ_GM_SKILL_FB_INVALID_PASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( pnet_msg_client->bALL )
	{
		BOOL bUpdatePassive = FALSE;
		for ( WORD MID=0; MID<EMSKILLCLASS_NSIZE; ++MID )
		{
			for ( WORD SID=0; SID<GLSkillMan::MAX_CLASSSKILL; ++SID )
			{
				PGLSKILL pskill_data = GLSkillMan::GetInstance().GetData ( MID, SID );
				if ( pskill_data )
				{
					if ( !(pskill_data->m_sLEARN.dwCLASS&m_emClass) )			continue;
					if ( ISLEARNED_SKILL(pskill_data->m_sBASIC.sNATIVEID ) )	continue;
					
					m_ExpSkills.insert ( std::make_pair(pskill_data->m_sBASIC.sNATIVEID.dwID,SCHARSKILL(pskill_data->m_sBASIC.sNATIVEID,(WORD)pskill_data->m_sBASIC.dwMAXLEVEL-1,NATIVEID_NULL(),0)) );
					
					if ( pskill_data->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE ){
						INIT_DATA ( FALSE, FALSE );
					}

					QuestStartFromGetSKILL ( pskill_data->m_sBASIC.sNATIVEID );

					SCHARSKILL* pchar_skill = GETLEARNED_SKILL ( pskill_data->m_sBASIC.sNATIVEID );
					if ( pchar_skill )
					{
						GLMSG::SNETPC_GM_COMMAND_GET_SKILL2_FB	net_msg_skill_learn;
						net_msg_skill_learn.sidSkill = pchar_skill->sNativeID;
						net_msg_skill_learn.wLevel = pchar_skill->wLevel;
						GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_skill_learn );
					}
					
					if ( pskill_data->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
						bUpdatePassive = TRUE;
				}
			}
		}

		if ( bUpdatePassive )
		{
			GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
			NetMsgBrd.dwGaeaID = m_dwGaeaID;
			NetMsgBrd.sSKILL_DATA.Assign( m_sSUM_PASSIVE );
			SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
		}
	}
	else
	{
		GLSKILL* pskill_data = GLSkillMan::GetInstance().GetData ( pnet_msg_client->sidSkill );
		if ( !pskill_data )
		{
			net_msg_fb.emFB = EMREQ_GM_SKILL_FB_INVALID_SKILL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			return;
		}

		if ( !(pskill_data->m_sLEARN.dwCLASS&m_emClass) )
		{
			net_msg_fb.emFB = EMREQ_GM_SKILL_FB_CANNOT_LEARN;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			return;
		}

		if ( ISLEARNED_SKILL(pskill_data->m_sBASIC.sNATIVEID ) )
		{
			net_msg_fb.emFB = EMREQ_GM_SKILL_FB_CANNOT_LEARN;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			return;
		}

		m_ExpSkills.insert ( std::make_pair(pskill_data->m_sBASIC.sNATIVEID.dwID,SCHARSKILL(pskill_data->m_sBASIC.sNATIVEID,(WORD)pskill_data->m_sBASIC.dwMAXLEVEL-1,NATIVEID_NULL(),0)) );

		if ( pskill_data->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE ){
			INIT_DATA ( FALSE, FALSE );
		}

		QuestStartFromGetSKILL ( pskill_data->m_sBASIC.sNATIVEID );

		SCHARSKILL* pchar_skill = GETLEARNED_SKILL ( pskill_data->m_sBASIC.sNATIVEID );
		if ( pchar_skill )
		{
			GLMSG::SNETPC_GM_COMMAND_GET_SKILL2_FB	net_msg_skill_learn;
			net_msg_skill_learn.sidSkill = pchar_skill->sNativeID;
			net_msg_skill_learn.wLevel = pchar_skill->wLevel;
			GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &net_msg_skill_learn );
		}

		if ( pskill_data->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
		{
			GLMSG::SNETPC_UPDATE_PASSIVE_BRD NetMsgBrd;
			NetMsgBrd.dwGaeaID = m_dwGaeaID;
			NetMsgBrd.sSKILL_DATA.Assign( m_sSUM_PASSIVE );
			SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
		}
	}

	net_msg_fb.emFB = EMREQ_GM_SKILL_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);


	/*gm command logs, Juver, 2018/08/18 */
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		char command[1024];

		if ( pnet_msg_client->bALL )
			sprintf_s( command, 1024, "get_skill %s", "all" );
		else
			sprintf_s( command, 1024, "get_skill [%u/%u]", pnet_msg_client->sidSkill.wMainID, pnet_msg_client->sidSkill.wSubID );

		gm_cmd_logs *pdb_action = new gm_cmd_logs( m_dwClientID, m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
	}

}

/* additional gm commands, Juver, 2018/03/02 */
void GLChar::GMGetLevel( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_GM_COMMAND_GET_LEVEL *pnet_msg_client = (GLMSG::SNETPC_GM_COMMAND_GET_LEVEL *) nmg;
	GLMSG::SNETPC_GM_COMMAND_GET_LEVEL_FB net_msg_fb;

	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		net_msg_fb.emFB = EMREQ_GM_LEVEL_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( strcmp( pnet_msg_client->szPass, CGMCommandPass::GetInstance().strPassGetLevel.c_str() ) != 0 )
	{
		net_msg_fb.emFB = EMREQ_GM_LEVEL_FB_INVALID_PASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	WORD wLevel = pnet_msg_client->wLevel;
	if( m_emClass == GLCC_EXTREME_M || m_emClass == GLCC_EXTREME_W )
	{
		if ( m_wLevel >= GLCONST_CHAR::wMAX_EXTREME_LEVEL )
		{
			net_msg_fb.emFB = EMREQ_GM_LEVEL_FB_MAX_LEVEL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			return;
		}

		if ( wLevel >= GLCONST_CHAR::wMAX_EXTREME_LEVEL )
			wLevel = GLCONST_CHAR::wMAX_EXTREME_LEVEL;
	}
	else
	{
		if ( m_wLevel >= GLCONST_CHAR::wMAX_LEVEL )
		{
			net_msg_fb.emFB = EMREQ_GM_LEVEL_FB_MAX_LEVEL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
			return;
		}

		if ( wLevel >= GLCONST_CHAR::wMAX_LEVEL )
			wLevel = GLCONST_CHAR::wMAX_LEVEL;
	}

	WORD wlevel_orig = m_wLevel;

	for ( WORD i=wlevel_orig; i<wLevel; ++i )
	{
		m_sExperience.lnNow = GET_LEVELUP_EXP()+1;
		m_sExperience.lnMax = GET_LEVELUP_EXP();

		if ( m_sExperience.ISOVER() )
		{
			GLMSG::SNETPC_REQ_LEVELUP NetMsg;
			MsgReqLevelUp ( (NET_MSG_GENERIC*) &NetMsg );
		}
	}

	m_sExperience.lnNow = 0;
	m_sExperience.lnMax = GET_LEVELUP_EXP();
	
	net_msg_fb.wLevel = m_wLevel;
	net_msg_fb.emFB = EMREQ_GM_LEVEL_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);


	/*gm command logs, Juver, 2018/08/18 */
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		char command[1024];
		sprintf_s( command, 1024, "get_level %u", pnet_msg_client->wLevel );
		gm_cmd_logs *pdb_action = new gm_cmd_logs( m_dwClientID, m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
	}
}

/* additional gm commands, Juver, 2018/03/02 */
void GLChar::GMGetStats( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_GM_COMMAND_GET_STATS *pnet_msg_client = (GLMSG::SNETPC_GM_COMMAND_GET_STATS *) nmg;
	GLMSG::SNETPC_GM_COMMAND_GET_STATS_FB net_msg_fb;

	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		net_msg_fb.emFB = EMREQ_GM_STATS_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( strcmp( pnet_msg_client->szPass, CGMCommandPass::GetInstance().strPassGetStats.c_str() ) != 0 )
	{
		net_msg_fb.emFB = EMREQ_GM_STATS_FB_INVALID_PASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( pnet_msg_client->emStat == EMSIZE )
	{
		m_sStats.wPow += pnet_msg_client->wNum;
		m_sStats.wStr += pnet_msg_client->wNum;
		m_sStats.wSpi += pnet_msg_client->wNum;
		m_sStats.wDex += pnet_msg_client->wNum;
		m_sStats.wInt += pnet_msg_client->wNum;
		m_sStats.wSta += pnet_msg_client->wNum;
	}
	else
	{
		switch( pnet_msg_client->emStat )
		{
		case EMPOW:		m_sStats.wPow += pnet_msg_client->wNum;	break;
		case EMSTR:		m_sStats.wStr += pnet_msg_client->wNum;	break;
		case EMSPI:		m_sStats.wSpi += pnet_msg_client->wNum;	break;
		case EMDEX:		m_sStats.wDex += pnet_msg_client->wNum;	break;
		case EMSTA:		m_sStats.wSta += pnet_msg_client->wNum;	break;
		};
	}

	INIT_DATA( FALSE, FALSE, 1.0f, false );

	m_sHP.TO_FULL ();
	m_sMP.TO_FULL ();
	m_sSP.TO_FULL ();	

	net_msg_fb.emFB = EMREQ_GM_STATS_FB_OK;
	net_msg_fb.sStats = m_sStats;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);


	/*gm command logs, Juver, 2018/08/18 */
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		char command[1024];
		sprintf_s( command, 1024, "get_stats type:%u num:%u", pnet_msg_client->emStat, pnet_msg_client->wNum );
		gm_cmd_logs *pdb_action = new gm_cmd_logs( m_dwClientID, m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
	}
}

/* additional gm commands, Juver, 2018/03/02 */
void GLChar::GMGetStatsP( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_GM_COMMAND_GET_STATSP *pnet_msg_client = (GLMSG::SNETPC_GM_COMMAND_GET_STATSP *) nmg;
	GLMSG::SNETPC_GM_COMMAND_GET_STATSP_FB net_msg_fb;

	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		net_msg_fb.emFB = EMREQ_GM_STATSP_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( strcmp( pnet_msg_client->szPass, CGMCommandPass::GetInstance().strPassGetStatsP.c_str() ) != 0 )
	{
		net_msg_fb.emFB = EMREQ_GM_STATSP_FB_INVALID_PASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	m_wStatsPoint += pnet_msg_client->wNum;

	net_msg_fb.emFB = EMREQ_GM_STATSP_FB_OK;
	net_msg_fb.wStatsP = m_wStatsPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);

	/*gm command logs, Juver, 2018/08/18 */
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		char command[1024];
		sprintf_s( command, 1024, "get_statpoint num:%u", pnet_msg_client->wNum );
		gm_cmd_logs *pdb_action = new gm_cmd_logs( m_dwClientID, m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
	}
}

/* additional gm commands, Juver, 2018/03/02 */
void GLChar::GMGetSkillP( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_GM_COMMAND_GET_SKILLP *pnet_msg_client = (GLMSG::SNETPC_GM_COMMAND_GET_SKILLP *) nmg;
	GLMSG::SNETPC_GM_COMMAND_GET_SKILLP_FB net_msg_fb;

	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		net_msg_fb.emFB = EMREQ_GM_SKILLP_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( strcmp( pnet_msg_client->szPass, CGMCommandPass::GetInstance().strPassGetSkillP.c_str() ) != 0 )
	{
		net_msg_fb.emFB = EMREQ_GM_SKILLP_FB_INVALID_PASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	m_dwSkillPoint += pnet_msg_client->wNum;

	net_msg_fb.emFB = EMREQ_GM_SKILLP_FB_OK;
	net_msg_fb.dwSkillP = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);

	/*gm command logs, Juver, 2018/08/18 */
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		char command[1024];
		sprintf_s( command, 1024, "get_skillpoint num:%u", pnet_msg_client->wNum );
		gm_cmd_logs *pdb_action = new gm_cmd_logs( m_dwClientID, m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ",  m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
	}
}


///*hide costume, EJCode, 2018/11/18 */
//HRESULT GLChar::msg_hide_costume( NET_MSG_GENERIC* nmg )
//{
//	if ( !RPARAM::hide_costume_use )	return E_FAIL;
//
//	GLMSG::SNETPC_HIDE_COSTUME* net_msg_client = ( GLMSG::SNETPC_HIDE_COSTUME* ) nmg;
//
//	GLMSG::SNETPC_HIDE_COSTUME_FB net_msg_fb;
//
//	if ( m_hide_costume_timer < RPARAM::hide_costume_delay_time )
//	{
//		net_msg_fb.fb = EMFB_HIDE_COSTUME_DELAY_TIME;
//		GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
//		return E_FAIL;
//	}
//
//	m_hide_costume = net_msg_client->hide_costume;
//
//	GLMSG::SNETPC_HIDE_COSTUME_BRD net_msg_broadcast;
//	net_msg_broadcast.hide_costume = m_hide_costume;
//	net_msg_broadcast.dwGaeaID = m_dwGaeaID;
//	SendMsgViewAround ( reinterpret_cast<NET_MSG_GENERIC*>(&net_msg_broadcast) );
//
//	net_msg_fb.hide_costume = m_hide_costume;
//	net_msg_fb.fb = EMFB_HIDE_COSTUME_OK;
//	GLGaeaServer::GetInstance().SENDTOCLIENT( m_dwClientID, &net_msg_fb );
//
//	m_hide_costume_timer = 0.0f;
//
//	return S_OK;
//}

/*client tick count check, EJCode, 2018/11/26 */
void GLChar::client_tick_count_check_reply( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_CLIENT_TICK_COUNT_CHECK_RESULT* net_msg_client = ( GLMSG::SNETPC_CLIENT_TICK_COUNT_CHECK_RESULT* ) nmg;

	if ( !RPARAM::client_tick_count_check )	return;

	if ( m_pGLGaeaServer )
	{
		if ( m_pGLGaeaServer->IsEmulatorMode() )	return;

		DWORD cur_tick = net_msg_client->result;

		if ( m_client_tick_count_check_last_tick == 0 )
		{
			m_client_tick_count_check_last_tick = cur_tick;
		}
		else
		{
			DWORD diff = m_client_tick_count_check_last_tick > cur_tick ? m_client_tick_count_check_last_tick - cur_tick : cur_tick - m_client_tick_count_check_last_tick;

			if ( diff > RPARAM::client_tick_count_max_diff )
			{
				m_client_tick_count_check_error_count++;

				//CDebugSet::ToFileWithTime( "_tick_count.txt", "char:[%u] %s tick count too high %d possible speed hack!, cur:%u last:%u err:%u", 
					//m_dwCharID, m_szName, diff, cur_tick, m_client_tick_count_check_last_tick, m_client_tick_count_check_error_count );
			}
			else
			{
				m_client_tick_count_check_error_count = 0;
			}

			if ( m_client_tick_count_check_error_count >= RPARAM::client_tick_count_max_error )
			{
				//CDebugSet::ToFileWithTime( "_tick_count.txt", "char:[%u] %s max error count reached:%u max:%u", m_dwCharID, m_szName,
					//m_client_tick_count_check_error_count, RPARAM::client_tick_count_max_error );

				m_pGLGaeaServer->character_disconnect_request_char_id( m_dwCharID );
			}

			m_client_tick_count_check_last_tick = cur_tick;
		}
	}
}


void GLChar::GMGetCrowTime( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_GM_COMMAND_GET_CROW_TIME *pNetMsgClient = (GLMSG::SNETPC_GM_COMMAND_GET_CROW_TIME *) nmg;
	GLMSG::SNETPC_GM_COMMAND_GET_CROW_TIME_FB NetMsgFB;
	NetMsgFB.emFB = EMREQ_GM_GET_CROW_TIME_FB_FAIL;
	NetMsgFB.sMapID = pNetMsgClient->sMapID;
	NetMsgFB.sMobID = pNetMsgClient->sMobID;

	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		NetMsgFB.emFB = EMREQ_GM_GET_CROW_TIME_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( strcmp( pNetMsgClient->szPass, CGMCommandPass::GetInstance().strPassGetCrowTime.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMREQ_GM_GET_CROW_TIME_FB_INVALID_PASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
		return;
	}

	SCROWDATA* pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( pNetMsgClient->sMobID );
	if ( !pCrowData )
	{
		NetMsgFB.emFB = EMREQ_GM_GET_CROW_TIME_FB_INVALID_CROW;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SMAPNODE* pMapNode = GLGaeaServer::GetInstance().FindMapNode( pNetMsgClient->sMapID );
	if ( !pMapNode )
	{
		NetMsgFB.emFB = EMREQ_GM_GET_CROW_TIME_FB_INVALID_MAP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	GLLandMan* pLand = GLGaeaServer::GetInstance().GetByMapID ( pNetMsgClient->sMapID );
	if ( !pLand )
	{
		NetMsgFB.emFB = EMREQ_GM_GET_CROW_TIME_FB_INVALID_MAP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	GLMobScheduleMan* pScheduleMan = pLand->GetMobSchMan();
	if ( !pScheduleMan )
	{
		NetMsgFB.emFB = EMREQ_GM_GET_CROW_TIME_FB_CROW_FIND;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	GLMobSchedule* pSchedule = pScheduleMan->FindMobScheduleCrowID( pNetMsgClient->sMobID );
	if ( !pSchedule )
	{
		NetMsgFB.emFB = EMREQ_GM_GET_CROW_TIME_FB_CROW_FIND;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	NetMsgFB.emFB = EMREQ_GM_GET_CROW_TIME_FB_OK;

	NetMsgFB.bAlive				= pSchedule->m_bALive;
	NetMsgFB.bUseRegenTime		= pSchedule->GetUseRegenTime();

	for( int i=0; i<7; ++i )
		NetMsgFB.bDayOfWeek[i]	= pSchedule->m_bDayOfWeek[i];

	NetMsgFB.nRegenHour			= pSchedule->m_nRegenHour;
	NetMsgFB.nRegenMinute		= pSchedule->m_nRegenMin;
	NetMsgFB.fTimer				= pSchedule->m_fTimer;
	
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	/*gm command logs, Juver, 2018/08/18 */
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		char command[1024];
		sprintf_s( command, 1024, "get_crow_time mob:[%u/%u]%s map:[%u/%u]%s", 
			pCrowData->m_sBasic.sNativeID.wMainID, 
			pCrowData->m_sBasic.sNativeID.wSubID, 
			pCrowData->GetName(),
			pMapNode->sNativeID.wMainID,
			pMapNode->sNativeID.wSubID,
			pMapNode->strMapName.c_str() );

		gm_cmd_logs *pdb_action = new gm_cmd_logs( m_dwClientID, m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, command );
	}
}

/* personal lock system, Juver, 2020/01/27 */
void GLChar::MsgPersonalLockRequest( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_PERSONAL_LOCK_REQUEST *pNetMsgClient = (GLMSG::SNETPC_PERSONAL_LOCK_REQUEST *) nmg;

	GLMSG::SNETPC_PERSONAL_LOCK_REQUEST_FB NetMsgFB;
	NetMsgFB.emLock = pNetMsgClient->emLock;
	NetMsgFB.emFB = EMREQ_PERSONAL_LOCK_REQUEST_FAIL;

	if ( !RPARAM::bUsePersonalLock )
	{
		NetMsgFB.emFB = EMREQ_PERSONAL_LOCK_REQUEST_FUNCTION_DISABLED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( pNetMsgClient->emLock < 0 || pNetMsgClient->emLock >= EMPERSONAL_LOCK_SIZE )
	{
		NetMsgFB.emFB = EMREQ_PERSONAL_LOCK_REQUEST_INVALID_LOCK_TYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( m_fPersonalLockDelayTimer < RPARAM::fPersonalLockCoolDownTime )
	{
		NetMsgFB.emFB = EMREQ_PERSONAL_LOCK_REQUEST_REQUIRE_TIME;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( strlen( m_sPersonalLock[pNetMsgClient->emLock].szPin ) == 0 )
	{
		NetMsgFB.emFB = EMREQ_PERSONAL_LOCK_REQUEST_PIN_TO_CREATE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	NetMsgFB.emFB = EMREQ_PERSONAL_LOCK_REQUEST_PIN_TO_INPUT;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	m_fPersonalLockDelayTimer = 0.0f;
}

/* personal lock system, Juver, 2020/01/27 */
void GLChar::MsgPersonalLockCreatePin(NET_MSG_GENERIC* nmg)
{
    GLMSG::SNETPC_PERSONAL_LOCK_CREATE_PIN* pNetMsgClient = (GLMSG::SNETPC_PERSONAL_LOCK_CREATE_PIN*)nmg;

    GLMSG::SNETPC_PERSONAL_LOCK_CREATE_PIN_FB NetMsgFB;
    NetMsgFB.emLock = pNetMsgClient->emLock;
    NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_FAILED;

    if (!RPARAM::bUsePersonalLock)
    {
        NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_FUNCTION_DISABLED;
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
        return;
    }

    if (pNetMsgClient->emLock < 0 || pNetMsgClient->emLock >= EMPERSONAL_LOCK_SIZE)
    {
        NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_INVALID_LOCK_TYPE;
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
        return;
    }

    if (strlen(m_sPersonalLock[pNetMsgClient->emLock].szPin) != 0)
    {
        NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_ALREADY_ENABLED;
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
        return;
    }

    if (m_fPersonalLockDelayTimer < static_cast<WORD>(RPARAM::fPersonalLockCoolDownTime))
    {
        NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_TIME_OUT;
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
        return;
    }

    // Check for the required item
    SINVENITEM* pInvenItem = m_cInventory.FindItem(ITEM_PERSONAL_LOCK_ENABLE);
    if (!pInvenItem)
    {
        NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_FAILED;  // Fixed enum name
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
        return;
    }

    // Consume the item
    bool bDelete = DoDrugInvenItem(pInvenItem->wPosX, pInvenItem->wPosY);
    if (!bDelete)
    {
        NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_FAILED;
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
        return;
    }

    std::string strPin1(pNetMsgClient->szPin1);
    std::string strPin2(pNetMsgClient->szPin2);

    if (strPin1.empty() || strPin2.empty())
    {
        NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_EMPTY;
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
        return;
    }

    if (strPin1 != strPin2)
    {
        NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_MISMATCH;
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
        return;
    }

    if (strPin1.find_first_not_of("0123456789") != std::string::npos)
    {
        NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_KEY_NUMBER_ONLY;
        GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);
        return;
    }

    StringCchCopy(m_sPersonalLock[pNetMsgClient->emLock].szPin, PERSONAL_LOCK_PIN_SIZE + 1, strPin1.c_str());

    NetMsgFB.emFB = EMFB_PERSONAL_LOCK_CREATE_PIN_PUTON_DONE;
    GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgFB);

    m_fPersonalLockDelayTimer = 0.0f;
}



/* personal lock system, Juver, 2020/01/27 */
void GLChar::MsgPersonalLockInputPin( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_PERSONAL_LOCK_INPUT_PIN *pNetMsgClient = (GLMSG::SNETPC_PERSONAL_LOCK_INPUT_PIN *) nmg;
	
	GLMSG::SNETPC_PERSONAL_LOCK_INPUT_PIN_FB NetMsgFB;
	NetMsgFB.emLock = pNetMsgClient->emLock;
	NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_FAILED;

	if ( !RPARAM::bUsePersonalLock )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_FUNCTION_DISABLED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( pNetMsgClient->emLock < 0 || pNetMsgClient->emLock >= EMPERSONAL_LOCK_SIZE )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_INVALID_LOCK_TYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( strlen( m_sPersonalLock[pNetMsgClient->emLock].szPin ) == 0 )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}
	
	if ( m_fPersonalLockDelayTimer < RPARAM::fPersonalLockCoolDownTime )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_TIME_OUT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}
	
	std::string strPin(pNetMsgClient->szPin);
	
	if ( strPin.size() <= 0 )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_EMPTY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}
	
	if ( strPin.size() < PERSONAL_LOCK_PIN_SIZE || strPin.size() > PERSONAL_LOCK_PIN_SIZE )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_SIZE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}
	
	if ( strPin.find_first_not_of( "0123456789" ) != std::string::npos )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_NUMBER_ONLY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}
	
	CString strTEMP( strPin.c_str() );
		
	if ( STRUTIL::CheckString( strTEMP ) || CRanFilter::GetInstance().NameFilter( strTEMP ) )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_NUMBER_ONLY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}
	
	if ((strTEMP.FindOneOf(" ") != -1) ) 
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_NUMBER_ONLY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}
	
	if ( strcmp( m_sPersonalLock[pNetMsgClient->emLock].szPin, strPin.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_WRONG;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

		//increment wrong input
		m_sPersonalLock[pNetMsgClient->emLock].wErrorCount ++;

		//max attempt exceeded
		if ( m_sPersonalLock[pNetMsgClient->emLock].wErrorCount >= RPARAM::wPersonalLockMaxAttempt )
		{
			NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_KEY_WRONG_MAXED;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

			GLGaeaServer::GetInstance().character_disconnect_request_char_id( m_dwCharID, PERSONAL_LOCK_RETRY_MAXED_DISCONNECT_TIME );
		}

		return;
	}
	
	m_sPersonalLock[pNetMsgClient->emLock].bLock = !m_sPersonalLock[pNetMsgClient->emLock].bLock;
	
	//input success, reset any wrong input
	m_sPersonalLock[pNetMsgClient->emLock].wErrorCount = 0;

	NetMsgFB.bLock = m_sPersonalLock[pNetMsgClient->emLock].bLock;
	NetMsgFB.emFB = EMFB_PERSONAL_LOCK_INPUT_PIN_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
	
	m_fPersonalLockDelayTimer = 0.0f;
}

/* personal lock system, Juver, 2020/01/27 */
void GLChar::MsgPersonalLockResetPin( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_PERSONAL_LOCK_PIN_RESET *pNetMsgClient = (GLMSG::SNETPC_PERSONAL_LOCK_PIN_RESET *) nmg;

	GLMSG::SNETPC_PERSONAL_LOCK_PIN_RESET_FB NetMsgFB;
	NetMsgFB.emLock = pNetMsgClient->emLock;
	NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RESET_FAILED;

	if ( !RPARAM::bUsePersonalLock )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RESET_FUNCTION_DISABLED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( pNetMsgClient->emLock < 0 || pNetMsgClient->emLock > EMPERSONAL_LOCK_SIZE )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RESET_INVALID_LOCK_TYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	for ( int i=0; i<EMPERSONAL_LOCK_SIZE; ++i )
	{
		NetMsgFB.bLock[i] = m_sPersonalLock[i].bLock;
	}
	
	if ( m_fPersonalLockDelayTimer < RPARAM::fPersonalLockCoolDownTime )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RESET_TIME_OUT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_PERSONAL_LOCK_RESET );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RESET_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	bool bDelete = DoDrugInvenItem ( pInvenItem->wPosX, pInvenItem->wPosY );
	if ( !bDelete )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RESET_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( pNetMsgClient->emLock >= EMPERSONAL_LOCK_SIZE )
	{
		for ( int i=0; i<EMPERSONAL_LOCK_SIZE; ++i )
		{
			StringCchCopy ( m_sPersonalLock[i].szPin, PERSONAL_LOCK_PIN_SIZE+1, "" );
			m_sPersonalLock[i].bLock = false;
		}
	}
	else
	{
		StringCchCopy ( m_sPersonalLock[pNetMsgClient->emLock].szPin, PERSONAL_LOCK_PIN_SIZE+1, "" );
		m_sPersonalLock[pNetMsgClient->emLock].bLock = false;
	}
	

	for ( int i=0; i<EMPERSONAL_LOCK_SIZE; ++i )
	{
		NetMsgFB.bLock[i] = m_sPersonalLock[i].bLock;
	}

	NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RESET_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	m_fPersonalLockDelayTimer = 0.0f;
}

/* personal lock system, Juver, 2020/01/27 */
void GLChar::MsgPersonalLockChangePin( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_PERSONAL_LOCK_PIN_CHANGE *pNetMsgClient = (GLMSG::SNETPC_PERSONAL_LOCK_PIN_CHANGE *) nmg;

	GLMSG::SNETPC_PERSONAL_LOCK_PIN_CHANGE_FB NetMsgFB;
	NetMsgFB.emLock = pNetMsgClient->emLock;
	NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_FAILED;

	if ( !RPARAM::bUsePersonalLock )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_FUNCTION_DISABLED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( pNetMsgClient->emLock < 0 || pNetMsgClient->emLock >= EMPERSONAL_LOCK_SIZE )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_INVALID_LOCK_TYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_PERSONAL_LOCK_CHANGE_PIN );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( m_fPersonalLockDelayTimer < RPARAM::fPersonalLockCoolDownTime )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_TIME_OUT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	std::string strPinOld(pNetMsgClient->szPinOld);
	std::string strPinNew(pNetMsgClient->szPinNew);
	std::string strPinNew2(pNetMsgClient->szPinNew2);

	if ( strPinOld.empty() || strPinNew.empty() || strPinNew2.empty() )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_EMPTY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( strPinOld.size() < PERSONAL_LOCK_PIN_SIZE || strPinOld.size() > PERSONAL_LOCK_PIN_SIZE ||
		strPinNew.size() < PERSONAL_LOCK_PIN_SIZE || strPinNew.size() > PERSONAL_LOCK_PIN_SIZE ||
		strPinNew2.size() < PERSONAL_LOCK_PIN_SIZE || strPinNew2.size() > PERSONAL_LOCK_PIN_SIZE )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_SIZE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( strPinOld.find_first_not_of( "0123456789" ) != std::string::npos ||
		strPinNew.find_first_not_of( "0123456789" ) != std::string::npos ||
		strPinNew2.find_first_not_of( "0123456789" ) != std::string::npos )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_NUMBER_ONLY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	CString strPinOldTEMP( strPinOld.c_str() );
	CString strPinNewTEMP( strPinNew.c_str() );
	CString strPinNew2TEMP( strPinNew2.c_str() );

	if ( STRUTIL::CheckString( strPinOldTEMP ) || CRanFilter::GetInstance().NameFilter( strPinOldTEMP ) ||
		STRUTIL::CheckString( strPinNewTEMP ) || CRanFilter::GetInstance().NameFilter( strPinNewTEMP )  ||
		STRUTIL::CheckString( strPinNew2TEMP ) || CRanFilter::GetInstance().NameFilter( strPinNew2TEMP ) )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_NUMBER_ONLY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( (strPinOldTEMP.FindOneOf(" ") != -1) || (strPinNewTEMP.FindOneOf(" ") != -1) || (strPinNew2TEMP.FindOneOf(" ") != -1) ) 
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_NUMBER_ONLY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( strcmp( strPinNew.c_str(), strPinNew2.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_MISMATCH;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( strcmp( m_sPersonalLock[pNetMsgClient->emLock].szPin, strPinOld.c_str() ) != 0 )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_INCORRECT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	bool bDelete = DoDrugInvenItem ( pInvenItem->wPosX, pInvenItem->wPosY );
	if ( !bDelete )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	StringCchCopy ( m_sPersonalLock[pNetMsgClient->emLock].szPin, PERSONAL_LOCK_PIN_SIZE+1, strPinNew.c_str() );

	NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_CHANGE_PIN_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	m_fPersonalLockDelayTimer = 0.0f;
}

/* personal lock system, Juver, 2020/01/31 */
void GLChar::MsgPersonalLockRecoverPin( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_PERSONAL_LOCK_PIN_RECOVER *pNetMsgClient = (GLMSG::SNETPC_PERSONAL_LOCK_PIN_RECOVER *) nmg;

	GLMSG::SNETPC_PERSONAL_LOCK_PIN_RECOVER_FB NetMsgFB;
	NetMsgFB.emLock = pNetMsgClient->emLock;
	NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RECOVER_FAILED;

	if ( !RPARAM::bUsePersonalLock )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RECOVER_FUNCTION_DISABLED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( pNetMsgClient->emLock < 0 || pNetMsgClient->emLock > EMPERSONAL_LOCK_SIZE )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RECOVER_INVALID_LOCK_TYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( m_fPersonalLockDelayTimer < RPARAM::fPersonalLockCoolDownTime )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RECOVER_TIME_OUT;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	SINVENITEM* pInvenItem = m_cInventory.FindItem ( ITEM_PERSONAL_LOCK_RECOVER_PIN );
	if ( !pInvenItem )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RECOVER_NOT_CARD;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	bool bDelete = DoDrugInvenItem ( pInvenItem->wPosX, pInvenItem->wPosY );
	if ( !bDelete )
	{
		NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RECOVER_FAILED;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return;
	}

	if ( pNetMsgClient->emLock >= EMPERSONAL_LOCK_SIZE )
	{
		for ( int i=0; i<EMPERSONAL_LOCK_SIZE; ++i )
		{
			StringCchCopy ( NetMsgFB.sPin[i].szPin, PERSONAL_LOCK_PIN_SIZE+1, m_sPersonalLock[i].szPin );
		}
	}
	else
	{
		StringCchCopy ( NetMsgFB.sPin[pNetMsgClient->emLock].szPin, PERSONAL_LOCK_PIN_SIZE+1, m_sPersonalLock[pNetMsgClient->emLock].szPin );
	}


	NetMsgFB.emFB = EMFB_PERSONAL_LOCK_PIN_RECOVER_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

	m_fPersonalLockDelayTimer = 0.0f;
}

/* gm command inven clear, Juver, 2020/05/09 */
void GLChar::GMInvenClear( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_GM_INVEN_CLEAR *pnet_msg_client = (GLMSG::SNETPC_GM_INVEN_CLEAR *) nmg;
	GLMSG::SNETPC_GM_INVEN_CLEAR_FB net_msg_fb;

	if ( m_dwUserLvl < NSUSER_TYPE::USER_TYPE_GM3 )
	{
		net_msg_fb.emFB = EMREQ_GM_INVEN_CLEAR_FB_NOT_GM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	if ( strcmp( pnet_msg_client->szPass, CGMCommandPass::GetInstance().strPassInvenClear.c_str() ) != 0 )
	{
		net_msg_fb.emFB = EMREQ_GM_INVEN_CLEAR_FB_INVALID_PASS;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);
		return;
	}

	m_cInventory.DeleteItemAll();

	net_msg_fb.emFB = EMREQ_GM_INVEN_CLEAR_FB_SUCCESS;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&net_msg_fb);

	/*gm command logs, Juver, 2018/08/18 */
	if ( GLGaeaServer::GetInstance().GetDBMan() )
	{
		gm_cmd_logs *pdb_action = new gm_cmd_logs( m_dwClientID, m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, "gm clear inven" );
		GLGaeaServer::GetInstance().GetDBMan()->AddJob ( pdb_action );

		CDebugSet::ToFileWithTime( "gm_cmd_logs.txt", "[%u-%u][%u]%s %s ", m_dwUserID, m_dwUserLvl, m_dwCharID, m_szName, "gm clear inven" );
	}
}

/* pvp club death match, Juver, 2020/11/26 */
HRESULT GLChar::MsgReqPVPClubDeathMatchReBirth ( NET_MSG_GENERIC* nmg )
{
	if ( !IsSTATE(EM_ACT_DIE) )
	{
		DEBUGMSG_WRITE ( "GLChar::MsgReqPVPClubDeathMatchReBirth IsSTATE(EM_ACT_DIE) false" );
		return S_OK;
	}

	ReSetSTATE(EM_ACT_DIE);

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ *pNetMsg = (GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_C2AF_REVIVE_REQ *) nmg;
	
	SNATIVEID sLobbyMap = PVPClubDeathMatchField::GetInstance().m_sLobbyMap;
	DWORD dwLobbyGate = PVPClubDeathMatchField::GetInstance().m_dwLobbyGate;

	GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( sLobbyMap );
	if ( !pLandMan )
	{
		CDebugSet::ToFileWithTime( "_pvpcapturetheflag.txt", "GLChar::MsgReqPVPClubDeathMatchReBirth invalid lobby map[%d/%d]", sLobbyMap.wMainID );
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLChar::MsgReqPVPClubDeathMatchReBirth invalid lobby map[%d/%d]", sLobbyMap.wSubID );
		return FALSE;
	}

	PDXLANDGATE pLandGate = pLandMan->GetLandGateMan().FindLandGate(dwLobbyGate);
	if ( !pLandGate )
	{
		CDebugSet::ToFileWithTime( "_pvpcapturetheflag.txt", "GLChar::MsgReqPVPClubDeathMatchReBirth invalid lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wMainID );
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLChar::MsgReqPVPClubDeathMatchReBirth invalid lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wSubID );
		return FALSE;
	}

	BOOL bOk = GLGaeaServer::GetInstance().RequestReBirth ( m_dwGaeaID, pLandMan->GetMapID(), dwLobbyGate, D3DXVECTOR3(0,0,0) );
	if ( !bOk )
	{
		CDebugSet::ToFileWithTime( "_pvpcapturetheflag.txt", "GLChar::MsgReqPVPClubDeathMatchReBirth !bOk lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wMainID );
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLChar::MsgReqPVPClubDeathMatchReBirth !bOk lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wSubID );
	}

	m_fGenAge = 0.0f;
	GLCHARLOGIC_SERVER::INIT_RECOVER();
	TurnAction ( GLAT_IDLE );

	GLGaeaServer::GetInstance().ReserveDropOutPet ( SDROPOUTPETINFO(m_dwPetGUID,true,true) );
	//GLGaeaServer::GetInstance().ReserveDropOutSummon ( SDROPOUTSUMMONINFO(m_dwSummonGUID,true) );
	GLGaeaServer::GetInstance().SaveVehicle( m_dwClientID, m_dwGaeaID, true );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		GLGaeaServer::GetInstance().ReserveDropOutSummon ( m_dwSummonGUID_FLD[i] );

	GLMSG::SNETPC_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB NetMsg;
	NetMsg.sMapID = m_pLandMan->GetMapID();
	NetMsg.vPos = GetPosition();
	NetMsg.wNowHP = m_sHP.wNow;
	NetMsg.wNowMP = m_sMP.wNow;
	NetMsg.wNowSP = m_sSP.wNow;

	GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID,&NetMsg);

	return S_OK;
}

HRESULT GLChar::MsgReqAutoPotion ( NET_MSG_GENERIC* nmg )
{
	if ( !IsValidBody() )	return E_FAIL;

	GLMSG::SNETPC_REQ_AUTO_POTION *pNetMsg = (GLMSG::SNETPC_REQ_AUTO_POTION *) nmg;
	GLMSG::SNETPC_REQ_AUTO_POTION_FB NetMsgFb;

	if ( RPARAM::bUseAutoPotion == FALSE )
	{
		NetMsgFb.emFB = EMREQ_AUTO_POTION_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFb);
		return E_FAIL;
	}

	if ( pNetMsg->fHP <= 0.0f || pNetMsg->fMP <= 0.0f || pNetMsg->fSP <= 0.0f )
	{
		NetMsgFb.emFB = EMREQ_AUTO_POTION_FB_INVALID_VALUE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFb);
		return E_FAIL;
	}

	if ( m_pAutoPotion == FALSE )
	{
		NetMsgFb.emFB = EMREQ_AUTO_POTION_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFb);
		return E_FAIL;
	}

	SMAPNODE* pMapNode = GLGaeaServer::GetInstance().FindMapNode( m_sMapID );
	if ( !pMapNode )
	{
		NetMsgFb.emFB = EMREQ_AUTO_POTION_FB_NOT_MAP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFb);
		return E_FAIL;
	}

	m_pAutoPotion->m_fHPThreshold = pNetMsg->fHP;
	m_pAutoPotion->m_fMPThreshold = pNetMsg->fMP;
	m_pAutoPotion->m_fSPThreshold = pNetMsg->fSP;

	NetMsgFb.fHP = m_pAutoPotion->m_fHPThreshold;
	NetMsgFb.fMP = m_pAutoPotion->m_fMPThreshold;
	NetMsgFb.fSP = m_pAutoPotion->m_fSPThreshold;

	NetMsgFb.emFB = EMREQ_AUTO_POTION_FB_DONE;

	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFb);

	return S_OK;
}

HRESULT GLChar::MsgReqRebornA ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_CARD_REBORN_A *pNetMsg = (GLMSG::SNETPC_CARD_REBORN_A *)nmg;

	GLMSG::SNETPC_CARD_REBORN_A_FB MsgFB;

	//cant find the item!
	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFB = EMINVEN_CARD_REBORN_FB_NOITEMINVEN;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//item is in cooldown
	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		MsgFB.emFB = EMINVEN_CARD_REBORN_FB_FAIL;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//item data wrong
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pITEM )
	{
		MsgFB.emFB = EMINVEN_CARD_REBORN_FB_NOITEMDATA;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//wrong item type
	if ( pITEM->sBasicOp.emItemType!=ITEM_REBORD_CARD_A )
	{
		MsgFB.emFB = EMINVEN_CARD_REBORN_FB_NOITEMTYPE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}


	//do the magic here

	//get reborn data from array matching the current reborn
	SREBORN* pReborn = GLReborn::GetInstance().GetRebornData( m_dwReborn );

	//get also the reborn data for max reborn
	SREBORN* pRebornMax = GLReborn::GetInstance().GetRebornDataMax();

	//this is for reborn below maximum setting
	if ( pReborn && m_dwReborn < GLReborn::GetInstance().GetMaxReborn() )
	{
		//check money
		if ( m_lnMoney < pReborn->dwPrice )
		{
			MsgFB.emFB = EMINVEN_CARD_REBORN_FB_NOMONEY;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}

		//check level
		if ( m_wLevel < pReborn->wLevel )
		{
			MsgFB.emFB = EMINVEN_CARD_REBORN_FB_NOLEVEL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}


		//done checking
		//hope its all correct lol
		//do changes to char data
		m_wLevel = pReborn->wLevelAfter; //replace level
		m_dwReborn += pReborn->wRebornIncrease; //increase reborn
		m_sStats = pReborn->sStatsReplace; //replace stats
		m_wStatsPoint = pReborn->wRemainStats; ; //replace remain
		m_lnMoney -= pReborn->dwPrice; //deduct the gold
	}

	//for reborns more than max reborn
	else if ( pRebornMax && m_dwReborn >= GLReborn::GetInstance().GetMaxReborn() )
	{
		//check money
		if ( m_lnMoney < pRebornMax->dwPrice )
		{
			MsgFB.emFB = EMINVEN_CARD_REBORN_FB_NOMONEY;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}

		//check level
		if ( m_wLevel < pRebornMax->wLevel )
		{
			MsgFB.emFB = EMINVEN_CARD_REBORN_FB_NOLEVEL;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
			return E_FAIL;
		}

		//do changes to char data
		m_wLevel = pRebornMax->wLevelAfter; //change level
		m_dwReborn += pRebornMax->wRebornIncrease; //increase reborn
		m_sStats = pRebornMax->sStatsReplace; //replace stats
		m_wStatsPoint = pRebornMax->wRemainStats; ; //replace remain
		m_lnMoney -= pRebornMax->dwPrice; //deduct the gold
	}

	//do quest for reborn
//	for( WORD i=0;i<m_wReborn;++i)	
//		QuestStartFromGetREBORN( i );

//	DoQuestReborn();

	//close the reborn window
	MsgFB.emFB = EMINVEN_CARD_REBORN_FB_CLOSE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	//delete the card
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	//disconnect the player
	MsgFB.emFB = EMINVEN_CARD_REBORN_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	//since were going to disconnect player
	//no need to update data in client

	return S_OK;
}

HRESULT GLChar::MsgReqRebornB ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_CARD_REBORN_B *pNetMsg = (GLMSG::SNETPC_CARD_REBORN_B *)nmg;

	GLMSG::SNETPC_CARD_REBORN_B_FB MsgFB;
	MsgFB.wReborn = m_dwReborn;

	//cant find the item!
	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.bOK = FALSE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//item is in cooldown
	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		MsgFB.bOK = FALSE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//item data wrong
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pITEM )
	{
		MsgFB.bOK = FALSE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//wrong item type
	if ( pITEM->sBasicOp.emItemType!=ITEM_REBORD_CARD_B )
	{
		MsgFB.bOK = FALSE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	WORD wRebornAdd = pITEM->sSuitOp.wReModelNum;
	if ( wRebornAdd == 0 )
	{
		MsgFB.bOK = FALSE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	m_dwReborn += wRebornAdd; //increase reborn

	//do quest for reborn
//	for( WORD i=0;i<m_wReborn;++i)	
//		QuestStartFromGetREBORN( i );

//	DoQuestReborn();

	//delete the card
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	MsgFB.wReborn = m_dwReborn;
	MsgFB.bOK = TRUE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	return S_OK;
}

HRESULT GLChar::MsgReqExhangeItem( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNET_REQ_EXCHANGE_ITEM *pNetMsg = (GLMSG::SNET_REQ_EXCHANGE_ITEM *)nmg;
	GLMSG::SNET_REQ_EXCHANGE_ITEM_FB NetMsgFB;

	SINVENITEM* pInvenToCheck = m_cInventory.GetItem ( pNetMsg->wCardPosX, pNetMsg->wCardPosY );
	if ( !pInvenToCheck )	
	{
		NetMsgFB.emFB = EMEXCHANGE_ITEM_CARD_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pItemCard = GLItemMan::GetInstance().GetItem( pInvenToCheck->sItemCustom.sNativeID );
	if ( pItemCard == NULL )
	{
		NetMsgFB.emFB = EMEXCHANGE_ITEM_CARD_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	nsexchange_item::Data* pData = nsexchange_item::DataManager::GetInstance().findData( pItemCard->getExchangeItem() );
	if ( pData == NULL )
	{
		NetMsgFB.emFB = EMEXCHANGE_ITEM_CARD_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SNATIVEID sItemResult = pNetMsg->sID;
	nsexchange_item::Result* pResult = pData->findResult( sItemResult.dwID );
	if ( pResult == NULL )
	{
		NetMsgFB.emFB = EMEXCHANGE_ITEM_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	SITEM* pItemResult = GLItemMan::GetInstance().GetItem( pResult->sItemID );
	if ( pItemResult == NULL )	
	{
		NetMsgFB.emFB = EMEXCHANGE_ITEM_INVALID_ITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	//get cost
	WORD wPointCost = pResult->wPointCost;
	
	//no change item points cant continue!
	if ( m_wExchangeItemPoints < wPointCost )
	{
		NetMsgFB.emFB = EMEXCHANGE_ITEM_ERROR_NOCIP;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
		return E_FAIL;
	}

	if( pNetMsg->wCardPosX < EM_INVENSIZE_X && pNetMsg->wCardPosY < EM_INVENSIZE_Y )
	{
		SINVENITEM* pINVENITEM_DEL = m_cInventory.GetItem ( pNetMsg->wCardPosX, pNetMsg->wCardPosY );
		if ( pINVENITEM_DEL == NULL )
		{
			//cant find change item card
			//card was already check above, this is not necessary
			NetMsgFB.emFB = EMEXCHANGE_ITEM_CARD_ERROR;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}

		SITEM* pITEM_DEL = GLItemMan::GetInstance().GetItem( pINVENITEM_DEL->sItemCustom.sNativeID );
		if( (pITEM_DEL == NULL) || (pITEM_DEL->sBasicOp.emItemType != ITEM_EXCHANGE_ITEM)  )		
		{
			//change item card wrong type
			//or change item card invalid
			NetMsgFB.emFB = EMEXCHANGE_ITEM_ITEMTYPE_ERROR;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}

		SINVENITEM* pINVENITEM_CHANGE = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
		if ( pINVENITEM_CHANGE == NULL )
		{
			//cant find item to change
			NetMsgFB.emFB = EMEXCHANGE_ITEM_INVALID_ITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}

		nsexchange_item::Require* pRequire = pData->findRequire( pINVENITEM_CHANGE->sItemCustom.sNativeID.dwID );
		if ( pRequire == NULL )
		{
			//selected item not found in itemlist!
			NetMsgFB.emFB = EMEXCHANGE_ITEM_INVALID_ITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}

		SITEM* pITEM_CHANGE = GLItemMan::GetInstance().GetItem( pINVENITEM_CHANGE->sItemCustom.sNativeID );
		if( pITEM_CHANGE == NULL )		
		{
			//invalid item to change
			NetMsgFB.emFB = EMEXCHANGE_ITEM_INVALID_ITEM;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);
			return E_FAIL;
		}

		if ( CheckCoolTime( pINVENITEM_DEL->sItemCustom.sNativeID ) )	
		{
			//change item card in cooldown
			return E_FAIL;
		}

		//delete the card
		DoDrugInvenItem ( pINVENITEM_DEL->wPosX, pINVENITEM_DEL->wPosY );

		GLITEMLMT::GetInstance().ReqItemConversion ( pINVENITEM_CHANGE->sItemCustom, ID_CHAR, m_dwCharID );

		//change the mid and sid of item
		pINVENITEM_CHANGE->sItemCustom.sNativeID = pNetMsg->sID;

		//updated item in client side
		GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
		NetItemUpdate.wPosX = pINVENITEM_CHANGE->wPosX;
		NetItemUpdate.wPosY = pINVENITEM_CHANGE->wPosY;
		NetItemUpdate.sItemCustom = pINVENITEM_CHANGE->sItemCustom;
		NetItemUpdate.bChangeID = true;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetItemUpdate);

		//close the change item window
		NetMsgFB.emFB = EMEXCHANGE_ITEM_DONE;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&NetMsgFB);

		//everything done now lets deduct the points
		m_wExchangeItemPoints -= wPointCost;

		GLMSG::SNETPC_UPDATE_CHANGE_ITEM_POINTS netMsgChangeItemPoints;
		netMsgChangeItemPoints.wChangeItemPoints = m_wExchangeItemPoints;
		GLGaeaServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &netMsgChangeItemPoints );
	}	

	return S_OK;
}

HRESULT GLChar::MsgReqExhangeItemCard( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_EXCHANGE_ITEM_CARD *pNetMsg = (GLMSG::SNETPC_REQ_EXCHANGE_ITEM_CARD *)nmg;

	GLMSG::SNETPC_REQ_EXCHANGE_ITEM_CARD_FB MsgFB;
	MsgFB.wChangeItemPoint = m_wExchangeItemPoints;

	//cant find the item!
	SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
	if ( !pINVENITEM )
	{
		MsgFB.emFb = EMEXCHANGE_ITEM_POINT_CARD_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//item is in cooldown
	if ( CheckCoolTime( pINVENITEM->sItemCustom.sNativeID ) )
	{
		MsgFB.emFb = EMEXCHANGE_ITEM_POINT_CARD_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//item data wrong
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pINVENITEM->sItemCustom.sNativeID );
	if ( !pITEM )
	{
		MsgFB.emFb = EMEXCHANGE_ITEM_POINT_CARD_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	//wrong item type
	if ( pITEM->sBasicOp.emItemType!=ITEM_EXCHANGE_ITEM_POINT )
	{
		MsgFB.emFb = EMEXCHANGE_ITEM_POINT_CARD_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	WORD wIncrease = pITEM->sSuitOp.wReModelNum;
	if ( wIncrease == 0 )
	{
		MsgFB.emFb = EMEXCHANGE_ITEM_POINT_CARD_ERROR;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);
		return E_FAIL;
	}

	m_wExchangeItemPoints += wIncrease;

	//delete the card
	DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	MsgFB.wChangeItemPoint = m_wExchangeItemPoints;
	MsgFB.emFb = EMEXCHANGE_ITEM_POINT_CARD_DONE;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID,&MsgFB);

	return S_OK;
}

HRESULT GLChar::MsgReqPVPPBGReBirth ( NET_MSG_GENERIC* nmg )
{
	if ( !IsSTATE(EM_ACT_DIE) )
	{
		DEBUGMSG_WRITE ( "GLChar::MsgReqPVPPBGReBirth IsSTATE(EM_ACT_DIE) false" );
		return S_OK;
	}

	ReSetSTATE(EM_ACT_DIE);

	GLMSG::SNETPC_PVP_PBG_C2AF_REVIVE_REQ *pNetMsg = (GLMSG::SNETPC_PVP_PBG_C2AF_REVIVE_REQ *) nmg;

	SNATIVEID sLobbyMap = PVPPBG::ManagerField::GetInstance().m_sLobbyMap;
	DWORD dwLobbyGate = PVPPBG::ManagerField::GetInstance().m_dwLobbyGate;

	GLLandMan *pLandMan = GLGaeaServer::GetInstance().GetByMapID ( sLobbyMap );
	if ( !pLandMan )
	{
		CDebugSet::ToFileWithTime( "_PVPPBG.txt", "GLChar::MsgReqPVPPBGReBirth invalid lobby map[%d/%d]", sLobbyMap.wMainID );
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLChar::MsgReqPVPPBGReBirth invalid lobby map[%d/%d]", sLobbyMap.wSubID );
		return FALSE;
	}

	PDXLANDGATE pLandGate = pLandMan->GetLandGateMan().FindLandGate(dwLobbyGate);
	if ( !pLandGate )
	{
		CDebugSet::ToFileWithTime( "_PVPPBG.txt", "GLChar::MsgReqPVPPBGReBirth invalid lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wMainID );
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLChar::MsgReqPVPPBGReBirth invalid lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wSubID );
		return FALSE;
	}

	BOOL bOk = GLGaeaServer::GetInstance().RequestReBirth ( m_dwGaeaID, pLandMan->GetMapID(), dwLobbyGate, D3DXVECTOR3(0,0,0) );
	if ( !bOk )
	{
		CDebugSet::ToFileWithTime( "_PVPPBG.txt", "GLChar::MsgReqPVPPBGReBirth !bOk lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wMainID );
		GLGaeaServer::GetInstance().GetConsoleMsg()->Write( "GLChar::MsgReqPVPPBGReBirth !bOk lobby gate[%d] map[%d/%d]", dwLobbyGate, sLobbyMap.wSubID );
	}

	m_fGenAge = 0.0f;
	GLCHARLOGIC_SERVER::INIT_RECOVER();
	TurnAction ( GLAT_IDLE );

	GLGaeaServer::GetInstance().ReserveDropOutPet ( SDROPOUTPETINFO(m_dwPetGUID,true,true) );
	//GLGaeaServer::GetInstance().ReserveDropOutSummon ( SDROPOUTSUMMONINFO(m_dwSummonGUID,true) );
	GLGaeaServer::GetInstance().SaveVehicle( m_dwClientID, m_dwGaeaID, true );

	/*skill summon, Juver, 2017/10/09 */
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		GLGaeaServer::GetInstance().ReserveDropOutSummon ( m_dwSummonGUID_FLD[i] );

	GLMSG::SNETPC_PVP_PBG_F2AC_REVIVE_REQ_FB NetMsg;
	NetMsg.sMapID = m_pLandMan->GetMapID();
	NetMsg.vPos = GetPosition();
	NetMsg.wNowHP = m_sHP.wNow;
	NetMsg.wNowMP = m_sMP.wNow;
	NetMsg.wNowSP = m_sSP.wNow;

	GLGaeaServer::GetInstance().SENDTOAGENT(m_dwClientID,&NetMsg);

	return S_OK;
}
