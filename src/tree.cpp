#include "../include/tree.h"

#ifdef STR
    typedef const char* ElemType;

    const char*     INITIAL     =     "init";
    const char*     DESTROYED   =     "dest";
#endif

#ifdef INT
    typedef int ElemType;

    const int       INITIAL     =     111111;
    const int       DESTROYED   =      -4942;
#endif

#ifdef DOUBLE
    typedef double ElemType;

    double          INITIAL     =     111111;
    double          DESTROYED   =      -7234;
#endif

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

CTree::CTree () :
    treeRoot_   (nullptr),
    size_       (0) 
    {}

CTree::CTree (item* node):
    treeRoot_   (node),
    size_       (0)
    {}

CTree::~CTree ()
{
    printf ("Вызван деструктор\n");
    TreeDtor_ (treeRoot_);
}

void CTree::TreeDtor_ (struct item* node)
{
    if (node == nullptr)
        return ;

    if (buff.buffer_)
    {
        free (buff.buffer_);
        buff.buffer_ = nullptr;
    }

    if (node->left)
        TreeDtor_ (node->left);
    if (node->right)
        TreeDtor_ (node->right);

    //node->data = DESTROYED;
    delete[] node;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

struct item* CTree::addItm (struct item* prefItm, ElemType data, int mode)
{
    if (CheckEmpty_ (prefItm) == EMPTY)         //check tree on empty
    {
        treeRoot_->data.STR = data;
         
        return treeRoot_;
    }
                
    struct item* newItm = new item;
    
    newItm->data.STR = data;
 
    switch (mode)
    {
        case STANDART:
            CompareAndAddItm_ (prefItm, newItm);
            break;

        case ADDLEFT:
            prefItm->left = newItm;
            break;

        case ADDRIGHT:
            prefItm->right = newItm;
            break;

        default:
            assert (!"ERROR!!! Unknown mode for CTree::addItm\n");
    }

    return newItm;
}

int CTree::CheckEmpty_ (struct item* prefItm)
{
    if (!prefItm)
    {
        if (!treeRoot_)
        {
            treeRoot_ = new item;        

            return EMPTY;
        }

        else
        {
            printf ("ERROR!!! Tree is not empty\n");
            printf ("ERROR!!! Expected not null pointer\n");
            assert (prefItm);
        }
    }

    return NOMISTAKE;
}

int CTree::CompareAndAddItm_ (struct item* prefItm, struct item* newItm)
{
    switch (Compare (newItm->data.STR, prefItm->data.STR))
    {
        case LEFT:
        {
            if (prefItm->left)
                printf ("This item filled\n");
            else
                prefItm->left = newItm;
            
            return NOMISTAKE;
        }

        case RIGHT:
        {
            if (prefItm->right)
                printf ("This item filled\n");
            else
                prefItm->right = newItm;
            
            return NOMISTAKE;
        }

        default:
        {
            printf ("ERROR!!! Compare error!\n");
            return MISTAKE;
        }
    }
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

item* CtorItem (typeNode type, structData data, item* left, item* right, item* parent)
{
    item* newItm = new item;
    newItm->type = type;
    newItm->data = data;
    newItm->left = left;
    newItm->right = right;
    newItm->parent = parent;

    return newItm;
}

item* CTree::root ()
{
    return treeRoot_;
}

int Compare (ElemType data, ElemType object)
{
    #ifndef STR
        if (data > object)
            return RIGHT;
        
        else if (data < object)
            return LEFT;
        
        else if (data == object)
            return EQUAL;
    #endif

    #ifdef STR
        if (!strcmp (data, object))
            return EQUAL;
            
        else if (strlen (data) > strlen (object))
            return RIGHT;
        
        else if (strlen (data) <= strlen (object))
            return LEFT;
    #endif

    return NOCOMP;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

struct item* CTree::findItm (ElemType data)
{
    struct item* item = treeRoot_; 

    while (item)
    {  
        //printf ("data: \"%s\"\n obj : \"%s\"\n\n", data, item->data);
        switch (Compare (data, item->data.STR))
        { 
            case LEFT:
                item = item->left;
                break;

            case RIGHT:
                item = item->right;
                break;

            case EQUAL:
                return item;
                
            default:
                printf ("ERROR!!! Compare error!\n");
                break;
        }
    }

    printf ("\"");
    printf ("%s", data);
    printf ("\" not found\n");

    return nullptr;
}

void CTree::addBuffer (char* buffer)
{
    buff.buffer_ = buffer;
    buff.buffForNew = (char*) calloc (STARTNUM, MAXANSWSIZE); 
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

int CTree::TreeOk_ ()
{
    printf ("%s\n", treeRoot_->data.STR);

    return NOMISTAKE;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

int CTree::graphDump ()
{
    DotCtor ("TB", "darkkhaki");

    CtorElemsForDot (treeRoot_);

    CtorEdgeForDot (treeRoot_);

    DotEnd ();

    return NOMISTAKE;
}

int CtorElemsForDot (struct item* node)
{
    if (!node)  
    {
        printf ("Tree is empty\n");
        return NOMISTAKE;
    }  

    DotElemCtor (node, node->type, node->data, node->left, node->right);

    if (node->left)
        CtorElemsForDot (node->left);
    if (node->right)
        CtorElemsForDot (node->right);

    return NOMISTAKE;
}

int CtorEdgeForDot (struct item* node)
{
    if (!node)  
        return NOMISTAKE;  

    if (node->left)
    {
        DotLeftEdgeCtor (node, node->left);

        CtorEdgeForDot (node->left);
    }
    
    if (node->right)
    {
        DotRightEdgeCtor (node, node->right);

        CtorEdgeForDot (node->right);
    }

    return NOMISTAKE;
}



/*
int CTree::delItm (struct item* node)
{
   
}
*/
