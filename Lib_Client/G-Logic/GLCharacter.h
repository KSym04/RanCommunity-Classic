#pragma once

#include "./GLCharDefine.h"
#include "./GLCharData.h"
#include "./GLogicData.h"
#include "./GLCharLogicClient.h"
#include "./GLCOPY.h"

#include "./GLContrlMsg.h"
#include "./gltradeclient.h"
#include "./GLPrivateMarket.h"
#include "./GLCLUBMAN.h"

#include "../../Lib_Engine/NaviMesh/Actor.h"
#include "../../Lib_Engine/Meshs/DxSkinChar.h"
#include "../../Lib_Network/s_NetGlobal.h"

#include "../NpcTalk/NpcTalkData.h"

/*skill summon, Juver, 2017/10/08 */
#include "./GLSummonDefine.h"

#include "../../Lib_Helper/cMtSafeVar.h"

#include "./GLMaxRvPreview.h"

/*skill activated effect, Juver, 2017/12/29 */
class DxEffSingleGroup;

/*npc shop, Juver, 2017/07/27 */
#define NPC_SHOP_DELAY			0.0f //Removed Delay from 2.0 to 0.0f - jxyy

/*item exchange, Juver, 2017/10/13 */
#define NPC_ITEM_EXCHANGE_DELAY	1.0f

/*extreme auto tab, Juver, 2018/09/10 */
#define SKILL_CHANGE_WEAPON_TAB_PENDING_TIME 5.0f

#define NPC_CONTRIBUTION_SHOP_DELAY 2.0f
/*12-9-14, Battle Pass - CNDev*/
#define BATTLEPASS_DELAY 2.0f
/*12-16-24, daily login - CNDev*/
#define DAILYLOGIN_DELAY 2.0f
enum EMREACTION
{
	REACT_MOB		= 0,
	REACT_NPC		= 1,
	REACT_P2P		= 2,
	REACT_PVP		= 3,
	REACT_ITEM		= 4,
	REACT_SKILL		= 5,
	REACT_GATHERING	= 6,
	REACT_TOGGLE_VEHICLE	= 7,
	REACT_SWAP_WEAPON		= 8,
	REACT_SIZE		= 9
};

namespace COMMENT
{
	extern std::string szEMREACTION[REACT_SIZE+1];
};

struct SREACTION
{
	EMREACTION		emREACTION;	//	예약행위.

	bool			bCONTINUE;	//	연속 수행.

	D3DXVECTOR3		vMARK_POS;	//	선행위치.

	STARGETID		sTARID;		//	피행위자.

	SREACTION () :
		emREACTION(REACT_SIZE),
		bCONTINUE(false),

		vMARK_POS(FLT_MAX,FLT_MAX,FLT_MAX)
	{
	}

	void RESET ()
	{
		vMARK_POS = D3DXVECTOR3(FLT_MAX,FLT_MAX,FLT_MAX);
		emREACTION = REACT_SIZE;
	}

	bool ISVALID ()		{ return (emREACTION!=REACT_SIZE); }
};

//	Note : 엑션 행위 타겟들.. ( 우선순위 순으로 정렬. )
//
enum EMACTIONTAR
{
	EMACTAR_NULL	= 0,
	
	EMACTAR_PC_PVP	= 1,
	EMACTAR_PC_P2P	= 2,
	
	EMACTAR_MOB		= 3,
	EMACTAR_NPC		= 4,
	
	EMACTAR_PC_OUR	= 5,
	EMACTAR_PC_ANY	= 6,

	EMACTAR_ITEM	= 7,

	EMACTAR_SUMMON_ATTACK	= 8,
	EMACTAR_MATERIAL	= 9, 

	/* crow zone, Juver, 2018/02/21 */
	EMACTAR_ZONE_NAME	= 10,
	EMACTAR_GATE_NAME	= 11,
};

enum EMFIND_TAR
{
	ENFIND_TAR_NULL		= 0,
	EMFIND_TAR_ANY		= 1,
	EMFIND_TAR_ENEMY	= 2,
	EMFIND_TAR_OUR		= 3,
	EMFIND_TAR_OUR_DIE	= 4,
	EMFIND_TAR_ANY_DIE	= 5,
};

struct SACTIONTAR
{
	EMACTIONTAR		emACTAR;
	STARGETID		sTARID;

	SACTIONTAR () :
		emACTAR(EMACTAR_NULL)
	{
	}
};

class GLCharClient;
class GLAutoPotion;
class GLAutoPilotC;

typedef std::vector<USER_ATTEND_INFO>		VECATTEND;

class GLCharacter : public GLCHARLOGIC_CLIENT, public GLCOPY
{

protected:
	const static float	m_fELAPS_MOVE;

	enum { EMWAIT_COUNT = 10 };

protected:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	Actor				m_actorMove;
	DxSkinChar*			m_pSkinChar;

	D3DXVECTOR3			m_vDir;
	D3DXVECTOR3			m_vDirOrig;
	D3DXVECTOR3			m_vPos;
	D3DXMATRIX			m_matTrans;

	D3DXVECTOR3			m_vServerPos;

	//	Note : 행동.
	//
protected:
	EMACTIONTYPE		m_Action;				//	현제 액션.
	DWORD				m_dwActState;			//	행동 상태.
	
	STARGETID			m_sTargetID;
	
	/*multi skill Juver, 2018/09/04 */
	WORD				m_wTARGET_NUM_MAIN;
	WORD				m_wTARGET_NUM_SUB;
	D3DXVECTOR3			m_vTARGET_POS_MAIN;
	D3DXVECTOR3			m_vTARGET_POS_SUB;
	STARID				m_sTARGET_IDS_MAIN[EMTARGET_NET];
	STARID				m_sTARGET_IDS_SUB[EMTARGET_NET];

	TARGETMAP			m_vecTAR_TEMP;

	SNATIVEID			m_sRunSkill;				// 선택한 스킬
	SNATIVEID			m_sActiveSkill;				// 구동중인 스킬

	DWORD				m_dwANISUBCOUNT;
	DWORD				m_dwANISUBSELECT;

	DWORD				m_dwANISUBGESTURE;

	EMITEM_QUESTION		m_emOldQuestionType;	//	효과 생성/종료용으로 직전의 타입을 저장하고 있음.

	/*dmk14 pk safetime*/
	//float				m_fMoveDelay;

//public:
//	BOOL IsSafeTime () const						{ return m_fMoveDelay < GLCONST_CHAR::fPK_SAFE_TIME; }
//	void ResetSafeTime()							{ m_fMoveDelay = 0.0f; }
//
protected:
	float				m_fLastMsgMoveSend;
	GLMSG::SNETPC_GOTO	m_sLastMsgMove;
	GLMSG::SNETPC_GOTO	m_sLastMsgMoveSend;

	SREACTION			m_sREACTION;

protected:
	float				m_fIdleTime;
	float				m_fattTIMER;
	int					m_nattSTEP;
	float				m_fAccumTime;

	DWORD				m_dwWAIT;

public:
	DWORD				m_dwGaeaID;

protected:
	DWORD				m_dwNumStorageItem[EMSTORAGE_CHANNEL];	//	창고에 들어있는 아이템 갯수.
	SCONFTING_CLT		m_sCONFTING;							//	진행 대련 정보.

public:
	WORD				m_wPMPosX;								//	개인상점 개설 허가권.
	WORD				m_wPMPosY;
	GLPrivateMarket		m_sPMarket;								//	개인 상점.

	std::vector< DWORD > m_vecMarketClick;						//	클릭한 개인상점 리스트

	BOOL				m_bCLUB_CERTIFY;						//	선도 클럽 결정전 인증 유무.
	float				m_fCLUB_CERTIFY_TIMER;					//	선도 클럽 결정전 인증 시간.
	GLCLUB				m_sCLUB;								//	클럽 정보.

public:
	WORD				m_wInvenPosX1;				// Temp
	WORD				m_wInvenPosY1;
	WORD				m_wInvenPosX2;				// Temp
	WORD				m_wInvenPosY2;

	/*item wrapper, Juver, 2018/01/12 */
	WORD				m_wInvenPosX3;				// Temp
	WORD				m_wInvenPosY3;

	SITEMCUSTOM			m_sTempItemCustom;

	/*skill summon, Juver, 2017/10/08 */
	DWORD				m_dwSummonGUID[SKILL_SUMMON_MAX_CLIENT_NUM];					//  소환수 ID

	HMODULE m_hCheckStrDLL;
	BOOL (_stdcall *m_pCheckString)(CString);

	// Note : 헤어샾에서 머리를 바꿀 경우 임시로 데이터를 가지고 있음.
private:
	BOOL			m_bEnableHairSytle;
	BOOL			m_bEnableHairColor;
	BOOL			m_bEnableFaceStyle;
	WORD			m_wHairColorTEMP;
	WORD			m_wHairStyleTEMP;
	WORD			m_wFaceStyleTEMP;

	BOOL			m_bAttackable;							// 공격가능유무
	BOOL			m_bMiniGameOpen;

	/*npc shop, Juver, 2017/07/27 */
	SNATIVEID		m_sNPCShopCrow;
	SNATIVEID		m_sNPCShopItem;
	WORD			m_wNPCShopType;
	float			m_fNPCShopDelay;

	/*item exchange, Juver, 2017/10/13 */
	float			m_fNPCItemExchangeDelay;

	/*12-9-14, Battle Pass - CNDev*/
	float			m_fBattlePassDelay;

