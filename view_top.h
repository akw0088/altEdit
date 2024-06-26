#pragma once


class Document;

class CViewTop : public CScrollView
{
protected:
	CViewTop();
	DECLARE_DYNCREATE(CViewTop)

public:
	Document* GetDocument();

	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

public:
	int selected;
	point_t p1_orig;
	point_t p2_orig;

	virtual ~CViewTop();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void InvertLine (CDC* pDC, POINT from, POINT to);
	POINT m_mouseStart;
	POINT m_mouseEnd;
	HCURSOR m_hCursor;

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline Document* CViewTop::GetDocument()
   { return (Document*)m_pDocument; }
#endif

