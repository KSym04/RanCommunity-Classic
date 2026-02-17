#pragma once

#include "../Lib_Engine/DxOctree/DxLandMan.h"

class CLandmanTab;
// CLandmanEffect dialog

class CLandmanEffect : public CPropertyPage
{
	DECLARE_DYNAMIC(CLandmanEffect)

public:
	CLandmanEffect(LOGFONT logfont);
	virtual ~CLandmanEffect();

// Dialog Data
	enum { IDD = IDD_PAGE_WLD_EFFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CLandmanTab*	m_pTab;
	int				m_CallPage;
	BOOL			m_bDlgInit;
	CFont*			m_pFont;
	DxLandMan*		m_pData;

public:
	CListCtrl		m_List;

public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CLandmanTab* pSheetTab ) { m_pTab = pSheetTab; }
	void	SetData( DxLandMan* pData );
	void	ResetData();

	BOOL	ShowData();
	BOOL	SaveData();
	void	JumpTo( const D3DXMATRIX& sMat );
	void	ShowEdit( BOOL bShow );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnWldClose();
	afx_msg void OnBnClickedBtnEffectAdd();
	afx_msg void OnBnClickedBtnEffectDel();
	afx_msg void OnNMDblclkListEffect(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedBtnEffectEditMatrix();
	afx_msg void OnBnClickedButtonEffectFile();
	afx_msg void OnBnClickedButtonEffectSave();
	afx_msg void OnBnClickedButtonEffectCancel();
	afx_msg void OnBnClickedBtnEffectEditMatrixCancel();
};
