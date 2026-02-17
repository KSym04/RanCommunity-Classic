
// PatchBuilderDlg.h : header file
//

#pragma once

#include "./RPatcherData.h"
#include "../Lib_Helper/TextProgressCtrl.h"

// CPatchBuilderDlg dialog
class CPatchBuilderDlg : public CDialog
{
// Construction
public:
	CPatchBuilderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PATCH_BUILDER_DIALOG };

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

	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CListBox		m_ctrl_list_logs;
	CTextProgressCtrl	m_ctrl_progress_total;
	CStatic			m_ctrl_progress_static;		
	ULONGLONG		m_cur_pos;
	ULONGLONG		m_end_pos;

	PATCH_BUILDER_FILE_DATA_MAP m_map_file_scan;
	PATCH_BUILDER_FILE_DATA_MAP m_map_patch_list;
	PATCH_BUILDER_FILE_DATA_MAP m_map_update_list;
	PATCH_BUILDER_FILE_DATA_MAP m_map_upload_list;

public:
	void setup_app_path();
	void show_path_info();
	void logs_add(  const char *szFormat, ... );
	void update_progress();
	void set_button( BOOL b );

	void scan_path_recurse ( std::string &dir );
	BOOL read_patch_list();
	BOOL save_patch_list();
	BOOL save_patch_list_client();

	void action_scan_start();
	void action_build_start();
	void action_upload_start();

	void scan_source();
	void build_patch();
	void compare_hash();
	void upload_patch();

	void update_upload_list();
	void read_upload_list();
	void save_upload_list();

protected:
	static unsigned int WINAPI scan_start( void* pDlg );
	static unsigned int WINAPI build_start( void* pDlg );
	static unsigned int WINAPI upload_start( void* pDlg );

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedButtonBuild();
	afx_msg void OnBnClickedButtonUploadPatch();
};
