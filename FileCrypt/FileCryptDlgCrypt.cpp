
// FileCryptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./FileCrypt.h"
#include "./FileCryptDlg.h"
#include "./FileCryptGlobal.h"

#include "../Lib_Engine/Core/NSRInfo.h"
#include "../Lib_Engine/Common/RCryptXML.h"
#include "../Lib_Engine/Common/RCryptByte.h"

#include "../Lib_Helper/EtcFunction.h"

#include "../Lib_Engine/Common/Rijndael.h"
#include "../Lib_Engine/Common/RijndaelGS.h"
#include "../Lib_Engine/Common/RijndaelGSV4.h"
#include "../Lib_Engine/Common/StringFile.h"
#include "../Lib_Engine/Common/StringFileGS.h"
#include "../Lib_Engine/Common/StringFileGSV4.h"

#include "../Lib_Engine/DxCommon/MemoryTexture.h"
#include "../Lib_Engine/DxCommon/MemoryXFile.h"
#include "../Lib_Engine/Meshs/SAnimation.h"
#include "../Lib_Engine/Meshs/DxSkinAniMan.h"
#include "../Lib_Engine/Common/CompByte.h"
#include "../Lib_Engine/Common/Ran2Crypt.h"
#include "../Lib_Engine/DxEffect/Single/DxEffSinglePropGMan.h"
#include "../Lib_Engine/Meshs/DxSkinPieceContainer.h"
#include "../Lib_Engine/Meshs/DxSkinMeshMan.h"

#include <boost/filesystem.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CFileCryptDlg::DoCrypt( BOOL bENCRYPT )
{
	if ( m_strInputFolder.empty() || m_strOutputFolder.empty() )
	{
		InfoAdd( "Folder invalid Input:'%s' Output:'%s'", m_strInputFolder.c_str(), m_strOutputFolder.c_str() );
		return;
	}

	InfoReset();
	InfoAdd( "Begin search files in folder: '%s'", m_strInputFolder.c_str() );

	m_cTree.CreateTree( m_strInputFolder );
	if( m_cTree.IsEmpty() )
	{
		InfoAdd( "Folder: '%s' is empty!", m_strInputFolder.c_str() );
		return;
	}

	InfoAdd( "Found:%d Files", m_cTree.GetFileMap().size() );
	InfoAdd( "Files will be saved in Folder: '%s'", m_strOutputFolder.c_str() );

	EMCRYPT_FILETYPE emFileType = static_cast<EMCRYPT_FILETYPE>( GetWin_Combo_Sel( this, IDC_CMB_FILETYPE) );
	switch( emFileType )
	{
	case EMCRYPT_FILETYPE_TEXTFILE:
		{
			if ( bENCRYPT )
				TextEncrypt();
			else
				TextDecrypt();
		}break;
	case EMCRYPT_FILETYPE_STREAMFILE:
		{
			if ( bENCRYPT )
				StreamEncrypt();
			else
				StreamDecrypt();
		}break;
	case EMCRYPT_FILETYPE_STREAMFILE_R2:
		{
			if ( bENCRYPT )
				InfoAdd( "No Encryption for Ran2" );
			else
				StreamDecryptR2();
		}break;

	case EMCRYPT_FILETYPE_STREAMFILE_ORIG:
		{
			if ( bENCRYPT )
				InfoAdd( "No Encryption for Orig Stream" );
			else
				StreamDecryptOrig();
		}break;

	case EMCRYPT_FILETYPE_XMLCRYPT:
		{
			if ( bENCRYPT )
				XMLEncrypt();
			else
				XMLDecrypt();
		}break;

	case EMCRYPT_FILETYPE_TEXTURECRYPT:
		{
			if ( bENCRYPT )
				TextureEncrypt();
			else
				TextureDecrypt();
		}break;

	case EMCRYPT_FILETYPE_XFILE_CRYPT:
		{
			if ( bENCRYPT )
				XFileEncrypt();
			else
				XFileDecrypt();
		}break;

	case EMCRYPT_FILETYPE_ANIMFILE:
		{
			if ( bENCRYPT )
				AniFileEncrypt();
			else
				InfoAdd( "No Decryption for Animation Files" );
		}break;

	case EMCRYPT_FILETYPE_EGP_TO_NEW:
		{
			if ( bENCRYPT )
				EGPtoNEW();
			else
				InfoAdd( "No Decryption for EGP Files" );
		}break;

	case EMCRYPT_FILETYPE_R2_EGP_TO_NEW:
		{
			if ( bENCRYPT )
				Ran2EGPtoNEW();
			else
				InfoAdd( "No Decryption for EGP Files" );
		}break;

	case EMCRYPT_FILETYPE_CPS_TO_NEW:
		{
			if ( bENCRYPT )
				CPStoNEW();
			else
				InfoAdd( "No Decryption for CPS Files" );
		}break;

	case EMCRYPT_FILETYPE_R2_CPS_TO_NEW:
		{
			if ( bENCRYPT )
				Ran2CPStoNEW();
			else
				InfoAdd( "No Decryption for CPS Files" );
		}break;

	case EMCRYPT_FILETYPE_TEXTFILE_GS:
		{
			if ( bENCRYPT )
				TextEncryptGS();
			else
				TextDecryptGS();
		}break;

	case EMCRYPT_FILETYPE_TEXTFILE_GS_V4:
		{
			if ( bENCRYPT )
				TextEncryptGSV4();
			else
				TextDecryptGSV4();
		}break;
	};

}

