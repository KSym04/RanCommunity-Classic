
// FileCryptDlg.h : header file
//

#include "../Lib_Engine/Common/CFileFind.h"

#pragma once

// CFileCryptDlg dialog
class CFileCryptDlg : public CDialog
{
// Construction
public:
	CFileCryptDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FILECRYPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CEdit			m_cEdit;
	CFileFindTree	m_cTree;
	std::string		m_strInputFolder;
	std::string		m_strOutputFolder;

public:
	void InfoAdd( const char *szFormat, ... );
	void InfoReset();
	void SettingChange();

public:
	void DoCrypt( BOOL bENCRYPT );
	void TextEncrypt();
	void TextDecrypt();
	void StreamEncrypt();
	void StreamDecrypt();
	void StreamDecryptR2();
	void StreamDecryptOrig();

	void XMLEncrypt();
	void XMLDecrypt();

	void TextureEncrypt();
	void TextureDecrypt();

	void XFileEncrypt();
	void XFileDecrypt();

	void AniFileEncrypt();

	void EGPtoNEW();
	void Ran2EGPtoNEW();

	void CPStoNEW();
	void Ran2CPStoNEW();

	void TextEncryptGS();
	void TextDecryptGS();

	void TextEncryptGSV4();
	void TextDecryptGSV4();

public:
	afx_msg void OnCbnSelchangeCmbFiletype();
	afx_msg void OnBnClickedBtnInput();
	afx_msg void OnBnClickedBtnOutput();
	afx_msg void OnBnClickedBtnInplace();
	afx_msg void OnBnClickedBtnEncrypt();
	afx_msg void OnBnClickedBtnDecrypt();
};
