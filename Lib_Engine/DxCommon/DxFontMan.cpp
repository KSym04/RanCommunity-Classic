#include "stdafx.h"
#include "./D3DFont.h"
#include "./D3DFontX.h"

#include "./dxfontman.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::string _DEFAULT_FONT			= _T("Tahoma");
char _BOLD_FONT[128] = _T("Tahoma Bold");
DWORD _DEFAULT_FONT_FLAG			= 0;
DWORD _DEFAULT_FONT_SHADOW_FLAG		= D3DFONT_SHADOW | D3DFONT_KSC5601;
DWORD _DEFAULT_FONT_SHADOW_EX_FLAG	= D3DFONT_SHADOW_EX | D3DFONT_KSC5601;
DWORD _DEFAULT_FONT_BOLD_FLAG		= D3DFONT_BOLD | D3DFONT_KSC5601;

LPD3DXSPRITE DxFontMan::g_pSprite = NULL;

namespace COMMENT
{
	std::string FONTSYS[2] =
	{
		"D3DFONT",
		"D3DXFONT"
	};
};

// Define code pages 
int CD3DFontPar::nCodePage[language::LANG_NUM] =
{
	0,			// DEFAULT 
	950,		// TRADITIONAL CHINESE
	932,		// JAPANESE
	949,		// KOREAN
	936,		// SIMPLIFIED CHINESE
	874,		// THAILAND WINDOWS_874
	1258,		// VIETNAM WINDOWS_1258
	//	1252,		// INDONESIA
};

// Define charset
int CD3DFontPar::nCharSet[language::LANG_NUM] =
{
	DEFAULT_CHARSET,		// Default 
	CHINESEBIG5_CHARSET,	// TRADITIONAL CHINESE
	SHIFTJIS_CHARSET,		// JAPANESE
	HANGUL_CHARSET,			// KOREAN
	GB2312_CHARSET,			// SIMPLIFIED CHINESE
	DEFAULT_CHARSET,		// THAILAND - THAI_CHARSET There is a problem with the setting.
	VIETNAMESE_CHARSET,		// VIETNAM
	//	ARABIC_CHARSET,
	//	INDONESIAN_CHARSET,
};

// Define Default font
char CD3DFontPar::szDefaultFontName[language::LANG_NUM][50] =
{
	"\x54\x00\x61\x00\x68\x00\x6F\x00\x6D\x00\x61\x00\x00\x00",					// Default font
	"\xB0\x65\x30\x7D\x0E\x66\xD4\x9A\x00\x00",									// TRADITIONAL_CHINESE font
	"\x2D\xFF\x33\xFF\x20\x00\x30\xFF\xB4\x30\xB7\x30\xC3\x30\xAF\x30\x00\x00",	// JAPANESE font
	"\x74\xAD\xBC\xB9\x00\x00",													// KOREAN font
	"\xB0\x65\x8B\x5B\x53\x4F\x00\x00",											// SIMPLIFIED_CHINESE font
	"\x41\x00\x72\x00\x69\x00\x61\x00\x6C\x00\x00\x00",							// Arial
	"\x41\x00\x72\x00\x69\x00\x61\x00\x6C\x00\x00\x00",							// Arial
	//	"\x41\x00\x72\x00\x69\x00\x61\x00\x6C\x00\x00\x00",							// Arial
};

TCHAR CD3DFontPar::szEnglishFontName[language::LANG_NUM][20] =
{
	_T("Tahoma"),		// Default font
	_T("MingLiU"),		// TRADITIONAL_CHINESE font
	_T("MS PGothic"),	// JAPANESE font
	_T("Gulim"),		// KOREAN font
	_T("NSimSun"),		// SIMPLIFIED_CHINESE font
	_T("Arial"),		// THAILAND font
	_T("Arial"),		// VIETNAM font
	//	_T("Arial"),	// Indonesia
};

