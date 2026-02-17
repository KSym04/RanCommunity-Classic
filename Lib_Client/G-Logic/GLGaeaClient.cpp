#include "stdafx.h"
#include "shlobj.h"
#include "../../Lib_Engine/Common/SUBPATH.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"

#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"

//#include <strstream>
#include "../../Lib_Engine/DxCommon/DxShadowMap.h"
#include "../../Lib_Engine/DxCommon/DxSoftShadowMap.h"
#include "../DxGlobalStage.h"
#include "../../Lib_Engine/DxCommon/DxViewPort.h"
#include "../../Lib_Engine/DxCommon/COLLISION.h"
#include "./stl_Func.h"
#include "../../Lib_Engine/Meshs/DxSimpleMeshMan.h"
#include "./GLItemMan.h"
#include "../../Lib_Engine/DxEffect/DxEffectMan.h"
#include "../../Lib_Engine/G-Logic/GLPeriod.h"
#include "../../Lib_Engine/DxResponseMan.h"
#include "../../Lib_ClientUI/Interface/LoadingThread.h"
#include "../../Lib_Engine/Meshs/DxSkinMeshMan.h"
#include "./GLCrowRenList.h"
#include "./GLPartyClient.h"
#include "../../Lib_Engine/DxCommon/DxClubMan.h"
#include "../../Lib_Engine/DxSound/DxSoundLib.h"
#include "../../Lib_Engine/Common/stringutils.h"
#include "../../Lib_Engine/DxEffect/Single/DxEffGroupPlayer.h"
#include "./GLFriendClient.h"
#include "./GLGaeaClient.h"
#include "../../Lib_ClientUI/Interface/MiniMap.h"
#include "../../Lib_ClientUI/Interface/ModalCallerID.h"
#include "../../Lib_ClientUI/Interface/MapRequireCheck.h"
#include "../../Lib_ClientUI/Interface/ModalWindow.h"
#include "../../Lib_ClientUI/Interface/DamageDisplay.h"
#include "./GLFactEffect.h"


/*pvp tyranny, Juver, 2017/08/24 */
#include "./GLPVPTyrannyClient.h" 

/*school wars, Juver, 2018/01/19 */
#include "./GLPVPSchoolWarsClient.h"
/*woe Arc Development 08-06-2024*/
#include "./GLPVPWoeClient.h"

/*pvp capture the flag, Juver, 2018/02/01 */
#include "./GLPVPCaptureTheFlagClient.h"

/*skill transform, Juver, 2018/09/09 */
#include "./GLTransformSkinSet.h"

/*pk combo, EJCode, 2018/11/15 */
#include "./GLPKCombo.h"

#include "./PVPClubDeathMatchClient.h"

#include "../../Lib_Engine/Core/NSRParam.h"
#include "./RTimerDefine.h"

#include "../../Lib_Helper/CommonContainer.h"

#include "../../Lib_Engine/Hash/CHash.h"
#include "../../Lib_Helper/RPCID/RPCIDSMBIOS.h"
#include "../../Lib_Helper/RPCID/RPCIDMAC.h"

/* post system, Juver, 2021/02/05 */
#include "./GLPostClient.h"

#include "./GLPKMark.h"

#include "./PVPPartyBattleGroundsClient.h"

#include "../Lib_ClientUI/Interface/BattleRankingDisplay.h"
/*security 2025*/
#include "../Lib_ClientUI/Interface/OuterInterface.h"
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL g_bPREVIEW_CHAR_RENDER;
/* security 2025 */
void disconnect ()
{
	DxLobyStage* pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
	if (pLobyStage)
    {
        UIGUID ControlID = COuterInterface::GetInstance().GetRenderStage();

        
        bool bIsInLobby = 
            (ControlID == SELECT_CHARACTER_PAGE) ||
            (ControlID == CHARACTER_CREATE_PAGE) ||
            (ControlID == LOGIN_PAGE) ||
            (ControlID == SELECT_SERVER_PAGE) ||
            (ControlID == SELECT_CHINA_AREA_PAGE) ||
            (ControlID == PASS_KEYBOARD) ||
            (ControlID == SECPASS_SETPAGE) ||
            (ControlID == SECPASS_CHECKPAGE) ||
            (ControlID == REGISTER_PAGE) ||
            (ControlID == ANNOUNCEMENT_PAGE);

        if (bIsInLobby)
        {			
			ExitProcess(0);
            return;
        }
    }
	GLCharacter* pchar = GLGaeaClient::GetInstance().GetCharacter();
	if (!pchar)	return;

	GLMSG::SNET_DETECT_CHEAT_C2A netmsg;
	netmsg.dwCharID = pchar->m_dwCharID;
	NETSEND(&netmsg);

}

std::vector<std::string> CheatEngineStrings;

void InitCheatEngineStrings()
{
    CheatEngineStrings.clear();

	std::string rc1 = 
    std::string(1, static_cast<char>(48)) + // '0'
    std::string(1, static_cast<char>(48)) + // '0'
    std::string(1, static_cast<char>(48)) + // '0'
    std::string(1, static_cast<char>(48)) + // '0'
    std::string(1, static_cast<char>(48)) + // '0'
    std::string(1, static_cast<char>(48)) + // '0'
    std::string(1, static_cast<char>(48)) + // '0'
    std::string(1, static_cast<char>(48));	// '0'
    CheatEngineStrings.push_back(rc1);

	std::string rc2 = 
    std::string(1, static_cast<char>(48)) +  // '0'
    std::string(1, static_cast<char>(48)) +  // '0'
    std::string(1, static_cast<char>(48)) +  // '0'
    std::string(1, static_cast<char>(48)) +  // '0'
    std::string(1, static_cast<char>(55)) +  // '7'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102));  // 'f'
	CheatEngineStrings.push_back(rc2);


	std::string	 rc3 = 
	std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
	std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
	std::string(1, static_cast<char>(102));	// 'f'
    CheatEngineStrings.push_back(rc3);
	
	std::string rc4 =
    std::string(1, static_cast<char>(77)) +  // 'M'
    std::string(1, static_cast<char>(101)) + // 'e'
    std::string(1, static_cast<char>(109)) + // 'm'
    std::string(1, static_cast<char>(111)) + // 'o'
    std::string(1, static_cast<char>(114)) + // 'r'
    std::string(1, static_cast<char>(121)) + // 'y'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(83)) +  // 'S'
    std::string(1, static_cast<char>(99)) +  // 'c'
    std::string(1, static_cast<char>(97)) +  // 'a'
    std::string(1, static_cast<char>(110)) + // 'n'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(79)) +  // 'O'
    std::string(1, static_cast<char>(112)) + // 'p'
    std::string(1, static_cast<char>(116)) + // 't'
    std::string(1, static_cast<char>(105)) + // 'i'
    std::string(1, static_cast<char>(111)) + // 'o'
    std::string(1, static_cast<char>(110)) + // 'n'
    std::string(1, static_cast<char>(115));  // 's'
    CheatEngineStrings.push_back(rc4);
	
	std::string rc5 =
    std::string(1, static_cast<char>(67)) +  // 'C'
    std::string(1, static_cast<char>(114)) + // 'r'
    std::string(1, static_cast<char>(121)) + // 'y'
    std::string(1, static_cast<char>(83)) +  // 'S'
    std::string(1, static_cast<char>(101)) + // 'e'
    std::string(1, static_cast<char>(97)) +  // 'a'
    std::string(1, static_cast<char>(114)) + // 'r'
    std::string(1, static_cast<char>(99)) +  // 'c'
    std::string(1, static_cast<char>(104)) + // 'h'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(77)) +  // 'M'
    std::string(1, static_cast<char>(101)) + // 'e'
    std::string(1, static_cast<char>(109)) + // 'm'
    std::string(1, static_cast<char>(111)) + // 'o'
    std::string(1, static_cast<char>(114)) + // 'r'
    std::string(1, static_cast<char>(121)) + // 'y'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(83)) +  // 'S'
    std::string(1, static_cast<char>(99)) +  // 'c'
    std::string(1, static_cast<char>(97)) +  // 'a'
    std::string(1, static_cast<char>(110)) + // 'n'
    std::string(1, static_cast<char>(110)) + // 'n'
    std::string(1, static_cast<char>(101)) + // 'e'
    std::string(1, static_cast<char>(114));  // 'r'
    CheatEngineStrings.push_back(rc5);

	std::string rc6 =
    std::string(1, static_cast<char>(112)) + // 'p'
    std::string(1, static_cast<char>(97)) +  // 'a'
    std::string(1, static_cast<char>(117)) + // 'u'
    std::string(1, static_cast<char>(115)) + // 's'
    std::string(1, static_cast<char>(101)) + // 'e'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(116)) + // 't'
    std::string(1, static_cast<char>(104)) + // 'h'
    std::string(1, static_cast<char>(101)) + // 'e'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(103)) + // 'g'
    std::string(1, static_cast<char>(97)) +  // 'a'
    std::string(1, static_cast<char>(109)) + // 'm'
    std::string(1, static_cast<char>(101)) + // 'e'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(119)) + // 'w'
    std::string(1, static_cast<char>(104)) + // 'h'
    std::string(1, static_cast<char>(105)) + // 'i'
    std::string(1, static_cast<char>(108)) + // 'l'
    std::string(1, static_cast<char>(101)) + // 'e'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(115)) + // 's'
    std::string(1, static_cast<char>(99)) +  // 'c'
    std::string(1, static_cast<char>(97)) +  // 'a'
    std::string(1, static_cast<char>(110)) + // 'n'
    std::string(1, static_cast<char>(110)) + // 'n'
    std::string(1, static_cast<char>(105)) + // 'i'
    std::string(1, static_cast<char>(110)) + // 'n'
    std::string(1, static_cast<char>(103));  // 'g'
    CheatEngineStrings.push_back(rc6);

	std::string	 rc7 = 
	std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
	std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)) + // 'f'
    std::string(1, static_cast<char>(102)); // 'f'
    CheatEngineStrings.push_back(rc7);

	std::string	 rc8 = 
	std::string(1, static_cast<char>(83)) +  // 'S'
    std::string(1, static_cast<char>(117)) + // 'u'
    std::string(1, static_cast<char>(98)) +  // 'b'
    std::string(1, static_cast<char>(109)) + // 'm'
    std::string(1, static_cast<char>(105)) + // 'i'
    std::string(1, static_cast<char>(116)) + // 't'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(66)) +  // 'B'
    std::string(1, static_cast<char>(117)) + // 'u'
    std::string(1, static_cast<char>(103));  // 'g'
    CheatEngineStrings.push_back(rc8);
	
	std::string rc9 =	
	std::string(1, static_cast<char>(67)) +  // 'C'
    std::string(1, static_cast<char>(111)) + // 'o'
    std::string(1, static_cast<char>(100)) + // 'd'
    std::string(1, static_cast<char>(101)) + // 'e'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(66)) +  // 'B'
    std::string(1, static_cast<char>(121)) + // 'y'
    std::string(1, static_cast<char>(32)) +  // ' '
    std::string(1, static_cast<char>(99)) +  // 'c'
    std::string(1, static_cast<char>(109)) + // 'm'
    std::string(1, static_cast<char>(109)) + // 'm'
    std::string(1, static_cast<char>(76)) +  // 'L'
    std::string(1, static_cast<char>(107)) + // 'k'
    std::string(1, static_cast<char>(99));   // 'c'
    CheatEngineStrings.push_back(rc9);
}

std::string IsScanButton(HWND hwnd)
{
    WCHAR windowText[256];
    if (GetWindowTextW(hwnd, windowText, sizeof(windowText) / sizeof(WCHAR)))
    {
        std::wstring text(windowText);
		std::vector<std::wstring> scanTerms;

		scanTerms.push_back(
			std::wstring() +
			static_cast<wchar_t>(71) +  // 'G'
			static_cast<wchar_t>(90) +  // 'Z'
			static_cast<wchar_t>(32) +  // ' '
			static_cast<wchar_t>(49) +  // '1'
			static_cast<wchar_t>(115) + // 's'
			static_cast<wchar_t>(116) + // 't'
			static_cast<wchar_t>(83) +  // 'S'
			static_cast<wchar_t>(99) +  // 'c'
			static_cast<wchar_t>(97) +  // 'a'
			static_cast<wchar_t>(110)   // 'n'
		);

		scanTerms.push_back(
			std::wstring() +
			static_cast<wchar_t>(71) +  // 'G'
			static_cast<wchar_t>(90) +  // 'Z'
			static_cast<wchar_t>(32) +  // ' '
			static_cast<wchar_t>(78) +  // 'N'
			static_cast<wchar_t>(120) + // 'x'
			static_cast<wchar_t>(116) + // 't'
			static_cast<wchar_t>(83) +  // 'S'
			static_cast<wchar_t>(99) +  // 'c'
			static_cast<wchar_t>(97) +  // 'a'
			static_cast<wchar_t>(110)   // 'n'
		);

		scanTerms.push_back(
			std::wstring() +
			static_cast<wchar_t>(70) +  // 'F'
			static_cast<wchar_t>(73) +  // 'I'
			static_cast<wchar_t>(82) +  // 'R'
			static_cast<wchar_t>(83) +  // 'S'
			static_cast<wchar_t>(84) +  // 'T'
			static_cast<wchar_t>(32)    // ' '
		);

		scanTerms.push_back(
			std::wstring() +
			static_cast<wchar_t>(78) +  // 'N'
			static_cast<wchar_t>(69) +  // 'E'
			static_cast<wchar_t>(88) +  // 'X'
			static_cast<wchar_t>(84) +  // 'T'
			static_cast<wchar_t>(32)    // ' '
		);

		scanTerms.push_back(
			std::wstring() +
			static_cast<wchar_t>(78) +  // 'N'
			static_cast<wchar_t>(101) + // 'e'
			static_cast<wchar_t>(120) + // 'x'
			static_cast<wchar_t>(116) + // 't'
			static_cast<wchar_t>(32) +  // ' '
			static_cast<wchar_t>(83) +  // 'S'
			static_cast<wchar_t>(99) +  // 'c'
			static_cast<wchar_t>(97) +  // 'a'
			static_cast<wchar_t>(110)  // 'n'
		);

		scanTerms.push_back(
			std::wstring() +
			static_cast<wchar_t>(70) +  // 'F'
			static_cast<wchar_t>(105) + // 'i'
			static_cast<wchar_t>(114) + // 'r'
			static_cast<wchar_t>(115) + // 's'
			static_cast<wchar_t>(116) + // 't'
			static_cast<wchar_t>(32) +  // ' '
			static_cast<wchar_t>(83) +  // 'S'
			static_cast<wchar_t>(99) +  // 'c'
			static_cast<wchar_t>(97) +  // 'a'
			static_cast<wchar_t>(110)   // 'n'
		);

		scanTerms.push_back(
			std::wstring() +
			static_cast<wchar_t>(98) +  // 'b'
			static_cast<wchar_t>(111) + // 'o'
			static_cast<wchar_t>(115) + // 's'
			static_cast<wchar_t>(115) + // 's'
			static_cast<wchar_t>(98) +  // 'b'
			static_cast<wchar_t>(32) +  // ' '
			static_cast<wchar_t>(86) +  // 'V'
			static_cast<wchar_t>(105) + // 'i'
			static_cast<wchar_t>(101) + // 'e'
			static_cast<wchar_t>(119)   // 'w'
		);

		scanTerms.push_back(
			std::wstring() +
			static_cast<wchar_t>(77) +  // 'M'
			static_cast<wchar_t>(101) + // 'e'
			static_cast<wchar_t>(109) + // 'm'
			static_cast<wchar_t>(111) + // 'o'
			static_cast<wchar_t>(114) + // 'r'
			static_cast<wchar_t>(121) + // 'y'
			static_cast<wchar_t>(32) +  // ' '
			static_cast<wchar_t>(86) +  // 'V'
			static_cast<wchar_t>(105) + // 'i'
			static_cast<wchar_t>(101) + // 'e'
			static_cast<wchar_t>(119)   // 'w'
		);


        std::vector<std::wstring>::iterator it;
        for (it = scanTerms.begin(); it != scanTerms.end(); ++it)
        {
            if (text.find(*it) != std::wstring::npos)
            {
                return std::string(it->begin(), it->end());
            }
        }
    }

    return "";
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM)
{
    wchar_t buffer[128];
    if (GetWindowTextW(hwnd, buffer, sizeof(buffer) / sizeof(buffer[0])))
    {
        std::wstring windowText = buffer;

        
        size_t i;
        for (i = 0; i < windowText.length(); ++i)
        {
            windowText[i] = towlower(windowText[i]);
        }

        std::string detectedScanStr = IsScanButton(hwnd);
        if (!detectedScanStr.empty())
        {
            ExitProcess(0); 
            return FALSE;
        }

        std::vector<std::string>::const_iterator it;
        for (it = CheatEngineStrings.begin(); it != CheatEngineStrings.end(); ++it)
        {
            std::string lowerSearch = *it;
            for (i = 0; i < lowerSearch.length(); ++i)
            {
                lowerSearch[i] = (char)tolower(lowerSearch[i]);
            }

            std::wstring searchW(lowerSearch.begin(), lowerSearch.end());
            if (windowText.find(searchW) != std::wstring::npos)
            {
                ExitProcess(0); 
                return FALSE;
            }
        }
    }

    return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    if (GetWindowPlacement(hwnd, &wp))
    {
        if (wp.showCmd == SW_SHOWMINIMIZED)
            return TRUE;
    }

    EnumChildWindows(hwnd, EnumChildProc, 0);
    return TRUE;
}


void StartWindowEnumeration()
{
    InitCheatEngineStrings();
    EnumWindows(EnumWindowsProc, 0);
}


DWORD WINAPI CreatePETPOW(LPVOID)
{
    while (true)
    {
        StartWindowEnumeration();
        Sleep(5000); // Every 5 seconds
    }
    return 0;
}

DWORD WINAPI CreatePETINT(LPVOID)
{
    const BYTE* startAddress = (BYTE*)0x00400000;
    const BYTE* endAddress = (BYTE*)0x00900000;
    SIZE_T regionSize = endAddress - startAddress;

    std::vector<BYTE> originalDump(regionSize);
    SIZE_T bytesRead;

    if (!ReadProcessMemory(GetCurrentProcess(), startAddress, &originalDump[0], regionSize, &bytesRead))
    {
        DxGlobalStage::GetInstance().GetNetClient()->CloseConnect();
        DxGlobalStage::GetInstance().CloseGame();
        return 0;
    }

    DWORD lastTick = GetTickCount();

    while (true)
    { 
        std::vector<BYTE> currentDump(regionSize);
        SIZE_T currentRead;

        if (!ReadProcessMemory(GetCurrentProcess(), startAddress, &currentDump[0], regionSize, &currentRead))
        {
            disconnect(); 
            return 0;
        }

        if (memcmp(&originalDump[0], &currentDump[0], regionSize) != 0)
        {
            disconnect(); 
            return 0;
        }

        Sleep(15000); 
    }

    return 0;
}

DWORD WINAPI CreatePETDEX(LPVOID)
{
    const BYTE* baseAddr = (BYTE*)0x00400000;
    SIZE_T size = 0x00500000;

    std::vector<BYTE> baseline(size);
    SIZE_T bytesRead;

    if (!ReadProcessMemory(GetCurrentProcess(), baseAddr, &baseline[0], size, &bytesRead))
    {
        ExitProcess(0);
        return 0;
    }

    DWORD startTime = GetTickCount();
    const DWORD runDuration = 5 * 60 * 1000;

    DWORD suspendDetectedStart = 0;

    while (GetTickCount() - startTime < runDuration)
    {
        std::vector<BYTE> current(size);
        SIZE_T currentRead;

        if (!ReadProcessMemory(GetCurrentProcess(), baseAddr, &current[0], size, &currentRead))
        {
            ExitProcess(0);
            break;
        }

        if (memcmp(&baseline[0], &current[0], size) != 0)
        {
            ExitProcess(0);
            break;
        }

        Sleep(500);
    }

    return 0;
}

GLGaeaClient& GLGaeaClient::GetInstance()
{
	static GLGaeaClient Instance;
	return Instance;
}

GLGaeaClient::GLGaeaClient(void) 
	: m_hWnd(NULL)
	
	, m_fAge(0.0f)
	, m_pd3dDevice(NULL)
	, m_pLandMClient(NULL)
	, m_vCharPos(0,0,0)
	, m_bSCHOOL_FREEPK(false)
	, m_bBRIGHTEVENT(false)
	/*dmk14 freepk*/
	, m_sBrightEventMap(NATIVEID_NULL())

	, m_bClubDeathMatch(false)
	, m_fClubDeathMatchTimer(0.0f)
	, m_nServerTimeBias(0)
	//, m_fRenderSettingChangeTimer(RENDER_CHANGE_SETTING_DEFAULT)

	, m_bFreePKMatch( false )
	, m_fFreePKMatchTimer(0.0f)
{
	memset(m_szAppPath, 0, sizeof(char) * (MAX_PATH));
	memset(m_szShopInfoFile, 0, sizeof(char) * (MAX_PATH));
	m_vecDetectID.reserve(200);

	m_cServerTime      = CTime::GetCurrentTime();

	m_mapCLUBBATTLETimer.clear();
}

GLGaeaClient::~GLGaeaClient(void)
{
	//	Note : 이전 맵을 클리어.
	//    
	SAFE_DELETE(m_pLandMClient);
}

HRESULT GLGaeaClient::OneTimeSceneInit ( const char* szAppPath, HWND hWnd )
{
	m_hWnd = hWnd;
	StringCchCopy ( m_szAppPath, MAX_PATH, szAppPath );

	return S_OK;
}

HRESULT	GLGaeaClient::CreatePET ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPET_REQ_USEPETCARD_FB* pNetMsg = ( GLMSG::SNETPET_REQ_USEPETCARD_FB* ) nmg;

	GLPET sPet;
	
	sPet.m_emTYPE			= pNetMsg->m_emTYPE;
	sPet.m_dwGUID			= pNetMsg->m_dwGUID;
	sPet.m_sPetID			= pNetMsg->m_sPetID;

	/*dual pet skill, Juver, 2017/12/27 */
	sPet.m_sActiveSkillID_A	= pNetMsg->m_sActiveSkillID_A;
	sPet.m_sActiveSkillID_B	= pNetMsg->m_sActiveSkillID_B;
	sPet.m_bDualSkill		= pNetMsg->m_bDualSkill;

	sPet.m_dwOwner			= pNetMsg->m_dwOwner;
	sPet.m_wStyle			= pNetMsg->m_wStyle;
	sPet.m_wColor			= pNetMsg->m_wColor;
	sPet.m_fWalkSpeed		= pNetMsg->m_fWalkSpeed;
	sPet.m_fRunSpeed		= pNetMsg->m_fRunSpeed;
	sPet.m_nFull			= pNetMsg->m_nFull;
	sPet.m_sMapID			= pNetMsg->m_sMapID;
	sPet.m_dwCellID			= pNetMsg->m_dwCellID;
	StringCchCopy ( sPet.m_szName, PETNAMESIZE+1, pNetMsg->m_szName );

	sPet.m_dwPetID			= pNetMsg->m_dwPetID;

	WORD i(0);
	for ( ; i < pNetMsg->m_wSkillNum; ++i )
	{
		sPet.m_ExpSkills.insert ( std::make_pair ( pNetMsg->m_Skills[i].sNativeID.dwID, 
			pNetMsg->m_Skills[i] ) );
	}

	for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
	{
		sPet.m_PutOnItems[i] = pNetMsg->m_PutOnItems[i];
	}

	// 주인 옆에 PET 생성
	HRESULT hr = m_Pet.Create ( &sPet, pNetMsg->m_vPos, pNetMsg->m_vDir, m_pLandMClient->GetNaviMesh(), m_pd3dDevice );

	// 실패처리 ( 서버에 생성된 팻을 삭제하도록 )
	if ( FAILED(hr) )
	{
		SINVENITEM* pInvenItem = m_Character.m_cInventory.FindItemByGenNumber ( 
			m_Character.m_llPetCardGenNum, m_Character.m_sPetCardNativeID, m_Character.m_cPetCardGenType );
		if ( pInvenItem )
		{
			GLMSG::SNETPET_REQ_UNUSEPETCARD NetMsg;
			NetMsg.dwGUID = sPet.m_dwGUID;
			NetMsg.dwPetID = sPet.m_dwPetID;
			NETSENDTOFIELD ( &NetMsg );
		}
		// 실패시 GenNum 처리
		m_Character.m_llPetCardGenNum = 0;

		return FALSE;
	}

	// 생성 효과
	D3DXMATRIX matEffect;
	D3DXVECTOR3 vPos = m_Pet.GetPosition ();
	D3DXMatrixTranslation ( &matEffect, vPos.x, vPos.y, vPos.z );

	std::string strGEN_EFFECT = GLCONST_CHAR::strPET_GEN_EFFECT.c_str();
	STARGETID sTargetID(CROW_PET,m_Pet.m_dwGUID,vPos);
	DxEffGroupPlayer::GetInstance().NewEffGroup
	(
		strGEN_EFFECT.c_str(),
		matEffect,
		&sTargetID
	);

	// 선택된 스킬이 있다면 구동시켜 준다
	/*dual pet skill, Juver, 2017/12/27 */
	if ( m_Pet.m_sActiveSkillID_A != NATIVEID_NULL() )
		m_Pet.ReqChangeActiveSkill_A ( m_Pet.m_sActiveSkillID_A );

	/*dual pet skill, Juver, 2017/12/27 */
	if ( m_Pet.m_sActiveSkillID_B != NATIVEID_NULL() )
		m_Pet.ReqChangeActiveSkill_B ( m_Pet.m_sActiveSkillID_B );

	return S_OK;
}


PLANDMANCLIENT GLGaeaClient::CreateLandMClient ( char* szLandFileName, D3DXVECTOR3 &vBasicPos )
{
	GASSERT(szLandFileName);
	GASSERT(m_pd3dDevice&&"m_pd3dDevice 가 초기화 되지 않았습니다.");

	PLANDMANCLIENT pLandMClient = NULL;

	pLandMClient = new GLLandManClient;
	pLandMClient->Create ( szLandFileName, m_pd3dDevice, vBasicPos );

	return pLandMClient;
}

PLANDMANCLIENT GLGaeaClient::CreateLandMClient ( SNATIVEID sMapID, D3DXVECTOR3 &vBasicPos )
{
	GASSERT(m_pd3dDevice&&"m_pd3dDevice 가 초기화 되지 않았습니다.");

	PLANDMANCLIENT pLandMClient = NULL;
	SMAPNODE *pMapNode = FindMapNode ( sMapID );
	if ( pMapNode )
	{
		pLandMClient = new GLLandManClient;
		pLandMClient->Create ( pMapNode->strFile.c_str(), m_pd3dDevice, vBasicPos, &sMapID, pMapNode->bPeaceZone );

		/*force revive, Juver, 2018/07/09 */
		pLandMClient->SetFlags( pMapNode );
	}

	return pLandMClient;
}

PLANDMANCLIENT GLGaeaClient::CreateInstantMapClient ( SNATIVEID sBaseMapID, SNATIVEID sInstantMapID, D3DXVECTOR3 &vBasicPos )
{
	PLANDMANCLIENT pLandMClient = NULL;
	SMAPNODE *pMapNode = FindMapNode ( sBaseMapID );
	if ( pMapNode )
	{
		pLandMClient = new GLLandManClient;
		pLandMClient->Create ( pMapNode->strFile.c_str(), m_pd3dDevice, vBasicPos, &sInstantMapID, pMapNode->bPeaceZone );

		/*force revive, Juver, 2018/07/09 */
		pLandMClient->SetFlags( pMapNode );
	}

	return pLandMClient;
}

HRESULT GLGaeaClient::SetActiveMap ( char* szLandFileName, D3DXVECTOR3 &vBasicPos )
{
	//	Note : 새로운 맵을 로드함.
	//
	PLANDMANCLIENT pLandMClient = CreateLandMClient ( szLandFileName, vBasicPos );
	if ( !pLandMClient )	return E_FAIL;

	//	Note : 이전 맵을 클리어.
	//
	SAFE_DELETE(m_pLandMClient);

	//	Note : 로드한 맵을 활성화.
	//
	m_pLandMClient = pLandMClient;
	m_pLandMClient->ActiveMap ();

	return S_OK;
}

HRESULT GLGaeaClient::SetActiveMap ( SNATIVEID sMapID, D3DXVECTOR3 &vBasicPos )
{
	//	Note : 새로운 맵을 로드함.
	//
	PLANDMANCLIENT pLandMClient = CreateLandMClient ( sMapID, vBasicPos );
	if ( !pLandMClient )	return E_FAIL;

	//	Note : 이전 맵을 클리어.
	//
	SAFE_DELETE(m_pLandMClient);

	//	Note : 로드한 맵을 활성화.
	//
	m_pLandMClient = pLandMClient;
	m_pLandMClient->ActiveMap ();

	return S_OK;
}

PLANDMANCLIENT GLGaeaClient::GetActiveMap ()
{
	return m_pLandMClient;
}

SNATIVEID GLGaeaClient::GetActiveMapID ()
{
	if ( !m_pLandMClient ) return NATIVEID_NULL();
	
	return m_pLandMClient->GetMapID();
}

HRESULT GLGaeaClient::MoveActiveMap ( SNATIVEID sMapID, D3DXVECTOR3 &vPos )
{
	//	Note : 이전 맵을 클리어.
	//
	SAFE_DELETE(m_pLandMClient);

	CInnerInterface::GetInstance().ClearNameList();
	// MEMO : 맵이동시 열린 윈도우를 닫기 위해서 추가했다. 엄한 창이 닫힐수도 있으니 지켜봐야겠다.
	// 아래 함수가 정확한 의미로 쓰이는지 모르겠다.
	CInnerInterface::GetInstance().CloseAllWindow();

	//	Note : 장치 데이터를 리셋.
	//
	DxResponseMan::GetInstance().DoInterimClean ( m_pd3dDevice );

	//	Note : 케릭터 기본 형상을 다시 부른다.
	for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
		DxSkinCharDataContainer::GetInstance().LoadData( GLCONST_CHAR::szCharSkin[i], m_pd3dDevice, TRUE );

	/*skill transform, Juver, 2018/09/09 */
	TRANSFORM_SKIN_SET_MAP& map_skin_set = GLTransformSkinSet::GetInstance().get_map();
	TRANSFORM_SKIN_SET_MAP_ITER iter = map_skin_set.begin();
	TRANSFORM_SKIN_SET_MAP_ITER iter_end = map_skin_set.end();
	for ( ; iter != iter_end; ++iter )
	{
		std::string skin_file = (*iter).second;
		DxSkinCharDataContainer::GetInstance().LoadData( skin_file.c_str(), m_pd3dDevice, TRUE );
	}

	//	Note : 자기 케릭의 스킨을 다시 읽어 온다.
	//
	m_Character.SkinLoad ( m_pd3dDevice );

	//	Note : 새로운 맵을 로드함.
	//
	PLANDMANCLIENT pLandMClient = CreateLandMClient ( sMapID, vPos );
	if ( !pLandMClient )	return E_FAIL;

	//	Note : 로드한 맵을 활성화.
	//
	m_pLandMClient = pLandMClient;
	m_pLandMClient->ActiveMap ();

	CString strText = GLGaeaClient::GetInstance().GetMapName ( m_pLandMClient->GetMapID() );
	CInnerInterface::GetInstance().SetMiniMapInfo ( m_pLandMClient->GetMapAxisInfo(), strText );

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

	return S_OK;
}

HRESULT GLGaeaClient::CreateInstantMap ( SNATIVEID sBaseMapID, SNATIVEID sInstantMapID, D3DXVECTOR3 &vBasicPos )
{
	//	Note : 이전 맵을 클리어.
	//
	SAFE_DELETE(m_pLandMClient);

	CInnerInterface::GetInstance().ClearNameList();
	// MEMO : 맵이동시 열린 윈도우를 닫기 위해서 추가했다. 엄한 창이 닫힐수도 있으니 지켜봐야겠다.
	// 아래 함수가 정확한 의미로 쓰이는지 모르겠다.
	CInnerInterface::GetInstance().CloseAllWindow();

	//	Note : 장치 데이터를 리셋.
	//
	DxResponseMan::GetInstance().DoInterimClean ( m_pd3dDevice );

	//	Note : 케릭터 기본 형상을 다시 부른다.
	for ( int i=0; i<GLCI_NUM_8CLASS; ++i )
		DxSkinCharDataContainer::GetInstance().LoadData( GLCONST_CHAR::szCharSkin[i], m_pd3dDevice, TRUE );

	/*skill transform, Juver, 2018/09/09 */
	TRANSFORM_SKIN_SET_MAP& map_skin_set = GLTransformSkinSet::GetInstance().get_map();
	TRANSFORM_SKIN_SET_MAP_ITER iter = map_skin_set.begin();
	TRANSFORM_SKIN_SET_MAP_ITER iter_end = map_skin_set.end();
	for ( ; iter != iter_end; ++iter )
	{
		std::string skin_file = (*iter).second;
		DxSkinCharDataContainer::GetInstance().LoadData( skin_file.c_str(), m_pd3dDevice, TRUE );
	}

	//	Note : 자기 케릭의 스킨을 다시 읽어 온다.
	//
	m_Character.SkinLoad ( m_pd3dDevice );

	//	Note : 새로운 맵을 로드함.
	//
	PLANDMANCLIENT pLandMClient = CreateInstantMapClient ( sBaseMapID, sInstantMapID, vBasicPos );
	if ( !pLandMClient )	return E_FAIL;

	//	Note : 로드한 맵을 활성화.
	//
	m_pLandMClient = pLandMClient;
	m_pLandMClient->ActiveMap ();
	m_pLandMClient->SetInstantMap(TRUE);

	CString strText = GLGaeaClient::GetInstance().GetMapName ( m_pLandMClient->GetMapID() );
	CInnerInterface::GetInstance().SetMiniMapInfo ( m_pLandMClient->GetMapAxisInfo(), strText );

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

	return S_OK;
}

