// DlgCharset.cpp : implementation file
//

#include "stdafx.h"
#include "./GameEmulator.h"
#include "./DlgCharset.h"
#include "./EmulatorData.h"

#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "../Lib_Client/G-Logic/GLogicData.h"
#include "../Lib_Network/s_NetGlobal.h"
#include "../Lib_Engine/G-Logic/GLogic.h"

#include "../Lib_Client/G-Logic/UserTypeDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDlgCharset dialog

IMPLEMENT_DYNAMIC(CDlgCharset, CDialog)

CDlgCharset::CDlgCharset(CWnd* pParent /*=NULL*/, SCharacterSetting* pCharSetting /*= NULL*/)
	: CDialog(CDlgCharset::IDD, pParent)
	, m_pCharacterSetting( pCharSetting )
{

}

CDlgCharset::~CDlgCharset()
{
}

void CDlgCharset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCharset, CDialog)
	ON_BN_CLICKED(IDC_BTN_CHARSET, OnBnClickedBtnCharset)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_LEVEL_MAX, &CDlgCharset::OnBnClickedButtonLevelMax)
	ON_BN_CLICKED(IDC_BUTTON_STATS_10K, &CDlgCharset::OnBnClickedButtonStats10k)
	ON_BN_CLICKED(IDC_BUTTON_MONEY_1B, &CDlgCharset::OnBnClickedButtonMoney1b)
	ON_BN_CLICKED(IDC_BUTTON_ACTIVITY_10K, &CDlgCharset::OnBnClickedButtonActivity10k)
	ON_BN_CLICKED(IDC_BUTTON_CONTRIBUTION_10K, &CDlgCharset::OnBnClickedButtonContribution10k)
	ON_BN_CLICKED(IDC_BUTTON_WAR_CHIPS_10K, &CDlgCharset::OnBnClickedButtonWarChips10k)
	ON_BN_CLICKED(IDC_BUTTON_GAME_POINTS_10K, &CDlgCharset::OnBnClickedButtonGamePoints10k)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_TIME_10K, &CDlgCharset::OnBnClickedButtonPlayTime10k)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_POINT_10K, &CDlgCharset::OnBnClickedButtonPlayPoint10k)
	ON_BN_CLICKED(IDC_BUTTON_REBORN_100, &CDlgCharset::OnBnClickedButtonReborn100)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_P, &CDlgCharset::OnBnClickedButtonSkillP)
	ON_BN_CLICKED(IDC_BUTTON_STATS_P, &CDlgCharset::OnBnClickedButtonStatsP)
	ON_BN_CLICKED(IDC_BUTTON_EXCHANGE_ITEM_P, &CDlgCharset::OnBnClickedButtonExchangeItemP)
END_MESSAGE_MAP()


// CDlgCharset message handlers
BOOL CDlgCharset::OnInitDialog()
{
	CDialog::OnInitDialog();


	SetWin_Combo_Init( this, IDC_COMBO_USERTYPE, COMMENT::USER_TYPE, NSUSER_TYPE::USER_TYPE_INDEX_SIZE );

	SetWin_Combo_Init( this, IDC_COMBO_VIP_LEVEL, COMMENT::CHAR_VIP, EMVIP_LEVEL_SIZE );

	if ( m_pCharacterSetting )
	{
		SetWin_Text( this, IDC_EDIT_CHARSET, m_pCharacterSetting->strCharSet.c_str() );

		SetWin_Combo_Sel( this, IDC_COMBO_USERTYPE, COMMENT::USER_TYPE[m_pCharacterSetting->dwUserLevelIndex].c_str() );

		SetWin_Text( this, IDC_EDIT_CHAR_NAME, m_pCharacterSetting->strCharName.c_str() );
		SetWin_Num_uint( this, IDC_EDIT_CHAR_LEVEL, m_pCharacterSetting->wLevel );
		SetWin_Num_uint( this, IDC_EDIT_CHAR_STATS, m_pCharacterSetting->wStats );
		SetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_MONEY, m_pCharacterSetting->llMoney );
		SetWin_Num_uint( this, IDC_EDIT_CHAR_ACTIVITY_P, m_pCharacterSetting->dwActivityPoints );
		SetWin_Num_uint( this, IDC_EDIT_CHAR_CONTRI_P, m_pCharacterSetting->dwContributionPoints );
		SetWin_Num_uint( this, IDC_EDIT_CHAR_WAR_CHIPS, m_pCharacterSetting->dwWarChips );
		SetWin_Num_uint( this, IDC_EDIT_CHAR_GAME_POINTS, m_pCharacterSetting->dwGamePoints );

		SetWin_Checkb( this, IDC_CHK_OPEN_SKILLS, m_pCharacterSetting->bOpenSkills );
		SetWin_Checkb( this, IDC_CHK_OPEN_INVENTORY, m_pCharacterSetting->bOpenInventory );

		/* user flag verified, Juver, 2020/02/25 */
		SetWin_Checkb( this, IDC_CHK_USER_VERIFIED, m_pCharacterSetting->bUserVerified );

		/* user flag restricted, Juver, 2020/04/20 */
		SetWin_Checkb( this, IDC_CHK_USER_RESTRICTED, m_pCharacterSetting->bUserRestricted );

		/* play time system, Juver, 2021/01/26 */
		SetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_PLAY_TIME, m_pCharacterSetting->llChaPlayTime );
		SetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_PLAY_POINT, m_pCharacterSetting->llChaPlayPoint );

		/* reborn system, Juver, 2021/09/17 */
		SetWin_Num_uint( this, IDC_EDIT_CHAR_REBORN, m_pCharacterSetting->dwReborn );

		SetWin_Num_uint( this, IDC_EDIT_CHAR_SKILL_P, m_pCharacterSetting->wSkillPoints );

		SetWin_Num_uint( this, IDC_EDIT_CHAR_STATS_P, m_pCharacterSetting->wStatPoints );

		SetWin_Combo_Sel( this, IDC_COMBO_VIP_LEVEL, COMMENT::CHAR_VIP[m_pCharacterSetting->emVIP].c_str() );
		SetWin_Num_uint( this, IDC_EDIT_EXCHANGE_ITEM_P, m_pCharacterSetting->wExchangeItemPoints );
	}

	
	return TRUE;
}

