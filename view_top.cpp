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

IMPLEMENT_DYNCREATE(CViewTop, CScrollView)

BEGIN_MESSAGE_MAP(CViewTop, CScrollView)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



CViewTop::CViewTop()
{
	m_hCursor = AfxGetApp ()->LoadStandardCursor (IDC_CROSS);

    selected = -1;
}

CViewTop::~CViewTop()
{
}

BOOL CViewTop::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CViewTop::OnDraw(CDC* pDC)
{
    Document* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


    CString view_name("Top");
    RECT rect;

    rect.top = GRID_SIZE;
    rect.left = GRID_SIZE;
    rect.right = GRID_SIZE * 20;
    rect.bottom = GRID_SIZE * 20;
    pDC->SetBkColor(RGB(255, 255, 255));

    pDC->DrawText(view_name, &rect, 0);

    if (pDoc->IsGridVisible () && selected == -1)
    {
        draw_grid(pDC);
    }

    int nCount = pDoc->GetBrushCount();
    if (nCount)
    {
        for (int i = 0; i < nCount; i++)
        {
            pDoc->GetBrush(i)->Draw(pDC, 0);
        }
    }


}

void CViewTop::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
    SetScrollSizes(MM_TEXT, CSize(0, 0));
}

#ifdef _DEBUG
void CViewTop::AssertValid() const
{
    CView::AssertValid();
}

void CViewTop::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

Document* CViewTop::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Document)));
	return (Document *)m_pDocument;
}
#endif


BOOL CViewTop::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    ::SetCursor (m_hCursor);
    return TRUE;	
}

void CViewTop::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);

	CPoint pos = point;
    CClientDC dc (this);
    OnPrepareDC (&dc);
    dc.DPtoLP (&pos);

    if (GetDocument ()->IsGridVisible ())
    {
        pos.x = ((pos.x + 0) / GRID_SIZE) * GRID_SIZE;
        pos.y = ((pos.y + 0) / GRID_SIZE) * GRID_SIZE;
    }


    Document* pDoc = GetDocument();
    int nCount = pDoc->GetLineCount();
    if (nCount)
    {
        for (int i = 0; i < nCount; i++)
        {
            point_t p1 = pDoc->GetBrush(i)->p1;
            point_t p2 = pDoc->GetBrush(i)->p2;

            if (
                (( pos.x <= p1.x && pos.x >= p2.x ) || (pos.x >= p1.x && pos.x <= p2.x)) &&
                (( pos.y <= p1.z && pos.y >= p2.z ) || (pos.y >= p1.z && pos.y <= p2.z))
                )
            {
                pDoc->GetBrush(i)->orig_color = pDoc->GetBrush(i)->color;
                pDoc->GetBrush(i)->color = RGB(128, 0, 0);
                selected = i;
                p1_orig = pDoc->GetBrush(i)->p1;
                p2_orig = pDoc->GetBrush(i)->p2;
                break;
            }
        }
    }

    m_mouseStart.x = pos.x;
    m_mouseStart.y = pos.y;
    m_mouseEnd.x = pos.x;
    m_mouseEnd.y = pos.y;
    SetCapture ();
}

void CViewTop::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);

    if (GetCapture() == this)
	{
		CPoint pos = point;
		CClientDC dc (this);
		OnPrepareDC (&dc);
		dc.DPtoLP (&pos);

        if (GetDocument ()->IsGridVisible ())
        {
            pos.x = ((pos.x + 0) / GRID_SIZE) * GRID_SIZE;
            pos.y = ((pos.y + 0) / GRID_SIZE) * GRID_SIZE;
        }

        if (CPoint(m_mouseEnd.x, m_mouseEnd.y) != pos)
        {

            if (selected == -1)
            {
                InvertLine(&dc, CPoint(m_mouseStart.x, m_mouseStart.y), CPoint(m_mouseEnd.x, m_mouseEnd.y));
                InvertLine(&dc, CPoint(m_mouseStart.x, m_mouseStart.y), CPoint(pos.x, pos.y));
                m_mouseEnd.x = pos.x;
                m_mouseEnd.y = pos.y;
            }
            else
            {
                m_mouseEnd.x = pos.x;
                m_mouseEnd.y = pos.y;
                Document* pDoc = GetDocument();
                pDoc->GetBrush(selected)->p1.x = p1_orig.x + (m_mouseEnd.x - m_mouseStart.x);
                pDoc->GetBrush(selected)->p1.z = p1_orig.z + (m_mouseEnd.y - m_mouseStart.y);
                pDoc->GetBrush(selected)->p2.x = p2_orig.x + (m_mouseEnd.x - m_mouseStart.x);
                pDoc->GetBrush(selected)->p2.z = p2_orig.z + (m_mouseEnd.y - m_mouseStart.y);
                pDoc->UpdateAllViews(NULL);
            }
        }

    }
}

void CViewTop::OnLButtonUp(UINT nFlags, CPoint point)
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
            pos.x = ((pos.x + 0) / GRID_SIZE) * GRID_SIZE;
            pos.y = ((pos.y + 0) / GRID_SIZE) * GRID_SIZE;
        }


        if (selected == -1)
        {
            InvertLine(&dc, m_mouseStart, m_mouseEnd);


            Document* pDoc = GetDocument();
            point_t from;
            point_t to;

            from.x = m_mouseStart.x;
            from.y = DEFAULT_FROM;
            from.z = m_mouseStart.y;

            to.x = m_mouseEnd.x;
            to.y = DEFAULT_TO;
            to.z = m_mouseEnd.y;

            Brush* pLine = pDoc->AddBrush(from, to);
        }
        else
        {
            Document* pDoc = GetDocument();
            pDoc->GetBrush(selected)->p1.x = p1_orig.x + (m_mouseEnd.x - m_mouseStart.x);
            pDoc->GetBrush(selected)->p1.z = p1_orig.z + (m_mouseEnd.y - m_mouseStart.y);
            pDoc->GetBrush(selected)->p2.x = p2_orig.x + (m_mouseEnd.x - m_mouseStart.x);
            pDoc->GetBrush(selected)->p2.z = p2_orig.z + (m_mouseEnd.y - m_mouseStart.y);
            pDoc->GetBrush(selected)->color = pDoc->GetBrush(selected)->orig_color;
            pDoc->UpdateAllViews(NULL);
        }
    }	
    selected = -1;

}

void CViewTop::InvertLine(CDC *pDC, POINT from, POINT to)
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

void CViewTop::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    if (lHint == 0x7C)
	{
        Brush* pBrush = (Brush *) pHint;
		ASSERT (pBrush->IsKindOf (RUNTIME_CLASS (Brush)));
		CClientDC dc (this);
		OnPrepareDC (&dc);
        pBrush->Draw (&dc, 0);
		return;
	}	
	CScrollView::OnUpdate (pSender, lHint, pHint);
}
