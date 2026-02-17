#include "StdAfx.h"
#include "TyrannyWarRankingDisplayGuildSlot.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "BasicLineBox.h"
#include "BasicTextBox.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_Client/G-Logic/GLCrowData.h"
#include "../Lib_Client/G-Logic/GLCrow.h"
#include "../Lib_Client/G-Logic/GLCrowClient.h"
//#include "../Lib_Client/G-Logic/GlogicEx.h"
#include "../Lib_Client/G-Logic/GLCharLogicServer.h"
#include "../Lib_Client/G-Logic/GLCharLogicClient.h"
#include "../Lib_Engine/DxCommon/DxClubMan.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_Client/DxGlobalStage.h"
#include "../Lib_Client/G-Logic/GLCharData.h"
#include "../Lib_Client/G-Logic/GLCharacter.h"
#include "../Lib_Client/G-Logic/GLChar.h"
#include "../Lib_Client/G-Logic/GLCharClient.h"
#include "../Lib_Client/G-Logic/GLogicData.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "UIInfoLoader.h"
#include "../Lib_Client/G-Logic/glcrowrenlist.h"
#include "../Lib_Client/G-Logic/GLStringTable.h"

 /*pvp tyranny, Juver, 2017/08/24 */
#include "../Lib_Client/G-Logic/GLPVPTyrannyField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCTyrannyWarRankingDisplayGuildSlot::CCTyrannyWarRankingDisplayGuildSlot ()
	: m_pClubRankTextSlot(NULL)
	, m_pClubNameTextSlot(NULL)
	, m_pClubScoreTextSlotKill(NULL)
	, m_pClubScoreTextSlotDeath(NULL)
	, m_pGuildImage(NULL)
	, m_pKillIcon( NULL )
	, m_pDeathIcon( NULL )
	, m_pLineBox( NULL )
{
}

CCTyrannyWarRankingDisplayGuildSlot::~CCTyrannyWarRankingDisplayGuildSlot ()
{
}

void CCTyrannyWarRankingDisplayGuildSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBox = new CBasicLineBox;
	m_pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBox->CreateBaseBoxQuestList ( "TYRANNY_RANKING_LINEBOX_GUILD_WHITE_SELF_SLOT" );
	m_pLineBox->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBox );

	m_pLineBoxWhite = new CBasicLineBox;
	m_pLineBoxWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxWhite->CreateBaseBoxWhiteBlankBody ( "TYRANNY_RANKING_LINEBOX_GUILD_WHITE_SLOTRANK" );
	m_pLineBoxWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxWhite );

	m_pLineBoxRankWhite = new CBasicLineBox;
	m_pLineBoxRankWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxRankWhite->CreateBaseBoxWhiteBlankBody ( "TYRANNY_RANKING_LINEBOX_GUILD_WHITE_RANKNUMBER" );
	m_pLineBoxRankWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxRankWhite );

	m_pLineBoxCharacterDetailsWhite = new CBasicLineBox;
	m_pLineBoxCharacterDetailsWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxCharacterDetailsWhite->CreateBaseBoxWhiteBlankBody ( "TYRANNY_RANKING_LINEBOX_GUILD_WHITE_CHARACTER_DETAILS" );
	m_pLineBoxCharacterDetailsWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxCharacterDetailsWhite );

	m_pLineBoxCharacterNameWhite = new CBasicLineBox;
	m_pLineBoxCharacterNameWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxCharacterNameWhite->CreateBaseBoxWhiteBlankBody ( "TYRANNY_RANKING_LINEBOX_GUILD_WHITE_CHARACTER_CHARACTER_NAME" );
	m_pLineBoxCharacterNameWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxCharacterNameWhite );

	m_pClubRankTextSlot = new CBasicTextBox;
	m_pClubRankTextSlot->CreateSub ( this, "TYRANNY_RANKING_TEXT_BASE_RANK_GUILD_SLOT" );
	m_pClubRankTextSlot->SetFont ( pFont8 );
	m_pClubRankTextSlot->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pClubRankTextSlot->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pClubRankTextSlot );

	m_pClubNameTextSlot = new CBasicTextBox;
	m_pClubNameTextSlot->CreateSub ( this, "TYRANNY_RANKING_TEXT_BASE_GUILD_NAME_SLOT" );
	m_pClubNameTextSlot->SetFont ( pFont8 );
	m_pClubNameTextSlot->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pClubNameTextSlot->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pClubNameTextSlot );

	m_pClubScoreTextSlotKill = new CBasicTextBox;
	m_pClubScoreTextSlotKill->CreateSub ( this, "TYRANNY_RANKING_TEXT_BASE_GUILD_KILLSCORE_RESULT" );
	m_pClubScoreTextSlotKill->SetFont ( pFont8 );
	m_pClubScoreTextSlotKill->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pClubScoreTextSlotKill->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pClubScoreTextSlotKill );

	m_pClubScoreTextSlotDeath = new CBasicTextBox;
	m_pClubScoreTextSlotDeath->CreateSub ( this, "TYRANNY_RANKING_TEXT_BASE_GUILD_DEATHSCORE_RESULT" );
	m_pClubScoreTextSlotDeath->SetFont ( pFont8 );
	m_pClubScoreTextSlotDeath->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pClubScoreTextSlotDeath->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pClubScoreTextSlotDeath );


	m_pKillIcon = new CUIControl;
	m_pKillIcon->CreateSub ( this, "TYRANNY_RANKING_GUILD_SLOT_IMAGES_KILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pKillIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pKillIcon );

	m_pDeathIcon = new CUIControl;
	m_pDeathIcon->CreateSub ( this, "TYRANNY_RANKING_GUILD_SLOT_IMAGES_DEATH", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pDeathIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pDeathIcon );

	m_pGuildImage = new CUIControl;
	m_pGuildImage->CreateSub(this, "TYRANNY_RANKING_GUILD_CLUB_MARK", UI_FLAG_DEFAULT, GUILD_DUMMY);
	RegisterControl(m_pGuildImage);

	UIRECT RcOLD;
	UIRECT RcNEW1;


	// m_pGuildImage
	{
		RcNEW1 = UIRECT(40.00, 26.00, 17.00, 17.00);
		RcOLD = m_pGuildImage->GetLocalPos();
		m_pGuildImage->AlignSubControl(RcOLD, RcNEW1);
		m_pGuildImage->SetLocalPos(RcNEW1);
		m_pGuildImage->SetVisibleSingle(FALSE);
	}
}

