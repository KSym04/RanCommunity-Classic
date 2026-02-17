#include "StdAfx.h"
#include <process.h>
#include "./LoadingThread.h"

#include "../../Lib_Engine/Common/SubPath.h"
#include "../../Lib_Engine/DxCommon/DxGrapUtils.h"
#include "../../Lib_Engine/DxResponseMan.h"
#include "./UITextControl.h"
#include "./GameTextControl.h"

#include "../../Lib_Engine/DxCommon/TextureManager.h"
#include "../../Lib_Engine/DxCommon/DxFontMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace NLOADINGTHREAD;
using namespace NLOADINGTIP;

//	-------------------- [ CUSTOM VERTEX 설정 ] --------------------
const	DWORD TEXTUREVERTEXFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
struct TEXTUREVERTEX
{
	union
	{
		struct { D3DXVECTOR4 vPos; };
		struct { float x, y, z, rhw; };
	};

	DWORD Diffuse;

	union
	{
		struct { D3DXVECTOR2 vUV; };
		struct { float tu, tv; };
	};

	TEXTUREVERTEX () :
		x(0.0f),
		y(0.0f),
		z(0.0f),
		rhw(1.0f),
		Diffuse(0xFFFFFFFF),
		tu(0.0f),
		tv(0.0f)
	{		
	}

	TEXTUREVERTEX ( float _x, float _y, float _tu, float _tv ) :
		x(_x),
		y(_y),
		z(0.0f),
		rhw(1.0f),
		Diffuse(0xFFFFFFFF),
		tu(_tu),
		tv(_tv)
	{		
	}

	void	SetPos ( float _x, float _y )
	{
		x = _x;
		y = _y;
	}

	void	SetTexPos ( float _tu, float _tv )
	{
		tu = _tu;
		tv = _tv;
	}
};

//	-------------------- [ CUSTOM VERTEX 제어 메소드 설정 ] --------------------

static HRESULT	CreateVB ( LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DVERTEXBUFFERQ& pTextureVB, TEXTUREVERTEX VertexArray[6] )
{	
	HRESULT hr = S_OK;
	hr = pd3dDevice->CreateVertexBuffer( 6*sizeof(TEXTUREVERTEX), 0, TEXTUREVERTEXFVF,
											D3DPOOL_MANAGED, &pTextureVB, NULL );
    if( FAILED ( hr ) )	return hr;    

    VOID* pVertices;
	hr = pTextureVB->Lock( 0, sizeof ( TEXTUREVERTEX ) * 6, (VOID**)&pVertices, 0 );
    if( FAILED ( hr ) ) return hr;

    memmove( pVertices, VertexArray, sizeof ( TEXTUREVERTEX ) * 6 );

    hr = pTextureVB->Unlock();
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}

static void SetVertexPos ( TEXTUREVERTEX VertexArray[6], float LEFT, float TOP, float SIZEX, float SIZEY )
{
	float RIGHT = LEFT + SIZEX;
	float BOTTOM = TOP + SIZEY;

	VertexArray[0].SetPos ( LEFT,	TOP );
	VertexArray[1].SetPos ( RIGHT,	TOP);
	VertexArray[2].SetPos ( LEFT,	BOTTOM );

	VertexArray[3].SetPos ( LEFT,	BOTTOM );
	VertexArray[4].SetPos ( RIGHT,	TOP);
	VertexArray[5].SetPos ( RIGHT,	BOTTOM );
}

static void SetTexturePos ( TEXTUREVERTEX VertexArray[6], float LEFT, float TOP, float SIZEX, float SIZEY, float TEX_SIZEX, float TEX_SIZEY )
{
	float RIGHT = LEFT + SIZEX;
	float BOTTOM = TOP + SIZEY;

	VertexArray[0].SetTexPos ( LEFT / TEX_SIZEX,	TOP / TEX_SIZEY );
	VertexArray[1].SetTexPos ( RIGHT / TEX_SIZEX,	TOP / TEX_SIZEY);
	VertexArray[2].SetTexPos ( LEFT / TEX_SIZEX,	BOTTOM / TEX_SIZEY );

	VertexArray[3].SetTexPos ( LEFT / TEX_SIZEX,	BOTTOM / TEX_SIZEY );
	VertexArray[4].SetTexPos ( RIGHT / TEX_SIZEX,	TOP / TEX_SIZEY );
	VertexArray[5].SetTexPos ( RIGHT / TEX_SIZEX,	BOTTOM / TEX_SIZEY );
}

static HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice, LPDIRECT3DTEXTUREQ pLoadingTexture, LPDIRECT3DVERTEXBUFFERQ pTextureVB )
{
	HRESULT hr = S_OK;

	hr = pd3dDevice->SetTexture ( 0, pLoadingTexture );
	if ( FAILED ( hr ) ) return hr;

    hr = pd3dDevice->SetStreamSource( 0, pTextureVB, 0, sizeof(TEXTUREVERTEX) );
	if ( FAILED ( hr ) ) return hr;

	hr = pd3dDevice->SetFVF( TEXTUREVERTEXFVF );
	if ( FAILED ( hr ) ) return hr;

	hr = pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
	if ( FAILED ( hr ) ) return hr;

	hr = pd3dDevice->SetTexture ( 0, NULL );
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}

BOOL MessagePump()
{
   MSG msg;

   while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
   {
       if(AfxGetApp() -> PumpMessage())
       {
             ::PostQuitMessage(0);
             return FALSE;
       }
   }

   return TRUE;
}

// Add PC performance detection and adaptive loading
static DWORD GetPCPerformanceScore()
{
    DWORD dwScore = 0;
    
    // Get CPU info
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    dwScore += sysInfo.dwNumberOfProcessors * 100; // More cores = faster
    
    // Get memory info
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    dwScore += (DWORD)(memInfo.ullTotalPhys / (1024 * 1024 * 1024)) * 50; // More RAM = faster
    
    // Get disk performance (rough estimate)
    DWORD dwSectorsPerCluster, dwBytesPerSector, dwNumberOfFreeClusters, dwTotalNumberOfClusters;
    if (GetDiskFreeSpace(NULL, &dwSectorsPerCluster, &dwBytesPerSector, &dwNumberOfFreeClusters, &dwTotalNumberOfClusters))
    {
        dwScore += (dwBytesPerSector * dwSectorsPerCluster) / 1024; // Faster disk = higher score
    }
    
    return dwScore;
}

// Adaptive loading time calculation based on PC performance
static DWORD CalculateAdaptiveLoadingTime()
{
    DWORD dwPerformanceScore = GetPCPerformanceScore();
    
    // ULTRA FAST loading time based on performance
    if (dwPerformanceScore > 1000) // High-end PC
        return 300;  // 0.3 seconds - INSTANT!
    else if (dwPerformanceScore > 500) // Mid-range PC
        return 500;  // 0.5 seconds - FAST!
    else if (dwPerformanceScore > 200) // Low-end PC
        return 1000; // 1 second
    else // Very old PC
        return 1500; // 1.5 seconds
}

// Enhanced progress calculation with 100 steps
static float CalculateAdaptiveProgress(DWORD dwStartTime, DWORD dwTotalTime)
{
    DWORD dwCurrentTime = GetTickCount();
    DWORD dwElapsed = dwCurrentTime - dwStartTime;
    
    float fProgress = static_cast<float>(dwElapsed) / static_cast<float>(dwTotalTime);
    
    // Clamp between 0.0 and 1.0
    if (fProgress < 0.0f) fProgress = 0.0f;
    if (fProgress > 1.0f) fProgress = 1.0f;
    
    return fProgress;
}

