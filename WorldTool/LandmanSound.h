#pragma once

#include "../Lib_Engine/DxOctree/DxLandMan.h"
#include "../Lib_Engine/DxSound/StaticSoundMan.h"

class CLandmanTab;
// CLandmanSound dialog

class CLandmanSound : public CPropertyPage
{
	DECLARE_DYNAMIC(CLandmanSound)

public:
	CLandmanSound(LOGFONT logfont);
	virtual ~CLandmanSound();

// Dialog Data
	enum { IDD = IDD_PAGE_WLD_SOUND };

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
	void	JumpTo( D3DXMATRIX sMat );

	void	ShowEdit( BOOL bShow );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnWldClose();
	afx_msg void OnBnClickedBtnSoundAdd();
	afx_msg void OnBnClickedBtnSoundDel();
	afx_msg void OnNMDblclkListSound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnEditSave();
	afx_msg void OnBnClickedBtnEditCancel();
	afx_msg void OnBnClickedBtnSoundFile();
};
