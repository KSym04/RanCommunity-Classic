#include "StdAfx.h"
#include "./CharViewOptionEP9.h"

#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicComboBox.h"
#include "./BasicComboBoxRollOver.h"
#include "./BasicButton.h"
#include "./BasicTextButton.h"

#include "./UITextControl.h"
#include "./GameTextControl.h"
#include "./InnerInterface.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#include "../../Lib_Engine/Core/NSRProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*

CBasicCharViewOptionEP9::CBasicCharViewOptionEP9 ()
	: m_pFont(NULL)
	, m_RollOverID(NO_ID)
	, m_bFirstLBUP(FALSE)

	, m_pLineBoxMain(FALSE)

	, m_pButtonSelfApply(NULL)
	, m_pStaticSelfApply(NULL)

	, m_pButtonReset(NULL)

	, m_pStaticPartHead(NULL)
	, m_pStaticPartUpper(NULL)
	, m_pStaticPartLower(NULL)
	, m_pStaticPartGloves(NULL)
	, m_pStaticPartShoes(NULL)
	, m_pStaticPartWeapon(NULL)
	, m_pStaticPartNecklace(NULL)
	, m_pStaticPartBracelet(NULL)
	, m_pStaticPartRing(NULL)
	, m_pStaticPartBelt(NULL)
	, m_pStaticPartEarring(NULL)
	, m_pStaticPartAccessory(NULL)
	, m_pStaticPartDecoration(NULL)

	, m_pComboBoxPartHeadOpen(NULL)
	, m_pComboBoxPartUpperOpen(NULL)
	, m_pComboBoxPartLowerOpen(NULL)
	, m_pComboBoxPartGlovesOpen(NULL)
	, m_pComboBoxPartShoesOpen(NULL)
	, m_pComboBoxPartWeaponOpen(NULL)
	, m_pComboBoxPartNecklaceOpen(NULL)
	, m_pComboBoxPartBraceletOpen(NULL)
	, m_pComboBoxPartRingOpen(NULL)
	, m_pComboBoxPartBeltOpen(NULL)
	, m_pComboBoxPartEarringOpen(NULL)
	, m_pComboBoxPartAccessoryOpen(NULL)
	, m_pComboBoxPartDecorationOpen(NULL)

	, m_pComboBoxPartHeadRollOver(NULL)
	, m_pComboBoxPartUpperRollOver(NULL)
	, m_pComboBoxPartLowerRollOver(NULL)
	, m_pComboBoxPartGlovesRollOver(NULL)
	, m_pComboBoxPartShoesRollOver(NULL)
	, m_pComboBoxPartWeaponRollOver(NULL)
	, m_pComboBoxPartNecklaceRollOver(NULL)
	, m_pComboBoxPartBraceletRollOver(NULL)
	, m_pComboBoxPartRingRollOver(NULL)
	, m_pComboBoxPartBeltRollOver(NULL)
	, m_pComboBoxPartEarringRollOver(NULL)
	, m_pComboBoxPartAccessoryRollOver(NULL)
	, m_pComboBoxPartDecorationRollOver(NULL)

	, m_pStaticVehicleSimple(NULL)
	, m_pButtonVehicleSimple(NULL)

	, m_emRenderHead(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderUpper(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderLower(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderGloves(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderShoes(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderWeapon(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderNeckLace(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderBracelet(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderRing(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderBelt(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderEarring(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderAccessory(ERENDER_SETTING_TYPE_DEFAULT)
	, m_emRenderDecoration(ERENDER_SETTING_TYPE_DEFAULT)

	, m_bVehicleSimple(FALSE)
	, m_bApplySelf(FALSE)
{
}

CBasicCharViewOptionEP9::~CBasicCharViewOptionEP9 ()
{
}

void CBasicCharViewOptionEP9::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pStaticSelfApply = new CBasicTextBox;
	m_pStaticSelfApply->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_SELF_APPLY" );
	m_pStaticSelfApply->SetFont ( m_pFont );
	m_pStaticSelfApply->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticSelfApply->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_OPTION", 0 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticSelfApply );

	m_pButtonSelfApply = new CBasicButton;
	m_pButtonSelfApply->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_BUTTON_SELF_APPLY", UI_FLAG_DEFAULT, CHARACTER_RENDER_SETTING_WINDOW_SELF_APPLY );
	m_pButtonSelfApply->CreateFlip ( "EP9_VIEW_CHARACTER_RENDER_SETTING_BUTTON_SELF_APPLY_F", CBasicButton::RADIO_FLIP );
	m_pButtonSelfApply->SetControlNameEx ( "EP9_VIEW_CHARACTER_RENDER_SETTING_BUTTON_SELF_APPLY" );
	RegisterControl ( m_pButtonSelfApply );

	m_pStaticVehicleSimple = new CBasicTextBox;
	m_pStaticVehicleSimple->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_VEHICLE_SIMPLE" );
	m_pStaticVehicleSimple->SetFont ( m_pFont );
	m_pStaticVehicleSimple->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticVehicleSimple->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_OPTION", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticVehicleSimple );

	m_pButtonVehicleSimple = new CBasicButton;
	m_pButtonVehicleSimple->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_BUTTON_VEHICLE_SIMPLE", UI_FLAG_DEFAULT, CHARACTER_RENDER_SETTING_WINDOW_VEHICLE_SIMPLE );
	m_pButtonVehicleSimple->CreateFlip ( "EP9_VIEW_CHARACTER_RENDER_SETTING_BUTTON_VEHICLE_SIMPLE_F", CBasicButton::RADIO_FLIP );
	m_pButtonVehicleSimple->SetControlNameEx ( "EP9_VIEW_CHARACTER_RENDER_SETTING_BUTTON_VEHICLE_SIMPLE" );
	RegisterControl ( m_pButtonVehicleSimple );

	m_pButtonReset = new CBasicTextButton;
	m_pButtonReset->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_BUTTON_RESET );
	m_pButtonReset->CreateBaseButton ( "EP9_VIEW_CHARACTER_RENDER_SETTING_BUTTON_RESET",  CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_OPTION", 2 ) );
	RegisterControl ( m_pButtonReset );

	m_pStaticPartHead = new CBasicTextBox;
	m_pStaticPartHead->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_HEAD" );
	m_pStaticPartHead->SetFont ( m_pFont );
	m_pStaticPartHead->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartHead->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 0 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartHead );

	m_pStaticPartUpper = new CBasicTextBox;
	m_pStaticPartUpper->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_UPPER" );
	m_pStaticPartUpper->SetFont ( m_pFont );
	m_pStaticPartUpper->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartUpper->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 1 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartUpper );

	m_pStaticPartLower = new CBasicTextBox;
	m_pStaticPartLower->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_LOWER" );
	m_pStaticPartLower->SetFont ( m_pFont );
	m_pStaticPartLower->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartLower->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 2 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartLower );

	m_pStaticPartGloves = new CBasicTextBox;
	m_pStaticPartGloves->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_GLOVES" );
	m_pStaticPartGloves->SetFont ( m_pFont );
	m_pStaticPartGloves->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartGloves->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 3 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartGloves );

	m_pStaticPartShoes = new CBasicTextBox;
	m_pStaticPartShoes->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_SHOES" );
	m_pStaticPartShoes->SetFont ( m_pFont );
	m_pStaticPartShoes->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartShoes->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 4 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartShoes );

	m_pStaticPartWeapon = new CBasicTextBox;
	m_pStaticPartWeapon->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_WEAPON" );
	m_pStaticPartWeapon->SetFont ( m_pFont );
	m_pStaticPartWeapon->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartWeapon->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 5 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartWeapon );

	m_pStaticPartNecklace = new CBasicTextBox;
	m_pStaticPartNecklace->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_NECKLACE" );
	m_pStaticPartNecklace->SetFont ( m_pFont );
	m_pStaticPartNecklace->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartNecklace->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 6 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartNecklace );

	m_pStaticPartBracelet = new CBasicTextBox;
	m_pStaticPartBracelet->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_BRACELET" );
	m_pStaticPartBracelet->SetFont ( m_pFont );
	m_pStaticPartBracelet->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartBracelet->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 7 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartBracelet );

	m_pStaticPartRing = new CBasicTextBox;
	m_pStaticPartRing->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_RING" );
	m_pStaticPartRing->SetFont ( m_pFont );
	m_pStaticPartRing->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartRing->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 8 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartRing );

	m_pStaticPartBelt = new CBasicTextBox;
	m_pStaticPartBelt->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_BELT" );
	m_pStaticPartBelt->SetFont ( m_pFont );
	m_pStaticPartBelt->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartBelt->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 9 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartBelt );

	m_pStaticPartEarring = new CBasicTextBox;
	m_pStaticPartEarring->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_EARRING" );
	m_pStaticPartEarring->SetFont ( m_pFont );
	m_pStaticPartEarring->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartEarring->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 10 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartEarring );

	m_pStaticPartAccessory = new CBasicTextBox;
	m_pStaticPartAccessory->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_ACCESSORY" );
	m_pStaticPartAccessory->SetFont ( m_pFont );
	m_pStaticPartAccessory->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartAccessory->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 11 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartAccessory );

	m_pStaticPartDecoration = new CBasicTextBox;
	m_pStaticPartDecoration->CreateSub ( this, "EP9_VIEW_CHARACTER_RENDER_SETTING_STATIC_PART_DECORATION" );
	m_pStaticPartDecoration->SetFont ( m_pFont );
	m_pStaticPartDecoration->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pStaticPartDecoration->SetOneLineText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_PARTS", 12 ), NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pStaticPartDecoration );



	m_pComboBoxPartHeadOpen = new CBasicComboBox;
	m_pComboBoxPartHeadOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_HEAD_COMBO_OPEN );
	m_pComboBoxPartHeadOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_HEAD_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartHeadOpen );

	m_pComboBoxPartUpperOpen = new CBasicComboBox;
	m_pComboBoxPartUpperOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_UPPER_COMBO_OPEN );
	m_pComboBoxPartUpperOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_UPPER_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartUpperOpen );

	m_pComboBoxPartLowerOpen = new CBasicComboBox;
	m_pComboBoxPartLowerOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_LOWER_COMBO_OPEN );
	m_pComboBoxPartLowerOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_LOWER_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartLowerOpen );

	m_pComboBoxPartGlovesOpen = new CBasicComboBox;
	m_pComboBoxPartGlovesOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_GLOVES_COMBO_OPEN );
	m_pComboBoxPartGlovesOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_GLOVES_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartGlovesOpen );

	m_pComboBoxPartShoesOpen = new CBasicComboBox;
	m_pComboBoxPartShoesOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_SHOES_COMBO_OPEN );
	m_pComboBoxPartShoesOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_SHOES_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartShoesOpen );

	m_pComboBoxPartWeaponOpen = new CBasicComboBox;
	m_pComboBoxPartWeaponOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_WEAPON_COMBO_OPEN );
	m_pComboBoxPartWeaponOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_WEAPON_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartWeaponOpen );

	m_pComboBoxPartNecklaceOpen = new CBasicComboBox;
	m_pComboBoxPartNecklaceOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_NECKLACE_COMBO_OPEN );
	m_pComboBoxPartNecklaceOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_NECKLACE_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartNecklaceOpen );

	m_pComboBoxPartBraceletOpen = new CBasicComboBox;
	m_pComboBoxPartBraceletOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_BRACELET_COMBO_OPEN );
	m_pComboBoxPartBraceletOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_BRACELET_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartBraceletOpen );

	m_pComboBoxPartRingOpen = new CBasicComboBox;
	m_pComboBoxPartRingOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_RING_COMBO_OPEN );
	m_pComboBoxPartRingOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_RING_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartRingOpen );

	m_pComboBoxPartBeltOpen = new CBasicComboBox;
	m_pComboBoxPartBeltOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_BELT_COMBO_OPEN );
	m_pComboBoxPartBeltOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_BELT_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartBeltOpen );

	m_pComboBoxPartEarringOpen = new CBasicComboBox;
	m_pComboBoxPartEarringOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_EARRING_COMBO_OPEN );
	m_pComboBoxPartEarringOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_EARRING_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartEarringOpen );

	m_pComboBoxPartAccessoryOpen = new CBasicComboBox;
	m_pComboBoxPartAccessoryOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_ACCESSORY_COMBO_OPEN );
	m_pComboBoxPartAccessoryOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_ACCESSORY_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartAccessoryOpen );

	m_pComboBoxPartDecorationOpen = new CBasicComboBox;
	m_pComboBoxPartDecorationOpen->CreateSub ( this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_DECORATION_COMBO_OPEN );
	m_pComboBoxPartDecorationOpen->CreateBaseComboBox ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_DECORATION_COMBO_OPEN" );			
	RegisterControl ( m_pComboBoxPartDecorationOpen );



	m_pComboBoxPartHeadRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartHeadRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_HEAD_COMBO_ROLLOVER );
	m_pComboBoxPartHeadRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_HEAD_COMBO_ROLLOVER" );			
	m_pComboBoxPartHeadRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartHeadRollOver );

	m_pComboBoxPartUpperRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartUpperRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_UPPER_COMBO_ROLLOVER );
	m_pComboBoxPartUpperRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_UPPER_COMBO_ROLLOVER" );			
	m_pComboBoxPartUpperRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartUpperRollOver );

	m_pComboBoxPartLowerRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartLowerRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_LOWER_COMBO_ROLLOVER );
	m_pComboBoxPartLowerRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_LOWER_COMBO_ROLLOVER" );			
	m_pComboBoxPartLowerRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartLowerRollOver );

	m_pComboBoxPartGlovesRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartGlovesRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_GLOVES_COMBO_ROLLOVER );
	m_pComboBoxPartGlovesRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_GLOVES_COMBO_ROLLOVER" );			
	m_pComboBoxPartGlovesRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartGlovesRollOver );

	m_pComboBoxPartShoesRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartShoesRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_SHOES_COMBO_ROLLOVER );
	m_pComboBoxPartShoesRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_SHOES_COMBO_ROLLOVER" );			
	m_pComboBoxPartShoesRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartShoesRollOver );

	m_pComboBoxPartWeaponRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartWeaponRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_WEAPON_COMBO_ROLLOVER );
	m_pComboBoxPartWeaponRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_WEAPON_COMBO_ROLLOVER" );			
	m_pComboBoxPartWeaponRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartWeaponRollOver );

	m_pComboBoxPartNecklaceRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartNecklaceRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_NECKLACE_COMBO_ROLLOVER );
	m_pComboBoxPartNecklaceRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_NECKLACE_COMBO_ROLLOVER" );			
	m_pComboBoxPartNecklaceRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartNecklaceRollOver );

	m_pComboBoxPartBraceletRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartBraceletRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_BRACELET_COMBO_ROLLOVER );
	m_pComboBoxPartBraceletRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_BRACELET_COMBO_ROLLOVER" );			
	m_pComboBoxPartBraceletRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartBraceletRollOver );

	m_pComboBoxPartRingRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartRingRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_RING_COMBO_ROLLOVER );
	m_pComboBoxPartRingRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_RING_COMBO_ROLLOVER" );			
	m_pComboBoxPartRingRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartRingRollOver );

	m_pComboBoxPartBeltRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartBeltRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_BELT_COMBO_ROLLOVER );
	m_pComboBoxPartBeltRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_BELT_COMBO_ROLLOVER" );			
	m_pComboBoxPartBeltRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartBeltRollOver );

	m_pComboBoxPartEarringRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartEarringRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_EARRING_COMBO_ROLLOVER );
	m_pComboBoxPartEarringRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_EARRING_COMBO_ROLLOVER" );			
	m_pComboBoxPartEarringRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartEarringRollOver );

	m_pComboBoxPartAccessoryRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartAccessoryRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_ACCESSORY_COMBO_ROLLOVER );
	m_pComboBoxPartAccessoryRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_ACCESSORY_COMBO_ROLLOVER" );			
	m_pComboBoxPartAccessoryRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartAccessoryRollOver );

	m_pComboBoxPartDecorationRollOver = new CBasicComboBoxRollOver;
	m_pComboBoxPartDecorationRollOver->CreateSub ( this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, CHARACTER_RENDER_SETTING_WINDOW_PART_DECORATION_COMBO_ROLLOVER );
	m_pComboBoxPartDecorationRollOver->CreateBaseComboBoxRollOver ( "EP9_VIEW_CHARACTER_RENDER_SETTING_PART_DECORATION_COMBO_ROLLOVER" );			
	m_pComboBoxPartDecorationRollOver->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pComboBoxPartDecorationRollOver );
}