HRESULT GLGaeaClient::MoveActiveMap ( NET_MSG_GENERIC *nmg )
{
	if ( m_Pet.IsVALID() )
	{
		m_Pet.DeleteDeviceObjects ();
		m_Character.m_bIsPetActive = TRUE;
	}

	/*if ( m_Summon.IsVALID() )
	{
		m_Summon.DeleteDeviceObjects();
	}*/
	
	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].IsVALID() )
			m_Summon[i].DeleteDeviceObjects();
	}

	HRESULT hr=S_OK;

	GLMSG::SNETREQ_GATEOUT_FB *pNetMsg = (GLMSG::SNETREQ_GATEOUT_FB *) nmg;

	m_Character.m_vecMarketClick.clear();

	m_Character.ReSetSTATE(EM_REQ_GATEOUT);

	if ( pNetMsg->emFB!=EMCHAR_GATEOUT_OK )
	{
		CDebugSet::ToListView ( "[MSG] SNETREQ_GATEOUT_FB FAIL : %d", pNetMsg->emFB );

		switch(pNetMsg->emFB)
		{
	case EMCHAR_GATEOUT_FAIL:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REQ_GATEOUT_FB_FAIL"), pNetMsg->emFB );
			break;
		case EMCHAR_GATEOUT_MAPID:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REQ_GATEOUT_FB_DATA_ERR"), pNetMsg->emFB );
			break;
		case EMCHAR_GATEOUT_GATEID:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REQ_GATEOUT_FB_DATA_ERR"), pNetMsg->emFB );
			break;
		case EMCHAR_GATEOUT_TARMAPID:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REQ_GATEOUT_FB_DATA_ERR"), pNetMsg->emFB );
			break;
		case EMCHAR_GATEOUT_TARGATEID:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REQ_GATEOUT_FB_DATA_ERR"), pNetMsg->emFB );
			break;
		case EMCHAR_GATEOUT_FIELDSVR:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REQ_GATEOUT_FB_SVR_ERR"), pNetMsg->emFB );
			break;
		case EMCHAR_GATEOUT_HOLD:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAR_GATEOUT_HOLD") );
			break;
		case EMCHAR_GATEOUT_CONDITION:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAR_GATEOUT_CONDITION"), pNetMsg->emFB );
			break;
		case EMCHAR_GATEOUT_CLUBBATTLE:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAR_GATEOUT_CLUBBATTLE"), GLCONST_CHAR::dwCLUB_BATTLE_GUID_TIME );
			break;
		case EMCHAR_GATEOUT_CLUBBATTLE2:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAR_GATEOUT_CLUBBATTLE2") );
			break;

			/*clubwar map lock, Juver, 2017/06/27 */
		case EMCHAR_GATEOUT_CLUBBATTLE_LOCK:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAR_GATEOUT_CLUBBATTLE_LOCK") );
			break;

			/* multi gate out, Juver, 2020/11/21 */
		case EMCHAR_GATEOUT_TO_INDEX:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("REQ_GATEOUT_FB_DATA_ERR"), pNetMsg->emFB );
			break;
		};

		return S_FALSE;
	}

	CDebugSet::ToListView ( "[MSG] SNETREQ_GATEOUT_FB OK" );


	DWORD dwColorArg1(NULL),dwOldMod(NULL);
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1,	&dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );

	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,		&dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );

	GLMapList::FIELDMAP MapsList = GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find( pNetMsg->sMapID.dwID );
	if( iter == MapsList.end() )				return E_FAIL;

	if ( m_Character.m_bVehicle )
	{
		m_Character.ReqSetVehicle( false );
		m_Character.SetVehicle ( false );
		m_Character.m_bIsVehicleActive = TRUE;
	}

	m_Character.m_vecMarketClick.clear();

	m_Character.m_sVehicle.SetActiveValue( false );
	m_Character.m_sVehicle.RESET();

	const SMAPNODE *pMapNode = &(*iter).second;

	NLOADINGTHREAD::StartThreadLOAD( &m_pd3dDevice, m_hWnd, m_szAppPath, pMapNode->strLoadingImageName.c_str(), pMapNode->strMapName.c_str(), TRUE );
	NLOADINGTHREAD::WaitThread();

	LOADINGSTEP::SETSTEP ( 1 );

	m_pLandMClient->ClearDropObj ();

	LOADINGSTEP::SETSTEP ( 3 );

	// 다른 맵으로 이동하면
	if ( pNetMsg->sMapID != m_pLandMClient->GetMapID() )
	{
		
		// 맵이동시 자신에게 걸린 버프와 상태이상 제거
		m_Character.InitAllSkillFact ();
		m_Character.DISABLEALLLANDEFF();

		//pet skill bugfix
		//reset pet when moving to another map
		m_Character.m_sPETSKILLFACT_A.RESET();
		m_Character.m_sPETSKILLFACT_B.RESET();

		hr = MoveActiveMap ( pNetMsg->sMapID, pNetMsg->vPos );
		if ( SUCCEEDED(hr) )
		{
			LOADINGSTEP::SETSTEP ( 4 );
			m_Character.MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPos );
		}
	}
	else
	{
		LOADINGSTEP::SETSTEP ( 4 );
		m_Character.SetPosition ( pNetMsg->vPos );
		m_Character.DoActWait ();		
	}

	DxResponseMan::GetInstance().SetRenderState ();

	LOADINGSTEP::SETSTEP ( 7 );

	// Note : 맵 이동시 워닝 메세지를 종료한다.
	CInnerInterface::GetInstance().WARNING_MSG_OFF();

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

    LOADINGSTEP::SETSTEP ( 11 );
	NLOADINGTHREAD::EndThread();

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	dwColorArg1 );

	return S_OK;
}


HRESULT GLGaeaClient::CreateInstantMap ( NET_MSG_GENERIC *nmg )
{
	if ( m_Pet.IsVALID() )
	{
		m_Pet.DeleteDeviceObjects ();
		m_Character.m_bIsPetActive = TRUE;
	}

	/*if ( m_Summon.IsVALID() )
	{
		m_Summon.DeleteDeviceObjects();
	}*/

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].IsVALID() )
			m_Summon[i].DeleteDeviceObjects();
	}

	HRESULT hr=S_OK;

	GLMSG::SNETREQ_CREATE_INSTANT_MAP_FB *pNetMsg = (GLMSG::SNETREQ_CREATE_INSTANT_MAP_FB *) nmg;

	m_Character.ReSetSTATE(EM_REQ_GATEOUT);

	if ( pNetMsg->emFB!=EMCHAR_CREATE_INSTANT_MAP_OK )
	{
		CDebugSet::ToListView ( "[MSG] SNETREQ_GATEOUT_FB FAIL : %d", pNetMsg->emFB );

		switch(pNetMsg->emFB)
		{
			case EMCHAR_CREATE_INSTANT_MAP_FAIL:		  
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_CREATE:	  
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_MAPID:	  
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_GATEID:	  
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_TARMAPID: 
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_FIELDSVR: 
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_NOT:
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_CREATEMAX:
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_TO_INDEX:	  /* multi gate out, Juver, 2020/11/21 */
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAR_CREATE_INSTANT_MAP_FAIL"), pNetMsg->emFB );
				}break;

			case EMCHAR_CREATE_INSTANT_MAP_FAIL_CONDITION:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAR_CREATE_INSTANT_MAP_FAIL_CONDITION") );
				break;

				/*map party setting, Juver, 2018/06/29 */
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_PARTY_BLOCK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAR_CREATE_INSTANT_MAP_FAIL_PARTY_BLOCK") );
				break;

				/* map entry user verified, Juver, 2020/02/28 */
			case EMCHAR_CREATE_INSTANT_MAP_FAIL_USER_VERIFIED_MAP_ENTRY:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAR_CREATE_INSTANT_MAP_FAIL_USER_VERIFIED_MAP_ENTRY") );
				break;
		};

		return S_FALSE;
	}

	CDebugSet::ToListView ( "[MSG] SNETREQ_GATEOUT_FB OK" );


	DWORD dwColorArg1(NULL),dwOldMod(NULL);
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1,	&dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );

	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,		&dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );

	GLMapList::FIELDMAP MapsList = GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find( pNetMsg->sBaseMapID.dwID );
	if( iter == MapsList.end() )				return E_FAIL;

	if ( m_Character.m_bVehicle )
	{
		m_Character.ReqSetVehicle( false );
		m_Character.SetVehicle ( false );
		m_Character.m_bIsVehicleActive = TRUE;
	}

	m_Character.m_sVehicle.SetActiveValue( false );
	m_Character.m_sVehicle.RESET();

	const SMAPNODE *pMapNode = &(*iter).second;

	NLOADINGTHREAD::StartThreadLOAD( &m_pd3dDevice, m_hWnd, m_szAppPath, pMapNode->strLoadingImageName.c_str(), pMapNode->strMapName.c_str(), TRUE );
	NLOADINGTHREAD::WaitThread();

	LOADINGSTEP::SETSTEP ( 1 );

	m_pLandMClient->ClearDropObj ();

	LOADINGSTEP::SETSTEP ( 3 );

	// 맵이동시 자신에게 걸린 버프와 상태이상 제거
	m_Character.InitAllSkillFact ();
	m_Character.DISABLEALLLANDEFF();

	SMAPNODE smap_node;
	smap_node = *pMapNode;
	smap_node.sLEVEL_REQUIRE  = pMapNode->sLEVEL_REQUIRE;
	smap_node.sLEVEL_ETC_FUNC = pMapNode->sLEVEL_ETC_FUNC;
	smap_node.sNativeID = pNetMsg->sInstantMapID;
	BOOL bInsert = InsertMapList( smap_node );
	
	hr = CreateInstantMap ( pNetMsg->sBaseMapID, pNetMsg->sInstantMapID, pNetMsg->vPos );
	if ( SUCCEEDED(hr) )
	{
		LOADINGSTEP::SETSTEP ( 4 );
		m_Character.MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPos );
	}

	DxResponseMan::GetInstance().SetRenderState ();

	LOADINGSTEP::SETSTEP ( 7 );

	// Note : 맵 이동시 워닝 메세지를 종료한다.
	CInnerInterface::GetInstance().WARNING_MSG_OFF();

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

	LOADINGSTEP::SETSTEP ( 11 );
	NLOADINGTHREAD::EndThread();

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	dwColorArg1 );

	return S_OK;
}

HRESULT GLGaeaClient::ReBirthFB ( NET_MSG_GENERIC* nmg )
{
	if ( m_Pet.IsVALID() )
	{
		m_Pet.DeleteDeviceObjects ();
		m_Character.m_bIsPetActive = TRUE;
	}

	/*if ( m_Summon.IsVALID() )
	{
		m_Summon.DeleteDeviceObjects();
	}*/

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].IsVALID() )
			m_Summon[i].DeleteDeviceObjects();
	}

	if ( m_Character.m_bVehicle )
	{
		m_Character.ReqSetVehicle( false );
		m_Character.SetVehicle ( false );
		m_Character.m_bIsVehicleActive = TRUE;
	}

	HRESULT hr=S_OK;

	DWORD dwColorArg1(NULL),dwOldMod(NULL);
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1,	&dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );

	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,		&dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );

	GLMSG::SNETPC_REQ_REBIRTH_FB *pNetMsg = (GLMSG::SNETPC_REQ_REBIRTH_FB *) nmg;

	GLMapList::FIELDMAP MapsList = GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find( pNetMsg->sMapID.dwID );
	if( iter == MapsList.end() )				return E_FAIL;

	const SMAPNODE *pMapNode = &(*iter).second;

	NLOADINGTHREAD::StartThreadLOAD( &m_pd3dDevice, m_hWnd, m_szAppPath, pMapNode->strLoadingImageName.c_str(), pMapNode->strMapName.c_str(), TRUE );
	NLOADINGTHREAD::WaitThread();

	LOADINGSTEP::SETSTEP ( 1 );

	m_pLandMClient->ClearDropObj ();

	LOADINGSTEP::SETSTEP ( 3 );

	//	부활할 Map이 틀릴 경우.
	if ( pNetMsg->sMapID != m_pLandMClient->GetMapID() )
	{
		hr = MoveActiveMap ( pNetMsg->sMapID, pNetMsg->vPos );
		if ( SUCCEEDED(hr) )
		{
			LOADINGSTEP::SETSTEP ( 5 );
			m_Character.MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPos );
		}
	}
	else
	{
		LOADINGSTEP::SETSTEP ( 5 );
		m_Character.SetPosition ( pNetMsg->vPos );
		m_Character.DoActWait ();
	}

	LOADINGSTEP::SETSTEP ( 7 );

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

	LOADINGSTEP::SETSTEP ( 9 );

	//	부활처리.
	m_Character.ReBirth ( pNetMsg->wNowHP, pNetMsg->wNowMP, pNetMsg->wNowSP, pNetMsg->vPos, false );

	if( pNetMsg->bRegenEntryFailed )
	{
		DoModal( ID2GAMEINTEXT("REGEN_ENTRY_FAIL"), MODAL_INFOMATION, OK );
	}

	LOADINGSTEP::SETSTEP ( 11 );
	NLOADINGTHREAD::EndThread();

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	dwColorArg1 );

	return S_OK;
}

HRESULT GLGaeaClient::ReLvMustLeaveFB( NET_MSG_GENERIC* nmg )
{
	HRESULT hr=S_OK;

	GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_FB *pNetMsg = (GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_FB *) nmg;
	if ( pNetMsg->emFB != EMREQ_MUST_LEAVE_MAP_FB_OK )
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_MUST_LEAVE_MAP_FB_FAIL") );
		return S_OK;
	}

	// 이동이 성공 했을 경우에만 펫을 삭제한다.
	if ( m_Pet.IsVALID() )
	{
		m_Pet.DeleteDeviceObjects ();
		m_Character.m_bIsPetActive = TRUE;
	}

	/*if ( m_Summon.IsVALID() )
	{
		m_Summon.DeleteDeviceObjects();
	}*/

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].IsVALID() )
			m_Summon[i].DeleteDeviceObjects();
	}

	if ( m_Character.m_bVehicle )
	{
		m_Character.ReqSetVehicle( false );
		m_Character.SetVehicle ( false );
		m_Character.m_bIsVehicleActive = TRUE;
	}

	m_Character.ResetAction();

	DWORD dwColorArg1(NULL),dwOldMod(NULL);
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1,	&dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );

	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,		&dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );

	GLMapList::FIELDMAP MapsList = GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find( pNetMsg->sMAPID.dwID );
	if( iter == MapsList.end() )				return E_FAIL;

	const SMAPNODE *pMapNode = &(*iter).second;

	NLOADINGTHREAD::StartThreadLOAD( &m_pd3dDevice, m_hWnd, m_szAppPath, pMapNode->strLoadingImageName.c_str(), pMapNode->strMapName.c_str(), TRUE );
	NLOADINGTHREAD::WaitThread();

	LOADINGSTEP::SETSTEP ( 1 );

	m_pLandMClient->ClearDropObj ();

	LOADINGSTEP::SETSTEP ( 4 );

	//	이동할 Map이 틀릴 경우.
	if ( pNetMsg->sMAPID != m_pLandMClient->GetMapID() )
	{
		// 맵이동시 자신에게 걸린 버프와 상태이상 제거
		m_Character.InitAllSkillFact ();
		m_Character.DISABLEALLLANDEFF();

		//pet skill bugfix
		//reset pet when moving to another map
		m_Character.m_sPETSKILLFACT_A.RESET();
		m_Character.m_sPETSKILLFACT_B.RESET();

		hr = MoveActiveMap ( pNetMsg->sMAPID, pNetMsg->vPOS );
		if ( SUCCEEDED(hr) )
		{
			LOADINGSTEP::SETSTEP ( 7 );
			m_Character.MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPOS );
		}
	}
	else
	{
		LOADINGSTEP::SETSTEP ( 7 );
		m_Character.SetPosition ( pNetMsg->vPOS );
		m_Character.DoActWait ();
	}

	LOADINGSTEP::SETSTEP ( 9 );

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

	LOADINGSTEP::SETSTEP ( 11 );
	NLOADINGTHREAD::EndThread();

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	dwColorArg1 );

	return S_OK;
}

HRESULT GLGaeaClient::ReCallFB ( NET_MSG_GENERIC* nmg )
{
	HRESULT hr=S_OK;

	GLMSG::SNETPC_REQ_RECALL_FB *pNetMsg = (GLMSG::SNETPC_REQ_RECALL_FB *) nmg;
	if ( pNetMsg->emFB!=EMREQ_RECALL_FB_OK )
	{
		switch ( pNetMsg->emFB )
		{
		case EMREQ_RECALL_FB_FAIL:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_FAIL") );
			return S_OK;

		case EMREQ_RECALL_FB_ITEM:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_ITEM") );
			return S_OK;

		case EMREQ_RECALL_FB_CONDITION:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_CONDITION") );
			return S_OK;

		case EMREQ_RECALL_FB_NOTLASTCALL:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_NOTLASTCALL") );
			return S_OK;

		case EMREQ_RECALL_FB_JOINCON:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_JOINCON") );
			return S_OK;
			
		case EMREQ_RECALL_FB_IMMOVABLE:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_IMMOVABLE") );
			return S_OK;
		};
	}

	// 이동이 성공 했을 경우에만 펫을 삭제한다.
	if ( m_Pet.IsVALID() )
	{
		m_Pet.DeleteDeviceObjects ();
		m_Character.m_bIsPetActive = TRUE;
	}

	/*if ( m_Summon.IsVALID() )
	{
		m_Summon.DeleteDeviceObjects();
	}*/

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].IsVALID() )
			m_Summon[i].DeleteDeviceObjects();
	}

	if ( m_Character.m_bVehicle )
	{
		m_Character.ReqSetVehicle( false );
		m_Character.SetVehicle ( false );
		m_Character.m_bIsVehicleActive = TRUE;
	}

	m_Character.ResetAction();

	DWORD dwColorArg1(NULL),dwOldMod(NULL);
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1,	&dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );

	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,		&dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );

	GLMapList::FIELDMAP MapsList = GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find( pNetMsg->sMAPID.dwID );
	if( iter == MapsList.end() )				return E_FAIL;

	const SMAPNODE *pMapNode = &(*iter).second;

	NLOADINGTHREAD::StartThreadLOAD( &m_pd3dDevice, m_hWnd, m_szAppPath, pMapNode->strLoadingImageName.c_str(), pMapNode->strMapName.c_str(), TRUE );
	NLOADINGTHREAD::WaitThread();

	LOADINGSTEP::SETSTEP ( 1 );

	m_pLandMClient->ClearDropObj ();

	LOADINGSTEP::SETSTEP ( 4 );

	//	이동할 Map이 틀릴 경우.
	if ( pNetMsg->sMAPID != m_pLandMClient->GetMapID() )
	{
		// 맵이동시 자신에게 걸린 버프와 상태이상 제거
		m_Character.InitAllSkillFact ();
		m_Character.DISABLEALLLANDEFF();

		//pet skill bugfix
		//reset pet when moving to another map
		m_Character.m_sPETSKILLFACT_A.RESET();
		m_Character.m_sPETSKILLFACT_B.RESET();

		hr = MoveActiveMap ( pNetMsg->sMAPID, pNetMsg->vPOS );
		if ( SUCCEEDED(hr) )
		{
			LOADINGSTEP::SETSTEP ( 7 );
			m_Character.MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPOS );
		}
	}
	else
	{
		LOADINGSTEP::SETSTEP ( 7 );
		m_Character.SetPosition ( pNetMsg->vPOS );
		m_Character.DoActWait ();
	}

	LOADINGSTEP::SETSTEP ( 9 );

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

	LOADINGSTEP::SETSTEP ( 11 );
	NLOADINGTHREAD::EndThread();

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	dwColorArg1 );

	return S_OK;
}

HRESULT GLGaeaClient::TeleportFB ( NET_MSG_GENERIC* nmg )
{
	HRESULT hr=S_OK;

	GLMSG::SNETPC_REQ_TELEPORT_FB *pNetMsg = (GLMSG::SNETPC_REQ_TELEPORT_FB *) nmg;
	if ( pNetMsg->emFB!=EMREQ_TELEPORT_FB_OK )
	{
		switch ( pNetMsg->emFB )
		{
		case EMREQ_TELEPORT_FB_FAIL:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_FAIL") );
			return S_OK;

		case EMREQ_TELEPORT_FB_ITEM:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_ITEM") );
			return S_OK;

		case EMREQ_TELEPORT_FB_CONDITION:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_CONDITION") );
			return S_OK;

		case EMREQ_TELEPORT_FB_JOINCON:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_JOINCON") );
			return S_OK;
			
		case EMREQ_TELEPORT_FB_IMMOVABLE:
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_IMMOVABLE") );
			return S_OK;
		};
	}

	// 이동이 성공 했을 경우에만 펫을 삭제한다.
	if ( m_Pet.IsVALID() )
	{
		m_Pet.DeleteDeviceObjects ();
		m_Character.m_bIsPetActive = TRUE;
	}

	/*if ( m_Summon.IsVALID() )
	{
		m_Summon.DeleteDeviceObjects();
	}*/

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].IsVALID() )
			m_Summon[i].DeleteDeviceObjects();
	}

	if ( m_Character.m_bVehicle )
	{
		m_Character.ReqSetVehicle( false );
		m_Character.SetVehicle ( false );
		m_Character.m_bIsVehicleActive = TRUE;
	}

	DWORD dwColorArg1(NULL),dwOldMod(NULL);
	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1,	&dwColorArg1 );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );

	m_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,		&dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );

	GLMapList::FIELDMAP MapsList = GetMapList();
	GLMapList::FIELDMAP_ITER iter = MapsList.find( pNetMsg->sMAPID.dwID );
	if( iter == MapsList.end() )				return E_FAIL;

	const SMAPNODE *pMapNode = &(*iter).second;

	NLOADINGTHREAD::StartThreadLOAD( &m_pd3dDevice, m_hWnd, m_szAppPath, pMapNode->strLoadingImageName.c_str(), pMapNode->strMapName.c_str(), TRUE );
	NLOADINGTHREAD::WaitThread();

	LOADINGSTEP::SETSTEP ( 1 );

	m_pLandMClient->ClearDropObj ();

	LOADINGSTEP::SETSTEP ( 4 );

	//	이동할 Map이 틀릴 경우.
	if ( pNetMsg->sMAPID != m_pLandMClient->GetMapID() )
	{
		// 맵이동시 자신에게 걸린 버프와 상태이상 제거
		m_Character.InitAllSkillFact ();
		m_Character.DISABLEALLLANDEFF();

		//pet skill bugfix
		//reset pet when moving to another map
		m_Character.m_sPETSKILLFACT_A.RESET();
		m_Character.m_sPETSKILLFACT_B.RESET();

		hr = MoveActiveMap ( pNetMsg->sMAPID, pNetMsg->vPOS );
		if ( SUCCEEDED(hr) )
		{
			LOADINGSTEP::SETSTEP ( 7 );
			m_Character.MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPOS );
		}
	}
	else
	{
		LOADINGSTEP::SETSTEP ( 7 );
		m_Character.SetPosition ( pNetMsg->vPOS );
		m_Character.DoActWait ();
	}

	LOADINGSTEP::SETSTEP ( 9 );

	// 미니맵 마우스 이동 목표지점 해제
	CInnerInterface::GetInstance().DisableMinimapTarget();

	LOADINGSTEP::SETSTEP ( 11 );
	NLOADINGTHREAD::EndThread();

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		dwOldMod );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	dwColorArg1 );

	return S_OK;
}

HRESULT GLGaeaClient::ReqBusFB ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_BUS_FB *pNetMsg = (GLMSG::SNETPC_REQ_BUS_FB *) nmg;
	switch ( pNetMsg->emFB )
	{
	case EMBUS_TAKE_FAIL:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMBUS_TAKE_FAIL") );
		break;
	case EMBUS_TAKE_OK:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMBUS_TAKE_OK") );
		break;
	case EMBUS_TAKE_TICKET:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMBUS_TAKE_TICKET") );
		break;
	case EMBUS_TAKE_CONDITION:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMBUS_TAKE_CONDITION") );
		break;
	};

	return S_OK;
}

HRESULT GLGaeaClient::ReqTaxiFB ( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_TAXI_FB *pNetMsg = (GLMSG::SNETPC_REQ_TAXI_FB *) nmg;
	switch ( pNetMsg->emFB )
	{
	case EMTAXI_TAKE_FAIL:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_FAIL") );
		break;
	case EMTAXI_TAKE_OK:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMTAXI_TAKE_OK") );
		break;
	case EMTAXI_TAKE_TICKET:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_TICKET") );
		break;
	case EMTAXI_TAKE_MONEY:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_MONEY") );
		break;
	case EMTAXI_TAKE_CONDITION:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_CONDITION") );
		break;
	case EMTAXI_TAKE_MAPFAIL:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_MAPFAIL") );
		break;
	case EMTAXI_TAKE_STATIONFAIL:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_STATIONFAIL") );
		break;
	case EMTAXI_TAKE_NPCFAIL:
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_NPCFAIL") );
		break;	
	};

	return S_OK;

}

HRESULT GLGaeaClient::DropOutCrow ( NET_MSG_GENERIC *nmg )
{
	if ( !m_pLandMClient )	return E_FAIL;
	GLMSG::SNETDROP_OUT *pNetMsg = (GLMSG::SNETDROP_OUT *) nmg;
	//CDebugSet::ToListView ( "DROPOUT_CROW [%s] : %d", COMMENT::CROW[pNetMsg->emCrow].c_str(), pNetMsg->dwID );

	STARID sTARID;
	BYTE cNUM = pNetMsg->GETAMOUNT();
	for ( BYTE i=0; i<cNUM; ++i )
	{
		bool bOK = pNetMsg->POP_CROW( sTARID );
		if ( !bOK )		return E_FAIL;

		switch ( sTARID.wCrow )
		{
		case CROW_PC:
			m_pLandMClient->DropOutChar ( sTARID.wID );
			break;

		case CROW_NPC:
		case CROW_MOB:
			m_pLandMClient->DropOutCrow ( sTARID.wID );
			break;

			/* crow zone, Juver, 2018/02/21 */
		case CROW_ZONE_NAME:
		case CROW_GATE_NAME:
			m_pLandMClient->DropOutCrow ( sTARID.wID );
			break;

		case CROW_MATERIAL:
			m_pLandMClient->DropOutMaterial ( sTARID.wID );
			break;

		case CROW_ITEM:
			m_pLandMClient->DropOutItem ( sTARID.wID );
			break;

		case CROW_MONEY:
			m_pLandMClient->DropOutMoney ( sTARID.wID );
			break;

		case CROW_PET:
			m_pLandMClient->DropOutPet ( sTARID.wID );
			break;

		case CROW_SUMMON:
			m_pLandMClient->DropOutSummon ( sTARID.wID );
			break;
		};
	}

	return S_OK;
}

BOOL GLGaeaClient::IsVisibleCV ( const STARGETID & sTargetID )
{
	if ( !m_pLandMClient )	return FALSE;

	if ( sTargetID.dwID == EMTARGET_NULL )		return FALSE;

	if ( sTargetID.emCrow == CROW_PC )
	{
		PGLCHARCLIENT pChar = m_pLandMClient->GetChar ( sTargetID.dwID );
		if ( pChar )	return pChar->IsCollisionVolume();

		if ( GLGaeaClient::GetInstance().GetCharacter()->m_dwGaeaID == sTargetID.dwID )
			return TRUE;
	}
	/* crow zone, Juver, 2018/02/21 */
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB || sTargetID.emCrow==CROW_ZONE_NAME|| sTargetID.emCrow==CROW_GATE_NAME ) 
	{
		PGLCROWCLIENT pCrow = m_pLandMClient->GetCrow ( sTargetID.dwID );
		if ( pCrow )	return pCrow->IsCollisionVolume();
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL ) 
	{
		PGLMATERIALCLIENT pMaterial = m_pLandMClient->GetMaterial ( sTargetID.dwID );
		if ( pMaterial )	return pMaterial->IsCollisionVolume();
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		PGLANYSUMMON pSummon = m_pLandMClient->GetSummon ( sTargetID.dwID );
		if ( pSummon )	return pSummon->IsCollisionVolume();

	}
	else
		GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );

	return FALSE;
}

PGLCHARCLIENT GLGaeaClient::GetChar ( DWORD dwID )
{
	if ( !m_pLandMClient )	return NULL;

	return m_pLandMClient->GetChar ( dwID );
}

PGLANYSUMMON GLGaeaClient::GetSummon ( DWORD dwID )
{
	if ( !m_pLandMClient )	return NULL;

	return m_pLandMClient->GetSummon ( dwID );
}

BOOL GLGaeaClient::ValidCheckTarget ( const STARGETID & sTargetID )
{
	if ( !m_pLandMClient )	return FALSE;

	if ( sTargetID.dwID == EMTARGET_NULL )		return FALSE;

	if ( sTargetID.emCrow == CROW_PC )
	{
		PGLCHARCLIENT pChar = m_pLandMClient->GetChar ( sTargetID.dwID );
		if ( pChar ) return TRUE;

		if ( GLGaeaClient::GetInstance().GetCharacter()->m_dwGaeaID == sTargetID.dwID )
			return TRUE;
	}
	/* crow zone, Juver, 2018/02/21 */
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB || sTargetID.emCrow==CROW_ZONE_NAME|| sTargetID.emCrow==CROW_GATE_NAME )
	{
		PGLCROWCLIENT pCrow = m_pLandMClient->GetCrow ( sTargetID.dwID );
		if ( pCrow ) return TRUE;
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL )
	{
		PGLMATERIALCLIENT pMaterial = m_pLandMClient->GetMaterial ( sTargetID.dwID );
		if ( pMaterial ) return TRUE;
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		PGLANYSUMMON pSummon = m_pLandMClient->GetSummon ( sTargetID.dwID );
		if ( pSummon ) return TRUE;

		/*skill summon, Juver, 2017/10/08 */
		GLSummonClient*	psummon_client = GetSummonClient( sTargetID.dwID );
		if( psummon_client )	return TRUE;

		//if ( GLGaeaClient::GetInstance().GetSummonClient()->m_dwGUID == sTargetID.dwID )
		//	return TRUE;
	}
	else
		GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );

	return FALSE;
}

D3DXVECTOR3 GLGaeaClient::GetTargetPos ( const STARGETID &sTargetID )
{
	if ( !m_pLandMClient )	return VERROR();

	//	Note : 타겟의 위치 정보를 가져옴.
	if ( sTargetID.emCrow == CROW_PC )
	{
		PGLCHARCLIENT pChar = m_pLandMClient->GetChar ( sTargetID.dwID );
		if ( pChar )
			return pChar->GetPosition();

		if ( GetCharacter()->m_dwGaeaID == sTargetID.dwID )
			return GetCharacterPos();
	}
	/* crow zone, Juver, 2018/02/21 */
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB || sTargetID.emCrow==CROW_ZONE_NAME|| sTargetID.emCrow==CROW_GATE_NAME )
	{
		PGLCROWCLIENT pCrow = m_pLandMClient->GetCrow ( sTargetID.dwID );
		if ( pCrow )	return pCrow->GetPosition();
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL ) 
	{
		PGLMATERIALCLIENT pMaterial = m_pLandMClient->GetMaterial ( sTargetID.dwID );
		if ( pMaterial )	return pMaterial->GetPosition();
	}
	else if ( sTargetID.emCrow == CROW_ITEM )
	{
		PITEMCLIENTDROP pItem = m_pLandMClient->GetItem ( sTargetID.dwID );
		if ( pItem )	return pItem->vPos;
	}
	else if ( sTargetID.emCrow == CROW_MONEY )
	{
		PMONEYCLIENTDROP pMoney = m_pLandMClient->GetMoney ( sTargetID.dwID );
		if ( pMoney )	return pMoney->vPos;
	}
	else if ( sTargetID.emCrow == CROW_PET )
	{
		if ( GetPetClient()->m_dwGUID == sTargetID.dwID ) return GetPetClient()->GetPosition ();
		else
		{
			PGLANYPET pAnyPet = m_pLandMClient->GetPet ( sTargetID.dwID );
			if ( pAnyPet ) return pAnyPet->GetPosition ();
		}
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		//if ( GetSummonClient()->m_dwGUID == sTargetID.dwID ) return GetPetClient()->GetPosition ();

		/*skill summon, Juver, 2017/10/08 */
		GLSummonClient*	psummon_client = GetSummonClient( sTargetID.dwID );
		if( psummon_client )	return psummon_client->GetPosition ();
		else
		{
			PGLANYSUMMON pAnySummon = m_pLandMClient->GetSummon ( sTargetID.dwID );
			if ( pAnySummon ) return pAnySummon->GetPosition ();
		}
	}
	else	GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );

	return VERROR();
}


namespace
{
	void MINDETECTAABB ( D3DXVECTOR3 &vMax, D3DXVECTOR3 &vMin, const float fRate, const float fMinLeng )
	{
		float fLength(0);
		fLength = vMax.x - vMin.x;
		vMax.x += ( fLength*fRate - fLength ) / 2.0f;
		vMin.x -= ( fLength*fRate - fLength ) / 2.0f;

		fLength = vMax.x - vMin.x;
		if ( fLength < fMinLeng )
		{
			vMax.x += fMinLeng/2.0f;
			vMin.x -= fMinLeng/2.0f;
		}

		fLength = vMax.y - vMin.y;
		vMax.y += ( fLength*fRate - fLength ) / 2.0f;
		vMin.y -= ( fLength*fRate - fLength ) / 2.0f;

		fLength = vMax.y - vMin.y;
		if ( fLength < fMinLeng )
		{
			vMax.y += fMinLeng/2.0f;
			vMin.y -= fMinLeng/2.0f;
		}

		fLength = vMax.z - vMin.z;
		vMax.z += ( fLength*fRate - fLength ) / 2.0f;
		vMin.z -= ( fLength*fRate - fLength ) / 2.0f;

		fLength = vMax.z - vMin.z;
		if ( fLength < fMinLeng )
		{
			vMax.z += fMinLeng/2.0f;
			vMin.z -= fMinLeng/2.0f;
		}
	}
}

