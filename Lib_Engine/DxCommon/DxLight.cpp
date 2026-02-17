#include "stdafx.h"
#include "DxLight.h"

#include "../Common/SerialFile.h"
#include "./DxMethods.h"
#include "./EditMeshs.h"
#include "./DxLightMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static void ColorUp( D3DCOLORVALUE& d3dColor, float fValue )
{
	float fDiffR = d3dColor.r;
	float fDiffG = d3dColor.g;
	float fDiffB = d3dColor.b;

	fDiffR *= fValue;
	fDiffG *= fValue;
	fDiffB *= fValue;

	d3dColor.r = d3dColor.r + fDiffR;
	d3dColor.g = d3dColor.g + fDiffG;
	d3dColor.b = d3dColor.b + fDiffB;

	if( d3dColor.r > 1.f )		d3dColor.r = 1.f;
	if( d3dColor.g > 1.f )		d3dColor.g = 1.f;
	if( d3dColor.b > 1.f )		d3dColor.b = 1.f;
}


void DXLIGHT::SetRange ( float fRange )
{
	SerialData ();
}

void DXLIGHT::SerialData ()
{
	m_Light.Position.x = m_matWorld._41;
	m_Light.Position.y = m_matWorld._42;
	m_Light.Position.z = m_matWorld._43;

	//	Note : 위치 수성시에 AABB-Tree 또한 재정렬 한다.
	//
	DxLightMan::GetInstance()->MakeAABBTree ();
}

HRESULT DXLIGHT::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	//	-----------------------------[테스트용]--------------------------------
	//
	EDITMESHS::RENDERSPHERE ( pd3dDevice, m_Light.Position, m_Light.Range );
	return S_OK;
}

void	DXLIGHT::SaveSet ( CSerialFile &SFile )
{
	SFile << (DWORD)VERSION_DXLIGHT;
	SFile << (DWORD)sizeof(DXLIGHT);

	SFile.WriteBuffer ( m_szLightName, sizeof ( char ) * 256 );
	SFile.WriteBuffer( m_matWorld, sizeof( D3DXMATRIX ) );
	
	SFile << m_bEnable;
	SFile << m_BaseDiffuse;

	SFile << unknown_var1;
	SFile << unknown_var2;
	SFile << unknown_var3;
	SFile << unknown_var4;
	SFile << unknown_var5;

	m_Light.Diffuse = m_BaseDiffuse;	// 기본값 저장

	if ( m_Light.Type == D3DLIGHT_POINT )	// 엠뷰언트 값 저장
	{
		m_Light.Ambient.r = 0.f;
		m_Light.Ambient.g = 0.f;
		m_Light.Ambient.b = 0.f;
	}

	SFile.WriteBuffer ( &m_Light, sizeof ( D3DLIGHTQ ) );

	if ( pNext )
	{
		SFile << (BOOL)TRUE;
		pNext->SaveSet ( SFile );
	}
	else
	{
		SFile << (BOOL)FALSE;
	}
}

