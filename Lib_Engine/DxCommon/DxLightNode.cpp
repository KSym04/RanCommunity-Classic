#include "stdafx.h"
#include "./DxLightNode.h"

#include "../Common/SerialFile.h"
#include "./DxLight.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL DXLIGHTNODE::IsCollisionVolume ( CLIPVOLUME& cv )
{
	if (((cv.pNear.a * fMinX + cv.pNear.b * fMaxY + cv.pNear.c * fMinZ + cv.pNear.d) > -0.01f) &&	//TOP_LEFT_FRONT
		((cv.pNear.a * fMinX + cv.pNear.b * fMaxY + cv.pNear.c * fMaxZ + cv.pNear.d) > -0.01f) &&	//TOP_LEFT_BACK
		((cv.pNear.a * fMaxX + cv.pNear.b * fMaxY + cv.pNear.c * fMaxZ + cv.pNear.d) > -0.01f) &&	//TOP_RIGHT_BACK
		((cv.pNear.a * fMaxX + cv.pNear.b * fMaxY + cv.pNear.c * fMinZ + cv.pNear.d) > -0.01f) &&	//TOP_RIGHT_FRONT

		((cv.pNear.a * fMinX + cv.pNear.b * fMinY + cv.pNear.c * fMinZ + cv.pNear.d) > -0.01f) &&	//BOTTOM_LEFT_FRONT
		((cv.pNear.a * fMinX + cv.pNear.b * fMinY + cv.pNear.c * fMaxZ + cv.pNear.d) > -0.01f) &&	//BOTTOM_LEFT_BACK
		((cv.pNear.a * fMaxX + cv.pNear.b * fMinY + cv.pNear.c * fMaxZ + cv.pNear.d) > -0.01f) &&	//BOTTOM_RIGHT_BACK
		((cv.pNear.a * fMaxX + cv.pNear.b * fMinY + cv.pNear.c * fMinZ + cv.pNear.d) > -0.01f))		//BOTTOM_RIGHT_FRONT
		return FALSE;

	else if (((cv.pFar.a * fMinX + cv.pFar.b * fMaxY + cv.pFar.c * fMinZ + cv.pFar.d) > -0.01f) &&	//TOP_LEFT_FRONT
		((cv.pFar.a * fMinX + cv.pFar.b * fMaxY + cv.pFar.c * fMaxZ + cv.pFar.d) > -0.01f) &&	//TOP_LEFT_BACK
		((cv.pFar.a * fMaxX + cv.pFar.b * fMaxY + cv.pFar.c * fMaxZ + cv.pFar.d) > -0.01f) &&	//TOP_RIGHT_BACK
		((cv.pFar.a * fMaxX + cv.pFar.b * fMaxY + cv.pFar.c * fMinZ + cv.pFar.d) > -0.01f) &&	//TOP_RIGHT_FRONT

		((cv.pFar.a * fMinX + cv.pFar.b * fMinY + cv.pFar.c * fMinZ + cv.pFar.d) > -0.01f) &&	//BOTTOM_LEFT_FRONT
		((cv.pFar.a * fMinX + cv.pFar.b * fMinY + cv.pFar.c * fMaxZ + cv.pFar.d) > -0.01f) &&	//BOTTOM_LEFT_BACK
		((cv.pFar.a * fMaxX + cv.pFar.b * fMinY + cv.pFar.c * fMaxZ + cv.pFar.d) > -0.01f) &&	//BOTTOM_RIGHT_BACK
		((cv.pFar.a * fMaxX + cv.pFar.b * fMinY + cv.pFar.c * fMinZ + cv.pFar.d) > -0.01f))		//BOTTOM_RIGHT_FRONT
		return FALSE;

	if (((cv.pLeft.a * fMinX + cv.pLeft.b * fMaxY + cv.pLeft.c * fMinZ + cv.pLeft.d) > -0.01f) &&	//TOP_LEFT_FRONT
		((cv.pLeft.a * fMinX + cv.pLeft.b * fMaxY + cv.pLeft.c * fMaxZ + cv.pLeft.d) > -0.01f) &&	//TOP_LEFT_BACK
		((cv.pLeft.a * fMaxX + cv.pLeft.b * fMaxY + cv.pLeft.c * fMaxZ + cv.pLeft.d) > -0.01f) &&	//TOP_RIGHT_BACK
		((cv.pLeft.a * fMaxX + cv.pLeft.b * fMaxY + cv.pLeft.c * fMinZ + cv.pLeft.d) > -0.01f) &&	//TOP_RIGHT_FRONT

		((cv.pLeft.a * fMinX + cv.pLeft.b * fMinY + cv.pLeft.c * fMinZ + cv.pLeft.d) > -0.01f) &&	//BOTTOM_LEFT_FRONT
		((cv.pLeft.a * fMinX + cv.pLeft.b * fMinY + cv.pLeft.c * fMaxZ + cv.pLeft.d) > -0.01f) &&	//BOTTOM_LEFT_BACK
		((cv.pLeft.a * fMaxX + cv.pLeft.b * fMinY + cv.pLeft.c * fMaxZ + cv.pLeft.d) > -0.01f) &&	//BOTTOM_RIGHT_BACK
		((cv.pLeft.a * fMaxX + cv.pLeft.b * fMinY + cv.pLeft.c * fMinZ + cv.pLeft.d) > -0.01f))		//BOTTOM_RIGHT_FRONT
		return FALSE;

	else if (((cv.pRight.a * fMinX + cv.pRight.b * fMaxY + cv.pRight.c * fMinZ + cv.pRight.d) > -0.01f) &&	//TOP_LEFT_FRONT
		((cv.pRight.a * fMinX + cv.pRight.b * fMaxY + cv.pRight.c * fMaxZ + cv.pRight.d) > -0.01f) &&	//TOP_LEFT_BACK
		((cv.pRight.a * fMaxX + cv.pRight.b * fMaxY + cv.pRight.c * fMaxZ + cv.pRight.d) > -0.01f) &&	//TOP_RIGHT_BACK
		((cv.pRight.a * fMaxX + cv.pRight.b * fMaxY + cv.pRight.c * fMinZ + cv.pRight.d) > -0.01f) &&	//TOP_RIGHT_FRONT

		((cv.pRight.a * fMinX + cv.pRight.b * fMinY + cv.pRight.c * fMinZ + cv.pRight.d) > -0.01f) &&	//BOTTOM_LEFT_FRONT
		((cv.pRight.a * fMinX + cv.pRight.b * fMinY + cv.pRight.c * fMaxZ + cv.pRight.d) > -0.01f) &&	//BOTTOM_LEFT_BACK
		((cv.pRight.a * fMaxX + cv.pRight.b * fMinY + cv.pRight.c * fMaxZ + cv.pRight.d) > -0.01f) &&	//BOTTOM_RIGHT_BACK
		((cv.pRight.a * fMaxX + cv.pRight.b * fMinY + cv.pRight.c * fMinZ + cv.pRight.d) > -0.01f))		//BOTTOM_RIGHT_FRONT
		return FALSE;

	if (((cv.pTop.a * fMinX + cv.pTop.b * fMaxY + cv.pTop.c * fMinZ + cv.pTop.d) > -0.01f) &&	//TOP_LEFT_FRONT
		((cv.pTop.a * fMinX + cv.pTop.b * fMaxY + cv.pTop.c * fMaxZ + cv.pTop.d) > -0.01f) &&	//TOP_LEFT_BACK
		((cv.pTop.a * fMaxX + cv.pTop.b * fMaxY + cv.pTop.c * fMaxZ + cv.pTop.d) > -0.01f) &&	//TOP_RIGHT_BACK
		((cv.pTop.a * fMaxX + cv.pTop.b * fMaxY + cv.pTop.c * fMinZ + cv.pTop.d) > -0.01f) &&	//TOP_RIGHT_FRONT

		((cv.pTop.a * fMinX + cv.pTop.b * fMinY + cv.pTop.c * fMinZ + cv.pTop.d) > -0.01f) &&	//BOTTOM_LEFT_FRONT
		((cv.pTop.a * fMinX + cv.pTop.b * fMinY + cv.pTop.c * fMaxZ + cv.pTop.d) > -0.01f) &&	//BOTTOM_LEFT_BACK
		((cv.pTop.a * fMaxX + cv.pTop.b * fMinY + cv.pTop.c * fMaxZ + cv.pTop.d) > -0.01f) &&	//BOTTOM_RIGHT_BACK
		((cv.pTop.a * fMaxX + cv.pTop.b * fMinY + cv.pTop.c * fMinZ + cv.pTop.d) > -0.01f))		//BOTTOM_RIGHT_FRONT
		return FALSE;

	else if (((cv.pBottom.a * fMinX + cv.pBottom.b * fMaxY + cv.pBottom.c * fMinZ + cv.pBottom.d) > -0.01f) &&	//TOP_LEFT_FRONT
		((cv.pBottom.a * fMinX + cv.pBottom.b * fMaxY + cv.pBottom.c * fMaxZ + cv.pBottom.d) > -0.01f) &&	//TOP_LEFT_BACK
		((cv.pBottom.a * fMaxX + cv.pBottom.b * fMaxY + cv.pBottom.c * fMaxZ + cv.pBottom.d) > -0.01f) &&	//TOP_RIGHT_BACK
		((cv.pBottom.a * fMaxX + cv.pBottom.b * fMaxY + cv.pBottom.c * fMinZ + cv.pBottom.d) > -0.01f) &&	//TOP_RIGHT_FRONT

		((cv.pBottom.a * fMinX + cv.pBottom.b * fMinY + cv.pBottom.c * fMinZ + cv.pBottom.d) > -0.01f) &&	//BOTTOM_LEFT_FRONT
		((cv.pBottom.a * fMinX + cv.pBottom.b * fMinY + cv.pBottom.c * fMaxZ + cv.pBottom.d) > -0.01f) &&	//BOTTOM_LEFT_BACK
		((cv.pBottom.a * fMaxX + cv.pBottom.b * fMinY + cv.pBottom.c * fMaxZ + cv.pBottom.d) > -0.01f) &&	//BOTTOM_RIGHT_BACK
		((cv.pBottom.a * fMaxX + cv.pBottom.b * fMinY + cv.pBottom.c * fMinZ + cv.pBottom.d) > -0.01f))		//BOTTOM_RIGHT_FRONT
		return FALSE;

	return TRUE;
}


