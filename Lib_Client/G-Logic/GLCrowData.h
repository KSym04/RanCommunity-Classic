#ifndef GLCROWDATA_H_
#define GLCROWDATA_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include "./GLCharData.h"
#include "./GLCharDefine.h"
#include "./GLInventory.h"
#include "../NpcTalk/NpcDialogueSet.h"

#include "../../Lib_Engine/G-Logic/GLDefine.h"
#include "../../Lib_Engine/Meshs/SAnimation.h"
#include "../../Lib_Engine/NaviMesh/NavigationPath.h"
 
#include "./GLCrowDataBasic.h"
#include "./GLCrowDataAction.h"
#include "./GLCrowDataGen.h"
#include "./GLCrowDataAttack.h"
#include "./GLCrowDataWarden.h"

/*pvp tyranny, Juver, 2017/08/25 */
#include "./GLPVPTyrannyDefine.h" 
/*woe Arc Development 08-06-2024*/
#include "./GLPVPWoeDefine.h"
#include "./GLCrowSale.h"
#include "./GLCurrencyShop.h"
#include "./GLNPCShop.h"
#include "./GLItemExchange.h"
#include "./GLContributionShop.h"

/* skill turn/find invisible, Juver, 2020/04/25 */
#define CROW_DEFAULT_INVISIBLE_LEVEL				10
#define CROW_DEFAULT_INVISIBLE_ANIMATION			0
#define CROW_DEFAULT_INVISIBLE_DISABLE_IN_COMBAT	false
#define CROW_DEFAULT_SHOW_INVISIBLE_LEVEL			10
#define CROW_DEFAULT_SHOW_INVISIBLE_RANGE			100

//	Note : 다수 아이템 발생시 처리 구조.
//
struct SGENITEM
{
	enum
	{
		EMGNUM		= 5,
		EMGSPEC		= 30,
		EMSPEC_NULL	= -1
	};

	struct SGNUM	//	발생 갯수별 확율.
	{
		WORD	wNum;
		float	fRateL;
		float	fRateH;

		SGNUM () :
			wNum(0),
			fRateL(100.0f),
			fRateH(100.0f)
		{
		}
	};

	struct SGITEMSPEC	//	발생 아이템군별 확율.
	{
		DWORD	dwSPECID;
		float	fRateL;
		float	fRateH;
		bool	bSPECID;

		SGITEMSPEC () :
			dwSPECID(EMSPEC_NULL),
			fRateL(100.0f),
			fRateH(100.0f),
			bSPECID( false )
		{
		}
	};

	/*genitem skill spec, Juver, 2018/06/30 */
	struct SGITEMSPEC_SKILL
	{
		DWORD	dwSPECID;
		float	fRateL;
		float	fRateH;
		bool	bSPECID;
		SNATIVEID sidSkill;

		SGITEMSPEC_SKILL () :
			dwSPECID(EMSPEC_NULL),
			fRateL(100.0f),
			fRateH(100.0f),
			bSPECID( false ),
			sidSkill(false)
		{
		}
	};

	float		m_fGenRate;				//	발생 확율.
	SGNUM		m_sGenNum[EMGNUM];		//	발생 갯수별 확율 분포.
	SGITEMSPEC	m_sItemSpec[EMGSPEC];	//	발생 아이템 군.
	SGITEMSPEC_SKILL m_sItemSpecSkill[EMGSPEC];	/*genitem skill spec, Juver, 2018/06/30 */

	SGENITEM() :
		m_fGenRate(0)
	{
	}

	BOOL LOADFILE ( const char* szFileName );

	BOOL ISGEN ( float fGenRate = 1.0f );							//	발생 확율.
	WORD SELECTNUM ();						//	발생 갯수.
	DWORD SELECTSPECID ( float fGEN_RATE, bool& bSPECID );	//	발생 군.

	/*genitem skill spec, Juver, 2018/07/01 */
	DWORD SELECTSPECID_SKILL ( float fGEN_RATE, bool& bSPECID, SNATIVEID& sidSkill );
};

//	Note : Question 아이템 발생시 처리 구조 by 경대
//
struct SQTGENITEM
{
	enum { EMQTMAX = 20 };

	struct ITEM
	{
		SNATIVEID	m_sGenItemID;
		float		m_fGenRate;

