#include "stdafx.h"
#include "PVPWoeTowerCapture.h"

#include "BasicLineBox.h"
#include "BasicTextBox.h"
#include "../Lib_Engine/DxCommon/D3DFont.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "InnerInterface.h"

#include "../Lib_Client/G-Logic/GLPVPWoeClient.h"
#include "../Lib_Client/G-Logic/GLPVPWoeField.h"

#include "../Lib_Engine/DxCommon/DxClubMan.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../Lib_Client/G-Logic/GLGaeaServer.h"
#include "../Lib_Client/G-Logic/GLAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPWoeTowerCapture::CPVPWoeTowerCapture ()
	: m_bCHECK_MOUSE_STATE(FALSE)
	, m_bFirstGap(FALSE)
	, m_nPosX(0)
	, m_nPosY(0)
	, m_fUpdateTime(0.0f)
	, m_pGuildName(NULL)
	, m_pTextTimer(NULL)
{
}

CPVPWoeTowerCapture::~CPVPWoeTowerCapture ()
{
}

void CPVPWoeTowerCapture::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	m_pBackGround = new CUIControl;
	m_pBackGround->CreateSub ( this, "WOE_TOWER_CAPTURE_BG", UI_FLAG_DEFAULT, WOE_TOWER_CAPTURE_BG);
	m_pBackGround->SetVisibleSingle(TRUE);
	RegisterControl (m_pBackGround);

	m_pGuildName = new CBasicTextBox;
	m_pGuildName->CreateSub ( this, "WOE_TOWER_CAPTURE_GUILD_TEXT" );
	m_pGuildName->SetFont ( pFont );
	m_pGuildName->SetTextAlign (TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y);
	m_pGuildName->AddText("Guild Holder:");
	RegisterControl (m_pGuildName);

	m_pHolderName = new CBasicTextBox;
	m_pHolderName->CreateSub(this, "WOE_TOWER_CAPTURE_HOLDER_TEXT");
	m_pHolderName->SetFont(pFont);
	m_pHolderName->SetTextAlign(TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y);
	m_pHolderName->AddText("None");
	RegisterControl(m_pHolderName);

	m_pTextTimer = new CBasicTextBox;
	m_pTextTimer->CreateSub ( this, "WOE_TOWER_CAPTURE_TIME_TEXT" );
	m_pTextTimer->SetFont ( pFont );
	m_pTextTimer->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextTimer->AddText( "FF:FF", NS_UITEXTCOLOR::WHITE );
	RegisterControl ( m_pTextTimer );

	m_pClubDefault = new CUIControl;
	m_pClubDefault->CreateSub(this, "WOE_TOWER_CLUB_DEFAULT_MARK", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pClubDefault->SetVisibleSingle(FALSE);
	RegisterControl(m_pClubDefault);

	m_pClubMark = new CUIControl;
	m_pClubMark->CreateSub(this, "WOE_TOWER_CLUB_MARK", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pClubMark->SetVisibleSingle(FALSE);
	RegisterControl(m_pClubMark);
}

void CPVPWoeTowerCapture::CheckMouseState ()
{
	const DWORD dwMsg = GetMessageEx ();
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		if( UIMSG_LB_DUP & dwMsg )
		{
			AddMessageEx( UIMSG_PVPWOE_TOWER_CAPTURE_LBDUP );
			SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
			return ;
		}

		if ( dwMsg & UIMSG_LB_DOWN )
		{
			SetExclusiveControl();	

			if ( !m_bFirstGap )
			{
				UIRECT rcPos = GetGlobalPos ();
				m_vGap.x = m_nPosX - rcPos.left;
				m_vGap.y = m_nPosY - rcPos.top;
				m_bFirstGap = TRUE;

				SetDiffuse ( D3DCOLOR_ARGB ( 0xB4, 0xFF, 0xFF, 0xFF ) );
			}
		}
		else if ( CHECK_LB_UP_LIKE ( dwMsg ) )
		{
			ResetExclusiveControl();
			m_bFirstGap = FALSE;				

			SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
		}
	}
	else if ( CHECK_LB_UP_LIKE ( dwMsg ) )		
	{								
		ResetExclusiveControl();
		m_bFirstGap = FALSE;					

		SetDiffuse ( D3DCOLOR_ARGB ( 0xFF, 0xFF, 0xFF, 0xFF ) );
	}
}

