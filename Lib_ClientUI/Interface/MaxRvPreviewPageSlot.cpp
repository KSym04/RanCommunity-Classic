#include "StdAfx.h"

#include "../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../Lib_Engine/DxCommon/d3dfont.h"

#include "./BasicTextBoxEx.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "./MaxRvPreviewPageSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMaxRvPreviewPageSlot::CMaxRvPreviewPageSlot ()	
	: m_pTextOption(NULL)
	, m_pTextValueMin(NULL)
	, m_pTextValueMax(NULL)
{
}

CMaxRvPreviewPageSlot::~CMaxRvPreviewPageSlot ()
{
}

void CMaxRvPreviewPageSlot::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pTextOption = new CBasicTextBoxEx;
	m_pTextOption->CreateSub ( this, "MAXRV_PREVIEW_PAGE_STAT_OPTION" );
	m_pTextOption->SetFont ( pFont8 );
	m_pTextOption->SetTextAlign( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pTextOption );

	m_pTextValueMin = new CBasicTextBoxEx;
	m_pTextValueMin->CreateSub ( this, "MAXRV_PREVIEW_PAGE_STAT_MIN" );
	m_pTextValueMin->SetFont ( pFont8 );
	m_pTextValueMin->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pTextValueMin );

	m_pTextValueMax = new CBasicTextBoxEx;
	m_pTextValueMax->CreateSub ( this, "MAXRV_PREVIEW_PAGE_STAT_MAX" );
	m_pTextValueMax->SetFont ( pFont8 );
	m_pTextValueMax->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pTextValueMax );



}

void CMaxRvPreviewPageSlot::ResetText()
{
	if ( m_pTextOption )
		m_pTextOption->ClearText();

	if ( m_pTextValueMin )
		m_pTextValueMin->ClearText();

	if ( m_pTextValueMax )
		m_pTextValueMax->ClearText();
}

void CMaxRvPreviewPageSlot::SetOption( EMRANDOM_OPT emOption, float fMin, float fMax )
{
	if( (EMR_OPT_NULL < emOption) && (emOption < EMR_OPT_SIZE) )
	{
		if ( m_pTextOption )
		{
			m_pTextOption->SetText( ID2GAMEWORD("ITEM_RANDOM_OPTION", emOption ), NS_UITEXTCOLOR::UIINFO_OPTION1 );
		}

		if ( m_pTextValueMin )
		{
			float fDisplayValue = GetOptionValue( emOption, fMin );

			CString strText;
			strText.Format( "%s", ID2GAMEWORD("MAXRV_PREVIEW_SLOT_TEXT", 0 ) );

			if( fDisplayValue > 0.0f)
					strText += _T('+');
			if( fDisplayValue < 0.0f) 
				strText += _T('-');

			strText.AppendFormat( "%.2f", fDisplayValue );

			if( IsOptionPer(emOption) ) 
				strText += _T('%');

			m_pTextValueMin->SetText( strText, NS_UITEXTCOLOR::UIINFO_OPTION1 );
		}

		if ( m_pTextValueMax )
		{
			float fDisplayValue = GetOptionValue( emOption, fMax );

			CString strText;
			strText.Format( "%s", ID2GAMEWORD("MAXRV_PREVIEW_SLOT_TEXT", 1 ) );

			if( fDisplayValue > 0.0f)
				strText += _T('+');
			if( fDisplayValue < 0.0f) 
				strText += _T('-');

			strText.AppendFormat( "%.2f", fDisplayValue );

			if( IsOptionPer(emOption) ) 
				strText += _T('%');

			m_pTextValueMax->SetText( strText, NS_UITEXTCOLOR::UIINFO_OPTION1 );
		}
	}	
}

