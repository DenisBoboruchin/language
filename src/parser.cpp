#include "../include/parser.h"

const char* str = nullptr;
int         p   =       0;

int GetG (const char* s)
{
    str = s;
    p = 0;

    int val = GetE ();
    
    if (str[p] != '$')
    {
        PrintError ();
        assert (!"SyntaxError, expected '$'");
    }

    return val;
}

static int GetE ()
{
    int val = 0;
    val = GetT ();

    while (str[p] == '+' || str[p] == '-')
    {
        int op = str[p];
        p++;

        int val2 = GetT ();

        if (op == '+')
            val += val2;
        else
            val -= val2;
    }

    return val;
}

static int GetT ()
{
    int val = 0;
    val = GetP ();

    while (str[p] == '*' || str[p] == '/')
    {
        int op = str[p];
        p++;

        int val2 = GetP ();

        if (op == '*')
            val *= val2;
        else
            val /= val2;   //check null
    }

    return val;
}

static int GetP ()
{
    if (str[p] == '(')
    {
        p++;
        int val = GetE ();

        if (str[p] != ')')
        {
            PrintError ();
            assert (!"SyntaxError, expected ')'");
        }

        p++;

        return val;
    }

    else
        return GetN ();
}

static int GetN ()
{
    int val = 0;
    int tempP = p;

    while ('0' <= str[p] && str[p] <= '9')
    {
        val = val * 10 + (str[p] - '0');
        p++;
    }

    if (tempP == p)
    {
        PrintError ();

        assert (!"SyntaxError");
    }
    
    return val;
}

static int PrintError ()
{
    printf ("SyntaxError!!!\n");
    printf ("%s\n", str);
    printf ("%*s\n", p, "^");

    return NOMISTAKE;
}
