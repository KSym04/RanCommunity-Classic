#include "stdafx.h"
#include "ServerManager.h"
#include "ServerManagerDlg.h"
#include "Winuser.h"
#include "ChatMsgDlg.h"
#include "../Lib_Network/s_CClientConsoleMsg.h"
#include "s_CJobSchedule.h"
#include ".\servermanagerdlg.h"

#include "../Lib_Engine/Common/StlFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CServerManagerDlg::CServerManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerManagerDlg::IDD, pParent)
    , m_pJobDlg      (NULL)
    , m_pNetClient   (NULL)
    , m_nMaxUser     (0)
    , m_pLogFile     (NULL)
    , m_strNoticeMsg (_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    if (CoInitialize(NULL) != S_OK)
    {
        MessageBox("CoInitialize() Failed");
        return;
    }
	m_vecServerList.clear();
}

CServerManagerDlg::~CServerManagerDlg()
{  
    SAFE_DELETE(m_pLogFile);
    SAFE_DELETE(m_pNetClient);
}

void CServerManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVER, m_ListServer);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Control(pDX, IDC_LED1, m_Led1);
	DDX_Control(pDX, IDC_EDIT_REFRESH_RATE, m_EditRefreshRate);
	DDX_Control(pDX, IDC_STATIC_MAX, m_MaxUser);
	DDX_Control(pDX, IDC_STATIC_CRT, m_CrtUser);

	DDX_Control(pDX, IDC_EDIT_MSG1, m_EditMsg[0]);
	DDX_Control(pDX, IDC_EDIT_MSG2, m_EditMsg[1]);
	DDX_Control(pDX, IDC_EDIT_MSG3, m_EditMsg[2]);
	DDX_Control(pDX, IDC_EDIT_MSG4, m_EditMsg[3]);
	DDX_Control(pDX, IDC_EDIT_MSG5, m_EditMsg[4]);
	DDX_Control(pDX, IDC_EDIT_MSG6, m_EditMsg[5]);
	DDX_Control(pDX, IDC_EDIT_MSG7, m_EditMsg[6]);
	DDX_Control(pDX, IDC_EDIT_MSG8, m_EditMsg[7]);
	DDX_Control(pDX, IDC_EDIT_MSG9, m_EditMsg[8]);
	DDX_Control(pDX, IDC_EDIT_MSG10, m_EditMsg[9]);
	DDX_Control(pDX, IDC_EDIT_MSG11, m_EditMsg[10]);
	DDX_Control(pDX, IDC_EDIT_MSG12, m_EditMsg[11]);
	DDX_Control(pDX, IDC_EDIT_MSG13, m_EditMsg[12]);
	DDX_Control(pDX, IDC_EDIT_MSG14, m_EditMsg[13]);
	DDX_Control(pDX, IDC_EDIT_MSG15, m_EditMsg[14]);
	DDX_Control(pDX, IDC_EDIT_MSG16, m_EditMsg[15]);
	DDX_Control(pDX, IDC_EDIT_MSG17, m_EditMsg[16]);
	DDX_Control(pDX, IDC_EDIT_MSG18, m_EditMsg[17]);
	DDX_Control(pDX, IDC_EDIT_MSG19, m_EditMsg[18]);
	DDX_Control(pDX, IDC_EDIT_MSG20, m_EditMsg[19]);

	DDX_Control(pDX, IDC_EDIT_CONSOLE, m_EditConsole);
}

BEGIN_MESSAGE_MAP(CServerManagerDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBnClickedBtnConnect)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SERVER, OnNMDblclkListServer)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SERVER, OnNMRclickListServer)
	ON_COMMAND(ID_CONTEXTMENU_OPEN, OnContextmenuOpen)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_COMMAND(ID_CONTEXTMENU_MESSAGE, OnContextmenuMessage)
	ON_BN_CLICKED(IDC_BTN_JOB_SCHEDULE, OnBnClickedBtnJobSchedule)
	ON_EN_MAXTEXT(IDC_EDIT_CONSOLE, OnEnMaxtextEditConsole)
	ON_WM_SIZE()
	ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BTN_MSG_SND, OnBnClickedBtnMsgSnd)
	ON_COMMAND(ID_134, OnClickMenuExit)
	ON_BN_CLICKED(IDC_SEND_TYPE1, OnBnClickedSendType1)
	ON_BN_CLICKED(IDC_SEND_TYPE2, OnBnClickedSendType2)
	ON_COMMAND(ID_JOB_ADD, OnJobAdd)
	ON_COMMAND(ID_JOB_EDIT, OnJobEdit)
	ON_COMMAND(ID_JOB_DELETE, OnJobDelete)
	ON_BN_CLICKED(IDC_VIEW10_BUTTON, OnBnClickedView10Button)
	ON_BN_CLICKED(IDC_VIEW20_BUTTON, OnBnClickedView20Button)
	ON_BN_CLICKED(IDC_VIEW1_BUTTON, OnBnClickedView1Button)
	ON_WM_MOVING()
	ON_BN_CLICKED(IDC_BTN_MSG_CLEAR, OnBnClickedBtnMsgClear)
