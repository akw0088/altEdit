#pragma once

#include "Document.h"

class CViewPerspective : public CView
{
protected:
	CViewPerspective();
	DECLARE_DYNCREATE(CViewPerspective)
public:

protected:
	HGLRC hglrc_legacy;
	HGLRC hglrc;
	CDC* m_pDC;

public:
	BOOL SetupPixelFormat();
	BOOL InitOpenGL();

	void SetContext() { wglMakeCurrent(m_pDC->GetSafeHdc(), hglrc); }
	void SwapGLBuffers() { SwapBuffers(m_pDC->GetSafeHdc()); }


protected:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual ~CViewPerspective();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

