#pragma once

class CServerSettingTab;
// CPageBasic dialog

class CPageBasic : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageBasic)

public:
	CPageBasic(LOGFONT logfont);
	virtual ~CPageBasic();

// Dialog Data
	enum { IDD = IDD_PAGE_BASIC };

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
	afx_msg void OnBnClickedButtonBasicSave();
	afx_msg void OnBnClickedButtonHashBuild();
};
