#ifndef PARSER
#define PARSER

#include <stdio.h>
#include <assert.h>

const int NOMISTAKE = 0;

struct sentence 
{
    const char* str = nullptr;
    int         p   =       0;  
};

int GetGrammar (const char* str);

#define parsSymb sent->str[sent->p]

#endif
