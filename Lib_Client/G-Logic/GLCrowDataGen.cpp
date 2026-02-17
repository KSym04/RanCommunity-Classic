#include "stdafx.h"
#include "./GLCrowDataGen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool SCROWGEN::LOAD_0200 ( basestream &SFile )
{
	SFile >> m_wGenItem_Rate;
	SFile >> m_wGenMoney_Rate;
	SFile >> m_sGenItemID.dwID;
	SFile >> m_dwGenMoney;
	SFile >> m_strQtGenItem[0];
	SFile >> m_strGenItem[0];

	/*Money Luck, Juver, 2017/08/24 */
	SFile >> m_fMoneyLuckRate;
	SFile >> m_wMoneyLuckMax;

	return true;
}

/* multiple crow gen, Juver, 2018/02/21 */
bool SCROWGEN::LOAD_0104 ( basestream &SFile )
{
	SFile >> m_wGenItem_Rate;
	SFile >> m_wGenMoney_Rate;
	SFile >> m_sGenItemID.dwID;
	SFile >> m_dwGenMoney;

	std::string strTextArray1, strTextArray2;
	SFile >> strTextArray1;
	SFile >> strTextArray2;

	std::transform ( strTextArray1.begin(), strTextArray1.end(), strTextArray1.begin(), tolower );
	std::transform ( strTextArray2.begin(), strTextArray2.end(), strTextArray2.begin(), tolower );

	std::string strDelimiter = ";";
	std::map<std::string,std::string> mapgenitem;
	std::map<std::string,std::string> mapqgenitem;
	std::map<std::string,std::string>::iterator iter;

	if ( strTextArray1.size() )
	{
		std::string strSearch = strTextArray1.c_str();
		size_t pos = 0;	
		while ((pos = strSearch.find(strDelimiter)) != std::string::npos) 
		{
			std::string token = strSearch.substr(0, pos);
			
			if ( token.find(".genitem") != std::string::npos ){	
				if ( token.find("q_") != std::string::npos )
					mapqgenitem[token.c_str()] = token.c_str();
				else
					mapgenitem[token.c_str()] = token.c_str();
			}

			strSearch.erase(0, pos + strDelimiter.length());
		}
	}

	if ( strTextArray2.size() )
	{
		std::string strSearch = strTextArray2.c_str();
		size_t pos = 0;	
		while ((pos = strSearch.find(strDelimiter)) != std::string::npos) 
		{
			std::string token = strSearch.substr(0, pos);

			if ( token.find(".genitem") != std::string::npos ){	
				if ( token.find("q_") != std::string::npos )
					mapqgenitem[token.c_str()] = token.c_str();
				else
					mapgenitem[token.c_str()] = token.c_str();
			}

			strSearch.erase(0, pos + strDelimiter.length());
		}
	}

	for ( int i=0; i<MAX_QGEN_ITEM_ARRAY; ++i )
		m_strQtGenItem[i] = "";

	for ( int i=0; i<MAX_GEN_ITEM_ARRAY; ++i )
		m_strGenItem[i] = "";

	if ( mapgenitem.size() > 0 )
	{
		WORD windex = 0;
		for ( iter = mapgenitem.begin(); iter != mapgenitem.end(); ++ iter )
		{
			std::string strtext = iter->second;
			if ( windex >= MAX_GEN_ITEM_ARRAY )	continue;
			m_strGenItem[windex] = strtext;
			windex++;
		}
	}

	if ( mapqgenitem.size() > 0 )
	{
		WORD windex = 0;
		for ( iter = mapqgenitem.begin(); iter != mapqgenitem.end(); ++ iter )
		{
			std::string strtext = iter->second;
			if ( windex >= MAX_QGEN_ITEM_ARRAY )	continue;
			m_strQtGenItem[windex] = strtext;
			windex++;
		}
	}

	return true;
}

bool SCROWGEN::LOAD_0103 ( basestream &SFile )
{
	SFile >> m_wGenItem_Rate;
	SFile >> m_wGenMoney_Rate;
	SFile >> m_sGenItemID.dwID;
	SFile >> m_dwGenMoney;
	SFile >> m_strQtGenItem[0];
	SFile >> m_strGenItem[0];

	return true;
}

bool SCROWGEN::LOAD_0102 ( basestream &SFile )
{
	SFile >> m_dwGenMoney;
	SFile >> m_sGenItemID.dwID;
	SFile >> m_wGenMoney_Rate;
	SFile >> m_wGenItem_Rate;

	SFile >> m_strGenItem[0];
	SFile >> m_strQtGenItem[0];

	return true;
}

