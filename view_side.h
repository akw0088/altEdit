#pragma once


class Document;

class CViewSide : public CScrollView
{
protected: // create from serialization only
	CViewSide();
	DECLARE_DYNCREATE(CViewSide)

// Attributes
public:
	Document* GetDocument();

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
public:
	int selected;
	point_t p1_orig;
	point_t p2_orig;

	virtual ~CViewSide();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	void InvertLine (CDC* pDC, CPoint from, CPoint to);
	POINT m_mouseStart;
	POINT m_mouseEnd;
	HCURSOR m_hCursor;
	//{{AFX_MSG(CSketchView)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SketchView.cpp
inline Document* CViewSide::GetDocument()
   { return (Document*)m_pDocument; }
#endif