void CFileCryptDlg::TextEncrypt()
{
	InfoAdd( "Start TextFile Encrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		CStringFile stringFile;
		if ( !stringFile.Open ( strInputFilePath.c_str(), FALSE ) )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CString strTemp, strLine;
		while ( stringFile.GetNextLine(strLine) )
		{
			strLine += "\r\n";
			strTemp += strLine;
		}

		stringFile.Close();

		CRijndael oRijndael;
		oRijndael.Initialize( CRijndael::VERSION, CRijndael::sm_Version[CRijndael::VERSION-1].c_str(), CRijndael::sm_KeyLength[CRijndael::VERSION-1] );

		int nLen = oRijndael.GetEncryptStringLength( strTemp );
		char * szBuffer = new char[nLen];

		oRijndael.EncryptEx( strTemp.GetString(), szBuffer, strTemp.GetLength() );
		int nVersion = CRijndael::VERSION;

		CFile file;
		if ( !file.Open( strOutputFile.c_str(), CFile::modeCreate|CFile::modeWrite ) )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			delete [] szBuffer;
			continue;
		}

		file.Write( &nVersion, sizeof(int) );
		file.Write( szBuffer, nLen );
		file.Close();

		delete [] szBuffer;
		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::TextDecrypt()
{
	InfoAdd( "Start TextFile Decrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		CStringFile stringFile;
		if ( !stringFile.Open ( strInputFilePath.c_str(), TRUE ) )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CString strTemp, strLine;
		while ( stringFile.GetNextLine(strLine) )
		{
			strLine += "\r\n";
			strTemp += strLine;
		}

		stringFile.Close();

		CFile file;
		if ( !file.Open( strOutputFile.c_str(), CFile::modeCreate|CFile::modeWrite ) )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			continue;
		}

		file.Write( strTemp.GetString(), strTemp.GetLength() );
		file.Close();
		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::StreamEncrypt()
{
#ifdef _DEBUG
	EMRCRYPT_BYTE_TYPE emByteCrypt_Type = static_cast<EMRCRYPT_BYTE_TYPE>( GetWin_Combo_Sel( this, IDC_CMB_CRYPTTYPE) );
	if ( emByteCrypt_Type == EMRCRYPT_BYTE_TYPE_NONE )
	{
		InfoAdd( "Invalid Encode Type" );
		return;
	}

	EMCRYPT_BYPASSHEADER emBypassHeader = static_cast<EMCRYPT_BYPASSHEADER>( GetWin_Combo_Sel( this, IDC_CMB_BYPASSHEADER) );
	int nBYPASS = 0;
	if ( emBypassHeader == EMCRYPT_BYPASSHEADER_1 )
		nBYPASS = 128;
	else if ( emBypassHeader == EMCRYPT_BYPASSHEADER_2 )
		nBYPASS = 132;
	else if ( emBypassHeader == EMCRYPT_BYPASSHEADER_3 )
		nBYPASS = 136;

	InfoAdd( "Start StreamFile Encrypt Using: %s", FILE_CRYPT_GLOBAL::byte_crypt_type[emByteCrypt_Type].c_str() );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		fseek ( pFileInput, 0, SEEK_END );
		int nSize = ftell ( pFileInput );
		fseek ( pFileInput, 0, SEEK_SET );	

		BYTE* pBuffer = new BYTE[nSize];
		fread( pBuffer , sizeof(BYTE), nSize, pFileInput );
		fclose( pFileInput );

		if ( nBYPASS >= nSize )
		{
			InfoAdd( "Bypass size more than file size! File: '%s'", strInputFileName.c_str() );
			delete[] pBuffer;
			continue;
		}

		RCryptByte cCrypt;
		cCrypt.SetType(emByteCrypt_Type);
		cCrypt.Encrypt( (BYTE*)pBuffer+nBYPASS, nSize-nBYPASS );

		FILE* pFileOutput = NULL;
		pFileOutput = fopen( strOutputFile.c_str(), "wb" );
		if ( !pFileOutput )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			delete[] pBuffer;
			continue;
		}

		fwrite( pBuffer, sizeof(BYTE), nSize, pFileOutput );
		fclose( pFileOutput );

		delete[] pBuffer;

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
#endif
}

void CFileCryptDlg::StreamDecrypt()
{
#ifdef _DEBUG
	EMRCRYPT_BYTE_TYPE emByteCrypt_Type = static_cast<EMRCRYPT_BYTE_TYPE>( GetWin_Combo_Sel( this, IDC_CMB_CRYPTTYPE) );

	if ( emByteCrypt_Type == EMRCRYPT_BYTE_TYPE_NONE )
	{
		InfoAdd( "Invalid Encode Type" );
		return;
	}

	EMCRYPT_BYPASSHEADER emBypassHeader = static_cast<EMCRYPT_BYPASSHEADER>( GetWin_Combo_Sel( this, IDC_CMB_BYPASSHEADER) );
	int nBYPASS = 0;
	if ( emBypassHeader == EMCRYPT_BYPASSHEADER_1 )
		nBYPASS = 128;
	else if ( emBypassHeader == EMCRYPT_BYPASSHEADER_2 )
		nBYPASS = 132;
	else if ( emBypassHeader == EMCRYPT_BYPASSHEADER_3 )
		nBYPASS = 136;

	InfoAdd( "Start StreamFile Decrypt Using: %s", FILE_CRYPT_GLOBAL::byte_crypt_type[emByteCrypt_Type].c_str() );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		fseek ( pFileInput, 0, SEEK_END );
		int nSize = ftell ( pFileInput );
		fseek ( pFileInput, 0, SEEK_SET );	

		BYTE* pBuffer = new BYTE[nSize];
		fread( pBuffer , sizeof(BYTE), nSize, pFileInput );
		fclose( pFileInput );

		if ( nBYPASS >= nSize )
		{
			InfoAdd( "Bypass size more than file size! File: '%s'", strInputFileName.c_str() );
			delete[] pBuffer;
			continue;
		}

		RCryptByte cCrypt;
		cCrypt.SetType(emByteCrypt_Type);
		cCrypt.Decrypt( (BYTE*)pBuffer+nBYPASS, nSize-nBYPASS );

		FILE* pFileOutput = NULL;
		pFileOutput = fopen( strOutputFile.c_str(), "wb" );
		if ( !pFileOutput )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			delete[] pBuffer;
			continue;
		}

		fwrite( pBuffer, sizeof(BYTE), nSize, pFileOutput );
		fclose( pFileOutput );

		delete[] pBuffer;

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
#endif
}