		ITEM() : m_sGenItemID(false), m_fGenRate(0)
		{
		}
	};

	ITEM	m_sItem[EMQTMAX];
	int		m_nItemNum;

	SQTGENITEM() 
		: m_nItemNum(0)
	{
	}

	BOOL LOADFILE ( const char * szFileName );

	SNATIVEID SELECTITEM(); // 아이템 발생
};

typedef std::map<DWORD,LONGLONG>	NPCSELL_PRICE_MAP;
typedef NPCSELL_PRICE_MAP::iterator NPCSELL_PRICE_MAP_ITER;

/* multiple crow gen, Juver, 2018/02/21 */
typedef std::vector<SGENITEM>	VEC_SGENITEM;
typedef std::vector<SQTGENITEM>	VEC_SQTGENITEM;

/* Boss Spawn Viewer, Review000 */
typedef std::map<DWORD,DWORD>			MOB_DROP_SPECID_MAP;	//--.second == SPEC ID from .genitem file
typedef MOB_DROP_SPECID_MAP::iterator	MOB_DROP_SPECID_MAP_ITER;

struct SCROWDATA
{
	enum
	{
		VERSION = 0x0101,
		EMMAXATTACK = SCROWACTION::ACTION_MAXATTACK,

		FILE_SBASIC			= 1,
		FILE_SACTION		= 2,
		FILE_SGEN			= 3,
		FILE_SATTACK		= 4,
		FILE_SATTACK_EX		= 5,
		FILE_SWARDEN		= 6,
		FILE_END_DATA		= 0xEDEDEDED,
	};

	union
	{
		struct
		{
			SCROWBASIC			m_sBasic;
		};

		//changes to SCROWBASIC structure or variable type/arrangement must be also applied here
		struct
		{
			SNATIVEID			sNativeID;					//	고유 ID.
			EMCROW				m_emCrow;					//	Npc, Mob.
			EMTRIBE				m_emTribe;					//	종족.
			EMBRIGHT			m_emBright;					//	속성 (광/암)
			char				m_szName[CHAR_SZNAME];		//	이름.
			WORD				m_wLevel;					//	레벨.
			DWORD				m_dwHP;						//	HP
			WORD				m_wMP;						//	MP
			WORD				m_wSP;						//	SP
			float				m_fIncHP;					//	HP 회복율.
			float				m_fIncMP;					//	MP 회복율.
			float				m_fIncSP;					//	SP 회복율.
			WORD				m_wDefense;					//	방어력.
			WORD				m_wViewRange;				//	시야 거리.
			WORD				m_wAvoidRate;				//	회피율.
			WORD				m_wHitRate;					//	명중율.
			DWORD				m_dwBonusExp;				//	추가경험치.
			WORD				m_wBonusCP;
			SRESIST				m_sResist;					//	저항값.
			bool				m_bOverlapAttack;			//  겹쳤을때 공격 가능 여부
			DWORD				m_dwGenTime;
			WORD				m_wGatherTimeLow;			// 채집시간(Low)
			WORD				m_wGatherTimeHigh;			// 채집시간(High)
			float				m_fGatherRate;				// 채집 확률
			WORD				m_wGatherAnimation;			// 애니메이션
			PETTYPE				m_emPetType;
			SUMMON_TYPE			m_emSummonType;
			EMNPC_TYPE			m_emNPCType;
			bool				m_bHideName;				/*hide crow name, Juver, 2017/09/01 */
			DWORD				m_dwNotice;					/*crow notice, Juver, 2017/11/25 */
			DWORD				dummy_var_0;
			bool				dummy_var_1;
			bool				dummy_var_2;
			EMNPC_CTF_FLAG_TYPE	m_emCTFType;				/*pvp capture the flag, Juver, 2018/02/03 */
		};
	};

	SCROWACTION				m_sAction;
	SCROWGEN				m_sGenerate;

	//	npc talk 정보.
	CNpcDialogueSet			m_sNpcTalkDlg;

	//	공격, Skill 설정.
	SCROWATTACK				m_sCrowAttack[EMMAXATTACK];

	//	생성 아이템군 설정 [server 에서만 필요한 정보]
	/* multiple crow gen, Juver, 2018/02/21 */
	VEC_SGENITEM			m_vecGenItem;
	VEC_SQTGENITEM			m_vecQtGenItem; // Question Item 생성 by 경대