// Background worker style frame rate control - OPTIMIZED FOR FAST LOADING
static void FrameRateControl(DWORD& dwLastTime, DWORD dwTargetFPS = 120)
{
    DWORD dwCurrentTime = GetTickCount();
    DWORD dwFrameTime = 1000 / dwTargetFPS;
    
    // Optimized for fast loading - minimal sleep
    if (dwCurrentTime - dwLastTime < dwFrameTime)
    {
        // Minimal yield for fast loading
        Sleep(0);
    }
    dwLastTime = GetTickCount();
}

// Add background resource cleanup system
namespace NRESOURCE_CLEANUP
{
    HANDLE hCleanupThread = NULL;
    BOOL bCleanupRunning = FALSE;
    CRITICAL_SECTION csCleanupQueue;
    std::vector<LPDIRECT3DTEXTUREQ> vecTexturesToRelease;
    std::vector<LPDIRECT3DVERTEXBUFFERQ> vecVertexBuffersToRelease;
    std::vector<LPD3DXSPRITE> vecSpritesToRelease;
    std::vector<std::string> vecTextureNamesToRelease;
    
    // Add immediate cleanup flag for critical resources
    BOOL bImmediateCleanup = FALSE;

    unsigned int WINAPI BackgroundCleanupThread(LPVOID pData)
    {
        while (bCleanupRunning)
        {
            EnterCriticalSection(&csCleanupQueue);
            
            // Process texture releases
            for (size_t i = 0; i < vecTexturesToRelease.size(); i++)
            {
                SAFE_RELEASE(vecTexturesToRelease[i]);
            }
            vecTexturesToRelease.clear();
            
            // Process vertex buffer releases
            for (size_t i = 0; i < vecVertexBuffersToRelease.size(); i++)
            {
                SAFE_RELEASE(vecVertexBuffersToRelease[i]);
            }
            vecVertexBuffersToRelease.clear();
            
            // Process sprite releases
            for (size_t i = 0; i < vecSpritesToRelease.size(); i++)
            {
                SAFE_RELEASE(vecSpritesToRelease[i]);
            }
            vecSpritesToRelease.clear();
            
            // Process texture name releases
            for (size_t i = 0; i < vecTextureNamesToRelease.size(); i++)
            {
                LPDIRECT3DTEXTUREQ pDummyTexture = NULL;
                TextureManager::ReleaseTexture(vecTextureNamesToRelease[i].c_str(), pDummyTexture);
            }
            vecTextureNamesToRelease.clear();
            
            LeaveCriticalSection(&csCleanupQueue);
            
            		// More aggressive cleanup during loading - reduce sleep time
		Sleep(3); // Further reduced to 3ms for even faster cleanup
        }
        
        return 0;
    }
    
    // Initialize background cleanup system
    HRESULT InitializeBackgroundCleanup()
    {
        InitializeCriticalSection(&csCleanupQueue);
        bCleanupRunning = TRUE;
        bImmediateCleanup = FALSE;
        
        hCleanupThread = (HANDLE)_beginthreadex(
            NULL,
            0,
            BackgroundCleanupThread,
            NULL,
            0,
            NULL);
            
        if (!hCleanupThread)
        {
            DeleteCriticalSection(&csCleanupQueue);
            return E_FAIL;
        }
        
        return S_OK;
    }
    
    // Queue texture for background release
    void QueueTextureRelease(LPDIRECT3DTEXTUREQ pTexture)
    {
        if (pTexture)
        {
            EnterCriticalSection(&csCleanupQueue);
            vecTexturesToRelease.push_back(pTexture);
            LeaveCriticalSection(&csCleanupQueue);
            
            // Force immediate cleanup for critical resources
            if (bImmediateCleanup)
            {
                Sleep(1); // Brief pause to allow cleanup
            }
        }
    }
    
    // Queue vertex buffer for background release
    void QueueTextureRelease(LPDIRECT3DVERTEXBUFFERQ pVertexBuffer)
    {
        if (pVertexBuffer)
        {
            EnterCriticalSection(&csCleanupQueue);
            vecVertexBuffersToRelease.push_back(pVertexBuffer);
            LeaveCriticalSection(&csCleanupQueue);
            
            // Force immediate cleanup for critical resources
            if (bImmediateCleanup)
            {
                Sleep(1); // Brief pause to allow cleanup
            }
        }
    }
    
    // Queue sprite for background release
    void QueueTextureRelease(LPD3DXSPRITE pSprite)
    {
        if (pSprite)
        {
            EnterCriticalSection(&csCleanupQueue);
            vecSpritesToRelease.push_back(pSprite);
            LeaveCriticalSection(&csCleanupQueue);
            
            // Force immediate cleanup for critical resources
            if (bImmediateCleanup)
            {
                Sleep(1); // Brief pause to allow cleanup
            }
        }
    }
    
    // Queue texture name for background release
    void QueueTextureNameRelease(const char* szTextureName)
    {
        if (szTextureName)
        {
            EnterCriticalSection(&csCleanupQueue);
            vecTextureNamesToRelease.push_back(std::string(szTextureName));
            LeaveCriticalSection(&csCleanupQueue);
            
            // Force immediate cleanup for critical resources
            if (bImmediateCleanup)
            {
                Sleep(1); // Brief pause to allow cleanup
            }
        }
    }
    
    // Enable immediate cleanup mode for critical loading phases
    void EnableImmediateCleanup(BOOL bEnable)
    {
        bImmediateCleanup = bEnable;
    }
    
    // Force immediate cleanup of all queued resources
    void ForceImmediateCleanup()
    {
        EnterCriticalSection(&csCleanupQueue);
        
        // Process all queued resources immediately
        for (size_t i = 0; i < vecTexturesToRelease.size(); i++)
        {
            SAFE_RELEASE(vecTexturesToRelease[i]);
        }
        vecTexturesToRelease.clear();
        
        for (size_t i = 0; i < vecVertexBuffersToRelease.size(); i++)
        {
            SAFE_RELEASE(vecVertexBuffersToRelease[i]);
        }
        vecVertexBuffersToRelease.clear();
        
        for (size_t i = 0; i < vecSpritesToRelease.size(); i++)
        {
            SAFE_RELEASE(vecSpritesToRelease[i]);
        }
        vecSpritesToRelease.clear();
        
        for (size_t i = 0; i < vecTextureNamesToRelease.size(); i++)
        {
            LPDIRECT3DTEXTUREQ pDummyTexture = NULL;
            TextureManager::ReleaseTexture(vecTextureNamesToRelease[i].c_str(), pDummyTexture);
        }
        vecTextureNamesToRelease.clear();
        
        LeaveCriticalSection(&csCleanupQueue);
    }
    
    // Shutdown background cleanup system
    void ShutdownBackgroundCleanup()
    {
        bCleanupRunning = FALSE;
        
        // Force final cleanup before shutdown
        ForceImmediateCleanup();
        
        if (hCleanupThread)
        {
            WaitForSingleObject(hCleanupThread, 5000); // Wait up to 5 seconds
            CloseHandle(hCleanupThread);
            hCleanupThread = NULL;
        }
        
        DeleteCriticalSection(&csCleanupQueue);
    }
}

