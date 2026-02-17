/*!
 * \file DxShadowMeshData.h
 *
 * \author Juver
 * \date May 2018
 *
 * 
 */
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct SCOPYMESHDATA
{
	LPDIRECT3DVERTEXBUFFERQ	pVB;
	LPDIRECT3DINDEXBUFFERQ	pIB;
	DWORD dwVert;
	DWORD dwFace;
	DWORD dwFVF;

	SCOPYMESHDATA()
		: pVB(NULL)
		, pIB(NULL)
		, dwVert(0)
		, dwFace(0)
		, dwFVF(0)
	{
	};
};

typedef std::vector<SCOPYMESHDATA>	VECDXMESH;