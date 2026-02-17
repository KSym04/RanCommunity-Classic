#include "stdafx.h"
#include "./GLMapNode.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


SMAPNODE& SMAPNODE::operator = ( const SMAPNODE& value )
{
	if( this == &value )
		return *this;

	bUsed					= value.bUsed;

	strFile					= value.strFile;
	sNativeID				= value.sNativeID;
	dwFieldSID				= value.dwFieldSID;

	bPeaceZone				= value.bPeaceZone;
	bCommission				= value.bCommission;
	bPKZone					= value.bPKZone;
	bFreePK					= value.bFreePK;
	bItemDrop				= value.bItemDrop;
	bMove					= value.bMove;
	bRestart				= value.bRestart;
	bPetActivity			= value.bPetActivity;
	bDECEXP					= value.bDECEXP;
	bVehicleActivity		= value.bVehicleActivity;
	bClubBattleZone			= value.bClubBattleZone;
	
	bInstantMap				= value.bInstantMap;
	bQBoxEnable				= value.bQBoxEnable;

	bLunchBoxForbid			= value.bLunchBoxForbid;

	/*combatpoint logic, Juver, 2017/05/29 */
	bCPReset				= value.bCPReset; 
	bCPResetDeath			= value.bCPResetDeath;

	bPKMap					= value.bPKMap;

	bUIMapSelect			= value.bUIMapSelect;
	bUIMapInfo				= value.bUIMapInfo;
	bClubPKRecord			= value.bClubPKRecord;

	/* map private market setting, Juver, 2017/10/02 */
	bOpenPrivateMarket		= value.bOpenPrivateMarket;	

	bPartySparring			= value.bPartySparring;

	/*map move settings, Juver, 2017/11/24 */
	bBlockTaxi				= value.bBlockTaxi;
	bBlockFriendCard		= value.bBlockFriendCard;
	bBlockRecall			= value.bBlockRecall;
	bBlockTeleport			= value.bBlockTeleport;

	/*global buffs map setting, Juver, 2018/01/23 */
	bDisableSystemBuffs		= value.bDisableSystemBuffs;

	/*hp potion map setting, Juver, 2018/01/23 */
	bBlockHPPotion			= value.bBlockHPPotion;

	/*map party setting, Juver, 2018/06/29 */
	bBlockParty				= value.bBlockParty;

	/*force revive, Juver, 2018/07/09 */
	bForceRevive			= value.bForceRevive;

	/* open pk function, Juver, 2019/12/23 */
	bOpenPK					= value.bOpenPK;

	/* death hide, Juver, 2020/02/02 */
	bDeathHide				= value.bDeathHide;

	/* map entry user verified, Juver, 2020/02/27 */
	bUserVerifiedMapEntry	= value.bUserVerifiedMapEntry;

	/* costume combine map restriction, Juver, 2020/09/11 */
	bCostumeCombineRestrictMoveSpeed	= value.bCostumeCombineRestrictMoveSpeed;
	bCostumeCombineRestrictAttackSpeed	= value.bCostumeCombineRestrictAttackSpeed;

	/* trade restricted, Juver, 2021/06/07 */
	bTradeRestricted		= value.bTradeRestricted;

	strMapName				= value.strMapName;
	strBGM					= value.strBGM;
	strLoadingImageName		= value.strLoadingImageName;

	bBattleZone				= value.bBattleZone;
	bCDMZone				= value.bCDMZone;
	
	/*woe Arc Development 08-06-2024*/
	bPVPWoeZone = value.bPVPWoeZone;

	/*pvp tyranny, Juver, 2017/08/25 */
	bPVPTyrannyZone			= value.bPVPTyrannyZone;	

	/*school wars, Juver, 2018/01/19 */
	bPVPSchoolWarsZone		= value.bPVPSchoolWarsZone;	

	/*pvp capture the flag, Juver, 2018/01/24 */
	bPVPCaptureTheFlagZone	= value.bPVPCaptureTheFlagZone;	

	/* pvp club death match, Juver, 2020/11/25 */
	bPVPClubDeathMatchBattleZone	= value.bPVPClubDeathMatchBattleZone;
	bPVPClubDeathMatchLobbyZone		= value.bPVPClubDeathMatchLobbyZone;

	bAllowHideDead				= value.bAllowHideDead;

	bShowPkMark					= value.bShowPkMark;

	bUseAutoPotion				= value.bUseAutoPotion;
	bAutoPotionManual			= value.bAutoPotionManual;

	bUseAutoPilot				= value.bUseAutoPilot;

	bPKMapCount					= value.bPKMapCount;

	bUseCaptcha					= value.bUseCaptcha;

	sLEVEL_REQUIRE				= value.sLEVEL_REQUIRE;
	sLEVEL_ETC_FUNC				= value.sLEVEL_ETC_FUNC;
	
	bPVPPBGBattleZone			= value.bPVPPBGBattleZone;
	bPVPPBGLobbyZone			= value.bPVPPBGLobbyZone;

	bBlockGeneralChat			= value.bBlockGeneralChat;

	bResetSkillDelayOnDie		= value.bResetSkillDelayOnDie;
	
	/* Boss Spawn Viewer, Review000 */
	bBossMap				= value.bBossMap;
	
	/*woe Arc Development 08-06-2024*/
	bPVPWoeZone = false;

	return *this;
}

