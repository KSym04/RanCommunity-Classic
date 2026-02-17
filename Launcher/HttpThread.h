#pragma once

#include "./LPatchThread.h"
#include "./RLauncherData.h"

class CHttpPatch;
struct S_PATCH_THREAD_PARAM;

class CHttpThread : public CLPatchThread
{

	DECLARE_DYNCREATE(CHttpThread)

protected:
	S_PATCH_THREAD_PARAM * m_pPatchThreadParam;
	DWORD m_nDlgThreadID;

protected:
	BOOL GETFILE_USEHTTP( CHttpPatch* pHttpPatch, std::string strRemoteSubPath, std::string strFileName, CString strTempDir );

protected:
	BOOL Initialize()		{ return TRUE; }
	
public:
	CHttpThread(){}
	virtual ~CHttpThread(){}

protected:
	DECLARE_MESSAGE_MAP()
};
