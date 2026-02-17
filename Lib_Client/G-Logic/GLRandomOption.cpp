#include "StdAfx.h"
#include "GLRandomOption.h"

#include "../../Lib_Engine/Common/GLTexFile.h"
#include "../../Lib_Engine/G-Logic/GLogic.h"
#include "../../Lib_Engine/Common/StringFile.h"
#include "../../Lib_Engine/Common/StringUtils.h"

#include <afxdlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void SRANDOM_OPTION_SECTION::LoadSection_1_2 ( basestream &SFile, EMRANDOM_OPT emTYPE )
{
	bool bscale = COMMENT::isRandomValueScale( emTYPE );
	if ( bscale )
	{
		SFile >> fRATE;
		SFile >> fHIGH;
		SFile >> fLOW;

		fRATE = fRATE * 100.0f;
	}
	else
	{
		float frate;
		int nhigh, nlow;
		SFile >> frate;
		SFile >> nhigh;
		SFile >> nlow;

		fRATE = frate * 100.0f;
		fHIGH = float(nhigh);
		fLOW = float(nlow);
	}
}

void SRANDOM_OPTION_SECTION::LoadSection ( basestream &SFile )
{
	SFile >> fRATE;
	SFile >> fHIGH;
	SFile >> fLOW;
}

void SRANDOM_OPTION_SECTION::SaveSection ( CSerialFile &SFile )
{
	SFile << fRATE;
	SFile << fHIGH;
	SFile << fLOW;
}

void SRANDOM_OPTION_SET::LoadSet_1_2 ( basestream &SFile )
{
	DWORD dwtype(0);
	SFile >> fRATE;
	SFile >> dwtype; emTYPE = static_cast<EMRANDOM_OPT>(dwtype);

	bool bscale = COMMENT::isRandomValueScale( emTYPE );
	if ( bscale )
	{
		SFile >> fHIGH;
		SFile >> fLOW;
	}
	else
	{	
		int nhigh, nlow;
		SFile >> nhigh;
		SFile >> nlow;

		fHIGH = float(nhigh);
		fLOW = float(nlow);
	}

	DWORD dwsize(0);
	SFile >> dwsize;

	for ( DWORD i=0; i<dwsize; ++i )
	{
		SRANDOM_OPTION_SECTION section;
		section.LoadSection_1_2( SFile, emTYPE );
		vecSECT.push_back( section );
	}
}

void SRANDOM_OPTION_SET::LoadSet ( basestream &SFile )
{
	DWORD dwtype(0);
	SFile >> fRATE;
	SFile >> dwtype; emTYPE = static_cast<EMRANDOM_OPT>(dwtype);
	SFile >> fHIGH;
	SFile >> fLOW;

	DWORD dwsize(0);
	SFile >> dwsize;

	for ( DWORD i=0; i<dwsize; ++i )
	{
		SRANDOM_OPTION_SECTION random_section;
		random_section.LoadSection( SFile );
		vecSECT.push_back( random_section );
	}
}

void SRANDOM_OPTION_SET::SaveSet ( CSerialFile &SFile )
{
	SFile << fRATE;
	SFile << (DWORD)emTYPE;
	SFile << fHIGH;
	SFile << fLOW;

	SFile << (DWORD)vecSECT.size();
	for ( DWORD i=0; i<(DWORD)vecSECT.size(); ++i )
	{
		vecSECT[i].SaveSection( SFile );
	}
}

const bool SRANDOM_OPTION_SET::loadCsv(CStringArray& StrArray)
{
	fRATE	= (float)atof(StrArray[10]);
	emTYPE	= (EMRANDOM_OPT)atoi(StrArray[11]);

	const int nValueSize(atoi(StrArray[12]));
	vecSECT.reserve(nValueSize);

	int nCsvArray = 13;

	for ( INT i = 0; i < nValueSize; ++i )
	{
		SRANDOM_OPTION_SECTION _value;
		_value.fRATE = (float)atof(StrArray[nCsvArray++]);
		_value.fLOW = (float)atof(StrArray[nCsvArray++]);
		_value.fHIGH = (float)atof(StrArray[nCsvArray++]);
		vecSECT.push_back( _value );
	}

	GenerateRange();

	return true;
}

