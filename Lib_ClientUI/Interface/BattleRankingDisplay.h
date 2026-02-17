#pragma once
#include "./UIWindowEx.h"

class CBasicLineBox;
class CBasicTextBox;
class CBasicTextButton;

struct SBATTLE_RANKING;
class CBattleRankingDisplay : public CUIWindowEx
{
private:
	enum
	{
		nMAX_LBOX			= 6,
		nMAX_TEXT_TITLE2	= 3,
		nMAX_TEXT_RANK		= 11
	};

	enum
	{
		BUTTON_RANK_PLAYER = ET_CONTROL_NEXT,
		BUTTON_RANK_CLUB,
	};

private:
	CBasicLineBox*	m_pLBox_[nMAX_LBOX];
	CBasicTextBox*	m_pText_Title1;
	CBasicTextBox*	m_pText_Title2_[nMAX_TEXT_TITLE2];
	CBasicTextBox*	m_pText_Rank_[nMAX_TEXT_RANK];
	CBasicTextBox*	m_pText_Name_[nMAX_TEXT_RANK];
	CBasicTextBox*	m_pText_KD_[nMAX_TEXT_RANK];

	CBasicTextButton*	m_pRankButtonPlayer;
	CBasicTextButton*	m_pRankButtonClub;
public:
	CBattleRankingDisplay();
	virtual ~CBattleRankingDisplay();

public:
	void CreateSubControl();
	virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

private:
	CBasicTextBox*		CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);
	CBasicTextButton*	CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText );
	void DefaultPos();

public:
	void ResetAll();
	void SetBattleTitle(CString strTitle);
	void AddBattleRanking(SBATTLE_RANKING sCWRanking);
	void AddMyBattleRanking(SBATTLE_RANKING sCWRanking);
	void RefreshBattleRank();
	int	 GetCurrentTabPage();
};
