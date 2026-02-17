#include "stdafx.h"
#include "./GLPostData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//----------------------------------------------------------------------------//

SAPOST::SAPOST()
	: m_llPostID(0)
	, m_byPostState(0)
	, m_byPostType(0)
	, m_byPostClass(0)
	, m_byChargePayment(0)
	, m_iSender(0)
	, m_iRecver(0)
	, m_iDesign(0)
	, m_SendDate(0)
	, m_RecvDate(0)
	, m_ReturnDate(0)
	, m_RecvBackDate(0)
	, m_DeleteDate(0)
	, m_llAttachMoney(0)
	, m_llCharge(0)
	, m_byMoneyAccept(0)
	, m_byAttach01Accept(0)
{
	memset(m_Title, 0, POST_MAX_TITLE_LENGTH);
	memset(m_Content, 0, POST_MAX_CONTENT_LENGTH);
	memset(m_SenderName, 0, SAPOST_CHAR_LENGTH); //+3 : to avoid margin
	memset(m_RecverName, 0, SAPOST_CHAR_LENGTH); //+3 : to avoid margin
}

SAPOST::~SAPOST()
{

}

void SAPOST::SetTitle(const std::string& Data)
{
	StringCchCopy(m_Title, POST_MAX_TITLE_LENGTH, Data.c_str());
}

void SAPOST::SetContent(const std::string& Data)
{
	StringCchCopy(m_Content, POST_MAX_CONTENT_LENGTH, Data.c_str());
}

void SAPOST::SetSenderName(const std::string& Data)
{
	StringCchCopy(m_SenderName, SAPOST_CHAR_LENGTH, Data.c_str()); //+3 : to avoid margin
}

void SAPOST::SetRecverName(const std::string& Data)
{
	StringCchCopy(m_RecverName, SAPOST_CHAR_LENGTH, Data.c_str()); //+3 : to avoid margin
}


//----------------------------------------------------------------------------//

SAPOSTSTATE::SAPOSTSTATE()
	: llPostID(0)
	, byPostState(0)
	, byPostType(0)
	, byPostClass(0)
	, iSender(0)
	, iRecver(0)
	, iDesign(0)
	, llCharge(0)
	, SendDate(0)
	, RecvDate(0)
	, ReturnDate(0)
	, RecvBackDate(0)
	, DeleteDate(0)
{
	memset(SenderName, 0, POST_NAME_LENGTH);	//+3 : to avoid margin
	memset(RecverName, 0, POST_NAME_LENGTH);	//+3 : to avoid margin
}

SAPOSTSTATE::~SAPOSTSTATE()
{

}

void SAPOSTSTATE::SetSenderName(const std::string& Name)
{
	StringCchCopy(SenderName, POST_NAME_LENGTH, Name.c_str());
}

void SAPOSTSTATE::SetRecverName(const std::string& Name)
{
	StringCchCopy(RecverName, POST_NAME_LENGTH, Name.c_str());
}


//----------------------------------------------------------------------------//

SAPOSTINFO::SAPOSTINFO()
	: dwPostID(0)
	, dwSendChaNum(0)
	, dwSendUserNum(0)
	, dwRecvChaNum(0)
	, tSendDate(0)
	, wStyle(0)
	, wMailType(0)
	, wAttachType(0)
	, bRead(0)
	, wMoneyType(0)
	, llMoney(0)
	, sItemCutom(SITEMCUSTOM(NATIVEID_NULL())) 
{
}

//----------------------------------------------------------------------------//


// Note : 파일을 읽어들인다. (bjju.post)
namespace GLCONST_POST
{
	LONGLONG	llPOST_MAX_ATTACH_MONEY		= 200000000;

	FLOAT		fPOSTBOX_DISTANCE			= 30.0f;			// 우체통 이용가능한 최대 거리

	FLOAT		fPOST_COMMISSION			= 0.002f;			// 우편 첨부 금액에 대한 수수료
	DWORD		dwPOST_BASE_FEE				= 1000;				// Post Fee for Text mail
	DWORD		dwPOST_COMMISSION_ITEM		= 1000;				// Post Commission for Item attachment
	DWORD		dwPOST_COMMISSION_DESIGN	= 1000;				// Post Commission for Design card

	DWORD		dwPOST_HOW_MANY_GET			= 30;				// how many posts will be get at once
	DWORD       dwPOST_STATE_HOW_MANY_GET   = 30;               // how many post state will be get at once (bjju.post)
	DWORD		dwPOST_SEND_STATE_HOW_MANY_GET	= 15;			// how many post state which be sent mail will be get at once
	DWORD		dwPOST_RECV_STATE_HOW_MANY_GET	= 15;			// how many post state which be received mail will be get at once
	DWORD		dwPOST_EXPIRE_DURATION_DAY	= 7;
	DWORD		dwPOST_SEND_COOL_TIME_MS	= 6000;				// 1min (ms)
	DWORD		dwPOST_RECV_COOL_TIME_MS	= 10000;			// 1min (ms)
	DWORD		dwPOST_SEND_REQUIRE_LEVEL	= 10;				// Require cha's Level for post sending
};

//----------------------------------------------------------------------------//