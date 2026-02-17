#pragma once


class CMainTab;
// CEditorBar dialog

class CEditorBar : public CDialogBar
{
	DECLARE_DYNAMIC(CEditorBar)

public:
	CEditorBar();   // standard constructor
	virtual ~CEditorBar();

// Dialog Data
	enum { IDD = IDD_DLG_MAIN };

public:
	CMainTab	*m_pTab;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg LONG OnInitDialog ( UINT, LONG );
	DECLARE_MESSAGE_MAP()


};
