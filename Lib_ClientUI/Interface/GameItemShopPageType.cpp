#include "stdafx.h"

#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./InnerInterface.h"

#include "./GameItemShopPageType.h"
#include "./GameItemShopPageTypeBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGameItemShopPageType::CGameItemShopPageType ()
	: m_nSelect(0)
{
	for( int i=0; i<GAME_ITEM_SHOP_TYPE_SIZE; ++i )
		m_pIconType[i] = NULL;
}

CGameItemShopPageType::~CGameItemShopPageType ()
{
}

void CGameItemShopPageType::CreateSubControl ()
{
	std::string stricon[GAME_ITEM_SHOP_TYPE_SIZE] = 
	{
		"GAME_ITEM_SHOP_TYPE_SPECIAL_BASE",
		"GAME_ITEM_SHOP_TYPE_CONSUMABLE_BASE",
		"GAME_ITEM_SHOP_TYPE_WEARABLE_BASE",
		"GAME_ITEM_SHOP_TYPE_ACCESSORY_BASE",
		"GAME_ITEM_SHOP_TYPE_PET_BASE",
		"GAME_ITEM_SHOP_TYPE_VEHICLE_BASE",
	};

	std::string stricon_base[GAME_ITEM_SHOP_TYPE_SIZE] = 
	{
		"GAME_ITEM_SHOP_TYPE_SPECIAL_INACTIVE",
		"GAME_ITEM_SHOP_TYPE_CONSUMABLE_INACTIVE",
		"GAME_ITEM_SHOP_TYPE_WEARABLE_INACTIVE",
		"GAME_ITEM_SHOP_TYPE_ACCESSORY_INACTIVE",
		"GAME_ITEM_SHOP_TYPE_PET_INACTIVE",
		"GAME_ITEM_SHOP_TYPE_VEHICLE_INACTIVE",
	};

	std::string stricon_select[GAME_ITEM_SHOP_TYPE_SIZE] = 
	{
		"GAME_ITEM_SHOP_TYPE_SPECIAL_ACTIVE",
		"GAME_ITEM_SHOP_TYPE_CONSUMABLE_ACTIVE",
		"GAME_ITEM_SHOP_TYPE_WEARABLE_ACTIVE",
		"GAME_ITEM_SHOP_TYPE_ACCESSORY_ACTIVE",
		"GAME_ITEM_SHOP_TYPE_PET_ACTIVE",
		"GAME_ITEM_SHOP_TYPE_VEHICLE_ACTIVE",
	};

	std::string stricon_over[GAME_ITEM_SHOP_TYPE_SIZE] = 
	{
		"GAME_ITEM_SHOP_TYPE_SPECIAL_OVER",
		"GAME_ITEM_SHOP_TYPE_CONSUMABLE_OVER",
		"GAME_ITEM_SHOP_TYPE_WEARABLE_OVER",
		"GAME_ITEM_SHOP_TYPE_ACCESSORY_OVER",
		"GAME_ITEM_SHOP_TYPE_PET_OVER",
		"GAME_ITEM_SHOP_TYPE_VEHICLE_OVER",
	};

	for( int i=0; i<GAME_ITEM_SHOP_TYPE_SIZE; ++i )
	{
		m_pIconType[i] = new CGameItemShopPageTypeBase;
		m_pIconType[i]->CreateSub ( this, stricon[i].c_str(), UI_FLAG_DEFAULT, GAME_ITEM_SHOP_TYPE_SPECIAL + i );
		m_pIconType[i]->CreateSubControl ( stricon_base[i].c_str(), stricon_select[i].c_str(), stricon_over[i].c_str() );
		RegisterControl ( m_pIconType[i] );
	}

	DefaultSelect();
}

void CGameItemShopPageType::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case GAME_ITEM_SHOP_TYPE_SPECIAL:
	case GAME_ITEM_SHOP_TYPE_CONSUMABLE:
	case GAME_ITEM_SHOP_TYPE_WEARABLE:
	case GAME_ITEM_SHOP_TYPE_ACCESSORY:
	case GAME_ITEM_SHOP_TYPE_PET:
	case GAME_ITEM_SHOP_TYPE_VEHICLE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - GAME_ITEM_SHOP_TYPE_SPECIAL;

				if ( nSelect >= 0 && nSelect < GAME_ITEM_SHOP_TYPE_SIZE )
				{
					if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
					{
						m_nSelect = nSelect;

						AddMessageEx ( UIMSG_GAME_ITEM_SHOP_SELECT );

						for ( int i=0; i<GAME_ITEM_SHOP_TYPE_SIZE; ++i )
						{
							if ( m_pIconType[i] )
								m_pIconType[i]->SetSelect( i == nSelect );
						}
					}

					CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine( ID2GAMEWORD( "GAME_ITEM_SHOP_PAGE_CATEGORY", nSelect ), NS_UITEXTCOLOR::WHITE );
				}
			}
		}break;
	};
}

void CGameItemShopPageType::DefaultSelect()
{
	for ( int i=0; i<GAME_ITEM_SHOP_TYPE_SIZE; ++i )
	{
		if ( m_pIconType[i] )
			m_pIconType[i]->SetSelect( i == 0 );
	}

	m_nSelect = 0;
}

