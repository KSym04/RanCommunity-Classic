/////////////////////////////////////////////////////////////////////////////
//	FileName	: GlobalRankingPageSlot.h
//	Project		: RanClientUILib
//	Coder		: LG-7

#pragma once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "../Lib_Client/DxGlobalStage.h"

class CBasicLineBox;
class CBasicTextBox;
class CGlobalRankingPageSlot : public CUIGroup
{
private:
	enum
	{
		DUMMY = NO_ID + 1,
		CLASS_DUMMY,
		SCHOOL_DUMMY,
		GUILD_DUMMY
	};

	enum { nMAX_TEXT = 6 };

private:
	int					m_nIndex1;
	int					m_nIndex2;
	BOOL				m_bGuild;
	int					m_nSize;
	BOOL				m_bHaveData;
	LPDIRECT3DDEVICEQ	m_pd3dDevice;

private:
	CBasicLineBox*	m_pLBoxA_[8];
	CBasicLineBox*	m_pLBoxB_[8];
	CUIControl*		m_pDummy;
	CBasicTextBox*	m_pText_[nMAX_TEXT];
	CUIControl*		m_pClassImage;
	CUIControl*		m_pSchoolImage;
	CUIControl*		m_pGuildImage;
	CUIControl*		m_pRankImage; //MMR RANK
	STOP_RANK_KILL	m_sTopKill;
	STOP_RANK_KILL_BR	m_sTopKillBr;
	STOP_RANK_KILL_SW	m_sTopKillSw;
	STOP_RANK_KILL_AR	m_sTopKillAr;
	STOP_RANK_KILL_SH	m_sTopKillSh;
	STOP_RANK_KILL_EX	m_sTopKillEx;
	STOP_RANK_KILL_SC	m_sTopKillSc;
	STOP_RANK_KILL_AS	m_sTopKillAs;
	STOP_RANK_RICH	m_sTopRich;
	/*Top Level*/
	STOP_RANK_LEVEL	m_sTopLevel;
	STOP_RANK_GUILD	m_sTopGuild;

	STOP_RANK_RESU	m_sTopResu;/*Top Resu, Mhundz */
	STOP_RANK_MMR	m_sTopMMR;//MMR Rank
public:
	CGlobalRankingPageSlot();
	virtual ~CGlobalRankingPageSlot();

public:
	void CreateSubControl(int nIndex1, int nIndex2, BOOL bGuild);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual HRESULT InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
	virtual HRESULT Render(LPDIRECT3DDEVICEQ pd3dDevice);

private:
	CBasicTextBox* CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);
	void DefaultPos();

public:
	void RESET();
	void SetRankingKill(STOP_RANK_KILL sTopKill);
	/*re-coded LG-7 Global Ranking by DevArt22*/
	void SetRankingKillBr(STOP_RANK_KILL_BR sTopKill);
	void SetRankingKillSw(STOP_RANK_KILL_SW sTopKill);
	void SetRankingKillAr(STOP_RANK_KILL_AR sTopKill);
	void SetRankingKillSh(STOP_RANK_KILL_SH sTopKill);
	void SetRankingKillEx(STOP_RANK_KILL_EX sTopKill);
	void SetRankingKillSc(STOP_RANK_KILL_SC sTopKill);
	void SetRankingKillAs(STOP_RANK_KILL_AS sTopKill);

	void SetRankingRich(STOP_RANK_RICH sTopRich);
	/*Top Level*/
	void SetRankingLevel(STOP_RANK_LEVEL sTopLevel);
	void SetRankingGuild(STOP_RANK_GUILD sTopGuild);
	/*Top Resu, Mhundz */
	void SetRankingResu(STOP_RANK_RESU sTopResu);
	void SetRankingMMR(STOP_RANK_MMR sTopMMR);//MMR Rank
	int CalculateVal ( DWORD dwCalc );//MMR Rank

	BOOL IsHaveData() { return m_bHaveData; }
};