	SCROWWARDEN				m_sCrowWarden;
	
	// For Codex - JX
	std::string				m_strSaleType[SCROWACTION::SALENUM];
	GLInventory				m_sSaleItems[SCROWACTION::SALENUM];
	NPCSELL_PRICE_MAP		m_mapNpcSellPrice;

	//	에니메이션 정보. ( 시간/스탭 ) [server 에서만 필요한 정보]
	//	
	VECANIATTACK*			m_pANIMATION;

	VECANIATTACK&			GetAnimation ( EMANI_MAINTYPE emMType, EMANI_SUBTYPE emSType );

	/* Boss Spawn Viewer, Review000 */
	/////////////////////////////////////////////////////////////////
	//--this map will code to each crow in each map, might use many memory, but just on client side
	MOB_DROP_SPECID_MAP 	m_mapMobDropSpecID;			// [MobDB][Construction] by edwin 20190804 ***** [Edition 1] 
	/////////////////////////////////////////////////////////////////
	BOOL IsCdCertify ()		{ return m_sAction.m_dwActFlag&EMCROWACT_CDCERTIFY; }
	BOOL IsBusUnit ()		{ return m_sAction.m_dwActFlag&EMCROWACT_BUSUNIT; }
	BOOL IsTarShort ()		{ return m_sAction.m_dwActFlag&EMCROWACT_TARSHORT; }
	BOOL IsIgnoreShock()	{ return m_sAction.m_dwActFlag&EMCROWACT_IGNORE_SHOCK; }

	BOOL IsInVisible ()		{ return m_sAction.m_dwActFlag&EMCROWACT_INVISIBLE; }
	BOOL IsRecVisible ()	{ return m_sAction.m_dwActFlag&EMCROWACT_RECVISIBLE; }

	BOOL IsBarrier ()		{ return m_sAction.m_dwActFlag&EMCROWACT_BARRIER; }
	BOOL IsPosHold ()		{ return m_sAction.m_dwActFlag&EMCROWACT_POSHOLD; }
	BOOL IsDirHold ()		{ return m_sAction.m_dwActFlag&EMCROWACT_DIRHOLD; }

	/*block crow push/pull, Juver, 2019/02/14 */
	bool IsBlockPushPull()	{ return m_sBasic.m_bBlockPushPull; }

	bool IsBlockStun() { return m_sBasic.m_bBlockStun; }

	const char* GetName();
	const char* GetTalkFile ()		{ return m_sAction.m_strTalkFile.c_str(); }
	const char* GetSkinObjFile ()	{ return m_sAction.m_strSkinObj.c_str(); }

	DWORD GetSaleNum () { return SCROWACTION::SALENUM; }
	std::string GetCrowSale( DWORD dwChannel );
	
	GLInventory* GetSaleInven ( DWORD dwIndex )
	{
		if ( GetSaleNum() <= dwIndex )	return NULL;
		return &m_sSaleItems[dwIndex];
	}
	std::string GetSaleType ( DWORD dwIndex )
	{
		if ( GetSaleNum() <= dwIndex )	return "";
		return m_strSaleType[dwIndex];
	}
	LONGLONG GetNpcSellPrice ( DWORD dwID )
	{
		NPCSELL_PRICE_MAP_ITER it = m_mapNpcSellPrice.find(dwID);
		if( it == m_mapNpcSellPrice.end() )
		{
			return 0;
		}
		return it->second;
	}

	void LoadAniSet ( const char* szSkinObj );
	SCROWDATA& Assign ( SCROWDATA &Data );

	HRESULT LoadFile ( basestream &SFile, BOOL bServer, bool bPastLoad );
	HRESULT SaveFile ( CSerialFile &SFile );

	static VOID SaveCsvHead ( std::fstream &SFile );
	VOID SaveCsv ( std::fstream &SFile );
	VOID LoadCsv ( CStringArray &StrArray );

	SCROWDATA () :
		m_pANIMATION(NULL)
	{
		/* Boss Spawn Viewer, Review000 */
		////////////////////////////////////////////////////////////
		m_mapMobDropSpecID.clear();	// [MobDB][Construction] by edwin 20190804 ***** [Edition 1] 
		////////////////////////////////////////////////////////////
	}

