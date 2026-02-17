#include "stdafx.h"

#include "../DxCommon/DxMethods.h"
#include "../DxCommon/EDITMESHS.h"
#include "../NaviMesh/NavigationMesh.h"
#include "../Common/SerialFile.h"
#include "../Common/basestream.h"
#include "../Common/StlFunctions.h"

#include <algorithm>
#include "./DxLandGateMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL DxLandGateMan::m_bREND = TRUE;

void DxLandGate::SetGate ( DWORD dwFlags, DWORD GateID, const D3DXVECTOR2 &vDiv )
{
	m_dwGeteFlags = dwFlags;
	m_GateID = GateID;
	m_vDiv = vDiv;
}

void DxLandGate::SetPos ( const D3DXVECTOR3 &vPos )
{
	D3DXVECTOR3 vSize = m_vMax - m_vMin;

	m_vMax = vPos + vSize/2;
	m_vMin = vPos - vSize/2;
}

void DxLandGate::SetBox ( const D3DXVECTOR3 &vMax, const D3DXVECTOR3 &vMin )
{
	GASSERT(vMax.x>=vMin.x);
	GASSERT(vMax.y>=vMin.y);
	GASSERT(vMax.z>=vMin.z);

	m_vMax = vMax;
	m_vMin = vMin;
}

D3DXVECTOR3 DxLandGate::GetGenPos ( DxLandGate::EMGENPOS_SEL emGenSel )
{
	if ( m_dwStartPNum==0 )		return D3DXVECTOR3(0,0,0);

	DWORD dwGenPos = 0;
	switch ( emGenSel )
	{
	case GEN_FIRST:
		{
			dwGenPos = 0;
		}
		break;

	case GEN_RENDUM:
		{
			if ( ++m_dwLastUsedPos < m_dwStartPNum )
			{
				dwGenPos = m_dwLastUsedPos;
			}
			else
			{
				dwGenPos = m_dwLastUsedPos = 0;
			}
		}
		break;
	};

	return m_pvStartPos[dwGenPos];
}

void DxLandGate::SetObjRotate90()
{
	D3DXMATRIX matRotate;
	D3DXMatrixRotationY( &matRotate, D3DX_PI*0.5f );

	D3DXMatrixAABBTransform( m_vMax, m_vMin, matRotate );

	for ( DWORD i=0; i<m_dwStartPNum; i++ )
	{
		D3DXVec3TransformCoord( &m_pvStartPos[i], &m_pvStartPos[i], &matRotate );
	}
}

