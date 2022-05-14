#include "../include/parser.h"

static item* GetStr                 (sentence* sent);
static item* GetIf                  (sentence* sent);
static item* GetPrimaryBody         (sentence* sent);
static item* GetEqual               (sentence* sent);
static item* GetPrimaryComparison   (sentence* sent);
static item* GetComparison          (sentence* sent);
static item* GetExpression          (sentence* sent);
static item* GetTerm                (sentence* sent);
static item* GetDegree              (sentence* sent);
static item* GetPrimaryExpression   (sentence* sent);
static item* GetSign                (sentence* sent);
static item* GetNumber              (sentence* sent);
static item* GetWord                (sentence* sent);

static int PrintError               (sentence* sent);

static constr CheckConstruction        (const char* word);

item* GetGrammar (const char* str)
{
    sentence sent = {};
    sent.str = str;
    sent.p = 0;

    item* node = GetStr (&sent);
    
    if (sent.str[sent.p] != '$')
    {
        PrintError (&sent);
        assert (!"SyntaxError, expected '$'");
    }

    return node;
}

static item* GetStr (sentence* sent)
{
    item* nodeLeft = GetIf (sent);
    item* node = nodeLeft;

    if (parsSymb != ';')
    {
        PrintError (sent);
        assert (!"SyntaxError, expected ';'");
    }

    while (parsSymb == ';')
    {
        nodeLeft = node;
        sent->p++;
    
        item* nodeRight = GetIf (sent);
 
        if (nodeRight->type == ERR)
            return node;

        if (parsSymb != ';')
        {
            PrintError (sent);
            assert (!"SyntaxError, expected ';'");
        }

        node = new item;
        node->type = OP;

        node->data.OP = semicolon;

        node->left = nodeLeft;

        node->right = nodeRight;
    }

    return node;
}

static item* GetIf (sentence* sent)
{
    int temp = sent->p;
    item* node = GetWord (sent);

    if (node->data.CONSTR != mif)
    {
        sent->p = temp;
        delete[] node;
        return GetEqual (sent);
    }

    node->left = GetPrimaryComparison (sent);            // can't =

    node->right = GetPrimaryBody (sent);                             // primary mb...

    return node;
}

static item* GetPrimaryBody (sentence* sent)
{ 
    if (parsSymb != '{')
    {
        PrintError (sent);
        assert (!"SyntaxError, expected '{'");
    }
    sent->p++;

    item* node = GetStr (sent);

    if (parsSymb != '}')
    {
        PrintError (sent);
        assert (!"SyntaxError, expected '}'");
    }
    sent->p++;

    return node;
}

static item* GetEqual (sentence* sent)
{
    item* temp = GetExpression (sent);                      //
    if ((temp->type == ERR) || (temp->type == CONSTR))
    { 
        if (parsSymb == '=')
        {  
            PrintError (sent);
            assert (!"SyntaxError, expected '='");
        }
        else
            return temp;                                    //
    }

    if (parsSymb != '=')
        return temp;                                        //

    sent->p++;
    item* node = new item;
    node->type = OP;
    node->data.OP = equ;

    node->left = temp;
    node->right = GetComparison (sent);

    return node;
}

static item* GetPrimaryComparison (sentence* sent)
{
    if (parsSymb == '(')
    {
        sent->p++;
        item* node = GetComparison (sent);

        if (parsSymb != ')')
        {
            PrintError (sent);
            assert (!"SyntaxError, expected ')'");
        }

        sent->p++;

        return node;
    }

    else
    {
        PrintError (sent);
        assert (!"SyntaxError, expected '('");
    }
}
///////////////////////////////////////////////////////////////////////////
static item* GetComparison (sentence* sent)
{
    item* temp = GetExpression (sent);
    if ((temp->type == ERR) || (temp->type == CONSTR))
    { 
        if ((parsSymb == '>') || (parsSymb == '<'))
        {  
            PrintError (sent);
            assert (!"SyntaxError, expected '='");
        }
        else
            return temp;
    }

    int op = parsSymb;
    if ((op != '>') && (op != '<'))
        return temp;

    sent->p++;
    item* node = new item;
    node->type = OP;
    if (op == '>')
        node->data.OP = more;
    else
        node->data.OP = smaller;

    node->left = temp;
    node->right = GetExpression (sent);

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
            item* node = new item;
            
            node->type = OP;
            node->data.OP = sub;
            node->left = GetDegree (sent);

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

        if (('a' <= parsSymb && parsSymb <= 'z') || ('A' <= parsSymb && parsSymb <= 'Z'))
        {
            PrintError (sent);
            assert (!"SyntaxError");
        }
    }

    item* node = nullptr;
    if (tempP != sent->p)
    {
        node = new item;
        node->type = INT;
        node->data.INT = val;
    }

    else
        node = GetWord (sent);

    return node;
}

static item* GetWord (sentence* sent)
{
    char* word = new char[MAXWORDLEN];
    int counter = 0;

    int tempP = sent->p;

    while (('a' <= parsSymb && parsSymb <= 'z') || ('A' <= parsSymb && parsSymb <= 'Z'))
    {
        word[counter] = parsSymb;
        sent->p++;
        counter++;

        if (counter >= MAXWORDLEN - 1)
            assert (!"SyntaxError!!! very long word");
    }
    word[counter] = '\0';

    item* node = new item;

    if (tempP == sent->p)
    {
        node->type = ERR;
        
        return node;
        //PrintError (sent);
        //assert (!"SyntaxError");
    }
    
    constr checkConstr = CheckConstruction (word);
    if (checkConstr)
    {
        node->type = CONSTR;
        node->data.CONSTR  = checkConstr;
    }

    else
    {
        node->type = STR;
        node->data.STR  = word;
    } 
    return node;
}

static constr CheckConstruction (const char* word)
{
    if (!strcmp (word, "if"))
        return mif;

    else if (!strcmp (word, "for"))
        return mfor;

    else if (!strcmp (word, "while"))
        return mwhile;

    return str;
}

static int PrintError (sentence* sent)
{
    printf ("SyntaxError!!!\n");
    printf ("%s\n", sent->str);
    printf ("%*s\n", sent->p, "^");

    return NOMISTAKE;
}
