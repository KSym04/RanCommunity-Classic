#include "StdAfx.h"
#include "./BasicTextBox.h"
#include "../../Lib_Engine/GUInterface/UIRenderQueue.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*item link, Juver, 2017/07/31 */
/* Chat Color, Mhundz 02/22/25 */
SITEMLINK CBasicTextBox::GetItemLink ( int nIndex )
{
	const int nSize = (int)m_TextBox.size ();

	SITEMLINK sITEM;
	if ( nSize < 1 )
	{
		GASSERT ( 0 && "" );
		return sITEM;
	}

	if ( nIndex < 0 || nSize <= nIndex )
	{
		GASSERT ( 0 && "" );
		return sITEM;
	}	

	static CString strCombine;
	strCombine.Empty ();

	STEXTPART& sTEXTPART = m_TextBox[nIndex];

	sITEM = sTEXTPART.m_sItemLink;
	return sITEM;
}

/*item link, Juver, 2017/07/31 */
/* Chat Color, Mhundz 02/22/25 */
int	CBasicTextBox::AddText ( const CString& strText, SITEMLINK* pItemLink , const D3DCOLOR& TextColor )
{
	if ( !m_pFont )
	{
		GASSERT ( 0 && "폰트가 널입니다. SetFont()를 이용하여 폰트부터 값을 넣으십시오." );
		return 0;
	}

	//	문자열 자르기
	NS_UITEXTUTIL::ClearCRLFText ();
	NS_UITEXTUTIL::MULTILINETEXT& MLText = NS_UITEXTUTIL::GetMLTextWithoutCRLF ( strText, GetLocalPos().sizeX, m_pFont );

	int nSplitLine = (int)MLText.size ();

	STEXTPART sTextPart;
	
	if ( pItemLink )
	{
		SITEM* pITEMFIND = GLItemMan::GetInstance().GetItem( pItemLink->sNativeID );
		if ( pITEMFIND )
		{
			CString strKEY = pItemLink->GetFilter();
			BOOL bLINKEXIST = FALSE;
			int nSTART = 0;
			{
				//verify if itemlink text does really exist in input text
				std::string strSEARCH = strText.GetString();
				int nFIND = strSEARCH.find( strKEY.GetString() );
				if (nFIND!=std::string::npos)	
				{
					//found it now itemlink is really valid
					bLINKEXIST = TRUE;
					nSTART = nFIND;
					sTextPart.m_sItemLink = *pItemLink;
				}
			}

			if ( bLINKEXIST )
			{
				BOOL bFOUNDLINK = FALSE;

				//now we verify if itemlink text is not being split to lines
				for ( int i = 0; i < nSplitLine; ++i )
				{
					std::string strSEARCH = MLText[i].strLine.GetString();
					int nFIND = strSEARCH.find( strKEY.GetString() );
					if (nFIND!=std::string::npos)	
					{
						bFOUNDLINK = TRUE;
					}
				}

				if ( !bFOUNDLINK )
				{
					//damn problem text got split need to fix
					CString strNewKEY;
					strNewKEY += "\r\n";
					strNewKEY += pItemLink->GetFilter();

					CString strTEXTNEW = strText;
					strTEXTNEW.Replace( strKEY.GetString(), strNewKEY.GetString() );
					NS_UITEXTUTIL::ClearCRLFText ();
					MLText = NS_UITEXTUTIL::GetMLTextWithoutCRLF ( strTEXTNEW, GetLocalPos().sizeX, m_pFont );
				}
			}
		}		
	}





	//	데이타로 집어넣기
	for ( int i = 0; i < nSplitLine; ++i )
	{		
		STEXTLINE sTextLine;

		if ( (i - 1 == nSplitLine) && !MLText[i].strLine.GetLength () )
		{
			//	마지막라인이 빈 경우.
		}
		else
		{
			SIZE strSize;
			m_pFont->GetTextExtent ( MLText[i].strLine.GetString(), strSize );

			sTextLine.strSize = strSize;
			sTextLine.bLASTLINE = MLText[i].bLASTLINE;
			if ( m_nAlign == TEXT_ALIGN_BOTH_X )
			{				
				NS_UITEXTUTIL::ClearWordText ();
				NS_UITEXTUTIL::STRINGVECTOR& WordText = NS_UITEXTUTIL::GetWordText ( MLText[i].strLine );

				for ( int k = 0; k < (int)WordText.size(); ++k )
				{
					STEXTWORD sTextWord;

					sTextWord.strWord = WordText[k];
					sTextWord.dwColor = TextColor;

					if ( WordText[k].GetLength () )
					{
						m_pFont->GetTextExtent ( WordText[k].GetString(), strSize );
						m_pFont->PushText( WordText[k].GetString() );
					}
					else
					{
						strSize.cx = 0;
						strSize.cy = BLANKSIZE.cy;
					}

					sTextWord.strSize = strSize;
					sTextLine.vectorWord.push_back ( sTextWord );
				}

				m_nTotalLine++;
				sTextPart.dequeLine.push_back ( sTextLine );
			}
			else
			{
				STEXTWORD sTextWord;
				sTextWord.strWord = MLText[i].strLine;
				sTextWord.dwColor = TextColor;

				sTextWord.strSize = strSize;

				if( MLText[i].strLine.GetLength() )
					m_pFont->PushText( MLText[i].strLine.GetString() );

				sTextLine.vectorWord.push_back ( sTextWord );
				sTextPart.dequeLine.push_back ( sTextLine );

				m_nTotalLine++;
			}
		}
	}	

	m_TextBox.push_back ( sTextPart );

	//	제한 라인 범위가 넘을 경우, 잘라냄
	for ( ; m_nLimitLine < m_nTotalLine; --m_nTotalLine )
	{		
		STEXTPART& sDelTextPart = m_TextBox.front ();
		DEQUETEXTLINE& dequeLine = sDelTextPart.dequeLine;

		dequeLine.pop_front ();

		//	만약, 문단에 실제 글이 없을 경우,
		//	문단을 날린다.
		if ( dequeLine.size () < 1 )
		{
			if ( m_ImageList.size () )
			{
				DeleteControl ( m_ImageList[0], 0 );
				m_ImageList.pop_front ();
			}
			m_TextBox.pop_front ();
		}
	}

	ReCalcRenderPos ();

	return (int)m_TextBox.size () - 1;
}

