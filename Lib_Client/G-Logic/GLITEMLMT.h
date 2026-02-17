#pragma once
#include <hash_map>
#include "./GLItemMan.h"

#include "../../Lib_Engine/G-Logic/GLDBMan.h"
#include "../../Lib_Engine/G-Logic/DxMsgServer.h"

#include "./GLRouteDefine.h"


struct SITEMLMT
{
	//version 1 default
	//version 2 added lnActionSystem, lnActionUser
	//version 3 added lnItemMix, lnPVPEvent
	enum { VERSION = 0x003, };

	LONGLONG lnDEFAULT;
	LONGLONG lnINIT;
	LONGLONG lnSHOP;
	LONGLONG lnNPC;
	LONGLONG lnQUEST;
	LONGLONG lnMOB;
	LONGLONG lnGM;
	LONGLONG lnOLD;
	LONGLONG lnBILLING;
	LONGLONG lnItemMix;
	LONGLONG lnGather;
	LONGLONG lnSystem;
	LONGLONG lnActionSystem;
	LONGLONG lnActionUser;
	LONGLONG lnPVPEvent;

	SITEMLMT ()
		: lnDEFAULT(0)
		, lnINIT(0)
		, lnSHOP(0)
		, lnNPC(0)
		, lnQUEST(0)
		, lnMOB(0)
		, lnGM(0)
		, lnOLD(0)
		, lnBILLING(0)
		, lnItemMix(0)
		, lnGather(0)
		, lnSystem(0)
		, lnActionSystem(0)
		, lnActionUser(0)
		, lnPVPEvent(0)
	{
	}

	LONGLONG GETTOTAL () const
	{
		return lnDEFAULT +
			lnINIT +
			lnSHOP +
			lnNPC +
			lnQUEST +
			lnMOB +
			lnGM +
			lnOLD +
			lnBILLING +
			lnItemMix +
			lnGather +
			lnSystem +
			lnActionSystem +
			lnActionUser +
			lnPVPEvent
			;
	}
};

class GLITEMLMT
{
public:
	enum { VERSION = 0x0001, };
	typedef stdext::hash_map<DWORD,SITEMLMT>	GLGENITEM;
	typedef GLGENITEM::iterator					GLGENITEM_ITER;

protected:
	std::string		m_strPATH;
	GLDBMan*		m_pDBMan;
	DxMsgServer*	m_pMsgServer;
	
	int				m_nSGNum;
	int				m_nSvrNum;
	DWORD			m_dwFieldID;

public:
	void SetPath ( std::string strPATH )										{ m_strPATH = strPATH; }
	void SetDBMan ( GLDBMan* pDBMan )											{ m_pDBMan = pDBMan; }
	void SetServer ( DxMsgServer* pMsgServer, DWORD dwFieldID=UINT_MAX )		{ m_pMsgServer = pMsgServer; m_dwFieldID = dwFieldID; }

	void ReadMaxKey ();

protected:
	GLGENITEM	m_mapGENITEM;

protected:
	void SetItemGenNum ( const SNATIVEID &sNID, EMITEMGEN emTYPE, LONGLONG lnMax );
	LONGLONG GetItemGenNum ( const SNATIVEID &sNID, EMITEMGEN emTYPE );

public:
	//	발생 가능한지 점검. ( 발생 여유 갯수가 있는지 보는것. )
	bool DoCheckItemGen ( const SNATIVEID &sNID, EMITEMGEN emTYPE=EMGEN_DEFAULT );

	//	발생 갯수 등록. ( 1개 단위. )
	LONGLONG RegItemGen ( const SNATIVEID &sNID, EMITEMGEN emTYPE=EMGEN_DEFAULT );

	void RegPickUpInfo ( DWORD dwGaeaID, const SNATIVEID &sNID );

public:
	void ReqItemRoute ( const SITEMCUSTOM &sITEM, EMIDTYPE emFROME, DWORD dwFROMID, EMIDTYPE emTO, DWORD dwTOID, EMITEM_ROUTE emROUTE, int nNum );
	void ReqMoneyExc ( EMIDTYPE emFROM, DWORD dwFrom, EMIDTYPE emTO, DWORD dwTo, LONGLONG lnPrice, DWORD nFlag );
	void ReqItemConversion ( const SITEMCUSTOM &sITEM, EMIDTYPE emFROM, DWORD dwFrom );
	void ReqRandomItem ( const SITEMCUSTOM &sITEM );
	
	void ReqAction ( DWORD dwCI, EMLOGACTION emACT, EMIDTYPE emTAR, DWORD dwTAR, __int64 nEXP, int nBRIGHT, int nLIFE, int nMONEY );
	void ReqPetAction ( int nPetNum, SNATIVEID nItemID, EMPET_ACTION emAction, int nPetFull);
	void ReqVehicleAction ( int nVehicleNum, SNATIVEID nItemID, EMVEHICLE_ACTION emAction, int nVehicleFull);

private:
	GLITEMLMT(void);

public:
	~GLITEMLMT(void);

public:
	static GLITEMLMT& GetInstance();
};

