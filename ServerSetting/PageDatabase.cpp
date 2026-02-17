// PageDatabase.cpp : implementation file
//

#include "stdafx.h"
#include "./ServerSetting.h"
#include "./PageDatabase.h"
#include "afxdialogex.h"
#include "./ServerSettingTab.h"
#include "../Lib_Network/s_CCfg.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/HLibDataConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPageDatabase dialog

IMPLEMENT_DYNAMIC(CPageDatabase, CPropertyPage)

CPageDatabase::CPageDatabase(LOGFONT logfont)
	: CPropertyPage(CPageDatabase::IDD)
	, m_pFont(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CPageDatabase::~CPageDatabase()
{
	SAFE_DELETE(m_pFont);
}

void CPageDatabase::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageDatabase, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DATABASE_SAVE, &CPageDatabase::OnBnClickedButtonDatabaseSave)
END_MESSAGE_MAP()


// CPageDatabase message handlers
BOOL CPageDatabase::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CServerSettingTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	ShowData();

	return TRUE;  
}

BOOL CPageDatabase::ShowData()
{
	SetWin_Text( this, IDC_EDIT_USER_ODBC_SERVER, CCfg::GetInstance()->GetUserOdbcName() );
	SetWin_Text( this, IDC_EDIT_USER_ODBC_USER, CCfg::GetInstance()->GetUserOdbcUser() );
	SetWin_Text( this, IDC_EDIT_USER_ODBC_PASS, CCfg::GetInstance()->GetUserOdbcPass() );
	SetWin_Text( this, IDC_EDIT_USER_ODBC_DATABASE, CCfg::GetInstance()->GetUserOdbcDatabase() );
	SetWin_Num_int( this, IDC_EDIT_USER_ODBC_POOL_SIZE, CCfg::GetInstance()->GetUserOdbcPoolSize() );
	SetWin_Num_int( this, IDC_EDIT_USER_ODBC_RESPONSE_TIME, CCfg::GetInstance()->GetUserOdbcResponseTime() );

	SetWin_Text( this, IDC_EDIT_GAME_ODBC_SERVER, CCfg::GetInstance()->GetGameOdbcName() );
	SetWin_Text( this, IDC_EDIT_GAME_ODBC_USER, CCfg::GetInstance()->GetGameOdbcUser() );
	SetWin_Text( this, IDC_EDIT_GAME_ODBC_PASS, CCfg::GetInstance()->GetGameOdbcPass() );
	SetWin_Text( this, IDC_EDIT_GAME_ODBC_DATABASE, CCfg::GetInstance()->GetGameOdbcDatabase() );
	SetWin_Num_int( this, IDC_EDIT_GAME_ODBC_POOL_SIZE, CCfg::GetInstance()->GetGameOdbcPoolSize() );
	SetWin_Num_int( this, IDC_EDIT_GAME_ODBC_RESPONSE_TIME, CCfg::GetInstance()->GetGameOdbcResponseTime() );

	SetWin_Text( this, IDC_EDIT_LOG_ODBC_SERVER, CCfg::GetInstance()->GetLogOdbcName() );
	SetWin_Text( this, IDC_EDIT_LOG_ODBC_USER, CCfg::GetInstance()->GetLogOdbcUser() );
	SetWin_Text( this, IDC_EDIT_LOG_ODBC_PASS, CCfg::GetInstance()->GetLogOdbcPass() );
	SetWin_Text( this, IDC_EDIT_LOG_ODBC_DATABASE, CCfg::GetInstance()->GetLogOdbcDatabase() );
	SetWin_Num_int( this, IDC_EDIT_LOG_ODBC_POOL_SIZE, CCfg::GetInstance()->GetLogOdbcPoolSize() );
	SetWin_Num_int( this, IDC_EDIT_LOG_ODBC_RESPONSE_TIME, CCfg::GetInstance()->GetLogOdbcResponseTime() );

	SetWin_Text( this, IDC_EDIT_SHOP_ODBC_SERVER, CCfg::GetInstance()->GetShopOdbcName() );
	SetWin_Text( this, IDC_EDIT_SHOP_ODBC_USER, CCfg::GetInstance()->GetShopOdbcUser() );
	SetWin_Text( this, IDC_EDIT_SHOP_ODBC_PASS, CCfg::GetInstance()->GetShopOdbcPass() );
	SetWin_Text( this, IDC_EDIT_SHOP_ODBC_DATABASE, CCfg::GetInstance()->GetShopOdbcDatabase() );
	SetWin_Num_int( this, IDC_EDIT_SHOP_ODBC_POOL_SIZE, CCfg::GetInstance()->GetShopOdbcPoolSize() );
	SetWin_Num_int( this, IDC_EDIT_SHOP_ODBC_RESPONSE_TIME, CCfg::GetInstance()->GetShopOdbcResponseTime() );

	SetWin_Text( this, IDC_EDIT_TERRA_ODBC_SERVER, CCfg::GetInstance()->GetTerraOdbcName() );
	SetWin_Text( this, IDC_EDIT_TERRA_ODBC_USER, CCfg::GetInstance()->GetTerraOdbcUser() );
	SetWin_Text( this, IDC_EDIT_TERRA_ODBC_PASS, CCfg::GetInstance()->GetTerraOdbcPass() );
	SetWin_Text( this, IDC_EDIT_TERRA_ODBC_DATABASE, CCfg::GetInstance()->GetTerraOdbcDatabase() );
	SetWin_Num_int( this, IDC_EDIT_TERRA_ODBC_POOL_SIZE, CCfg::GetInstance()->GetTerraOdbcPoolSize() );
	SetWin_Num_int( this, IDC_EDIT_TERRA_ODBC_RESPONSE_TIME, CCfg::GetInstance()->GetTerraOdbcResponseTime() );

	SetWin_Text( this, IDC_EDIT_GSP_ODBC_SERVER, CCfg::GetInstance()->GetGspOdbcName() );
	SetWin_Text( this, IDC_EDIT_GSP_ODBC_USER, CCfg::GetInstance()->GetGspOdbcUser() );
	SetWin_Text( this, IDC_EDIT_GSP_ODBC_PASS, CCfg::GetInstance()->GetGspOdbcPass() );
	SetWin_Text( this, IDC_EDIT_GSP_ODBC_DATABASE, CCfg::GetInstance()->GetGspOdbcDatabase() );
	SetWin_Num_int( this, IDC_EDIT_GSP_ODBC_POOL_SIZE, CCfg::GetInstance()->GetGspOdbcPoolSize() );
	SetWin_Num_int( this, IDC_EDIT_GSP_ODBC_RESPONSE_TIME, CCfg::GetInstance()->GetGspOdbcResponseTime() );

	SetWin_Text( this, IDC_EDIT_KOR_ODBC_SERVER, CCfg::GetInstance()->GetKorOdbcName() );
	SetWin_Text( this, IDC_EDIT_KOR_ODBC_USER, CCfg::GetInstance()->GetKorOdbcUser() );
	SetWin_Text( this, IDC_EDIT_KOR_ODBC_PASS, CCfg::GetInstance()->GetKorOdbcPass() );
	SetWin_Text( this, IDC_EDIT_KOR_ODBC_DATABASE, CCfg::GetInstance()->GetKorOdbcDatabase() );
	SetWin_Num_int( this, IDC_EDIT_KOR_ODBC_POOL_SIZE, CCfg::GetInstance()->GetKorOdbcPoolSize() );
	SetWin_Num_int( this, IDC_EDIT_KOR_ODBC_RESPONSE_TIME, CCfg::GetInstance()->GetKorOdbcResponseTime() );

	SetWin_Text( this, IDC_EDIT_MY_ODBC_SERVER, CCfg::GetInstance()->GetMyOdbcName() );
	SetWin_Text( this, IDC_EDIT_MY_ODBC_USER, CCfg::GetInstance()->GetMyOdbcUser() );
	SetWin_Text( this, IDC_EDIT_MY_ODBC_PASS, CCfg::GetInstance()->GetMyOdbcPass() );
	SetWin_Text( this, IDC_EDIT_MY_ODBC_DATABASE, CCfg::GetInstance()->GetMyOdbcDatabase() );
	SetWin_Num_int( this, IDC_EDIT_MY_ODBC_POOL_SIZE, CCfg::GetInstance()->GetMyOdbcPoolSize() );
	SetWin_Num_int( this, IDC_EDIT_MY_ODBC_RESPONSE_TIME, CCfg::GetInstance()->GetMyOdbcResponseTime() );

	return TRUE;  
}

