#include "StdAfx.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLCharacter.h"
#include "../../Lib_Client/G-Logic/GLGaeaClient.h"

#include "./BasicLineBox.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumb.h"
#include "./BasicScrollThumbFrame.h"
#include "./ItemImage.h"
#include "./BasicTextBox.h"
#include "./BasicTextBoxEx.h"

#include "./InnerInterface.h"

#include "./MaxRvPreviewPageSlot.h"
#include "./MaxRvPreviewPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMaxRvPreviewPage::CMaxRvPreviewPage ()	
	: m_emType(EMMAXRV_PREVIEW_NONE)
	, m_nStart(0)
	, m_nTotal(0)
	, m_pScrollBar(NULL)
	, m_pItemImage(NULL)
	, m_pItemName(NULL)
{
	for ( int i=0; i<MAXRV_PREVIEW_PAGE_MAX_SLOT; ++i )
	{
		m_pSlot[i] = NULL;
	}
}

CMaxRvPreviewPage::~CMaxRvPreviewPage ()
{
}

void CMaxRvPreviewPage::CreateSubControl ()
{
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 12, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MAXRV_PREVIEW_PAGE_ITEM_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MAXRV_PREVIEW_PAGE_STAT_LINE" );
	RegisterControl ( pBasicLineBox );

	pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxQuestList ( "MAXRV_PREVIEW_PAGE_SCROLLBAR_BACKGROUND" );
	RegisterControl ( pBasicLineBox );

	std::string strSlot[MAXRV_PREVIEW_PAGE_MAX_SLOT] = 
	{
		"MAXRV_PREVIEW_PAGE_STAT_00",
		"MAXRV_PREVIEW_PAGE_STAT_01",
		"MAXRV_PREVIEW_PAGE_STAT_02",
		"MAXRV_PREVIEW_PAGE_STAT_03",
		"MAXRV_PREVIEW_PAGE_STAT_04",
		"MAXRV_PREVIEW_PAGE_STAT_05",
		"MAXRV_PREVIEW_PAGE_STAT_06",
		"MAXRV_PREVIEW_PAGE_STAT_07",
		"MAXRV_PREVIEW_PAGE_STAT_08",
		"MAXRV_PREVIEW_PAGE_STAT_09",
	};

	for ( int i=0; i<MAXRV_PREVIEW_PAGE_MAX_SLOT; ++i )
	{
		m_pSlot[i] = new CMaxRvPreviewPageSlot;
		m_pSlot[i]->CreateSub ( this, strSlot[i].c_str() );
		m_pSlot[i]->CreateSubControl();
		m_pSlot[i]->SetVisibleSingle(FALSE);
		RegisterControl ( m_pSlot[i] );	
	}

	m_pScrollBar = new CBasicScrollBarEx;
	m_pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, MAXRV_PREVIEW_PAGE_SCROLLBAR );
	m_pScrollBar->CreateBaseScrollBar ( "MAXRV_PREVIEW_PAGE_SCROLLBAR" );
	m_pScrollBar->GetThumbFrame()->SetState ( 1, MAXRV_PREVIEW_PAGE_MAX_SLOT );
	RegisterControl ( m_pScrollBar );

	m_pItemImage = new CItemImage;
	m_pItemImage->CreateSub ( this, "MAXRV_PREVIEW_PAGE_ITEM_IMAGE", UI_FLAG_DEFAULT, MAXRV_PREVIEW_PAGE_ITEM_IMAGE );
	m_pItemImage->CreateSubControl ();
	RegisterControl ( m_pItemImage );

	m_pItemName = new CBasicTextBoxEx;
	m_pItemName->CreateSub ( this, "MAXRV_PREVIEW_PAGE_ITEM_NAME" );
	m_pItemName->SetFont ( pFont10 );
	m_pItemName->SetTextAlign( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pItemName );
}

void CMaxRvPreviewPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	const SMAXRV_PREVIEW& sData = GLGaeaClient::GetInstance().GetCharacter()->m_sMaxRvPreview;

	m_nTotal = 0;
	
	if ( m_emType == EMMAXRV_PREVIEW_MOBGEN )
		m_nTotal = static_cast<int>(sData.m_vecOptionMobGen.size());
	else if ( m_emType == EMMAXRV_PREVIEW_REBUILD )
		m_nTotal = static_cast<int>(sData.m_vecOptionRebuild.size());

	if ( m_pScrollBar )
	{
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal();

			if( nTotal != m_nTotal )
				m_pScrollBar->GetThumbFrame()->SetState ( m_nTotal, MAXRV_PREVIEW_PAGE_MAX_SLOT );	
		}
		{
			CBasicScrollThumbFrame* pThumbFrame = m_pScrollBar->GetThumbFrame ();
			int nTotal = pThumbFrame->GetTotal ();
			if ( nTotal <= MAXRV_PREVIEW_PAGE_MAX_SLOT ) return ;
			const int nViewPerPage = pThumbFrame->GetViewPerPage ();

			if ( nViewPerPage < nTotal )
			{
				int nCurPos = 0;
				const int nMovableLine = nTotal - nViewPerPage;
				float fPercent = pThumbFrame->GetPercent ();
				nCurPos = (int)floor(fPercent * nMovableLine);
				if ( nCurPos < 0 ) nCurPos = 0;
				if ( m_nStart == nCurPos ) return;
				m_nStart = nCurPos;
				RenderView();
			}
		}
	}
}

void CMaxRvPreviewPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
	}
	else
	{
	}
}

void CMaxRvPreviewPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch( ControlID )
	{
	case MAXRV_PREVIEW_PAGE_ITEM_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				SMAXRV_PREVIEW& sData = GLGaeaClient::GetInstance().GetCharacter()->m_sMaxRvPreview;

				if( sData.m_sItemCustom.sNativeID == NATIVEID_NULL() ) return;

				CInnerInterface::GetInstance().SHOW_ITEM_INFO( sData.m_sItemCustom, FALSE, FALSE, FALSE, NULL, NULL );
			}
		}
		break;
	};
}

void CMaxRvPreviewPage::ResetItemData()
{
	if ( m_pItemImage )
		m_pItemImage->ResetItem();

	if ( m_pItemName )
		m_pItemName->ClearText();

	for ( int i=0; i<MAXRV_PREVIEW_PAGE_MAX_SLOT; ++i )
	{
		if ( m_pSlot[i] )
			m_pSlot[i]->ResetText();
	}
}

void CMaxRvPreviewPage::ShowItemData()
{
	ResetItemData();

	const SMAXRV_PREVIEW& sData = GLGaeaClient::GetInstance().GetCharacter()->m_sMaxRvPreview;

	SITEM* pITEM = GLItemMan::GetInstance().GetItem( sData.m_sItemID );
	if ( pITEM )
	{
		if ( m_pItemName )		
		{
			m_pItemName->SetText( pITEM->GetName(), COMMENT::ITEMCOLOR[pITEM->sBasicOp.emLevel] );
		}

		if ( m_pItemImage )		
		{
			m_pItemImage->SetItem( pITEM->sBasicOp.sICONID, pITEM->GetInventoryFile() );
		}
	}


	m_nTotal = 0;

	if ( m_emType == EMMAXRV_PREVIEW_MOBGEN )
		m_nTotal = static_cast<int>(sData.m_vecOptionMobGen.size());
	else if ( m_emType == EMMAXRV_PREVIEW_REBUILD )
		m_nTotal = static_cast<int>(sData.m_vecOptionRebuild.size());

	RenderView();
}


void CMaxRvPreviewPage::RenderView()
{
	const SMAXRV_PREVIEW& sData = GLGaeaClient::GetInstance().GetCharacter()->m_sMaxRvPreview;

	for ( int i=0; i < MAXRV_PREVIEW_PAGE_MAX_SLOT; ++ i )
	{
		if ( m_pSlot[i] )
		{
			m_pSlot[i]->ResetText();
			m_pSlot[i]->SetVisibleSingle( FALSE );
		}
	}

	int nEnd = m_nStart + MAXRV_PREVIEW_PAGE_MAX_SLOT;
	int nSlot = 0;

	if ( m_emType == EMMAXRV_PREVIEW_MOBGEN )
	{
		for ( int i = m_nStart; i < nEnd; ++ i )
		{
			if ( i >= (int)sData.m_vecOptionMobGen.size() )	
				continue;

			if ( m_pSlot[nSlot] )
			{
				const SMAXRV_OPTION& sOption = sData.m_vecOptionMobGen[i];
				m_pSlot[nSlot]->SetVisibleSingle( TRUE );
				m_pSlot[nSlot]->SetOption( sOption.m_emOption, sOption.m_fMin, sOption.m_fMax );
				nSlot ++ ;
			}
		}
	}
	else if ( m_emType == EMMAXRV_PREVIEW_REBUILD )
	{
		for ( int i = m_nStart; i < nEnd; ++ i )
		{
			if ( i >= (int)sData.m_vecOptionRebuild.size() )	
				continue;

			if ( m_pSlot[nSlot] )
			{
				const SMAXRV_OPTION& sOption = sData.m_vecOptionRebuild[i];
				m_pSlot[nSlot]->SetVisibleSingle( TRUE );
				m_pSlot[nSlot]->SetOption( sOption.m_emOption, sOption.m_fMin, sOption.m_fMax );
				nSlot ++ ;
			}
		}
	}
	
}
