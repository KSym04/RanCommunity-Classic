#pragma once

#include "../Lib_Engine/G-Logic/GLDefine.h"

#include "./SearchDefine.h"

// CSearch2 dialog

class CSearch2 : public CDialogEx
{
	DECLARE_DYNAMIC(CSearch2)

public:
	CSearch2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearch2();

// Dialog Data
	enum { IDD = IDD_DIALOG_SEARCH2 };

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
	afx_msg void OnBnClickedButtonGo();
};
