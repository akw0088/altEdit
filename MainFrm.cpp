#include "stdafx.h"
#include "WinApp.h"

#include "view_perspective.h"
#include "view_top.h"
#include "view_front.h"
#include "view_side.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()


CMainFrame::CMainFrame()
{
	init = false;
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}


#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CRect rect;
	GetClientRect(&rect);

	m_wndSplitter.CreateStatic(this, 2, 2);

	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CViewPerspective), CSize(rect.right / 2, rect.bottom / 2), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CViewTop), CSize(rect.right / 2, rect.bottom / 2), pContext);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CViewFront), CSize(rect.right / 2, rect.bottom / 2), pContext);
	m_wndSplitter.CreateView(1, 1, RUNTIME_CLASS(CViewSide), CSize(rect.right / 2, rect.bottom / 2), pContext);

	toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	toolbar.LoadToolBar(TEXT("mfcToolBar"));
	toolbar.LoadBitmap(IDR_TOOLBAR1);

	init = true;

	return TRUE;
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);


	if (init)
	{
		RECT rect;
		m_wndSplitter.GetClientRect(&rect);

		m_wndSplitter.SetRowInfo(0, rect.bottom / 2, 0);
		m_wndSplitter.SetRowInfo(1, rect.bottom / 2, 0);
		m_wndSplitter.SetColumnInfo(0, rect.right / 2, 0);
		m_wndSplitter.SetColumnInfo(1, rect.right / 2, 0);

		GetActiveDocument()->UpdateAllViews(NULL);
	}
}