#pragma once

#define RANKING_SIZE 10
#include "../Lib_ClientUI/Interface/UIWindowEx.h"

class CWoeRankingResuPageSlot;
class CBasicTextBox;
class CWoeRankingResuPage : public CUIWindowEx
{
public:
	void CreateSubControl();

	CWoeRankingResuPageSlot* m_pSlot[RANKING_SIZE];
	CWoeRankingResuPageSlot* m_pOwnSlot;

	CBasicTextBox* m_pTextBoxRank;
	CBasicTextBox* m_pTextBoxSchool;
	CBasicTextBox* m_pTextBoxClass;
	CBasicTextBox* m_pTextBoxName;
	CBasicTextBox* m_pTextBoxResu;

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

public:
	void ClearData();
	void Refresh();
};
