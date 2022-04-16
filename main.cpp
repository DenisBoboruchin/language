#include <stdio.h>

#include "include/differenciator.h"

int main ()
{
    CTree t1;

    t1.addItm (nullptr, "*");

    t1.addItm (t1.findItm ("*"), "+", ADDRIGHT);
    t1.addItm (t1.findItm ("*"), "5", ADDLEFT);

    t1.addItm (t1.findItm ("+"), "3", ADDLEFT);
    t1.addItm (t1.findItm ("+"), "x", ADDRIGHT);
    
    printf ("adssda\n");
    t1.graphDump ();

    printf ("%d\n", GetG ("345+22$"));

   // CTree t1 = CtorTreeFromFile ("dump/startEquation.txt");

    return 0;
}
