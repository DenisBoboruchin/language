#ifndef TREE
#define TREE

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "graphviz.h"
#include "../include/parser.h"

const int           DESTROYSZ   =     -12323;

extern const int    NOMISTAKE;
const int           MISTAKE     =          1;
                                
const int           EMPTY       =       9234;
const int           NOTFOUND    =       -583;

const int           STARTNUM    =          7;

const int           MAXANSWSIZE =         50;

// для добавления в конкретно правую или левую ветку
enum addMode
{
    ADDLEFT     =    123,
    ADDRIGHT            ,
    STANDART
};

// для определения результата сравнения 
enum comp
{
    LEFT        =   3523,
    RIGHT               ,
    EQUAL               ,
    NOCOMP
};

// тип узла 
enum typeNode
{
    DBL         =    512,
    CHR         =    123,
    INT         =    213,
    OPERATOR    =   2139
};

enum operate
{
    def     =   '.',
    mul     =   '*',
    sub     =   '+'
};

//-------------------------------------------------------------------------
//--------------------------TYPE_OF_THE_TREE-------------------------------
//-------------------------------------------------------------------------
//#define ELEM_FMT "%s"

//typedef const char* ElemType;

//данные узла (юнион)
struct structData
{
    int             INT     =       0;
    double          DOUBLE  =       0;
    char            CHR     =       0;
    const char*     STR     = nullptr;
    operate         OP      =     def;
};

struct item
{
    typeNode        type        =        INT;
	structData    	data	    =	      {};

    struct item*    parent      =    nullptr;

    struct item*	left	    =	 nullptr;
    struct item*    right       =    nullptr;
};

struct buf
{
    char*           buffer_     =    nullptr;
    char*           buffForNew  =    nullptr;
    int             counter     =          0;
};

class CTree
{
private:
	struct item*    treeRoot_   =    nullptr; 
    int             size_       =          0;

    int             TreeOk_             ();
    int             CheckEmpty_         (struct item* prefItm);
    void            TreeDtor_           (struct item* node);
    int             CompareAndAddItm_   (struct item* prefI, struct item* newI);

public:
    struct buf      buff        =         {};
    
    explicit        CTree               ();
                    ~CTree              ();

                    CTree               (const CTree& obj);

    struct item*    addItm              (struct item* prefItm, ElemType data, int mode = STANDART);
    struct item*    findItm             (ElemType data);

    void            addBuffer           (char* buffer);

    item*           root                ();
    int             graphDump           ();
};

int CtorElemsForDot             (struct item* node);
int CtorEdgeForDot              (struct item* node);

int Compare                     (ElemType data, ElemType object);

//#undef ELEM_FMT

#endif
