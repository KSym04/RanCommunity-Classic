#pragma once

#include "../Lib_Client/G-Logic/GLSetOption.h"

// CDlgSetOptionEdit dialog

class CDlgSetOptionEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetOptionEdit)

public:
	CDlgSetOptionEdit(CWnd* pParent, SET_OPTION_DATA* pOptionData);   // standard constructor
	virtual ~CDlgSetOptionEdit();

// Dialog Data
	enum { IDD = IDD_SET_OPTION_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	SET_OPTION_DATA* m_pOptionData;

	void UpdateItem( int nID_MID, int nID_SID, int nID_NAME );

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonItemSlotAdd0();
	afx_msg void OnBnClickedButtonItemSlotAdd1();
	afx_msg void OnBnClickedButtonItemSlotAdd2();
	afx_msg void OnBnClickedButtonItemSlotAdd3();
	afx_msg void OnBnClickedButtonItemSlotAdd4();
	afx_msg void OnBnClickedButtonItemSlotAdd5();
	afx_msg void OnBnClickedButtonItemSlotAdd6();
	afx_msg void OnBnClickedButtonItemSlotAdd7();
	afx_msg void OnBnClickedButtonItemSlotAdd8();
	afx_msg void OnBnClickedButtonItemSlotAdd9();
	afx_msg void OnBnClickedButtonItemSlotAdd10();
	afx_msg void OnBnClickedButtonItemSlotAdd11();
	afx_msg void OnBnClickedButtonItemSlotAdd12();
	afx_msg void OnBnClickedButtonItemSlotAdd13();
	afx_msg void OnBnClickedButtonItemSlotDel0();
	afx_msg void OnBnClickedButtonItemSlotDel1();
	afx_msg void OnBnClickedButtonItemSlotDel2();
	afx_msg void OnBnClickedButtonItemSlotDel3();
	afx_msg void OnBnClickedButtonItemSlotDel4();
	afx_msg void OnBnClickedButtonItemSlotDel5();
	afx_msg void OnBnClickedButtonItemSlotDel6();
	afx_msg void OnBnClickedButtonItemSlotDel7();
	afx_msg void OnBnClickedButtonItemSlotDel8();
	afx_msg void OnBnClickedButtonItemSlotDel9();
	afx_msg void OnBnClickedButtonItemSlotDel10();
	afx_msg void OnBnClickedButtonItemSlotDel11();
	afx_msg void OnBnClickedButtonItemSlotDel12();
	afx_msg void OnBnClickedButtonItemSlotDel13();
	afx_msg void OnEnChangeEditItemSlotItemMid0();
	afx_msg void OnEnChangeEditItemSlotItemSid0();
	afx_msg void OnEnChangeEditItemSlotItemMid1();
	afx_msg void OnEnChangeEditItemSlotItemSid1();
	afx_msg void OnEnChangeEditItemSlotItemMid2();
	afx_msg void OnEnChangeEditItemSlotItemSid2();
	afx_msg void OnEnChangeEditItemSlotItemMid3();
	afx_msg void OnEnChangeEditItemSlotItemSid3();
	afx_msg void OnEnChangeEditItemSlotItemMid4();
	afx_msg void OnEnChangeEditItemSlotItemSid4();
	afx_msg void OnEnChangeEditItemSlotItemMid5();
	afx_msg void OnEnChangeEditItemSlotItemSid5();
	afx_msg void OnEnChangeEditItemSlotItemMid6();
	afx_msg void OnEnChangeEditItemSlotItemSid6();
	afx_msg void OnEnChangeEditItemSlotItemMid7();
	afx_msg void OnEnChangeEditItemSlotItemSid7();
	afx_msg void OnEnChangeEditItemSlotItemMid8();
	afx_msg void OnEnChangeEditItemSlotItemSid8();
	afx_msg void OnEnChangeEditItemSlotItemMid9();
	afx_msg void OnEnChangeEditItemSlotItemSid9();
	afx_msg void OnEnChangeEditItemSlotItemMid10();
	afx_msg void OnEnChangeEditItemSlotItemSid10();
	afx_msg void OnEnChangeEditItemSlotItemMid11();
	afx_msg void OnEnChangeEditItemSlotItemSid11();
	afx_msg void OnEnChangeEditItemSlotItemMid12();
	afx_msg void OnEnChangeEditItemSlotItemSid12();
	afx_msg void OnEnChangeEditItemSlotItemMid13();
	afx_msg void OnEnChangeEditItemSlotItemSid13();
};
