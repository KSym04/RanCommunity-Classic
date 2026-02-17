#include "stdafx.h"
#include "./GLItem.h"
#include "./GLItemMan.h"
#include "./GLogicData.h"
#include "./GLStringTable.h"
#include "./GLSetOption.h"
#include "../Lib_Engine/Core/NSRParam.h"

#include "../../Lib_Engine/Common/StringUtils.h"
#include "../../Lib_Engine/DxCommon/Collision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////
//	$1.	2003-01-17 Txx:xx	[최초 릴리즈]
////////////////////////////////////////////////////////////////////////////////////


BOOL SITEM::SaveFile ( CSerialFile &SFile )
{
	SFile << DWORD(VERSION);

	SFile << (DWORD)FILE_SBASIC;
	{
		SFile << DWORD(ITEM::SITEMBASIC::VERSION);

		SFile.BeginBlock();
		{
			sBasicOp.SAVE ( SFile );
		}
		SFile.EndBlock();
	}

	SFile << (DWORD)FILE_SSUIT;
	{
		SFile << DWORD(ITEM::SSUIT::VERSION);
		SFile << (DWORD)sizeof(ITEM::SSUIT);
		SFile.WriteBuffer ( &sSuitOp, sizeof(ITEM::SSUIT) );
	}

	SFile << (DWORD)FILE_SDRUG;
	{
		SFile << DWORD(ITEM::SDRUG::VERSION);
		SFile << (DWORD)sizeof(ITEM::SDRUG);
		SFile.WriteBuffer ( &sDrugOp, sizeof(ITEM::SDRUG) );
	}

	SFile << (DWORD)FILE_SSKILLBOOK;
	{
		SFile << DWORD(ITEM::SSKILLBOOK::VERSION);
		SFile << (DWORD)sizeof(ITEM::SSKILLBOOK);
		SFile.WriteBuffer ( &sSkillBookOp, sizeof(ITEM::SSKILLBOOK) );
	}

	SFile << SITEM::FILE_SGENERATE;
	{
		SFile << DWORD(ITEM::SGENERATE::VERSION);
		SFile << (DWORD)sizeof(ITEM::SGENERATE);
		SFile.WriteBuffer ( &sGenerateOp, sizeof(ITEM::SGENERATE) );
	}

	SFile << SITEM::FILE_SGRINDING;
	{
		SFile << DWORD(ITEM::SGRINDING::VERSION);

		SFile.BeginBlock();
		{
			sGrindingOp.SAVE ( SFile );
		}
		SFile.EndBlock();
	}

	SFile << SITEM::FILE_BOX;
	{
		sBox.SAVE ( SFile );
	}

	SFile << SITEM::FILE_RANDOMBOX;
	{
		sRandomBox.SAVE ( SFile );
	}

	SFile << SITEM::FILE_QUESTIONITEM;
	{
		sQuestionItem.SAVE ( SFile );
	}

	SFile << SITEM::FILE_RANDOMITEM;
	{
		sRandomOpt.SAVE ( SFile );
	}

	SFile << SITEM::FILE_PET;	// PetData
	{
		sPet.SAVE( SFile );
	}

	SFile << SITEM::FILE_VEHICLE;
	{
		sVehicle.SAVE( SFile );
	}

	SFile << SITEM::FILE_PETSKINPACK;
	{
		sPetSkinPack.SAVE( SFile );
	}

	SFile << SITEM::FILE_ETCH_1;
	etc_1.SAVE( SFile );

	SFile << SITEM::FILE_ETCH_2;
	etc_2.SAVE( SFile );

	/*rv card, Juver, 2017/11/25 */
	SFile << SITEM::FILE_RVCARD;
	sRvCard.SAVE( SFile );


	SFile << (DWORD)FILE_END_DATA;

	return TRUE;
}

BOOL SITEM::LoadFile ( basestream &SFile, bool bPastLoad  )
{
	DWORD dwVersion;
	SFile >> dwVersion;

	if ( dwVersion > VERSION )
	{
		CDebugSet::ErrorVersion( "SITEM::LoadFile", dwVersion );
		return FALSE;
	}

	BOOL b104UP = ( dwVersion > 0x0104 );
	BOOL b104	= ( dwVersion == 0x0104 );

	DWORD suit_version(0);

	DWORD dwDataType;
	DWORD dwVer, dwSize;

	SFile >> dwDataType;
	while ( dwDataType!=SITEM::FILE_END_DATA )
	{
		switch ( dwDataType )
		{
		case SITEM::FILE_SBASIC:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if( bPastLoad )
				{
					if ( dwVer==0x0100 )
					{
						ITEM::SITEMBASIC_100 sOldData;
						GASSERT(sizeof(sOldData)==dwSize);
						SFile.ReadBuffer ( &sOldData, sizeof(sOldData) );

						sBasicOp.Assign ( sOldData );
					}
					else if ( dwVer==0x0101 )
					{
						ITEM::SITEMBASIC_101 sOldData;
						GASSERT(sizeof(sOldData)==dwSize);
						SFile.ReadBuffer ( &sOldData, sizeof(sOldData) );

						sBasicOp.Assign ( sOldData );
					}
					else if ( dwVer==0x0102 )
					{
						ITEM::SITEMBASIC_102 sOldData;
						GASSERT(sizeof(sOldData)==dwSize);
						SFile.ReadBuffer ( &sOldData, sizeof(sOldData) );

						sBasicOp.Assign ( sOldData );
					}
					else if ( dwVer==0x0103 )
					{
						sBasicOp.LOAD_103(SFile);
					}
					else if ( dwVer == 0x0104 )
					{
						sBasicOp.LOAD_104( SFile );
					}
					else if ( dwVer == 0x0105 )
					{
						sBasicOp.LOAD_105( SFile );
					}
					else if ( dwVer == 0x0106 )
					{
						sBasicOp.LOAD_106( SFile );
					}
					else if ( dwVer == 0x0107 )
					{
						sBasicOp.LOAD_107( SFile );
					}
					else if ( dwVer == 0x0108 )
					{
						sBasicOp.LOAD_108( SFile );
					}
					else if ( dwVer == 0x0109 )
					{
						sBasicOp.LOAD_109( SFile );
					}
					else if ( dwVer == 0x0110 )
					{
						sBasicOp.LOAD_110( SFile );
					}
					else if ( dwVer == 0x0111 )
					{
						sBasicOp.LOAD_111( SFile );
					}
					else if ( dwVer == 0x0112 )
					{
						sBasicOp.LOAD_112( SFile );
					}
					else if ( dwVer == 0x0113 )
					{
						sBasicOp.LOAD_113( SFile );
					}
					else if ( dwVer == 0x0114 )
					{
						sBasicOp.LOAD_114( SFile );
					}
					else if ( dwVer == 0x0115 )
					{
						sBasicOp.LOAD_115( SFile );
					}
					else if ( dwVer == 0x0116 )
					{
						sBasicOp.LOAD_116( SFile );
					}
					else if ( dwVer == 0x0117 )
					{
						sBasicOp.LOAD_117( SFile );
					}
					else if ( dwVer == 0x0118 )
					{
						sBasicOp.LOAD_118( SFile );
					}
					else if ( dwVer == 0x0119 )
					{
						sBasicOp.LOAD_119( SFile );
					}
					else if ( dwVer == 0x0121 )
					{
						sBasicOp.LOAD_121( SFile );
					}
					else if ( dwVer == 0x0122 )
					{
						sBasicOp.LOAD_122( SFile );
					}
#ifdef _DEBUG
					else if ( dwVer == 0x0123 )
					{
						sBasicOp.LOAD_123( SFile );
					}
					else if ( dwVer == 0x0124 )
					{
						sBasicOp.LOAD_124( SFile );
					}
					else if ( dwVer == 0x0125 )
					{
						sBasicOp.LOAD_125( SFile );
					}
					else if ( dwVer == 0x0126 )
					{
						sBasicOp.LOAD_126( SFile );
					}
					else if ( dwVer == 0x0127 )
					{
						sBasicOp.LOAD_127( SFile );
					}
					else if ( dwVer == 0x0128 )
					{
						sBasicOp.LOAD_128( SFile );
					}
					else if ( dwVer == 0x0129 )
					{
						sBasicOp.LOAD_129( SFile );
					}
#endif
					else if ( dwVer == 0x0200 )
					{
						sBasicOp.LOAD_200( SFile );
					}
					else if ( dwVer == 0x0201 )
					{
						sBasicOp.LOAD_201( SFile );
					}
					else if ( dwVer == 0x0202 )
					{
						sBasicOp.LOAD_202( SFile );
					}
					else if ( dwVer == 0x0203 )
					{
						sBasicOp.LOAD_203( SFile );
					}
					else if ( dwVer == 0x0204 )
					{
						sBasicOp.LOAD_204( SFile );
					}
					else if ( dwVer == 0x0205 )
					{
						sBasicOp.LOAD_205( SFile );
					}
					else if ( dwVer == 0x0206 )
					{
						sBasicOp.LOAD_206( SFile );
					}
					else if ( dwVer == 0x0207 )
					{
						sBasicOp.LOAD_207( SFile );
					}
					else if ( dwVer == 0x0208 )
					{
						sBasicOp.LOAD_208( SFile );
					}
					else if ( dwVer == 0x0209 )
					{
						sBasicOp.LOAD_209( SFile );
					}
					else if ( dwVer == 0x0210 )
					{
						sBasicOp.LOAD_210( SFile );
					}
					else if ( dwVer == 0x0211 )
					{
						sBasicOp.LOAD_211( SFile );
					}
					else if ( dwVer == 0x0212 )
					{
						sBasicOp.LOAD_212( SFile );
					}
					else if ( dwVer == 0x0213 )
					{
						sBasicOp.LOAD_213( SFile );
					}
					else if ( dwVer == 0x0214 )
					{
						sBasicOp.LOAD_214( SFile );
					}
					else if ( dwVer == 0x0215 )
					{
						sBasicOp.LOAD_215( SFile );
					}
					else if ( dwVer == 0x0216 )
					{
						sBasicOp.LOAD_216( SFile );
					}
					else if ( dwVer == 0x0217 )
					{
						sBasicOp.LOAD_217( SFile );
					}
					else if ( dwVer == 0x0218 )
					{
						sBasicOp.LOAD_218( SFile );
					}
					else if ( dwVer == 0x0219 )
					{
						sBasicOp.LOAD_219( SFile );
					}
					else if ( dwVer == 0x0220 )
					{
						sBasicOp.LOAD_220( SFile );
					}
					else if ( ITEM::SITEMBASIC::VERSION==dwVer )
					{
						sBasicOp.LOAD( SFile );
					}
					else
					{
						CDebugSet::ErrorVersion( "ITEM::SITEMBASIC PastLoad", dwVer );
						SFile.SetOffSet ( SFile.GetfTell()+dwSize );
					}
				}else{
					if ( ITEM::SITEMBASIC::VERSION==dwVer )
					{
						sBasicOp.LOAD( SFile );
					}
					else
					{
						CDebugSet::ErrorVersion( "ITEM::SITEMBASIC", dwVer );
						SFile.SetOffSet ( SFile.GetfTell()+dwSize );
						return FALSE;
					}
				}
			}break;

		case SITEM::FILE_SSUIT:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				suit_version = dwVer;

				if ( dwVer==0x0100 )
				{
					ITEM::SSUIT_100 sSuit100;
					GASSERT(sizeof(ITEM::SSUIT_100)==dwSize);
					SFile.ReadBuffer ( &sSuit100, sizeof(ITEM::SSUIT_100) );

					sSuitOp.Assign ( sSuit100 );
				}
				else if ( dwVer==0x0101 )
				{
					ITEM::SSUIT_101 sSuit101;
					GASSERT(sizeof(ITEM::SSUIT_101)==dwSize);
					SFile.ReadBuffer ( &sSuit101, sizeof(ITEM::SSUIT_101) );

					sSuitOp.Assign ( sSuit101 );
				}
				else if ( dwVer==0x0102 )
				{
					ITEM::SSUIT_102 sSuit102;
					GASSERT(sizeof(ITEM::SSUIT_102)==dwSize);
					SFile.ReadBuffer ( &sSuit102, sizeof(ITEM::SSUIT_102) );

					sSuitOp.Assign ( sSuit102 );
				}
				else if ( dwVer==0x0103 )
				{
					ITEM::SSUIT_103 sSuit103;
					GASSERT(sizeof(ITEM::SSUIT_103)==dwSize);
					SFile.ReadBuffer ( &sSuit103, sizeof(ITEM::SSUIT_103) );

					sSuitOp.Assign ( sSuit103 );
				}
				else if ( dwVer==0x0104 )
				{
					ITEM::SSUIT_104 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0105 )
				{
					ITEM::SSUIT_105 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0106 ) // By 경대
				{
					ITEM::SSUIT_106 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0107 )
				{
					ITEM::SSUIT_107 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0108 )
				{
					ITEM::SSUIT_108 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0109 )
				{
					ITEM::SSUIT_109 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0110 )
				{
					ITEM::SSUIT_110 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==0x0112 )
				{
					ITEM::SSUIT_112 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
#ifdef _DEBUG
				else if ( dwVer==0x0113 )
				{
					ITEM::SSUIT_113 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
#endif
				else if ( dwVer==0x0200 )
				{
					ITEM::SSUIT_200 sOldSuit;
					GASSERT(sizeof(sOldSuit)==dwSize);
					SFile.ReadBuffer ( &sOldSuit, sizeof(sOldSuit) );

					sSuitOp.Assign ( sOldSuit );
				}
				else if ( dwVer==ITEM::SSUIT::VERSION )
				{
					GASSERT(sizeof(ITEM::SSUIT)==dwSize);
					SFile.ReadBuffer ( &sSuitOp, sizeof(ITEM::SSUIT) );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SSUIT", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_SDRUG:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					ITEM::SDRUG_100 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sDrugOp.Assign ( sOLD );
				}
				else if ( dwVer==0x0101 )
				{
					ITEM::SDRUG_101 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sDrugOp.Assign ( sOLD );
				}
				else if ( dwVer==0x0102 )
				{
					ITEM::SDRUG_102 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sDrugOp.Assign ( sOLD );
				}
#ifdef _DEBUG
				else if ( dwVer==0x0103 )
				{
					ITEM::SDRUG_103 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sDrugOp.Assign ( sOLD );
				}
				else if ( dwVer==0x0104 )
				{
					ITEM::SDRUG_104 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sDrugOp.Assign ( sOLD );
				}
				else if ( dwVer==0x0105 )
				{
					ITEM::SDRUG_105 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sDrugOp.Assign ( sOLD );
				}
#endif
				else if ( dwVer==ITEM::SDRUG::VERSION )
				{
					GASSERT(sizeof(ITEM::SDRUG)==dwSize);
					SFile.ReadBuffer ( &sDrugOp, sizeof(ITEM::SDRUG) );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SDRUG", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_SSKILLBOOK:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					ITEM::SSKILLBOOK_100 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sSkillBookOp.Assign ( sOLD );
				}
				else if ( dwVer==0x0101 )
				{
					ITEM::SSKILLBOOK_101 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sSkillBookOp.Assign ( sOLD );
				}
				else if ( dwVer==ITEM::SSKILLBOOK::VERSION )
				{
					GASSERT(sizeof(ITEM::SSKILLBOOK)==dwSize);
					SFile.ReadBuffer ( &sSkillBookOp, sizeof(ITEM::SSKILLBOOK) );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SSKILLBOOK", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_SGENERATE:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if ( dwVer==0x0100 )
				{
					ITEM::SGENERATE_100 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sGenerateOp.Assign ( sOLD );
				}
				else if ( dwVer==0x0101 )
				{
					ITEM::SGENERATE_101 sOLD;
					GASSERT(sizeof(sOLD)==dwSize);
					SFile.ReadBuffer ( &sOLD, sizeof(sOLD) );

					sGenerateOp.Assign ( sOLD );
				}
				else if ( dwVer==ITEM::SGENERATE::VERSION )
				{
					GASSERT(sizeof(ITEM::SGENERATE)==dwSize);
					SFile.ReadBuffer ( &sGenerateOp, sizeof(ITEM::SGENERATE) );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SGENERATE", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_SGRINDING:
			{
				SFile >> dwVer;
				SFile >> dwSize;

				if (ITEM::SGRINDING::VERSION==dwVer )
				{
					sGrindingOp.LOAD( SFile );
				}
				else if ( dwVer==0x0105 )
				{
					sGrindingOp.LOAD_0105( SFile );
				}
				else if ( dwVer==0x0104 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_104)==dwSize);

					ITEM::SGRINDING_104 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else if ( dwVer==0x0103 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_103)==dwSize);

					ITEM::SGRINDING_103 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else if ( dwVer==0x0102 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_102)==dwSize);

					ITEM::SGRINDING_102 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else if ( dwVer==0x0101 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_101)==dwSize);

					ITEM::SGRINDING_101 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else if ( dwVer==0x0100 )
				{
					GASSERT(sizeof(ITEM::SGRINDING_100)==dwSize);

					ITEM::SGRINDING_100 sGrindingOp_Old;
					SFile.ReadBuffer ( &sGrindingOp_Old, sizeof(sGrindingOp_Old) );
					sGrindingOp.Assign ( sGrindingOp_Old );
				}
				else
				{
					CDebugSet::ErrorVersion( "ITEM::SGRINDING", dwVer );
					SFile.SetOffSet ( SFile.GetfTell()+dwSize );
				}
			}break;

		case SITEM::FILE_BOX:
			{
				sBox.LOAD ( SFile );
			}break;

		case SITEM::FILE_RANDOMBOX:
			{
				sRandomBox.LOAD ( SFile );
			}break;

		case SITEM::FILE_QUESTIONITEM:
			{
				sQuestionItem.LOAD ( SFile );
			}break;

		case SITEM::FILE_RANDOMITEM:
			{
				if ( b104 )
					sRandomOpt.LOAD_OLD ( SFile );
				else if ( b104UP )
					sRandomOpt.LOAD ( SFile );
			}break;

		case SITEM::FILE_PET:	// PetData
			{
				sPet.LOAD( SFile );
			}break;

		case SITEM::FILE_VEHICLE:
			{
				sVehicle.LOAD( SFile );			
			}break;

		case SITEM::FILE_PETSKINPACK:
			{
				sPetSkinPack.LOAD( SFile );			
			}break;

		case SITEM::FILE_ETCH_1:
			{
				etc_1.LOAD( SFile );			
			}break;

		case SITEM::FILE_ETCH_2:
			{
				etc_2.LOAD( SFile );			
			}break;

			/*rv card, Juver, 2017/11/25 */
		case SITEM::FILE_RVCARD:
			{
				sRvCard.LOAD( SFile );			
			}break;

		default:
			{
				CDebugSet::ErrorCode( "SITEM::LoadFile", "Unknown Data Type:%d", dwDataType );

				SFile >> dwVer;
				SFile >> dwSize;
				SFile.SetOffSet ( SFile.GetfTell()+dwSize );
			}
			break;
		};

		SFile >> dwDataType;
	};

	//fixup for able to color item 
	if ( suit_version >= 0x0112 && suit_version <= 0x0200 )
	{
		if ( sSuitOp.bItemColor )
		{
			if ( sBasicOp.emItemType == ITEM_SUIT )
			{
				sBasicOp.bItemColor = TRUE;
				sBasicOp.wItemColor1 = 27431;
				sBasicOp.wItemColor2 = 28987;
			}
			else if ( sBasicOp.emItemType == ITEM_VEHICLE )
			{
				sVehicle.bUseColor = TRUE;
				sVehicle.wDefaultColor1 = 27431;
				sVehicle.wDefaultColor2 = 28987;
			}
		}
	}

	return TRUE;
}

