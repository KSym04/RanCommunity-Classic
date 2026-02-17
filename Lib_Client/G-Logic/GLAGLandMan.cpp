#include "stdafx.h"
#include "./GLAGLandMan.h"
#include "./GLAgentServer.h"
#include "../../Lib_Engine/DxOctree/DxLandMan.h"

#include "./GLMapList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLAGLandMan::GLAGLandMan(void) :
	m_dwClubMapID(UINT_MAX),
	m_bGuidBattleMap(false),
	m_bClubDeathMatchMap(false),
	m_bClubWarLock(false),			/*clubwar map lock, Juver, 2017/06/27 */
	m_bPVPTyrannyMap(false),		/*pvp tyranny, Juver, 2017/08/24 */
	m_bPVPSchoolWarsMap(false),		/*school wars, Juver, 2018/01/19 */
	m_bPVPCaptureTheFlagMap(false),	/*pvp capture the flag, Juver, 2018/01/24 */

	/* pvp club death match, Juver, 2020/11/26 */
	m_bPVPClubDeathMatchBattleMap(false),	
	m_bPVPClubDeathMatchLobbyMap(false),

	m_bPVPPBGBattleMap(false),
	m_bPVPPBGLobbyMap(false),
	
	m_bPVPWoeMap(false)	/*woe Arc Development 08-06-2024*/
{
}

GLAGLandMan::~GLAGLandMan(void)
{
	CleanUp ();
}

BOOL GLAGLandMan::LoadWldFile ( const char* szWldFile, bool bLandGateLoad )
{
	HRESULT hr;

	m_sLevelHead.m_strWldFile = szWldFile;

	char szPathName[MAX_PATH] = {0};
	StringCchCopy ( szPathName, MAX_PATH, DxLandMan::GetPath() );
	StringCchCat ( szPathName, MAX_PATH, m_sLevelHead.m_strWldFile.c_str() );

	char szFileType[FILETYPESIZE];
	DWORD dwVersion;

	CSerialFile SFile;
	if ( hr = SFile.OpenFile ( FOT_READ, szPathName ) )
	{
		//	Note : 파일 형식 버전확인.
		//
		SFile.GetFileType( szFileType, FILETYPESIZE, dwVersion );

		if ( strcmp(DxLandMan::FILEMARK,szFileType) )
		{
			CDebugSet::ErrorCode( "GLAGLandMan::LoadWldFile", "Incorrect file: %s head expected:%s~result%s", m_sLevelHead.m_strWldFile.c_str(), DxLandMan::FILEMARK, szFileType );
			return FALSE;
		}

		//update NSLANDMAN_SUPPORT for new file support
		if ( !NSLANDMAN_SUPPORT::IsLandManSupported( dwVersion ))
		{
			CDebugSet::ErrorVersion( "GLAGLandMan::LoadWldFile", SFile.GetFileName(), dwVersion );
			return FALSE;
		}

		if ( dwVersion >= DxLandMan::VERSION_ENCODE )
			SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_1_VER_1 );

		if ( dwVersion >= DxLandMan::VERSION_ENCODE_2 )
			SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_1_VER_2 );

		if ( dwVersion >= DxLandMan::VERSION_ENCODE_3 )
			SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_1_VER_3 );

		if ( dwVersion >= DxLandMan::VERSION_ENCODE_4 )
			SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_WLD_1_VER_4 );

		if ( dwVersion >= DxLandMan::ENCODE_VER_BYTE_CRYPT_2021_WLD1_V1 )
			SFile.SetEncodeType( EMRCRYPT_BYTE_TYPE_2021_WLD1_V1 );

		SLAND_FILEMARK sLandMark;

		//	Note : .wld 파일의 맵ID는 사용하지 않음.
		SNATIVEID sMapID;
		SFile >> sMapID.dwID;

		char szMapName[MAXLANDNAME];
		SFile.ReadBuffer ( szMapName, sizeof(char)*MAXLANDNAME );

		sLandMark.LoadSet ( SFile );

		//	Note : 출입구 설정 읽기.
		//
		if ( sLandMark.dwGATE_MARK && bLandGateLoad )
		{
			SFile.SetOffSet ( sLandMark.dwGATE_MARK );
			m_cLandGateMan.Load ( SFile );
		}
	}

	return TRUE;
}

