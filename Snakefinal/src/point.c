#include "point.h"

bool points_have_same_coord(Point *p1, Point *p2){
    bool ok=false;
    if((p1->x==p2->x)&&(p1->y==p2->y)){
        ok=true;
    }
    return ok;
}