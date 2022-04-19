#include "../include/parser.h"

static int GetExpression        (sentence* sent);
static int GetTerm              (sentence* sent);
static int GetDegree            (sentence* sent);
static int GetPrimaryExpression (sentence* sent);
static int GetSign              (sentence* sent);
static int GetNumber            (sentence* sent);

static int PrintError           (sentence* sent);

int GetGrammar (const char* str)
{
    sentence sent = {};
    sent.str = str;
    sent.p = 0;

    int val = GetExpression (&sent);
    
    if (sent.str[sent.p] != '$')
    {
        PrintError (&sent);
        assert (!"SyntaxError, expected '$'");
    }

    return val;
}

static int GetExpression (sentence* sent)
{
    int val = GetTerm (sent);

    while (parsSymb == '+' || parsSymb == '-')
    {
        int op = parsSymb;
        sent->p++;

        int val2 = GetTerm (sent);

        if (op == '+')
            val += val2;
        else
            val -= val2;
    }

    return val;
}

static int GetTerm (sentence* sent)
{
    int val = GetSign (sent);

    while (parsSymb == '*' || parsSymb == '/')
    {
        int op = parsSymb;
        sent->p++;

        int val2 = GetSign (sent);

        if (op == '*')
            val *= val2;
        else
            val /= val2;   //check null
    }

    return val;
}

static int GetSign (sentence* sent)
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
            return - GetDegree (sent);
    }

    return GetDegree (sent);
}

static int GetDegree (sentence* sent)
{
    int val = GetPrimaryExpression (sent);

    while (parsSymb == '^')
    {
        sent->p++;

        int val2 = GetDegree (sent);

        val = (int) pow (val, val2);
    }

    return val;
}

static int GetPrimaryExpression (sentence* sent)
{
    if (parsSymb == '(')
    {
        sent->p++;
        int val = GetExpression (sent);

        if (parsSymb != ')')
        {
            PrintError (sent);
            assert (!"SyntaxError, expected ')'");
        }

        sent->p++;

        return val;
    }

    else
        return GetNumber (sent);
}

static int GetNumber (sentence* sent)
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