BOOL SITEM::isWrappable ()
{
	if ( sBasicOp.IsEVENT() )					return FALSE; //event item
	if ( sBasicOp.IsDISGUISE() )				return FALSE; //costume
	if ( IsTIMELMT() )							return FALSE; //expiry
	if ( sBasicOp.sNativeID.wMainID == 76 )		return FALSE; //gm item
	if ( sBasicOp.emItemType != ITEM_SUIT )		return FALSE; //wear item

	/*wrapper bypass, Juver, 2019/02/20 */
	//this bypasses the conditions below
	if ( sBasicOp.bWrapperBypass )				return TRUE;

	/* block wrapper, Juver, 2021/06/08 */
	//wrapper is blocked
	if ( sBasicOp.IsBlockWrapper() )			return FALSE; 

	if ( sBasicOp.dwReqActivityPoint != 0 )		return FALSE; //activity item
	if ( sBasicOp.dwReqContributionPoint != 0 )	return FALSE; //contribution item
	
	if ( sSuitOp.emSuit != SUIT_HEADGEAR && 
		sSuitOp.emSuit != SUIT_UPPER &&
		sSuitOp.emSuit != SUIT_LOWER &&
		sSuitOp.emSuit != SUIT_HAND &&
		sSuitOp.emSuit != SUIT_FOOT &&
		sSuitOp.emSuit != SUIT_HANDHELD &&
		sSuitOp.emSuit != SUIT_VEHICLE &&
		sSuitOp.emSuit != SUIT_BELT )		return FALSE;

	return TRUE;
}

bool SITEM::isWearable ()
{
	if( sBasicOp.emItemType == ITEM_SUIT || 
		sBasicOp.emItemType == ITEM_ARROW || 
		sBasicOp.emItemType == ITEM_CHARM || 
		sBasicOp.emItemType == ITEM_BULLET || 
		sBasicOp.emItemType == ITEM_ANTI_DISAPPEAR || 
		sBasicOp.emItemType == ITEM_REVIVE || 
		sBasicOp.emItemType == ITEM_VEHICLE )
	{
		return true;
	}

	return false;
}

/* item pile checker, Juver, 2020/10/02 */
bool SITEM::IsOverPile( WORD wTurnNum )
{
	//item pile num is more than maximum set
	if ( ISPILE() && wTurnNum > sDrugOp.wPileNum )
	{
		return true;
	}
	
	//non pile items should never have pile num of more than 1
	if ( !ISPILE() && wTurnNum > 1 )
	{
		return true;
	}
	
	return false;
}

VOID SITEM::SaveCsvHead ( std::fstream &SFile )
{
	ITEM::SITEMBASIC::SaveCsvHead( SFile );
	ITEM::SSUIT::SaveCsvHead( SFile );
	ITEM::SDRUG::SaveCsvHead( SFile );
	ITEM::SSKILLBOOK::SaveCsvHead( SFile );
	ITEM::SGRINDING::SaveCsvHead( SFile );
	ITEM::SGENERATE::SaveCsvHead( SFile );
	ITEM::SBOX::SaveCsvHead( SFile );
	ITEM::SRANDOMBOX::SaveCsvHead( SFile );
	ITEM::SQUESTIONITEM::SaveCsvHead( SFile );
	ITEM::SRANDOM_OPT::SaveCsvHead( SFile );
	ITEM::SPET::SaveCsvHead( SFile );	// PetData
	ITEM::SVEHICLE::SaveCsvHead( SFile );
	ITEM::SPETSKINPACKITEM::SaveCsvHead( SFile );
	ITEM::ETC_1::SaveCsvHead( SFile );
	ITEM::ETC_2::SaveCsvHead( SFile );
	ITEM::SRVCARD::SaveCsvHead( SFile ); /*rv card, Juver, 2017/11/25 */

	SFile << std::endl;
}

VOID SITEM::SaveCsv ( std::fstream &SFile )
{
	sBasicOp.SaveCsv( SFile );
	sSuitOp.SaveCsv( SFile );
	sDrugOp.SaveCsv( SFile );
	sSkillBookOp.SaveCsv( SFile );
	sGrindingOp.SaveCsv( SFile );
	sGenerateOp.SaveCsv( SFile );
	sBox.SaveCsv( SFile );
	sRandomBox.SaveCsv( SFile );
	sQuestionItem.SaveCsv( SFile );
	sRandomOpt.SaveCsv( SFile );
	sPet.SaveCsv( SFile );				// PetData
	sVehicle.SaveCsv( SFile );
	sPetSkinPack.SaveCsv( SFile );
	etc_1.SaveCsv( SFile );
	etc_2.SaveCsv( SFile );
	sRvCard.SaveCsv( SFile ); /*rv card, Juver, 2017/11/25 */

	SFile << std::endl;
}

VOID SITEM::LoadCsv ( CStringArray &StrArray )
{
	int iCsvCur = 0;

	sBasicOp.LoadCsv( StrArray, iCsvCur );
	sSuitOp.LoadCsv( StrArray, iCsvCur );
	sDrugOp.LoadCsv( StrArray, iCsvCur );
	sSkillBookOp.LoadCsv( StrArray, iCsvCur );
	sGrindingOp.LoadCsv( StrArray, iCsvCur );
	sGenerateOp.LoadCsv( StrArray, iCsvCur );
	sBox.LoadCsv( StrArray, iCsvCur );
	sRandomBox.LoadCsv( StrArray, iCsvCur );
	sQuestionItem.LoadCsv( StrArray, iCsvCur );
	sRandomOpt.LoadCsv( StrArray, iCsvCur );
	sPet.LoadCsv( StrArray, iCsvCur );	// PetData
	sVehicle.LoadCsv( StrArray, iCsvCur );
	sPetSkinPack.LoadCsv( StrArray, iCsvCur );
	etc_1.LoadCsv( StrArray, iCsvCur );
	etc_2.LoadCsv( StrArray, iCsvCur );
	sRvCard.LoadCsv( StrArray, iCsvCur ); /*rv card, Juver, 2017/11/25 */
}

WORD SITEM::GETAPPLYNUM () const
{
	switch ( sBasicOp.emItemType )
	{
	case ITEM_ARROW:
	case ITEM_CHARM:
	case ITEM_BULLET: /*gun-bullet logic, Juver, 2017/05/27 */

	case ITEM_FIRECRACKER:
	case ITEM_LOUDSPEAKER:
	case ITEM_TICKET:
	case ITEM_RECALL:
	case ITEM_STORAGE_CONNECT:
	case ITEM_DISJUNCTION:
	case ITEM_2FRIEND:
	case ITEM_REVIVE: // 귀혼주일 경우
	case ITEM_ANTI_DISAPPEAR:
	case ITEM_REMODEL:
	case ITEM_GARBAGE_CARD:
	case ITEM_TELEPORT_CARD:
		return sDrugOp.wCureVolume;
	};

	//	귀혼주일 경우.
	if ( sDrugOp.emDrug == ITEM_DRUG_CALL_REVIVE && sBasicOp.emItemType==ITEM_SUIT )
	{
		return sDrugOp.wCureVolume;
	}

	return 1;
}

SITEM& SITEM::operator = ( const SITEM& rvalue )
{
	if( this == &rvalue )
		return *this;

	sBasicOp		= rvalue.sBasicOp;
	sSuitOp			= rvalue.sSuitOp;
	sDrugOp			= rvalue.sDrugOp;
	sSkillBookOp	= rvalue.sSkillBookOp;
	sGrindingOp		= rvalue.sGrindingOp;
	sGenerateOp		= rvalue.sGenerateOp;
	sBox			= rvalue.sBox;
	sRandomBox		= rvalue.sRandomBox;
	sQuestionItem	= rvalue.sQuestionItem;
	sRandomOpt		= rvalue.sRandomOpt;
	sPet			= rvalue.sPet;
	sVehicle		= rvalue.sVehicle;
	sPetSkinPack	= rvalue.sPetSkinPack;
	etc_1			= rvalue.etc_1;
	etc_2			= rvalue.etc_2;

	sRvCard			= rvalue.sRvCard; /*rv card, Juver, 2017/11/25 */

	return *this;
}

