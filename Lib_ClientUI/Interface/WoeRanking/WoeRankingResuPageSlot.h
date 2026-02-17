#pragma once

#include "../Lib_ClientUI/Interface/UIWindowEx.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"

class CBasicTextBox;
class CWoeRankingResuPageSlot : public CUIWindowEx
{
public:
	void CreateSubControl();

	CBasicTextBox* m_pRankResult;
	CBasicTextBox* m_pNameResult;
	CBasicTextBox* m_pResuResult;

	CUIControl* m_pSchoolResult[GLSCHOOL_NUM];
	CUIControl* m_pClassResult[GLCC_ALL_6CLASS];

public:
	void UpdateData(int nRank, WORD wSchool, WORD wClass, std::string strName, WORD wResu);

public:
	void ClearData();
};