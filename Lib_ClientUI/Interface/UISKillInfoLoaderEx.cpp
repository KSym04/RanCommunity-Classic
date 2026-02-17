#include "stdafx.h"
#include "./UIInfoLoader.h"

#include "./InnerInterface.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Client/G-Logic/GLGaeaClient.h"
#include "../../Lib_Client/G-Logic/GLSkillPurchase.h"
#include "../../Lib_Engine/Core/NSRParam.h"

#include <boost/format.hpp>
#include "./UIInfoLoaderDefine.h"

#include "../../Lib_Client/G-Logic/GLItemMan.h"
#include "../../Lib_Client/G-Logic/GLItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NS_UIINFO_BASIC;

namespace NS_SKILLINFO_EX
{
	SNATIVEID	m_sNativeID_Back;
	BOOL		m_bNextLevel_Back = FALSE;

	/*multi skill Juver, 2018/09/05 */
	SNATIVEID	m_sNativeID2Main_Back;
	SNATIVEID	m_sNativeID2Sub_Back;
	BOOL		m_bNextLevel2_Back = FALSE;

	/*extended buff info, EJCode, 2018/10/06 */
	SNATIVEID	m_sNativeID3_Back;
	WORD		m_wLevel3_Back;

	void RESET()
	{
		m_sNativeID_Back = NATIVEID_NULL();
		m_bNextLevel_Back = FALSE;

		/*multi skill Juver, 2018/09/05 */
		m_sNativeID2Main_Back = NATIVEID_NULL();
		m_sNativeID2Sub_Back = NATIVEID_NULL();
		m_bNextLevel2_Back = FALSE;

		/*extended buff info, EJCode, 2018/10/06 */
		m_sNativeID3_Back = NATIVEID_NULL();
		m_wLevel3_Back = 0;
	}

	int AddTextNoSplit(CString strText, D3DCOLOR dwColor)
	{
		return CInnerInterface::GetInstance().InfoDisplayAddTextNoSplit(strText, dwColor);
	}

	void AddTextLongestLineSplit(CString strText, D3DCOLOR dwColor)
	{
		CInnerInterface::GetInstance().InfoDisplayAddTextLongestLineSplit(strText, dwColor);
	}

	void AddStringNoSplit(int nIndex, CString strText, D3DCOLOR dwColor)
	{
		return CInnerInterface::GetInstance().InfoDisplayAddStringNoSplit(nIndex, strText, dwColor);
	}

	void AddTitle(CString strText, D3DCOLOR dwColor)
	{
		CInnerInterface::GetInstance().InfoDisplayAddTitle(strText, dwColor);
	}

	void SetIconImage(SNATIVEID sICONINDEX, const char* szTexture)
	{
		CInnerInterface::GetInstance().InfoDisplaySetIcon(sICONINDEX, szTexture);
	}

	void RemoveAllInfo()
	{
		CInnerInterface::GetInstance().InfoDisplayClearText();
	}

	boost::format  CreateFormat(const std::string & f_string) 
	{
		boost::format fmter(f_string);
		fmter.exceptions(boost::io::all_error_bits ^ (boost::io::too_many_args_bit | boost::io::too_few_args_bit));
		return fmter;
	}

	void AddInfoReqWeapon(const SKILL::SSKILLBASIC sBASIC)
	{
		if ((sBASIC.emUSE_LITEM && (sBASIC.emUSE_LITEM != SKILLATT_NOCARE)) || (sBASIC.emUSE_RITEM && (sBASIC.emUSE_RITEM != SKILLATT_NOCARE)))
		{
			CString strText("");
			strText.Format("%s :", ID2GAMEWORD("SKILL_BASIC_INFO", 9));
			int nIndex = AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
			
			if (sBASIC.emUSE_LITEM)
			{
				if (sBASIC.emUSE_LITEM != SKILLATT_NOCARE)
				{
					AddStringNoSplit(nIndex, COMMENT::SKILLATTACK[sBASIC.emUSE_LITEM].c_str(), NS_UITEXTCOLOR::WHITE);
					AddStringNoSplit(nIndex, " ", NS_UITEXTCOLOR::WHITE);
				}
			}

			if (sBASIC.emUSE_RITEM)
			{
				if (sBASIC.emUSE_RITEM != SKILLATT_NOCARE)
				{
					AddStringNoSplit(nIndex, COMMENT::SKILLATTACK[sBASIC.emUSE_RITEM].c_str(), NS_UITEXTCOLOR::WHITE);
				}
			}
		}
	}

