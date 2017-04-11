#include "typedef.h"

bool CComparator::operator()(const CBrick* arg1,const CBrick* arg2){
    if(arg1->y > arg2->y) return true;
    if(arg1->y == arg2->y && arg1->x > arg2->x)
        return true;
    return false;
}
