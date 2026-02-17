#pragma once

#include "../Lib_ClientUI/Interface/UIWindowEx.h"

#define BUTTON_SIZE 3

class CBasicTextBoxEx;
class CBasicLineBox;
class CUIControl;
class CWoeRankingPlayerPage;
class CWoeRankingGuildPage;
class CWoeRankingResuPage;
class CWoeRanking : public CUIWindowEx
{
private:
	enum
	{
		WOE_BUTTON0 = ET_CONTROL_NEXT,
		WOE_BUTTON1,
		WOE_BUTTON2,

		WOE_SELECTION_PLAYER,
		WOE_SELECTION_GUILD,
		WOE_SELECTION_RESU
	};
public:
	void CreateSubControl();

	CBasicLineBox* m_pLineBox;

	CBasicTextBox* m_pTextBoxPlayer;
	CBasicTextBox* m_pTextBoxGuild;
	CBasicTextBox* m_pTextBoxResu;

	CBasicTextBoxEx* m_pListTextBox;

	CUIControl* m_pButton[BUTTON_SIZE];
	CUIControl* m_pButtonF[BUTTON_SIZE];

public:
	virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void SetVisibleSingle(BOOL bVisible);

public:
	void PlayerRadio();
	void GuildRadio();
	void ResuRadio();

	void RefreshRanking();

public:
	CWoeRankingPlayerPage* m_pWoePlayerPage;
	CWoeRankingGuildPage* m_pWoeGuildPage;
	CWoeRankingResuPage* m_pWoeResuPage;

private:
	int	m_nSelectIndexType;
};