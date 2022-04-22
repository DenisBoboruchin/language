#ifndef PARSER
#define PARSER

#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "../include/tree.h"

const int NOMISTAKE     =  0;
const int MAXWORDLEN    = 20;

struct sentence 
{
    const char* str = nullptr;
    int         p   =       0;  
};

struct item* GetGrammar (const char* str);

#define parsSymb sent->str[sent->p]

#endif