const char* SITEM::GetName()
{
	const char* szName = GLStringTable::GetInstance().GetString( sBasicOp.strName.c_str(), GLStringTable::ITEM );
	if ( !szName )		return sBasicOp.strName.c_str();

	return szName;
}

const char* SITEM::GetComment()
{
	const char* szComment = GLStringTable::GetInstance().GetString( sBasicOp.strComment.c_str(), GLStringTable::ITEM );
	if ( !szComment )	return NULL;

	return szComment;
}

namespace
{
	BYTE CAST2GRADE ( int nVALUE, int nSTEP )
	{
		BYTE cGRADE(0);
		if ( nVALUE > 0 )	cGRADE += 1;
		cGRADE += BYTE ( (nVALUE-1)/nSTEP );

		if ( cGRADE>9 )		cGRADE = 9;
		return cGRADE;
	}

	BYTE CAST2GRADE_EX ( int nVALUE, int nSTEP1, int nSTEP2 )
	{
		nVALUE = nVALUE * nSTEP1;

		BYTE cGRADE(0);
		if ( nVALUE > 0 )	cGRADE += 1;
		cGRADE += BYTE ( (nVALUE-1)/nSTEP2 );

		if ( cGRADE>9 )		cGRADE = 9;
		return cGRADE;
	}
};

SITEMCUSTOM::SITEMCUSTOM ()
	: sNativeID(false)
	, nidDISGUISE(false)
	, tBORNTIME(0)
	, tDISGUISE(0)
	, lnGenNum(0)
	, wTurnNum(1)
	, cGenType(EMGEN_DEFAULT)
	, cChnID(0)
	
	, cFieldID(0)
	
	, cDAMAGE(0)
	, cDEFENSE(0)
	, cRESIST_FIRE(0)
	
	, cRESIST_ICE(0)
	, cRESIST_ELEC(0)
	, cRESIST_POISON(0)
	, cRESIST_SPIRIT(0)
	
	, cOptTYPE1(0)
	, cOptTYPE2(0)
	, cOptTYPE3(0)
	, cOptTYPE4(0)
	
	, nOptVALUE1(0)
	, nOptVALUE2(0)
	, nOptVALUE3(0)
	, nOptVALUE4(0)

	, dwPetID(0)
	, dwVehicleID( 0 )
	, bVietnamGainItem( false )

	/*item color, Juver, 2018/01/04 */
	, wColor1(ITEMCOLOR_WHITE)
	, wColor2(ITEMCOLOR_WHITE)

	/* Item Reform use limit, Juver, 2021/07/22 */
	, wReformUseCount(0)

	/* set item option, Juver, 2021/07/30 */
	, wSetItemID(SET_OPTION_NULL)
{
}

SITEMCUSTOM::SITEMCUSTOM ( SNATIVEID	sNID )
	: sNativeID(sNID)
	, nidDISGUISE(false)
	, tBORNTIME(0)
	, tDISGUISE(0)
	, lnGenNum(0)
	, wTurnNum(1)
	
	, cGenType(EMGEN_DEFAULT)
	, cChnID(0)
	
	, cFieldID(0)
	
	, cDAMAGE(0)
	, cDEFENSE(0)
	, cRESIST_FIRE(0)
	
	, cRESIST_ICE(0)
	, cRESIST_ELEC(0)
	, cRESIST_POISON(0)
	, cRESIST_SPIRIT(0)
	
	, cOptTYPE1(0)
	, cOptTYPE2(0)
	, cOptTYPE3(0)
	, cOptTYPE4(0)
	
	, nOptVALUE1(0)
	, nOptVALUE2(0)
	, nOptVALUE3(0)
	, nOptVALUE4(0)

	, dwPetID(0)
	, dwVehicleID( 0 )
	, bVietnamGainItem ( 0 )

	/*item color, Juver, 2018/01/04 */
	, wColor1(ITEMCOLOR_WHITE)
	, wColor2(ITEMCOLOR_WHITE)

	/* Item Reform use limit, Juver, 2021/07/22 */
	, wReformUseCount(0)

	/* set item option, Juver, 2021/07/30 */
	, wSetItemID(SET_OPTION_NULL)
{
}

void SITEMCUSTOM::ResetGrind ( EMGRINDING_TYPE emTYPE )
{
	switch ( emTYPE )
	{
	case EMGRINDING_NONE:
		cDAMAGE = (0);
		cDEFENSE = (0);

		cRESIST_FIRE = (0);
		cRESIST_ICE = (0);
		cRESIST_ELEC = (0);
		cRESIST_POISON = (0);
		cRESIST_SPIRIT = (0);
		break;

	case EMGRINDING_DAMAGE:
		cDAMAGE = (0);
		break;
	case EMGRINDING_DEFENSE:
		cDEFENSE = (0);
		break;

	case EMGRINDING_RESIST_FIRE:
		cRESIST_FIRE = (0);
		break;
	case EMGRINDING_RESIST_ICE:
		cRESIST_ICE = (0);
		break;
	case EMGRINDING_RESIST_ELEC:
		cRESIST_ELEC = (0);
		break;
	case EMGRINDING_RESIST_POISON:
		cRESIST_POISON = (0);
		break;
	case EMGRINDING_RESIST_SPIRIT:
		cRESIST_SPIRIT = (0);
		break;
	};
}

bool SITEMCUSTOM::operator == ( const SITEMCUSTOM &value )
{
	return !memcmp(this,&value,sizeof(SITEMCUSTOM));
}

