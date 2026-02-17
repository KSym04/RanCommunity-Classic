// PageServer2.cpp : implementation file
//

#include "stdafx.h"
#include "./ServerSetting.h"
#include "./PageServer2.h"
#include "afxdialogex.h"
#include "./ServerSettingTab.h"
#include "../Lib_Network/s_CCfg.h"
#include "../Lib_Helper/EtcFunction.h"
#include "../Lib_Helper/HLibDataConvert.h"
#include "WinSock2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPageServer2 dialog

IMPLEMENT_DYNAMIC(CPageServer2, CPropertyPage)

CPageServer2::CPageServer2(LOGFONT logfont)
	: CPropertyPage(CPageServer2::IDD)
	, m_pFont(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CPageServer2::~CPageServer2()
{
	SAFE_DELETE(m_pFont);
}

void CPageServer2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FIELD_SERVER, m_ListField );
}


BEGIN_MESSAGE_MAP(CPageServer2, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_SAVE, &CPageServer2::OnBnClickedButtonFieldSave)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_CANCEL, &CPageServer2::OnBnClickedButtonFieldCancel)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_2_FIELD_EDIT, &CPageServer2::OnBnClickedButtonServer2FieldEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FIELD_SERVER, &CPageServer2::OnNMDblclkListFieldServer)
END_MESSAGE_MAP()


