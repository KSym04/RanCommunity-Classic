#include "stdafx.h"
#include "./GLCrowData.h"

#include "./GLItemMan.h"

#include "../../Lib_Engine/Common/StlFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GLCrowDataMan::AdditionalLoad( PCROWDATA pData )
{
	if ( !pData )	return;

	if ( GLItemMan::GetInstance().ValidTable() )
	{
		for ( int i=0; i<SCROWACTION::SALENUM; ++i )
		{
			if ( pData->m_sAction.m_strSaleFile[i].size() )
			{
				CrowSaleLoad( pData->m_sAction.m_strSaleFile[i] );
			}
		}

		if ( pData->m_sAction.m_strCurrencyShop.size() )
			CurrencyShopLoad( pData->m_sAction.m_strCurrencyShop );

		/*npc shop, Juver, 2017/07/25 */
		if ( pData->m_sAction.m_strShopFile.size() )
			NPCShopLoad( pData->m_sAction.m_strShopFile );

		/*item exchange, Juver, 2017/10/11 */
		if ( pData->m_sAction.m_strItemExchangeFile.size() )
			NPCItemExchangeLoad( pData->m_sAction.m_strItemExchangeFile );

		if ( pData->m_sAction.m_strContributionShop.size() )
			NPCContributionShopLoad(pData->m_sAction.m_strContributionShop);
	}
}

SCROWSALE_DATA* GLCrowDataMan::CrowSaleLoad( std::string strFile )
{
	if ( strFile.empty() )	return NULL;

	SCROWSALE_DATA* pData = NULL;

	pData = CrowSaleFind( strFile );
	if ( pData )	return pData;

	pData = new SCROWSALE_DATA;
	bool bLoad = pData->Load( strFile );
	if ( !bLoad )
	{
		SAFE_DELETE( pData );
		return NULL;
	}

	m_mapCrowSale.insert( std::make_pair( pData->m_strName, pData ) );

	return pData;
}

SCROWSALE_DATA* GLCrowDataMan::CrowSaleFind( std::string strFile )
{
	SCROWSALE_DATA_POINTER_MAP_ITER it = m_mapCrowSale.find( strFile );
	if ( it != m_mapCrowSale.end() )	return it->second;

	return NULL;
}

bool GLCrowDataMan::CrowSaleDelete( std::string strFile )
{
	SCROWSALE_DATA_POINTER_MAP_ITER it = m_mapCrowSale.find( strFile );
	if ( it == m_mapCrowSale.end() )	return false;

	delete (*it).second;
	m_mapCrowSale.erase ( it );

	return true;
}

void GLCrowDataMan::CrowSaleCleanUp()
{
	std::for_each ( m_mapCrowSale.begin(), m_mapCrowSale.end(), std_afunc::DeleteMapObject() );
	m_mapCrowSale.clear();
}


SCURRENCY_SHOP_DATA* GLCrowDataMan::CurrencyShopLoad( std::string strFile )
{
	if ( strFile.empty() )	return NULL;

	SCURRENCY_SHOP_DATA* pShop = NULL;

	pShop = CurrencyShopFind( strFile );
	if ( pShop )	return pShop;

	pShop = new SCURRENCY_SHOP_DATA;
	bool bLoad = pShop->Load( strFile );
	if ( !bLoad )
	{
		SAFE_DELETE( pShop );
		return NULL;
	}

	m_mapCurrencyShop.insert( std::make_pair( pShop->m_strName, pShop ) );

	return pShop;
}

SCURRENCY_SHOP_DATA* GLCrowDataMan::CurrencyShopFind( std::string strFile )
{
	SCURRENCY_SHOP_DATA_POINTER_MAP_ITER it = m_mapCurrencyShop.find( strFile );
	if ( it != m_mapCurrencyShop.end() )	return it->second;

	return NULL;
}

void GLCrowDataMan::CurrencyShopDelete( std::string strFile )
{
	SCURRENCY_SHOP_DATA_POINTER_MAP_ITER it = m_mapCurrencyShop.find( strFile );
	if ( it == m_mapCurrencyShop.end() )	return;

	delete (*it).second;
	m_mapCurrencyShop.erase ( it );
}

void GLCrowDataMan::CurrencyShopCleanUp()
{
	std::for_each ( m_mapCurrencyShop.begin(), m_mapCurrencyShop.end(), std_afunc::DeleteMapObject() );
	m_mapCurrencyShop.clear();
}


