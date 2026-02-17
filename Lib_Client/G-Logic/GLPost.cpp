#include "stdafx.h"
#include "./GLPost.h"


#include "./GLPostData.h"
#include "./GLogicData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


LONGLONG GLPost::llPOST_EXPIRE_DURATION_SEC = 0; 


GLPost::GLPost()
{
}

GLPost::~GLPost()
{
}

const LONGLONG GLPost::GetCommision ( const BOOL IsAttachDesign, const BOOL IsAttachItem, const LONGLONG llAttachMoney )
{
	LONGLONG llPostFee = (LONGLONG)GLCONST_POST::dwPOST_BASE_FEE;

	if ( IsAttachItem )
	{
		llPostFee += (LONGLONG)GLCONST_POST::dwPOST_COMMISSION_ITEM;
	}

	if ( IsAttachDesign )
	{
		llPostFee += (LONGLONG)GLCONST_POST::dwPOST_COMMISSION_DESIGN;
	}

	// Note : 에러검사
	{
		if ( llAttachMoney < 0 )
		{
			return 0;
		}

		if ( llPostFee < 0 )
		{
			return 0;
		}
	}

	llPostFee += GLogicData::GetInstance().MultiplyBigValueByPoint( llAttachMoney, GLCONST_POST::fPOST_COMMISSION );

	return llPostFee;
}