unsigned int WINAPI	LoadingThread( LPVOID pData )
{
	if( n_strTextureName.IsEmpty() || n_strTextureName == _T("null") )
	{
		srand ( (UINT)time( NULL ) );
		int nIndex = (rand() % 2) + 11;

		n_strTextureName.Format( "loading_%03d.dds", nIndex );	
	}

	TCHAR szTexture[256] = {0};
	StringCchPrintf ( szTexture, 256, n_strTextureName.GetString(), n_szAppPath, SUBPATH::TEXTURE_FILE_ROOT );

	LPDIRECT3DDEVICEQ& pd3dDevice = *n_ppd3dDevice;
	LPDIRECT3DTEXTUREQ pLoadingTexture = NULL;
	LPDIRECT3DTEXTUREQ pLoadingUnderTex = NULL;
	LPDIRECT3DTEXTUREQ pLoadingTopTex = NULL;
	LPDIRECT3DTEXTUREQ pLoadingStepTex = NULL;
	LPDIRECT3DTEXTUREQ pLoadingBackTex = NULL;
	LPDIRECT3DTEXTUREQ pHintIconTex = NULL;
	LPDIRECT3DTEXTUREQ pMapNameBackTex = NULL;

	std::string	  m_strLoadingStepTex= "loading_st.dds";
	std::string	  m_strLoadingBackTex = "ld_back.dds";
	std::string	  m_strLoadingUnderTex = "ld_under.dds";
	std::string	  m_strLoadingTopTex = "ld_top.dds";
	std::string	  m_strHintIconTex = "HintIcon.dds";
	std::string	  m_strMapNameBackTex = "mapnameback.dds";

	LPD3DXSPRITE pLoadingStepSprite = NULL;

	HRESULT hr = S_OK;

	if ( FAILED ( TextureManager::LoadTexture( szTexture, pd3dDevice, pLoadingTexture, 0, 0 ) ) )	
	{
		StringCchPrintf ( szTexture, 256, "loading_000.dds", n_szAppPath, SUBPATH::TEXTURE_FILE_ROOT );

		if ( FAILED ( TextureManager::LoadTexture( szTexture, pd3dDevice, pLoadingTexture, 0, 0 ) ) )	
		{
			return	ErrorLoadingTexture();
		}
	}

	if ( FAILED ( TextureManager::LoadTexture( m_strLoadingStepTex.c_str(), pd3dDevice, pLoadingStepTex, 0, 0 ) ) )	
		return	ErrorLoadingTexture();
	if ( FAILED ( TextureManager::LoadTexture( m_strLoadingBackTex.c_str(), pd3dDevice, pLoadingBackTex, 0, 0 ) ) )	
		return	ErrorLoadingTexture();
	if ( FAILED ( TextureManager::LoadTexture( m_strLoadingUnderTex.c_str(), pd3dDevice, pLoadingUnderTex, 0, 0 ) ) )	
		return	ErrorLoadingTexture();
	if ( FAILED ( TextureManager::LoadTexture( m_strLoadingTopTex.c_str(), pd3dDevice, pLoadingTopTex, 0, 0 ) ) )	
		return	ErrorLoadingTexture();
	if ( FAILED ( TextureManager::LoadTexture( m_strHintIconTex.c_str(), pd3dDevice, pHintIconTex, 0, 0 ) ) )	
		return	ErrorLoadingTexture();
	if ( FAILED ( TextureManager::LoadTexture( m_strMapNameBackTex.c_str(), pd3dDevice, pMapNameBackTex, 0, 0 ) ) )	
		return	ErrorLoadingTexture();

	DeWait ();

	RECT rect;
	GetClientRect ( n_hWnd, &rect );

	const float fWidth = float(rect.right - rect.left);
	const float fHeight = float(rect.bottom - rect.top);

	const float	fRealImageX = 1024.0f;
	const float	fRealImageY = 768.0f;
	const float	fImageSize = 1023.0f;

	const float fWidthRatio = fRealImageX / fImageSize;
	const float fHeightRatio= fRealImageY / fImageSize;

	const float LeftPos  = 0.0f;
	const float TopPos   = 0.0f;
	const float RightPos = fWidth;
	const float BottomPos= fHeight;

	float ROOT_LEFT		= LeftPos;
	float ROOT_TOP		= TopPos;
	float ROOT_SIZEX	= fWidth;
	float ROOT_SIZEY	= fHeight;

	const D3DXVECTOR2 vld_topTexPos ( 0, 0 );
	const D3DXVECTOR2 vld_topTexSize ( 1024, 256 );
	const D3DXVECTOR2 vld_topTexRecSize ( 1024, 140 );
	const D3DXVECTOR2 vld_topRenderPos ( 0, 0 );
	const D3DXVECTOR2 vld_topRenderSize ( 1024, 128 );

	const D3DXVECTOR2 vld_midTexPos ( 0, 0 );
	const D3DXVECTOR2 vld_midTexSize ( 1024, 512 );
	const D3DXVECTOR2 vld_midTexRecSize ( 1024, 512 );
	const D3DXVECTOR2 vld_midRenderPos ( 0, 128 );
	const D3DXVECTOR2 vld_midRenderSize ( 1024, 512 );

	const D3DXVECTOR2 vld_underTexPos ( 0, 7 );
	const D3DXVECTOR2 vld_underTexSize ( 1024, 256 );
	const D3DXVECTOR2 vld_underTexRecSize ( 1024, 140 );
	const D3DXVECTOR2 vld_underRenderPos ( 0, 640 );
	const D3DXVECTOR2 vld_underRenderSize ( 1024, 128 );

	const D3DXVECTOR2 vld_backTexPos ( 0, 0 );
	const D3DXVECTOR2 vld_backTexSize ( 128, 128 );
	const D3DXVECTOR2 vld_backTexRecSize ( 128, 128 );
	const D3DXVECTOR2 vld_backRenderSize ( 128, 128 );
	const D3DXVECTOR2 vld_backRenderPos ( fWidth - (vld_backRenderSize.x+15.0f), fHeight -(vld_backRenderSize.y+5.0f) );

	const D3DXVECTOR2 vld_HintIconTexPos ( 0, 0 );
	const D3DXVECTOR2 vld_HintIconTexSize ( 128, 64 );
	const D3DXVECTOR2 vld_HintIconTexRecSize ( 100, 60 );
	const D3DXVECTOR2 vld_HintIconRenderPos ( 15, 645 );
	const D3DXVECTOR2 vld_HintIconRenderSize ( 100, 60 );

	const D3DXVECTOR2 vld_MapNameBackTexPos ( 0, 0 );
	const D3DXVECTOR2 vld_MapNameBackTexSize ( 512, 64 );
	const D3DXVECTOR2 vld_MapNameBackTexRecSize ( 336, 57 );
	const D3DXVECTOR2 vld_MapNameBackRenderSize ( 336, 57 );
	const D3DXVECTOR2 vld_MapNameBackRenderPos( (fWidth - vld_MapNameBackRenderSize.x) /2 , (vld_topRenderSize.y - vld_MapNameBackRenderSize.y) /2);

	D3DXVECTOR2 vCopyrightAlign;
	vCopyrightAlign.x = 15.0f;
	vCopyrightAlign.y = fHeight - 30.0f;

	D3DXVECTOR2 vld_topAlignSize;
	vld_topAlignSize.x = static_cast<float>(floor(vld_topRenderSize.x * fWidth / fRealImageX));
	vld_topAlignSize.y = static_cast<float>(floor(vld_topRenderSize.y * fHeight / fRealImageY));

	D3DXVECTOR2 vld_topAlignPos;
	vld_topAlignPos.x = static_cast<float>(floor(vld_topRenderPos.x * fWidth / fRealImageX));
	vld_topAlignPos.y = static_cast<float>(floor(vld_topRenderPos.y * fHeight / fRealImageY));

	D3DXVECTOR2 vld_midAlignSize;
	vld_midAlignSize.x = static_cast<float>(floor(vld_midRenderSize.x * fWidth / fRealImageX));
	vld_midAlignSize.y = static_cast<float>(floor(vld_midRenderSize.y * fHeight / fRealImageY));
	D3DXVECTOR2 vld_midAlignPos;
	vld_midAlignPos.x = static_cast<float>(floor(vld_midRenderPos.x * fWidth / fRealImageX));
	vld_midAlignPos.y = static_cast<float>(floor(vld_midRenderPos.y * fHeight / fRealImageY));

	D3DXVECTOR2 vld_underAlignSize;
	vld_underAlignSize.x = static_cast<float>(floor(vld_underRenderSize.x * fWidth / fRealImageX));
	vld_underAlignSize.y = static_cast<float>(floor(vld_underRenderSize.y * fHeight / fRealImageY));

	D3DXVECTOR2 vld_underAlignPos;
	vld_underAlignPos.x = static_cast<float>(floor(vld_underRenderPos.x * fWidth / fRealImageX));
	vld_underAlignPos.y = static_cast<float>(floor(vld_underRenderPos.y * fHeight / fRealImageY));

	D3DXVECTOR2 vld_HintIconAlignPos;
	vld_HintIconAlignPos.x = static_cast<float>(floor(vld_HintIconRenderPos.x * fWidth / fRealImageX));
	vld_HintIconAlignPos.y = static_cast<float>(floor(vld_HintIconRenderPos.y * fHeight / fRealImageY));

	LPDIRECT3DVERTEXBUFFERQ pTextureVB	= NULL; 
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_midAlignPos.x, ROOT_TOP + vld_midAlignPos.y, vld_midAlignSize.x, vld_midAlignSize.y );
		SetTexturePos( VertexArray, vld_midTexPos.x, vld_midTexPos.y, vld_midTexRecSize.x, vld_midTexRecSize.y, vld_midTexSize.x, vld_midTexSize.y );

		if ( FAILED ( CreateVB ( pd3dDevice, pTextureVB, VertexArray ) ) )	
			return ErrorCreateVB();
	}

	LPDIRECT3DVERTEXBUFFERQ pldTopVB	= NULL; 
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_topAlignPos.x, ROOT_TOP + vld_topAlignPos.y, vld_topAlignSize.x, vld_topAlignSize.y );
		SetTexturePos( VertexArray, vld_topTexPos.x, vld_topTexPos.y, vld_topTexRecSize.x, vld_topTexRecSize.y, vld_topTexSize.x, vld_topTexSize.y );

		if ( FAILED ( CreateVB ( pd3dDevice, pldTopVB, VertexArray ) ) )	
			return ErrorCreateVB();
	}

	LPDIRECT3DVERTEXBUFFERQ pldUnderVB	= NULL; 
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_underAlignPos.x, ROOT_TOP + vld_underAlignPos.y, vld_underAlignSize.x, vld_underAlignSize.y );
		SetTexturePos( VertexArray, vld_underTexPos.x, vld_underTexPos.y, vld_underTexRecSize.x, vld_underTexRecSize.y, vld_underTexSize.x, vld_underTexSize.y );

		if ( FAILED ( CreateVB ( pd3dDevice, pldUnderVB, VertexArray ) ) )	
			return ErrorCreateVB();
	}

	LPDIRECT3DVERTEXBUFFERQ pldbackVB	= NULL; 
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_backRenderPos.x, ROOT_TOP + vld_backRenderPos.y, vld_backRenderSize.x, vld_backRenderSize.y );
		SetTexturePos( VertexArray, vld_backTexPos.x, vld_backTexPos.y, vld_backTexRecSize.x, vld_backTexRecSize.y, vld_backTexSize.x, vld_backTexSize.y );

		if ( FAILED ( CreateVB ( pd3dDevice, pldbackVB, VertexArray ) ) )	
			return ErrorCreateVB();
	}

	LPDIRECT3DVERTEXBUFFERQ pldHintIconVB	= NULL; 
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_HintIconAlignPos.x, ROOT_TOP + vld_HintIconAlignPos.y, vld_HintIconRenderSize.x, vld_HintIconRenderSize.y );
		SetTexturePos( VertexArray, vld_HintIconTexPos.x, vld_HintIconTexPos.y, vld_HintIconTexRecSize.x, vld_HintIconTexRecSize.y, vld_HintIconTexSize.x, vld_HintIconTexSize.y );

		if ( FAILED ( CreateVB ( pd3dDevice, pldHintIconVB, VertexArray ) ) )	
			return ErrorCreateVB();
	}

	LPDIRECT3DVERTEXBUFFERQ pldMapNameBackVB	= NULL; 
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vld_MapNameBackRenderPos.x, ROOT_TOP + vld_MapNameBackRenderPos.y, vld_MapNameBackRenderSize.x, vld_MapNameBackRenderSize.y );
		SetTexturePos( VertexArray, vld_MapNameBackTexPos.x, vld_MapNameBackTexPos.y, vld_MapNameBackTexRecSize.x, vld_MapNameBackTexRecSize.y, vld_MapNameBackTexSize.x, vld_MapNameBackTexSize.y );

		if ( FAILED ( CreateVB ( pd3dDevice, pldMapNameBackVB, VertexArray ) ) )	
			return ErrorCreateVB();
	}

	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	if( pFont )
		pFont->UsageCS( TRUE );

	CD3DFontPar* pFont12 = DxFontMan::GetInstance().LoadDxFont( _BOLD_FONT, 13, _DEFAULT_FONT_SHADOW_FLAG );
	if( pFont12 )
		pFont12->UsageCS( TRUE );

	int	ldstep = 0;
	int ldstepmulti = 1;
	int nTipIndex = 0;
	
	if ( n_bTIP )
	{
		if( NLOADINGTIP::GetTipSize() > 0 )
		{
			srand ( (UINT)time( NULL ) );
			nTipIndex = rand () % NLOADINGTIP::GetTipSize();
		}
	}

	// Frame rate control variables
	DWORD dwLastFrameTime = GetTickCount();
	DWORD dwFrameCount = 0;
	
	// Loading progress variables
	DWORD dwLoadingStartTime = GetTickCount();
	DWORD dwTotalTime = 300; // 0.3 seconds total loading time - INSTANT LOADING!
	float fCurrentProgress = 0.0f;
	float fSmoothProgress = 0.0f;
	const int nMAX_STEP = 100; // 100-step progress bar for smooth animation

	while ( n_bRender )
	{
		// Frame rate control - optimized for fast loading (120 FPS)
		FrameRateControl(dwLastFrameTime, 120);
		
		// Process Windows messages to prevent "not responding"
		if (!MessagePump())
			break;

		// Calculate real-time progress (0.0 to 1.0) - ULTRA FAST LOADING
		fCurrentProgress = CalculateAdaptiveProgress(dwLoadingStartTime, dwTotalTime);
		
		// Smooth progress animation for better visual feedback
		fSmoothProgress += (fCurrentProgress - fSmoothProgress) * 0.1f;
		
		// Check if loading is complete (progress >= 1.0)
		if (fCurrentProgress >= 1.0f)
		{
			// Ensure smooth progress reaches 100% before exiting
			while (fSmoothProgress < 0.99f)
			{
				fSmoothProgress += (1.0f - fSmoothProgress) * 0.1f;
				
				// Continue rendering to show the 100% progress
				if( pLoadingStepSprite == NULL )
				{
					if ( FAILED (D3DXCreateSprite(pd3dDevice, &pLoadingStepSprite)))
						MessageBoxA( NULL, "Cannot Create Sprite", 0, 0 );
				}

				HRESULT hr;

				if( FAILED( hr = pd3dDevice->TestCooperativeLevel() ) )
				{
					if( D3DERR_DEVICELOST == hr )
					{
						CDebugSet::ToListView ( "[ERROR] D3DERR_DEVICELOST _ LoadingThread() FAILED" );
						continue;
					}

					if( D3DERR_DEVICENOTRESET == hr )
						continue;
				}

				hr = pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L );

				if( SUCCEEDED( pd3dDevice->BeginScene() ) )
				{
					hr = Render ( pd3dDevice, pLoadingTopTex, pldTopVB );
					hr = Render ( pd3dDevice, pLoadingTexture, pTextureVB );
					hr = Render ( pd3dDevice, pLoadingUnderTex, pldUnderVB );

					DWORD dwAlphaBlendEnable;
					pd3dDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable );

					pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
					pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

					DWORD dwAlphaOP;
					pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwAlphaOP );
					pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

					DWORD dwMin, dwMag, dwMip;
					pd3dDevice->GetSamplerState( 0, D3DSAMP_MINFILTER,	&dwMin );
					pd3dDevice->GetSamplerState( 0, D3DSAMP_MAGFILTER,	&dwMag );
					pd3dDevice->GetSamplerState( 0, D3DSAMP_MIPFILTER,	&dwMip );

					pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	D3DTEXF_POINT );
					pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	D3DTEXF_POINT );
					pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	D3DTEXF_NONE );

					hr = Render ( pd3dDevice, pLoadingBackTex, pldbackVB );

					// Show 100% progress (step 15 = last sprite)
					RECT srcRectLoadingSprite;
					srcRectLoadingSprite.top    = (( 15 / 4 ) * 105);
					srcRectLoadingSprite.left   = (( 15 % 4 ) * 105);
					srcRectLoadingSprite.bottom = (( 15 / 4 ) * 105) + 105;
					srcRectLoadingSprite.right  = (( 15 % 4 ) * 105) + 105;

					D3DXVECTOR3 vPosition( vld_backRenderPos.x + 12.0f, vld_backRenderPos.y + 12.0f, 0.0f );
					pLoadingStepSprite->Begin( D3DXSPRITE_ALPHABLEND );
					pLoadingStepSprite->Draw( pLoadingStepTex, &srcRectLoadingSprite, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &vPosition, D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f) );

					if ( n_bTIP )
					{
						hr = Render ( pd3dDevice, pHintIconTex, pldHintIconVB );

						pFont->DrawText( vld_HintIconAlignPos.x+85.0f, vld_HintIconAlignPos.y+20.0f, NS_UITEXTCOLOR::WHITE, NLOADINGTIP::GetTip( nTipIndex ) );	
						pFont->DrawText( vld_HintIconAlignPos.x+85.0f, vld_HintIconAlignPos.y+35.0f, NS_UITEXTCOLOR::WHITE, NLOADINGTIP::GetTip( nTipIndex+1 ) );
					}
					
					if ( !n_strMapName.IsEmpty() )
					{
						hr = Render ( pd3dDevice, pMapNameBackTex, pldMapNameBackVB );

						CString strMapName("");
						strMapName.Format("< %s >", n_strMapName.GetString() );
						if( pFont12 )
						{
							SIZE textsizeMapName;
							HRESULT hrGetExtent = pFont12->GetTextExtent ( strMapName.GetString(), textsizeMapName );
							if ( hrGetExtent == S_OK )
							{
								float fCenterScreenX = ( ROOT_SIZEX / 2.f ) - float( textsizeMapName.cx / 2 );
								pFont12->DrawText( fCenterScreenX, vld_MapNameBackRenderPos.y+20.0f, NS_UITEXTCOLOR::WHITE, strMapName.GetString() );
							}
						}
					}

					if( pFont )
						pFont->DrawText( vCopyrightAlign.x, vCopyrightAlign.y, NS_UITEXTCOLOR::WHITE, ID2GAMEWORD("COPYRIGHT_COMPANY_LOAD") );

					pLoadingStepSprite->End();

					pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwAlphaOP );
					pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable );

					pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	dwMin );
					pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	dwMag );
					pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	dwMip );

					pd3dDevice->EndScene();
					pd3dDevice->Present( NULL, NULL, NULL, NULL );
				}
				
				// Small delay to show the 100% progress
				Sleep(10);
				
				// Force immediate cleanup during 100% display to prevent delays
				NRESOURCE_CLEANUP::ForceImmediateCleanup();
			}
			
			// Ensure final 100% display
			fSmoothProgress = 1.0f;
			// Loading complete - exit the loop
			break;
		}

		if( pLoadingStepSprite == NULL )
		{
			if ( FAILED (D3DXCreateSprite(pd3dDevice, &pLoadingStepSprite)))
				MessageBoxA( NULL, "Cannot Create Sprite", 0, 0 );
		}

		HRESULT hr;

		if( FAILED( hr = pd3dDevice->TestCooperativeLevel() ) )
		{
			if( D3DERR_DEVICELOST == hr )
			{
				CDebugSet::ToListView ( "[ERROR] D3DERR_DEVICELOST _ LoadingThread() FAILED" );
				continue;
			}

			if( D3DERR_DEVICENOTRESET == hr )
				continue;
		}

		hr = pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L );

		if( SUCCEEDED( pd3dDevice->BeginScene() ) )
		{
			hr = Render ( pd3dDevice, pLoadingTopTex, pldTopVB );
			hr = Render ( pd3dDevice, pLoadingTexture, pTextureVB );
			hr = Render ( pd3dDevice, pLoadingUnderTex, pldUnderVB );

			DWORD dwAlphaBlendEnable;
			pd3dDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable );

			pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
			pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

			DWORD dwAlphaOP;
			pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwAlphaOP );
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

			DWORD dwMin, dwMag, dwMip;
			pd3dDevice->GetSamplerState( 0, D3DSAMP_MINFILTER,	&dwMin );
			pd3dDevice->GetSamplerState( 0, D3DSAMP_MAGFILTER,	&dwMag );
			pd3dDevice->GetSamplerState( 0, D3DSAMP_MIPFILTER,	&dwMip );

			pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	D3DTEXF_POINT );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	D3DTEXF_POINT );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	D3DTEXF_NONE );

			hr = Render ( pd3dDevice, pLoadingBackTex, pldbackVB );

			// Calculate progress bar position based on real-time progress
			// Map progress to 16 sprite positions (4x4 grid) - 0 to 15
			int nProgressStep = (int)(fSmoothProgress * 15.0f); // 15.0f to get range 0-15
			nProgressStep = max(0, min(nProgressStep, 15)); // Clamp to valid sprite range (0-15)
			
			RECT srcRectLoadingSprite;
			srcRectLoadingSprite.top    = (( nProgressStep / 4 ) * 105);
			srcRectLoadingSprite.left   = (( nProgressStep % 4 ) * 105);
			srcRectLoadingSprite.bottom = (( nProgressStep / 4 ) * 105) + 105;
			srcRectLoadingSprite.right  = (( nProgressStep % 4 ) * 105) + 105;

			D3DXVECTOR3 vPosition( vld_backRenderPos.x + 12.0f, vld_backRenderPos.y + 12.0f, 0.0f );
			pLoadingStepSprite->Begin( D3DXSPRITE_ALPHABLEND );
			pLoadingStepSprite->Draw( pLoadingStepTex, &srcRectLoadingSprite, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &vPosition, D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f) );

			if ( n_bTIP )
			{
				hr = Render ( pd3dDevice, pHintIconTex, pldHintIconVB );

				pFont->DrawText( vld_HintIconAlignPos.x+85.0f, vld_HintIconAlignPos.y+20.0f, NS_UITEXTCOLOR::WHITE, NLOADINGTIP::GetTip( nTipIndex ) );	
				pFont->DrawText( vld_HintIconAlignPos.x+85.0f, vld_HintIconAlignPos.y+35.0f, NS_UITEXTCOLOR::WHITE, NLOADINGTIP::GetTip( nTipIndex+1 ) );
			}
			
			if ( !n_strMapName.IsEmpty() )
			{
				hr = Render ( pd3dDevice, pMapNameBackTex, pldMapNameBackVB );

				CString strMapName("");
				strMapName.Format("< %s >", n_strMapName.GetString() );
				if( pFont12 )
				{
					SIZE textsizeMapName;
					HRESULT hrGetExtent = pFont12->GetTextExtent ( strMapName.GetString(), textsizeMapName );
					if ( hrGetExtent == S_OK )
					{
						float fCenterScreenX = ( ROOT_SIZEX / 2.1f ) - float( textsizeMapName.cx / 2 );
						pFont12->DrawText( fCenterScreenX, vld_MapNameBackRenderPos.y+22.0f, NS_UITEXTCOLOR::WHITE, strMapName.GetString() );
					}
				}
			}

			if( pFont )
				pFont->DrawText( vCopyrightAlign.x, vCopyrightAlign.y, NS_UITEXTCOLOR::WHITE, ID2GAMEWORD("COPYRIGHT_COMPANY_LOAD") );

			pLoadingStepSprite->End();

			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwAlphaOP );
			pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable );

			pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	dwMin );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	dwMag );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	dwMip );

			pd3dDevice->EndScene();
			pd3dDevice->Present( NULL, NULL, NULL, NULL );
		}

		// Real-time progress is calculated above - no need for manual step increment
		// The progress bar now updates based on actual elapsed time
	}

	if( pFont )
		pFont->UsageCS( FALSE );

	if( pFont12 )
		pFont12->UsageCS( FALSE );

	// Enable immediate cleanup for critical loading completion
	NRESOURCE_CLEANUP::EnableImmediateCleanup(TRUE);
	
	// Queue texture releases for background cleanup (non-blocking)
	NRESOURCE_CLEANUP::QueueTextureRelease(pTextureVB);
	NRESOURCE_CLEANUP::QueueTextureRelease(pldTopVB);
	NRESOURCE_CLEANUP::QueueTextureRelease(pldUnderVB);
	NRESOURCE_CLEANUP::QueueTextureRelease(pldbackVB);
	NRESOURCE_CLEANUP::QueueTextureRelease(pldHintIconVB);
	NRESOURCE_CLEANUP::QueueTextureRelease(pldMapNameBackVB);
	NRESOURCE_CLEANUP::QueueTextureRelease(pLoadingStepSprite);

	// Queue texture name releases for background cleanup (non-blocking)
	NRESOURCE_CLEANUP::QueueTextureNameRelease(szTexture);
	NRESOURCE_CLEANUP::QueueTextureNameRelease(m_strLoadingTopTex.c_str());
	NRESOURCE_CLEANUP::QueueTextureNameRelease(m_strLoadingUnderTex.c_str());
	NRESOURCE_CLEANUP::QueueTextureNameRelease(m_strLoadingStepTex.c_str());
	NRESOURCE_CLEANUP::QueueTextureNameRelease(m_strLoadingBackTex.c_str());
	NRESOURCE_CLEANUP::QueueTextureNameRelease(m_strHintIconTex.c_str());
	NRESOURCE_CLEANUP::QueueTextureNameRelease(m_strMapNameBackTex.c_str());
	
	// Force immediate cleanup to prevent delays
	NRESOURCE_CLEANUP::ForceImmediateCleanup();

	DeWait ();

	n_ExitState = eNORMAL;

	return 0;
}


