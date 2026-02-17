#ifndef GLCROWSALE_H__INCLUDED
#define GLCROWSALE_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLInventory.h"
#include "./GLCharDefine.h"

typedef std::map<DWORD,LONGLONG>				CROW_SALE_DATA_PRICE_MAP;
typedef CROW_SALE_DATA_PRICE_MAP::iterator		CROW_SALE_DATA_PRICE_MAP_ITER;

struct SCROWSALE_DATA
{
	std::string m_strName;
	std::string	m_strSaleType;

	GLInventory	m_sSaleItems;

	CROW_SALE_DATA_PRICE_MAP	m_mapSalePrice;

	bool Load( std::string strFile );
	void ChangeGradeValueFromItemEditValue( SITEMCUSTOM &sItem );

	LONGLONG GetNpcSellPrice ( DWORD dwItemID );
	GLInventory* GetSaleInven();
	std::string GetSaleType();

	void CleanUp();

	SCROWSALE_DATA();
	~SCROWSALE_DATA();
};

typedef std::map<std::string, SCROWSALE_DATA*>		SCROWSALE_DATA_POINTER_MAP;
typedef SCROWSALE_DATA_POINTER_MAP::iterator		SCROWSALE_DATA_POINTER_MAP_ITER;

#endif // GLCROWSALE_H__INCLUDED
