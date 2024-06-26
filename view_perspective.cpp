#include "stdafx.h"
#include "Brush.h"
#include "WinApp.h"
#include "Document.h"

#include "view_perspective.h"
#include "view_top.h"
#include "view_front.h"
#include "view_side.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CViewPerspective, CScrollView)

BEGIN_MESSAGE_MAP(CViewPerspective, CScrollView)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


CViewPerspective::CViewPerspective()
{
	m_hCursor = AfxGetApp ()->LoadStandardCursor (IDC_CROSS);
}

CViewPerspective::~CViewPerspective()
{
}

BOOL CViewPerspective::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CViewPerspective::OnDraw(CDC* pDC)
{
    Document* pDoc = GetDocument();
    ASSERT_VALID(pDoc);


    CString view_name("Perspective");
    RECT rect;

    rect.top = GRID_SIZE;
    rect.left = GRID_SIZE;
    rect.right = GRID_SIZE * 20;
    rect.bottom = GRID_SIZE * 20;
    pDC->SetBkColor(RGB(255, 255, 255));

    pDC->DrawText(view_name, &rect, 0);

    if (pDoc->IsGridVisible())
    {
//        draw_grid(pDC);
    }


    int nCount = pDoc->GetBrushCount();
    if (nCount)
    {
        for (int i = 0; i < nCount; i++)
        {
            pDoc->GetBrush(i)->Draw(pDC, 3);
        }
    }


}
void CViewPerspective::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
//    SetScrollSizes(MM_TEXT, CSize(GRID_WIDTH, GRID_HEIGHT));
    SetScrollSizes(MM_TEXT, CSize(0, 0));
}


#ifdef _DEBUG
void CViewPerspective::AssertValid() const
{
	CScrollView::AssertValid();
}

void CViewPerspective::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

Document* CViewPerspective::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Document)));
	return (Document*)m_pDocument;
}
#endif

BOOL CViewPerspective::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    ::SetCursor (m_hCursor);
    return TRUE;	
}

void CViewPerspective::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);

	CPoint pos = point;
    CClientDC dc (this);
    OnPrepareDC (&dc);
    dc.DPtoLP (&pos);

    if (GetDocument ()->IsGridVisible ())
    {
        pos.x = ((pos.x + GRID_OFFSET) / GRID_SIZE) * GRID_SIZE;
        pos.y = ((pos.y + GRID_OFFSET) / GRID_SIZE) * GRID_SIZE;
    }



    m_ptFrom.x = pos.x;
    m_ptFrom.y = pos.y;
    m_ptFrom.z = DEFAULT_FROM;

    m_ptTo.x = pos.x;
    m_ptTo.y = pos.y;
    m_ptTo.z = DEFAULT_TO;

    SetCapture ();
}

void CViewPerspective::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);

    if (GetCapture () == this)
    {
		CPoint pos = point;
		CClientDC dc (this);
		OnPrepareDC (&dc);
		dc.DPtoLP (&pos);

        if (GetDocument ()->IsGridVisible ())
        {
            pos.x = ((pos.x + GRID_OFFSET) / GRID_SIZE) * GRID_SIZE;
            pos.y = ((pos.y + GRID_OFFSET) / GRID_SIZE) * GRID_SIZE;
        }

        if (CPoint(m_ptTo.x, m_ptTo.y) != pos)
        {
            InvertLine (&dc, CPoint(m_ptFrom.x, m_ptFrom.y), CPoint(m_ptTo.x, m_ptTo.y));
            InvertLine (&dc, CPoint(m_ptFrom.x, m_ptFrom.y), CPoint(pos.x, pos.y));
            m_ptTo.x = pos.x;
            m_ptTo.y = pos.y;
            m_ptTo.z = DEFAULT_TO;
        }
    }
}

void CViewPerspective::OnLButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonUp(nFlags, point);

    if (GetCapture () == this)
    {
        ::ReleaseCapture ();

		CPoint pos = point;
		CClientDC dc (this);
		OnPrepareDC (&dc);
		dc.DPtoLP (&pos);

        if (GetDocument ()->IsGridVisible ())
        {
            pos.x = ((pos.x + GRID_OFFSET) / GRID_SIZE) * GRID_SIZE;
            pos.y = ((pos.y + GRID_OFFSET) / GRID_SIZE) * GRID_SIZE;
        }

        InvertLine (&dc, CPoint(m_ptFrom.x, m_ptFrom.y), CPoint(m_ptTo.x, m_ptTo.y));

        Document* pDoc = GetDocument ();
        Brush* pLine = pDoc->AddBrush (m_ptFrom, m_ptTo);
    }	
}

void CViewPerspective::InvertLine(CDC *pDC, POINT from, POINT to)
{
    RECT rect;

    rect.left = from.x;
    rect.right = to.x;

    rect.top = from.y;
    rect.bottom = to.y;

    int nOldMode = pDC->SetROP2 (R2_NOT);
    pDC->MoveTo (from);
    pDC->Rectangle(&rect);
    pDC->SetROP2 (nOldMode);
}

void CViewPerspective::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == 0x7C)
    {
        Brush* pBrush = (Brush *) pHint;
		ASSERT (pBrush->IsKindOf (RUNTIME_CLASS (Brush)));
		CClientDC dc (this);
		OnPrepareDC (&dc);
        pBrush->Draw (&dc, 3);
		return;
	}	
	CScrollView::OnUpdate (pSender, lHint, pHint);
}