int CBasicTextBox::AddMultiTextNoSplit ( STEXTMULTIWORDVEC &vecMultiWord )
{
	if ( !m_pFont )
	{
		GASSERT ( 0 && "폰트가 널입니다. SetFont()를 이용하여 폰트부터 값을 넣으십시오." );
		return 0;
	}

	if ( vecMultiWord.empty() )	return 0;

	STEXTPART sTEXTPART;
	STEXTLINE sTEXTLINE;
	sTEXTLINE.strSize.cx = 0;
	sTEXTLINE.strSize.cy = 0; 

	for ( int i=0; i<(int)vecMultiWord.size(); ++i )
	{
		STEXTMULTIWORD& sText = vecMultiWord[i];

		if ( sText.strWord.GetLength() )
		{
			STEXTWORD sTEXTWORD;
			sTEXTWORD.strWord = sText.strWord;
			sTEXTWORD.dwColor = sText.dwColor;
			m_pFont->GetTextExtent ( sTEXTWORD.strWord.GetString(), sTEXTWORD.strSize );
			m_pFont->PushText( sTEXTWORD.strWord.GetString() );

			sTEXTLINE.vectorWord.push_back ( sTEXTWORD );
			sTEXTLINE.strSize.cx += sTEXTWORD.strSize.cx;

			if ( sTEXTWORD.strSize.cy > sTEXTLINE.strSize.cy )
				sTEXTLINE.strSize.cy = sTEXTWORD.strSize.cy;
		}
	}
	
	sTEXTPART.dequeLine.push_back ( sTEXTLINE );
	m_TextBox.push_back ( sTEXTPART );

	m_nTotalLine++;

	//	제한 라인 범위가 넘을 경우, 잘라냄
	for ( ; m_nLimitLine < m_nTotalLine; --m_nTotalLine )
	{		
		sTEXTPART = m_TextBox.front ();
		DEQUETEXTLINE& dequeLine = sTEXTPART.dequeLine;

		dequeLine.pop_front ();

		//	만약, 문단에 실제 글이 없을 경우,
		//	문단을 날린다.
		if ( dequeLine.size () < 1 )
		{
			if ( m_ImageList.size () )
			{
				DeleteControl ( m_ImageList[0], 0 );
				m_ImageList.pop_front ();
			}
			m_TextBox.pop_front ();
		}
	}

	ReCalcRenderPos ();

	return (int)m_TextBox.size () - 1;
}