const bool SRANDOM_OPTION_SET::saveCsv(std::fstream& SFile)
{
	SFile << fRATE << ",";
	SFile << unsigned int(emTYPE) << ",";
	const unsigned int _sizeValue(vecSECT.size());
	SFile << _sizeValue << ",";

	for ( unsigned int _index(0); _index < _sizeValue; ++_index )
	{
		const SRANDOM_OPTION_SECTION& _value(vecSECT[_index]);
		SFile << _value.fRATE << ",";
		SFile << _value.fLOW << ",";
		SFile << _value.fHIGH << ",";
	}
	return true;
}

void SRANDOM_OPTION_SET::GenerateRange ()
{
	fLOW = 0.0f;
	fHIGH = 0.0f;

	if ( vecSECT.size() )
	{
		fLOW = vecSECT[0].fLOW;
		fHIGH = vecSECT[0].fHIGH;
	}

	for ( DWORD i=0; i<(DWORD)vecSECT.size(); ++i )
	{
		if ( vecSECT[i].fLOW < fLOW )
			fLOW = vecSECT[i].fLOW;

		if ( vecSECT[i].fHIGH > fHIGH )
			fHIGH = vecSECT[i].fHIGH;
	}
}

float SRANDOM_OPTION_SET::GetTotalRate ()
{
	float fTotal = 0.0f;

	for ( DWORD i=0; i<(DWORD)vecSECT.size(); ++i )
	{
		fTotal += vecSECT[i].fRATE;
	}

	return fTotal;
}

void SRANDOM_OPTION_GEN::LoadGen_2 ( basestream &SFile )
{
	SFile >> fRATE;
	SFile >> fD_point;
	SFile >> fS_value;

	SFile >> wMinGain;
	SFile >> wMaxGain;
	SFile >> dwMapSetSize;
	SFile >> dwMapSetRSize;

	DWORD dwsize(0);
	SFile >> dwsize;
	for ( DWORD i=0; i<dwsize; ++i )
	{
		SRANDOM_OPTION_SET random_set;
		random_set.LoadSet_1_2( SFile );
		mapSET.insert( std::make_pair( random_set.emTYPE, random_set ) );
	}

	DWORD dwsizeR(0);
	SFile >> dwsizeR;
	for ( DWORD i=0; i<dwsizeR; ++i )
	{
		SRANDOM_OPTION_SET random_set;
		random_set.LoadSet_1_2( SFile );
		mapSETR.insert( std::make_pair( random_set.emTYPE, random_set ) );
	}
}

void SRANDOM_OPTION_GEN::LoadGen_1 ( basestream &SFile )
{
	SFile >> fRATE;
	SFile >> fD_point;
	SFile >> fS_value;

	//SFile >> wMinGain;
	//SFile >> wMaxGain;
	wMinGain = 0;
	wMaxGain = 5;

	SFile >> dwMapSetSize;
	SFile >> dwMapSetRSize;

	DWORD dwsize(0);
	SFile >> dwsize;
	for ( DWORD i=0; i<dwsize; ++i )
	{
		SRANDOM_OPTION_SET random_set;
		random_set.LoadSet_1_2( SFile );
		mapSET.insert( std::make_pair( random_set.emTYPE, random_set ) );
	}

	DWORD dwsizeR(0);
	SFile >> dwsizeR;
	for ( DWORD i=0; i<dwsizeR; ++i )
	{
		SRANDOM_OPTION_SET random_set;
		random_set.LoadSet_1_2( SFile );
		mapSETR.insert( std::make_pair( random_set.emTYPE, random_set ) );
	}
}

void SRANDOM_OPTION_GEN::LoadGen_200 ( basestream &SFile )
{
	SFile >> fRATE;
	SFile >> fD_point;
	SFile >> fS_value;

	SFile >> wMinGain;
	SFile >> wMaxGain;
	SFile >> dwMapSetSize;
	SFile >> dwMapSetRSize;

	DWORD dwsize(0);
	SFile >> dwsize;
	for ( DWORD i=0; i<dwsize; ++i )
	{
		SRANDOM_OPTION_SET random_set;
		random_set.LoadSet( SFile );
		mapSET.insert( std::make_pair( random_set.emTYPE, random_set ) );
	}

	DWORD dwsizeR(0);
	SFile >> dwsizeR;
	for ( DWORD i=0; i<dwsizeR; ++i )
	{
		SRANDOM_OPTION_SET random_set;
		random_set.LoadSet( SFile );
		mapSETR.insert( std::make_pair( random_set.emTYPE, random_set ) );
	}
}

