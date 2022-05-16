#ifndef NRANSLATOR
#define TRANSLATOR

#include <stdio.h>
#include <sys/stat.h>

#include "../include/tree.h"
#include "../include/parser.h"

size_t      GetSizeBuf      (const char* origName);
char*       CreateBuf       (size_t* sizeBuf, const char* origName);

int         TranslateToAsm  (CTree tree);

int         Translating     (FILE* asmFile, item* node);
int         WorkWithOP      (FILE* asmFile, item* node);
int         WorkWithConstr  (FILE* asmFile, item* node);

int         PrintAsmOP      (FILE* asmFile, item* node, operate op);

#endif