bool SITEMCUSTOM::operator != ( const SITEMCUSTOM &value )
{
	return 0!=memcmp(this,&value,sizeof(SITEMCUSTOM));
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_100 &sOld )
{
	sNativeID = sOld.sNativeID;

	cGenType = EMGEN_OLD;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_101 &sOld )
{
	sNativeID = sOld.sNativeID;

	cGenType = EMGEN_OLD;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE ( sOld.sResist.nSpirit, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_102 &sOld )
{
	sNativeID = sOld.sNativeID;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.dwGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE ( sOld.sResist.nSpirit, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_103 &sOld )
{
	sNativeID = sOld.sNativeID;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE ( sOld.sResist.nSpirit, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_104 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE ( sOld.nDamage, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE ( sOld.nDefense, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE ( sOld.sResist.nFire, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE ( sOld.sResist.nIce, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE ( sOld.sResist.nElectric, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE ( sOld.sResist.nPoison, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE ( sOld.sResist.nSpirit, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_105 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = CAST2GRADE_EX ( sOld.cDAMAGE, 3, GLCONST_CHAR::wDAMAGE_GRADE );
	cDEFENSE = CAST2GRADE_EX ( sOld.cDEFENSE, 3, GLCONST_CHAR::wDEFENSE_GRADE );

	cRESIST_FIRE = CAST2GRADE_EX ( sOld.cRESIST_FIRE, 3, GLCONST_CHAR::wRESIST_FIRE_GRADE );
	cRESIST_ICE = CAST2GRADE_EX ( sOld.cRESIST_ICE, 3, GLCONST_CHAR::wRESIST_ICE_GRADE );
	cRESIST_ELEC = CAST2GRADE_EX ( sOld.cRESIST_ELEC, 3, GLCONST_CHAR::wRESIST_ELEC_GRADE );
	cRESIST_POISON = CAST2GRADE_EX ( sOld.cRESIST_POISON, 3, GLCONST_CHAR::wRESIST_POISON_GRADE );
	cRESIST_SPIRIT = CAST2GRADE_EX ( sOld.cRESIST_SPIRIT, 3, GLCONST_CHAR::wRESIST_SPIRIT_GRADE );

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_106 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;
	tDISGUISE = 0;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = sOld.cDAMAGE;
	cDEFENSE = sOld.cDEFENSE;

	cRESIST_FIRE = sOld.cRESIST_FIRE;
	cRESIST_ICE = sOld.cRESIST_ICE;
	cRESIST_ELEC = sOld.cRESIST_ELEC;
	cRESIST_POISON = sOld.cRESIST_POISON;
	cRESIST_SPIRIT = sOld.cRESIST_SPIRIT;

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_107 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;
	tDISGUISE = sOld.tDISGUISE;

	cGenType = sOld.cGenType;
	cFieldID = sOld.cFieldID;
	cChnID = 0;
	lnGenNum = sOld.lnGenNum;

	wTurnNum = sOld.wTurnNum;

	cDAMAGE = sOld.cDAMAGE;
	cDEFENSE = sOld.cDEFENSE;

	cRESIST_FIRE = sOld.cRESIST_FIRE;
	cRESIST_ICE = sOld.cRESIST_ICE;
	cRESIST_ELEC = sOld.cRESIST_ELEC;
	cRESIST_POISON = sOld.cRESIST_POISON;
	cRESIST_SPIRIT = sOld.cRESIST_SPIRIT;

	if ( cRESIST_FIRE==9 && cRESIST_ICE==9 && cRESIST_ELEC==9 && cRESIST_POISON==9 && cRESIST_SPIRIT==9 )
	{
		SITEM *pITEM = GLItemMan::GetInstance().GetItem ( sNativeID );

		bool bRESET_GRINDING = pITEM && pITEM->sSuitOp.wReModelNum==0;
		if ( !bRESET_GRINDING )		bRESET_GRINDING = ( pITEM && ( pITEM->sSuitOp.gdDamage != GLPADATA(0,0) ) );
		if ( bRESET_GRINDING )
		{
			ResetGrind(EMGRINDING_RESIST_FIRE);
			ResetGrind(EMGRINDING_RESIST_ICE);
			ResetGrind(EMGRINDING_RESIST_ELEC);
			ResetGrind(EMGRINDING_RESIST_POISON);
			ResetGrind(EMGRINDING_RESIST_SPIRIT);
		}
	}

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_108 &sOld )
{
	sNativeID = sOld.sNativeID;
	nidDISGUISE = sOld.nidDISGUISE;
	tBORNTIME = sOld.tBORNTIME;
	tDISGUISE = sOld.tDISGUISE;

	cGenType = sOld.cGenType;
	cChnID = sOld.cChnID;
	cFieldID = sOld.cFieldID;
	lnGenNum = sOld.lnGenNum;
	wTurnNum = sOld.wTurnNum;

	cDAMAGE = sOld.cDAMAGE;
	cDEFENSE = sOld.cDEFENSE;

	cRESIST_FIRE = sOld.cRESIST_FIRE;
	cRESIST_ICE = sOld.cRESIST_ICE;
	cRESIST_ELEC = sOld.cRESIST_ELEC;
	cRESIST_POISON = sOld.cRESIST_POISON;
	cRESIST_SPIRIT = sOld.cRESIST_SPIRIT;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_109 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_110 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_111 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_QUEST_115 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	bVietnamGainItem	= sOld.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1				= (WORD)sOld.dwColor1;
	wColor2				= (WORD)sOld.dwColor2;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_QUEST_116 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	bVietnamGainItem	= sOld.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1				= (WORD)sOld.dwColor1;
	wColor2				= (WORD)sOld.dwColor2;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_QUEST_117 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	bVietnamGainItem	= sOld.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1				= (WORD)sOld.dwColor1;
	wColor2				= (WORD)sOld.dwColor2;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_112 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	bVietnamGainItem	= sOld.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1 = ITEMCOLOR_WHITE;
	wColor2 = ITEMCOLOR_WHITE;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_200 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	bVietnamGainItem	= sOld.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1				= sOld.wColor1;
	wColor2				= sOld.wColor2;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= 0;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEMCUSTOM_201 &sOld )
{
	sNativeID			= sOld.sNativeID;	
	nidDISGUISE			= sOld.nidDISGUISE;
	tBORNTIME			= sOld.tBORNTIME;
	tDISGUISE			= sOld.tDISGUISE;

	lnGenNum			= sOld.lnGenNum;
	wTurnNum			= sOld.wTurnNum;
	cGenType			= sOld.cGenType;	
	cChnID				= sOld.cChnID;		

	cFieldID			= sOld.cFieldID;

	cDAMAGE				= sOld.cDAMAGE;	
	cDEFENSE			= sOld.cDEFENSE;	
	cRESIST_FIRE		= sOld.cRESIST_FIRE;
	cRESIST_ICE			= sOld.cRESIST_ICE;
	cRESIST_ELEC		= sOld.cRESIST_ELEC;
	cRESIST_POISON		= sOld.cRESIST_POISON;
	cRESIST_SPIRIT		= sOld.cRESIST_SPIRIT;

	cOptTYPE1			= sOld.cOptTYPE1;
	cOptTYPE2			= sOld.cOptTYPE2;
	cOptTYPE3			= sOld.cOptTYPE3;
	cOptTYPE4			= sOld.cOptTYPE4;

	nOptVALUE1			= sOld.nOptVALUE1;
	nOptVALUE2			= sOld.nOptVALUE2;
	nOptVALUE3			= sOld.nOptVALUE3;
	nOptVALUE4			= sOld.nOptVALUE4;
	dwPetID				= sOld.dwPetID;
	dwVehicleID			= sOld.dwVehicleID;

	bVietnamGainItem	= sOld.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1				= sOld.wColor1;
	wColor2				= sOld.wColor2;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= sOld.wReformUseCount;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID = 0;
}

void SITEMCUSTOM::Assign ( const SITEM_LOBY &sOldLoby )
{
	sNativeID			= sOldLoby.sNativeID;	
	nidDISGUISE			= sOldLoby.nidDISGUISE;

	lnGenNum			= sOldLoby.lnGenNum;
	wTurnNum			= sOldLoby.wTurnNum;
	cGenType			= sOldLoby.cGenType;	
	cChnID				= sOldLoby.cChnID;		

	cFieldID			= sOldLoby.cFieldID;

	cDAMAGE				= sOldLoby.cDAMAGE;	
	cDEFENSE			= sOldLoby.cDEFENSE;	
	cRESIST_FIRE		= sOldLoby.cRESIST_FIRE;
	cRESIST_ICE			= sOldLoby.cRESIST_ICE;
	cRESIST_ELEC		= sOldLoby.cRESIST_ELEC;
	cRESIST_POISON		= sOldLoby.cRESIST_POISON;
	cRESIST_SPIRIT		= sOldLoby.cRESIST_SPIRIT;


    cOptTYPE1			= sOldLoby.cOptTYPE1;
	cOptTYPE2			= sOldLoby.cOptTYPE2;
	cOptTYPE3			= sOldLoby.cOptTYPE3;
	cOptTYPE4			= sOldLoby.cOptTYPE4;

	nOptVALUE1			= sOldLoby.nOptVALUE1;
	nOptVALUE2			= sOldLoby.nOptVALUE2;
	nOptVALUE3			= sOldLoby.nOptVALUE3;
	nOptVALUE4			= sOldLoby.nOptVALUE4;

	/*item color, Juver, 2018/01/04 */
	wColor1				= sOldLoby.wColor1;
	wColor2				= sOldLoby.wColor2;
}

void SITEMCUSTOM::Assign ( const SITEMCLIENT &sClient )
{
	nidDISGUISE = sClient.nidDISGUISE;
	sNativeID = sClient.sNativeID;
	wTurnNum = sClient.wTurnNum;

	/*additional RV, Juver, 2018/06/15 */
	cOptTYPE1 = sClient.cOptTYPE01;
	cOptTYPE2 = sClient.cOptTYPE02;
	cOptTYPE3 = sClient.cOptTYPE03;
	cOptTYPE4 = sClient.cOptTYPE04;
	nOptVALUE1 = sClient.nOptVALUE01;
	nOptVALUE2 = sClient.nOptVALUE02;
	nOptVALUE3 = sClient.nOptVALUE03;
	nOptVALUE4 = sClient.nOptVALUE04;

	/*item color, Juver, 2018/01/04 */
	wColor1				= sClient.wColor1;
	wColor2				= sClient.wColor2;
}

/*item link, Juver, 2017/07/31 */
void SITEMCUSTOM::Assign ( const SITEMLINK &sItemLink )
{
	sNativeID			= sItemLink.sNativeID;	
	nidDISGUISE			= sItemLink.nidDISGUISE;
	tBORNTIME			= sItemLink.tBORNTIME;
	tDISGUISE			= sItemLink.tDISGUISE;

	lnGenNum			= sItemLink.lnGenNum;
	wTurnNum			= sItemLink.wTurnNum;
	cGenType			= sItemLink.cGenType;	
	cChnID				= sItemLink.cChnID;		

	cFieldID			= sItemLink.cFieldID;

	cDAMAGE				= sItemLink.cDAMAGE;	
	cDEFENSE			= sItemLink.cDEFENSE;	
	cRESIST_FIRE		= sItemLink.cRESIST_FIRE;
	cRESIST_ICE			= sItemLink.cRESIST_ICE;
	cRESIST_ELEC		= sItemLink.cRESIST_ELEC;
	cRESIST_POISON		= sItemLink.cRESIST_POISON;
	cRESIST_SPIRIT		= sItemLink.cRESIST_SPIRIT;

	cOptTYPE1			= sItemLink.cOptTYPE1;
	cOptTYPE2			= sItemLink.cOptTYPE2;
	cOptTYPE3			= sItemLink.cOptTYPE3;
	cOptTYPE4			= sItemLink.cOptTYPE4;

	nOptVALUE1			= sItemLink.nOptVALUE1;
	nOptVALUE2			= sItemLink.nOptVALUE2;
	nOptVALUE3			= sItemLink.nOptVALUE3;
	nOptVALUE4			= sItemLink.nOptVALUE4;
	dwPetID				= sItemLink.dwPetID;
	dwVehicleID			= sItemLink.dwVehicleID;
	bVietnamGainItem	= sItemLink.bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	wColor1				= sItemLink.wColor1;
	wColor2				= sItemLink.wColor2;

	/* Item Reform use limit, Juver, 2021/07/22 */
	wReformUseCount		= sItemLink.wReformUseCount;

	/* set item option, Juver, 2021/07/30 */
	wSetItemID			= sItemLink.wSetItemID;
}

/*item link, Juver, 2017/07/31 */
void SITEMCUSTOM::Convert ( SITEMLINK &sItemLink )
{
	sItemLink.sNativeID			= sNativeID;	
	sItemLink.nidDISGUISE		= nidDISGUISE;
	sItemLink.tBORNTIME			= tBORNTIME;
	sItemLink.tDISGUISE			= tDISGUISE;

	sItemLink.lnGenNum			= lnGenNum;
	sItemLink.wTurnNum			= wTurnNum;
	sItemLink.cGenType			= cGenType;	
	sItemLink.cChnID			= cChnID;		

	sItemLink.cFieldID			= cFieldID;

	sItemLink.cDAMAGE			= cDAMAGE;	
	sItemLink.cDEFENSE			= cDEFENSE;	
	sItemLink.cRESIST_FIRE		= cRESIST_FIRE;
	sItemLink.cRESIST_ICE		= cRESIST_ICE;
	sItemLink.cRESIST_ELEC		= cRESIST_ELEC;
	sItemLink.cRESIST_POISON	= cRESIST_POISON;
	sItemLink.cRESIST_SPIRIT	= cRESIST_SPIRIT;

	sItemLink.cOptTYPE1	= cOptTYPE1;
	sItemLink.cOptTYPE2	= cOptTYPE2;
	sItemLink.cOptTYPE3	= cOptTYPE3;
	sItemLink.cOptTYPE4	= cOptTYPE4;

	sItemLink.nOptVALUE1	= nOptVALUE1;
	sItemLink.nOptVALUE2	= nOptVALUE2;
	sItemLink.nOptVALUE3	= nOptVALUE3;
	sItemLink.nOptVALUE4	= nOptVALUE4;
	sItemLink.dwPetID		= dwPetID;
	sItemLink.dwVehicleID	= dwVehicleID;
	sItemLink.bVietnamGainItem	= bVietnamGainItem;

	/*item color, Juver, 2018/01/04 */
	sItemLink.wColor1	= wColor1;
	sItemLink.wColor2	= wColor2;

	/* Item Reform use limit, Juver, 2021/07/22 */
	sItemLink.wReformUseCount	= wReformUseCount;

	/* set item option, Juver, 2021/07/30 */
	sItemLink.wSetItemID		= wSetItemID;
}


// *****************************************************
// Desc: 인첸트 데미지
// *****************************************************
WORD SITEMCUSTOM::GETGRADE_DAMAGE () const
{
	BYTE cGRADE = grade_damage();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX;

    if ( cGRADE > GRADE_HIGH )
	{
		WORD wDAMAGE = getdamage().wHigh; // 물리 데미지
		WORD wMaDAMAGE = getmadamage();   // 기력 데미지
		WORD wDAMAGE_RATE = max (wDAMAGE,wMaDAMAGE);
		
		BYTE cOVER = cGRADE - GRADE_HIGH - 1;
		BYTE cOVER_N = GRADE_HIGH - GRADE_NORMAL;
		
		//	"비율"
		//		*, GRADE_NORMAL 등급 이상은 비율로 증가.
		wDAMAGE_RATE = (WORD) ( wDAMAGE_RATE * GLCONST_CHAR::fDAMAGE_GRADE * cOVER_N * 0.01f );
		//		*, GRADE_NORMAL 등급 이하는 고정비율로 증가.
		wDAMAGE_RATE += GLCONST_CHAR::wDAMAGE_GRADE * GRADE_NORMAL;
		//		*, GRADE_HIGH 등급 이상은 등급별 비율로 증가
		wDAMAGE_RATE = (WORD)( wDAMAGE_RATE * GLCONST_CHAR::fDAMAGE_GRADE_TOP[cOVER] );

		//	"고정"
		WORD wDAMAGE_VALUE = GLCONST_CHAR::wDAMAGE_GRADE * GRADE_HIGH;
		//	"고정"
		wDAMAGE_VALUE = wDAMAGE_VALUE * GLCONST_CHAR::fDAMAGE_GRADE_TOP[cOVER];

		//	"비율", "고정" 중 큰것을 선택.
		return max ( wDAMAGE_RATE, wDAMAGE_VALUE );


	}
	else if ( cGRADE > GRADE_NORMAL )
	{
		//	"연마 공격력" 산출시 물리데미지와 기력치 증가율 두가지 중 큰것을 선택.
		WORD wDAMAGE = getdamage().wHigh; // 물리 데미지
		WORD wMaDAMAGE = getmadamage();   // 기력 데미지
		WORD wDAMAGE_RATE = max (wDAMAGE,wMaDAMAGE);

		BYTE cOVER = cGRADE-GRADE_NORMAL;

		//	"비율"
		//		*, GRADE_NORMAL 등급 이상은 비율로 증가.
		wDAMAGE_RATE = (WORD) ( wDAMAGE_RATE * GLCONST_CHAR::fDAMAGE_GRADE * cOVER * 0.01f );
		//		*, GRADE_NORMAL 등급 이하는 고정비율로 증가.
		wDAMAGE_RATE += GLCONST_CHAR::wDAMAGE_GRADE * GRADE_NORMAL;

		//	"고정"
		WORD wDAMAGE_VALUE = GLCONST_CHAR::wDAMAGE_GRADE * cGRADE;

		//	"비율", "고정" 중 큰것을 선택.
		return max ( wDAMAGE_RATE, wDAMAGE_VALUE );
	}

	//	"고정"
	WORD wDAMAGE_VALUE = GLCONST_CHAR::wDAMAGE_GRADE * cGRADE;
	return wDAMAGE_VALUE;
}

WORD SITEMCUSTOM::GETGRADE_DEFENSE () const
{
	BYTE cGRADE = grade_defense();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX;

	//	방어력 비율 값.
	if ( cGRADE > GRADE_HIGH )
	{
		//	"연마 방어력" 산출.
		WORD wDEFENSE_RATE = getdefense();


		BYTE cOVER = cGRADE - GRADE_HIGH - 1;
		BYTE cOVER_N = GRADE_HIGH - GRADE_NORMAL;

		//	"비율"
		//		*, GRADE_NORMAL 등급 이상은 비율로 증가.
		wDEFENSE_RATE = (WORD) ( wDEFENSE_RATE*GLCONST_CHAR::fDEFENSE_GRADE * cOVER_N * 0.01f );
		//		*, GRADE_NORMAL 등급 이하는 고정비율로 증가.
		wDEFENSE_RATE += GLCONST_CHAR::wDEFENSE_GRADE*GRADE_NORMAL;
		//		*, GRADE_HIGH 등급 이상은 등급별 비율로 증가
		wDEFENSE_RATE = (WORD) ( wDEFENSE_RATE * GLCONST_CHAR::fDEFENSE_GRADE_TOP[cOVER] );

		//	"고정"
		WORD wDEFENSE_VALUE = GLCONST_CHAR::wDEFENSE_GRADE * GRADE_HIGH;
		
		//	"고정"
		wDEFENSE_VALUE = wDEFENSE_VALUE * GLCONST_CHAR::fDEFENSE_GRADE_TOP[cOVER];	

		//	"비율", "고정" 중 큰것을 선택.
		return max ( wDEFENSE_RATE, wDEFENSE_VALUE );


	}
	else if ( cGRADE > GRADE_NORMAL )
	{
		//	"연마 방어력" 산출.
		WORD wDEFENSE_RATE = getdefense();


		BYTE cOVER = cGRADE-GRADE_NORMAL;

		//	"비율"
		//		*, GRADE_NORMAL 등급 이상은 비율로 증가.
		wDEFENSE_RATE = (WORD) ( wDEFENSE_RATE*GLCONST_CHAR::fDEFENSE_GRADE * cOVER * 0.01f );
		//		*, GRADE_NORMAL 등급 이하는 고정비율로 증가.
		wDEFENSE_RATE += GLCONST_CHAR::wDEFENSE_GRADE*GRADE_NORMAL;

		//	"고정"
		WORD wDEFENSE_VALUE = GLCONST_CHAR::wDEFENSE_GRADE * cGRADE;

		//	"비율", "고정" 중 큰것을 선택.
		return max ( wDEFENSE_RATE, wDEFENSE_VALUE );
	}

	//	"고정"
	WORD wDEFENSE_VALUE = GLCONST_CHAR::wDEFENSE_GRADE * cGRADE;
	return wDEFENSE_VALUE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_FIRE () const
{
	BYTE cGRADE = grade_fire();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_FIRE_GRADE * cGRADE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_ICE () const
{
	BYTE cGRADE = grade_ice();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_ICE_GRADE * cGRADE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_ELEC () const
{
	BYTE cGRADE = grade_elec();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_ELEC_GRADE * cGRADE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_POISON () const
{
	BYTE cGRADE = grade_poison();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_POISON_GRADE * cGRADE;
}

WORD SITEMCUSTOM::GETGRADE_RESIST_SPIRIT () const
{
	BYTE cGRADE = grade_spirit();
	if ( cGRADE > GLCONST_CHAR::wGRADE_MAX )	cGRADE = GLCONST_CHAR::wGRADE_MAX_REGI;

	return GLCONST_CHAR::wRESIST_SPIRIT_GRADE * cGRADE;
}

BYTE SITEMCUSTOM::grade_damage () const
{
	return BYTE ( cDAMAGE + GETOptVALUE(EMR_OPT_GRIND_DAMAGE) );
}

BYTE SITEMCUSTOM::grade_defense () const
{
	return BYTE ( cDEFENSE + GETOptVALUE(EMR_OPT_GRIND_DEFENSE) );
}

BYTE SITEMCUSTOM::GETGRADE ( EMGRINDING_TYPE emTYPE ) const
{
	switch ( emTYPE )
	{
		//	Note : EMGRINDING_NUNE 으로 등급 질의시에 아이템 통합 등급 표시. ( 단 저항 부분 제외 )
	case EMGRINDING_NONE:
		{
			BYTE cDAM = grade_damage();
			BYTE cDEF = grade_defense();
			return cDAM > cDEF ? cDAM : cDEF;
		}

	case EMGRINDING_DAMAGE:
		return grade_damage();

	case EMGRINDING_DEFENSE:
		return grade_defense();

	case EMGRINDING_RESIST_FIRE:
		return grade_fire();

	case EMGRINDING_RESIST_ICE:
		return grade_ice();

	case EMGRINDING_RESIST_ELEC:
		return grade_elec();

	case EMGRINDING_RESIST_POISON:
		return grade_poison();

	case EMGRINDING_RESIST_SPIRIT:
		return grade_spirit();
	};

	return 0;
}

BYTE SITEMCUSTOM::GETGRADE_EFFECT () const
{
	BYTE cGRADE = GETGRADE(EMGRINDING_NONE);

	if ( cGRADE==0 )	return 0;

	// +9 등급 이하일 경우 
	if( cGRADE <= 9 )
	{
		return cGRADE = 1 + (cGRADE-1)/2;
	}
	
	return cGRADE = cGRADE - 4;
}

inline float SITEMCUSTOM::GETOptVALUE ( EMRANDOM_OPT emOPT, short nVALUE ) const
{
	switch ( emOPT )
	{
	default:
	case EMR_OPT_NULL:
		return 0.0f;

	case EMR_OPT_DAMAGE:
	case EMR_OPT_DEFENSE:
	case EMR_OPT_MOVE_SPEED:
		return nVALUE*0.01f;

	case EMR_OPT_HITRATE:
	case EMR_OPT_AVOIDRATE:
		return nVALUE*0.01f;

	case EMR_OPT_HP:
	case EMR_OPT_MP:
	case EMR_OPT_SP:
		return nVALUE;

	case EMR_OPT_HP_INC:
	case EMR_OPT_MP_INC:
	case EMR_OPT_SP_INC:
	case EMR_OPT_HMS_INC:
		return nVALUE*0.0001f;

	case EMR_OPT_GRIND_DAMAGE:
	case EMR_OPT_GRIND_DEFENSE:
		return nVALUE;

	case EMR_OPT_RANGE:
		return nVALUE*0.01f;

	case EMR_OPT_DIS_SP:
		return nVALUE;

	case EMR_OPT_RESIST:
		return nVALUE;

	/*official random value, Juver, 2018/06/09 */
	case EMR_OPT_DAMAGE_PLUS:
	case EMR_OPT_DEFENSE_PLUS:
	case EMR_OPT_HITRATE_PLUS:
	case EMR_OPT_AVOIDRATE_PLUS:
	case EMR_OPT_POW_PLUS:
	case EMR_OPT_STR_PLUS:
	case EMR_OPT_SPI_PLUS:
	case EMR_OPT_DEX_PLUS:
	case EMR_OPT_STA_PLUS:
	case EMR_OPT_PA_PLUS:
	case EMR_OPT_SA_PLUS:
	case EMR_OPT_MA_PLUS:
	case EMR_OPT_POTION_HP_PLUS:
	case EMR_OPT_POTION_MP_PLUS:
	case EMR_OPT_POTION_SP_PLUS:
	case EMR_OPT_GAIN_CP_PLUS:
		return nVALUE;

		/*additional RV, Juver, 2018/06/14 */
	case EMR_OPT_EX_MOVE_SPEED:
	case EMR_OPT_EX_ATTACK_SPEED:
	case EMR_OPT_EX_CRITICAL_HIT_CHANCE:
	case EMR_OPT_EX_CRITICAL_HIT_DAMAGE:
	case EMR_OPT_EX_CRUSHING_BLOW_CHANCE:
	case EMR_OPT_EX_CRUSHING_BLOW_DAMAGE:
		return nVALUE*0.01f;

		/*item max CP, Juver, 2018/07/09 */
	case EMR_OPT_EX_MAX_CP:
		return nVALUE;

	};

	return 0.0f;
}

void SITEMCUSTOM::SETOptVALUE ( int nIndex, EMRANDOM_OPT emOPT, float fVALUE )
{
	short nVALUE(0);

	switch ( emOPT )
	{
	default:
	case EMR_OPT_NULL:
		nVALUE = 0;
		break;

	case EMR_OPT_DAMAGE:
	case EMR_OPT_DEFENSE:
	case EMR_OPT_MOVE_SPEED:
		nVALUE = (short) (fVALUE*100.0f);
		break;

	case EMR_OPT_HITRATE:
	case EMR_OPT_AVOIDRATE:
		nVALUE = (short) (fVALUE*100.0f);
		break;

	case EMR_OPT_HP:
	case EMR_OPT_MP:
	case EMR_OPT_SP:
		nVALUE = (short) (fVALUE);
		break;

	case EMR_OPT_HP_INC:
	case EMR_OPT_MP_INC:
	case EMR_OPT_SP_INC:
	case EMR_OPT_HMS_INC:
		nVALUE = (short) (fVALUE*10000);
		break;

	case EMR_OPT_GRIND_DAMAGE:
	case EMR_OPT_GRIND_DEFENSE:
		nVALUE = (short) (fVALUE);
		break;

	case EMR_OPT_RANGE:
		nVALUE = (short) (fVALUE*100);
		break;

	case EMR_OPT_DIS_SP:
		nVALUE = (short) (fVALUE);
		break;

	case EMR_OPT_RESIST:
		nVALUE = (short) (fVALUE);
		break;

	/*official random value, Juver, 2018/06/09 */
	case EMR_OPT_DAMAGE_PLUS:
	case EMR_OPT_DEFENSE_PLUS:
	case EMR_OPT_HITRATE_PLUS:
	case EMR_OPT_AVOIDRATE_PLUS:
	case EMR_OPT_POW_PLUS:
	case EMR_OPT_STR_PLUS:
	case EMR_OPT_SPI_PLUS:
	case EMR_OPT_DEX_PLUS:
	case EMR_OPT_STA_PLUS:
	case EMR_OPT_PA_PLUS:
	case EMR_OPT_SA_PLUS:
	case EMR_OPT_MA_PLUS:
	case EMR_OPT_POTION_HP_PLUS:
	case EMR_OPT_POTION_MP_PLUS:
	case EMR_OPT_POTION_SP_PLUS:
	case EMR_OPT_GAIN_CP_PLUS:
		nVALUE = (short) (fVALUE);
		break;

		/*additional RV, Juver, 2018/06/14 */
	case EMR_OPT_EX_MOVE_SPEED:
	case EMR_OPT_EX_ATTACK_SPEED:
	case EMR_OPT_EX_CRITICAL_HIT_CHANCE:
	case EMR_OPT_EX_CRITICAL_HIT_DAMAGE:
	case EMR_OPT_EX_CRUSHING_BLOW_CHANCE:
	case EMR_OPT_EX_CRUSHING_BLOW_DAMAGE:
		nVALUE = (short) (fVALUE*100.0f);
		break;

		/*item max CP, Juver, 2018/07/09 */
	case EMR_OPT_EX_MAX_CP:
		nVALUE = (short) (fVALUE);
		break;

	};

	switch ( nIndex )
	{
	case 0:
		cOptTYPE1 = emOPT;
		nOptVALUE1 = nVALUE;
		break;

	case 1:
		cOptTYPE2 = emOPT;
		nOptVALUE2 = nVALUE;
		break;

	case 2:
		cOptTYPE3 = emOPT;
		nOptVALUE3 = nVALUE;
		break;

	case 3:
		cOptTYPE4 = emOPT;
		nOptVALUE4 = nVALUE;
		break;
	}
}

void SITEMCUSTOM::SETOptVALUEEmpty( EMRANDOM_OPT emOPT, float fVALUE )	// ITEMREBUILD_MARK
{
	if( cOptTYPE1 == 0 )
	{
		SETOptVALUE( 0, emOPT, fVALUE );
	}
	else if( cOptTYPE2 == 0 )
	{
		SETOptVALUE( 1, emOPT, fVALUE );
	}
	else if( cOptTYPE3 == 0 )
	{
		SETOptVALUE( 2, emOPT, fVALUE );
	}
	else if( cOptTYPE4 == 0 )
	{
		SETOptVALUE( 3, emOPT, fVALUE );
	}
}

int SITEMCUSTOM::RESETOptVALUE()
{
	int iEmpty = 0;

	if( IsValidRESETOpt( 0 ) )
	{
		cOptTYPE1 = 0;
		nOptVALUE1 = 0;
		++iEmpty;
	}

	if( IsValidRESETOpt( 1 ) )
	{
		cOptTYPE2 = 0;
		nOptVALUE2 = 0;
		++iEmpty;
	}

	if( IsValidRESETOpt( 2 ) )
	{
		cOptTYPE3 = 0;
		nOptVALUE3 = 0;
		++iEmpty;
	}

	if( IsValidRESETOpt( 3 ) )
	{
		cOptTYPE4 = 0;
		nOptVALUE4 = 0;
		++iEmpty;
	}

	/* official random option, Juver, 2018/02/28 */
	return ( SRANDOM_OPTION_GEN::MAX_OPT - iEmpty );
}

bool SITEMCUSTOM::IsValidRESETOpt( int nIndex )
{
	BYTE optType[] = { cOptTYPE1, cOptTYPE2, cOptTYPE3, cOptTYPE4 };

	if( optType[ nIndex ] == EMR_OPT_GRIND_DAMAGE && cDAMAGE )
		return false;
	if( optType[ nIndex ] == EMR_OPT_GRIND_DEFENSE && cDEFENSE )
		return false;

	return true;
}

float SITEMCUSTOM::GETOptVALUE1 () const
{
	EMRANDOM_OPT emOPT = GETOptTYPE1();
	short nVALUE = nOptVALUE1;

	return GETOptVALUE ( emOPT, nVALUE );
}

float SITEMCUSTOM::GETOptVALUE2 () const
{
	EMRANDOM_OPT emOPT = GETOptTYPE2();
	short nVALUE = nOptVALUE2;

	return GETOptVALUE ( emOPT, nVALUE );
}

float SITEMCUSTOM::GETOptVALUE3 () const
{
	EMRANDOM_OPT emOPT = GETOptTYPE3();
	short nVALUE = nOptVALUE3;

	return GETOptVALUE ( emOPT, nVALUE );
}

float SITEMCUSTOM::GETOptVALUE4 () const
{
	EMRANDOM_OPT emOPT = GETOptTYPE4();
	short nVALUE = nOptVALUE4;

	return GETOptVALUE ( emOPT, nVALUE );
}

BOOL SITEMCUSTOM::IsPerRandOpt( INT nRandOPT ) const
{
	/*official random value, Juver, 2018/06/08 */
	switch( nRandOPT )
	{
	case EMR_OPT_NULL:
	case EMR_OPT_DAMAGE:
	case EMR_OPT_DEFENSE:
	case EMR_OPT_HITRATE:
	case EMR_OPT_AVOIDRATE:
	case EMR_OPT_HP_INC:
	case EMR_OPT_MP_INC:
	case EMR_OPT_SP_INC:
	case EMR_OPT_HMS_INC:
	case EMR_OPT_MOVE_SPEED:
		return TRUE;

		/*additional RV, Juver, 2018/06/14 */
	case EMR_OPT_EX_MOVE_SPEED:
	case EMR_OPT_EX_ATTACK_SPEED:
	case EMR_OPT_EX_CRITICAL_HIT_CHANCE:
	case EMR_OPT_EX_CRITICAL_HIT_DAMAGE:
	case EMR_OPT_EX_CRUSHING_BLOW_CHANCE:
	case EMR_OPT_EX_CRUSHING_BLOW_DAMAGE:
		return TRUE;

	default:
		return FALSE;
	};

	//switch( nRandOPT )
	//{
	//case EMR_OPT_GRIND_DAMAGE:	// 공격력 연마.
	//case EMR_OPT_GRIND_DEFENSE:	// 방어력 연마.
	//case EMR_OPT_RANGE:			// 공격 영역.
	//case EMR_OPT_DIS_SP:		// sp 소모량.
	//	return FALSE;

	//default :
	//	return TRUE;
	//}

	return FALSE;
}

float SITEMCUSTOM::GETOptVALUE ( EMRANDOM_OPT emOPT ) const
{
	if ( GETOptTYPE1()==emOPT )			return GETOptVALUE1();
	else if ( GETOptTYPE2()==emOPT )	return GETOptVALUE2();
	else if ( GETOptTYPE3()==emOPT )	return GETOptVALUE3();
	else if ( GETOptTYPE4()==emOPT )	return GETOptVALUE4();

	return 0;
}

short SITEMCUSTOM::GETOptVALUE_SHORT ( EMRANDOM_OPT emOPT ) const
{
	if ( GETOptTYPE1()==emOPT )			return nOptVALUE1;
	else if ( GETOptTYPE2()==emOPT )	return nOptVALUE2;
	else if ( GETOptTYPE3()==emOPT )	return nOptVALUE3;
	else if ( GETOptTYPE4()==emOPT )	return nOptVALUE4;

	return 0;
}

BOOL SITEMCUSTOM::IsSetRandOpt() const
{
	if( GETOptTYPE1() != EMR_OPT_NULL )			return TRUE;
	else if( GETOptTYPE2() != EMR_OPT_NULL )	return TRUE;
	else if( GETOptTYPE3() != EMR_OPT_NULL )	return TRUE;
	else if( GETOptTYPE4() != EMR_OPT_NULL )	return TRUE;

	return FALSE;
}

#pragma optimize( "", off )
/* official random option, Juver, 2018/02/28 */
bool SITEMCUSTOM::GENERATE_RANDOM_OPT( bool bNew, BOOL block0, BOOL block1, BOOL block2, BOOL block3 )
{
	SRANDOM_OPTION_GEN *pRANDOM_SET = GLItemMan::GetInstance().GetItemRandomOpt( sNativeID );
	if( !pRANDOM_SET )
		return false;

	const SRANDOM_OPTION_GEN &sRANDOM = *pRANDOM_SET;

	if( bNew && !RANDOM_GEN( sRANDOM.fRATE ) )
		return true;

	if ( bNew )
	{
		std::vector< EMRANDOM_OPT > vecORDER;
		vecORDER.reserve( sRANDOM.mapSET.size() );
		for( SRANDOM_OPTION_SET_MAP_CITER iter = sRANDOM.mapSET.begin(); iter != sRANDOM.mapSET.end(); ++ iter ){
			vecORDER.push_back( (*iter).second.emTYPE );
		}

		std::random_shuffle( vecORDER.begin(), vecORDER.end() );

		int nGEN(0);
		for(int i=0; i<(int)vecORDER.size() && nGEN<SRANDOM_OPTION_GEN::MAX_OPT && nGEN < sRANDOM.wMaxGenD; ++i )
		{
			SRANDOM_OPTION_SET_MAP_CITER iter = sRANDOM.mapSET.find( vecORDER[i] );
			if ( iter == sRANDOM.mapSET.end() )	continue;

			const SRANDOM_OPTION_SET* pSET = &(*iter).second;

			if( pSET->emTYPE == EMR_OPT_NULL )	continue;

			float fNowRate = seqrandom::getpercent();
			if( fNowRate < pSET->fRATE )
			{
				int iOptSe = GENERATE_RANDOM_OPT_SE( pSET );
				if( 0 <= iOptSe && iOptSe < (int)pSET->vecSECT.size() )
				{
					float fPOS = seqrandom::getpercent();
					float fVALUE = pSET->vecSECT[ iOptSe ].fLOW;
					fVALUE += ( pSET->vecSECT[ iOptSe ].fHIGH - pSET->vecSECT[ iOptSe ].fLOW ) * fPOS * 0.01f;

					//this should not happen, unless compiler bug--
					if ( fVALUE == 0.0f )
					{
						if ( RPARAM::bRandomOptionLogs == 1 )
						{
							CDebugSet::ToFileWithTime( "_random_option.txt", "result 0 going to retry! file :%s type :%d rate:%g low:%g high:%g value:%g fPOS:%g", 
								pRANDOM_SET->strFileName.c_str(), pSET->emTYPE, pSET->vecSECT[iOptSe].fRATE, pSET->vecSECT[iOptSe].fLOW, pSET->vecSECT[iOptSe].fHIGH, fVALUE, fPOS );
						}

						fPOS = seqrandom::getpercent();
						fVALUE = pSET->vecSECT[ iOptSe ].fLOW;
						fVALUE += ( pSET->vecSECT[ iOptSe ].fHIGH - pSET->vecSECT[ iOptSe ].fLOW ) * fPOS * 0.01f;

						if ( fVALUE == 0.0f )
						{
							if ( RPARAM::bRandomOptionLogs )
							{
								CDebugSet::ToFileWithTime( "_random_option.txt", "result 0 after retry! file :%s type :%d rate:%g low:%g high:%g value:%g fPOS:%g", 
									pRANDOM_SET->strFileName.c_str(), pSET->emTYPE, pSET->vecSECT[iOptSe].fRATE, pSET->vecSECT[iOptSe].fLOW, pSET->vecSECT[iOptSe].fHIGH, fVALUE, fPOS );
							}
						}
					}

					//dont use 0 values!
					if ( fVALUE != 0.0f )
					{
						SETOptVALUEEmpty( (EMRANDOM_OPT)pSET->emTYPE, fVALUE );

						++nGEN;
					}

					
				}
				else
				{
					if ( RPARAM::bRandomOptionLogs )
					{
						CDebugSet::ToFileWithTime( "_random_option.txt", "generate failed option :%s invalid select: %d", pRANDOM_SET->strFileName.c_str(), iOptSe );
					}
				}
			}
		}

		if ( nGEN == 0 )
		{
			if ( RPARAM::bRandomOptionLogs )
			{
				CDebugSet::ToFileWithTime( "_random_option.txt", "generate failed file :%s mapset size: %d", pRANDOM_SET->strFileName.c_str(), (DWORD)sRANDOM.mapSET.size() );
			}
		}
	}
	else
	{
		/*item random option rebuild, Juver, 2018/07/05 */
		BYTE	cOptTYPE1_SAVE = cOptTYPE1;
		BYTE	cOptTYPE2_SAVE = cOptTYPE2;
		BYTE	cOptTYPE3_SAVE = cOptTYPE3;
		BYTE	cOptTYPE4_SAVE = cOptTYPE4;
		short	nOptVALUE1_SAVE = nOptVALUE1;
		short	nOptVALUE2_SAVE = nOptVALUE2;
		short	nOptVALUE3_SAVE = nOptVALUE3;
		short	nOptVALUE4_SAVE = nOptVALUE4;

		std::vector< EMRANDOM_OPT > vecORDER;
		vecORDER.reserve( sRANDOM.mapSETR.size() );
		for( SRANDOM_OPTION_SET_MAP_CITER iter = sRANDOM.mapSETR.begin(); iter != sRANDOM.mapSETR.end(); ++ iter )
		{
			EMRANDOM_OPT opt = (*iter).second.emTYPE;

			if ( block0 && cOptTYPE1_SAVE == opt )	continue;
			if ( block1 && cOptTYPE2_SAVE == opt )	continue;
			if ( block2 && cOptTYPE3_SAVE == opt )	continue;
			if ( block3 && cOptTYPE4_SAVE == opt )	continue;

			vecORDER.push_back( opt );
		}

		std::random_shuffle( vecORDER.begin(), vecORDER.end() );

		int nGEN = RESETOptVALUE();

		if ( block0 )
		{
			cOptTYPE1 = cOptTYPE1_SAVE;
			nOptVALUE1 = nOptVALUE1_SAVE;
			nGEN++;
		}

		if ( block1 )
		{
			cOptTYPE2 = cOptTYPE2_SAVE;
			nOptVALUE2 = nOptVALUE2_SAVE;
			nGEN++;
		}

		if ( block2 )
		{
			cOptTYPE3 = cOptTYPE3_SAVE;
			nOptVALUE3 = nOptVALUE3_SAVE;
			nGEN++;
		}

		if ( block3 )
		{
			cOptTYPE4 = cOptTYPE4_SAVE;
			nOptVALUE4 = nOptVALUE4_SAVE;
			nGEN++;
		}

		for(int i=0; i<(int)vecORDER.size() && nGEN<SRANDOM_OPTION_GEN::MAX_OPT && nGEN < sRANDOM.wMaxGenR; ++i )
		{
			SRANDOM_OPTION_SET_MAP_CITER iter = sRANDOM.mapSETR.find( vecORDER[i] );
			if ( iter == sRANDOM.mapSETR.end() )	continue;

			const SRANDOM_OPTION_SET* pSET = &(*iter).second;

			if( pSET->emTYPE == EMR_OPT_NULL )	continue;
			if( pSET->emTYPE == EMR_OPT_GRIND_DAMAGE && cDAMAGE )		continue;
			if( pSET->emTYPE == EMR_OPT_GRIND_DEFENSE && cDEFENSE )		continue;

			float fNowRate = seqrandom::getpercent();
			if( fNowRate < pSET->fRATE )
			{
				int iOptSe = GENERATE_RANDOM_OPT_SE( pSET );
				if( 0 <= iOptSe && iOptSe < (int)pSET->vecSECT.size() )
				{
					float fPOS = seqrandom::getpercent();
					float fVALUE = pSET->vecSECT[ iOptSe ].fLOW;
					fVALUE += ( pSET->vecSECT[ iOptSe ].fHIGH - pSET->vecSECT[ iOptSe ].fLOW ) * fPOS * 0.01f;

					//this should not happen, unless compiler bug--
					if ( fVALUE == 0.0f )
					{
						//retry random
						CDebugSet::ToFileWithTime( "_random_option2.txt", "result 0 going to retry! file :%s type :%d rate:%g low:%g high:%g value:%g fPOS:%g", 
							pRANDOM_SET->strFileName.c_str(), pSET->emTYPE, pSET->vecSECT[iOptSe].fRATE, pSET->vecSECT[iOptSe].fLOW, pSET->vecSECT[iOptSe].fHIGH, fVALUE, fPOS );

						fPOS = seqrandom::getpercent();
						fVALUE = pSET->vecSECT[ iOptSe ].fLOW;
						fVALUE += ( pSET->vecSECT[ iOptSe ].fHIGH - pSET->vecSECT[ iOptSe ].fLOW ) * fPOS * 0.01f;

						if ( fVALUE == 0.0f )
						{
							CDebugSet::ToFileWithTime( "_random_option2.txt", "result 0 after retry! file :%s type :%d rate:%g low:%g high:%g value:%g fPOS:%g", 
								pRANDOM_SET->strFileName.c_str(), pSET->emTYPE, pSET->vecSECT[iOptSe].fRATE, pSET->vecSECT[iOptSe].fLOW, pSET->vecSECT[iOptSe].fHIGH, fVALUE, fPOS );
						}
					}

					//dont use 0 values!
					if ( fVALUE != 0.0f )
					{
						SETOptVALUEEmpty( (EMRANDOM_OPT)pSET->emTYPE, fVALUE );

						++nGEN;
					}

					
				}
				else
				{
					CDebugSet::ToFileWithTime( "_random_option2.txt", "generate failed option :%s invalid select: %d", pRANDOM_SET->strFileName.c_str(), iOptSe );
				}
			}
		}
	}

	return true;
}

#pragma optimize( "", on ) 


#pragma optimize( "", off )
/* official random option, Juver, 2018/02/28 */
int SITEMCUSTOM::GENERATE_RANDOM_OPT_SE( const SRANDOM_OPTION_SET* pOptSet )
{
	if( !pOptSet )
		return INT_MIN;

	float fRatePer = seqrandom::getpercent();
	float fRateCur = 0.0f;

	for( int i=0; i<(int)pOptSet->vecSECT.size(); ++i )
	{
		float fRate = pOptSet->vecSECT[i].fRATE;

		if( ( fRateCur <= fRatePer ) && ( fRatePer < ( fRateCur + fRate ) ) )
			return i;

		fRateCur += fRate;
	}

	return INT_MIN;
}

#pragma optimize( "", on )

// *****************************************************
// Desc: 아이템 공격 데미지 반환
// *****************************************************
GLPADATA SITEMCUSTOM::getdamage () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return GLPADATA();
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	GLPADATA sDAMAGE = sSUIT.gdDamage;

	// 렌던 수치 합산
	/*official random value, Juver, 2018/06/08 */
	float fRATE = GETOptVALUE(EMR_OPT_DAMAGE);
	float addvalue = GETOptVALUE(EMR_OPT_DAMAGE_PLUS);
	if ( fRATE!=0.0f || addvalue != 0.0f )
	{
		sDAMAGE.wLow = WORD(sSUIT.gdDamage.wLow*(100.0f+fRATE)*0.01f);
		sDAMAGE.wHigh = WORD(sSUIT.gdDamage.wHigh*(100.0f+fRATE)*0.01f);

		sDAMAGE.VAR_PARAM( (int)addvalue );
	}

	return sDAMAGE;
}

// *****************************************************
// Desc: 기력(마력) 데미지 반환
// *****************************************************
WORD SITEMCUSTOM::getmadamage () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	WORD wMaDAMAGE(0);

	for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
	{
		const ITEM::SADDON &sADDON = sSUIT.sADDON[i];
		if ( sADDON.emTYPE==EMADD_MA )
		{
			wMaDAMAGE += sADDON.nVALUE;
		}
	}

	/*costume combine stats, Juver, 2017/09/01 */
	SITEM *pitem_data_disguise = GLItemMan::GetInstance().GetItem(nidDISGUISE);
	if ( pitem_data_disguise )
	{
		for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			const ITEM::SADDON &saddon_disguise = pitem_data_disguise->sSuitOp.sADDON[i];
			if ( saddon_disguise.emTYPE==EMADD_MA )
			{
				wMaDAMAGE += saddon_disguise.nVALUE;
			}
		}
	}
	
	// 기력 데미지에도 랜덤 수치를 가산한다
	/*official random value, Juver, 2018/06/08 */
	float fRATE = GETOptVALUE(EMR_OPT_DAMAGE);
	float addvalue = GETOptVALUE(EMR_OPT_DAMAGE_PLUS);
	addvalue = GETOptVALUE(EMR_OPT_MA_PLUS) + addvalue;
	if ( fRATE != 0.0f || addvalue != 0.0f )
	{
		wMaDAMAGE = WORD((100.0f+fRATE)*wMaDAMAGE*0.01f);

		if ( (int(wMaDAMAGE)+int(addvalue)) < 0 )				wMaDAMAGE = 0;
		else if ( (int(wMaDAMAGE)+int(addvalue))>int(0xffff) )	wMaDAMAGE = 0;
		else													wMaDAMAGE += (WORD)addvalue;
	}

	return wMaDAMAGE;
}

/*official random value, Juver, 2018/06/11 */
WORD SITEMCUSTOM::get_pa_damage () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	WORD value_total(0);

	for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
	{
		const ITEM::SADDON &sADDON = sSUIT.sADDON[i];
		if ( sADDON.emTYPE==EMADD_PA )
		{
			value_total += sADDON.nVALUE;
		}
	}

	/*costume combine stats, Juver, 2017/09/01 */
	SITEM *pitem_data_disguise = GLItemMan::GetInstance().GetItem(nidDISGUISE);
	if ( pitem_data_disguise )
	{
		for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			const ITEM::SADDON &saddon_disguise = pitem_data_disguise->sSuitOp.sADDON[i];
			if ( saddon_disguise.emTYPE==EMADD_PA )
			{
				value_total += saddon_disguise.nVALUE;
			}
		}
	}

	float addvalue = GETOptVALUE(EMR_OPT_PA_PLUS);
	if ( addvalue != 0.0f )
	{
		if ( (int(value_total)+int(addvalue)) < 0 )					value_total = 0;
		else if ( (int(value_total)+int(addvalue))>int(0xffff) )	value_total = 0;
		else														value_total += (WORD)addvalue;
	}

	return value_total;
}

/*official random value, Juver, 2018/06/11 */
WORD SITEMCUSTOM::get_sa_damage () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	WORD value_total(0);

	for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
	{
		const ITEM::SADDON &sADDON = sSUIT.sADDON[i];
		if ( sADDON.emTYPE==EMADD_SA )
		{
			value_total += sADDON.nVALUE;
		}
	}

	/*costume combine stats, Juver, 2017/09/01 */
	SITEM *pitem_data_disguise = GLItemMan::GetInstance().GetItem(nidDISGUISE);
	if ( pitem_data_disguise )
	{
		for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			const ITEM::SADDON &saddon_disguise = pitem_data_disguise->sSuitOp.sADDON[i];
			if ( saddon_disguise.emTYPE==EMADD_SA )
			{
				value_total += saddon_disguise.nVALUE;
			}
		}
	}

	float addvalue = GETOptVALUE(EMR_OPT_SA_PLUS);
	if ( addvalue != 0.0f )
	{
		if ( (int(value_total)+int(addvalue)) < 0 )					value_total = 0;
		else if ( (int(value_total)+int(addvalue))>int(0xffff) )	value_total = 0;
		else														value_total += (WORD)addvalue;
	}

	return value_total;
}

short SITEMCUSTOM::getdefense () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nDEFENSE = sSUIT.nDefense;

	/*official random value, Juver, 2018/06/09 */
	float fRATE = GETOptVALUE(EMR_OPT_DEFENSE);
	float addvalue = GETOptVALUE(EMR_OPT_DEFENSE_PLUS);
	if ( fRATE != 0.0f || addvalue != 0.0f )
	{
		nDEFENSE = short(addvalue) - short((100.0f+fRATE)*nDEFENSE*-0.01f);
	}

	return nDEFENSE;
}

float SITEMCUSTOM::GETMOVESPEED() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fSpeed = sSUIT.sVOLUME.fVolume;

	// 렌던 수치 합산
	float fRATE = GETOptVALUE(EMR_OPT_MOVE_SPEED);
	if ( fRATE!=0 )
	{
		fSpeed = ( fSpeed * ( 100.0f + fRATE )  * 0.01f );
	}

	return fSpeed;
}

float SITEMCUSTOM::GETMOVESPEEDR() const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fSpeed = sSUIT.sVARIATE.fVariate;

/*
	// 렌던 수치 합산
	float fRATE = GETOptVALUE(EMR_OPT_MOVE_SPEED);
	if ( fRATE!=0 )
	{
		fSpeed = ( fSpeed * ( 100.0f + fRATE )  * 0.01f );
	}
*/
	return fSpeed;
}

GLPADATA SITEMCUSTOM::GETDAMAGE () const
{
	GLPADATA &sDAMAGE = getdamage();

	sDAMAGE.wLow += GETGRADE_DAMAGE();
	sDAMAGE.wHigh += GETGRADE_DAMAGE();

	return sDAMAGE;
}

WORD SITEMCUSTOM::GETMaDAMAGE () const
{
	WORD wMaDAMAGE = getmadamage();

	wMaDAMAGE += GETGRADE_DAMAGE();

	return wMaDAMAGE;
}

short SITEMCUSTOM::GETDEFENSE () const
{
	short nDEFENSE = getdefense();

	nDEFENSE += GETGRADE_DEFENSE();

	return nDEFENSE;
}

short SITEMCUSTOM::GETHITRATE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nHITRATE = sSUIT.nHitRate;

	/*official random value, Juver, 2018/06/09 */
	float fRATE = GETOptVALUE(EMR_OPT_HITRATE);
	float addvalue = GETOptVALUE(EMR_OPT_HITRATE_PLUS);
	if ( fRATE != 0.0f || addvalue != 0.0f )
	{
		nHITRATE = short(addvalue) + short(nHITRATE+fRATE);
	}

	return nHITRATE;
}

short SITEMCUSTOM::GETAVOIDRATE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nAVOIDRATE = sSUIT.nAvoidRate;

	/*official random value, Juver, 2018/06/09 */
	float fRATE = GETOptVALUE(EMR_OPT_AVOIDRATE);
	float addvalue = GETOptVALUE(EMR_OPT_AVOIDRATE_PLUS);
	if ( fRATE != 0.0f || addvalue != 0.0f )
	{
		nAVOIDRATE = short(addvalue) + short(nAVOIDRATE+fRATE);
	}

	return nAVOIDRATE;
}

WORD SITEMCUSTOM::GETATTRANGE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	WORD wATTRANGE = sSUIT.wAttRange;

	float fRATE = GETOptVALUE(EMR_OPT_RANGE);
	if ( fRATE!=0.0f )
	{
		wATTRANGE = WORD(wATTRANGE+fRATE);
	}

	return wATTRANGE;
}

WORD SITEMCUSTOM::GETREQ_SP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	WORD wREQSP = sSUIT.wReqSP;

	float fVALUE = GETOptVALUE(EMR_OPT_DIS_SP);
	if ( fVALUE!=0.0f )
	{
		if ( fVALUE+wREQSP>0.0f )	wREQSP = WORD(wREQSP+fVALUE);
		else						wREQSP = 0;
	}

	return wREQSP;
}

