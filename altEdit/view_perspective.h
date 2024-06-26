#pragma once


class Document;

#include "types.h"

class CViewPerspective : public CScrollView
{
protected:
	CViewPerspective();
	DECLARE_DYNCREATE(CViewPerspective)

public:
	Document* GetDocument();

	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

public:
	virtual ~CViewPerspective();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void InvertLine (CDC* pDC, POINT from, POINT to);
	point_t m_ptFrom;
	point_t m_ptTo;
	HCURSOR m_hCursor;

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline Document* CViewPerspective::GetDocument()
   { return (Document*)m_pDocument; }
#endif
