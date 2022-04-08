#ifndef GRAPHVIZ
#define GRAPHVIZ

#include <stdio.h>
#include <stdlib.h>

typedef const char* ElemType;
#define ELEM_FMTS "%s"

void    DotCtor         (const char* rankdir = "LR", const char *color = "darkkhaki");

void    DotElemCtor     (void* i, ElemType data, void* next = 0, void* prev = 0, 
        const char* shape = "record", const char* color = "darkslategray1",
        const char* label = "elem", const char* style = "filled");
//void    DotInfoElemCtor (int capacity, int size, int head, int tail, 
  //      int free, int statLin, const char* shape = "record", 
   //     const char* color = "darksalmon", const char* style = "filled");

void    DotLeftEdgeCtor     (void* num1, void* num2, const char* style = "none", const char* color = "dodgerblue");   
void    DotRightEdgeCtor     (void* num1, void* num2, const char* style = "none", const char* color = "dodgerblue");   

void    DotPrintf       (const char* text, int num = -1);

void    DotEnd          (void);   

#undef ElemType

#endif
