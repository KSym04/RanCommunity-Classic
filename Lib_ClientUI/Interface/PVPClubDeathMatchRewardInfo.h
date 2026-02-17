#pragma	once

#include "./UIWindowEx.h"

#include "../../Lib_Client/G-Logic/PVPClubDeathMatchDefine.h"

class CBasicTextBox;
class CItemImage;


class	CPVPClubDeathMatchRewardInfo : public CUIWindowEx
{
	enum
	{
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_00 = ET_CONTROL_NEXT,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_01,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_02,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_03,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_04,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_05,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_06,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_07,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_08,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_09,
		PVP_CLUB_DEATH_MATCH_REWARD_INFO_ITEM_10,
	};

public:
	CPVPClubDeathMatchRewardInfo ();
	virtual ~CPVPClubDeathMatchRewardInfo ();

public:
	void	CreateSubControl ();

public:
	CBasicTextBox*	m_pTextHeadRank;
	CBasicTextBox*	m_pTextHeadItem;
	CBasicTextBox*	m_pTextHeadContri;

	CBasicTextBox*	m_pTextRank[PVP_CLUB_DEATH_MATCH_RANK_TOP];
	CBasicTextBox*	m_pTextItem[PVP_CLUB_DEATH_MATCH_RANK_TOP];
	CBasicTextBox*	m_pTextContri[PVP_CLUB_DEATH_MATCH_RANK_TOP];
	CItemImage*		m_pItemImage[PVP_CLUB_DEATH_MATCH_RANK_TOP];

public:
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

	void ShowInfo();
};