BOOL SMAPNODE::LOAD ( basestream &SFile )
{
	DWORD dwVer(0);

	SFile >> dwVer;

	if ( dwVer == VERSION )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 
		SFile >> bCPResetDeath; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;

		SFile >> bUseAutoPotion;
		SFile >> bAutoPotionManual;

		SFile >> bUseAutoPilot;

		SFile >> bPKMapCount;

		SFile >> bUseCaptcha;

		SFile >> bBlockGeneralChat;

		SFile >> bResetSkillDelayOnDie;
		
		SFile >> bBossMap;
	}
	else if ( dwVer == 0x0220 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 
		SFile >> bCPResetDeath; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;

		SFile >> bUseAutoPotion;
		SFile >> bAutoPotionManual;

		SFile >> bUseAutoPilot;

		SFile >> bPKMapCount;

		SFile >> bUseCaptcha;

		SFile >> bBlockGeneralChat;

		SFile >> bResetSkillDelayOnDie;
		
		SFile >> bBossMap;
	}
	else if ( dwVer == 0x0219 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 
		SFile >> bCPResetDeath; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;

		SFile >> bUseAutoPotion;
		SFile >> bAutoPotionManual;

		SFile >> bUseAutoPilot;

		SFile >> bPKMapCount;

		SFile >> bUseCaptcha;

		SFile >> bBlockGeneralChat;
	}
	else if ( dwVer == 0x0218 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 
		SFile >> bCPResetDeath; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;

		SFile >> bUseAutoPotion;
		SFile >> bAutoPotionManual;

		SFile >> bUseAutoPilot;

		SFile >> bPKMapCount;

		SFile >> bUseCaptcha;
	}
	else if ( dwVer == 0x0217 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 
		SFile >> bCPResetDeath; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;

		SFile >> bUseAutoPotion;

		SFile >> bUseAutoPilot;

		SFile >> bPKMapCount;

		SFile >> bUseCaptcha;
	}
	else if ( dwVer == 0x0216 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 
		SFile >> bCPResetDeath; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;

		SFile >> bUseAutoPotion;

		SFile >> bUseAutoPilot;

		SFile >> bPKMapCount;
	}
	else if ( dwVer == 0x0215 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;

		SFile >> bUseAutoPotion;

		SFile >> bUseAutoPilot;

		SFile >> bPKMapCount;
	}
	else if ( dwVer == 0x0214 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;

		SFile >> bUseAutoPotion;

		SFile >> bUseAutoPilot;
	}
	else if ( dwVer == 0x0213 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;

		SFile >> bUseAutoPotion;
	}
	else if ( dwVer == 0x0212 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;

		SFile >> bShowPkMark;
	}
	else if ( dwVer == 0x0211 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;

		SFile >> bAllowHideDead;
	}
	else if ( dwVer == 0x0210 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;

		/* trade restricted, Juver, 2021/06/07 */
		SFile >> bTradeRestricted;
	}
	else if ( dwVer == 0x0209 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;

		/* costume combine map restriction, Juver, 2020/09/11 */
		SFile >> bCostumeCombineRestrictMoveSpeed;
		SFile >> bCostumeCombineRestrictAttackSpeed;
	}
	else if ( dwVer == 0x0208 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;

		/* map entry user verified, Juver, 2020/02/27 */
		SFile >> bUserVerifiedMapEntry;
	}
	else if ( dwVer == 0x0207 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		 /*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset;

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		 /* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket;

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;

		/* death hide, Juver, 2020/02/02 */
		SFile >> bDeathHide;
	}
	else if ( dwVer == 0x0206 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		 /* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket;

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;

		/* open pk function, Juver, 2019/12/23 */
		SFile >> bOpenPK;
	}
	else if ( dwVer == 0x0205 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		 /*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset;

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;

		/*force revive, Juver, 2018/07/09 */
		SFile >> bForceRevive;
	}
	else if ( dwVer == 0x0204 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;

		/*map party setting, Juver, 2018/06/29 */
		SFile >> bBlockParty;
	}
	else if ( dwVer == 0x0203 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;

		/*hp potion map setting, Juver, 2018/01/23 */
		SFile >> bBlockHPPotion;
	}
	else if ( dwVer == 0x0202 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;

		/*global buffs map setting, Juver, 2018/01/23 */
		SFile >> bDisableSystemBuffs;
	}
	else if ( dwVer == 0x0201 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;

		/*map move settings, Juver, 2017/11/25 */
		SFile >> bBlockTaxi;
		SFile >> bBlockFriendCard;
		SFile >> bBlockRecall;
		SFile >> bBlockTeleport;
	}
	else if ( dwVer == 0x0200 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;

		SFile >> bUIMapSelect;
		SFile >> bUIMapInfo;
		SFile >> bClubPKRecord;

		/* map private market setting, Juver, 2017/10/02 */
		SFile >> bOpenPrivateMarket; 

		SFile >> bPartySparring;
	}
	else if ( dwVer == 0x0103 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 

		SFile >> bPKMap;
	}
	else if ( dwVer == 0x0102 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;

		/*combatpoint logic, Juver, 2017/05/29 */
		SFile >> bCPReset; 
	}
	else if ( dwVer == 0x0101 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;

		SFile >> bLunchBoxForbid;
	}
	else if ( dwVer == 0x0100 )
	{
		SFile >> bUsed;
		SFile >> strFile;
		SFile >> sNativeID.wMainID;
		SFile >> sNativeID.wSubID;
		SFile >> dwFieldSID;

		SFile >> bPeaceZone;

		bool bCommissionLoad;
		SFile >> bCommissionLoad;
		bCommission = !bCommissionLoad;

		SFile >> bPKZone;
		SFile >> bFreePK;
		SFile >> bItemDrop;
		SFile >> bMove;
		SFile >> bRestart;
		SFile >> bPetActivity;
		SFile >> bDECEXP;
		SFile >> bVehicleActivity;
		SFile >> bClubBattleZone;				

		SFile >> strMapName;
		SFile >> strBGM;
		SFile >> strLoadingImageName;

		SFile >> bInstantMap;
		SFile >> bQBoxEnable;
	}
	else
	{
		CDebugSet::ErrorVersion( "SMAPNODE_DATA::LOAD", dwVer );
		return FALSE;
	}


	bBattleZone				= false;
	bCDMZone				= false;

	/*pvp tyranny, Juver, 2017/08/25 */
	bPVPTyrannyZone			= false;	

	/*school wars, Juver, 2018/01/19 */
	bPVPSchoolWarsZone		= false;	

	/*pvp capture the flag, Juver, 2018/01/24 */
	bPVPCaptureTheFlagZone	= false;	

	/* pvp club death match, Juver, 2020/11/25 */
	bPVPClubDeathMatchBattleZone	= false;
	bPVPClubDeathMatchLobbyZone		= false;

	bPVPPBGBattleZone = false;
	bPVPPBGLobbyZone = false;

	sLEVEL_REQUIRE  = SLEVEL_REQUIRE();
	sLEVEL_ETC_FUNC = SLEVEL_ETC_FUNC();


	return TRUE;
}