void	DXLIGHT::LoadSet( CSerialFile &SFile )
{
	DWORD dwVER(0);
	DWORD dwSIZE(0);

	SFile >> dwVER;
	SFile >> dwSIZE;

	if( dwVER == VERSION_DXLIGHT )
	{
		SFile.ReadBuffer ( m_szLightName, sizeof ( char ) * 256 );
		SFile.ReadBuffer( &m_matWorld, sizeof( D3DXMATRIX ) );
		
		SFile >> m_bEnable;
		SFile >> m_BaseDiffuse;

		SFile >> unknown_var1;
		SFile >> unknown_var2;
		SFile >> unknown_var3;
		SFile >> unknown_var4;
		SFile >> unknown_var5;

		SFile.ReadBuffer ( &m_Light, sizeof ( D3DLIGHTQ ) );

		m_Light.Attenuation0 = float(unknown_var1)*0.009f;
		m_Light.Attenuation1 = float(unknown_var2)*0.009f/(m_Light.Range*0.5f);		// 최대거리일 경우, (값 = 색 / 1.5f)
		m_Light.Attenuation2 = float(unknown_var3)*0.009f/((m_Light.Range*m_Light.Range)*0.5f);	

		if ( (m_BaseDiffuse.a != m_Light.Diffuse.a) || (m_BaseDiffuse.r != m_Light.Diffuse.r) ||
			(m_BaseDiffuse.g != m_Light.Diffuse.g) || (m_BaseDiffuse.b != m_Light.Diffuse.b))
		{
			m_Light.Ambient	= m_BaseDiffuse;
			m_BaseDiffuse	= m_Light.Diffuse;
		}

		float	fSpecular;
		fSpecular = m_Light.Diffuse.r;
		if ( fSpecular >= 1.f )		m_Light.Specular.r = 1.f;
		else						m_Light.Specular.r = fSpecular;

		fSpecular = m_Light.Diffuse.g;
		if ( fSpecular >= 1.f )		m_Light.Specular.g = 1.f;
		else						m_Light.Specular.g = fSpecular;

		fSpecular = m_Light.Diffuse.b;
		if ( fSpecular >= 1.f )		m_Light.Specular.b = 1.f;
		else						m_Light.Specular.b = fSpecular;

		BOOL	bResult = FALSE;
		SFile >> bResult;
		if ( bResult )
		{		
			pNext = new DXLIGHT();
			pNext->LoadSet ( SFile );	
		}
	}
	else if( dwVER == 0x0102 )
	{
		SFile.ReadBuffer( &m_matWorld, sizeof( D3DXMATRIX ) );
		SFile.ReadBuffer ( m_szLightName, sizeof ( char ) * 256 );

		SFile >> unknown_var4;
		SFile >> m_bEnable;
		SFile >> m_BaseDiffuse;
		SFile >> unknown_var1;
		SFile >> unknown_var2;
		SFile >> unknown_var3;
		SFile >> unknown_var5;

		SFile.ReadBuffer ( &m_Light, sizeof ( D3DLIGHTQ ) );

		m_Light.Attenuation0 = float(unknown_var1)*0.009f;
		m_Light.Attenuation1 = float(unknown_var2)*0.009f/(m_Light.Range*0.5f);		// 최대거리일 경우, (값 = 색 / 1.5f)
		m_Light.Attenuation2 = float(unknown_var3)*0.009f/((m_Light.Range*m_Light.Range)*0.5f);	

		if ( (m_BaseDiffuse.a != m_Light.Diffuse.a) || (m_BaseDiffuse.r != m_Light.Diffuse.r) ||
			(m_BaseDiffuse.g != m_Light.Diffuse.g) || (m_BaseDiffuse.b != m_Light.Diffuse.b))
		{
			m_Light.Ambient	= m_BaseDiffuse;
			m_BaseDiffuse	= m_Light.Diffuse;
		}

		float	fSpecular;
		fSpecular = m_Light.Diffuse.r;
		if ( fSpecular >= 1.f )		m_Light.Specular.r = 1.f;
		else						m_Light.Specular.r = fSpecular;

		fSpecular = m_Light.Diffuse.g;
		if ( fSpecular >= 1.f )		m_Light.Specular.g = 1.f;
		else						m_Light.Specular.g = fSpecular;

		fSpecular = m_Light.Diffuse.b;
		if ( fSpecular >= 1.f )		m_Light.Specular.b = 1.f;
		else						m_Light.Specular.b = fSpecular;
	}
	else if( dwVER == 0x0101 )
	{
		SFile.ReadBuffer( &m_matWorld, sizeof( D3DXMATRIX ) );
		SFile.ReadBuffer ( m_szLightName, sizeof ( char ) * 256 );

		SFile >> unknown_var4;
		SFile >> m_bEnable;
		SFile >> m_BaseDiffuse;
		SFile >> unknown_var1;
		SFile >> unknown_var2;
		SFile >> unknown_var3;

		SFile.ReadBuffer ( &m_Light, sizeof ( D3DLIGHTQ ) );

		m_Light.Attenuation0 = float(unknown_var1)*0.009f;
		m_Light.Attenuation1 = float(unknown_var2)*0.009f/(m_Light.Range*0.5f);		// 최대거리일 경우, (값 = 색 / 1.5f)
		m_Light.Attenuation2 = float(unknown_var3)*0.009f/((m_Light.Range*m_Light.Range)*0.5f);	

		if ( (m_BaseDiffuse.a != m_Light.Diffuse.a) || (m_BaseDiffuse.r != m_Light.Diffuse.r) ||
			(m_BaseDiffuse.g != m_Light.Diffuse.g) || (m_BaseDiffuse.b != m_Light.Diffuse.b))
		{
			m_Light.Ambient	= m_BaseDiffuse;
			m_BaseDiffuse	= m_Light.Diffuse;
		}

		float	fSpecular;
		fSpecular = m_Light.Diffuse.r;
		if ( fSpecular >= 1.f )		m_Light.Specular.r = 1.f;
		else						m_Light.Specular.r = fSpecular;

		fSpecular = m_Light.Diffuse.g;
		if ( fSpecular >= 1.f )		m_Light.Specular.g = 1.f;
		else						m_Light.Specular.g = fSpecular;

		fSpecular = m_Light.Diffuse.b;
		if ( fSpecular >= 1.f )		m_Light.Specular.b = 1.f;
		else						m_Light.Specular.b = fSpecular;
	}
	else if( dwVER == 0x0100 )
	{
		SFile.ReadBuffer( &m_matWorld, sizeof( D3DXMATRIX ) );
		SFile.ReadBuffer ( m_szLightName, sizeof ( char ) * 256 );
		
		SFile >> unknown_var4;
		SFile >> m_bEnable;
		SFile >> m_BaseDiffuse;
		SFile >> unknown_var1;
		SFile >> unknown_var2;
		SFile >> unknown_var3;

		SFile.ReadBuffer ( &m_Light, sizeof ( D3DLIGHTQ ) );

		m_Light.Attenuation0 = float(unknown_var1)*0.009f;
		m_Light.Attenuation1 = float(unknown_var2)*0.009f/(m_Light.Range*0.5f);		// 최대거리일 경우, (값 = 색 / 1.5f)
		m_Light.Attenuation2 = float(unknown_var3)*0.009f/((m_Light.Range*m_Light.Range)*0.5f);	

		if ( (m_BaseDiffuse.a != m_Light.Diffuse.a) || (m_BaseDiffuse.r != m_Light.Diffuse.r) ||
			(m_BaseDiffuse.g != m_Light.Diffuse.g) || (m_BaseDiffuse.b != m_Light.Diffuse.b))
		{
			m_Light.Ambient	= m_BaseDiffuse;
			m_BaseDiffuse	= m_Light.Diffuse;
		}

		float	fSpecular;
		fSpecular = m_Light.Diffuse.r;
		if ( fSpecular >= 1.f )		m_Light.Specular.r = 1.f;
		else						m_Light.Specular.r = fSpecular;

		fSpecular = m_Light.Diffuse.g;
		if ( fSpecular >= 1.f )		m_Light.Specular.g = 1.f;
		else						m_Light.Specular.g = fSpecular;

		fSpecular = m_Light.Diffuse.b;
		if ( fSpecular >= 1.f )		m_Light.Specular.b = 1.f;
		else						m_Light.Specular.b = fSpecular;

		BOOL	bResult = FALSE;
		SFile >> bResult;
		if ( bResult )
		{		
			pNext = new DXLIGHT();
			pNext->LoadSet ( SFile );	
		}
	}
	else
	{
		CDebugSet::ErrorVersion( "DXLIGHT::LoadSet", SFile.GetFileName(), dwVER );
		DWORD dwCur = SFile.GetfTell ();
		SFile.SetOffSet ( dwCur+dwSIZE );
	}		
}

