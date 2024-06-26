#pragma once

#include "Brush.h"

typedef CTypedPtrArray<CObArray, Brush*> BrushArray;

class Document : public CDocument
{
protected:
	Document();
	DECLARE_DYNCREATE(Document)

public:

	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();

	Brush* GetBrush(int nIndex);
	int GetBrushCount ();
	Brush* AddBrush (point_t from, point_t to);
	BOOL IsGridVisible ();
	virtual ~Document();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

protected:
	BrushArray m_arrBrushes;
	BOOL m_bShowGrid;
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};