	/*12-16-24, daily login - CNDev*/
	bool			m_bDailyLoginRemain;
	float			m_fDailyLoginDelay;
public:
	/*product item, Juver, 2017/10/18 */
	DWORD			m_dwProductID;
	BOOL			m_bItemCompoundTask;
	DWORD			m_dwItemCompoundTaskID;
	float			m_fItemCompoundTaskTime;
	float			m_fItemCompoundTaskTimer;

	/*activity system, Juver, 2017/11/05 */
	float			m_fCharTitleDelay;

	/*pvp capture the flag, Juver, 2018/02/07 */
	float			m_fCaptureTheFlagHoldFlagTimer;
	BOOL			m_bCaptureTheFlagLocatorOn;
	/*12-16-24, daily login - CNDev*/
	float			m_fDailyLoginRemain;

	/*modern inventory window, Juver, 2018/04/09 */
	WORD			m_wInventoryUsePosX;				// Temp
	WORD			m_wInventoryUsePosY;
	BOOL			m_bGarbageUseCard;

	SNATIVEID		m_sNPCContributionShopCrow;
	SNATIVEID		m_sNPCContributionShopItem;
	float			m_fNPCContributionShopDelay;

	float			m_fMaxRVPreviewTimer;

	GLAutoPotion*	m_pAutoPotion;
	GLAutoPilotC*	m_pAutoPilot;

	bool			m_bExchangeItemOpen;
	SINVEN_POS		m_sExchangeItemInvenPos;	

public:
	BOOL IsOpenMiniGame()			{ return m_bMiniGameOpen; }
	VOID OpenMiniGame()				{ m_bMiniGameOpen = TRUE; }
	VOID CloseMiniGame()			{ m_bMiniGameOpen = FALSE; }

public:
	void HairStyleInitData();
	void HairStyleChange( WORD wStyle );
	void HairStyleEnd()						{ m_bEnableHairSytle = FALSE; }
	
	void HairColorInitData();
	void HairColorChange( WORD wColor );
	void HairColorEnd()						{ m_bEnableHairColor = FALSE; }

	void FaceStyleInitData();
	void FaceStyleChange( WORD wStyle );
	void FaceStyleEnd()						{ m_bEnableFaceStyle = FALSE; }

private:
	void HairStyleUpdate();
	void HairColorUpdate();
	void FaceStyleUpdate();

public:
	BOOL IsMarketClick( DWORD dwGaeaID );
	void InsertMarketClick( DWORD dwGaeaID );

public:
	//	Note : AABB
	//
	D3DXVECTOR3		m_vMaxOrg;
	D3DXVECTOR3		m_vMinOrg;

public:
	D3DXVECTOR3		m_vMax;
	D3DXVECTOR3		m_vMin;
	float			m_fHeight;

protected:
	//	수동적 행위 반복 체크 변수,
	//		수동적행위를 했는가? BOOL
	//		최근에 수동적 행위를 한뒤 경과시간.
	//		수동적 행위 총 경과 시간.
	//
// 	BOOL				m_bPASSIVITY_ACTION;
// 	float				m_fPASSIVITY_ACTION_ELAPS;
// 	float				m_fPASSIVITY_ACTION_TOTAL;
// 	BOOL				m_bPASSIVITY_ACTION_CHECK;
// 	float				m_fPASSIVITY_ACTION_CHECK;

// 	float				m_fKeyDownCheckTime;
// 	bool				m_bOneHourNotInputKey;

public:
	DWORD				m_dwNPCID;								//  가장 마지막에 클릭한 NPC ID

public:
	// PET
	LONGLONG		m_llPetCardGenNum;							// 활성화된 팻카드의 GenNum
	SNATIVEID		m_sPetCardNativeID;							// 활성화된 팻카드의 Nativeid
	BYTE			m_cPetCardGenType;							// 활성화된 팻카드의 GenType

	PETCARDINFO_MAP			m_mapPETCardInfo;					// 팻카드 정보 (팻의 상태가 변경될때마다 같이 변경해줘야함)
	PETCARDINFO_MAP			m_mapPETCardInfoTemp;				// 팻카드 정보 (거래시 사용)
	PETREVIVEINFO_MAP		m_mapPETReviveInfo;					// 부활시키기 위한 팻의 정보

	BOOL					m_bRecievedPetCardInfo;				// 팻카드 정보 수신여부
	BOOL					m_bIsPetActive;						// 맵을 이동할때 팻이 활동중이었는지 여부

public:
	VEHICLEITEMINFO_MAP			m_mapVEHICLEItemInfo;					// 탈것 아이템의 정보 ( 탈것 상태 변경 )
	VEHICLEITEMINFO_MAP			m_mapVEHICLEItemInfoTemp;				// 탈것 아이템의 정보 ( 거래시 사용 )

	GLVEHICLE		m_sVehicle;				// 탈것 구조체
	BOOL			m_bReqVehicle;			// 보드 탑승 및 해제 중이라면...
	BOOL			m_bIsVehicleActive;		// 맵 이동중 탈것 타고 있었다면...
	BOOL			m_bRecivedVehicleItemInfo;  // 탈것 정보 수신 여부

	bool			m_bOldVisibleTracingUI[8];
	bool			m_bNewVisibleTracingUI[8];

	float			m_fPrintProcessInfoMsgGap;



	bool			m_bGarbageOpen;
	SINVEN_POS		m_sGarbageItem[ITEM_GARBAGE_MAX_SLOT];			// 휴지통에 등록된 아이템

	bool			m_bItemShopOpen;		// ItemShopOpen ( Japan ) 
	
	bool			m_bReqAttendList;		// 출석 리스트
	VECATTEND		m_vecAttend;			// 출석 리스트
	DWORD			m_dwComboAttend;		// 연속 출석 일수
	__time64_t		m_tAttendLogin;			// 로그인시간 ( 출석부에만 사용 ) 
	DWORD			m_dwAttendTime;			// 출석 가능한 게임 접속 시간

	CString			m_strRecordChat;
	bool			m_bRecordChat;
	CTime			m_recordStartTime;

	SINVEN_POS		m_sItemMixPos[ITEMMIX_ITEMNUM];

	/*dash skill logic, Juver, 2017/06/17 */
	DWORD			m_dwActionAnim;

	/*game stats, Juver, 2017/06/21 */
	clock_t			m_clock_Packet_field;
	clock_t			m_clock_Packet_agent;
	bool			m_bRecievedPingAgent;
	bool			m_bRecievedPingField;

	/*rightclick wear/unwear, Juver, 2017/06/23 */
	bool			m_bRingSlotFirst;
	float			m_fWearDelay;

	/*pet fix add call delay, EJCode, 2018/11/28 */
	float			m_fPetUseDelay;

	/*charinfoview , Juver, 2017/11/11 */
	float			m_fReqCharInfoTimer;

	/*bike color , Juver, 2017/11/16 */
	WORD			m_wBikeColor[BIKE_COLOR_SLOT_PART_SIZE];
	BOOL			m_bEnableBikeColor;

	/*change scale card, Juver, 2018/01/03 */
	float			m_fActiveCharScale;
	BOOL			m_bChangeCharScale;

	/*item color, Juver, 2018/01/10 */
	WORD			m_wItemColor1;
	WORD			m_wItemColor2;
	BOOL			m_bEnableItemColor;
	EMSLOT			m_emSlotItemColor;

	/*item transfer card, Juver, 2018/01/18 */
	SINVEN_POS		m_sItemTransferPosOld;
	SINVEN_POS		m_sItemTransferPosNew;

	/* car, cart color, Juver, 2018/02/14 */
	WORD			m_wCarColorA;
	WORD			m_wCarColorB;
	BOOL			m_bEnableCarColor;

	/*ep8 puton slots, Juver, 2018/03/29 */
	bool			m_bAccessorySlotFirst;

	/*item random option rebuild, Juver, 2018/07/03 */
	SINVEN_POS		m_sItemRandomOptionRebuildItem;
	SINVEN_POS		m_sItemRandomOptionRebuildStamp;

	BOOL			m_bItemRandomOptionRebuildOptionLock0;
	BOOL			m_bItemRandomOptionRebuildOptionLock1;
	BOOL			m_bItemRandomOptionRebuildOptionLock2;
	BOOL			m_bItemRandomOptionRebuildOptionLock3;

	/*inventory sort, Juver, 2018/07/07 */
	float			m_fInventorySortTimer;

	/*buffs manual remove, Juver, 2018/08/14 */
	SNATIVEID		m_buff_remove_id;

	/*item drop question, Juver, 2018/08/22 */
	D3DXVECTOR3		m_vdrop_pos_temp;

	/*item bank question, Juver, 2018/08/22 */
	WORD			m_item_bank_pos_x;
	WORD			m_item_bank_pos_y;

	/*specific item box, Juver, 2018/09/03 */
	WORD			m_item_box_pos_x;
	WORD			m_item_box_pos_y;

	/*extreme auto tab, Juver, 2018/09/10 */
	BOOL			m_skill_change_weapon_tab_pending;
	float			m_skill_change_weapon_tab_pending_time;
	GLMSG::SNETPC_REQ_SKILL	m_skill_change_weapon_tab_pending_skill;

	/*manual lunchbox remove, Juver, 2018/09/19 */
	SNATIVEID		m_lunchbox_remove_id;

	/*item random option rebuild, Juver, 2018/07/03 */
	float			m_item_random_option_rebuild_timer;
	BOOL			m_item_random_option_rebuild_active_task;

	///*hide costume, EJCode, 2018/11/18 */
	//bool			m_hide_costume;
	//float			m_hide_costume_timer;

