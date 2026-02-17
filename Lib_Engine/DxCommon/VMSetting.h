/*!
 * \file VMSetting.h
 *
 * \author Juver
 * \date March 2018
 *
 * 
 */
#ifndef GLVMSETTING_H_
#define GLVMSETTING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct SVM_PROP_SETTING
{
	enum{ VMPROP_TEXTURE_NON_INDEX = 5 };

	std::string		strName;
	DWORD	dwVMType;

	BOOL	bWhenDayOffGlow;
	BOOL	bSpeedGlow;

	float	fScaleFlow;
	float	fAlphaFlow;
	float	fSpeedGlow;
	float	fAlphaGlow;
	float	fSpeedFlow;	
	float	fSpeedRotate;
	float	fSpeedFlowx;
	float	fSpeedFlowy;
	float	fDirectFlowx;
	float	fDirectFlowy;

	WORD	wTexSpec0;
	WORD	wTexSpec1;
	WORD	wAlphaTexFlow;
	WORD	wFlowTexFlow;
	WORD	wTexGlow;
	WORD	wTexRotate;

	SVM_PROP_SETTING()
		: strName("")
		, dwVMType(0)
		, bWhenDayOffGlow(FALSE)
		, bSpeedGlow(FALSE)
		, fScaleFlow(1.f)
		, fAlphaFlow(1.f)
		, fSpeedGlow(1.f)
		, fAlphaGlow(1.f)
		, fSpeedFlow(1.f)	
		, fSpeedRotate(1.f)
		, fSpeedFlowx(0.f)
		, fSpeedFlowy(1.f)
		, fDirectFlowx(0.f)
		, fDirectFlowy(1.f)
		, wTexSpec0(VMPROP_TEXTURE_NON_INDEX)
		, wTexSpec1(VMPROP_TEXTURE_NON_INDEX)
		, wAlphaTexFlow(VMPROP_TEXTURE_NON_INDEX)
		, wFlowTexFlow(VMPROP_TEXTURE_NON_INDEX)
		, wTexGlow(VMPROP_TEXTURE_NON_INDEX)
		, wTexRotate(VMPROP_TEXTURE_NON_INDEX)
	{

	}
};

typedef std::map<std::string,SVM_PROP_SETTING>	SVM_PROP_SETTING_MAP;
typedef SVM_PROP_SETTING_MAP::iterator			SVM_PROP_SETTING_MAP_ITER;


class GLVMSetting
{	
public:
	GLVMSetting();
	virtual ~GLVMSetting();

protected:
	std::string		m_strPATH;

public:
	SVM_PROP_SETTING_MAP	m_mapData;

public:
	void SetPath ( std::string strPATH )	{ m_strPATH = strPATH; }
	std::string GetPath ()					{ return m_strPATH.c_str(); }

	void CleanUp();
	void LoadFile();

public:
	SVM_PROP_SETTING*	GetProp( std::string strName );

public:
	static GLVMSetting& GetInstance ();
};


#endif // GLVMSETTING_H_