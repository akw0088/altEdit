#pragma once


class Document;

class CViewFront : public CScrollView
{
protected:
	CViewFront();
	DECLARE_DYNCREATE(CViewFront)

public:
	Document* GetDocument();


	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

public:
	int selected;
	point_t p1_orig;
	point_t p2_orig;

	virtual ~CViewFront();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void InvertLine (CDC* pDC, CPoint from, CPoint to);
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
inline Document* CViewFront::GetDocument()
   { return (Document*)m_pDocument; }
#endif
