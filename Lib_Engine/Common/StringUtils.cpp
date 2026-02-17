//	Note : 택스트 유틸리티.
//
//	(2002.12.02), [Seperator] 초기 제작. JDH.
//
//
#include "stdafx.h"

#include <boost/bind.hpp>

#include "./StringUtils.h"
#include "./CharSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace STRUTIL
{
	StateSaver::VEC_STR g_Separator;

	StateSaver::delimiters_t g_delimiters;

	void RegisterSeparator ( CString strSeparator )
	{
		if ( strSeparator.IsEmpty() )	return;

		if ( strSeparator.GetLength() > 1 )
		{
			MessageBox ( NULL, "separator must one char.", strSeparator, MB_OK );
			return;
		}

		g_Separator.push_back ( strSeparator.GetAt(0) );
		std::sort ( g_Separator.begin(), g_Separator.end() );
	}

	void ClearSeparator ()
	{
		g_Separator.clear();
	}

	void RegisterMatchingDelimiters( char open, char close, bool keep_delimiters )
	{
		delimiters_entry e;
		e.opening = open;
		e.closing = close;
		e.keep_delimiters = keep_delimiters;
		g_delimiters.push_back( e );
	}

	void ClearDelimiters()
	{
		g_delimiters.clear();
	}

	const int ERR_POS = -1;

	inline char* lower_bound ( char *pFirst, DWORD dwCount, char cFIND )
	{
		for (; 0 < dwCount; )
		{
			// divide and conquer, find half that contains answer
			DWORD dwCount2 = dwCount / 2;
			char *pMiddle = pFirst;
			pMiddle += dwCount2;

			if ( *pMiddle < cFIND )
				pFirst = ++pMiddle, dwCount -= dwCount2 + 1;
			else
				dwCount = dwCount2;
		}

		return (pFirst);
	}

	inline bool binary_search ( char *pFirst, DWORD dwCount, char cFIND )
	{
		const char *pFound = lower_bound ( pFirst, dwCount, cFIND );
		return (pFound!=(pFirst+dwCount) && !(cFIND < *pFound));
	}

	inline BOOL IsSeparator ( const char *_szBuffer, int _nLength, int &_nSepLength )
	{
		_nSepLength = ERR_POS;
		if ( _szBuffer==NULL || _szBuffer[0]==NULL )		return FALSE;

		bool bFOUND = binary_search ( &g_Separator[0], (DWORD)g_Separator.size(), _szBuffer[0] );
		if ( bFOUND )
		{
			_nSepLength = 1;
			return TRUE;
		}

		_nSepLength = 0;
		return FALSE;
	}

	bool is_opening_delimiter( char c, char &closing, bool &keep_delimiters )
	{
		StateSaver::delimiters_t::const_iterator it = std::find_if( g_delimiters.begin(), g_delimiters.end(),
																						boost::bind( &delimiters_entry::opening, _1 ) == c );
		if ( it == g_delimiters.end() )
			return false;
		assert( it->opening == c );
		closing = it->closing;
		keep_delimiters = it->keep_delimiters;
		return true;
	}

	BOOL StringSeparate ( CString &Str, CStringArray &StrArray )
	{
		StrArray.RemoveAll ();

		CString StrTemp;
		int nLength = Str.GetLength();
		LPCTSTR szBuffer = Str.GetString();

		bool delimited_token = false;
		char closing_delimiter;
		bool keep_delimiters;

		for ( int i=0; i<nLength; )
		{
			char cAt = szBuffer[i];

			if ( CHARSET::IsLeadChar(cAt) )
			{
				StrTemp += cAt;
				i++;

				if ( i<nLength )
				{
					StrTemp += szBuffer[i];
					i++;
				}

				continue;
			}

			int nSepLength = ERR_POS;
			if ( delimited_token )
			{
				StrTemp += cAt;
				i++;
				if ( cAt == closing_delimiter && ( i == nLength || IsSeparator( szBuffer + i, nLength - i, nSepLength ) ) )
				{
					assert( StrTemp.GetLength() >= 2 );
					if ( !keep_delimiters ) {
						StrTemp.Delete( StrTemp.GetLength() - 1 );
						StrTemp.Delete( 0 );
					}
					StrArray.Add ( StrTemp );
					StrTemp = "";
					delimited_token = false;
				}
			}
			else {
				if ( IsSeparator ( szBuffer+i, nLength-i, nSepLength ) )
				{
					if ( nSepLength == ERR_POS )	return FALSE;

					i += nSepLength;

					//	분리된 스트링 추가.
					if ( StrTemp.GetLength() > 0 )
					{
						StrArray.Add ( StrTemp );
						StrTemp = "";
					}
				}
				else
				{
					StrTemp += cAt;
					i++;
					if ( StrTemp.GetLength() == 1 && is_opening_delimiter( cAt, closing_delimiter, keep_delimiters ) )
						delimited_token = true;
				}
			}
		}

		//	분리된 스트링 추가.
		if ( StrTemp.GetLength() > 0 )
		{
			StrArray.Add ( StrTemp );
		}

		return FALSE;
	}

	StateSaver::StateSaver()
	{
		swap();
	}

	StateSaver::~StateSaver()
	{
		swap();
	}

	void StateSaver::swap()
	{
		separators_.swap( g_Separator );
		delimiters_.swap( g_delimiters );
	}

	BOOL ChangeExt ( const char* _szSrc, CString &strDes, const char* szDesExt )
	{
		char szSrcTemp[MAX_PATH] = "";
		StringCchCopy( szSrcTemp, MAX_PATH, _szSrc );

		_strlwr ( szSrcTemp );
		char* strExtension = strrchr(szSrcTemp,'.');
		if ( !strExtension )					return FALSE;

		int nLength = (int)(strExtension - szSrcTemp);
		
		for ( int i=0; i<nLength; i++ )
		{
			strDes += szSrcTemp[i];
		}
		strDes += szDesExt;

		return TRUE;
	}

	BOOL ChangeExt ( const char* _szSrc, const char* szSrcExt, CString &strDes, const char* szDesExt )
	{
		char szSrcTemp[MAX_PATH] = "";
		StringCchCopy( szSrcTemp, MAX_PATH, _szSrc );

		_strlwr ( szSrcTemp );
		char* strExtension = strrchr(szSrcTemp,'.');
		if ( !strExtension )					return FALSE;
		if ( _stricmp(strExtension,szSrcExt) )	return FALSE;

		int nLength = (int)(strExtension - szSrcTemp);
		
		for ( int i=0; i<nLength; i++ )
		{
			strDes += szSrcTemp[i];
		}

		strDes += szDesExt;

		return TRUE;
	}

	VOID OutputStrCsv( std::fstream &SFile, std::string &Str )
	{
		// Csv 파일 로딩할 시 Separater는 구분자가 연속되면 무시하기때문에 공백 넣어둠
		if( Str.empty() )
		{
			SFile << " " << ",";
			return;
		}

		// 문자열에 쉼표 ','가 포함될 수 있기때문에 숫자1 옆의 문자 '`'로 교체함
		for( std::string::iterator ci = Str.begin(); ci != Str.end(); ++ci )
			if( ( *ci ) == ',' )
				*ci = '`';
		SFile << Str << ",";
	}

	VOID InputStrCsv( CString &Src, std::string &Dest )
	{
		Dest.clear();

		// OutputStrCsv 때문에 공백이면 클리어하고 아니면 값을 저장
		if( CString::StringLength( Src ) == 0 )
			return;
		if( Src.GetAt( 0 ) == ' ' && CString::StringLength( Src ) < 2 )
			return;

		// 문자열에 '`'가 포함되어 있으면 쉼표 ','로 교체함
		Src.Replace( '`', ',' );
		Dest = Src;
	}

	BOOL CheckString( const CString & strChk )
	{
		if( strChk.FindOneOf("~!@#$%^&*+|:?><,.;[]{}()\n\t\v\b\r\a\\\?\'\" ") != -1 )
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL CheckVietnamString( const CString & strChk )
	{
		int size = 0;
		TCHAR chBefore = '\0';
		size = strChk.GetLength();

		TCHAR strText[MAX_PATH] = {0};
		_tcscpy( strText, strChk );

		for ( int i = 0; i < size; ++i )
		{
			if ( CheckCharSunjo(strText[i])  &&  !CheckCharMoum(chBefore) )
			{
				return TRUE;
			}

			chBefore = strText[i];
		}

		return FALSE;
	}
	BOOL CheckCharSunjo( const TCHAR chText )
	{
		const int nSungjo = 5;
		static const UCHAR szSungjo[nSungjo] = { 0xcc, 0xd2, 0xde, 0xec, 0xf2 }; // 성조

		for ( int i = 0; i< nSungjo; ++i )
		{
			if ( chText == szSungjo[i] )
				return TRUE;
		}
		return FALSE;
	}

	BOOL CheckCharMoum( const TCHAR chText )
	{
		const int nMoum = 24;				
		/* A, a, E, e, I, i , O, o, U, u Y, y 베트남 모음들 */
		static const UCHAR szMoum[nMoum] = { 0x41, 0x61, 0xc3, 0xe3, 0xc2, 0xe2, 0x45, 0x65, 0xca, 0xea,
											 0x49, 0x69, 0x4f, 0x6f, 0xd4, 0xf4, 0xd5, 0xf5, 0x55, 0x75,
											 0xdd, 0xfd, 0x59, 0x79 };

		for ( int i = 0; i< nMoum; ++i )
		{
			if ( chText == szMoum[i] )
				return TRUE;
		}
		return FALSE;
	}

	BOOL CheckString_Special( const CString & strChk )
	{
		if( strChk.FindOneOf("_~!@#$%^&*+|:?><,.;[]{}()\n\t\v\b\r\a\\\?\'\" ") != -1 )
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL CheckString_Special2( const CString & strChk )
	{
		if( strChk.FindOneOf("~!#$%^&*+|:?><,;[]{}()\n\t\v\b\r\a\\\?\'\" ") != -1 )
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL CheckString_Special3( const CString & strChk )
	{
		if( strChk.FindOneOf("~!#$%^&*+|:?><,;[]{}()\n\t\v\b\r\a\\\?\'\"") != -1 )
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL CheckString_Letter( const CString & strChk )
	{
		if( strChk.FindOneOf("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1 )
		{
			return TRUE;
		}

		return FALSE;
	}
};

void CSEPARATOR::Register ( char cSEP )
{
	m_Separator.push_back ( cSEP );
	std::sort ( m_Separator.begin(), m_Separator.end() );
}

void CSEPARATOR::Clear ()
{
	m_Separator.clear();
}

inline bool CSEPARATOR::IsSeparator ( char cVALUE )
{
	return STRUTIL::binary_search ( &m_Separator[0], (DWORD)m_Separator.size(), cVALUE );
}

bool CSEPARATOR::DoSeparate ( const std::string strSource, std::vector<std::string> &strArray )
{
	strArray.clear();
	std::string strtemp;
	strtemp.reserve ( strSource.size() );

	//CString StrTemp;
	int nLength = (int)strSource.length();
	const char* szSOURCE = strSource.c_str();

	for ( int i=0; i<nLength; )
	{
		char cVAL = szSOURCE[i];

		if ( CHARSET::IsLeadChar(cVAL) )
		{
			strtemp += cVAL;
			++i;

			if ( i<nLength )
			{
				strtemp += szSOURCE[i];
				++i;
			}

			continue;
		}

		if ( !IsSeparator(cVAL) )
		{
			strtemp += cVAL;
			++i;
			continue;
		}

		++i;

		//	분리된 스트링 추가.
		if ( !strtemp.empty() )
		{
			strArray.push_back ( strtemp );
			strtemp = "";
		}
	}

	//	분리된 스트링 추가.
	if ( !strtemp.empty() )
	{
		strArray.push_back ( strtemp );
	}

	return FALSE;
}

#if 0
struct StringUtils_test {
	StringUtils_test()
	{
		STRUTIL::StateSaver saver;

		STRUTIL::RegisterSeparator ( "\t" );
		STRUTIL::RegisterSeparator ( " " );
		STRUTIL::RegisterSeparator ( "," );

		CString strLine;
		CStringArray strArray ;

		strLine = "abc def";
		STRUTIL::StringSeparate ( strLine, strArray );
		assert( strArray.GetSize() == 2 );
		assert( strArray[ 0 ] == "abc" );
		assert( strArray[ 1 ] == "def" );

		STRUTIL::RegisterMatchingDelimiters( '"', '"' );
		STRUTIL::RegisterMatchingDelimiters( '<', '>', true );

		strLine = "abc def";
		STRUTIL::StringSeparate ( strLine, strArray );
		assert( strArray.GetSize() == 2 );
		assert( strArray[ 0 ] == "abc" );
		assert( strArray[ 1 ] == "def" );

		strLine = "abcdef";
		STRUTIL::StringSeparate ( strLine, strArray );
		assert( strArray.GetSize() == 1 );
		assert( strArray[ 0 ] == "abcdef" );

		strLine = "abc \"hey how you doin'\" <not bad thank you> great stuff";
		STRUTIL::StringSeparate ( strLine, strArray );
		assert( strArray.GetSize() == 5 );
		assert( strArray[ 0 ] == "abc" );
		assert( strArray[ 1 ] == "hey how you doin'" );
		assert( strArray[ 2 ] == "<not bad thank you>" );
		assert( strArray[ 3 ] == "great" );
		assert( strArray[ 4 ] == "stuff" );

		strLine = "\"hey how you doin'\",<not bad thank you>";
		STRUTIL::StringSeparate ( strLine, strArray );
		assert( strArray.GetSize() == 2 );
		assert( strArray[ 0 ] == "hey how you doin'" );
		assert( strArray[ 1 ] == "<not bad thank you>" );
	}
}	g_strutils_test;
#endif
