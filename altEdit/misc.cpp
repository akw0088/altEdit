#include "StdAfx.h"
#include "types.h"

void draw_grid(CDC* pDC)
{
    POINT lp;


    lp.x = GRID_SIZE + GRID_OFFSET;
    lp.y = GRID_SIZE + GRID_OFFSET;
    for (int x = 0; x < GRID_WIDTH; x += GRID_SIZE)
    {
        lp.x = x + GRID_OFFSET;
        lp.y = GRID_SIZE + GRID_OFFSET;
        pDC->MoveTo(lp);
        for (int y = 0; y < GRID_HEIGHT; y += GRID_SIZE)
        {
            POINT p;

            p.x = x + GRID_OFFSET;
            p.y = y + GRID_OFFSET;
            pDC->LineTo(p);
            lp = p;
        }
    }


    lp.x = GRID_SIZE + GRID_OFFSET;
    lp.y = GRID_SIZE + GRID_OFFSET;
    for (int y = 0; y < GRID_HEIGHT; y += GRID_SIZE)
    {
        lp.y = y + GRID_OFFSET;
        lp.x = GRID_SIZE + GRID_OFFSET;
        pDC->MoveTo(lp);
        for (int x = 0; x < GRID_WIDTH; x += GRID_SIZE)
        {
            POINT p;

            p.x = x + GRID_OFFSET;
            p.y = y + GRID_OFFSET;
            pDC->LineTo(p);
            lp = p;
        }
    }
}