#pragma once

#define RANKING_SIZE 10
#include "../Lib_ClientUI/Interface/UIWindowEx.h"

class CBasicTextBox;
class CWoeRankingPlayerPageSlot;
class CWoeRankingPlayerPage : public CUIWindowEx
{
public:
	void CreateSubControl();

	CWoeRankingPlayerPageSlot* m_pSlot[RANKING_SIZE];
	CWoeRankingPlayerPageSlot* m_pOwnSlot;

	CBasicTextBox* m_pTextBoxRank;
	CBasicTextBox* m_pTextBoxSchool;
	CBasicTextBox* m_pTextBoxClass;
	CBasicTextBox* m_pTextBoxName;
	CBasicTextBox* m_pTextBoxKill;
	CBasicTextBox* m_pTextBoxDeath;
	CBasicTextBox* m_pTextBoxCountHit;

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

public:
	void ClearData();
	void Refresh();
};