	float			m_fPetLastUseTime;
	float			m_fPetBlockTime;

	float			m_fCurrencyShopDelayTimer;

	/* party finder, Juver, 2020/01/03 */
	float			m_fPartyFinderSearchDelayTimer;
	float			m_fPartyFinderJoinDelayTimer;

	/* skill turn/find invisible, Juver, 2020/04/26 */
	bool			m_bSkillTurnInvisibleLast;
	WORD			m_wSkillTurnInvisibleAnimationLast;
	SNATIVEID		m_sSkillTurnInvisibleIDLast;

	/* chaos machine, Juver, 2021/07/09 */
	SINVEN_POS		m_sChaosMachineItemPos;
	DWORD			m_dwChaosMachineNPCID;

	/* tyranny AFK disconnect, Juver, 2021/08/07 */
	float			m_fTyrannyAFKTimer;

	float			m_fCaptureTheFlagAFKTimer;

	SMAXRV_PREVIEW	m_sMaxRvPreview;

	float			m_fCaptchaWaitTimer;
	bool			m_bCaptchaWait;

public:
	//void DoPASSIVITY ( BOOL bPASSIVITY );
	//bool CheckPASSIVITY ( float fElaps );
	//void ResetPASSIVITY ();
	//bool IsBlockPASSIVITY ();

	void UpdateSpecialSkill();
public:
	BOOL ISCONFRONT_TAR ( const STARGETID &sTargetID );
	void DELCONFRONT_MEMBER ( DWORD dwGaeaID );

public:
	BOOL IsReActionable ( const STARGETID &sTargetID, BOOL bENEMY=TRUE );

public:
	BOOL IsSTATE ( DWORD dwState )		 			{ return m_dwActState&dwState; }
	
	virtual BOOL IsACTION ( EMACTIONTYPE emCur )	{ return m_Action == emCur; }

public:
	void SetSTATE ( DWORD dwState )					{ m_dwActState |= dwState; }
	void ReSetSTATE ( DWORD dwState )				{ m_dwActState &= ~dwState; }

public:
	BOOL IsCtrlBlockBody ();
	BOOL IsValidBody ();
	BOOL IsDie ();
	BOOL IsRunning ()								{ return IsACTION(GLAT_MOVE) && IsSTATE(EM_ACT_RUN); };
	BOOL IsRunMode ()								{ return IsSTATE(EM_ACT_RUN); };

	virtual BOOL IsPartyMaster ();
	BOOL IsPartyMem ();

	BOOL IsClubMaster ();
	BOOL IsCDCertify ();

protected:
	void TurnAction ( EMACTIONTYPE toAction );

public:
	void ResetAction() { TurnAction( GLAT_IDLE ); }

public:
	float GetMoveVelo ();
	BOOL ActionMoveTo ( float fTime, D3DXVECTOR3 vFromPt, D3DXVECTOR3 vTargetPt, BOOL bContinue=FALSE, BOOL bREACT=FALSE );
	BOOL LargeMapMoveTo( D3DXVECTOR3* pPos );
	BOOL LargeMapTeleportTo( WORD wX, WORD wY );

public:
	Actor& GetActorMove ()						{ return m_actorMove; }
	DxSkinChar* GetSkinChar ()					{ return m_pSkinChar; }

public:
	HRESULT SetPosition ( D3DXVECTOR3 vPos );
	virtual const D3DXVECTOR3 &GetPosition ()	{ return m_vPos; }
	float GetDirection ();
	D3DXVECTOR3 GetDirectionVector () { return m_vDir; }

public:
	SCHARDATA2& GetCharData ()			{ return (*this); }
	GLCHARLOGIC_CLIENT& GetCharLogic ()	{ return (*this); }
	GLTradeClient& GetTradeClient ()	{ return GLTradeClient::GetInstance(); }
	SNATIVEID GetskillRunSlot ()		{ return m_sRunSkill; }
	SNATIVEID GetActiveSkillRun()		{ return m_sActiveSkill; }
	SCONFTING_CLT& GetConfting ()		{ return m_sCONFTING; }
	float GetBuyRate ();
	float GetSaleRate ();

public:
	const SITEMCUSTOM& GET_PRETRADE_ITEM ();
	BOOL IsVALID_STORAGE ( DWORD dwChannel );

	STARGETID GetTargetID () { return m_sTargetID; }


public:
	DWORD DetectGate ();
	void DetectGateToMapName ( std::vector<std::string> &vecString );

	HRESULT UpdateSuit( BOOL bChangeHair = TRUE, BOOL bChangeFace = TRUE );

public:
	virtual EMCROW GetCrow () const			{ return CROW_PC; }
	virtual DWORD GetCtrlID () const		{ return m_dwGaeaID; }
	virtual DWORD GetCharID () const		{ return m_dwCharID; }

	virtual const char* GetName () const	{ return m_szName; }
	virtual GLDWDATA GetHp () const			{ return m_sHP; }

	virtual WORD GetBodyRadius ();
	virtual float GetBodyHeight ()			{ return m_fHeight; }
	virtual D3DXVECTOR3 GetPosBodyHeight ();
	virtual void GetSkinAABB( D3DXVECTOR3& vMin, D3DXVECTOR3& vMax );
	virtual float GetSkinScale();
	virtual float GetSkinHeight();

	virtual bool IsSafeZone () const { return m_bSafeZone; }
	bool IsSafeTime () const { return m_bSafeTime; } /* SafeTime Logic - Montage, 3-28-25 */

protected:
	BOOL RECEIVE_SKILLFACT ( const SNATIVEID skill_id, const WORD wlevel, const DWORD dwSELECT, WORD _wCasterCrow, DWORD _dwCasterID );

	virtual void ReceiveDamage ( WORD wDamage, DWORD dwDamageFlag, STARGETID sACTOR );
	virtual void ReceiveAVoid ();
	virtual void ReceiveSwing ();
	void ReceiveShock ();

	void PrintTracingUserWindowInfo();
	void PrintTracingUserProcessInfo( float fElapsedTime );

public:
	virtual void MsgProcess( NET_MSG_GENERIC* nmg );


	void MsgProcessMiniGame( NET_MSG_GENERIC* nmg );
	void MsgProcessAlliance( NET_MSG_GENERIC* nmg );
	void MsgProcessClub( NET_MSG_GENERIC* nmg );
	void MsgProcessQuest( NET_MSG_GENERIC* nmg );
	void MsgProcessConfront( NET_MSG_GENERIC* nmg );

	void MsgProcessClubBattle( NET_MSG_GENERIC* nmg );
	void MsgProcessAllianceBattle( NET_MSG_GENERIC* nmg );

	void MsgDefenseSkillActive( GLMSG::SNETPC_DEFENSE_SKILL_ACTIVE* nmg );

	bool IsInsertToInven ( PITEMCLIENTDROP pItemDrop );

public:
	DWORD GetAmountActionQ ( WORD wSLOT );

public:
	bool IsInvenSplitItem ( WORD wPosX, WORD wPosY, bool bVietnamInven = FALSE );
	bool IsStorageSplitItem ( DWORD dwChannel, WORD wPosX, WORD wPosY );
	bool IsClubStorageSplitItem ( DWORD dwChannel, WORD wPosX, WORD wPosY );
	bool IsNpcPileItem ( SNATIVEID sNID_NPC, DWORD dwChannel, WORD wPosX, WORD wPosY );
	bool IsRestartPossible(); // 이 맵에서 재시작이 되는지

	BOOL IsVaildTradeInvenSpace ();

	bool IsKEEP_CLUB_STORAGE ( DWORD dwCHANNEL );

public:
	void ReqToggleRun ();
	void ReqTogglePeaceMode ();
	void ReqGateOut ( DWORD dwToIndex );

	/*gm command logs, Juver, 2018/08/17 */
	void ReqVisibleNone (BOOL bcmd=FALSE);
	void ReqVisibleOff (BOOL bcmd=FALSE);
	void ReqVisibleOn (BOOL bcmd=FALSE);

	bool ReqGESTURE ( int nMOTION, bool bCOMMAND=false );

	void SET_RECORD_CHAT();
	void UPDATE_RECORD_CHAT( bool bCloseClient = FALSE );

public:
	EMCONFT_TYPE GetConftType ( DWORD dwID );

public:
	void InitAllSkillFact ();

public:
	LONGLONG GetCalcTaxiCharge( int nSelectMap, int nSelectStop );

public:
	// 해킹 재현을 위한 메시지 발송
	void SendHackingMSG ();
	void SendHackingMSG1 ();

public:
	bool IsCoolTime( SNATIVEID sNativeID );
	bool CheckCoolTime( SNATIVEID sNativeID );
	bool SetCoolTime ( ITEM_COOLTIME& sCoolTime, EMCOOL_TYPE emCoolType );
	__time64_t	GetMaxCoolTime ( SNATIVEID sNativeID );

	ITEM_COOLTIME*	GetCoolTime( DWORD dwCoolID, EMCOOL_TYPE emCoolType );
	ITEM_COOLTIME*	GetCoolTime ( SNATIVEID sNativeID );
	
public:
	//	Note : 스킬 배우기 요청. ( 인벤 아이템으로. )
	HRESULT ReqInvenSkill ( WORD wPosX, WORD wPosY );

	//	Note : 스킬 배우기 요청. ( 창고 아이템으로. )
	HRESULT ReqStorageSkill ( DWORD dwChannel, WORD wPosX, WORD wPosY );

