#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"
#include "../../Lib_Client/G-Logic/GLParty.h"

struct GLPARTY_CLIENT;
class CModernPartySlot;
class CBasicTextBox;
class CBasicComboBox;
class CBasicComboBoxRollOver;
class CBasicButton;
class CMultiModeButton;

class	CModernPartyWindowPageParty : public CUIGroup
{
protected:
	enum
	{
		PARTY_SLOT_MEMBER0 = NO_ID + 1,
		PARTY_SLOT_MEMBER1,
		PARTY_SLOT_MEMBER2,
		PARTY_SLOT_MEMBER3,
		PARTY_SLOT_MEMBER4,
		PARTY_SLOT_MEMBER5,
		PARTY_SLOT_MEMBER6,
		PARTY_SLOT_MEMBER7,
		PARTY_ITEM_COMBO_OPEN,
		PARTY_ITEM_COMBO_ROLLOVER,
		//PARTY_ITEMFILTER_COMBO_OPEN,
		//PARTY_ITEMFILTER_COMBO_ROLLOVER,
		PARTY_MONEY_COMBO_OPEN,		
		PARTY_MONEY_COMBO_ROLLOVER,
		PARTY_BUTTON_RELEASE,
		PARTY_BUTTON_BAN,
		PARTY_BUTTON_DELEGATE,
		PARTY_BUTTON_INFO,
		PARTY_BUTTON_WITHDRAW,
		PARTY_CHECKBOX_BUFF,
		PARTY_CHECKBOX_INFO,
		PARTY_CHECKBOX_PRIVATE
	};
	
public:
	CModernPartyWindowPageParty ();
	virtual	~CModernPartyWindowPageParty ();

public:
	void	CreateSubControl ();
	CBasicTextBox*	CreateStaticControl ( char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	CMultiModeButton* CreateButton( char* szControl, const char* szText, const UIGUID& cID = NO_ID );

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void	SetPartyInfo ( DWORD dwPartyID, DWORD dwMasterID );

	DWORD	GetPartyID ();
	DWORD	GetMasterID();

private:
	GLPARTY_CLIENT*		FindSelfClient ();
	void	InitButtons();
	BOOL	IsSameMap ( GLPARTY_CLIENT *pSelf, GLPARTY_CLIENT *pMember );

private:
	void	SetSlotMember ( CModernPartySlot* pPartySlot, GLPARTY_CLIENT *pMember, BOOL bSameMap );
	void	SetSelect( int nSelect );
	void	CheckChanges();

private:
	CModernPartySlot*		m_pPartySlot[MAXPARTY];
	CBasicComboBox*			m_pComboBoxItemOpen;
	//CBasicComboBox*			m_pComboBoxItemFilterOpen;
	CBasicComboBox*			m_pComboBoxMoneyOpen;	
	CBasicComboBoxRollOver*	m_pComboBoxItemRollOver;
	//CBasicComboBoxRollOver*	m_pComboBoxItemFilterRollOver;
	CBasicComboBoxRollOver*	m_pComboBoxMoneyRollOver;
	CMultiModeButton*		m_pButtonRelease;
	CMultiModeButton*		m_pButtonBan;
	CMultiModeButton*		m_pButtonDelegate;
	CMultiModeButton*		m_pButtonInfo;
	CMultiModeButton*		m_pButtonWithdraw;
	CBasicButton*			m_pCheckBoxBuff;
	CBasicButton*			m_pCheckBoxInfo;
	CBasicTextBox*			m_pTextNoParty;
	CBasicButton*			m_pCheckBoxPrivate;

private:
	SPARTY_OPT	m_sPartyOption;

private:
	DWORD	m_dwPartyID;
	DWORD	m_dwMasterID;
	int		m_nSelect;

private:
	UIGUID	m_RollOverID;
	BOOL	m_bFirstLBUP;
	BOOL	m_bMASTER;
	BOOL	m_bMEMBER;
};