void CBasicCharViewOptionEP9::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( m_RollOverID == NO_ID )
	{
		CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	}
	else
	{
		ResetMessageEx ();

		CUIControl::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		CUIControl* pControl = m_ControlContainer.FindControl ( m_RollOverID );
		if ( !pControl )
		{
			GASSERT ( 0 && "Invalid RollOver ID." );
			return;
		}

		pControl->Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

		DWORD dwControlMsg = pControl->GetMessageEx ();
		NS_UIDEBUGSET::BlockBegin ();
		if ( dwControlMsg ) TranslateUIMessage ( m_RollOverID, dwControlMsg );
		NS_UIDEBUGSET::BlockEnd ();

		if ( !(dwControlMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL) )
		{
			DWORD dwMsg = GetMessageEx ();
			if ( dwMsg & UIMSG_LB_UP )
			{
				if ( !m_bFirstLBUP )
				{
					m_RollOverID = NO_ID;
					pControl->SetVisibleSingle ( FALSE );					
				}
				m_bFirstLBUP = FALSE;
			}
		}
	}
}

void CBasicCharViewOptionEP9::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case CHARACTER_RENDER_SETTING_WINDOW_SELF_APPLY:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bApplySelf = !m_bApplySelf;

				if ( m_pButtonSelfApply )
					m_pButtonSelfApply->SetFlip( m_bApplySelf );
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_VEHICLE_SIMPLE:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bVehicleSimple = !m_bVehicleSimple;

				if ( m_pButtonVehicleSimple )
					m_pButtonVehicleSimple->SetFlip( m_bVehicleSimple );
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_HEAD_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_HEAD_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_UPPER_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_UPPER_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_LOWER_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_LOWER_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_GLOVES_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_GLOVES_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_SHOES_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_SHOES_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_WEAPON_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_WEAPON_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_NECKLACE_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_NECKLACE_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_BRACELET_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_BRACELET_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_RING_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_RING_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_BELT_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_BELT_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_EARRING_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_EARRING_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_ACCESSORY_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(TRUE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(FALSE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_ACCESSORY_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_DECORATION_COMBO_OPEN:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					m_pComboBoxPartHeadRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartUpperRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartLowerRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartGlovesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartShoesRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartWeaponRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartNecklaceRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBraceletRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartRingRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartBeltRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartEarringRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartAccessoryRollOver->SetVisibleSingle(FALSE);
					m_pComboBoxPartDecorationRollOver->SetVisibleSingle(TRUE);

					m_RollOverID = CHARACTER_RENDER_SETTING_WINDOW_PART_DECORATION_COMBO_ROLLOVER;
					m_bFirstLBUP = TRUE;
				}
			}
		}break;


	case CHARACTER_RENDER_SETTING_WINDOW_PART_HEAD_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartHeadRollOver )
				{
					int nIndex = m_pComboBoxPartHeadRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderHead = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderHead, m_pComboBoxPartHeadOpen );
				}	
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_UPPER_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartUpperRollOver )
				{
					int nIndex = m_pComboBoxPartUpperRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderUpper = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderUpper, m_pComboBoxPartUpperOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_LOWER_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartLowerRollOver )
				{
					int nIndex = m_pComboBoxPartLowerRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderLower = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderLower, m_pComboBoxPartLowerOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_GLOVES_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartGlovesRollOver )
				{
					int nIndex = m_pComboBoxPartGlovesRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderGloves = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderGloves, m_pComboBoxPartGlovesOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_SHOES_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartShoesRollOver )
				{
					int nIndex = m_pComboBoxPartShoesRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderShoes = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderShoes, m_pComboBoxPartShoesOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_WEAPON_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartWeaponRollOver )
				{
					int nIndex = m_pComboBoxPartWeaponRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderWeapon = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderWeapon, m_pComboBoxPartWeaponOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_NECKLACE_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartNecklaceRollOver )
				{
					int nIndex = m_pComboBoxPartNecklaceRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderNeckLace = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderNeckLace, m_pComboBoxPartNecklaceOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_BRACELET_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartBraceletRollOver )
				{
					int nIndex = m_pComboBoxPartBraceletRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderBracelet = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderBracelet, m_pComboBoxPartBraceletOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_RING_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartRingRollOver )
				{
					int nIndex = m_pComboBoxPartRingRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderRing = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderRing, m_pComboBoxPartRingOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_BELT_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartBeltRollOver )
				{
					int nIndex = m_pComboBoxPartBeltRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderBelt = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderBelt, m_pComboBoxPartBeltOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_EARRING_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartEarringRollOver )
				{
					int nIndex = m_pComboBoxPartEarringRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderEarring = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderEarring, m_pComboBoxPartEarringOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_ACCESSORY_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartAccessoryRollOver )
				{
					int nIndex = m_pComboBoxPartAccessoryRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderAccessory = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderAccessory, m_pComboBoxPartAccessoryOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_PART_DECORATION_COMBO_ROLLOVER:
		{
			if ( dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL )
			{				
				DWORD dwMsg = GetMessageEx ();
				dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
				ResetMessageEx ();
				AddMessageEx ( dwMsg );
				break;
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{			
				if ( m_pComboBoxPartDecorationRollOver )
				{
					int nIndex = m_pComboBoxPartDecorationRollOver->GetSelectIndex ();
					if ( nIndex < 0 ) return;

					m_emRenderDecoration = (ERENDER_SETTING_TYPE)nIndex;
					SetTextComboPart( m_emRenderDecoration, m_pComboBoxPartDecorationOpen );
				}
			}
		}break;

	case CHARACTER_RENDER_SETTING_WINDOW_BUTTON_RESET:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				ResetOption();
			}
		}break;

	};
}