void CPVPWoeTowerCapture::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	m_bCHECK_MOUSE_STATE = false;
	m_nPosX = x;
	m_nPosY = y;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( IsExclusiveSelfControl() ){
		SetGlobalPos ( D3DXVECTOR2 ( x - m_vGap.x, y - m_vGap.y ) );
	}

	m_fUpdateTime += fElapsedTime;
	if ( m_fUpdateTime >= 0.5f ){
		m_fUpdateTime = 0.0f;

		UpdateInfo();
	}
}

void CPVPWoeTowerCapture::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( !m_bCHECK_MOUSE_STATE )
	{
		CheckMouseState ();
		m_bCHECK_MOUSE_STATE = true;
	}
}

void CPVPWoeTowerCapture::UpdateInfo()
{
	WOE_STATE emstate = GLPVPWoeClient::GetInstance().m_emState;
	float fbattle_remain_time = GLPVPWoeClient::GetInstance().m_fRemain;
	
	m_dwClubID = GLPVPWoeClient::GetInstance().m_dwEmperiumGuildPrev;
	m_dwEmperiumMarkVer = GLPVPWoeClient::GetInstance().m_dwEmperiumMarkVerPrev;
	m_strGuildName = GLPVPWoeClient::GetInstance().strName;

	if (m_pHolderName)
	{
		m_pHolderName->SetText(m_strGuildName.c_str(), NS_UITEXTCOLOR::GREEN_WOW);
	}

	if ( m_pTextTimer )
		m_pTextTimer->ClearText();
	
	if ( emstate == WOE_STATE_BATTLE )
	{
		if ( m_pTextTimer )
		{
			if ( fbattle_remain_time > 0.0f )
			{
				DWORD dwTextColor = NS_UITEXTCOLOR::ORANGE;
				if ( fbattle_remain_time < 300.0f ){
					dwTextColor = NS_UITEXTCOLOR::RED;
				}
				CString strCombine;
				strCombine.Format ( "%02d:%02d", (DWORD)fbattle_remain_time/60, (DWORD)fbattle_remain_time%60 );
				m_pTextTimer->AddText( strCombine.GetString(), dwTextColor );
			}
		}
	}
}
HRESULT CPVPWoeTowerCapture::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	GASSERT(pd3dDevice);
	m_pd3dDevice = pd3dDevice;

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects(pd3dDevice);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT CPVPWoeTowerCapture::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
	if (!CInnerInterface::GetInstance().IsVisibleGroup(PVP_WOE_TOWER_CAPTURE))
	{
		m_pClubMark->SetVisibleSingle(FALSE);
		return CUIGroup::Render(pd3dDevice);
	}

	HRESULT hr = S_OK;

	m_pClubMark->SetVisibleSingle(FALSE);

	{
		hr = CUIGroup::Render(pd3dDevice);
		if (FAILED(hr)) return hr;
	}

	BOOL bGuild = m_dwClubID ? TRUE : FALSE;
	if (bGuild && m_dwClubID > 0)
	{
		m_pClubMark->SetVisibleSingle(TRUE);
		const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
		const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_dwClubID, m_dwEmperiumMarkVer);

		m_pClubMark->SetTexturePos(0, sMarkData.vTex_1_LU);
		m_pClubMark->SetTexturePos(1, sMarkData.vTex_2_RU);
		m_pClubMark->SetTexturePos(3, sMarkData.vTex_3_LD);
		m_pClubMark->SetTexturePos(2, sMarkData.vTex_4_RD);
		m_pClubMark->SetTexture(sMarkData.pddsTexture);
		m_pClubMark->Render(pd3dDevice);
	}
	return hr;
}