BOOL SMAPNODE::SAVE ( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION;

	SFile << bUsed;
	SFile << strFile;
	SFile << sNativeID.wMainID;
	SFile << sNativeID.wSubID;
	SFile << dwFieldSID;

	SFile << bPeaceZone;

	bool bCommissionSave = !bCommission;
	SFile << bCommissionSave;

	SFile << bPKZone;
	SFile << bFreePK;
	SFile << bItemDrop;
	SFile << bMove;
	SFile << bRestart;
	SFile << bPetActivity;
	SFile << bDECEXP;
	SFile << bVehicleActivity;
	SFile << bClubBattleZone;				

	SFile << strMapName;
	SFile << strBGM;
	SFile << strLoadingImageName;

	SFile << bInstantMap;
	SFile << bQBoxEnable;

	SFile << bLunchBoxForbid;

	/*combatpoint logic, Juver, 2017/05/29 */
	SFile << bCPReset; 
	SFile << bCPResetDeath; 

	SFile << bPKMap;

	SFile << bUIMapSelect;
	SFile << bUIMapInfo;
	SFile << bClubPKRecord;

	/* map private market setting, Juver, 2017/10/02 */
	SFile << bOpenPrivateMarket; 

	SFile << bPartySparring;

	/*map move settings, Juver, 2017/11/25 */
	SFile << bBlockTaxi;
	SFile << bBlockFriendCard;
	SFile << bBlockRecall;
	SFile << bBlockTeleport;

	/*global buffs map setting, Juver, 2018/01/23 */
	SFile << bDisableSystemBuffs;

	/*hp potion map setting, Juver, 2018/01/23 */
	SFile << bBlockHPPotion;

	/*map party setting, Juver, 2018/06/29 */
	SFile << bBlockParty;

	/*force revive, Juver, 2018/07/09 */
	SFile << bForceRevive;

	/* open pk function, Juver, 2019/12/23 */
	SFile << bOpenPK;

	/* death hide, Juver, 2020/02/02 */
	SFile << bDeathHide;

	/* map entry user verified, Juver, 2020/02/27 */
	SFile << bUserVerifiedMapEntry;

	/* costume combine map restriction, Juver, 2020/09/11 */
	SFile << bCostumeCombineRestrictMoveSpeed;
	SFile << bCostumeCombineRestrictAttackSpeed;

	/* trade restricted, Juver, 2021/06/07 */
	SFile << bTradeRestricted;

	SFile << bAllowHideDead;

	SFile << bShowPkMark;

	SFile << bUseAutoPotion;
	SFile << bAutoPotionManual;

	SFile << bUseAutoPilot;

	SFile << bPKMapCount;

	SFile << bUseCaptcha;

	SFile << bBlockGeneralChat;

	SFile << bResetSkillDelayOnDie;
	
	/* Boss Spawn Viewer, Review000 */
	SFile << bBossMap;

	return TRUE;
}