HRESULT DxLandGate::Load ( basestream	&SFile )
{
	DWORD dwVer, dwSize;
	SFile >> dwVer;
	SFile >> dwSize;

	if ( VERSION == dwVer )
	{
		SFile.ReadBuffer ( &m_Prop, sizeof(PROPERTY) );
		
		if ( m_Prop.m_dwStartPNum != 0 )
		{
			m_pvStartPos = new D3DXVECTOR3[m_Prop.m_dwStartPNum];
			SFile.ReadBuffer ( m_pvStartPos, sizeof(D3DXVECTOR3)*m_Prop.m_dwStartPNum );
		}
	}
	else if ( dwVer == 0x0103 )
	{
		PROPERTY_103 sProp;
		SFile.ReadBuffer ( &sProp, sizeof(PROPERTY_103) );

		StringCchCopy( m_Prop.m_szName, MAX_SZNAME, sProp.m_szName);
		StringCchCopy( m_Prop.m_szInfo, MAX_SZNAME, sProp.m_szInfo);
		m_Prop.m_GateID = sProp.m_GateID;
		m_Prop.m_dwGeteFlags = sProp.m_dwGeteFlags;
		m_Prop.m_dwStartPNum = sProp.m_dwStartPNum;

		m_Prop.m_vDiv = sProp.m_vDiv;
		m_Prop.m_vMin = sProp.m_vMin;
		m_Prop.m_vMax = sProp.m_vMax;

		/* multi gate out, Juver, 2020/11/17 */
		for ( int i=0; i<MAX_GATE_OUT; ++i )
		{
			StringCchCopy( m_Prop.m_szNameKEY[i], MAX_SZNAME, sProp.m_szNameKEY[i] );
			m_Prop.m_dwNameINDEX[i] = sProp.m_dwNameINDEX[i];

			m_Prop.m_ToMapID[i] = sProp.m_ToMapID[i];
			m_Prop.m_ToGateID[i] = sProp.m_ToGateID[i];
		}
		
		m_Prop.m_dwToMapNum = sProp.m_dwToMapNum;
		m_Prop.m_bNewSystem = FALSE;

		if ( m_Prop.m_dwStartPNum != 0 )
		{
			m_pvStartPos = new D3DXVECTOR3[m_Prop.m_dwStartPNum];
			SFile.ReadBuffer ( m_pvStartPos, sizeof(D3DXVECTOR3)*m_Prop.m_dwStartPNum );
		}
	}
	else if ( dwVer == 0x0102 )
	{
		PROPERTY_102 sProp;
		SFile.ReadBuffer ( &sProp, sizeof(PROPERTY_102) );

		StringCchCopy( m_Prop.m_szName, MAX_SZNAME, sProp.m_szName);
		StringCchCopy( m_Prop.m_szInfo, MAX_SZNAME, sProp.m_szInfo);
		m_Prop.m_GateID = sProp.m_GateID;
		m_Prop.m_dwGeteFlags = sProp.m_dwGeteFlags;
		m_Prop.m_dwStartPNum = sProp.m_dwStartPNum;
		
		m_Prop.m_vDiv = sProp.m_vDiv;
		m_Prop.m_vMin = sProp.m_vMin;
		m_Prop.m_vMax = sProp.m_vMax;

		/* multi gate out, Juver, 2020/11/17 */
		m_Prop.m_ToMapID[0] = sProp.m_ToMapID;
		m_Prop.m_ToGateID[0] = sProp.m_ToGateID;
		m_Prop.m_dwToMapNum = 1;
		m_Prop.m_bNewSystem = FALSE;

		if ( m_Prop.m_dwStartPNum != 0 )
		{
			m_pvStartPos = new D3DXVECTOR3[m_Prop.m_dwStartPNum];
			SFile.ReadBuffer ( m_pvStartPos, sizeof(D3DXVECTOR3)*m_Prop.m_dwStartPNum );
		}
	}
	else if ( dwVer == 0x0101 )
	{
		PROPERTY_101 sProp;
		SFile.ReadBuffer ( &sProp, sizeof(PROPERTY_101) );

		StringCchCopy( m_Prop.m_szName, MAX_SZNAME, sProp.m_szName);
		m_Prop.m_GateID = sProp.m_GateID;
		m_Prop.m_dwGeteFlags = sProp.m_dwGeteFlags;
		m_Prop.m_dwStartPNum = sProp.m_dwStartPNum;
		
		m_Prop.m_vDiv = sProp.m_vDiv;
		m_Prop.m_vMin = sProp.m_vMin;
		m_Prop.m_vMax = sProp.m_vMax;

		/* multi gate out, Juver, 2020/11/17 */
		m_Prop.m_ToMapID[0] = sProp.m_ToMapID;
		m_Prop.m_ToGateID[0] = sProp.m_ToGateID;
		m_Prop.m_dwToMapNum = 1;
		m_Prop.m_bNewSystem = FALSE;

		if ( m_Prop.m_dwStartPNum != 0 )
		{
			m_pvStartPos = new D3DXVECTOR3[m_Prop.m_dwStartPNum];
			SFile.ReadBuffer ( m_pvStartPos, sizeof(D3DXVECTOR3)*m_Prop.m_dwStartPNum );
		}
	}
	else if ( dwVer == 0x0100 )
	{
		PROPERTY_100 sProp;
		SFile.ReadBuffer ( &sProp, sizeof(PROPERTY_100) );

		StringCchCopy( m_Prop.m_szName, MAX_SZNAME, sProp.m_szName);
		m_Prop.m_GateID = sProp.m_GateID;
		m_Prop.m_dwGeteFlags = sProp.m_dwGeteFlags;
		m_Prop.m_dwStartPNum = sProp.m_dwStartPNum;

		m_Prop.m_vDiv = sProp.m_vDiv;
		m_Prop.m_vMin = sProp.m_vMin;
		m_Prop.m_vMax = sProp.m_vMax;

		/* multi gate out, Juver, 2020/11/17 */
		m_Prop.m_ToMapID[0] = sProp.m_ToMapID;
		m_Prop.m_ToGateID[0] = sProp.m_ToGateID;
		m_Prop.m_dwToMapNum = 1;
		m_Prop.m_bNewSystem = FALSE;

		if ( m_Prop.m_dwStartPNum != 0 )
		{
			m_pvStartPos = new D3DXVECTOR3[m_Prop.m_dwStartPNum];
			SFile.ReadBuffer ( m_pvStartPos, sizeof(D3DXVECTOR3)*m_Prop.m_dwStartPNum );
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DxLandGate::Load", dwVer );
		SFile.SetOffSet ( SFile.GetfTell()+dwSize );
	}

	return S_OK;
}

HRESULT DxLandGate::Save ( CSerialFile	&SFile )
{
	DWORD dwSize = sizeof(PROPERTY) + sizeof(DWORD) + sizeof(D3DXVECTOR3)*m_dwStartPNum;

	SFile << (DWORD)VERSION;
	SFile << dwSize;

	SFile.WriteBuffer ( &m_Prop, sizeof(PROPERTY) );
		
	if ( m_Prop.m_dwStartPNum != 0 )
	{
		SFile.WriteBuffer ( m_pvStartPos, sizeof(D3DXVECTOR3)*m_Prop.m_dwStartPNum );
	}

	return S_OK;
}

VOID DxLandGate::SaveCsvHead ( std::fstream &SFile )
{
	SFile<< "m_szName"								<< ",";
	SFile<< "m_szInfo"								<< ",";
	SFile<< "m_GateID"								<< ",";
	SFile<< "m_dwGateFlags"							<< ",";
	SFile<< "m_dwStartPNum"							<< ",";

	SFile<< "m_vDiv.x"								<< ",";
	SFile<< "m_vDiv.y"								<< ",";

	SFile<< "m_vMin.x"								<< ",";
	SFile<< "m_vMin.y"								<< ",";
	SFile<< "m_vMin.z"								<< ",";

	SFile<< "m_vMax.x"								<< ",";
	SFile<< "m_vMax.y"								<< ",";
	SFile<< "m_vMax.z"								<< ",";

	SFile<< "m_dwToMapNum"							<< ",";
	SFile<< "m_bNewSystem"							<< ",";

	for ( int i=0; i<MAX_GATE_OUT; ++i )
	{
		SFile << "m_szNameKEY " << i << ",";
		SFile << "m_dwNameINDEX " << i << ",";
		SFile << "m_ToMapID.wMid " << i << ",";
		SFile << "m_ToMapID.WSid " << i << ",";
		SFile << "m_ToGateID " << i << ",";
	}

	SFile << std::endl;
}

VOID DxLandGate::SaveCsv ( std::fstream &SFile )	const
{
	if( strlen( m_szName ) < 1 )
		SFile << " " << ",";
	else
		SFile << m_szName << ",";

	if( strlen( m_szInfo ) < 1 )
		SFile << " " << ",";
	else
		SFile << m_szInfo << ",";

	SFile << m_GateID  << ",";
	SFile << m_dwGeteFlags  << ",";
	SFile << m_dwStartPNum  << ",";

	SFile << m_vDiv.x  << ",";
	SFile << m_vDiv.y  << ",";

	SFile << m_vMin.x  << ",";
	SFile << m_vMin.y  << ",";
	SFile << m_vMin.z  << ",";

	SFile << m_vMax.x  << ",";
	SFile << m_vMax.y  << ",";
	SFile << m_vMax.z  << ",";

	SFile << m_dwToMapNum  << ",";
	SFile << m_bNewSystem  << ",";

	for ( int i=0; i<MAX_GATE_OUT; ++i )
	{
		if( strlen( m_szNameKEY[i] ) < 1 )
			SFile << " " << ",";
		else
			SFile << m_szNameKEY[i] << ",";

		SFile << m_dwNameINDEX[i]  << ",";

		SFile << m_ToMapID[i].wMainID  << ",";
		SFile << m_ToMapID[i].wSubID  << ",";
		SFile << m_ToGateID[i]  << ",";
	}

	SFile << std::endl;
}

VOID DxLandGate::LoadCsv ( CStringArray &StrArray )
{
	int iCsvCur = 0;

	CString strName = StrArray[ iCsvCur++ ];
	if( strlen( strName ) > 1 && strName.GetAt( 0 ) != ' ' )
		StringCchCopy( m_szName, MAX_SZNAME, strName );

	CString strInfo = StrArray[ iCsvCur++ ];
	if( strlen( strInfo ) > 1 && strInfo.GetAt( 0 ) != ' ' )
		StringCchCopy( m_szInfo, MAX_SZNAME, strInfo );

	m_GateID = (DWORD)_atoi64( StrArray[ iCsvCur++ ] );
	m_dwGeteFlags = (DWORD)_atoi64( StrArray[ iCsvCur++ ] );
	m_dwStartPNum = (DWORD)_atoi64( StrArray[ iCsvCur++ ] );


	m_vDiv.x = (float)atof( StrArray[ iCsvCur++ ] );
	m_vDiv.y = (float)atof( StrArray[ iCsvCur++ ] );

	m_vMin.x = (float)atof( StrArray[ iCsvCur++ ] );
	m_vMin.y = (float)atof( StrArray[ iCsvCur++ ] );
	m_vMin.z = (float)atof( StrArray[ iCsvCur++ ] );

	m_vMax.x = (float)atof( StrArray[ iCsvCur++ ] );
	m_vMax.y = (float)atof( StrArray[ iCsvCur++ ] );
	m_vMax.z = (float)atof( StrArray[ iCsvCur++ ] );

	m_dwToMapNum = (DWORD)_atoi64( StrArray[ iCsvCur++ ] );
	m_bNewSystem = (int)atoi( StrArray[ iCsvCur++ ] );

	for ( int i=0; i<MAX_GATE_OUT; ++i )
	{
		CString strNameKey = StrArray[ iCsvCur++ ];
		if( strlen( strNameKey ) > 1 && strNameKey.GetAt( 0 ) != ' ' )
			StringCchCopy( m_szNameKEY[i], MAX_SZNAME, strNameKey );

		m_dwNameINDEX[i] = (DWORD)_atoi64( StrArray[ iCsvCur++ ] );

		m_ToMapID[i].wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		m_ToMapID[i].wSubID  = (WORD)atoi( StrArray[ iCsvCur++ ] );

		m_ToGateID[i] = (DWORD)_atoi64( StrArray[ iCsvCur++ ] );
	}

}

void DxLandGate::operator= ( DxLandGate &LandGate )
{
	m_Prop = LandGate.m_Prop;

	SAFE_DELETE_ARRAY(m_pvStartPos);
	m_pvStartPos = new D3DXVECTOR3[m_Prop.m_dwStartPNum];
	memcpy ( m_pvStartPos, LandGate.m_pvStartPos, sizeof(D3DXVECTOR3)*m_Prop.m_dwStartPNum );
}

BOOL DxLandGate::MakeStartPos ( NavigationMesh* pNaviMesh )
{
	GASSERT(pNaviMesh);
	SAFE_DELETE_ARRAY(m_pvStartPos);

	D3DXVECTOR3 vTestUp;
	D3DXVECTOR3 vTestDown;

	D3DXVECTOR3 vCollision;
	DWORD dwCollisionID;
	BOOL bCollision;
	D3DXVECTOR3 vCenter = (m_vMax+m_vMin)/2;

	m_dwStartPNum = 0;
	std::vector<D3DXVECTOR3> PosArray;
	for ( float fX=(m_vMin.x+m_vDiv.x); fX<(m_vMax.x-m_vDiv.x); fX+=m_vDiv.x )
	for ( float fZ=(m_vMin.z+m_vDiv.y); fZ<(m_vMax.z-m_vDiv.y); fZ+=m_vDiv.y )
	{
		vTestUp = D3DXVECTOR3(fX,m_vMax.y,fZ);
		vTestDown = D3DXVECTOR3(fX,-FLT_MAX,fZ);

		pNaviMesh->IsCollision ( vTestUp, vTestDown, vCollision, dwCollisionID, bCollision );
		if ( bCollision )
		{
			PosArray.push_back ( vCollision );
			m_dwStartPNum++;
		}
	}
	
	if ( m_dwStartPNum == 0 )
	{
		vTestUp = D3DXVECTOR3(vCenter.x, m_vMax.y, vCenter.z );
		vTestDown = D3DXVECTOR3(vCenter.x, -FLT_MAX, vCenter.z );

		pNaviMesh->IsCollision ( vTestUp, vTestDown, vCollision, dwCollisionID, bCollision );
		if ( bCollision )
		{
			m_pvStartPos = new D3DXVECTOR3[1];
			m_pvStartPos[0] = vCollision;
			m_dwStartPNum = 1;
		}
		else	return FALSE;
	}
	else
	{
		//	Note : PosArray 에 있는 위치를 정렬후 배열에 복사 작업.
		//
		m_pvStartPos = new D3DXVECTOR3[m_dwStartPNum];
		if ( m_dwStartPNum > 1 )
		{
			std_afunc::CCompare Comp(vCenter);
			std::sort ( PosArray.begin(), PosArray.end(), Comp );

			for ( DWORD j=0; j<m_dwStartPNum; j++ )
				m_pvStartPos[j] = *(PosArray.begin()+j);
		}
	}

	return TRUE;
}

HRESULT DxLandGate::Render ( LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwRenderColor /*= 0xffffffff*/ )
{
	EDITMESHS::RENDERAABB ( pd3dDevice, m_vMax, m_vMin, dwRenderColor );

	for ( DWORD i=0; i<m_dwStartPNum; i++ )
	{
		EDITMESHS::RENDERSPHERE ( pd3dDevice, m_pvStartPos[i], 2.0f, NULL, dwRenderColor );
	}

	return S_OK;
}



/* multi gate out, Juver, 2020/11/16 */
char* DxLandGate::GetNameKEY( DWORD dwIndex )
{
	if ( dwIndex < MAX_GATE_OUT )
	{
		return m_szNameKEY[dwIndex];
	}

	return  m_szNameKEY[DEFAULT_GATE_OUT_INDEX];
}

/* multi gate out, Juver, 2020/11/16 */
DWORD DxLandGate::GetNameINDEX( DWORD dwIndex )
{
	if ( dwIndex < MAX_GATE_OUT )
	{
		return m_dwNameINDEX[dwIndex];
	}

	return  m_dwNameINDEX[DEFAULT_GATE_OUT_INDEX];
}

/* multi gate out, Juver, 2020/11/16 */
SNATIVEID DxLandGate::GetToMapID( DWORD dwIndex )
{
	if ( dwIndex < MAX_GATE_OUT )
	{
		return m_ToMapID[dwIndex];
	}

	return  m_ToMapID[DEFAULT_GATE_OUT_INDEX];
}

/* multi gate out, Juver, 2020/11/16 */
DWORD DxLandGate::GetToGateID( DWORD dwIndex )
{
	if ( dwIndex < MAX_GATE_OUT )
	{
		return m_ToGateID[dwIndex];
	}

	return  m_ToGateID[DEFAULT_GATE_OUT_INDEX];
}

/* multi gate out, Juver, 2020/11/16 */
void DxLandGate::SetNameKEY( DWORD dwIndex, const char* szNameKEY )
{
	if ( dwIndex < MAX_GATE_OUT )
	{
		StringCchCopy( m_szNameKEY[dwIndex], MAX_SZNAME, szNameKEY );
	}
}

/* multi gate out, Juver, 2020/11/16 */
void DxLandGate::SetNameINDEX( DWORD dwIndex, DWORD dwNameINDEX )
{
	if ( dwIndex < MAX_GATE_OUT )
	{
		m_dwNameINDEX[dwIndex] = dwNameINDEX;
	}
}

/* multi gate out, Juver, 2020/11/16 */
void DxLandGate::SetToMapID( DWORD dwIndex, SNATIVEID sMapID )
{
	if ( dwIndex < MAX_GATE_OUT )
	{
		m_ToMapID[dwIndex] = sMapID;
	}
}

/* multi gate out, Juver, 2020/11/16 */
void DxLandGate::SetToGateID( DWORD dwIndex, DWORD dwGateID )
{
	if ( dwIndex < MAX_GATE_OUT )
	{
		m_ToGateID[dwIndex] = dwGateID;
	}
}

/* multi gate out, Juver, 2020/11/17 */
void DxLandGate::CountToMap()
{
	m_dwToMapNum = 0;

	for( DWORD i=0; i<MAX_GATE_OUT; ++i )
	{
		if ( m_ToMapID[i].IsValidNativeID() )
		{
			m_dwToMapNum++;
		}
	}
}

DxLandGateMan::DxLandGateMan(void) :
	m_dwNumLandGate(0),
	m_pLandGateList(NULL),
	m_pLandGateTree(NULL)
{
}

DxLandGateMan::~DxLandGateMan(void)
{
	CleanUp ();
}

void DxLandGateMan::AddLandGate ( PDXLANDGATE pLandGate )
{
	m_dwNumLandGate++;
	pLandGate->m_pNext = m_pLandGateList;
	m_pLandGateList = pLandGate;
}

void DxLandGateMan::DelLandGate ( char* szName )
{
	GASSERT(szName);
	PDXLANDGATE pCur, pBack;

	if ( !strcmp(m_pLandGateList->GetName(),szName) )
	{
		pCur = m_pLandGateList;
		m_pLandGateList = m_pLandGateList->m_pNext;

		pCur->m_pNext = NULL;
		delete pCur;

		m_dwNumLandGate--;
		return;
	}

	pCur = m_pLandGateList;
	while ( pCur )
	{
		if ( !strcmp(pCur->GetName(),szName) )
		{
			pBack->m_pNext = pCur->m_pNext;

			pCur->m_pNext = NULL;
			delete pCur;

			m_dwNumLandGate--;
			return;
		}

		pBack = pCur;
		pCur = pCur->m_pNext;
	}
}

void DxLandGateMan::DelLandGate ( DWORD dwGateID )
{
	PDXLANDGATE pGATE = FindLandGate( dwGateID );
	if ( pGATE )
		DelLandGate( pGATE->GetName() );
}

PDXLANDGATE DxLandGateMan::FindLandGate ( const char* szName )
{
	GASSERT(szName);

	PDXLANDGATE pCur = m_pLandGateList;
	while ( pCur )
	{
		if ( !strcmp(szName,pCur->GetName()) )	return pCur;
		pCur = pCur->m_pNext;
	}

	return NULL;
}

PDXLANDGATE DxLandGateMan::FindLandGate ( DWORD dwGateID )
{
	PDXLANDGATE pCur = m_pLandGateList;
	while ( pCur )
	{
		if ( dwGateID == pCur->GetGateID() )	return pCur;
		pCur = pCur->m_pNext;
	}

	return NULL;
}

void DxLandGateMan::BuildTree ()
{
	COLLISION::MakeAABBTree ( m_pLandGateTree, m_pLandGateList );
}

void DxLandGateMan::SetObjRotate90()
{
	PDXLANDGATE pCur = m_pLandGateList;
	while( pCur )
	{
		pCur->SetObjRotate90();
		pCur = pCur->m_pNext;
	}
	COLLISION::MakeAABBTree( m_pLandGateTree, m_pLandGateList );
}

PDXLANDGATE DxLandGateMan::DetectGate ( const D3DXVECTOR3 &vPos )
{
	if ( !m_pLandGateTree )		return NULL;

	PDXLANDGATE pLandGate = (PDXLANDGATE) COLLISION::IsCollisonPointToOBJAABBTree ( m_pLandGateTree, vPos );

	if ( pLandGate )	return pLandGate;
	return NULL;
}

void DxLandGateMan::CleanUp ()
{
	m_dwNumLandGate = 0;
	SAFE_DELETE(m_pLandGateList);
	SAFE_DELETE(m_pLandGateTree);
}

HRESULT DxLandGateMan::Render ( LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwRenderColor /*= 0xffffffff*/ )
{
	PDXLANDGATE pCur = m_pLandGateList;
	while ( pCur )
	{
		pCur->Render ( pd3dDevice, dwRenderColor );
		pCur = pCur->m_pNext;
	}

	return S_OK;
}

VOID DxLandGateMan::SaveCsvHead( std::fstream &SFile )
{
	SFile << "Gate Name" << ",";
	SFile << "Gate Info" << ",";
	SFile << "Gate ID" << ",";
	SFile << "Type" << ",";

	SFile << "SizeA" << ",";
	SFile << "SizeB" << ",";


	/* multi gate out, Juver, 2020/11/16 */
	for ( DWORD i=0; i<DxLandGate::MAX_GATE_OUT; ++i )
	{
		SFile << "ToMapMID " << i << ",";
		SFile << "ToMapSID " << i << ",";
		SFile << "ToGate " << i << ",";
		SFile << "NameINDEX " << i << ",";
		SFile << "NameKEY " << i << ",";
	}

	SFile << "dwToMapNum" << ",";
	SFile << "bNewSystem" << ",";

}

VOID DxLandGateMan::SaveCsv( std::fstream &SFile )
{
	PDXLANDGATE pCur = m_pLandGateList;
	while ( pCur )
	{
		SFile << pCur->GetName() << ",";
		SFile << pCur->GetInfo() << ",";
		SFile << pCur->GetGateID() << ",";

		SFile << pCur->GetFlags() << ",";

		SFile << pCur->GetDiv().x << ",";
		SFile << pCur->GetDiv().y << ",";

		/* multi gate out, Juver, 2020/11/16 */
		for ( DWORD i=0; i<DxLandGate::MAX_GATE_OUT; ++i )
		{
			SNATIVEID sMapID = pCur->GetToMapID(i);
			DWORD dwGateID = pCur->GetToGateID(i);
			DWORD dwNameINDEX = pCur->GetNameINDEX(i);
			std::string strNameKEY = pCur->GetNameKEY(i);

			SFile << sMapID.wMainID << ",";
			SFile << sMapID.wSubID << ",";
			SFile << dwGateID << ",";
			SFile << dwNameINDEX << ",";
			SFile << strNameKEY << ",";
		}

		SFile << pCur->GetToMapNum() << ",";
		SFile << pCur->GetNewSystem() << ",";


		SFile << std::endl;

		pCur = pCur->m_pNext;
	};
}

VOID DxLandGateMan::LoadCsv( CStringArray &StrArray )
{
	PDXLANDGATE pCur = new DxLandGate;

	int			iCsvCur = 0;
	DWORD	    dwFlags;
	DWORD	    dwGateID;
	D3DXVECTOR2 vDiv;

	pCur->SetName( StrArray[ iCsvCur++ ] );
	pCur->SetInfo( StrArray[ iCsvCur++ ] );

	dwGateID = (DWORD)atoi( StrArray[ iCsvCur++ ] );
	dwFlags  = (DWORD)atoi( StrArray[ iCsvCur++ ] );

	vDiv.x = (float)atof( StrArray[ iCsvCur++ ] );
	vDiv.y = (float)atof( StrArray[ iCsvCur++ ] );

	pCur->SetGate( dwFlags, dwGateID, vDiv );

	/* multi gate out, Juver, 2020/11/16 */
	for ( DWORD i=0; i<DxLandGate::MAX_GATE_OUT; ++i )
	{
		SNATIVEID sMapID(false);
		DWORD dwGateID(0);
		DWORD dwNameINDEX(0);
		std::string strNameKEY("");

		sMapID.wMainID = (WORD)atoi( StrArray[ iCsvCur++ ] );
		sMapID.wSubID  = (WORD)atoi( StrArray[ iCsvCur++ ] );
		dwGateID = (DWORD)atoi( StrArray[ iCsvCur++ ] );
		dwNameINDEX = (DWORD)atoi( StrArray[ iCsvCur++ ] );
		strNameKEY = std::string( StrArray[ iCsvCur++ ] );

		pCur->SetToMapID( i, sMapID );
		pCur->SetToGateID( i, dwGateID );
		pCur->SetNameINDEX( i, dwNameINDEX );
		pCur->SetNameKEY( i, strNameKEY.c_str() );
	}

	DWORD dwToMapNum = (DWORD)atoi( StrArray[ iCsvCur++ ] );
	BOOL bNewSystem = (BOOL)atoi( StrArray[ iCsvCur++ ] );
	pCur->SetToMapNum(dwToMapNum);
	pCur->SetNewSystem(bNewSystem);

	AddLandGate ( pCur );
}

HRESULT DxLandGateMan::Load_107 ( basestream &SFile, DWORD dwGroupNum  )
{
	CleanUp ();

	for( DWORD i = 0; i<dwGroupNum; ++i )
	{

		BOOL bExist = FALSE;
		SFile >> bExist;
		while ( bExist )
		{
			PDXLANDGATE pCur = new DxLandGate;
			pCur->Load ( SFile );

			AddLandGate ( pCur );

			SFile >> bExist;
		}
	}

	BuildTree ();

	return S_OK;
}

HRESULT DxLandGateMan::Load ( basestream &SFile )
{
	BOOL bExist = FALSE;
	CleanUp ();

	SFile >> bExist;
	while ( bExist )
	{
		PDXLANDGATE pCur = new DxLandGate;
		pCur->Load ( SFile );
	
		AddLandGate ( pCur );

		SFile >> bExist;
	}

	BuildTree ();

	return S_OK;
}

HRESULT DxLandGateMan::Save ( CSerialFile	&SFile )
{
	PDXLANDGATE pCur = m_pLandGateList;
	while ( pCur )
	{
		SFile << TRUE;
		pCur->Save ( SFile );

		pCur = pCur->m_pNext;
	};
	
	SFile << FALSE;

	return S_OK;
}

DxLandGateMan& DxLandGateMan::operator= ( DxLandGateMan &LandGateMan )
{
	CleanUp ();

	PDXLANDGATE pCur = LandGateMan.m_pLandGateList;
	while ( pCur )
	{
		DxLandGate *pNew = new DxLandGate;
		*pNew = *pCur;
		AddLandGate ( pNew );

		pCur = pCur->m_pNext;
	}

	BuildTree();

	return *this;
}

