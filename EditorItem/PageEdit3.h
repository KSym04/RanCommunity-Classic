#pragma once


// CPageEdit3 dialog
class	CsheetWithTab;
struct	SITEM;

class CPageEdit3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageEdit3)

public:
	CPageEdit3(LOGFONT logfont);
	virtual ~CPageEdit3();

// Dialog Data
	enum { IDD = IDD_EDIT3 };

private:
	CsheetWithTab*	m_pSheetTab;
	int				m_CallPage;
	BOOL			m_bDlgInit;
	CFont*			m_pFont;

public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CsheetWithTab* pSheetTab ) { m_pSheetTab = pSheetTab; }

public:
	SITEM*	m_pDummyItem;
	SITEM*	m_pItem;

public:
	BOOL	ItemSet ( SITEM* pItem );
	void	ItemClear();

	BOOL	DataDefault();
	BOOL	DataShow();
	BOOL	DataSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonPagePrev();
	afx_msg void OnBnClickedButtonPageNext();
	afx_msg void OnBnClickedButtonPageCancel();
	afx_msg void OnBnClickedButtonPageSave();
	afx_msg void OnBnClickedButtonItemDefaultColor1();
	afx_msg void OnBnClickedButtonItemDefaultColor2();
};