unsigned int WINAPI	LoadingThread_Classic( LPVOID pData )
{
	if( n_strTextureName.IsEmpty() || n_strTextureName == _T("null") )
	{
		srand ( (UINT)time( NULL ) );
		int nIndex = (rand() % 2) + 11;

		n_strTextureName.Format( "loading_%03d.tga", nIndex );	
	}

	TCHAR szTexture[256] = {0};
	StringCchPrintf ( szTexture, 256, n_strTextureName.GetString(), n_szAppPath, SUBPATH::TEXTURE_FILE_ROOT );

	LPDIRECT3DDEVICEQ& pd3dDevice = *n_ppd3dDevice;
	LPDIRECT3DTEXTUREQ pLoadingTexture = NULL;
	LPDIRECT3DTEXTUREQ pCopyRightTex = NULL;

	HRESULT hr = S_OK;
	hr = TextureManager::LoadTexture( szTexture, pd3dDevice, pLoadingTexture, 0, 0 );
	if ( FAILED ( hr ) )
	{		
		DeWait ();
		n_ExitState = eERROR;
		return 0;
	}

	DeWait ();

	RECT rect;
	GetClientRect ( n_hWnd, &rect );

	const float fWidth = float(rect.right - rect.left);
	const float fHeight = float(rect.bottom - rect.top);

	const float	fRealImageX = 1024.0f;
	const float	fRealImageY = 768.0f;
	const float	fImageSize = 1023.0f;

	const float fWidthRatio = fRealImageX / fImageSize;
	const float fHeightRatio= fRealImageY / fImageSize;

	const float LeftPos  = 0.0f;
	const float TopPos   = 0.0f;
	const float RightPos = fWidth;
	const float BottomPos= fHeight;

	float ROOT_LEFT		= LeftPos;
	float ROOT_TOP		= TopPos;
	float ROOT_SIZEX	= fWidth;
	float ROOT_SIZEY	= fHeight;


	const D3DXVECTOR2 vProgressBarTex ( 0, 769 );
	const D3DXVECTOR2 vProgressBarBackTex ( 0, 791 );
	const D3DXVECTOR2 vProgressBarSize ( 582, 9 );
	const D3DXVECTOR2 TextureRenderPos ( 215, 584 );	


	const D3DXVECTOR2 vOver15 ( 940, 20 );
	const D3DXVECTOR2 vOver15Size ( 64, 64 );
	const D3DXVECTOR2 vOver15Tex ( 0, 0 );	
	const D3DXVECTOR2 vOver15_800 ( 716, 20 );

	D3DXVECTOR2 vProgressBarAlignSize;
	vProgressBarAlignSize.x = static_cast<float>(floor(vProgressBarSize.x * fWidth / fRealImageX));
	vProgressBarAlignSize.y = static_cast<float>(floor(vProgressBarSize.y * fHeight/ fRealImageX));

	D3DXVECTOR2 vProgressBarAlign;
	vProgressBarAlign.x = (fWidth - vProgressBarAlignSize.x)/2.0f;
	vProgressBarAlign.y = static_cast<float>(floor(TextureRenderPos.y * fHeight/ fRealImageY));

	D3DXVECTOR2 vCopyrightAlign;
	vCopyrightAlign.x = 15.0f;
	vCopyrightAlign.y = fHeight - 30.0f;

	D3DXVECTOR2 vOver15Align;
	vOver15Align.x = fWidth - (vOver15Size.x+15.0f);
	vOver15Align.y = 15.0f;

	LPDIRECT3DVERTEXBUFFERQ pTextureVB        = NULL; // Buffer to hold vertices
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT, ROOT_TOP, ROOT_SIZEX, ROOT_SIZEY );
		SetTexturePos( VertexArray, 0.0f, 0.0f, fRealImageX, fRealImageY, fImageSize, fImageSize );

		if ( FAILED ( CreateVB ( pd3dDevice, pTextureVB, VertexArray ) ) )
		{
			n_ExitState = eERROR;
			return 0;
		}
	}

	LPDIRECT3DVERTEXBUFFERQ pProgressBackVB;
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vProgressBarAlign.x, ROOT_TOP + vProgressBarAlign.y, vProgressBarAlignSize.x, vProgressBarSize.y );
		SetTexturePos( VertexArray, vProgressBarBackTex.x, vProgressBarBackTex.y, vProgressBarSize.x, vProgressBarSize.y, fImageSize, fImageSize );

		if ( FAILED ( CreateVB ( pd3dDevice, pProgressBackVB, VertexArray ) ) )
		{
			n_ExitState = eERROR;
			return 0;
		}
	}

	LPDIRECT3DVERTEXBUFFERQ pProgressBarVB;
	{
		TEXTUREVERTEX VertexArray[6];
		SetVertexPos ( VertexArray, ROOT_LEFT + vProgressBarAlign.x, ROOT_TOP + vProgressBarAlign.y, vProgressBarAlignSize.x, vProgressBarSize.y );
		SetTexturePos( VertexArray, vProgressBarTex.x, vProgressBarTex.y, vProgressBarSize.x, vProgressBarSize.y, fImageSize, fImageSize );

		if ( FAILED ( CreateVB ( pd3dDevice, pProgressBarVB, VertexArray ) ) )
		{
			n_ExitState = eERROR;
			return 0;
		}
	}

	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	if( pFont )
		pFont->UsageCS( TRUE );

	// Frame rate control variables for classic thread
	DWORD dwLastFrameTimeClassic = GetTickCount();
	
	// Real-time progress tracking
	DWORD dwLoadingStartTime = GetTickCount();
	float fLastProgress = 0.0f;
	float fSmoothProgress = 0.0f;

	while ( n_bRender )
	{
		// Frame rate control - optimized for fast loading (120 FPS)
		FrameRateControl(dwLastFrameTimeClassic, 120);
		
		// Process Windows messages to prevent "not responding"
		if (!MessagePump())
			break;

		//	UPDATE - Real-time progress calculation
		{
			{
				// Calculate real-time progress (0.0 to 1.0) - ULTRA FAST LOADING
				float fRealProgress = CalculateAdaptiveProgress(dwLoadingStartTime, CalculateAdaptiveLoadingTime()); // 1 second total - ULTRA FAST!
				
				// Smooth progress animation
				float fTargetProgress = fRealProgress;
				float fSmoothFactor = 0.1f; // Adjust for smoother/faster animation
				fLastProgress += (fTargetProgress - fLastProgress) * fSmoothFactor;
				
				// Convert to 100-step progress for smoother animation
				const int nMAX_STEP = 100; // 100 steps for smoother progress
				int pri_Step = static_cast<int>(fLastProgress * nMAX_STEP);
				if (pri_Step > nMAX_STEP) pri_Step = nMAX_STEP;

				float fPercent = static_cast<float>(pri_Step) / static_cast<float>(nMAX_STEP);
				float fTEX_SIZEX = (vProgressBarTex.x + vProgressBarSize.x) * fPercent / fImageSize;
				float fSIZEX = static_cast<float>(ceil((vProgressBarAlign.x + vProgressBarAlignSize.x) * fPercent));

				// Check if loading is complete (progress >= 1.0)
				if (fRealProgress >= 1.0f)
				{
					// Ensure progress bar reaches 100% before exiting
					while (fLastProgress < 0.99f)
					{
						fLastProgress += (1.0f - fLastProgress) * 0.1f;
						
						// Update progress bar to show 100%
						pri_Step = nMAX_STEP; // Force to 100%
						fPercent = 1.0f;
						fTEX_SIZEX = (vProgressBarTex.x + vProgressBarSize.x) * fPercent / fImageSize;
						fSIZEX = static_cast<float>(ceil((vProgressBarAlign.x + vProgressBarAlignSize.x) * fPercent));
						
						// Update vertex buffer to show 100% progress
						{
							VOID* pVertices;
							if( FAILED( pProgressBarVB->Lock( 0, sizeof ( TEXTUREVERTEX ) * 6, (VOID**)&pVertices, 0 ) ) )
							{
								n_ExitState = eERROR;
								if( pFont )
									pFont->UsageCS( FALSE );
								return 0;
							}
							TEXTUREVERTEX* pVerticesPart = (TEXTUREVERTEX*)pVertices;
							pVerticesPart[1].x = fSIZEX;
							pVerticesPart[1].tu = fTEX_SIZEX;
							pVerticesPart[4].x = fSIZEX;
							pVerticesPart[4].tu = fTEX_SIZEX;
							pVerticesPart[5].x = fSIZEX;
							pVerticesPart[5].tu = fTEX_SIZEX;

							pProgressBarVB->Unlock();
						}
						
						// Continue rendering to show the 100% progress
						HRESULT hr;
						if( FAILED( hr = pd3dDevice->TestCooperativeLevel() ) )
						{
							if( D3DERR_DEVICELOST == hr )
							{
								CDebugSet::ToListView ( "[ERROR] D3DERR_DEVICELOST _ LoadingThread() FAILED" );
								continue;
							}

							if( D3DERR_DEVICENOTRESET == hr )
								continue;
						}

						hr = pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L );

						if( SUCCEEDED( pd3dDevice->BeginScene() ) )
						{
							hr = Render ( pd3dDevice, pLoadingTexture, pTextureVB );

							DWORD dwAlphaBlendEnable;
							pd3dDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable );

							pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
							pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
							pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

							DWORD dwAlphaOP;
							pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwAlphaOP );
							pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

							DWORD dwMin, dwMag, dwMip;
							pd3dDevice->GetSamplerState( 0, D3DSAMP_MINFILTER,	&dwMin );
							pd3dDevice->GetSamplerState( 0, D3DSAMP_MAGFILTER,	&dwMag );
							pd3dDevice->GetSamplerState( 0, D3DSAMP_MIPFILTER,	&dwMip );

							pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	D3DTEXF_POINT );
							pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	D3DTEXF_POINT );
							pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	D3DTEXF_NONE );

							hr = Render ( pd3dDevice, pLoadingTexture, pProgressBackVB );

							if( pFont )
								pFont->DrawText( vCopyrightAlign.x, vCopyrightAlign.y, NS_UITEXTCOLOR::WHITE, ID2GAMEWORD("COPYRIGHT_COMPANY_LOAD") );

							hr = Render ( pd3dDevice, pLoadingTexture, pProgressBarVB );

							pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwAlphaOP );
							pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable );

							pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	dwMin );
							pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	dwMag );
							pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	dwMip );

							pd3dDevice->EndScene();
							pd3dDevice->Present( NULL, NULL, NULL, NULL );
						}
						
						// Small delay to show the 100% progress
						Sleep(10);
						
						// Force immediate cleanup during 100% display to prevent delays
						NRESOURCE_CLEANUP::ForceImmediateCleanup();
					}
					
					// Ensure final 100% display
					fLastProgress = 1.0f;
					// Loading complete - exit the loop
					break;
				}

				{
					VOID* pVertices;
					if( FAILED( pProgressBarVB->Lock( 0, sizeof ( TEXTUREVERTEX ) * 6, (VOID**)&pVertices, 0 ) ) )
					{
						n_ExitState = eERROR;
						if( pFont )
							pFont->UsageCS( FALSE );
						return 0;
					}
					TEXTUREVERTEX* pVerticesPart = (TEXTUREVERTEX*)pVertices;
					pVerticesPart[1].x = fSIZEX;
					pVerticesPart[1].tu = fTEX_SIZEX;
					pVerticesPart[4].x = fSIZEX;
					pVerticesPart[4].tu = fTEX_SIZEX;
					pVerticesPart[5].x = fSIZEX;
					pVerticesPart[5].tu = fTEX_SIZEX;

					pProgressBarVB->Unlock();
				}
			}
		}

		HRESULT hr;
		if( FAILED( hr = pd3dDevice->TestCooperativeLevel() ) )
		{
			// If the device was lost, do not render until we get it back
			if( D3DERR_DEVICELOST == hr )
			{
				CDebugSet::ToListView ( "[ERROR] D3DERR_DEVICELOST _ LoadingThread() FAILED" );
				continue;
			}

			// Check if the device needs to be resized.
			if( D3DERR_DEVICENOTRESET == hr )
				continue;
		}

		hr = pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L );

		// Begin the scene
		if( SUCCEEDED( pd3dDevice->BeginScene() ) )
		{
			hr = Render ( pd3dDevice, pLoadingTexture, pTextureVB );

			DWORD dwAlphaBlendEnable;
			pd3dDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable );

			pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
			pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

			DWORD dwAlphaOP;
			pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP,   &dwAlphaOP );
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

			DWORD dwMin, dwMag, dwMip;
			pd3dDevice->GetSamplerState( 0, D3DSAMP_MINFILTER,	&dwMin );
			pd3dDevice->GetSamplerState( 0, D3DSAMP_MAGFILTER,	&dwMag );
			pd3dDevice->GetSamplerState( 0, D3DSAMP_MIPFILTER,	&dwMip );

			pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	D3DTEXF_POINT );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	D3DTEXF_POINT );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	D3DTEXF_NONE );

			hr = Render ( pd3dDevice, pLoadingTexture, pProgressBackVB );



			if( pFont )
				pFont->DrawText( vCopyrightAlign.x, vCopyrightAlign.y, NS_UITEXTCOLOR::WHITE, ID2GAMEWORD("COPYRIGHT_COMPANY_LOAD") );

			hr = Render ( pd3dDevice, pLoadingTexture, pProgressBarVB );


			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   dwAlphaOP );
			pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable );

			pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER,	dwMin );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER,	dwMag );
			pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER,	dwMip );

			// End the scene.
			pd3dDevice->EndScene();
			pd3dDevice->Present( NULL, NULL, NULL, NULL );
		}

		// No sleep needed - frame rate control already handles timing
		// This prevents any freezing while maintaining smooth 60 FPS
	}

	if( pFont )
		pFont->UsageCS( FALSE );

	// Enable immediate cleanup for critical loading completion
	NRESOURCE_CLEANUP::EnableImmediateCleanup(TRUE);
	
	// Queue texture releases for background cleanup (non-blocking)
	NRESOURCE_CLEANUP::QueueTextureRelease(pTextureVB);
	NRESOURCE_CLEANUP::QueueTextureRelease(pProgressBackVB);
	NRESOURCE_CLEANUP::QueueTextureRelease(pProgressBarVB);

	// Queue texture name releases for background cleanup (non-blocking)
	NRESOURCE_CLEANUP::QueueTextureNameRelease(szTexture);
	
	// Force immediate cleanup to prevent delays
	NRESOURCE_CLEANUP::ForceImmediateCleanup();

	DeWait ();
	n_ExitState = eNORMAL;

	return 0;
}


