#include "stdafx.h"
#include <string.h>
#include <time.h>
#include "CManagerCfg.h"
#include "../Lib_Network/s_CClientConsoleMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CServerManagerCfg* CServerManagerCfg::SelfInstance = NULL;

CServerManagerCfg::CServerManagerCfg()
{
	SetDefault();
}

CServerManagerCfg* CServerManagerCfg::GetInstance()
{
	if (SelfInstance == NULL)
		SelfInstance = new CServerManagerCfg();

	return SelfInstance;
}

void CServerManagerCfg::ReleaseInstance()
{
	if (SelfInstance != NULL)
	{
		delete SelfInstance;
		SelfInstance = NULL;
	}
}

void CServerManagerCfg::SetDefault(void)
{
	m_nSessionPort   = 0;	
}

CServerManagerCfg::~CServerManagerCfg()
{
}

CString	CServerManagerCfg::GetAppPath ()
{
	CString strFullPath;
	CString strCommandLine;

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);
	strCommandLine = szPath;

	if ( !strCommandLine.IsEmpty() )
	{
		DWORD dwFind = strCommandLine.ReverseFind ( '\\' );
		if ( dwFind != -1 )
		{
			strFullPath = strCommandLine.Left ( dwFind );
			
			if ( !strFullPath.IsEmpty() )
			if ( strFullPath.GetAt(0) == '"' )
				strFullPath = strFullPath.Right ( strFullPath.GetLength() - 1 );
		}
	}

	return strFullPath;
}

int CServerManagerCfg::Load()
{
	// Data type problem
	CString strFileName = GetAppPath() + "\\ServerManager.cfg";
	
	FILE *oFile = NULL;
	char szLine[1024+1];

	// Open for read 
	if ((oFile = fopen(strFileName.GetString(), "r")) == NULL)
	{
		return 0;
	}	

	// Read a line and process
	while (::fgets(szLine, 1024, oFile))
	{
		Process(szLine);
	}
	
	// Close file
	fclose(oFile);
	return 1;
}

int CServerManagerCfg::Process(char* szLine)
{
	char seps[]   = " ,\t\n";
	char *token;

	token = strtok(szLine, seps);
	while (token != NULL)
	{	
		if (strcmp(token, "//") == 0) 
		{
			return 0;
		} 
		else if (strcmp(token, "session_ip") == 0) 
		{
			token = strtok(NULL, seps);
			if (token)
			{
				m_strSessionIP = token;
				m_strSessionIP.Trim(_T(" "));
				CClientConsoleMessage::GetInstance()->WriteWithTime(_T("Session IP:%s"), m_strSessionIP);
			}
			else
			{
				return 0;
			}
		}
		else if (strcmp(token, "session_port") == 0)
		{
			token = strtok(NULL, seps);
			if (token) 
			{
				m_nSessionPort = atoi(token);
				CClientConsoleMessage::GetInstance()->WriteWithTime(_T("Session Port:%d"), m_nSessionPort);
			}
			else
			{
				return 0;
			}
		}
		else
		{			
		}
		// Get next token
		token = ::strtok( NULL, seps );
	}
	return 0;
}

int CServerManagerCfg::GetSessionPort()
{ 
	return m_nSessionPort;
}

CString CServerManagerCfg::GetSessionIP()
{
	return m_strSessionIP;
}
