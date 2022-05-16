#include "../include/translator.h"

int TranslateToAsm (CTree tree)
{
    FILE* asmFile = fopen ("a.asm", "w+");

    item* root = tree.root();

    int error = Translating (asmFile, root);

    fclose (asmFile);
    
    return error;
}

int Translating (FILE* asmFile, item* node)
{
    if (node->type == OP)
        WorkWithOP (asmFile, node);

    else if (node->type == CONSTR)
        WorkWithConstr (asmFile, node);

    return 0;
}

int WorkWithOP (FILE* asmFile, item* node)
{
    switch (node->data.OP)
    {
        case semicolon:
            Translating (asmFile, node->left);
            break;
       
        case mul:
        case divv:
        case add:       
        case sub:
            PrintAsmOP (asmFile, node, node->data.OP);
            break;

        case deg:
        case more:
        case smaller:
        case equ:

        case def:
        default:
            assert (!"ERROR TRANSLATING!!!");
    }

    return 0;
}

int WorkWithConstr (FILE* asmFile, item* node)
{
    switch (node->data.CONSTR)
    {
        case mif:
        {
            
            break;
        }

        case mfor:
        case mwhile:
        case str:

        default:
            assert (!"ERROR TRANSLATING!!!");
    }

    return 0;
}

int PrintAsmOP (FILE* asmFile, item* node, operate op)
{
    if (node->left->type == STRID)
        fprintf (asmFile, "PUSH [%d]\n", node->left->data.STRID);
    else
        fprintf (asmFile, "PUSH %d\n", node->left->data.INT);
    
    if (node->right->type == STRID)
        fprintf (asmFile, "PUSH [%d]\n", node->left->data.STRID);
    else
        fprintf (asmFile, "PUSH %d\n", node->left->data.INT);

    switch (op)
    {   
        case mul:
            fprintf (asmFile, "MUL\n");
            break;
        case divv:
            fprintf (asmFile, "DIV\n");
            break;
        case add:       
            fprintf (asmFile, "ADD\n");
            break;
        case sub:
            fprintf (asmFile, "SUB\n");
            break;

        default:
            assert (!"ERROR TRANSLATING!!!");
    }

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


size_t GetSizeBuf (const char* origName)
{
    struct stat stbuf;

    assert (origName != NULL);

    stat(origName, &stbuf);

    return stbuf.st_size;
}

char* CreateBuf(size_t* sizeBuf, const char* origName)
{
    assert (sizeBuf != NULL);
    assert (*sizeBuf > 0);

    char* buffer = (char*) calloc(*sizeBuf + 1, sizeof(char));
    assert (buffer != NULL);

    FILE* text = fopen(origName, "r");
    assert (text != NULL);

    size_t newSize = fread(buffer, sizeof(*buffer), *sizeBuf, text);
    assert (fclose(text) == 0);

    if (newSize < *sizeBuf)
    {
        *sizeBuf = newSize;
        buffer = (char*) realloc(buffer, *sizeBuf);
    }

    return buffer;
}