VOID SMAPNODE::SaveCsvHead ( std::fstream &SFile )
{
	SFile<< "MID"									<< ",";
	SFile<< "SID"									<< ",";

	SFile<< "MapName"								<< ",";
	SFile<< "Used"									<< ",";
	SFile<< "FileName"								<< ",";
	SFile<< "FieldServer"							<< ",";
	SFile<< "Bgm"									<< ",";
	SFile<< "LoadingImg"							<< ",";

	SFile<< "bPeaceZone"							<< ",";
	SFile<< "bCommission"							<< ",";
	SFile<< "bPKZone"								<< ",";
	SFile<< "bFreePK"								<< ",";
	SFile<< "bItemDrop"								<< ",";
	SFile<< "bMove"									<< ",";
	SFile<< "bRestart"								<< ",";
	SFile<< "bPetActivity"							<< ",";
	SFile<< "bDECEXP"								<< ",";
	SFile<< "bVehicleActivity"						<< ",";
	SFile<< "bClubBattleZone"						<< ",";
	SFile<< "bInstantMap"							<< ",";
	SFile<< "bQBoxEnable"							<< ",";

	SFile<< "bLunchBoxForbid"						<< ",";
	SFile<< "bCPReset"								<< ",";
	SFile<< "bCPResetDeath"							<< ",";
	SFile<< "bPKMap"								<< ",";
	SFile<< "bUIMapSelect"							<< ",";
	SFile<< "bUIMapInfo"							<< ",";
	SFile<< "bClubPKRecord"							<< ",";
	SFile<< "bOpenPrivateMarket"					<< ",";
	SFile<< "bPartySparring"						<< ",";
	SFile<< "bBlockTaxi"							<< ",";
	SFile<< "bBlockFriendCard"						<< ",";
	SFile<< "bBlockRecall"							<< ",";
	SFile<< "bBlockTeleport"						<< ",";
	SFile<< "bDisableSystemBuffs"					<< ",";
	SFile<< "bBlockHPPotion"						<< ",";
	SFile<< "bBlockParty"							<< ",";
	SFile<< "bForceRevive"							<< ",";
	SFile<< "bOpenPK"								<< ",";
	SFile<< "bDeathHide"							<< ",";
	SFile<< "bUserVerifiedMapEntry"					<< ",";
	SFile<< "bCostumeCombineRestrictMoveSpeed"		<< ",";
	SFile<< "bCostumeCombineRestrictAttackSpeed"	<< ",";
	SFile<< "bTradeRestricted"						<< ",";
	SFile<< "bAllowHideDead"						<< ",";
	SFile<< "bShowPkMark"							<< ",";
	SFile<< "bUseAutoPotion"						<< ",";
	SFile<< "bAutoPotionManual"						<< ",";
	SFile<< "bUseAutoPilot"							<< ",";
	SFile<< "bPKMapCount"							<< ",";
	SFile<< "bUseCaptcha"							<< ",";
	SFile<< "bBlockGeneralChat"						<< ",";
	SFile<< "bResetSkillDelayOnDie"					<< ",";
	SFile<< "bBossMap"								<< ",";

	SFile << std::endl;
}

