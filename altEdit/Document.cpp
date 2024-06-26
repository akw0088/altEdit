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


IMPLEMENT_DYNCREATE(Document, CDocument)

BEGIN_MESSAGE_MAP(Document, CDocument)
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
END_MESSAGE_MAP()

Document::Document()
{
    m_bShowGrid = TRUE;
}

Document::~Document()
{
}

BOOL Document::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_bShowGrid = TRUE;
	return TRUE;
}

void Document::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        ar << m_bShowGrid;
    }
    else
    {
        ar >> m_bShowGrid;
    }
    m_arrBrushes.Serialize (ar);
}

#ifdef _DEBUG
void Document::AssertValid() const
{
	CDocument::AssertValid();
}

void Document::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif

BOOL Document::IsGridVisible()
{
	return m_bShowGrid;
}

void Document::OnViewGrid()
{
    if (m_bShowGrid)
        m_bShowGrid = FALSE;
    else
        m_bShowGrid = TRUE;

    SetModifiedFlag (TRUE);
    UpdateAllViews (NULL);	
}

void Document::OnUpdateViewGrid(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck (m_bShowGrid);
}

Brush* Document::AddBrush(point_t from, point_t to)
{
    Brush* pBrush = NULL;

    try
    {
        pBrush = new Brush(from, to);
        m_arrBrushes.Add (pBrush);
        SetModifiedFlag (TRUE);
		UpdateAllViews (NULL, 0x7C, pBrush);
    }
    catch (CMemoryException* e)
    {
        AfxMessageBox (_T ("Out of memory"));
        if (pBrush != NULL)
        {
            delete pBrush;
            pBrush = NULL;
        }
        e->Delete ();   
    }       
    return pBrush;
}

int Document::GetBrushCount()
{
    return m_arrBrushes.GetSize ();
}

Brush* Document::GetBrush(int nIndex)
{
	ASSERT (nIndex < GetBrushCount ());
	return m_arrBrushes[nIndex];
}

void Document::DeleteContents()
{
    int nCount = GetBrushCount ();

    if (nCount)
    {
        for (int i = 0; i < nCount; i++)
            delete m_arrBrushes[i];
        m_arrBrushes.RemoveAll ();
    }
    CDocument::DeleteContents();
}