void SRANDOM_OPTION_GEN::LoadGen ( basestream &SFile )
{
	SFile >> fRATE;
	SFile >> fD_point;
	SFile >> fS_value;

	SFile >> wMinGain;
	SFile >> wMaxGain;
	SFile >> dwMapSetSize;
	SFile >> dwMapSetRSize;

	SFile >> wMaxGenD;
	SFile >> wMaxGenR;

	DWORD dwsize(0);
	SFile >> dwsize;
	for ( DWORD i=0; i<dwsize; ++i )
	{
		SRANDOM_OPTION_SET random_set;
		random_set.LoadSet( SFile );
		mapSET.insert( std::make_pair( random_set.emTYPE, random_set ) );
	}

	DWORD dwsizeR(0);
	SFile >> dwsizeR;
	for ( DWORD i=0; i<dwsizeR; ++i )
	{
		SRANDOM_OPTION_SET random_set;
		random_set.LoadSet( SFile );
		mapSETR.insert( std::make_pair( random_set.emTYPE, random_set ) );
	}
}

void SRANDOM_OPTION_GEN::SaveGen ( CSerialFile &SFile )
{
	SFile << fRATE;
	SFile << fD_point;
	SFile << fS_value;

	SFile << wMinGain;
	SFile << wMaxGain;
	SFile << dwMapSetSize;
	SFile << dwMapSetRSize;

	SFile << wMaxGenD;
	SFile << wMaxGenR;

	SFile << (DWORD)mapSET.size();
	for( SRANDOM_OPTION_SET_MAP_ITER iter = mapSET.begin(); iter != mapSET.end(); ++ iter )
	{
		(*iter).second.SaveSet( SFile );
	}

	SFile << (DWORD)mapSETR.size();
	for( SRANDOM_OPTION_SET_MAP_ITER iter = mapSETR.begin(); iter != mapSETR.end(); ++ iter )
	{
		(*iter).second.SaveSet( SFile );
	}
}

const bool SRANDOM_OPTION_GEN::loadCsv(CStringArray& StrArray)
{
	fRATE = (float)atof(StrArray[1]);
	fD_point = (float)atof(StrArray[2]);
	fS_value = (float)atof(StrArray[3]);
	wMinGain = atoi(StrArray[4]);
	wMaxGain = atoi(StrArray[5]);

	wMaxGenD = atoi(StrArray[6]);
	wMaxGenR = atoi(StrArray[7]);

	int nGroup = atoi(StrArray[8]);
	int nSize = atoi(StrArray[9]);

	for ( int i=0; i<nSize; ++i )
	{
		SRANDOM_OPTION_SET sSet;
		sSet.loadCsv( StrArray );

		if ( nGroup == 0 )
		{
			mapSET[sSet.emTYPE] = sSet;
		}
		else if ( nGroup == 1 )
		{
			mapSETR[sSet.emTYPE] = sSet;
		}
	}

	return true;
}

const bool SRANDOM_OPTION_GEN::saveCsv(std::fstream& SFile)
{
	std::string _stringName(strFileName);
	std::transform(_stringName.begin(), _stringName.end(), _stringName.begin(), tolower);

	for( SRANDOM_OPTION_SET_MAP_ITER iter = mapSET.begin(); iter != mapSET.end(); ++ iter )
	{
		SFile << _stringName << ",";
		SFile << fRATE << ",";
		SFile << fD_point << ",";
		SFile << fS_value << ",";
		SFile << wMinGain << ",";
		SFile << wMaxGain << ",";

		SFile << wMaxGenD << ",";
		SFile << wMaxGenR << ",";

		SFile << 0 << ",";
		SFile << (int)mapSET.size() << ","; 

		SRANDOM_OPTION_SET sRandomOptionSet = (*iter).second;
		sRandomOptionSet.saveCsv(SFile);

		SFile << std::endl;
	}

	for( SRANDOM_OPTION_SET_MAP_ITER iter = mapSETR.begin(); iter != mapSETR.end(); ++ iter )
	{
		SFile << _stringName << ",";
		SFile << fRATE << ",";
		SFile << fD_point << ",";
		SFile << fS_value << ",";
		SFile << wMinGain << ",";
		SFile << wMaxGain << ",";
		
		SFile << wMaxGenD << ",";
		SFile << wMaxGenR << ",";

		SFile << 1 << ",";
		SFile << (int)mapSETR.size() << ","; 

		SRANDOM_OPTION_SET sRandomOptionSet = (*iter).second;
		sRandomOptionSet.saveCsv(SFile);

		SFile << std::endl;
	}

	return true;
}

