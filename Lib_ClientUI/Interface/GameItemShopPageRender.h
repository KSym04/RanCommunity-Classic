/*!
 * \file GameItemShopPageRender.h
 *
 * \author Juver
 * \date 2021/12/22
 *
 * 
 */

#ifndef GAMEITEMSHOPPAGERENDER_H_INCLUDED__
#define GAMEITEMSHOPPAGERENDER_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/Meshs/DxSkinChar.h"
#include "../../Lib_Client/G-Logic/GLItem.h"
#include "../../Lib_Client/G-Logic/GLCharDefine.h"


class	CGameItemShopPageRender : public CUIGroup
{
private:
	enum
	{
	};

public:
	CGameItemShopPageRender ();
	virtual	~CGameItemShopPageRender ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT DeleteDeviceObjects ();
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );

public:
	BOOL				m_bMale;
	SNATIVEID			m_sItem[ITEM::SBOX::ITEM_SIZE];

	DxSkinChar*			m_pSkinChar;
	D3DXMATRIX			m_matTrans;
	
	bool				m_bReady;
	float				m_fRenderDelay;
	D3DXVECTOR3			m_vRot;

public:
	void DefaultPreview();
	void CreateModel( LPDIRECT3DDEVICEQ pd3dDevice );
	void RenderModel( LPDIRECT3DDEVICEQ pd3dDevice );
	void UpdateSuit( LPDIRECT3DDEVICEQ pd3dDevice );
	void ClearRender();

};

#endif // GAMEITEMSHOPPAGERENDER_H_INCLUDED__
