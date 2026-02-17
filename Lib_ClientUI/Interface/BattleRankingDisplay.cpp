#include "stdafx.h"
#include "BattleRankingDisplay.h"

#include "./BasicLineBox.h"
#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"
#include "./BasicTextBox.h"

#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "./BasicTextButton.h"
#include "./BasicButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBattleRankingDisplay::CBattleRankingDisplay()
: m_pText_Title1(NULL)
, m_pRankButtonPlayer(NULL)
, m_pRankButtonClub(NULL)
{
	memset(m_pLBox_,		NULL, sizeof(m_pLBox_));
	memset(m_pText_Title2_,	NULL, sizeof(m_pText_Title2_));
	memset(m_pText_Rank_,	NULL, sizeof(m_pText_Rank_));
	memset(m_pText_Name_,	NULL, sizeof(m_pText_Name_));
	memset(m_pText_KD_,		NULL, sizeof(m_pText_KD_));
}

CBattleRankingDisplay::~CBattleRankingDisplay()
{
}

void CBattleRankingDisplay::CreateSubControl()
{
	for (int i = 0; i < nMAX_LBOX; i++)
	{
		m_pLBox_[i] = new CBasicLineBox;
		m_pLBox_[i]->CreateSub(this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pLBox_[i]->CreateBaseBoxWhiteBlankBody ("BASIC_LINE_BOX_WHITE");
		RegisterControl(m_pLBox_[i]);
	}

	CD3DFontPar* pFont8		= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8,		_DEFAULT_FONT_SHADOW_FLAG);
	CD3DFontPar* pFont10	= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 10,	_DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignMid		= TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;

	m_pText_Title1 = new CBasicTextBox;
	m_pText_Title1 = CreateStaticControl("BASIC_WINDOW", pFont10, nAlignMid);

	for (int i = 0; i < nMAX_TEXT_TITLE2; i++)
		m_pText_Title2_[i] = CreateStaticControl("BASIC_WINDOW", pFont8, nAlignMid);

	for (int i = 0; i < nMAX_TEXT_RANK; i++)
	{
		m_pText_Rank_[i]	= CreateStaticControl("BASIC_WINDOW", pFont8, nAlignMid);
		m_pText_Name_[i]	= CreateStaticControl("BASIC_WINDOW", pFont8, nAlignMid);
		m_pText_KD_[i]		= CreateStaticControl("BASIC_WINDOW", pFont8, nAlignMid);
	}

	DefaultPos();

	for (int i = 0; i < nMAX_TEXT_TITLE2; i++)
		m_pText_Title2_[i]->SetOneLineText((char*)ID2GAMEWORD("BATTLE_RANKING_LABEL", i), NS_UITEXTCOLOR::LIGHTSKYBLUE);

	m_pRankButtonPlayer = CreateTextButton23 ( "BATTLE_RANKING_BUTTON_PLAYER", BUTTON_RANK_PLAYER,  (char*)ID2GAMEWORD ( "LG7_RANK_BUTTON", 0 ) );
	m_pRankButtonClub = CreateTextButton23 ( "BATTLE_RANKING_BUTTON_CLUB", BUTTON_RANK_CLUB,  (char*)ID2GAMEWORD ( "LG7_RANK_BUTTON", 1 ) );

	m_pRankButtonClub->SetFlip(FALSE);
}

HRESULT CBattleRankingDisplay::RestoreDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	HRESULT hr = CUIGroup::RestoreDeviceObjects(pd3dDevice);
	if (FAILED(hr))
		return hr;

	const LONG lResolution	= CInnerInterface::GetInstance().GetResolution();
	WORD X_RES				= HIWORD(lResolution);
	WORD Y_RES				= LOWORD(lResolution);
	const UIRECT& rcWindow	= GetGlobalPos();

	D3DXVECTOR2 vPos;
	vPos.x = X_RES - rcWindow.sizeX - 5.0f;
	vPos.y = (Y_RES - rcWindow.sizeY) / 2.0f;
	SetGlobalPos(vPos);

	return S_OK;
}