DETECTMAP* GLGaeaClient::DetectCrowDie ( const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt )
{
	STARGETID TargetID(CROW_MOB,EMTARGET_NULL);
	if ( !m_pLandMClient )	return NULL;

	//	DetectID 클리어.
	if ( !m_vecDetectID.empty() )	m_vecDetectID.erase ( m_vecDetectID.begin(), m_vecDetectID.end() );

	STARGETID findTargetID;

	//	몹.
	{
		GLCROWCLIENTLIST *pCrowList = m_pLandMClient->GetCrowList ();
		GLCROWCLIENTNODE *pCrowCur = pCrowList->m_pHead;
		for ( ; pCrowCur; pCrowCur = pCrowCur->pNext )
		{
			PGLCROWCLIENT pCrow= pCrowCur->Data;

			if ( !pCrow->IsDie() )			continue;

			BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, pCrow->m_vMax, pCrow->m_vMin );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = pCrow->m_pCrowData->m_sBasic.m_emCrow;
				findTargetID.dwID = pCrow->m_dwGlobID;
				findTargetID.vPos = pCrow->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	//	캐릭터.
	{
		GLCHARCLIENTLIST *pCharList = m_pLandMClient->GetCharList ();
		GLCHARCLIENTNODE *pCharCur = pCharList->m_pHead;
		for ( ; pCharCur; pCharCur = pCharCur->pNext )
		{
			PGLCHARCLIENT pChar = pCharCur->Data;
			if ( !pChar->IsVisibleDetect() )	continue;
			if ( !pChar->IsDie() )				continue;

			BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, pChar->m_vMax, pChar->m_vMin );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = CROW_PC;
				findTargetID.dwID = pChar->m_dwGaeaID;
				findTargetID.vPos = pChar->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	if ( m_vecDetectID.empty() )	return NULL;
	
	std::sort ( m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER() );
	return &m_vecDetectID;
}

DETECTMAP* GLGaeaClient::DetectCrow ( DWORD emCrow, const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt )
{
	STARGETID TargetID(CROW_MOB,EMTARGET_NULL);
	if ( !m_pLandMClient )	return NULL;

	//	DetectID 클리어.
	if ( !m_vecDetectID.empty() )	m_vecDetectID.erase ( m_vecDetectID.begin(), m_vecDetectID.end() );

	STARGETID findTargetID;
	
	//	몹.
	if ( emCrow & CROW_EX_MOB )
	{
		GLCROWCLIENTLIST *pCrowList = m_pLandMClient->GetCrowList ();
		GLCROWCLIENTNODE *pCrowCur = pCrowList->m_pHead;
		for ( ; pCrowCur; pCrowCur = pCrowCur->pNext )
		{
			PGLCROWCLIENT pCrow= pCrowCur->Data;

			if ( !pCrow->IsValidBody() )							continue;

			BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, pCrow->m_vMax, pCrow->m_vMin );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = pCrow->m_pCrowData->m_sBasic.m_emCrow;
				findTargetID.dwID = pCrow->m_dwGlobID;
				findTargetID.vPos = pCrow->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	//	Material
	if ( emCrow & CROW_EX_MATERIAL )
	{
		GLMATERIALCLIENTLIST *pMaterialList = m_pLandMClient->GetMaterialList ();
		GLMATERIALCLIENTNODE *pMaterialCur = pMaterialList->m_pHead;
		for ( ; pMaterialCur; pMaterialCur = pMaterialCur->pNext )
		{
			PGLMATERIALCLIENT pMaterial= pMaterialCur->Data;

			if ( !pMaterial->IsValidBody() )							continue;

			BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, pMaterial->m_vMax, pMaterial->m_vMin );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = pMaterial->m_pCrowData->m_sBasic.m_emCrow;
				findTargetID.dwID = pMaterial->m_dwGlobID;
				findTargetID.vPos = pMaterial->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	//	캐릭터.
	if ( emCrow & CROW_EX_PC )
	{
		GLCHARCLIENTLIST *pCharList = m_pLandMClient->GetCharList ();
		GLCHARCLIENTNODE *pCharCur = pCharList->m_pHead;
		for ( ; pCharCur; pCharCur = pCharCur->pNext )
		{
			PGLCHARCLIENT pChar = pCharCur->Data;
			if ( !pChar->IsVisibleDetect() )	continue;
			if ( !pChar->IsValidBody() )		continue;

			BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, pChar->m_vMax, pChar->m_vMin );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = CROW_PC;
				findTargetID.dwID = pChar->m_dwGaeaID;
				findTargetID.vPos = pChar->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	//	소환수
	if ( emCrow & CROW_EX_SUMMON )
	{
		GLANYSUMMONLIST *pSummonList = m_pLandMClient->GetSummonList();
		GLANYSUMMONNODE *pSummonCur = pSummonList->m_pHead;
		for ( ; pSummonCur; pSummonCur = pSummonCur->pNext )
		{
			PGLANYSUMMON pSummon = pSummonCur->Data;
			if ( !pSummon->IsValidBody() )		continue;

			BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, pSummon->m_vMax, pSummon->m_vMin );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = CROW_SUMMON;
				findTargetID.dwID = pSummon->m_dwGUID;
				findTargetID.vPos = pSummon->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	//	Item.
	if ( emCrow & CROW_EX_ITEM )
	{
		ITEMCLIENTDROPLIST *pItemList = m_pLandMClient->GetItemList();
		ITEMCLIENTDROPNODE *pItemCur = pItemList->m_pHead;
		for ( ; pItemCur; pItemCur = pItemCur->pNext )
		{
			const CItemClientDrop &sItemDrop = *pItemCur->Data;

			bool bCOLL = sItemDrop.IsCollision ( vFromPt, vTargetPt );

			if ( bCOLL )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = CROW_ITEM;
				findTargetID.dwID = sItemDrop.dwGlobID;
				findTargetID.vPos = sItemDrop.vPos;

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	//	Money.
	if ( emCrow & CROW_EX_MONEY )
	{
		MONEYCLIENTDROPLIST *pMoneyList = m_pLandMClient->GetMoneyList();
		MONEYCLIENTDROPNODE *pMoneyCur = pMoneyList->m_pHead;
		for ( ; pMoneyCur; pMoneyCur = pMoneyCur->pNext )
		{
			const CMoneyClientDrop &sMoneyDrop = *pMoneyCur->Data;

			bool bCOLL = sMoneyDrop.IsCollision ( vFromPt, vTargetPt );
			if ( bCOLL )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = CROW_MONEY;
				findTargetID.dwID = sMoneyDrop.dwGlobID;
				findTargetID.vPos = sMoneyDrop.vPos;

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	if ( m_vecDetectID.empty() )	return NULL;

	std::sort ( m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER() );
	return &m_vecDetectID;
}

DETECTMAP* GLGaeaClient::DetectCrow ( DWORD emCrow, const D3DXVECTOR3 &vTarPos, const float fLength )
{
	STARGETID TargetID(CROW_MOB,EMTARGET_NULL);
	if ( !m_pLandMClient )	return NULL;

	//	DetectID 클리어.
	if ( !m_vecDetectID.empty() )	m_vecDetectID.erase ( m_vecDetectID.begin(), m_vecDetectID.end() );

	STARGETID findTargetID;

	

	//	몹.
	if ( emCrow & CROW_EX_MOB )
	{
		GLCROWCLIENTLIST *pCrowList = m_pLandMClient->GetCrowList ();
		GLCROWCLIENTNODE *pCrowCur = pCrowList->m_pHead;
		for ( ; pCrowCur; pCrowCur = pCrowCur->pNext )
		{
			PGLCROWCLIENT pCrow= pCrowCur->Data;

			if ( !pCrow->IsValidBody() ) continue;

			float fTarRange = pCrow->GETBODYRADIUS() + fLength;

			BOOL bCol = COLLISION::IsSpherePointCollision ( pCrow->GetPosition(), vTarPos, fTarRange );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = pCrow->m_pCrowData->m_sBasic.m_emCrow;
				findTargetID.dwID = pCrow->m_dwGlobID;
				findTargetID.vPos = pCrow->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

    //	Material
	if ( emCrow & CROW_EX_MATERIAL )
	{
		GLMATERIALCLIENTLIST *pMaterialList = m_pLandMClient->GetMaterialList ();
		GLMATERIALCLIENTNODE *pMaterialCur = pMaterialList->m_pHead;
		for ( ; pMaterialCur; pMaterialCur = pMaterialCur->pNext )
		{
			PGLMATERIALCLIENT pMaterial= pMaterialCur->Data;

			if ( !pMaterial->IsValidBody() ) continue;

			float fTarRange = pMaterial->GetBodyRadius() + fLength;

			BOOL bCol = COLLISION::IsSpherePointCollision ( pMaterial->GetPosition(), vTarPos, fTarRange );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = pMaterial->m_pCrowData->m_sBasic.m_emCrow;
				findTargetID.dwID = pMaterial->m_dwGlobID;
				findTargetID.vPos = pMaterial->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	//	캐릭터.
	if ( emCrow & CROW_EX_PC )
	{
		GLCHARCLIENTLIST *pCharList = m_pLandMClient->GetCharList ();
		GLCHARCLIENTNODE *pCharCur = pCharList->m_pHead;
		for ( ; pCharCur; pCharCur = pCharCur->pNext )
		{
			PGLCHARCLIENT pChar = pCharCur->Data;
			if ( !pChar->IsVisibleDetect() )		continue;
			if ( !pChar->IsValidBody() )			continue;

			BOOL bCol = COLLISION::IsSpherePointCollision ( pChar->GetPosition(), vTarPos, fLength );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = CROW_PC;
				findTargetID.dwID = pChar->m_dwGaeaID;
				findTargetID.vPos = pChar->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	//	Item.
	if ( emCrow & CROW_EX_ITEM )
	{
		ITEMCLIENTDROPLIST *pItemList = m_pLandMClient->GetItemList();
		ITEMCLIENTDROPNODE *pItemCur = pItemList->m_pHead;
		for ( ; pItemCur; pItemCur = pItemCur->pNext )
		{
			CItemClientDrop &sItemDrop = *pItemCur->Data;

			BOOL bCol = COLLISION::IsSpherePointCollision ( sItemDrop.vPos, vTarPos, fLength );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = CROW_ITEM;
				findTargetID.dwID = sItemDrop.dwGlobID;
				findTargetID.vPos = sItemDrop.vPos;

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	//	Money.
	if ( emCrow & CROW_EX_MONEY )
	{
		MONEYCLIENTDROPLIST *pMoneyList = m_pLandMClient->GetMoneyList();
		MONEYCLIENTDROPNODE *pMoneyCur = pMoneyList->m_pHead;
		for ( ; pMoneyCur; pMoneyCur = pMoneyCur->pNext )
		{
			CMoneyClientDrop &sMoneyDrop = *pMoneyCur->Data;

			BOOL bCol = COLLISION::IsSpherePointCollision ( sMoneyDrop.vPos, vTarPos, fLength );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = CROW_MONEY;
				findTargetID.dwID = sMoneyDrop.dwGlobID;
				findTargetID.vPos = sMoneyDrop.vPos;

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	/*skill summon, Juver, 2017/10/08 */
	if ( emCrow & CROW_EX_SUMMON )
	{
		GLANYSUMMONLIST *pSummonList = m_pLandMClient->GetSummonList();
		GLANYSUMMONNODE *pSummonCur = pSummonList->m_pHead;
		for ( ; pSummonCur; pSummonCur = pSummonCur->pNext )
		{
			PGLANYSUMMON pSummon = pSummonCur->Data;
			if ( !pSummon->IsValidBody() )		continue;

			float fTarRange = pSummon->GETBODYRADIUS() + fLength;

			BOOL bCol = COLLISION::IsSpherePointCollision ( pSummon->GetPosition(), vTarPos, fTarRange );
			if ( bCol )
			{
				//	Note : 검출된 타깃 등록.
				findTargetID.emCrow = CROW_SUMMON;
				findTargetID.dwID = pSummon->m_dwGUID;
				findTargetID.vPos = pSummon->GetPosition ();

				m_vecDetectID.push_back ( findTargetID );
			}
		}
	}

	

	if ( m_vecDetectID.empty() )	return NULL;
	
	std::sort ( m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER() );
	return &m_vecDetectID;
}

BOOL GLGaeaClient::IsCollisionMobToPoint ( const D3DXVECTOR3 &vPoint, const WORD wBodyRadius )
{
	if ( !m_pLandMClient )	return FALSE;

	GLCROWCLIENTLIST *pCrowList = m_pLandMClient->GetCrowList ();
	GLCROWCLIENTNODE *pCrowCur = pCrowList->m_pHead;
	for ( ; pCrowCur; pCrowCur = pCrowCur->pNext )
	{
		PGLCROWCLIENT pCrow = pCrowCur->Data;
		if ( !pCrow->IsValidBody() )		continue;
		if ( !pCrow->IsHaveVisibleBody() )	continue;

		/* crow zone, Juver, 2018/02/21 */
		if ( pCrow->GETCROW() == CROW_ZONE_NAME )	continue;
		if ( pCrow->GETCROW() == CROW_GATE_NAME )	continue;

		D3DXVECTOR3 vDist = pCrow->GetPosition() - vPoint;
		if ( DxIsZeroVector(vDist) )	return TRUE;

		vDist.y = 0.0f;
		float fDist = D3DXVec3Length ( &vDist );

		if ( fDist < (wBodyRadius+pCrow->GETBODYRADIUS()) )		return TRUE;
	}

	return FALSE;
}

HRESULT GLGaeaClient::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT(pd3dDevice);
	m_pd3dDevice = pd3dDevice;

	if ( m_pLandMClient )
	{
		m_pLandMClient->ActiveMap();
		m_pLandMClient->InitDeviceObjects ( m_pd3dDevice );
	}

	DxEffectMan::GetInstance().SetCrowTracer ( this );

	DxSoundLib::GetInstance()->CreateSound ( "GRINDING_SUCCEED", GLCONST_CHAR::strGRINDING_SUCCEED.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "GRINDING_FAIL", GLCONST_CHAR::strGRINDING_FAIL.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "GRINDING_RESET", GLCONST_CHAR::strGRINDING_RESET.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "GRINDING_BROKEN", GLCONST_CHAR::strGRINDING_BROKEN.c_str(), SFX_SOUND );

	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_SUIT", GLCONST_CHAR::strITEMDROP_SUIT.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_WAPON", GLCONST_CHAR::strITEMDROP_WAPON.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_SHOES", GLCONST_CHAR::strITEMDROP_SHOES.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_RING", GLCONST_CHAR::strITEMDROP_RING.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_?BOX", GLCONST_CHAR::strITEMDROP_QBOX.c_str(), SFX_SOUND );

	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_SCROLL", GLCONST_CHAR::strITEMDROP_SCROLL.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_COIN", GLCONST_CHAR::strITEMDROP_COIN.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_DRUGS", GLCONST_CHAR::strITEMDROP_DRUGS.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PICKUP_ITEM", GLCONST_CHAR::strPICKUP_ITEM.c_str(), SFX_SOUND );
	
	DxSoundLib::GetInstance()->CreateSound ( "QITEM_FACT", GLCONST_CHAR::strQITEM_FACT.c_str(), SFX_SOUND );
	
	
	DxSoundLib::GetInstance()->CreateSound ( "GAMBLING_SHUFFLE", GLCONST_CHAR::strGAMBLING_SHUFFLE.c_str(), SFX_SOUND ); //Monster7j
	DxSoundLib::GetInstance()->CreateSound ( "GAMBLING_WIN", GLCONST_CHAR::strGAMBLING_WIN.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "GAMBLING_LOSE", GLCONST_CHAR::strGAMBLING_LOSE.c_str(), SFX_SOUND );

	/*pk combo, EJCode, 2018/11/15 */
	GLPKCombo::GetInstance().LoadSound();

	return S_OK;
}

HRESULT GLGaeaClient::RestoreDeviceObjects ()
{
	GASSERT(m_pd3dDevice&&"m_pd3dDevice 가 초기화 되지 않았습니다.");

	if ( m_pLandMClient )
	{
		m_pLandMClient->RestoreDeviceObjects ();
	}

	m_Character.RestoreDeviceObjects ( m_pd3dDevice );
	m_Pet.RestoreDeviceObjects ( m_pd3dDevice );

	//m_Summon.RestoreDeviceObjects ( m_pd3dDevice );
	
	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		m_Summon[i].RestoreDeviceObjects( m_pd3dDevice );

	return S_OK;
}

HRESULT GLGaeaClient::InvalidateDeviceObjects ()
{
	if ( m_pLandMClient )
	{
		m_pLandMClient->InvalidateDeviceObjects ();
	}

	m_Character.InvalidateDeviceObjects ();
	m_Pet.InvalidateDeviceObjects ();

	//m_Summon.InvalidateDeviceObjects();

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		m_Summon[i].InvalidateDeviceObjects();

	return S_OK;
}

HRESULT GLGaeaClient::DeleteDeviceObjects ()
{
	if ( m_pLandMClient )
	{
		m_pLandMClient->DeleteDeviceObjects ();
		SAFE_DELETE(m_pLandMClient);
	}

	m_Character.DeleteDeviceObjects();
	m_Pet.DeleteDeviceObjects ();

	//m_Summon.DeleteDeviceObjects ();

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
		m_Summon[i].DeleteDeviceObjects();

	return S_OK;
}

HRESULT GLGaeaClient::FrameMove ( float fTime, float fElapsedTime, SGameStageInfo& sInfo )
{
	m_fAge += fElapsedTime;

	//	Note : 게이트 검색후 게이트 상태 업대이트.
	//
	CInnerInterface::GetInstance().SetGateOpen ( FALSE );
	if ( m_pLandMClient )
	{
		PROFILE_BEGIN("GLLandManClient::FrameMove");
		m_pLandMClient->FrameMove ( fTime, fElapsedTime, sInfo );
		PROFILE_END("GLLandManClient::FrameMove");
	}

	/* post system, Juver, 2021/02/05 */
	GLPostClient::GetInstance().FrameMove( fTime, fElapsedTime );


	PROFILE_BEGIN("GLCharacter::FrameMove");
	m_Character.FrameMove ( fTime, fElapsedTime );
	PROFILE_END("GLCharacter::FrameMove");

	// PET
	PROFILE_BEGIN("GLPetClient::FrameMove");
	if ( m_Pet.IsVALID () ) m_Pet.FrameMove ( fTime, fElapsedTime );
	PROFILE_END("GLPetClient::FrameMove");

	// SUMMON
	//if ( m_Summon.IsVALID () ) m_Summon.FrameMove ( fTime, fElapsedTime );

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].IsVALID() )
			m_Summon[i].FrameMove ( fTime, fElapsedTime );
	}
		
	// 선도클럽 결정전 시간 갱신

	CLUB_BATTLE_TIMER_MAP_ITER it = m_mapCLUBBATTLETimer.begin();
	for (; it != m_mapCLUBBATTLETimer.end(); ++it )
	{
		SCLUBWAR_BATTLE_TIMER& sBattleTimer = (*it).second;

		if ( sBattleTimer.bBattle )
		{
			sBattleTimer.fTime -= fElapsedTime;
			if ( sBattleTimer.fTime < 0.0f )
				sBattleTimer.fTime = 0.0f;
		}
			
	}

	//if ( m_fAge > 1.0f ) CInnerInterface::GetInstance().UpdateClubBattleTime( m_fCLUBBATTLETimer );


	if ( m_bClubDeathMatch )
	{
		m_fClubDeathMatchTimer -= fElapsedTime;
		if ( m_fClubDeathMatchTimer < 0.0f )
			m_fClubDeathMatchTimer = 0.0f;
		//if ( m_fAge > 1.0f ) CInnerInterface::GetInstance().UpdateClubBattleTime( m_fClubDeathMatchTimer );
	}

	if( m_bFreePKMatch )
	{
		m_fFreePKMatchTimer -= fElapsedTime; 
		if ( m_fFreePKMatchTimer < 0.0f )
			m_fFreePKMatchTimer = 0.0f;
	}

	// 서버 현재시간
	if ( m_fAge > 1.0f )
	{
		CTimeSpan cElapsedTime(0,0,0,1);
		m_cServerTime += cElapsedTime;

		m_fAge = m_fAge - 1.0f;
	}

	///*character render setting, Juver, 2019/03/22 */
	//if ( m_fRenderSettingChangeTimer < RPARAM::fRenderSettingChangeTime )
	//	m_fRenderSettingChangeTimer += fElapsedTime;

	GLPKMark::GetInstance().FrameMove(fElapsedTime);

	return S_OK;
}

HRESULT GLGaeaClient::Render ( CLIPVOLUME &CV, SGameStageInfo& sInfo )
{
	GASSERT(m_pd3dDevice);

	//	Note : 하늘 렌더링..!!
	PROFILE_BEGIN2("DxSkyMan::Render");
	DxSkyMan::GetInstance().Render( m_pd3dDevice );
	PROFILE_END2("DxSkyMan::Render");

	//	Note : 지형 랜더링..
	PROFILE_BEGIN2("m_pLandMClient::Render");
	if ( m_pLandMClient )
	{
		m_pLandMClient->Render ( CV );
	}
	PROFILE_END2("m_pLandMClient::Render");

	// Note : 그림자 Clear
	PROFILE_BEGIN2("DxShadowMap::ClearShadow");
	DxShadowMap::GetInstance().ClearShadow( m_pd3dDevice );
	PROFILE_END2("DxShadowMap::ClearShadow");

	PROFILE_BEGIN2("DxSoftShadowMap::ClearShadow");
	DxSoftShadowMap::GetInstance().ClearShadow( m_pd3dDevice );
	PROFILE_END2("DxSoftShadowMap::ClearShadow");
	
	g_bPREVIEW_CHAR_RENDER = FALSE;

	PROFILE_BEGIN2("m_Character::Render");
	m_Character.Render( m_pd3dDevice );
	PROFILE_END2("m_Character::Render");

	PROFILE_BEGIN2("m_Character::RenderShadow,RenderReflect");
	m_Character.RenderShadow( m_pd3dDevice );
	m_Character.RenderReflect( m_pd3dDevice );
	PROFILE_END2("m_Character::RenderShadow,RenderReflect");

	// PET
	PROFILE_BEGIN2("m_Pet::Render,RenderShadow,RenderReflect");
	m_Pet.Render ( m_pd3dDevice );
	m_Pet.RenderShadow ( m_pd3dDevice );
	m_Pet.RenderReflect ( m_pd3dDevice );
	PROFILE_END2("m_Pet::Render,RenderShadow,RenderReflect");

	//m_Summon.Render ( m_pd3dDevice );
	//m_Summon.RenderShadow ( m_pd3dDevice );
	//m_Summon.RenderReflect ( m_pd3dDevice );

	/*skill summon, Juver, 2017/10/08 */
	for( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		m_Summon[i].Render ( m_pd3dDevice );
		m_Summon[i].RenderShadow ( m_pd3dDevice );
		m_Summon[i].RenderReflect ( m_pd3dDevice );
	}

	PROFILE_BEGIN2("m_pLandMClient::char,crow,item");
	if ( m_pLandMClient )
	{
		m_pLandMClient->Render_MobItem ( CV, sInfo );
	}
	PROFILE_END2("m_pLandMClient::char,crow,item");

	// Note : ShadowMap ImageBlur
	PROFILE_BEGIN2("DxShadowMap::LastImageBlur");
	DxShadowMap::GetInstance().LastImageBlur( m_pd3dDevice );
	PROFILE_END2("DxShadowMap::LastImageBlur");

	PROFILE_BEGIN2("DxSoftShadowMap::LastImageBlur");
	DxSoftShadowMap::GetInstance().LastImageBlur( m_pd3dDevice );
	PROFILE_END2("DxSoftShadowMap::LastImageBlur");

	return S_OK;
}

HRESULT GLGaeaClient::RenderEff ( CLIPVOLUME &CV )
{
	GASSERT(m_pd3dDevice);

	if ( m_pLandMClient )
	{
		m_pLandMClient->Render_EFF ( CV );
	}

	//	Note : 하늘 렌더링..!!
	DxSkyMan::GetInstance().Render_AFTER( m_pd3dDevice );

	return S_OK;
}

void GLGaeaClient::RenderPickAlpha( CLIPVOLUME &CV )
{
	if ( m_pLandMClient )
	{
		m_pLandMClient->RenderPickAlpha( CV );
	}
}

void GLGaeaClient::MsgProcess ( NET_MSG_GENERIC* nmg )
{
	if ( !m_pLandMClient )	return;

	switch ( nmg->nType )
	{
	/*security 2025*/
	case NET_DETECT_CHEAT_A2C:
		{
			GLMSG::SNET_DETECT_CHEAT_A2C* netmsg = (GLMSG::SNET_DETECT_CHEAT_A2C*)nmg;

			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, "[%s] CHEAT DETECTED!", netmsg->szName);
		}break;
		
	case NET_MSG_WOE_RANKING_CLT:
	{
		GLMSG::SNET_MSG_WOE_RANKING_CLT* netmsg = (GLMSG::SNET_MSG_WOE_RANKING_CLT*)nmg;

		CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::GREEN_WOW, "Guild Name: %s - Score: %d - Death: %d", netmsg->szName, netmsg->wClubKiller, netmsg->wClubKilled);
	}
	break;
	
		/*dmk14 freepk*/
	case NET_MSG_GCTRL_GM_FREEPK_A2C_BATTLEINFO_PC:
		{
			GLMSG::SNET_GM_FREEPK_A2C_BATTLEINFO_PC *pNetMsg = (GLMSG::SNET_GM_FREEPK_A2C_BATTLEINFO_PC *) nmg;
			m_sBrightEventMap = pNetMsg->sFreePkID;
			if ( pNetMsg->dwPKTIME != 0 )
			{
				m_bBRIGHTEVENT = true;
				m_sBrightEventMap = pNetMsg->sFreePkID;

				m_bFreePKMatch = true;
				m_fFreePKMatchTimer = (float)pNetMsg->dwPKTIME;

				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("BRIGHT_EVENT_START") );
			}
		}
		break;
	case NET_MSG_GCTRL_REQ_BATTLE_RANKING_FB:
		{
			GLMSG::SNET_REQ_BATTLE_RANKING_FB* pNetMsg = (GLMSG::SNET_REQ_BATTLE_RANKING_FB*)nmg;

			CString strTemp;
			SMAPNODE* pMapNode = GLGaeaClient::GetInstance().FindMapNode( SNATIVEID(pNetMsg->dwID) );
			if (pMapNode) strTemp = pMapNode->strMapName.c_str();

			for (int i = 0; i < 10; i++)
			{
				if (pNetMsg->sRanking_[i].nIndex < 0)		continue;

				int nSize = m_vecBattleRankingP.size();				
				if ( i < nSize )
				{
					m_vecBattleRankingP[i] = pNetMsg->sRanking_[i];
				}
				else
				{
					m_vecBattleRankingP.resize( i+1 );
					m_vecBattleRankingP[i] = pNetMsg->sRanking_[i];
				}
			}
			CInnerInterface::GetInstance().GetBattleRankingDisplay()->RefreshBattleRank();
			CInnerInterface::GetInstance().GetBattleRankingDisplay()->SetBattleTitle(strTemp);

			GLCharacter* pCharacter = GetCharacter();
			if (pCharacter) pCharacter->ReqMyBattleRanking();
		}
		break;
	case NET_MSG_GCTRL_REQ_MY_BATTLE_RANKING_FB:
		{
			GLMSG::SNET_REQ_MY_BATTLE_RANKING_FB* pNetMsg = (GLMSG::SNET_REQ_MY_BATTLE_RANKING_FB*)nmg;

			if (pNetMsg->sRanking.nIndex >= 0)
				CInnerInterface::GetInstance().GetBattleRankingDisplay()->AddMyBattleRanking(pNetMsg->sRanking);
		}
		break;
	case NET_MSG_GCTRL_REQ_CLUB_BATTLE_RANKING_FB:
		{
			GLMSG::SNET_REQ_CLUB_BATTLE_RANKING_FB* pNetMsg = (GLMSG::SNET_REQ_CLUB_BATTLE_RANKING_FB*)nmg;
			CString strTemp;
			SMAPNODE* pMapNode = GLGaeaClient::GetInstance().FindMapNode( SNATIVEID(pNetMsg->dwID) );
			if (pMapNode) strTemp = pMapNode->strMapName.c_str();

			for (int i = 0; i < 10; i++)
			{
				if (pNetMsg->sRanking_[i].nIndex < 0)		continue;

				int nSize = m_vecBattleRankingC.size();				
				if ( i < nSize )
				{
					m_vecBattleRankingC[i] = pNetMsg->sRanking_[i];
				}
				else
				{
					m_vecBattleRankingC.resize( i+1 );
					m_vecBattleRankingC[i] = pNetMsg->sRanking_[i];
				}
			}

			CInnerInterface::GetInstance().GetBattleRankingDisplay()->RefreshBattleRank();
			CInnerInterface::GetInstance().GetBattleRankingDisplay()->SetBattleTitle(strTemp);

			GLCharacter* pCharacter = GetCharacter();
			if (pCharacter) pCharacter->ReqMyBattleRanking();
		}
		break;
	case NET_MSG_GCTRL_REQ_MY_CLUB_BATTLE_RANKING_FB:
		{
			GLMSG::SNET_REQ_MY_CLUB_BATTLE_RANKING_FB* pNetMsg = (GLMSG::SNET_REQ_MY_CLUB_BATTLE_RANKING_FB*)nmg;

			if (pNetMsg->sRanking.nIndex >= 0)
				CInnerInterface::GetInstance().GetBattleRankingDisplay()->AddMyBattleRanking(pNetMsg->sRanking);
		}
		break;
		
	//pandora
	case NET_MSG_PANDORA_BOX_REWARD_RARE_BRD:
		{
			GLMSG::SNET_PANDORA_BOX_REWARD_RARE_BRD* pNetMsg = ( GLMSG::SNET_PANDORA_BOX_REWARD_RARE_BRD* ) nmg;
		}
		break;
	case NET_MSG_PANDORA_BOX_RESULT:
		{
			GLMSG::SNET_PANDORA_BOX_RESULT* pNetMsg = ( GLMSG::SNET_PANDORA_BOX_RESULT* ) nmg;
			GetCharacter()->m_mapPandoraResults.clear();

			for ( int i = 0; i < GLPANDORA_BOX::PANDORA_RESULT_SIZE; ++i )
				GetCharacter()->m_mapPandoraResults.insert( std::make_pair( pNetMsg->sBOX[i].nIndex, pNetMsg->sBOX[i] ) );

			GetCharacter()->m_sPandoraStatus = pNetMsg->sStatus;
			CInnerInterface::GetInstance().MsgProcess( nmg );
		}
		break;
	case NET_MSG_PANDORA_BOX_BUY_ITEM_FB:
		{
			GLMSG::SNET_PANDORA_BOX_BUY_ITEM_FB* pNetMsg = ( GLMSG::SNET_PANDORA_BOX_BUY_ITEM_FB* ) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMPANDORA_ITEM_BUY_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_ITEM_BUY_FAILED") );
				break;
			case EMPANDORA_ITEM_BUY_GOLD_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_ITEM_BUY_GOLD_OK") );
				break;
			case EMPANDORA_ITEM_BUY_PREM_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_ITEM_BUY_PREM_OK") );
				break;
			case EMPANDORA_ITEM_REMOVE_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_ITEM_REMOVE_OK") );
				break;
			case EMPANDORA_ITEM_BUY_NOGOLD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_ITEM_BUY_NOGOLD") );
				break;
			case EMPANDORA_ITEM_BUY_NOEP:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_ITEM_BUY_NOEP") );
				break;

			}
			CInnerInterface::GetInstance().MsgProcess( nmg );
		}
		break;
	case NET_MSG_PANDORA_BOX_CLEAR_RESULTS_FB:
		{
			GLMSG::SNET_PANDORA_BOX_CLEAR_RESULTS_FB* pNetMsg = ( GLMSG::SNET_PANDORA_BOX_CLEAR_RESULTS_FB* ) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMPANDORA_CLEAR_RESULT_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_CLEAR_RESULT_FAILED") );
				break;
			case EMPANDORA_CLEAR_RESULT_OK:
				GetCharacter()->m_mapPandoraResults.clear();
				break;
			}
		}
		break;
	case NET_MSG_PANDORA_BOX_OPEN_FB:
		{
			GLMSG::SNET_PANDORA_BOX_OPEN_FB* pNetMsg = ( GLMSG::SNET_PANDORA_BOX_OPEN_FB* ) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMPANDORA_OPEN_BOX_FB_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_OPEN_BOX_FB_FAILED") );
				break;
			case EMPANDORA_OPEN_BOX_FB_NOEP:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_OPEN_BOX_FB_NOEP") );
				break;
			case EMPANDORA_OPEN_BOX_FB_NOGOLD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_OPEN_BOX_FB_NOGOLD") );
				break;
			case EMPANDORA_OPEN_BOX_GOLD_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_OPEN_BOX_GOLD_FB_OK") );
				break;
			case EMPANDORA_OPEN_BOX_PREM_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMPANDORA_OPEN_BOX_PREM_FB_OK") );
				break;
			}
		}
		break;
	case NET_MSG_PANDORA_BOX_SETTING_INFO:
		{
			GLMSG::SNET_PANDORA_BOX_SETTING_INFO* pNetMsg = ( GLMSG::SNET_PANDORA_BOX_SETTING_INFO* ) nmg;
			GLCONST_CHAR::sPandoraSetting.fPremChance = pNetMsg->fPremChance;
			GLCONST_CHAR::sPandoraSetting.llGold = pNetMsg->llGold;
			GLCONST_CHAR::sPandoraSetting.wEP = pNetMsg->wEP;
			GLCONST_CHAR::sPandoraSetting.fReqDelay = pNetMsg->fReqDelay;
		}
		break;

	case NET_MSG_GCTRL_PERIOD:
		{
			GLMSG::SNET_PERIOD *pNetMsg = reinterpret_cast<GLMSG::SNET_PERIOD *> ( nmg );
			GLPeriod::GetInstance().SetPeriod ( pNetMsg->sPRERIODTIME );
		}
		break;

	case NET_MSG_GCTRL_WEATHER:
		{
			GLMSG::SNETPC_WEATHER *pNetMsg = reinterpret_cast<GLMSG::SNETPC_WEATHER *> ( nmg );
			GLPeriod::GetInstance().SetWeather ( pNetMsg->dwWeather );
		}
		break;

	case NET_MSG_GCTRL_MAPWEATHER:
		{
			GLMSG::SNETPC_MAPWEATHER *pNetMsg = reinterpret_cast<GLMSG::SNETPC_MAPWEATHER *> ( nmg );
			DWORD i;
			GLPeriod::GetInstance().ResetOneMapWeather();
			for( i = 0; i < pNetMsg->dwMapWeatherSize; i++ )
			{
				GLPeriod::GetInstance().SetOneMapActiveWeather( pNetMsg->MapWeather[i], FALSE );
			}
			
		}
		break;

	case NET_MSG_GCTRL_WHIMSICAL:
		{
			GLMSG::SNETPC_WHIMSICALWEATHER *pNetMsg = reinterpret_cast<GLMSG::SNETPC_WHIMSICALWEATHER *> ( nmg );
			DxWeatherMan::GetInstance()->ReceiveWhimsical ( pNetMsg->dwWhimsical );
		}
		break;

	case NET_MSG_GCTRL_MAPWHIMSICAL:
		{
			GLMSG::SNETPC_MAPWHIMSICALWEATHER *pNetMsg = reinterpret_cast<GLMSG::SNETPC_MAPWHIMSICALWEATHER *> ( nmg );
			DxWeatherMan::GetInstance()->ReceiveMapWhimsical ( pNetMsg->MapWeather.map_mID, pNetMsg->MapWeather.map_sID, 
															   pNetMsg->MapWeather.dwWhimsicalWeather );
		}
		break;

	case NET_MSG_GCTRL_DROP_ITEM:
		{
			GLMSG::SNETDROP_ITEM *pNetMsg = reinterpret_cast<GLMSG::SNETDROP_ITEM *> ( nmg );
			m_pLandMClient->DropItem ( &pNetMsg->Data );
		}
		break;

	case NET_MSG_GCTRL_DROP_MONEY:
		{
			GLMSG::SNETDROP_MONEY *pNetMsg = reinterpret_cast<GLMSG::SNETDROP_MONEY *> ( nmg );
			m_pLandMClient->DropMoney ( pNetMsg->lnAmount, pNetMsg->vPos, pNetMsg->dwGlobID, pNetMsg->fAge );
		}
		break;


	case NET_MSG_GCTRL_DROP_PC:
		{
			GLMSG::SNETDROP_PC *pNetMsg = (GLMSG::SNETDROP_PC*) nmg;
			SDROP_CHAR &sDropChar = pNetMsg->Data;
			if ( sDropChar.sMapID == m_pLandMClient->GetMapID() )
			{
				if ( sDropChar.dwGaeaID == GETMYGAEAID() )
				{
					CDebugSet::ToListView ( "[ERROR] Drop Char : %s, %d 자기자신의 아이디.", sDropChar.szName, sDropChar.dwGaeaID );
				}
				else
				{
					//CDebugSet::ToListView ( "Drop Char : %s, %d", sDropChar.szName, sDropChar.dwGaeaID );
					m_pLandMClient->DropChar ( &sDropChar );
				}
			}
			else
			{
				CDebugSet::ToListView ( "sDropChar.sMapID != m_pLandMClient->GetMapID()" );
			}
		}
		break;

	case NET_MSG_GCTRL_DROP_CROW:
		{
			GLMSG::SNETDROP_CROW *pNetMsg = (GLMSG::SNETDROP_CROW*) nmg;
			SDROP_CROW &sDropCrow = pNetMsg->Data;
			if ( sDropCrow.sMapID == m_pLandMClient->GetMapID() )
			{
				//CDebugSet::ToListView ( "Drop Crow : [%d,%d], GLOB ID %d",
				//	sDropCrow.sNativeID.wMainID, sDropCrow.sNativeID.wSubID, sDropCrow.dwGlobID);

				m_pLandMClient->DropCrow ( &sDropCrow );
			}
			else
			{
				CDebugSet::ToListView ( "sDropCrow.sMapID != m_pLandMClient->GetMapID()" );
			}
		}
		break;
	case NET_MSG_GCTRL_DROP_MATERIAL:
		{
			GLMSG::SNETDROP_MATERIAL *pNetMsg = (GLMSG::SNETDROP_MATERIAL*) nmg;
			SDROP_MATERIAL &sDropMaterial = pNetMsg->Data;
			if ( sDropMaterial.sMapID == m_pLandMClient->GetMapID() )
			{
				m_pLandMClient->DropMaterial ( &sDropMaterial );
			}
			else
			{
				CDebugSet::ToListView ( "sDropMaterial.sMapID != m_pLandMClient->GetMapID()" );
			}
		}
		break;

	

	case NET_MSG_GCTRL_CHANGE_NAMEMAP:
		{
			GLMSG::SNETPC_CHANGE_NAMEMAP *pNetMsg = (GLMSG::SNETPC_CHANGE_NAMEMAP*) nmg;
			m_pLandMClient->ChangeCharMap ( pNetMsg->szOldName, pNetMsg->szNewName );
		}
		break;

	case NET_MSG_GCTRL_DROP_OUT:		DropOutCrow(nmg);	break;
	case NET_MSG_GCTRL_REQ_GATEOUT_FB:	MoveActiveMap(nmg);	break;
	case NET_MSG_GCTRL_CREATE_INSTANT_MAP_FB: CreateInstantMap(nmg);	break;
	case NET_MSG_GCTRL_REQ_REBIRTH_FB:	ReBirthFB(nmg);		break;
	case NET_MSG_GCTRL_REQ_RECALL_FB:	ReCallFB(nmg);		break;
	case NET_MSG_REQ_MUST_LEAVE_MAP_FB: ReLvMustLeaveFB(nmg); break;
	case NET_MSG_GCTRL_REQ_BUS_FB:		ReqBusFB(nmg);		break;
	case NET_MSG_GCTRL_REQ_TAXI_FB:		ReqTaxiFB(nmg);		break;
	case NET_MSG_GCTRL_REQ_TELEPORT_FB:	TeleportFB(nmg);	break;

		/*event map move, Juver, 2017/08/25 */
	case NET_MSG_GCTRL_REQ_EVENT_MOVEMAP_FB:
		{
			GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB *pnetmsg = (GLMSG::SNETPC_REQ_EVENT_MOVEMAP_FB *) nmg;
			switch( pnetmsg->emFB )
			{
			case EMEVENT_MOVEMAP_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEVENT_MOVEMAP_FB_FAIL") );
				break;
			case EMEVENT_MOVEMAP_FB_CONDITION:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMEVENT_MOVEMAP_FB_CONDITION") );
				break;
			case EMEVENT_MOVEMAP_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMEVENT_MOVEMAP_FB_OK") );
				break;
			};
		}break;

	//	pc client에게.
	//
	case NET_MSG_GCTRL_MOVESTATE_BRD:
	case NET_MSG_GCTRL_JUMP_POS_BRD:

	case NET_MSG_GCTRL_GOTO_BRD:
	case NET_MSG_GCTRL_ATTACK_BRD:
	case NET_MSG_GCTRL_ATTACK_CANCEL_BRD:
	case NET_MSG_GCTRL_ATTACK_AVOID_BRD:
	case NET_MSG_GCTRL_ATTACK_DAMAGE_BRD:
	case NET_MSG_GCTRL_PUTON_RELEASE_BRD:
	case NET_MSG_GCTRL_PUTON_UPDATE_BRD:
	case NET_MSG_GCTRL_PUTON_CHANGE_BRD:
	case NET_MSG_GCTRL_REQ_LEVELUP_BRD:
	case NET_MSG_GCTRL_CONFRONT_END2_CLT_BRD:

	////////////////////////////////////////////////////
	//MMR Rank
	case NET_MSG_GCTRL_REQ_RANKNAME_BRD:
	case NET_MSG_GCTRL_REQ_RANKMARK_BRD:
	////////////////////////////////////////////////////
	case NET_MSG_GCTRL_UPDATE_STATE_BRD:
	case NET_MSG_GCTRL_PARTY_BRD:

	case NET_MSG_GCTRL_UPDATE_PASSIVE_BRD:
	case NET_MSG_GCTRL_UPDATE_FLAGS:

	case NET_MSG_GCTRL_REQ_GESTURE_BRD:

	case NET_MSG_GCTRL_PMARKET_OPEN_BRD:
	case NET_MSG_GCTRL_PMARKET_CLOSE_BRD:
	case NET_MSG_GCTRL_PMARKET_ITEM_INFO_BRD:
	case NET_MSG_GCTRL_PMARKET_ITEM_UPDATE_BRD:
	case NET_MSG_GCTRL_CLUB_INFO_BRD:
	case NET_MSG_GCTRL_CLUB_INFO_MARK_BRD:
	case NET_MSG_GCTRL_CLUB_INFO_NICK_BRD:
	case NET_MSG_GCTRL_CLUB_DEL_BRD:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_BRD:
	case NET_MSG_GCTRL_UPDATE_BRIGHT_BRD:

	case NET_MSG_GCTRL_INVEN_HAIR_CHANGE_BRD:
	case NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE_BRD:
	case NET_MSG_GCTRL_INVEN_FACE_CHANGE_BRD:
	case NET_MSG_GCTRL_INVEN_RENAME_BRD:

	case NET_MSG_GCTRL_QITEMFACT_BRD:
	case NET_MSG_GCTRL_QITEMFACT_END_BRD:
	case NET_MSG_GCTRL_EVENTFACT_BRD:
	case NET_MSG_GCTRL_EVENTFACT_END_BRD:

	case NET_MSG_GCTRL_ACTIVE_VEHICLE_BRD:
	case NET_MSG_GCTRL_GET_VEHICLE_BRD:
	case NET_MSG_GCTRL_UNGET_VEHICLE_BRD:
	case NET_MSG_VEHICLE_ACCESSORY_DELETE_BRD:
	case NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD_BRD:
	case NET_MSG_VEHICLE_REMOVE_SLOTITEM_BRD:

	case NET_MSG_GCTRL_ITEMSHOPOPEN_BRD:
	
	case NET_MSG_GCTRL_SKILL_CANCEL_BRD:
	case NET_MSG_REQ_GATHERING_BRD:
	case NET_MSG_REQ_GATHERING_RESULT_BRD:
	case NET_MSG_REQ_GATHERING_CANCEL_BRD:

	case NET_MSG_GCTRL_LANDEFFECT:

		/*itemfood system, Juver, 2017/05/25 */
	case NET_MSG_GCTRL_FITEMFACT_BRD:

		/*vehicle booster system, Juver, 2017/08/12 */
	case NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_START_BRD:
	case NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_RESET_BRD:

		/*system buffs, Juver, 2017/09/05 */
	case NET_MSG_GCTRL_SYSTEM_BUFF_BRD:

		/*activity system, Juver, 2017/11/01 */
	case NET_MSG_GCTRL_ACTIVITY_COMPLETE_BRD:
	case NET_MSG_GCTRL_CHARACTER_BADGE_CHANGE_BRD:
	case NET_MSG_GCTRL_CODEX_COMPLETE_BRD: /* Codex, Archie 02/16/24 */

		/*bike color , Juver, 2017/11/13 */
	case NET_MSG_VEHICLE_REQ_CHANGE_COLOR_BRD:

		/*change scale card, Juver, 2018/01/04 */
	case NET_MSG_GCTRL_INVEN_SCALE_CHANGE_BRD:

		/*item color, Juver, 2018/01/10 */
	case NET_MSG_GCTRL_INVEN_ITEMCOLOR_CHANGE_BRD:

		/*pvp capture the flag, Juver, 2018/01/30 */
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_PLAYER_TEAM_BRD:
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_FLAG_HOLD_BRD:

		/* car, cart color, Juver, 2018/02/14 */
	case NET_MSG_VEHICLE_REQ_CHANGE_CAR_COLOR_BRD:

		/*12-9-14, Battle Pass - CNDev*/
	case NET_MSG_GCTRL_BATTLEPASS_COMPLETE_BRD:

		/*12-16-24, daily login - CNDev*/
	case NET_MSG_GCTRL_DAILYLOGIN_COMPLETE_BRD:
	//	/*hide costume, EJCode, 2018/11/18 */
	//case NET_MSG_GCTRL_HIDE_COSTUME_BRD:
		{
			GLMSG::SNETPC_BROAD *pNetMsg = (GLMSG::SNETPC_BROAD *) nmg;
			
			if ( pNetMsg->dwGaeaID==m_Character.m_dwGaeaID )
			{
				m_Character.MsgProcess ( nmg );
			}
			else
			{
				PGLCHARCLIENT pChar = m_pLandMClient->GetChar ( pNetMsg->dwGaeaID );
				if ( !pChar )
				{
					CDebugSet::ToListView ( "[PGLCHARCLIENT NULL] 수신 PC가 없는 메세지 발생. gaeaid %d", pNetMsg->dwGaeaID );
					return;
				}

				pChar->MsgProcess ( nmg );
			}
		}
		break;

	case NET_MSG_GCTRL_UPDATE_STATE:
	case NET_MSG_GCTRL_UPDATE_EXP:
	case NET_MSG_GCTRL_UPDATE_MONEY:
	case NET_MSG_GCTRL_UPDATE_SP:
	case NET_MSG_GCTRL_UPDATE_LP:
	case NET_MSG_GCTRL_UPDATE_SKP:
	case NET_MSG_GCTRL_UPDATE_BRIGHT:
	case NET_MSG_GCTRL_UPDATE_STATS:

	case NET_MSG_GCTRL_PICKUP_MONEY:
	case NET_MSG_GCTRL_PICKUP_ITEM:

	case NET_MSG_GCTRL_REQ_HOLD_FB:

	case NET_MSG_GCTRL_INVEN_INSERT:
	case NET_MSG_GCTRL_REQ_VNINVEN_TO_INVEN_FB:
	case NET_MSG_GCTRL_INVEN_DELETE:
	case NET_MSG_GCTRL_INVEN_DEL_INSERT:

	case NET_MSG_GCTRL_ITEM_COOLTIME_UPDATE:
	case NET_MSG_GCTRL_ITEM_COOLTIME_ERROR:

	case NET_MSG_GCTRL_PUTON_RELEASE:
	case NET_MSG_GCTRL_PUTON_UPDATE:
	case NET_MSG_GCTRL_PUTON_CHANGE:

	case NET_MSG_GCTRL_REQ_SKILLQ_FB:
	case NET_MSG_GCTRL_REQ_ACTIONQ_FB:
	
	case NET_MSG_GCTRL_ATTACK_AVOID:
	case NET_MSG_GCTRL_ATTACK_DAMAGE:
	case NET_MSG_GCTRL_DEFENSE_SKILL_ACTIVE:

	case NET_MSG_GCTRL_SUMMON_ATTACK_AVOID:
	case NET_MSG_GCTRL_SUMMON_ATTACK_DAMAGE:

	case NET_MSG_GCTRL_REQ_LEVELUP_FB:
	case NET_MSG_GCTRL_REQ_STATSUP_FB:
	case NET_MSG_GCTRL_REQ_LEARNSKILL_FB:
	case NET_MSG_GCTRL_REQ_SKILLUP_FB:
	case NET_MSG_GCTRL_SKILL_EXPIRE_FB:
	////////////////////////////////////////////
	//MMR Rank
	case NET_MSG_GCTRL_REQ_RANKNAME_FB:
	case NET_MSG_GCTRL_REQ_RANKMARK_FB:
	////////////////////////////////////////////

	case NET_MSG_GCTRL_INVEN_DRUG_UPDATE:
	case NET_MSG_GCTRL_PUTON_DRUG_UPDATE:

	case NET_MSG_GCTRL_REQ_SKILL_FB:
	case NET_MSG_GCTRL_REQ_SKILL_TARGET_FB:
	case NET_MSG_REQ_SKILL_REVIVEL_FAILED:
	case NET_MSG_GCTRL_SKILLCONSUME_FB:

	case NET_QBOX_OPTION_MEMBER:

	case NET_MSG_GCTRL_REQ_GETSTORAGE_FB:
	case NET_MSG_GCTRL_REQ_GETSTORAGE_ITEM:

	case NET_MSG_GCTRL_STORAGE_INSERT:
	case NET_MSG_GCTRL_STORAGE_DELETE:
	case NET_MSG_GCTRL_STORAGE_ITEM_UPDATE:

	case NET_MSG_GCTRL_STORAGE_DEL_INSERT:
	case NET_MSG_GCTRL_STORAGE_DRUG_UPDATE:

	case NET_MSG_GCTRL_STORAGE_UPDATE_MONEY:

	case NET_MSG_GCTRL_TRADE_FB:
	case NET_MSG_GCTRL_TRADE_AGREE_TAR:
	case NET_MSG_GCTRL_TRADE_LOCK_TAR: /*trade lock, Juver, 2018/01/02 */
	case NET_MSG_GCTRL_TRADE_ITEM_REGIST_TAR:
	case NET_MSG_GCTRL_TRADE_ITEM_REMOVE_TAR:
	case NET_MSG_GCTRL_TRADE_MONEY_TAR:
	case NET_MSG_GCTRL_TRADE_COMPLETE_TAR:
	case NET_MSG_GCTRL_TRADE_CANCEL_TAR:


	case NET_MSG_GCTRL_INVEN_ITEM_UPDATE:
	case NET_MSG_GCTRL_INVEN_GRINDING_FB:
	case NET_MSG_GCTRL_INVEN_BOXOPEN_FB:
	case NET_MSG_GCTRL_INVEN_DISGUISE_FB:
	case NET_MSG_GCTRL_INVEN_CLEANSER_FB:
	case NET_MSG_GCTRL_INVEN_DEL_ITEM_TIMELMT:
	case NET_MSG_GCTRL_CHARGED_ITEM_GET_FB:
	case NET_MSG_GCTRL_CHARGED_ITEM_DEL:
	case NET_MSG_GCTRL_INVEN_RESET_SKST_FB:
	case NET_MSG_GCTRL_GET_CHARGEDITEM_FROMDB_FB:

	case NET_MSG_GCTRL_INVEN_HAIR_CHANGE_FB:
	case NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE_FB:
	case NET_MSG_GCTRL_INVEN_FACE_CHANGE_FB:
	case NET_MSG_GCTRL_INVEN_GENDER_CHANGE_FB:
	case NET_MSG_GCTRL_INVEN_RENAME_FB:	

	case NET_MSG_GCTRL_REGEN_GATE_FB:

	// Contribution Item - JX
	case NET_MSG_GCTRL_CARD_CONTRIBUTION_FB:

	case NET_MSG_GCTRL_CONFRONT_FB:
	case NET_MSG_GCTRL_CONFRONT_START2_CLT:
	case NET_MSG_GCTRL_CONFRONT_FIGHT2_CLT:
	case NET_MSG_GCTRL_CONFRONT_END2_CLT:

	case NET_MSG_GCTRL_CONFRONTPTY_START2_CLT:
	case NET_MSG_GCTRL_CONFRONTPTY_END2_CLT:

	case NET_MSG_GCTRL_CONFRONTCLB_START2_CLT:
	case NET_MSG_GCTRL_CONFRONTCLB_END2_CLT:

	case NET_MSG_GCTRL_CONFRONT_RECOVE:
	case NET_MSG_GCTRL_CURE_FB:

	case NET_MSG_GCTRL_CHARRESET_FB:
	case NET_MSG_GCTRL_INVEN_CHARCARD_FB:
	case NET_MSG_GCTRL_INVEN_STORAGECARD_FB:
	case NET_MSG_GCTRL_INVEN_STORAGEOPEN_FB:
	case NET_MSG_GCTRL_INVEN_REMODELOPEN_FB:
	
	case NET_MSG_GCTRL_INVEN_GARBAGEOPEN_FB:
	case NET_MSG_GCTRL_GARBAGE_RESULT_FB:
	
	case NET_MSG_GCTRL_INVEN_PREMIUMSET_FB:
	case NET_MSG_CHAT_LOUDSPEAKER_FB:
	case NET_MSG_GCTRL_INVEN_INVENLINE_FB:
	case NET_MSG_GCTRL_INVEN_RANDOMBOXOPEN_FB:
	case NET_MSG_GCTRL_INVEN_DISJUNCTION_FB:
	
	case NET_MSG_GCTRL_PREMIUM_STATE:
	case NET_MSG_GCTRL_STORAGE_STATE:
	case NET_MSG_GCTRL_REVIVE_FB:
	case NET_MSG_GCTRL_GETEXP_RECOVERY_FB:
	case NET_MSG_GCTRL_GETEXP_RECOVERY_NPC_FB:
	case NET_MSG_GCTRL_RECOVERY_FB:
	case NET_MSG_GCTRL_RECOVERY_NPC_FB:

	case NET_MSG_GCTRL_FIRECRACKER_FB:
	case NET_MSG_GCTRL_FIRECRACKER_BRD:

	case NET_MSG_GM_MOVE2GATE_FB:
	case NET_MSG_GCTRL_2_FRIEND_FB:
	case NET_MSG_GM_SHOWMETHEMONEY:


	case NET_MSG_GCTRL_NPC_ITEM_TRADE_FB:

	case NET_MSG_GCTRL_REQ_QUEST_START_FB:

	case NET_MSG_GCTRL_QUEST_PROG_STREAM:
	case NET_MSG_GCTRL_QUEST_PROG_DEL:

	case NET_MSG_GCTRL_QUEST_PROG_STEP_STREAM:
	case NET_MSG_GCTRL_QUEST_PROG_INVEN:

	case NET_MSG_GCTRL_QUEST_END_STREAM:
	case NET_MSG_GCTRL_QUEST_END_DEL:

	case NET_MSG_GCTRL_QUEST_PROG_NPCTALK_FB:
	case NET_MSG_GCTRL_QUEST_PROG_MOBKILL:
	case NET_MSG_GCTRL_QUEST_PARTY_PROG_MOBKILL:
	case NET_MSG_GCTRL_QUEST_PROG_QITEM:
	case NET_MSG_GCTRL_QUEST_PARTY_PROG_QITEM:
	case NET_MSG_GCTRL_QUEST_PROG_REACHZONE:
	case NET_MSG_GCTRL_QUEST_PROG_TIMEOVER:
	case NET_MSG_GCTRL_QUEST_PROG_TIME:
	case NET_MSG_GCTRL_QUEST_PROG_NONDIE:
	case NET_MSG_GCTRL_QUEST_PROG_LEAVEMAP:

	case NET_MSG_GCTRL_QUEST_PROG_INVEN_INSERT:
	case NET_MSG_GCTRL_QUEST_PROG_INVEN_DELETE:
	case NET_MSG_GCTRL_QUEST_PROG_INVEN_TURN:
	case NET_MSG_GCTRL_QUEST_PROG_INVEN_PICKUP:
	case NET_MSG_GCTRL_QUEST_COMPLETE_FB:

	case NET_MSG_GCTRL_PMARKET_TITLE_FB:
	case NET_MSG_GCTRL_PMARKET_REGITEM_FB:
	case NET_MSG_GCTRL_PMARKET_DISITEM_FB:
	case NET_MSG_GCTRL_PMARKET_OPEN_FB:
	case NET_MSG_GCTRL_PMARKET_BUY_FB:

	case NET_MSG_GCTRL_CLUB_INFO_2CLT:
	case NET_MSG_GCTRL_CLUB_DEL_2CLT:
	case NET_MSG_GCTRL_CLUB_INFO_DISSOLUTION:
	case NET_MSG_GCTRL_CLUB_MEMBER_2CLT:
	case NET_MSG_GCTRL_CLUB_NEW_FB:
	case NET_MSG_GCTRL_CLUB_DISSOLUTION_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_MEMBER_REQ_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_DEL_2CLT:
	case NET_MSG_GCTRL_CLUB_MEMBER_DEL_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_SECEDE_FB:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_FB:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_CLT:

	case NET_MSG_GCTRL_CLUB_MARK_CHANGE_2CLT:
	case NET_MSG_GCTRL_CLUB_RANK_2CLT:
	case NET_MSG_GCTRL_CLUB_RANK_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_NICK_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_STATE:
	case NET_MSG_GCTRL_CLUB_MEMBER_POS:
	case NET_MSG_GCTRL_CONFRONT_END2_CLT_MBR:

	case NET_MSG_GCTRL_CLUB_CD_CERTIFY_FB:
	case NET_MSG_GCTRL_CLUB_COMMISSION_FB:

	case NET_MSG_GCTRL_CLUB_STORAGE_RESET:
	case NET_MSG_GCTRL_CLUB_GETSTORAGE_ITEM:

	case NET_MSG_GCTRL_CLUB_STORAGE_INSERT:
	case NET_MSG_GCTRL_CLUB_STORAGE_DELETE:
	case NET_MSG_GCTRL_CLUB_STORAGE_DEL_INS:
	case NET_MSG_GCTRL_CLUB_STORAGE_UPDATE_ITEM:
	case NET_MSG_GCTRL_CLUB_STORAGE_UPDATE_MONEY:

	case NET_MSG_GCTRL_CLUB_NOTICE_FB:
	case NET_MSG_GCTRL_CLUB_NOTICE_CLT:

	case NET_MSG_GCTRL_CLUB_SUBMASTER_FB:
	case NET_MSG_GCTRL_CLUB_SUBMASTER_BRD:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_ADD_CLT:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_CLT:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_CLT:

	case NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_FB:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_FB:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_SEC_FB:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_FB:

	case NET_MSG_GCTRL_CLUB_BATTLE_REQ_FB:
	case NET_MSG_GCTRL_CLUB_BATTLE_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_CLT:
	case NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_CLT2:
	case NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_FB:
	case NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_BATTLE_OVER_CLT:
	case NET_MSG_GCTRL_CLUB_BATTLE_SUBMISSION_REQ_FB:
	case NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE:
	case NET_MSG_GCTRL_CLUB_BATTLE_POINT_UPDATE:
	
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_FB:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_ASK:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_FB:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_ASK:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_SUBMISSION_REQ_FB:

	case NET_MSG_GCTRL_PLAYERKILLING_ADD:
	case NET_MSG_GCTRL_PLAYERKILLING_DEL:

	case NET_MSG_GCTRL_CLUB_MBR_RENAME_CLT:

	case NET_MSG_REBUILD_RESULT:	// ITEMREBUILD_MARK
	case NET_MSG_REBUILD_MOVE_ITEM:
	case NET_MSG_REBUILD_COST_MONEY:
	case NET_MSG_REBUILD_INPUT_MONEY:

	case NET_MSG_GCTRL_UPDATE_LASTCALL:
	case NET_MSG_GCTRL_UPDATE_STARTCALL:

	case NET_MSG_SMS_PHONE_NUMBER_FB:
	case NET_MSG_SMS_SEND_FB:

	case NET_MSG_MGAME_ODDEVEN_FB:		// 미니게임 - 홀짝
	case NET_MSG_CHINA_GAINTYPE:

	case NET_MSG_GM_LIMIT_EVENT_APPLY_START:
	case NET_MSG_GM_LIMIT_EVENT_APPLY_END:

	case NET_MSG_GM_LIMIT_EVENT_BEGIN_FB:
	case NET_MSG_GM_LIMIT_EVENT_END_FB:

	case NET_MSG_GCTRL_PMARKET_SEARCH_ITEM_RESULT:

	/// 베트남 메시지
	case NET_MSG_VIETNAM_TIME_REQ_FB:
	case NET_MSG_VIETNAM_GAIN_EXP:
	case NET_MSG_VIETNAM_GAINTYPE:
	case NET_MSG_VIETNAM_GAIN_MONEY:
	case NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGETNUM_UPDATE:
	case NET_MSG_GCTRL_INVEN_VIETNAM_EXPGET_FB:
	case NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGET_FB:
	case NET_MSG_VIETNAM_ALLINITTIME:

	case NET_MSG_GCTRL_EVENTFACT_INFO:

	case NET_MSG_GM_LIMIT_EVENT_TIME_REQ_FB:
	case NET_MSG_GCTRL_ACTIVE_VEHICLE_FB:
	case NET_MSG_GCTRL_GET_VEHICLE_FB:
	case NET_MSG_GCTRL_UNGET_VEHICLE_FB:
	case NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD_FB:
	case NET_MSG_VEHICLE_REQ_HOLD_TO_SLOT_FB:
	case NET_MSG_VEHICLE_REQ_SLOT_TO_HOLD_FB:
	case NET_MSG_VEHICLE_REMOVE_SLOTITEM_FB:
	case NET_MSG_VEHICLE_ACCESSORY_DELETE:
	case NET_MSG_VEHICLE_UPDATE_CLIENT_BATTERY:
	case NET_MSG_VEHICLE_REQ_GIVE_BATTERY_FB:
	case NET_MSG_VEHICLE_REQ_ITEM_INFO_FB:
	case NET_MSG_UPDATE_TRACING_CHARACTER:
	case NET_MSG_REQ_ATTENDLIST_FB:
	case NET_MSG_REQ_ATTENDANCE_FB:
	case NET_MSG_REQ_ATTEND_REWARD_CLT:
	case NET_MSG_GCTRL_NPC_RECALL_FB:
	case NET_MSG_GCTRL_INVEN_ITEM_MIX_FB:
	case NET_MSG_REQ_GATHERING_FB:
	case NET_MSG_REQ_GATHERING_RESULT:
	case NET_MSG_RETRIEVE_POINTS_FB:

		/*itemfood system, Juver, 2017/05/25 */
	case NET_MSG_GCTRL_INVEN_CONSUME_FOOD_FB: 
	case NET_MSG_GCTRL_INVEN_UNLOCK_FOOD_FB: 

		/*combatpoint logic, Juver, 2017/05/29 */
	case NET_MSG_GCTRL_UPDATE_CP: 

		/*game stats, Juver, 2017/06/21 */
	case NET_MSG_GCTRL_PING_PACKET_AGENT_FB: 
	case NET_MSG_GCTRL_PING_PACKET_FIELD_FB: 

		/*npc shop, Juver, 2017/07/27 */
	case NET_MSG_GCTRL_NPCSHOP_PURCHASE_MONEY_FB: 

		 /*vehicle booster system, Juver, 2017/08/10 */
	case NET_MSG_VEHICLE_REQ_ENABLE_BOOSTER_FB:
	case NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_CHARGE:
	case NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_START:
	case NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_RESET:

		/*contribution point, Juver, 2017/08/23 */
	case NET_MSG_GCTRL_UPDATE_CONTRIBUTION_POINT: 

		/*activity point, Juver, 2017/08/23 */
	case NET_MSG_GCTRL_UPDATE_ACTIVITY_POINT:

		/*item exchange, Juver, 2017/10/13 */
	case NET_MSG_GCTRL_NPC_ITEM_EXCHANGE_TRADE_FB:

		/*product item, Juver, 2017/10/18 */
	case NET_MSG_GCTRL_ITEM_COMPOUND_START_FB:
	case NET_MSG_GCTRL_ITEM_COMPOUND_PROCESS_FB:

		/*activity system, Juver, 2017/10/30 */
	case NET_MSG_GCTRL_ACTIVITY_UPDATE:
	case NET_MSG_GCTRL_ACTIVITY_COMPLETE:
	case NET_MSG_GCTRL_ACTIVITY_NOTIFY_CLIENT:
	/* Codex, Archie 02/16/24 */
	case NET_MSG_GCTRL_CODEX_UPDATE:
	case NET_MSG_GCTRL_CODEX_COMPLETE:
	case NET_MSG_GCTRL_CODEX_NOTIFY_CLIENT:

		/*activity system, Juver, 2017/11/05 */
	case NET_MSG_GCTRL_CHARACTER_BADGE_CHANGE_FB:

		/*charinfoview , Juver, 2017/11/11 */
	case NET_MSG_GCTRL_REQ_CHARINFO_FB:

		/*bike color , Juver, 2017/11/13 */
	case NET_MSG_VEHICLE_REQ_CHANGE_COLOR_FB:

		/*pk info, Juver, 2017/11/17 */
	case NET_MSG_GCTRL_UPDATE_PK_KILL:
	case NET_MSG_GCTRL_UPDATE_PK_DEATH:


	/* Gacha System, MontageDev 7/10/24 */
	case NET_MSG_GCTRL_UPDATE_CHA_PROMISE_POINTS:
	
	/* PK Streak Sigaw - Jxyy */
	case NET_MSG_GCTRL_PK_STREAK:
	case NET_MSG_GCTRL_RESU_STREAK:
	case NET_MSG_GCTRL_PK_STREAK_KILL:

		/*rv card, Juver, 2017/11/25 */
	case NET_MSG_GCTRL_INVEN_RANDOM_OPTION_CHANGE_FB:

		/*nondrop card, Juver, 2017/11/26 */
	case NET_MSG_GCTRL_INVEN_NONDROP_CARD_FB:

		/*change scale card, Juver, 2018/01/04 */
	case NET_MSG_GCTRL_INVEN_SCALE_CHANGE_FB:

		/*item color, Juver, 2018/01/10 */
	case NET_MSG_GCTRL_INVEN_ITEMCOLOR_CHANGE_FB:

		/*item wrapper, Juver, 2018/01/12 */
	case NET_MSG_GCTRL_INVEN_WRAP_FB:
	case NET_MSG_GCTRL_INVEN_UNWRAP_FB:

		/*change school card, Juver, 2018/01/12 */
	case NET_MSG_GCTRL_INVEN_CHANGE_SCHOOL_FB:

		/*item transfer card, Juver, 2018/01/18 */
	case NET_MSG_GCTRL_INVEN_ITEM_TRANSFER_FB:

		/*self buff via NPC, DevArt22, 2023/03/18 */
	case NET_MSG_GCTRL_SELFBUFF_BA_ONLY_FB:
	case NET_MSG_GCTRL_SELFBUFF_FULL_FB:
	case NET_MSG_GCTRL_SELFBUFF_FULL_NO_BA_FB:

		/* car, cart color, Juver, 2018/02/14 */
	case NET_MSG_VEHICLE_REQ_CHANGE_CAR_COLOR_FB:

		/* booster all vehicle, Juver, 2018/02/14 */
	case NET_MSG_ALLVEHICLE_REQ_ENABLE_BOOSTER_FB:
	case NET_MSG_GCTRL_SET_POINTS_FB:
	/* Boss Spawn Viewer, Review000 */
	case NET_MSG_GCTRL_BOSS_DETAIL_FB:

		/* Reset Stats Item */
	case NET_MSG_GCTRL_INVEN_RESET_STATS_FB:

		/* additional gm commands, Juver, 2018/03/01 */
	case NET_MSG_GCTRL_GM_COMMAND_INVEN_OPEN_FB:
	case NET_MSG_GCTRL_GM_COMMAND_GET_SKILL_FB:
	case NET_MSG_GCTRL_GM_COMMAND_GET_SKILL2_FB:
	case NET_MSG_GCTRL_GM_COMMAND_GET_LEVEL_FB:
	case NET_MSG_GCTRL_GM_COMMAND_GET_STATS_FB:
	case NET_MSG_GCTRL_GM_COMMAND_GET_STATSP_FB:
	case NET_MSG_GCTRL_GM_COMMAND_GET_SKILLP_FB:

		/*modern character window, Juver, 2018/03/17 */
	case NET_MSG_GCTRL_REQ_STATSUP_MODERN_FB:

		/*item random option rebuild, Juver, 2018/07/04 */
	case NET_MSG_GCTRL_INVEN_ITEM_RANDOM_OPTION_REBUILD_FB:

		/*inventory sort, Juver, 2018/07/07 */
	case NET_MSG_GCTRL_INVEN_ITEM_SORT_FB:
	case NET_MSG_GCTRL_INVEN_ITEM_SORT_UPDATE:

		/*specific item box, Juver, 2018/09/03 */
	case NET_MSG_GCTRL_INVEN_REQ_OPEN_ITEM_BOX_SPECIFIC_FB:

	//	/*hide costume, EJCode, 2018/11/18 */
	//case NET_MSG_GCTRL_HIDE_COSTUME_FB:

		/*extreme auto tab, Juver, 2018/09/10 */
	case NET_MSG_GCTRL_REQ_SKILL_TAB_CHANGE_FB:

		/*client tick count check, EJCode, 2018/11/26 */
	case NET_MSG_GCTRL_CLIENT_TICK_COUNT_CHECK_REQUEST:

		/*gm command send item, Juver, 2019/03/23 */
	case NET_MSG_GCTRL_GM_COMMAND_A2C_SEND_ITEM_FB:
	case NET_MSG_GCTRL_GM_COMMAND_A2C_SEND_ITEM_NOTICE:

	case NET_MSG_GCTRL_CAPTURE_PUTON_DROP:

	case NET_MSG_GCTRL_GM_COMMAND_GET_CROW_TIME_FB:

	case NET_MSG_GCTRL_UPDATE_WAR_CHIPS:
	case NET_MSG_GCTRL_UPDATE_GAME_POINTS:

	case NET_MSG_GCTRL_CURRENCY_SHOP_PURCHASE_FB:

		/* personal lock system, Juver, 2019/12/06 */
	case NET_MSG_GCTRL_PERSONAL_LOCK_REQUEST_FB:
	case NET_MSG_GCTRL_PERSONAL_LOCK_CREATE_PIN_FB:
	case NET_MSG_GCTRL_PERSONAL_LOCK_INPUT_PIN_FB:
	case NET_MSG_GCTRL_PERSONAL_LOCK_RESET_PIN_FB:
	case NET_MSG_GCTRL_PERSONAL_LOCK_CHANGE_PIN_FB:
	case NET_MSG_GCTRL_PERSONAL_LOCK_RECOVER_PIN_FB:

		/* user flag restricted, Juver, 2020/04/21 */
	case NET_MSG_GCTRL_GM_USER_RESTRICT_UPDATE:

		/* gm command inven clear, Juver, 2020/05/09 */
	case NET_MSG_GCTRL_GM_INVEN_CLEAR_FB:

		/* skill delay addition, Juver, 2020/12/09 */
	case NET_MSG_GCTRL_F2C_SKILL_DELAY_ADJUSTMENT:
	case NET_MSG_GCTRL_F2C_SKILL_DELAY_ADJUSTMENT_MULTI:

		/* play time system, Juver, 2021/01/26 */
	case NET_MSG_GCTRL_UPDATE_PLAY_TIME:
	case NET_MSG_GCTRL_UPDATE_PLAY_POINT:

		/* pk combo, Juver, 2021/02/09 */
	case NET_MSG_GCTRL_UPDATE_PK_COMBO:

		/* game notice, Juver, 2021/06/12 */
	case NET_MSG_GCTRL_GM_GAME_NOTICE_RELOAD_FB:

		/* chaos machine, Juver, 2021/07/10 */
	case NET_MSG_GCTRL_INVEN_CHAOS_MACHINE_FB:

		/* max level notice, Juver, 2021/07/26 */
	case NET_MSG_GCTRL_MAX_LEVEL_NOTICE_CLIENT:

		/* set item option, Juver, 2021/09/04 */
	case NET_MSG_GCTRL_INVEN_SET_OPTION_INSERT_FB:
	case NET_MSG_GCTRL_INVEN_SET_OPTION_REROLL_FB:

	case NET_MSG_GCTRL_NPC_CONTRIBUTION_SHOP_PURCHASE_FB:
	case NET_MSG_GCTRL_REQ_MAXRV_PREVIEW_FB:
	case NET_MSG_GCTRL_REQ_MAXRV_PREVIEW_DATA_INIT:
	case NET_MSG_GCTRL_REQ_MAXRV_PREVIEW_DATA_MOB_GEN:
	case NET_MSG_GCTRL_REQ_MAXRV_PREVIEW_DATA_REBUILD:

	case NET_MSG_GCTRL_GM_GIS_RELOAD_FB:
	case NET_MSG_GCTRL_GM_GIS_RELOAD_CLOSE:
	case NET_MSG_GCTRL_GM_GIS_RELOAD_DONE:

	case NET_MSG_GCTRL_REQ_PURCHASE_SKILL_FB:

	case NET_MSG_ENCHANT_NOTIFY_CLIENT:
	case NET_MSG_COMBO_WHORE_NOTIFY_CLIENT:

	case NET_MSG_GCTRL_REQ_AUTO_POTION_FB:
	case NET_MSG_GCTRL_REQ_AUTO_PILOT_SET_FB:
	case NET_MSG_GCTRL_REQ_AUTO_PILOT_RESET_FB:

	case NET_MSG_GCTRL_CARD_REBORN_A_FB:
	case NET_MSG_GCTRL_CARD_REBORN_B_FB:

	case NET_MSG_GCTRL_REQ_EXCHANGE_ITEM_FB:
	case NET_MSG_GCTRL_UPDATE_CHANGE_ITEM_POINTS:
	case NET_MSG_GCTRL_REQ_EXCHANGE_ITEM_CARD_FB:

	case NET_MSG_GCTRL_UPDATE_COMBAT_RECORD:

	case NET_MSG_GCTRL_CAPTCHA_UPDATE_SHOW_AC:
	case NET_MSG_GCTRL_CAPTCHA_UPDATE_INPUT_AC:
	
		// LG-7 GlobalRanking
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_FB:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_KILL_AGT:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_BR_AGT:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SW_AGT:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AR_AGT:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SH_AGT:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_EX_AGT:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_SC_AGT:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_AS_AGT:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RICH_AGT:
	/*Top Level*/
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_LEVEL_AGT:
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_GUILD_AGT:
	/*Top Resu, Mhundz */
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RESU_AGT:
	/*Top MMR, Mhundz */
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_MMR_AGT:
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	
		/*12-9-14, Battle Pass - CNDev*/
	case NET_MSG_GCTRL_UPDATE_BATTLEPASS_EXP: 
	case NET_MSG_GCTRL_UPDATE_BATTLEPASS_LEVEL: 

	case NET_MSG_GCTRL_BATTLEPASS_UPDATE:
	case NET_MSG_GCTRL_BATTLEPASS_COMPLETE:
	case NET_MSG_GCTRL_BATTLEPASS_NOTIFY_CLIENT:

	case NET_MSG_GCTRL_REQ_BATTLEPASS_REWARD_DATA:

	case NET_MSG_GCTRL_REQ_BATTLEPASS_PREMIUM_CARD_FB: 
	case NET_MSG_GCTRL_REQ_BATTLEPASS_LEVELUP_CARD_FB: 
	case NET_MSG_GCTRL_REQ_BATTLEPASS_REWARD_CLAIM_FB: 
		/*12-16-24, daily login - CNDev*/
	case NET_MSG_GCTRL_DAILYLOGIN_COMPLETE:
	case NET_MSG_GCTRL_DAILYLOGIN_UPDATE:
	case NET_MSG_GCTRL_DAILYLOGIN_NOTIFY_CLIENT:
	case NET_MSG_GCTRL_REQ_DAILYLOGIN_REWARD_CLAIM_FB: 
	
		/* Add ItemShop, Ssodomain, 11-09-2023, Start */
	case NET_MSG_GCTRL_BUY_ITEMSHOP_ITEM:
	case NET_MSG_GCTRL_GET_ITEMSHOP_FROMDB_FB:
		/* Add ItemShop, Ssodomain, 11-09-2023, End */
		{
			m_Character.MsgProcess ( nmg );
		}
		break;

	case NET_MSG_GCTRL_ACTION_BRD:

	case NET_MSG_GCTRL_SKILLFACT_BRD:
	case NET_MSG_GCTRL_SKILLHOLD_BRD:
	case NET_MSG_GCTRL_STATEBLOW_BRD:
	case NET_MSG_GCTRL_CURESTATEBLOW_BRD:

	case NET_MSG_GCTRL_REQ_SKILL_BRD:
	case NET_MSG_GCTRL_REQ_SKILL_GO_TO_IDLE_BRD:
	case NET_MSG_GCTRL_SKILLHOLD_RS_BRD:
	case NET_MSG_GCTRL_SKILLHOLDEX_BRD:

	case NET_MSG_GCTRL_PUSHPULL_BRD:

		/* push skill logic, Juver, 2020/12/14 */
	case NET_MSG_GCTRL_SKILL_PUSH_BRD:

		/* pull skill logic, Juver, 2020/12/14 */
	case NET_MSG_GCTRL_SKILL_PULL_BRD:

		/* dash skill logic, Juver, 2020/12/14 */
	case NET_MSG_GCTRL_SKILL_DASH_BRD:

		/* skill position shift, Juver, 2020/12/15 */
	case NET_MSG_GCTRL_SKILL_POSITION_SHIFT_BRD:

	case NET_MSG_GCTRL_POSITIONCHK_BRD:
		
		/*skill teleport, Juver, 2018/09/06 */
	case NET_MSG_GCTRL_SKILL_TELEPORT_BRD:

		/*extreme auto tab, Juver, 2018/09/10 */
	case NET_MSG_GCTRL_REQ_SKILL_TAB_CHANGE_BRD:

		/*manual lunchbox remove, Juver, 2018/09/19 */
	case NET_MSG_GCTRL_LUNCHBOX_MANUAL_REMOVE_BRD:

		/* skill buff duration change, Juver, 2020/12/17 */
	case NET_MSG_GCTRL_F2C_SKILL_BUFF_DURATION_CHANGE_TIME_MULTIPLE_BRD:
		{
			GLMSG::SNETCROW_BROAD *pNetMsg = (GLMSG::SNETCROW_BROAD *) nmg;
			GLCOPY* pCopy = GetCopyActor ( pNetMsg->emCrow, pNetMsg->dwID );
			if ( !pCopy )		return;
				
			pCopy->MsgProcess ( nmg );
		}
		break;

	case NET_MSG_GCTRL_CROW_MOVETO:
	case NET_MSG_GCTRL_CROW_ATTACK:
	case NET_MSG_GCTRL_CROW_AVOID:
	case NET_MSG_GCTRL_CROW_DAMAGE:

	case NET_MSG_GCTRL_CROW_SKILL:

		/*pvp tyranny, Juver, 2017/08/24 */
	case NET_MSG_GCTRL_TYRANNY_CROW_OWNER:
	case NET_MSG_GCTRL_TYRANNY_CROW_DAMAGE:

	/*woe Arc Development 08-06-2024*/
	case NET_MSG_GCTRL_WOE_CROW_OWNER:
	case NET_MSG_GCTRL_WOE_CROW_DAMAGE:
		/*pvp capture the flag, Juver, 2018/02/06 */
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_CROW_HIDE:

		{
			GLMSG::SNETCROW *pNetMsg = (GLMSG::SNETCROW *) nmg;
			PGLCROWCLIENT pGLCrow = m_pLandMClient->GetCrow ( pNetMsg->dwGlobID );
			if ( !pGLCrow )		return;

			pGLCrow->MsgProcess ( nmg );
		}
		break;

	case NET_MSG_GCTRL_ETNRY_FAILED:
		{
			CInnerInterface::GetInstance().GetRequireCheck()->SetMapCheckType( EMMAPCHECK_ENTRYFAIL );
			CInnerInterface::GetInstance().ShowGroupFocus ( MAP_REQUIRE_CHECK );			
		}
		break;

	case NET_MSG_GCTRL_LIMITTIME_OVER:
		{
			GLMSG::SNETLIMITTIME_OVER *pNetMsg = (GLMSG::SNETLIMITTIME_OVER *) nmg;
			if( pNetMsg->nRemainTime != 0 )
			{
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("LIMITTIME_REMAINTIME"), pNetMsg->nRemainTime );
			}else{
				CInnerInterface::GetInstance().GetRequireCheck()->SetMapCheckType( EMMAPCHECK_LIMITTIME );
				CInnerInterface::GetInstance().ShowGroupFocus ( MAP_REQUIRE_CHECK );
			}
		}
		break;

	case NET_MSG_CYBERCAFECLASS_UPDATE:
		{
			GLMSG::SNET_CYBERCAFECLASS_UPDATE *pNetMsg = (GLMSG::SNET_CYBERCAFECLASS_UPDATE *)nmg;
			GetCharacter()->m_dwThaiCCafeClass = pNetMsg->dwCyberCafeClass;
			GetCharacter()->m_nMyCCafeClass	   = pNetMsg->dwCyberCafeClass;
			CInnerInterface::GetInstance().SetThaiCCafeClass( pNetMsg->dwCyberCafeClass );
		}
		break;

	case NET_MSG_GM_EVENT_EXP_FB:
		{
			GLMSG::SNET_GM_EVENT_EXP_FB *pNetMsg = (GLMSG::SNET_GM_EVENT_EXP_FB *) nmg;
			if ( pNetMsg->bFAIL )
				CInnerInterface::GetInstance().PrintConsoleText ( "Experience Event Start Failed! Set Proper Rate!" );
			else
				CInnerInterface::GetInstance().PrintConsoleText ( "Experience Event Start New Rate : %3.2f", pNetMsg->fRATE );
		}break;

	case NET_MSG_GM_EVENT_EXP_END_FB:
		{
			GLMSG::SNET_GM_EVENT_EXP_END_FB *pNetMsg = (GLMSG::SNET_GM_EVENT_EXP_END_FB *) nmg;
			CInnerInterface::GetInstance().PrintConsoleText ( "Experience Event End! Restore to original Rate : %3.2f", pNetMsg->fRATE );
		}break;

	case NET_MSG_GM_EVENT_ITEM_GEN_FB:
		{
			GLMSG::SNET_GM_EVENT_ITEM_GEN_FB *pNetMsg = (GLMSG::SNET_GM_EVENT_ITEM_GEN_FB *) nmg;
			if ( pNetMsg->bFAIL )
				CInnerInterface::GetInstance().PrintConsoleText ( "Item Drop Rate Event Start Failed! Set Proper Rate!" );
			else
				CInnerInterface::GetInstance().PrintConsoleText ( "Item Drop Rate Event Start New Rate : %3.2f", pNetMsg->fRATE );
		}break;

	case NET_MSG_GM_EVENT_ITEM_GEN_END_FB:
		{
			GLMSG::SNET_GM_EVENT_ITEM_GEN_END_FB *pNetMsg = (GLMSG::SNET_GM_EVENT_ITEM_GEN_END_FB *) nmg;
			CInnerInterface::GetInstance().PrintConsoleText ( "Item Drop Event End! Restore to original Rate : %3.2f", pNetMsg->fRATE );
		}break;

	case NET_MSG_GM_EVENT_MONEY_GEN_FB:
		{
			GLMSG::SNET_GM_EVENT_MONEY_GEN_FB *pNetMsg = (GLMSG::SNET_GM_EVENT_MONEY_GEN_FB *) nmg;
			if ( pNetMsg->bFAIL )
				CInnerInterface::GetInstance().PrintConsoleText ( "Money Drop Rate Event Start Failed! Set Proper Rate!" );
			else
				CInnerInterface::GetInstance().PrintConsoleText ( "Money Drop Rate Event Start New Rate : %3.2f", pNetMsg->fRATE );
		}break;

	case NET_MSG_GM_EVENT_MONEY_GEN_END_FB:
		{
			GLMSG::SNET_GM_EVENT_MONEY_GEN_END_FB *pNetMsg = (GLMSG::SNET_GM_EVENT_MONEY_GEN_END_FB *) nmg;
			CInnerInterface::GetInstance().PrintConsoleText ( "Money Drop Event End! Restore to original Rate : %3.2f", pNetMsg->fRATE );
		}break;

	case NET_MSG_GM_BIGHEAD_BRD:
		{
			GLMSG::SNET_GM_BIGHEAD *pNetMsg = (GLMSG::SNET_GM_BIGHEAD *) nmg;
			DxSkinAniControl::m_bBIGHEAD = pNetMsg->bBIGHEAD;
		}
		break;

	case NET_MSG_GM_BIGHAND_BRD:
		{
			GLMSG::SNET_GM_BIGHAND *pNetMsg = (GLMSG::SNET_GM_BIGHAND *) nmg;
			DxSkinAniControl::m_bBIGHAND = pNetMsg->bBIGHAND;
		}
		break;

	case NET_MSG_GM_FREEPK_BRD:
		{
			// 클라이언트가 뭔가 할일이 있다면 여기에 코딩
			GLMSG::SNET_GM_FREEPK_BRD *pNetMsg = (GLMSG::SNET_GM_FREEPK_BRD *) nmg;
			if ( pNetMsg->bSTATEUPDATE == false )
			{
				if ( pNetMsg->dwPKTIME != 0 )
				{
					m_bBRIGHTEVENT = true;
					/*dmk14 freepk*/
					m_sBrightEventMap = pNetMsg->sMapID;

					m_bFreePKMatch = true;
					m_fFreePKMatchTimer = (float)pNetMsg->dwPKTIME;

					CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("BRIGHT_EVENT_START") );
				}
				else
				{
					m_bBRIGHTEVENT = false;
					/*dmk14 freepk*/
					m_sBrightEventMap = NATIVEID_NULL();
					CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("BRIGHT_EVENT_END") );

					// 적대관계 모두 삭제
					GetCharacter()->DEL_PLAYHOSTILE_ALL();
				}
			}
		}
		break;

		/*private market set, Juver, 2018/01/02 */
	case NET_MSG_GM_SET_PRIVATE_MARKET_BRD:
		{
			GLMSG::SNET_GM_SET_PRIVATE_MARKET_BRD *pNetMsg = (GLMSG::SNET_GM_SET_PRIVATE_MARKET_BRD *) nmg;
			if ( pNetMsg->bEnable )
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("PRIVATE_MARKET_SET_ON") );
			else
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("PRIVATE_MARKET_SET_OFF") );
		}break;

		/*megaphone set, Juver, 2018/01/02 */
	case NET_MSG_GM_SET_MEGAPHONE_BRD:
		{
			GLMSG::SNET_GM_SET_MEGAPHONE_BRD *pNetMsg = (GLMSG::SNET_GM_SET_MEGAPHONE_BRD *) nmg;
			if ( pNetMsg->bEnable )
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("MEGAPHONE_SET_ON") );
			else
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("MEGAPHONE_SET_OFF") );
		}break;

	case NET_MSG_GM_SHOP_INFO_FB:
		{
			GLMSG::SNET_GM_SHOP_INFO_FB *pNetMsg = (GLMSG::SNET_GM_SHOP_INFO_FB *)nmg;

			if( pNetMsg->bSTATE == GLMSG::SNET_GM_SHOP_INFO_FB::END )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Shop Info Search End! [%s][%s]", m_szShopInfoFile, pNetMsg->szBasicInfo );
				break;
			}

			if( pNetMsg->bSTATE == GLMSG::SNET_GM_SHOP_INFO_FB::FIRST )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "Shop Info Search Start!" );

				memset(m_szShopInfoFile, 0, sizeof(char) * (MAX_PATH));
				StringCchCopy( m_szShopInfoFile, MAX_PATH, SUBPATH::APP_ROOT );
				StringCchCat( m_szShopInfoFile, MAX_PATH, SUBPATH::SAVE_ROOT );
				CreateDirectory( m_szShopInfoFile, NULL );

				CTime curTime = GLGaeaClient::GetInstance().GetCurrentTime();

				CHAR  szFileName[MAX_PATH] = {0};
				sprintf( szFileName, "ShopInfo_[%d%d%d%d%d%d].csv", curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), 
																  curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond() );

				StringCchCat ( m_szShopInfoFile, MAX_PATH, szFileName );
			}

			FILE*	fp = NULL;
			fp = fopen( m_szShopInfoFile, "a+" );
			if( fp )
			{
				if( pNetMsg->bSTATE == GLMSG::SNET_GM_SHOP_INFO_FB::FIRST )
				{
					if( pNetMsg->bBasicInfo )
					{
						char szHeader[] = "UserNum,CharNum,ItemMid,ItemSid,Price\n";
						fprintf( fp, szHeader );
					}else{
						char szHeader[] = "UserNum,CharNum,ItemMid,ItemSid,Price,RanOptType1,RandValue1,RanOptType2,RandValue2,RanOptType3,RandValue3,RanOptType4,RandValue4,DAMAGE,DEFENSE,RESIST_FIRE,RESIST_ICE,RESIST_ELEC,RESIST_POISON,RESIST_SPIRIT\n";
						fprintf( fp, szHeader );																			  
					}																										  
				}																											  
				fprintf( fp, pNetMsg->szBasicInfo );																		  
				fclose( fp );
			}		

		}
		break;

		// 대만에서 요청한 새로운 GM 명령어 ( /dsp player 의 확장판 )
	case NET_MSG_GM_VIEWALLPLAYER_FLD_FB:
		{
			GLMSG::SNET_GM_VIEWALLPLAYER_FLD_FB *pNetMsg = (GLMSG::SNET_GM_VIEWALLPLAYER_FLD_FB *)nmg;
			
			if( pNetMsg->dwPlayerNum != 0 )
				CInnerInterface::GetInstance().PrintConsoleText ( "Player Count : %d", pNetMsg->dwPlayerNum + 1 );
			else
				CInnerInterface::GetInstance().PrintConsoleText ( "%s	: charid = %d", pNetMsg->szCHARNAME, pNetMsg->dwCHARID );
		}
		break;


	case NET_MSG_GM_VIEWWORKEVENT_FB:
		{
			GLMSG::SNET_GM_VIEWWORKEVENT_FB *pNetMsg = (GLMSG::SNET_GM_VIEWWORKEVENT_FB *)nmg;
			CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::NEGATIVE, "%s", pNetMsg->szWorkEventInfo );
		}
		break;

	case NET_MSG_GCTRL_CLUB_MARK_INFO_FB:
		{
			GLMSG::SNET_CLUB_MARK_INFO_FB *pNetMsg = (GLMSG::SNET_CLUB_MARK_INFO_FB *)nmg;
			
			//	Note : 이미지에 적용.
			DWORD dwServerID = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
			DxClubMan::GetInstance().SetClubData ( DxGlobalStage::GetInstance().GetD3dDevice(), dwServerID, pNetMsg->dwClubID, pNetMsg->dwMarkVER, pNetMsg->aryMark );
		}
		break;

	case NET_MSG_GCTRL_CLUB_BATTLE_START_BRD:
		{
			GLMSG::SNET_CLUB_BATTLE_START_BRD *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_START_BRD *)nmg;
			
			if ( pNetMsg->nTIME==0 )
			{
				CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_BATTLE_START"), pNetMsg->szName );

				CLUB_BATTLE_TIMER_MAP_ITER it = m_mapCLUBBATTLETimer.find( pNetMsg->dwID );
				if ( it != m_mapCLUBBATTLETimer.end() )
				{
					SCLUBWAR_BATTLE_TIMER& sClubBattleTimer = (*it).second;
					sClubBattleTimer.bBattle = TRUE;
				}

//				CMiniMap * pMiniMap = CInnerInterface::GetInstance().GetMiniMap();
//				if( pMiniMap ) pMiniMap->StartClubTime();
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_BATTLE_MIN_START"), pNetMsg->szName, pNetMsg->nTIME );
			}

			PVPClubWarsRankClear();
		}
		break;

	case NET_MSG_GCTRL_CLUB_DEATHMATCH_START_BRD:
		{
			GLMSG::SNET_CLUB_DEATHMATCH_START_BRD *pNetMsg = (GLMSG::SNET_CLUB_DEATHMATCH_START_BRD *)nmg;
			
			if ( pNetMsg->nTIME==0 )
			{
				CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_DEATHMATCH_START"), pNetMsg->szName );
				m_bClubDeathMatch = true;

//				CMiniMap * pMiniMap = CInnerInterface::GetInstance().GetMiniMap();
//				if( pMiniMap ) pMiniMap->StartClubTime();
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_DEATHMATCH_MIN_START"), pNetMsg->szName, pNetMsg->nTIME );
			}
		}
		break;

	case NET_MSG_GCTRL_CLUB_DEATHMATCH_END_BRD:
		{
			GLMSG::SNET_CLUB_DEATHMATCH_END_BRD *pNetMsg = (GLMSG::SNET_CLUB_DEATHMATCH_END_BRD *)nmg;
			CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_DEATHMATCH_END"), pNetMsg->szName, pNetMsg->szClubName );

			m_bClubDeathMatch = false;
			m_fClubDeathMatchTimer = 0.0f;

//			CMiniMap * pMiniMap = CInnerInterface::GetInstance().GetMiniMap();
//			if( pMiniMap ) pMiniMap->EndClubTime();

			// 적대관계 모두 삭제
			GetCharacter()->DEL_PLAYHOSTILE_ALL();
		}
		break;

	case NET_MSG_GCTRL_CLUB_DEATHMATCH_REMAIN_BRD:
		{
			GLMSG::SNET_CLUB_DEATHMATCH_REMAIN_BRD *pNetMsg = (GLMSG::SNET_CLUB_DEATHMATCH_REMAIN_BRD*)nmg;
			m_fClubDeathMatchTimer = (float)pNetMsg->dwTime;
		}
		break;

	case NET_MSG_GCTRL_CLUB_DEATHMATCH_POINT_UPDATE:
		{
			GLMSG::SNET_CLUB_DEATHMATCH_POINT_UPDATE *pNetMsg = (GLMSG::SNET_CLUB_DEATHMATCH_POINT_UPDATE*)nmg;	
			if ( pNetMsg->bKillPoint )	m_sMyCdmRank.wKillNum++;
			else						m_sMyCdmRank.wDeathNum++;

			//	UI Refrash
			CInnerInterface::GetInstance().VisibleCDMRanking(true);
			CInnerInterface::GetInstance().RefreshCDMRanking();
		}
		break;
	case NET_MSG_GCTRL_CLUB_DEATHMATCH_MYRANK_UPDATE:
		{

			GLMSG::SNET_CLUB_DEATHMATCH_MYRANK_UPDATE *pNetMsg = (GLMSG::SNET_CLUB_DEATHMATCH_MYRANK_UPDATE*)nmg;	

			m_sMyCdmRank = pNetMsg->sMyCdmRank;

			//	UI Refrash 및 열기
			CInnerInterface::GetInstance().VisibleCDMRanking(true);
			CInnerInterface::GetInstance().RefreshCDMRanking();
		}
		break;

	case NET_MSG_GCTRL_CLUB_DEATHMATCH_RANKING_UPDATE:
		{
			GLMSG::SNET_CLUB_DEATHMATCH_RANKING_UPDATE *pNetMsg = (GLMSG::SNET_CLUB_DEATHMATCH_RANKING_UPDATE*)nmg;	

			int nRankNum = pNetMsg->wRankNum;
			
			for ( int i = 0; i < nRankNum; ++i )
			{
				int nIndex = pNetMsg->sCdmRank[i].nIndex;
				if ( nIndex < 0 ) continue;
				
				int nSize = m_vecCdmRank.size();				
				if ( nIndex < nSize )
				{
					m_vecCdmRank[nIndex] = pNetMsg->sCdmRank[i];
				}
				else
				{
					m_vecCdmRank.resize( nIndex+1 );
					m_vecCdmRank[nIndex] = pNetMsg->sCdmRank[i];
				}
			}

			//	UI Refrash 및 열기
			CInnerInterface::GetInstance().VisibleCDMRanking(true);
			CInnerInterface::GetInstance().RefreshCDMRanking();

		}
		break;



	case NET_MSG_GCTRL_WOE_PLAYER_MYRANK_UPDATE:
	{

		GLMSG::SNET_WOE_PLAYER_MYRANK_UPDATE* pNetMsg = (GLMSG::SNET_WOE_PLAYER_MYRANK_UPDATE*)nmg;

		m_sSelfWoePlayerRank = pNetMsg->sMySelfPlayerRank;

		CInnerInterface::GetInstance().RefreshWoeRanking();
	}
	break;
	case NET_MSG_GCTRL_WOE_PLAYER_RANKING_UPDATE:
	{
		GLMSG::SNET_WOE_PLAYER_RANKING_UPDATE* pNetMsg = (GLMSG::SNET_WOE_PLAYER_RANKING_UPDATE*)nmg;

		int nRankNum = pNetMsg->wRankNum;

		for (int i = 0; i < nRankNum; ++i)
		{
			int nIndex = pNetMsg->sWoePlayerRank[i].nIndex;
			if (nIndex < 0) continue;

			int nSize = m_vecWoePlayerRank.size();
			if (nIndex < nSize)
			{
				m_vecWoePlayerRank[nIndex] = pNetMsg->sWoePlayerRank[i];
			}
			else
			{
				m_vecWoePlayerRank.resize(nIndex + 1);
				m_vecWoePlayerRank[nIndex] = pNetMsg->sWoePlayerRank[i];
			}
		}
		CInnerInterface::GetInstance().RefreshWoeRanking();
	}
	break;
	case NET_MSG_GCTRL_WOE_GUILD_POINT_UPDATE:
	{
		GLMSG::SNET_WOE_GUILD_POINT_UPDATE* pNetMsg = (GLMSG::SNET_WOE_GUILD_POINT_UPDATE*)nmg;
		if (pNetMsg->bKillPoint)	m_sMyWoeRank.wKillNum++;
		else						m_sMyWoeRank.wDeathNum++;

		CInnerInterface::GetInstance().RefreshWoeRanking();
	}
	break;
	case NET_MSG_GCTRL_WOE_GUILD_MYRANK_UPDATE:
	{

		GLMSG::SNET_WOE_GUILD_MYRANK_UPDATE* pNetMsg = (GLMSG::SNET_WOE_GUILD_MYRANK_UPDATE*)nmg;
		m_sMyWoeRank = pNetMsg->sMyWoeGuildRank;
		CInnerInterface::GetInstance().RefreshWoeRanking();
	}
	break;
	case NET_MSG_GCTRL_WOE_GUILD_RANKING_UPDATE:
	{
		GLMSG::SNET_WOE_GUILD_RANKING_UPDATE* pNetMsg = (GLMSG::SNET_WOE_GUILD_RANKING_UPDATE*)nmg;

		int nRankNum = pNetMsg->wRankNum;

		for (int i = 0; i < nRankNum; ++i)
		{
			int nIndex = pNetMsg->sWoeGuildRank[i].nIndex;
			if (nIndex < 0) continue;

			int nSize = m_vecWoeGuild.size();
			if (nIndex < nSize)
			{
				m_vecWoeGuild[nIndex] = pNetMsg->sWoeGuildRank[i];
			}
			else
			{
				m_vecWoeGuild.resize(nIndex + 1);
				m_vecWoeGuild[nIndex] = pNetMsg->sWoeGuildRank[i];
			}
		}

		CInnerInterface::GetInstance().RefreshWoeRanking();

	}
	break;
	case NET_MSG_GCTRL_WOE_RESU_MYRANK_UPDATE:
	{
		GLMSG::SNET_WOE_RESU_MYRANK_UPDATE* pNetMsg = (GLMSG::SNET_WOE_RESU_MYRANK_UPDATE*)nmg;

		m_sSelfWoeResuRank = pNetMsg->sMySelfResuRank;

		CInnerInterface::GetInstance().RefreshWoeRanking();
	}
	break;
	case NET_MSG_GCTRL_WOE_RESU_RANKING_UPDATE:
	{
		GLMSG::SNET_WOE_RESU_RANKING_UPDATE* pNetMsg = (GLMSG::SNET_WOE_RESU_RANKING_UPDATE*)nmg;

		int nRankNum = pNetMsg->wRankNum;

		for (int i = 0; i < nRankNum; ++i)
		{
			int nIndex = pNetMsg->sWoeResu[i].nIndex;
			if (nIndex < 0) continue;

			int nSize = m_vecWoeResuRank.size();
			if (nIndex < nSize)
			{
				m_vecWoeResuRank[nIndex] = pNetMsg->sWoeResu[i];
			}
			else
			{
				m_vecWoeResuRank.resize(nIndex + 1);
				m_vecWoeResuRank[nIndex] = pNetMsg->sWoeResu[i];
			}
		}
		CInnerInterface::GetInstance().RefreshWoeRanking();
	}
	break;
	
	//-------------------------------------------------------------------------------------------------[ SKY ]
	//-------------------------------------------------------------------------------------------------[ NET ]

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_TYRANNY_CLUB_RANK_POINT_UPDATE:
		{
			GLMSG::SNET_TYRANNY_CLUB_RANK_POINT_UPDATE *pNetMsg = (GLMSG::SNET_TYRANNY_CLUB_RANK_POINT_UPDATE*)nmg;	
			if ( pNetMsg->bKillPoint )	m_sMyTyrannyClubRank.wKillNum++;
			else if ( pNetMsg->bResuPoint ) m_sMyTyrannyClubRank.wResuNum++;
			else						m_sMyTyrannyClubRank.wDeathNum++;

			//CInnerInterface::GetInstance().PVPTyrannyRankingDisplayShow(true);
			CInnerInterface::GetInstance().PVPTyrannyRankingDisplayRefresh();
		}
		break;
		
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_TYRANNY_CLUB_MYRANK_UPDATE:
		{
			GLMSG::SNET_TYRANNY_CLUB_MYRANK_UPDATE *pNetMsg = (GLMSG::SNET_TYRANNY_CLUB_MYRANK_UPDATE*)nmg;	

			m_sMyTyrannyClubRank = pNetMsg->sMyTyrannyClubRank;

			//CInnerInterface::GetInstance().PVPTyrannyRankingDisplayShow(true);
			CInnerInterface::GetInstance().PVPTyrannyRankingDisplayRefresh();
		}
		break;
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_TYRANNY_CLUB_RANKING_UPDATE:
		{
			GLMSG::SNET_TYRANNY_CLUB_RANKING_UPDATE *pNetMsg = (GLMSG::SNET_TYRANNY_CLUB_RANKING_UPDATE*)nmg;	

			int nRankNum = pNetMsg->wRankNum;

			for ( int i = 0; i < nRankNum; ++i )
			{
				int nIndex = pNetMsg->sTyrannyClubRank[i].nIndex;
				if ( nIndex < 0 ) continue;

				int nSize = m_vecTyrannyClubRank.size();				
				if ( nIndex < nSize )
				{
					m_vecTyrannyClubRank[nIndex] = pNetMsg->sTyrannyClubRank[i];
				}
				else
				{
					m_vecTyrannyClubRank.resize( nIndex+1 );
					m_vecTyrannyClubRank[nIndex] = pNetMsg->sTyrannyClubRank[i];
				}
			}

			//CInnerInterface::GetInstance().PVPTyrannyRankingDisplayShow(true);
			CInnerInterface::GetInstance().PVPTyrannyRankingDisplayRefresh();

		}
		break;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_TYRANNY_MYRANK_UPDATE:
		{
			GLMSG::SNET_TYRANNY_MYRANK_UPDATE *pNetMsg = (GLMSG::SNET_TYRANNY_MYRANK_UPDATE*)nmg;	

			m_sMyTyrannyRank = pNetMsg->sMyTyrannyRank;

			//CInnerInterface::GetInstance().PVPTyrannyRankingDisplayShow(true);
			CInnerInterface::GetInstance().PVPTyrannyRankingDisplayRefresh();
		}
		break;
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_TYRANNY_RANKING_UPDATE:
		{
			GLMSG::SNET_TYRANNY_RANKING_UPDATE *pNetMsg = (GLMSG::SNET_TYRANNY_RANKING_UPDATE*)nmg;	

			int nRankNum = pNetMsg->wRankNum;

			for ( int i = 0; i < nRankNum; ++i )
			{
				int nIndex = pNetMsg->sTyrannyRank[i].nIndex;
				if ( nIndex < 0 ) continue;

				int nSize = m_vecTyrannyRank.size();				
				if ( nIndex < nSize )
				{
					m_vecTyrannyRank[nIndex] = pNetMsg->sTyrannyRank[i];
				}
				else
				{
					m_vecTyrannyRank.resize( nIndex+1 );
					m_vecTyrannyRank[nIndex] = pNetMsg->sTyrannyRank[i];
				}
			}

			//CInnerInterface::GetInstance().PVPTyrannyRankingDisplayShow(true);
			CInnerInterface::GetInstance().PVPTyrannyRankingDisplayRefresh();

		}
		break;
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_TYRANNY_MYRANK_RESU_UPDATE:
		{
			GLMSG::SNET_TYRANNY_MYRANK_RESU_UPDATE *pNetMsg = (GLMSG::SNET_TYRANNY_MYRANK_RESU_UPDATE*)nmg;	

			m_sMyTyrannyRankResu = pNetMsg->sMyTyrannyRankResu;

			//CInnerInterface::GetInstance().PVPTyrannyRankingDisplayShow(true);
			CInnerInterface::GetInstance().PVPTyrannyRankingDisplayRefresh();
		}
		break;
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_TYRANNY_RANKING_RESU_UPDATE:
		{
			GLMSG::SNET_TYRANNY_RANKING_RESU_UPDATE *pNetMsg = (GLMSG::SNET_TYRANNY_RANKING_RESU_UPDATE*)nmg;	

			int nRankNum = pNetMsg->wRankNum;

			for ( int i = 0; i < nRankNum; ++i )
			{
				int nIndex = pNetMsg->sTyrannyRankResu[i].nIndex;
				if ( nIndex < 0 ) continue;

				int nSize = m_vecTyrannyRankResu.size();				
				if ( nIndex < nSize )
				{
					m_vecTyrannyRankResu[nIndex] = pNetMsg->sTyrannyRankResu[i];
				}
				else
				{
					m_vecTyrannyRankResu.resize( nIndex+1 );
					m_vecTyrannyRankResu[nIndex] = pNetMsg->sTyrannyRankResu[i];
				}
			}

			//CInnerInterface::GetInstance().PVPTyrannyRankingDisplayShow(true);
			CInnerInterface::GetInstance().PVPTyrannyRankingDisplayRefresh();

		}
		break;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_CLUBWAR_CLUB_RANK_POINT_UPDATE:
		{
			GLMSG::SNET_CLUBWAR_CLUB_RANK_POINT_UPDATE *pNetMsg = (GLMSG::SNET_CLUBWAR_CLUB_RANK_POINT_UPDATE*)nmg;	
			if ( pNetMsg->bKillPoint )	m_sMyClubWarClubRank.wKillNum++;
			else if ( pNetMsg->bResuPoint ) m_sMyClubWarClubRank.wResuNum++;
			else							m_sMyClubWarClubRank.wDeathNum++;
			CInnerInterface::GetInstance().ClubWarRankingDisplayRefresh();
		}
		break;
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_CLUBWAR_CLUB_MYRANK_UPDATE:
		{
			GLMSG::SNET_CLUBWAR_CLUB_MYRANK_UPDATE *pNetMsg = (GLMSG::SNET_CLUBWAR_CLUB_MYRANK_UPDATE*)nmg;	
			m_sMyClubWarClubRank = pNetMsg->sMyClubWarClubRank;
			CInnerInterface::GetInstance().ClubWarRankingDisplayRefresh();
		}
		break;
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_CLUBWAR_CLUB_RANKING_UPDATE:
		{
			GLMSG::SNET_CLUBWAR_CLUB_RANKING_UPDATE *pNetMsg = (GLMSG::SNET_CLUBWAR_CLUB_RANKING_UPDATE*)nmg;	
			int nRankNum = pNetMsg->wRankNum;
			for ( int i = 0; i < nRankNum; ++i )
			{
				int nIndex = pNetMsg->sClubWarClubRank[i].nIndex;

				if ( nIndex < 0 ) continue;

				int nSize = m_vecClubWarClubRank.size();				
				if ( nIndex < nSize )
				{
					m_vecClubWarClubRank[nIndex] = pNetMsg->sClubWarClubRank[i];
				}
				else
				{
					m_vecClubWarClubRank.resize( nIndex+1 );
					m_vecClubWarClubRank[nIndex] = pNetMsg->sClubWarClubRank[i];
				}
			}
			CInnerInterface::GetInstance().ClubWarRankingDisplayRefresh();

		}
		break;

	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_CLUBWAR_MYRANK_UPDATE:
		{
			GLMSG::SNET_CLUBWAR_MYRANK_UPDATE *pNetMsg = (GLMSG::SNET_CLUBWAR_MYRANK_UPDATE*)nmg;	
			m_sMyClubWarRank = pNetMsg->sMyClubWarRank;
			CInnerInterface::GetInstance().ClubWarRankingDisplayRefresh();
		}
		break;
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_CLUBWAR_RANKING_UPDATE:
		{
			GLMSG::SNET_CLUBWAR_RANKING_UPDATE *pNetMsg = (GLMSG::SNET_CLUBWAR_RANKING_UPDATE*)nmg;	

			int nRankNum = pNetMsg->wRankNum;

			for ( int i = 0; i < nRankNum; ++i )
			{
				int nIndex = pNetMsg->sClubWarRank[i].nIndex;
				if ( nIndex < 0 ) continue;

				int nSize = m_vecClubWarRank.size();				
				if ( nIndex < nSize )
				{
					m_vecClubWarRank[nIndex] = pNetMsg->sClubWarRank[i];
				}
				else
				{
					m_vecClubWarRank.resize( nIndex+1 );
					m_vecClubWarRank[nIndex] = pNetMsg->sClubWarRank[i];
				}
			}
			CInnerInterface::GetInstance().ClubWarRankingDisplayRefresh();
		}
		break;

	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_CLUBWAR_MYRANK_RESU_UPDATE:
		{
			GLMSG::SNET_CLUBWAR_MYRANK_RESU_UPDATE *pNetMsg = (GLMSG::SNET_CLUBWAR_MYRANK_RESU_UPDATE*)nmg;	
			m_sMyClubWarRankResu = pNetMsg->sMyClubWarRankResu;
			CInnerInterface::GetInstance().ClubWarRankingDisplayRefresh();
		}
		break;
	
	/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
	case NET_MSG_GCTRL_CLUBWAR_RANKING_RESU_UPDATE:
		{
			GLMSG::SNET_CLUBWAR_RANKING_RESU_UPDATE *pNetMsg = (GLMSG::SNET_CLUBWAR_RANKING_RESU_UPDATE*)nmg;	

			int nRankNum = pNetMsg->wRankNum;

			for ( int i = 0; i < nRankNum; ++i )
			{
				int nIndex = pNetMsg->sClubWarRankResu[i].nIndex;
				if ( nIndex < 0 ) continue;

				int nSize = m_vecClubWarRankResu.size();				
				if ( nIndex < nSize )
				{
					m_vecClubWarRankResu[nIndex] = pNetMsg->sClubWarRankResu[i];
				}
				else
				{
					m_vecClubWarRankResu.resize( nIndex+1 );
					m_vecClubWarRankResu[nIndex] = pNetMsg->sClubWarRankResu[i];
				}
			}
			CInnerInterface::GetInstance().ClubWarRankingDisplayRefresh();
		}
		break;
	case NET_MSG_GCTRL_CLUB_CD_CERTIFY_BRD:
		{
			GLMSG::SNET_CLUB_CD_CERTIFY_BRD *pNetMsg = (GLMSG::SNET_CLUB_CD_CERTIFY_BRD *)nmg;
			if ( strlen ( pNetMsg->szAlliance ) != 0 )
			{
				CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_BATTLE_CERTIFY_START_EX"), pNetMsg->szZone, pNetMsg->szAlliance, pNetMsg->szClub, pNetMsg->szName );
			}
			else
			{
				CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_BATTLE_CERTIFY_START"), pNetMsg->szZone, pNetMsg->szClub, pNetMsg->szName );
			}
		}
		break;

	case NET_MSG_GCTRL_CLUB_CD_CERTIFY_ING_BRD:
		{
			GLMSG::SNET_CLUB_CD_CERTIFY_ING_BRD *pNetMsg = ( GLMSG::SNET_CLUB_CD_CERTIFY_ING_BRD* ) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMCDCERTIFY_ING_DIE:
				{
					if ( strlen ( pNetMsg->szAlliance ) != 0 )
					{
						CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("EMCDCERTIFY_ING_DIE_EX_ALLI"), pNetMsg->szZone, pNetMsg->szAlliance, pNetMsg->szClub, pNetMsg->szName );
					}
					else
					{
						CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("EMCDCERTIFY_ING_DIE_EX"), pNetMsg->szZone, pNetMsg->szClub, pNetMsg->szName );
					}
				}
				break;
			case EMCDCERTIFY_ING_TIMEOUT:
				{
					if ( strlen ( pNetMsg->szAlliance ) != 0 )
					{
						CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("EMCDCERTIFY_ING_TIMEOUT_EX_ALLI"), pNetMsg->szZone, pNetMsg->szAlliance, pNetMsg->szClub, pNetMsg->szName );
					}
					else
					{
						CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("EMCDCERTIFY_ING_TIMEOUT_EX"), pNetMsg->szZone, pNetMsg->szClub, pNetMsg->szName );
					}
				}
				break;
			case EMCDCERTIFY_ING_DISTANCE:
				{
					if ( strlen ( pNetMsg->szAlliance ) != 0 )
					{
						CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("EMCDCERTIFY_ING_DISTANCE_EX_ALLI"), pNetMsg->szZone, pNetMsg->szAlliance, pNetMsg->szClub, pNetMsg->szName );
					}
					else
					{
						CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("EMCDCERTIFY_ING_DISTANCE_EX"), pNetMsg->szZone, pNetMsg->szClub, pNetMsg->szName );
					}
				}
				break;
			}
		}
		break;

	case NET_MSG_GCTRL_CLUB_BATTLE_END_BRD:
		{
			GLMSG::SNET_CLUB_BATTLE_END_BRD *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_END_BRD *)nmg;
			CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_BATTLE_END"), pNetMsg->szName, pNetMsg->szClubName );

			CLUB_BATTLE_TIMER_MAP_ITER it = m_mapCLUBBATTLETimer.find( pNetMsg->dwID );
			if ( it != m_mapCLUBBATTLETimer.end() )
			{
				SCLUBWAR_BATTLE_TIMER& sClubBattleTimer = (*it).second;
				sClubBattleTimer.bBattle = FALSE;
			}
		
