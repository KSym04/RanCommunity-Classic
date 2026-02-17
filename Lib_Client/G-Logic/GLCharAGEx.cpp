#include "stdafx.h"
#include "./GLCharAG.h"

#include "./GLAgentServer.h"

#include "../../Lib_Engine/Core/NSRLog.h"
#include "../../Lib_Engine/Core/NSRParam.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* HWID watchlist, Juver, 2020/05/05 */
void GLCharAG::HWIDWatchListDisconnect()
{
	if( !m_bUserFlagHWIDWatchlist )	return;

	RLOG::LogFile( "_HWIDWatchlist.txt", "disconnect user:[%u]%s char:[%u]%s", m_dwUserID, m_szUserName, m_dwCharID, m_szName );

	GLMSG::SNET_GM_KICK_USER_PROC_FLD NetMsgFld;
	NetMsgFld.dwID = m_dwCharID;
	GLAgentServer::GetInstance().SENDTOALLCHANNEL ( &NetMsgFld );

	GLMSG::SNET_GM_KICK_USER_PROC NetMsgProc;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgProc );

	GLAgentServer::GetInstance().ReserveDropOutPC ( m_dwGaeaID );
}

/* award title, Juver, 2021/02/12 */
HRESULT GLCharAG::AwardTitleRequestData ( NET_MSG_GENERIC* nmg )
{
	MAP_AWARD_TITLE_DATA_ITER it_b = m_mapAwardTitle.begin();
	MAP_AWARD_TITLE_DATA_ITER it_e = m_mapAwardTitle.end();

	for ( ; it_b!=it_e; ++it_b )
	{
		SAWARD_TITLE_DATA &sData = (*it_b).second;

		GLMSG::SNETPC_AWARD_TITLE_UPDATE_DATA NetMsgList;
		NetMsgList.sData = sData;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsgList );
	}

	GLMSG::SNETPC_AWARD_TITLE_UPDATE_DONE NetMsg;
	GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &NetMsg );

	return S_OK;
}

