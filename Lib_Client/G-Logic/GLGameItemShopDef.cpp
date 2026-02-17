#include "stdafx.h"
#include "./GLGameItemShopDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SGAME_ITEM_SHOP_DATA::SGAME_ITEM_SHOP_DATA()
	: wIndex(GAME_ITEM_SHOP_INDEX_NULL)
	, wProductNum(0)
	, sItemID(false)
	, wCategory(0)
	, wStock(0)
	, wPrice(0)
	, wType(0)
{

}

SGAME_ITEM_SHOP_DATA::~SGAME_ITEM_SHOP_DATA()
{

}

bool SGAME_ITEM_SHOP_DATA::operator < ( const SGAME_ITEM_SHOP_DATA& sData )
{			
	if ( wIndex < sData.wIndex )
		return true;
	else if ( wIndex == sData.wIndex && wProductNum < sData.wProductNum )
		return true;

	return  false;
}

SGAME_ITEM_SHOP_DATA& SGAME_ITEM_SHOP_DATA::operator= ( const SGAME_ITEM_SHOP_DATA &cData )
{
	if( this == &cData )	return *this;

	wIndex			= cData.wIndex;
	wProductNum		= cData.wProductNum;
	sItemID			= cData.sItemID;

	wCategory		= cData.wCategory;
	wStock			= cData.wStock;
	wPrice			= cData.wPrice;

	wType			= cData.wType;



	return *this;
}

SGAME_ITEM_SHOP_POINT_INFO::SGAME_ITEM_SHOP_POINT_INFO()
	: dwUserID(0)
	, dwUserPoint(0)
	, dwVotePoint(0)
	, bUserPointUpdate(FALSE)
	, bVotePointUpdate(FALSE)
{

}

SGAME_ITEM_SHOP_POINT_INFO::~SGAME_ITEM_SHOP_POINT_INFO()
{

}


namespace COMMENT 
{
	std::string GAME_ITEM_SHOP_TYPE[GAME_ITEM_SHOP_TYPE_SIZE] = 
	{
		"None",
		"PointShop",
		"VoteShop",
	};

	std::string GAME_ITEM_SHOP_CATEGORY[GAME_ITEM_SHOP_CATEGORY_SIZE] = 
	{
		"Special",
		"Consumables",
		"Wearables",
		"Accessories",
		"Pets",
		"Vehicles",
	};
};