//similar to SITEMCUSTOM::GETOptVALUE and SITEMCUSTOM::SETOptVALUE
//updates to the functions mentioned above should be also applied here
//we cannot directly call that function as it is not static
//fValue is raw value from randomizer data so it should be converted in ways the game interpreted those values
float CMaxRvPreviewPageSlot::GetOptionValue( EMRANDOM_OPT emOption, float fValue )
{
	short nVALUE(0);

	//random option raw value to SITEMCUSTOM stored value
	switch ( emOption )
	{
	default:
	case EMR_OPT_NULL:
		nVALUE = 0;
		break;

	case EMR_OPT_DAMAGE:
	case EMR_OPT_DEFENSE:
	case EMR_OPT_MOVE_SPEED:
		nVALUE = (short) (fValue*100.0f);
		break;

	case EMR_OPT_HITRATE:
	case EMR_OPT_AVOIDRATE:
		nVALUE = (short) (fValue*100.0f);
		break;

	case EMR_OPT_HP:
	case EMR_OPT_MP:
	case EMR_OPT_SP:
		nVALUE = (short) (fValue);
		break;

	case EMR_OPT_HP_INC:
	case EMR_OPT_MP_INC:
	case EMR_OPT_SP_INC:
	case EMR_OPT_HMS_INC:
		nVALUE = (short) (fValue*10000);
		break;

	case EMR_OPT_GRIND_DAMAGE:
	case EMR_OPT_GRIND_DEFENSE:
		nVALUE = (short) (fValue);
		break;

	case EMR_OPT_RANGE:
		nVALUE = (short) (fValue*100);
		break;

	case EMR_OPT_DIS_SP:
		nVALUE = (short) (fValue);
		break;

	case EMR_OPT_RESIST:
		nVALUE = (short) (fValue);
		break;

		/*official random value, Juver, 2018/06/09 */
	case EMR_OPT_DAMAGE_PLUS:
	case EMR_OPT_DEFENSE_PLUS:
	case EMR_OPT_HITRATE_PLUS:
	case EMR_OPT_AVOIDRATE_PLUS:
	case EMR_OPT_POW_PLUS:
	case EMR_OPT_STR_PLUS:
	case EMR_OPT_SPI_PLUS:
	case EMR_OPT_DEX_PLUS:
	case EMR_OPT_STA_PLUS:
	case EMR_OPT_PA_PLUS:
	case EMR_OPT_SA_PLUS:
	case EMR_OPT_MA_PLUS:
	case EMR_OPT_POTION_HP_PLUS:
	case EMR_OPT_POTION_MP_PLUS:
	case EMR_OPT_POTION_SP_PLUS:
	case EMR_OPT_GAIN_CP_PLUS:
		nVALUE = (short) (fValue);
		break;

		/*additional RV, Juver, 2018/06/14 */
	case EMR_OPT_EX_MOVE_SPEED:
	case EMR_OPT_EX_ATTACK_SPEED:
	case EMR_OPT_EX_CRITICAL_HIT_CHANCE:
	case EMR_OPT_EX_CRITICAL_HIT_DAMAGE:
	case EMR_OPT_EX_CRUSHING_BLOW_CHANCE:
	case EMR_OPT_EX_CRUSHING_BLOW_DAMAGE:
		nVALUE = (short) (fValue*100.0f);
		break;

		/*item max CP, Juver, 2018/07/09 */
	case EMR_OPT_EX_MAX_CP:
		nVALUE = (short) (fValue);
		break;

	};


	//SITEMCUSTOM value to ui displayed value

	switch ( emOption )
	{
	default:
	case EMR_OPT_NULL:
		return 0.0f;

	case EMR_OPT_DAMAGE:
	case EMR_OPT_DEFENSE:
	case EMR_OPT_MOVE_SPEED:
		return nVALUE*0.01f;

	case EMR_OPT_HITRATE:
	case EMR_OPT_AVOIDRATE:
		return nVALUE*0.01f;

	case EMR_OPT_HP:
	case EMR_OPT_MP:
	case EMR_OPT_SP:
		return nVALUE;

	case EMR_OPT_HP_INC:
	case EMR_OPT_MP_INC:
	case EMR_OPT_SP_INC:
	case EMR_OPT_HMS_INC:
		return nVALUE*0.0001f;

	case EMR_OPT_GRIND_DAMAGE:
	case EMR_OPT_GRIND_DEFENSE:
		return nVALUE;

	case EMR_OPT_RANGE:
		return nVALUE*0.01f;

	case EMR_OPT_DIS_SP:
		return nVALUE;

	case EMR_OPT_RESIST:
		return nVALUE;

		/*official random value, Juver, 2018/06/09 */
	case EMR_OPT_DAMAGE_PLUS:
	case EMR_OPT_DEFENSE_PLUS:
	case EMR_OPT_HITRATE_PLUS:
	case EMR_OPT_AVOIDRATE_PLUS:
	case EMR_OPT_POW_PLUS:
	case EMR_OPT_STR_PLUS:
	case EMR_OPT_SPI_PLUS:
	case EMR_OPT_DEX_PLUS:
	case EMR_OPT_STA_PLUS:
	case EMR_OPT_PA_PLUS:
	case EMR_OPT_SA_PLUS:
	case EMR_OPT_MA_PLUS:
	case EMR_OPT_POTION_HP_PLUS:
	case EMR_OPT_POTION_MP_PLUS:
	case EMR_OPT_POTION_SP_PLUS:
	case EMR_OPT_GAIN_CP_PLUS:
		return nVALUE;

		/*additional RV, Juver, 2018/06/14 */
	case EMR_OPT_EX_MOVE_SPEED:
	case EMR_OPT_EX_ATTACK_SPEED:
	case EMR_OPT_EX_CRITICAL_HIT_CHANCE:
	case EMR_OPT_EX_CRITICAL_HIT_DAMAGE:
	case EMR_OPT_EX_CRUSHING_BLOW_CHANCE:
	case EMR_OPT_EX_CRUSHING_BLOW_DAMAGE:
		return nVALUE*0.01f;

		/*item max CP, Juver, 2018/07/09 */
	case EMR_OPT_EX_MAX_CP:
		return nVALUE;

	};

	return 0.0f;
}

BOOL CMaxRvPreviewPageSlot::IsOptionPer( EMRANDOM_OPT emOption )
{
	/*official random value, Juver, 2018/06/08 */
	switch( emOption )
	{
	case EMR_OPT_NULL:
	case EMR_OPT_DAMAGE:
	case EMR_OPT_DEFENSE:
	case EMR_OPT_HITRATE:
	case EMR_OPT_AVOIDRATE:
	case EMR_OPT_HP_INC:
	case EMR_OPT_MP_INC:
	case EMR_OPT_SP_INC:
	case EMR_OPT_HMS_INC:
	case EMR_OPT_MOVE_SPEED:
		return TRUE;

		/*additional RV, Juver, 2018/06/14 */
	case EMR_OPT_EX_MOVE_SPEED:
	case EMR_OPT_EX_ATTACK_SPEED:
	case EMR_OPT_EX_CRITICAL_HIT_CHANCE:
	case EMR_OPT_EX_CRITICAL_HIT_DAMAGE:
	case EMR_OPT_EX_CRUSHING_BLOW_CHANCE:
	case EMR_OPT_EX_CRUSHING_BLOW_DAMAGE:
		return TRUE;

	default:
		return FALSE;
	};

	return FALSE;
}