int SITEMCUSTOM::GETADDHP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nHP(0);

	for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
	{
		if ( sSUIT.sADDON[addon].emTYPE==EMADD_HP )
		{
			nHP += sSUIT.sADDON[addon].nVALUE;
		}
	}

	nHP += (int) GETOptVALUE(EMR_OPT_HP);

	return nHP;
}

int SITEMCUSTOM::GETADDMP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nMP(0);

	for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
	{
		if ( sSUIT.sADDON[addon].emTYPE==EMADD_MP )
		{
			nMP += sSUIT.sADDON[addon].nVALUE;
		}
	}

	nMP += (int) GETOptVALUE(EMR_OPT_MP);

	return nMP;
}

int SITEMCUSTOM::GETADDSP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	int nSP(0);

	for ( DWORD addon=0; addon<ITEM::SSUIT::ADDON_SIZE; ++addon )
	{
		if ( sSUIT.sADDON[addon].emTYPE==EMADD_SP )
		{
			nSP += sSUIT.sADDON[addon].nVALUE;
		}
	}

	nSP += (int) GETOptVALUE(EMR_OPT_SP);

	return nSP;
}

int SITEMCUSTOM::GETADDMA () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	INT wMaDAMAGE(0);

	for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
	{
		const ITEM::SADDON &sADDON = sSUIT.sADDON[i];
		if ( sADDON.emTYPE==EMADD_MA )
		{
			wMaDAMAGE += sADDON.nVALUE;
		}
	}

	/*official random value, Juver, 2018/06/08 */
	float fRATE = GETOptVALUE(EMR_OPT_DAMAGE);
	float addvalue = GETOptVALUE(EMR_OPT_DAMAGE_PLUS);
	addvalue = GETOptVALUE(EMR_OPT_MA_PLUS) + addvalue;
	if ( fRATE != 0.0f || addvalue != 0.0f )
	{
		wMaDAMAGE = INT(addvalue) - INT((100.0f+fRATE)*wMaDAMAGE*-0.01f);
	}

	return wMaDAMAGE;
}