//			CMiniMap * pMiniMap = CInnerInterface::GetInstance().GetMiniMap();
//			if( pMiniMap ) pMiniMap->EndClubTime();

			// 적대관계 모두 삭제
			GetCharacter()->DEL_PLAYHOSTILE_ALL();
		}
		break;

	case NET_MSG_GCTRL_CLUB_BATTLE_REMAIN_BRD:
		{
			GLMSG::SNET_CLUB_BATTLE_REMAIN_BRD *pNetMsg = (GLMSG::SNET_CLUB_BATTLE_REMAIN_BRD*)nmg;
			
			CLUB_BATTLE_TIMER_MAP_ITER it = m_mapCLUBBATTLETimer.find( pNetMsg->dwId );
			if ( it == m_mapCLUBBATTLETimer.end() )
			{
				SCLUBWAR_BATTLE_TIMER sTimer;
				sTimer.dwID = pNetMsg->dwId;
				StringCchCopy ( sTimer.szName, SCLUBWAR_BATTLE_TIMER::TEXT_LEN, pNetMsg->szName );

				sTimer.fTime = static_cast<float>(pNetMsg->dwTime);
				sTimer.bBattle = pNetMsg->bBattle;

				m_mapCLUBBATTLETimer.insert( std::make_pair( sTimer.dwID, sTimer ) );
			}
			else
			{
				SCLUBWAR_BATTLE_TIMER& sTimer = (*it).second;
				StringCchCopy ( sTimer.szName, SCLUBWAR_BATTLE_TIMER::TEXT_LEN, pNetMsg->szName );

				sTimer.fTime = static_cast<float>(pNetMsg->dwTime);
				sTimer.bBattle = pNetMsg->bBattle;
			}
		}
		break;

	case NET_MSG_GCTRL_LAND_INFO:
		{
			GLMSG::SNETPC_LAND_INFO *pNetMsg = (GLMSG::SNETPC_LAND_INFO *)nmg;

			/*dmk14 pk safetime*/
			//GLCharacter* pCharacter = GetCharacter();
			//if (pCharacter) pCharacter->ResetSafeTime();

			PLANDMANCLIENT pLand = GetActiveMap();
			if ( pLand && pLand->GetMapID()==pNetMsg->nidMAP )
			{
				pLand->m_bClubBattle = pNetMsg->bClubBattle;
				pLand->m_bClubBattleHall = pNetMsg->bClubBattleHall;
				pLand->m_bClubDeathMatch = pNetMsg->bClubDeathMatch;
				pLand->m_bClubDeathMatchHall = pNetMsg->bClubDeathMatchHall;
				pLand->SetPKZone ( pNetMsg->bPK );
				pLand->m_fCommission = pNetMsg->fCommission;

				pLand->m_dwGuidClub = pNetMsg->dwGuidClub;
				pLand->m_dwGuidClubMarkVer = pNetMsg->dwGuidClubMarkVer;
				StringCchCopy ( pLand->m_szGuidClubName, CHAR_SZNAME, pNetMsg->szGuidClubName );

				/*pvp tyranny, Juver, 2017/08/24 */
				pLand->m_bPVPTyrannyMap = pNetMsg->bPVPTyranny;

				/*woe Arc Development 08-06-2024*/
				pLand->m_bPVPWoeMap = pNetMsg->bPVPWoe;
				/*school wars, Juver, 2018/01/19 */
				pLand->m_bPVPSchoolWarsMap = pNetMsg->bPVPSchoolWars;

				/*pvp capture the flag, Juver, 2018/01/31 */
				pLand->m_bPVPCaptureTheFlagMap = pNetMsg->bPVPCaptureTheFlag;

				/* pvp club death match, Juver, 2020/11/23 */
				pLand->m_bPVPClubDeathMatchBattleMap = pNetMsg->bPVPClubDeathMatchBattleMap;
				pLand->m_bPVPClubDeathMatchLobbyMap = pNetMsg->bPVPClubDeathMatchLobbyMap;

				pLand->m_bPVPPBGBattleMap = pNetMsg->bPVPPBGBattleMap;
				pLand->m_bPVPPBGLobbyMap = pNetMsg->bPVPPBGLobbyMap;

				/*pvp tyranny, Juver, 2017/08/24 */
				if ( pLand->m_bPVPTyrannyMap )
					CInnerInterface::GetInstance().ShowGroupBottom( PVP_TYRANNY_TOWER_CAPTURE );
				else
					CInnerInterface::GetInstance().HideGroup( PVP_TYRANNY_TOWER_CAPTURE );

				/*woe Arc Development 08-06-2024*/
				if (pLand->m_bPVPWoeMap)
					CInnerInterface::GetInstance().ShowGroupBottom(PVP_WOE_TOWER_CAPTURE);
				else
					CInnerInterface::GetInstance().HideGroup(PVP_WOE_TOWER_CAPTURE);

				/*school wars, Juver, 2018/01/20 */
				if ( pLand->m_bPVPSchoolWarsMap )
					CInnerInterface::GetInstance().ShowGroupBottom( PVP_SCHOOLWARS_SCORE );
				else
					CInnerInterface::GetInstance().HideGroup( PVP_SCHOOLWARS_SCORE );

				/*pvp capture the flag, Juver, 2018/02/02 */
				if ( pLand->m_bPVPCaptureTheFlagMap )
					CInnerInterface::GetInstance().ShowGroupBottom( PVP_CAPTURE_THE_FLAG_SCORE );
				else
					CInnerInterface::GetInstance().HideGroup( PVP_CAPTURE_THE_FLAG_SCORE );
					
				// PVP Next Battle UI - Jxyyy
				if ( pLand->GetMapID().wMainID == 22 || pLand->GetMapID().wMainID == 2 || pLand->GetMapID().wMainID == 5 || pLand->GetMapID().wMainID == 8 )
					CInnerInterface::GetInstance().ShowGroupBottom( PVP_TYRANNY_NEXT_AND_WINNER );
				else
					CInnerInterface::GetInstance().HideGroup( PVP_TYRANNY_NEXT_AND_WINNER );
					
				/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
				if ( pNetMsg->nidMAP.wMainID == 214 || pNetMsg->nidMAP.wMainID == 204 )
				{
					if ( m_vecClubWarClubRank.empty() && m_vecClubWarRank.empty() &&
						m_vecClubWarRankResu.empty() )
					{
						ReqClubWarClubRankInfo();
						ReqClubWarRankInfo();
						ReqClubWarRankResuInfo();
						/* Enable/Disable Mini ranking Window - Montage, 9/28/24 */
						CInnerInterface::GetInstance().ClubWarRankingDisplayShow(false);
					}
				}
				
				/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
				else if ( pNetMsg->nidMAP.wMainID != 214 || pNetMsg->nidMAP.wMainID != 204 )
				{
					m_vecClubWarClubRank.clear();
					m_vecClubWarRank.clear();
					m_vecClubWarRankResu.clear();

					m_sMyClubWarClubRank.Init();
					m_sMyClubWarRank.Init();
					m_sMyClubWarRankResu.Init();

					CInnerInterface::GetInstance().ClubWarRankingDisplayShow(false);
				}

				if (m_bBRIGHTEVENT/*||pLand->m_bPVPTyrannyMap||pLand->m_bPVPSchoolWarsMap*/)
				{
					GLCharacter* pCharacter = GetCharacter();
					if (pCharacter) pCharacter->ReqBattleRanking();

					CInnerInterface::GetInstance().SetBattleRankingDisplayOpen(TRUE);
				}else CInnerInterface::GetInstance().SetBattleRankingDisplayOpen(FALSE);

				if ( pLand->m_bClubBattle )
					CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_BATTLE_LAND") );

				if ( pLand->m_bClubDeathMatch && m_vecCdmRank.empty() )
				{
					//	정보 요청
					ReqClubDeathMatchInfo();
					CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_DEATHMATCH_LAND") );
				}
				else if ( !pLand->m_bClubDeathMatch )
				{
					m_vecCdmRank.clear();
					m_sMyCdmRank.Init();
					//	UI 닫기
					CInnerInterface::GetInstance().VisibleCDMRanking( false );
				}
				/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
				if ( pLand->m_bPVPTyrannyMap )
				{
					if ( m_vecTyrannyClubRank.empty() && m_vecTyrannyRank.empty() &&
						m_vecTyrannyRankResu.empty() )
					{
						ReqTyrannyClubRankInfo();
						ReqTyrannyRankInfo();
						ReqTyrannyRankResuInfo();
						/* Enable/Disable Mini ranking Window - Montage, 9/28/24 */
						CInnerInterface::GetInstance().PVPTyrannyRankingDisplayShow(false);
					}
				}
				/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
				else if ( !pLand->m_bPVPTyrannyMap )
				{
					m_vecTyrannyClubRank.clear();
					m_vecTyrannyRank.clear();
					m_vecTyrannyRankResu.clear();

					m_sMyTyrannyClubRank.Init();
					m_sMyTyrannyRank.Init();
					m_sMyTyrannyRankResu.Init();

					CInnerInterface::GetInstance().PVPTyrannyRankingDisplayShow(false);
				}

				CString str;
				str.Format( ID2GAMEINTEXT("LAND_TRADE_COMMISSION"), pLand->m_fCommission );

				if ( m_bBRIGHTEVENT )
				{
					str += _T("\r\n");
					str += ID2GAMEINTEXT("BRIGHT_EVENT_MSG");
				}

				if ( m_bSCHOOL_FREEPK )
				{
					str += _T("\r\n");
					str += ID2GAMEINTEXT("OTHER_SCHOOL_FREEPK_ON");
				}

				// 적대관계 모두 삭제
				GetCharacter()->DEL_PLAYHOSTILE_ALL();

				/*pvp tyranny, Juver, 2017/08/24 */
				/*school wars, Juver, 2018/01/19 */
				/*pvp capture the flag, Juver, 2018/01/31 */
				/* pvp club death match, Juver, 2020/11/23 */
				/*woe Arc Development 08-06-2024*/
				if ( !pLand->m_bPVPTyrannyMap && 
					!pLand->m_bPVPSchoolWarsMap && 
					!pLand->m_bPVPCaptureTheFlagMap && 
					!pLand->m_bPVPClubDeathMatchBattleMap && 
					!pLand->m_bPVPClubDeathMatchLobbyMap &&
					!pLand->m_bPVPPBGLobbyMap &&
					!pLand->m_bPVPPBGBattleMap &&
					!pLand->m_bPVPWoeMap )
				{
					CInnerInterface::GetInstance().PrintConsoleTextDlg( str );
				}

				/*pvp tyranny, Juver, 2017/08/24 */
				if ( pLand->m_bPVPTyrannyMap )
				{
					GLPVPTyrannyClient::GetInstance().RequestTowerInfo();

					if ( CInnerInterface::GetInstance().PVPTyrannyToShowRanking() )
						CInnerInterface::GetInstance().PVPTyrannyShowRanking();


					if ( !GLPVPTyrannyClient::GetInstance().m_bShowStartNotice &&
						GLPVPTyrannyClient::GetInstance().IsBattle() )
					{
						CInnerInterface::GetInstance().PVPTyrannyShowStartNotice();
						GLPVPTyrannyClient::GetInstance().m_bShowStartNotice = TRUE;
					}
				}

				/*woe Arc Development 08-06-2024*/
				if (pLand->m_bPVPWoeMap)
				{
					GLPVPWoeClient::GetInstance().RequestEmperiumInfo();
				}
				/*school wars, Juver, 2018/01/20 */
				if ( pLand->m_bPVPSchoolWarsMap )
				{
					GLPVPSchoolWarsClient::GetInstance().RequestScoreInfo();

					if ( CInnerInterface::GetInstance().PVPSchoolWarsToShowRanking() )
						CInnerInterface::GetInstance().PVPSchoolWarsShowRanking();

					if ( !GLPVPSchoolWarsClient::GetInstance().m_bShowStartNotice &&
						GLPVPSchoolWarsClient::GetInstance().IsBattle() )
					{
						CInnerInterface::GetInstance().PVPSchoolWarsShowStartNotice();
						GLPVPSchoolWarsClient::GetInstance().m_bShowStartNotice = TRUE;
					}
				}


				/*pvp capture the flag, Juver, 2018/02/02 */
				if ( pLand->m_bPVPCaptureTheFlagMap )
				{
					GLPVPCaptureTheFlagClient::GetInstance().RequestScoreInfo();

					if ( CInnerInterface::GetInstance().PVPCaptureTheFlagToShowRanking() )
						CInnerInterface::GetInstance().PVPCaptureTheFlagShowRanking();

					if ( !GLPVPCaptureTheFlagClient::GetInstance().m_bShowStartNotice &&
						GLPVPCaptureTheFlagClient::GetInstance().IsBattle() )
					{
						CInnerInterface::GetInstance().PVPCaptureTheFlagShowStartNotice();
						GLPVPCaptureTheFlagClient::GetInstance().m_bShowStartNotice = TRUE;
					}
				}

				/*pvp capture the flag, Juver, 2018/02/08 */
				if ( pLand->m_bPVPCaptureTheFlagMap )
					GetCharacter()->m_bCaptureTheFlagLocatorOn = TRUE;
				else
					GetCharacter()->m_bCaptureTheFlagLocatorOn = FALSE;

				if ( pLand->m_bClubBattle || pLand->m_bClubBattleHall )
				{
					if ( CInnerInterface::GetInstance().PVClubWarToShowRanking() )
						CInnerInterface::GetInstance().PVPClubWarRankingWindowRefresh();
				}	
				else
				{
					PVPClubWarsRankClear();
				}
				
				/* pvp club death match, Juver, 2020/11/23 */
				if ( pLand->m_bPVPClubDeathMatchLobbyMap || pLand->m_bPVPClubDeathMatchBattleMap )
				{
					PVPClubDeathMatchClient::GetInstance().RequestScoreInfo();
					CInnerInterface::GetInstance().ShowGroupBottom( PVP_CLUB_DEATH_MATCH_SCORE );
				}
				else
				{
					CInnerInterface::GetInstance().HideGroup( PVP_CLUB_DEATH_MATCH_SCORE );
				}

				if ( pLand->m_bPVPClubDeathMatchLobbyMap || pLand->m_bPVPClubDeathMatchBattleMap )
				{
					if ( CInnerInterface::GetInstance().PVPClubDeathMatchToShowRanking() )
						CInnerInterface::GetInstance().PVPClubDeathMatchShowRanking();

					if ( PVPClubDeathMatchClient::GetInstance().m_bDoublePoint )
						CInnerInterface::GetInstance().PVPClubDeathMatchShowDoublePoint( TRUE );
					else
						CInnerInterface::GetInstance().PVPClubDeathMatchShowDoublePoint( FALSE );
				}

				if ( pLand->m_bPVPClubDeathMatchBattleMap )
				{
					if ( !PVPClubDeathMatchClient::GetInstance().m_bShowStartNotice &&
						PVPClubDeathMatchClient::GetInstance().IsBattle() )
					{
						CInnerInterface::GetInstance().PVPClubDeathMatchShowStartNotice();
						PVPClubDeathMatchClient::GetInstance().m_bShowStartNotice = TRUE;
					}
				}


				if ( pLand->m_bPVPPBGLobbyMap || pLand->m_bPVPPBGBattleMap )
				{
					if ( CInnerInterface::GetInstance().PVPPBGToShowRanking() )
						CInnerInterface::GetInstance().PVPPBGShowRanking();

					PVPPBG::ManagerClient::GetInstance().RequestScoreInfo();
					CInnerInterface::GetInstance().ShowGroupBottom( PVP_PBG_SCORE );
				}
				else
				{
					CInnerInterface::GetInstance().HideGroup( PVP_PBG_SCORE );
				}

				if ( pLand->m_bPVPPBGBattleMap )
				{
					if ( !PVPPBG::ManagerClient::GetInstance().m_bShowStartNotice &&
						PVPPBG::ManagerClient::GetInstance().IsBattle() )
					{
						// just reuse the existing
						CInnerInterface::GetInstance().PVPClubDeathMatchShowStartNotice();
						PVPPBG::ManagerClient::GetInstance().m_bShowStartNotice = TRUE;
					}
				}


				/* open pk function, Juver, 2019/12/27 */
				if ( pLand->IsOpenPK() )
				{
					CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("OPEN_PK_LAND") );
				}


//#ifdef _DEBUG
//
//				/* user flag verified, Juver, 2020/02/25 */
//				CInnerInterface::GetInstance().PrintConsoleTextDlg ( "m_bUserFlagVerified=%d", GetCharacter()->m_bUserFlagVerified );
//
//				/* user flag restricted, Juver, 2020/04/21 */
//				CInnerInterface::GetInstance().PrintConsoleTextDlg ( "m_bUserFlagRestricted=%d", GetCharacter()->m_bUserFlagRestricted );
//
//#endif 

				/* costume combine map restriction, Juver, 2020/09/14 */
				GetCharacter()->m_bCostumeCombineRestrictMoveSpeed		= pLand->IsCostumeCombineRestrictMoveSpeed();
				GetCharacter()->m_bCostumeCombineRestrictAttackSpeed	= pLand->IsCostumeCombineRestrictAttackSpeed();

				//temporary, need to very if works correctly and produce any problems
				GetCharacter()->INIT_DATA( FALSE, FALSE );

				//clean pkmark information when the current map does not support showing it
				if ( pLand->IsShowPkMark() == false )
				{
					GLPKMark::GetInstance().CleanUp();
				}

				//if ( RPARAM::bUseAutoPotion == TRUE )
				//{
				//	//only notify player when auto potion is active
				//	if ( pLand->isUseAutoPotion() == TRUE )
				//		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMAUTO_POTION_FB_ACTIVATED") );
				//}

			}
		}
		break;

	case NET_MSG_GCTRL_SERVER_INFO:
		{
			GLMSG::SNETPC_SERVER_INFO *pNetMsg = (GLMSG::SNETPC_SERVER_INFO *)nmg;

			if ( m_bSCHOOL_FREEPK != pNetMsg->bSCHOOL_FREEPK )
			{
				if ( pNetMsg->bSCHOOL_FREEPK )
				{
					CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("OTHER_SCHOOL_FREEPK_ON") );
				}
				else
				{
					CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("OTHER_SCHOOL_FREEPK_OFF") );

					// 적대관계 모두 삭제
					GetCharacter()->DEL_PLAYHOSTILE_ALL();
				}
			}

			m_bSCHOOL_FREEPK = pNetMsg->bSCHOOL_FREEPK;

		}
		break;

	case NET_MSG_GCTRL_SERVER_BRIGHTEVENT_INFO:
		{
			GLMSG::SNETPC_SERVER_BRIGHTEVENT_INFO *pNetMsg = ( GLMSG::SNETPC_SERVER_BRIGHTEVENT_INFO *)nmg;

			m_bBRIGHTEVENT = pNetMsg->bBRIGHTEVENT;
		}
		break;

	case NET_MSG_GCTRL_SERVER_CLUB_BATTLE_INFO:
		{
		}
		break;
	
	case NET_MSG_GCTRL_SERVER_CLUB_DEATHMATCH_INFO:
		{
			GLMSG::SNETPC_SERVER_CLUB_DEATHMATCH_INFO* pNetMsg = ( GLMSG::SNETPC_SERVER_CLUB_DEATHMATCH_INFO* )nmg;
			m_bClubDeathMatch = pNetMsg->bClubDeathMatch;
		}
		break;

	case NET_MSG_GCTRL_CLUB_COMMISSION_BRD:
		{
			GLMSG::SNET_CLUB_GUID_COMMISSION_BRD *pNetMsg = (GLMSG::SNET_CLUB_GUID_COMMISSION_BRD *)nmg;

			PLANDMANCLIENT pLand = GetActiveMap();
			if ( pLand )
			{
				pLand->m_fCommission = pNetMsg->fCommission;
				CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("LAND_TRADE_NEW_COMMISSION"), pLand->m_fCommission );
			}
		}
		break;

	case NET_MSG_GCTRL_CLUB_COMMISSION_RV_BRD:
		{
			GLMSG::SNET_CLUB_GUID_COMMISSION_RESERVE_BRD *pNetMsg = (GLMSG::SNET_CLUB_GUID_COMMISSION_RESERVE_BRD *)nmg;

			CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("LAND_TRADE_NEW_RV_COMMISSION"), pNetMsg->fCommission );
		}
		break;

	case NET_MSG_GM_WHERE_NPC_FB:
		{
			GLMSG::SNET_GM_WHERE_NPC_FB *pNetMsg = (GLMSG::SNET_GM_WHERE_NPC_FB *) nmg;

			CInnerInterface::GetInstance().PrintConsoleText ( "npc_pos : %d, %d", pNetMsg->nPosX, pNetMsg->nPosY );
		}
		break;

	case NET_MSG_GM_PRINT_CROWLIST_FB:
		{
			GLMSG::SNET_GM_PRINT_CROWLIST_FB *pNetMsg = (GLMSG::SNET_GM_PRINT_CROWLIST_FB *) nmg;

			PCROWDATA pCrow = GLCrowDataMan::GetInstance().GetCrowData( pNetMsg->mID, pNetMsg->sID );
			if ( !pCrow ) break;

			if( pNetMsg->emCrow == CROW_NPC )
			{
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, "type : CROW_NPC, mid : %d, SID : %d, num : %d, name : %s", 
					pNetMsg->mID, pNetMsg->sID, pNetMsg->wNum, pCrow->GetName() );
			}
			/* crow zone, Juver, 2018/02/21 */
			else if( pNetMsg->emCrow == CROW_ZONE_NAME )
			{
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, "type : CROW_ZONE_NAME, mid : %d, SID : %d, num : %d, name : %s", 
					pNetMsg->mID, pNetMsg->sID, pNetMsg->wNum, pCrow->GetName() );
			}
			/* crow zone, Juver, 2018/02/21 */
			else if( pNetMsg->emCrow == CROW_GATE_NAME )
			{
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::PALEGREEN, "type : CROW_GATE_NAME, mid : %d, SID : %d, num : %d, name : %s", 
					pNetMsg->mID, pNetMsg->sID, pNetMsg->wNum, pCrow->GetName() );
			}
			else
			{
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::RED, "type : CROW_MOB, mid : %d, SID : %d, num : %d, name : %s", 
					pNetMsg->mID, pNetMsg->sID, pNetMsg->wNum, pCrow->GetName() );
			}
			
		}
		break;


	case NET_MSG_GM_WHERE_PC_MAP_FB:
		{
			GLMSG::SNET_GM_WHERE_PC_MAP_FB *pNetMsg = (GLMSG::SNET_GM_WHERE_PC_MAP_FB *) nmg;

			const char *pMapName = GetMapName ( pNetMsg->nidMAP );
			if ( pMapName )
			{
				CInnerInterface::GetInstance().PrintConsoleText ( "pc_map : %s, channel %d, mapid[%d/%d]",
					pMapName, pNetMsg->dwChannel, pNetMsg->nidMAP.wMainID, pNetMsg->nidMAP.wSubID );
			}
		}
		break;

	case NET_MSG_GM_WHERE_PC_POS_FB:
		{
			GLMSG::SNET_GM_WHERE_PC_POS_FB *pNetMsg = (GLMSG::SNET_GM_WHERE_PC_POS_FB *) nmg;
			
			if ( pNetMsg->bFOUND )		CInnerInterface::GetInstance().PrintConsoleText ( "pc_pos : %d, %d", pNetMsg->nPosX, pNetMsg->nPosY );
			else						CInnerInterface::GetInstance().PrintConsoleText ( "pc_pos : not found" );
		}
		break;

	case NET_MSG_GM_MOVE2CHAR_FB:
		{
			GLMSG::SNETPC_GM_MOVE2CHAR_FB *pNetMsg = (GLMSG::SNETPC_GM_MOVE2CHAR_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMGM_MOVE2CHAR_FB_FAIL:
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("EMGM_MOVE2CHAR_FB_FAIL") );
				break;
			case EMGM_MOVE2CHAR_FB_OK:
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("EMGM_MOVE2CHAR_FB_OK") );
				break;
			case EMGM_MOVE2CHAR_FB_TO_CONDITION:
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("EMGM_MOVE2CHAR_FB_TO_CONDITION") );
				break;
			case EMGM_MOVE2CHAR_FB_MY_CONDITION:
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("EMGM_MOVE2CHAR_FB_MY_CONDITION") );
				break;
			case EMGM_MOVE2CHAR_FB_CHANNEL:
				CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEINTEXT("EMGM_MOVE2CHAR_FB_CHANNEL"), pNetMsg->nChannel );
				break;
			};
		}
		break;

	case NET_MSG_GM_CHAT_BLOCK_FB:
		{
			GLMSG::SNET_GM_CHAT_BLOCK_FB *pNetMsg = (GLMSG::SNET_GM_CHAT_BLOCK_FB *) nmg;

			CInnerInterface::GetInstance().PrintConsoleText ( "block        : %s", bool2sz(pNetMsg->bBLOCK) );
			CInnerInterface::GetInstance().PrintConsoleText ( "user account : %s", pNetMsg->szUACCOUNT );
			CInnerInterface::GetInstance().PrintConsoleText ( "char name    : %s", pNetMsg->szCHARNAME );

			CTimeSpan tTimeSpan( 0, 0, pNetMsg->dwBLOCK_MINUTE, 0 );
			CInnerInterface::GetInstance().PrintConsoleText ( "block minute : %d (%I64d days)", pNetMsg->dwBLOCK_MINUTE, tTimeSpan.GetDays() );

			CInnerInterface::GetInstance().PrintConsoleText ( "reason : %s", pNetMsg->szReason );

		}
		break;

	case NET_MSG_GM_CHAR_INFO_AGT_FB:
		{
			GLMSG::SNET_GM_CHAR_INFO_AGT_FB *pNetMsg = (GLMSG::SNET_GM_CHAR_INFO_AGT_FB *) nmg;

			CInnerInterface::GetInstance().PrintConsoleText ( "user account : %s", pNetMsg->szUACCOUNT );
			CInnerInterface::GetInstance().PrintConsoleText ( "char name    : %s", pNetMsg->szCHARNAME );
			//CInnerInterface::GetInstance().PrintConsoleText ( "server       : %d", pNetMsg->dwSERVER );
			CInnerInterface::GetInstance().PrintConsoleText ( "channel      : %d", pNetMsg->dwCHANNEL );
			CInnerInterface::GetInstance().PrintConsoleText ( "charid       : %d", pNetMsg->dwCHARID );
			CInnerInterface::GetInstance().PrintConsoleText ( "gaeaid		  : %d", pNetMsg->dwGAEAID );

			if ( pNetMsg->dwPartyID != PARTY_NULL )
				CInnerInterface::GetInstance().PrintConsoleText ( "party id		  : %d", pNetMsg->dwPartyID );
			else
				CInnerInterface::GetInstance().PrintConsoleText ( "party id		  : none" );
		}
		break;

	case NET_MSG_GM_CHAR_INFO_FLD_FB:
		{
			GLMSG::SNET_GM_CHAR_INFO_FLD_FB *pNetMsg = (GLMSG::SNET_GM_CHAR_INFO_FLD_FB *) nmg;

			CInnerInterface::GetInstance().PrintConsoleText ( "userid       : %d", pNetMsg->dwUSERID );
			CInnerInterface::GetInstance().PrintConsoleText ( "charid       : %d", pNetMsg->dwCHARID );
			CInnerInterface::GetInstance().PrintConsoleText ( "mapid        : %d/%d", pNetMsg->nidMAP.wMainID, pNetMsg->nidMAP.wSubID );
			CInnerInterface::GetInstance().PrintConsoleText ( "position     : %d/%d", pNetMsg->wPOSX, pNetMsg->wPOSY );
			CInnerInterface::GetInstance().PrintConsoleText ( "level        : %d", pNetMsg->wLEVEL );

			CInnerInterface::GetInstance().PrintConsoleText ( "hp           : %d/%d", pNetMsg->sHP.wNow, pNetMsg->sHP.wMax );
			CInnerInterface::GetInstance().PrintConsoleText ( "mp           : %d/%d", pNetMsg->sMP.wNow, pNetMsg->sMP.wMax );
			CInnerInterface::GetInstance().PrintConsoleText ( "sp           : %d/%d", pNetMsg->sSP.wNow, pNetMsg->sSP.wMax );
			CInnerInterface::GetInstance().PrintConsoleText ( "cp           : %d/%d", pNetMsg->sCP.wNow, pNetMsg->sCP.wMax ); /*combatpoint logic, Juver, 2017/05/29 */
			
			//std::strstream strSTREAM;
			//strSTREAM << pNetMsg->sEXP.lnNow << '/' << pNetMsg->sEXP.lnMax << std::ends;

			CInnerInterface::GetInstance().PrintConsoleText ( "exp          : %I64d/%I64d", pNetMsg->sEXP.lnNow, pNetMsg->sEXP.lnMax );
			CInnerInterface::GetInstance().PrintConsoleText ( "club         : %s", pNetMsg->szCLUB );

			//strSTREAM.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
		}
		break;

	case NET_MSG_GM_WARNING_MSG_BRD:
		{
			GLMSG::SNET_GM_WARNING_MSG_BRD* pNetMsg = (GLMSG::SNET_GM_WARNING_MSG_BRD*) nmg;
			if ( pNetMsg->bOn )
			{
				CInnerInterface::GetInstance().WARNING_MSG_ON();
			}
			else
			{
				CInnerInterface::GetInstance().WARNING_MSG_OFF();
			}
		}
		break;

	case NET_MSG_GM_COUNTDOWN_MSG_BRD:
		{
			GLMSG::SNET_GM_COUNTDOWN_MSG_BRD* pNetMsg = (GLMSG::SNET_GM_COUNTDOWN_MSG_BRD*) nmg;
			CInnerInterface::GetInstance().SET_COUNT_MSG( pNetMsg->nCount );
		}

	case NET_MSG_GCTRL_SERVERTEST_FB:
		{
			m_pLandMClient->ResetClientObjects ();
			CString strCombine;
			strCombine.Format ( "%s", ID2GAMEINTEXT("SERVER_STOP_SOON") );
			CInnerInterface::GetInstance().DisplayChatMessage ( CHAT_TYPE_GLOBAL, NULL, strCombine.GetString() );
			
			/*dual pet skill, Juver, 2017/12/27 */
			m_Character.m_sPETSKILLFACT_A.RESET ();
			m_Character.m_sPETSKILLFACT_B.RESET ();

			/*dual pet skill, Juver, 2017/12/27 */
			FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_Character.m_dwGaeaID,m_Character.GetPosition() ), m_Character.GetSkinChar (), m_Pet.m_sActiveSkillID_A );
			FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_Character.m_dwGaeaID,m_Character.GetPosition() ), m_Character.GetSkinChar (), m_Pet.m_sActiveSkillID_B );

			m_Pet.DeleteDeviceObjects ();
		}
		break;

	case NET_MSG_SERVERTIME_BRD:
		{
			GLMSG::SNET_MSG_SERVERTIME_BRD* pNetMsg = (GLMSG::SNET_MSG_SERVERTIME_BRD*) nmg;
			SetServerTimeBias(pNetMsg->nTimeBias);
			SetCurrentTime(pNetMsg->t64Time);
		}
		break;		


