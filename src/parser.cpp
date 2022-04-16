#include "../include/parser.h"

const char* str = nullptr;
int         p   =       0;

int GetG (const char* s)
{
    str = s;
    p = 0;

    int val = GetE ();
    assert (str[p] == '$');

    return val;
}

int GetE ()
{
    int val = 0;
    val = GetN ();

    int op = str[p];
    while (op == '+' || op == '-')
    {
        op = str[p];
        p++;

        int val2 = GetN ();

        if (op == '+')
            val += val2;
        else
            val -= val2;
    }

    return val;
}

int GetN ()
{
    int val = 0;

    while ('0' <= str[p] && str[p] <= '9')
    {
        val = val * 10 + (str[p] - '0');
        p++;
    }

    return val;
}