void GLCharAG::CaptchaUpdate(float fElapsedTime)
{
    GLAGLandMan* pLandMan = GLAgentServer::GetInstance().GetByMapID(m_sCurMapID);
    if (pLandMan && pLandMan->isUseCaptcha())
    {
        if (m_bCaptchaShow)
        {
            m_fCaptchaWaitTimer += fElapsedTime;

            // Check if multiple CAPTCHA shows are enabled
            if (RPARAM::bMultipleCaptchaShowTimer == 0)
            {
                // If disabled, only use the initial captcha show time
                if (m_fCaptchaWaitTimer >= RPARAM::fCaptchaShowTimer && !m_bCaptchaShow)
                {
                    m_fCaptchaShowTimer = 0.0f;
                    m_fCaptchaWaitTimer = 0.0f;
                    m_bCaptchaShow = true;
                    m_dwCaptchaValue = RandomNumber(1000, 999999);  // Random CAPTCHA value

                    // Send CAPTCHA to client
                    GLMSG::SNET_CAPTCHA_UPDATE_SHOW_AC msg;
                    msg.dwCaptcha = m_dwCaptchaValue;
                    msg.fWaitTime = RPARAM::fCaptchaWaitTimer;
                    GLAgentServer::GetInstance().SENDTOCLIENT(m_dwClientID, &msg);
                }
            }
            else
            {
                // If multiple CAPTCHA shows are enabled (bMultipleCaptchaShowTimer == 1)
                const float captchaIntervals[] = {3 * 60.0f, 5 * 60.0f, 10 * 60.0f}; // in seconds
                const int numIntervals = sizeof(captchaIntervals) / sizeof(captchaIntervals[0]);

                // The time at which the CAPTCHA will be shown based on intervals
                float timeToShowCaptcha = RPARAM::fCaptchaShowTimer;

                // Add intervals progressively if necessary
                for (int i = 0; i < numIntervals; ++i)
                {
                    timeToShowCaptcha += captchaIntervals[i];
                }

                // Now, if the user has spent enough time for any of the stages, show the CAPTCHA
                if (m_fCaptchaWaitTimer >= timeToShowCaptcha && !m_bCaptchaShow)
                {
                    m_fCaptchaShowTimer = 0.0f;
                    m_fCaptchaWaitTimer = 0.0f;
                    m_bCaptchaShow = true;
                    m_dwCaptchaValue = RandomNumber(1000, 999999);  // Random CAPTCHA value

                    // Send CAPTCHA to client
                    GLMSG::SNET_CAPTCHA_UPDATE_SHOW_AC msg;
                    msg.dwCaptcha = m_dwCaptchaValue;
                    msg.fWaitTime = RPARAM::fCaptchaWaitTimer;
                    GLAgentServer::GetInstance().SENDTOCLIENT(m_dwClientID, &msg);
                }

                // If the time exceeds the final stage (28 minutes), reset the timer to 10 minutes
                if (m_fCaptchaWaitTimer >= (RPARAM::fCaptchaShowTimer + 3*60 + 5*60 + 10*60))
                {
                    m_fCaptchaWaitTimer = 0.0f;  // Reset the timer
                    m_bCaptchaShow = false;      // Reset CAPTCHA visibility flag
                    // Optionally, you can also reset m_dwCaptchaValue here if needed
                }
            }

            // Now check if the CAPTCHA timeout has occurred (after the CAPTCHA was shown but not entered)
            if (m_fCaptchaWaitTimer >= RPARAM::fCaptchaWaitTimer)
            {
                // Timeout occurred, disconnect the player
                GLMSG::SNET_CHARACTER_DISCONNECT_AGENT_TO_FIELD net_msg_field;
                net_msg_field.char_id = m_dwCharID;
                net_msg_field.bCloseClient = true;
                GLAgentServer::GetInstance().SENDTOALLCHANNEL(&net_msg_field);

                GLMSG::SNET_CHARACTER_DISCONNECT_AGENT_TO_CHAR net_msg_client;
                net_msg_client.char_id = m_dwCharID;
                net_msg_client.bCloseClient = true;
                GLAgentServer::GetInstance().SENDTOCLIENT(m_dwClientID, &net_msg_client);

                GLAgentServer::GetInstance().ReserveDropOutPC(m_dwGaeaID);

                // Optionally, log the disconnection
                // CDebugSet::ToFileWithTime( "_captcha_disconnect.txt", "[%u][%u] %s", m_dwUserID, m_dwCharID, m_szName );
            }
        }
        else
        {
            m_fCaptchaShowTimer += fElapsedTime;

            // Trigger the first CAPTCHA show
            if (m_fCaptchaShowTimer >= RPARAM::fCaptchaShowTimer)
            {
                m_fCaptchaShowTimer = 0.0f;
                m_fCaptchaWaitTimer = 0.0f;
                m_bCaptchaShow = true;
                m_dwCaptchaValue = RandomNumber(1000, 999999); 

                // Send CAPTCHA to client
                GLMSG::SNET_CAPTCHA_UPDATE_SHOW_AC msg;
                msg.dwCaptcha = m_dwCaptchaValue;
                msg.fWaitTime = RPARAM::fCaptchaWaitTimer;
                GLAgentServer::GetInstance().SENDTOCLIENT(m_dwClientID, &msg);
            }
        }
    }
    else
    {
        CaptchaReset();
    }
}



void GLCharAG::CaptchaInput(DWORD dwCaptcha)
{
	if ( m_bCaptchaShow )
	{
		if (m_dwCaptchaValue == dwCaptcha)
		{
			//reset
			m_bCaptchaShow = false;
			m_fCaptchaShowTimer = 0.0f;
			m_fCaptchaWaitTimer = 0.0f;
			m_dwCaptchaValue = 0;

			//input correct
			//hide captcha in client
			GLMSG::SNET_CAPTCHA_UPDATE_INPUT_AC msg;
			msg.emFb = EMCAPTCHA_CA_SUCCESS;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &msg );
		}
		else
		{
			//wrong captcha 
			GLMSG::SNET_CAPTCHA_UPDATE_INPUT_AC msg;
			msg.emFb = EMCAPTCHA_CA_INCORRECT;
			GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &msg );
		}
	}
	else
	{
		//wrong timing 
		GLMSG::SNET_CAPTCHA_UPDATE_INPUT_AC msg;
		msg.emFb = EMCAPTCHA_CA_ERROR;
		GLAgentServer::GetInstance().SENDTOCLIENT ( m_dwClientID, &msg );
	}

}

void GLCharAG::CaptchaReset()
{
	//reset all captcha related values
	m_fCaptchaShowTimer = 0.0f;
	m_fCaptchaWaitTimer = 0.0f;
	m_bCaptchaShow = false;
	m_dwCaptchaValue = 0;
}