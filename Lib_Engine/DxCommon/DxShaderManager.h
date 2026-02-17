/*!
 * \file DxShaderManager.h
 *
 * \author Juver
 * \date April 2018
 *
 * 
 */

#if !defined(_DXSHADER_MANAGER_INCLUDED_H_)
#define _DXSHADER_MANAGER_INCLUDED_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class DxShaderManager
{
private:
	typedef std::map<std::string,ID3DXEffect*>		D3DXEFFECT_MAP;
	typedef D3DXEFFECT_MAP::iterator				D3DXEFFECT_MAP_ITER;

protected:
	std::string		m_strPATH;

	D3DXEFFECT_MAP	m_mapShader;
	BOOL			m_bShaderUse;

public:
	void SetPath ( std::string strPATH )	{ m_strPATH = strPATH; }
	std::string GetPath ()					{ return m_strPATH.c_str(); }

	ID3DXEffect* GetShader( std::string strShader );
	ID3DXEffect* LoadFile( std::string strFile, LPDIRECT3DDEVICEQ pd3dDevice );
	ID3DXEffect* LoadString( std::string strString, std::string strName, LPDIRECT3DDEVICEQ pd3dDevice );

	void FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );
	void CreateDevice( LPDIRECT3DDEVICEQ pd3dDevice );
	void ResetDevice( LPDIRECT3DDEVICEQ pd3dDevice );
	void LostDevice( LPDIRECT3DDEVICEQ pd3dDevice );
	void DestroyDevice();

protected:
	DxShaderManager();

public:
	~DxShaderManager();

public:
	static DxShaderManager& GetInstance();
};

#endif // !defined(_DXSHADER_MANAGER_INCLUDED_H_)
