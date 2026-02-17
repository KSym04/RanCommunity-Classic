#ifndef DXLIGHT_H_
#define DXLIGHT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class	CSerialFile;

#define STRING_LENGTH_256	256

class DXLIGHT
{
public:
	enum { VERSION_DXLIGHT = 0x0200 };

public:
	char			m_szLightName[STRING_LENGTH_256];
	BOOL			m_bEnable;
	D3DCOLORVALUE	m_BaseDiffuse;
	D3DXMATRIX		m_matWorld;

	D3DLIGHTQ		m_Light;

	BOOL			m_bEditCheck;

	int			unknown_var1;
	int			unknown_var2;
	int			unknown_var3;
	BOOL		unknown_var4;
	float		unknown_var5;

	//	Note : 단방향 리스트.
	DXLIGHT			*pNext;

public:
	DXLIGHT ()
		: m_bEnable(TRUE)
		, m_bEditCheck(FALSE)
		, pNext(NULL)

		, unknown_var1(100)
		, unknown_var2(0)
		, unknown_var3(0)
		, unknown_var4(FALSE)
		, unknown_var5(0.0f)

	  {
		  memset( m_szLightName, 0, sizeof(char)*STRING_LENGTH_256 );
		  D3DXMatrixIdentity( &m_matWorld );
	  }

	  virtual ~DXLIGHT ()
	  {
		  SAFE_DELETE(pNext);
	  }

	  void SetDefault ()
	  {
		  StringCchCopy( m_szLightName, STRING_LENGTH_256, "UNNAMED" );

		  SecureZeroMemory( &m_Light, sizeof(D3DLIGHTQ) );
		  m_Light.Type        = D3DLIGHT_DIRECTIONAL;
		  m_Light.Diffuse.r   = 1.f;
		  m_Light.Diffuse.g   = 1.f;
		  m_Light.Diffuse.b   = 1.f;
		  m_Light.Ambient.r	= 0.4f;
		  m_Light.Ambient.g	= 0.4f;
		  m_Light.Ambient.b	= 0.4f;
		  m_Light.Specular.r   = 1.0f;
		  m_Light.Specular.g   = 1.0f;
		  m_Light.Specular.b   = 1.0f;

		  D3DXVECTOR3 vDirection = D3DXVECTOR3(-1.0f, -2.0f, 2.0f);
		  D3DXVec3Normalize ( &vDirection, &vDirection );
		  m_Light.Direction = vDirection;

		  m_Light.Range		= 80.0f;

		  m_Light.Attenuation0 = 1.0f;
		  m_Light.Attenuation1 = 0.0f;
		  m_Light.Attenuation2 = 0.0f;

		  //m_Ambient.a = 0.0f;
		  m_BaseDiffuse.r = 1.0f;
		  m_BaseDiffuse.g = 1.0f;
		  m_BaseDiffuse.b = 1.0f;

		  unknown_var1 = 100;
		  unknown_var2 = 0;
		  unknown_var3 = 0;
		  unknown_var4 = FALSE;
		  unknown_var5 = 1.0f;
	  }

	  void operator= ( const DXLIGHT& Light )
	  {
		  StringCchCopy( m_szLightName, STRING_LENGTH_256, Light.m_szLightName );
		  m_bEnable			= Light.m_bEnable;
		  m_BaseDiffuse		= Light.m_BaseDiffuse;
		  m_Light			= Light.m_Light;
		  m_matWorld		= Light.m_matWorld;

		  unknown_var1 		= Light.unknown_var1;
		  unknown_var2 		= Light.unknown_var2;
		  unknown_var3 		= Light.unknown_var3;
		  unknown_var4		= Light.unknown_var4;
		  unknown_var5		= Light.unknown_var5;

		  // pNext 는 대입 연산에서 제외된다.
	  }

	  void SetRange ( float fRange );
	  virtual void SerialData ();

	  HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	//	<--	데이타 Save/Load
	void	SaveSet ( CSerialFile &SFile );
	void	LoadSet ( CSerialFile &SFile );
	void	LoadSet_Ver106( CSerialFile &SFile );	//106 107 108
	void	LoadSet_Ver103( CSerialFile &SFile );	//103 104 105
	void	LoadSet_Ver101( CSerialFile &SFile );	//101 102
	void	LoadSet_Ver100( CSerialFile &SFile );
	
	//	-->	데이타 Save/Load
};

typedef DXLIGHT* PDXLIGHT;

#endif // DXLIGHT_H_