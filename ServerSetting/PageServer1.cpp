// PageServer1.cpp : implementation file
//

#include "stdafx.h"
#include "./ServerSetting.h"
#include "./PageServer1.h"
#include "afxdialogex.h"
#include "./ServerSettingTab.h"
#include "../Lib_Network/s_CCfg.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/HLibDataConvert.h"
#include "WinSock2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPageServer1 dialog

IMPLEMENT_DYNAMIC(CPageServer1, CPropertyPage)

CPageServer1::CPageServer1(LOGFONT logfont)
	: CPropertyPage(CPageServer1::IDD)
	, m_pFont(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CPageServer1::~CPageServer1()
{
	SAFE_DELETE(m_pFont);
}

void CPageServer1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageServer1, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_1_SAVE, &CPageServer1::OnBnClickedButtonServer1Save)
END_MESSAGE_MAP()


// CPageServer1 message handlers
BOOL CPageServer1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CServerSettingTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	ShowData();

	return TRUE;  
}

BOOL CPageServer1::ShowData()
{
	G_SERVER_INFO* psession = CCfg::GetInstance()->GetSessionServer();
	if ( psession )
	{
		SetWin_Text( this, IDC_EDIT_SESSION_NAME, psession->szServerName );
		SetWin_Text( this, IDC_EDIT_SESSION_IP, psession->szServerIP );
		SetWin_Num_int( this, IDC_EDIT_SESSION_PORT, psession->nServicePort );
		SetWin_Text( this, IDC_EDIT_SESSION_USER, psession->szUserID );
		SetWin_Text( this, IDC_EDIT_SESSION_PASS, psession->szUserPass );
	}

	G_SERVER_INFO* plogin = CCfg::GetInstance()->GetLoginServer();
	if ( plogin )
	{
		SetWin_Text( this, IDC_EDIT_LOGIN_NAME, plogin->szServerName );
		SetWin_Text( this, IDC_EDIT_LOGIN_IP, plogin->szServerIP );
		SetWin_Num_int( this, IDC_EDIT_LOGIN_PORT, plogin->nServicePort );
		SetWin_Text( this, IDC_EDIT_LOGIN_USER, plogin->szUserID );
		SetWin_Text( this, IDC_EDIT_LOGIN_PASS, plogin->szUserPass );
	}

	SetWin_Text( this, IDC_EDIT_AGENT_IP, CCfg::GetInstance()->GetAgentIP() );

	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_0, CCfg::GetInstance()->IsPkChannel(0) );
	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_1, CCfg::GetInstance()->IsPkChannel(1) );
	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_2, CCfg::GetInstance()->IsPkChannel(2) );
	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_3, CCfg::GetInstance()->IsPkChannel(3) );
	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_4, CCfg::GetInstance()->IsPkChannel(4) );
	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_5, CCfg::GetInstance()->IsPkChannel(5) );
	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_6, CCfg::GetInstance()->IsPkChannel(6) );
	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_7, CCfg::GetInstance()->IsPkChannel(7) );
	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_8, CCfg::GetInstance()->IsPkChannel(8) );
	SetWin_Check( this, IDC_CHECK_CHANNEL_PK_9, CCfg::GetInstance()->IsPkChannel(9) );

	return TRUE;  
}

BOOL CPageServer1::SaveData()
{
	G_SERVER_INFO* psession = CCfg::GetInstance()->GetSessionServer();
	if ( psession )
	{
		StringCchCopy(psession->szServerName, SERVER_NAME_LENGTH+1, GetWin_Text( this, IDC_EDIT_SESSION_NAME ).GetString() );
		StringCchCopy(psession->szServerIP, MAX_IP_LENGTH+1, GetWin_Text( this, IDC_EDIT_SESSION_IP ).GetString() );

		unsigned long ulAddr = ::inet_addr( psession->szServerIP );
		if ( ulAddr!=INADDR_NONE )
			psession->ulServerIP = ulAddr;

		psession->nServicePort = GetWin_Num_int( this, IDC_EDIT_SESSION_PORT );

		StringCchCopy(psession->szUserID, USR_ID_LENGTH+1, GetWin_Text( this, IDC_EDIT_SESSION_USER ).GetString() );
		StringCchCopy(psession->szUserPass, USR_PASS_LENGTH+1, GetWin_Text( this, IDC_EDIT_SESSION_PASS ).GetString() );
	}

	G_SERVER_INFO* plogin = CCfg::GetInstance()->GetLoginServer();
	if ( plogin )
	{
		StringCchCopy(plogin->szServerName, SERVER_NAME_LENGTH+1, GetWin_Text( this, IDC_EDIT_LOGIN_NAME ).GetString() );
		StringCchCopy(plogin->szServerIP, MAX_IP_LENGTH+1, GetWin_Text( this, IDC_EDIT_LOGIN_IP ).GetString() );

		unsigned long ulAddr = ::inet_addr( plogin->szServerIP );
		if ( ulAddr!=INADDR_NONE )
			plogin->ulServerIP = ulAddr;

		plogin->nServicePort = GetWin_Num_int( this, IDC_EDIT_LOGIN_PORT );

		StringCchCopy(plogin->szUserID, USR_ID_LENGTH+1, GetWin_Text( this, IDC_EDIT_LOGIN_USER ).GetString() );
		StringCchCopy(plogin->szUserPass, USR_PASS_LENGTH+1, GetWin_Text( this, IDC_EDIT_LOGIN_PASS ).GetString() );
	}

	CCfg::GetInstance()->SetAgentIP( GetWin_Text( this, IDC_EDIT_AGENT_IP ).GetString() );

	CCfg::GetInstance()->SetPkChannel(0, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_0 )? true:false );
	CCfg::GetInstance()->SetPkChannel(1, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_1 )? true:false );
	CCfg::GetInstance()->SetPkChannel(2, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_2 )? true:false );
	CCfg::GetInstance()->SetPkChannel(3, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_3 )? true:false );
	CCfg::GetInstance()->SetPkChannel(4, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_4 )? true:false );
	CCfg::GetInstance()->SetPkChannel(5, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_5 )? true:false );
	CCfg::GetInstance()->SetPkChannel(6, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_6 )? true:false );
	CCfg::GetInstance()->SetPkChannel(7, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_7 )? true:false );
	CCfg::GetInstance()->SetPkChannel(8, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_8 )? true:false );
	CCfg::GetInstance()->SetPkChannel(9, GetWin_Check( this, IDC_CHECK_CHANNEL_PK_9 )? true:false );

	return TRUE;  
}

void CPageServer1::OnBnClickedButtonServer1Save()
{
	if ( SaveData() )
		CDebugSet::MsgBox( GetSafeHwnd(), "Page Server 1 Saved!" );
	else
		CDebugSet::MsgBox( GetSafeHwnd(), "Page Server 1 Save Failed!");
}
