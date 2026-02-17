#ifndef DXLIGHT_NODE_H_
#define DXLIGHT_NODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./DxCustomTypes.h"

class	CSerialFile;
class	DXLIGHT;

struct DXLIGHTNODE
{
	union
	{
		struct { float fMaxX, fMaxY, fMaxZ; };
		struct { D3DXVECTOR3 vMax; };
	};

	union
	{
		struct { float fMinX, fMinY, fMinZ; };
		struct { D3DXVECTOR3 vMin; };
	};

	DXLIGHT			*pLight;

	//	Note : 정렬 트리용.
	//
	DXLIGHTNODE		*pLeftChild;
	DXLIGHTNODE		*pRightChild;

	BOOL IsCollisionVolume ( CLIPVOLUME & cv );

	DXLIGHTNODE () :
	fMaxX(0.0f),
		fMaxY(0.0f),
		fMaxZ(0.0f),
		fMinX(0.0f),
		fMinY(0.0f),
		fMinZ(0.0f),
		pLight(NULL),
		pLeftChild(NULL),
		pRightChild(NULL)
	{
	}

	~DXLIGHTNODE ()
	{
		SAFE_DELETE(pLeftChild);
		SAFE_DELETE(pRightChild);
	}

public:
	//	<--	데이타 Save/Load
	void	SaveSet ( CSerialFile &SFile );
	void	LoadSet ( CSerialFile &SFile, DXLIGHT	*pLtListHead );
	//	-->	데이타 Save/Load
};


#endif // DXLIGHT_NODE_H_