/*official random value, Juver, 2018/06/11 */
int SITEMCUSTOM::GETADDPA () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	INT value_total(0);

	for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
	{
		const ITEM::SADDON &sADDON = sSUIT.sADDON[i];
		if ( sADDON.emTYPE==EMADD_PA )
		{
			value_total += sADDON.nVALUE;
		}
	}

	value_total += (int) GETOptVALUE(EMR_OPT_PA_PLUS);

	return value_total;
}

/*official random value, Juver, 2018/06/11 */
int SITEMCUSTOM::GETADDSA () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	INT value_total(0);

	for ( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i )
	{
		const ITEM::SADDON &sADDON = sSUIT.sADDON[i];
		if ( sADDON.emTYPE==EMADD_SA )
		{
			value_total += sADDON.nVALUE;
		}
	}

	value_total += (int) GETOptVALUE(EMR_OPT_SA_PLUS);

	return value_total;
}

float SITEMCUSTOM::GETINCHP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fINC(0);

	//	Note : 변화율 효과.
	if ( sSUIT.sVARIATE.emTYPE==EMVAR_HP )
	{
		fINC += sSUIT.sVARIATE.fVariate;
	}

	fINC += GETOptVALUE(EMR_OPT_HP_INC);

	return fINC;
}

float SITEMCUSTOM::GETINCMP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fINC(0);

	//	Note : 변화율 효과.
	if ( sSUIT.sVARIATE.emTYPE==EMVAR_MP )
	{
		fINC += sSUIT.sVARIATE.fVariate;
	}

	fINC += GETOptVALUE(EMR_OPT_MP_INC);

	return fINC;
}

