#ifndef NSRENGINEGLOBAL_H__INCLUDED
#define NSRENGINEGLOBAL_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace RENGINE_GLOBAL
{
	void OneTimeSceneInit();
	void InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	void RestoreDeviceObjects();
	void InvalidateDeviceObjects();
	void DeleteDeviceObjects();
	void FinalCleanup();
	void FrameMove( float fTime, float fElapsedTime );
	void Render ( LPDIRECT3DDEVICEQ pd3dDevice );
	void DoInterimClean ( LPDIRECT3DDEVICEQ pd3dDevice );

	extern bool	bDISP_CONSOLE;
	extern bool	bDISP_FPS;

	extern DWORD dwSamplerMinFilter;
	extern DWORD dwSamplerMagFilter;
	extern DWORD dwSamplerMaxAnisotropy;

	/* camera zoom gm command, Juver, 2020/06/10 */
	extern float fCameraZoomMultiplier;

	void SetTextureFilters();

	/* camera zoom gm command, Juver, 2020/06/10 */
	void CameraZoomMultiplierReset();
	void CameraZoomMultiplierSet(float fValue);
};


#endif // NSRENGINEGLOBAL_H__INCLUDED
