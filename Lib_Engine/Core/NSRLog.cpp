#include "stdafx.h"
#include "./NSRLog.h"

#include "./NSRPath.h"
#include "./NSRInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RLOG
{
	void Init()
	{

	};

	void Log ( const char* szFormat, ... )
	{
		va_list argList;
		char szBuffer[LOG_FORMAT_BUFFER] = "";

		va_start(argList, szFormat);
		StringCbVPrintf(szBuffer, LOG_FORMAT_BUFFER, szFormat, argList);
		va_end(argList);

		char	szLogFile[MAX_PATH] = "";
		StringCchCopy( szLogFile, MAX_PATH, RPATH::getLogsPath() );
		StringCchCat( szLogFile, MAX_PATH, RINFO::getc( EMRINFO_LOG_FILE_NAME ) );

		FILE *fp;
		fp = fopen ( szLogFile, "at" );
		if ( fp )
		{
			CTime Time = CTime::GetCurrentTime();

			char szTime[MAX_PATH] = "";
			sprintf_s( szTime, MAX_PATH, RINFO::getc( EMRINFO_LOG_TIME_FORMAT ), Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond() );

			fprintf ( fp, "[%s]%s\n", szTime, szBuffer );
			fclose(fp);
		}
	};

	void LogFile ( const char* szFileName, const char* szFormat, ... )
	{
		va_list argList;
		char szBuffer[LOG_FORMAT_BUFFER] = "";

		va_start(argList, szFormat);
		StringCbVPrintf(szBuffer, LOG_FORMAT_BUFFER, szFormat, argList);
		va_end(argList);

		char	szLogFile[MAX_PATH] = "";
		StringCchCopy( szLogFile, MAX_PATH, RPATH::getLogsPath() );
		StringCchCat( szLogFile, MAX_PATH, szFileName );

		FILE *fp;
		fp = fopen ( szLogFile, "at" );
		if ( fp )
		{
			CTime Time = CTime::GetCurrentTime();

			char szTime[MAX_PATH] = "";
			sprintf_s( szTime, MAX_PATH, RINFO::getc( EMRINFO_LOG_TIME_FORMAT ), Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond() );

			fprintf ( fp, "[%s]%s\n", szTime, szBuffer );
			fclose(fp);
		}
	};

	void LogFileNoDate ( const char* szFileName, const char* szFormat, ... )
	{
		va_list argList;
		char szBuffer[LOG_FORMAT_BUFFER] = "";

		va_start(argList, szFormat);
		StringCbVPrintf(szBuffer, LOG_FORMAT_BUFFER, szFormat, argList);
		va_end(argList);

		char	szLogFile[MAX_PATH] = "";
		StringCchCopy( szLogFile, MAX_PATH, RPATH::getLogsPath() );
		StringCchCat( szLogFile, MAX_PATH, szFileName );

		FILE *fp;
		fp = fopen ( szLogFile, "at" );
		if ( fp )
		{	
			fprintf ( fp, "%s\n", szBuffer );
			fclose(fp);
		}
	};


};
