// WorldToolView.h : interface of the CWorldToolView class
//

#include "../Lib_Engine/dxframe/D3DApp.h"

#pragma once

#include "../Lib_Engine/DxOctree/DxLandMan.h"
#include "../Lib_Engine/DxEffect/DxPieceContainer.h"

#include "../Lib_Engine/DxCommon/DxObjectMRS.h"

class CWorldToolView : public CView, public CD3DApplication
{
protected: // create from serialization only
	CWorldToolView();
	DECLARE_DYNCREATE(CWorldToolView)

// Attributes
public:
	//CWorldToolDoc* GetDocument() const;
	static	CWorldToolView * GetView();

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CWorldToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

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

public:
	D3DXMATRIX		m_matLocal;
	DxLandMan*		m_pLand;
	DxPieceEdit*	m_pPiece;
	std::string		m_strFileName;

	BOOL	m_bDebugInfo;
	BOOL	m_bShowSphere;
	BOOL	m_bShadowWireFrame;
	D3DXVECTOR3		m_vPos;

	BOOL	m_bRenderEffectAABB;
	BOOL	m_bRenderEffectName;

	DxObjectMRS		m_DxEditMRS;

	BOOL			m_bEDITMRS;

public:
	void ResetMouseEdit();
	void EditMRSSet( BOOL bENABLE );

public:
	void	DataCleanUp();
	void	ResetEditor();

	BOOL	WLDLoad( std::string strFile );
	BOOL	WLDSave( std::string strFile );
	BOOL	PIELoad( std::string strFile );
	BOOL	PIESave( std::string strFile );

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);	

	afx_msg void OnRenderDebuginfo();
	afx_msg void OnUpdateRenderDebuginfo(CCmdUI *pCmdUI);
	afx_msg void OnCameraSpeed500();
	afx_msg void OnUpdateCameraSpeed500(CCmdUI *pCmdUI);
	afx_msg void OnCameraSpeed1000();
	afx_msg void OnUpdateCameraSpeed1000(CCmdUI *pCmdUI);
	afx_msg void OnCameraSpeed1500();
	afx_msg void OnUpdateCameraSpeed1500(CCmdUI *pCmdUI);
	afx_msg void OnCameraSpeeddefault();
	afx_msg void OnUpdateCameraSpeeddefault(CCmdUI *pCmdUI);
	afx_msg void OnCameraResetposition();
	afx_msg void OnFograngeRange5k();
	afx_msg void OnUpdateFograngeRange5k(CCmdUI *pCmdUI);
	afx_msg void OnFograngeRange10k();
	afx_msg void OnUpdateFograngeRange10k(CCmdUI *pCmdUI);
	afx_msg void OnFograngeRange20k();
	afx_msg void OnUpdateFograngeRange20k(CCmdUI *pCmdUI);
	afx_msg void OnFograngeRange50k();
	afx_msg void OnUpdateFograngeRange50k(CCmdUI *pCmdUI);
	afx_msg void OnFograngeRangedefault();
	afx_msg void OnUpdateFograngeRangedefault(CCmdUI *pCmdUI);
	afx_msg void OnFograngeRangedefaultmid();
	afx_msg void OnUpdateFograngeRangedefaultmid(CCmdUI *pCmdUI);
	afx_msg void OnFograngeRange();
	afx_msg void OnUpdateFograngeRange(CCmdUI *pCmdUI);
	afx_msg void OnSkyBluetime();
	afx_msg void OnSkyRedtime();
	afx_msg void OnSkyNoontime();
	afx_msg void OnSkyNighttime();
	afx_msg void OnSkyFadeout();
	afx_msg void OnSkyFadein();
	afx_msg void OnWeatherWeatherrain();
	afx_msg void OnWeatherWeathersnow();
	afx_msg void OnWeatherWeatherleaves();
	afx_msg void OnWeatherWeatherspore();
	afx_msg void OnLightmanPointlightrender();
	afx_msg void OnUpdateLightmanPointlightrender(CCmdUI *pCmdUI);
	afx_msg void OnLightmanLightnamerender();
	afx_msg void OnUpdateLightmanLightnamerender(CCmdUI *pCmdUI);

	afx_msg void OnRenderSoundmanrangerender();
	afx_msg void OnUpdateRenderSoundmanrangerender(CCmdUI *pCmdUI);
	afx_msg void OnRenderSoundmannamerender();
	afx_msg void OnUpdateRenderSoundmannamerender(CCmdUI *pCmdUI);
	afx_msg void OnRenderShowpositionsphere();
	afx_msg void OnUpdateRenderShowpositionsphere(CCmdUI *pCmdUI);
	afx_msg void OnRenderRendershadowwireframe();
	afx_msg void OnUpdateRenderRendershadowwireframe(CCmdUI *pCmdUI);
	afx_msg void OnRenderPieceusecollision();
	afx_msg void OnUpdateRenderPieceusecollision(CCmdUI *pCmdUI);
	afx_msg void OnRenderPieceusedistance();
	afx_msg void OnUpdateRenderPieceusedistance(CCmdUI *pCmdUI);

	afx_msg void OnRenderRendereffectaabb();
	afx_msg void OnUpdateRenderRendereffectaabb(CCmdUI *pCmdUI);
	afx_msg void OnRenderRendereffectname();
	afx_msg void OnUpdateRenderRendereffectname(CCmdUI *pCmdUI);
	afx_msg void OnEditEditmatrixmove();
	afx_msg void OnUpdateEditEditmatrixmove(CCmdUI *pCmdUI);
	afx_msg void OnEditEditmatrixrotate();
	afx_msg void OnUpdateEditEditmatrixrotate(CCmdUI *pCmdUI);
	afx_msg void OnEditEditmatrixscale();
	afx_msg void OnUpdateEditEditmatrixscale(CCmdUI *pCmdUI);
};

//#ifndef _DEBUG  // debug version in WorldToolView.cpp
//inline CWorldToolDoc* CWorldToolView::GetDocument() const
//   { return reinterpret_cast<CWorldToolDoc*>(m_pDocument); }
//#endif