BOOL GLAGLandMan::LoadFile ( const char *szFile )
{
	GASSERT(szFile);

	BOOL bOk(FALSE);
	bOk = GLLevelFile::LoadFile ( szFile, TRUE, NULL );
	if ( !bOk )			return FALSE;

	bOk = LoadWldFile ( m_sLevelHead.m_strWldFile.c_str(), GLLevelFile::GetFileVer()<=VERSION_BEFORE_GATE );
	if ( !bOk )			return FALSE;

	return TRUE;
}

void GLAGLandMan::CleanUp ()
{
	m_setPC.clear();
	m_dwClubMapID			= UINT_MAX;
	m_bGuidBattleMap		= false;
	m_bClubDeathMatchMap	= false;

	m_bClubWarLock			= false;	/*clubwar map lock, Juver, 2017/06/27 */
	m_bPVPTyrannyMap		= false;	/*pvp tyranny, Juver, 2017/08/24 */
	m_bPVPSchoolWarsMap		= false;	/*school wars, Juver, 2018/01/19 */
	m_bPVPCaptureTheFlagMap	= false;	/*pvp capture the flag, Juver, 2018/01/24 */

	/* pvp club death match, Juver, 2020/11/26 */
	m_bPVPClubDeathMatchBattleMap	= false;
	m_bPVPClubDeathMatchLobbyMap	= false;

	m_bPVPPBGBattleMap = false;
	m_bPVPPBGLobbyMap = false;
	
	m_bPVPWoeMap = false; /*woe Arc Development 08-06-2024*/

	m_cLandGateMan.CleanUp ();
}

bool GLAGLandMan::DropPC ( DWORD dwID )
{
	m_setPC.insert ( dwID );

	return true;
}

bool GLAGLandMan::DropOutPC ( DWORD dwID )
{
	SETPC_ITER pos = m_setPC.find ( dwID );
	if ( pos==m_setPC.end() )	return false;

	m_setPC.erase ( pos );

	return true;
}

void GLAGLandMan::SENDTOALLCLIENT ( LPVOID nmg )
{
	PGLCHARAG pCHAR = NULL;
	SETPC_ITER pos = m_setPC.begin();
	SETPC_ITER end = m_setPC.end();
	for ( ; pos!=end; ++pos )
	{
		pCHAR = GLAgentServer::GetInstance().GetChar ( (*pos) );
		if ( !pCHAR )	
		{
			continue;
		}
		else
		{
			GLAgentServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, nmg );
		}
	}
}

/*map party setting, Juver, 2018/06/29 */
void GLAGLandMan::SetFlags( const SMAPNODE* pnode )
{
	if ( !pnode )	return;

	m_bCPReset				= pnode->bCPReset;
	m_bCPResetDeath			= pnode->bCPResetDeath;

	/*map party setting, Juver, 2018/06/29 */
	m_bBlockParty			= pnode->bBlockParty;

	/*force revive, Juver, 2018/07/09 */
	m_bForceRevive			= pnode->bForceRevive;

	/*map move settings, Juver, 2017/11/25 */
	m_bBlockTaxi			= pnode->bBlockTaxi;
	m_bBlockFriendCard		= pnode->bBlockFriendCard;
	m_bBlockRecall			= pnode->bBlockRecall;
	m_bBlockTeleport		= pnode->bBlockTeleport;

	/* open pk function, Juver, 2019/12/23 */
	m_bOpenPK				= pnode->bOpenPK;

	/* death hide, Juver, 2020/02/02 */
	m_bDeathHide			= pnode->bDeathHide;

	/* map entry user verified, Juver, 2020/02/27 */
	m_bUserVerifiedMapEntry	= pnode->bUserVerifiedMapEntry;

	/* costume combine map restriction, Juver, 2020/09/11 */
	m_bCostumeCombineRestrictMoveSpeed		= pnode->bCostumeCombineRestrictMoveSpeed;
	m_bCostumeCombineRestrictAttackSpeed	= pnode->bCostumeCombineRestrictAttackSpeed;

	/* trade restricted, Juver, 2021/06/07 */
	m_bTradeRestricted		= pnode->bTradeRestricted;

	m_bAllowHideDead		= pnode->bAllowHideDead;

	m_bShowPkMark			= pnode->bShowPkMark;

	m_bUseAutoPotion		= pnode->bUseAutoPotion;
	m_bAutoPotionManual		= pnode->bAutoPotionManual;

	m_bUseAutoPilot			= pnode->bUseAutoPilot;

	m_bPKMapCount			= pnode->bPKMapCount;

	m_bUseCaptcha			= pnode->bUseCaptcha;

	m_bBlockGeneralChat		= pnode->bBlockGeneralChat;

	m_bResetSkillDelayOnDie	= pnode->bResetSkillDelayOnDie;
}