// CPageServer2 message handlers
BOOL CPageServer2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pTab->ChangeDialogFont( this, m_pFont, CServerSettingTab::CDF_TOPLEFT );
	m_bDlgInit = TRUE;

	RECT rectCtrl;
	m_ListField.SetExtendedStyle ( m_ListField.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_ListField.GetClientRect ( &rectCtrl );
	m_ListField.InsertColumn( 0, "Channel", LVCFMT_LEFT, ( 13* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListField.InsertColumn( 1, "FieldID", LVCFMT_LEFT, ( 13* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListField.InsertColumn( 2, "Name", LVCFMT_LEFT, ( 20* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListField.InsertColumn( 3, "IP", LVCFMT_LEFT, ( 20* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListField.InsertColumn( 4, "Proxy", LVCFMT_LEFT, ( 20* ( rectCtrl.right - rectCtrl.left ) ) / 100 );
	m_ListField.InsertColumn( 5, "Port", LVCFMT_LEFT, ( 13* ( rectCtrl.right - rectCtrl.left ) ) / 100 );

	ShowData();

	return TRUE;  
}

BOOL CPageServer2::ShowData()
{
	m_ListField.DeleteAllItems();
	m_ListField.SetRedraw( FALSE );

	int nIndex = 0;
	for( int x = 0; x < MAX_CHANNEL_NUMBER; ++ x )
	{
		for( int y = 0; y < FIELDSERVER_MAX; ++ y )
		{
			F_SERVER_INFO* pfield = CCfg::GetInstance()->GetFieldServer( y, x );
			if ( pfield )
			{
				m_ListField.InsertItem( nIndex, _HLIB::cstring_dword(x).GetString() );
				m_ListField.SetItemText( nIndex, 1, _HLIB::cstring_dword(y).GetString() );
				m_ListField.SetItemText( nIndex, 2, pfield->szServerName );
				m_ListField.SetItemText( nIndex, 3, pfield->szServerIP );
				m_ListField.SetItemText( nIndex, 4, pfield->szServerIPProxy );
				m_ListField.SetItemText( nIndex, 5, _HLIB::cstring_dword( pfield->nServicePort ).GetString() );
			}
			
			nIndex++;
		}
	}

	m_ListField.SetRedraw( TRUE );

	SetWin_Num_int( this, IDC_EDIT_FIELD_SERVER_MAX_ID, CCfg::GetInstance()->GetMaxFieldNumber() );

	SetWin_Enable( this, IDC_EDIT_FIELD_NAME, FALSE );
	SetWin_Enable( this, IDC_EDIT_FIELD_IP, FALSE );
	SetWin_Enable( this, IDC_EDIT_FIELD_IP2, FALSE );
	SetWin_Enable( this, IDC_EDIT_FIELD_PORT, FALSE );
	SetWin_Enable( this, IDC_BUTTON_FIELD_SAVE, FALSE );
	SetWin_Enable( this, IDC_BUTTON_FIELD_CANCEL, FALSE );

	return TRUE;  
}

BOOL CPageServer2::SaveData()
{
	return TRUE;  
}

void CPageServer2::OnBnClickedButtonFieldSave()
{
	int channel = GetWin_Num_int( this, IDC_EDIT_FIELD_CHANNEL );
	int field = GetWin_Num_int( this, IDC_EDIT_FIELD_ID );

	F_SERVER_INFO* pfield = CCfg::GetInstance()->GetFieldServer( field, channel );
	if ( pfield )
	{
		StringCchCopy(pfield->szServerName, SERVER_NAME_LENGTH+1, GetWin_Text( this, IDC_EDIT_FIELD_NAME ).GetString() );
		StringCchCopy(pfield->szServerIP, MAX_IP_LENGTH+1, GetWin_Text( this, IDC_EDIT_FIELD_IP ).GetString() );
		StringCchCopy(pfield->szServerIPProxy, MAX_IP_LENGTH+1, GetWin_Text( this, IDC_EDIT_FIELD_IP2 ).GetString() );

		unsigned long ulAddr = ::inet_addr( pfield->szServerIP );
		if ( ulAddr!=INADDR_NONE )
			pfield->ulServerIP = ulAddr;

		pfield->nServicePort = GetWin_Num_int( this, IDC_EDIT_FIELD_PORT );

		CDebugSet::MsgBox( GetSafeHwnd(), "Saved" );
	}


	CCfg::GetInstance()->SetMaxFieldNumber( 0 );

	for( int x = 0; x < MAX_CHANNEL_NUMBER; ++ x )
	{
		for( int y = 0; y < FIELDSERVER_MAX; ++ y )
		{
			F_SERVER_INFO* pfield_server = CCfg::GetInstance()->GetFieldServer( y, x );
			if ( pfield_server && pfield_server->IsValid() && x == 0 )
			{
				int maxfield = CCfg::GetInstance()->GetMaxFieldNumber();
				if ( y > maxfield )
				{
					maxfield = y;
					CCfg::GetInstance()->SetMaxFieldNumber( maxfield );
				}
			}
		}
	}


	ShowData();

	for( int nItem = 0; nItem < m_ListField.GetItemCount(); nItem++)
	{
		int cur_channel = atoi( m_ListField.GetItemText( nItem, 0 ).GetString() );
		int cur_field = atoi( m_ListField.GetItemText( nItem, 1 ).GetString() );
		if( cur_channel == channel && cur_field == field )
		{
			m_ListField.EnsureVisible( nItem, TRUE );
			m_ListField.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
			GotoDlgCtrl(GetDlgItem(m_ListField.GetDlgCtrlID())); 
		}
	}
}

void CPageServer2::OnBnClickedButtonFieldCancel()
{
	ShowData();

	int channel = GetWin_Num_int( this, IDC_EDIT_FIELD_CHANNEL );
	int field = GetWin_Num_int( this, IDC_EDIT_FIELD_ID );

	for( int nItem = 0; nItem < m_ListField.GetItemCount(); nItem++)
	{
		int cur_channel = atoi( m_ListField.GetItemText( nItem, 0 ).GetString() );
		int cur_field = atoi( m_ListField.GetItemText( nItem, 1 ).GetString() );
		if( cur_channel == channel && cur_field == field )
		{
			m_ListField.EnsureVisible( nItem, TRUE );
			m_ListField.SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED );
			GotoDlgCtrl(GetDlgItem(m_ListField.GetDlgCtrlID())); 
		}
	}
}

void CPageServer2::OnBnClickedButtonServer2FieldEdit()
{
	EditField();
}

void CPageServer2::OnNMDblclkListFieldServer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	EditField();
}

void CPageServer2::EditField()
{
	int select = m_ListField.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( select == -1 ) return;

	int channel = atoi( m_ListField.GetItemText( select, 0 ).GetString() );
	int field = atoi( m_ListField.GetItemText( select, 1 ).GetString() );

	SetWin_Num_int( this, IDC_EDIT_FIELD_CHANNEL, channel );
	SetWin_Num_int( this, IDC_EDIT_FIELD_ID, field );

	F_SERVER_INFO* pfield = CCfg::GetInstance()->GetFieldServer( field, channel );
	if ( pfield )
	{
		SetWin_Enable( this, IDC_EDIT_FIELD_NAME, TRUE );
		SetWin_Enable( this, IDC_EDIT_FIELD_IP, TRUE );
		SetWin_Enable( this, IDC_EDIT_FIELD_IP2, TRUE );
		SetWin_Enable( this, IDC_EDIT_FIELD_PORT, TRUE );
		SetWin_Enable( this, IDC_BUTTON_FIELD_SAVE, TRUE );
		SetWin_Enable( this, IDC_BUTTON_FIELD_CANCEL, TRUE );

		SetWin_Text( this, IDC_EDIT_FIELD_NAME, pfield->szServerName );
		SetWin_Text( this, IDC_EDIT_FIELD_IP, pfield->szServerIP );
		SetWin_Text( this, IDC_EDIT_FIELD_IP2, pfield->szServerIPProxy );
		SetWin_Num_int( this, IDC_EDIT_FIELD_PORT, pfield->nServicePort );

		SetWin_Enable( this, IDC_BUTTON_FIELD_SAVE, TRUE );
		SetWin_Enable( this, IDC_BUTTON_FIELD_CANCEL, TRUE );
	}
}