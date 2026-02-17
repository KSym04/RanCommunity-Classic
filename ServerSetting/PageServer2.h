#pragma once

#include "../Lib_Network/s_NetGlobal.h"

class CServerSettingTab;
// CPageServer2 dialog

class CPageServer2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageServer2)

public:
	CPageServer2(LOGFONT logfont);
	virtual ~CPageServer2();

// Dialog Data
	enum { IDD = IDD_PAGE_SERVER_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CServerSettingTab*		m_pTab;
	int				m_CallPage;
	BOOL			m_bDlgInit;
	CFont*			m_pFont;


public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CServerSettingTab* pSheetTab ) { m_pTab = pSheetTab; }
	BOOL	ShowData();
	BOOL	SaveData();

public:
	CListCtrl		m_ListField;
	void EditField();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFieldSave();
	afx_msg void OnBnClickedButtonFieldCancel();
	afx_msg void OnBnClickedButtonServer2FieldEdit();
	afx_msg void OnNMDblclkListFieldServer(NMHDR *pNMHDR, LRESULT *pResult);
};
