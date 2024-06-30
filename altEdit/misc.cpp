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

BOOL setupPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd;
    INT pixelformat;

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
    pfd.dwLayerMask = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    //pfd.cAccumBits	= 32;
    pfd.cStencilBits = 8;

    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0)
    {
        MessageBox(NULL, TEXT("ChoosePixelFormat failed"), TEXT("Error"), MB_OK);
        return FALSE;
    }

    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
    {
        MessageBox(NULL, TEXT("SetPixelFormat failed"), TEXT("Error"), MB_OK);
        return FALSE;
    }
    return TRUE;
}



char *get_file(char *filename, unsigned int *size)
{
	FILE	*file;
	char	*buffer;
	int	file_size, bytes_read;

	file = fopen(filename, "rb");
	if (file == NULL)
		return 0;
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = new char[file_size + 1];
	bytes_read = (int)fread(buffer, sizeof(char), file_size, file);
	if (bytes_read != file_size)
	{
		delete[] buffer;
		fclose(file);
		return 0;
	}
	fclose(file);
	buffer[file_size] = '\0';

	if (size != NULL)
	{
		*size = file_size;
	}


	return buffer;
}