BOOL SRANDOM_OPTION_GEN::CheckData ()
{
	if ( GetSetSize() != GetSetRSize() )
	{
		CDebugSet::ToFileWithTime( "_random_option_load.txt", "%s mapset size:%u not equal to mapsetR size: %u", strFileName.c_str(), GetSetSize(), GetSetRSize() );
		return TRUE;
	}

	for( SRANDOM_OPTION_SET_MAP_ITER iter = mapSET.begin(); iter != mapSET.end(); ++ iter )
	{
		if ( (*iter).second.fRATE > 100.0f || (*iter).second.fRATE < 0.0f )
		{
			CDebugSet::ToFileWithTime( "_random_option_load.txt", "%s mapset Option: %u rate:% g should be 0~100", strFileName.c_str(), (*iter).second.emTYPE,  (*iter).second.fRATE );

			return TRUE;
			break;
		}
	}

	for( SRANDOM_OPTION_SET_MAP_ITER iter = mapSETR.begin(); iter != mapSETR.end(); ++ iter )
	{
		if ( (*iter).second.fRATE > 100.0f || (*iter).second.fRATE < 0.0f )
		{
			CDebugSet::ToFileWithTime( "_random_option_load.txt", "%s mapsetR Option: %u rate: %g should be 0~100", strFileName.c_str(), (*iter).second.emTYPE,  (*iter).second.fRATE );

			return TRUE;
			break;
		}
	}


	SRANDOM_OPTION_SET_MAP_CITER it2_b = mapSET.begin();
	SRANDOM_OPTION_SET_MAP_CITER it2_e = mapSET.end();
	for( ; it2_b != it2_e; ++it2_b )
	{
		const SRANDOM_OPTION_SET &random_option_set = (*it2_b).second;

		float fRate = 0.0;
		for ( unsigned int i=0; i<random_option_set.vecSECT.size(); ++i )
		{
			fRate += random_option_set.vecSECT[i].fRATE;
		}

		if( std::abs(fRate - 100.0f) > 0.001 )
		{
			CDebugSet::ToFileWithTime( "_random_option_load.txt", "%s mapset Option: %u vecset total rate: %g should be 100", strFileName.c_str(), random_option_set.emTYPE,  fRate );

			return TRUE;
			break;
		}
	}

	it2_b = mapSETR.begin();
	it2_e = mapSETR.end();
	for( ; it2_b != it2_e; ++it2_b )
	{
		const SRANDOM_OPTION_SET &random_option_set = (*it2_b).second;

		float fRate = 0.0;
		for ( unsigned int i=0; i<random_option_set.vecSECT.size(); ++i )
		{
			fRate += random_option_set.vecSECT[i].fRATE;
		}

		if( std::abs(fRate - 100.0f) > 0.001 )
		{
			CDebugSet::ToFileWithTime( "_random_option_load.txt", "%s mapsetR Option: %u vecset total rate: %g should be 100", strFileName.c_str(), random_option_set.emTYPE,  fRate );

			return TRUE;
			break;
		}
	}




	return FALSE;
}

const char* GLRandomOptionManager::_FILEHEAD = "RandomOptionServer";

GLRandomOptionManager& GLRandomOptionManager::GetInstance ()
{
	static GLRandomOptionManager _instance;
	return _instance;
}

GLRandomOptionManager::GLRandomOptionManager()
{
	memset(m_szFileName, 0, sizeof(char) * (MAX_PATH));
}

GLRandomOptionManager::~GLRandomOptionManager()
{
}

void GLRandomOptionManager::CleanUp()
{
	m_mapRandomOption.clear();
}

