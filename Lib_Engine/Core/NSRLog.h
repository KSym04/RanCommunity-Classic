#ifndef NSRLOG_H__INCLUDED
#define NSRLOG_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LOG_FORMAT_BUFFER 1024

namespace RLOG
{
	void Init();

	void Log( const char *szFormat, ... );	
	void LogFile( const char* szFileName, const char* szFormat, ... );
	void LogFileNoDate( const char* szFileName, const char* szFormat, ... );
};

#endif // NSRLOG_H__INCLUDED