void CDlgCharset::OnBnClickedBtnCharset()
{
	CString szFilter = "charset file  |*.charset|";
	CFileDialog dlg ( TRUE, ".charset", GLOGIC::GetPath(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		if ( m_pCharacterSetting )
		{
			m_pCharacterSetting->strCharSet = dlg.GetFileName().GetString();	
			SetWin_Text( this, IDC_EDIT_CHARSET, m_pCharacterSetting->strCharSet.c_str() );
		}
	}
}

void CDlgCharset::OnBnClickedOk()
{
	if ( m_pCharacterSetting )
	{
		std::string strCharSet = GetWin_Text( this, IDC_EDIT_CHARSET );
		if ( strCharSet.size() )
			m_pCharacterSetting->strCharSet = strCharSet;

		m_pCharacterSetting->dwUserLevelIndex = GetWin_Combo_Sel( this, IDC_COMBO_USERTYPE );

		std::string strChaName = GetWin_Text( this, IDC_EDIT_CHAR_NAME );
		if ( strChaName.size() )
			m_pCharacterSetting->strCharName = strChaName.c_str();

		m_pCharacterSetting->wLevel = GetWin_Num_uint( this, IDC_EDIT_CHAR_LEVEL );

		if ( m_pCharacterSetting->wLevel < 1 ) 
			m_pCharacterSetting->wLevel = 1;

		if ( m_pCharacterSetting->wLevel > MAX_CHARACTER_LEVEL )	
			m_pCharacterSetting->wLevel = MAX_CHARACTER_LEVEL; 

		m_pCharacterSetting->wStats = GetWin_Num_uint( this, IDC_EDIT_CHAR_STATS );
		m_pCharacterSetting->llMoney = GetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_MONEY );
		m_pCharacterSetting->dwActivityPoints = GetWin_Num_uint( this, IDC_EDIT_CHAR_ACTIVITY_P );
		m_pCharacterSetting->dwContributionPoints = GetWin_Num_uint( this, IDC_EDIT_CHAR_CONTRI_P );
		m_pCharacterSetting->dwWarChips = GetWin_Num_uint( this, IDC_EDIT_CHAR_WAR_CHIPS );
		m_pCharacterSetting->dwGamePoints = GetWin_Num_uint( this, IDC_EDIT_CHAR_GAME_POINTS );

		m_pCharacterSetting->bOpenSkills = GetWin_Checkb( this, IDC_CHK_OPEN_SKILLS );
		m_pCharacterSetting->bOpenInventory = GetWin_Checkb( this, IDC_CHK_OPEN_INVENTORY );

		/* user flag verified, Juver, 2020/02/25 */
		m_pCharacterSetting->bUserVerified = GetWin_Checkb( this, IDC_CHK_USER_VERIFIED );

		/* user flag restricted, Juver, 2020/04/20 */
		m_pCharacterSetting->bUserRestricted = GetWin_Checkb( this, IDC_CHK_USER_RESTRICTED );

		/* play time system, Juver, 2021/01/26 */
		m_pCharacterSetting->llChaPlayTime = GetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_PLAY_TIME );
		m_pCharacterSetting->llChaPlayPoint = GetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_PLAY_POINT );

		/* reborn system, Juver, 2021/09/17 */
		m_pCharacterSetting->dwReborn = GetWin_Num_uint( this, IDC_EDIT_CHAR_REBORN );

		m_pCharacterSetting->wSkillPoints = GetWin_Num_uint( this, IDC_EDIT_CHAR_SKILL_P );

		m_pCharacterSetting->wStatPoints = GetWin_Num_uint( this, IDC_EDIT_CHAR_STATS_P );

		m_pCharacterSetting->emVIP = static_cast<EMVIP_LEVEL>( GetWin_Combo_Sel( this, IDC_COMBO_VIP_LEVEL ));
		m_pCharacterSetting->wExchangeItemPoints = GetWin_Num_uint( this, IDC_EDIT_EXCHANGE_ITEM_P );
	}

	CDialog::OnOK();
}

