// WorldToolDoc.h : interface of the CWorldToolDoc class
//


#pragma once

class CWorldToolDoc : public CDocument
{
protected: // create from serialization only
	CWorldToolDoc();
	DECLARE_DYNCREATE(CWorldToolDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CWorldToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