HRESULT GLRandomOptionManager::LoadFile ( const char* szFile )
{
	char szFullPath[MAX_PATH] = {0};
	StringCchCopy ( szFullPath, MAX_PATH, GLOGIC::GetServerPath() );
	StringCchCat ( szFullPath, MAX_PATH, szFile );

	std::auto_ptr<basestream> pBStream( GLOGIC::openfile_basestream(GLOGIC::bGLOGIC_ZIPFILE, 
		GLOGIC::strGLOGIC_SERVER_ZIPFILE.c_str(), szFullPath, szFile,
		EMRCRYPT_BYTE_TYPE_NONE, GLOGIC::bGLOGIC_PACKFILE ) );

	if ( !pBStream.get() )
	{
		if ( strlen( szFile ) > 0 )
			CDebugSet::ErrorFile( "GLRandomOptionManager::LoadFile", szFile );

		return E_FAIL;
	}

	StringCchCopy( m_szFileName, MAX_PATH, szFile );

	basestream &SFile = *pBStream;

	DWORD dwfile_version;
	char szfile_type[_MAX_FNAME];
	SFile.GetFileType( szfile_type, _MAX_FNAME, dwfile_version );

	SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_OLD );

	if ( dwfile_version>=VERSION_ENCODE )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_RANDOM_OPTION_FILE_VER_1 );
	
	if ( dwfile_version>=VERSION_ENCODE2 )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_RANDOM_OPTION_FILE_VER_2 );

	if ( dwfile_version>=ENCODE_VER_BYTE_CRYPT_2021_RANDOM_OPTION_V1 )
		SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_2021_RANDOM_OPTION_V1 );

	CleanUp();

	if ( dwfile_version == VERSION )
	{
		DWORD dwnum(0);
		SFile >> dwnum;

		for ( DWORD i=0; i<dwnum; i++ )
		{
			std::string strbinfile;
			SFile >> strbinfile;

			std::transform ( strbinfile.begin(), strbinfile.end(), strbinfile.begin(), tolower );

			SRANDOM_OPTION_GEN sdata;
			sdata.LoadGen( SFile );
			sdata.strFileName = strbinfile;
			m_mapRandomOption.insert( std::make_pair( sdata.strFileName, sdata ) );
		}
	}
	else if ( dwfile_version == 0x0202 )
	{
		DWORD dwnum(0);
		SFile >> dwnum;

		for ( DWORD i=0; i<dwnum; i++ )
		{
			std::string strbinfile;
			SFile >> strbinfile;

			std::transform ( strbinfile.begin(), strbinfile.end(), strbinfile.begin(), tolower );

			SRANDOM_OPTION_GEN sdata;
			sdata.LoadGen_200( SFile );
			sdata.strFileName = strbinfile;
			m_mapRandomOption.insert( std::make_pair( sdata.strFileName, sdata ) );
		}
	}
	else if ( dwfile_version == 0x0201 )
	{
		DWORD dwnum(0);
		SFile >> dwnum;

		for ( DWORD i=0; i<dwnum; i++ )
		{
			std::string strbinfile;
			SFile >> strbinfile;

			std::transform ( strbinfile.begin(), strbinfile.end(), strbinfile.begin(), tolower );

			SRANDOM_OPTION_GEN sdata;
			sdata.LoadGen_200( SFile );
			sdata.strFileName = strbinfile;
			m_mapRandomOption.insert( std::make_pair( sdata.strFileName, sdata ) );
		}
	}
	else if ( dwfile_version == 0x0200 )
	{
		DWORD dwnum(0);
		SFile >> dwnum;

		for ( DWORD i=0; i<dwnum; i++ )
		{
			std::string strbinfile;
			SFile >> strbinfile;

			std::transform ( strbinfile.begin(), strbinfile.end(), strbinfile.begin(), tolower );

			SRANDOM_OPTION_GEN sdata;
			sdata.LoadGen_200( SFile );
			sdata.strFileName = strbinfile;
			m_mapRandomOption.insert( std::make_pair( sdata.strFileName, sdata ) );
		}
	}
	else if ( dwfile_version == 2 )
	{
		DWORD dwnum(0);
		SFile >> dwnum;

		for ( DWORD i=0; i<dwnum; i++ )
		{
			std::string strbinfile;
			SFile >> strbinfile;

			std::transform ( strbinfile.begin(), strbinfile.end(), strbinfile.begin(), tolower );

			SRANDOM_OPTION_GEN sdata;
			sdata.LoadGen_2( SFile );
			sdata.strFileName = strbinfile;
			m_mapRandomOption.insert( std::make_pair( sdata.strFileName, sdata ) );
		}
	}
	else if ( dwfile_version == 1 )
	{
		DWORD dwnum(0);
		SFile >> dwnum;

		for ( DWORD i=0; i<dwnum; i++ )
		{
			std::string strbinfile;
			SFile >> strbinfile;

			std::transform ( strbinfile.begin(), strbinfile.end(), strbinfile.begin(), tolower );

			SRANDOM_OPTION_GEN sdata;
			sdata.LoadGen_1( SFile );
			sdata.strFileName = strbinfile;
			m_mapRandomOption.insert( std::make_pair( sdata.strFileName, sdata ) );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "GLRandomOptionManager::LoadFile", dwfile_version );
	}
	
	return S_OK;
}

