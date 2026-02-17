#include "stdafx.h"

#include <boost/foreach.hpp>
#include "./tinyxml.h"

#include "../Core/NSRParam.h"
#include "../Common/StringFormat.h"
#include "../Common/DepthWithTextToFile.h"
#include "../DxCommon/DxFontMan.h"

#include "./UIControlListXml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace UIControlList
{
	bool IsValidData( const void* szValue, const TCHAR* szMsg, int nCount, std::string& strID )
	{
		if( szValue == NULL )
		{
			std::string ErrorMsg(
				sc::string::format(
					"[UIControlList/%1%] Count:%2%, ID:%3% Data is not valid",
					szMsg, nCount, strID ) );
			
			CDebugSet::ToLogFileSTR( ErrorMsg );

			return false;
		}

		return true;
	}

	bool IsValidData( const TCHAR* szValue, const TCHAR* szMsg, int nCount, std::string& strID )
	{
		if( szValue[ 0 ] == _T( '\0' ) )
		{
			std::string ErrorMsg(
				sc::string::format(
					"[UIControlList/%1%] Count:%2%, ID:%3% Data is not valid",
					szMsg, nCount, strID ) );
			
			CDebugSet::ToLogFileSTR( ErrorMsg );

			return false;
		}

		return true;
	}

	bool Parse( TiXmlDocument& XmlDoc, ControlMap& mapControl )
	{
		LONG nCount = 0;
		TCHAR szValue[ 1024 ] = { 0 };
		WCHAR szValueW[ 1024 ] = { 0 };

		TiXmlNode* pRootNode = NULL;
		TiXmlElement* pRootElement = NULL;
		TiXmlElement* pCurrentElement = NULL;

		try
		{
			pRootNode = XmlDoc.FirstChild( "INTERFACE" );
			if( !pRootNode )
				return false;

			pRootElement = pRootNode->FirstChildElement( "CONTROL" );
			while( pRootElement )
			{
				std::string strID;
				SControl sControl;

				if( !IsValidData( pRootElement, "CONTROL", nCount, std::string( "" ) ) )
					goto GotoNextElement;

				_tcscpy( szValue, pRootElement->Attribute( "Id" ) );
				if( !IsValidData( szValue, "CONTROL/Id", nCount, std::string( "" ) ) )
					goto GotoNextElement;
				strID = szValue;

				if( mapControl.find( strID ) != mapControl.end() )
				{
					std::string ErrorMsg(
						sc::string::format(
							"[UIControlList] Count:%1%, ID:%2% Duplicate ID",
							nCount, strID ) );
					
					CDebugSet::ToLogFileSTR( ErrorMsg );

					goto GotoNextElement;
				}

				pCurrentElement = pRootElement->FirstChildElement( "WINDOW_POS" );
				if( pCurrentElement )
				{
					_tcscpy( szValue, pCurrentElement->Attribute( "X" ) );
					if( !IsValidData( szValue, "CONTROL/WINDOW_POS/X", nCount, strID ) )
						goto GotoNextElement;
					sControl.m_vPos.x = (float)_tstof( szValue );

					_tcscpy( szValue, pCurrentElement->Attribute( "Y" ) );
					if( !IsValidData( szValue, "CONTROL/WINDOW_POS/Y", nCount, strID ) )
						goto GotoNextElement;
					sControl.m_vPos.y = (float)_tstof( szValue );

					_tcscpy( szValue, pCurrentElement->Attribute( "W" ) );
					if( !IsValidData( szValue, "CONTROL/WINDOW_POS/W", nCount, strID ) )
						goto GotoNextElement;
					sControl.m_vPos.z = (float)_tstof( szValue );

					_tcscpy( szValue, pCurrentElement->Attribute( "H" ) );
					if( !IsValidData( szValue, "CONTROL/WINDOW_POS/H", nCount, strID ) )
						goto GotoNextElement;
					sControl.m_vPos.w = (float)_tstof( szValue );
				}

				pCurrentElement = pRootElement->FirstChildElement( "TEXTURE" );
				if( pCurrentElement )
				{
					_tcscpy( szValue, pCurrentElement->Attribute( "Id" ) );
					if( !IsValidData( szValue, "CONTROL/TEXTURE/Id", nCount, strID ) )
						goto GotoNextElement;
					sControl.m_strTextureID = szValue;
				}

				mapControl.insert( ControlMapValue( strID, sControl ) );

GotoNextElement:
				pRootElement = pRootElement->NextSiblingElement( "CONTROL" );
				++nCount;
			}
		}
		catch( ... )
		{
			return false;
		}

		return true;
	}

	void LoadFileList( const std::string& strFilePath, std::vector< std::string >& vecFileList )
    {
        TiXmlDocument XmlDoc;

        if( !XmlDoc.LoadFile( strFilePath ) )
            return;

        TiXmlNode* pRootNode = NULL;
        TiXmlElement* pRootElement = NULL;

        try
        {
            pRootNode = XmlDoc.FirstChild( "INTERFACE" );
            if( !pRootNode )
                goto GotoLoadFileListEnd;

            pRootElement = pRootNode->FirstChildElement( "UIXML" );
            while( pRootElement )
            {
                const char* pChar = pRootElement->FirstChild()->Value();
                if( pChar )
                    vecFileList.push_back( pChar );

                pRootElement = pRootElement->NextSiblingElement( "UIXML" );
            }
        }
        catch( ... )
        {
        }

GotoLoadFileListEnd:
        XmlDoc.Clear();
    }

	bool CXmlLoader::Load( const std::string& strPath, const std::string& strListName, ControlMap& mapControl )
	{
		bool bReturn = true;

        std::string strFilePath = strPath;
		strFilePath.append( strListName );

		std::vector< std::string > vecFileList;
        LoadFileList( strFilePath, vecFileList );

		BOOST_FOREACH( std::string& strFileName, vecFileList )
		{
            strFilePath = strPath;
            strFilePath.append( strFileName );

            TiXmlDocument XmlDoc;
            
            if( !XmlDoc.LoadFile( strFilePath ) )
                continue;

            if( !Parse( XmlDoc, mapControl ) )
				bReturn = false;

            XmlDoc.Clear();
        }

		return bReturn;
	}
}