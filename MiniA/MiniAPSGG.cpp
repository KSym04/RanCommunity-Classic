#include "stdafx.h"
#include "./MiniA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef USE_PSGG

BOOL CMiniAApp::PSGGStart()
{
	m_hPSGG = LoadLibraryA("PS.dll");
	if (m_hPSGG)
	{
		if ( GetProcAddress(m_hPSGG, "wzAudioPlay") == NULL )
		{
			m_hPSGG = NULL;
			return FALSE;
		}
	}
	else 
	{
		return FALSE;
	}

	return TRUE;
}

#endif