	//	Note : 귀환 요청.
	HRESULT ReqReCall ( WORD wPosX, WORD wPosY );

	//	Note : 소환서 사용 요청.
	HRESULT ReqTeleport ( WORD wPosX, WORD wPosY );

	//	Note : 선물상자 열기.
	HRESULT ReqBoxOpen ( WORD wPosX, WORD wPosY );

	//	Note : 랜덤아이탬 박스 열기.
	HRESULT ReqRandumBoxOpen ( WORD wPosX, WORD wPosY );

	//	Note : 랜덤아이탬 박스 열기.
	HRESULT ReqDisJunction ( WORD wPosX, WORD wPosY );

	//	Note : 펫 랜덤 스킨 아이템 열기.
	HRESULT ReqPetSkinPackOpen ( WORD wPosX, WORD wPosY );
 	//itemmall
	HRESULT	ReqRetrievePoints ();
	//	Note : 연마하기.
	HRESULT ReqGrinding ( WORD wPosX, WORD wPosY );

	//	Note : 코스툼 복장과 일반 복장과 결합.
	HRESULT ReqDisguise ( WORD wPosX, WORD wPosY );

	//	Note : 코스툼 복장과 일반 복장과 분리.
	HRESULT ReqCleanser ( WORD wPosX, WORD wPosY );

	//	Note : 케릭터 카드 사용.
	/*self buff via NPC, DevArt22, 2023/03/18 */
	HRESULT ReqSelfBuffBAOnly ( DWORD dwNpcID, DWORD dwGlobalID );
	HRESULT ReqSelfBuffFull ( DWORD dwNpcID, DWORD dwGlobalID );
	HRESULT ReqSelfBuffFullNoBA ( DWORD dwNpcID, DWORD dwGlobalID );
	HRESULT ReqCharCard ( WORD wPosX, WORD wPosY );

	//	Note : 창고 카드 사용.
	HRESULT ReqStorageCard ( WORD wPosX, WORD wPosY, WORD wSTORAGE );

	//	Note : 창고 카드 사용.
	HRESULT ReqInvenLineCard ( WORD wPosX, WORD wPosY );

	//	Note : 창고 연결 카드 사용 요청
	HRESULT ReqStorageOpenCard ( WORD wPosX, WORD wPosY );

	//	Note : 창고 연결 카드 사용 완료
	HRESULT ReqStorageCloseCard();

	//	Note : 프리미엄셋 사용.
	HRESULT ReqPremiumSet ( WORD wPosX, WORD wPosY );
	
	//	Note : 스킬및 스텟 리셋.
	HRESULT ResetSkillStats ( WORD wPosX, WORD wPosY );
	HRESULT ReqResetSkillStats ( );

	// Contribution Item - JX
	HRESULT	ContributionPointCard ( WORD wPosX, WORD wPosY );
	HRESULT ReqContributionPointCard ();

	//	Note : 헤어스타일 변경.
	HRESULT ReqInvenHairChange ( WORD wPosX, WORD wPosY );

	HRESULT ReqInvenHairStyleChange ( WORD wHairStyle );
	HRESULT InvenHairStyleChange ( WORD wPosX, WORD wPosY );

	HRESULT ReqInvenHairColorChange ( WORD wHairColor );
	HRESULT InvenHairColorChange ( WORD wPosX, WORD wPosY );

	//	Note : 얼굴스타일 변경.
	HRESULT ReqInvenFaceChange ( WORD wPosX, WORD wPosY );

	HRESULT ReqInvenFaceStyleChange ( WORD wHairStyle );
	HRESULT InvenFaceStyleChange ( WORD wPosX, WORD wPosY );

	//	Note : 택시카드 사용
	HRESULT InvenUseTaxiCard( WORD wPosX, WORD wPosY );

	//	Note : NPC 소환
	HRESULT InvenUseNpcRecall( WORD wPosX, WORD wPosY );

	// Note : 성별 변경
	HRESULT InvenGenderChange ( WORD wPosX, WORD wPosY );
	HRESULT	ReqInvenGenderChange ( WORD wFace, WORD wHair );

	// Note : 이름 변경.
	HRESULT ReqInvenRename ( const char* szCharName );

	HRESULT InvenRename ( WORD wPosX, WORD wPosY );

	//	Memo : 개조 기능
	HRESULT ReqRemodelOpenCard ( WORD wPosX, WORD wPosY );

	//	Memo : 휴지통 기능
	HRESULT ReqGabargeOpenCard ( WORD wPosX, WORD wPosY );

	//	Note : 베트남 아이템, 경험치 획득 카드 사용
	HRESULT ReqInvenVietnamGet ( WORD wPosX, WORD wPosY, bool bGetExp );

	/* qitem inven, 2024/01/07 */
	HRESULT ReqInvenQItem ( WORD wPosX, WORD wPosY );

	//Item Card Extend Costume Expiration
	HRESULT ReqDisguiseExtend ( WORD wPosX, WORD wPosY );

public:
	//	Note : 거래할 금액 넣기.
	//HRESULT ReqTradeMoney ( LONGLONG lnMoney );

	//	Note : 거래할 아이템 넣기/제거/교환.
	HRESULT ReqTradeBoxTo ( WORD wPosX, WORD wPosY );

	//	Note : 거래 수락.
	HRESULT ReqTradeAgree ();

	//	Note : 거래 취소.
	HRESULT ReqTradeCancel ();

	/*trade lock, Juver, 2018/01/02 */
	HRESULT ReqTradeLock(); 

	//	Note : 대련 요청.
	HRESULT ReqConflict ( DWORD dwID, const SCONFT_OPTION &sOption );

public:
	//	Note : 창고 정보를 서버에 요청.
	HRESULT ReqGetStorage ( DWORD dwChannel, DWORD dwNPCID );
	
	//	Note : 창고 아이템 들때, 놓을때, 교환할때, 합칠때.
	HRESULT ReqStorageTo ( DWORD dwChannel, WORD wPosX, WORD wPosY );

	//	Note : 인벤토리 겹침 아이템 분리.
	HRESULT ReqStorageSplit ( DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wSplitNum );

	//	Note : 창고 아이템 사용할때 ( 마시기, 스킬배우기 등 ).
	HRESULT ReqStorageDrug ( DWORD dwChannel, WORD wPosX, WORD wPosY );

	//	Note : 창고 돈 넣기.
	HRESULT ReqStorageSaveMoney ( LONGLONG lnMoney );

	//	Note : 창고 돈 빼내기.
	HRESULT ReqStorageDrawMoney ( LONGLONG lnMoney );

	//	Note : 상인 Npc에서 물품을 구입하거나 판매시.
	HRESULT ReqNpcTo ( SNATIVEID sNID_NPC, DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wBuyNum=1 );

	//	Note : 구입한 물품 꺼내오기.
	HRESULT ReqChargedItemTo ( WORD wPosX, WORD wPosY );

	//	Note : 구입한 이이템 정보 가져오기
	HRESULT ReqItemBankInfo ();

public:
	//	Note : 필드 아이템(돈) 주을때.
	HRESULT ReqFieldTo ( const STARGETID &sTargetID, bool bPet = false );

	//	Note : 인벤토리 아이템 들때, 놓을때, 교환할때, 합칠때.
	HRESULT ReqInvenTo ( WORD wPosX, WORD wPosY );
	//	Note : 베트남 인벤토리 아이템 들때, 놓을때, 교환할때, 합칠때.
	HRESULT ReqVNInvenTo ( WORD wPosX, WORD wPosY );
	//	Note : 베트남 인벤토리의 아이템 전체 삭제
	HRESULT ReqVNInveReset ();
	//	Note : 베트남 인벤토리에서 오른쪽 버튼으로 아이템을 옮길 경우
	HRESULT ReqVietemInvenTo ( WORD wPosX, WORD wPosY );

	//	Note : 인벤토리 겹침 아이템 분리.
	HRESULT ReqInvenSplit ( WORD wPosX, WORD wPosY, WORD wSplitNum );

	//	Note : 인벤토리 아이템 사용할때 ( 마시기, 스킬배우기 등 ).
	HRESULT ReqInvenDrug ( WORD wPosX, WORD wPosY, BOOL bFromInventory );

	//	Note : 엑션 퀵슬롯에 있는 것을 쓰기. ( 약품일 경우 마시기 ).
	HRESULT ReqActionQ ( WORD wSLOT );

	//	Note : 스킬 렙업 요청.
	HRESULT ReqSkillUp ( const SNATIVEID skill_id );

	//	Note : 사용할 스킬 설정.
	HRESULT ReqSkillRunSet ( const WORD wSLOT );
	HRESULT ReqSkillRunReSet ();

	//	Note : 아이템 슬롯에 등록/제거.
	HRESULT ReqItemQuickSet ( const WORD wSLOT );
	HRESULT ReqItemQuickReSet ( const WORD wSLOT );

	//	Note : 스킬 슬롯에 등록/제거.
	HRESULT ReqSkillQuickSet ( const SNATIVEID skill_id, const WORD wSLOT );
	HRESULT ReqSkillQuickSet ( const SNATIVEID skill_id );
	HRESULT ReqSkillQuickReSet ( const WORD wSLOT );

	//	Note : 아이템을 착용하거나 들어올림.
	HRESULT ReqSlotTo ( EMSLOT emSlot );

	//	Note : 극강부 무기를 메인과 서브 교체.
	HRESULT ReqSlotChange();

	//	Note : 아이템을 구입하거나/팔아버림.
	//	Note : 아이템 바닥에 버림.
	HRESULT ReqHoldToField ( const D3DXVECTOR3 &vPos );
	