char CD3DFontPar::szLocaleName[language::LANG_NUM][50] =
{
	_T("C"),					// Default font
	_T("Chinese_Taiwan"),		// TRADITIONAL_CHINESE font
	_T(" "),					// JAPANESE font
	_T("Korean"),				// KOREAN font
	_T("Chinese-simplified"),	// SIMPLIFIED_CHINESE font
	_T("thai"),					// THAILAND font
	_T("vietnamese"),			// VIETNAM font
	//	_T("Indonesian")		// INDONESIA font
};

WCHAR CD3DFontPar::szInstallLayoutOrTipName[language::LANG_NUM][250] =
{
	/* Default					*/	L"",
	/* TRADITIONALL	- Taiwan	*/	L"0404:{531FDEBF-9B4C-4A43-A2AA-960E8FCDC732}{B2F9C502-1742-11D4-9790-0080C882687E}",	// Sinjueum, Jueum - 0404:{531FDEBF-9B4C-4A43-A2AA-960E8FCDC732}{761309DE-317A-11D4-9B5D-0080C882687E}
	/* JAPANESE					*/	L"",
	/* KOREAN					*/	L"0409:00000409",
	/* SIMPLIFIED - China		*/	L"0804:{81D4E9C9-1D3B-41BC-9E6C-4B40BF79E35E}{F3BA9077-6C7E-11D4-97FA-0080C882687E}",
	/* THAILAND					*/	L"041e:0000041e",
	/* VIETNAM					*/	L"042a:0000042a",

};

// Keyboard layout values are different for each os 
// It seems that the value should be changed according to the basic keyboard data; HKEY_CURRENT_USER/KeyBoard Layout/Preload/1
char CD3DFontPar::szVistaKeyboardLayout[language::LANG_NUM][250] = 
{
	/* Default					*/ "",
	/* TRADITIONALL	- Taiwan	*/ "00000404",
	/* JAPANESE					*/ "",
	/* KOREAN					*/ "00000409",
	/* SIMPLIFIED - China		*/ "00000804",//"00000804",
	/* THAILAND					*/ "0000041e",
	/* VIETNAM					*/ "0000042a",
	//	/* INDONESIA				*/ "00000409",
};

char CD3DFontPar::szKeyboardLayout[language::LANG_NUM][250] = 
{
	/* Default					*/ "",
	/* TRADITIONALL	- Taiwan	*/ "00000404",
	/* JAPANESE					*/ "",
	/* KOREAN					*/ "00000409",
	/* SIMPLIFIED - China		*/ "e00e0804",//"00000804",
	/* THAILAND					*/ "0000041e",
	/* VIETNAM					*/ "0000042a",
	//	/* INDONESIA				*/ "00000409",
};

HKL CD3DFontPar::HklList[language::LANG_NUM][10] =
{
	{HKL(0x00000000),},
	{HKL(0x04040404), HKL(0x0)},
	{HKL(0x00000000),},
	{HKL(0x04120412), HKL(0xe0200412)},
	{HKL(0x08040804), HKL(0x00000804), HKL(0xe00e0804), HKL(0x0)},
	{HKL(0x041e041e), HKL(0x0)},
	{HKL(0x042a042a), HKL(0x0)},
	//	{HKL(0x04210421), HKL(0x0)},
};

int CD3DFontPar::GetCodePage ( language::LANGFLAG nLang )
{
	return nCodePage[nLang];
}

BOOL CD3DFontPar::IsExistenceKeyboardLayout( const language::LANGFLAG nLang, const HKL hKeyboardLayout )
{
	const int iHKLSize = sizeof( HklList[nLang] )/sizeof(HKL);
	for ( int i = 0; HklList[nLang][i] != NULL && i < iHKLSize; ++i )
	{
		if ( HklList[nLang][i] == hKeyboardLayout )
			return TRUE;
	}
	return FALSE;
}

INT CD3DFontPar::MultiToWide( const TCHAR* strText, INT iText, WCHAR* wstrText, INT iWText )
{
	if( !strText || !iText || !wstrText || !iWText )
		return 0;
	return MultiByteToWideChar( nCodePage[ m_emLangFlag ], 0, strText, iText, wstrText, iWText );
}

