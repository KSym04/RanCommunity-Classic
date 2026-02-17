
#pragma	once

#include "../../Lib_Engine/GUInterface/UIGroup.h"
#include "../../Lib_Engine/G-Logic/GLDefine.h"

class	CBasicVarTextBoxExIcon : public CUIGroup
{
public:
	CBasicVarTextBoxExIcon ();
	virtual	~CBasicVarTextBoxExIcon ();

public:
	void	CreateSubControl ();

public:
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
    void	SetIcon( SNATIVEID sICONINDEX, const char* szTexture );
	void	ResetIcon();

private:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;

private:
	SNATIVEID	m_sICONINDEX;
	CString		m_strTextureName;

private:
	CUIControl*		m_pIcon;
};