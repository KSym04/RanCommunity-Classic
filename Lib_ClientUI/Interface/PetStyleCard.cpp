#include "StdAfx.h"
#include "./PetStyleCard.h"

#include "./BasicTextBox.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "./GameTextControl.h"
#include "./BasicTextButton.h"
#include "./InnerInterface.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLogicData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BOUND_OF_INDEX	-1

INT nPET_STYLE_INDEX_A[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_B[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_C[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_D[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_E[6] = {BOUND_OF_INDEX,0,1,2,3,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_F[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_G[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_H[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_I[6] = {BOUND_OF_INDEX,0,1,2,3,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_J[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_K[6] = {BOUND_OF_INDEX,0,1,2,3,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_L[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_M[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_N[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};

/* new pet types, Juver, 2020/09/16 */
INT nPET_STYLE_INDEX_014[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_015[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_016[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_017[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_018[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_019[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_020[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_021[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_022[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_023[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_024[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_025[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_026[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};
INT nPET_STYLE_INDEX_027[7] = {BOUND_OF_INDEX,0,1,2,3,4,BOUND_OF_INDEX};


TCHAR * szPETSTYLECARD_NAME_A = _T("PETSTYLECARD_NAME_A");
TCHAR * szPETSTYLECARD_NAME_B = _T("PETSTYLECARD_NAME_B");
TCHAR * szPETSTYLECARD_NAME_C = _T("PETSTYLECARD_NAME_C");
TCHAR * szPETSTYLECARD_NAME_D = _T("PETSTYLECARD_NAME_D");
TCHAR * szPETSTYLECARD_NAME_E = _T("PETSTYLECARD_NAME_E");
TCHAR * szPETSTYLECARD_NAME_F = _T("PETSTYLECARD_NAME_F");
TCHAR * szPETSTYLECARD_NAME_G = _T("PETSTYLECARD_NAME_G");
TCHAR * szPETSTYLECARD_NAME_H = _T("PETSTYLECARD_NAME_H");
TCHAR * szPETSTYLECARD_NAME_I = _T("PETSTYLECARD_NAME_I");
TCHAR * szPETSTYLECARD_NAME_J = _T("PETSTYLECARD_NAME_J");
TCHAR * szPETSTYLECARD_NAME_K = _T("PETSTYLECARD_NAME_K");
TCHAR * szPETSTYLECARD_NAME_L = _T("PETSTYLECARD_NAME_L");
TCHAR * szPETSTYLECARD_NAME_M = _T("PETSTYLECARD_NAME_M");
TCHAR * szPETSTYLECARD_NAME_N = _T("PETSTYLECARD_NAME_N");

/* new pet types, Juver, 2020/09/16 */
TCHAR * szPETSTYLECARD_NAME_014 = _T("PETSTYLECARD_NAME_014");
TCHAR * szPETSTYLECARD_NAME_015 = _T("PETSTYLECARD_NAME_015");
TCHAR * szPETSTYLECARD_NAME_016 = _T("PETSTYLECARD_NAME_016");
TCHAR * szPETSTYLECARD_NAME_017 = _T("PETSTYLECARD_NAME_017");
TCHAR * szPETSTYLECARD_NAME_018 = _T("PETSTYLECARD_NAME_018");
TCHAR * szPETSTYLECARD_NAME_019 = _T("PETSTYLECARD_NAME_019");
TCHAR * szPETSTYLECARD_NAME_020 = _T("PETSTYLECARD_NAME_020");
TCHAR * szPETSTYLECARD_NAME_021 = _T("PETSTYLECARD_NAME_021");
TCHAR * szPETSTYLECARD_NAME_022 = _T("PETSTYLECARD_NAME_022");
TCHAR * szPETSTYLECARD_NAME_023 = _T("PETSTYLECARD_NAME_023");
TCHAR * szPETSTYLECARD_NAME_024 = _T("PETSTYLECARD_NAME_024");
TCHAR * szPETSTYLECARD_NAME_025 = _T("PETSTYLECARD_NAME_025");
TCHAR * szPETSTYLECARD_NAME_026 = _T("PETSTYLECARD_NAME_026");
TCHAR * szPETSTYLECARD_NAME_027 = _T("PETSTYLECARD_NAME_027");


CPetStyleCard::CPetStyleCard ()
	: m_pStyleName(NULL)
	, m_pPetStylePos(NULL)
	, m_pId2GameText(NULL)
	, m_wPet(0)
	, m_wPetBack(0)
	, m_emTYPE(PET_TYPE_NONE)
	, m_bOKButton(FALSE)
{
	SecureZeroMemory( m_pPetStyle, sizeof( m_pPetStyle ) );
}

CPetStyleCard::~CPetStyleCard ()
{
}

CBasicTextBox* CPetStyleCard::CreateStaticControl( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

CUIControl* CPetStyleCard::CreateControl( const char * szControl )
{
	GASSERT( szControl );

	CUIControl* pControl = new CUIControl;
	pControl->CreateSub( this, szControl );
	pControl->SetVisibleSingle( FALSE );
	RegisterControl( pControl );

	return pControl;
}

void CPetStyleCard::CreateSubControl ()
{
	CD3DFontPar * pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9 );

	CUIWindowEx::CreateControl( "HAIRSTYLECARD_TITLE_BACK_L" );
	CUIWindowEx::CreateControl( "HAIRSTYLECARD_TITLE_BACK_R" );
	m_pStyleName = CreateStaticControl( "HAIRSTYLECARD_TITLE", pFont, TEXT_ALIGN_CENTER_X );

	{ // Note : 스타일 변경 버튼
		CBasicButton* pButton = new CBasicButton;
		pButton->CreateSub( this, "HAIRSTYLECARD_LEFT", UI_FLAG_DEFAULT, PETSTYLECARD_LEFT );
		pButton->CreateFlip( "HAIRSTYLECARD_LEFT_FLIP", CBasicButton::MOUSEIN_FLIP );
		RegisterControl( pButton );

		pButton = new CBasicButton;
		pButton->CreateSub( this, "HAIRSTYLECARD_RIGHT", UI_FLAG_DEFAULT, PETSTYLECARD_RIGHT );
		pButton->CreateFlip( "HAIRSTYLECARD_RIGHT_FLIP", CBasicButton::MOUSEIN_FLIP );
		RegisterControl( pButton );
	}

	{ // Note : 버튼
		CBasicTextButton* pButton = new CBasicTextButton;
		pButton->CreateSub( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, PETSTYLECARD_OK );
		pButton->CreateBaseButton( "HAIRSTYLECARD_OK", CBasicTextButton::SIZE14, 
									CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD( "MODAL_BUTTON", 0 ) );
		pButton->SetShortcutKey( DIK_RETURN, DIK_NUMPADENTER );
		RegisterControl( pButton );

		pButton = new CBasicTextButton;
		pButton->CreateSub( this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, PETSTYLECARD_CANCEL );
		pButton->CreateBaseButton( "HAIRSTYLECARD_CANCEL", CBasicTextButton::SIZE14, 
									CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD( "MODAL_BUTTON", 1 ) );
		pButton->SetShortcutKey( DIK_ESCAPE );
		RegisterControl( pButton );
	}

	{
		CString strCombine;

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_A].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_A_%02d", i );
			m_pPetStyle[PET_TYPE_A][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_B].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_B_%02d", i );
			m_pPetStyle[PET_TYPE_B][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_C].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_C_%02d", i );
			m_pPetStyle[PET_TYPE_C][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_D].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_D_%02d", i );
			m_pPetStyle[PET_TYPE_D][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_E].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_E_%02d", i );
			m_pPetStyle[PET_TYPE_E][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_F].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_F_%02d", i );
			m_pPetStyle[PET_TYPE_F][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_G].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_G_%02d", i );
			m_pPetStyle[PET_TYPE_G][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_H].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_H_%02d", i );
			m_pPetStyle[PET_TYPE_H][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_I].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_I_%02d", i );
			m_pPetStyle[PET_TYPE_I][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_J].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_J_%02d", i );
			m_pPetStyle[PET_TYPE_J][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_K].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_K_%02d", i );
			m_pPetStyle[PET_TYPE_K][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_L].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_L_%02d", i );
			m_pPetStyle[PET_TYPE_L][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_M].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_M_%02d", i );
			m_pPetStyle[PET_TYPE_M][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_N].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_N_%02d", i );
			m_pPetStyle[PET_TYPE_N][i] = CreateControl( strCombine.GetString() );
		}


		/* new pet types, Juver, 2020/09/17 */
		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_014].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_014_%02d", i );
			m_pPetStyle[PET_TYPE_014][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_015].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_015_%02d", i );
			m_pPetStyle[PET_TYPE_015][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_016].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_016_%02d", i );
			m_pPetStyle[PET_TYPE_016][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_017].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_017_%02d", i );
			m_pPetStyle[PET_TYPE_017][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_018].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_018_%02d", i );
			m_pPetStyle[PET_TYPE_018][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_019].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_019_%02d", i );
			m_pPetStyle[PET_TYPE_019][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_020].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_020_%02d", i );
			m_pPetStyle[PET_TYPE_020][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_021].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_021_%02d", i );
			m_pPetStyle[PET_TYPE_021][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_022].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_022_%02d", i );
			m_pPetStyle[PET_TYPE_022][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_023].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_023_%02d", i );
			m_pPetStyle[PET_TYPE_023][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_024].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_024_%02d", i );
			m_pPetStyle[PET_TYPE_024][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_025].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_025_%02d", i );
			m_pPetStyle[PET_TYPE_025][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_026].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_026_%02d", i );
			m_pPetStyle[PET_TYPE_026][i] = CreateControl( strCombine.GetString() );
		}

		for( INT i=0; i<GLCONST_PET::sPETSTYLE[PET_TYPE_027].wSTYLENum; ++i )
		{
			strCombine.Format( "PETSTYLECARD_027_%02d", i );
			m_pPetStyle[PET_TYPE_027][i] = CreateControl( strCombine.GetString() );
		}

	}
}

void CPetStyleCard::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case PETSTYLECARD_LEFT:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				INT nBackPos = *m_pPetStylePos;

				--m_pPetStylePos;

				if( *m_pPetStylePos == BOUND_OF_INDEX )	++m_pPetStylePos;
				else
				{
					m_pPetStyle[m_emTYPE][nBackPos]->SetVisibleSingle( FALSE );
					m_pPetStyle[m_emTYPE][*m_pPetStylePos]->SetVisibleSingle( TRUE );

					m_pStyleName->SetText( ID2GAMEINTEXT( m_pId2GameText, *m_pPetStylePos ) );


					--m_wPet;
					GLGaeaClient::GetInstance().GetPetClient()->StyleChange( m_wPet );
				}
			}
		}
		break;

	case PETSTYLECARD_RIGHT:
		{
			if( CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				INT nBackPos = *m_pPetStylePos;

				++m_pPetStylePos;

				if( *m_pPetStylePos == BOUND_OF_INDEX )	--m_pPetStylePos;
				else
				{
					m_pPetStyle[m_emTYPE][nBackPos]->SetVisibleSingle( FALSE );
					m_pPetStyle[m_emTYPE][*m_pPetStylePos]->SetVisibleSingle( TRUE );

					m_pStyleName->SetText( ID2GAMEINTEXT( m_pId2GameText, *m_pPetStylePos ) );

					++m_wPet;
					GLGaeaClient::GetInstance().GetPetClient()->StyleChange( m_wPet );
				}
			}
		}
		break;

	case PETSTYLECARD_OK:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				m_bOKButton = TRUE;
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}
		break;

	case PETSTYLECARD_CANCEL:
	case ET_CONTROL_BUTTON:
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				CInnerInterface::GetInstance().HideGroup( GetWndID() );
			}
		}
		break;
	}

	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );
}

void CPetStyleCard::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		m_emTYPE = GLGaeaClient::GetInstance().GetPetClient()->m_emTYPE;

		switch( m_emTYPE )
		{
		case PET_TYPE_A:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_A;
				m_pId2GameText = szPETSTYLECARD_NAME_A;
			}
			break;
		case PET_TYPE_B:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_B;
				m_pId2GameText = szPETSTYLECARD_NAME_B;
			}
			break;
		case PET_TYPE_C:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_C;
				m_pId2GameText = szPETSTYLECARD_NAME_C;
			}
			break;
		case PET_TYPE_D:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_D;
				m_pId2GameText = szPETSTYLECARD_NAME_D;
			}
			break;

		case PET_TYPE_E:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_E;
				m_pId2GameText = szPETSTYLECARD_NAME_E;
			}break;

		case PET_TYPE_F:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_F;
				m_pId2GameText = szPETSTYLECARD_NAME_F;
			}break;

		case PET_TYPE_G:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_G;
				m_pId2GameText = szPETSTYLECARD_NAME_G;
			}break;

		case PET_TYPE_H:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_H;
				m_pId2GameText = szPETSTYLECARD_NAME_H;
			}break;

		case PET_TYPE_I:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_I;
				m_pId2GameText = szPETSTYLECARD_NAME_I;
			}break;

		case PET_TYPE_J:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_J;
				m_pId2GameText = szPETSTYLECARD_NAME_J;
			}break;

		case PET_TYPE_K:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_K;
				m_pId2GameText = szPETSTYLECARD_NAME_K;
			}break;

		case PET_TYPE_L:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_L;
				m_pId2GameText = szPETSTYLECARD_NAME_L;
			}break;

		case PET_TYPE_M:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_M;
				m_pId2GameText = szPETSTYLECARD_NAME_M;
			}break;

		case PET_TYPE_N:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_N;
				m_pId2GameText = szPETSTYLECARD_NAME_N;
			}break;

			/* new pet types, Juver, 2020/09/17 */
		case PET_TYPE_014:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_014;
				m_pId2GameText = szPETSTYLECARD_NAME_014;
			}break;

		case PET_TYPE_015:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_015;
				m_pId2GameText = szPETSTYLECARD_NAME_015;
			}break;

		case PET_TYPE_016:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_016;
				m_pId2GameText = szPETSTYLECARD_NAME_016;
			}break;

		case PET_TYPE_017:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_017;
				m_pId2GameText = szPETSTYLECARD_NAME_017;
			}break;

		case PET_TYPE_018:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_018;
				m_pId2GameText = szPETSTYLECARD_NAME_018;
			}break;

		case PET_TYPE_019:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_019;
				m_pId2GameText = szPETSTYLECARD_NAME_019;
			}break;

		case PET_TYPE_020:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_020;
				m_pId2GameText = szPETSTYLECARD_NAME_020;
			}break;

		case PET_TYPE_021:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_021;
				m_pId2GameText = szPETSTYLECARD_NAME_021;
			}break;

		case PET_TYPE_022:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_022;
				m_pId2GameText = szPETSTYLECARD_NAME_022;
			}break;

		case PET_TYPE_023:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_023;
				m_pId2GameText = szPETSTYLECARD_NAME_023;
			}break;

		case PET_TYPE_024:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_024;
				m_pId2GameText = szPETSTYLECARD_NAME_024;
			}break;

		case PET_TYPE_025:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_025;
				m_pId2GameText = szPETSTYLECARD_NAME_025;
			}break;

		case PET_TYPE_026:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_026;
				m_pId2GameText = szPETSTYLECARD_NAME_026;
			}break;

		case PET_TYPE_027:
			{
				m_pPetStylePos = nPET_STYLE_INDEX_027;
				m_pId2GameText = szPETSTYLECARD_NAME_027;
			}break;



		default:
			GASSERT( 0 && "PETTYPE::PET_TYPE_NONE" );
			return;
		}

		m_wPet = GLGaeaClient::GetInstance().GetPetClient()->m_wStyle;
		m_wPetBack = m_wPet;

		m_pPetStylePos += m_wPet + 1;

		m_pStyleName->AddText( ID2GAMEINTEXT( m_pId2GameText, *m_pPetStylePos ) );
		m_pPetStyle[m_emTYPE][*m_pPetStylePos]->SetVisibleSingle( TRUE );

		GLGaeaClient::GetInstance().GetPetClient()->StyleInitData();
	}
	else
	{
		if( m_bOKButton )
		{
			GLGaeaClient::GetInstance().GetPetClient()->ReqChangeStyle( m_wPet );
			m_bOKButton = FALSE;
		}
		else
		{
			GLGaeaClient::GetInstance().GetPetClient()->StyleChange( m_wPetBack );
		}

		m_pStyleName->ClearText();
		m_pPetStyle[m_emTYPE][*m_pPetStylePos]->SetVisibleSingle( FALSE );
		
		GLGaeaClient::GetInstance().GetPetClient()->StyleEnd();
	}
}