void	DXLIGHT::LoadSet_Ver106( CSerialFile &SFile )
{
	BOOL	bResult = FALSE;

	SFile.ReadBuffer( &m_matWorld, sizeof( D3DXMATRIX ) );

	SFile.ReadBuffer ( m_szLightName, sizeof ( char ) * 256 );
	SFile >> m_bEnable;
	unknown_var4 = m_bEnable;

	SFile >> m_BaseDiffuse;

	SFile >> unknown_var1;
	SFile >> unknown_var2;
	SFile >> unknown_var3;

	SFile.ReadBuffer ( &m_Light, sizeof ( D3DLIGHTQ ) );

	m_Light.Attenuation0 = float(unknown_var1)*0.009f;
	m_Light.Attenuation1 = float(unknown_var2)*0.009f/(m_Light.Range*0.5f);		// 최대거리일 경우, (값 = 색 / 1.5f)
	m_Light.Attenuation2 = float(unknown_var3)*0.009f/((m_Light.Range*m_Light.Range)*0.5f);	

	if ( (m_BaseDiffuse.a != m_Light.Diffuse.a) || (m_BaseDiffuse.r != m_Light.Diffuse.r) ||
		(m_BaseDiffuse.g != m_Light.Diffuse.g) || (m_BaseDiffuse.b != m_Light.Diffuse.b))
	{
		m_Light.Ambient	= m_BaseDiffuse;
		m_BaseDiffuse	= m_Light.Diffuse;
	}

	float	fSpecular;
	fSpecular = m_Light.Diffuse.r;
	if ( fSpecular >= 1.f )		m_Light.Specular.r = 1.f;
	else						m_Light.Specular.r = fSpecular;

	fSpecular = m_Light.Diffuse.g;
	if ( fSpecular >= 1.f )		m_Light.Specular.g = 1.f;
	else						m_Light.Specular.g = fSpecular;

	fSpecular = m_Light.Diffuse.b;
	if ( fSpecular >= 1.f )		m_Light.Specular.b = 1.f;
	else						m_Light.Specular.b = fSpecular;

	SFile >> bResult;
	if ( bResult )
	{		
		pNext = new DXLIGHT();
		pNext->LoadSet_Ver106 ( SFile );	
	}
}