	~SCROWDATA ()
	{
		SAFE_DELETE_ARRAY(m_pANIMATION);
	}
};
typedef SCROWDATA*		PCROWDATA;


class GLCrowDataMan
{
public:
	enum
	{
		ENCODE_VER_OLD			= 0x0102,
		ENCODE_VER_BYTE_CRYPT	= 0x0200,
		ENCODE_VER_BYTE_CRYPT2	= 0x0201,
		ENCODE_VER_BYTE_CRYPT3	= 0x0202,

		ENCODE_VER_BYTE_CRYPT_2021_CROW_V1	= 0x0203,

		VERSION					= 0x0203,

		MAX_CROW_MID	= 2048,
		MAX_CROW_SID	= 1024
	};

public:
	static const char*	_FILEHEAD;
	static const char*	_LOGFILE;
	static const char*	_STRINGTABLE;

protected:
	char				m_szFileName[MAX_PATH];

	PCROWDATA**			m_ppPCROWDATA;
	bool				m_bModify;

	LPDIRECT3DDEVICEQ	m_pd3dDevice;

public:
	PCROWDATA GetMainCrowData ( WORD wMainID );

	PCROWDATA GetCrowData ( WORD wMID, WORD wSID );
	PCROWDATA GetCrowData ( SNATIVEID sNativeID );

	bool	ValidData ();
	bool	IsModify ()								{ return m_bModify; }

public:
	WORD FindFreeCrowDataMID ();
	WORD FindFreeCrowDataSID ( WORD wMainID );

public:
	BOOL InsertCrowData ( WORD wMID, WORD wSID, PCROWDATA pCrowData, bool binner=false );
	BOOL InsertCrowData ( SNATIVEID sNativeID, PCROWDATA pCrowData, bool binner=false )
	{
		return InsertCrowData ( sNativeID.wMainID, sNativeID.wSubID, pCrowData, binner );
	}

public:
	BOOL DeleteCrowData ( WORD wMID, WORD wSID );
	BOOL DeleteCrowData ( SNATIVEID sNativeID );

public:
	HRESULT OneTimeSceneInit ();
	HRESULT FinalCleanup ();

public:
	HRESULT	SyncStringTable();
	HRESULT SyncUpdateData ();
	HRESULT SaveCsvFile( CWnd* pWnd );
	HRESULT LoadCsvFile( CWnd* pWnd );

public:
	HRESULT LoadFile ( const char* szFile, BOOL bServer, bool bPastLoad );
	HRESULT SaveFile ( const char* szFile );

public:
	SCROWSALE_DATA_POINTER_MAP				m_mapCrowSale;
	SCURRENCY_SHOP_DATA_POINTER_MAP			m_mapCurrencyShop;
	SNPC_SHOP_DATA_POINTER_MAP				m_mapNPCShop;
	SNPC_ITEM_EXCHANGE_POINTER_MAP			m_mapNPCItemExchange;
	SNPC_CONTRIBUTION_SHOP_DATA_MAP			m_mapNPCContributionShop;

public:
	void AdditionalLoad( PCROWDATA pData );

	SCROWSALE_DATA* CrowSaleLoad( std::string strFile );
	SCROWSALE_DATA* CrowSaleFind( std::string strFile );
	bool CrowSaleDelete( std::string strFile );
	void CrowSaleCleanUp();

	SCURRENCY_SHOP_DATA* CurrencyShopLoad( std::string strFile );
	SCURRENCY_SHOP_DATA* CurrencyShopFind( std::string strFile );
	void CurrencyShopDelete( std::string strFile );
	void CurrencyShopCleanUp();

	SNPC_SHOP_DATA* NPCShopLoad( std::string strFile );
	SNPC_SHOP_DATA* NPCShopFind( std::string strFile );
	void NPCShopDelete( std::string strFile );
	void NPCShopCleanUp();

	SNPC_ITEM_EXCHANGE* NPCItemExchangeLoad( std::string strFile );
	SNPC_ITEM_EXCHANGE*	NPCItemExchangeFind( std::string strFile );
	void NPCItemExchangeDelete( std::string strFile );
	void NPCItemExchangeCleanUp();