CBasicTextBox* CBattleRankingDisplay::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CBattleRankingDisplay::DefaultPos()
{
	UIRECT RcOLD;
	UIRECT RcNEW1;

	// m_pLBox_
	{
		// m_pLBox_[0]
		{
			RcOLD = m_pLBox_[0]->GetLocalPos();
			RcNEW1 = UIRECT(
				0.0f,
				0.0f,
				200.0f,
				18.0f
				);
			m_pLBox_[0]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[0]->SetLocalPos(RcNEW1);
			m_pLBox_[0]->SetGlobalPos(RcNEW1);
		}

		// m_pLBox_[1]
		{
			RcOLD = m_pLBox_[1]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[0]->GetLocalPos().left,
				m_pLBox_[0]->GetLocalPos().bottom - 1.0f,
				m_pLBox_[0]->GetLocalPos().sizeX,
				m_pLBox_[0]->GetLocalPos().sizeY
				);
			m_pLBox_[1]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[1]->SetLocalPos(RcNEW1);
			m_pLBox_[1]->SetGlobalPos(RcNEW1);
		}

		// m_pLBox_[2]
		{
			RcOLD = m_pLBox_[2]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[1]->GetLocalPos().left,
				m_pLBox_[1]->GetLocalPos().top,
				40.0f,
				100.0f
				);
			m_pLBox_[2]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[2]->SetLocalPos(RcNEW1);
			m_pLBox_[2]->SetGlobalPos(RcNEW1);
		}

		// m_pLBox_[3]
		{
			RcOLD = m_pLBox_[3]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[2]->GetLocalPos().right - 1.0f,
				m_pLBox_[2]->GetLocalPos().top,
				100.0f,
				m_pLBox_[2]->GetLocalPos().sizeY
				);
			m_pLBox_[3]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[3]->SetLocalPos(RcNEW1);
			m_pLBox_[3]->SetGlobalPos(RcNEW1);
		}

		// m_pLBox_[4]
		{
			RcOLD = m_pLBox_[4]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[3]->GetLocalPos().right - 1.0f,
				m_pLBox_[3]->GetLocalPos().top,
				60.0f + 2.0f,
				m_pLBox_[3]->GetLocalPos().sizeY
				);
			m_pLBox_[4]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[4]->SetLocalPos(RcNEW1);
			m_pLBox_[4]->SetGlobalPos(RcNEW1);
		}

		// m_pLBox_[5]
		{
			RcOLD = m_pLBox_[5]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[2]->GetLocalPos().left,
				m_pLBox_[2]->GetLocalPos().bottom - m_pLBox_[1]->GetLocalPos().sizeY,
				m_pLBox_[1]->GetLocalPos().sizeX,
				m_pLBox_[1]->GetLocalPos().sizeY
				);
			m_pLBox_[5]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[5]->SetLocalPos(RcNEW1);
			m_pLBox_[5]->SetGlobalPos(RcNEW1);
		}
	}

	// m_pText_Title1
	{
		RcOLD = m_pText_Title1->GetLocalPos();
		RcNEW1 = UIRECT(
			m_pLBox_[0]->GetLocalPos().left,
			m_pLBox_[0]->GetLocalPos().top,
			m_pLBox_[0]->GetLocalPos().sizeX,
			m_pLBox_[0]->GetLocalPos().sizeY - 2.0f
			);
		m_pText_Title1->AlignSubControl(RcOLD, RcNEW1);
		m_pText_Title1->SetLocalPos(RcNEW1);
	}

	// m_pText_Title2_
	{
		// m_pText_Title2_[0]
		{
			RcOLD = m_pText_Title2_[0]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[2]->GetLocalPos().left,
				m_pLBox_[2]->GetLocalPos().top,
				m_pLBox_[2]->GetLocalPos().sizeX,
				m_pLBox_[1]->GetLocalPos().sizeY - 2.0f
				);
			m_pText_Title2_[0]->AlignSubControl(RcOLD, RcNEW1);
			m_pText_Title2_[0]->SetLocalPos(RcNEW1);
		}

		// m_pText_Title2_[1]
		{
			RcOLD = m_pText_Title2_[1]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[3]->GetLocalPos().left,
				m_pLBox_[3]->GetLocalPos().top,
				m_pLBox_[3]->GetLocalPos().sizeX,
				m_pLBox_[1]->GetLocalPos().sizeY - 2.0f
				);
			m_pText_Title2_[1]->AlignSubControl(RcOLD, RcNEW1);
			m_pText_Title2_[1]->SetLocalPos(RcNEW1);
		}

		// m_pText_Title2_[2]
		{
			RcOLD = m_pText_Title2_[2]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[4]->GetLocalPos().left,
				m_pLBox_[4]->GetLocalPos().top,
				m_pLBox_[4]->GetLocalPos().sizeX,
				m_pLBox_[1]->GetLocalPos().sizeY - 2.0f
				);
			m_pText_Title2_[2]->AlignSubControl(RcOLD, RcNEW1);
			m_pText_Title2_[2]->SetLocalPos(RcNEW1);
		}
	}

	// m_pText_Rank_
	{
		// m_pText_Rank_[0]
		{
			RcOLD = m_pText_Rank_[0]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[2]->GetLocalPos().left,
				m_pLBox_[1]->GetLocalPos().bottom,
				m_pLBox_[2]->GetLocalPos().sizeX,
				16.0f
				);
			m_pText_Rank_[0]->AlignSubControl(RcOLD, RcNEW1);
			m_pText_Rank_[0]->SetLocalPos(RcNEW1);
		}

		// m_pText_Rank_
		for (int i = 1; i < nMAX_TEXT_RANK; i++)
		{
			RcOLD = m_pText_Rank_[i]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pText_Rank_[i - 1]->GetLocalPos().left,
				m_pText_Rank_[i - 1]->GetLocalPos().bottom,
				m_pText_Rank_[i - 1]->GetLocalPos().sizeX,
				m_pText_Rank_[i - 1]->GetLocalPos().sizeY
				);
			m_pText_Rank_[i]->AlignSubControl(RcOLD, RcNEW1);
			m_pText_Rank_[i]->SetLocalPos(RcNEW1);
		}
	}

	// m_pLBox_
	{
		// m_pLBox_[2]
		{
			RcOLD = m_pLBox_[2]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[2]->GetLocalPos().left,
				m_pLBox_[2]->GetLocalPos().top,
				m_pLBox_[2]->GetLocalPos().sizeX,
				m_pText_Rank_[nMAX_TEXT_RANK - 1]->GetLocalPos().top + 2.0f
				);
			m_pLBox_[2]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[2]->SetLocalPos(RcNEW1);
			m_pLBox_[2]->SetGlobalPos(RcNEW1);
		}

		// m_pLBox_[3]
		{
			RcOLD = m_pLBox_[3]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[3]->GetLocalPos().left,
				m_pLBox_[3]->GetLocalPos().top,
				m_pLBox_[3]->GetLocalPos().sizeX,
				m_pLBox_[2]->GetLocalPos().sizeY
				);
			m_pLBox_[3]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[3]->SetLocalPos(RcNEW1);
			m_pLBox_[3]->SetGlobalPos(RcNEW1);
		}

		// m_pLBox_[4]
		{
			RcOLD = m_pLBox_[4]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[4]->GetLocalPos().left,
				m_pLBox_[4]->GetLocalPos().top,
				m_pLBox_[4]->GetLocalPos().sizeX,
				m_pLBox_[3]->GetLocalPos().sizeY
				);
			m_pLBox_[4]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[4]->SetLocalPos(RcNEW1);
			m_pLBox_[4]->SetGlobalPos(RcNEW1);
		}

		// m_pLBox_[5]
		{
			RcOLD = m_pLBox_[5]->GetLocalPos();
			RcNEW1 = UIRECT(
				m_pLBox_[2]->GetLocalPos().left,
				m_pLBox_[2]->GetLocalPos().bottom - m_pLBox_[1]->GetLocalPos().sizeY,
				m_pLBox_[1]->GetLocalPos().sizeX,
				m_pLBox_[1]->GetLocalPos().sizeY
				);
			m_pLBox_[5]->AlignSubControl(RcOLD, RcNEW1);
			m_pLBox_[5]->SetLocalPos(RcNEW1);
			m_pLBox_[5]->SetGlobalPos(RcNEW1);
		}
	}

	// m_pText_Name_
	for(int i = 0; i < nMAX_TEXT_RANK; i++)
	{
		RcOLD = m_pText_Name_[i]->GetLocalPos();
		RcNEW1 = UIRECT(
			m_pLBox_[3]->GetLocalPos().left,
			m_pText_Rank_[i]->GetLocalPos().top,
			m_pLBox_[3]->GetLocalPos().sizeX,
			m_pText_Rank_[i]->GetLocalPos().sizeY
			);
		m_pText_Name_[i]->AlignSubControl(RcOLD, RcNEW1);
		m_pText_Name_[i]->SetLocalPos(RcNEW1);
	}

	// m_pText_KD_
	for (int i = 0; i < nMAX_TEXT_RANK; i++)
	{
		RcOLD = m_pText_KD_[i]->GetLocalPos();
		RcNEW1 = UIRECT(
			m_pLBox_[4]->GetLocalPos().left,
			m_pText_Rank_[i]->GetLocalPos().top,
			m_pLBox_[4]->GetLocalPos().sizeX,
			m_pText_Rank_[i]->GetLocalPos().sizeY
			);
		m_pText_KD_[i]->AlignSubControl(RcOLD, RcNEW1);
		m_pText_KD_[i]->SetLocalPos(RcNEW1);
	}

	{
		RcOLD = GetLocalPos();
		RcNEW1 = UIRECT(
			0.0f,
			0.0f,
			m_pLBox_[0]->GetLocalPos().sizeX,
			m_pLBox_[2]->GetLocalPos().bottom
			);
		SetLocalPos(RcNEW1);
		SetGlobalPos(RcNEW1);

		const LONG lResolution	= CInnerInterface::GetInstance().GetResolution();
		WORD X_RES				= HIWORD(lResolution);
		WORD Y_RES				= LOWORD(lResolution);
		const UIRECT& rcWindow	= GetGlobalPos();

		D3DXVECTOR2 vPos;
		vPos.x = X_RES - rcWindow.sizeX - 5.0f;
		vPos.y = (Y_RES - rcWindow.sizeY) / 2.0f;
		SetGlobalPos(vPos);
	}
}