void CBasicCharViewOptionEP9::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		LoadData();
		LoadCurrentOption();
	}
}

void CBasicCharViewOptionEP9::LoadData()
{
	if ( m_pComboBoxPartHeadRollOver )
	{
		m_pComboBoxPartHeadRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartHeadRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartUpperRollOver )
	{
		m_pComboBoxPartUpperRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartUpperRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartLowerRollOver )
	{
		m_pComboBoxPartLowerRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartLowerRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartGlovesRollOver )
	{
		m_pComboBoxPartGlovesRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartGlovesRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartShoesRollOver )
	{
		m_pComboBoxPartShoesRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartShoesRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartWeaponRollOver )
	{
		m_pComboBoxPartWeaponRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartWeaponRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartNecklaceRollOver )
	{
		m_pComboBoxPartNecklaceRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartNecklaceRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartBraceletRollOver )
	{
		m_pComboBoxPartBraceletRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartBraceletRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartRingRollOver )
	{
		m_pComboBoxPartRingRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartRingRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartBeltRollOver )
	{
		m_pComboBoxPartBeltRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartBeltRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartEarringRollOver )
	{
		m_pComboBoxPartEarringRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartEarringRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartAccessoryRollOver )
	{
		m_pComboBoxPartAccessoryRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartAccessoryRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}

	if ( m_pComboBoxPartDecorationRollOver )
	{
		m_pComboBoxPartDecorationRollOver->ClearText ();
		for ( int i = 0; i < ERENDER_SETTING_TYPE_SIZE; i++ )
		{
			m_pComboBoxPartDecorationRollOver->AddText ( ID2GAMEWORD ( "EP9_VIEW_CHARACTER_RENDER_SETTING_WINDOW_OPTION", i ) );
		}
	}
}

