#include "stdafx.h"
#include "./GLAwardTitle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


SAWARD_TITLE_DATA::SAWARD_TITLE_DATA()
	: dwID(AWARD_TITLE_ID_NULL)
	, emType(EMAWARD_TITLE_TYPE_SIZE)
{
	memset(szTitle, 0, sizeof(szTitle));
};

SAWARD_TITLE_DATA::~SAWARD_TITLE_DATA()
{

}

void SAWARD_TITLE_DATA::Reset()
{

}

BOOL SAWARD_TITLE_DATA::IsValid()
{
	return TRUE;
}

SAWARD_TITLE_DATA& SAWARD_TITLE_DATA::operator= ( const SAWARD_TITLE_DATA &cData )
{
	if( this == &cData )	return *this;

	dwID	= cData.dwID;
	emType	= cData.emType;

	StringCbCopy( szTitle, AWARD_TITLE_LENGTH+1, cData.szTitle );

	return *this;
}


