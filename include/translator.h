#ifndef TRANSLATOR
#define TRANSLATOR

#include <stdio.h>
#include <sys/stat.h>

#include "../include/tree.h"
#include "../include/parser.h"

int         TranslateToAsm  (CTree tree);

int         Translating     (FILE* asmFile, item* node);
int         WorkWithOP      (FILE* asmFile, item* node);
int         WorkWithConstr  (FILE* asmFile, item* node);

int         PrintAsmOP      (FILE* asmFile, item* node);
int         PrintAsmEqu     (FILE* asmFile, item* node);
int         PrintAsmComp    (FILE* asmFile, item* node);
int         PrintAsmFor     (FILE* asmFile, item* node);

#endif