void CBattleRankingDisplay::ResetAll()
{
	m_pText_Title1->ClearText();

	for (int i = 0; i < nMAX_TEXT_RANK; i++)
	{
		m_pText_Rank_[i]->ClearText();
		m_pText_Name_[i]->ClearText();
		m_pText_KD_[i]->ClearText();
	}
}

void CBattleRankingDisplay::SetBattleTitle(CString strTitle)
{
	m_pText_Title1->SetOneLineText(strTitle, NS_UITEXTCOLOR::LIGHTSKYBLUE);
}

void CBattleRankingDisplay::AddBattleRanking(SBATTLE_RANKING sCWRanking)
{
	CString strRank;
	strRank.Format("%d", sCWRanking.nIndex + 1);
	m_pText_Rank_[sCWRanking.nIndex]->SetOneLineText(strRank, NS_UITEXTCOLOR::LIGHTSKYBLUE);

	m_pText_Name_[sCWRanking.nIndex]->SetOneLineText(sCWRanking.szChaName, NS_UITEXTCOLOR::LIGHTSKYBLUE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[1].Format("%d ", sCWRanking.wKill);
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[2] = ": ";
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::LIGHTSKYBLUE;
	ml_Text.strText[3].Format("%d ", sCWRanking.wDead);
	ml_Text.dwColor[3] = NS_UITEXTCOLOR::RED;

	m_pText_KD_[sCWRanking.nIndex]->ClearText();
	m_pText_KD_[sCWRanking.nIndex]->AddMultiText(ml_Text);
}

void CBattleRankingDisplay::AddMyBattleRanking(SBATTLE_RANKING sCWRanking)
{
	CString strRank;
	strRank.Format("%d", sCWRanking.nIndex + 1);
	m_pText_Rank_[nMAX_TEXT_RANK - 1]->SetOneLineText(strRank, NS_UITEXTCOLOR::LIGHTSKYBLUE);
	m_pText_Name_[nMAX_TEXT_RANK - 1]->SetOneLineText(sCWRanking.szChaName, NS_UITEXTCOLOR::LIGHTSKYBLUE);

	CBasicTextBox::MULTITEXT ml_Text;
	ml_Text.strText[1].Format("%d ", sCWRanking.wKill);
	ml_Text.dwColor[1] = NS_UITEXTCOLOR::PALEGREEN;
	ml_Text.strText[2] = ": ";
	ml_Text.dwColor[2] = NS_UITEXTCOLOR::LIGHTSKYBLUE;
	ml_Text.strText[3].Format("%d ", sCWRanking.wDead);
	ml_Text.dwColor[3] = NS_UITEXTCOLOR::RED;

	m_pText_KD_[nMAX_TEXT_RANK - 1]->ClearText();
	m_pText_KD_[nMAX_TEXT_RANK - 1]->AddMultiText(ml_Text);
}

CBasicTextButton*  CBattleRankingDisplay::CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE23;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButtonEx ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText, _DEFAULT_FONT_SHADOW_FLAG, NS_UITEXTCOLOR::WHITE );
	RegisterControl ( pTextButton );
	return pTextButton;
}

