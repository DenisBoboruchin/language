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
            Translating (asmFile, node->right);
            fprintf (asmFile, "\n\n\n");
            break;
       
        case mul:
        case divv:
        case sub:       
        case add:
            PrintAsmOP (asmFile, node);
            break;
        
        case equ:
            PrintAsmEqu (asmFile, node);
            break;

        case more:
        case smaller:
        case ordinary:
            PrintAsmComp (asmFile, node);
            break;

        case deg:
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
            if (node->left->type == OP)
                WorkWithOP (asmFile, node->left);
            else
                fprintf (asmFile, "PUSH %d\n", node->left->data.INT);
                    
            fprintf (asmFile, "PUSH 0\n");
            fprintf (asmFile, "JE DENIF%p\n\n", node);
            Translating (asmFile, node->right);
            fprintf (asmFile, "\nDENIF%p\n\n", node);
            break;
        }

        case mprintf:
        {
            fprintf (asmFile, "\n\nPUSH [%d]\nOUT\n\n\n", node->left->data.STRID);
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

int PrintAsmOP (FILE* asmFile, item* node)
{
    if (!node->left)
        fprintf (asmFile, "PUSH 0\n");
    else if (node->left->type == STRID)
        fprintf (asmFile, "PUSH [%d]\n", node->left->data.STRID);
    else if (node->left->type == INT)
        fprintf (asmFile, "PUSH %d\n", node->left->data.INT);
    else
        WorkWithOP (asmFile, node->left);
        
    if (node->right->type == STRID)
        fprintf (asmFile, "PUSH [%d]\n", node->right->data.STRID);
    else if (node->right->type == INT)
        fprintf (asmFile, "PUSH %d\n", node->right->data.INT);
    else
        WorkWithOP (asmFile, node->right);

    switch (node->data.OP)
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

int PrintAsmEqu (FILE* asmFile, item* node)
{
    if (node->right->type == STRID)
        fprintf (asmFile, "PUSH [%d]\n", node->right->data.STRID);
    else if (node->right->type == INT)
        fprintf (asmFile, "PUSH %d\n", node->right->data.INT);
    else
        WorkWithOP (asmFile, node->right);    
    
    fprintf (asmFile, "POP  [%d]\n", node->left->data.STRID);
    fprintf (asmFile, "PUSH 1\n\n");

    return 0;
}

int PrintAsmComp (FILE* asmFile, item* node)
{
    /*
    fprintf (asmFile, "PUSH 1\n");
    
    if (node->left->type == STRID)
        fprintf (asmFile, "PUSH [%d]\n", node->left->data.STRID);
    else if (node->left->type == INT)
        fprintf (asmFile, "PUSH %d\n", node->left->data.INT);
    else
        WorkWithOP (asmFile, node->left);

    if (node->right->type == STRID)
        fprintf (asmFile, "PUSH [%d]\n", node->right->data.STRID);
    else if (node->right->type == INT)
        fprintf (asmFile, "PUSH %d\n", node->right->data.INT);
    else
        WorkWithOP (asmFile, node->right);

    fprintf (asmFile, "\nSUB\n\nPUSH 0\n");
    fprintf (asmFile, "JB COMPARE%p\n", node);

    if (node->data.OP == more)
        fprintf (asmFile, "POP rdx\nPUSH 0\n");

    fprintf (asmFile, "COMPARE%p\n", node);

    if (node->data.OP == smaller)
        fprintf (asmFile, "POP rdx\nPUSH 0\n\n");
 
    return 0;
    */
  
    if (node->left->type == STRID)
        fprintf (asmFile, "PUSH [%d]\n", node->left->data.STRID);
    else if (node->left->type == INT)
        fprintf (asmFile, "PUSH %d\n", node->left->data.INT);
    else
        WorkWithOP (asmFile, node->left);

    if (node->right->type == STRID)
        fprintf (asmFile, "PUSH [%d]\n", node->right->data.STRID);
    else if (node->right->type == INT)
        fprintf (asmFile, "PUSH %d\n", node->right->data.INT);
    else
        WorkWithOP (asmFile, node->right);

    
    fprintf (asmFile, "\nSUB\nPOP rdx\n");
    
    fprintf (asmFile, "PUSH rdx\nPUSH 0\n");
    fprintf (asmFile, "JB COMPAREB%p\n", node);
    
    fprintf (asmFile, "PUSH rdx\n PUSH 0\n");
    fprintf (asmFile, "JA COMPAREA%p\n", node);

    if (node->data.OP == ordinary)
        fprintf (asmFile, "PUSH 1\n");
    else
        fprintf (asmFile, "PUSH 0\n");
 
    fprintf (asmFile, "JMP JMPEXIT%p\n", node);

////////////////////////////////////////////////////////////////      
    fprintf (asmFile, "COMPAREA%p\n", node);

    if (node->data.OP == more)
        fprintf (asmFile, "PUSH 1\n");
    else
        fprintf (asmFile, "PUSH 0\n");
    
    fprintf (asmFile, "JMP JMPEXIT%p\n", node);
    
////////////////////////////////////////////////////////////////
    fprintf (asmFile, "COMPAREB%p\n", node);
    
    if (node->data.OP == smaller)
        fprintf (asmFile, "PUSH 1\n");
    else 
        fprintf (asmFile, "PUSH 0\n");

    fprintf (asmFile, "JMPEXIT%p\n\n", node);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

