
// EffectToolView.h : interface of the CEffectToolView class
//
#include "../Lib_Engine/dxframe/D3DApp.h"

#include "../Lib_Engine/DxEffect/Single/DxEffSingle.h"

#pragma once


class CEffectToolView : public CView, public CD3DApplication
{
protected: // create from serialization only
	CEffectToolView();
	DECLARE_DYNCREATE(CEffectToolView)

// Attributes
public:
	//CEffectToolDoc* GetDocument() const;
	static	CEffectToolView * GetView();

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CEffectToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	D3DXMATRIX		m_matLocal;

	std::string			m_strFileName;
	DxEffSingleGroup*	m_pEffSingle;
	EFF_PROPGROUP*		m_pPropGroup;

public:
	BOOL	m_bRenderDebug;
	BOOL	m_bRenderPlane;
	BOOL	m_bEffectFrameMove;
	BOOL	m_bEffectRender;

public:
	void	DataReset();


public:
	HRESULT ReSizeWindow ( int cx, int cy )
	{
		if ( cx < 10 )	cx = 10;
		if ( cy < 10 )	cy = 10;

		m_d3dpp.BackBufferWidth  = cx;
		m_d3dpp.BackBufferHeight = cy;

		return Resize3DEnvironment();
	}

private:
	BOOL	m_bCreated;
	HCURSOR	m_hCursorDefault;

protected:
	virtual void PostNcDestroy();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

public:
	void SetActive ( BOOL bActive );

public:
	virtual void OnInitialUpdate();
	virtual HRESULT FrameMove3DEnvironment();
	virtual HRESULT Render3DEnvironment();
	HRESULT ConfirmDevice( D3DCAPSQ*, DWORD, D3DFORMAT );

	HRESULT OneTimeSceneInit();
	HRESULT CreateObjects();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT FrameMove();
	HRESULT Render();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT FinalCleanup();
	LPDIRECT3DDEVICEQ Get3DDevice()	{	return m_pd3dDevice;	}

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);	

	afx_msg void OnRenderDebug();
	afx_msg void OnUpdateRenderDebug(CCmdUI *pCmdUI);
	afx_msg void OnRenderPlane();
	afx_msg void OnUpdateRenderPlane(CCmdUI *pCmdUI);
	afx_msg void OnRenderRenderaabb();
	afx_msg void OnUpdateRenderRenderaabb(CCmdUI *pCmdUI);

	afx_msg void OnRenderResetcamera();
	afx_msg void OnButtonFileNew();
	afx_msg void OnButtonFileOpen();
	afx_msg void OnButtonFileOpen2();
	afx_msg void OnButtonFileSave();

	afx_msg void OnButtonEffectPlay();
	afx_msg void OnButtonEffectStop();
	afx_msg void OnButtonEffectPause();
};

//#ifndef _DEBUG  // debug version in EffectToolView.cpp
//inline CEffectToolDoc* CEffectToolView::GetDocument() const
//   { return reinterpret_cast<CEffectToolDoc*>(m_pDocument); }
//#endif

