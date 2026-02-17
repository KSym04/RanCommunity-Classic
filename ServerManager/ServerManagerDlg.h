#ifndef S_SERVERMANAGERDLG_H_
#define S_SERVERMANAGERDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"
#include "afxtempl.h"

#include "s_CNetCtrl.h"
#include "Led.h" // LED Control
#include "ListCtrlEx.h"
#include "JobManagerDlg.h"

#include "MatrixStatic.h"
#include "CLogFile.h"
#include "CManagerCfg.h"

#include <atlbase.h>

struct G_SERVER_INFO_DLG
{
	G_SERVER_INFO gsi;
	BOOL bOpen;	
	BOOL bCheck;
	int  nRow;

	G_SERVER_INFO_DLG()
	{
		bOpen = FALSE;
		bCheck = FALSE;
		nRow = -1;
	};

	friend bool operator==(const G_SERVER_INFO_DLG& a, const G_SERVER_INFO_DLG& b)
	{
		if ((strcmp(a.gsi.szServerIP, b.gsi.szServerIP) == 0) &&
			(a.gsi.nServicePort == b.gsi.nServicePort)) 
			return true;
		else
			return false;
	};
};

struct G_SVR_SIMPLE
{
	char	szServerIP[MAX_IP_LENGTH+1];
	int		nServicePort;	
};

class CServerManagerDlg : public CDialog
{
public:
	CServerManagerDlg(CWnd* pParent = NULL);
    virtual ~CServerManagerDlg();

	enum { IDD = IDD_SERVERMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	
// ±¸Çö
protected:
	HICON m_hIcon;
	CMatrixStatic m_MaxUser;
	CMatrixStatic m_CrtUser;

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:	
	CListCtrl			m_ListServer;

	CIPAddressCtrl		m_IPAddress;
	CEdit				m_Port;
	CNetCtrl*			m_pNetClient;
	CLed				m_Led1;	
	UINT*				m_nTimer;
	CEdit				m_EditRefreshRate;
	int					m_nMaxUser;
	CJobManagerDlg*		m_pJobDlg;
	CLogFile*			m_pLogFile;

	int					m_SendMsgNum;

	CTypedPtrList<CPtrList, G_SERVER_INFO_DLG*> m_DlgList;
	std::vector<G_SERVER_INFO_DLG*>				m_vecServerList;

	CString m_strNoticeMsg;

public:	
	void	MsgProcess(NET_MSG_GENERIC* nmg);
	void	MsgAllSvrInfo(NET_MSG_GENERIC* nmg);

    void	InitListHead();
    
	void	AddListCtrl(G_SERVER_INFO_DLG* gsid);
	void	UpdateListCtrl(G_SERVER_INFO& gsi);
	void	UpdateSvrListStart();
	void	UpdateSvrListEnd();

	G_SVR_SIMPLE GetSvr();
	void	OnStopTimer();
	CString	GetAppPath();
    

	void    SetLedRed();
	void    SetLedGreen();

	void    PlayServerDownSound();
	void    PlayTracingUserLogOnOff();


	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNMDblclkListServer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListServer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextmenuOpen();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnContextmenuMessage();
	afx_msg void OnBnClickedBtnIdsearch();
	afx_msg void OnBnClickedBtnJobSchedule();
	afx_msg void OnEnMaxtextEditConsole();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();    
    afx_msg void OnBnClickedButton2();    
    afx_msg void OnBnClickedBtnMsgSnd();

protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	afx_msg void OnClickMenuExit();
	afx_msg void OnBnClickedSendType1();
	afx_msg void OnBnClickedSendType2();
	afx_msg void OnJobAdd();
	afx_msg void OnJobEdit();
	afx_msg void OnJobDelete();
	CEdit m_EditMsg[20];

	CEdit m_EditConsole;
	afx_msg void OnBnClickedView10Button();
	afx_msg void OnBnClickedView20Button();
	afx_msg void OnBnClickedView1Button();


	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);

	afx_msg void OnBnClickedBtnMsgClear();
};

#endif // S_SERVERMANAGERDLG_H_
