#include "stdafx.h"
#include "./GLCrowDataWarden.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool SCROWWARDEN::LOAD_0100( basestream &SFile )
{
	SFile >> dwData1;
	SFile >> dwData2;
	SFile >> dwGenerateMoney;

	std::string strTextArray;
	SFile >> strTextArray;

	std::string strDelimiter = ";";
	
	int nIndex = 0;
	if ( strTextArray.size() )
	{
		std::string strSearch = strTextArray.c_str();
		size_t pos = 0;	
		while ((pos = strSearch.find(strDelimiter)) != std::string::npos) 
		{
			std::string token = strSearch.substr(0, pos);

			if ( token.find(".genitem") != std::string::npos )
			{	
				if ( nIndex < SCROWWARDEN::SCROWWARDEN_MAX )
				{
					strGenerateItem[nIndex] = token.c_str();
					nIndex ++;
				}
			}


			CDebugSet::ToFile(  "123.txt", "%s", token.c_str() );

			strSearch.erase(0, pos + strDelimiter.length());
		}
	}

	return true;
}

bool SCROWWARDEN::LOAD( basestream &SFile )
{
	SFile >> dwData1;
	SFile >> dwData2;
	SFile >> dwGenerateMoney;

	for ( int i=0; i<SCROWWARDEN::SCROWWARDEN_MAX; ++i )
		SFile >> strGenerateItem[i];

	return true;
}

bool SCROWWARDEN::SAVE( CSerialFile &SFile )
{
	SFile << dwData1;
	SFile << dwData2;
	SFile << dwGenerateMoney;

	for ( int i=0; i<SCROWWARDEN::SCROWWARDEN_MAX; ++i )
		SFile << strGenerateItem[i];

	return true;
}

VOID SCROWWARDEN::SaveCsvHead ( std::fstream &SFile )
{
	SFile << "dwData1" << ",";
	SFile << "dwData2" << ",";
	SFile << "dwGenerateMoney" << ",";

	for( int i=0; i<SCROWWARDEN::SCROWWARDEN_MAX; ++i )
		SFile << "strGenerateItem" << ",";
}

VOID SCROWWARDEN::SaveCsv ( std::fstream &SFile )
{
	SFile << dwData1 << ",";
	SFile << dwData2 << ",";
	SFile << dwGenerateMoney << ",";

	for( int i=0; i<SCROWWARDEN::SCROWWARDEN_MAX; ++i )
		STRUTIL::OutputStrCsv( SFile, strGenerateItem[i] );
}

VOID SCROWWARDEN::LoadCsv ( CStringArray &StrArray, int& iCsvCur )
{
	dwData1 = (DWORD)atol( StrArray[ iCsvCur++ ] );
	dwData2 = (DWORD)atol( StrArray[ iCsvCur++ ] );
	dwGenerateMoney = (DWORD)atol( StrArray[ iCsvCur++ ] );

	for( int i=0; i<SCROWWARDEN::SCROWWARDEN_MAX; ++i )
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], strGenerateItem[i] );
}