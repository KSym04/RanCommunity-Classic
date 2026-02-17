#include "stdafx.h"
#include "./WorldToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CWorldToolView::ResetMouseEdit()
{

	m_bEDITMRS = FALSE;

	m_DxEditMRS.SetMatrix( NULL );

}

void CWorldToolView::EditMRSSet( BOOL bENABLE )
{
	ResetMouseEdit();
	m_bEDITMRS = bENABLE;
}