	SNPC_CONTRIBUTION_SHOP_DATA*	NPCContributionShopLoad(std::string strFile);
	SNPC_CONTRIBUTION_SHOP_DATA*	NPCContributionShopFind( std::string strFile );
	void NPCContributionShopDelete( std::string strFile );
	void NPCContributionShopCleanUp();

public:
	GLCrowDataMan ();

public:
	~GLCrowDataMan ();

public:
	static GLCrowDataMan& GetInstance();
};

struct SDROP_CROW
{
	enum { CROW_GEM=0x0001, };

	SNATIVEID			sNativeID;	//	Crow 고유 ID.

	SNATIVEID			sMapID;		//	맵 ID.
	DWORD				dwCeID;		//	셀 ID.
	DWORD				dwGlobID;	//	생성 메모리 인덱스용.
	D3DXVECTOR3			vPos;		//	위치.
	D3DXVECTOR3			vDir;		//	방향.
	FLOAT				fScale;		//  크기.

	// 서버 클라이언트간 몹의 위치 동기화를 위해
	D3DXVECTOR3			vStartPos;  // 이동시 시작위치. 추가된거
	NavigationPath::WAYPOINT sNextWaypoint; // 다음 웨이포인트. 추가된거

	DWORD				dwNowHP;	//	생명량.
	WORD				wNowMP;		//	정신량.

	DWORD				dwFLAGS;	//	기타 속성.
	EMACTIONTYPE		emAction;	//	행동.
	DWORD				dwActState;	//	행동 상태.

	STARGETID			TargetID;	//	목표 유닛, 위치.

	SDROP_SKILLFACT		sSKILLFACT[SKILLFACT_SIZE];
	SDROP_STATEBLOW		sSTATEBLOWS[EMBLOW_MULTI];

	/*pvp tyranny, Juver, 2017/08/25 */
	WORD				m_wTyrannyOwner;
	float				m_fTyrannyDamage[TYRANNY_SCHOOL_SIZE];

	DWORD				m_dwTyrannyClubOwner;
	DWORD				m_dwTyrannyClubMarkVer;
	
	/*woe Arc Development 08-06-2024*/
	WORD	m_wWoeOwner;

	/*pvp capture the flag, Juver, 2018/02/06 */
	bool				m_bCaptureTheFlagCrowHide;

	SDROP_CROW () 
		: sMapID(NATIVEID_NULL())
		, dwCeID(0)
		, dwGlobID(0)
		, vPos(0,0,0)
		, vDir(1,0,0)
		, dwNowHP(0)
		, wNowMP(0)
		, dwFLAGS(NULL)
		, emAction(GLAT_IDLE)
		, dwActState(0)
		, m_wTyrannyOwner(TYRANNY_SCHOOL_SIZE)	/*pvp tyranny, Juver, 2017/08/25 */
		, m_bCaptureTheFlagCrowHide(false)		/*pvp capture the flag, Juver, 2018/02/06 */
		, m_dwTyrannyClubOwner(CLUB_NULL)
		, m_dwTyrannyClubMarkVer(0)
		, m_wWoeOwner(CLUB_NULL)	/*woe Arc Development 08-06-2024*/

	{
		/*pvp tyranny, Juver, 2017/08/25 */
		for ( int i=0; i<TYRANNY_SCHOOL_SIZE; ++ i )
			m_fTyrannyDamage[i] = 0.0f;
	}
};

struct SDROP_MATERIAL
{
	enum { CROW_GEM=0x0001, };

	SNATIVEID			sNativeID;	//	Crow 고유 ID.

	SNATIVEID			sMapID;		//	맵 ID.
	DWORD				dwCeID;		//	셀 ID.
	DWORD				dwGlobID;	//	생성 메모리 인덱스용.
	D3DXVECTOR3			vPos;		//	위치.
	D3DXVECTOR3			vDir;		//	방향.

	DWORD				dwFLAGS;	//	기타 속성.
	EMACTIONTYPE		emAction;	//	행동.
	DWORD				dwActState;	//	행동 상태.

	SDROP_MATERIAL () 
		: sMapID(NATIVEID_NULL())
		, dwCeID(0)
		, dwGlobID(0)
		, vPos(0,0,0)
		, vDir(1,0,0)
		, dwFLAGS(NULL)
		, emAction(GLAT_IDLE)
		, dwActState(0)
	{
	}
};


#endif // GLCROWDATA_H_