HRESULT GLRandomOptionManager::SaveFile ( const char* szFile )
{
	char szFullPath[MAX_PATH];
	StringCchCopy ( szFullPath, MAX_PATH, GLOGIC::GetServerPath() );
	StringCchCat ( szFullPath, MAX_PATH, szFile );

	CSerialFile SFile;
	SFile.SetFileType ( _FILEHEAD, VERSION );
	if ( !SFile.OpenFile ( FOT_WRITE, szFullPath ) )	return E_FAIL;
	SFile.SetEncodeType ( EMRCRYPT_BYTE_TYPE_2021_RANDOM_OPTION_V1 );

	SFile << (DWORD)m_mapRandomOption.size();
	for( SRANDOM_OPTION_GEN_MAP_ITER iter = m_mapRandomOption.begin(); iter != m_mapRandomOption.end(); ++iter )
	{
		SFile << (*iter).second.strFileName;
		(*iter).second.SaveGen( SFile );
	}

	StringCchCopy( m_szFileName, MAX_PATH, szFile );

	return S_OK;
}

SRANDOM_OPTION_GEN* GLRandomOptionManager::GetRandomOption( std::string strFile )
{
	if ( strFile.empty() )	return NULL;

	std::transform ( strFile.begin(), strFile.end(), strFile.begin(), tolower );

	SRANDOM_OPTION_GEN_MAP_ITER iter = m_mapRandomOption.find( strFile );
	if ( iter == m_mapRandomOption.end() )	return NULL;

	return &(*iter).second;
}

const bool GLRandomOptionManager::CSVLoad(CWnd* pWnd)
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

		SRANDOM_OPTION_GEN* pGen = GetRandomOption(_stringName);
		if ( pGen )
		{
			pGen->loadCsv(StrArray);
		}
		else
		{
			SRANDOM_OPTION_GEN sGen;
			sGen.strFileName = _stringName;
			sGen.loadCsv( StrArray );
			m_mapRandomOption.insert( std::make_pair( sGen.strFileName, sGen ) );
		}
	}

	return true;
}

const bool GLRandomOptionManager::CSVSave(CWnd* pWnd)
{
	CFileDialog dlg( FALSE, ".csv", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "csv file (*.csv)|*.csv|", pWnd);

	std::string InitPath(GLOGIC::GetPath());
	dlg.m_ofn.lpstrInitialDir = InitPath.c_str();
	if (dlg.DoModal() != IDOK)
		return false;

	std::string FileName(dlg.GetPathName().GetString());
	std::fstream streamFILE;
	streamFILE.open(FileName.c_str(), std::ios_base::out);

	saveCsvHead(streamFILE);

	SRANDOM_OPTION_GEN_MAP_ITER it_b = m_mapRandomOption.begin();
	SRANDOM_OPTION_GEN_MAP_ITER it_e = m_mapRandomOption.end();
	for( ; it_b != it_e; ++it_b )
	{
		std::string strName = (*it_b).second.strFileName;
		(*it_b).second.saveCsv( streamFILE );
	}

	streamFILE.close();
	return true;
}

void GLRandomOptionManager::saveCsvHead(std::fstream& SFile) const
{
	SFile << "Random Option" << ",";	// 0
	SFile << "Rate" << ",";				// 1 
	SFile << "D_Point" << ",";			// 2
	SFile << "S_Point" << ",";			// 3
	SFile << "wMinGain" << ",";			// 4
	SFile << "wMaxGain" << "," ;		// 5

	SFile << "wMaxGenD" << "," ;		// 6
	SFile << "wMaxGenR" << "," ;		// 7

	SFile << "Group" << ",";			// 8
	SFile << "Option Size" << ",";		// 9

	SFile << "Option Rate" << "," << "Option Type" << "," << "Section Num" << ",";
	SFile << "Rate1" << ","  << "Min 1" << "," << "Max 1" << ",";
	SFile << "Rate2" << ","  << "Min 2" << "," << "Max 2" << ",";
	SFile << "Rate3" << ","  << "Min 3" << "," << "Max 3" << ",";
	SFile << "Rate4" << ","  << "Min 4" << "," << "Max 4" << ",";
	SFile << "Rate5" << ","  << "Min 5" << "," << "Max 5" << ",";
	SFile << "Rate6" << ","  << "Min 6" << "," << "Max 6" << ",";
	SFile << "Rate7" << ","  << "Min 7" << "," << "Max 7" << ",";
	SFile << "Rate8" << ","  << "Min 8" << "," << "Max 8" << ",";
	SFile << "Rate9" << ","  << "Min 9" << "," << "Max 9" << ",";
	SFile << "Rate10" << "," << "Min 10" << ","<< "Max 10"<< ",";

	SFile << std::endl;
}

