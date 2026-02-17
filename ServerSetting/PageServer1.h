#pragma once

class CServerSettingTab;
// CPageServer1 dialog

class CPageServer1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageServer1)

public:
	CPageServer1(LOGFONT logfont);
	virtual ~CPageServer1();

// Dialog Data
	enum { IDD = IDD_PAGE_SERVER_1 };

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
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonServer1Save();
};