	void AddInfoConsumption(const SKILL::CDATA_LVL sDATA_LVL)
	{
		CString	strText("");
		DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
		std::vector<SINFO_TEXT> vecText;

		if (0 < sDATA_LVL.wUSE_ARROWNUM)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_CDATA_CVL_BASIC_USE_VALUE", 0), sDATA_LVL.wUSE_ARROWNUM);
			vecText.push_back( SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sDATA_LVL.wUSE_CHARMNUM)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_CDATA_CVL_BASIC_USE_VALUE", 1), sDATA_LVL.wUSE_CHARMNUM);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sDATA_LVL.wUSE_BULLETNUM)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_CDATA_CVL_BASIC_USE_VALUE", 7), sDATA_LVL.wUSE_BULLETNUM);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sDATA_LVL.wUSE_HP)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_CDATA_CVL_BASIC_USE_VALUE", 3), sDATA_LVL.wUSE_HP);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sDATA_LVL.wUSE_MP)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_CDATA_CVL_BASIC_USE_VALUE", 4), sDATA_LVL.wUSE_MP);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sDATA_LVL.wUSE_SP)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_CDATA_CVL_BASIC_USE_VALUE", 5), sDATA_LVL.wUSE_SP);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sDATA_LVL.wUSE_CP)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_CDATA_CVL_BASIC_USE_VALUE", 6), sDATA_LVL.wUSE_CP);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		int nIndex = 0;
		for (size_t i = 0; i < vecText.size(); ++i)
		{
			SINFO_TEXT& sData = vecText[i];
			if (i == 0)
			{
				strText.Format("[%s]", ID2GAMEWORD("SKILL_BASIC_INFO", 10));
				AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
			}

			AddTextNoSplit(sData.strText, sData.dwTextColor);
		}
	}

	void AddInfoCooldown(const SKILL::CDATA_LVL sDATA_LVL)
	{
		if (0 < sDATA_LVL.fDELAYTIME)
		{
			CString	strText;
			strText.Format("%s :%2.1f %s", ID2GAMEWORD("SKILL_CDATA_CVL_BASIC", 0), sDATA_LVL.fDELAYTIME, ID2GAMEWORD("WAITSERVER_TIMELEFT_UNIT", 0));
			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
		}
	}

	void AddInfoReqLearn(const SKILL::SLEARN_LVL sLEARN_LVL, SNATIVEID NeedSkillID)
	{
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		if (!pCharacter)	return;

		CString	strText("");
		DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
		std::vector<SINFO_TEXT> vecText;

		if (0 < sLEARN_LVL.dwSKP)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_ADVANCED_INFO", 2), sLEARN_LVL.dwSKP);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx(pCharacter->m_dwSkillPoint >= sLEARN_LVL.dwSKP);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sLEARN_LVL.dwLEVEL)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_ADVANCED_INFO", 3), sLEARN_LVL.dwLEVEL);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx(pCharacter->GETLEVEL() >= int(sLEARN_LVL.dwLEVEL));
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}


		if (0 < sLEARN_LVL.sSTATS.wPow)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_ADVANCED_INFO_STATS", 0), sLEARN_LVL.sSTATS.wPow);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx(pCharacter->m_sSUMSTATS.wPow >= sLEARN_LVL.sSTATS.wPow);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sLEARN_LVL.sSTATS.wStr)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_ADVANCED_INFO_STATS", 1), sLEARN_LVL.sSTATS.wStr);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx(pCharacter->m_sSUMSTATS.wStr >= sLEARN_LVL.sSTATS.wStr);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sLEARN_LVL.sSTATS.wSpi)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_ADVANCED_INFO_STATS", 2), sLEARN_LVL.sSTATS.wSpi);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx(pCharacter->m_sSUMSTATS.wSpi >= sLEARN_LVL.sSTATS.wSpi);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sLEARN_LVL.sSTATS.wDex)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_ADVANCED_INFO_STATS", 3), sLEARN_LVL.sSTATS.wDex);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx(pCharacter->m_sSUMSTATS.wDex >= sLEARN_LVL.sSTATS.wDex);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sLEARN_LVL.sSTATS.wInt)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_ADVANCED_INFO_STATS", 4), sLEARN_LVL.sSTATS.wInt);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx(pCharacter->m_sSUMSTATS.wInt >= sLEARN_LVL.sSTATS.wInt);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}

		if (0 < sLEARN_LVL.sSTATS.wSta)
		{
			strText.Format("%s(%d)", ID2GAMEWORD("SKILL_ADVANCED_INFO_STATS", 5), sLEARN_LVL.sSTATS.wSta);
			dwTextColor = NS_UITEXTCONTROL::GetEvaluateColorEx(pCharacter->m_sSUMSTATS.wSta >= sLEARN_LVL.sSTATS.wSta);
			vecText.push_back(SINFO_TEXT(strText, dwTextColor));
		}


		int nIndex = 0;
		for (size_t i = 0; i < vecText.size(); ++i)
		{
			SINFO_TEXT& sData = vecText[i];
			if (i == 0)
			{
				strText.Format("[%s]", ID2GAMEWORD("SKILL_BASIC_INFO", 11));
				AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
			}

			AddTextNoSplit(sData.strText, sData.dwTextColor);
		}

		if (NeedSkillID != NATIVEID_NULL())
		{
			BOOL bVALID = FALSE;
			BOOL bNeedSkillLevel = FALSE;

			CString strNeedSkillName;
			CString strNeedSkillLevel;

			PGLSKILL pNeedSkill = GLSkillMan::GetInstance().GetData(NeedSkillID.wMainID, NeedSkillID.wSubID);
			strNeedSkillName.Format("%s:%s", ID2GAMEWORD("SKILL_ADVANCED_INFO", 0), pNeedSkill->GetName());
			bVALID = pCharacter->ISLEARNED_SKILL(NeedSkillID);

			if (0 < sLEARN_LVL.dwSKILL_LVL)
			{
				strNeedSkillLevel.Format("%s:%d", ID2GAMEWORD("SKILL_ADVANCED_INFO", 1), (sLEARN_LVL.dwSKILL_LVL + 1));
				bNeedSkillLevel = TRUE;

				SCHARDATA2::SKILL_MAP& map = pCharacter->m_ExpSkills;
				SCHARDATA2::SKILL_MAP_ITER iter = map.find(NeedSkillID.dwID);
				if (iter != map.end())
				{
					SCHARSKILL& rCharSkill = (*iter).second;
					bVALID = rCharSkill.wLevel >= sLEARN_LVL.dwSKILL_LVL;
				}
			}

			if (vecText.empty())
			{
				strText.Format("[%s]", ID2GAMEWORD("SKILL_BASIC_INFO", 11));
				AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
			}

			int nIndex = AddTextNoSplit(strNeedSkillName, NS_UITEXTCONTROL::GetEvaluateColorEx(bVALID));

			if (bNeedSkillLevel)
			{
				AddStringNoSplit(nIndex, " / ", NS_UITEXTCOLOR::WHITE);
				AddStringNoSplit(nIndex, strNeedSkillLevel, NS_UITEXTCONTROL::GetEvaluateColorEx(bVALID));
			}
		}

	}

	void AddInfoSkillRangeTargetSelf(const SKILL::CDATA_LVL sDATA_LVL, EMIMPACT_REALM emIMPACT_REALM, EMIMPACT_SIDE emIMPACT_SIDE, WORD wSKILLRANGE)
	{
		switch (emIMPACT_REALM)
		{
			case REALM_SELF:
			{
				AddTextNoSplit(ID2GAMEINTEXT("SKILL_AREA_SELF_SELF"), NS_UITEXTCOLOR::WHITE);
			}break;

			case REALM_ZONE:
			{
				switch (emIMPACT_SIDE)
				{
					case SIDE_OUR:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_SELF_ZONE_OUR")) % float(sDATA_LVL.wAPPLYRANGE / 10.0f) % sDATA_LVL.wTARNUM;
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;

					case SIDE_ENEMY:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_SELF_ZONE_ENEMY")) % float(sDATA_LVL.wAPPLYRANGE / 10.0f) % sDATA_LVL.wTARNUM;
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;

					case SIDE_ANYBODY:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_SELF_ZONE_ANYBODY")) % float(sDATA_LVL.wAPPLYRANGE / 10.0f) % sDATA_LVL.wTARNUM;
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;
				};
			}break;
		};
	}

	void AddInfoSkillRangeTargetSpec(const SKILL::CDATA_LVL sDATA_LVL, EMIMPACT_REALM emIMPACT_REALM, EMIMPACT_SIDE	emIMPACT_SIDE, WORD wSKILLRANGE)
	{
		switch (emIMPACT_REALM)
		{
			case REALM_SELF:
			{
				switch (emIMPACT_SIDE)
				{
					case SIDE_OUR:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_TARGET_SELF_OUR")) % float(wSKILLRANGE / 10.0f);
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;

					case SIDE_ENEMY:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_TARGET_SELF_ENEMY")) % float(wSKILLRANGE / 10.0f);
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;

					case SIDE_ANYBODY:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_TARGET_SELF_ANYBODY")) % float(wSKILLRANGE / 10.0f);
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;
				};
			}break;

			case REALM_ZONE:
			{
				switch (emIMPACT_SIDE)
				{
					case SIDE_OUR:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_TARGET_ZONE_OUR")) % float(wSKILLRANGE / 10.0f) % float(sDATA_LVL.wAPPLYRANGE / 10.0f) % sDATA_LVL.wTARNUM ;
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;

					case SIDE_ENEMY:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_TARGET_ZONE_ENEMY")) % float(wSKILLRANGE / 10.0f) % float(sDATA_LVL.wAPPLYRANGE / 10.0f) % sDATA_LVL.wTARNUM;
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;

					case SIDE_ANYBODY:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_TARGET_ZONE_ANYBODY")) % float(wSKILLRANGE / 10.0f) % float(sDATA_LVL.wAPPLYRANGE / 10.0f) % sDATA_LVL.wTARNUM;
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;
				};
			}break;

			case REALM_FANWIZE:
			{
				switch (emIMPACT_SIDE)
				{
					case SIDE_ENEMY:
					{
						boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_TARGET_FANWIZE_ENEMY"))  % float(wSKILLRANGE / 10.0f) % sDATA_LVL.wAPPLYANGLE % sDATA_LVL.wTARNUM;
						AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
					}break;
				};
			}break;
		};
	}

	void AddInfoSkillRangeTargetSelfToSpec(const SKILL::CDATA_LVL sDATA_LVL, EMIMPACT_REALM	emIMPACT_REALM, EMIMPACT_SIDE emIMPACT_SIDE, WORD wSKILLRANGE)
	{
		switch (emIMPACT_SIDE)
		{
			case SIDE_ENEMY:
			{
				boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_TOSPEC_ENEMY")) % float(wSKILLRANGE / 10.0f) % sDATA_LVL.wTARNUM % WORD(sDATA_LVL.wPIERCENUM+1);
				AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
			}break;
		};
	}

	void AddInfoSkillRangeTargetZone(const SKILL::CDATA_LVL sDATA_LVL, EMIMPACT_REALM emIMPACT_REALM, EMIMPACT_SIDE	emIMPACT_SIDE, WORD wSKILLRANGE)
	{
		switch (emIMPACT_SIDE)
		{
			case SIDE_OUR:
			{
				boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_ZONE_OUR"))  % float(sDATA_LVL.wAPPLYRANGE / 10.0f) % sDATA_LVL.wTARNUM;
				AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
			}break;

		case SIDE_ENEMY:
			{
				boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_ZONE_ENEMY")) % float(sDATA_LVL.wAPPLYRANGE / 10.0f) % sDATA_LVL.wTARNUM;
				AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
			}break;

		case SIDE_ANYBODY:
			{
				boost::format fmt = CreateFormat(ID2GAMEINTEXT("SKILL_AREA_ZONE_ANYBODY")) % float(sDATA_LVL.wAPPLYRANGE / 10.0f) % sDATA_LVL.wTARNUM;
				AddTextLongestLineSplit(fmt.str().c_str(), NS_UITEXTCOLOR::WHITE);
			}break;
		};
	}

	void AddInfoSkillRange(const SKILL::CDATA_LVL sDATA_LVL, const SKILL::SSKILLBASIC sBASIC)
	{
		AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);

		CString	strText;
		strText.Format("[%s]", ID2GAMEWORD("SKILL_BASIC_INFO", 12));
		AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

		EMIMPACT_TAR emIMPACT_TAR = sBASIC.emIMPACT_TAR;
		EMIMPACT_REALM emIMPACT_REALM = sBASIC.emIMPACT_REALM;
		EMIMPACT_SIDE emIMPACT_SIDE = sBASIC.emIMPACT_SIDE;
		WORD wSKILLRANGE = sBASIC.wTARRANGE;

		
		switch (emIMPACT_TAR)
		{
		case TAR_SELF:
			AddInfoSkillRangeTargetSelf(sDATA_LVL, emIMPACT_REALM, emIMPACT_SIDE, wSKILLRANGE);
			break;

		case TAR_SPEC:
			AddInfoSkillRangeTargetSpec(sDATA_LVL, emIMPACT_REALM, emIMPACT_SIDE, wSKILLRANGE);
			break;

		case TAR_SELF_TOSPEC:
			AddInfoSkillRangeTargetSelfToSpec(sDATA_LVL, emIMPACT_REALM, emIMPACT_SIDE, wSKILLRANGE);
			break;

		case TAR_ZONE:
			AddInfoSkillRangeTargetZone(sDATA_LVL, emIMPACT_REALM, emIMPACT_SIDE, wSKILLRANGE);
			break;

		default:
			AddTextLongestLineSplit(ID2GAMEINTEXT("SKILL_AREA_LAND"), NS_UITEXTCOLOR::WHITE);
			break;	
		};
	}

	void AddInfoSkillEffectBasic(bool& bSpace, SKILL::EMTYPES emType, const SKILL::CDATA_LVL sDATA_LVL, bool bPassive)
	{
		if (sDATA_LVL.fBASIC_VAR == 0.0f)	return;

		EMSKILL_BASIC_CASE_TYPE emCaseType = EMSKILL_BASIC_CASE_TYPE_NONE;

		if (bPassive)
		{
			if (0.0f == sDATA_LVL.fLIFE)
			{
				if (sDATA_LVL.fBASIC_VAR <= 0.0)
					emCaseType = EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_BASIC;
				else
					emCaseType = EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_BASIC;
			}
			else
			{
				if (sDATA_LVL.fBASIC_VAR <= 0.0)
					emCaseType = EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_TIMER;
				else
					emCaseType = EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_TIMER;
			}
		}
		else
		{
			if (0.0f == sDATA_LVL.fLIFE)
			{
				if (sDATA_LVL.fBASIC_VAR <= 0.0)
					emCaseType = EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_BASIC;
				else
					emCaseType = EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_BASIC;
			}
			else
			{
				if (sDATA_LVL.fBASIC_VAR <= 0.0)
					emCaseType = EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_TIMER;
				else
					emCaseType = EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_TIMER;
			}
		}

		bool bPercent = COMMENT::IsSKILL_PER(emType);
		float fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[emType];
		float fBASIC_VAR = sDATA_LVL.fBASIC_VAR;
		float fLIFE = sDATA_LVL.fLIFE;

		boost::format fmt;

		switch (emType)
		{
			case SKILL::EMFOR_HP:
			case SKILL::EMFOR_MP:
			case SKILL::EMFOR_SP:
			{
				switch (emCaseType)
				{
					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_TIMER:
					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE1_1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;

					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_TIMER:
					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE1_1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_TIMER:
					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;

					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_TIMER:
					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1_1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;


				};
			}break;

			case SKILL::EMFOR_VARHP:
			case SKILL::EMFOR_VARMP:
			case SKILL::EMFOR_VARSP:
			case SKILL::EMFOR_VARAP:
			{
				switch (emCaseType)
				{
					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3_1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3_1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;
				};
			}break;

			case SKILL::EMFOR_DEFENSE:
			case SKILL::EMFOR_HITRATE:
			case SKILL::EMFOR_AVOIDRATE:

			case SKILL::EMFOR_VARDAMAGE:
			case SKILL::EMFOR_VARDEFENSE:
			case SKILL::EMFOR_PA:
			case SKILL::EMFOR_SA:
			case SKILL::EMFOR_MA:

			case SKILL::EMFOR_RESIST:
			case SKILL::EMFOR_SUMMONTIME:
			{
				switch (emCaseType)
				{
					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;


					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

				};
			}break;

			case SKILL::EMFOR_CURE:
			{
				fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_INFOR_CURE");
			}break;

			case SKILL::EMFOR_HP_RATE:
			case SKILL::EMFOR_MP_RATE:
			case SKILL::EMFOR_SP_RATE:
			{
				switch (emCaseType)
				{
					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE6_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE6")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE6_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE6")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE4_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE4")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE4_1")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE4")) % fLIFE % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1")) % COMMENT::SKILL_TYPES[emType].c_str() % fBASIC_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1_1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1")) % COMMENT::SKILL_TYPES[emType].c_str() % -fBASIC_VAR;
					}break;

				};
			}break;

			
			
		};

		
		CString strText = fmt.str().c_str();
		if (strText.GetLength())
		{
			if (!bSpace)
			{
				AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);

				CString strTemp;
				strTemp.Format("[%s]", ID2GAMEWORD("SKILL_BASIC_INFO", 13));
				AddTextNoSplit(strTemp, NS_UITEXTCOLOR::UIINFO_OPTION2);

				bSpace = true;
			}

			AddTextLongestLineSplit(strText, NS_UITEXTCOLOR::UIINFO_OPTION2);
		}
	}

	void AddOnSkillInfo(bool& bSpace, EMIMPACT_ADDON emADDON, bool bPassive, float fVar, float fLife)
	{
		EMSKILL_BASIC_CASE_TYPE emCaseType = EMSKILL_BASIC_CASE_TYPE_NONE;

		if (bPassive)
		{
			if (0.0f == fLife)
			{
				if (fVar <= 0.0)
					emCaseType = EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_BASIC;
				else
					emCaseType = EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_BASIC;
			}
			else
			{
				if (fVar <= 0.0)
					emCaseType = EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_TIMER;
				else
					emCaseType = EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_TIMER;
			}
		}
		else
		{
			if (0.0f == fLife)
			{
				if (fVar <= 0.0)
					emCaseType = EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_BASIC;
				else
					emCaseType = EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_BASIC;
			}
			else
			{
				if (fVar <= 0.0)
					emCaseType = EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_TIMER;
				else
					emCaseType = EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_TIMER;
			}
		}

		int nMultiply = 1;
		if (fVar <= 0.0)
			nMultiply = -1;
		else
			nMultiply = 1;

		bool bPercent = COMMENT::IsIMPACT_ADDON_PER(emADDON);

		float fADDON_VAR_SCALE = fVar * COMMENT::IMPACT_ADDON_SCALE[emADDON];
		float fADDON_VAR = fVar;
		float fLIFE = fLife;

		boost::format fmt;

		switch (emADDON)
		{
		case EMIMPACTA_NONE:
		case EMIMPACTA_HITRATE:
		case EMIMPACTA_AVOIDRATE:

		case EMIMPACTA_DAMAGE:
		case EMIMPACTA_DEFENSE:

		case EMIMPACTA_VARHP:
		case EMIMPACTA_VARMP:
		case EMIMPACTA_VARSP:
		case EMIMPACTA_VARAP:

		case EMIMPACTA_DAMAGE_RATE:
		case EMIMPACTA_DEFENSE_RATE:

		case EMIMPACTA_PA:
		case EMIMPACTA_SA:
		case EMIMPACTA_MA:

		case EMIMPACTA_HP_RATE:
		case EMIMPACTA_MP_RATE:
		case EMIMPACTA_SP_RATE:

		case EMIMPACTA_RESIST:
		case EMIMPACTA_CHANGESTATS:
			{
				switch (emCaseType)
				{
					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")) % fLIFE % COMMENT::IMPACT_ADDON[emADDON].c_str() % fADDON_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")) % fLIFE % COMMENT::IMPACT_ADDON[emADDON].c_str() % fADDON_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")) % fLIFE % COMMENT::IMPACT_ADDON[emADDON].c_str() % -fADDON_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")) % fLIFE % COMMENT::IMPACT_ADDON[emADDON].c_str() % -fADDON_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")) % COMMENT::IMPACT_ADDON[emADDON].c_str() % fADDON_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")) % COMMENT::IMPACT_ADDON[emADDON].c_str() % fADDON_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_NORMAL_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")) % COMMENT::IMPACT_ADDON[emADDON].c_str() % -fADDON_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")) % COMMENT::IMPACT_ADDON[emADDON].c_str() % -fADDON_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")) % fLIFE % COMMENT::IMPACT_ADDON[emADDON].c_str() % fADDON_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")) % fLIFE % COMMENT::IMPACT_ADDON[emADDON].c_str() % fADDON_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_TIMER:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")) % fLIFE % COMMENT::IMPACT_ADDON[emADDON].c_str() % -fADDON_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")) % fLIFE % COMMENT::IMPACT_ADDON[emADDON].c_str() % -fADDON_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_INCREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")) % COMMENT::IMPACT_ADDON[emADDON].c_str() % fADDON_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")) % COMMENT::IMPACT_ADDON[emADDON].c_str() % fADDON_VAR;
					}break;

					case EMSKILL_BASIC_CASE_TYPE_PASSIVE_DECREASE_BASIC:
					{
						if (bPercent)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")) % COMMENT::IMPACT_ADDON[emADDON].c_str() % -fADDON_VAR_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")) % COMMENT::IMPACT_ADDON[emADDON].c_str() % -fADDON_VAR;
					}break;

				};

			}break;


		case EMIMPACTA_HP_RECOVERY_VAR:
			fmt = CreateFormat(ID2GAMEINTEXT("SKILL_IMPACT_ADDON_19")) % float(nMultiply * fVar) % fLife;
		break;

		case EMIMPACTA_MP_RECOVERY_VAR:
			fmt = CreateFormat(ID2GAMEINTEXT("SKILL_IMPACT_ADDON_20")) % float(nMultiply * fVar) % fLife;
		break;

		case EMIMPACTA_SP_RECOVERY_VAR:
			fmt = CreateFormat(ID2GAMEINTEXT("SKILL_IMPACT_ADDON_21")) % float(nMultiply * fVar) % fLife;
		break;

		case EMIMPACTA_CP_RECOVERY_VAR:
			fmt = CreateFormat(ID2GAMEINTEXT("SKILL_IMPACT_ADDON_22")) % float(nMultiply * fVar) % fLife;
		break;

		case EMIMPACTA_CP_AUTO_VAR:
			fmt = CreateFormat(ID2GAMEINTEXT("SKILL_IMPACT_ADDON_23")) % fADDON_VAR_SCALE % float(nMultiply * fVar) % fLife;
			break;
		};


		CString strText = fmt.str().c_str();
		if (strText.GetLength())
		{
			if (!bSpace)
			{
				AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);

				CString strTemp;
				strTemp.Format("[%s]", ID2GAMEWORD("SKILL_BASIC_INFO", 13));
				AddTextNoSplit(strTemp, NS_UITEXTCOLOR::UIINFO_OPTION2);

				bSpace = true;
			}

			AddTextLongestLineSplit(strText, NS_UITEXTCOLOR::UIINFO_OPTION2);
		}
	}

	void AddInfoSkillEffectAddOn(bool& bSpace, const SKILL::SAPPLY sAPPLY, bool bPassive, WORD wLEVEL )
	{
		for (int nImpactIndex = 0; nImpactIndex < SKILL::MAX_IMPACT; ++nImpactIndex)
		{
			EMIMPACT_ADDON	emADDON = sAPPLY.sImpacts[nImpactIndex].emADDON;
			const float fADDON_VAR = sAPPLY.sImpacts[nImpactIndex].fADDON_VAR[wLEVEL];
			const float fLIFE = sAPPLY.sDATA_LVL[wLEVEL].fLIFE;

			if (emADDON != EMIMPACTA_NONE)
			{
				AddOnSkillInfo(bSpace, emADDON, bPassive, fADDON_VAR, fLIFE);
			}
		}
	}
	
	void SpecSkillInfo(bool& bSpace, const SKILL::SSPEC sSPEC, const EMSPEC_ADDON emSpecAddOn, float fLife, bool bPassive, EMIMPACT_TAR emIMPACT_TAR, EMIMPACT_REALM emIMPACT_REALM, EMIMPACT_SIDE emIMPACT_SIDE, EMELEMENT emELEM )
	{
		float fVAR1_SCALE = sSPEC.fVAR1 * COMMENT::SPEC_ADDON_VAR1_SCALE[emSpecAddOn];
		float fVAR2_SCALE = sSPEC.fVAR2 * COMMENT::SPEC_ADDON_VAR2_SCALE[emSpecAddOn];
		float fLIFE = fLife;

		boost::format fmt;

		switch (emSpecAddOn)
		{
			case EMSPECA_PUSHPULL:
			{
				if (0.0 != fVAR2_SCALE)
				{
					if (fVAR1_SCALE <= 0.0)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_1_1")) % fVAR2_SCALE % fVAR1_SCALE;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_1")) % fVAR2_SCALE % fVAR1_SCALE;
				}
			}break;

			case EMSPECA_REFDAMAGE:
			{
				if (0.0 == fLIFE)
					fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_2_1");
				else
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_2")) % fLIFE;
			}break;

			case EMSPECA_REBIRTH:
				fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_3");
			break;

			case EMSPECA_HP_GATHER:
			{
				if (0.0 != fVAR1_SCALE)
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_4")) % fVAR1_SCALE;
			}break;

			case EMSPECA_MP_GATHER:
			{
				if (0.0 != fVAR1_SCALE)
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_5")) % fVAR1_SCALE;
			}break;

			case EMSPECA_SP_GATHER:
			{
				if (0.0 != fVAR1_SCALE)
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_6")) % fVAR1_SCALE;
			}break;


			case EMSPECA_HP_DIV:
				fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_7");
			break;

			case EMSPECA_MP_DIV:
				fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_8");
			break;

			case EMSPECA_SP_DIV:
				fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_9");
			break;

			case EMSPECA_NONBLOW:
			case EMSPECA_RECBLOW:
			{
				CString strTemp("");

				if (sSPEC.dwFLAG & DIS_NUMB)
					strTemp.AppendFormat("%s,", COMMENT::BLOW[EMBLOW_NUMB].c_str());

				if (sSPEC.dwFLAG & DIS_STUN)
					strTemp.AppendFormat("%s,", COMMENT::BLOW[EMBLOW_STUN].c_str());

				if (sSPEC.dwFLAG & DIS_STONE)
					strTemp.AppendFormat("%s,", COMMENT::BLOW[EMBLOW_STONE].c_str());

				if (sSPEC.dwFLAG & DIS_BURN)
					strTemp.AppendFormat("%s,", COMMENT::BLOW[EMBLOW_BURN].c_str());

				if (sSPEC.dwFLAG & DIS_FROZEN)
					strTemp.AppendFormat("%s,", COMMENT::BLOW[EMBLOW_FROZEN].c_str());

				if (sSPEC.dwFLAG & DIS_MAD)
					strTemp.AppendFormat("%s,", COMMENT::BLOW[EMBLOW_MAD].c_str());

				if (sSPEC.dwFLAG & DIS_POISON)
					strTemp.AppendFormat("%s,", COMMENT::BLOW[EMBLOW_POISON].c_str());

				if (sSPEC.dwFLAG & DIS_CURSE)
					strTemp.AppendFormat("%s,", COMMENT::BLOW[EMBLOW_CURSE].c_str());

				if (strTemp.GetLength() <= 0 )
					strTemp.AppendFormat("%s,", "None" );

				if (emSpecAddOn == EMSPECA_NONBLOW)	
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_10")) % strTemp;
				else if (emSpecAddOn == EMSPECA_RECBLOW)
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_11")) % strTemp;
			}break;

			case EMSPECA_PIERCE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (0.0 == fLIFE)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_12_1")) % fVAR1_SCALE;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_12")) % fLIFE % fVAR1_SCALE;
				}
			}break;

			case EMSPECA_TARRANGE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (fVAR1_SCALE <= 0.0)
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_13_3")) % -fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_13_1")) % fLIFE % -fVAR1_SCALE;
					}
					else
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_13_2")) % fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_13")) % fLIFE % fVAR1_SCALE;
					}
				}
			
			}break;

			case EMSPECA_MOVEVELO:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (fVAR1_SCALE <= 0.0)
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_14_3")) % -fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_14_1")) % fLIFE % -fVAR1_SCALE;
					}
					else
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_14_2")) % fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_14")) % fLIFE % fVAR1_SCALE;
					}
				}
				
			}break;

			/* dash skill logic, Juver, 2020/12/14 */
			case EMSPECA_SKILL_DASH:
				fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_15")) % fVAR1_SCALE % fVAR2_SCALE;
			break;

			/* skill turn/find invisible, Juver, 2020/04/28 */
			case EMSPECA_TURN_INVISIBLE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_16")) % fLIFE % WORD(fVAR1_SCALE);
					
				}
			}break;

			/* skill turn/find invisible, Juver, 2020/04/28 */
			case EMSPECA_SHOW_INVISIBLE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_17")) % fLIFE % WORD(fVAR1_SCALE) % WORD(fVAR2_SCALE);
				}
			}break;
			
			case EMSPECA_ATTACKVELO:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (fVAR1_SCALE >= 0.0)
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_18_3")) % fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_18_1")) % fLIFE % fVAR1_SCALE;
					}
					else
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_18_2")) % -fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_18")) % fLIFE % -fVAR1_SCALE;
					}
				}
			}break;

			/* skill delay addition, Juver, 2020/12/08 */
			case EMSPECA_SKILLDELAY:
			{
				if (0.0 != fVAR2_SCALE)
				{
					int nMultiplier = 1;
					CString strTemp("");
					if (fVAR1_SCALE >= 0.0)
					{
						nMultiplier = 1;
						strTemp = "Increased";
					}
					else
					{
						nMultiplier = -1;
						strTemp = "Decreased";
					}

					if ( ( sSPEC.dwFLAG & SKILL::SPEC_SKILL_DELAY_TYPE_SINGLE ) && ( sSPEC.dwFLAG & SKILL::SPEC_SKILL_DELAY_TYPE_DURATION ) )
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_19_2")) % fVAR2_SCALE % fLIFE % float(nMultiplier * fVAR1_SCALE) % strTemp;
					}

					if ( sSPEC.dwFLAG & SKILL::SPEC_SKILL_DELAY_TYPE_SINGLE )
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_19")) % fVAR2_SCALE % float(nMultiplier * fVAR1_SCALE) % strTemp;
					}

					if ( sSPEC.dwFLAG & SKILL::SPEC_SKILL_DELAY_TYPE_DURATION )
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_19_1")) % fVAR2_SCALE % fLIFE % float(nMultiplier * fVAR1_SCALE) % strTemp;
					}

					if ( sSPEC.dwFLAG == SKILL::SPEC_SKILL_DELAY_TYPE_OLD )
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_19_3")) % fVAR1_SCALE;
					}
				}
				else
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_19_3")) %  fVAR1_SCALE;
				}
			}break;

			case EMSPECA_CRUSHING_BLOW:
			{
				if (0.0 != fVAR2_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_20")) % fVAR2_SCALE % GLCONST_CHAR::fPUSHPULL_VELO % fVAR1_SCALE;
				}
			}break;

			case EMSPECA_PSY_DAMAGE_REDUCE:
			{
				if (0.0 == fLIFE)
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_21_1")) % fVAR1_SCALE;
				else
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_21")) % fLIFE % fVAR1_SCALE;
			}break;

			case EMSPECA_MAGIC_DAMAGE_REDUCE:
			{
				if (0.0 == fLIFE)
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_22_1")) % fVAR1_SCALE;
				else
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_22")) % fLIFE % fVAR1_SCALE;
			}break;

			case EMSPECA_PSY_DAMAGE_REFLECTION:
			{
				if (0.0 != fVAR2_SCALE)
				{
					if (0.0 == fLIFE)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_23_1")) % fVAR1_SCALE % fVAR2_SCALE;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_23")) % fLIFE % fVAR2_SCALE % fVAR1_SCALE;
				}
				
			}break;

			case EMSPECA_MAGIC_DAMAGE_REFLECTION:
			{
				if (0.0 != fVAR2_SCALE)
				{
					if (0.0 == fLIFE)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_24_1")) % fVAR1_SCALE % fVAR2_SCALE;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_24")) % fLIFE % fVAR2_SCALE % fVAR1_SCALE;
				}

			}break;

			case EMSPECA_BUFF_REMOVE:
			{
				if (sSPEC.dwFLAG == 0)
					fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_25_0");

				if (sSPEC.dwFLAG == 1)
					fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_25_1");

				if (sSPEC.dwFLAG == 2)
					fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_25_2");

				if (sSPEC.dwFLAG == 3)
					fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_25_3");
			}break;

			case EMSPECA_STUN:
			{
				if (0.0 == fLIFE)
					fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_26_1");
				else
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_26")) % fLIFE;
			}break;

			case EMSPECA_DEFENSE_SKILL_ACTIVE:
			{
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData(sSPEC.dwNativeID);
				if (pSkill)
				{
					CString strTemp;
					strTemp.Format("%s Lv%d", pSkill->GetName(), sSPEC.dwFLAG);

					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_27")) % strTemp % fVAR1_SCALE;
				}
				
			}break;

			/* pull skill logic, Juver, 2020/12/14 */
			case EMSPECA_PULL:
			{
				if (0.0 != fVAR1_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_28")) % fVAR1_SCALE % fVAR2_SCALE;
				}
			}break;

			/* push skill logic, Juver, 2020/12/14 */
			case EMSPECA_PUSH:
			{
				if (0.0 != fVAR1_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_29")) % fVAR1_SCALE % GLCONST_CHAR::fPUSHPULL_VELO % fVAR2_SCALE;
				}
			}break;

			case EMSPECA_CONTINUOUS_DAMAGE:
			{
				if (0.0 != fLIFE)
				{
					if (fVAR2_SCALE <= 0.0)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_30_1")) % fLIFE % fVAR1_SCALE % float(fVAR2_SCALE * -1.0);
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_30")) % fLIFE % fVAR1_SCALE % fVAR2_SCALE;
				}
			}break;

			case EMSPECA_CURSE:
			{
				if (0.0 == fLIFE)
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_31_1")) % fVAR1_SCALE;
				else
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_31")) % fLIFE % fVAR1_SCALE;
			}break;

			case EMSPECA_CHANGE_ATTACK_RANGE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					int nMultiplier = 1;
					CString strTemp("");
					if (fVAR1_SCALE >= 0.0)
					{
						nMultiplier = 1;
						strTemp = "Increased";
					}
					else
					{
						nMultiplier = -1;
						strTemp = "Decreased";
					}

					if (0.0 == fLIFE)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_32_1")) % float(nMultiplier * fVAR1_SCALE) % strTemp;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_32")) % fLIFE % float(nMultiplier * fVAR1_SCALE) % strTemp;;
				}
				
			}break;

			case EMSPECA_CHANGE_APPLY_RANGE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (fVAR1_SCALE <= 0.0)
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_33_3")) % -fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_33_2")) % fLIFE % -fVAR1_SCALE;
					}
					else
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_33_1")) % fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_33")) % fLIFE % fVAR1_SCALE;
					}
				}
			}break;

			case EMSPECA_PROHIBIT_POTION:
			{
				if (0.0 != fLIFE)
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_34")) % fLIFE;
				else
					fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_34_1");
			}break;

			case EMSPECA_PROHIBIT_SKILL:
			{
				if (0.0 == fLIFE)
					fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_35_1");
				else
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_35")) % fLIFE;
			}break;

			case EMSPECA_IGNORE_DAMAGE:
			{
				if (0.0 != fVAR1_SCALE && 0.0 != fVAR2_SCALE )
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_36_2")) % fLIFE % fVAR1_SCALE % fVAR2_SCALE;
				}
				else
				{
					if (0.0 != fVAR2_SCALE)
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_36_1")) % fLIFE % fVAR2_SCALE;
					}
					else if (0.0 != fVAR1_SCALE)
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_36")) % fLIFE % fVAR1_SCALE;
					}
					else
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_36_3")) % fLIFE;
					}
				}
			}break;

			case EMSPECA_ITEMDROP_RATE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					CString strTemp("");
					if (fVAR1_SCALE >= 0.0)

						strTemp = "Increased";
					else
						strTemp = "Decreased";

					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_37")) % strTemp % fVAR1_SCALE;
				}
			}break;

			case EMSPECA_MONEYDROP_RATE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					CString strTemp("");
					if (fVAR1_SCALE >= 0.0)

						strTemp = "Increased";
					else
						strTemp = "Decreased";

					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_38")) % strTemp % fVAR1_SCALE;
				}
			}break;

			case EMSPECA_GETEXP_RATE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (fVAR1_SCALE <= 0.0)
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_39_3")) % -fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_39_1")) % fLIFE % -fVAR1_SCALE;
					}
					else
					{
						if (0.0 == fLIFE)
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_39_2")) % fVAR1_SCALE;
						else
							fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_39")) % fLIFE % fVAR1_SCALE;
					}
				}
			}break;

			case EMSPECA_TALK_TO_NPC:
			{
				fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_40");
			}break;

			case EMSPECA_SPECIAL_ITEM_DROP:
			{
				fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_41");
			}break;

			case EMSPECA_TELEPORT:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (emIMPACT_TAR && emIMPACT_TAR != TAR_SPEC && emIMPACT_TAR != TAR_SELF_TOSPEC)
						fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_42_1");
					else
						fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_42");
				}
			}break;

			/* skill position shift, Juver, 2020/12/15 */
			case EMSPECA_POSITION_SHIFT:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (sSPEC.dwFLAG == SKILL::EMSPEC_POSITION_SHIFT_TYPE_SWAP)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_43")) % fVAR1_SCALE;
					else if (sSPEC.dwFLAG == SKILL::EMSPEC_POSITION_SHIFT_TYPE_DASH)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_43_1")) % fVAR1_SCALE;
					else if (sSPEC.dwFLAG == SKILL::EMSPEC_POSITION_SHIFT_TYPE_PULL)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_43_2")) % fVAR1_SCALE;
				}
				
			}break;

			/* skill amplify, Juver, 2020/12/09 */
			case EMSPECA_AMPLIFY:
			{
				CString strFlag;

				if (sSPEC.dwFLAG & SKILL::EMAPPLY_FLAG_PHY_SHORT)
				{
					if (strFlag.GetLength() > 0)	strFlag += "/";
					strFlag += COMMENT::SKILL_APPLY[SKILL::EMAPPLY_PHY_SHORT].c_str();
				}

				if (sSPEC.dwFLAG & SKILL::EMAPPLY_FLAG_PHY_LONG)
				{
					if (strFlag.GetLength() > 0)	strFlag += "/";
					strFlag += COMMENT::SKILL_APPLY[SKILL::EMAPPLY_PHY_LONG].c_str();
				}

				if (sSPEC.dwFLAG & SKILL::EMAPPLY_FLAG_MAGIC)
				{
					if (strFlag.GetLength() > 0)	strFlag += "/";
					strFlag += COMMENT::SKILL_APPLY[SKILL::EMAPPLY_MAGIC].c_str();
				}

				int nMultiplier1 = 1;
				CString strChange1("");
				if (fVAR1_SCALE >= 0.0)
				{
					nMultiplier1 = 1;
					strChange1 = "Increased";
				}
				else
				{
					nMultiplier1 = -1;
					strChange1 = "Decreased";
				}

				int nMultiplier2 = 1;
				CString strChange2("");
				if (fVAR2_SCALE >= 0.0)
				{
					nMultiplier2 = 1;
					strChange2 = "Increased";
				}
				else
				{
					nMultiplier2 = -1;
					strChange2 = "Decreased";
				}

				if (0.0 != fVAR1_SCALE && 0.0 != fVAR2_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_44"))
						% fLIFE 
						% strFlag 
						% float(nMultiplier1 * fVAR1_SCALE) 
						% strChange1
						% float(nMultiplier2 * fVAR2_SCALE)
						% strChange2;
				}
				else
				{
					if (0.0 != fVAR1_SCALE)
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_44_1")) % fLIFE % strFlag % float(nMultiplier1 * fVAR1_SCALE) % strChange1;
					}

					if (0.0 != fVAR2_SCALE)
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_44_2")) % fLIFE % strFlag % float(nMultiplier2 * fVAR2_SCALE) % strChange2;
					}
				}
				
			}break;

			case EMSPECA_ILLUSION:
			{
				if (0.0 != fVAR2_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_45")) % fLIFE;
				}
			}break;

			case EMSPECA_VEHICLE_OFF:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (fLIFE <= 0.0 || 1.0 != fVAR2_SCALE)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_45")) % fVAR1_SCALE;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_46_1")) % fLIFE % fVAR1_SCALE;
				}
			}break;

			/* skill buff duration change, Juver, 2020/12/16 */
			case EMSPECA_DURATION_CHANGE:
			{
				CString strFlag;

				if (sSPEC.dwFLAG & SKILL::EMACTION_FLAG_NORMAL)
				{
					if (strFlag.GetLength() > 0)	strFlag += "/";
					strFlag += COMMENT::SKILL_ACTION_TYPE[SKILL::EMACTION_NORMAL].c_str();
				}

				if (sSPEC.dwFLAG & SKILL::EMACTION_FLAG_BUFF)
				{
					if (strFlag.GetLength() > 0)	strFlag += "/";
					strFlag += COMMENT::SKILL_ACTION_TYPE[SKILL::EMACTION_BUFF].c_str();
				}

				if (sSPEC.dwFLAG & SKILL::EMACTION_FLAG_DEBUFF)
				{
					if (strFlag.GetLength() > 0)	strFlag += "/";
					strFlag += COMMENT::SKILL_ACTION_TYPE[SKILL::EMACTION_DEBUFF].c_str();
				}

				if (sSPEC.dwFLAG & SKILL::EMACTION_FLAG_LIMIT)
				{
					if (strFlag.GetLength() > 0)	strFlag += "/";
					strFlag += COMMENT::SKILL_ACTION_TYPE[SKILL::EMACTION_LIMIT].c_str();
				}

				int nMultiplier1 = 1;
				CString strChange1("");
				if (fVAR1_SCALE >= 0.0)
				{
					nMultiplier1 = 1;
					strChange1 = "Increased";
				}
				else
				{
					nMultiplier1 = -1;
					strChange1 = "Decreased";
				}

				int nMultiplier2 = 1;
				CString strChange2("");
				if (fVAR2_SCALE >= 0.0)
				{
					nMultiplier2 = 1;
					strChange2 = "Increased";
				}
				else
				{
					nMultiplier2 = -1;
					strChange2 = "Decreased";
				}

				if (0.0 != fVAR1_SCALE && 0.0 != fVAR2_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_47_1"))
						% strFlag
						% float(nMultiplier1 * fVAR1_SCALE)
						% strChange1
						% fLIFE
						% float(nMultiplier2 * fVAR2_SCALE)
						% strChange2;
				}
				else
				{
					if (0.0 != fVAR1_SCALE)
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_47")) % strFlag % float(nMultiplier1 * fVAR1_SCALE) % strChange1;
					}

					if (0.0 != fVAR2_SCALE)
					{
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_47_2")) % fLIFE % strFlag % float(nMultiplier2 * fVAR2_SCALE) % strChange2;
					}
				}
			}break;

			/* skill stigma, Juver, 2020/12/11 */
			case EMSPECA_STIGMA:
			{
				if (0.0 != fVAR2_SCALE)
				{
					CString strFlag;

					if (sSPEC.dwFLAG & SKILL::EMSPEC_STIGMA_TYPE_FLAG_ATTACK)
					{
						if (strFlag.GetLength() > 0)	strFlag += "/";
						strFlag += COMMENT::SPEC_STIGMA_TYPE[SKILL::EMSPEC_STIGMA_TYPE_ATTACK].c_str();
					}

					if (sSPEC.dwFLAG & SKILL::EMSPEC_STIGMA_TYPE_FLAG_SKILL)
					{
						if (strFlag.GetLength() > 0)	strFlag += "/";
						strFlag += COMMENT::SPEC_STIGMA_TYPE[SKILL::EMSPEC_STIGMA_TYPE_SKILL].c_str();
					}

					if (sSPEC.dwFLAG & SKILL::EMSPEC_STIGMA_TYPE_FLAG_BUFF)
					{
						if (strFlag.GetLength() > 0)	strFlag += "/";
						strFlag += COMMENT::SPEC_STIGMA_TYPE[SKILL::EMSPEC_STIGMA_TYPE_BUFF].c_str();
					}

					if (sSPEC.dwFLAG & SKILL::EMSPEC_STIGMA_TYPE_FLAG_DEBUFF)
					{
						if (strFlag.GetLength() > 0)	strFlag += "/";
						strFlag += COMMENT::SPEC_STIGMA_TYPE[SKILL::EMSPEC_STIGMA_TYPE_DEBUFF].c_str();
					}

					if (sSPEC.dwFLAG & SKILL::EMSPEC_STIGMA_TYPE_FLAG_LIMIT)
					{
						if (strFlag.GetLength() > 0)	strFlag += "/";
						strFlag += COMMENT::SPEC_STIGMA_TYPE[SKILL::EMSPEC_STIGMA_TYPE_LIMIT].c_str();
					}

					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_48")) % fLIFE % strFlag % fVAR2_SCALE;
				}
			}break;

			case EMSPECA_TRANSFORM:
			{
				if (0.0 != fVAR1_SCALE)
				{
					//this codes assumes theres only 2 types of transforms
					//needs to be updated when new type is added
					if (sSPEC.dwFLAG == SKILL::EMSPEC_TRANSFORM_TYPE_CHANGE_SKIN)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_49")) % fLIFE ;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_49_1")) % fLIFE ;
				}		
			}break;

			/* skill effect release, Juver, 2020/04/30 */
			case EMSPECA_EFFECT_RELEASE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_50")) % fVAR1_SCALE;
				}
			}break;

			case EMSPECA_DAMAGE_LOOP:
			{
				if (fLIFE <= 0.0 || 1.0 != fVAR2_SCALE)
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_51")) % fLIFE;
				else
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_51_1")) % fLIFE;
			}break;

			case EMSPECA_PET_OFF:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (0.0 == fLIFE)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_52")) % fVAR1_SCALE;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_52_1")) % fVAR1_SCALE % fLIFE;
				}
			}break;

			/* immune skill logic, Juver, 2020/12/25 */
			case EMSPECA_IMMUNE:
			{
				if ( ( 0.0 != fVAR1_SCALE ) || ( 0.0 != fVAR2_SCALE ) )
				{
					CString strApplyFlag("");
					DWORD dwApplyFlag = DWORD(fVAR1_SCALE);

					if (dwApplyFlag & SKILL::EMAPPLY_FLAG_PHY_SHORT)
					{
						if (strApplyFlag.GetLength() > 0)	strApplyFlag += "/";
						strApplyFlag += COMMENT::SKILL_APPLY[SKILL::EMAPPLY_PHY_SHORT].c_str();
					}

					if (dwApplyFlag & SKILL::EMAPPLY_FLAG_PHY_LONG)
					{
						if (strApplyFlag.GetLength() > 0)	strApplyFlag += "/";
						strApplyFlag += COMMENT::SKILL_APPLY[SKILL::EMAPPLY_PHY_LONG].c_str();
					}

					if (dwApplyFlag & SKILL::EMAPPLY_FLAG_MAGIC)
					{
						if (strApplyFlag.GetLength() > 0)	strApplyFlag += "/";
						strApplyFlag += COMMENT::SKILL_APPLY[SKILL::EMAPPLY_MAGIC].c_str();
					}


					CString strActionFlag("");
					DWORD dwActionFlag = DWORD(fVAR2_SCALE);

					if (dwActionFlag & SKILL::EMACTION_FLAG_NORMAL)
					{
						if (strActionFlag.GetLength() > 0)	strActionFlag += "/";
						strActionFlag += COMMENT::SKILL_ACTION_TYPE[SKILL::EMACTION_NORMAL].c_str();
					}

					if (dwActionFlag & SKILL::EMACTION_FLAG_BUFF)
					{
						if (strActionFlag.GetLength() > 0)	strActionFlag += "/";
						strActionFlag += COMMENT::SKILL_ACTION_TYPE[SKILL::EMACTION_BUFF].c_str();
					}

					if (dwActionFlag & SKILL::EMACTION_FLAG_DEBUFF)
					{
						if (strActionFlag.GetLength() > 0)	strActionFlag += "/";
						strActionFlag += COMMENT::SKILL_ACTION_TYPE[SKILL::EMACTION_DEBUFF].c_str();
					}

					if (dwActionFlag & SKILL::EMACTION_FLAG_LIMIT)
					{
						if (strActionFlag.GetLength() > 0)	strActionFlag += "/";
						strActionFlag += COMMENT::SKILL_ACTION_TYPE[SKILL::EMACTION_LIMIT].c_str();
					}

					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_53")) % fLIFE % strApplyFlag % strActionFlag ;
				}
			}break;

			/* skill hostile, Juver, 2020/12/16 */
			case EMSPECA_HOSTILE_ON:
			{
				if (0.0 != fVAR1_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_54")) % fVAR1_SCALE % fVAR2_SCALE;
				}
			}break;

			/* skill hostile, Juver, 2020/12/16 */
			case EMSPECA_HOSTILE_OFF:
				fmt = CreateFormat("%s") % ID2GAMEINTEXT("SKILL_SPEC_ADDON_55");
			break;

			case EMSPECA_TARGET_FIX:
			{
				if (0.0 != fVAR1_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_56")) % fVAR1_SCALE % fLIFE % fVAR2_SCALE;
				}
			}break;

			case EMSPECA_MANIPULATE:
			{
				if (0.0 != fVAR1_SCALE)
				{
					if (sSPEC.dwFLAG == SKILL::SPEC_MANIPULATE_TYPE_1)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_57")) % fVAR1_SCALE % fLIFE % fVAR2_SCALE;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_57_1")) % fVAR1_SCALE % fLIFE % fVAR2_SCALE;
				}
			}break;

			case EMSPECA_RANDOM_EXP_RATE:
			{
				float fExpGet = 1.0f; //need more data

				DWORD dwOverlaps = DWORD(fVAR2_SCALE);

				if (dwOverlaps == 1)
				{
					if (fExpGet <= 0.0)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_58_3")) % fExpGet % dwOverlaps;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_58_2")) % fExpGet % dwOverlaps;
				}
				else
				{
					if (fExpGet <= 0.0)
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_58_3")) % fExpGet % dwOverlaps;
					else
						fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_58_1")) % fExpGet % dwOverlaps;
				}
			}break;

			case EMSPECA_SKILLLINK:
			{
				//if (!a8 || 0.0 == fVAR1_SCALE)

				if ( 0.0 != fVAR1_SCALE)
				{
					fmt = CreateFormat(ID2GAMEINTEXT("SKILL_SPEC_ADDON_59")) %  float(fVAR1_SCALE * 100.0);
				}
			}break;
		};

		CString strText = fmt.str().c_str();
		if (strText.GetLength())
		{
			if (!bSpace)
			{
				AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);

				CString strTemp;
				strTemp.Format("[%s]", ID2GAMEWORD("SKILL_BASIC_INFO", 13));
				AddTextNoSplit(strTemp, NS_UITEXTCOLOR::UIINFO_OPTION2);

				bSpace = true;
			}

			AddTextLongestLineSplit(strText, NS_UITEXTCOLOR::UIINFO_OPTION2);
		}
	}

	void AddInfoSkillEffectSpec(bool& bSpace, const SKILL::SAPPLY sAPPLY, bool bPassive, WORD wLEVEL, EMIMPACT_TAR emIMPACT_TAR, EMIMPACT_REALM emIMPACT_REALM, EMIMPACT_SIDE emIMPACT_SIDE)
	{
		for (int nSpecIndex = 0; nSpecIndex < SKILL::MAX_SPEC; ++nSpecIndex)
		{
			const SKILL::SSPEC& sSPEC = sAPPLY.sSpecs[nSpecIndex].sSPEC[wLEVEL];
			const EMSPEC_ADDON emSpecAddOn = sAPPLY.sSpecs[nSpecIndex].emSPEC;
			const float fLIFE = sAPPLY.sDATA_LVL[wLEVEL].fLIFE;

			if (emSpecAddOn != EMSPECA_NULL)
			{
				SpecSkillInfo(bSpace, sSPEC, emSpecAddOn, fLIFE, bPassive, emIMPACT_TAR, emIMPACT_REALM, emIMPACT_SIDE, sAPPLY.emELEMENT);
			}
		}

	}

	void AddInfoSkillEffectBlow(EMSTATE_BLOW emSTATE_BLOW, const SKILL::SSTATE_BLOW sSTATE_BLOW, float fLife)
	{
		if (emSTATE_BLOW == EMBLOW_NONE)	return;

		AddTextNoSplit(" ", NS_UITEXTCOLOR::UIINFO_OPTION2);

		float fVAR1_SCALE = sSTATE_BLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emSTATE_BLOW];
		float fVAR2_SCALE = sSTATE_BLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emSTATE_BLOW];

		float fRate = sSTATE_BLOW.fRATE;
		float fLIFE = fLife;

		BOOL bValidVar1 = (fVAR1_SCALE != 0.0);
		BOOL bValidVar2 = (fVAR2_SCALE != 0.0);
		BOOL bValidVar3 = bValidVar1 && bValidVar2;

		if (fVAR1_SCALE < 0.0)	fVAR1_SCALE = -fVAR1_SCALE;
		if (fVAR2_SCALE < 0.0)	fVAR2_SCALE = -fVAR2_SCALE;

		boost::format fmt;

		switch (emSTATE_BLOW)
		{
			case EMBLOW_NUMB:
			{
				if (bValidVar3)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_NUMB")) % fRate % fLIFE % fVAR1_SCALE % fVAR2_SCALE;
				else if (bValidVar1)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_NUMB_1")) % fRate % fLIFE % fVAR1_SCALE;
				else if (bValidVar2)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_NUMB_2")) % fRate % fLIFE % fVAR2_SCALE;
			}break;

			case EMBLOW_STUN:
			{
				fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_STUN")) % fRate % fLIFE;
			}break;

			case EMBLOW_STONE:
			{
				if (bValidVar3)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_STONE")) % fRate % fLIFE % fVAR1_SCALE % fVAR2_SCALE;
				else if (bValidVar1)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_STONE_1")) % fRate % fLIFE % fVAR1_SCALE;
				else if (bValidVar2)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_STONE_2")) % fRate % fLIFE % fVAR2_SCALE;
			}break;

			case EMBLOW_BURN:
			{
				if (bValidVar2)
				{
					if (fVAR2_SCALE < 0.0)	fVAR2_SCALE = -fVAR2_SCALE;
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_BURN")) % fRate % fLIFE % fVAR2_SCALE;
				}
				else if (bValidVar1)
				{
					if (fVAR1_SCALE < 0.0)	fVAR1_SCALE = -fVAR1_SCALE;
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_BURN")) % fRate % fLIFE % fVAR1_SCALE;
				}
			}break;

			case EMBLOW_FROZEN:
			{
				if (bValidVar3)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_FROZEN")) % fRate % fLIFE % fVAR1_SCALE % fVAR2_SCALE;
				else if (bValidVar1)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_FROZEN_1")) % fRate % fLIFE % fVAR1_SCALE;
				else if (bValidVar2)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_FROZEN_2")) % fRate % fLIFE % fVAR2_SCALE;
			}break;

			case EMBLOW_MAD:
			{
				if (bValidVar3)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_MAD")) % fRate % fLIFE % fVAR1_SCALE % fVAR2_SCALE;
				else if (bValidVar1)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_MAD_1")) % fRate % fLIFE % fVAR1_SCALE;
				else if (bValidVar2)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_MAD_2")) % fRate % fLIFE % fVAR2_SCALE;
			}break;

			case EMBLOW_POISON:
			{
				if (bValidVar2)
				{
					if (fVAR2_SCALE < 0.0)	fVAR2_SCALE = -fVAR2_SCALE;
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_POISON")) % fRate % fLIFE % fVAR2_SCALE;
				}
				else if (bValidVar1)
				{
					if (fVAR1_SCALE < 0.0)	fVAR1_SCALE = -fVAR1_SCALE;
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_POISON")) % fRate % fLIFE % fVAR1_SCALE;
				}

			}break;

			case EMBLOW_CURSE:
			{
				if (bValidVar2)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_CURSE")) % fRate % fLIFE % fVAR2_SCALE;
				else if (bValidVar1)
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_CURSE")) % fRate % fLIFE % fVAR1_SCALE;
				else
					fmt = CreateFormat(ID2GAMEINTEXT("DES_STATE_BLOW_CURSE_1")) % fRate;
			}break;
		};

		CString strText = fmt.str().c_str();
		if (strText.GetLength())
			AddTextLongestLineSplit(strText, NS_UITEXTCOLOR::UIINFO_PLUS);

	}

	void LOAD(GLSKILL* const pSkill, SCHARSKILL* const pCharSkill, BOOL bNextLevel)
	{
		WORD wDispLevel = 0;
		if (pCharSkill)	
		{
			wDispLevel = pCharSkill->wLevel + 1;
		}

		BOOL bMASTER = GLGaeaClient::GetInstance().GetCharacter()->ISMASTER_SKILL(pSkill->m_sBASIC.sNATIVEID);

		if (bNextLevel&&bMASTER)
			bNextLevel = FALSE;
		
		//icon 
		SetIconImage(pSkill->m_sEXT_DATA.sICONINDEX, pSkill->m_sEXT_DATA.strICONFILE.c_str());

		//name
		{
			CString strText("");
			CString strBLANKSPACE = "       ";

			if (pCharSkill && bMASTER) //skill is learned and is mastered
			{
				strText.Format("%s%s (%s)", strBLANKSPACE.GetString(), pSkill->GetName(), ID2GAMEWORD("SKILL_ADDITIONAL_INFO_ETC", 0));
			}
			else if (pCharSkill) //skill is learned not mastered
			{
				strText.Format("%s%s (%s:%d)", strBLANKSPACE.GetString(), pSkill->GetName(), ID2GAMEWORD("SKILL_ADDITIONAL_INFO_ETC", 1), bNextLevel ? wDispLevel + 1 : wDispLevel);
			}
			else // skill is not learned
			{
				strText.Format("%s%s", strBLANKSPACE.GetString(), pSkill->GetName());
			}

			const int nNameMinSize = 50;
			if (strText.GetLength() < nNameMinSize)
			{
				std::string strBlank(nNameMinSize - strText.GetLength(), ' ');
				strText.Format("%s%s", strText.GetString(), strBlank.c_str());
			}

			AddTitle(strText, NS_UITEXTCOLOR::WHITE);
		}

		//move 1 line
		{
			AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);
		}

		bool bPASSIVE = (pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE);

		//grade, element type
		{
			CString strText("");
			CString strBLANKSPACE("         ");
			strText.Format("%s%s:%d / %s", strBLANKSPACE.GetString(), ID2GAMEWORD("SKILL_ADDITIONAL_INFO_ETC", 2), pSkill->m_sBASIC.dwGRADE, COMMENT::ELEMENT[pSkill->m_sAPPLY.emELEMENT].c_str());

			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

			//action type and apply type
			if (bPASSIVE)
			{
				strText.Format("%s%s / %s", strBLANKSPACE.GetString(), COMMENT::SKILL_ACTION_TYPE[pSkill->m_sBASIC.emACTION].c_str(), COMMENT::SKILL_ROLE[pSkill->m_sBASIC.emROLE].c_str());
			}
			else 
			{
				strText.Format("%s%s / %s", strBLANKSPACE.GetString(), COMMENT::SKILL_ACTION_TYPE[pSkill->m_sBASIC.emACTION].c_str(), COMMENT::SKILL_APPLY[pSkill->m_sBASIC.emAPPLY].c_str());
			}

			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
		}

		if( pCharSkill )
		{
			CString strText("");

			CTime cTime(pCharSkill->tBORNTIME);
			if ( cTime.GetYear()!=1970 )
			{
				SITEM* pItemData = GLItemMan::GetInstance().GetItem ( pCharSkill->sItemID );
				if( pItemData )
				{
					CTimeSpan sLMT(pItemData->sDrugOp.tTIME_LMT);
					cTime += sLMT;

					CString strExpireDate;
					strExpireDate = CInnerInterface::GetInstance().MakeString ( ID2GAMEWORD("ITEM_EXPIRE_DATE"),
						(cTime.GetYear ()%2000), cTime.GetMonth (), cTime.GetDay (), cTime.GetHour (), cTime.GetMinute () );

					strText.Format("%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 4 ), strExpireDate );
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
				}
			}
		}

		//weapon requirement
		AddInfoReqWeapon(pSkill->m_sBASIC);

		const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;

		WORD wShowLevel = 0;
		if (pCharSkill)
		{
			wShowLevel = pCharSkill->wLevel;

			if (bNextLevel) 
				wShowLevel = wShowLevel + 1;
		}

		if (!bPASSIVE )
		{
			//consumption info
			AddInfoConsumption(sAPPLY.sDATA_LVL[wShowLevel]);

			//cooldown info
			AddInfoCooldown(sAPPLY.sDATA_LVL[wShowLevel]);
		}

		BOOL bNOT_LEARN = wDispLevel == 0;
		if ((!bMASTER && bNextLevel) || bNOT_LEARN)
		{
			WORD wNextLevel = wDispLevel;
			SKILL::SLEARN& sLEARN = pSkill->m_sLEARN;
			SKILL::SLEARN_LVL& sLEARN_LVL = sLEARN.sLVL_STEP[wNextLevel];

			//learn req
			AddInfoReqLearn(sLEARN_LVL, pSkill->m_sLEARN.sSKILL);

			GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
			if (pCharacter)
			{
				CString strText("");

				//only show skill purchase price when skill is not learned
				/*if ( bNOT_LEARN )
				{
					SSKILL_PURCHASE* pSkillPurchase = GLSkillPurchase::GetInstance().getSkillPurchase( pSkill->m_sBASIC.sNATIVEID );
					if ( pSkillPurchase )
					{
						CString strText("");
						strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_PURCHASE", 0), pSkillPurchase->dwCost );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnMoney >= pSkillPurchase->dwCost ) );
					}
				}*/

				if ( RPARAM::bVIPSystem && pSkill->m_sLEARN.emVIPLevel != EMVIP_LEVEL_NONE )
				{
					
					strText.Format("%s:%s",ID2GAMEWORD("SKILL_BASIC_INFO_EX", 0), COMMENT::CHAR_VIP[pSkill->m_sLEARN.emVIPLevel].c_str());
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( 
						static_cast<int> (pCharacter->m_emVIPLevel ) >= static_cast<int> ( pSkill->m_sLEARN.emVIPLevel )) );
				}

				if ( RPARAM::bRBSystem && pSkill->m_sLEARN.dwRebornReq != 0 )
				{
					strText.Format("%s:%d",ID2GAMEWORD("SKILL_BASIC_INFO_EX", 1), pSkill->m_sLEARN.dwRebornReq);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_dwReborn >= pSkill->m_sLEARN.dwRebornReq ) );
				}
			}
			
		}

		if (!bPASSIVE)
		{
			//consumption info
			AddInfoSkillRange(sAPPLY.sDATA_LVL[wShowLevel], pSkill->m_sBASIC );
		}

		bool bSpace = false;
		AddInfoSkillEffectBasic( bSpace, sAPPLY.emBASIC_TYPE, sAPPLY.sDATA_LVL[wShowLevel], bPASSIVE );
		AddInfoSkillEffectAddOn(bSpace, sAPPLY, bPASSIVE, wShowLevel);
		AddInfoSkillEffectSpec(bSpace, sAPPLY, bPASSIVE, wShowLevel, pSkill->m_sBASIC.emIMPACT_TAR, pSkill->m_sBASIC.emIMPACT_REALM, pSkill->m_sBASIC.emIMPACT_SIDE);
		AddInfoSkillEffectBlow(sAPPLY.emSTATE_BLOW, sAPPLY.sSTATE_BLOW[wShowLevel], sAPPLY.sDATA_LVL[wShowLevel].fLIFE);

		//skill description
		if (!pSkill->GetDesc())	return;
		int StrLength = static_cast<int>(strlen(pSkill->GetDesc()));
		if (StrLength)
		{
			AddTextNoSplit(" ", NS_UITEXTCOLOR::UIINFO_OPTION2);
			AddTextLongestLineSplit(pSkill->GetDesc(), NS_UITEXTCOLOR::WHITE);
		}
	}


	void LOAD_SUB(GLSKILL* const pSkill, GLSKILL* const pSkillMain, SCHARSKILL* const pCharSkill, BOOL bNextLevel)
	{
		WORD wDispLevel = 0;
		if (pCharSkill)
		{
			wDispLevel = pCharSkill->wLevel + 1;
		}

		BOOL bMASTER = GLGaeaClient::GetInstance().GetCharacter()->ISMASTER_SKILL(pSkill->m_sBASIC.sNATIVEID);

		/*multi skill Juver, 2018/09/05 */
		if (pSkillMain)
			bMASTER = GLGaeaClient::GetInstance().GetCharacter()->ISMASTER_SKILL(pSkillMain->m_sBASIC.sNATIVEID);

		if (bNextLevel&&bMASTER)
			bNextLevel = FALSE;

		//icon 
		SetIconImage(pSkill->m_sEXT_DATA.sICONINDEX, pSkill->m_sEXT_DATA.strICONFILE.c_str());

		//name
		{
			CString strText("");
			CString strBLANKSPACE = "       ";

			if (pCharSkill && bMASTER) //skill is learned and is mastered
			{
				strText.Format("%s%s (%s)", strBLANKSPACE.GetString(), pSkill->GetName(), ID2GAMEWORD("SKILL_ADDITIONAL_INFO_ETC", 0));
			}
			else if (pCharSkill) //skill is learned not mastered
			{
				strText.Format("%s%s (%s:%d)", strBLANKSPACE.GetString(), pSkill->GetName(), ID2GAMEWORD("SKILL_ADDITIONAL_INFO_ETC", 1), bNextLevel ? wDispLevel + 1 : wDispLevel);
			}
			else // skill is not learned
			{
				strText.Format("%s%s", strBLANKSPACE.GetString(), pSkill->GetName());
			}

			const int nNameMinSize = 50;
			if (strText.GetLength() < nNameMinSize)
			{
				std::string strBlank(nNameMinSize - strText.GetLength(), ' ');
				strText.Format("%s%s", strText.GetString(), strBlank.c_str());
			}

			AddTitle(strText, NS_UITEXTCOLOR::WHITE);
		}

		//move 1 line
		{
			AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);
		}

		bool bPASSIVE = (pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE);

		//grade, element type
		{
			CString strText("");
			CString strBLANKSPACE("         ");
			strText.Format("%s%s:%d / %s", strBLANKSPACE.GetString(), ID2GAMEWORD("SKILL_ADDITIONAL_INFO_ETC", 2), pSkill->m_sBASIC.dwGRADE, COMMENT::ELEMENT[pSkill->m_sAPPLY.emELEMENT].c_str());

			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);

			//action type and apply type
			if (bPASSIVE)
			{
				strText.Format("%s%s / %s", strBLANKSPACE.GetString(), COMMENT::SKILL_ACTION_TYPE[pSkill->m_sBASIC.emACTION].c_str(), COMMENT::SKILL_ROLE[pSkill->m_sBASIC.emROLE].c_str());
			}
			else
			{
				strText.Format("%s%s / %s", strBLANKSPACE.GetString(), COMMENT::SKILL_ACTION_TYPE[pSkill->m_sBASIC.emACTION].c_str(), COMMENT::SKILL_APPLY[pSkill->m_sBASIC.emAPPLY].c_str());
			}

			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
		}

		/*multi skill, Juver, 2018/08/28 */
		AddTextNoSplit(ID2GAMEWORD("MULTI_SKILL_TEXT"), NS_UITEXTCOLOR::UIINFO_BLUE);

		const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;

		WORD wShowLevel = 0;
		if (pCharSkill)
		{
			wShowLevel = pCharSkill->wLevel;

			if (bNextLevel)
				wShowLevel = wShowLevel + 1;
		}

		if (!bPASSIVE)
		{
			//consumption info
			AddInfoSkillRange(sAPPLY.sDATA_LVL[wShowLevel], pSkill->m_sBASIC);
		}

		bool bSpace = false;
		AddInfoSkillEffectBasic(bSpace, sAPPLY.emBASIC_TYPE, sAPPLY.sDATA_LVL[wShowLevel], bPASSIVE);
		AddInfoSkillEffectAddOn(bSpace, sAPPLY, bPASSIVE, wShowLevel);
		AddInfoSkillEffectSpec(bSpace, sAPPLY, bPASSIVE, wShowLevel, pSkill->m_sBASIC.emIMPACT_TAR, pSkill->m_sBASIC.emIMPACT_REALM, pSkill->m_sBASIC.emIMPACT_SIDE);
		AddInfoSkillEffectBlow(sAPPLY.emSTATE_BLOW, sAPPLY.sSTATE_BLOW[wShowLevel], sAPPLY.sDATA_LVL[wShowLevel].fLIFE);

		//skill description
		if (!pSkill->GetDesc())	return;
		int StrLength = static_cast<int>(strlen(pSkill->GetDesc()));
		if (StrLength)
		{
			AddTextNoSplit(" ", NS_UITEXTCOLOR::UIINFO_OPTION2);
			AddTextLongestLineSplit(pSkill->GetDesc(), NS_UITEXTCOLOR::WHITE);
		}
	}



	void LOAD(const SNATIVEID& sNativeID, const BOOL bNextLevel)
	{
		if (sNativeID != NATIVEID_NULL())
		{
			if ((bNextLevel != m_bNextLevel_Back) || (sNativeID != m_sNativeID_Back))
			{
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData(sNativeID.wMainID, sNativeID.wSubID);
				if (pSkill)	//	스킬이 존재할 경우
				{
					RemoveAllInfo();

					SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL(sNativeID);
					LOAD(pSkill, pCharSkill, bNextLevel);
				}

				m_sNativeID_Back = sNativeID;
				m_bNextLevel_Back = bNextLevel;
			}
		}
	}

	/*multi skill Juver, 2018/09/05 */
	void LOAD_SUB(const SNATIVEID& sNativeIDMain, const SNATIVEID& sNativeIDSub, const BOOL bNextLevel)
	{
		if (sNativeIDMain != NATIVEID_NULL() && sNativeIDSub != NATIVEID_NULL())
		{
			if ((bNextLevel != m_bNextLevel2_Back) || (sNativeIDMain != m_sNativeID2Main_Back) || (sNativeIDSub != m_sNativeID2Sub_Back))
			{
				PGLSKILL pSkillMain = GLSkillMan::GetInstance().GetData(sNativeIDMain.wMainID, sNativeIDMain.wSubID);
				if (!pSkillMain)	return;

				PGLSKILL pSkillSub = GLSkillMan::GetInstance().GetData(sNativeIDSub.wMainID, sNativeIDSub.wSubID);
				if (!pSkillSub)		return;

				RemoveAllInfo();

				SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL(sNativeIDMain);
				LOAD_SUB(pSkillSub, pSkillMain, pCharSkill, bNextLevel);

				m_sNativeID2Main_Back = sNativeIDMain;
				m_sNativeID2Sub_Back = sNativeIDSub;
				m_bNextLevel2_Back = bNextLevel;
			}
		}
	}

	/*extended buff info, EJCode, 2018/10/06 */
	void LOAD_SIMPLE(const SNATIVEID& sNativeID, const WORD wLevel)
	{
		if (sNativeID != NATIVEID_NULL() && sNativeID != NATIVEID_NULL())
		{
			if ( (sNativeID != m_sNativeID3_Back) || (wLevel != m_wLevel3_Back))
			{
				RemoveAllInfo();

				m_sNativeID3_Back = sNativeID;
				m_wLevel3_Back = wLevel;

				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData(sNativeID.wMainID, sNativeID.wSubID);
				if (!pSkill)	return;

				if (wLevel >= SKILL::MAX_SKILL_LEVEL)	return;

				//name
				{
					CString strText("");
					strText.Format("%s", pSkill->GetName());

					const int nNameMinSize = 50;
					if (strText.GetLength() < nNameMinSize)
					{
						std::string strBlank(nNameMinSize - strText.GetLength(), ' ');
						strText.Format("%s%s", strText.GetString(), strBlank.c_str());
					}

					AddTitle(strText, NS_UITEXTCOLOR::WHITE);
				}

				//add space
				AddTextNoSplit(" ", NS_UITEXTCOLOR::WHITE);

				{
					const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;

					bool bSpace = false;
					AddInfoSkillEffectBasic(bSpace, sAPPLY.emBASIC_TYPE, sAPPLY.sDATA_LVL[wLevel], FALSE);
					AddInfoSkillEffectAddOn(bSpace, sAPPLY, FALSE, wLevel);
					AddInfoSkillEffectSpec(bSpace, sAPPLY, FALSE, wLevel, pSkill->m_sBASIC.emIMPACT_TAR, pSkill->m_sBASIC.emIMPACT_REALM, pSkill->m_sBASIC.emIMPACT_SIDE);
				}



			}

		}
	}

};