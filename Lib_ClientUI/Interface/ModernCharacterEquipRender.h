/*!
 * \file ModernCharacterEquipRender.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */

#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/Meshs/DxSkinChar.h"

#define RENDER_DELAY_TIME 0.2f

class	CModernCharacterEquipRender : public CUIGroup
{

private:
	enum
	{
	};

public:
	CModernCharacterEquipRender ();
	virtual	~CModernCharacterEquipRender ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT DeleteDeviceObjects ();
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );

public:
	DxSkinChar*			m_pSkinChar;
	D3DXMATRIX			m_matTrans;
	bool				m_bReady;
	float				m_fRenderDelay;
	D3DXVECTOR3			m_vRot;
	float				m_fScaleRange;
	bool				m_bUpdateSuit;

public:
	void	CreateModel( LPDIRECT3DDEVICEQ pd3dDevice );
	void	RenderModel( LPDIRECT3DDEVICEQ pd3dDevice );
	void	UpdateSuit( LPDIRECT3DDEVICEQ pd3dDevice );
	void	ClearRender();

};