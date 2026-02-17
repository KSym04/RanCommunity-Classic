#include "stdafx.h"
#include "./GLSetOption.h"

#include "../../Lib_Engine/Common/SUBPATH.h"
#include "../../Lib_Engine/Common/seqrandom.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"

#include "./GLogicData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char* GLSetOptionData::_FILEHEAD = "SetOptionData";
const char* GLSetOptionGen::_FILEHEAD = "SetOptionGen";

GLSetOptionData& GLSetOptionData::GetInstance ()
{
	static GLSetOptionData _instance;
	return _instance;
}

GLSetOptionData::GLSetOptionData()
	: m_strFileName("")
{

}

GLSetOptionData::~GLSetOptionData()
{
	CleanUp();
}

void GLSetOptionData::CleanUp()
{
	m_mapSetOptionData.clear();
}

BOOL GLSetOptionData::LoadSetOptionData( std::string strFile )
{
	if ( strFile.empty() )	return FALSE;

	std::string strFullPath = GLOGIC::GetPath();
	strFullPath += strFile;

	std::auto_ptr<basestream> pBStream
		(
			GLOGIC::openfile_basestream
			(
				GLOGIC::bGLOGIC_ZIPFILE, 
				GLOGIC::strGLOGIC_ZIPFILE.c_str(), 
				strFullPath.c_str(), 
				strFile.c_str(),
				EMRCRYPT_BYTE_TYPE_NONE,
				GLOGIC::bGLOGIC_PACKFILE 
			) 
		);

	if ( !pBStream.get() )
	{
		if ( strFile.size() )
			CDebugSet::ErrorFile( "GLSetOption::LoadSetOption", strFile.c_str() );

		return FALSE;
	}

	m_strFileName = strFile;

	basestream &SFile = *pBStream;

	DWORD dwFileVersion;
	char szFileType[_MAX_FNAME];
	SFile.GetFileType( szFileType, _MAX_FNAME, dwFileVersion );

	SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_OLD );

	if ( dwFileVersion>=VERSION_ENCODE )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_2021_SET_OPTION_V1 );

	CleanUp();

	if ( dwFileVersion == VERSION )
	{
		DWORD dwNum(0);
		SFile >> dwNum;

		for ( DWORD i=0; i<dwNum; i++ )
		{
			SSET_OPTION_DATA sOption;
			sOption.Load( SFile );

			m_mapSetOptionData.insert( std::make_pair( sOption.wID, sOption ) );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "GLSetOption::LoadSetOption", dwFileVersion );
	}

	return TRUE;


}


BOOL GLSetOptionData::SaveSetOptionData( std::string strFile )
{
	if ( strFile.empty() )	return FALSE;

	std::string strFullPath = GLOGIC::GetPath();
	strFullPath += strFile;
	
	CSerialFile SFile;
	SFile.SetFileType ( _FILEHEAD, VERSION );
	if ( !SFile.OpenFile ( FOT_WRITE, strFullPath.c_str() ) )	return FALSE;
	SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_2021_SET_OPTION_V1 );

	SFile << (DWORD)m_mapSetOptionData.size();

	for( SET_OPTION_DATA_MAP_ITER iter = m_mapSetOptionData.begin(); iter != m_mapSetOptionData.end(); ++iter )
	{
		(*iter).second.Save( SFile );
	}

	m_strFileName = strFile;

	return TRUE;
}

const bool GLSetOptionData::CSVLoad(CWnd* pWnd)
{
	// Clear;
	CleanUp();

	CFileDialog dlg( TRUE, ".csv", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "csv file (*.csv)|*.csv|", pWnd );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() != IDOK )
		return S_FALSE;

	CStringFile StrFile( 10240 );
	if( !StrFile.Open ( dlg.GetPathName().GetString() ) )	
		return S_FALSE;

	STRUTIL::ClearSeparator();
	STRUTIL::RegisterSeparator( "," );

	CString strLine;
	CStringArray StrArray;

	StrFile.GetNextLine( strLine );
	STRUTIL::StringSeparate( strLine, StrArray );

	int iLine = 1;

	while( StrFile.GetNextLine( strLine ) )
	{
		++iLine;

		STRUTIL::StringSeparate( strLine, StrArray );

		WORD wID = (WORD)atoi( StrArray[0] );
		
		SSET_OPTION_DATA* pData = GetData(wID);
		if ( pData )
		{
			pData->LoadCsv(StrArray);
		}
		else
		{
			SSET_OPTION_DATA sData;
			sData.LoadCsv( StrArray );
			m_mapSetOptionData.insert( std::make_pair( sData.wID, sData ) );
		}
	}

	return true;
}