float SITEMCUSTOM::GETINCSP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fINC(0);

	//	Note : 변화율 효과.
	if ( sSUIT.sVARIATE.emTYPE==EMVAR_SP )
	{
		fINC += sSUIT.sVARIATE.fVariate;
	}

	fINC += GETOptVALUE(EMR_OPT_SP_INC);

	return fINC;
}

float SITEMCUSTOM::GETINCAP () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	float fINC(0);

	//	Note : 변화율 효과.
	if ( sSUIT.sVARIATE.emTYPE==EMVAR_AP )
	{
		fINC += sSUIT.sVARIATE.fVariate;
	}

	fINC += GETOptVALUE(EMR_OPT_HMS_INC);

	return fINC;
}

WORD SITEMCUSTOM::GETRESIST_FIRE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nRESIST = sSUIT.sResist.nFire;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_FIRE();
}

WORD SITEMCUSTOM::GETRESIST_ICE () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nRESIST = sSUIT.sResist.nIce;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_ICE();
}

WORD SITEMCUSTOM::GETRESIST_ELEC () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nRESIST = sSUIT.sResist.nElectric;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_ELEC();
}

WORD SITEMCUSTOM::GETRESIST_POISON () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nRESIST = sSUIT.sResist.nPoison;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_POISON();
}

WORD SITEMCUSTOM::GETRESIST_SPIRIT () const
{
	SITEM *pITEM = GLItemMan::GetInstance().GetItem(sNativeID);
	if ( !pITEM )	return 0;
	ITEM::SSUIT &sSUIT = pITEM->sSuitOp;

	short nRESIST = sSUIT.sResist.nSpirit;

	float fVALUE = GETOptVALUE(EMR_OPT_RESIST);
	if ( fVALUE!=0.0f )
	{
		nRESIST = int(nRESIST+fVALUE);
	}

	return nRESIST + GETGRADE_RESIST_SPIRIT();
}

/*official random value, Juver, 2018/06/09 */
short SITEMCUSTOM::GET_STATS_POW () const
{
	short value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_POW_PLUS);
	if ( addvalue != 0.0f )
		value = short(value+addvalue);

	return value;
}

/*official random value, Juver, 2018/06/09 */
short SITEMCUSTOM::GET_STATS_STR () const
{
	short value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_STR_PLUS);
	if ( addvalue != 0.0f )
		value = short(value+addvalue);

	return value;
}

/*official random value, Juver, 2018/06/09 */
short SITEMCUSTOM::GET_STATS_SPI () const
{
	short value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_SPI_PLUS);
	if ( addvalue != 0.0f )
		value = short(value+addvalue);

	return value;
}

/*official random value, Juver, 2018/06/09 */
short SITEMCUSTOM::GET_STATS_DEX () const
{
	short value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_DEX_PLUS);
	if ( addvalue != 0.0f )
		value = short(value+addvalue);

	return value;
}

/*official random value, Juver, 2018/06/09 */
short SITEMCUSTOM::GET_STATS_STA () const
{
	short value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_STA_PLUS);
	if ( addvalue != 0.0f )
		value = short(value+addvalue);

	return value;
}

/*official random value, Juver, 2018/06/12 */
WORD SITEMCUSTOM::get_potion_hp () const
{
	WORD value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_POTION_HP_PLUS);
	if ( addvalue != 0.0f )
		value = WORD(value+addvalue);

	return value;
}

/*official random value, Juver, 2018/06/12 */
WORD SITEMCUSTOM::get_potion_mp () const
{
	WORD value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_POTION_MP_PLUS);
	if ( addvalue != 0.0f )
		value = WORD(value+addvalue);

	return value;
}

/*official random value, Juver, 2018/06/12 */
WORD SITEMCUSTOM::get_potion_sp () const
{
	WORD value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_POTION_SP_PLUS);
	if ( addvalue != 0.0f )
		value = WORD(value+addvalue);

	return value;
}

