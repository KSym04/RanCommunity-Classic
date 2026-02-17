/*!
 * \file GLPostControlMsg.h
 *
 * \author Juver
 * \date 2021/02/05
 *
 * 
 */

#ifndef GLPOSTCONTROLMSG_H_INCLUDED__
#define GLPOSTCONTROLMSG_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./GLContrlBaseMsg.h"
#include "./GLPostDefine.h"
#include "./GLPostData.h"

#include <boost/static_assert.hpp>

namespace GLMSG
{
#pragma pack(1)

	struct SNET_POST_SEND : public NET_MSG_GENERIC
	{		
		int			nReqID;		// This ID is set by Client-side and used for identify 
		// matching of Post-Reqeust and Post-Response, especially ReqChaNum_FB and PostSend.
		// And same ID will be included in all related-msgs to this reuqest.

		BYTE		byPostState;
		BYTE		byPostType;
		BYTE		byPostClass;
		BYTE		byPayment;

		int			iSender;
		int			iRecver;
		int			iDesign;
		int			nUserNumRecver;	//Recver'sUserNum

		LONGLONG	llCharge;
		LONGLONG	llAttachMoney;	//Change the Sender's money and then put the money into DB.

		//char		SendDate[20];	//yyyy-mm-dd hh:mm:ss
		char		Title[POST_MAX_TITLE_LENGTH];
		char		Content[POST_MAX_CONTENT_LENGTH];

		//char		SenderName[CHR_ID_LENGTH +3];	//SenderName MUST be set at server-side.
		char		RecverName[CHR_ID_LENGTH +3];	//+3 : to avoid margin

		// Attached Item		
		WORD		xAttach01;
		WORD		yAttach01;
		WORD		nAttach01;
		SNATIVEID	idAttach01;

		SNET_POST_SEND()
		{
			dwSize = sizeof(SNET_POST_SEND);
			nType = NET_MSG_POST_SEND;
			BOOST_STATIC_ASSERT(sizeof(SNET_POST_SEND)<=NET_DATA_BUFSIZE);
		}
	};

#pragma pack()
};


#endif // GLPOSTCONTROLMSG_H_INCLUDED__