namespace	NLOADINGTIP
{
	LOADING_TIP_VEC		n_vecTip;

	int GetTipSize()
	{
		return (int)n_vecTip.size();
	}

	void InsertTip( CString strText )
	{
		if ( strText.GetLength() <= 0 )	return;
		n_vecTip.push_back( strText );
	}

	CString GetTip( int nIndex )
	{
		if ( n_vecTip.empty() )	return "";
		if ( nIndex < 0 )		return "";
		if ( nIndex >= (int)n_vecTip.size() )	return "";
		return n_vecTip[nIndex];
	};

	void Clear()
	{
		n_vecTip.clear();
	}
};

namespace	NLOADINGTHREAD
{
	DWORD				n_dwThreadID;
	LPDIRECT3DDEVICEQ*	n_ppd3dDevice;
	HWND				n_hWnd;
	BOOL				n_bWait;
	BOOL				n_bRender;
	char				n_szAppPath[MAX_PATH] = {0};
	int					n_ExitState;
	int					n_Step = 0;
	HANDLE				n_hThread = NULL;
	CString				n_strTextureName = "";
	CString				n_strMapName = "";
	BOOL				n_bTIP = FALSE;
	
	// Add proper synchronization objects
	HANDLE				n_hWaitEvent = NULL;
	HANDLE				n_hRenderEvent = NULL;
	CRITICAL_SECTION	n_csThreadSync;

