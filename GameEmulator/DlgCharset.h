#pragma once

struct SCharacterSetting;

// CDlgCharset dialog

class CDlgCharset : public CDialog
{
	DECLARE_DYNAMIC(CDlgCharset)

public:
	CDlgCharset(CWnd* pParent = NULL, SCharacterSetting* pCharSetting = NULL );   // standard constructor
	virtual ~CDlgCharset();

// Dialog Data
	enum { IDD = IDD_DLG_CHARSET };

public:
	SCharacterSetting* m_pCharacterSetting;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedBtnCharset();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonLevelMax();
	afx_msg void OnBnClickedButtonStats10k();
	afx_msg void OnBnClickedButtonMoney1b();
	afx_msg void OnBnClickedButtonActivity10k();
	afx_msg void OnBnClickedButtonContribution10k();
	afx_msg void OnBnClickedButtonWarChips10k();
	afx_msg void OnBnClickedButtonGamePoints10k();
	afx_msg void OnBnClickedButtonPlayTime10k();
	afx_msg void OnBnClickedButtonPlayPoint10k();
	afx_msg void OnBnClickedButtonReborn100();
	afx_msg void OnBnClickedButtonSkillP();
	afx_msg void OnBnClickedButtonStatsP();
	afx_msg void OnBnClickedButtonExchangeItemP();
};
