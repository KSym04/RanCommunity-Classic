#pragma once

#include "../Lib_ClientUI/Interface/UIWindowEx.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"

class CBasicTextBox;
class CWoeRankingPlayerPageSlot : public CUIWindowEx
{
public:
	void CreateSubControl();

	CBasicTextBox* m_pRankResult;
	CBasicTextBox* m_pNameResult;
	CBasicTextBox* m_pKillResult;
	CBasicTextBox* m_pDeathResult;
	CBasicTextBox* m_pCountHit;

	CUIControl* m_pSchoolResult[GLSCHOOL_NUM];
	CUIControl* m_pClassResult[GLCC_ALL_6CLASS];

public:
	void UpdateData(int nRank, WORD wSchool, WORD wClass, std::string strName, WORD wKill, WORD wDeath, DWORD dwCountHit);

public:
	void ClearData();
};