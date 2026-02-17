#pragma	once

#include "../Lib_ClientUI/Interface/UIWindowEx.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "../Lib_Client/G-Logic/GLCrowData.h"

#include "../Lib_Client/G-Logic/GLMapList.h"
struct GLCHARLOGIC;

class	CBasicTextBoxEx;
class	CBasicScrollBarEx;
class	CBasicTextButton;
class	CBasicButton;
class	CBossListSlot;
class	CItemImage;
class	CMobPreviewWindowRender;

struct SMAPDATA_LIST
{
	SNATIVEID sMAPID;
	std::string strNAME;

	SMAPDATA_LIST ()
		/*:*/
	{
		strNAME = "";
	};
};

class CBossDetailsWindow : public CUIWindowEx
{
public:

public:
	CBossDetailsWindow ();
	virtual	~CBossDetailsWindow ();

public:
	void	CreateSubControl ();
protected: 

	enum
	{
		BOSS_DETAILS_MAP_LIST = ET_CONTROL_NEXT,
		BOSS_DETAILS_SCROLL,

		BOSS_DETAILS_SLOT1,
		BOSS_DETAILS_SLOT2,
		BOSS_DETAILS_SLOT3,
		BOSS_DETAILS_SLOT4,
		BOSS_DETAILS_SLOT5,
		BOSS_DETAILS_SLOT6,
		BOSS_DETAILS_SLOT7,
		BOSS_DETAILS_SLOT8,

		REWARD_ITEM1,
		REWARD_ITEM2,
		REWARD_ITEM3,
		REWARD_ITEM4,
		REWARD_ITEM5,
		REWARD_ITEM6,
		REWARD_ITEM7,
		REWARD_ITEM8,
		REWARD_ITEM9,
		REWARD_ITEM10,
		REWARD_ITEM11,
		REWARD_ITEM12,
		REWARD_ITEM13,
		REWARD_ITEM14,
		REWARD_ITEM15,
		REWARD_ITEM16,
		REWARD_ITEM17,
		REWARD_ITEM18,
		REWARD_ITEM19,
		REWARD_ITEM20,
		REWARD_ITEM21,
		REWARD_ITEM22,
		REWARD_ITEM23,
		REWARD_ITEM24,
		BUTTON_LEFT,
		BUTTON_RIGHT,
	};

	static const int REWARD_H_LINE_COUNT = 4;
	static const int REWARD_V_LINE_COUNT = 6;
	static const int REWARD_COUNT = REWARD_H_LINE_COUNT * REWARD_V_LINE_COUNT;
	static const int BOSS_LIST_MAX_SLOT = 8;


protected: 
	typedef stdext::hash_map<DWORD,SNATIVEID>	REWARDITEM_MAP;
	typedef REWARDITEM_MAP::iterator			REWARDITEM_MAP_ITER;
	REWARDITEM_MAP								m_RewardItemMap;
	std::set<DWORD>								m_UniqueRewardItemSet;

	CItemImage*									m_pRewardItems[REWARD_V_LINE_COUNT][REWARD_H_LINE_COUNT];
public:
	int m_nSlotNumber;

private:
	CBasicTextBoxEx*	m_pListTextBox;
	CBasicTextBox*		m_pTextPage;
	CBasicScrollBarEx*	m_pListScrollBar;
	CBossListSlot*		m_pBossListSlot[BOSS_LIST_MAX_SLOT];
	CBasicTextBox*		m_pTextInfoLevel;
	CBasicTextBox*		m_pTextInfoHP;
	CBasicTextBox*		m_pTextInfoAttack;
	CBasicTextBox*		m_pTextInfoAvoid;
	CBasicTextBox*		m_pTextInfoGold;
	CMobPreviewWindowRender* m_pRender; //RENDER
	CBasicTextBox* m_pMobNameText;
	CBasicButton* m_pButtonLeft; 
	CBasicButton* m_pButtonRight; //END

private:
	int	m_nSelectIndexType;
	float m_fClickTimer;

public:
	void LOAD_MAP_LIST();
	void SelectMapRequest( DWORD dwType );
	void SetSlotItem (SNATIVEID sMobID, float fTimer, BOOL isAlive);
	CItemImage*					CreateItemImage ( const char* szControl, UIGUID ControlID );
	void GetDropItem (SNATIVEID sMobID);
	void SetBossInformation (SCROWDATA* pCROW);
	void ResetItems ();
	void ClearRender();
	void StartPreviewMob(SNATIVEID sMobID);

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
};