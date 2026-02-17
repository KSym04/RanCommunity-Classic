#pragma once

#include "string"

#include "../../Lib_Engine/Common/CList.h"
#include "./GLLandManClient.h"
#include "./GLogicData.h"
#include "./GLCharacter.h"
#include "./GLMapList.h"
#include "../../Lib_Engine/G-Logic/GLCrowTracer.h"
#include "./GLCOPY.h"
#include "../../Lib_Engine/G-Logic/GLActiveMap.h"
#include "./GLPetClient.h"
#include "./GLSummonClient.h"

#include "./GLPVPClubWarData.h"
#include "./GLPartyFinder.h"
#include "./SClubBattleTime.h"

#include "../SGameStageInfo.h"
#include "./GLPKMark.h"
#include "./GLPVPTyrannyData.h"

//#define _SYNC_TEST
/* security 2025 */
DWORD WINAPI StartupIntegrityCheck(LPVOID);
DWORD WINAPI CreatePETINT(LPVOID);
DWORD WINAPI CreatePETPOW(LPVOID);
DWORD WINAPI CreatePETDEX(LPVOID);



class GLGaeaClient : public GLMapList , public GLCrowTracer, public GLActiveMap
{
protected:
	char						m_szAppPath[MAX_PATH];
	HWND						m_hWnd;

	float						m_fAge;
	LPDIRECT3DDEVICEQ			m_pd3dDevice;
	PLANDMANCLIENT				m_pLandMClient;

	GLCharacter					m_Character;
	GLPetClient					m_Pet;
	
	DETECTMAP					m_vecDetectID;
	D3DXVECTOR3					m_vCharPos;

	bool						m_bSCHOOL_FREEPK;	// 학원간 자유 PK
	bool						m_bBRIGHTEVENT;		// 성향 이벤트
	/*dmk14 freepk*/
	SNATIVEID					m_sBrightEventMap;
	
	CTime						m_cServerTime;			// 서버의 현재 시간
	int							m_nServerTimeBias;		// 서버 TimeZone ( +Hour )

	char						m_szShopInfoFile[MAX_PATH];

public:
	bool						m_bClubDeathMatch;
	float						m_fClubDeathMatchTimer;
	CLUB_BATTLE_TIMER_MAP		m_mapCLUBBATTLETimer;

	bool						m_bFreePKMatch;
	float						m_fFreePKMatchTimer;

public:
	/*skill summon, Juver, 2017/10/08 */
	GLSummonClient				m_Summon[SKILL_SUMMON_MAX_CLIENT_NUM];

public:
	CDM_RANK_VEC				m_vecCdmRank;
	SCDM_RANK_EX				m_sMyCdmRank;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	TYRANNY_CLUB_RANK_VEC				m_vecTyrannyClubRank;
	STYRANNY_CLUB_RANK_EX				m_sMyTyrannyClubRank;

	TYRANNY_RANK_VEC					m_vecTyrannyRank;
	STYRANNY_RANK_EX					m_sMyTyrannyRank;

	TYRANNY_RANK_RESU_VEC				m_vecTyrannyRankResu;
	STYRANNY_RANK_RESU_EX				m_sMyTyrannyRankResu;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	CLUBWAR_CLUB_RANK_VEC				m_vecClubWarClubRank;
	SCLUBWAR_CLUB_RANK_EX				m_sMyClubWarClubRank;

	CLUBWAR_RANK_VEC					m_vecClubWarRank;
	SCLUBWAR_RANK_EX					m_sMyClubWarRank;

	CLUBWAR_RANK_RESU_VEC				m_vecClubWarRankResu;
	SCLUBWAR_RANK_RESU_EX				m_sMyClubWarRankResu;
	///*character render setting, Juver, 2019/03/22 */
	//float						m_fRenderSettingChangeTimer;

	SPVP_CLUB_WAR_CLIENT_DATA_MAP	m_mapClubWarData;

	/* party finder, Juver, 2020/01/11 */
	SPARTY_FINDER_RESULT_VEC	m_vecPartyFinderResult;

	TG_RANK_VEC					m_vecTGRank;
	STG_RANK_EX					m_sMyTGRank;

