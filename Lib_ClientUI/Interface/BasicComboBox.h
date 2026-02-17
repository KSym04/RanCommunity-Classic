//	기본 정보 표시
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.11.31]
//			@ 작성
//

#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"

#include "./UITextControl.h"

////////////////////////////////////////////////////////////////////
//	사용자 메시지 정의
const DWORD UI_MSG_COMBOBOX_ROLLOVER = UIMSG_USER1;
////////////////////////////////////////////////////////////////////

class	CBasicTextBox;

class	CBasicComboBox : public CUIGroup
{
protected:
	enum
	{
		BASIC_COMBOBOX_BUTTON_DOWN = 1,
		BASIC_COMBOBOX_TEXTBOX
	};

public:
	CBasicComboBox ();
	virtual	~CBasicComboBox ();

public:
	void	SetText ( CString str, DWORD dwTextColor = NS_UITEXTCOLOR::WHITE );

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	CreateBaseComboBox ( char* szComboBoxControl );	
	void	CreateBaseComboBox ( char* szComboBoxControl, DWORD dwFontSize, DWORD dwFontFlag );	

private:
	void	CreateComboImage ();
	void	CreateTextBox ();

private:
	CBasicTextBox*	m_pTextBox;
};
