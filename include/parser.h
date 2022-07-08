#ifndef PARSER
#define PARSER

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <sys/stat.h>

#include "../include/tree.h"

const int NOMISTAKE     =  0;
const int MAXWORDLEN    = 20;

struct sentence 
{
    const char* str         = nullptr;
    int         p           =       0;
    int         pStart      =       0; 
    int         line        =       1;
    int         size        =       0;

    int         labelPoint  =       0;
    int         labelSize   =       0;
    char**      label       = nullptr; 
};

struct item* GetGrammar (const char* str);

#define parsSymb sent->str[sent->p]

#endif