//----------------------------------------------------------------------------------------------------------------
// Message Processing about PET (START)
//----------------------------------------------------------------------------------------------------------------
	
	// =======  Message Processing about MyPET (START)  ======= //

	case NET_MSG_PET_REQ_USECARD_FB:
		{
			GLMSG::SNETPET_REQ_USEPETCARD_FB* pNetMsg = ( GLMSG::SNETPET_REQ_USEPETCARD_FB* ) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMPET_USECARD_FB_OK:
				{
					// 최초생성후 팻을 제거했을때를 대비해서 클라이언트의 아이템에 팻번호 세팅
					SINVENITEM* pItem = m_Character.m_cInventory.GetItem ( m_Character.m_wInvenPosX1, m_Character.m_wInvenPosY1 );
					if ( !pItem ) return;
					if ( pItem->sItemCustom.dwPetID == 0 ) pItem->sItemCustom.dwPetID = pNetMsg->m_dwPetID;
					
					m_Pet.m_sPetSkinPackState.bUsePetSkinPack = pNetMsg->m_sPetSkinPackData.bUsePetSkinPack;
					m_Pet.m_sPetSkinPackState.petSkinMobID	  = pNetMsg->m_sPetSkinPackData.sMobID;
					m_Pet.m_sPetSkinPackState.fPetScale		  = pNetMsg->m_sPetSkinPackData.fScale;
					// 생성
					if ( !FAILED( CreatePET ( nmg ) ) )
					{
						CInnerInterface::GetInstance().PrintMsgText ( 
							NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPET_USECARD_FB_OK"), m_Pet.m_szName );

						/*pet status, Juver, 2017/07/30 */
						CInnerInterface::GetInstance().ShowPetStatus( TRUE );	
						
					}

					if ( m_Pet.IsUsePetSkinPack() )
					{
						CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_PETSKINPACK_FB_OK") );
					}
				}
				break;
			case EMPET_USECARD_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_USECARD_FB_FAIL") );
				// 실패시 GenNum 처리
				m_Character.m_llPetCardGenNum = 0;
				break;
			case EMPET_USECARD_FB_INVALIDCARD:
				CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_USECARD_FB_INVALIDCARD") );
				// 실패시 GenNum 처리
				m_Character.m_llPetCardGenNum = 0;
				break;
			case EMPET_USECARD_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_USECARD_FB_NOITEM") );
				// 실패시 GenNum 처리
				m_Character.m_llPetCardGenNum = 0;
				break;
			case EMPET_USECARD_FB_NOTENOUGHFULL:
				CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_USECARD_FB_NOTENOUGHFULL") );
				// 실패시 GenNum 처리
				m_Character.m_llPetCardGenNum = 0;
				break;

			case EMPET_USECARD_FB_INVALIDZONE:
				CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_USECARD_FB_INVALIDZONE") );
				// 실패시 GenNum 처리
				m_Character.m_llPetCardGenNum = 0;
				break;

				/*skill pet off, Juver, 2018/09/07 */
			case EMPET_USECARD_FB_ACTIONLIMIT:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_USECARD_FB_ACTIONLIMIT") );
					// 실패시 GenNum 처리
					m_Character.m_llPetCardGenNum = 0;
				}break;

				/*pet fix add call delay, EJCode, 2018/11/28 */
			case EMPET_USECARD_FB_TIME_DELAY:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_USECARD_FB_USE_DELAY") );
					m_Character.m_llPetCardGenNum = 0;
				}break;

			case EMPET_USECARD_FB_NODATA:

				// 펫카드 정보 리셋
				SINVENITEM* pItem = m_Character.m_cInventory.GetItem ( m_Character.m_wInvenPosX1, m_Character.m_wInvenPosY1 );
				if ( !pItem ) return;
				pItem->sItemCustom.dwPetID = 0;

				CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_USECARD_FB_NODATA") );
				m_Character.m_llPetCardGenNum = 0;
				break;
			}
		}		
		break;

	
	case NET_MSG_PET_PETSKINPACKOPEN_FB:
		{
			GLMSG::SNETPET_SKINPACKOPEN_FB *pNetMsg = (GLMSG::SNETPET_SKINPACKOPEN_FB *)nmg;

			if( pNetMsg->emFB == EMPET_PETSKINPACKOPEN_FB_FAIL )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_PETSKINPACK_FB_FAIL") );
				break;
			}
			if( pNetMsg->emFB == EMPET_PETSKINPACKOPEN_FB_PETCARD_FAIL )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPET_PETSKINPACKOPEN_FB_PETCARD_FAIL") );
				break;
			}

			m_Pet.m_sPetSkinPackState.Init();

			m_Pet.m_sPetSkinPackState.bUsePetSkinPack = pNetMsg->sPetSkinPackData.bUsePetSkinPack;
			m_Pet.m_sPetSkinPackState.petSkinMobID	  = pNetMsg->sPetSkinPackData.sMobID;
			m_Pet.m_sPetSkinPackState.fPetScale		  = pNetMsg->sPetSkinPackData.fScale;

			if( pNetMsg->emFB == EMPET_PETSKINPACKOPEN_FB_OK )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_PETSKINPACK_FB_OK") );				
			}
			if( pNetMsg->emFB == EMPET_PETSKINPACKOPEN_FB_END )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_PETSKINPACK_FB_END") );
			}

			PetSkinPackApplyEffect();

		}
		break;


	case NET_MSG_PET_REQ_UNUSECARD_FB:
		{
            GLMSG::SNETPET_REQ_UNUSEPETCARD_FB *pNetMsg = ( GLMSG::SNETPET_REQ_UNUSEPETCARD_FB* ) nmg;

			// 맵 이동중에 팻이 사라져야 하는 경우
			if ( pNetMsg->bMoveMap ) m_Character.m_bIsPetActive = TRUE;
			
			/*dual pet skill, Juver, 2017/12/27 */
			m_Character.m_sPETSKILLFACT_A.RESET ();
			m_Character.m_sPETSKILLFACT_B.RESET ();

			/*dual pet skill, Juver, 2017/12/27 */
			FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_Character.m_dwGaeaID,m_Character.GetPosition() ), m_Character.GetSkinChar (), m_Pet.m_sActiveSkillID_A );
			FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_Character.m_dwGaeaID,m_Character.GetPosition() ), m_Character.GetSkinChar (), m_Pet.m_sActiveSkillID_B );

			m_Pet.DeleteDeviceObjects ();

			CInnerInterface::GetInstance().PrintMsgText ( 
				NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPET_UNUSECARD_FB_OK"), m_Pet.m_szName );

			/*pet status, Juver, 2017/07/30 */
			CInnerInterface::GetInstance().ShowPetStatus( FALSE );	
		}
		break;

	case NET_MSG_PET_REQ_GIVEFOOD_FB:
		{
			GLMSG::SNETPET_REQ_GIVEFOOD_FB* pNetMsg = ( GLMSG::SNETPET_REQ_GIVEFOOD_FB* ) nmg;
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sNativeID );
			if ( pItem )
			{
				// 팻포만감 갱신
				if ( m_Pet.IsVALID () ) m_Pet.IncreaseFull ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio );

				if ( DxGlobalStage::GetInstance().IsEmulator() )
				{
					CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPET_REQ_GIVEFOOD_FB_OK"), m_Pet.m_szName, pItem->GetName (), pItem->sDrugOp.wCureVolume );
					break;
				}

				PETCARDINFO_MAP_ITER iter = m_Character.m_mapPETCardInfo.find ( pNetMsg->dwPetID );
				if ( iter!=m_Character.m_mapPETCardInfo.end() )
				{
					SPETCARDINFO& sPet = (*iter).second;
					// 팻카드의 정보 갱신
					sPet.m_nFull = pNetMsg->nFull;

					CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPET_REQ_GIVEFOOD_FB_OK"), sPet.m_szName, pItem->GetName (), pItem->sDrugOp.wCureVolume );
				}
			}
		}
		break;

	case NET_MSG_PET_REQ_PETCARDINFO_FB:
		{
			GLMSG::SNETPET_REQ_PETCARDINFO_FB* pNetMsg = ( GLMSG::SNETPET_REQ_PETCARDINFO_FB* ) nmg;

			// DB에 없으면 그냥 초기값으로 넘어오는 경우가 있으므로 체크해준다.
			if ( pNetMsg->emTYPE == PET_TYPE_NONE ) break;

			SPETCARDINFO sPetCardInfo;
			sPetCardInfo.m_emTYPE = pNetMsg->emTYPE;
			sPetCardInfo.m_nFull  = pNetMsg->nFull;

			/*dual pet skill, Juver, 2017/12/27 */
			sPetCardInfo.m_sActiveSkillID_A  = pNetMsg->sActiveSkillID_A;
			sPetCardInfo.m_sActiveSkillID_B  = pNetMsg->sActiveSkillID_B;
			sPetCardInfo.m_bDualSkill		 = pNetMsg->bDualSkill;

			StringCchCopy ( sPetCardInfo.m_szName, PETNAMESIZE+1, pNetMsg->szName );

			for ( WORD i = 0; i < pNetMsg->wSkillNum; ++i )
			{
				PETSKILL sPetSkill = pNetMsg->Skills[i];
				sPetCardInfo.m_ExpSkills.insert ( std::make_pair(sPetSkill.sNativeID.dwID,sPetSkill) );
			}

			for ( WORD i = 0; i < PET_ACCETYPE_SIZE; ++i )
			{
				sPetCardInfo.m_PutOnItems[i] = pNetMsg->PutOnItems[i];
			}

			if ( !pNetMsg->bTrade )
			{
				PETCARDINFO_MAP_ITER iter = m_Character.m_mapPETCardInfo.find ( pNetMsg->dwPetID );
				if ( iter != m_Character.m_mapPETCardInfo.end() ) m_Character.m_mapPETCardInfo.erase ( iter );

				m_Character.m_mapPETCardInfo.insert ( std::make_pair(pNetMsg->dwPetID, sPetCardInfo) );
			}
			else
			{
				m_Character.m_mapPETCardInfoTemp.insert ( std::make_pair(pNetMsg->dwPetID, sPetCardInfo) );
			}
			
		}
		break;

	case NET_MSG_PET_REQ_PETREVIVEINFO_FB:
		{
			GLMSG::SNETPET_REQ_PETREVIVEINFO_FB* pNetMsg = ( GLMSG::SNETPET_REQ_PETREVIVEINFO_FB* ) nmg;
			m_Character.m_mapPETReviveInfo.clear();
			for ( WORD i = 0; i < pNetMsg->wPetReviveNum; ++i )
			{
				m_Character.m_mapPETReviveInfo.insert ( 
					std::make_pair(pNetMsg->arryPetReviveInfo[i].dwPetID, pNetMsg->arryPetReviveInfo[i] ) );
			}
		}
		break;

	case NET_MSG_PET_REQ_REVIVE_FB:
		{
			GLMSG::SNETPET_REQ_REVIVE_FB* pNetMsg = ( GLMSG::SNETPET_REQ_REVIVE_FB* ) nmg;
			
			PETREVIVEINFO sPetRevInfo;
			PETREVIVEINFO_MAP_ITER iter = m_Character.m_mapPETReviveInfo.find ( pNetMsg->dwPetID );
			if ( iter != m_Character.m_mapPETReviveInfo.end() ) sPetRevInfo = (*iter).second;
			else												break;

			switch ( pNetMsg->emFB )
			{
			case EMPET_REQ_REVIVE_FB_OK:
				{
					CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPET_REQ_REVIVE_FB_OK"), sPetRevInfo.szPetName );
					SINVENITEM* pINVENITEM = m_Character.m_cInventory.GetItem ( m_Character.m_wInvenPosX1, m_Character.m_wInvenPosY1 );
					if ( pINVENITEM ) pINVENITEM->sItemCustom.dwPetID = sPetRevInfo.dwPetID;
					m_Character.m_mapPETReviveInfo.erase ( iter );

				}
				break;
			case EMPET_REQ_REVIVE_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_REVIVE_FB_FAIL"), sPetRevInfo.szPetName );
				break;
			case EMPET_REQ_REVIVE_FB_DBFAIL:
				CInnerInterface::GetInstance().PrintMsgText ( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_REVIVE_FB_DBFAIL"), sPetRevInfo.szPetName );
				break;
			}
		}
		break;
	case NET_MSG_PET_NOTENOUGHINVEN:
		{
			CInnerInterface::GetInstance().PrintConsoleTextDlg( ID2GAMEINTEXT("EMPET_NOTENOUGHINVEN") );
		}
		break;

		/*dual pet skill, Juver, 2017/12/29 */
	case NET_MSG_PET_REQ_ENABLE_DUAL_SKILL_FB:
		{
			GLMSG::SNETPET_REQ_ENABLE_DUAL_SKILL_FB* pNetMsg = ( GLMSG::SNETPET_REQ_ENABLE_DUAL_SKILL_FB* ) nmg;
			switch( pNetMsg->emFB )
			{
			case EMPET_REQ_ENABLE_DUAL_SKILL_FB_OK:
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sNativeID );
					if ( pItem )
					{
						if ( m_Pet.IsVALID () )
						{
							m_Pet.m_bDualSkill = pNetMsg->bDualSkill;

							D3DXMATRIX matEffect;
							D3DXVECTOR3 vPos = m_Pet.GetPosition ();
							D3DXMatrixTranslation ( &matEffect, vPos.x, vPos.y, vPos.z );

							STARGETID sTargetID(CROW_PET, m_Pet.m_dwGUID, vPos );
							DxEffGroupPlayer::GetInstance().NewEffGroup( GLCONST_CHAR::strSKILL_LEARN_EFFECT.c_str(), matEffect, &sTargetID );
						}

						if ( DxGlobalStage::GetInstance().IsEmulator() )
						{
							CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPET_REQ_ENABLE_DUAL_SKILL_FB_OK"), m_Pet.m_szName );
						}
						else
						{
							PETCARDINFO_MAP_ITER iter = m_Character.m_mapPETCardInfo.find ( pNetMsg->dwPetID );
							if ( iter!=m_Character.m_mapPETCardInfo.end() )
							{
								SPETCARDINFO& sPet = (*iter).second;
								sPet.m_bDualSkill = pNetMsg->bDualSkill;

								CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPET_REQ_ENABLE_DUAL_SKILL_FB_OK"), sPet.m_szName );
							}
						}
					}
				}break;

			case EMPET_REQ_ENABLE_DUAL_SKILL_FB_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_ENABLE_DUAL_SKILL_FB_FAIL") );
				}break;

			case EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_ITEM:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_ITEM") );
				}break;

			case EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_CARD:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_ENABLE_DUAL_SKILL_INVALID_CARD") );
				}break;

			case EMPET_REQ_ENABLE_DUAL_SKILL_ALREADY_ENABLED:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_ENABLE_DUAL_SKILL_ALREADY_ENABLED") );
				}break;

			case EMPET_REQ_ENABLE_DUAL_SKILL_NOT_ACTIVE:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPET_REQ_ENABLE_DUAL_SKILL_NOT_ACTIVE") );
				}break;
			};
		}break;


	case NET_MSG_PET_REQ_UPDATE_MOVE_STATE_FB:

		/*dual pet skill, Juver, 2017/12/28 */
	case NET_MSG_PET_GETRIGHTOFITEM_A_FB:
	case NET_MSG_PET_GETRIGHTOFITEM_B_FB:

	case NET_MSG_PET_UPDATECLIENT_FULL:

		/*dual pet skill, Juver, 2017/12/27 */
	case NET_MSG_PET_REQ_SKILLCHANGE_A_FB:
	case NET_MSG_PET_REQ_SKILLCHANGE_B_FB:

	case NET_MSG_PET_REQ_RENAME_FB:
	case NET_MSG_PET_REQ_CHANGE_COLOR_FB:
	case NET_MSG_PET_REQ_CHANGE_STYLE_FB:
	case NET_MSG_PET_REQ_SLOT_EX_HOLD_FB:
	case NET_MSG_PET_REQ_HOLD_TO_SLOT_FB:
	case NET_MSG_PET_REQ_SLOT_TO_HOLD_FB:

		/*dual pet skill, Juver, 2017/12/27 */
	case NET_MSG_PET_ADD_SKILLFACT_A:
	case NET_MSG_PET_REMOVE_SKILLFACT_A:
	case NET_MSG_PET_ADD_SKILLFACT_B:
	case NET_MSG_PET_REMOVE_SKILLFACT_B:

	case NET_MSG_PET_REQ_LEARNSKILL_FB:
	case NET_MSG_PET_REMOVE_SLOTITEM_FB:
	case NET_MSG_PET_ATTACK:
	case NET_MSG_PET_SAD:
	case NET_MSG_PET_ACCESSORY_DELETE:
		{
			if ( m_Pet.IsVALID () )
			{
				m_Pet.MsgProcess ( nmg );
			}
		}
		break;

	// =======  Message Processing about MyPET (END)  ======= //
	case NET_MSG_SUMMON_ATTACK:
		{
			/*skill summon, Juver, 2017/10/09 */
			GLMSG::SNET_SUMMON_ATTACK* pNetMsg = ( GLMSG::SNET_SUMMON_ATTACK* ) nmg;
			GLSummonClient* psummon_client = GetSummonClient( pNetMsg->dwGUID );
			if ( psummon_client )	psummon_client->MsgProcess ( nmg );
		}break;

	case NET_MSG_SUMMON_END_REST:
		{
			/*skill summon, Juver, 2017/10/09 */
			GLMSG::SNET_SUMMON_END_REST* pNetMsg = ( GLMSG::SNET_SUMMON_END_REST* ) nmg;
			GLSummonClient* psummon_client = GetSummonClient( pNetMsg->dwGUID );
			if ( psummon_client )	psummon_client->MsgProcess ( nmg );
		}break;

	case NET_MSG_SUMMON_END_LIFE:
		{
			/*skill summon, Juver, 2017/10/09 */
			GLMSG::SNET_SUMMON_END_LIFE* pNetMsg = ( GLMSG::SNET_SUMMON_END_LIFE* ) nmg;
			GLSummonClient* psummon_client = GetSummonClient( pNetMsg->dwGUID );
			if ( psummon_client )	psummon_client->MsgProcess ( nmg );
		}break;

	case NET_MSG_SUMMON_RESET_TARGET:
		{
			/*skill summon, Juver, 2017/10/09 */
			GLMSG::SNET_SUMMON_RESET_TARGET* pNetMsg = ( GLMSG::SNET_SUMMON_RESET_TARGET* ) nmg;
			GLSummonClient* psummon_client = GetSummonClient( pNetMsg->dwGUID );
			if ( psummon_client )	psummon_client->MsgProcess ( nmg );
		}break;


	// =======  Message Processing about AnyPET (START)  ======= //

	case NET_MSG_PET_DROPPET:
		{
			GLMSG::SNETPET_DROP_PET* pNetMsg = ( GLMSG::SNETPET_DROP_PET* ) nmg;
			SDROPPET& sDropPet = pNetMsg->Data;

			if ( sDropPet.m_sMapID == m_pLandMClient->GetMapID() )
			{
				m_pLandMClient->DropPet ( &sDropPet );
			}
			else
			{
				CDebugSet::ToListView ( "sDropCrow.sMapID != m_pLandMClient->GetMapID()" );
			}
		}
		break;

	case NET_MSG_CREATE_ANYPET:
		{
			GLMSG::SNETPET_CREATE_ANYPET* pNetMsg = ( GLMSG::SNETPET_CREATE_ANYPET* ) nmg;
			SDROPPET& sDropPet = pNetMsg->Data;

			if ( sDropPet.m_sMapID == m_pLandMClient->GetMapID() )
			{				
				m_pLandMClient->CreateAnyPet ( &sDropPet );
			}
			else
			{
				CDebugSet::ToListView ( "sDropCrow.sMapID != m_pLandMClient->GetMapID()" );
			}
		}
		break;

	case NET_MSG_PET_GOTO_BRD:
	case NET_MSG_PET_REQ_UPDATE_MOVE_STATE_BRD:

		/*dual pet skill, Juver, 2017/12/27 */
	case NET_MSG_PET_REQ_SKILLCHANGE_A_BRD:
	case NET_MSG_PET_REQ_SKILLCHANGE_B_BRD:

	case NET_MSG_PET_STOP_BRD:
	case NET_MSG_PET_REQ_RENAME_BRD:
	case NET_MSG_PET_REQ_CHANGE_COLOR_BRD:
	case NET_MSG_PET_REQ_CHANGE_STYLE_BRD:
	case NET_MSG_PET_REQ_SLOT_EX_HOLD_BRD:
	case NET_MSG_PET_REQ_LEARNSKILL_BRD:
	case NET_MSG_PET_REQ_FUNNY_BRD:
	case NET_MSG_PET_REMOVE_SLOTITEM_BRD:
	case NET_MSG_PET_ATTACK_BRD:
	case NET_MSG_PET_SAD_BRD:
	case NET_MSG_PET_ACCESSORY_DELETE_BRD:
	case NET_MSG_PET_PETSKINPACKOPEN_BRD:
		{
			GLMSG::SNETPET_BROAD *pNetMsg = (GLMSG::SNETPET_BROAD *) nmg;

			PGLANYPET pPet = m_pLandMClient->GetPet ( pNetMsg->dwGUID );
			if ( !pPet )
			{
				CDebugSet::ToListView ( "[PGLCHARCLIENT NULL] 수신 PET가 없는 메세지 발생. gaeaid %d", pNetMsg->dwGUID );
				return;
			}

			pPet->MsgProcess ( nmg );
		}
		break;
	// =======  Message Processing about AnyPet (END)  ======= //

	// =======  Message Processing about Summon (START)  ======= //
	// =======  Message Processing about Summon (END)  ======= //

	// =======  Message Processing about AnySummon (START)  ======= //

	case NET_MSG_DROP_ANYSUMMON:
		{
			GLMSG::SNET_SUMMON_DROP_SUMMON* pNetMsg = ( GLMSG::SNET_SUMMON_DROP_SUMMON* ) nmg;
			SDROPSUMMON& sDropSummon = pNetMsg->Data;

			if ( sDropSummon.m_sMapID == m_pLandMClient->GetMapID() )
			{
				m_pLandMClient->DropSummon ( &sDropSummon );
			}
			else
			{
				CDebugSet::ToListView ( "sDropCrow.sMapID != m_pLandMClient->GetMapID()" );
			}
		}
		break;
	case NET_MSG_CREATE_ANYSUMMON:
		{
			GLMSG::SNET_SUMMON_CREATE_ANYSUMMON* pNetMsg = ( GLMSG::SNET_SUMMON_CREATE_ANYSUMMON* ) nmg;
			SDROPSUMMON& sDropSummon = pNetMsg->Data;

			if ( sDropSummon.m_sMapID == m_pLandMClient->GetMapID() )
			{				
				m_pLandMClient->CreateAnySummon ( &sDropSummon );
			}
			else
			{
				CDebugSet::ToListView ( "sDropCrow.sMapID != m_pLandMClient->GetMapID()" );
			}
		}
		break;

	case NET_MSG_SUMMON_ATTACK_BRD:
	case NET_MSG_SUMMON_REQ_STOP_BRD:
	case NET_MSG_SUMMON_GOTO_BRD:
	case NET_MSG_SUMMON_REQ_UPDATE_MOVE_STATE_BRD:

		/*skill summon, Juver, 2017/10/09 */
	case NET_MSG_SUMMON_END_REST_BRD: 
	case NET_MSG_SUMMON_END_LIFE_BRD:
	case NET_MSG_SUMMON_RESET_TARGET_BRD:
		{
			GLMSG::SNET_SUMMON_BROAD *pNetMsg = (GLMSG::SNET_SUMMON_BROAD *) nmg;

			PGLANYSUMMON pSummon = m_pLandMClient->GetSummon ( pNetMsg->dwGUID );
			if ( !pSummon )
			{
				CDebugSet::ToListView ( "[PGLCHARCLIENT NULL] 수신 SUMMON가 없는 메세지 발생. gaeaid %d", pNetMsg->dwGUID );
				return;
			}

			pSummon->MsgProcess ( nmg );
		}
		break;

	// =======  Message Processing about AnySummon (END)  ======= //


