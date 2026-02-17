#include "stdafx.h"
#include "./s_COdbcManager.h"
#include "./s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/* award title, Juver, 2021/02/12 */
int	COdbcManager::getChaTitle( DWORD dwChaNum, MAP_AWARD_TITLE_DATA& mapTitle )
{
	//validity check
	if ( dwChaNum == 0	)	return DB_ERROR;

	//reset to defaults
	mapTitle.clear();

	//get data from db
	SQLRETURN sReturn = 0;
	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn)				 return DB_ERROR;


	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "SELECT idx, Title, Type From ChaAwardTitle WHERE ChaNum=%d AND Enabled=1 ", dwChaNum );

	sReturn = ::SQLExecDirect(pConn->hStmt, (SQLCHAR*)szTemp, SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);		
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	SQLINTEGER nTitleID = 0, cbTitleID = SQL_NTS;
	SQLCHAR    szTitle[AWARD_TITLE_LENGTH+1] = {0}; SQLINTEGER cbTitle = SQL_NTS;
	SQLINTEGER nTitleType = 0, cbTitleType = SQL_NTS;

	while (true)
	{
		sReturn = ::SQLFetch(pConn->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
		{
			Print(szTemp);		
			Print(GetErrorString(pConn->hStmt));
			m_pGameDB->FreeConnection(pConn);

			return DB_ERROR;
		}

		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{		
			::SQLGetData(pConn->hStmt,  1, SQL_C_LONG,	&nTitleID,				0, &cbTitleID);
			::SQLGetData(pConn->hStmt,  2, SQL_C_CHAR,  szTitle, AWARD_TITLE_LENGTH, &cbTitle);
			::SQLGetData(pConn->hStmt,  3, SQL_C_LONG,	&nTitleType,			0, &cbTitleType);

			SAWARD_TITLE_DATA sTitle;
			sTitle.dwID = (DWORD)nTitleID;

			if (cbTitle != 0 && cbTitle != -1)
				::StringCchCopy(sTitle.szTitle, AWARD_TITLE_LENGTH+1, (const TCHAR*) szTitle);

			sTitle.emType = (EMAWARD_TITLE_TYPE)nTitleType;
			
			mapTitle.insert ( std::make_pair( sTitle.dwID, sTitle ) );
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}	  

	m_pGameDB->FreeConnection(pConn);

	return DB_OK;
}