	HRESULT	StartThreadLOAD(LPDIRECT3DDEVICEQ* ppd3dDevice, 
							HWND hWnd, 
							const char* szAppPath, 
							const CString & strTextureName, 
							const CString & strMapName,
							BOOL bTIP )
	{
		// Initialize synchronization objects
		InitializeCriticalSection(&n_csThreadSync);
		n_hWaitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		n_hRenderEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
		
		if (!n_hWaitEvent || !n_hRenderEvent)
		{
			DeleteCriticalSection(&n_csThreadSync);
			return E_FAIL;
		}
		
		// Initialize background resource cleanup system
		if (FAILED(NRESOURCE_CLEANUP::InitializeBackgroundCleanup()))
		{
			DeleteCriticalSection(&n_csThreadSync);
			return E_FAIL;
		}

		n_ppd3dDevice = ppd3dDevice;
		n_hWnd = hWnd;
		StringCchCopy ( n_szAppPath, MAX_PATH, szAppPath );
		n_strTextureName = strTextureName;
		n_strMapName = strMapName;
		n_bTIP = bTIP;
		
		n_bRender = TRUE;
		EnWait ();
		n_ExitState = eNORMAL;

#if defined( BUILD_EP6 ) || defined( BUILD_EP4 )
		n_hThread = (HANDLE) ::_beginthreadex(
			NULL,
			0,
			LoadingThread_Classic,
			NULL,
			0,
			(unsigned int*) &n_dwThreadID );
#else
		n_hThread = (HANDLE) ::_beginthreadex(
			NULL,
			0,
			LoadingThread,
			NULL,
			0,
			(unsigned int*) &n_dwThreadID );
#endif 
		
		if ( !n_hThread )
		{
			DeWait();
			return E_FAIL;
		}

		return S_OK;
	}