void	DXLIGHTNODE::SaveSet ( CSerialFile &SFile )
{
	BOOL	bResult = FALSE;

	SFile.WriteBuffer ( &vMax, sizeof ( D3DXVECTOR3 ) );
	SFile.WriteBuffer ( &vMin, sizeof ( D3DXVECTOR3 ) );

	if ( pLight )
	{
		SFile << BOOL ( TRUE );
		SFile.WriteBuffer ( pLight->m_szLightName, sizeof ( char ) * 256 );
	}
	else
	{
		SFile << BOOL ( FALSE );
	}

	if ( pLeftChild )
	{
		SFile << BOOL ( TRUE );
		pLeftChild->SaveSet ( SFile );		
	}
	else
	{
		SFile << BOOL ( FALSE );
	}

	if ( pRightChild )
	{
		SFile << BOOL ( TRUE );
		pRightChild->SaveSet ( SFile );
	}
	else
	{
		SFile << BOOL ( FALSE );
	}
}

void	DXLIGHTNODE::LoadSet ( CSerialFile &SFile, DXLIGHT	*pLtListHead )
{
	BOOL	bResult = FALSE;

	SFile.ReadBuffer ( &vMax, sizeof ( D3DXVECTOR3 ) );
	SFile.ReadBuffer ( &vMin, sizeof ( D3DXVECTOR3 ) );

	SFile >> bResult;
	if ( bResult )
	{
		char	szLightName[STRING_LENGTH_256] = "";		
		SFile.ReadBuffer ( szLightName, sizeof ( char ) * STRING_LENGTH_256 );

		//	<--	이름으로 노드를 찾아 연결함
		DXLIGHT	*pLtListNode = pLtListHead;
		while ( pLtListNode )
		{
			if ( !strcmp ( szLightName, pLtListNode->m_szLightName ) )
			{
				pLight = pLtListNode;
				break;
			}
			pLtListNode = pLtListNode->pNext;
		}
	}

	SFile >> bResult;
	if ( bResult )
	{
		pLeftChild = new DXLIGHTNODE();
		pLeftChild->LoadSet ( SFile, pLtListHead );
	}

	SFile >> bResult;
	if ( bResult )
	{
		pRightChild = new DXLIGHTNODE();
		pRightChild->LoadSet ( SFile, pLtListHead );
	}
}
