#ifndef CServerManagerCfg_H_
#define CServerManagerCfg_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRSAFE_LIB
#define STRSAFE_NO_DEPRECATE

#include "strsafe.h" // Safe string function

// Auto Link Library Files
#pragma comment (lib,"strsafe.lib")

#define USER_ID_LENGTH				20
#define	USER_PASS_LENGTH			20

#define DB_SVR_NAME_LENGTH			50
#define DB_NAME_LENGTH				50


class CServerManagerCfg
{
protected:
	
public:
	static CServerManagerCfg* GetInstance();
	static void	ReleaseInstance();

	void		SetDefault(void);
	int			Process(char* szLine);
	int			Load();
	CString		GetAppPath();

	int         GetSessionPort();
	CString     GetSessionIP();

private:
	CServerManagerCfg();
	virtual ~CServerManagerCfg();
	static CServerManagerCfg* SelfInstance;

protected:
	CString m_strSessionIP;	
	int  m_nSessionPort;
};

#endif // CServerManagerCfg_H_