void CFileCryptDlg::StreamDecryptR2()
{
	InfoAdd( "Start StreamFile(Ran2) Decrypt" );

	BOOL bConvertCrypt = GetWin_Check( this, IDC_CHECK_CONVERT_ENCRYPT );
	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		fseek ( pFileInput, 0, SEEK_END );
		int nSize = ftell ( pFileInput );
		fseek ( pFileInput, 0, SEEK_SET );	

		BYTE* pBuffer = new BYTE[nSize];
		fread( pBuffer , sizeof(BYTE), nSize, pFileInput );
		fclose( pFileInput );

		if ( 144 >= nSize )
		{
			InfoAdd( "Skip size more than file size! File: '%s'", strInputFileName.c_str() );
			delete[] pBuffer;
			continue;
		}

		if ( 132 >= nSize )
		{
			InfoAdd( "Bypass size more than file size! File: '%s'", strInputFileName.c_str() );
			delete[] pBuffer;
			continue;
		}

		RAN2_CRYPT::byte_decode( (BYTE*)pBuffer+132, nSize-132 );

		//copy data
		//skip 12 bytes
		int nBufferNewSize = nSize-12;
		BYTE* pBufferNew = new BYTE[nBufferNewSize];
		memcpy( pBufferNew, pBuffer, 132 );
		memcpy( pBufferNew+132, pBuffer+144, nSize-144 );
		
		if ( bConvertCrypt )
			compbyte::encode_old( (BYTE*)pBufferNew+132, nBufferNewSize-132 );

		FILE* pFileOutput = NULL;
		pFileOutput = fopen( strOutputFile.c_str(), "wb" );
		if ( !pFileOutput )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			delete[] pBuffer;
			delete[] pBufferNew;
			continue;
		}

		fwrite( pBufferNew, sizeof(BYTE), nBufferNewSize, pFileOutput );
		fclose( pFileOutput );

		delete[] pBuffer;
		delete[] pBufferNew;

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::StreamDecryptOrig()
{
	EMCRYPT_BYPASSHEADER emBypassHeader = static_cast<EMCRYPT_BYPASSHEADER>( GetWin_Combo_Sel( this, IDC_CMB_BYPASSHEADER) );
	int nBYPASS = 0;
	if ( emBypassHeader == EMCRYPT_BYPASSHEADER_1 )
		nBYPASS = 128;
	else if ( emBypassHeader == EMCRYPT_BYPASSHEADER_2 )
		nBYPASS = 132;
	else if ( emBypassHeader == EMCRYPT_BYPASSHEADER_3 )
		nBYPASS = 136;

	InfoAdd( "Start StreamFile Decrypt Using Old Stream" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		fseek ( pFileInput, 0, SEEK_END );
		int nSize = ftell ( pFileInput );
		fseek ( pFileInput, 0, SEEK_SET );	

		BYTE* pBuffer = new BYTE[nSize];
		fread( pBuffer , sizeof(BYTE), nSize, pFileInput );
		fclose( pFileInput );

		if ( nBYPASS >= nSize )
		{
			InfoAdd( "Bypass size more than file size! File: '%s'", strInputFileName.c_str() );
			delete[] pBuffer;
			continue;
		}

		compbyte::decode_old( (BYTE*)pBuffer+nBYPASS, nSize-nBYPASS );

		FILE* pFileOutput = NULL;
		pFileOutput = fopen( strOutputFile.c_str(), "wb" );
		if ( !pFileOutput )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			delete[] pBuffer;
			continue;
		}

		fwrite( pBuffer, sizeof(BYTE), nSize, pFileOutput );
		fclose( pFileOutput );

		delete[] pBuffer;

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::XMLEncrypt()
{
	InfoAdd( "Start XML File Encrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;
		boost::filesystem::path fspFullPath(strOutputFile);
		fspFullPath.replace_extension( RINFO::getc( EMRINFO_XML_FILE_EXT_ENCRYPT ) );
		strOutputFile = fspFullPath.string();

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		fseek ( pFileInput, 0, SEEK_END );
		int nSize = ftell ( pFileInput );
		fseek ( pFileInput, 0, SEEK_SET );	

		BYTE* pBuffer = new BYTE[nSize];
		fread( pBuffer , sizeof(BYTE), nSize, pFileInput );
		fclose( pFileInput );

		CRCryptXML cXMLCrypt;
		cXMLCrypt.Initialize();
		cXMLCrypt.Encrypt( pBuffer, nSize );

		FILE* pFileOutput = NULL;
		pFileOutput = fopen( strOutputFile.c_str(), "wb" );
		if ( !pFileOutput )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			delete[] pBuffer;
			continue;
		}

		fwrite( pBuffer, sizeof(BYTE), nSize, pFileOutput );
		fclose( pFileOutput );

		delete[] pBuffer;

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::XMLDecrypt()
{
	InfoAdd( "Start XML File Decrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;
		boost::filesystem::path fspFullPath(strOutputFile);
		fspFullPath.replace_extension( RINFO::getc( EMRINFO_XML_FILE_EXT_DECRYPT ) );
		strOutputFile = fspFullPath.string();

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		fseek ( pFileInput, 0, SEEK_END );
		int nSize = ftell ( pFileInput );
		fseek ( pFileInput, 0, SEEK_SET );	

		BYTE* pBuffer = new BYTE[nSize];
		fread( pBuffer , sizeof(BYTE), nSize, pFileInput );
		fclose( pFileInput );

		CRCryptXML cXMLCrypt;
		cXMLCrypt.Initialize();
		cXMLCrypt.Decrypt( pBuffer, nSize );

		FILE* pFileOutput = NULL;
		pFileOutput = fopen( strOutputFile.c_str(), "wb" );
		if ( !pFileOutput )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			delete[] pBuffer;
			continue;
		}

		fwrite( pBuffer, sizeof(BYTE), nSize, pFileOutput );
		fclose( pFileOutput );

		delete[] pBuffer;

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}


void CFileCryptDlg::TextureEncrypt()
{
	InfoAdd( "Start Texture File Encrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CMemoryTexture cTexture;
		bool bok = cTexture.EncryptTexture( pFileInput, strOutputFile.c_str() );
		if ( !bok )	
		{
			InfoAdd( "Failed Open Encrypt File: '%s' to %s", strInputFileName.c_str(), strOutputFile.c_str() );
			continue;
		}

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::TextureDecrypt()
{
	InfoAdd( "Start Texture File Decrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CMemoryTexture cTexture;
		bool bok = cTexture.DecryptTextureToFile( pFileInput, strOutputFile.c_str() );
		if ( !bok )	
		{
			InfoAdd( "Failed Open Encrypt File: '%s' to %s", strInputFileName.c_str(), strOutputFile.c_str() );
			continue;
		}

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}


void CFileCryptDlg::XFileEncrypt()
{
	InfoAdd( "Start X File Encrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CMemoryXFile cXFile;
		bool bok = cXFile.EncryptSkin( pFileInput, strOutputFile.c_str() );
		if ( !bok )	
		{
			InfoAdd( "Failed Open Encrypt File: '%s' to %s", strInputFileName.c_str(), strOutputFile.c_str() );
			continue;
		}

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::XFileDecrypt()
{
	InfoAdd( "Start X File Decrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		FILE* pFileInput = NULL;
		pFileInput = fopen( strInputFilePath.c_str(), "rb" );
		if ( !pFileInput )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CMemoryXFile cXFile;
		bool bok = cXFile.DecryptSkinToTile( pFileInput, strOutputFile.c_str() );
		if ( !bok )	
		{
			InfoAdd( "Failed Open Encrypt File: '%s' to %s", strInputFileName.c_str(), strOutputFile.c_str() );
			continue;
		}

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::AniFileEncrypt()
{
	InfoAdd( "Start Animation(Bin) File Encrypt" );

	std::string skinpath = m_strOutputFolder;
	skinpath += "\\";
	DxSkinAniMan::GetInstance().OneTimeSceneInit( (char*)skinpath.c_str() );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		SAnimContainer anim;
		HRESULT hr = anim.LoadAnimFromFile( strInputFilePath.c_str() );
		if ( FAILED( hr) )
		{
			InfoAdd( "Failed Load Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		std::string replace = strInputFileName.c_str();
		std::size_t found = replace.rfind('.');
		if ( found != std::string::npos )
		{
			replace.erase( found );
			replace.append(".x");

			StringCchCopy( anim.m_szName, ACF_SZNAME, replace.c_str() );
		}
		else
		{
			InfoAdd( "Invalid Extension of Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		hr = anim.SaveToFile(); 
		if ( FAILED( hr) )
		{
			InfoAdd( "Failed Save File: '%s' to %s", strInputFileName.c_str(), strOutputFile.c_str() );
			continue;
		}
	
		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::EGPtoNEW()
{
	InfoAdd( "Start EGP->NEW File Convert" );

	std::string strEffectPath = m_strInputFolder;
	strEffectPath += "\\";
	DxEffSinglePropGMan::GetInstance().SetPath( strEffectPath.c_str() );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		EFF_PROPGROUP* pEFF = new EFF_PROPGROUP;
		pEFF->CleanUp();

		HRESULT hr = pEFF->LoadFile( strInputFileName.c_str(), NULL );
		if ( FAILED( hr ))
		{
			InfoAdd( "Failed Load Input File: '%s'", strInputFileName.c_str() );

			pEFF->CleanUp();
			SAFE_DELETE( pEFF );
			continue;
		}

		hr  = pEFF->SaveFile( strOutputFile.c_str() );
		if ( FAILED( hr ))
		{
			InfoAdd( "Failed Save File: '%s' to %s", strInputFileName.c_str(), strOutputFile.c_str() );

			pEFF->CleanUp();
			SAFE_DELETE( pEFF );
			continue;
		}

		pEFF->CleanUp();
		SAFE_DELETE( pEFF );

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::Ran2EGPtoNEW()
{
	InfoAdd( "Start Ran2 EGP->NEW File Convert" );

	std::string strEffectPath = m_strInputFolder;
	strEffectPath += "\\";
	DxEffSinglePropGMan::GetInstance().SetPath( strEffectPath.c_str() );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		EFF_PROPGROUP* pEFF = new EFF_PROPGROUP;
		pEFF->CleanUp();

		HRESULT hr = pEFF->R2LoadFile( strInputFileName.c_str(), NULL );
		if ( FAILED( hr ))
		{
			InfoAdd( "Failed Load Input File: '%s'", strInputFileName.c_str() );

			pEFF->CleanUp();
			SAFE_DELETE( pEFF );
			continue;
		}

		hr  = pEFF->SaveFile( strOutputFile.c_str() );
		if ( FAILED( hr ))
		{
			InfoAdd( "Failed Save File: '%s' to %s", strInputFileName.c_str(), strOutputFile.c_str() );

			pEFF->CleanUp();
			SAFE_DELETE( pEFF );
			continue;
		}

		pEFF->CleanUp();
		SAFE_DELETE( pEFF );

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::CPStoNEW()
{
	InfoAdd( "Start CPS->NEW File Convert" );

	std::string strEtcSavePath = m_strOutputFolder;
	strEtcSavePath += "\\etc\\";
	DxSkinMeshMan::GetInstance().OneTimeSceneInit( (char*)strEtcSavePath.c_str() );
	CreateDirectory ( strEtcSavePath.c_str(), NULL );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;

		DxSkinPiece* pPiece = new DxSkinPiece;

		std::string strSkinPiecePath = m_strInputFolder;
		strSkinPiecePath += "\\";
		DxSkinPieceContainer::GetInstance().SetPath( (char*)strSkinPiecePath.c_str() );

		HRESULT hr = pPiece->LoadPiece( strInputFileName.c_str(), NULL, FALSE );
		if ( FAILED( hr ))
		{
			InfoAdd( "Failed Load Input File: '%s'", strInputFileName.c_str() );

			pPiece->ClearAll();
			SAFE_DELETE( pPiece );
			continue;
		}


		strSkinPiecePath = m_strOutputFolder;
		strSkinPiecePath += "\\";
		DxSkinPieceContainer::GetInstance().SetPath( (char*)strSkinPiecePath.c_str() );

		hr  = pPiece->SavePiece( strInputFileName.c_str() );
		if ( FAILED( hr ))
		{
			InfoAdd( "Failed Save File: '%s' to %s", strInputFileName.c_str(), strInputFileName.c_str() );

			pPiece->ClearAll();
			SAFE_DELETE( pPiece );
			continue;
		}

		pPiece->ClearAll();
		SAFE_DELETE( pPiece );

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}


void CFileCryptDlg::Ran2CPStoNEW()
{
	InfoAdd( "Start Ran2 CPS->NEW File Convert" );

	std::string strEtcSavePath = m_strOutputFolder;
	strEtcSavePath += "\\etc\\";
	DxSkinMeshMan::GetInstance().OneTimeSceneInit( (char*)strEtcSavePath.c_str() );
	CreateDirectory ( strEtcSavePath.c_str(), NULL );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;

		DxSkinPiece* pPiece = new DxSkinPiece;

		std::string strSkinPiecePath = m_strInputFolder;
		strSkinPiecePath += "\\";
		DxSkinPieceContainer::GetInstance().SetPath( (char*)strSkinPiecePath.c_str() );

		HRESULT hr = pPiece->R2LoadPiece( strInputFileName.c_str(), NULL, FALSE );
		if ( FAILED( hr ))
		{
			InfoAdd( "Failed Load Input File: '%s'", strInputFileName.c_str() );

			pPiece->ClearAll();
			SAFE_DELETE( pPiece );
			continue;
		}


		strSkinPiecePath = m_strOutputFolder;
		strSkinPiecePath += "\\";
		DxSkinPieceContainer::GetInstance().SetPath( (char*)strSkinPiecePath.c_str() );

		hr  = pPiece->SavePiece( strInputFileName.c_str() );
		if ( FAILED( hr ))
		{
			InfoAdd( "Failed Save File: '%s' to %s", strInputFileName.c_str(), strInputFileName.c_str() );

			pPiece->ClearAll();
			SAFE_DELETE( pPiece );
			continue;
		}

		pPiece->ClearAll();
		SAFE_DELETE( pPiece );

		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::TextEncryptGS()
{
	InfoAdd( "Start TextFile GS Encrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		CStringFile stringFile;
		if ( !stringFile.Open ( strInputFilePath.c_str(), FALSE ) )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CString strTemp, strLine;
		while ( stringFile.GetNextLine(strLine) )
		{
			strLine += "\r\n";
			strTemp += strLine;
		}

		stringFile.Close();

		CRijndaelGS oRijndael;
		oRijndael.Initialize( 
			CRijndaelGS::sm_Version[CRijndaelGS::VERSION-1].c_str(), 
			CRijndaelGS::sm_KeyLength[CRijndaelGS::VERSION-1],  
			CRijndaelGS::VERSION_DATE, 
			CRijndaelGS::VERSION,
			CRijndaelGS::sm_chain0, 
			CRijndaelGS::sm_KeyLength[CRijndaelGS::VERSION-1] );

		int nLen = oRijndael.GetEncryptStringLength( strTemp );
		char * szBuffer = new char[nLen];

		oRijndael.EncryptEx( strTemp.GetString(), szBuffer, strTemp.GetLength() );
		int nVersion = CRijndaelGS::VERSION;

		CFile file;
		if ( !file.Open( strOutputFile.c_str(), CFile::modeCreate|CFile::modeWrite ) )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			delete [] szBuffer;
			continue;
		}

		file.Write( &nVersion, sizeof(int) );
		file.Write( szBuffer, nLen );
		file.Close();

		delete [] szBuffer;
		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::TextDecryptGS()
{
	InfoAdd( "Start TextFile GS Decrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		CStringFileGS stringFile;
		if ( !stringFile.Open ( strInputFilePath.c_str(), TRUE ) )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CString strTemp, strLine;
		while ( stringFile.GetNextLine(strLine) )
		{
			strLine += "\r\n";
			strTemp += strLine;
		}

		stringFile.Close();

		CFile file;
		if ( !file.Open( strOutputFile.c_str(), CFile::modeCreate|CFile::modeWrite ) )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			continue;
		}

		file.Write( strTemp.GetString(), strTemp.GetLength() );
		file.Close();
		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}


void CFileCryptDlg::TextEncryptGSV4()
{
	InfoAdd( "Start TextFile GSV4 Encrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		CStringFile stringFile;
		if ( !stringFile.Open ( strInputFilePath.c_str(), FALSE ) )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CString strTemp, strLine;
		while ( stringFile.GetNextLine(strLine) )
		{
			strLine += "\r\n";
			strTemp += strLine;
		}

		stringFile.Close();

		CRijndaelGSV4 oRijndael;
		oRijndael.Initialize( CRijndaelGSV4::sm_Version[CRijndaelGSV4::VERSION-1].c_str(), CRijndaelGSV4::sm_KeyLength[CRijndaelGSV4::VERSION-1] );

		int nLen = oRijndael.GetEncryptStringLength( strTemp );
		char * szBuffer = new char[nLen];

		oRijndael.EncryptEx( strTemp.GetString(), szBuffer, strTemp.GetLength() );
		int nVersion = CRijndaelGSV4::VERSION;

		CFile file;
		if ( !file.Open( strOutputFile.c_str(), CFile::modeCreate|CFile::modeWrite ) )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			delete [] szBuffer;
			continue;
		}

		file.Write( &nVersion, sizeof(int) );
		file.Write( szBuffer, nLen );
		file.Close();

		delete [] szBuffer;
		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}

void CFileCryptDlg::TextDecryptGSV4()
{
	InfoAdd( "Start TextFile GSV4 Decrypt" );

	CFileFindTree::FILEMAP mapFile = m_cTree.GetFileMap();
	int nSuccess = 0;
	int nTotalFiles = (int)mapFile.size();
	CFileFindTree::FILEMAP_ITER iter_start = mapFile.begin();
	CFileFindTree::FILEMAP_ITER iter_end = mapFile.end();
	for ( ; iter_start != iter_end; ++iter_start )
	{
		std::string strInputFileName = (*iter_start).first;
		std::string strInputFilePath = (*iter_start).second;
		std::string strOutputFile = m_strOutputFolder + "\\" + strInputFileName;

		CStringFileGSV4 stringFile;
		if ( !stringFile.Open ( strInputFilePath.c_str(), TRUE ) )
		{
			InfoAdd( "Failed Open Input File: '%s'", strInputFileName.c_str() );
			continue;
		}

		CString strTemp, strLine;
		while ( stringFile.GetNextLine(strLine) )
		{
			strLine += "\r\n";
			strTemp += strLine;
		}

		stringFile.Close();

		CFile file;
		if ( !file.Open( strOutputFile.c_str(), CFile::modeCreate|CFile::modeWrite ) )
		{
			InfoAdd( "Failed Open Output File: '%s'", strOutputFile.c_str() );
			continue;
		}

		file.Write( strTemp.GetString(), strTemp.GetLength() );
		file.Close();
		nSuccess++;
	}

	InfoAdd( "%d Files Finished", nSuccess );

	if ( nSuccess != nTotalFiles )
	{
		InfoAdd( "%d Files Failed", nTotalFiles - nSuccess );
	}
}