BOOL CPageDatabase::SaveData()
{
	CCfg::GetInstance()->SetUserOdbcName( GetWin_Text( this, IDC_EDIT_USER_ODBC_SERVER ).GetString() );
	CCfg::GetInstance()->SetUserOdbcUser( GetWin_Text( this, IDC_EDIT_USER_ODBC_USER ).GetString() );
	CCfg::GetInstance()->SetUserOdbcPass( GetWin_Text( this, IDC_EDIT_USER_ODBC_PASS ).GetString() );
	CCfg::GetInstance()->SetUserOdbcDatabase( GetWin_Text( this, IDC_EDIT_USER_ODBC_DATABASE ).GetString() );
	CCfg::GetInstance()->SetUserOdbcPoolSize( GetWin_Num_int( this, IDC_EDIT_USER_ODBC_POOL_SIZE ) );
	CCfg::GetInstance()->SetUserOdbcResponseTime( GetWin_Num_int( this, IDC_EDIT_USER_ODBC_RESPONSE_TIME ) );

	CCfg::GetInstance()->SetGameOdbcName( GetWin_Text( this, IDC_EDIT_GAME_ODBC_SERVER ).GetString() );
	CCfg::GetInstance()->SetGameOdbcUser( GetWin_Text( this, IDC_EDIT_GAME_ODBC_USER ).GetString() );
	CCfg::GetInstance()->SetGameOdbcPass( GetWin_Text( this, IDC_EDIT_GAME_ODBC_PASS ).GetString() );
	CCfg::GetInstance()->SetGameOdbcDatabase( GetWin_Text( this, IDC_EDIT_GAME_ODBC_DATABASE ).GetString() );
	CCfg::GetInstance()->SetGameOdbcPoolSize( GetWin_Num_int( this, IDC_EDIT_GAME_ODBC_POOL_SIZE ) );
	CCfg::GetInstance()->SetGameOdbcResponseTime( GetWin_Num_int( this, IDC_EDIT_GAME_ODBC_RESPONSE_TIME ) );

	CCfg::GetInstance()->SetLogOdbcName( GetWin_Text( this, IDC_EDIT_LOG_ODBC_SERVER ).GetString() );
	CCfg::GetInstance()->SetLogOdbcUser( GetWin_Text( this, IDC_EDIT_LOG_ODBC_USER ).GetString() );
	CCfg::GetInstance()->SetLogOdbcPass( GetWin_Text( this, IDC_EDIT_LOG_ODBC_PASS ).GetString() );
	CCfg::GetInstance()->SetLogOdbcDatabase( GetWin_Text( this, IDC_EDIT_LOG_ODBC_DATABASE ).GetString() );
	CCfg::GetInstance()->SetLogOdbcPoolSize( GetWin_Num_int( this, IDC_EDIT_LOG_ODBC_POOL_SIZE ) );
	CCfg::GetInstance()->SetLogOdbcResponseTime( GetWin_Num_int( this, IDC_EDIT_LOG_ODBC_RESPONSE_TIME ) );

	CCfg::GetInstance()->SetShopOdbcName( GetWin_Text( this, IDC_EDIT_SHOP_ODBC_SERVER ).GetString() );
	CCfg::GetInstance()->SetShopOdbcUser( GetWin_Text( this, IDC_EDIT_SHOP_ODBC_USER ).GetString() );
	CCfg::GetInstance()->SetShopOdbcPass( GetWin_Text( this, IDC_EDIT_SHOP_ODBC_PASS ).GetString() );
	CCfg::GetInstance()->SetShopOdbcDatabase( GetWin_Text( this, IDC_EDIT_SHOP_ODBC_DATABASE ).GetString() );
	CCfg::GetInstance()->SetShopOdbcPoolSize( GetWin_Num_int( this, IDC_EDIT_SHOP_ODBC_POOL_SIZE ) );
	CCfg::GetInstance()->SetShopOdbcResponseTime( GetWin_Num_int( this, IDC_EDIT_SHOP_ODBC_RESPONSE_TIME ) );

	CCfg::GetInstance()->SetTerraOdbcName( GetWin_Text( this, IDC_EDIT_TERRA_ODBC_SERVER ).GetString() );
	CCfg::GetInstance()->SetTerraOdbcUser( GetWin_Text( this, IDC_EDIT_TERRA_ODBC_USER ).GetString() );
	CCfg::GetInstance()->SetTerraOdbcPass( GetWin_Text( this, IDC_EDIT_TERRA_ODBC_PASS ).GetString() );
	CCfg::GetInstance()->SetTerraOdbcDatabase( GetWin_Text( this, IDC_EDIT_TERRA_ODBC_DATABASE ).GetString() );
	CCfg::GetInstance()->SetTerraOdbcPoolSize( GetWin_Num_int( this, IDC_EDIT_TERRA_ODBC_POOL_SIZE ) );
	CCfg::GetInstance()->SetTerraOdbcResponseTime( GetWin_Num_int( this, IDC_EDIT_TERRA_ODBC_RESPONSE_TIME ) );

	CCfg::GetInstance()->SetGspOdbcName( GetWin_Text( this, IDC_EDIT_GSP_ODBC_SERVER ).GetString() );
	CCfg::GetInstance()->SetGspOdbcUser( GetWin_Text( this, IDC_EDIT_GSP_ODBC_USER ).GetString() );
	CCfg::GetInstance()->SetGspOdbcPass( GetWin_Text( this, IDC_EDIT_GSP_ODBC_PASS ).GetString() );
	CCfg::GetInstance()->SetGspOdbcDatabase( GetWin_Text( this, IDC_EDIT_GSP_ODBC_DATABASE ).GetString() );
	CCfg::GetInstance()->SetGspOdbcPoolSize( GetWin_Num_int( this, IDC_EDIT_GSP_ODBC_POOL_SIZE ) );
	CCfg::GetInstance()->SetGspOdbcResponseTime( GetWin_Num_int( this, IDC_EDIT_GSP_ODBC_RESPONSE_TIME ) );

	CCfg::GetInstance()->SetKorOdbcName( GetWin_Text( this, IDC_EDIT_KOR_ODBC_SERVER ).GetString() );
	CCfg::GetInstance()->SetKorOdbcUser( GetWin_Text( this, IDC_EDIT_KOR_ODBC_USER ).GetString() );
	CCfg::GetInstance()->SetKorOdbcPass( GetWin_Text( this, IDC_EDIT_KOR_ODBC_PASS ).GetString() );
	CCfg::GetInstance()->SetKorOdbcDatabase( GetWin_Text( this, IDC_EDIT_KOR_ODBC_DATABASE ).GetString() );
	CCfg::GetInstance()->SetKorOdbcPoolSize( GetWin_Num_int( this, IDC_EDIT_KOR_ODBC_POOL_SIZE ) );
	CCfg::GetInstance()->SetKorOdbcResponseTime( GetWin_Num_int( this, IDC_EDIT_KOR_ODBC_RESPONSE_TIME ) );

	CCfg::GetInstance()->SetMyOdbcName( GetWin_Text( this, IDC_EDIT_MY_ODBC_SERVER ).GetString() );
	CCfg::GetInstance()->SetMyOdbcUser( GetWin_Text( this, IDC_EDIT_MY_ODBC_USER ).GetString() );
	CCfg::GetInstance()->SetMyOdbcPass( GetWin_Text( this, IDC_EDIT_MY_ODBC_PASS ).GetString() );
	CCfg::GetInstance()->SetMyOdbcDatabase( GetWin_Text( this, IDC_EDIT_MY_ODBC_DATABASE ).GetString() );
	CCfg::GetInstance()->SetMyOdbcPoolSize( GetWin_Num_int( this, IDC_EDIT_MY_ODBC_POOL_SIZE ) );
	CCfg::GetInstance()->SetMyOdbcResponseTime( GetWin_Num_int( this, IDC_EDIT_MY_ODBC_RESPONSE_TIME ) );

	return TRUE;  
}

void CPageDatabase::OnBnClickedButtonDatabaseSave()
{
	if ( SaveData() )
		CDebugSet::MsgBox( GetSafeHwnd(), "Page Database Saved!" );
	else
		CDebugSet::MsgBox( GetSafeHwnd(), "Page Database Save Failed!");
}