bool SCROWGEN::LOAD ( basestream &SFile )
{
	SFile >> m_wGenItem_Rate;
	SFile >> m_wGenMoney_Rate;
	SFile >> m_sGenItemID.dwID;
	SFile >> m_dwGenMoney;

	/* multiple crow gen, Juver, 2018/02/21 */
	for ( int i=0; i<SCROWGEN::MAX_QGEN_ITEM_ARRAY; ++i )
		SFile >> m_strQtGenItem[i];

	for ( int i=0; i<SCROWGEN::MAX_GEN_ITEM_ARRAY; ++i )
		SFile >> m_strGenItem[i];

	/*Money Luck, Juver, 2017/08/24 */
	SFile >> m_fMoneyLuckRate;
	SFile >> m_wMoneyLuckMax;

	return true;
}

bool SCROWGEN::SAVE ( CSerialFile &SFile )
{
	SFile << m_wGenItem_Rate;
	SFile << m_wGenMoney_Rate;
	SFile << m_sGenItemID.dwID;
	SFile << m_dwGenMoney;

	/* multiple crow gen, Juver, 2018/02/21 */
	for ( int i=0; i<SCROWGEN::MAX_QGEN_ITEM_ARRAY; ++i )
		SFile << m_strQtGenItem[i];

	for ( int i=0; i<SCROWGEN::MAX_GEN_ITEM_ARRAY; ++i )
		SFile << m_strGenItem[i];

	/*Money Luck, Juver, 2017/08/24 */
	SFile << m_fMoneyLuckRate;
	SFile << m_wMoneyLuckMax;

	return true;
}

VOID SCROWGEN::SaveCsvHead ( std::fstream &SFile )
{
	SFile << "dwGenMoney" << ",";
	SFile << "sGenItemID wMainID" << ",";
	SFile << "sGenItemID wSubID" << ",";
	SFile << "wGenMoney_Rate" << ",";
	SFile << "wGenItem_Rate" << ",";

	/*Money Luck, Juver, 2017/08/24 */
	SFile << "fMoneyLuckRate" << ",";
	SFile << "wMoneyLuckMax" << ",";

	/* multiple crow gen, Juver, 2018/02/21 */
	for( int i=0; i<SCROWGEN::MAX_GEN_ITEM_ARRAY; ++i )
		SFile << " strGenItem " << i << ",";

	for( int i=0; i<SCROWGEN::MAX_QGEN_ITEM_ARRAY; ++i )
		SFile << " strQtGenItem " << i << ",";
}

VOID SCROWGEN::SaveCsv ( std::fstream &SFile )
{
	SFile << m_dwGenMoney << ",";
	SFile << m_sGenItemID.wMainID << ",";
	SFile << m_sGenItemID.wSubID << ",";
	SFile << m_wGenMoney_Rate << ",";
	SFile << m_wGenItem_Rate << ",";

	/*Money Luck, Juver, 2017/08/24 */
	SFile << m_fMoneyLuckRate << ",";
	SFile << m_wMoneyLuckMax << ",";

	/* multiple crow gen, Juver, 2018/02/21 */
	for( int i=0; i<SCROWGEN::MAX_GEN_ITEM_ARRAY; ++i )
		STRUTIL::OutputStrCsv( SFile, m_strGenItem[i] );

	for( int i=0; i<SCROWGEN::MAX_QGEN_ITEM_ARRAY; ++i )
		STRUTIL::OutputStrCsv( SFile, m_strQtGenItem[i] );
}

VOID SCROWGEN::LoadCsv ( CStringArray &StrArray, int& iCsvCur )
{
	m_dwGenMoney = (DWORD)atol( StrArray[ iCsvCur++ ] );
	m_sGenItemID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
	m_sGenItemID.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
	m_wGenMoney_Rate = (WORD)atoi( StrArray[ iCsvCur++ ] );
	m_wGenItem_Rate = (WORD)atoi( StrArray[ iCsvCur++ ] );

	/*Money Luck, Juver, 2017/08/24 */
	m_fMoneyLuckRate = (float)atof( StrArray[ iCsvCur++ ] );
	m_wMoneyLuckMax = (WORD)atoi( StrArray[ iCsvCur++ ] );

	/* multiple crow gen, Juver, 2018/02/21 */
	for( int i=0; i<SCROWGEN::MAX_GEN_ITEM_ARRAY; ++i )
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], m_strGenItem[i] );

	for( int i=0; i<SCROWGEN::MAX_QGEN_ITEM_ARRAY; ++i )
		STRUTIL::InputStrCsv( StrArray[ iCsvCur++ ], m_strQtGenItem[i] );
}