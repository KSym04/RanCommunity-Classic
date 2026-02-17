#pragma once

#include <hash_map>
#include "./GLInventory.h"


struct SSALEITEM
{
	SNATIVEID	sSALEPOS;
	SNATIVEID	sINVENPOS;
	SITEMCUSTOM	sITEMCUSTOM;
	LONGLONG	llPRICE;
	DWORD		dwNUMBER;
	bool		bSOLD;

	SSALEITEM () :
		sINVENPOS(false),
		llPRICE(0),
		dwNUMBER(1),
		bSOLD(false)
	{
	}
};

struct SFINDRESULT
{
	SNATIVEID	sSaleItemID;
	LONGLONG	llPRICE;

	SFINDRESULT () :
		sSaleItemID(false),
		llPRICE(0)
	{
	}
};


typedef std::map<DWORD,SSEARCHITEMDATA>		MAPSEARCH;
typedef MAPSEARCH::iterator					MAPSEARCH_ITER;

class GLPrivateMarket
{
public:
	enum
	{
		EM_SALE_INVEN_X	= 6,
		EM_SALE_INVEN_Y	= 4,
		EMMAX_SALE_NUM	= EM_SALE_INVEN_X * EM_SALE_INVEN_Y
	};

	//offline vend
	enum EM_SALE_TYPE
	{
		EM_SALE_TYPE_NORMAL = 0,
		EM_SALE_TYPE_PREMIUM = 1,
		EM_SALE_TYPE_SIZE = 2,
	};

	typedef stdext::hash_map<DWORD,SSALEITEM>	MAPITEM;
	typedef MAPITEM::iterator					MAPITEM_ITER;



protected:
	bool		m_bOPEN;
	std::string	m_strTITLE;
	MAPITEM		m_mapSALE;
	GLInventory	m_invenSALE;

	MAPSEARCH	m_mapSearchItem;

	EM_SALE_TYPE m_emSale;	//offline vend
public:
	bool FindInsertPos ( SNATIVEID nidITEM, SNATIVEID &sSALEPOS );

public:
	void SetTITLE ( std::string strTITLE );
	bool SetSaleState ( SNATIVEID sSALEPOS, DWORD dwNUM, bool bSOLD );

	bool DisItem ( SNATIVEID sSALEPOS, bool bSearchMarket = FALSE );
	bool RegItem ( const SINVENITEM &sInvenItem, LONGLONG llPRICE, DWORD dwNUM, SNATIVEID sSALEPOS=SNATIVEID(false), bool bSearchMarket = FALSE );

	//offline vend
	void SetSaleType ( EM_SALE_TYPE emSale );
	bool IsPremiumMarket () { return m_emSale == EM_SALE_TYPE_PREMIUM; }

public:
	bool IsOpen ()				{ return m_bOPEN; }
	bool IsRegItem ( SNATIVEID nidITEM );
	bool IsRegInvenPos ( SNATIVEID sINVENPOS );

public:
	const std::string& GetTitle ()				{ return m_strTITLE; }
	DWORD GetItemNum ()							{ return static_cast<DWORD>(m_mapSALE.size()); }
	GLInventory& GetInven ()					{ return m_invenSALE; }

	DWORD GetItemTurnNum ( SNATIVEID nidITEM );

	GLPrivateMarket::MAPITEM& GetItemList ()	{ return m_mapSALE; }
	SSALEITEM* GetItem ( SNATIVEID sSALEPOS );

public:
	bool DoSale ( SNATIVEID sSALEPOS, DWORD dwNUM, bool bSearchMarket = FALSE );

public:
	bool DoMarketOpen ();
	void DoMarketClose();

	void DoMarketInfoRelease ();

	MAPSEARCH GetSearchItemList() const { return m_mapSearchItem; }
	std::vector<SFINDRESULT> FindItem( SSEARCHITEMDATA sSearchData );

public:
	GLPrivateMarket(void);
	~GLPrivateMarket(void);
};