HRESULT GLRandomOptionManager::Import ( const char* szFile )
{
	if( !szFile )				return E_FAIL;
	if( strlen( szFile ) == 0 )	return E_FAIL;

	std::string strPath;
	strPath = GLOGIC::GetServerPath();
	strPath += szFile;

	SRANDOM_OPTION_GEN random_option_gen;
	random_option_gen.strFileName = szFile;
	std::transform ( random_option_gen.strFileName.begin(), random_option_gen.strFileName.end(), random_option_gen.strFileName.begin(), tolower );

	gltexfile cFILE;
	cFILE.reg_sep( '\t' );
	cFILE.reg_sep( ' ' );
	cFILE.reg_sep( ',' );

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_SERVER_ZIPFILE );

	if( !cFILE.open( strPath, true ) )	return E_FAIL;

	cFILE.getflag( "fRATE", 1, 1, random_option_gen.fRATE );
	cFILE.getflag( "D_point", 1, 1, random_option_gen.fD_point );
	cFILE.getflag( "S_value", 1, 1, random_option_gen.fS_value );

	DWORD dwset_num = 0;
	cFILE.getflag( "dwSET_NUM", 1, 1, dwset_num );

	TCHAR szOPTION[64] = {0};
	TCHAR szOPT_SE[64] = {0};

	for( DWORD i=0; i<dwset_num; ++i )
	{
		SRANDOM_OPTION_SET random_option_set;

		DWORD dwsection_num = 0;
		DWORD dwsection_type = 0;
		_snprintf( szOPTION, 64, "NEW_OPTION_%u", i+1 );
		cFILE.getflag( szOPTION, 1, 3, random_option_set.fRATE );
		cFILE.getflag( szOPTION, 2, 3, dwsection_type);
		cFILE.getflag( szOPTION, 3, 3, dwsection_num );

		random_option_set.emTYPE = static_cast<EMRANDOM_OPT> (dwsection_type);

		for( DWORD j=0; j<dwsection_num; ++j )
		{
			SRANDOM_OPTION_SECTION random_option_section;
			_snprintf( szOPT_SE, 64, "NEW_OPT_SE_%u_%u", i+1, j+1 );
			cFILE.getflag( szOPT_SE, 1, 3, random_option_section.fRATE );
			cFILE.getflag( szOPT_SE, 2, 3, random_option_section.fHIGH );
			cFILE.getflag( szOPT_SE, 3, 3, random_option_section.fLOW );
			random_option_set.vecSECT.push_back( random_option_section );
		}

		random_option_set.GenerateRange();
		random_option_gen.mapSET.insert( std::make_pair( random_option_set.emTYPE, random_option_set ) );
	}

	for( DWORD i=0; i<dwset_num; ++i )
	{
		SRANDOM_OPTION_SET random_option_set;

		DWORD dwsection_num = 0;
		DWORD dwsection_type = 0;
		_snprintf( szOPTION, 64, "REB_OPTION_%u", i+1 );
		cFILE.getflag( szOPTION, 1, 3, random_option_set.fRATE );
		cFILE.getflag( szOPTION, 2, 3, dwsection_type);
		cFILE.getflag( szOPTION, 3, 3, dwsection_num );

		random_option_set.emTYPE = static_cast<EMRANDOM_OPT> (dwsection_type);

		for( DWORD j=0; j<dwsection_num; ++j )
		{
			SRANDOM_OPTION_SECTION random_option_section;
			_snprintf( szOPT_SE, 64, "REB_OPT_SE_%u_%u", i+1, j+1 );
			cFILE.getflag( szOPT_SE, 1, 3, random_option_section.fRATE );
			cFILE.getflag( szOPT_SE, 2, 3, random_option_section.fHIGH );
			cFILE.getflag( szOPT_SE, 3, 3, random_option_section.fLOW );
			random_option_set.vecSECT.push_back( random_option_section );
		}

		random_option_set.GenerateRange();
		random_option_gen.mapSETR.insert( std::make_pair( random_option_set.emTYPE, random_option_set ) );
	}

	m_mapRandomOption.insert( std::make_pair( random_option_gen.strFileName, random_option_gen ) );

	return S_OK;
}


