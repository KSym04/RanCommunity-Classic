/**MMR Rank Stats, Mhundz 01/02/2025**/
#include "StdAfx.h"
#include "MMRPage.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"

#include "../BasicLineBox.h"
#include "../BasicTextBox.h"
#include "../BasicTextBoxEx.h"
#include "../BasicScrollBarEx.h"
#include "../BasicScrollThumbFrame.h"
#include "../BasicButton.h"
#include "../BasicProgressBar.h"

#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../InnerInterface.h"
#include "../Lib_Client/G-Logic/GLCharData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMMRPage::CMMRPage ()	
	: m_pListTextBox(NULL)
	, m_nSelectIndex(-1)
	, m_pTextTitle(NULL)
	, m_pTextReportTitle(NULL)
	, m_fActPointUpdate(0.0f)
	, m_dwHPIncr(0)
	, m_dwMPIncr(0)
	, m_dwSPIncr(0)
	, m_dwAttIncr(0)
	, m_dwDefIncr(0)
	, m_dwShootIncr(0)
	, m_dwMeleeIncr(0)
	, m_dwEnerIncr(0)
	, m_dwResistIncr(0)
	, m_dwHRIncr(0)
	, m_dwARIncr(0)
	, m_pMMRRankText(NULL)
	, m_pMMRText(NULL)

{
	for( int i=0; i<4; ++i )
		m_pTextList[i] = NULL;

	memset ( m_pRankMark, 0, sizeof( CUIControl* ) * nRANK_MARK );
}

CMMRPage::~CMMRPage ()
{
}

