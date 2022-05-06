#ifndef TREE
#define TREE

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../include/parser.h"

const int           DESTROYSZ   =     -12323;

extern const int    NOMISTAKE;
const int           MISTAKE     =          1;
                                
const int           EMPTY       =       9234;
const int           NOTFOUND    =       -583;

const int           STARTNUM    =          7;

const int           MAXANSWSIZE =         50;

#define ElemType const char*

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
    ERR         =   -442,
    DBL         =    512,
    CHR         =    123,
    INT         =    213,
    STR         =    334,
    OP          =   2140,
    CONSTR      =    777
};

enum operate
{
    def     =   '.',
    mul     =   '*',
    divv    =   '/',
    add     =   '+',
    sub     =   '-',
    deg     =   '^',
    equ     =   '='
};

enum constr
{
    str     =   0,
    mif          ,
    mwhile       ,
    mfor  
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
    double          DBL     =       0;
    char            CHR     =       0;
    const char*     STR     = nullptr;
    operate         OP      =     def;
    constr          CONSTR  =     str;
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

#include "graphviz.h"

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
    explicit        CTree               (item* node);
                    ~CTree              ();

                    CTree               (const CTree& tree);

    struct item*    addItm              (struct item* prefItm, ElemType data, int mode = STANDART);
    struct item*    findItm             (ElemType data);

    void            addBuffer           (char* buffer);

    CTree           operator=           (const CTree& tree);

    item*           root                ();
    int             graphDump           ();
};

item* CopyTree (item* node);
item* CtorItem (typeNode type, structData data, item* left, item* right, item* parent = nullptr);
item* CopyItem (item* item);
int CtorElemsForDot             (struct item* node);
int CtorEdgeForDot              (struct item* node);

int Compare                     (ElemType data, ElemType object);

//#undef ELEM_FMT

#endif