CUIControl* CCTyrannyWarRankingDisplayGuildSlot::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	return pControl;
}

void CCTyrannyWarRankingDisplayGuildSlot::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage( cID, dwMsg );
}

void CCTyrannyWarRankingDisplayGuildSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

HRESULT CCTyrannyWarRankingDisplayGuildSlot::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	GASSERT(pd3dDevice);
	m_pd3dDevice = pd3dDevice;

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects(pd3dDevice);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT CCTyrannyWarRankingDisplayGuildSlot::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
	GASSERT( pd3dDevice );

	if ( !IsVisible ())
	{
		m_pGuildImage->SetVisibleSingle(FALSE);
		return CUIGroup::Render(pd3dDevice);
	}

	HRESULT hr = S_OK;

    m_pGuildImage->SetVisibleSingle(FALSE);
    {
        hr = CUIGroup::Render(pd3dDevice);
        if (FAILED(hr)) return hr;
    }

	/* FIX RSiLENT , ISSUE ABOUT RENDER WHEN VISIBLE THE WINDOW */
    GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
    if (!pChar) return S_OK;

    GLCLUB& sCLUB = pChar->m_sCLUB;
    DWORD dwClubID = pChar->m_dwGuild;

	if (sCLUB.m_dwID == CLUB_NULL)
	{
		int nServer = static_cast<int>(pChar->m_dwServerID);
		int nID = static_cast<int>(sCLUB.m_dwID);
		DWORD dwVer = sCLUB.m_dwMarkVER;

		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData(pd3dDevice, nServer, nID, dwVer);

		m_pGuildImage->SetTexture(sMarkData.pddsTexture);
		m_pGuildImage->SetVisibleSingle(TRUE);

		hr = m_pGuildImage->Render(pd3dDevice);
		if (FAILED(hr)) return hr;
	}
	else
	{
		int nServer = static_cast<int>(pChar->m_dwServerID);
		int nID = static_cast<int>(sCLUB.m_dwID);
		DWORD dwVer = sCLUB.m_dwMarkVER;

		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData(pd3dDevice, nServer, nID, dwVer);

		m_pGuildImage->SetTexture(sMarkData.pddsTexture);
		m_pGuildImage->SetVisibleSingle(TRUE);

		hr = m_pGuildImage->Render(pd3dDevice);
		if (FAILED(hr)) return hr;
	}

    return S_OK;
}

