#pragma once


void draw_grid(CDC* pDC);

#define GRID_SIZE 8
#define GRID_OFFSET 25
#define GRID_WIDTH 800
#define GRID_HEIGHT 800

// when creating 2d, this is default for unseen dimension
#define DEFAULT_FROM 64
#define DEFAULT_TO 128


typedef struct
{
	int x;
	int y;
	int z;
} point_t;