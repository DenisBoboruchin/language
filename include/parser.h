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

int GetG (const char* s);
static int GetE ();
static int GetT ();
static int GetP ();
static int GetN ();

static int PrintError ();

#endif
