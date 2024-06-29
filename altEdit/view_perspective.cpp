#include "stdafx.h"
#include "view_perspective.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CViewPerspective, CView)

CViewPerspective::CViewPerspective()
{
}

CViewPerspective::~CViewPerspective()
{
}

BOOL CViewPerspective::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd
		1,                                // version number
		PFD_DRAW_TO_WINDOW |              // support window
		PFD_SUPPORT_OPENGL |              // support OpenGL
		PFD_GENERIC_ACCELERATED |
		PFD_DOUBLEBUFFER,                 // double buffered
		PFD_TYPE_RGBA,                    // RGBA type
		32,                               // 24-bit color depth
		0, 0, 0, 0, 0, 0,                 // color bits ignored
		0,                                // no alpha buffer
		0,                                // shift bit ignored
		0,                                // no accumulation buffer
		0, 0, 0, 0,                       // accumulation bits ignored
		24,                               // 16-bit z-buffer
		8,                                // no stencil buffer
		0,                                // no auxiliary buffer
		PFD_MAIN_PLANE,                   // main layer
		0,                                // reserved
		0, 0, 0                           // layer masks ignored
	};

	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	if (m_nPixelFormat == 0)
		return FALSE;

	return ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd);
}

BOOL CViewPerspective::InitOpenGL()
{
	m_pDC = new CClientDC(this);

	if (m_pDC == NULL)
		return FALSE;

	if (!SetupPixelFormat())
		return FALSE;

	hglrc_legacy = ::wglCreateContext(m_pDC->GetSafeHdc());

	if (hglrc_legacy == 0)
		return FALSE;

	if (::wglMakeCurrent(m_pDC->GetSafeHdc(), hglrc_legacy) == FALSE)
		return FALSE;
	glewInit();

	if (::wglewIsSupported("WGL_ARB_create_context") == TRUE)
	{
		const int context[] =
		{
			// using 3.1 instead of 4.4 so we can keep legacy matrix operations
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 1,
			//				WGL_CONTEXT_FLAGS_ARB,
			//				WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
			//				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		};

		const int pixelformat[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0,
		};

		int format;
		unsigned int num_formats;

		::wglChoosePixelFormatARB(m_pDC->GetSafeHdc(), (int *)pixelformat, NULL, 1, &format, &num_formats);
		hglrc = ::wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, context);
		if (hglrc != NULL)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hglrc_legacy);
			wglMakeCurrent(m_pDC->GetSafeHdc(), hglrc);
		}
		else
		{
			MessageBox(TEXT("Graphics Driver does not support OpenGL 4.4, update driver and/or GPU"), TEXT("Fatal Error"), 0);
			hglrc = hglrc_legacy;
		}
	}
	else
	{
		//opengl 2.0
		hglrc = hglrc_legacy;
	}


	glClearColor(0.0f, 0.0f, 0.25f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glLineWidth(2.0f);
	glPointSize(3.0f);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CViewPerspective, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CViewPerspective::OnDraw(CDC* pDC)
{
	SetContext();
	glClear(GL_COLOR_BUFFER_BIT);
	SwapGLBuffers();
}

#ifdef _DEBUG
void CViewPerspective::AssertValid() const
{
	CView::AssertValid();
}

void CViewPerspective::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif


int CViewPerspective::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!InitOpenGL())
	{
		MessageBox(TEXT("Error setting up OpenGL!"), TEXT("Init Error!"),
			MB_OK | MB_ICONERROR);
		return -1;
	}

	return 0;
}

void CViewPerspective::OnDestroy()
{
	CView::OnDestroy();

	wglMakeCurrent(0, 0);
	wglDeleteContext(hglrc);
	if (m_pDC)
	{
		delete m_pDC;
	}
	m_pDC = NULL;
}

BOOL CViewPerspective::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CViewPerspective::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC,
		::LoadCursor(NULL, IDC_ARROW), NULL, NULL);
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}