void CBattleRankingDisplay::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case BUTTON_RANK_PLAYER:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_pRankButtonPlayer->SetFlip(TRUE);
				m_pRankButtonClub->SetFlip(FALSE);

				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if (pCharacter)	pCharacter->ReqBattleRanking();
				ResetAll();
			}
		}
		break;
	case BUTTON_RANK_CLUB:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_pRankButtonPlayer->SetFlip(FALSE);
				m_pRankButtonClub->SetFlip(TRUE);

				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if (pCharacter)	pCharacter->ReqBattleRanking();
				ResetAll();
			}
		}
		break;
	}
}

void CBattleRankingDisplay::RefreshBattleRank()
{
	ResetAll();

	int nRankNum = GLGaeaClient::GetInstance().m_vecBattleRankingP.size();
	VEC_BATTLE_RANKING vecRanking = GLGaeaClient::GetInstance().m_vecBattleRankingP;

	if( m_pRankButtonClub->IsFlip() )
	{
		nRankNum = 0;
		vecRanking.clear();

		nRankNum = GLGaeaClient::GetInstance().m_vecBattleRankingC.size();
		vecRanking = GLGaeaClient::GetInstance().m_vecBattleRankingC;
	}

	for ( int i=0; i<nRankNum; ++i )
	{
		if ( i >= RANKING_NUM )	break;
		AddBattleRanking( vecRanking[i] );
	}
}

int CBattleRankingDisplay::GetCurrentTabPage()
{
	if( m_pRankButtonPlayer->IsFlip() )	return 1;
	if( m_pRankButtonClub->IsFlip() )	return 2;

	return 0;
}