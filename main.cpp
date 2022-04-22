#include <stdio.h>

#include "include/differenciator.h"

int main ()
{
    item* node = GetGrammar ("-word^(2-1)+(10+5*2^(1+1)^3)*2$");
    
    CTree t1 (node);
    CTree t2 (t1);

    t2.graphDump ();

    return 0;
}
