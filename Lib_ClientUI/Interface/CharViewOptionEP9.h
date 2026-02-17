#pragma	once
/*
#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/Core/NSRProfileDef.h"

class	CBasicLineBox;
class	CBasicTextBox;
class	CD3DFontPar;
class	CBasicComboBox;
class	CBasicComboBoxRollOver;
class	CBasicButton;
class	CBasicTextButton;

class	CBasicCharViewOptionEP9 : public CUIGroup
{
public:

protected:
	enum
	{
		CHARACTER_RENDER_SETTING_WINDOW_PART_HEAD_COMBO_OPEN = NO_ID + 1,	
		CHARACTER_RENDER_SETTING_WINDOW_PART_HEAD_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_UPPER_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_UPPER_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_LOWER_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_LOWER_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_GLOVES_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_GLOVES_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_SHOES_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_SHOES_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_WEAPON_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_WEAPON_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_NECKLACE_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_NECKLACE_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_BRACELET_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_BRACELET_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_RING_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_RING_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_BELT_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_BELT_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_EARRING_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_EARRING_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_ACCESSORY_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_ACCESSORY_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_PART_DECORATION_COMBO_OPEN,
		CHARACTER_RENDER_SETTING_WINDOW_PART_DECORATION_COMBO_ROLLOVER,
		CHARACTER_RENDER_SETTING_WINDOW_VEHICLE_SIMPLE,
		CHARACTER_RENDER_SETTING_WINDOW_SELF_APPLY,
		CHARACTER_RENDER_SETTING_WINDOW_BUTTON_RESET,
	};

public:
	CBasicCharViewOptionEP9 ();
	virtual	~CBasicCharViewOptionEP9 ();

public:
	void	CreateSubControl ();

private:
	CD3DFontPar*				m_pFont;

	CBasicLineBox*				m_pLineBoxMain;

	CBasicTextBox*				m_pStaticSelfApply;
	CBasicButton*				m_pButtonSelfApply;

	CBasicTextButton*			m_pButtonReset;

	CBasicTextBox*				m_pStaticPartHead;
	CBasicTextBox*				m_pStaticPartUpper;
	CBasicTextBox*				m_pStaticPartLower;
	CBasicTextBox*				m_pStaticPartGloves;
	CBasicTextBox*				m_pStaticPartShoes;
	CBasicTextBox*				m_pStaticPartWeapon;
	CBasicTextBox*				m_pStaticPartNecklace;
	CBasicTextBox*				m_pStaticPartBracelet;
	CBasicTextBox*				m_pStaticPartRing;
	CBasicTextBox*				m_pStaticPartBelt;
	CBasicTextBox*				m_pStaticPartEarring;
	CBasicTextBox*				m_pStaticPartAccessory;
	CBasicTextBox*				m_pStaticPartDecoration;


	CBasicComboBox*				m_pComboBoxPartHeadOpen;
	CBasicComboBox*				m_pComboBoxPartUpperOpen;
	CBasicComboBox*				m_pComboBoxPartLowerOpen;
	CBasicComboBox*				m_pComboBoxPartGlovesOpen;
	CBasicComboBox*				m_pComboBoxPartShoesOpen;
	CBasicComboBox*				m_pComboBoxPartWeaponOpen;
	CBasicComboBox*				m_pComboBoxPartNecklaceOpen;
	CBasicComboBox*				m_pComboBoxPartBraceletOpen;
	CBasicComboBox*				m_pComboBoxPartRingOpen;
	CBasicComboBox*				m_pComboBoxPartBeltOpen;
	CBasicComboBox*				m_pComboBoxPartEarringOpen;
	CBasicComboBox*				m_pComboBoxPartAccessoryOpen;
	CBasicComboBox*				m_pComboBoxPartDecorationOpen;


	CBasicComboBoxRollOver*		m_pComboBoxPartHeadRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartUpperRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartLowerRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartGlovesRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartShoesRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartWeaponRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartNecklaceRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartBraceletRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartRingRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartBeltRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartEarringRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartAccessoryRollOver;
	CBasicComboBoxRollOver*		m_pComboBoxPartDecorationRollOver;

	CBasicTextBox*				m_pStaticVehicleSimple;
	CBasicButton*				m_pButtonVehicleSimple;

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void LoadData();
	void LoadCurrentOption ();
	void ResetOption();

	void SetTextComboPart( DWORD dwIndex, CBasicComboBox* pComboBox );

private:
	UIGUID	m_RollOverID;
	BOOL	m_bFirstLBUP;

public:
	ERENDER_SETTING_TYPE	m_emRenderHead;
	ERENDER_SETTING_TYPE	m_emRenderUpper;
	ERENDER_SETTING_TYPE	m_emRenderLower;
	ERENDER_SETTING_TYPE	m_emRenderGloves;
	ERENDER_SETTING_TYPE	m_emRenderShoes;
	ERENDER_SETTING_TYPE	m_emRenderWeapon;
	ERENDER_SETTING_TYPE	m_emRenderNeckLace;
	ERENDER_SETTING_TYPE	m_emRenderBracelet;
	ERENDER_SETTING_TYPE	m_emRenderRing;
	ERENDER_SETTING_TYPE	m_emRenderBelt;
	ERENDER_SETTING_TYPE	m_emRenderEarring;
	ERENDER_SETTING_TYPE	m_emRenderAccessory;
	ERENDER_SETTING_TYPE	m_emRenderDecoration;
	BOOL					m_bVehicleSimple;
	BOOL					m_bApplySelf;

};
*/