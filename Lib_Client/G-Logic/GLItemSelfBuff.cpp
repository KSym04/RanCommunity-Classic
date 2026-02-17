#include "stdafx.h"
#include "GLItemSelfBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace ITEM
{

	SELFBUFF::SELFBUFF ()
	{
		CLEAR ();
	}

	void SELFBUFF::CLEAR ()
	{
		bData = false;

		/*dmk14 box selection*/
		//m_bSelect = FALSE;
		//m_nMaxSelect = 0;

		for ( int i=0; i<ITEM_SIZE; ++i )
		{
			sBUFFS[i].sidSkill = SNATIVEID(false);
			sBUFFS[i].wLevel = 0;
		}
	}

	bool SELFBUFF::INSERT ( const SNATIVEID &sidSkill, DWORD wLevel )
	{
		if ( sidSkill==SNATIVEID(false) )	return false;

		for ( int i=0; i<ITEM_SIZE; ++i )
		{
			if ( sBUFFS[i].sidSkill==SNATIVEID(false) )
			{
				sBUFFS[i].sidSkill = sidSkill;
				sBUFFS[i].wLevel = wLevel;
				return true;
			}
		}

		//std::vector<DWORD>	vecTEMP;
		//for ( i=0; i<ITEM_SIZE; ++i )	vecTEMP.push_back ( sITEMS[i].dwID );
		//std::sort ( vecTEMP.begin(), vecTEMP.end() );

		//CLEAR ();
		//for ( i=0; i<ITEM_SIZE; ++i )	sITEMS[i].dwID = vecTEMP[i];

		return false;
	}

	bool SELFBUFF::DEL ( int nIndex )
	{
		if ( nIndex < 0 )				return false;
		if ( ITEM_SIZE <= nIndex )		return false;

		sBUFFS[nIndex].sidSkill = SNATIVEID(false);
		sBUFFS[nIndex].wLevel = 1;

		//std::vector<DWORD>	vecTEMP;
		//for ( int i=0; i<ITEM_SIZE; ++i )	vecTEMP.push_back ( sITEMS[i].dwID );
		//std::sort ( vecTEMP.begin(), vecTEMP.end() );

		//CLEAR ();
		//for ( i=0; i<ITEM_SIZE; ++i )	sITEMS[i].dwID = vecTEMP[i];

		return true;
	}

	bool SELFBUFF::VALID ()
	{
		for ( int i=0; i<ITEM_SIZE; ++i )
		{
			if ( sBUFFS[i].sidSkill!=SNATIVEID(false) )	return true;
		}

		return false;
	}

	BOOL SELFBUFF::LOAD ( basestream &SFile )
	{
		DWORD dwVER(0), dwSIZE(0);
		SFile >> dwVER;
		SFile >> dwSIZE;

		switch (dwVER)
		{
		case VERSION:
			{
				/*dm1k4 box selection*/
				//SFile >> m_bSelect;
				//SFile >> m_nMaxSelect;

				SFile.ReadBuffer ( sBUFFS, sizeof(SELFBUFFS_ITEM)*ITEM_SIZE );
				SFile >> bData;
			}break;

		case 0x103:
			{
				SFile.ReadBuffer ( sBUFFS, sizeof(SELFBUFFS_ITEM)*ITEM_SIZE );
				SFile >> bData;
			}break;

		case 0x102:
			{
				SFile.ReadBuffer ( sBUFFS, sizeof(SELFBUFFS_ITEM)*ITEM_SIZE );
			}break;

		case 0x101:
			{
				SELFBUFFS_ITEM_101 _sOLD_ITEMS[ITEM_SIZE];
				SFile.ReadBuffer ( _sOLD_ITEMS, sizeof(SELFBUFFS_ITEM_101)*ITEM_SIZE );

				for ( int i=0; i<ITEM_SIZE; ++i )
				{
					sBUFFS[i].sidSkill = _sOLD_ITEMS[i].sidSkill;
					sBUFFS[i].wLevel = _sOLD_ITEMS[i].wLevel;
				}
			}break;

		case 0x100:
			{
				SNATIVEID _sOLD_ITEMS[ITEM_SIZE];
				SFile.ReadBuffer ( _sOLD_ITEMS, sizeof(DWORD)*ITEM_SIZE );

				for ( int i=0; i<ITEM_SIZE; ++i )
				{
					sBUFFS[i].sidSkill = _sOLD_ITEMS[i];
					sBUFFS[i].wLevel = 1;
				}
			}break;

		default:
			{
				CDebugSet::ErrorVersion( "ITEM::SELFBUFF", dwVER );
				SFile.SetOffSet ( SFile.GetfTell()+dwSIZE );
			}break;
		};

		return TRUE;
	}

	BOOL SELFBUFF::SAVE ( CSerialFile &SFile )
	{
		SFile << DWORD(VERSION);
		SFile.BeginBlock();
		{
			/*dm1k4 box selection*/
			//SFile << m_bSelect;
			//SFile << m_nMaxSelect;

			SFile.WriteBuffer ( sBUFFS, sizeof(SELFBUFFS_ITEM)*ITEM_SIZE );
			SFile << bData;
		}
		SFile.EndBlock();

		return TRUE;
	}

	VOID SELFBUFF::SaveCsvHead ( std::fstream &SFile )
	{
		/*dmk14 box selection*/
		//SFile << "sBUFFS bSelectBox" << ",";
		//SFile << "sBUFFS nMaxSelect" << ",";

		for( int i=0; i<ITEM_SIZE; ++i )
		{
			SFile << "sBUFFS " << i << " sidSkill wMainID" << ",";
			SFile << "sBUFFS " << i << " sidSkill wSubID" << ",";
			SFile << "sBUFFS " << i << " wLevel" << ",";
		}

		SFile << "bData" << ",";
	}

	VOID SELFBUFF::SaveCsv ( std::fstream &SFile )
	{
		/*dmk14 box selection*/
		//SFile << m_bSelect << ",";
		//SFile << m_nMaxSelect << ",";

		for( int i=0; i<ITEM_SIZE; ++i )
		{
			SFile << sBUFFS[i].sidSkill.wMainID << ",";
			SFile << sBUFFS[i].sidSkill.wSubID << ",";
			SFile << sBUFFS[i].wLevel << ",";
		}

		SFile << bData << ",";
	}

	VOID SELFBUFF::LoadCsv ( CStringArray &StrArray, int &iCsvCur  )
	{
		/*dmk14 box selection*/
		//m_bSelect = (BOOL)atol( StrArray[ iCsvCur++ ] );
		//m_nMaxSelect = atoi( StrArray[ iCsvCur++ ] );

		for( int i=0; i<ITEM_SIZE; ++i )
		{
			sBUFFS[i].sidSkill.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sBUFFS[i].sidSkill.wSubID = (WORD)atoi( StrArray[ iCsvCur++ ] );
			sBUFFS[i].wLevel = (WORD)atol( StrArray[ iCsvCur++ ] );
		}

		bData = (bool)atoi( StrArray[ iCsvCur++ ] );
	}

};