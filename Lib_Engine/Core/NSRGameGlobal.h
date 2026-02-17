#ifndef NSRGAMEGLOBAL_H__INCLUDED
#define NSRGAMEGLOBAL_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace RGAME_GLOBAL
{
	void SETIDPARAM1 ( std::string strPARAM );
	void SETIDPARAM2 ( std::string strPARAM );
	void SETIDPARAM3 ( std::string strPARAM );
	void SETJAPANUUID( std::string strUUID );

	const TCHAR* GETIDPARAM1 ();
	const TCHAR* GETIDPARAM2 ();
	const TCHAR* GETIDPARAM3 ();
	const TCHAR* GETJAPANUUID();

	bool VALIDIDPARAM ();

	BOOL SETUSERID ( const TCHAR* szUSERID );
	BOOL SETCHARID ( const TCHAR* szCHARID );
	std::string GETCHARID();
	std::string GETUSERID_ENC ();
	std::string GETUSERID_DEC ();
	std::string GETUSERID_NULL();
	void SETUSERID_ENC( std::string strUser );

	extern BOOL	bSAVE_USERID;

	extern BOOL bNPCShopConfirm;
	extern BOOL bItemPreviewStance;
	extern BOOL	bVehicleWindowRun;

	extern BOOL bAutoDraw;
	extern BOOL bConfirmDraw;

	extern BOOL bNPCContributionShopConfirm;

};

#endif // NSRGAMEGLOBAL_H__INCLUDED
