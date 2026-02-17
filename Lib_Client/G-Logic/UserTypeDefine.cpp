#include "stdafx.h"
#include "./UserTypeDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NSUSER_TYPE
{
	EMUSER_TYPE_INDEX UserTypeToUserTypeIndex( EMUSER_TYPE emType )
	{
		EMUSER_TYPE_INDEX emResult = USER_TYPE_INDEX_COMMON;

		switch( emType )
		{
		case USER_TYPE_COMMON:	
			emResult = USER_TYPE_INDEX_COMMON;
			break;

		case USER_TYPE_PREMIUM:
			emResult = USER_TYPE_INDEX_PREMIUM;
			break;

		case USER_TYPE_SPECIAL:
			emResult = USER_TYPE_INDEX_SPECIAL;
			break;

		case USER_TYPE_GM4:
			emResult = USER_TYPE_INDEX_GM4;
			break;

		case USER_TYPE_GM3:
			emResult = USER_TYPE_INDEX_GM3;
			break;

		case USER_TYPE_GM2:
			emResult = USER_TYPE_INDEX_GM2;
			break;

		case USER_TYPE_GM1:
			emResult = USER_TYPE_INDEX_GM1;
			break;

		case USER_TYPE_MASTER:
			emResult = USER_TYPE_INDEX_MASTER;
			break;
		}
		
		return emResult;
	};

	EMUSER_TYPE UserTypeIndexToUserType( EMUSER_TYPE_INDEX emType )
	{
		EMUSER_TYPE emResult = USER_TYPE_COMMON;

		switch ( emType )
		{
		case USER_TYPE_INDEX_COMMON:
			emResult = USER_TYPE_COMMON;
			break;

		case USER_TYPE_INDEX_PREMIUM:
			emResult = USER_TYPE_PREMIUM;
			break;

		case USER_TYPE_INDEX_SPECIAL:
			emResult = USER_TYPE_SPECIAL;
			break;

		case USER_TYPE_INDEX_GM4:
			emResult = USER_TYPE_GM4;
			break;

		case USER_TYPE_INDEX_GM3:
			emResult = USER_TYPE_GM3;
			break;

		case USER_TYPE_INDEX_GM2:
			emResult = USER_TYPE_GM2;
			break;

		case USER_TYPE_INDEX_GM1:
			emResult = USER_TYPE_GM1;
			break;

		case USER_TYPE_INDEX_MASTER:
			emResult = USER_TYPE_MASTER;
			break;
		};

		return emResult;
	};
};

namespace COMMENT
{
	std::string USER_TYPE[NSUSER_TYPE::USER_TYPE_INDEX_SIZE] = 
	{
		"Normal(0)",				//USER_TYPE_INDEX_COMMON
		"Premium(1)",				//USER_TYPE_INDEX_PREMIUM
		"Special(10)",				//USER_TYPE_INDEX_SPECIAL
		"GM4(19)",					//USER_TYPE_INDEX_GM4
		"GM3(20)",					//USER_TYPE_INDEX_GM3
		"GM2(21)",					//USER_TYPE_INDEX_GM2
		"GM1(22)",					//USER_TYPE_INDEX_GM1
		"Master(30)",				//USER_TYPE_INDEX_MASTER
	};
};