END_MESSAGE_MAP()


// CServerManagerDlg 메시지 처리기

BOOL CServerManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);

    SAFE_DELETE(m_pLogFile);
	m_pLogFile = new CLogFile(_T("ServerState"));
	if (m_pLogFile)
		m_pLogFile->Write(_T("Time,ServerType,Group,User,Total,Max"));	

    // Create console 
	CClientConsoleMessage::GetInstance()->SetControl(GetDlgItem(IDC_EDIT_CONSOLE)->m_hWnd);	

	// Create cfg module
	CServerManagerCfg::GetInstance()->Load();

	// Create network module
    // Load Winsock dll
	NET_InitializeSocket();
    SAFE_DELETE(m_pNetClient);
	m_pNetClient = new CNetCtrl(m_hWnd);
	m_pNetClient->SetCtrlHandle(this);

	// Create LED Control
	m_Led1.SetLed( CLed::LED_COLOR_RED, CLed::LED_ON, CLed::LED_SQUARE );
	
	// Max, Current User 
	m_MaxUser.SetNumberOfLines(1);	
	m_MaxUser.SetXCharsPerLine(7);
	m_MaxUser.SetArrange(CMatrixStatic::RIGHT);
	m_MaxUser.SetSize(CMatrixStatic::TINY);
	m_MaxUser.SetDisplayColors(RGB(0, 0, 0), RGB(255, 181, 63), RGB(103, 64, 23));
	m_MaxUser.AdjustClientXToSize(7);
	m_MaxUser.AdjustClientYToSize(1);
	m_MaxUser.SetText(_T("0"));

	m_CrtUser.SetNumberOfLines(1);
	m_CrtUser.SetXCharsPerLine(7);
	m_CrtUser.SetArrange(CMatrixStatic::RIGHT);
	m_CrtUser.SetSize(CMatrixStatic::TINY);
	m_CrtUser.SetDisplayColors(RGB(0, 0, 0), RGB(255, 181, 63), RGB(103, 64, 23));	
	m_CrtUser.AdjustClientXToSize(7);
	m_CrtUser.AdjustClientYToSize(1);
	m_CrtUser.SetText(_T("0"));	

	// Sizing Control
	CRect clientRect;
	GetClientRect(&clientRect);
	SendMessage(WM_SIZE, SIZE_RESTORED,	MAKELONG(clientRect.Width(), clientRect.Height()));

	// Setting Default value
	m_IPAddress.SetWindowText(CServerManagerCfg::GetInstance()->GetSessionIP());

	CString strTemp;
	strTemp.Format(_T("%d"), CServerManagerCfg::GetInstance()->GetSessionPort());
	m_Port.SetWindowText(strTemp.GetString());

	m_EditRefreshRate.SetWindowText(_T("60"));
	
	InitListHead(); // Init listcontrol head
	CheckRadioButton( IDC_SEND_TYPE1, IDC_SEND_TYPE2, IDC_SEND_TYPE1 );

	BYTE i;
	for( i = 0; i < 8; i++ )
	{
		GetDlgItem( IDC_SVR_CHECK1+i )->EnableWindow(FALSE);
	}

	RECT rect;
	for( i = 0; i < 20; i++ )
	{
		m_EditMsg[i].SetLimitText(48);
		if( i == 0  )
		{
			m_EditMsg[0].GetWindowRect( &rect );
			rect.top -= 48; rect.bottom -= 48;
		}else{
			m_EditMsg[i].ShowWindow(SW_HIDE);
		}
        m_EditMsg[i].MoveWindow( &rect, TRUE );
		rect.top += 23; rect.bottom += 23;
	}

	OnBnClickedView1Button();

	m_SendMsgNum = 1;

	// Start Job Schedule Timer
	SetTimer(100, 60000, 0);

	return TRUE;
}