void	DXLIGHT::LoadSet_Ver103( CSerialFile &SFile )
{
	BOOL	bResult = FALSE;

	SFile.ReadBuffer( &m_matWorld, sizeof( D3DXMATRIX ) );

	SFile.ReadBuffer ( m_szLightName, sizeof ( char ) * 256 );
	SFile >> m_bEnable;
	unknown_var4 = m_bEnable;

	SFile >> m_BaseDiffuse;

	SFile.ReadBuffer ( &m_Light, sizeof ( D3DLIGHTQ ) );

	if ( (m_BaseDiffuse.a != m_Light.Diffuse.a) || (m_BaseDiffuse.r != m_Light.Diffuse.r) ||
		(m_BaseDiffuse.g != m_Light.Diffuse.g) || (m_BaseDiffuse.b != m_Light.Diffuse.b))
	{
		m_Light.Ambient	= m_BaseDiffuse;
		m_BaseDiffuse	= m_Light.Diffuse;
	}

	m_Light.Attenuation0 = 1.0f;
	m_Light.Attenuation1 = 1.0f/m_Light.Range;		// 최대거리일 경우, (값 = 색 / 1.5f)
	m_Light.Attenuation2 = 0.0f;

	float	fSpecular;
	fSpecular = m_Light.Diffuse.r;
	if ( fSpecular >= 1.f )		m_Light.Specular.r = 1.f;
	else						m_Light.Specular.r = fSpecular;

	fSpecular = m_Light.Diffuse.g;
	if ( fSpecular >= 1.f )		m_Light.Specular.g = 1.f;
	else						m_Light.Specular.g = fSpecular;

	fSpecular = m_Light.Diffuse.b;
	if ( fSpecular >= 1.f )		m_Light.Specular.b = 1.f;
	else						m_Light.Specular.b = fSpecular;

	SFile >> bResult;
	if ( bResult )
	{		
		pNext = new DXLIGHT();
		pNext->LoadSet_Ver103 ( SFile );	
	}
}

