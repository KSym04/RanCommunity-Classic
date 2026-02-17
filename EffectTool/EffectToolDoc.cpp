
// EffectToolDoc.cpp : implementation of the CEffectToolDoc class
//

#include "stdafx.h"
#include "EffectTool.h"

#include "EffectToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEffectToolDoc

IMPLEMENT_DYNCREATE(CEffectToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CEffectToolDoc, CDocument)
END_MESSAGE_MAP()


// CEffectToolDoc construction/destruction

CEffectToolDoc::CEffectToolDoc()
{
	// TODO: add one-time construction code here

}

CEffectToolDoc::~CEffectToolDoc()
{
}

BOOL CEffectToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CEffectToolDoc serialization

void CEffectToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CEffectToolDoc diagnostics

#ifdef _DEBUG
void CEffectToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEffectToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEffectToolDoc commands