	//	Note : 폭죽을 바닥에 사용.
	HRESULT ReqHoldToFieldFireCracker ( const D3DXVECTOR3 &vPos );

	//	Note : 돈 바닥에 버림.
	HRESULT ReqMoneyToField ( LONGLONG lnMoney );

	//	Note : 부활 위치 지정 요청.
	HRESULT ReqReGenGate ( DWORD dwNpcID );

	//	Note : 귀혼주로 부활 요청.
	HRESULT ReqReGenRevive ();

	//  Note : 서버에 경험치 복구 부활을 요청한다.
	HRESULT ReqRecoveryExp ();

	//  Note : 서버에 경험치 복구를 요청한다. (NPC)
	HRESULT ReqRecoveryExpNpc ( DWORD dwNpcID );
	
	//  Note : 복구할 경험치를 묻는다.
	HRESULT ReqGetReExp ();	
	HRESULT ReqGetReExpNpc ( DWORD dwNpcID );	

	//	Note : 치료 요청.
	HRESULT ReqCure ( DWORD dwNpcID, DWORD dwGlobalID );
	HRESULT MsgCureFB ( NET_MSG_GENERIC* nmg );

	//	Note : 케릭터 정보 리셋. ( stats, skill )
	HRESULT ReqCharReset ( DWORD dwNpcID );

	//	Note : NPC와 item을 교환 A:npc에게 주는것, b:npc에게서 받는것.
	HRESULT ReqItemTrade ( DWORD dwNpcID, DWORD dwGlobalID, SNPC_ITEM *pDwA_NID, DWORD dwB_NID );

	//	Note : 친구에게 가기.
	HRESULT	Req2Friend ( const char *szNAME );
	/*dmk14 fast teleport fix*/
	HRESULT	Req2FriendCancel();

	/*dmk14 send notif less data*/
	HRESULT ReqTogglePlayerKillDisp( BOOL bDisplay );
	
	/* Boss Spawn Viewer, Review000 */
	HRESULT ReqBossDetails ( DWORD dwGlobID, SNATIVEID sMapID, SNATIVEID sMobID );
public:
	//	Note : 새로운 퀘스트 시작.
	HRESULT ReqQuestStart ( DWORD dwNpcID, DWORD dwTalkID, DWORD dwQUESTID );

	//	Note : 퀘스트 진행. ( npc와 대화 )
	HRESULT ReqQuestStepNpcTalk ( DWORD dwNpcID, DWORD dwTalkID, DWORD dwQUESTID, DWORD dwQUESTSTEP );

	//	Note : 퀘스트 포기.
	HRESULT ReqQuestGiveUp ( DWORD dwQUESTID );

	//	Note : 퀘스트 읽어보기 요청 ( 읽었음을 응답. )
	HRESULT ReqQuestREADINGReset ( DWORD dwQUESTID );

	//	Note : 퀘스트 완료요청.
	HRESULT ReqQuestComplete ( DWORD dwQUESTID );

	//	Note : 버스 승차 요청.
	HRESULT ReqBusStation ( DWORD dwNpcID, DWORD dwSTATION );

	//	Note : 택시 승차 요청.
	HRESULT ReqTaxiStation ( WORD wPosX, WORD wPosY, int nSelectMap, int nSelectStop );

	//	Note : 확성기 사용 요청.
	/*item link, Juver, 2017/07/31 */
	HRESULT ReqLoudSpeaker ( const char* szChat, SITEMLINK* pItemLink ); 

public:
	//	Note : 개인상점의 타이틀 설정.
	HRESULT ReqPMarketTitle ( const char* szTitle );

	//	Note : 개인상점에 아이템을 등록.
	HRESULT ReqPMarketRegItem ( WORD wPosX, WORD wPosY, LONGLONG llMoney, DWORD dwNum );

	//	Note : 개인상점에 등록된 아이템을 제거.
	HRESULT ReqPMarketDisItem ( WORD wPosX, WORD wPosY );

	//	Note : 개인상점 열기.
	HRESULT ReqPMarketOpen ();

	//	Note : 개인상점 닫기.
	HRESULT ReqPMarketClose ();

	//	Note : 다른 사람의 개인상점의 정보 수신.
	HRESULT ReqPMarketInfo ( DWORD dwGaeaID );
	HRESULT ReqPMarketInfoRelease ( DWORD dwGaeaID );

	//	Note : 다른 사람의 개인 상점에서 물품을 구입.
	HRESULT ReqPMarketBuy ( DWORD dwGaeaID, WORD wPosX, WORD wPosY, DWORD dwNum );

public:
	//	Note : 클럽 생성.
	HRESULT ReqClubNew ( DWORD dwNpcID, const char* szClubName );
	//	Note : 클럽 랭크업.
	HRESULT ReqClubRank ( DWORD dwNpcID );

	//	Note : 클럽 해산.
	HRESULT ReqClubDissolution ();
	//	Note : 클럽 해산 취소.
	HRESULT ReqClubDissolutionCancel ();
	//	Note : 클럽 가입 요청 ( 마스터가 예비 클럽원에게 ).
	HRESULT ReqClubJoin ( DWORD dwGaeaID );
	//	Note : 클럽 가입 요청에 대한 응답하기.
	HRESULT ReqClubJoinAns ( DWORD dwMaster, bool bOK );
	//	Note : 클럽원을 강제 탈퇴시키기.
	HRESULT ReqClubMemberDel ( DWORD dwMember );
	//	Note : 클럽 멤버 탈퇴.
	HRESULT ReqClubSecede ();
	//	Note : 클럽 마스터 권한 위임
	HRESULT ReqClubAuthority ( DWORD dwMember );
	//	Note : 클럽 마스터 권한 위임 답변
	HRESULT ReqClubAuthorityAns ( bool bOK );

	//	Note : 클럽 마크 갱신 요청.
	HRESULT ReqClubMarkInfo ( DWORD dwClubID, DWORD dwMarkVer );

	//	Note : 새 클럽 마크로 변경 요청.
	HRESULT ReqClubMarkChange ( const char* szFileName );
	//	Note : 클럽 멤버 별명.
	HRESULT ReqClubNick ( const char* szNickName );

	//	Note : cd 인증.
	HRESULT ReqCDCertify (DWORD dwNpcID );

	//	Note : 선도지역 수수료 설정.
	HRESULT ReqGuidCommission ( DWORD dwNPCID, float fRATE );

	//	Note : 클럽 공지.
	HRESULT ReqClubNotice ( const char* szClubNotice );

	//	Note : 클럽 부마 설정.
	HRESULT ReqClubSubMaster ( DWORD dwCharID, DWORD dwClubFlag );

	//	Note : 클럽 동맹 요청.
	HRESULT ReqClubAlliance ( DWORD dwGaeaID );

	//	Note : 클럽 동맹 요청 답변.
	HRESULT ReqClubAllianceAns ( DWORD dwChiefCharID, bool bOK );

	//	Note : 클럽 동맹 탈퇴 요청.
	HRESULT ReqClubAllianceSec ();

	//	Note : 클럽 동맹 제명 요청.
	HRESULT ReqClubAllianceDel ( DWORD dwCharID );

	//	Note : 클럽 동맹 해체 요청.
	HRESULT ReqClubAllianceDis ();

	//	Note : 클럽 배틀 요청.
	HRESULT ReqClubBattle ( DWORD dwGaeaID, DWORD dwTime );

	//	Note : 클럽 배틀 요청 답변.
	HRESULT ReqClubBattleAns ( DWORD dwChiefCharID, bool bOK );

	//	Note : 클럽 배틀 휴전 요청.
	HRESULT ReqClubBattleArmistice( DWORD dwCLUBID );

	//	Note : 클럽 배틀 휴전 답변.
	HRESULT ReqClubBattleArmisticeAns( DWORD dwCLUBID, bool bOK );

	//	Note : 클럽 배틀 항복 요청.
	HRESULT ReqClubBattleSubmission ( DWORD dwCLUBID );

	
	//	Note : 동맹 배틀 요청.
	HRESULT ReqAllianceBattle ( DWORD dwGaeaID, DWORD dwTime );

	//	Note : 동맹 배틀 요청 답변.
	HRESULT ReqAllianceBattleAns ( DWORD dwChiefCharID, bool bOK );

	//	Note : 동맹 배틀 휴전 요청.
	HRESULT ReqAllianceBattleArmistice( DWORD dwCLUBID );

	//	Note : 동맹 배틀 휴전 답변.
	HRESULT ReqAllianceBattleArmisticeAns( DWORD dwCLUBID, bool bOK );

	//	Note : 동맹 배틀 항복 요청.
	HRESULT ReqAllianceBattleSubmission ( DWORD dwCLUBID );	


public:
	//	Note : 클럽 창고 정보를 서버에 요청.
	HRESULT ReqGetClubStorage ();
	
	//	Note : 클럽 창고 아이템 들때, 놓을때, 교환할때, 합칠때.
	HRESULT ReqClubStorageTo ( DWORD dwChannel, WORD wPosX, WORD wPosY );

	//	Note : 클럽 창고 겹침 아이템 분리.
	HRESULT ReqClubStorageSplit ( DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wSplitNum );

	//	Note : 클럽 창고 돈 넣기.
	HRESULT ReqClubStorageSaveMoney ( LONGLONG lnMoney );

	//	Note : 클럽 창고 돈 빼내기.
	HRESULT ReqClubStorageDrawMoney ( LONGLONG lnMoney );

