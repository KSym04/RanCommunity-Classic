#pragma once

#include "./HttpThread.h"
#include "../Lib_Engine/Common/SFileSystem.h"

class CHttpPatch;

class CAutoPatchThread : public CHttpThread
{
	DECLARE_DYNCREATE(CAutoPatchThread)

	enum 
	{
		PATCH_FILE_LIST_VERSION = 0x0100,
	};

public:
	CAutoPatchThread( LPVOID pParam, DWORD nThreadID );

protected:
	virtual void ThreadMain();

protected:
	GAME_LAUNCHER_FILE_DATA_MAP m_map_patch_list;
	GAME_LAUNCHER_FILE_DATA_MAP m_map_download_list;

protected:
	BOOL DownloadFilesByHttp( CHttpPatch* pHttpPatch );
	BOOL LoadList();
	void DeleteList();
	BOOL CreateFolders();
	BOOL CompareFileList();
	void Destroy();

public:
	CAutoPatchThread(){}
	virtual ~CAutoPatchThread(){}

protected:
	DECLARE_MESSAGE_MAP()
};
