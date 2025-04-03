#ifndef _POINT_H
#define _POINT_H

#include <stdbool.h>

// Structure des points
typedef struct {
    int x, y;
} Point;

bool points_have_same_coord(Point *p1, Point *p2);

#endif