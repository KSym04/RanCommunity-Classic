
// EffectToolDoc.h : interface of the CEffectToolDoc class
//


#pragma once


class CEffectToolDoc : public CDocument
{
protected: // create from serialization only
	CEffectToolDoc();
	DECLARE_DYNCREATE(CEffectToolDoc)

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
	virtual ~CEffectToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