	VEC_BATTLE_RANKING			m_vecBattleRankingP;
	VEC_BATTLE_RANKING			m_vecBattleRankingC;
	/*woe Arc Development 08-06-2024*/
	WOE_GUILD_RANK_VEC			m_vecWoeGuild;
	SWOE_GUILD_RANK_EX			m_sMyWoeRank;
	WOE_PLAYER_RANK_VEC			m_vecWoePlayerRank;
	SWOE_PLAYER_RANK_EX			m_sSelfWoePlayerRank;
	WOE_RANK_RESU_VEC			m_vecWoeResuRank;
	SWOE_RESU_RANK_EX			m_sSelfWoeResuRank;
public:
	bool IsSchoolFreePk ()		{ return m_bSCHOOL_FREEPK; }
	/*dmk14 freepk*/
	//bool IsBRIGHTEVENT ()		{ return m_bBRIGHTEVENT; }
	bool IsBRIGHTEVENT();

	bool IsClubDeathMatch()		{ return m_bClubDeathMatch; }

public:
	HRESULT OneTimeSceneInit ( const char* szAppPath, HWND hWnd );
	HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RestoreDeviceObjects ();

	HRESULT InvalidateDeviceObjects ();
	HRESULT DeleteDeviceObjects ();

	HRESULT FrameMove ( float fTime, float fElapsedTime, SGameStageInfo& sInfo );
	
	HRESULT Render ( CLIPVOLUME &CV, SGameStageInfo& sInfo );
	HRESULT RenderEff ( CLIPVOLUME &CV );
	void	RenderPickAlpha( CLIPVOLUME &CV );

protected:
	PLANDMANCLIENT CreateLandMClient ( char* szLandFileName, D3DXVECTOR3 &vBasicPos );
	PLANDMANCLIENT CreateLandMClient ( SNATIVEID sMapID, D3DXVECTOR3 &vBasicPos );
	PLANDMANCLIENT CreateInstantMapClient ( SNATIVEID sBaseMapID, SNATIVEID sInstantMapID, D3DXVECTOR3 &vBasicPos );

protected:
	HRESULT MoveActiveMap ( SNATIVEID sMapID, D3DXVECTOR3 &vPos );
	HRESULT CreateInstantMap ( SNATIVEID sBaseMapID, SNATIVEID sInstantMapID, D3DXVECTOR3 &vBasicPos );

public:
	HRESULT SetActiveMap ( char* szLandFileName, D3DXVECTOR3 &vBasicPos );
	HRESULT SetActiveMap ( SNATIVEID sMapID, D3DXVECTOR3 &vBasicPos );
	PLANDMANCLIENT GetActiveMap ();
	SNATIVEID GetActiveMapID ();

	HRESULT MoveActiveMap ( NET_MSG_GENERIC *nmg );
	HRESULT CreateInstantMap ( NET_MSG_GENERIC *nmg );
	HRESULT DropOutCrow ( NET_MSG_GENERIC *nmg );
	HRESULT ReBirthFB ( NET_MSG_GENERIC* nmg );
	HRESULT ReCallFB ( NET_MSG_GENERIC* nmg );
	HRESULT ReLvMustLeaveFB ( NET_MSG_GENERIC* nmg );
	HRESULT ReqBusFB ( NET_MSG_GENERIC* nmg );
	HRESULT ReqTaxiFB ( NET_MSG_GENERIC* nmg );
	HRESULT TeleportFB ( NET_MSG_GENERIC* nmg );

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	HRESULT ReqTyrannyClubRankInfo();
	HRESULT ReqTyrannyRankInfo();
	HRESULT ReqTyrannyRankResuInfo();

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	HRESULT ReqClubWarClubRankInfo();
	HRESULT ReqClubWarRankInfo();
	HRESULT ReqClubWarRankResuInfo();
public:
	HRESULT ReqClubDeathMatchInfo();
	/*woe Arc Development 08-06-2024*/
	HRESULT ReqWoeGuildRankingInfo();
	HRESULT ReqWoePlayerRankingInfo();
	HRESULT ReqWoeResuRankingInfo();


public:
	DWORD GetMobNameColor ( DWORD dwGlobID );
	BOOL IsMapCollsion( D3DXVECTOR3& vTargetPt, D3DXVECTOR3& vFromPt );

public:
	GLCOPY* GetCopyActor ( const STARGETID &sTARID )	{ return GetCopyActor ( sTARID.emCrow, sTARID.dwID ); }
	GLCOPY* GetCopyActor ( const EMCROW emCrow, const DWORD dwID );
	GLCOPY* GetCopyActor ( const std::string &strName );
	GLCharacter* GetCharacter ()						{ return &m_Character; }
	const D3DXVECTOR3 &GetCharacterPos ()				{ return m_Character.GetPosition(); }
	SCHARDATA2& GetCharacterData ()						{ return m_Character.GetCharData(); }
	GLCHARLOGIC_CLIENT& GetCharacterLogic ()				{ return m_Character.GetCharLogic(); }

