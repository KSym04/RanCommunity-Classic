
// LauncherDlg.h : header file
//

#pragma once

#include "./explorer1.h"
#include "./RLauncherData.h"
#include "./RLauncher.h"
#include "./LPatchThread.h"
#include "../Lib_Helper/TextProgressCtrl.h"

class CHttpPatch;
class CPatchDownloaderThread;

typedef CLPatchThread* LPCLPATCHTHREAD;

// CLauncherDlg dialog
class CLauncherDlg : public CDialog
{
// Construction
public:
	CLauncherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	BOOL		m_bmouse_down;
	CPoint		m_mouse_click_pos;

protected:
	CExplorer1	m_ctrl_brower_main;
	CButton		m_ctrl_button_start;
	CListBox	m_ctrl_list_logs;
	CTextProgressCtrl	m_ctrl_progress_active;
	CTextProgressCtrl	m_ctrl_progress_total;

private:	
	BOOL					m_bforce_terminate;
	CHttpPatch*				m_phttp_patch;
	GAME_LAUNCHER_FILE_DATA_MAP	m_map_patch_list;

protected:
	CLPatchThread*	m_ppatch_thread;

public:
	void setup_app_path();
	void load_configuration();
	void set_dialog_text();

	void logs_add( const CString & _text );
	void logs_add( UINT id );
	void logs_add( const PCHAR _text );

	void update_progress_bar();

	void login_block();
	void login_enable();

	void thread_begin( LAUNCHER_THREAD_TYPE emtype, LPCLPATCHTHREAD & pThread, LPVOID pParam, const CString & strParam = "" );
	void thread_end( LPCLPATCHTHREAD & pThread );

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedCancel();
};