void CBasicCharViewOptionEP9::LoadCurrentOption ()
{
	m_emRenderHead			= RPROFILE::emRenderHead;
	m_emRenderUpper			= RPROFILE::emRenderUpper;
	m_emRenderLower			= RPROFILE::emRenderLower;
	m_emRenderGloves		= RPROFILE::emRenderGloves;
	m_emRenderShoes			= RPROFILE::emRenderShoes;
	m_emRenderWeapon		= RPROFILE::emRenderWeapon;
	m_emRenderNeckLace		= RPROFILE::emRenderNeckLace;
	m_emRenderBracelet		= RPROFILE::emRenderBracelet;
	m_emRenderRing			= RPROFILE::emRenderRing;
	m_emRenderBelt			= RPROFILE::emRenderBelt;
	m_emRenderEarring		= RPROFILE::emRenderEarring;
	m_emRenderAccessory		= RPROFILE::emRenderAccessory;
	m_emRenderDecoration	= RPROFILE::emRenderDecoration;
	m_bVehicleSimple		= RPROFILE::bVehicleSimple;
	m_bApplySelf			= RPROFILE::bApplySelf;

	SetTextComboPart( m_emRenderHead, m_pComboBoxPartHeadOpen );
	SetTextComboPart( m_emRenderUpper, m_pComboBoxPartUpperOpen );
	SetTextComboPart( m_emRenderLower, m_pComboBoxPartLowerOpen );
	SetTextComboPart( m_emRenderGloves, m_pComboBoxPartGlovesOpen );
	SetTextComboPart( m_emRenderShoes, m_pComboBoxPartShoesOpen );
	SetTextComboPart( m_emRenderWeapon, m_pComboBoxPartWeaponOpen );
	SetTextComboPart( m_emRenderNeckLace, m_pComboBoxPartNecklaceOpen );
	SetTextComboPart( m_emRenderBracelet, m_pComboBoxPartBraceletOpen );
	SetTextComboPart( m_emRenderRing, m_pComboBoxPartRingOpen );
	SetTextComboPart( m_emRenderBelt, m_pComboBoxPartBeltOpen );
	SetTextComboPart( m_emRenderEarring, m_pComboBoxPartEarringOpen );
	SetTextComboPart( m_emRenderAccessory, m_pComboBoxPartAccessoryOpen );
	SetTextComboPart( m_emRenderDecoration, m_pComboBoxPartDecorationOpen );

	if ( m_pButtonVehicleSimple )
		m_pButtonVehicleSimple->SetFlip( m_bVehicleSimple );

	if ( m_pButtonSelfApply )
		m_pButtonSelfApply->SetFlip( m_bApplySelf );
}