VOID SMAPNODE::SaveCsv ( std::fstream &SFile )	const
{
	SFile << sNativeID.wMainID << ",";
	SFile << sNativeID.wSubID  << ",";

	STRUTIL::OutputStrCsv( SFile, std::string(strMapName) );

	SFile << ((INT)bUsed) << ",";

	STRUTIL::OutputStrCsv( SFile, std::string(strFile) );

	SFile << dwFieldSID << ",";

	STRUTIL::OutputStrCsv( SFile, std::string(strBGM) );

	STRUTIL::OutputStrCsv( SFile, std::string(strLoadingImageName) );

	SFile << ((INT)bPeaceZone)								<< ",";
	SFile << ((INT)!bCommission)							<< ",";
	SFile << ((INT)bPKZone)									<< ",";
	SFile << ((INT)bFreePK)									<< ",";
	SFile << ((INT)bItemDrop)								<< ",";
	SFile << ((INT)bMove)									<< ",";
	SFile << ((INT)bRestart)								<< ",";
	SFile << ((INT)bPetActivity)							<< ",";
	SFile << ((INT)bDECEXP)									<< ",";
	SFile << ((INT)bVehicleActivity)						<< ",";
	SFile << ((INT)bClubBattleZone)							<< ",";
	SFile << ((INT)bInstantMap)								<< ",";
	SFile << ((INT)bQBoxEnable)								<< ",";
	SFile << ((INT)bLunchBoxForbid)							<< ",";
	SFile << ((INT)bCPReset)								<< ",";
	SFile << ((INT)bCPResetDeath)							<< ",";
	SFile << ((INT)bPKMap)									<< ",";
	SFile << ((INT)bUIMapSelect)							<< ",";
	SFile << ((INT)bUIMapInfo)								<< ",";
	SFile << ((INT)bClubPKRecord)							<< ",";
	SFile << ((INT)bOpenPrivateMarket)						<< ",";
	SFile << ((INT)bPartySparring)							<< ",";
	SFile << ((INT)bBlockTaxi)								<< ",";
	SFile << ((INT)bBlockFriendCard)						<< ",";
	SFile << ((INT)bBlockRecall)							<< ",";
	SFile << ((INT)bBlockTeleport)							<< ",";
	SFile << ((INT)bDisableSystemBuffs)						<< ",";
	SFile << ((INT)bBlockHPPotion)							<< ",";
	SFile << ((INT)bBlockParty)								<< ",";
	SFile << ((INT)bForceRevive)							<< ",";
	SFile << ((INT)bOpenPK)									<< ",";
	SFile << ((INT)bDeathHide)								<< ",";
	SFile << ((INT)bUserVerifiedMapEntry)					<< ",";
	SFile << ((INT)bCostumeCombineRestrictMoveSpeed)		<< ",";
	SFile << ((INT)bCostumeCombineRestrictAttackSpeed)		<< ",";
	SFile << ((INT)bTradeRestricted)						<< ",";
	SFile << ((INT)bAllowHideDead)							<< ",";
	SFile << ((INT)bShowPkMark)								<< ",";
	SFile << ((INT)bUseAutoPotion)							<< ",";
	SFile << ((INT)bAutoPotionManual)						<< ",";
	SFile << ((INT)bUseAutoPilot)							<< ",";
	SFile << ((INT)bPKMapCount)								<< ",";
	SFile << ((INT)bUseCaptcha)								<< ",";
	SFile << ((INT)bBlockGeneralChat)						<< ",";
	SFile << ((INT)bResetSkillDelayOnDie)					<< ",";
	SFile << ((INT)bBossMap)								<< ",";
	
	SFile << std::endl;
}

