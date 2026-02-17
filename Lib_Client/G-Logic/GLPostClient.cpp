#include "stdafx.h"
#include "./GLPostClient.h"

#include "./GLPost.h"

#include "./GLGaeaClient.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


SAPOSTCLIENT::SAPOSTCLIENT()
	: SAPOST()
	, dwSTATE(0)
{
}

SAPOSTCLIENT::SAPOSTCLIENT( const SAPOST& rValue )
	: SAPOST( rValue )
	, dwSTATE(0)
{
}

const SAPOSTCLIENT& SAPOSTCLIENT::operator = ( const SAPOST& rValue )
{
	*((SAPOST*)this) = rValue;
	return *this;
}

const BOOL SAPOSTCLIENT::IsUnRead()
{
	if ( m_byPostState == EMPOST_UNREAD )
	{
		return TRUE;
	}
	else if ( m_byPostState == EMPOST_RETURN )
	{
		if ( m_RecvBackDate == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

const BOOL SAPOSTCLIENT::IsReturnPost ()
{
	if ( m_ReturnDate )
	{
		return TRUE;
	}

	return FALSE;
}

const BOOL SAPOSTCLIENT::IsDeletePost ()
{
	if ( m_byPostState == EMPOST_USER_DELETE )
	{
		return TRUE;
	}

	if ( m_byPostState == EMPOST_AUTO_DELETE )
	{
		return TRUE;
	}

	if ( m_DeleteDate )
	{
		return TRUE;
	}

	return FALSE;
}

const __time64_t SAPOSTCLIENT::GetRecentSendDate () 
{
	if ( m_ReturnDate )
	{
		return m_ReturnDate;	
	}

	return m_SendDate;
}

const DWORD SAPOSTCLIENT::GetExpireDay()
{
	CTime cSendTime;

	if ( m_ReturnDate != 0 )
	{
		cSendTime = CTime( m_ReturnDate );
	}
	else
	{
		if ( m_SendDate != 0 )
		{
			cSendTime = CTime( m_SendDate );
		}
		else
		{
			return 0;
		}
	}

	GLGaeaClient::GetInstance().GetConvertServerTime( cSendTime );

	CTime cCurrentTime;
	cCurrentTime = CTime::GetCurrentTime();
	GLGaeaClient::GetInstance().GetConvertServerTime( cCurrentTime );

	CTimeSpan cDiffTime = cCurrentTime - cSendTime;
	LONGLONG llSec = cDiffTime.GetTotalSeconds();

	DWORD dwDay = 0;

	if ( llSec <= GLPost::llPOST_EXPIRE_DURATION_SEC )
	{
		dwDay = (DWORD)(GLPost::llPOST_EXPIRE_DURATION_SEC - llSec)/60/60/24;
	}

	dwDay += 1;

	if ( GLCONST_POST::dwPOST_EXPIRE_DURATION_DAY < dwDay )
	{
		dwDay = GLCONST_POST::dwPOST_EXPIRE_DURATION_DAY;
	}

	return dwDay;
}


SAPOSTHISTORY::SAPOSTHISTORY()
	: SAPOSTSTATE ()
{

}

SAPOSTHISTORY::SAPOSTHISTORY ( const SAPOSTSTATE& rValue )
	: SAPOSTSTATE ( rValue )
{

}

const SAPOSTHISTORY& SAPOSTHISTORY::operator = ( const SAPOSTSTATE& rValue )
{
	*((SAPOSTSTATE*)this) = rValue;
	return *this;
}

void SAPOSTHISTORY::SET ( const SAPOSTCLIENT& sPost )
{
	this->llPostID	  = sPost.m_llPostID;

	this->byPostState = sPost.m_byPostState;
	this->byPostClass = sPost.m_byPostClass;
	this->byPostType  = sPost.m_byPostType;

	this->iSender  = sPost.m_iSender;
	this->iRecver  = sPost.m_iRecver;

	this->iDesign  = sPost.m_iDesign;
	this->llCharge = sPost.m_llCharge;

	this->SendDate = sPost.m_SendDate;
	this->RecvDate = sPost.m_RecvDate;
	this->ReturnDate = sPost.m_ReturnDate;
	this->RecvBackDate = sPost.m_RecvBackDate;
	this->DeleteDate = sPost.m_DeleteDate;

	strcpy_s ( this->SenderName, POST_NAME_LENGTH, sPost.m_SenderName );
	strcpy_s ( this->RecverName, POST_NAME_LENGTH,	sPost.m_RecverName );
}

void SAPOSTHISTORY::SET( GLCharacter* pCharacter, GLMSG::SNET_POST_SEND& nmg )
{
	this->llPostID = ID_NULL;

	this->byPostState = nmg.byPostState;
	this->byPostClass = nmg.byPostClass;
	this->byPostType  = nmg.byPostType;

	this->iSender  = nmg.iSender;
	this->iRecver  = nmg.iRecver;

	this->iDesign  = nmg.iDesign;
	this->llCharge = nmg.llCharge;

	this->SenderName    [0] = NULL;
	this->SendDate		    = 0;
	this->RecvDate		    = 0;
	this->ReturnDate	    = 0;
	this->RecvBackDate	    = 0;
	this->DeleteDate	    = 0;

	if ( pCharacter )
	{
		strcpy_s ( this->SenderName, POST_NAME_LENGTH, pCharacter->GetName() );
	}

	strcpy_s ( this->RecverName, POST_NAME_LENGTH, nmg.RecverName );

	// Note : 시간을 현재 시간으로 임의로 세팅해준다.
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		CTime cTime(st);
		this->SendDate = cTime.GetTime();
	}
}

const BOOL SAPOSTHISTORY::IsUnRead()
{
	if ( byPostState == EMPOST_UNREAD )
	{
		return TRUE;
	}
	else if ( byPostState == EMPOST_RETURN )
	{
		if ( RecvBackDate == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

const BOOL SAPOSTHISTORY::IsReturnPost()
{
	if ( ReturnDate )
	{
		return TRUE;
	}

	return FALSE;
}

const BOOL SAPOSTHISTORY::IsDeletePost()
{
	if ( byPostState == EMPOST_USER_DELETE )
	{
		return TRUE;
	}

	if ( byPostState == EMPOST_AUTO_DELETE )
	{
		return TRUE;
	}

	if ( DeleteDate )
	{
		return TRUE;
	}

	return FALSE;
}

const __time64_t SAPOSTHISTORY::GetRecentSendDate() 
{
	if ( ReturnDate != 0 )
	{
		return ReturnDate;	
	}

	return SendDate;
}



GLPostClient::GLPostClient()
	: m_bReqPost				 ( FALSE )
	, m_bReqSend				 ( FALSE )
	, m_dwLastSendTime			 ( 0 )
	, m_dwLastRecvTime			 ( 0 )
	, m_HistoryInfoSet           ( SPOSTHISTORYINFOSET::SPOSTHISTORYALLINFO )
	, m_SendHistoryInfoSet       ( SPOSTHISTORYINFOSET::SPOSTHISTORYSENDINFO )
	, m_RecvHistoryInfoSet       ( SPOSTHISTORYINFOSET::SPOSTHISTORYRECVINFO )
{
}

GLPostClient::~GLPostClient(void)
{
}

GLPostClient& GLPostClient::GetInstance()
{
	static GLPostClient Instance;
	return Instance;
}

const SAPOSTCLIENT* GLPostClient::GetSAPOSTCLIENT_ReceiveVec( DWORD dwIndex ) const
{
	if ( dwIndex >= m_ReceiveVec.size() )
		return NULL;

	return m_ReceiveVec[dwIndex];
}

HRESULT GLPostClient::FrameMove ( float fTime, float fElapsedTime )
{
	GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();

	// Note : 우체통을 사용할 수 있는지 체크해서 꺼준다.

	if (pChar && pChar->m_emPOSTBOXIDTYPE != EMPOSTBOXID_TYPE_NONE && pChar->m_sPOSTBOXID != NATIVEID_NULL())
	{
		/*if( true == GLWidgetScript::GetInstance().LuaCallFunc< bool >(
			NSWIDGET_SCRIPT::CF_PostBox_IsOpen ) )
		{
			bool bClose = false;
			if( true == GLWidgetScript::GetInstance().LuaCallFunc< bool >(
				NSWIDGET_SCRIPT::g_strFunc_PrivateMarket_IsOpen ) )
			{
				bClose = true;
			}

			if( true == GLWidgetScript::GetInstance().LuaCallFunc< bool >(
				NSWIDGET_SCRIPT::g_strFunc_Trade_IsOpen ) )
			{
				bClose = true;
			}

			if( false == pChar->IsPOSTBOXUSEABLE( pChar->m_emPOSTBOXIDTYPE, pChar->m_sPOSTBOXID ) )
			{
				bClose = true;
			}

			if (bClose)
			{
				GLWidgetScript::GetInstance().LuaCallFunc< void >(
					NSWIDGET_SCRIPT::g_strFunc_PostBox_Close );
				return S_FALSE;
			}
		}*/
	}


	return S_OK;
}

void GLPostClient::ClearReceivePost ()
{
	if ( !m_ReceiveMap.empty() )
	{
		m_ReceiveMap.clear();
	}

	if ( !m_ReceiveVec.empty() )
	{
		m_ReceiveVec.clear();
	}
}

const DWORD	GLPostClient::GetMaxPost ()
{
	return (DWORD)m_ReceiveVec.size();
}
