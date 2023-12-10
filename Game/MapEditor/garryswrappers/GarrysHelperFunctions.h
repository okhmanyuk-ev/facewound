#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

bool RectCollision(RECT* r1, RECT* r2);
void NormalizeRect(RECT* r1);
int
intersect_triangle(double orig[3], double dir[3],
                   double vert0[3], double vert1[3], double vert2[3],
                   double *t, double *u, double *v);