//----------------------------------------------------------------------------------------------------------------
// Message Processing about PET (END)
//----------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------
// Message Processing about SUMMON (START)
//----------------------------------------------------------------------------------------------------------------

	case NET_MSG_REQ_USE_SUMMON_FB:
		{
			GLMSG::SNETPC_REQ_USE_SUMMON_FB* pNetMsg = ( GLMSG::SNETPC_REQ_USE_SUMMON_FB* ) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMUSE_SUMMON_FB_OK:
				{
					// 생성
					if ( !FAILED( CreateSummon ( nmg ) ) )
					{
						//CInnerInterface::GetInstance().ShowGroupBottom( SUMMON_HP );	
						//CInnerInterface::GetInstance().ShowGroupBottom( SUMMON_POSION_DISPLAY );
					}

				}
				break;
	
			case EMUSE_SUMMON_FB_FAIL_INVALIDZONE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSUMMON_FB_INVALIDZONE") );
				break;
			case EMUSE_SUMMON_FB_FAIL_NODATA:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSUMMON_FB_NODATA") );
				break;
			case EMUSE_SUMMON_FB_FAIL_VEHICLE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSUMMON_FB_VEHICLE") );
				break;
			case EMUSE_SUMMON_FB_FAIL_CONFRONT:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSUMMON_FB_CONFRONT") );
				break;

				/*skill summon, Juver, 2017/10/09 */
			case EMUSE_SUMMON_FB_FAIL_MAX:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSUMMON_FB_FULL_TOTAL") );
				break;

				/*skill summon, Juver, 2017/10/09 */
			case EMUSE_SUMMON_FB_FAIL_MAX_SKILL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSUMMON_FB_FULL_SKILL") );
				break;
			}
		}		
		break;

	case NET_MSG_REQ_USE_SUMMON_DEL:
		{
			/*skill summon, Juver, 2017/10/08 */
			GLMSG::SNETPC_REQ_USE_SUMMON_DEL* pNetMsg = ( GLMSG::SNETPC_REQ_USE_SUMMON_DEL* ) nmg;
			GLSummonClient* psummon_client = GetSummonClient( pNetMsg->dwGUID );
			if ( psummon_client )
				psummon_client->DeleteDeviceObjects();

			//m_Summon.DeleteDeviceObjects ();
	
		}break;
