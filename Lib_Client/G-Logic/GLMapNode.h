#ifndef GLMAPNODE_H_
#define GLMAPNODE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "../../Lib_Engine/Common/ByteStream.h"
#include "../../Lib_Engine/Common/SerialFile.h"

#include "./GLLevelFile.h"
#include "./GLMapNodeDefine.h"

struct SMAPNODE
{
	enum { VERSION = 0x0221 };

	bool			bUsed;
	std::string		strFile;
	SNATIVEID		sNativeID;
	DWORD			dwFieldSID;

	bool			bPeaceZone;
	bool			bCommission;
	bool			bPKZone;
	bool			bFreePK;
	bool			bItemDrop;
	bool			bMove;
	bool			bRestart;
	bool			bPetActivity;
	bool			bDECEXP;
	bool			bVehicleActivity;
	bool			bClubBattleZone;
	bool			bInstantMap;
	bool			bQBoxEnable;
	bool			bLunchBoxForbid;

	/*combatpoint logic, Juver, 2017/05/29 */
	bool			bCPReset; 
	bool			bCPResetDeath;

	bool			bPKMap;

	bool			bUIMapSelect;
	bool			bUIMapInfo;
	bool			bClubPKRecord;

	/* map private market setting, Juver, 2017/10/02 */
	bool			bOpenPrivateMarket; 

	bool			bPartySparring;

	/*map move settings, Juver, 2017/11/24 */
	bool			bBlockTaxi;
	bool			bBlockFriendCard;
	bool			bBlockRecall;
	bool			bBlockTeleport;

	std::string		strMapName;
	std::string		strBGM;
	std::string		strLoadingImageName;

	bool			bBattleZone;	//club war
	bool			bCDMZone;		//club death match

	/*pvp tyranny, Juver, 2017/08/25 */
	bool			bPVPTyrannyZone;	

	/*school wars, Juver, 2018/01/19 */
	bool			bPVPSchoolWarsZone;		

	/*pvp capture the flag, Juver, 2018/01/24 */
	bool			bPVPCaptureTheFlagZone;	

	/* pvp club death match, Juver, 2020/11/25 */
	bool			bPVPClubDeathMatchBattleZone;
	bool			bPVPClubDeathMatchLobbyZone;

	/*global buffs map setting, Juver, 2018/01/23 */
	bool			bDisableSystemBuffs;

	/*hp potion map setting, Juver, 2018/01/23 */
	bool			bBlockHPPotion;
	
	/* Boss Spawn Viewer, Review000 */
	bool			bBossMap;

	/*map party setting, Juver, 2018/06/29 */
	bool			bBlockParty;

	/*force revive, Juver, 2018/07/09 */
	bool			bForceRevive;

	/* open pk function, Juver, 2019/12/23 */
	bool			bOpenPK;

	/* death hide, Juver, 2020/02/02 */
	bool			bDeathHide;

	/* map entry user verified, Juver, 2020/02/27 */
	bool			bUserVerifiedMapEntry;

	/* costume combine map restriction, Juver, 2020/09/11 */
	bool			bCostumeCombineRestrictMoveSpeed;
	bool			bCostumeCombineRestrictAttackSpeed;

	/* trade restricted, Juver, 2021/06/07 */
	bool			bTradeRestricted;

	bool			bAllowHideDead;

	bool			bShowPkMark;

	bool			bUseAutoPotion;
	bool			bAutoPotionManual;

	bool			bUseAutoPilot;

	bool			bPKMapCount;

	bool			bUseCaptcha;

	bool			bPVPPBGBattleZone;
	bool			bPVPPBGLobbyZone;

	bool			bBlockGeneralChat;

	bool			bResetSkillDelayOnDie;
	
	/*woe Arc Development 08-06-2024*/
	bool			bPVPWoeZone;

	SLEVEL_REQUIRE	sLEVEL_REQUIRE;
	SLEVEL_ETC_FUNC sLEVEL_ETC_FUNC;

	SMAPNODE () :
		bUsed(false),
		dwFieldSID(0),
		bPeaceZone(false),
		bCommission(false),
		bPKZone(false),
		bFreePK(false),
		bItemDrop(false),
		bMove(false),
		bRestart(false),
		bDECEXP(true),
		bPetActivity(false),
		bVehicleActivity(false),
		bClubBattleZone(false),
		bInstantMap(false),
		bQBoxEnable(false),
		bLunchBoxForbid(false),

		/*combatpoint logic, Juver, 2017/05/29 */
		bCPReset(false), 
		bCPResetDeath(true),		//defaults to true for original behavior or reseting cp on death

		bPKMap(false),
		bUIMapSelect(false),
		bUIMapInfo(false),
		bClubPKRecord(false),

		/* map private market setting, Juver, 2017/10/02 */
		bOpenPrivateMarket(true), 

		bPartySparring(false),

		/*map move settings, Juver, 2017/11/24 */
		bBlockTaxi(false),
		bBlockFriendCard(false),
		bBlockRecall(false),
		bBlockTeleport(false),

		/*global buffs map setting, Juver, 2018/01/23 */
		bDisableSystemBuffs(false),

		/*hp potion map setting, Juver, 2018/01/23 */
		bBlockHPPotion(false),

		/*map party setting, Juver, 2018/06/29 */
		bBlockParty(false),

		/*force revive, Juver, 2018/07/09 */
		bForceRevive(false),

		/* open pk function, Juver, 2019/12/23 */
		bOpenPK(false),

		/* death hide, Juver, 2020/02/02 */
		bDeathHide(false),

		/* map entry user verified, Juver, 2020/02/27 */
		bUserVerifiedMapEntry(false),

		bBattleZone(false),
		bCDMZone(false),

		/*pvp tyranny, Juver, 2017/08/25 */
		bPVPTyrannyZone(false),		

		/*school wars, Juver, 2018/01/19 */
		bPVPSchoolWarsZone(false),		

		/*pvp capture the flag, Juver, 2018/01/24 */
		bPVPCaptureTheFlagZone(false),	

		/* pvp club death match, Juver, 2020/11/25 */
		bPVPClubDeathMatchBattleZone(false),
		bPVPClubDeathMatchLobbyZone(false),

		/* costume combine map restriction, Juver, 2020/09/11 */
		bCostumeCombineRestrictMoveSpeed(false),
		bCostumeCombineRestrictAttackSpeed(false),

		/* trade restricted, Juver, 2021/06/07 */
		bTradeRestricted(false),

		bAllowHideDead(false),

		bShowPkMark(false),

		bUseAutoPotion(false),
		bAutoPotionManual(false),

		bUseAutoPilot(false),

		bPKMapCount(false),

		bUseCaptcha(false),

		bPVPPBGBattleZone(false),
		bPVPPBGLobbyZone(false),

		bBlockGeneralChat(false),
		bResetSkillDelayOnDie(false),
						
		/* Boss Spawn Viewer, Review000 */
		bBossMap(false),
		/*woe Arc Development 08-06-2024*/
		bPVPWoeZone(false)
	{
	}

	SMAPNODE ( const SMAPNODE& value )
	{
		operator = ( value );
	}

	SMAPNODE& operator = ( const SMAPNODE& value );

	BOOL LOAD ( basestream &SFile );
	BOOL SAVE ( CSerialFile &SFile );

	static VOID SaveCsvHead ( std::fstream &SFile );
	VOID SaveCsv ( std::fstream &SFile ) const;
	VOID LoadCsv ( CStringArray &StrArray );
};

#endif // GLMAPNODE_H_