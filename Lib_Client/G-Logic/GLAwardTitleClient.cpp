#include "stdafx.h"
#include "./GLAwardTitleClient.h"

#include "./DxGlobalStage.h"
#include "./GLContrlPcMsg2.h"

#include "../../Lib_ClientUI/Interface/GameTextControl.h"
#include "../../Lib_ClientUI/Interface/UITextControl.h"
#include "../../Lib_ClientUI/Interface/InnerInterface.h"
#include "../../Lib_ClientUI/Interface/StudentRecordWindow.h"
#include "../../Lib_ClientUI/Interface/PlayStatsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GLAwardTitleClient& GLAwardTitleClient::GetInstance ()
{
	static  GLAwardTitleClient Instance;
	return Instance;
}

GLAwardTitleClient::GLAwardTitleClient(void)
	: m_bReqData(false)
{
}

GLAwardTitleClient::~GLAwardTitleClient(void)
{
}

void GLAwardTitleClient::Clear()
{
	m_bReqData = false;
	m_mapAwardTitle.clear();
}

void GLAwardTitleClient::RequestData()
{
	if ( m_bReqData )	return;

	m_bReqData = true;

	GLMSG::SNETPC_AWARD_TITLE_REQUEST_DATA NetMsg;
	NETSEND(&NetMsg);
}

HRESULT GLAwardTitleClient::MsgProcess ( NET_MSG_GENERIC* nmg )
{
	if ( !nmg )	return E_FAIL;

	switch ( nmg->nType )
	{
	default:
		CDebugSet::ToListView ( "GLAwardTitleClient::MsgProcessFrame() not classified message : TYPE[%d]", nmg->nType );
		break;
	}

	return S_OK;
}

const TCHAR* GLAwardTitleClient::GetTitle( DWORD dwID, EMAWARD_TITLE_TYPE emType )
{
	MAP_AWARD_TITLE_DATA_ITER it =  m_mapAwardTitle.find( dwID );
	if ( it != m_mapAwardTitle.end() )
	{
		const SAWARD_TITLE_DATA& sData = (*it).second;
		if ( sData.emType == emType )
		{
			return &sData.szTitle[0];
		}
	}

	return NULL;
}

