#include "StdAfx.h"
#include "ClubWarRankingDisplayResuScoreSlot.h"
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClubWarRankingDisplayResuScoreSlot::CClubWarRankingDisplayResuScoreSlot ()
	: m_pResuRankTextSlot(NULL)
	, m_pResuNameTextSlot(NULL)
	, m_pResuScoreTextSlot(NULL)
	, m_pGuildImage(NULL)
	, m_pClassImage(NULL)
	, m_pLineBox( NULL )
	, m_pResuIcon( NULL )
{
	for( int i = 0; i < RANK_INFO_ICON_SCHOOL; ++i )
	{
		m_pSchoolIcon[i] = NULL;
	}
}

CClubWarRankingDisplayResuScoreSlot::~CClubWarRankingDisplayResuScoreSlot ()
{
}

void CClubWarRankingDisplayResuScoreSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBox = new CBasicLineBox;
	m_pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBox->CreateBaseBoxQuestList ( "CLUBWAR_RANKING_LINEBOX_RESU_WHITE_SELF_SLOT" );
	m_pLineBox->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBox );

	m_pLineBoxWhite = new CBasicLineBox;
	m_pLineBoxWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxWhite->CreateBaseBoxWhiteBlankBody ( "CLUBWAR_RANKING_LINEBOX_RESU_WHITE_SLOTRANK" );
	m_pLineBoxWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxWhite );

	m_pLineBoxRankWhite = new CBasicLineBox;
	m_pLineBoxRankWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxRankWhite->CreateBaseBoxWhiteBlankBody ( "CLUBWAR_RANKING_LINEBOX_RESU_WHITE_RANKNUMBER" );
	m_pLineBoxRankWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxRankWhite );

	m_pLineBoxCharacterDetailsWhite = new CBasicLineBox;
	m_pLineBoxCharacterDetailsWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxCharacterDetailsWhite->CreateBaseBoxWhiteBlankBody ( "CLUBWAR_RANKING_LINEBOX_RESU_WHITE_CHARACTER_DETAILS" );
	m_pLineBoxCharacterDetailsWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxCharacterDetailsWhite );

	m_pLineBoxCharacterNameWhite = new CBasicLineBox;
	m_pLineBoxCharacterNameWhite->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxCharacterNameWhite->CreateBaseBoxWhiteBlankBody ( "CLUBWAR_RANKING_LINEBOX_RESU_WHITE_CHARACTER_NAME" );
	m_pLineBoxCharacterNameWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxCharacterNameWhite );

	m_pResuRankTextSlot = new CBasicTextBox;
	m_pResuRankTextSlot->CreateSub ( this, "CLUBWAR_RANKING_TEXT_BASE_RANK_GUILD_SLOT" );
	m_pResuRankTextSlot->SetFont ( pFont8 );
	m_pResuRankTextSlot->SetTextAlign ( TEXT_ALIGN_LEFT );
	m_pResuRankTextSlot->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pResuRankTextSlot );

	m_pResuNameTextSlot = new CBasicTextBox;
	m_pResuNameTextSlot->CreateSub ( this, "CLUBWAR_RANKING_TEXT_BASE_CHARACTER_NAME_RESURRECTION_SLOT" );
	m_pResuNameTextSlot->SetFont ( pFont8 );
	m_pResuNameTextSlot->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pResuNameTextSlot->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pResuNameTextSlot );

	m_pResuScoreTextSlot = new CBasicTextBox;
	m_pResuScoreTextSlot->CreateSub ( this, "CLUBWAR_RANKING_TEXT_BASE_RESURRECTION_SCORE_RESULT" );
	m_pResuScoreTextSlot->SetFont ( pFont8 );
	m_pResuScoreTextSlot->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pResuScoreTextSlot->SetText( "--", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pResuScoreTextSlot );

	std::string strSchoolIcon[RANK_INFO_ICON_SCHOOL] = 
	{
		"CLUBWAR_RANKING_RESURRECTION_CHARACTER_SCHOOL_WINDOW_SM",
		"CLUBWAR_RANKING_RESURRECTION_CHARACTER_SCHOOL_WINDOW_HA",
		"CLUBWAR_RANKING_RESURRECTION_CHARACTER_SCHOOL_WINDOW_BH",
	};

	for( int i = 0; i < RANK_INFO_ICON_SCHOOL; ++i )
	{
		m_pSchoolIcon[i] = new CUIControl;
		m_pSchoolIcon[i]->CreateSub ( this, strSchoolIcon[i].c_str() );	
		m_pSchoolIcon[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolIcon[i] );
	}


	m_pResuIcon = new CUIControl;
	m_pResuIcon->CreateSub ( this, "CLUBWAR_RANKING_RESURRECTION_SLOT_IMAGES", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pResuIcon->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pResuIcon );

	m_pClassImage = new CUIControl;
	m_pClassImage->CreateSub(this, "CLUBWAR_RANKING_RESURRECTION_SLOT_CLASS_IMAGE", UI_FLAG_DEFAULT, CLASS_DUMMY);
	RegisterControl(m_pClassImage);


	m_pGuildImage = new CUIControl;
	m_pGuildImage->CreateSub(this, "CLUBWAR_RANKING_RESURRECTION_CLUB_MARK", UI_FLAG_DEFAULT, GUILD_DUMMY);
	RegisterControl(m_pGuildImage);

	UIRECT RcOLD;
	UIRECT RcNEW1;

	// m_pClassImage
	{
		RcNEW1 = UIRECT(40.00, 26.00, 17.00, 17.00);
		RcOLD = m_pClassImage->GetLocalPos();
		m_pClassImage->AlignSubControl(RcOLD, RcNEW1);
		m_pClassImage->SetLocalPos(RcNEW1);
		m_pClassImage->SetVisibleSingle(FALSE);
	}

	// m_pGuildImage
	{
		RcNEW1 = UIRECT(20.00, 26.00, 17.00, 17.00);
		RcOLD = m_pGuildImage->GetLocalPos();
		m_pGuildImage->AlignSubControl(RcOLD, RcNEW1);
		m_pGuildImage->SetLocalPos(RcNEW1);
		m_pGuildImage->SetVisibleSingle(FALSE);
	}
}