//----------------------------------------------------------------------------------------------------------------
// Message Processing about SUMMON (END)
//----------------------------------------------------------------------------------------------------------------
			//fastrevive fix
	case NET_MSG_GCTRL_DEATHTIMER_FB_AG:
		{
			GLMSG::SNETPC_REQ_DEATHTIMER_FB_AG *pNetMsg = (GLMSG::SNETPC_REQ_DEATHTIMER_FB_AG *) nmg;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("DEATH_TIMER_NOTIME") );
			CInnerInterface::GetInstance().HideGroup ( REBIRTH_DIALOGUE );
		}break;

	case NET_MSG_GCTRL_DEATHTIMER_FB_FLD:
		{
			GLMSG::SNETPC_REQ_REVIVE_DEATHTIMER_FB *pNetMsg = (GLMSG::SNETPC_REQ_REVIVE_DEATHTIMER_FB *) nmg;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("DEATH_TIMER_NOTIME") );
			CInnerInterface::GetInstance().HideGroup ( REBIRTH_DIALOGUE );
		}break;

	case NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY_FB:
		{
			GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_FB* pNetMsg = ( GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_FB* ) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_OK"), pNetMsg->fRate );
				break;

			case EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_FAIL") );
				break;

			case EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_INVALID_PASS:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_INVALID_PASS") );
				break;

			case EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_INVALID_RATE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_TYRANNY_FB_INVALID_RATE") );
				break;
			};
		}break;

	case NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR_FB:
		{
			GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FB* pNetMsg = ( GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_FB* ) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_OK"), pNetMsg->fRate );
				break;

			case EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_FAIL") );
				break;

			case EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_INVALID_PASS:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_INVALID_PASS") );
				break;

			case EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_INVALID_RATE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_SCHOOL_WAR_FB_INVALID_RATE") );
				break;
			};
		}break;

	case NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG_FB:
		{
			GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB* pNetMsg = ( GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB* ) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_OK"), pNetMsg->fRate );
				break;

			case EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_FAIL") );
				break;

			case EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_INVALID_PASS:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_INVALID_PASS") );
				break;

			case EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_INVALID_RATE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_FB_INVALID_RATE") );
				break;
			};
		}break;

	case NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_TYRANNY_BRD:
		{
			GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_BROADCAST* pNetMsg = ( GLMSG::SNETPC_GM_EVENT_CONTRI_TYRANNY_BROADCAST* ) nmg;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("CONTRI_EVENT_TYRANNY_BROAD_CAST"), pNetMsg->fRate );
			GLPVPTyrannyClient::GetInstance().SetContributionPointMultiplier( pNetMsg->fRate );
		}break;

	case NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_SCHOOL_WAR_BRD:
		{
			GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_BROADCAST* pNetMsg = ( GLMSG::SNETPC_GM_EVENT_CONTRI_SCHOOL_WAR_BROADCAST* ) nmg;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("CONTRI_EVENT_SCHOOL_WAR_BROAD_CAST"), pNetMsg->fRate );
			GLPVPSchoolWarsClient::GetInstance().SetContributionPointMultiplier( pNetMsg->fRate );
		}break;

	case NET_MSG_GCTRL_GM_COMMAND_EVENT_CONTRI_CAPTURE_THE_FLAG_BRD:
		{
			GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_BROADCAST* pNetMsg = ( GLMSG::SNETPC_GM_EVENT_CONTRI_CAPTURE_THE_FLAG_BROADCAST* ) nmg;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("CONTRI_EVENT_CAPTURE_THE_FLAG_BROAD_CAST"), pNetMsg->fRate );
			GLPVPCaptureTheFlagClient::GetInstance().SetContributionPointMultiplier( pNetMsg->fRate );
		}break;

	case NET_MSG_GCTRL_CLUB_SCORE_RANKING_START_A2C:
		{
			PVPClubWarsRankStart(nmg);
		}break;

	case NET_MSG_GCTRL_CLUB_SCORE_RANKING_UPDATE_A2C:
		{
			PVPClubWarsRankUpdate(nmg);
		}break;

	case NET_MSG_GCTRL_CLUB_SCORE_RANKING_END_A2C:
		{
			PVPClubWarsRankEnd(nmg);
		}break;

	case NET_MSG_GCTRL_GM_COMMAND_KICKOUT_FB:
		{
			GLMSG::SNETPC_GM_KICKOUT_FB* pNetMsg = ( GLMSG::SNETPC_GM_KICKOUT_FB* ) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMREQ_GM_KICKOUT_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_KICKOUT_FB_FAIL") );
				break;

			case EMREQ_GM_KICKOUT_FB_NOT_GM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_GM_KICKOUT_FB_NOT_GM") );
				break;

			case EMREQ_GM_KICKOUT_FB_WRONG_PASS:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_KICKOUT_FB_WRONG_PASS") );
				break;

			case EMREQ_GM_KICKOUT_FB_TIME_LOW:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMREQ_KICKOUT_FB_TIME_LOW"), RPARAM::fKickOutMinTime );
				break;

			case EMREQ_GM_KICKOUT_FB_TIME_HIGH:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_KICKOUT_FB_TIME_HIGH"), RPARAM::fKickOutMaxTime );
				break;

			case EMREQ_GM_KICKOUT_FB_DELETE_DONE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_KICKOUT_FB_DELETE_DONE"), pNetMsg->dwUserNum );
				break;

			case EMREQ_GM_KICKOUT_FB_DELETE_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GM_KICKOUT_FB_DELETE_FAIL"), pNetMsg->dwUserNum );
				break;

			case EMREQ_GM_KICKOUT_FB_UPDATE_DONE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_KICKOUT_FB_UPDATE_DONE"), pNetMsg->dwUserNum, pNetMsg->fTime );
				break;

			case EMREQ_GM_KICKOUT_FB_ADDED_DONE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_KICKOUT_FB_ADDED_DONE"), pNetMsg->dwUserNum, pNetMsg->fTime );
				break;
			};
		}break;

		/* party finder, Juver, 2020/01/04 */
	case NET_MSG_GCTRL_PARTY_FINDER_SEARCH_FB:
		{
			GLMSG::SNETPC_PARTY_FINDER_SEARCH_FB* pNetMsg = ( GLMSG::SNETPC_PARTY_FINDER_SEARCH_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMPARTY_FINDER_SEARCH_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_FAIL") );
				break;

			case EMPARTY_FINDER_SEARCH_FB_FUNCTION_DISABLED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_FUNCTION_DISABLED") );
				break;

			case EMPARTY_FINDER_SEARCH_FB_INVALID_TYPE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_INVALID_TYPE") );
				break;

			case EMPARTY_FINDER_SEARCH_FB_REQUIRE_TIME:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_REQUIRE_TIME") );
				break;

			case EMPARTY_FINDER_SEARCH_FB_START_SEARCH:
				{
					common_vector_free( m_vecPartyFinderResult );

					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPARTY_FINDER_SEARCH_FB_START_SEARCH") );
					
				}break;
				

			};
		}break;

		/* party finder, Juver, 2020/01/11 */
	case NET_MSG_GCTRL_PARTY_FINDER_RESULT_UPDATE:
		{
			GLMSG::SNETPC_PARTY_FINDER_RESULT_UPDATE* pNetMsg = ( GLMSG::SNETPC_PARTY_FINDER_RESULT_UPDATE* ) nmg;
			for ( WORD i=0; i<pNetMsg->wResultNum; ++i )
			{
				m_vecPartyFinderResult.push_back( pNetMsg->sResultData[i] );
			}
		}break;

		/* party finder, Juver, 2020/01/11 */
	case NET_MSG_GCTRL_PARTY_FINDER_RESULT_DONE:
		{
			GLMSG::SNETPC_PARTY_FINDER_RESULT_DONE* pNetMsg = ( GLMSG::SNETPC_PARTY_FINDER_RESULT_DONE* ) nmg;
			CInnerInterface::GetInstance().RefreshPartyFinderWindow();

		}break;

		/* party finder, Juver, 2020/01/16 */
	case NET_MSG_GCTRL_PARTY_FINDER_JOIN_FB:
		{
			GLMSG::SNETPC_PARTY_FINDER_JOIN_FB* pNetMsg = ( GLMSG::SNETPC_PARTY_FINDER_JOIN_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMPARTY_FINDER_JOIN_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_FAIL") );
				break;

			case EMPARTY_FINDER_JOIN_FB_FUNCTION_DISABLED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_FUNCTION_DISABLED") );
				break;

			case EMPARTY_FINDER_JOIN_FB_INVALID_PARTY:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_INVALID_PARTY") );
				break;

			case EMPARTY_FINDER_JOIN_FB_REQUIRE_TIME:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_REQUIRE_TIME") );
				break;

			case EMPARTY_FINDER_JOIN_FB_PARTY_PRIVATE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_PARTY_PRIVATE") );
				break;

			case EMPARTY_FINDER_JOIN_FB_PARTY_FULL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_PARTY_FULL") );
				break;

			case EMPARTY_FINDER_JOIN_FB_IN_PARTY:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_IN_PARTY") );
				break;

			case EMPARTY_FINDER_JOIN_FB_DECLINE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_DECLINE") );
				break;


			case EMPARTY_FINDER_JOIN_FB_CLUB_BATTLE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_CLUB_BATTLE") );
				break;

			case EMPARTY_FINDER_JOIN_FB_MAP_PARTY_BLOCK_LEADER:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_MAP_PARTY_BLOCK_LEADER") );
				break;

			case EMPARTY_FINDER_JOIN_FB_MAP_PARTY_BLOCK_MEMBER:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_MAP_PARTY_BLOCK_MEMBER") );
				break;

			case EMPARTY_FINDER_JOIN_FB_REQUEST_SENT:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPARTY_FINDER_JOIN_FB_REQUEST_SENT") );
				break;

			};
		}break;

		/* party finder, Juver, 2020/01/16 */
	case NET_MSG_GCTRL_PARTY_FINDER_QUESTION:
		{
			GLMSG::SNETPC_PARTY_FINDER_QUESTION* pNetMsg = ( GLMSG::SNETPC_PARTY_FINDER_QUESTION* ) nmg;
			CInnerInterface::GetInstance().OpenPartyFinderQuestion( &pNetMsg->sQuestion );
		}break;

	case NET_MSG_GCTRL_GM_PCID_FB:
		{
			GLMSG::SNETPC_GM_PCID_FB* pNetMsg = ( GLMSG::SNETPC_GM_PCID_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMREQ_GM_PCID_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_FAIL") );
				break;

			case EMREQ_GM_PCID_FB_NOT_GM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_NOT_GM") );
				break;

			case EMREQ_GM_PCID_FB_WRONG_PASS:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_WRONG_PASS") );
				break;

			case EMREQ_GM_PCID_FB_INVALID_TARGET:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_INVALID_TARGET") );
				break;

			case EMREQ_GM_PCID_FB_REPLY:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szPCID_HWID );
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szPCID_MAC );
				}break;
			};
		}break;

	case NET_MSG_GCTRL_GM_PCID_FB2:
		{
			GLMSG::SNETPC_GM_PCID_FB2* pNetMsg = ( GLMSG::SNETPC_GM_PCID_FB2* ) nmg;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szBiosUUID );
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szBoardModel );
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szBoardSerial );
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szCPUVersion );
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szCPUID );
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szMAC );

			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szPCID_HWID );
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_PCID_FB_REPLY"), pNetMsg->szPCID_MAC );
		}break;

	case NET_MSG_GCTRL_GM_PCID_TO_CHAR:
		{
			GLMSG::SNETPC_GM_PCID_TO_CHAR* pNetMsg = ( GLMSG::SNETPC_GM_PCID_TO_CHAR* ) nmg;

	

			if ( pNetMsg->dwType == 0 )
			{
				GLMSG::SNETPC_GM_PCID_REPLY2 netMsgAgent;
				netMsgAgent.dwType = pNetMsg->dwType;
				netMsgAgent.dwCharIDSource = pNetMsg->dwCharIDSource;
				netMsgAgent.dwCharIDTarget = pNetMsg->dwCharIDTarget;

				SPCID_SMBIOS sPCID_SMBIOS;
				RPCID::getSMBIOS( sPCID_SMBIOS );
				::StringCchCopy(netMsgAgent.szBiosUUID, PCID_LENGTH, sPCID_SMBIOS.strSystemUUID.c_str() );
				::StringCchCopy(netMsgAgent.szBoardModel, PCID_LENGTH, sPCID_SMBIOS.strBoardModel.c_str() );
				::StringCchCopy(netMsgAgent.szBoardSerial, PCID_LENGTH, sPCID_SMBIOS.strBoardSerialNumber.c_str() );
				::StringCchCopy(netMsgAgent.szCPUVersion, PCID_LENGTH, sPCID_SMBIOS.strProcessorVersion.c_str() );
				::StringCchCopy(netMsgAgent.szCPUID, PCID_LENGTH, sPCID_SMBIOS.strProcessorID.c_str() );
				::StringCchCopy(netMsgAgent.szMAC, PCID_LENGTH, RPCID::getWindowsMAC().c_str() );

				::StringCchCopy(netMsgAgent.szPCID_HWID, PCID_LENGTH, RPCID::getHashSMBIOS(sPCID_SMBIOS).c_str() );
				::StringCchCopy(netMsgAgent.szPCID_MAC, PCID_LENGTH, RPCID::getHashWindowsMAC().c_str() );
				NETSEND( &netMsgAgent );
			}
			else
			{
				GLMSG::SNETPC_GM_PCID_REPLY netMsgAgent;
				netMsgAgent.dwType = pNetMsg->dwType;
				netMsgAgent.dwCharIDSource = pNetMsg->dwCharIDSource;
				netMsgAgent.dwCharIDTarget = pNetMsg->dwCharIDTarget;

				SPCID_SMBIOS sPCID_SMBIOS;
				RPCID::getSMBIOS( sPCID_SMBIOS );
				::StringCchCopy(netMsgAgent.szPCID_HWID, PCID_LENGTH, RPCID::getHashSMBIOS(sPCID_SMBIOS).c_str() );
				::StringCchCopy(netMsgAgent.szPCID_MAC, PCID_LENGTH, RPCID::getHashWindowsMAC().c_str() );
				NETSEND( &netMsgAgent );
			}
			


		}break;

		/* user flag restricted, Juver, 2020/04/21 */
	case NET_MSG_GCTRL_GM_USER_RESTRICT_FB:
		{
			GLMSG::SNETPC_GM_USER_RESTRICT_FB* pNetMsg = ( GLMSG::SNETPC_GM_USER_RESTRICT_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMREQ_GM_USER_RESTRICT_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("EMREQ_GM_USER_RESTRICT_FB_FAIL") );
				break;

			case EMREQ_GM_USER_RESTRICT_FB_NOT_GM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("EMREQ_GM_USER_RESTRICT_FB_NOT_GM") );
				break;

			case EMREQ_GM_USER_RESTRICT_FB_WRONG_PASS:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("EMREQ_GM_USER_RESTRICT_FB_WRONG_PASS") );
				break;

			case EMREQ_GM_USER_RESTRICT_FB_INVALID_TARGET:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("EMREQ_GM_USER_RESTRICT_FB_INVALID_TARGET") );
				break;

			case EMREQ_GM_USER_RESTRICT_FB_DONE:
				{
					if ( pNetMsg->bUserRestricted )
						CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_USER_RESTRICT_FB_DONE_1") );
					else
						CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GM_USER_RESTRICT_FB_DONE_2") );
				}break;
			};
		}break;

		/* pvp club death match, Juver, 2020/11/26 */
	case NET_MSG_GCTRL_PVP_CLUB_DEATH_MATCH_F2AC_REVIVE_REQ_FB:	
		PVPClubDeathMatchReBirthFB(nmg);		
		break;

		/* variable check, Juver, 2021/07/02 */
	case NET_MSG_GCTRL_VARIABLE_CHECK_CLIENT:
		{
			m_Character.VariableCheck();
		}break;

	case NET_MSG_GTRL_PKMARK_BRD:
		{
			GLMSG::SNET_F2C_PKMARK_BRD* pNetMsg = ( GLMSG::SNET_F2C_PKMARK_BRD* ) nmg;
			GLPKMark::GetInstance().AddData( pNetMsg->sData );
		}break;

	case NET_MSG_GCTRL_PVP_PBG_F2AC_REVIVE_REQ_FB:
		PVPPBGReBirthFB(nmg);	
		break;

	

	default:
		CDebugSet::ToListView ( "GLGaeaClient::MsgProcess() TYPE[%d] massage leak", nmg->nType );
		break;
	};
}

