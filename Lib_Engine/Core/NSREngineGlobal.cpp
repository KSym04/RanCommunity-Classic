#include "stdafx.h"
#include "./NSREngineGlobal.h"
#include "./NSROption.h"
#include "./NSROptionDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace RENGINE_GLOBAL
{
	void OneTimeSceneInit()
	{
		SetTextureFilters();
	}

	void InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice )
	{
	}

	void RestoreDeviceObjects()
	{
	}

	void InvalidateDeviceObjects()
	{
	}

	void DeleteDeviceObjects()
	{
	}

	void FinalCleanup()
	{
	}

	void FrameMove( float fTime, float fElapsedTime )
	{
	}

	void Render ( LPDIRECT3DDEVICEQ pd3dDevice )
	{
	}

	void DoInterimClean ( LPDIRECT3DDEVICEQ pd3dDevice )
	{
	}

	bool	bDISP_CONSOLE		= false;
	bool	bDISP_FPS			= false;

	DWORD dwSamplerMinFilter = D3DTEXF_LINEAR;
	DWORD dwSamplerMagFilter = D3DTEXF_LINEAR;
	DWORD dwSamplerMaxAnisotropy = 0;

	/* camera zoom gm command, Juver, 2020/06/10 */
	float fCameraZoomMultiplier = 1.0f;

	void SetTextureFilters()
	{
		ROPTION::EMTEXTUREFILTER emFILTER = (ROPTION::EMTEXTUREFILTER)ROPTION::dwTextureFilter;

		switch ( emFILTER )
		{
		case ROPTION::EMTEXTUREFILTER_ANISOTROPIC_2:		
			{
				dwSamplerMinFilter = D3DTEXF_ANISOTROPIC;
				dwSamplerMagFilter = D3DTEXF_ANISOTROPIC;
				dwSamplerMaxAnisotropy = 2;	
			}break;

		case ROPTION::EMTEXTUREFILTER_ANISOTROPIC_4:	
			{
				dwSamplerMinFilter = D3DTEXF_ANISOTROPIC;
				dwSamplerMagFilter = D3DTEXF_ANISOTROPIC;
				dwSamplerMaxAnisotropy = 4;	
			}break;

		case ROPTION::EMTEXTUREFILTER_ANISOTROPIC_8:	
			{
				dwSamplerMinFilter = D3DTEXF_ANISOTROPIC;
				dwSamplerMagFilter = D3DTEXF_ANISOTROPIC;
				dwSamplerMaxAnisotropy = 8;	
			}break;

		case ROPTION::EMTEXTUREFILTER_ANISOTROPIC_16:
			{
				dwSamplerMinFilter = D3DTEXF_ANISOTROPIC;
				dwSamplerMagFilter = D3DTEXF_ANISOTROPIC;
				dwSamplerMaxAnisotropy = 16;
			}break;

		case ROPTION::EMTEXTUREFILTER_TRILINEAR:
		default:
			{
				dwSamplerMinFilter = D3DTEXF_LINEAR;
				dwSamplerMagFilter = D3DTEXF_LINEAR;
				dwSamplerMaxAnisotropy = 0;
			}break;

		};
	};

	/* camera zoom gm command, Juver, 2020/06/10 */
	void CameraZoomMultiplierReset()
	{
		fCameraZoomMultiplier = 1.0f;
	};

	/* camera zoom gm command, Juver, 2020/06/10 */
	void CameraZoomMultiplierSet(float fValue)
	{
		fCameraZoomMultiplier = 1.0f;
		fCameraZoomMultiplier += (fValue/100.0f);
	};

};
