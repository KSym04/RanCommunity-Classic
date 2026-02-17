/*!
 * \file GLPostData.h
 *
 * \author Juver
 * \date 2021/02/05
 *
 * 
 */

#ifndef GLPOSTDATA_H_INCLUDED__
#define GLPOSTDATA_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLPostDefine.h"
#include "./GLItem.h"

struct SAPOST
{
	enum 
	{ 
		SAPOST_CHAR_LENGTH = CHR_ID_LENGTH + 3,
	};

	LONGLONG			m_llPostID;

	BYTE				m_byPostState;
	BYTE				m_byPostType;	
	BYTE				m_byPostClass;
	BYTE				m_byChargePayment;

	int					m_iSender;
	int					m_iRecver;

	int					m_iDesign;

	char				m_Title[POST_MAX_TITLE_LENGTH];
	char				m_Content[POST_MAX_CONTENT_LENGTH];

	__time64_t          m_SendDate; // CurrentTime
	__time64_t          m_RecvDate;
	__time64_t          m_ReturnDate;
	__time64_t          m_RecvBackDate;
	__time64_t          m_DeleteDate;

	char				m_SenderName[SAPOST_CHAR_LENGTH];	//+3 : to avoid margin
	char				m_RecverName[SAPOST_CHAR_LENGTH];	//+3 : to avoid margin

	LONGLONG			m_llAttachMoney;
	LONGLONG			m_llCharge;

	SITEMCUSTOM			m_sAttach01;

	BYTE				m_byMoneyAccept;
	BYTE				m_byAttach01Accept;

	SAPOST();
	~SAPOST();

	void SetTitle(const std::string& Data);
	void SetContent(const std::string& Data);
	void SetSenderName(const std::string& Data);
	void SetRecverName(const std::string& Data);
};



struct SAPOSTSTATE
{
	enum 
	{ 
		POST_NAME_LENGTH = CHR_ID_LENGTH + 3,
	};

	LONGLONG			llPostID;

	BYTE				byPostState;
	BYTE				byPostType;	
	BYTE				byPostClass;	

	int					iSender;
	int					iRecver;

	int					iDesign;

	LONGLONG			llCharge;

	__time64_t          SendDate;
	__time64_t          RecvDate;
	__time64_t          ReturnDate;
	__time64_t          RecvBackDate;
	__time64_t          DeleteDate;    // Optional

	char				SenderName[POST_NAME_LENGTH];	//+3 : to avoid margin
	char				RecverName[POST_NAME_LENGTH];	//+3 : to avoid margin

	SAPOSTSTATE();
	~SAPOSTSTATE();

	void SetSenderName(const std::string& Name);
	void SetRecverName(const std::string& Name);
};



struct SAPOSTINFO
{
	DWORD			dwPostID;
	DWORD			dwSendChaNum;
	std::string		strSendChaName;
	DWORD			dwSendUserNum;
	DWORD			dwRecvChaNum;
	__time64_t		tSendDate;
	WORD			wStyle;
	WORD			wMailType;		// EMPOST_MAIL_TYPE
	WORD			wAttachType;	// EMPOST_ATTACH_TYPE
	bool			bRead;
	WORD			wMoneyType;		// EMPOST_MONEY_TYPE
	LONGLONG		llMoney;
	SITEMCUSTOM		sItemCutom;
	std::string		strTitle;
	std::string		strContent;
	bool			bAttachHole;

	SAPOSTINFO();
	~SAPOSTINFO();
};


namespace GLCONST_POST
{
	extern LONGLONG		llPOST_MAX_ATTACH_MONEY;

	extern FLOAT		fPOSTBOX_DISTANCE;				// 우체통 이용가능한 최대 거리

	extern FLOAT		fPOST_COMMISSION;				// 우편 첨부 금액에 대한 수수료
	extern DWORD		dwPOST_BASE_FEE;				// Post Fee for Text mail
	extern DWORD		dwPOST_COMMISSION_ITEM;			// Post Commission for Item attachment
	extern DWORD		dwPOST_COMMISSION_DESIGN;		// Post Commission for Design card

	extern DWORD		dwPOST_HOW_MANY_GET;				// how many posts will be get at once
	extern DWORD		dwPOST_STATE_HOW_MANY_GET;		    // how many post state will be get at once (bjju.post)
	extern DWORD		dwPOST_SEND_STATE_HOW_MANY_GET;		// how many post state which be sent mail will be get at once
	extern DWORD		dwPOST_RECV_STATE_HOW_MANY_GET;		// how many post state which be received mail will be get at once
	extern DWORD		dwPOST_EXPIRE_DURATION_DAY;
	extern DWORD		dwPOST_SEND_COOL_TIME_MS;			// 1min (ms)
	extern DWORD		dwPOST_RECV_COOL_TIME_MS;			// 1min (ms)
	extern DWORD		dwPOST_SEND_REQUIRE_LEVEL;		// Require cha's Level for post sending

	BOOL LOADFILE(const std::string& FileName, BOOL bServer=TRUE);
}

#endif // GLPOSTDATA_H_INCLUDED__
