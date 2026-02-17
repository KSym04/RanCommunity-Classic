/*!
 * \file DxTexEffect.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#pragma once


class CSerialFile;

class DxTexEffBase;

typedef std::multimap<std::string,DxTexEffBase*>		MAPDXTEXEFF;
typedef MAPDXTEXEFF::iterator							MAPDXTEXEFF_ITER;
typedef std::pair<MAPDXTEXEFF_ITER,MAPDXTEXEFF_ITER>	MAPDXTEXEFF_RANGE;

class DxTexEffect
{
protected:
	enum
	{
		VERSION_TEX_EFFECT	= 0x0204,

		ENCODE_VER_BYTE_CRYPT_2021_WLD3_V1	= 0x0204,

		VERSION_ENCODE_4	= 0x0203,
		VERSION_ENCODE_3	= 0x0202,
		VERSION_ENCODE_2	= 0x0201,
		VERSION_ENCODE		= 0x0200,
	};

public:
	MAPDXTEXEFF		m_mapTextEff;

public:
	void CleanUp();

public:
	BOOL Load_File( LPDIRECT3DDEVICEQ pd3dDevice, const char* szName );
	void Load( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_Ver_0100( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_Ver_0200( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_Ver_0201( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_Ver_0202( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_Ver_0203( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );
	void Load_Ver_0204( LPDIRECT3DDEVICEQ pd3dDevice, CSerialFile& SFile );

	void Save_File( const char* szName );
	void Save( CSerialFile& SFile );

public:
	DxTexEffect();
	~DxTexEffect();
};