void CCTyrannyWarRankingDisplayGuildSlot::ResetData()
{
	if ( m_pClubRankTextSlot )			m_pClubRankTextSlot->ClearText();
	if ( m_pClubNameTextSlot )			m_pClubNameTextSlot->ClearText();
	if ( m_pClubScoreTextSlotKill )		m_pClubScoreTextSlotKill->ClearText();
	if ( m_pClubScoreTextSlotDeath )	m_pClubScoreTextSlotDeath->ClearText();
	if ( m_pGuildImage )				m_pGuildImage->SetVisibleSingle(FALSE);
	if ( m_pKillIcon )					m_pKillIcon->SetVisibleSingle(FALSE);
	if ( m_pDeathIcon )					m_pDeathIcon->SetVisibleSingle(FALSE);
}

void CCTyrannyWarRankingDisplayGuildSlot::SetData( int nRank, std::string strClub, WORD wKill, WORD wDeath, WORD wGuNum, WORD wGuMarkVer )
{
	ResetData();

	if ( wGuNum > 0 && nRank > 0 )
	{
		const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		
		const DxClubMan::DXDATA& sMarkDataKR	= DxClubMan::GetInstance().GetClubData(DxGlobalStage::GetInstance().GetD3dDevice (), nSERVER, wGuNum, wGuMarkVer);
		m_pGuildImage->SetTexturePos(0, sMarkDataKR.vTex_1_LU);
		m_pGuildImage->SetTexturePos(1, sMarkDataKR.vTex_2_RU);
		m_pGuildImage->SetTexturePos(3, sMarkDataKR.vTex_3_LD);
		m_pGuildImage->SetTexturePos(2, sMarkDataKR.vTex_4_RD);

		const UIRECT& rcGlobalPosKR = m_pGuildImage->GetGlobalPos();
		m_pGuildImage->SetGlobalPos(UIRECT(
			ceil(rcGlobalPosKR.left),
			ceil(rcGlobalPosKR.top),
			ceil(rcGlobalPosKR.sizeX),
			ceil(rcGlobalPosKR.sizeY)));
		

		m_pGuildImage->SetTexture(sMarkDataKR.pddsTexture);
		m_pGuildImage->Render(m_pd3dDevice);
		m_pGuildImage->SetVisibleSingle(TRUE);

		// LineBox
		GLCharacter* pClientChar = GLGaeaClient::GetInstance().GetCharacter();
		if (pClientChar && pClientChar->m_dwGuild == wGuNum)
		{
			m_pLineBox->SetUseRender(TRUE);
			m_pLineBox->SetDiffuse(NS_UITEXTCOLOR::GOLD);
		}
		else
		{
			m_pLineBox->SetUseRender(TRUE);
			m_pLineBox->SetDiffuse(NS_UITEXTCOLOR::BLACK);
		}

		if ( m_pKillIcon )		m_pKillIcon->SetVisibleSingle(TRUE);
		if ( m_pDeathIcon )		m_pDeathIcon->SetVisibleSingle(TRUE);


		CString strText;
		if ( m_pClubRankTextSlot )
		{
			strText.Format( "%d",nRank );
			m_pClubRankTextSlot->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
		}

		if ( m_pClubNameTextSlot )
		{
			m_pClubNameTextSlot->AddText( strClub.c_str(), NS_UITEXTCOLOR::WHITE );
		}

		if ( m_pClubScoreTextSlotKill )
		{
			strText.Format( "%u", wKill );
			m_pClubScoreTextSlotKill->AddText( strText.GetString(), NS_UITEXTCOLOR::GREENYELLOW );
		}

		if ( m_pClubScoreTextSlotDeath )
		{
			strText.Format( "%u", wDeath );
			m_pClubScoreTextSlotDeath->AddText( strText.GetString(), NS_UITEXTCOLOR::ORNAGERED );
		}
	}
}