#include "stdafx.h"
#include "./MaterialShaderParam.h"


#include "./DxViewPort.h"
#include "./DxFogMan.h"
#include "./DxShaderManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define MULTI_LINE_STRING(...) #__VA_ARGS__

void SMATERIAL_SETTING::Update( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld )
{
	UpdateMatrix( pd3dDevice, matWorld );

	float fFogNear = DxFogMan::GetInstance().m_fLastNear;
	float fFogFar = DxFogMan::GetInstance().m_fLastFar;
	g_vFOG = D3DXVECTOR4( fFogFar, fFogFar - fFogNear, 1.0f, 1.0f );

	D3DXVECTOR3 vFrom = DxViewPort::GetInstance().GetFromPt();
	g_vCameraFrom = D3DXVECTOR4( vFrom.x, vFrom.y, vFrom.z, 1.0f );

	float fwindow_size = 0.5f + (0.5f / (float)512);
	g_vWindowSize = D3DXVECTOR4( fwindow_size, fwindow_size, 1.0f, 1.0f );
}

void SMATERIAL_SETTING::UpdateMatrix( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matWorld )
{
	D3DXMATRIX matView = DxViewPort::GetInstance().GetMatView();
	D3DXMATRIX matProj = DxViewPort::GetInstance().GetMatProj();
	g_matWorld = matWorld;

	D3DXMatrixMultiply(&g_matVP, &matView, &matProj );
	D3DXMatrixMultiply(&g_matWV, &g_matWorld, &matView );
	D3DXMatrixMultiply(&g_matWVP, &g_matWorld, &g_matVP );
}