const bool GLSetOptionData::CSVSave(CWnd* pWnd)
{
	CFileDialog dlg( FALSE, ".csv", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "csv file (*.csv)|*.csv|", pWnd);

	std::string InitPath(GLOGIC::GetPath());
	dlg.m_ofn.lpstrInitialDir = InitPath.c_str();
	if (dlg.DoModal() != IDOK)
		return false;

	std::string FileName(dlg.GetPathName().GetString());
	std::fstream SFile;
	SFile.open(FileName.c_str(), std::ios_base::out);

	SSET_OPTION_DATA::SaveCsvHead(SFile);

	SET_OPTION_DATA_MAP_ITER it_b = m_mapSetOptionData.begin();
	SET_OPTION_DATA_MAP_ITER it_e = m_mapSetOptionData.end();
	for( ; it_b != it_e; ++it_b )
	{
		const SSET_OPTION_DATA& sData = (*it_b).second;
		(*it_b).second.SaveCsv( SFile );
	}

	SFile.close();
	return true;
}


SSET_OPTION_DATA* GLSetOptionData::GetData( WORD wID )
{
	if ( wID == SET_OPTION_NULL )	return NULL;

	SET_OPTION_DATA_MAP_ITER iter = m_mapSetOptionData.find( wID );
	if ( iter == m_mapSetOptionData.end() )	return NULL;

	return &(*iter).second;
}




GLSetOptionGen& GLSetOptionGen::GetInstance ()
{
	static GLSetOptionGen _instance;
	return _instance;
}

GLSetOptionGen::GLSetOptionGen()
{

}

GLSetOptionGen::~GLSetOptionGen()
{
	CleanUp();
}

void GLSetOptionGen::CleanUp()
{
	m_mapSetOptionGen.clear();
}

BOOL GLSetOptionGen::LoadSetOptionGen( std::string strFile )
{
	if ( strFile.empty() )	return FALSE;

	std::string strFullPath = GLOGIC::GetPath();
	strFullPath += strFile;

	std::auto_ptr<basestream> pBStream
		(
		GLOGIC::openfile_basestream
		(
		GLOGIC::bGLOGIC_ZIPFILE, 
		GLOGIC::strGLOGIC_ZIPFILE.c_str(), 
		strFullPath.c_str(), 
		strFile.c_str(),
		EMRCRYPT_BYTE_TYPE_NONE,
		GLOGIC::bGLOGIC_PACKFILE 
		) 
		);

	if ( !pBStream.get() )
	{
		if ( strFile.size() )
			CDebugSet::ErrorFile( "GLSetOptionGen::LoadSetOption", strFile.c_str() );

		return FALSE;
	}

	m_strFileName = strFile;

	basestream &SFile = *pBStream;

	DWORD dwFileVersion;
	char szFileType[_MAX_FNAME];
	SFile.GetFileType( szFileType, _MAX_FNAME, dwFileVersion );

	SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_OLD );

	if ( dwFileVersion>=VERSION_ENCODE )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_2021_SET_OPTION_V1 );

	CleanUp();

	if ( dwFileVersion == VERSION )
	{
		DWORD dwNum(0);
		SFile >> dwNum;

		for ( DWORD i=0; i<dwNum; i++ )
		{
			SSET_OPTION_GEN sOption;
			sOption.Load( SFile );

			m_mapSetOptionGen.insert( std::make_pair( sOption.strGenName, sOption ) );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "GLSetOptionGen::LoadSetOption", dwFileVersion );
	}

	return TRUE;
}

