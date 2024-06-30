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
	time = 0.0f;
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


int CViewPerspective::load_shader(char *vertex_file, char *geometry_file, char *fragment_file)
{
	FILE		*fLog;
	int			success;
	int			max_attrib = 0;
	int			size = 0;

	fLog = fopen("infolog.txt", "a");
	fprintf(fLog, "OpenGL Version %s\n", glGetString(GL_VERSION));
	fprintf(fLog, "OpenGL Renderer %s\n", glGetString(GL_RENDERER));
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attrib);
	fprintf(fLog, "Max vertex attribs %d\n", max_attrib);

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
	fprintf(fLog, "Max texture size %dx%d\n", size, size);


	if (vertex_file)
	{
		vertex_src = get_file(vertex_file, NULL);
		if (vertex_src == NULL)
		{
			fprintf(fLog, "Unable to load vertex shader %s\n", vertex_file);
			fclose(fLog);
			return -1;
		}
		else
		{
			fprintf(fLog, "Loaded vertex shader %s\n", vertex_file);
		}


		vertex_handle = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_handle, 1, &vertex_src, NULL);
		glCompileShader(vertex_handle);
		glGetShaderiv(vertex_handle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int		info_size;
			char	*info_log;

			glGetShaderiv(vertex_handle, GL_INFO_LOG_LENGTH, &info_size);
			info_log = new char[info_size];

			glGetShaderInfoLog(vertex_handle, info_size, NULL, info_log);
			fprintf(fLog, "Error compiling: %s", info_log);
			fclose(fLog);
			delete[] info_log;
			return -1;
		}
		delete[] vertex_src;
		vertex_src = NULL;
	}

	if (geometry_file)
	{
		geometry_src = get_file(geometry_file, NULL);
		if (geometry_src == NULL)
		{
			fprintf(fLog, "Unable to load geometry shader %s\n", geometry_file);
			fclose(fLog);
			return -1;
		}
		else
		{
			fprintf(fLog, "Loaded geometry shader %s\n", geometry_file);
		}

		geometry_handle = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_handle, 1, &geometry_src, NULL);
		glCompileShader(geometry_handle);
		glGetShaderiv(geometry_handle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int		info_size;
			char	*info_log;

			glGetShaderiv(geometry_handle, GL_INFO_LOG_LENGTH, &info_size);
			info_log = new char[info_size];

			glGetShaderInfoLog(geometry_handle, info_size, NULL, info_log);
			fprintf(fLog, "Error compiling: %s", info_log);
			fclose(fLog);
			delete[] info_log;
			return -1;
		}
		delete[] geometry_src;
		geometry_src = NULL;
	}

	if (fragment_file)
	{
		fragment_src = get_file(fragment_file, NULL);
		if (fragment_src == NULL)
		{
			fprintf(fLog, "Unable to load fragment shader %s\n", fragment_file);
			fclose(fLog);
			return -1;
		}
		else
		{
			fprintf(fLog, "Loaded fragment shader %s\n", fragment_file);
		}

		fragment_handle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_handle, 1, &fragment_src, NULL);
		glCompileShader(fragment_handle);
		glGetShaderiv(fragment_handle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int		info_size;
			char	*info_log;

			glGetShaderiv(fragment_handle, GL_INFO_LOG_LENGTH, &info_size);
			info_log = new char[info_size];

			glGetShaderInfoLog(fragment_handle, info_size, NULL, info_log);
			fprintf(fLog, "Error compiling: %s", info_log);
			fclose(fLog);
			delete[] info_log;
			return -1;
		}
		delete[] fragment_src;
		fragment_src = NULL;
	}

	program_handle = glCreateProgram();
	if (vertex_file)
		glAttachShader(program_handle, vertex_handle);
	if (geometry_file)
		glAttachShader(program_handle, geometry_handle);
	if (fragment_file)
		glAttachShader(program_handle, fragment_handle);
	prelink();
	glLinkProgram(program_handle);
	glGetProgramiv(program_handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		int		info_size;
		char	*info_log;

		glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &info_size);
		info_log = new char[info_size];

		glGetProgramInfoLog(program_handle, info_size, NULL, info_log);
		fprintf(fLog, "Error linking: %s", info_log);
		fclose(fLog);
		delete[] info_log;
		return -1;
	}

	fprintf(fLog, "Link successful\n");
	fclose(fLog);
	return 0;
}


BOOL CViewPerspective::InitOpenGL()
{
	m_pDC = new CClientDC(this);


	SetTimer(WM_STEP, 16, NULL);

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
			//			MessageBox(TEXT("Graphics Driver does not support OpenGL 4.4, update driver and/or GPU"), TEXT("Fatal Error"), 0);
			hglrc = hglrc_legacy;
		}
	}
	else
	{
		//opengl 2.0
		hglrc = hglrc_legacy;
	}

	CRect rect;
	GetClientRect(&rect);

	glClearColor(0.0f, 0.0f, 0.25f, 0.0f);
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);
	glLineWidth(2.0f);
	glPointSize(3.0f);
	glColor3f(0.0f, 0.2f, 0.0f);
	glViewport(0, 0, rect.right, rect.bottom);


	load_shader("shader.vs", NULL, "shader.fs");


	float points[] = {
		-1.0f, -1.0f,  // first triangle
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		-1.0f,  1.0f,  // second triangle
		 1.0f, -1.0f,
		 1.0f,  1.0f
	};
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &points_vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), points, GL_STATIC_DRAW);

	int aPosition = glGetAttribLocation(program_handle, "a_position");

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	return TRUE;
}


BEGIN_MESSAGE_MAP(CViewPerspective, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CViewPerspective::OnDraw(CDC* pDC)
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(program_handle);
	glBindVertexArray(vao);

	// look up uniform locations
	int resolutionLocation = glGetUniformLocation(program_handle, "iResolution");
	int mouseLocation = glGetUniformLocation(program_handle, "iMouse");
	int timeLocation = glGetUniformLocation(program_handle, "iTime");

	// update uniforms
	glUniform2f(resolutionLocation, (float)width, (float)height);
	glUniform2f(mouseLocation, (float)mouse_x, (float)mouse_y);
	glUniform1f(timeLocation, time);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	SwapBuffers(m_pDC->GetSafeHdc());
}

int CViewPerspective::prelink()
{
	// bind varying names

	return 0;
}


void CViewPerspective::OnSize(UINT nType, int cx, int cy)
{
	glViewport(0, 0, cx, cy);

	width = cx;
	height = cy;
}

void CViewPerspective::OnMouseMove(UINT nFlags, CPoint point)
{
	mouse_x = point.x;
	mouse_y = point.y;
}


void CViewPerspective::OnTimer(UINT_PTR TimerVal)
{
	time += 0.016f;
	Invalidate(0);
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
