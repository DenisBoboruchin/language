#include <stdio.h>

#include "include/translator.h"

const char* NAME = "progtest.den";

int main ()
{
    size_t sizeBuf = GetSizeBuf (NAME);
    char* prog = CreateBuf (&sizeBuf, NAME);
    item* node = GetGrammar (prog);
    //item* node = GetGrammar ("if(tri>3){tri=3;two=2;if(r<2){x=4;};};y=-word^(3-1)+(10+5*2^(1+1)^3)*3;x=3>5^2;3+4;$");
    
    CTree t1 (node);
    CTree t2 (t1);
    CTree t3 = t2;

    TranslateToAsm (t3);
    t2.graphDump ();

    return 0;
}
