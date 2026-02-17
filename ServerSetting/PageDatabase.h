#pragma once

class CServerSettingTab;
// CPageDatabase dialog

class CPageDatabase : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageDatabase)

public:
	CPageDatabase(LOGFONT logfont);
	virtual ~CPageDatabase();

// Dialog Data
	enum { IDD = IDD_PAGE_DATABASE };

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

	afx_msg void OnBnClickedButtonDatabaseSave();
};
