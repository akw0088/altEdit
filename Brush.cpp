#include "stdafx.h"
#include "WinApp.h"
#include "Brush.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL (Brush, CCmdTarget, 1)

Brush::Brush()
{
    srand((unsigned int)time(0));

    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;

    color = RGB(r,g,b);

}

Brush::Brush(point_t from, point_t to)
{
	p1 = from;
	p2 = to;

    srand((unsigned int)time(0));
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;

    color = RGB(r, g, b);
 
}

Brush::~Brush()
{
}

BEGIN_MESSAGE_MAP(Brush, CCmdTarget)
END_MESSAGE_MAP()


void Brush::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        CString data;

        data.Format(TEXT("(( %d %d %d ) ( %d %d %d ))\r\n"),
            p1.x, p1.y, p1.z,
            p2.x, p2.y, p2.z);

        ar << data;
    }
    else
    {
        CString data;

        ar >> data;

        // This is 8 bit ascii, unicode on writing out, but ascii on way in
        CT2A char_array(data);

        int ret = sscanf(char_array, "(( %d %d %d ) ( %d %d %d ))\r\n",
            &p1.x, &p1.y, &p1.z,
            &p2.x, &p2.y, &p2.z);

        if (ret != 6)
        {
            MessageBox(0, TEXT("Bad data from file"), TEXT("Read Error"), 0);
        }
    }
}

void Brush::Draw(CDC *pDC, int side)
{
    RECT rect;

    if (side == 0)
    {
        // top view XZ
        rect.left   = p1.x + 1;
        rect.top    = p1.z + 1;
        rect.right  = p2.x + 1;
        rect.bottom = p2.z + 1;
        pDC->SelectStockObject(BLACK_PEN);

        CBrush brush(color);
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        pDC->Rectangle(&rect);
        pDC->SelectObject(pOldBrush);


    }
    else if (side == 1)
    {
        //Front view XY
        rect.left   = p1.x + 1;
        rect.top    = p1.y + 1;
        rect.right  = p2.x + 1;
        rect.bottom = p2.y + 1;
        pDC->SelectStockObject(BLACK_PEN);
        CBrush brush(color);
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        pDC->Rectangle(&rect);
        pDC->SelectObject(pOldBrush);
    }
    else if (side == 2)
    {
        // Side View ZY
        rect.left   = p1.z + 1;
        rect.top    = p1.y + 1;
        rect.right  = p2.z + 1;
        rect.bottom = p2.y + 1;
        pDC->SelectStockObject(BLACK_PEN);
        CBrush brush(color);
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        pDC->Rectangle(&rect);
        pDC->SelectObject(pOldBrush);
    }
    else if (side == 3)
    {
        // perspective
        // perspective should always be blank
        // until we put 3d in it
    }


}