int	CBasicTextBox::AddStringNoSplit ( int nIndex, const CString& strText, const D3DCOLOR& TextColor )
{
	if ( !m_pFont )
	{
		GASSERT ( 0 && "폰트가 널입니다. SetFont()를 이용하여 폰트부터 값을 넣으십시오." );
		return -1;
	}

	if ( (int) m_TextBox.size () <= nIndex )
	{
		GASSERT ( 0 && "인덱스가 범위를 넘어섭니다." );
		return -1;
	}

	if ( !strText.GetLength () )
	{
		GASSERT ( 0 && "문자열이 없습니다." );
		return -1;
	}

	STEXTPART& sTextPart = m_TextBox[nIndex];	

	{
		STEXTLINE& sTextLine = sTextPart.dequeLine.back ();

		{
			STEXTWORD sTextWord;
			sTextWord.strWord = strText;
			sTextWord.dwColor = TextColor;
			m_pFont->GetTextExtent ( sTextWord.strWord.GetString(), sTextWord.strSize );

			if( strText.GetLength() )
				m_pFont->PushText( strText.GetString() );

			sTextLine.vectorWord.push_back ( sTextWord );
			sTextLine.strSize.cx += sTextWord.strSize.cx;
			sTextLine.strSize.cy = sTextWord.strSize.cy;
		}		
	}

	for ( ; m_nLimitLine < m_nTotalLine; --m_nTotalLine )
	{		
		sTextPart = m_TextBox.front ();
		DEQUETEXTLINE& dequeLine = sTextPart.dequeLine;

		dequeLine.pop_front ();

		if ( dequeLine.size () < 1 )
		{
			if ( m_ImageList.size () )
			{
				DeleteControl ( m_ImageList[0], 0 );
				m_ImageList.pop_front ();
			}
			m_TextBox.pop_front ();
		}
	}

	ReCalcRenderPos ();

	return (int) m_TextBox.size () - 1;
}

int	CBasicTextBox::AddMultiText ( MULTITEXT strText )
{
	if ( !m_pFont )
	{
		GASSERT ( 0 && " " );
		return 0;
	}

	STEXTPART sTEXTPART;
	STEXTLINE sTEXTLINE;

	for ( int i = 0; i < 100; i++ )
	{
		if ( strText.strText[i] == "" ) continue;

		if ( strText.strText[i] )
		{
			STEXTWORD sTEXTWORD;
			sTEXTWORD.strWord = strText.strText[i];
			sTEXTWORD.dwColor = strText.dwColor[i];
			m_pFont->GetTextExtent ( sTEXTWORD.strWord.GetString(), sTEXTWORD.strSize );

			if( strText.strText[i].GetLength() ) m_pFont->PushText( strText.strText[i].GetString() );

			sTEXTLINE.vectorWord.push_back ( sTEXTWORD );
			sTEXTLINE.strSize.cx += sTEXTWORD.strSize.cx; 
			sTEXTLINE.strSize.cy = sTEXTWORD.strSize.cy;
		}
	}

	sTEXTPART.dequeLine.push_back ( sTEXTLINE );
	m_TextBox.push_back ( sTEXTPART );
	m_nTotalLine++;

	for ( ; m_nLimitLine < m_nTotalLine; --m_nTotalLine )
	{		
		sTEXTPART = m_TextBox.front ();
		DEQUETEXTLINE& dequeLine = sTEXTPART.dequeLine;

		dequeLine.pop_front ();

		if ( dequeLine.size () < 1 )
		{
			if ( m_ImageList.size () )
			{
				DeleteControl ( m_ImageList[0], 0 );
				m_ImageList.pop_front ();
			}
			m_TextBox.pop_front ();
		}
	}
    
	ReCalcRenderPos ();

	return (int)m_TextBox.size () - 1;
}