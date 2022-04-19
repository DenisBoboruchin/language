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
    
    t1.graphDump ();

    printf ("%d\n", GetGrammar ("-2^(-2*(2+-3))+(10+5*2^(1+1)^3)*2$"));

   // CTree t1 = CtorTreeFromFile ("dump/startEquation.txt");

    return 0;
}
