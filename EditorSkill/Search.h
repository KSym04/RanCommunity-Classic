#pragma once

#include "../Lib_Engine/G-Logic/GLDefine.h"

#include "./SearchDefine.h"


// CSearch dialog

class CSearch : public CDialog
{
	DECLARE_DYNAMIC(CSearch)

public:
	CSearch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearch();

// Dialog Data
	enum { IDD = IDD_DIALOG_SEARCH };

public:
	CListCtrl	m_List;
	SNATIVEID	m_sID;

public:
	std::vector<SSEARCH> *	m_vec;

	void SetContainer( std::vector<SSEARCH>* vec )
	{	
		m_vec = vec;	
	}

	void ShowResults();
	void SearchInit();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnCbnSelchangeComboSearchtype();
	afx_msg void OnBnClickedButtonGo();
};