/*official random value, Juver, 2018/06/12 */
WORD SITEMCUSTOM::get_potion_cp () const
{
	WORD value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_GAIN_CP_PLUS);
	if ( addvalue != 0.0f )
		value = WORD(value+addvalue);

	return value;
}

/*additional RV, Juver, 2018/06/14 */
float SITEMCUSTOM::get_ex_move_speed() const
{
	float fvalue = GETOptVALUE(EMR_OPT_EX_MOVE_SPEED);
	if ( fvalue != 0.0f )
		fvalue = fvalue * 0.01f;

	return fvalue;
}

/*additional RV, Juver, 2018/06/14 */
float SITEMCUSTOM::get_ex_attack_speed() const
{
	float fvalue = GETOptVALUE(EMR_OPT_EX_ATTACK_SPEED);
	if ( fvalue != 0.0f )
		fvalue = fvalue * 0.01f;

	return fvalue;
}

/*additional RV, Juver, 2018/06/14 */
float SITEMCUSTOM::get_ex_critical_hit_chance() const
{
	float fvalue = GETOptVALUE(EMR_OPT_EX_CRITICAL_HIT_CHANCE);
	return fvalue;
}

/*additional RV, Juver, 2018/06/14 */
float SITEMCUSTOM::get_ex_critical_hit_damage() const
{
	float fvalue = GETOptVALUE(EMR_OPT_EX_CRITICAL_HIT_DAMAGE);
	return fvalue;
}

/*additional RV, Juver, 2018/06/15 */
float SITEMCUSTOM::get_ex_crushing_blow_chance() const
{
	float fvalue = GETOptVALUE(EMR_OPT_EX_CRUSHING_BLOW_CHANCE);
	return fvalue;
}

/*additional RV, Juver, 2018/06/15 */
float SITEMCUSTOM::get_ex_crushing_blow_damage() const
{
	float fvalue = GETOptVALUE(EMR_OPT_EX_CRUSHING_BLOW_DAMAGE);
	return fvalue;
}

/*item max CP, Juver, 2018/07/09 */
WORD SITEMCUSTOM::get_max_cp () const
{
	WORD value = 0;
	float addvalue = GETOptVALUE(EMR_OPT_EX_MAX_CP);
	if ( addvalue != 0.0f )
		value = WORD(value+addvalue);

	return value;
}

SITEM_LOBY::SITEM_LOBY ()
	: sNativeID(false)
	, nidDISGUISE(false)

	, lnGenNum(0)
	, wTurnNum(1)
	, cGenType(EMGEN_DEFAULT)
	, cChnID(0)
	
	, cFieldID(0)
	
	, cDAMAGE(0)
	, cDEFENSE(0)
	, cRESIST_FIRE(0)
	
	, cRESIST_ICE(0)
	, cRESIST_ELEC(0)
	, cRESIST_POISON(0)
	, cRESIST_SPIRIT(0)

	, cOptTYPE1(0) // 공격력
	, cOptTYPE2(0) // 기저항
	, cOptTYPE3(0)
	, cOptTYPE4(0)

	, nOptVALUE1(0) // 공격력 %
	, nOptVALUE2(0) // 기저항 %
	, nOptVALUE3(0)
	, nOptVALUE4(0)

	/*item color, Juver, 2018/01/04 */
	, wColor1(ITEMCOLOR_WHITE)
	, wColor2(ITEMCOLOR_WHITE)
{
}

SITEM_LOBY::SITEM_LOBY ( SNATIVEID	sNID )
	: sNativeID(sNID)
	, nidDISGUISE(false)
	, lnGenNum(0)
	, wTurnNum(1)
	
	, cGenType(EMGEN_DEFAULT)
	, cChnID(0)
	
	, cFieldID(0)
	
	, cDAMAGE(0)
	, cDEFENSE(0)
	, cRESIST_FIRE(0)	
	, cRESIST_ICE(0)
	, cRESIST_ELEC(0)
	, cRESIST_POISON(0)
	, cRESIST_SPIRIT(0)

	, cOptTYPE1(0) // 공격력
	, cOptTYPE2(0) // 기저항
	, cOptTYPE3(0)
	, cOptTYPE4(0)

	, nOptVALUE1(0) // 공격력 %
	, nOptVALUE2(0) // 기저항 %
	, nOptVALUE3(0)
	, nOptVALUE4(0)

	/*item color, Juver, 2018/01/04 */
	, wColor1(ITEMCOLOR_WHITE)
	, wColor2(ITEMCOLOR_WHITE)
{
}

bool SITEM_LOBY::operator == ( const SITEM_LOBY &value )
{
	return !memcmp(this,&value,sizeof(SITEM_LOBY));
}

bool SITEM_LOBY::operator != ( const SITEM_LOBY &value )
{
	return 0!=memcmp(this,&value,sizeof(SITEM_LOBY));
}

void SITEM_LOBY::Assign ( const SITEMCUSTOM &sItemCustom )
{
	sNativeID			= sItemCustom.sNativeID;	
	nidDISGUISE			= sItemCustom.nidDISGUISE;

	lnGenNum			= sItemCustom.lnGenNum;
	wTurnNum			= sItemCustom.wTurnNum;
	cGenType			= sItemCustom.cGenType;	
	cChnID				= sItemCustom.cChnID;		

	cFieldID			= sItemCustom.cFieldID;

	cDAMAGE				= sItemCustom.cDAMAGE;	
	cDEFENSE			= sItemCustom.cDEFENSE;	
	cRESIST_FIRE		= sItemCustom.cRESIST_FIRE;
	cRESIST_ICE			= sItemCustom.cRESIST_ICE;
	cRESIST_ELEC		= sItemCustom.cRESIST_ELEC;
	cRESIST_POISON		= sItemCustom.cRESIST_POISON;
	cRESIST_SPIRIT		= sItemCustom.cRESIST_SPIRIT;

	cOptTYPE1			= sItemCustom.cOptTYPE1;
	cOptTYPE2			= sItemCustom.cOptTYPE2;
	cOptTYPE3			= sItemCustom.cOptTYPE3;
	cOptTYPE4			= sItemCustom.cOptTYPE4;
	
	nOptVALUE1			= sItemCustom.nOptVALUE1;
	nOptVALUE2			= sItemCustom.nOptVALUE2;
	nOptVALUE3			= sItemCustom.nOptVALUE3;
	nOptVALUE4			= sItemCustom.nOptVALUE4;

	/*item color, Juver, 2018/01/04 */
	wColor1				= sItemCustom.wColor1;
	wColor2				= sItemCustom.wColor2;

};

BYTE SITEMCLIENT::GETGRADE_EFFECT () const
{
	BYTE cGRADE = GETGRADE();

	if ( cGRADE==0 )	return 0;

	// +9 등급 이하일 경우 
	if( cGRADE <= 9 )
	{
		return cGRADE = 1 + (cGRADE-1)/2;
	}
	
	return cGRADE = cGRADE - 4;
}

BOOL SETPUTONITEMS_BYBUF ( SITEMCUSTOM *pPutOnItems, CByteStream &ByteStream )
{
	if ( ByteStream.IsEmpty() )	return TRUE;

	//	DWORD[VERSION] + DWORD[SIZE] + DWORD[NUMBER] + PUTONITEMS_ARRAY[SIZE]
	//
	DWORD dwVersion, dwSize, dwNum;

	ByteStream >> dwVersion;
	ByteStream >> dwSize;
	ByteStream >> dwNum;

	SITEMCUSTOM sItemCustom;
	for ( DWORD i=0; i<dwNum; i++ )
	{
		if ( i>=SLOT_TSIZE )	break;

		//	Note : 향후 버전에서 이전 버전을 대입 연산으로 처리 할 부분.
		//
		if ( dwVersion==0x0100 )
		{
			SITEMCUSTOM_100 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0101 )
		{
			SITEMCUSTOM_101 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0102 )
		{
			SITEMCUSTOM_102 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0103 )
		{
			SITEMCUSTOM_103 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0104 )
		{
			SITEMCUSTOM_104 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0105 )
		{
			SITEMCUSTOM_105 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0106 )
		{
			SITEMCUSTOM_106 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0107 )
		{
			SITEMCUSTOM_107 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0108 )
		{
			SITEMCUSTOM_108 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0109 )
		{
			SITEMCUSTOM_109 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );
		}
		else if ( dwVersion==0x0110)
		{
			SITEMCUSTOM_110 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );

		}
		else if ( dwVersion==0x0111)
		{
			SITEMCUSTOM_111 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );

		}
		else if ( dwVersion==0x0112)
		{
			SITEMCUSTOM_112 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );

		}
		else if ( dwVersion==0x0200)
		{
			SITEMCUSTOM_200 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );

		}
		else if ( dwVersion==0x0201)
		{
			SITEMCUSTOM_201 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			pPutOnItems[i].Assign ( sItemCustomOld );

		}
		else if ( dwVersion==SITEMCUSTOM::VERSION )
		{
			GASSERT(dwSize==sizeof(SITEMCUSTOM));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustom, sizeof(SITEMCUSTOM) );

			pPutOnItems[i] = sItemCustom;
		}
		else
		{
			CDebugSet::ErrorVersion( "SETPUTONITEMS_BYBUF1", dwVersion );
		}
	}

	return TRUE;
}


BOOL SETPUTONITEMS_BYBUF ( SITEM_LOBY *pPutOnItems, CByteStream &ByteStream )
{
	if ( ByteStream.IsEmpty() )	return TRUE;

	//	DWORD[VERSION] + DWORD[SIZE] + DWORD[NUMBER] + PUTONITEMS_ARRAY[SIZE]
	//
	DWORD dwVersion, dwSize, dwNum;

	ByteStream >> dwVersion;
	ByteStream >> dwSize;
	ByteStream >> dwNum;

	SITEMCUSTOM sItemCustom;
	for ( DWORD i=0; i<dwNum; i++ )
	{
		if ( i>=SLOT_TSIZE )	break;

		//	Note : 향후 버전에서 이전 버전을 대입 연산으로 처리 할 부분.
		//
		if ( dwVersion==0x0100 )
		{
			SITEMCUSTOM_100 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0101 )
		{
			SITEMCUSTOM_101 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0102 )
		{
			SITEMCUSTOM_102 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0103 )
		{
			SITEMCUSTOM_103 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0104 )
		{
			SITEMCUSTOM_104 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0105 )
		{
			SITEMCUSTOM_105 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0106 )
		{
			SITEMCUSTOM_106 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0107 )
		{
			SITEMCUSTOM_107 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0108 )
		{
			SITEMCUSTOM_108 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0109 )
		{
			SITEMCUSTOM_109 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0110 )
		{
			SITEMCUSTOM_110 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0111 )
		{
			SITEMCUSTOM_111 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0112 )
		{
			SITEMCUSTOM_112 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0200 )
		{
			SITEMCUSTOM_200 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==0x0201 )
		{
			SITEMCUSTOM_201 sItemCustomOld;
			GASSERT(dwSize==sizeof(sItemCustomOld));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustomOld, sizeof(sItemCustomOld) );

			sItemCustom.Assign ( sItemCustomOld );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else if ( dwVersion==SITEMCUSTOM::VERSION )
		{
			GASSERT(dwSize==sizeof(SITEMCUSTOM));
			ByteStream.ReadBuffer ( (LPBYTE)&sItemCustom, sizeof(SITEMCUSTOM) );
			pPutOnItems[i].Assign ( sItemCustom );
		}
		else
		{
			CDebugSet::ErrorVersion( "SETPUTONITEMS_BYBUF2", dwVersion );
		}
	}

	return TRUE;
}


bool CItemDrop::IsTakeItem ( DWORD dwPartyID, DWORD dwGaeaID )
{
	bool bhold(false);

	switch ( emGroup )
	{
	case EMGROUP_ONE:
		if ( dwHoldGID==GAEAID_NULL )		bhold = true;
		else if ( dwHoldGID==dwGaeaID )		bhold = true;
		break;

	case EMGROUP_PARTY:
		if ( dwHoldGID==PARTY_NULL )		bhold = true;
		else if ( dwHoldGID==dwPartyID )	bhold = true;
		break;

	case EMGROUP_GUILD:
		break;
	};

	return bhold;
}

bool CItemDrop::IsDropOut()
{
	return ( GLCONST_CHAR::wMAXITEM_AGE < fAge );
}

void CItemDrop::Update ( float fElapsedTime )
{
	fAge += fElapsedTime;

	//	Note : 임시 소유자 유효성 검사.
	//
	if ( dwHoldGID!=GAEAID_NULL )
	{
		if ( GLCONST_CHAR::wMAXITEM_HOLD < fAge )
		{
			switch ( emGroup )
			{
			case EMGROUP_ONE:
				dwHoldGID = GAEAID_NULL;
				break;

			case EMGROUP_PARTY:
				dwHoldGID = PARTY_NULL;
				break;

			case EMGROUP_GUILD:
				break;
			};
		}
	}	
}

bool CItemClientDrop::IsCollision ( const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt ) const
{
	BOOL bCol = COLLISION::IsCollisionLineToAABB ( vFromPt, vTargetPt, vMax, vMin );

	return FALSE != bCol;
}

bool CItemClientDrop::IsCollision ( CLIPVOLUME &cv ) const
{
	BOOL bCol = COLLISION::IsCollisionVolume ( cv, vMax, vMin );

	return FALSE != bCol;
}

HRESULT CItemClientDrop::RenderItem ( LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matWld, D3DMATERIALQ* pMaterials )
{
	if ( pSimMesh )
	{
		pSimMesh->RenderItem ( pd3dDevice, matWld, pMaterials );
	}

	return S_OK;
}