DWORD  GLRandomOptionManager::Export ()
{
	WORD wsaved = 0;
	for( SRANDOM_OPTION_GEN_MAP_ITER iter = m_mapRandomOption.begin(); iter != m_mapRandomOption.end(); ++iter )
	{
		SRANDOM_OPTION_GEN random_option_gen = (*iter).second;

		std::string stroutfile = GLOGIC::GetServerPath();
		stroutfile += "\\";
		stroutfile += random_option_gen.strFileName;

		CFile file;
		if ( file.Open( stroutfile.c_str(), CFile::modeCreate|CFile::modeWrite ) )
		{

			CString strtextbuffer;
			strtextbuffer.Format( "//Random Option File %s --EJCode \n", random_option_gen.strFileName.c_str() );
			strtextbuffer +="//\n";
			strtextbuffer +="//\n";
			strtextbuffer.AppendFormat( "fRATE\t%.2f\n", random_option_gen.fRATE );
			strtextbuffer.AppendFormat( "dwSET_NUM\t%u\n", random_option_gen.GetSetSize() );
			strtextbuffer.AppendFormat( "D_point\t%.2f\n", random_option_gen.fD_point );
			strtextbuffer.AppendFormat( "S_value\t%.2f\n", random_option_gen.fS_value );

			strtextbuffer +="//\n";

			DWORD dwindex1 = 0;
			for( SRANDOM_OPTION_SET_MAP_ITER iterset = random_option_gen.mapSET.begin(); iterset != random_option_gen.mapSET.end(); ++iterset, dwindex1 ++ )
			{
				SRANDOM_OPTION_SET random_option_set = (*iterset).second;

				strtextbuffer.AppendFormat( "NEW_OPTION_%u\t%.2f%s\t%u%s\t%u\n", dwindex1+1, random_option_set.fRATE, ",",random_option_set.emTYPE, ",", random_option_set.GetSectionSize() );

				for( DWORD k=0; k<random_option_set.GetSectionSize(); ++k )
				{
					strtextbuffer.AppendFormat( "NEW_OPT_SE_%u_%u\t%.2f%s\t%.2f%s\t%.2f\n", dwindex1+1, k+1, random_option_set.vecSECT[k].fRATE, ",", random_option_set.vecSECT[k].fHIGH, ",", random_option_set.vecSECT[k].fLOW );
				}

				strtextbuffer +="//\n";
			}

			strtextbuffer +="//\n";

			DWORD dwindex2 = 0;
			for( SRANDOM_OPTION_SET_MAP_ITER iterset = random_option_gen.mapSETR.begin(); iterset != random_option_gen.mapSETR.end(); ++iterset, dwindex2 ++ )
			{
				SRANDOM_OPTION_SET random_option_set = (*iterset).second;

				strtextbuffer.AppendFormat( "REB_OPTION_%u\t%.2f%s\t%u%s\t%u\n", dwindex2+1, random_option_set.fRATE, ",",random_option_set.emTYPE, ",", random_option_set.GetSectionSize() );

				for( DWORD k=0; k<random_option_set.GetSectionSize(); ++k )
				{
					strtextbuffer.AppendFormat( "REB_OPT_SE_%u_%u\t%.2f%s\t%.2f%s\t%.2f\n", dwindex2+1, k+1, random_option_set.vecSECT[k].fRATE, ",", random_option_set.vecSECT[k].fHIGH, ",", random_option_set.vecSECT[k].fLOW );
				}

				strtextbuffer +="//\n";
			}

			strtextbuffer +="//\n";

			CRijndael oRijndael;
			oRijndael.Initialize( CRijndael::VERSION, CRijndael::sm_Version[CRijndael::VERSION-1].c_str(), CRijndael::sm_KeyLength[CRijndael::VERSION-1] );

			int nLen = oRijndael.GetEncryptStringLength( strtextbuffer );
			char * szBuffer = new char[nLen];
			memset( szBuffer, 0, nLen );
			oRijndael.EncryptEx( strtextbuffer.GetString(), szBuffer, strtextbuffer.GetLength() );
			int nVersion = CRijndael::VERSION;
			file.Write( &nVersion, sizeof(int) );
			file.Write( szBuffer, nLen );
			file.Close();
			delete [] szBuffer;

			wsaved++;
		}

	}

	return wsaved;
}