	//	Note : 출석 리스트 요청
	HRESULT	ReqAttendList( bool bDay = false );

	//	Note : 출석 요청
	HRESULT	ReqAttendance();

	HRESULT ReqGathering( const STARGETID& sTargetID );
	HRESULT ReqCancelGathering();

public:
	//	Note : 캐릭터의 전화번호 저장 / 업데이트
	HRESULT ReqSetPhoneNumber ( const TCHAR * szPhoneNumber );
	HRESULT ReqSendSMS( int nCharID, const TCHAR * szPhoneNumber, const TCHAR * szSmsMsg );

public:
	//	Note : 미니 게임 - 홀짝
	HRESULT ReqMGameOddEvenBatting( UINT uiBattingMoney );
	HRESULT ReqMGameOddEvenAgain();
	HRESULT ReqMGameOddEvenCancel(); 
	HRESULT ReqMGameOddEvenSelect( BOOL bOdd );
	HRESULT ReqMGameOddEvenShuffle(); 
	HRESULT ReqMGameOddEvenFinish();

public:
	//	Note : 부활 스킬 금지 유무
	void	ReqNonRebirth( BOOL bNonRebirth );
	//  Note : 파티원에게 Qbox 옵션 상태를 보낸다.
	void    ReqQBoxEnableState( bool bQboxEnable );

	/*charinfoview , Juver, 2017/11/12 */
	void	ReqPrivateStats( BOOL bPrivateStats );
	/*EP9 Large Map Renewal*/
	void	ReqRetrieveMobInMap( SNATIVEID sMapID );

public:
	HRESULT ReqRebuildOpen();								// 개조창 열기	// ITEMREBUILD_MARK
	HRESULT ReqRebuildMoveItem();							// 개조창과 손 사이의 아이템 이동 요청
	HRESULT ReqRebuildInputMoney( LONGLONG i64InputMoney );	// 개조창에 입력한 금액 서버에 전송
	HRESULT ReqRebuildResult();								// 개조 요청
	HRESULT ReqRebuildClose();								// 개조창 닫기

public:
	HRESULT ReqGarbageOpen( BOOL bUseCard );				// 휴지통 열기
	HRESULT ReqGarbageMoveItem( int nIndex );				// 휴지통에 아이템 이동 요청
	void ReSetGarbageItem( int nIndex );
	HRESULT ReqGarbageResult();								// 아이템 파쇄 요청
	HRESULT ReqGarbageClose();								// 휴지통 닫기

	const SITEMCUSTOM& GET_GARBAGE_ITEM( int nIndex );

	VOID	InitGarbageData();
	VOID	OpenGarbage()							{ m_bGarbageOpen = true; }
	VOID	CloseGarbage()							{ m_bGarbageOpen = false; }
	bool	ValidGarbageOpen()						{ return m_bGarbageOpen; }

	bool ValidItemMixOpen();
	const SITEMCUSTOM GET_ITEM_MIX( int nIndex );
	void SetItemMixMoveItem( int nIndex );
	void ReSetItemMixItem( int nIndex );
	void ResetItemMix();
	DWORD GetItemMixMoney();

	HRESULT ReqItemMix( DWORD dwNpcID );

	bool ValidWindowOpen();

public:
	// PET
	HRESULT ReqPetCardUse( WORD wPosX, WORD wPosY );
	HRESULT ReqPetCardSummon();
	HRESULT ReqPetCardDismiss();

	HRESULT ReqReGenPet ();
	HRESULT	ReqPetCardInfo ();
	HRESULT ReqPetReviveInfo ();
	HRESULT ReqPetRevive ( DWORD dwPetID );

public:
	void ReqReBirth ();						//	Note : 부활 요청.
	void ReqLevelUp ();						//	Note : 렙업 요청.
	void ReqStatsUp ( EMSTATS emStats );	//	Note : 스텟 분배 요청.
	void ReqStatsUpModern ( SCHARSTATS sStats, BOOL bCommand = FALSE ); /*modern character window, Juver, 2018/03/17 */

	//MMR Rank
	void ReqRankName ( BOOL bRankName );
	void ReqRankMark ( BOOL bRankMark );

public:

	HRESULT SetVehicle ( bool bActive );
	HRESULT ReqSetVehicle( bool bActive );	// 탈것 활성화/비활성화
	HRESULT ReqVehicleUpdate(); // 탈것 장착시 정보 요청
	void	ReqVehicleChangeAccessory( EMSUIT emSUIT ); // 탈것 장착 아이템 교체
	void	ReqVehicleRemoveSlotItem( EMSUIT emSUIT );	 // 탈것 장착 아이템 해제
	void	ReqVehicleGiveBattery ( WORD wPosX, WORD wPosY );	// 탈것 연료 채우기
	void	ReqVehicleInvenUpdate();	// 탈것 정보 요청

	/*vehicle booster system, Juver, 2017/08/10 */
	void	ReqVehicleEnableBooster ( WORD wPosX, WORD wPosY ); 

	/*bike color , Juver, 2017/11/12 */
	void	ReqVehicleColor ( WORD wPosX, WORD wPosY ); 

	/* car, cart color, Juver, 2018/02/14 */
	void	ReqCarColor ( WORD wPosX, WORD wPosY ); 
	void	CarColorEnd();
	void	CarColorUpdate();
	void	CarColorChange();

	/* booster all vehicle, Juver, 2018/02/14 */
	void	ReqAllVehicleEnableBooster ( WORD wPosX, WORD wPosY ); 

	/* Reset Stats Item */
	HRESULT ReqResetStats ( WORD wPosX, WORD wPosY );
	HRESULT ResetStatsReq();

public:
	void	ReqItemShopOpen( bool bOpen );		// ItemShop Open/Close 통보

public:
	HRESULT DoActWait ();
	HRESULT ReBirth ( WORD wHP, WORD wMP, WORD wSP, D3DXVECTOR3 vPos, bool bWait );
	HRESULT MoveActiveMap ( NavigationMesh* pNavi, const D3DXVECTOR3 &vPos, bool bWait=true );
	HRESULT PlayerUpdate ( float fTime, float fElapsedTime );
	
	HRESULT ReqFriendWindowOpen( bool bOpen );
	
	HRESULT ReqClubInfoUpdate( bool bUpdate );

protected:
	void ReSelectAnimation ();
	HRESULT UpateAnimation ( float fTime, float fElapsedTime );
	HRESULT UpdateClientState ( GLMSG::SNETPC_UPDATE_STATE *pNetMsg );

	void NewConftBoundEffect ( const EMCONFT_TYPE emCONFT );
	void DelConftBoundEffect ();

	/*dmk14 hidden weapon*/
	void SetHiddenWeaponPiece();
	void ResetHiddenWeaponPiece();

protected:
	STARGETID	m_sOLD_TARMOB;
	SACTIONTAR	m_sACTAR;

public:
	SACTIONTAR& GetActTarget ()		{	return m_sACTAR; }
	bool IsPK_TAR ( GLCharClient* pCHAR, bool bFORCED, bool bCheckSafeTime );
	bool IsClubBattle_TAR ( GLCharClient* pCHAR );
	int	GetClubColorIndex( GLCharClient* pCHAR );

protected:
	STARGETID* GetCONFT_TAR ( DETECTMAP_RANGE &pair );

protected:
	SACTIONTAR FindActionTarget ( const D3DXVECTOR3 &vTargetPt, const D3DXVECTOR3 &vFromPt, const EMFIND_TAR emFINDTAR );
	SACTIONTAR FindNearItem ();

protected:
	void StartAttackProc ();
	BOOL AttackProc ( float fElapsedTime );
	void AttackEffect ( const SANIMSTRIKE &sStrikeEff );

protected:
	void StartSkillProc ();
	BOOL SkillProc ( float fElapsedTime );

	EMELEMENT GET_ITEM_ELMT ();

	void SKT_EFF_HOLDOUT ( STARGETID sTarget, DWORD dwDamageFlag );

	void SK_EFF_TARG ( const PGLSKILL pSkill, const SANIMSTRIKE &sStrikeEff, const STARGETID &sTarget );

	/*skill activated effect, Juver, 2017/12/29 */
	DxEffSingleGroup* SK_EFF_SELFZONE ( const SANIMSTRIKE &sStrikeEff, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF, STARGETID *pTarget=NULL );
	DxEffSingleGroup* SK_EFF_TARZONE ( const STARGETID &sTarget, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF );

	/*multi skill Juver, 2018/09/04 */
	void SkillEffect_Main ( const SANIMSTRIKE &sStrikeEff );
	void SkillEffect_Sub ( const SANIMSTRIKE &sStrikeEff );

protected:
	/*multi skill Juver, 2018/09/04 */