void	DXLIGHT::LoadSet_Ver101( CSerialFile &SFile )
{
	BOOL	bResult = FALSE;

	SFile.ReadBuffer( &m_matWorld, sizeof( D3DXMATRIX ) );

	SFile.ReadBuffer ( m_szLightName, sizeof ( char ) * 256 );
	SFile >> m_bEnable;
	unknown_var4 = m_bEnable;

	SFile >> m_BaseDiffuse;

	SFile.ReadBuffer ( &m_Light, sizeof ( D3DLIGHTQ ) );

	if ( (m_BaseDiffuse.a != m_Light.Diffuse.a) || (m_BaseDiffuse.r != m_Light.Diffuse.r) ||
		(m_BaseDiffuse.g != m_Light.Diffuse.g) || (m_BaseDiffuse.b != m_Light.Diffuse.b))
	{
		m_Light.Ambient	= m_BaseDiffuse;
		m_BaseDiffuse	= m_Light.Diffuse;
	}

	if ( m_Light.Type == D3DLIGHT_POINT )
	{
		ColorUp( m_Light.Diffuse, 0.6f );			// MODULATE2X -> MODULATE 로 바뀌면서 보정작업
		m_Light.Ambient.r = 0.f;
		m_Light.Ambient.g = 0.f;
		m_Light.Ambient.b = 0.f;
	}

	m_Light.Attenuation0 = 1.0f;
	m_Light.Attenuation1 = 1.0f/m_Light.Range;		// 최대거리일 경우, (값 = 색 / 1.5f)
	m_Light.Attenuation2 = 0.0f;

	float	fSpecular;
	fSpecular = m_Light.Diffuse.r;
	if ( fSpecular >= 1.f )		m_Light.Specular.r = 1.f;
	else						m_Light.Specular.r = fSpecular;

	fSpecular = m_Light.Diffuse.g;
	if ( fSpecular >= 1.f )		m_Light.Specular.g = 1.f;
	else						m_Light.Specular.g = fSpecular;

	fSpecular = m_Light.Diffuse.b;
	if ( fSpecular >= 1.f )		m_Light.Specular.b = 1.f;
	else						m_Light.Specular.b = fSpecular;

	SFile >> bResult;
	if ( bResult )
	{		
		pNext = new DXLIGHT();
		pNext->LoadSet_Ver101( SFile );	
	}
}

void	DXLIGHT::LoadSet_Ver100( CSerialFile &SFile )
{
	BOOL	bResult = FALSE;

	DXAFFINEPARTS m_sAffineParts;

	SFile >> bResult;
	if ( bResult )
	{
		SFile.ReadBuffer( &m_sAffineParts, sizeof ( DXAFFINEPARTS ) );
	}

	D3DXMatrixCompX( m_matWorld, m_sAffineParts );	// m_matWorld 만 사용하게 되어서 변환

	SFile.ReadBuffer ( m_szLightName, sizeof ( char ) * 256 );
	SFile >> m_bEnable;
	unknown_var4 = m_bEnable;

	SFile >> m_BaseDiffuse;

	SFile.ReadBuffer ( &m_Light, sizeof ( D3DLIGHTQ ) );

	if ( (m_BaseDiffuse.a != m_Light.Diffuse.a) || (m_BaseDiffuse.r != m_Light.Diffuse.r) ||
		(m_BaseDiffuse.g != m_Light.Diffuse.g) || (m_BaseDiffuse.b != m_Light.Diffuse.b))
	{
		m_Light.Ambient	= m_BaseDiffuse;
		m_BaseDiffuse	= m_Light.Diffuse;
	}

	if ( m_Light.Type == D3DLIGHT_POINT )
	{
		ColorUp( m_Light.Diffuse, 0.6f );			// MODULATE2X -> MODULATE 로 바뀌면서 보정작업
		m_Light.Ambient.r = 0.f;
		m_Light.Ambient.g = 0.f;
		m_Light.Ambient.b = 0.f;
	}

	m_Light.Attenuation0 = 1.0f;
	m_Light.Attenuation1 = 1.0f/m_Light.Range;		// 최대거리일 경우, (값 = 색 / 1.5f)
	m_Light.Attenuation2 = 0.0f;

	float	fSpecular;
	fSpecular = m_Light.Diffuse.r;
	if ( fSpecular >= 1.f )		m_Light.Specular.r = 1.f;
	else						m_Light.Specular.r = fSpecular;

	fSpecular = m_Light.Diffuse.g;
	if ( fSpecular >= 1.f )		m_Light.Specular.g = 1.f;
	else						m_Light.Specular.g = fSpecular;

	fSpecular = m_Light.Diffuse.b;
	if ( fSpecular >= 1.f )		m_Light.Specular.b = 1.f;
	else						m_Light.Specular.b = fSpecular;

	SFile >> bResult;
	if ( bResult )
	{		
		pNext = new DXLIGHT();
		pNext->LoadSet_Ver100( SFile );	
	}
}