BOOL GLSetOptionGen::SaveSetOptionGen( std::string strFile )
{
	if ( strFile.empty() )	return FALSE;

	std::string strFullPath = GLOGIC::GetPath();
	strFullPath += strFile;

	CSerialFile SFile;
	SFile.SetFileType ( _FILEHEAD, VERSION );
	if ( !SFile.OpenFile ( FOT_WRITE, strFullPath.c_str() ) )	return FALSE;
	SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_2021_SET_OPTION_V1 );

	SFile << (DWORD)m_mapSetOptionGen.size();

	for( SET_OPTION_GEN_MAP_ITER iter = m_mapSetOptionGen.begin(); iter != m_mapSetOptionGen.end(); ++iter )
	{
		(*iter).second.Save( SFile );
	}

	m_strFileName = strFile;

	return TRUE;
}


const bool GLSetOptionGen::CSVLoad(CWnd* pWnd)
{
	// Clear;
	CleanUp();

	CFileDialog dlg( TRUE, ".csv", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "csv file (*.csv)|*.csv|", pWnd );

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
	if ( dlg.DoModal() != IDOK )
		return S_FALSE;

	CStringFile StrFile( 10240 );
	if( !StrFile.Open ( dlg.GetPathName().GetString() ) )	
		return S_FALSE;

	STRUTIL::ClearSeparator();
	STRUTIL::RegisterSeparator( "," );

	CString strLine;
	CStringArray StrArray;

	StrFile.GetNextLine( strLine );
	STRUTIL::StringSeparate( strLine, StrArray );

	int iLine = 1;

	while( StrFile.GetNextLine( strLine ) )
	{
		++iLine;

		STRUTIL::StringSeparate( strLine, StrArray );

		std::string _stringName(StrArray[0]);

		SSET_OPTION_GEN* pData = GetData(_stringName);
		if ( pData )
		{
			pData->LoadCsv(StrArray);
		}
		else
		{
			SSET_OPTION_GEN sData;
			sData.LoadCsv( StrArray );
			m_mapSetOptionGen.insert( std::make_pair( sData.strGenName, sData ) );
		}
	}

	return true;
}

const bool GLSetOptionGen::CSVSave(CWnd* pWnd)
{
	CFileDialog dlg( FALSE, ".csv", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "csv file (*.csv)|*.csv|", pWnd);

	std::string InitPath(GLOGIC::GetPath());
	dlg.m_ofn.lpstrInitialDir = InitPath.c_str();
	if (dlg.DoModal() != IDOK)
		return false;

	std::string FileName(dlg.GetPathName().GetString());
	std::fstream SFile;
	SFile.open(FileName.c_str(), std::ios_base::out);

	SSET_OPTION_GEN::SaveCsvHead(SFile);

	SET_OPTION_GEN_MAP_ITER it_b = m_mapSetOptionGen.begin();
	SET_OPTION_GEN_MAP_ITER it_e = m_mapSetOptionGen.end();
	for( ; it_b != it_e; ++it_b )
	{
		const SSET_OPTION_GEN& sData = (*it_b).second;
		(*it_b).second.SaveCsv( SFile );
	}

	SFile.close();
	return true;
}

SSET_OPTION_GEN* GLSetOptionGen::GetData( std::string strName )
{
	if ( strName.empty() )	return NULL;

	SET_OPTION_GEN_MAP_ITER iter = m_mapSetOptionGen.find( strName );
	if ( iter == m_mapSetOptionGen.end() )	return NULL;

	return &(*iter).second;
}


#pragma optimize( "", off )
WORD GLSetOptionGen::GenerateSetOptionID( std::string strFile )
{
	if ( strFile.empty() )	return SET_OPTION_NULL;

	SSET_OPTION_GEN* pGen = GetData( strFile );
	if ( !pGen )			return SET_OPTION_NULL;

	float fRatePer = seqrandom::getpercent();
	float fRateCur = 0.0f;

	for( int i=0; i<(int)pGen->vecGen.size(); ++i )
	{
		float fRate = pGen->vecGen[i].fGenChance;
		WORD wID = pGen->vecGen[i].wGenID;

		if( ( fRateCur <= fRatePer ) && ( fRatePer < ( fRateCur + fRate ) ) )
			return wID;

		fRateCur += fRate;
	}

	return SET_OPTION_NULL;
}
#pragma optimize( "", on )