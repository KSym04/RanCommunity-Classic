// WorldToolDoc.cpp : implementation of the CWorldToolDoc class
//

#include "stdafx.h"
#include "WorldTool.h"

#include "WorldToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWorldToolDoc

IMPLEMENT_DYNCREATE(CWorldToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CWorldToolDoc, CDocument)
END_MESSAGE_MAP()


// CWorldToolDoc construction/destruction

CWorldToolDoc::CWorldToolDoc()
{
	// TODO: add one-time construction code here

}

CWorldToolDoc::~CWorldToolDoc()
{
}

BOOL CWorldToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CWorldToolDoc serialization

void CWorldToolDoc::Serialize(CArchive& ar)
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


// CWorldToolDoc diagnostics

#ifdef _DEBUG
void CWorldToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWorldToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWorldToolDoc commands