	PGLCHARCLIENT GetChar ( DWORD dwID );

	BOOL ValidCheckTarget ( const STARGETID & sTargetID );
	DETECTMAP* DetectCrowDie ( const D3DXVECTOR3 &vFrom, const D3DXVECTOR3 &vTarget );
	DETECTMAP* DetectCrow ( DWORD emCrow, const D3DXVECTOR3 &vFrom, const D3DXVECTOR3 &vTarget );
	DETECTMAP* DetectCrow ( DWORD emCrow, const D3DXVECTOR3 &vTarPos, const float fLength );
	BOOL IsCollisionMobToPoint ( const D3DXVECTOR3 &vPoint, const WORD wBodyRadius );

	D3DXVECTOR3* FindCharPos ( std::string strName );
	D3DXVECTOR3* FindCharHeadPos ( std::string strName );
	D3DXVECTOR3* FindCharHeadPos ( DWORD dwGaeaID );
	D3DXVECTOR3* FindMaterialHeadPos ( DWORD dwGaeaID );

	const CTime& GetCurrentTime() const { return m_cServerTime; }
	void SetCurrentTime(__time64_t tServerTime) { m_cServerTime = tServerTime; }
	void GetConvertServerTime( CTime& cTime );
	void SetServerTimeBias( int nTimeBias )	{ m_nServerTimeBias = nTimeBias; }

	// PET
	GLPetClient* GetPetClient () { return &m_Pet; }
	HRESULT		 CreatePET ( NET_MSG_GENERIC* nmg );
	void		 PetSkinPackApplyEffect();

	// SUMMON
	/*skill summon, Juver, 2017/10/08 */
	GLSummonClient* GetSummonClient( DWORD dwguid );// { return &m_Summon; }
	HRESULT			CreateSummon( NET_MSG_GENERIC* nmg );
	PGLANYSUMMON	GetSummon ( DWORD dwID );

public:

	///*character render setting, Juver, 2019/03/22 */
	//bool	IsChangeRenderReady();
	//void	ChangeRenderSettings();

	/* character simplify, Juver, 2021/07/29 */
	void UpdateSuitClientActor();

	void PVPClubWarsRankClear();
	void PVPClubWarsRankStart( NET_MSG_GENERIC* nmg );
	void PVPClubWarsRankUpdate( NET_MSG_GENERIC* nmg );
	void PVPClubWarsRankEnd( NET_MSG_GENERIC* nmg );
	bool PVPClubWarsRankValid();

	/* party finder, Juver, 2020/01/19 */
	void PartyFinderReset();

	/* pvp club death match, Juver, 2020/11/26 */
	HRESULT PVPClubDeathMatchReBirthFB ( NET_MSG_GENERIC* nmg );

	HRESULT PVPPBGReBirthFB ( NET_MSG_GENERIC* nmg );

public:
	void MsgProcess ( NET_MSG_GENERIC* nmg );

public:
	virtual DxLandMan* getactivemap()
	{
		PLANDMANCLIENT pMapClient = GetActiveMap ();
		if ( !pMapClient )	return NULL;

		return pMapClient->GetLandMan();
	}

public:
	virtual BOOL IsVisibleCV ( const STARGETID & sTargetID );
	virtual D3DXVECTOR3 GetTargetPos ( const STARGETID &sTargetID );
	virtual DxSkinChar* GetSkinChar ( const STARGETID &sTargetID );
	virtual float GetCrowDir ( const STARGETID &sTargetID );

protected:
	GLGaeaClient(void);

public:
	~GLGaeaClient(void);
	static GLGaeaClient& GetGaeaClient() 
	{
        static GLGaeaClient instance;
        return instance;
    }

public:
	static GLGaeaClient& GetInstance();
};

