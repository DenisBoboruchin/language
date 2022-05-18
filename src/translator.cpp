#include "../include/translator.h"

int TranslateToAsm (CTree tree)
{
    FILE* asmFile = fopen ("execute/a.asm", "w+");

    item* root = tree.root();

    int error = Translating (asmFile, root);

    fprintf (asmFile, "\nHLT\n");
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
        case sub:       
        case add:
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
            WorkWithOP (asmFile, node->left);
            fprintf (asmFile, "PUSH 0\n");
            fprintf (asmFile, "JE DENIF%p\n\n", node);
            Translating (asmFile, node->right);
            fprintf (asmFile, "\nDENIF%p\n\n", node);
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

    fprintf (asmFile, "\n");

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

