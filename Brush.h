#pragma once

#include "types.h"


class Brush : public CCmdTarget
{
	DECLARE_SERIAL (Brush)

	Brush();
	Brush(point_t from, point_t to);

public:
	void Draw (CDC* pDC, int side);
	virtual ~Brush();

	point_t p1;
	point_t p2;

	COLORREF color;
	COLORREF orig_color;


	virtual void Serialize(CArchive& ar);
protected:

	DECLARE_MESSAGE_MAP()
};
