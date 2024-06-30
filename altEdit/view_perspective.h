#pragma once

#include "Document.h"
#include "misc.h"

#define WM_STEP (WM_USER + 1)

class CViewPerspective : public CView
{
protected:
	CViewPerspective();
	DECLARE_DYNCREATE(CViewPerspective)
public:

protected:
	HGLRC hglrc_legacy;
	HGLRC hglrc;
	CDC* m_pDC;
	char *vertex_src;
	char *geometry_src;
	char *fragment_src;

	int vertex_handle;
	int geometry_handle;
	int fragment_handle;
	int program_handle;

	unsigned int points_vbo;
	unsigned int vao;

	int width;
	int height;

	float time;

	int mouse_x;
	int mouse_y;

public:
	BOOL SetupPixelFormat();
	BOOL InitOpenGL();


protected:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	int load_shader(char *vertex_file, char *geometry_file, char *fragment_file);
	void reload_shader();
	void destroy_shader();
	int prelink();

protected:
	virtual ~CViewPerspective();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR TimerVal);

	DECLARE_MESSAGE_MAP()
};