	void	WaitThread ()
	{
		// Use event-based waiting instead of busy waiting
		if (n_hWaitEvent)
		{
			WaitForSingleObject(n_hWaitEvent, INFINITE);
			ResetEvent(n_hWaitEvent);
		}
		EnWait ();
	}

	void	EndThread ()
	{		
		EnterCriticalSection(&n_csThreadSync);
		n_bRender = FALSE;
		LeaveCriticalSection(&n_csThreadSync);

		if ( n_ExitState == eNORMAL )
		{
			WaitThread ();
		}

		CloseHandle( n_hThread );
		n_hThread = NULL;

		// Clean up synchronization objects
		if (n_hWaitEvent)
		{
			CloseHandle(n_hWaitEvent);
			n_hWaitEvent = NULL;
		}
		if (n_hRenderEvent)
		{
			CloseHandle(n_hRenderEvent);
			n_hRenderEvent = NULL;
		}
		DeleteCriticalSection(&n_csThreadSync);
		
			// Force final cleanup before shutdown
	NRESOURCE_CLEANUP::ForceImmediateCleanup();
	
	// Shutdown background resource cleanup system
	NRESOURCE_CLEANUP::ShutdownBackgroundCleanup();

	n_strTextureName.Empty();
	}

	BOOL	GetWait ()
	{
		BOOL bResult;
		EnterCriticalSection(&n_csThreadSync);
		bResult = n_bWait;
		LeaveCriticalSection(&n_csThreadSync);
		return bResult;
	}

	void	DeWait  ()
	{
		EnterCriticalSection(&n_csThreadSync);
		n_bWait = FALSE;
		LeaveCriticalSection(&n_csThreadSync);
		
		if (n_hWaitEvent)
			SetEvent(n_hWaitEvent);
	}

	void	EnWait ()
	{
		EnterCriticalSection(&n_csThreadSync);
		n_bWait = TRUE;
		LeaveCriticalSection(&n_csThreadSync);
		
		if (n_hWaitEvent)
			ResetEvent(n_hWaitEvent);
	}

	void	SetStep ( int step )
	{
		EnterCriticalSection(&n_csThreadSync);
		n_Step = step;
		LeaveCriticalSection(&n_csThreadSync);
	}

	int		GetStep ()
	{
		int nResult;
		EnterCriticalSection(&n_csThreadSync);
		nResult = n_Step;
		LeaveCriticalSection(&n_csThreadSync);
		return nResult;
	}

	int		ErrorLoadingTexture()
	{
		DeWait ();
		n_ExitState = eERROR;
		return 0;
	}

	int		ErrorCreateVB()
	{
		n_ExitState = eERROR;
		return 0;
	}
};