INT CD3DFontPar::WideToMulti( const WCHAR* wstrText, INT iWText, TCHAR* strText, INT iText )
{
	if( !strText || !iText || !wstrText || !iWText )
		return 0;
	return WideCharToMultiByte( nCodePage[ m_emLangFlag ], 0, wstrText, iWText, strText, iText, NULL, NULL );
}

DxFontMan& DxFontMan::GetInstance()
{
	static DxFontMan Instance;
	return Instance;
}

DxFontMan::DxFontMan(void) :
	m_pd3dDevice(NULL),
	m_emFontSys(EMFONT_D3D),
	m_dwBufferMode(0),
	m_emLang(language::KOREAN)
{
}

DxFontMan::~DxFontMan(void)
{
	 DeleteDxFontAll();
}

HRESULT DxFontMan::SetFontSys ( const CString& strFontSys, language::LANGFLAG dwLang )
{
	if ( strFontSys==COMMENT::FONTSYS[0].c_str() )
		m_emFontSys = EMFONT_D3D;
	else if ( strFontSys==COMMENT::FONTSYS[1].c_str() )
		m_emFontSys = EMFONT_D3DX;

	m_emLang = dwLang;

	return S_OK;
}

CD3DFontPar* DxFontMan::FindFont ( const std::string& FontName, DWORD dwHeight, DWORD dwFlags )
{
	FONT_MAP_ITER iter = m_FontData.find(boost::make_tuple(FontName, dwHeight, dwFlags));
	if (iter != m_FontData.end())
		return iter->second;
	else
		return NULL;
}

CD3DFontPar* DxFontMan::LoadDxFont ( const std::string& FontName, DWORD dwHeight, DWORD dwFlags )
{
	CD3DFontPar* pD3dFont = FindFont ( FontName, dwHeight, dwFlags );
	if ( pD3dFont )
		return pD3dFont;

	//	Note : 새로운 폰트를 할당.
	//
	CD3DFontPar *pD3dFontNew=NULL;
	switch ( m_emFontSys )
	{
	case EMFONT_D3D:
		//if( RANPARAM::emSERVICE_TYPE != EMSERVICE_KOREA )
		//{
			pD3dFontNew = new CD3DFont ( FontName.c_str(), dwHeight, m_dwBufferMode, dwFlags, m_emLang);
			break;
		//}

	case EMFONT_D3DX:
		pD3dFontNew = new CD3DFontX ( FontName.c_str(), dwHeight, dwFlags, m_emLang);
		break;
	};

	//	Note : 새로운 폰트의 데이터 초기화.
	//
	if ( m_pd3dDevice )
		pD3dFontNew->Create ( m_pd3dDevice );

	//	Note : 새로운 폰트를 리스트에 삽입.
	//
	 m_FontData.insert(FONT_MAP_VALUE(boost::make_tuple(FontName, dwHeight, dwFlags), pD3dFontNew));

	return pD3dFontNew;
}

void DxFontMan::DeleteDxFont ( const std::string& FontName, DWORD dwHeight, DWORD dwFlags )
{
	FONT_MAP_ITER iter = m_FontData.find(boost::make_tuple(FontName, dwHeight, dwFlags));
	if (iter != m_FontData.end())
	{
		CD3DFontPar* pFont = iter->second;
		pFont->CleanUp();
		delete pFont;
		m_FontData.erase(iter);
	}
}

void DxFontMan::DeleteDxFontAll ()
{
	for (FONT_MAP_ITER iter=m_FontData.begin(); iter!=m_FontData.end(); ++iter)
	{
		CD3DFontPar* pFont = iter->second;
		pFont->CleanUp();
		delete pFont;
	}
	m_FontData.clear();
}

void DxFontMan::AddRef ( CD3DFontPar* pD3dFont )
{
	pD3dFont->AddRef ();
}