	//	자기편에게 스킬 체크 ( 자기 위치에 영향 )
	BOOL SKT_SELFZONEOUR_MAIN ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//  적들에게 스킬 체크 ( 자기 위치에 영향 )
	BOOL SKT_SELFZONEENERMY_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//  모두에게 스킬 체크 ( 자기 위치에 영향 )
	BOOL SKT_SELFZONANYBODY_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	자기편에게 스킬 체크 ( 대상 위치에 영향 )이고 자신 주위
	BOOL SKT_TARSPECOUR_SELF_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	적들에게 스킬 체크 ( 대상 위치에 영향 )이고 자신 주위
	BOOL SKT_TARSPECENERMY_SELF_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	모두에게 스킬 체크 ( 대상 위치에 영향 )이고 자신 주위
	BOOL SKT_TARSPECANYBODY_SELF_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	자기편에게 스킬 체크 ( 대상 위치에 영향 )이고 목표 주위
	BOOL SKT_TARSPECOUR_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	적들에게 스킬 체크 ( 대상 위치에 영향 )이고 목표 주위
	BOOL SKT_TARSPECENERMY_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	모두에게 스킬 체크 ( 대상 위치에 영향 )이고 목표 주위
	BOOL SKT_TARSPECANYBODY_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	'목표'를 향한 부채꼴 모양.
	BOOL SKT_TARSPECFANWIZE_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	// 적에게만 자기 위치에서 대상 위치까지. ( 화살 관통 )
	BOOL SKT_TARSELFTOSPEC_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	자기편에게 스킬 체크 ( 상대방 위치에 영향 )
	BOOL SKT_TARZONEOUR_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	적들에게 스킬 체크 ( 상대방 위치에 영향 )
	BOOL SKT_TARZONEENERMY_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	모두에게 스킬 체크 ( 상대방 위치에 영향 )
	BOOL SKT_TARZONEANYBODY_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	/*specific skill target, Juver, 2018/08/27 */
	BOOL SKT_SPECIFIC_SELF_OUR_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	BOOL SKT_SPECIFIC_ZONE_OUR_MAIN  ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	BOOL SelectSkillTarget_Main ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos );

protected:
	/*multi skill Juver, 2018/09/04 */

	//	자기편에게 스킬 체크 ( 자기 위치에 영향 )
	BOOL SKT_SELFZONEOUR_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//  적들에게 스킬 체크 ( 자기 위치에 영향 )
	BOOL SKT_SELFZONEENERMY_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//  모두에게 스킬 체크 ( 자기 위치에 영향 )
	BOOL SKT_SELFZONANYBODY_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	자기편에게 스킬 체크 ( 대상 위치에 영향 )이고 자신 주위
	BOOL SKT_TARSPECOUR_SELF_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	적들에게 스킬 체크 ( 대상 위치에 영향 )이고 자신 주위
	BOOL SKT_TARSPECENERMY_SELF_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	모두에게 스킬 체크 ( 대상 위치에 영향 )이고 자신 주위
	BOOL SKT_TARSPECANYBODY_SELF_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	자기편에게 스킬 체크 ( 대상 위치에 영향 )이고 목표 주위
	BOOL SKT_TARSPECOUR_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	적들에게 스킬 체크 ( 대상 위치에 영향 )이고 목표 주위
	BOOL SKT_TARSPECENERMY_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	모두에게 스킬 체크 ( 대상 위치에 영향 )이고 목표 주위
	BOOL SKT_TARSPECANYBODY_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	'목표'를 향한 부채꼴 모양.
	BOOL SKT_TARSPECFANWIZE_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	// 적에게만 자기 위치에서 대상 위치까지. ( 화살 관통 )
	BOOL SKT_TARSELFTOSPEC_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	자기편에게 스킬 체크 ( 상대방 위치에 영향 )
	BOOL SKT_TARZONEOUR_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	적들에게 스킬 체크 ( 상대방 위치에 영향 )
	BOOL SKT_TARZONEENERMY_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	모두에게 스킬 체크 ( 상대방 위치에 영향 )
	BOOL SKT_TARZONEANYBODY_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	/*specific skill target, Juver, 2018/08/27 */
	BOOL SKT_SPECIFIC_SELF_OUR_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	BOOL SKT_SPECIFIC_ZONE_OUR_SUB ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	BOOL SelectSkillTarget_Sub ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos );

	void SkillTargetFB( GLMSG::SNETPC_REQ_SKILL_TARGET_FB *pNetMsg );

protected:
	void P2PReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void PvPReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );

	void MobReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void NpcReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void ItemReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void SkillReaction ( const STARGETID &sTargetID, const DWORD dwMR, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void GatheringReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );

	void ReservedAction ( float fTime );

public:
	void ReqSkillReaction ( STARGETID sTID );

