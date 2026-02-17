#include "stdafx.h"
#include "./NSRParam.h"

#include "./NSRPath.h"
#include "./NSRInfo.h"
#include "./NSRLog.h"
#include "./NSROption.h"
#include "../Lib_Engine/Common/IniLoader.h"
#include "../Lib_Engine/DxCommon/DebugSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RPARAM
{
	BOOL Init ( bool bServer )
	{
		Load();
		ROPTION::Load( bServer );


#ifdef CH_PARAM

		if ( nChinaRegion == MAX_CHINA_REGION )	return TRUE;

		StringCchCopy( LoginAddress, RANPARAM_STRING_NUM_128, China_Region[nChinaRegion].LoginAddress.GetString() );
		nLoginPort =  China_Region[nChinaRegion].nLoginPort; 

		for( int i=0 ; ; ++i )
		{
			if( China_Region[nChinaRegion].HttpAddressTable[i].IsEmpty() ) 
				break;

			StringCchCopy( HttpAddressTable[i], RANPARAM_STRING_NUM_128, China_Region[nChinaRegion].HttpAddressTable[i].GetString() );
		}

		strNewsURL = China_Region[nChinaRegion].strNewsURL;
#endif

		return TRUE;
	}

	BOOL Load()
	{
		std::string	strFile = RPATH::getAppPath();
		strFile += RINFO::getc( EMRINFO_FILE_NAME_DBSLASH );
		strFile += RINFO::getc( EMRINFO_FILE_NAME_RPARAM );

		CIniLoader cFILE;

		if( !cFILE.open( strFile, true) )
		{
			RLOG::Log( RINFO::getc( EMRINFO_ERROR_RPARAM_LOAD ), strFile.c_str() );
			return FALSE;
		}

		//cFILE.SetUseErrorMsgBox( FALSE );

		{
			DWORD dwServiceType = EMSERVICE_KOREA;
			cFILE.getflag ( "SERVER SET", "dwServiceType", 0, 1, dwServiceType );
			emSERVICE_TYPE = (EMSERVICE_TYPE)dwServiceType;

			//cFILE.getflag_sz( "SERVER SET", "LoginAddress", 0, 1, LoginAddress, RANPARAM_STRING_NUM_128 );
			//cFILE.getflag ( "SERVER SET", "nLoginPort", 0, 1, nLoginPort );

			CString strSection;

			for ( int nLogin=0; nLogin<MAX_LOGIN_IP; ++nLogin )
			{
				strSection.Format( "LOGIN_%d", nLogin );
				cFILE.getflag( strSection.GetString(), "LoginAddress", 0, 1, sLoginAddress[nLogin].strLoginIP );
				cFILE.getflag( strSection.GetString(), "LoginPort", 0, 1, sLoginAddress[nLogin].nLoginPort );
			}
			

			UINT nNum = cFILE.GetKeyDataSize( "SERVER SET", "HttpAddress" );

			memset( HttpAddressTable, 0, sizeof(HttpAddressTable) );

			for ( UINT i=0; i<nNum; ++i )
			{
				CString strHttpAddress;
				cFILE.getflag( "SERVER SET", "HttpAddress", i, nNum, strHttpAddress );
				strHttpAddress.Trim( _T(" ") );

				StringCchCopy( HttpAddressTable[i], RANPARAM_STRING_NUM_128, strHttpAddress );
			}

			cFILE.getflag ( "SERVER SET", "strNewsURL", 0, 1, strNewsURL );
			cFILE.getflag ( "SERVER SET", "strConnectionURL", 0, 1, strConnectionURL );
			cFILE.getflag ( "SERVER SET", "bUsePassiveDN", 0, 1, bUsePassiveDN );	
		}

#ifdef CH_PARAM
		{
			CString strSection;

			for( UINT i=0; i<MAX_CHINA_REGION; ++i )
			{
				strSection.Format( "CH_REGION_%d", i );
				cFILE.getflag( strSection.GetString(), "strName", 0, 1, China_Region[i].strName );
				cFILE.getflag( strSection.GetString(), "LoginAddress", 0, 1, China_Region[i].LoginAddress );
				cFILE.getflag( strSection.GetString(), "nLoginPort", 0, 1, China_Region[i].nLoginPort );

				UINT nNum = cFILE.GetKeyDataSize( strSection.GetString(), "HttpAddress" );
				if( nNum > MAX_HTTP ) nNum = MAX_HTTP;
				for( UINT j=0; j<nNum; ++j )
				{
					cFILE.getflag( strSection.GetString(), "HttpAddress", j, nNum, China_Region[i].HttpAddressTable[j] );
					China_Region[i].HttpAddressTable[j].Trim( _T(" ") );
				}

				cFILE.getflag( strSection.GetString(), "strNewsURL", 0, 1, China_Region[i].strNewsURL );

				nNum = cFILE.GetKeyDataSize( strSection.GetString(), "ServerName" );
				if( nNum > MAX_SERVER ) nNum = MAX_SERVER;
				for( UINT j=0; j<nNum; ++j )
				{
					cFILE.getflag( strSection.GetString(), "ServerName", j, nNum, China_Region[i].ServerName[j] );
					China_Region[i].ServerName[j].Trim( _T(" ") );
				}

				cFILE.getflag( strSection.GetString(), "ItemShopAddress", 0, 1, China_Region[i].strItemShop );
			}
		}
#endif

		{
			DWORD dwLangSet = 0;
			cFILE.getflag( "GUI OPTION", "dwLangSet", 0, 1, dwLangSet );
			emLangSet = language::LANGFLAG(dwLangSet);

			cFILE.getflag( "GUI OPTION", "strGDIFont", 0, 1, strGDIFont );
		}
		
		{
			cFILE.getflag ( "TEXT FILE", "bXML_USE", 0, 1, bXML_USE );

			cFILE.getflag ( "TEXT FILE", "strCountry", 0, 1, strCountry );
			cFILE.getflag ( "TEXT FILE", "strLauncherText", 0, 1, strLauncherText );	
			cFILE.getflag ( "TEXT FILE", "strGameInText", 0, 1, strGameInText );
			cFILE.getflag ( "TEXT FILE", "strGameExText", 0, 1, strGameExText );
			cFILE.getflag ( "TEXT FILE", "strGameWord", 0, 1, strGameWord );
			cFILE.getflag ( "TEXT FILE", "strServerText", 0, 1, strServerText );

			cFILE.getflag ( "TEXT FILE", "strUIOuterCfg01", 0, 1, strUIOuterCfg01 );
			cFILE.getflag ( "TEXT FILE", "strUIOuterCfg02", 0, 1, strUIOuterCfg02 );
			cFILE.getflag ( "TEXT FILE", "strUIOuterCfg03", 0, 1, strUIOuterCfg03 );

			cFILE.getflag ( "TEXT FILE", "strUIInnerCfg01", 0, 1, strUIInnerCfg01 );
			cFILE.getflag ( "TEXT FILE", "strUIInnerCfg02", 0, 1, strUIInnerCfg02 );
			cFILE.getflag ( "TEXT FILE", "strUIInnerCfg03", 0, 1, strUIInnerCfg03 );
			cFILE.getflag ( "TEXT FILE", "strUIInnerCfg04", 0, 1, strUIInnerCfg04 );
			cFILE.getflag ( "TEXT FILE", "strUIInnerCfg05", 0, 1, strUIInnerCfg05 );
			cFILE.getflag ( "TEXT FILE", "strUIInnerCfg06", 0, 1, strUIInnerCfg06 );

			cFILE.getflag ( "TEXT FILE", "strUIExtCfg01", 0, 1, strUIExtCfg01 );
			cFILE.getflag ( "TEXT FILE", "strUIExtCfg02", 0, 1, strUIExtCfg02 );

			cFILE.getflag ( "TEXT FILE", "strUITextureList", 0, 1, strUITextureList );
			cFILE.getflag ( "TEXT FILE", "strUIXmlList", 0, 1, strUIXmlList );
		}

		cFILE.getflag_sz( "ETC OPTION", "HelpAddress", 0, 1, HelpAddress, RANPARAM_STRING_NUM_128 );
		cFILE.getflag_sz( "ETC OPTION", "ItemShopAddress", 0, 1, ItemShopAddress, RANPARAM_STRING_NUM_128 );
		cFILE.getflag_sz( "ETC OPTION", "RPFDataPath", 0, 1, RPFDataPath, RANPARAM_STRING_NUM_128 );

		//debug
		BOOL bErrorVersion_MessageBox	= TRUE;
		BOOL bErrorVersion_WriteLog		= TRUE;

		BOOL bErrorFile_MessageBox	= TRUE;
		BOOL bErrorFile_WriteLog	= TRUE;
		BOOL bErrorFile_Remember	= TRUE;
		BOOL bErrorFile_ClipBoard	= TRUE;

		BOOL bErrorCode_MessageBox	= TRUE;
		BOOL bErrorCode_WriteLog	= TRUE;

		std::string strErrorVersion_LogFileName = "";
		std::string strErrorFile_LogFileName = "";
		std::string strErrorCode_LogFileName = "";

		cFILE.getflag ( "DEBUGSET", "bErrorVersion_MessageBox", 0, 1, bErrorVersion_MessageBox );
		cFILE.getflag ( "DEBUGSET", "bErrorVersion_WriteLog", 0, 1, bErrorVersion_WriteLog );

		cFILE.getflag ( "DEBUGSET", "bErrorFile_MessageBox", 0, 1, bErrorFile_MessageBox );
		cFILE.getflag ( "DEBUGSET", "bErrorFile_WriteLog", 0, 1, bErrorFile_WriteLog );
		cFILE.getflag ( "DEBUGSET", "bErrorFile_Remember", 0, 1, bErrorFile_Remember );
		cFILE.getflag ( "DEBUGSET", "bErrorFile_ClipBoard", 0, 1, bErrorFile_ClipBoard );

		cFILE.getflag ( "DEBUGSET", "bErrorCode_MessageBox", 0, 1, bErrorCode_MessageBox );
		cFILE.getflag ( "DEBUGSET", "bErrorCode_WriteLog", 0, 1, bErrorCode_WriteLog );

		cFILE.getflag ( "DEBUGSET", "strErrorVersion_LogFileName", 0, 1, strErrorVersion_LogFileName );
		cFILE.getflag ( "DEBUGSET", "strErrorFile_LogFileName", 0, 1, strErrorFile_LogFileName );
		cFILE.getflag ( "DEBUGSET", "strErrorCode_LogFileName", 0, 1, strErrorCode_LogFileName );

		CDebugSet::SetErrorParam_Version( bErrorVersion_MessageBox, bErrorVersion_WriteLog, strErrorVersion_LogFileName );
		CDebugSet::SetErrorParam_File( bErrorFile_MessageBox, bErrorFile_WriteLog, bErrorFile_Remember, bErrorFile_ClipBoard, strErrorFile_LogFileName );
		CDebugSet::SetErrorParam_Code( bErrorCode_MessageBox, bErrorCode_WriteLog, strErrorCode_LogFileName );


		//check
		cFILE.getflag ( "GAME_CHECK", "bQuickSlotDelay", 0, 1, bQuickSlotDelay ); 
		cFILE.getflag ( "GAME_CHECK", "dwQuickSlotDelay", 0, 1, dwQuickSlotDelay ); 

		cFILE.getflag ( "GAME_CHECK", "hs_dll_version", 0, 1, hs_dll_version );	
		cFILE.getflag ( "GAME_CHECK", "hs_close_timer", 0, 1, hs_close_timer );	

		cFILE.getflag ( "GAME_CHECK", "client_hash_check_use", 0, 1, client_hash_check_use );	
		cFILE.getflag ( "GAME_CHECK", "client_hash_check_game_file_1", 0, 1, client_hash_check_game_file_1 );	
		cFILE.getflag ( "GAME_CHECK", "client_hash_check_game_file_2", 0, 1, client_hash_check_game_file_2 );	

		cFILE.getflag ( "GAME_CHECK", "client_tick_count_check", 0, 1, client_tick_count_check );
		cFILE.getflag ( "GAME_CHECK", "client_tick_count_timer", 0, 1, client_tick_count_timer );
		cFILE.getflag ( "GAME_CHECK", "client_tick_count_max_diff", 0, 1, client_tick_count_max_diff );
		cFILE.getflag ( "GAME_CHECK", "client_tick_count_max_error", 0, 1, client_tick_count_max_error );

		cFILE.getflag ( "GAME_CHECK", "strMShieldDLL1", 0, 1, strMShieldDLL1 );	
		cFILE.getflag ( "GAME_CHECK", "strMShieldDLL2", 0, 1, strMShieldDLL2 );	

		//PCID
		cFILE.getflag ( "GAME_CHECK", "bPCIDSingleLogin", 0, 1, bPCIDSingleLogin );
		cFILE.getflag ( "GAME_CHECK", "bPCIDCheckLogin", 0, 1, bPCIDCheckLogin );

		//lobby
		cFILE.getflag ( "GAME_LOBBY", "bRegister", 0, 1, bRegister );	
		cFILE.getflag ( "GAME_LOBBY", "bRegisterUseMD5", 0, 1, bRegisterUseMD5 );	

		cFILE.getflag ( "GAME_LOBBY", "register_email_check", 0, 1, register_email_check );	
		cFILE.getflag ( "GAME_LOBBY", "register_user_min", 0, 1, register_user_min );	
		cFILE.getflag ( "GAME_LOBBY", "register_user_max", 0, 1, register_user_max );	
		cFILE.getflag ( "GAME_LOBBY", "register_pass_min", 0, 1, register_pass_min );	
		cFILE.getflag ( "GAME_LOBBY", "register_pass_max", 0, 1, register_pass_max );	
		cFILE.getflag ( "GAME_LOBBY", "register_pass2_min", 0, 1, register_pass2_min );	
		cFILE.getflag ( "GAME_LOBBY", "register_pass2_max", 0, 1, register_pass2_max );	
		cFILE.getflag ( "GAME_LOBBY", "register_mail_min", 0, 1, register_mail_min );	
		cFILE.getflag ( "GAME_LOBBY", "register_mail_max", 0, 1, register_mail_max );	
		cFILE.getflag ( "GAME_LOBBY", "register_fullname_min", 0, 1, register_fullname_min );	
		cFILE.getflag ( "GAME_LOBBY", "register_fullname_max", 0, 1, register_fullname_max );	
		cFILE.getflag ( "GAME_LOBBY", "register_sa_min", 0, 1, register_sa_min );	
		cFILE.getflag ( "GAME_LOBBY", "register_sa_max", 0, 1, register_sa_max );	
		cFILE.getflag ( "GAME_LOBBY", "register_captcha_min", 0, 1, register_captcha_min );	
		cFILE.getflag ( "GAME_LOBBY", "register_captcha_max", 0, 1, register_captcha_max );	

		//PCID
		cFILE.getflag ( "GAME_LOBBY", "bRegisterPCIDCheck", 0, 1, bRegisterPCIDCheck );	

		DWORD dwReferral = 0;
		cFILE.getflag ( "GAME_LOBBY", "emRegisterReferral", 0, 1, dwReferral );	
		emRegisterReferral = static_cast<EMREFERRAL_REGISTER_TYPE>(dwReferral);

		cFILE.getflag ( "GAME_LOBBY", "bCharacterDelete", 0, 1, bCharacterDelete );	
		cFILE.getflag ( "GAME_LOBBY", "bCharacterDeleteMD5", 0, 1, bCharacterDeleteMD5 );	

		cFILE.getflag ( "GAME_LOBBY", "use_login_control", 0, 1, use_login_control );	
		cFILE.getflag ( "GAME_LOBBY", "use_login_control_md5", 0, 1, use_login_control_md5 );	

		cFILE.getflag ( "GAME_LOBBY", "bCreateGunner", 0, 1, bCreateGunner );	
		cFILE.getflag ( "GAME_LOBBY", "bCreateAssassin", 0, 1, bCreateAssassin );	
		cFILE.getflag ( "GAME_LOBBY", "bCreateMagician", 0, 1, bCreateMagician );	

		cFILE.getflag ( "GAME_LOBBY", "strLoginMapFile", 0, 1, strLoginMapFile );
		cFILE.getflag ( "GAME_LOBBY", "strLoginMapCameraPos", 0, 1, strLoginMapCameraPos );
		cFILE.getflag ( "GAME_LOBBY", "strLoginMapCameraAni", 0, 1, strLoginMapCameraAni );

		cFILE.getflag ( "GAME_LOBBY", "strSelectCharMapFile", 0, 1, strSelectCharMapFile );
		cFILE.getflag ( "GAME_LOBBY", "strSelectCharMapCameraPos", 0, 1, strSelectCharMapCameraPos );
		cFILE.getflag ( "GAME_LOBBY", "strSelectCharMapCameraAni", 0, 1, strSelectCharMapCameraAni );

		cFILE.getflag ( "GAME_LOBBY", "strCreateCharMapFile", 0, 1, strCreateCharMapFile );
		cFILE.getflag ( "GAME_LOBBY", "strCreateCharMapCameraPos", 0, 1, strCreateCharMapCameraPos );
		cFILE.getflag ( "GAME_LOBBY", "strCreateCharMapCameraAni", 0, 1, strCreateCharMapCameraAni );

		cFILE.getflag ( "GAME_LOBBY", "bLoginPageControl", 0, 1, bLoginPageControl );
		cFILE.getflag ( "GAME_LOBBY", "nLoginPageUserMaxLength", 0, 1, nLoginPageUserMaxLength );
		cFILE.getflag ( "GAME_LOBBY", "nLoginPagePassMaxLength", 0, 1, nLoginPagePassMaxLength );
		cFILE.getflag ( "GAME_LOBBY", "nLoginPageRPMaxLength", 0, 1, nLoginPageRPMaxLength );
		cFILE.getflag ( "GAME_LOBBY", "bLoginPageUseChannel", 0, 1, bLoginPageUseChannel );

		//feature
		cFILE.getflag_sz( "GAME_FEATURE", "szClientWindowTitle", 0, 1, szClientWindowTitle, RANPARAM_STRING_NUM_128 );

		cFILE.getflag ( "GAME_FEATURE", "bProduct", 0, 1, bProduct );	
		cFILE.getflag ( "GAME_FEATURE", "bProductIcon", 0, 1, bProductIcon );	

		cFILE.getflag ( "GAME_FEATURE", "bStudentRecord", 0, 1, bStudentRecord );	

		cFILE.getflag ( "GAME_FEATURE", "bActivity", 0, 1, bActivity );	

		cFILE.getflag ( "GAME_FEATURE", "bFeatureDailyLogin", 0, 1, bFeatureDailyLogin );	

		cFILE.getflag ( "GAME_FEATURE", "bWoeGVG", 0, 1, bWoeGVG );	

		cFILE.getflag ( "GAME_FEATURE", "bPVPWoeWar", 0, 1, bPVPWoeWar );
		
		cFILE.getflag ( "GAME_FEATURE", "bFeatureBattlePass", 0, 1, bFeatureBattlePass );	

		cFILE.getflag ( "GAME_FEATURE", "bFeaturePandora", 0, 1, bFeaturePandora );

		cFILE.getflag ( "GAME_FEATURE", "bFeaturePlatBuff", 0, 1, bFeaturePlatBuff );

		cFILE.getflag ( "GAME_FEATURE", "bFeatureCodex", 0, 1, bFeatureCodex );	

		cFILE.getflag ( "GAME_FEATURE", "bFeatureGacha", 0, 1, bFeatureGacha );	

		cFILE.getflag ( "GAME_FEATURE", "bFeatureBossViewer", 0, 1, bFeatureBossViewer );

		cFILE.getflag ( "GAME_FEATURE", "bOpenSkillsMax9", 0, 1, bOpenSkillsMax9 );

		cFILE.getflag ( "GAME_FEATURE", "bFeatureDisplayCP", 0, 1, bFeatureDisplayCP );

		cFILE.getflag ( "GAME_FEATURE", "bBypassLateRegTW", 0, 1, bBypassLateRegTW );

		cFILE.getflag ( "GAME_FEATURE", "bAutoInviGM", 0, 1, bAutoInviGM );

		cFILE.getflag ( "GAME_FEATURE", "dwGlobalRankingClass", 0, 1, dwGlobalRankingClass );

		cFILE.getflag ( "GAME_FEATURE", "dwNewsWindowButtons", 0, 1, dwNewsWindowButtons );

		/* charinfoview, Juver, 2020/03/03 */
		cFILE.getflag ( "GAME_FEATURE", "bViewCharInfo", 0, 1, bViewCharInfo );	
		cFILE.getflag ( "GAME_FEATURE", "fViewCharInfoDelay", 0, 1, fViewCharInfoDelay );	
		cFILE.getflag ( "GAME_FEATURE", "bViewCharAllowPrivate", 0, 1, bViewCharAllowPrivate );


		cFILE.getflag ( "GAME_FEATURE", "bUseRecoveryEXP", 0, 1, bUseRecoveryEXP );	
	
		cFILE.getflag ( "GAME_FEATURE", "bHideGMInfo", 0, 1, bHideGMInfo );	

		cFILE.getflag ( "GAME_FEATURE", "wSkillRangeCheckAdd", 0, 1, wSkillRangeCheckAdd );	

		cFILE.getflag ( "GAME_FEATURE", "bPetSkillDisplay", 0, 1, bPetSkillDisplay );	

		cFILE.getflag ( "GAME_FEATURE", "bRegionalChatColor", 0, 1, bRegionalChatColor );	
		cFILE.getflag ( "GAME_FEATURE", "nRegionalChatColorR", 0, 1, nRegionalChatColorR );	
		cFILE.getflag ( "GAME_FEATURE", "nRegionalChatColorG", 0, 1, nRegionalChatColorG );	
		cFILE.getflag ( "GAME_FEATURE", "nRegionalChatColorB", 0, 1, nRegionalChatColorB );	
		cFILE.getflag ( "GAME_FEATURE", "dwRegionalChatCost", 0, 1, dwRegionalChatCost );	

		cFILE.getflag ( "GAME_FEATURE", "bBlockDamageBack", 0, 1, bBlockDamageBack );	
		cFILE.getflag ( "GAME_FEATURE", "fBlockDamageBackTimer", 0, 1, fBlockDamageBackTimer );	

		cFILE.getflag ( "GAME_FEATURE", "bClubWarVerifyDisablePushPull", 0, 1, bClubWarVerifyDisablePushPull );	

		cFILE.getflag ( "GAME_FEATURE", "bChatShowStaffName", 0, 1, bChatShowStaffName );	

		cFILE.getflag ( "GAME_FEATURE", "bChangeSchoolReqCleanQuest", 0, 1, bChangeSchoolReqCleanQuest );	
		cFILE.getflag ( "GAME_FEATURE", "bChangeSchoolReqCleanGuild", 0, 1, bChangeSchoolReqCleanGuild );	
		cFILE.getflag ( "GAME_FEATURE", "bChangeSchoolReqMaxLevel", 0, 1, bChangeSchoolReqMaxLevel );	
		cFILE.getflag ( "GAME_FEATURE", "bChangeSchoolWipeQuest", 0, 1, bChangeSchoolWipeQuest );	

		cFILE.getflag ( "GAME_FEATURE", "dwItemTransferCost", 0, 1, dwItemTransferCost );	

		cFILE.getflag ( "GAME_FEATURE", "bEventTyranny", 0, 1, bEventTyranny );	
		cFILE.getflag ( "GAME_FEATURE", "bEventCaptureTheFlag", 0, 1, bEventCaptureTheFlag );	
		cFILE.getflag ( "GAME_FEATURE", "bEventSchoolWars", 0, 1, bEventSchoolWars );	
		cFILE.getflag ( "GAME_FEATURE", "bEventClubDeathMatch", 0, 1, bEventClubDeathMatch );		/* pvp club death match, Juver, 2020/11/26 */
		cFILE.getflag ( "GAME_FEATURE", "bEventPartyBattleGrounds", 0, 1, bEventPartyBattleGrounds );


		cFILE.getflag ( "GAME_FEATURE", "bUseClubAlliance", 0, 1, bUseClubAlliance );	

		cFILE.getflag ( "GAME_FEATURE", "use_inven_sort", 0, 1, use_inven_sort );	

		cFILE.getflag ( "GAME_FEATURE", "force_revive_time", 0, 1, force_revive_time );	

		cFILE.getflag ( "GAME_FEATURE", "allow_buff_remove", 0, 1, allow_buff_remove );	

		cFILE.getflag ( "GAME_FEATURE", "use_target_information", 0, 1, use_target_information );	

		cFILE.getflag ( "GAME_FEATURE", "allow_skill_tab_change", 0, 1, allow_skill_tab_change );	
		cFILE.getflag ( "GAME_FEATURE", "allow_skill_tab_change_buff_reset", 0, 1, allow_skill_tab_change_buff_reset );	

		cFILE.getflag ( "GAME_FEATURE", "allow_lunchbox_remove", 0, 1, allow_lunchbox_remove );	

		cFILE.getflag ( "GAME_FEATURE", "game_launch_param", 0, 1, game_launch_param );	

		cFILE.getflag ( "GAME_FEATURE", "extended_buff_info", 0, 1, extended_buff_info );	

		cFILE.getflag ( "GAME_FEATURE", "link_drop_item", 0, 1, link_drop_item );	

		cFILE.getflag ( "GAME_FEATURE", "block_locker_money_add", 0, 1, block_locker_money_add );	
		cFILE.getflag ( "GAME_FEATURE", "block_locker_money_take", 0, 1, block_locker_money_take );	
		cFILE.getflag ( "GAME_FEATURE", "block_guild_locker_money_add", 0, 1, block_guild_locker_money_add );	
		cFILE.getflag ( "GAME_FEATURE", "block_guild_locker_money_take", 0, 1, block_guild_locker_money_take );	

		cFILE.getflag ( "GAME_FEATURE", "damage_reduction_limit", 0, 1, damage_reduction_limit );	

		cFILE.getflag ( "GAME_FEATURE", "item_random_option_rebuild_use_delay", 0, 1, item_random_option_rebuild_use_delay );
		cFILE.getflag ( "GAME_FEATURE", "item_random_option_rebuild_delay_time", 0, 1, item_random_option_rebuild_delay_time );

		//cFILE.getflag ( "GAME_FEATURE", "hide_costume_use", 0, 1, hide_costume_use );
		//cFILE.getflag ( "GAME_FEATURE", "hide_costume_delay_time", 0, 1, hide_costume_delay_time );

		cFILE.getflag ( "GAME_FEATURE", "pet_call_delay", 0, 1, pet_call_delay );

		cFILE.getflag ( "GAME_FEATURE", "fRenderSettingChangeTime", 0, 1, fRenderSettingChangeTime );

		cFILE.getflag ( "GAME_FEATURE", "bTextClassConst", 0, 1, bTextClassConst );

		cFILE.getflag ( "GAME_FEATURE", "bClubWarRankings", 0, 1, bClubWarRankings );
		
		cFILE.getflag ( "GAME_FEATURE", "fCurrencyShopPurchaseDelayTime", 0, 1, fCurrencyShopPurchaseDelayTime );

		cFILE.getflag ( "GAME_FEATURE", "bCheckWearUse", 0, 1, bCheckWearUse );
		cFILE.getflag ( "GAME_FEATURE", "bCheckWearSlot", 0, 1, bCheckWearSlot );

		cFILE.getflag ( "GAME_FEATURE", "dwChatBlockMaxMinute", 0, 1, dwChatBlockMaxMinute );

		cFILE.getflag ( "GAME_FEATURE", "bClubWarGMMapStay", 0, 1, bClubWarGMMapStay );

		cFILE.getflag ( "GAME_FEATURE", "fKickOutMinTime", 0, 1, fKickOutMinTime );
		cFILE.getflag ( "GAME_FEATURE", "fKickOutMaxTime", 0, 1, fKickOutMaxTime );

		/* personal lock system, Juver, 2020/06/20 */
		cFILE.getflag ( "GAME_FEATURE", "bUsePersonalLock", 0, 1, bUsePersonalLock );
		cFILE.getflag ( "GAME_FEATURE", "fPersonalLockCoolDownTime", 0, 1, fPersonalLockCoolDownTime );
		cFILE.getflag ( "GAME_FEATURE", "wPersonalLockMaxAttempt", 0, 1, wPersonalLockMaxAttempt );

		/* party finder, Juver, 2019/12/28 */
		cFILE.getflag ( "GAME_FEATURE", "bUsePartyFinder", 0, 1, bUsePartyFinder );
		cFILE.getflag ( "GAME_FEATURE", "fPartyFinderSearchDelay", 0, 1, fPartyFinderSearchDelay );
		cFILE.getflag ( "GAME_FEATURE", "fPartyFinderJoinDelay", 0, 1, fPartyFinderJoinDelay );

		cFILE.getflag ( "GAME_FEATURE", "bGameItemShop", 0, 1, bGameItemShop );

		/* megaphone user verfied, Juver, 2020/02/27 */
		cFILE.getflag ( "GAME_FEATURE", "bUserVerifiedLoudSpeaker", 0, 1, bUserVerifiedLoudSpeaker );

		/* mob item drop direct +, Juver, 2020/10/16 */
		cFILE.getflag ( "GAME_FEATURE", "bMobItemDropDirectPlus", 0, 1, bMobItemDropDirectPlus );

		/* force auto revive, Juver, 2020/12/23 */
		cFILE.getflag ( "GAME_FEATURE", "fForceAutoReviveTime", 0, 1, fForceAutoReviveTime );

		/* post system, Juver, 2021/02/05 */
		cFILE.getflag ( "GAME_FEATURE", "bUsePost", 0, 1, bUsePost );

		cFILE.getflag ( "GAME_FEATURE", "fSRC1", 0, 1, fSRC1 );
		cFILE.getflag ( "GAME_FEATURE", "fSRC2", 0, 1, fSRC2 );

		/* skill collision check, Juver, 2021/07/10 */
		cFILE.getflag ( "GAME_FEATURE", "bSCCC", 0, 1, bSCCC );
		cFILE.getflag ( "GAME_FEATURE", "bSCCS", 0, 1, bSCCS );
		//cFILE.getflag ( "GAME_FEATURE", "bSCC", 0, 1, bSCC );
		//cFILE.getflag ( "GAME_FEATURE", "fSCC1", 0, 1, fSCC1 );
		//cFILE.getflag ( "GAME_FEATURE", "fSCC2", 0, 1, fSCC2 );
		//cFILE.getflag ( "GAME_FEATURE", "fSCC3", 0, 1, fSCC3 );

		/* tyranny AFK disconnect, Juver, 2021/08/06 */
		cFILE.getflag ( "GAME_FEATURE", "bTyrannyAFKDC", 0, 1, bTyrannyAFKDC );
		cFILE.getflag ( "GAME_FEATURE", "fTyrannyAFKDCTime", 0, 1, fTyrannyAFKDCTime );

		/* reborn system, Juver, 2021/09/17 */
		cFILE.getflag ( "GAME_FEATURE", "bRBSystem", 0, 1, bRBSystem );

		cFILE.getflag ( "GAME_FEATURE", "bCaptureTheFlagAFKDC", 0, 1, bCaptureTheFlagAFKDC );
		cFILE.getflag ( "GAME_FEATURE", "fCaptureTheFlagAFKDCTime", 0, 1, fCaptureTheFlagAFKDCTime );

		cFILE.getflag ( "GAME_FEATURE", "bDiscardItemQuestion", 0, 1, bDiscardItemQuestion );

		cFILE.getflag ( "GAME_FEATURE", "bMaxRvPreviewEnable", 0, 1, bMaxRvPreviewEnable );
		cFILE.getflag ( "GAME_FEATURE", "bMaxRvPreviewMobGen", 0, 1, bMaxRvPreviewMobGen );
		cFILE.getflag ( "GAME_FEATURE", "bMaxRvPreviewRebuild", 0, 1, bMaxRvPreviewRebuild );

		cFILE.getflag ( "GAME_FEATURE", "bUseHelpWindow", 0, 1, bUseHelpWindow );

		cFILE.getflag ( "GAME_FEATURE", "bEnchantNotice", 0, 1, bEnchantNotice );
		cFILE.getflag ( "GAME_FEATURE", "wEnchantNoticeMinLevel", 0, 1, wEnchantNoticeMinLevel );

		cFILE.getflag ( "GAME_FEATURE", "bComboWhoreNotice", 0, 1, bComboWhoreNotice );

		cFILE.getflag ( "GAME_FEATURE", "bShowPingFPS", 0, 1, bShowPingFPS );

		cFILE.getflag ( "GAME_FEATURE", "bShowNextTyranny", 0, 1, bShowNextTyranny );
		
		cFILE.getflag ( "GAME_FEATURE", "bUseAutoPotion", 0, 1, bUseAutoPotion );
		cFILE.getflag ( "GAME_FEATURE", "fAutoPotionDelay", 0, 1, fAutoPotionDelay );

		cFILE.getflag ( "GAME_FEATURE", "bUseAutoPilot", 0, 1, bUseAutoPilot );
		cFILE.getflag ( "GAME_FEATURE", "fAutoPilotRangeMax", 0, 1, fAutoPilotRangeMax );

		cFILE.getflag ( "GAME_FEATURE", "bVIPSystem", 0, 1, bVIPSystem );

		cFILE.getflag ( "GAME_FEATURE", "bAllowBTGParty", 0, 1, bAllowBTGParty );
		
		cFILE.getflag ( "GAME_FEATURE", "bMultipleCaptchaShowTimer", 0, 1, bMultipleCaptchaShowTimer );

		cFILE.getflag ( "GAME_FEATURE", "fCaptchaShowTimer", 0, 1, fCaptchaShowTimer );
		cFILE.getflag ( "GAME_FEATURE", "fCaptchaWaitTimer", 0, 1, fCaptchaWaitTimer );

		cFILE.getflag ( "GAME_FEATURE", "fDailyLoginTimeMax", 0, 1, fDailyLoginTimeMax );

		cFILE.getflag ( "GAME_FEATURE", "bCTFGVG", 0, 1, bCTFGVG );
		cFILE.getflag ( "GAME_FEATURE", "bSkillResetReturnSphere", 0, 1, bSkillResetReturnSphere );

		cFILE.getflag ( "GAME_FEATURE", "bHideMiniRank", 0, 1, bHideMiniRank );

		// Logging Controls
		cFILE.getflag ( "GAME_FEATURE", "bCaptchaDcLogs", 0, 1, bCaptchaDcLogs );
		cFILE.getflag ( "GAME_FEATURE", "bTyrannyAfkDcLogs", 0, 1, bTyrannyAfkDcLogs );
		cFILE.getflag ( "GAME_FEATURE", "bCharacterDcLogs", 0, 1, bCharacterDcLogs );
		cFILE.getflag ( "GAME_FEATURE", "bTyrannyRandomMapLogs", 0, 1, bTyrannyRandomMapLogs );
		cFILE.getflag ( "GAME_FEATURE", "bRandomOptionLogs", 0, 1, bRandomOptionLogs );	
		cFILE.getflag ( "GAME_FEATURE", "bItemSortLogs", 0, 1, bItemSortLogs );	

		return TRUE;
	}
};
