#include <stdio.h>

#include "include/differenciator.h"

int main ()
{
    item* node = GetGrammar ("y=-word^(2-1)+(10+5*2^(1+1)^3)*3$");
    
    CTree t1 (node);
    CTree t2 (t1);
    CTree t3 = t2;

    t2.graphDump ();

    return 0;
}
