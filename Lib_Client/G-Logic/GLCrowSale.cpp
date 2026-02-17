#include "stdafx.h"
#include "./GLCrowSale.h"
#include "./GLItemMan.h"
#include "./GLogicData.h"

#include "../../Lib_Engine/Core/NSRPath.h"
#include "../../Lib_Engine/Core/NSRInfo.h"
#include "../../Lib_Engine/Core/NSRLog.h"

#include "../../Lib_Engine/Common/gltexfile.h"
#include "../../Lib_Engine/G-Logic/GLOGIC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


SCROWSALE_DATA::SCROWSALE_DATA()
	: m_strName("")
	, m_strSaleType("")
{
	m_sSaleItems.SetState ( SALE_INVEN_X, SALE_INVEN_Y );
	m_mapSalePrice.clear();
}

SCROWSALE_DATA::~SCROWSALE_DATA()
{

}

bool SCROWSALE_DATA::Load( std::string strFile )
{
	if( strFile.empty() )		return false;

	std::string strPath;
	strPath = RPATH::getDataGLogicPath();
	strPath += strFile;

	gltexfile glTextFile;
	glTextFile.reg_sep ( '\t' );
	glTextFile.reg_sep ( ' ' );
	glTextFile.reg_sep ( ',' );
	glTextFile.reg_sep ( '[' );
	glTextFile.reg_sep ( ']' );

	if( GLOGIC::bGLOGIC_ZIPFILE ) 
		glTextFile.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	if ( !glTextFile.open ( strPath, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLCROW_SALE_LOAD ), strPath.c_str() );
		return false;
	}

	CleanUp();

	m_sSaleItems.SetState ( SALE_INVEN_X, SALE_INVEN_Y );

	m_strName = strFile;

	DWORD dwVERSION = 0;
	glTextFile.getflag ( "VERSION", 1, 1, dwVERSION );

	DWORD dwNUM = glTextFile.getflagnum ( "cInventory" );
	if ( dwNUM==0 )			return FALSE;

	DWORD dwPARAM_NUM = glTextFile.getparamnum ( "cInventory" );

	glTextFile.getflag ( "szSaleType", 1, 1, m_strSaleType );

	if ( dwPARAM_NUM==11 || dwPARAM_NUM==12 )
	{
		for ( DWORD i=0; i<dwNUM; ++i )
		{
			SINVENITEM_SAVE sITEM_SAVE;
			SITEMCUSTOM &sITEM = sITEM_SAVE.sItemCustom;
			DWORD dwPART_PARAM_NUM = glTextFile.getparamnum( i, "cInventory" );

			glTextFile.getflag ( i, "cInventory", 1,  dwPART_PARAM_NUM -1, sITEM.sNativeID.wMainID );
			glTextFile.getflag ( i, "cInventory", 2,  dwPART_PARAM_NUM -1, sITEM.sNativeID.wSubID );

			glTextFile.getflag ( i, "cInventory", 3,  dwPART_PARAM_NUM -1, sITEM.wTurnNum );

			glTextFile.getflag ( i, "cInventory", 4,  dwPART_PARAM_NUM -1, sITEM.cDAMAGE );
			glTextFile.getflag ( i, "cInventory", 5,  dwPART_PARAM_NUM -1, sITEM.cDEFENSE );

			glTextFile.getflag ( i, "cInventory", 6,  dwPART_PARAM_NUM -1, sITEM.cRESIST_FIRE );
			glTextFile.getflag ( i, "cInventory", 7,  dwPART_PARAM_NUM -1, sITEM.cRESIST_ICE );
			glTextFile.getflag ( i, "cInventory", 8,  dwPART_PARAM_NUM -1, sITEM.cRESIST_ELEC );
			glTextFile.getflag ( i, "cInventory", 9,  dwPART_PARAM_NUM -1, sITEM.cRESIST_POISON );
			glTextFile.getflag ( i, "cInventory", 10, dwPART_PARAM_NUM -1, sITEM.cRESIST_SPIRIT );
			ChangeGradeValueFromItemEditValue( sITEM );

			BOOL bOK = m_sSaleItems.InsertItem ( sITEM );
			if ( !bOK )
			{
				RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLCROW_SALE_LOAD_INSERT_FAILED_1 ), strPath.c_str(), sITEM.sNativeID.wMainID, sITEM.sNativeID.wSubID );
			}

			if( dwPART_PARAM_NUM  == 12 )
			{
				LONGLONG lNpcSellValue = 0;
				glTextFile.getflag ( i, "cInventory", 11, dwPART_PARAM_NUM -1, lNpcSellValue );
				m_mapSalePrice[sITEM.sNativeID.dwID] = lNpcSellValue;
			}
		}
	}
	else if ( dwPARAM_NUM==13 || dwPARAM_NUM==14 )
	{
		for ( DWORD i=0; i<dwNUM; ++i )
		{
			WORD wPosX(0), wPosY(0);
			SITEMCUSTOM sITEM;
			DWORD dwPART_PARAM_NUM = glTextFile.getparamnum( i, "cInventory" );

			glTextFile.getflag ( i, "cInventory", 1,  dwPART_PARAM_NUM-1, wPosX );
			glTextFile.getflag ( i, "cInventory", 2,  dwPART_PARAM_NUM-1, wPosY );

			glTextFile.getflag ( i, "cInventory", 3,  dwPART_PARAM_NUM-1, sITEM.sNativeID.wMainID );
			glTextFile.getflag ( i, "cInventory", 4,  dwPART_PARAM_NUM-1, sITEM.sNativeID.wSubID );

			glTextFile.getflag ( i, "cInventory", 5,  dwPART_PARAM_NUM-1, sITEM.wTurnNum );

			glTextFile.getflag ( i, "cInventory", 6,  dwPART_PARAM_NUM-1, sITEM.cDAMAGE );
			glTextFile.getflag ( i, "cInventory", 7,  dwPART_PARAM_NUM-1, sITEM.cDEFENSE );

			glTextFile.getflag ( i, "cInventory", 8,  dwPART_PARAM_NUM-1, sITEM.cRESIST_FIRE );
			glTextFile.getflag ( i, "cInventory", 9,  dwPART_PARAM_NUM-1, sITEM.cRESIST_ICE );
			glTextFile.getflag ( i, "cInventory", 10, dwPART_PARAM_NUM-1, sITEM.cRESIST_ELEC );
			glTextFile.getflag ( i, "cInventory", 11, dwPART_PARAM_NUM-1, sITEM.cRESIST_POISON );
			glTextFile.getflag ( i, "cInventory", 12, dwPART_PARAM_NUM-1, sITEM.cRESIST_SPIRIT );

			ChangeGradeValueFromItemEditValue( sITEM );

			BOOL bOK = m_sSaleItems.InsertItem ( sITEM, wPosX, wPosY, true );
			if ( !bOK )
			{
				RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLCROW_SALE_LOAD_INSERT_FAILED_2 ), strPath.c_str(), sITEM.sNativeID.wMainID, sITEM.sNativeID.wSubID, wPosX, wPosY );
			}

			if( dwPART_PARAM_NUM == 14 )
			{
				LONGLONG lNpcSellValue = 0;
				glTextFile.getflag ( i, "cInventory", 13, dwPART_PARAM_NUM-1, lNpcSellValue );
				m_mapSalePrice[sITEM.sNativeID.dwID] = lNpcSellValue;
			}
		}
	}
	else
	{
		RLOG::Log( RINFO::getc( EMRINFO_ERROR_GLCROW_SALE_LOAD_PARAM_NUM ), strPath.c_str(), dwPARAM_NUM );
		return false;
	}

	return true;
}


void SCROWSALE_DATA::ChangeGradeValueFromItemEditValue( SITEMCUSTOM &sItem )
{
	// legacy code retained for future use

	// change the default polishing value to the value set in the item edit (ignore the value of crowsale)
	// If the NPC never sells + items, return immediately
	return;

	SITEM* pItem = GLItemMan::GetInstance().GetItem( sItem.sNativeID );
	if( !pItem )
		return;

	sItem.cDAMAGE = (BYTE)pItem->sBasicOp.wGradeAttack;
	sItem.cDEFENSE = (BYTE)pItem->sBasicOp.wGradeDefense;
}

void SCROWSALE_DATA::CleanUp()
{
	m_strSaleType = "";
	m_mapSalePrice.clear();
}

LONGLONG SCROWSALE_DATA::GetNpcSellPrice( DWORD dwItemID )
{
	CROW_SALE_DATA_PRICE_MAP_ITER it = m_mapSalePrice.find(dwItemID);
	if( it == m_mapSalePrice.end() )	return 0;

	return it->second;
}

GLInventory* SCROWSALE_DATA::GetSaleInven()
{
	return &m_sSaleItems;
}

std::string SCROWSALE_DATA::GetSaleType()
{
	return m_strSaleType;
}