void CDlgCharset::OnBnClickedButtonLevelMax()
{
	SetWin_Num_uint( this, IDC_EDIT_CHAR_LEVEL, MAX_CHARACTER_LEVEL );
}

void CDlgCharset::OnBnClickedButtonStats10k()
{
	WORD wStats = GetWin_Num_uint( this, IDC_EDIT_CHAR_STATS );
	wStats += 10000;
	SetWin_Num_uint( this, IDC_EDIT_CHAR_STATS, wStats );
}

void CDlgCharset::OnBnClickedButtonMoney1b()
{
	LONGLONG llMoney = GetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_MONEY );
	llMoney += 1000000000;
	SetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_MONEY, llMoney );
}

void CDlgCharset::OnBnClickedButtonActivity10k()
{
	DWORD dwNum = GetWin_Num_uint( this, IDC_EDIT_CHAR_ACTIVITY_P );
	dwNum += 10000;
	SetWin_Num_uint( this, IDC_EDIT_CHAR_ACTIVITY_P, dwNum );
}

void CDlgCharset::OnBnClickedButtonContribution10k()
{
	LONGLONG llNum = GetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_CONTRI_P );
	llNum += 10000;
	SetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_CONTRI_P, llNum );
}

void CDlgCharset::OnBnClickedButtonWarChips10k()
{
	LONGLONG llNum = GetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_WAR_CHIPS );
	llNum += 10000;
	SetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_WAR_CHIPS, llNum );
}

void CDlgCharset::OnBnClickedButtonGamePoints10k()
{
	LONGLONG llNum = GetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_GAME_POINTS );
	llNum += 10000;
	SetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_GAME_POINTS, llNum );
}

void CDlgCharset::OnBnClickedButtonPlayTime10k()
{
	LONGLONG llVal = GetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_PLAY_TIME );
	llVal += 10000;
	SetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_PLAY_TIME, llVal );
}

void CDlgCharset::OnBnClickedButtonPlayPoint10k()
{
	LONGLONG llVal = GetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_PLAY_POINT );
	llVal += 10000;
	SetWin_Num_LONGLONG( this, IDC_EDIT_CHAR_PLAY_POINT, llVal );
}

void CDlgCharset::OnBnClickedButtonReborn100()
{
	/* reborn system, Juver, 2021/09/17 */
	WORD wVal = GetWin_Num_uint( this, IDC_EDIT_CHAR_REBORN );
	wVal += 100;
	SetWin_Num_uint( this, IDC_EDIT_CHAR_REBORN, wVal );
}

void CDlgCharset::OnBnClickedButtonSkillP()
{
	WORD wStats = GetWin_Num_uint( this, IDC_EDIT_CHAR_SKILL_P );
	wStats += 10000;
	SetWin_Num_uint( this, IDC_EDIT_CHAR_SKILL_P, wStats );
}

void CDlgCharset::OnBnClickedButtonStatsP()
{
	WORD wStats = GetWin_Num_uint( this, IDC_EDIT_CHAR_STATS_P );
	wStats += 10000;
	SetWin_Num_uint( this, IDC_EDIT_CHAR_STATS_P, wStats );
}

void CDlgCharset::OnBnClickedButtonExchangeItemP()
{
	WORD wVal = GetWin_Num_uint( this, IDC_EDIT_EXCHANGE_ITEM_P );
	wVal += 100;
	SetWin_Num_uint( this, IDC_EDIT_EXCHANGE_ITEM_P, wVal );
}
