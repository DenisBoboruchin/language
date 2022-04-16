#include "../include/parser.h"

static int GetE (sentence* sent);
static int GetT (sentence* sent);
static int GetP (sentence* sent);
static int GetN (sentence* sent);

static int PrintError (sentence* sent);

int GetG (const char* str)
{
    sentence sent = {};
    sent.str = str;
    sent.p = 0;

    int val = GetE (&sent);
    
    if (sent.str[sent.p] != '$')
    {
        PrintError (&sent);
        assert (!"SyntaxError, expected '$'");
    }

    return val;
}

static int GetE (sentence* sent)
{
    int val = 0;
    val = GetT (sent);

    while (parsSymb == '+' || parsSymb == '-')
    {
        int op = parsSymb;
        sent->p++;

        int val2 = GetT (sent);

        if (op == '+')
            val += val2;
        else
            val -= val2;
    }

    return val;
}

static int GetT (sentence* sent)
{
    int val = 0;
    val = GetP (sent);

    while (parsSymb == '*' || parsSymb == '/')
    {
        int op = parsSymb;
        sent->p++;

        int val2 = GetP (sent);

        if (op == '*')
            val *= val2;
        else
            val /= val2;   //check null
    }

    return val;
}

static int GetP (sentence* sent)
{
    if (parsSymb == '(')
    {
        sent->p++;
        int val = GetE (sent);

        if (parsSymb != ')')
        {
            PrintError (sent);
            assert (!"SyntaxError, expected ')'");
        }

        sent->p++;

        return val;
    }

    else
        return GetN (sent);
}

static int GetN (sentence* sent)
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
    
    return val;
}

static int PrintError (sentence* sent)
{
    printf ("SyntaxError!!!\n");
    printf ("%s\n", sent->str);
    printf ("%*s\n", sent->p, "^");

    return NOMISTAKE;
}