CUIControl* CClubWarRankingDisplayResuScoreSlot::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	return pControl;
}

void CClubWarRankingDisplayResuScoreSlot::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage( cID, dwMsg );
}

void CClubWarRankingDisplayResuScoreSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

HRESULT CClubWarRankingDisplayResuScoreSlot::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	GASSERT(pd3dDevice);
	m_pd3dDevice = pd3dDevice;

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects(pd3dDevice);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT CClubWarRankingDisplayResuScoreSlot::Render(LPDIRECT3DDEVICEQ pd3dDevice)
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

    GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
    if (!pChar) return S_OK;

    GLCLUB& sCLUB = pChar->m_sCLUB;
    DWORD dwClubID = pChar->m_dwGuild;

	if (sCLUB.m_dwID == CLUB_NULL)
	{

		// Load and render the guild badge
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
		// Load and render the guild badge
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

void CClubWarRankingDisplayResuScoreSlot::ResetData()
{
	if ( m_pResuRankTextSlot )		m_pResuRankTextSlot->ClearText();
	if ( m_pResuNameTextSlot )		m_pResuNameTextSlot->ClearText();
	if ( m_pResuScoreTextSlot )		m_pResuScoreTextSlot->ClearText();

	if ( m_pResuIcon )				m_pResuIcon->SetVisibleSingle ( FALSE );
	if ( m_pGuildImage )			m_pGuildImage->SetVisibleSingle(FALSE);
	if ( m_pClassImage )			m_pClassImage->SetVisibleSingle(FALSE);

	for( int i = 0; i < RANK_INFO_ICON_SCHOOL; ++i )
	{
		m_pSchoolIcon[i]->SetVisibleSingle ( FALSE );
	}
}

void CClubWarRankingDisplayResuScoreSlot::SetData( int nRank, std::string strName, WORD wResuScore, WORD wSchool, int nChaClass, WORD wGuNum, WORD wGuMarkVer )
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

		if (wSchool < RANK_INFO_ICON_SCHOOL)
		{
			m_pSchoolIcon[wSchool]->SetVisibleSingle(TRUE);
		}

		if ( m_pClassImage )
		{
			CString strClass;
			switch (nChaClass)
			{
				case 1:		strClass = "BR_M.png"; break;
				case 2:		strClass = "SM_M.png"; break;
				case 4:		strClass = "AR_W.png"; break;
				case 8:		strClass = "SH_W.png"; break;
				case 16:	strClass = "EX_M.png"; break;
				case 32:	strClass = "EX_W.png"; break;
				case 64:	strClass = "BR_W.png"; break;
				case 128:	strClass = "SM_W.png"; break;
				case 256:	strClass = "AR_M.png"; break;
				case 512:	strClass = "SH_M.png"; break;
				case 1024:	strClass = "GUN_M.png"; break;
				case 2048:	strClass = "GUN_W.png"; break;
				case 4096:	strClass = "ASS_M.png"; break;
				case 8192:	strClass = "ASS_W.png"; break;		
			}
			m_pClassImage->SetTextureName(strClass);
			m_pClassImage->LoadTexture(m_pd3dDevice);
			m_pClassImage->SetVisibleSingle(TRUE);
		}

		/* RSiLENT, ADDED COLOR LINE BOX */
		std::string playerCharacterName = GLGaeaClient::GetInstance().GetCharacter()->m_szName;
		if (strName == playerCharacterName)
		{
			m_pLineBox->SetUseRender(TRUE);
			m_pLineBox->SetDiffuse(NS_UITEXTCOLOR::WARMPINK);
		}
		else
		{
			m_pLineBox->SetUseRender(TRUE);
			m_pLineBox->SetDiffuse(NS_UITEXTCOLOR::BLACK);
		}

		if ( m_pResuIcon )		m_pResuIcon->SetVisibleSingle(TRUE);


		CString strText;

		if ( m_pResuRankTextSlot )
		{
			strText.Format( "%d",nRank );
			m_pResuRankTextSlot->AddText( strText.GetString(), NS_UITEXTCOLOR::WHITE );
		}

		if ( m_pResuNameTextSlot )
		{
			m_pResuNameTextSlot->AddText( strName.c_str(), NS_UITEXTCOLOR::WHITE );
		}

		if ( m_pResuScoreTextSlot )
		{
			strText.Format( "%u", wResuScore );
			m_pResuScoreTextSlot->AddText( strText.GetString(), NS_UITEXTCOLOR::GREENYELLOW );
		}
	}
}