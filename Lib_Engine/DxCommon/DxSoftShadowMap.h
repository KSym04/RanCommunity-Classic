#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./DxViewPort.h"
#include "./DxLightMan.h"
#include "./TextureManager.h"

class DxSkinChar;

enum EMSOFTSHADOWDETAIL
{
	SOFT_SHADOW_DISABLE		= 0,
	SOFT_SHADOW_ENABLE		= 1,
	SOFT_SHADOW_NSIZE		= 3,
};

class DxSoftShadowMap
{

protected:	
	static LPDIRECT3DSTATEBLOCK9	m_pSavedStateBlock;
	static LPDIRECT3DSTATEBLOCK9	m_pEffectStateBlock;

protected:
	const		static	float				EYE_TO_LOOKAT;

protected:
	DWORD		m_dwUseSwShader;
	BOOL		m_bUseMIXED;


	EMSOFTSHADOWDETAIL		m_emShadowDetail;

protected:
	float				m_fDistance;
	float				m_fDistanceRate;

	float				m_fTexWidth;
	float				m_fTexHeight;

	D3DXVECTOR3			m_vBaseViewMin;
	D3DXVECTOR3			m_vBaseViewMax;

	D3DXVECTOR3			m_vViewMin;
	D3DXVECTOR3			m_vViewMax;

	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProj;
	D3DXMATRIX			m_matLastWVP_D;

protected:
	BOOL	m_bUse;
	BOOL	m_bCharUse;
	BOOL	m_bImageBlurUse;

public:
	D3DXVECTOR3		GetViewMax ()			{ return m_vViewMax; }
	D3DXVECTOR3		GetViewMin ()			{ return m_vViewMin; }

public:
	D3DXMATRIX		GetMatrix_D ()			{ return m_matLastWVP_D; }

	EMSOFTSHADOWDETAIL		GetShadowDetail ()							{ return m_emShadowDetail; }
	void				SetShadowDetail ( EMSOFTSHADOWDETAIL Detail )	{ m_emShadowDetail = Detail; }

	BOOL	IsUseShadow()			{ return m_bUse; }
	void	SetUseShadow(BOOL bUse) { m_bUse = bUse; }

public:
	HRESULT ClearShadow ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RenderShadow ( DxSkinChar *pSkinObj, D3DXMATRIX &matRot, LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RenderShadowMob ( DxSkinChar *pSkinObj, D3DXMATRIX &matRot, LPDIRECT3DDEVICEQ pd3dDevice );

	HRESULT RenderShadowCharMob ( DxSkinChar *pSkinObj, D3DXMATRIX &matRot, LPDIRECT3DDEVICEQ pd3dDevice );
	void	LastImageBlur ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	HRESULT InitDeviceObjects ( LPDIRECT3DQ pD3D, LPDIRECT3DDEVICEQ pd3dDevice, D3DSURFACE_DESC &d3dsdBackBuffer );
	HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT InvalidateDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT DeleteDeviceObjects();

	HRESULT FrameMove ( float fTime, float fElapsedTime );

protected:
	HRESULT	Shadow_Directional_Light( LPDIRECT3DDEVICEQ pd3dDevice, DxSkinChar *pSkinObj, D3DXMATRIX &matRot );

protected:
	DxSoftShadowMap();

public:
	virtual ~DxSoftShadowMap();

public:
	static DxSoftShadowMap& GetInstance();
};
