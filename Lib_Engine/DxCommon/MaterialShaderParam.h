/*!
 * \file MaterialShaderParam.h
 *
 * \author Juver
 * \date April 2018
 *
 * 
 */

#if !defined(_MATERIAL_SHADER_PARAM_INCLUDED_H_)
#define _MATERIAL_SHADER_PARAM_INCLUDED_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./DxCustomTypes.h"

class DxStaticMaterialMesh;
class DxPiece;
class DxPieceEdit;
struct SSHADER_BASE;

enum EMMATERIAL_RENDER_TYPE
{
	EMMATERIAL_RENDER_TYPE_NORMAL			= 0,
	EMMATERIAL_RENDER_TYPE_ALPHA			= 1,
	EMMATERIAL_RENDER_TYPE_SOFTALPHA		= 2,
	EMMATERIAL_RENDER_TYPE_BLEND			= 3,
	EMMATERIAL_RENDER_TYPE_SPECULAR_BLEND	= 4,
	EMMATERIAL_RENDER_TYPE_MATERIAL_EFFECT	= 5,
	EMMATERIAL_RENDER_TYPE_SIZE			= 6,
};

struct SMATERIAL_SETTING
{
	EMMATERIAL_RENDER_TYPE m_emRender;
	LPDIRECT3DTEXTUREQ m_pLightMapDay;
	D3DXVECTOR4		g_vLightMapUV_Offset;
	float			g_fVS_1_1_ColorMulti;

	D3DXMATRIX		g_matWorld;
	D3DXMATRIX		g_matWV;
	D3DXMATRIX		g_matVP;
	D3DXMATRIX		g_matWVP;

	D3DXVECTOR4		g_vFOG;
	D3DXVECTOR4		g_vCameraFrom;
	D3DXVECTOR4		g_vWindowSize;

	float			g_fAlpha;

	SMATERIAL_SETTING()
		: m_emRender(EMMATERIAL_RENDER_TYPE_SIZE)
		, m_pLightMapDay(NULL)
		, g_vLightMapUV_Offset( 0.0f, 0.0f, 1.0f, 1.0f )
		, g_fVS_1_1_ColorMulti(0.75f)

		, g_vFOG( 1.0f, 1.0f, 1.0f, 1.0f )
		, g_vCameraFrom( 1.0f, 1.0f, 1.0f, 1.0f )
		, g_vWindowSize( 1.0f, 1.0f, 1.0f, 1.0f )
		, g_fAlpha(1.f)
	{
		D3DXMatrixIdentity( &g_matWorld );
		D3DXMatrixIdentity( &g_matWV );
		D3DXMatrixIdentity( &g_matVP );
		D3DXMatrixIdentity( &g_matWVP );
	}

	void	Update( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld );
	void	UpdateMatrix( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld );
};

#endif // !defined(_MATERIAL_SHADER_PARAM_INCLUDED_H_)