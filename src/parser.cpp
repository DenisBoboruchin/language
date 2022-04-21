#include "../include/parser.h"

static item* GetExpression        (sentence* sent);
static item* GetTerm              (sentence* sent);
static item* GetDegree            (sentence* sent);
static item* GetPrimaryExpression (sentence* sent);
static item* GetSign              (sentence* sent);
static item* GetNumber            (sentence* sent);

static int PrintError           (sentence* sent);

item* GetGrammar (const char* str)
{
    sentence sent = {};
    sent.str = str;
    sent.p = 0;

    item* node = GetExpression (&sent);
    
    if (sent.str[sent.p] != '$')
    {
        PrintError (&sent);
        assert (!"SyntaxError, expected '$'");
    }

    return node;
}

static item* GetExpression (sentence* sent)
{
    item* nodeLeft = GetTerm (sent);
    item* node = nodeLeft;

    while (parsSymb == '+' || parsSymb == '-')
    {
        int op = parsSymb;
        sent->p++;

        node = new item;
        node->type = OP;

        item* nodeRight = GetTerm (sent);

        if (op == '+')
            node->data.OP = add;
        else
            node->data.OP = sub;

        node->left = nodeLeft;
        node->right = nodeRight;
    }

    return node;
}

static item* GetTerm (sentence* sent)
{
    item* nodeLeft = GetSign (sent);
    item* node = nodeLeft;

    while (parsSymb == '*' || parsSymb == '/')
    {
        int op = parsSymb;
        sent->p++;

        node = new item;
        node->type = OP;

        item* nodeRight = GetSign (sent);

        if (op == '*')
            node->data.OP = mul;
        else
            node->data.OP = divv;  //check null
     
        node->left = nodeLeft;
        node->right = nodeRight;
    }

    return node;
}

static item* GetSign (sentence* sent)
{
    int op = parsSymb;

    if (op == '+' || op == '-')
    {
        sent->p++;

        if ((parsSymb == '+') || (parsSymb == '-'))         //check -+-+--+ - incorrect
        {                                                   //      3 +- 15 -   correct
            PrintError (sent);
            assert (!"SyntaxError");
        }

        else if (op == '-')
        {
            item* node = GetDegree (sent);
            node->data.INT = - node->data.INT;
            
            return node;
        }
    }

    return GetDegree (sent);
}

static item* GetDegree (sentence* sent)
{
    item* nodeLeft = GetPrimaryExpression (sent);
    item* node = nodeLeft;

    while (parsSymb == '^')
    {
        sent->p++;

        item* nodeRight = GetDegree (sent);

        node = new item;
        node->type = OP;
        node->data.OP = deg;

        node->left = nodeLeft;
        node->right = nodeRight;
        //val = (int) pow (val, val2);
    }

    return node;
}

static item* GetPrimaryExpression (sentence* sent)
{
    if (parsSymb == '(')
    {
        sent->p++;
        item* node = GetExpression (sent);

        if (parsSymb != ')')
        {
            PrintError (sent);
            assert (!"SyntaxError, expected ')'");
        }

        sent->p++;

        return node;
    }

    else
        return GetNumber (sent);
}

static item* GetNumber (sentence* sent)
{
    int val = 0;
    int tempP = sent->p;

    while ('0' <= parsSymb && parsSymb <= '9')
    {
        val = val * 10 + (parsSymb - '0');
        sent->p++;
    }

    if (tempP == sent->p)
    {
        PrintError (sent);
        assert (!"SyntaxError");
    }
    
    item* node = new item;
    node->type = INT;
    node->data.INT = val;

    return node;
}

static int PrintError (sentence* sent)
{
    printf ("SyntaxError!!!\n");
    printf ("%s\n", sent->str);
    printf ("%*s\n", sent->p, "^");

    return NOMISTAKE;
}