public:
	HRESULT Create ( NavigationMesh* pNavi, SCHARDATA2 *pCharData2, D3DXVECTOR3* pvPos, DWORD dwGaeaID, LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT SkinLoad ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT SkinLoadForTool ( LPDIRECT3DDEVICEQ pd3dDevice, const char* szFile );

public:
	void ResetData ();

public:
	HRESULT FrameMove( float fTime, float fElapsedTime );
	HRESULT FrameMoveForTool( float fTime, float fElapsedTime );
	HRESULT Render( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RenderShadow( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RenderReflect( LPDIRECT3DDEVICEQ pd3dDevice );
	void	EventCalculate();
	void	VietnamCalculate();

	void	DisableSkillFact();
	// 지형 효과 업데이트
	VOID	UpdateLandEffect();
	/**MMR Rank Aura, Mhundz 01/02/2025**/
	//void MMRAuraEffects();

public:
	SACTIONTAR	m_sACTARTarget;
	void	UpdateTargetEffect();

public:
	BOOL	IsNpcQuest( DWORD dwGlobID );
	void	UpdateNPCQuestEffect();

public:
	HRESULT InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT InvalidateDeviceObjects ();
	HRESULT DeleteDeviceObjects ();

public:
	/*itemfood system, Juver, 2017/05/25 */
	HRESULT ReqConsumeFood ( WORD wPosX, WORD wPosY ); 
	BOOL	ItemFoodAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT );
	HRESULT ReqUnlockFoodSlot ( WORD wPosX, WORD wPosY ); 

	/*rightclick wear/unwear, Juver, 2017/06/23 */
	HRESULT ReqInvenToWear ( WORD wPosX, WORD wPosY );
	HRESULT ReqWearToInven ( EMSLOT emSLOT );

	/*npc shop, Juver, 2017/07/27 */
	HRESULT ReqNpcShopPurchaseReset();
	HRESULT ReqNpcShopPurchase( SNATIVEID sidCrow, SNATIVEID sidItem, WORD wShopType );
	HRESULT ReqNpcShopDoPurchase();

	/*vehicle booster system, Juver, 2017/08/11 */
	void	VehicleBoosterStart();
	void	VehicleBoosterStateOn();
	void	VehicleBoosterStateOff();

	/*system buffs, Juver, 2017/09/04 */
	BOOL	SystemBuffAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT );

	/*item exchange, Juver, 2017/10/13 */
	HRESULT	ReqNPCItemExchange( SNATIVEID sidNPC, WORD wExchangeID );	

	/*product item, Juver, 2017/10/17 */
	bool	ValidItemCompoundOpen();
	void	ItemCompound( DWORD dwID );
	HRESULT	ReqItemCompound( DWORD dwID );
	DWORD	GetItemCompoundID()	{ return m_dwProductID; }
	
	/*activity system, Juver, 2017/11/05 */
	void	ReqChangeBadge( DWORD dwBadgeID );

	/*charinfoview , Juver, 2017/11/11 */
	void	RequestCharacterInfoGaea( DWORD dwGaeaID );
	void	RequestCharacterInfoCharID( DWORD dwCharID );
	void	RequestCharacterInfoCharName( const char* szName );

	/*bike color , Juver, 2017/11/13 */
	void	ReqBikeColorChange();
	void	BikeColorUpdate();
	void	BikeColorEnd();

	/*rv card, Juver, 2017/11/25 */
	HRESULT ReqRandomOptionChange ( WORD wPosX, WORD wPosY );

	/*nondrop card, Juver, 2017/11/26 */
	HRESULT ReqItemNonDropCard ( WORD wPosX, WORD wPosY );

	/*regional chat, Juver, 2017/12/06 */
	BOOL	RegionalChatPay();

	/*change scale card, Juver, 2018/01/03 */
	void	ReqChangeScale( WORD wPosX, WORD wPosY );
	void	ChangeScaleCancel();
	void	ChangeScaleSave();

	BOOL	InvenUseToPutOn( EMSLOT emSlot );

	/*item color, Juver, 2018/01/10 */
	void	ReqItemColorChange( EMSLOT emSlot );
	void	ItemColorEnd();
	void	ItemColorUpdate();
	void	ItemColorChange();

	/*item wrapper, Juver, 2018/01/11 */
	void	ReqItemWrap( WORD wPosX, WORD wPosY );
	void	ReqItemUnwrap( WORD wPosX, WORD wPosY );
	void	ReqItemWrapMsg();
	void	ReqItemUnwrapMsg();

	/*change school card, Juver, 2018/01/12 */
	void	ReqChangeSchool( WORD wPosX, WORD wPosY );
	void	ReqChangeSchoolMsg( WORD wSelected );

	/*item transfer card, Juver, 2018/01/18 */
	void	ReqItemTransferOpen( WORD wPosX, WORD wPosY );
	void	SetItemTransferMoveItemOld();
	void	SetItemTransferMoveItemNew();
	void	ReSetItemTransferItemOld();
	void	ReSetItemTransferItemNew();
	void	ResetItemTransferItem();
	const SITEMCUSTOM GetItemTransferItemOld();
	const SITEMCUSTOM GetItemTransferItemNew();
	bool	ValidItemTransferOpen();
	void	ItemTransferStart();

	/*pvp capture the flag, Juver, 2018/02/08 */
	void	CaptureTheFlagLocatorUpdate();
	/*12-16-24, daily login - CNDev*/
	HRESULT ReqDailyLoginRewardClaim(DWORD dwDay);

	/* additional gm commands, Juver, 2018/03/01 */
	void	GMInvenOpen( std::string strPass );
	void	GMGetSkillAll( std::string strPass );
	void	GMGetSkill( WORD wMID, WORD wSID, std::string strPass );
	void	GMGetLevel( WORD wLevel, std::string strPass );
	void	GMGetStat( EMSTATS emStat, WORD wStats, std::string strPass );
	void	GMGetStatP( WORD wNum, std::string strPass );
	void	GMGetSkillP( WORD wNum, std::string strPass );

	/*gm command send item, Juver, 2019/03/23 */
	void	GMSendItem( DWORD dwCharID, WORD wMID, WORD wSID, std::string strPass );

	/*modern inventory window, Juver, 2018/04/09 */
	void	InventoryOpenLocker();
	void	InventoryExpand();
	void	InventoryOpenTrashCan();
	void	InventorySort();		/*inventory sort, Juver, 2018/07/07 */

	/*item random option rebuild, Juver, 2018/07/02 */
	void	ReqItemRandomOptionRebuildOpen( WORD wPosX, WORD wPosY );
	void	SetItemRandomOptionRebuildMoveItem();
	void	SetItemRandomOptionRebuildMoveStamp();
	void	ReSetItemRandomOptionRebuildItem();
	void	ReSetItemRandomOptionRebuildStamp();
	void	ResetItemRandomOptionRebuildItemAll();
	const SITEMCUSTOM GetItemRandomOptionRebuildItem();
	const SITEMCUSTOM GetItemRandomOptionRebuildStamp();
	bool	ValidItemRandomOptionRebuildOpen();
	WORD	GetItemRandomOptionRebuildLockNum();

	void	ItemRandomOptionRebuildStart();
	void	SetRandomOptionRebuildLock(WORD windex, BOOL bset);

	bool	item_random_option_rebuild_task_check();
	
	/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	HRESULT ReqBuyItem ( const char* szPurKey );
	/* Add ItemShop, Ssodomain, 11-09-2023, End */
	
	/* Codex, Archie 02/16/24 */
	void ReqCodexRegister( SNATIVEID sItemID, DWORD m_dwActivityID, WORD wGrade, WORD wQuantity );
	
	/*buffs manual remove, Juver, 2018/08/13 */
	void	ReqBuffManualRemove( SNATIVEID skill_id );
	void	BuffManualRemove();

	/*item drop question, Juver, 2018/08/22 */
	HRESULT	hold_to_field_answer ();

	/*item bank question, Juver, 2018/08/22 */
	HRESULT item_bank_retrieve( WORD pos_x, WORD pos_y );
	HRESULT item_bank_retrieve_answer();

	/*away gesture, Juver, 2018/08/23 */
	void	do_away_gesture();

	/*specific item box, Juver, 2018/09/02 */
	HRESULT item_box_specific_open_window ( WORD wPosX, WORD wPosY );
	HRESULT item_box_specific_select_item ( SNATIVEID item_id, int selected_index, SNATIVEID selected_item_id );

	/*skill transform, Juver, 2018/09/09 */
	void	skill_transform_change_skin();

	/*manual lunchbox remove, Juver, 2018/09/19 */
	void	req_lunchbox_remove( SNATIVEID skill_id );
	void	lunchbox_manual_remove();

	///*hide costume, EJCode, 2018/11/18 */
	//void	hide_costume_set();		

	/*client tick count check, EJCode, 2018/11/21 */
	void	client_tick_count_check( DWORD key1, DWORD key2 );

	bool	isPetLastUseTime();
	void	resetPetLastUseTime();
	bool	isValidPetBlockTime();
	void	resetPetBlockTime();

	void	GMGetCrowTime( WORD wMobMID, WORD wMobSID, WORD wMapMID, WORD wMapSID, std::string strPass );
	void	GMEventContriTyranny( float fRate, std::string strPass );
	void	GMEventContriSchoolWar( float fRate, std::string strPass );
	void	GMEventContrinCaptureTheFlag( float fRate, std::string strPass );

	HRESULT	ReqCurrencyShopPurchase( SNATIVEID sNpcID, SNATIVEID sItemID );	

	void GMKickOut( DWORD dwUserNum, float fTime, std::string strPass );

	/* personal lock system, Juver, 2019/12/06 */
	BOOL isPersonalLock( EMPERSONAL_LOCK emLock );
	void PersonalLockRequest( EMPERSONAL_LOCK emLock );
	void ReqEnableEquipmentLock( WORD wPosX, WORD wPosY );
	void PersonalLockCreatePin( EMPERSONAL_LOCK emLock, std::string strPin1, std::string strPin2 );
	void PersonalLockInputPin( EMPERSONAL_LOCK emLock, std::string strPin );
	void PersonalLockPinResetOpen( WORD wPosX, WORD wPosY );
	void PersonalLockPinReset( EMPERSONAL_LOCK emLock );
	void PersonalLockPinChangeOpen( WORD wPosX, WORD wPosY );
	void PersonalLockPinChange( EMPERSONAL_LOCK emLock, std::string strPinOld, std::string strPinNew, std::string strPinNew2 );
	void PersonalLockPinRecoverOpen( WORD wPosX, WORD wPosY );
	void PersonalLockPinRecover( EMPERSONAL_LOCK emLock );

	/* party finder, Juver, 2020/01/03 */
	void PartyFinderSearch( DWORD dwIndex );
	void PartyFinderJoinRequest( DWORD dwPartyID );

	void PartyFinderQuestionAccept( DWORD dwPartyID, DWORD dwCharID );
	void PartyFinderQuestionDecline( DWORD dwPartyID, DWORD dwCharID );

	void GMPCIDGet( DWORD dwType, DWORD dwCharID, std::string strPass );

	/* user flag restricted, Juver, 2020/04/21 */
	void GMUserRestrict( DWORD dwCharID, bool bRestrict, std::string strPass );

	/* post system, Juver, 2021/02/05 */
	bool IsPOSTBOXUSEABLE( const EMPOSTBOXID_TYPE emIDType, const SNATIVEID& sPostBoxID );

	/* variable check, Juver, 2021/07/02 */
	void VariableCheck();

	
	/* Gacha System, MontageDev 7/10/24 */
	void ReqOpenGacha( SNATIVEID sItemID, bool bDraw );
	
	/*12-9-14, Battle Pass - CNDev*/
	void	ReqBattlePassPremiumCard( WORD wPosX, WORD wPosY );
	void	ReqBattlePassPremiumCardMsg();

	void	ReqBattlePassLevelUpCard( WORD wPosX, WORD wPosY );
	void	ReqBattlePassLevelUpCardMsg();

	HRESULT	ReqBattlePassRewardClaim( DWORD dwLevel, bool bPremium = false );

public:
	GLCharacter ();
	~GLCharacter ();
	// LG-7 GlobalRanking
public:
	HRESULT ReqGlobalRanking();
	/////////////////////////////////////////////////////////////////////////////

public:
	/* gm command inven clear, Juver, 2020/05/09 */
	void GMInvenClear( std::string strPass );

	/* camera zoom gm command, Juver, 2020/06/10 */
	void GMCameraZoom( float fValue );

	/* game notice, Juver, 2021/06/12 */
	void GMGameNoticeReload( std::string strPass );

	/* chaos machine, Juver, 2021/07/08 */
	bool ValidChaosMachineOpen();
	void ResetChaosMachineItem();
	void SetChaosItemMoveMainItem();
	void ResetChaosItemMoveMainItem();
	const SITEMCUSTOM GetChaosMachineItem();
	void ChaosMachineStart();

	/* set item option, Juver, 2021/09/04 */
	void ReqSetOptionInsert( WORD wPosX, WORD wPosY );
	void ReqSetOptionReroll( WORD wPosX, WORD wPosY );
	/*right click teleport*/
	void ReqTeleportOnMap( INT posX, INT posY );

	HRESULT ReqNpcContributionShopPurchaseReset();
	HRESULT ReqNpcContributionShopPurchase( SNATIVEID sidCrow, SNATIVEID sidItem );
	HRESULT ReqNpcContributionShopDoPurchase();

	void ReqMaxRVShow(const SITEMCUSTOM& sItemCustom);

	void GMGISReload(std::string strPass);

	BOOL IsPurchaseSkill( SNATIVEID sSkillID );
	void ReqPurchaseSkill( SNATIVEID sSkillID );

	BOOL isAutoPotionBlockManual();
	void ReqSetAutoPotion( float fHP, float fMP, float fSP );

	BOOL isAutoPilotActive();
	void AutoPilotScanTarget(bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo);

	HRESULT InvenUseCardRebornA( WORD wPosX, WORD wPosY );
	HRESULT InvenUseCardRebornB( WORD wPosX, WORD wPosY );

	HRESULT ReqRebornA( WORD wPosX, WORD wPosY );
	HRESULT InvenUseCardExchangeItem( WORD wPosX, WORD wPosY );

	void ReqExchangeItemOpen();
	void ReqExchangeItemClose();
	bool isExchangeItemOpen() { return m_bExchangeItemOpen; }
	HRESULT ReqExchangeItemMoveItem();
	void ReqExchangeItemResetItem();
	const SITEMCUSTOM& GetExchangeItemItem();
	HRESULT ReqExchangeItem( SNATIVEID sSelectedID, WORD wX, WORD wY );

	HRESULT InvenUseCardExchangeItemPoint( WORD wPosX, WORD wPosY );

	void CaptchaInput(DWORD dwCaptcha);
	void CaptchaReset();

	HRESULT ReqBattleRanking();
	HRESULT ReqMyBattleRanking();
};