SNPC_SHOP_DATA* GLCrowDataMan::NPCShopLoad( std::string strFile )
{
	if ( strFile.empty() )	return NULL;

	SNPC_SHOP_DATA* pShop = NULL;

	pShop = NPCShopFind( strFile );
	if ( pShop )	return pShop;

	pShop = new SNPC_SHOP_DATA;
	bool bLoad = pShop->Load( strFile );
	if ( !bLoad )
	{
		SAFE_DELETE( pShop );
		return NULL;
	}

	m_mapNPCShop.insert( std::make_pair( pShop->m_strName, pShop ) );

	return pShop;
}

SNPC_SHOP_DATA* GLCrowDataMan::NPCShopFind( std::string strFile )
{
	SNPC_SHOP_DATA_POINTER_MAP_ITER it = m_mapNPCShop.find( strFile );
	if ( it != m_mapNPCShop.end() )	return it->second;

	return NULL;
}

void GLCrowDataMan::NPCShopDelete( std::string strFile )
{
	SNPC_SHOP_DATA_POINTER_MAP_ITER it = m_mapNPCShop.find( strFile );
	if ( it == m_mapNPCShop.end() )	return;

	delete (*it).second;
	m_mapNPCShop.erase ( it );
}

void GLCrowDataMan::NPCShopCleanUp()
{
	std::for_each ( m_mapNPCShop.begin(), m_mapNPCShop.end(), std_afunc::DeleteMapObject() );
	m_mapNPCShop.clear();
}


SNPC_ITEM_EXCHANGE* GLCrowDataMan::NPCItemExchangeLoad( std::string strFile )
{
	if ( strFile.empty() )	return NULL;

	SNPC_ITEM_EXCHANGE* pShop = NULL;

	pShop = NPCItemExchangeFind( strFile );
	if ( pShop )	return pShop;

	pShop = new SNPC_ITEM_EXCHANGE;
	bool bLoad = pShop->Load( strFile );
	if ( !bLoad )
	{
		SAFE_DELETE( pShop );
		return NULL;
	}

	m_mapNPCItemExchange.insert( std::make_pair( pShop->m_strName, pShop ) );

	return pShop;
}

SNPC_ITEM_EXCHANGE* GLCrowDataMan::NPCItemExchangeFind( std::string strFile )
{
	SNPC_ITEM_EXCHANGE_POINTER_MAP_ITER it = m_mapNPCItemExchange.find( strFile );
	if ( it != m_mapNPCItemExchange.end() )	return it->second;

	return NULL;
}

void GLCrowDataMan::NPCItemExchangeDelete( std::string strFile )
{
	SNPC_ITEM_EXCHANGE_POINTER_MAP_ITER it = m_mapNPCItemExchange.find( strFile );
	if ( it == m_mapNPCItemExchange.end() )	return;

	delete (*it).second;
	m_mapNPCItemExchange.erase ( it );
}

void GLCrowDataMan::NPCItemExchangeCleanUp()
{
	std::for_each ( m_mapNPCItemExchange.begin(), m_mapNPCItemExchange.end(), std_afunc::DeleteMapObject() );
	m_mapNPCItemExchange.clear();
}


SNPC_CONTRIBUTION_SHOP_DATA* GLCrowDataMan::NPCContributionShopLoad( std::string strFile )
{
	if ( strFile.empty() )	return NULL;

	SNPC_CONTRIBUTION_SHOP_DATA* pShop = NULL;

	pShop = NPCContributionShopFind( strFile );
	if ( pShop )	return pShop;

	pShop = new SNPC_CONTRIBUTION_SHOP_DATA;
	bool bLoad = pShop->Load( strFile );
	if ( !bLoad )
	{
		SAFE_DELETE( pShop );
		return NULL;
	}

	m_mapNPCContributionShop.insert( std::make_pair( pShop->m_strName, pShop ) );

	return pShop;
}

SNPC_CONTRIBUTION_SHOP_DATA* GLCrowDataMan::NPCContributionShopFind( std::string strFile )
{
	SNPC_CONTRIBUTION_SHOP_DATA_MAP_ITER it = m_mapNPCContributionShop.find( strFile );
	if ( it != m_mapNPCContributionShop.end() )	return it->second;

	return NULL;
}

void GLCrowDataMan::NPCContributionShopDelete( std::string strFile )
{
	SNPC_CONTRIBUTION_SHOP_DATA_MAP_ITER it = m_mapNPCContributionShop.find( strFile );
	if ( it == m_mapNPCContributionShop.end() )	return;

	delete (*it).second;
	m_mapNPCContributionShop.erase ( it );
}

void GLCrowDataMan::NPCContributionShopCleanUp()
{
	std::for_each ( m_mapNPCContributionShop.begin(), m_mapNPCContributionShop.end(), std_afunc::DeleteMapObject() );
	m_mapNPCContributionShop.clear();
}
