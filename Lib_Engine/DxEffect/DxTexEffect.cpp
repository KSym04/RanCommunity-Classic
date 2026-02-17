#include "stdafx.h"
#include "DxTexEffect.h"

#include "../Common/StlFunctions.h"

#include "./DxTexEff.h"
#include "./DxTexEffMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxTexEffect::DxTexEffect() 
{
}

DxTexEffect::~DxTexEffect()
{
	CleanUp();
}

void DxTexEffect::CleanUp()
{
	std::for_each ( m_mapTextEff.begin(), m_mapTextEff.end(), std_afunc::DeleteMapObject() );
	m_mapTextEff.clear();
}