void CMMRPage::CreateSubControl ()
{
	int nAlign = TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y;
	int nAlignMid = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_MMR_OBJECT_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhite ( "RAN_MMR_OBJECT_LINE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_SPEC_MMR_LINEBOX_RESULT_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_SPEC_MMR_LINEBOX_RESULT_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_SPEC_MMR_LINEBOX_RESULT_CONTENT" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_SPEC_MMR_LINEBOX_RESULT_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_SPEC_MMR_LINEBOX_RESULT_CONTENT" );
	RegisterControl ( pLineBox );
	
	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_SPEC_MMR_LINEBOX_REPORT_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_SPEC_MMR_LINEBOX_REPORT_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_SPEC_MMR_LINEBOX_REPORT_TITLE" );
	RegisterControl ( pLineBox );

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( "RAN_SPEC_MMR_LINEBOX_REPORT_LIST" );
	RegisterControl ( pLineBox );
	
	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxWhiteBlankBody ( "RAN_SPEC_MMR_LINEBOX_REPORT_LIST" );
	RegisterControl ( pLineBox );

	m_pListTextBox = new CBasicTextBoxEx;
	m_pListTextBox->CreateSub ( this, "RAN_SPEC_MMR_REPORT_LIST", UI_FLAG_DEFAULT, MMR_PAGE_LIST_TEXTBOX );
	m_pListTextBox->SetFont ( pFont9 );		
	m_pListTextBox->SetLineInterval ( 3.0f );
	m_pListTextBox->SetSensitive ( true );
	m_pListTextBox->SetLimitLine ( 10000 );
	RegisterControl ( m_pListTextBox );

	int nTotalLine = m_pListTextBox->GetVisibleLine ();

	m_pTextTitle = new CBasicTextBox;
	m_pTextTitle->CreateSub ( this, "RAN_SPEC_MMR_WINDOW_RESULT_TITLE" );
	m_pTextTitle->SetFont ( pFont9 );
	m_pTextTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextTitle->SetText( ID2GAMEWORD( "RAN_SPEC_MMR_WINDOW_RESULT", 1 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextTitle );
	
	m_pTextReportTitle = new CBasicTextBox;
	m_pTextReportTitle->CreateSub ( this, "RAN_SPEC_MMR_WINDOW_REPORT_TITLE" );
	m_pTextReportTitle->SetFont ( pFont9 );
	m_pTextReportTitle->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextReportTitle->SetText( ID2GAMEWORD( "RAN_SPEC_MMR_WINDOW_RESULT", 0 ), NS_UITEXTCOLOR::ORANGE );
	RegisterControl ( m_pTextReportTitle );

	CreateUILineBoxQuestList( "RAN_MDEV_LINEBOX_MMRRANK" );

	m_pMMRProgressBar = new CBasicProgressBar;
	m_pMMRProgressBar->CreateSub ( this, "RAN_MDEV_MMRRANK_PROGRESSBAR" );
	m_pMMRProgressBar->CreateOverImage ( "RAN_MDEV_MMRRANK_PROGRESSBAR_OVERIMAGE" );
	m_pMMRProgressBar->SetControlNameEx ( "MMR" );
	RegisterControl ( m_pMMRProgressBar );

	CreateStaticControl( "RAN_MDEV_WINDOW_MMRRANK_STATIC", pFont9, ID2GAMEWORD("RAN_SPEC_MMR_WINDOW_RESULT", 2), NS_UITEXTCOLOR::WHITE, nAlign );
	CreateStaticControl( "RAN_MDEV_WINDOW_MMR_STATIC", pFont9, ID2GAMEWORD("RAN_SPEC_MMR_WINDOW_RESULT", 3), NS_UITEXTCOLOR::WHITE, nAlign );
	m_pMMRText = CreateText( "RAN_MDEV_WINDOW_MMR_TEXT", pFont9, nAlign );
	m_pMMRTextBar = CreateText( "RAN_MDEV_MMRRANK_PROGRESSBAR_TEXT", pFont9, nAlignMid );
	m_pMMRRankText = CreateText( "RAN_MDEV_WINDOW_MMRRANK_TEXT", pFont9, nAlign );
	
	std::string strtext[11] = 
	{
		"RAN_SPEC_MMR_REPORT_LIST00",
		"RAN_SPEC_MMR_REPORT_LIST01",
		"RAN_SPEC_MMR_REPORT_LIST02",
		"RAN_SPEC_MMR_REPORT_LIST03",
		"RAN_SPEC_MMR_REPORT_LIST04",
		"RAN_SPEC_MMR_REPORT_LIST05",
		"RAN_SPEC_MMR_REPORT_LIST06",
		"RAN_SPEC_MMR_REPORT_LIST07",
		"RAN_SPEC_MMR_REPORT_LIST08",
		"RAN_SPEC_MMR_REPORT_LIST09",
		"RAN_SPEC_MMR_REPORT_LIST10",
	};
	for( int i=0; i<11; ++i )
	{
		m_pTextList[i] = new CBasicTextBox;
		m_pTextList[i]->CreateSub ( this, strtext[i].c_str() );
		m_pTextList[i]->SetFont ( pFont9 );
		m_pTextList[i]->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
		m_pTextList[i]->SetText( "~", NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pTextList[i] );
	}

	{
		CUIControl	TempControl;
		TempControl.Create ( 1, "RAN_MDEV_WINDOW_RANK_MARK" );

		const UIRECT& rcLocalPos = TempControl.GetLocalPos ();	

		for ( int i = 0; i < nRANK_MARK; ++i )
		{
			CString strTemp;
			strTemp.Format ( "RAN_MDEV_ICON_RANK_IMAGE%d", i ); 

			m_pRankMark[i] = new CUIControl;
			m_pRankMark[i]->CreateSub ( this, strTemp.GetString(), UI_FLAG_DEFAULT, RAN_MDEV_ICON_RANK_IMAGE0+i );
			m_pRankMark[i]->SetLocalPos ( rcLocalPos );			
			m_pRankMark[i]->SetVisibleSingle ( FALSE );
			m_pRankMark[i]->SetTransparentOption( TRUE );
			RegisterControl ( m_pRankMark[i] );
		}
	}
}
CBasicLineBox* CMMRPage::CreateUILineBoxQuestList( char* szBaseControl )
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxQuestList ( szBaseControl );
	RegisterControl ( pLineBox );
	return pLineBox;
}
CBasicTextBox* CMMRPage::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, CString strText, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	pStaticText->AddText( strText.GetString(), dwColor );
	RegisterControl ( pStaticText );
	return pStaticText;
}