void DxFontMan::ReleaseDxFont ( const std::string& FontName, DWORD dwHeight, DWORD dwFlags )
{
	CD3DFontPar* pD3dFont = FindFont(FontName, dwHeight, dwFlags);
	if (pD3dFont && pD3dFont->SubRef () <= 0)
		DeleteDxFont(FontName, dwHeight, dwFlags);
}

void DxFontMan::ReleaseDxFont ( CD3DFontPar *pD3dFont )
{
	if ( pD3dFont->SubRef () <= 0 )
		DeleteDxFont ( pD3dFont->GetFontName(), pD3dFont->GetFontHeight(), pD3dFont->GetFontFlags() );
}

HRESULT DxFontMan::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	//	Note : d3dfont 를 사용하는 것이 타당한지 검사.
	//
	LPDIRECT3DQ pD3D=NULL;
	m_pd3dDevice->GetDirect3D ( &pD3D );

	D3DCAPSQ d3dCaps;
	D3DSURFACE_DESC d3dsdBackBuffer;

	pd3dDevice->GetDeviceCaps ( &d3dCaps );

	LPDIRECT3DSURFACEQ pBackBuffer;
	m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

    pBackBuffer->GetDesc ( &d3dsdBackBuffer );
    pBackBuffer->Release ();

	bool validD3DFONT(false);
	if ( (512<=d3dCaps.MaxTextureWidth) && (1024<=d3dCaps.MaxTextureHeight) )
	{
		HRESULT hr = pD3D->CheckDeviceFormat ( d3dCaps.AdapterOrdinal, d3dCaps.DeviceType, d3dsdBackBuffer.Format,
								0, D3DRTYPE_TEXTURE, D3DFMT_DXT1 );
		if ( SUCCEEDED(hr) )
		{
			validD3DFONT = true;
			m_dwBufferMode = 0;
		}
	}

	if ( !validD3DFONT )
	{
		if ( (512<=d3dCaps.MaxTextureWidth) && (512<=d3dCaps.MaxTextureHeight) )
		{
			validD3DFONT = true;
			m_dwBufferMode = 1;
		}
		else if ( (256<=d3dCaps.MaxTextureWidth) && (256<=d3dCaps.MaxTextureHeight) )
		{
			validD3DFONT = true;
			m_dwBufferMode = 2;
		}
	}

	if ( validD3DFONT==false && m_emFontSys==EMFONT_D3D )
		m_emFontSys = EMFONT_D3DX;

	//	Note : 참조한 D3D의 카운트를 감소시킴.
	//
	if ( pD3D )
		pD3D->Release();

	for (FONT_MAP_ITER iter=m_FontData.begin(); iter!=m_FontData.end(); ++iter)
	{
		CD3DFontPar* pFont = iter->second;
		pFont->InitDeviceObjects(pd3dDevice);
	}

	return S_OK;
}

HRESULT DxFontMan::RestoreDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
	for (FONT_MAP_ITER iter=m_FontData.begin(); iter!=m_FontData.end(); ++iter)
	{
		CD3DFontPar* pFont = iter->second;
		pFont->RestoreDeviceObjects();
	}

	SAFE_RELEASE( g_pSprite );
	D3DXCreateSprite( pd3dDevice, &g_pSprite );

	return S_OK;
}

HRESULT DxFontMan::InvalidateDeviceObjects()
{
	for (FONT_MAP_ITER iter=m_FontData.begin(); iter!=m_FontData.end(); ++iter)
	{
		CD3DFontPar* pFont = iter->second;
		pFont->InvalidateDeviceObjects();
	}

	SAFE_RELEASE( g_pSprite );

	return S_OK;
}

HRESULT DxFontMan::DeleteDeviceObjects()
{
	for (FONT_MAP_ITER iter=m_FontData.begin(); iter!=m_FontData.end(); ++iter)
	{
		CD3DFontPar* pFont = iter->second;
		pFont->DeleteDeviceObjects();
	}

	return S_OK;
}