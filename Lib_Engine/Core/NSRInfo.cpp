#include "stdafx.h"
#include "./NSRInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RINFO
{
	const TCHAR* getc( EMRINFO emType )
	{
		if ( emType >= EMRINFO_SIZE )	return NULL;
		return m_strInfo[emType].c_str();
	};

	std::string gets( EMRINFO emType )
	{
		if ( emType >= EMRINFO_SIZE )	return "";
		return m_strInfo[emType];
	};

	std::string m_strInfo[EMRINFO_SIZE] = 
	{
		"InfoBase",							//EMRINFO_RAN_ONLINE

		"\\",								//EMRINFO_FILE_NAME_DBSLASH
		"param.ini",						//EMRINFO_FILE_NAME_RPARAM
		"option.ini",						//EMRINFO_FILE_NAME_ROPTION

		"GameLogs.txt",						//EMRINFO_LOG_FILE_NAME
		"%d-%02d-%02d %02d:%02d:%02d",		//EMRINFO_LOG_TIME_FORMAT

		".xmlx",							//EMRINFO_XML_FILE_EXT_ENCRYPT
		".xml",								//EMRINFO_XML_FILE_EXT_DECRYPT

		"RPARAM::Load() %s failed",			//EMRINFO_ERROR_RPARAM_LOAD
		"ROPTION::Load() %s failed",		//EMRINFO_ERROR_ROPTION_LOAD
		"RPROFILE::Load() empty char id",	//EMRINFO_ERROR_RPROFILE_LOAD_CHAR_NAME

		"GLCurrencyShop::Load %s failed",	//EMRINFO_ERROR_GLCURRENCY_SHOP_LOAD
		"GLCurrencyShop::Load %u/%u exist",	//EMRINFO_ERROR_GLCURRENCY_SHOP_LOAD_EXIST

		"GLNPCShop::Load %s failed",				//EMRINFO_ERROR_GLNPC_SHOP_LOAD
		"GLNPCShop::Load %u exist type",			//EMRINFO_ERROR_GLNPC_SHOP_LOAD_EXIST_TYPE
		"GLNPCShop::Load %u/%u exist item",			//EMRINFO_ERROR_GLNPC_SHOP_LOAD_EXIST_ITEM
		"GLNPCShop::Load %u/%u item invalid type",	//EMRINFO_ERROR_GLNPC_SHOP_LOAD_ITEM_NO_TYPE

		"GLItemExchange::Load %s failed",		//EMRINFO_ERROR_GLITEM_EXCHANGE_LOAD
		"GLItemExchange::Load %u exist",		//EMRINFO_ERROR_GLITEM_EXCHANGE_LOAD_EXIST

		"GLCrowSale::Load %s failed",					//EMRINFO_ERROR_GLCROW_SALE_LOAD
		"GLCrowSale::Load %s invalid param num %u",		//EMRINFO_ERROR_GLCROW_SALE_LOAD_PARAM_NUM
		"GLCrowSale::Load %u insert failed %u/%u",		//EMRINFO_ERROR_GLCROW_SALE_LOAD_INSERT_FAILED_1
		"GLCrowSale::Load %u insert failed %u/%u pos %u/%u",	//EMRINFO_ERROR_GLCROW_SALE_LOAD_INSERT_FAILED_2

		"GMCommandPass.ini",						//EMRINFO_GM_COMMAND_PASS_FILE_NAME
		"CGMCommandPass::LoadFile %s failed",		//EMRINFO_GM_COMMAND_PASS_ERROR_FILE

		"PartyFinder.ini",							//EMRINFO_PARTY_FINDER_FILE_NAME
		"GLPartyFinder::LoadFile %s failed",		//EMRINFO_PARTY_FINDER_ERROR_FILE
	};
};