CBasicTextBox* CMMRPage::CreateText ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}
void CMMRPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
	switch ( ControlID )
	{
	case RAN_MDEV_ICON_RANK_IMAGE0:
	case RAN_MDEV_ICON_RANK_IMAGE1:
	case RAN_MDEV_ICON_RANK_IMAGE2:
	case RAN_MDEV_ICON_RANK_IMAGE3:
	case RAN_MDEV_ICON_RANK_IMAGE4:
	case RAN_MDEV_ICON_RANK_IMAGE5:
	case RAN_MDEV_ICON_RANK_IMAGE6:
	case RAN_MDEV_ICON_RANK_IMAGE7:
	case RAN_MDEV_ICON_RANK_IMAGE8:
	case RAN_MDEV_ICON_RANK_IMAGE9:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				const GLCHARLOGIC_CLIENT& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
				const char* RankName[] = {
					"Beginner", 
					"Trainee", 
					"Amateur", 
					"Hustler", 
					"Veteran",
					"SemiPro", 
					"Professional", 
					"Master", 
					"Legend", 
					"Immortal"
				};

				int rank = -1;
				for (int i = 9; i >= 0; --i)
				{
					if (sCharData.m_dwMMR >= GLCONST_CHAR::dwRank_Point[i])
					{
						rank = i;
						break;
					}
				}

				CString rankText;
				if (rank >= 0 && rank < 10)
				{
					rankText.Format("%s", RankName[rank]);
				}
				CInnerInterface::GetInstance().InfoDisplayShowCommonSingleLine(rankText, NS_UITEXTCOLOR::WHITE);
			}
		}
		break;
	};
}

void CMMRPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListTextBox->SetUseOverColor ( FALSE );
	if ( m_pListTextBox->GetCount () && 0 <= m_nSelectIndex )
		m_pListTextBox->SetUseTextColor ( m_nSelectIndex, FALSE );

	for ( int i = 0 ; i < nRANK_MARK; ++i ) 
		m_pRankMark[i]->SetVisibleSingle(FALSE);

	const GLCHARLOGIC_CLIENT& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();

	m_pRankMark[CalculateVal(sCharData.m_dwMMR)]->SetVisibleSingle(TRUE);
	
	SetMMR ( sCharData );
	SetMMRProgressBar ( sCharData );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	m_fActPointUpdate += fElapsedTime;
	if ( m_fActPointUpdate >= 0.2f )
	{
		m_fActPointUpdate = 0.0f;
		
		m_pListTextBox->SetVisibleSingle(false);

		m_dwHPIncr = sCharData.GETHPINC_MMR();
		m_dwMPIncr = sCharData.GETMPINC_MMR();
		m_dwSPIncr = sCharData.GETSPINC_MMR();
		m_dwAttIncr = sCharData.GETATTINC_MMR();
		m_dwDefIncr = sCharData.GETDEFINC_MMR();
		m_dwShootIncr = sCharData.GETSHOOTINC_MMR();
		m_dwMeleeIncr = sCharData.GETMELEEINC_MMR();
		m_dwEnerIncr = sCharData.GETENERGYINC_MMR();
		m_dwResistIncr = sCharData.GETRESISTINC_MMR();
		m_dwHRIncr = sCharData.GETHITRATEINC_MMR();
		m_dwARIncr = sCharData.GETAVOIDINC_MMR();
		
		CString strText;
		strText.Format( "HP Increase +%d", m_dwHPIncr);
		m_pTextList[0]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "MP Increase +%d", m_dwMPIncr);
		m_pTextList[1]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "SP Increase +%d", m_dwSPIncr);
		m_pTextList[2]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "Attack Increase +%d", m_dwAttIncr);
		m_pTextList[3]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "Defense Increase +%d", m_dwDefIncr);
		m_pTextList[4]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "Shooting Increase +%d", m_dwShootIncr);
		m_pTextList[5]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "Melee Increase +%d", m_dwMeleeIncr);
		m_pTextList[6]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "Energy Increase +%d", m_dwEnerIncr);
		m_pTextList[7]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "Resistance Increase +%d", m_dwResistIncr);
		m_pTextList[8]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "Hitrate Increase +%d", m_dwHRIncr);
		m_pTextList[9]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

		strText.Format( "Evasion Increase +%d", m_dwARIncr);
		m_pTextList[10]->SetText( strText, NS_UITEXTCOLOR::BLUE_WOW );

	}
}

void CMMRPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
		LoadData();
	}
}