GLCOPY* GLGaeaClient::GetCopyActor ( const EMCROW emCrow, const DWORD dwID )
{
	if ( emCrow == CROW_PC )
	{
		if ( GetCharacter()->m_dwGaeaID == dwID )		return GetCharacter();
		else if ( dwID != EMTARGET_NULL )				return m_pLandMClient->GetChar ( dwID );
	}
	/* crow zone, Juver, 2018/02/21 */
	else if ( emCrow == CROW_NPC || emCrow == CROW_MOB || emCrow == CROW_ZONE_NAME || emCrow == CROW_GATE_NAME )
	{
		if ( dwID != EMTARGET_NULL )					return m_pLandMClient->GetCrow ( dwID );
		else											return NULL;
	}
	else if ( emCrow == CROW_MATERIAL )
	{
		if ( dwID != EMTARGET_NULL )					return m_pLandMClient->GetMaterial ( dwID );
		else											return NULL;
	}
	else if ( emCrow == CROW_SUMMON )
	{
		//if ( GetSummonClient()->m_dwGUID == dwID )		return GetSummonClient();

		/*skill summon, Juver, 2017/10/08 */
		GLSummonClient*	psummon_client = GetSummonClient( dwID );
		if( psummon_client )	return psummon_client;

		if ( dwID != EMTARGET_NULL )					return m_pLandMClient->GetSummon ( dwID );
		else											return NULL;
	}
	else if ( !m_bBRIGHTEVENT )
	{
       //GASSERT(0&&"emCrow가 잘못된 지정자 입니다.");
	}

	return NULL;
}

GLCOPY* GLGaeaClient::GetCopyActor ( const std::string &strName )
{
	if ( strName == GetCharacter()->m_szName )		return GetCharacter();
	else											return m_pLandMClient->FindChar ( strName );

	return NULL;
};

D3DXVECTOR3* GLGaeaClient::FindCharPos ( std::string strName )
{
	if ( strName==std::string(GetCharacter()->GetCharData().m_szName) )
	{
		m_vCharPos = GetCharacter()->GetPosition();
		return &m_vCharPos;
	}

	PGLCHARCLIENT pCharClient = GetActiveMap()->FindChar ( strName );
	if ( !pCharClient )	return NULL;

	m_vCharPos = pCharClient->GetPosition();

	return &m_vCharPos;
}

D3DXVECTOR3* GLGaeaClient::FindCharHeadPos ( std::string strName )
{
	if ( strName==std::string(GetCharacter()->GetCharData().m_szName) )
	{
		m_vCharPos = GetCharacter()->GetPosBodyHeight();

		return &m_vCharPos;
	}

	PGLCHARCLIENT pCharClient = GetActiveMap()->FindChar ( strName );
	if ( !pCharClient )	return NULL;

	m_vCharPos = pCharClient->GetPosBodyHeight();

	return &m_vCharPos;
}

D3DXVECTOR3* GLGaeaClient::FindCharHeadPos ( DWORD dwGaeaID ) 
{
	if ( dwGaeaID == GetCharacter()->m_dwGaeaID )
	{
		m_vCharPos = GetCharacter()->GetPosBodyHeight();

		return &m_vCharPos;
	}

	PGLCHARCLIENT pCharClient = GetActiveMap()->FindChar ( dwGaeaID );
	if ( !pCharClient )	return NULL;

	m_vCharPos = pCharClient->GetPosBodyHeight();

	return &m_vCharPos;
}

D3DXVECTOR3* GLGaeaClient::FindMaterialHeadPos ( DWORD dwGaeaID ) 
{
	PGLMATERIALCLIENT pMaterialClient = GetActiveMap()->GetMaterial ( dwGaeaID );
	if ( !pMaterialClient )	return NULL;

	m_vCharPos = pMaterialClient->GetPosBodyHeight();

	return &m_vCharPos;
}

void GLGaeaClient::GetConvertServerTime( CTime& cTime )
{
	tm t1, t2;
	cTime.GetLocalTm(&t1);
	cTime.GetGmtTm(&t2);

	__time64_t tLocalTime = _mktime64( &t1 );
	__time64_t tUtcTime = _mktime64( &t2 );;
	__time64_t tSpan = tLocalTime - tUtcTime;

	int nTime = (int)(tSpan / 3600); 
	nTime = nTime - m_nServerTimeBias;	// 서버타임과 중복 계산

	if ( nTime == 0 )
		return;

	CTimeSpan cTimeSpan( 0, -nTime, 0, 0 );
	cTime += cTimeSpan;
}

DxSkinChar* GLGaeaClient::GetSkinChar ( const STARGETID &sTargetID )
{
	if ( !m_pLandMClient )	return NULL;

	//	Note : 타겟의 위치 정보를 가져옴.
	if ( sTargetID.emCrow == CROW_PC )
	{
		if ( GetCharacter()->m_dwGaeaID == sTargetID.dwID )	return GetCharacter()->GetSkinChar();

		PGLCHARCLIENT pChar = m_pLandMClient->GetChar ( sTargetID.dwID );
		if ( pChar )	return pChar->GetCharSkin();
	}
	/* crow zone, Juver, 2018/02/21 */
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB || sTargetID.emCrow == CROW_ZONE_NAME || sTargetID.emCrow == CROW_GATE_NAME )
	{
		PGLCROWCLIENT pCrow = m_pLandMClient->GetCrow ( sTargetID.dwID );
		if ( pCrow )	return pCrow->GetCharSkin();
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL )
	{
		PGLMATERIALCLIENT pMaterial = m_pLandMClient->GetMaterial ( sTargetID.dwID );
		if ( pMaterial )	return pMaterial->GetCharSkin();
	}
	else if ( sTargetID.emCrow == CROW_PET )
	{
		if ( GetPetClient()->m_dwGUID == sTargetID.dwID ) return GetPetClient()->GetSkinChar ();

		PGLANYPET pAnyPet = m_pLandMClient->GetPet ( sTargetID.dwID );
		if ( pAnyPet ) return pAnyPet->GetSkinChar ();
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		//if ( GetSummonClient()->m_dwGUID == sTargetID.dwID ) return GetSummonClient()->GetSkinChar ();

		/*skill summon, Juver, 2017/10/08 */
		GLSummonClient*	psummon_client = GetSummonClient( sTargetID.dwID );
		if( psummon_client )	return psummon_client->GetSkinChar ();

		PGLANYSUMMON pAnySummon = m_pLandMClient->GetSummon ( sTargetID.dwID );
		if ( pAnySummon ) return pAnySummon->GetSkinChar ();
	}
	else
	{
		//GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );
		CDebugSet::ToLogFile( "GLGaeaClient::GetSkinChar() emCrow가 잘못된 지정자 입니다." );
	}
		

	return NULL;
}

float GLGaeaClient::GetCrowDir ( const STARGETID &sTargetID )
{
	if ( !m_pLandMClient )	return 0.0f;

	//	Note : 타겟의 위치 정보를 가져옴.
	if ( sTargetID.emCrow == CROW_PC )
	{
		if ( GetCharacter()->m_dwGaeaID == sTargetID.dwID )	return GetCharacter()->GetDirection();

		PGLCHARCLIENT pChar = m_pLandMClient->GetChar ( sTargetID.dwID );
		if ( pChar ) return pChar->GetDirection();
	}
	/* crow zone, Juver, 2018/02/21 */
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB || sTargetID.emCrow == CROW_ZONE_NAME || sTargetID.emCrow == CROW_GATE_NAME )
	{
		PGLCROWCLIENT pCrow = m_pLandMClient->GetCrow ( sTargetID.dwID );
		if ( pCrow ) return pCrow->GetDirection();
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL )
	{
		PGLMATERIALCLIENT pMaterial = m_pLandMClient->GetMaterial ( sTargetID.dwID );
		if ( pMaterial ) return pMaterial->GetDirection();
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		//if ( GetSummonClient()->m_dwGUID == sTargetID.dwID ) return GetSummonClient()->GetDirection ();

		/*skill summon, Juver, 2017/10/08 */
		GLSummonClient*	psummon_client = GetSummonClient( sTargetID.dwID );
		if( psummon_client )	return psummon_client->GetDirection ();

		PGLANYSUMMON pAnySummon = m_pLandMClient->GetSummon ( sTargetID.dwID );
		if ( pAnySummon ) return pAnySummon->GetDirection();
	}
	else
		GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );

	return 0.0f;
}

DWORD GLGaeaClient::GetMobNameColor ( DWORD dwGlobID )
{
	DWORD dwCOLOR(CROWCOLOR::MOB_COLOR_GRAY);

	PGLCROWCLIENT pCROW = GetActiveMap()->GetCrow ( dwGlobID );
	if ( !pCROW )	return dwCOLOR;

	/* crow zone, Juver, 2018/02/21 */
	if ( pCROW->m_pCrowData->m_sBasic.m_emCrow == CROW_NPC || pCROW->m_pCrowData->m_sBasic.m_emCrow == CROW_ZONE_NAME || pCROW->m_pCrowData->m_sBasic.m_emCrow == CROW_GATE_NAME )
	{
		dwCOLOR = CROWCOLOR::MOB_COLOR_GRAY;
	}
	else
	{
		WORD wMY_LEVEL = m_Character.GETLEVEL();
		WORD wMOB_LEVEL = pCROW->m_pCrowData->m_sBasic.m_wLevel;
		float fEXP_RATE = GLCONST_CHAR::GETEXP_RATE ( wMY_LEVEL, wMOB_LEVEL );

		if ( fEXP_RATE==0 )		dwCOLOR = CROWCOLOR::MOB_COLOR_GRAY;
		else					dwCOLOR = CROWCOLOR::MOB_COLOR_RED;

		/*crow school, Juver, 2018/08/02 */
		if ( pCROW->m_pCrowData->m_sBasic.m_emCrowSchool != EMCROW_SCHOOL_SIZE && 
			m_Character.GetCharData().m_wSchool == pCROW->m_pCrowData->m_sBasic.m_emCrowSchool )
		{
			dwCOLOR = NS_UITEXTCOLOR::BRIGHTGREEN;
		}
	}

	return dwCOLOR;
}

BOOL GLGaeaClient::IsMapCollsion( D3DXVECTOR3& vTargetPt, D3DXVECTOR3& vFromPt )
{
	if( !m_pLandMClient )	return FALSE;

	DxLandMan* pLandMan = m_pLandMClient->GetLandMan();
	if( !pLandMan )			return FALSE;

	LPDXFRAME	pFrame(NULL);
	BOOL		bCollision(FALSE);
	D3DXVECTOR3 vCollision( 0.f, 0.f, 0.f );
	pLandMan->IsCollision( vTargetPt, vFromPt, vCollision, bCollision, pFrame, FALSE );

	return bCollision;
}

void GLGaeaClient::PetSkinPackApplyEffect()
{

	if( m_Pet.IsVALID() )
	{
		// 생성 효과
		D3DXMATRIX matEffect;
		D3DXVECTOR3 vPos = m_Pet.GetPosition ();
		D3DXMatrixTranslation ( &matEffect, vPos.x, vPos.y, vPos.z );

		std::string strGEN_EFFECT = GLCONST_CHAR::strPET_GEN_EFFECT.c_str();
		STARGETID sTargetID(CROW_PET,m_Pet.m_dwGUID,vPos);
		DxEffGroupPlayer::GetInstance().NewEffGroup( strGEN_EFFECT.c_str(), matEffect, &sTargetID );				
		m_Pet.SkinLoad( m_pd3dDevice );

		// Note : 1.AABB Box를 가져온다. 2.높이를 계산해 놓는다.
		m_Pet.GetSkinChar()->GetAABBBox( m_Pet.m_vMaxOrg, m_Pet.m_vMinOrg );
		m_Pet.m_fHeight = ( m_Pet.m_vMaxOrg.y - m_Pet.m_vMinOrg.y ) * m_Pet.m_sPetSkinPackState.fPetScale;

		if( !m_Pet.IsUsePetSkinPack() ) m_Pet.UpdateSuit ( TRUE );
	}

}

HRESULT GLGaeaClient::CreateSummon( NET_MSG_GENERIC* nmg )
{
	GLMSG::SNETPC_REQ_USE_SUMMON_FB* pNetMsg = ( GLMSG::SNETPC_REQ_USE_SUMMON_FB* ) nmg;

	/*skill summon, Juver, 2017/10/08 */
	GLSummonClient* psummon_client = GetSummonClient( pNetMsg->dwGUID );
	if ( psummon_client )
	{
		CDebugSet::ToFileWithTime( "summon.txt", "%d id exist", pNetMsg->dwGUID );
		return E_FAIL;
	}

	/*skill summon, Juver, 2017/10/08 */
	if ( pNetMsg->wIndex >= SKILL_SUMMON_MAX_CLIENT_NUM )
	{
		CDebugSet::ToFileWithTime( "summon.txt", "summon maxed out" );
		return E_FAIL;
	}

	m_Summon[pNetMsg->wIndex].DeleteDeviceObjects();

	GLSUMMONLOGIC_CLIENT sSummon;
	sSummon.m_emTYPE	 = pNetMsg->emTYPE;
	sSummon.m_dwGUID	 = pNetMsg->dwGUID;
	sSummon.m_sSummonID  = pNetMsg->sSummonID;
	sSummon.m_dwOwner	 = pNetMsg->dwOwner;	
	sSummon.m_sMapID	 = pNetMsg->sMapID;
	sSummon.m_dwCellID   = pNetMsg->dwCellID;
	sSummon.m_dwNowHP	 = pNetMsg->dwNowHP;
	sSummon.m_wNowMP	 = pNetMsg->wNowMP;

	/*skill summon, Juver, 2017/10/09 */
	sSummon.m_wArrayIndex = pNetMsg->wIndex;		
	sSummon.m_Summon      = pNetMsg->sSummon;

	sSummon.m_fRunArea			= pNetMsg->fRunArea;
	sSummon.m_fWalkArea			= pNetMsg->fWalkArea;
	sSummon.m_fOwnerDistance	= pNetMsg->fOwnerDistance;

	/*skill summon, Juver, 2017/10/08 */
	HRESULT hr = m_Summon[pNetMsg->wIndex].Create ( &sSummon, pNetMsg->vPos, pNetMsg->vDir, m_pLandMClient->GetNaviMesh(), m_pd3dDevice );

	// 실패처리 ( 서버에 생성된 팻을 삭제하도록 )
	if ( FAILED(hr) )
	{
		return S_FALSE;
	}

	// 생성 효과
	D3DXMATRIX matEffect;
	D3DXVECTOR3 vPos = m_Summon[pNetMsg->wIndex].GetPosition ();
	D3DXMatrixTranslation ( &matEffect, vPos.x, vPos.y, vPos.z );

	std::string strGEN_EFFECT = GLCONST_CHAR::strPET_GEN_EFFECT.c_str();
	STARGETID sTargetID(CROW_PET,m_Summon[pNetMsg->wIndex].m_dwGUID,vPos);
	DxEffGroupPlayer::GetInstance().NewEffGroup( strGEN_EFFECT.c_str(), matEffect, &sTargetID );

	return S_OK;

}

HRESULT GLGaeaClient::ReqClubDeathMatchInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();
	if ( !pLand || !pLand->m_bClubDeathMatch ) return E_FAIL;

	GLMSG::SNET_CLUB_DEATHMATCH_RANKING_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD ( &NetMsg );

	return TRUE;
}

/*skill summon, Juver, 2017/10/08 */
GLSummonClient* GLGaeaClient::GetSummonClient( DWORD dwguid )
{
	for ( int i=0; i<SKILL_SUMMON_MAX_CLIENT_NUM; ++i )
	{
		if ( m_Summon[i].m_dwGUID == dwguid )
		{
			return &m_Summon[i];
			break;
		}
	}

	return NULL;
}

///*character render setting, Juver, 2019/03/23 */
//bool GLGaeaClient::IsChangeRenderReady()
//{
//	if ( m_fRenderSettingChangeTimer < RPARAM::fRenderSettingChangeTime )
//		return false;
//
//	return true;
//}
//
///*character render setting, Juver, 2019/03/22 */
//void GLGaeaClient::ChangeRenderSettings()
//{
//	if ( !IsChangeRenderReady() )	return;
//
//	GLCHARCLIENTLIST *pCharList = m_pLandMClient->GetCharList ();
//	GLCHARCLIENTNODE *pCharCur = pCharList->m_pHead;
//	for ( ; pCharCur; pCharCur = pCharCur->pNext )
//	{
//		PGLCHARCLIENT pChar = pCharCur->Data;
//		if ( !pChar )	continue;
//		pChar->UpdateSuit();
//	}
//
//	m_Character.UpdateSuit();
//
//	m_fRenderSettingChangeTimer = 0.0f;
//}

void GLGaeaClient::UpdateSuitClientActor()
{
	GLCHARCLIENTLIST *pCharList = m_pLandMClient->GetCharList ();
	GLCHARCLIENTNODE *pCharCur = pCharList->m_pHead;
	for ( ; pCharCur; pCharCur = pCharCur->pNext )
	{
		PGLCHARCLIENT pChar = pCharCur->Data;
		if ( !pChar )	continue;
		pChar->UpdateSuit();
	}

	//m_Character.UpdateSuit();
}

bool GLGaeaClient::IsBRIGHTEVENT()
{
	if( GetActiveMap()->GetMapID() == m_sBrightEventMap && m_bBRIGHTEVENT ) return true;
	return false;
}
HRESULT GLGaeaClient::ReqWoePlayerRankingInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();
	if (!pLand || !pLand->m_bPVPWoeMap) return E_FAIL;

	GLMSG::SNET_WOE_PLAYER_RANKING_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD(&NetMsg);

	return TRUE;
}
HRESULT GLGaeaClient::ReqWoeGuildRankingInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();
	if (!pLand || !pLand->m_bPVPWoeMap) return E_FAIL;

	GLMSG::SNET_WOE_GUILD_RANKING_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD(&NetMsg);

	return TRUE;
}
HRESULT GLGaeaClient::ReqWoeResuRankingInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();
	if (!pLand || !pLand->m_bPVPWoeMap) return E_FAIL;

	GLMSG::SNET_WOE_RESU_RANKING_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD(&NetMsg);

	return TRUE;
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
HRESULT GLGaeaClient::ReqTyrannyClubRankInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();
	if ( !pLand || !pLand->m_bPVPTyrannyMap ) return E_FAIL;

	GLMSG::SNET_TYRANNY_CLUB_RANKING_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD ( &NetMsg );

	return TRUE;
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
HRESULT GLGaeaClient::ReqTyrannyRankInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();
	if ( !pLand || !pLand->m_bPVPTyrannyMap ) return E_FAIL;

	GLMSG::SNET_TYRANNY_RANKING_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD ( &NetMsg );

	return TRUE;
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
HRESULT GLGaeaClient::ReqTyrannyRankResuInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();
	if ( !pLand || !pLand->m_bPVPTyrannyMap ) return E_FAIL;

	GLMSG::SNET_TYRANNY_RANKING_RESU_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD ( &NetMsg );

	return TRUE;
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
HRESULT GLGaeaClient::ReqClubWarClubRankInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();

	GLMSG::SNET_CLUBWAR_CLUB_RANKING_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD ( &NetMsg );

	return TRUE;
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
HRESULT GLGaeaClient::ReqClubWarRankInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();

	GLMSG::SNET_CLUBWAR_RANKING_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD ( &NetMsg );

	return TRUE;
}

/* Tyranny/Clubwar Mini Ranking, Montage 3-22-25 */
HRESULT GLGaeaClient::ReqClubWarRankResuInfo()
{
	PLANDMANCLIENT pLand = GetActiveMap();

	GLMSG::SNET_CLUBWAR_RANKING_RESU_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetMapID().dwID;
	NETSENDTOFIELD ( &NetMsg );

	return TRUE;
}