void CServerManagerDlg::InitListHead()
{
	m_ListServer.SetExtendedStyle ( LVS_EX_FULLROWSELECT ); 

	LVCOLUMN Col;

	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 50;
	Col.fmt			= LVCFMT_LEFT;	
	Col.pszText		= _T("State");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListServer.InsertColumn(0, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 70;
	Col.fmt			= LVCFMT_LEFT;
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	Col.pszText		= _T("Kind");
	m_ListServer.InsertColumn(1, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;	
	Col.cx			= 100;
	Col.fmt			= LVCFMT_LEFT;	
	Col.pszText		= _T("Grp Num");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListServer.InsertColumn(2, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 110;
	Col.fmt			= LVCFMT_LEFT;	
	Col.pszText		= _T("Address");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListServer.InsertColumn(3, &Col);
		
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 50;
	Col.fmt			= LVCFMT_LEFT;	
	Col.pszText		= _T("Port");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListServer.InsertColumn(4, &Col);
		
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 100;
	Col.fmt			= LVCFMT_LEFT;	
	Col.pszText		= _T("Users/Max");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListServer.InsertColumn(5, &Col);
		
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 100;
	Col.fmt			= LVCFMT_LEFT;	
	Col.pszText		= _T("Percent");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListServer.InsertColumn(6, &Col);	

	m_ListServer.SetExtendedStyle ( LVS_EX_FULLROWSELECT ); 
}

CString	CServerManagerDlg::GetAppPath()
{
	CString strFullPath;
	CString strCommandLine;

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);
	strCommandLine = szPath;

	if (!strCommandLine.IsEmpty())
	{
		DWORD dwFind = strCommandLine.ReverseFind('\\');
		if (dwFind != -1)
		{
			strFullPath = strCommandLine.Left(dwFind);
			
			if (!strFullPath.IsEmpty())
			if (strFullPath.GetAt(0) == '"')
				strFullPath = strFullPath.Right(strFullPath.GetLength() - 1);
		}
	}
	return strFullPath;
}

void CServerManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);	
}

void CServerManagerDlg::OnClose()
{
	CDialog::OnClose();
}

void CServerManagerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	G_SERVER_INFO_DLG* gsid;
	POSITION pos = m_DlgList.GetHeadPosition();

	for (int i=0; i<m_DlgList.GetCount(); i++)
	{
		gsid = m_DlgList.GetAt(pos);
		
		for ( int ii = 0; ii<m_vecServerList.size(); ++ ii )
		{
			if ( m_vecServerList[ii]->gsi == gsid->gsi )
			{
				m_vecServerList.erase( m_vecServerList.begin() + ii );
				break;
			}
		}

        SAFE_DELETE(gsid);
		gsid = NULL;

		m_DlgList.GetNext(pos);
		Sleep( 1 );
	}
    m_DlgList.RemoveAll();

	for ( int ii = 0; ii<m_vecServerList.size(); ++ ii )
	{
		SAFE_DELETE( m_vecServerList[ii] );
	}
	m_vecServerList.clear();

	SAFE_DELETE(m_pJobDlg);
	
	KillTimer(100);
	CLIENT_JOB::CScheduler::GetInstance()->ReleaseInstance();
	CServerManagerCfg::GetInstance()->ReleaseInstance();
	CClientConsoleMessage::GetInstance()->ReleaseInstance();

    if ((m_pNetClient != NULL) && (m_pNetClient->IsOnline()))
    {
        m_pNetClient->CloseConnect();
    }
	SAFE_DELETE(m_pNetClient);	
	NET_CloseSocket();
}

void CServerManagerDlg::OnOK()
{	
	OnBnClickedBtnMsgSnd();
}

void CServerManagerDlg::OnCancel()
{	
	if (MessageBox(_T("Really Exit Program?"), _T("Warning"), MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
    {
        return;
    }
    CDialog::OnCancel();
}

void CServerManagerDlg::OnClickMenuExit()
{
	OnCancel();
}

void CServerManagerDlg::SetLedRed()
{
	m_Led1.SetLed( CLed::LED_COLOR_RED, CLed::LED_ON, CLed::LED_SQUARE );
}

void CServerManagerDlg::SetLedGreen()
{
	m_Led1.SetLed( CLed::LED_COLOR_RED, CLed::LED_ON, CLed::LED_SQUARE );
}

void CServerManagerDlg::OnBnClickedSendType1()
{
	BYTE i;
	for( i = 0; i < 8; i++ )
	{
		GetDlgItem( IDC_SVR_CHECK1+i )->EnableWindow(FALSE);
	}
}

void CServerManagerDlg::OnBnClickedSendType2()
{
	G_SERVER_INFO_DLG* gsid;
	size_t servSize = m_vecServerList.size();
	if (servSize)
	{
		for (size_t i=0; i< servSize; i++)
		{
			gsid = m_vecServerList[i];
			if (gsid->gsi.nServerType == NET_SERVER_AGENT)
			{
				if( gsid->gsi.nServerGroup-1 >= 0 && gsid->gsi.nServerGroup-1 < 8 )
					GetDlgItem( IDC_SVR_CHECK1+gsid->gsi.nServerGroup-1 )->EnableWindow(TRUE);
			}
			
		}
	}
}

void CServerManagerDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);
}