void CMMRPage::LoadData()
{
	if ( m_pListTextBox )	m_pListTextBox->ClearText();
	
	const GLCHARLOGIC_CLIENT& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
	if ( m_pListTextBox )
	{
		int nIndex;
		CString strText;

		strText.Format( "HP Increase + %d", m_dwHPIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 0 + 1 );

		strText.Format( "MP Increase + %d", m_dwMPIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 1 + 1 );

		strText.Format( "SP Increase + %d", m_dwSPIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 2 + 1 );
		
		strText.Format( "Attack Increase + %d", m_dwAttIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 3 + 1 );

		strText.Format( "Defense Increase + %d", m_dwDefIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 4 + 1 );

		strText.Format( "Shooting Increase + %d", m_dwShootIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 5 + 1 );

		strText.Format( "Melee Increase + %d", m_dwMeleeIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 6 + 1 );

		strText.Format( "Energy Increase + %d", m_dwEnerIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 7 + 1 );

		strText.Format( "Resistance Increase + %d", m_dwResistIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 8 + 1 );
		
		strText.Format( "Hitrate Increase + %d", m_dwHRIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 9 + 1 );
		
		strText.Format( "Evasion Increase + %d", m_dwARIncr);
		nIndex = m_pListTextBox->AddText ( strText.GetString(), NS_UITEXTCOLOR::BLUE_WOW );
		m_pListTextBox->SetTextData ( nIndex, 10 + 1 );
		
	}
}
void CMMRPage::SetMMR(const GLCHARLOGIC_CLIENT& sCharData)
{
    if (m_pMMRRankText)
    {
        const char* RankName[] = {
            "Beginner", 
			"Trainee", 
			"Amateur", 
			"Hustler", 
			"Veteran",
			"SemiPro", 
			"Professional", 
			"Master", 
			"Legend", 
			"Immortal"
        };

        int rank = -1;
        for (int i = 9; i >= 0; --i)
        {
            if (sCharData.m_dwMMR >= GLCONST_CHAR::dwRank_Point[i])
            {
                rank = i;
                break;
            }
        }

        m_pMMRRankText->ClearText();
        CString rankText;

        if (rank >= 0 && rank < 10)
        {
            rankText.Format("%s", RankName[rank]);
        }
        else
        {
            rankText = "Unranked";
        }

        m_pMMRRankText->SetText(rankText, NS_UITEXTCOLOR::GREENYELLOW);
    }

    if (m_pMMRText)
    {
        m_pMMRText->ClearText();
        CString mmrText = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT(sCharData.m_dwMMR, 3, ",");
        m_pMMRText->SetText(mmrText, NS_UITEXTCOLOR::GREENYELLOW);
    }
}

void CMMRPage::SetMMRProgressBar(const GLCHARLOGIC_CLIENT& sCharData)
{
    DWORD dwCurrent = sCharData.m_dwMMR;
    DWORD dwCurrentMax = GLCONST_CHAR::dwRank_Point[CalculateProgressVal(sCharData.m_dwMMR)];
    if (dwCurrent > dwCurrentMax) dwCurrentMax = dwCurrent;

    if (m_pMMRTextBar)
    {
        m_pMMRTextBar->ClearText();

        CString strCombine;
        strCombine.Format("%d", dwCurrent);
        int nIndex = m_pMMRTextBar->AddTextNoSplit(strCombine, NS_UITEXTCOLOR::WHITE);

        strCombine.Format(" / ");
        m_pMMRTextBar->AddString(nIndex, strCombine, NS_UITEXTCOLOR::LIGHTGREY);

        strCombine.Format("%d", dwCurrentMax);
        m_pMMRTextBar->AddString(nIndex, strCombine, NS_UITEXTCOLOR::WHITE);
    }

    if (m_pMMRProgressBar)
    {
        float fPercent = (dwCurrentMax > 0) ? (float(dwCurrent) / float(dwCurrentMax)) : 0.0f;
        m_pMMRProgressBar->SetPercent(fPercent);
    }
}

int CMMRPage::CalculateVal(DWORD dwValue)
{
    for (int i = 9; i >= 0; --i)
    {
        if (dwValue >= GLCONST_CHAR::dwRank_Point[i])
            return i;
    }
    return 0;
}


int CMMRPage::CalculateProgressVal(DWORD dwValue)
{
    for (int i = 0; i <= 9; ++i)
    {
        if (dwValue < GLCONST_CHAR::dwRank_Point[i])
            return i;
    }
    return 9;
}