VOID SMAPNODE::LoadCsv ( CStringArray &StrArray )
{
	int iCsvCur = 0;


	std::string strTemp;

	sNativeID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
	sNativeID.wSubID  = (WORD)atoi( StrArray[ iCsvCur++ ] );

	STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTemp );
	strMapName = strTemp;

	bUsed = (0!=atoi( StrArray[ iCsvCur++ ] ));

	STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTemp );
	strFile = strTemp;

	dwFieldSID = (DWORD)atoi( StrArray[ iCsvCur++ ] );

	STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTemp );
	strBGM = strTemp;

	STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strTemp );
	strLoadingImageName = strTemp;

	bPeaceZone							= (0!=atoi( StrArray[ iCsvCur++ ] ));

	// lets not confuse them, substituting them in reverse
	bCommission							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bCommission = !bCommission;

	bPKZone								= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bFreePK								= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bItemDrop							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bMove								= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bRestart							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bPetActivity						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bDECEXP								= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bVehicleActivity					= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bClubBattleZone						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bInstantMap							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bQBoxEnable							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bLunchBoxForbid						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bCPReset							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bCPResetDeath						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bPKMap								= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bUIMapSelect						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bUIMapInfo							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bClubPKRecord						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bOpenPrivateMarket					= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bPartySparring						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bBlockTaxi							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bBlockFriendCard					= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bBlockRecall						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bBlockTeleport						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bDisableSystemBuffs					= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bBlockHPPotion						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bBlockParty							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bForceRevive						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bOpenPK								= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bDeathHide							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bUserVerifiedMapEntry				= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bCostumeCombineRestrictMoveSpeed	= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bCostumeCombineRestrictAttackSpeed	= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bTradeRestricted					= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bAllowHideDead						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bShowPkMark							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bUseAutoPotion						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bAutoPotionManual					= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bUseAutoPilot						= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bPKMapCount							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bUseCaptcha							= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bBlockGeneralChat					= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bResetSkillDelayOnDie				= (0!=atoi( StrArray[ iCsvCur++ ] ));
	bBossMap							= (0!=atoi( StrArray[ iCsvCur++ ] ));
}