#pragma once

class CMainFrame : public CFrameWnd
{

protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:

	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	CSplitterWnd m_wndSplitter;
	CToolBar toolbar;
	bool init;


	DECLARE_MESSAGE_MAP()
};