void CBasicCharViewOptionEP9::ResetOption ()
{
	m_emRenderHead			= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderUpper			= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderLower			= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderGloves		= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderShoes			= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderWeapon		= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderNeckLace		= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderBracelet		= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderRing			= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderBelt			= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderEarring		= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderAccessory		= ERENDER_SETTING_TYPE_DEFAULT;
	m_emRenderDecoration	= ERENDER_SETTING_TYPE_DEFAULT;
	m_bVehicleSimple		= FALSE;
	m_bApplySelf			= FALSE;

	SetTextComboPart( m_emRenderHead, m_pComboBoxPartHeadOpen );
	SetTextComboPart( m_emRenderUpper, m_pComboBoxPartUpperOpen );
	SetTextComboPart( m_emRenderLower, m_pComboBoxPartLowerOpen );
	SetTextComboPart( m_emRenderGloves, m_pComboBoxPartGlovesOpen );
	SetTextComboPart( m_emRenderShoes, m_pComboBoxPartShoesOpen );
	SetTextComboPart( m_emRenderWeapon, m_pComboBoxPartWeaponOpen );
	SetTextComboPart( m_emRenderNeckLace, m_pComboBoxPartNecklaceOpen );
	SetTextComboPart( m_emRenderBracelet, m_pComboBoxPartBraceletOpen );
	SetTextComboPart( m_emRenderRing, m_pComboBoxPartRingOpen );
	SetTextComboPart( m_emRenderBelt, m_pComboBoxPartBeltOpen );
	SetTextComboPart( m_emRenderEarring, m_pComboBoxPartEarringOpen );
	SetTextComboPart( m_emRenderAccessory, m_pComboBoxPartAccessoryOpen );
	SetTextComboPart( m_emRenderDecoration, m_pComboBoxPartDecorationOpen );

	if ( m_pButtonVehicleSimple )
		m_pButtonVehicleSimple->SetFlip( m_bVehicleSimple );

	if ( m_pButtonSelfApply )
		m_pButtonSelfApply->SetFlip( m_bApplySelf );
}

void CBasicCharViewOptionEP9::SetTextComboPart( DWORD dwIndex, CBasicComboBox* pComboBox )
{
	if ( 0 <= dwIndex && dwIndex < ERENDER_SETTING_TYPE_SIZE )
	{
		CString strTemp( ID2GAMEWORD ( "CHARACTER_RENDER_SETTING_WINDOW_OPTION", dwIndex ) );

		if ( pComboBox )
			pComboBox->SetText ( strTemp );
	}
}
*/