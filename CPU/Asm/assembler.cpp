#include "assembler.h"

#define DEF_CMD(name, num, ...)                                                                         \
    if (!strcmp((strings + index)->str, #name))                                                         \
    {                                                                                                   \
        *(codeMassive + *size) = (char) num;                                                            \
        *size += sizeof(char);                                                                          \
                                                                                                        \
        if ((num & 0x40) && (num & 0x80) && (strcmp(#name, "RET")))                                     \
        {                                                                                               \
            if (*nPass == FIRSTPASS)                                                                    \
            {                                                                                           \
                index++;                                                                                \
                *size += sizeof(int);                                                                   \
                                                                                                        \
                continue;                                                                               \
            }                                                                                           \
                                                                                                        \
                                                                                                        \
            ERROR |= CheckTypeJmp(#name, strings, &index, codeMassive, size, labels, *nJMP);            \
                                                                                                        \
            continue;                                                                                   \
        }                                                                                               \
                                                                                                        \
        if (num & 0x10)                                                                                 \
        {                                                                                               \
            index++;                                                                                    \
            SkipTabs(strings, &index);                                                                  \
                                                                                                        \
            ERROR |= CheckTypeARG((strings + index)->str, codeMassive, size, num);                      \
                                                                                                        \
            continue;                                                                                   \
        }                                                                                               \
                                                                                                        \
        continue;                                                                                       \
    }

static const char* BINCODE = "execute/binCode.bin";

char* Assembler(const char* CMD)
{
    size_t  sizeBuf = GetSizeBuf(CMD);                                                           //from "SortText.h"
    char*   buffer  = CreateBuf(&sizeBuf, CMD);                                                  //from "SortText.h"
   
    size_t numLines          = NumberOfLines (buffer, sizeBuf);
    struct pointStr* strings = CrtorStrs (numLines, sizeBuf, buffer);                            //from "SortText.h"

    char*  codeMassive   = (char*)         calloc (numLines + 1, sizeof(int));
    struct Label* labels = (struct Label*) calloc (NUMLBL, sizeof(struct Label));

    int sizeCode = STARTSIZE;
    int nJMP     = STARTnJMP;
    int nPass    = FIRSTPASS;

    printf ("start first\n");
    int ERRORFLAG = Assembling(strings, codeMassive, &sizeCode, (int) numLines, &labels, &nJMP, &nPass);
    printf ("start second\n");
    ERRORFLAG    |= Assembling(strings, codeMassive, &sizeCode, (int) numLines, &labels, &nJMP, &nPass);
    printf ("end asemb\n");

    OutPutLabel(labels, nJMP);

    codeMassive = (char*) realloc(codeMassive, (sizeCode + 1) * sizeof(char));

    FILE* binCode = fopen(BINCODE, "wb");

    assert ((int) fwrite(codeMassive, sizeof(char), sizeCode + 1, binCode) == (int) sizeCode + 1);
    assert (fclose(binCode) == 0);

    printf ("end\n");
    //free(codeMassive);
    //free(buffer);
    //free(strings);
    //free(labels);

    Verification(ERRORFLAG);

    return codeMassive;
}

size_t NumberOfLines(char* buffer, const size_t sizeBuf)
{
    assert (buffer != NULL);
    assert (sizeBuf > 0);

    size_t numLines = 0;

    for (size_t i = 0; i < sizeBuf; i++)
    {
        if ((*(buffer + i) == '\n') || (*(buffer + i) == ' ') || (*(buffer + i) == '\t'))
        {
            *(buffer + i) = '\0';
            numLines++;
        }
    }

    return numLines;
}

int Assembling(struct pointStr* strings, char* codeMassive, int* size, int numLines, struct Label** labels, int* nJMP, int* nPass)
{
    assert (strings != NULL);
    assert (codeMassive != NULL);
    assert (size != NULL);
    assert (*size >= 0);
    assert (numLines >= 0);

    *size = 0;

    int ERROR  = NOMISTAKE;
    int HLTFLG = 0;

    for (int index = 0; index < numLines; index++)
    {
        SkipTabs(strings, &index);

        HLTFLG |= CheckHLT((strings + index)->str);

        #include "../commands.h"

        if (*nPass == FIRSTPASS)
            ERROR |= AddToLabel((strings + index)->str, *size, labels, nJMP);
    }
    #undef DEF_CMD

    if (*nPass == SECONDPASS)
    {
        ERROR |= CheckLabels(labels, *nJMP);
        LogHLT(HLTFLG);
    }

    (*nPass)++;

    return ERROR || (!HLTFLG);
}

int CheckTypeARG(char* str, char* codeMassive, int* size, int num)
{
    assert (str != NULL);
    assert (codeMassive != NULL);
    assert (size != NULL);
    assert (*size >= 0);

    if (char result = CheckOnRegs(str))
    {
        *(codeMassive + *size - 1) = (char) num | (1 << 5);
        *(codeMassive + *size) = result;

        *size += sizeof(char);

        return NOMISTAKE;
    }

    int value = 0;

    if (value = CheckOnNum(str))
    {
        int check = CheckArgOnNum(num);                         //��� POP

        if (value == 213481801)
            value = 0;

        *((int*) (codeMassive + *size)) = value;
        *size += sizeof(int);

        return check;
    }

    if (value = CheckOnMem(str))
    {
        *(codeMassive + *size - 1) = (char) num | (1 << 6);
        
        if (value == 213481801)
            value = 0;

        *((int*) (codeMassive + *size)) = value;

        *size += sizeof(int);

        return NOMISTAKE;
    }

    return CheckCmd(str, ERRORCMD);
}

int SkipTabs(struct pointStr* strings, int* index)
{
    while (!strlen((strings + *index)->str))
        (*index)++;
}

int CheckTypeJmp(const char* name, struct pointStr* strings, int* index, char* codeMassive, int* size, struct Label** labels, int nJMP)
{
    assert (name != NULL);
    assert (strings != NULL);
    assert (index != NULL);
    assert (*index >= 0);
    assert (codeMassive != NULL);
    assert (*size >= 0);
    assert (nJMP >= 0);

    (*index)++;
    SkipTabs(strings, index);

    if (!strcmp("CALL", name))
        return WorkWthCall((strings + *index)->str, codeMassive, size, labels, nJMP);

    return WorkWthJMP((strings + *index)->str, codeMassive, size, labels, nJMP);
}

int WorkWthCall(char* str, char* codeMassive, int* size, struct Label** labels, int nJMP)
{
    int num = FindMarkLabel(str, labels, nJMP);
    if (num != NOTFOUND)
    {
        *((int*) (codeMassive + *size)) = num;
        *size += sizeof(int);

        return NOMISTAKE;
    }

    printf("incorrect call argument\n");
    fprintf(logAsm, "incorrect call argument\n");

    return MISTAKE;
}

int WorkWthJMP(char* str, char* codeMassive, int* size, struct Label** labels, int nJMP)
{
    if (int value = CheckOnNum(str))
    {
        *((int*) (codeMassive + *size)) = value;
        *size += sizeof(int);

        return NOMISTAKE;
    }

    int num = FindMarkLabel(str, labels, nJMP);
    if (num != NOTFOUND)
    {
        *((int*) (codeMassive + *size)) = num;
        *size += sizeof(int);

        return NOMISTAKE;
    }

    printf("incorrect jmp argument %s\n", str);
    fprintf(logAsm, "incorrect jmp argument\n");

    return MISTAKE;
}

int FindMarkLabel(char* str, struct Label** labels, int nJMP)
{
    for (int num = 0; num < nJMP; num++)
        if (!strcmp(str, (*labels)[num].mark))
        {
            (*labels)[num].status = GOODLBL;
            return (*labels)[num].ip;
        }

    return NOTFOUND;
}

int FindNumLabel(char* str, struct Label** labels, int nJMP)
{
    for (int num = 0; num < nJMP; num++)
        if (!strcmp(str, (*labels)[num].mark))
            return num;

    return NOTFOUND;
}

int AddToLabel(char* str, int point, struct Label** labels, int* nJMP)
{
    assert (str != NULL);
    assert (labels != NULL);
    assert (*labels != NULL);
    assert (point >= 0);
    assert (*nJMP < NUMLBL);

    if (!CheckRepeatLbl(str, labels, *nJMP))
    {
        (*labels)[*nJMP].ip = point;
        strcpy((*labels)[*nJMP].mark, str);
        (*labels)[*nJMP].status = BADLBL;

        (*nJMP)++;
        return NOMISTAKE;
    }

    return MISTAKE;
}

int CheckRepeatLbl(char* str, struct Label** labels, int nJMP)
{
    if (CheckOnNum(str))
    {
        printf("%s is unknown command!\n", str);
        fprintf(logAsm, "%s is unknown command!\n", str);

        return MISTAKE;
    }

    for (int i = 0; i < nJMP; i++)
    {
        if (!strcmp(str, (*labels)[i].mark))
        {
            printf("%s occurs twice!\n", str);
            fprintf(logAsm, "%s occurs twice!\n", str);

            return MISTAKE;
        }
    }

    return NOMISTAKE;
}

int CheckLabels(struct Label** labels, int nJMP)
{
    for (int index = 0; index < nJMP; index++)
    {
        if ((*labels)[index].status == BADLBL)
        {
            printf("%s is unknown command!\n", (*labels)[index].mark);
            fprintf(logAsm, "%s is unknown command!\n", (*labels)[index].mark);

            return MISTAKE;
        }
    }

    return NOMISTAKE;
}

int OutPutLabel(struct Label* labels, int nJMP)
{
    assert (labels != NULL);
    assert (nJMP >= 0);

    for (int i = 0; i < nJMP; i++)
    {
        printf("ip:     %d\n", labels[i].ip);
        printf("mark:   %s\n", labels[i].mark);
        printf("status: %d\n", labels[i].status);
    }
}

int CheckOnNum(char* str)
{
    int value = -1233;
    char checkEnd = 0;

    if (sscanf(str, "%d%c", &value, &checkEnd) == 1)
    {
        if (value == 0)
            return 213481801;
        else
            return value;
    }

    return 0;
}

int CheckCmd(char* str, int j)
{
    assert (str != NULL);

    if ((strlen(str) > 0) && (j > 0))
    {
        printf("%s is unknown command!\t(line %3d)\n", str, j + 1);
        fprintf(logAsm, "%s is unknown command!\t(line %3d)\n", str, j + 1);

        return MISTAKE;
    }

    if ((strlen(str) > 0) && (j == ERRORCMD))
    {
        printf("%s is unknown argument\n", str);
        fprintf(logAsm, "%s is unknown argument\n", str);

        return MISTAKE;
    }

    return NOMISTAKE;
}

int CheckOnMem(char* str)
{
    char left   = 0;
    char right  = 0;
    int  value  = 0;

    if (sscanf(str, "%c%d%c", &left, &value, &right) == 3)
        if ((left == '[') && (right == ']'))
            if (value == 0)
                return 213481801;
            else
                return value;


    return 0;
}

char CheckOnRegs(char* str)
{
    assert (str != NULL);

    if ((strlen(str) != 3) || (str[0] != 'r') || (str[2] != 'x'))
        return NOTREG;

    for (int i = 0; i < NUMREG; i++)
        if (str[1] == ('a' + i))
            return (i + 1);

    return NOTREG;
}

int CheckArgOnNum(char num)
{
    if (num & 0x08)
    {
        printf("incorrect argument!\n");

        return MISTAKE;
    }

    return NOMISTAKE;
}

int CheckHLT(char* str)
{
    assert (str != NULL);

    if (!strcmp(str, "HLT"))
        return 1;
    else
        return 0;
}

void LogHLT(int HLTFLG)
{
    if (!HLTFLG)
    {
        printf("Expected \"HLT\"\n");
        fprintf(logAsm, "Expected \"HLT\"\n");
    }
}

void Verification(int ERROR)
{
    if (ERROR)
    {
        printf("ERROR ASSEMBLING\n");
        fprintf(logAsm, "ERROR ASSEMBLING\n");

        assert(!"ERROR");
    }

    else
    {
        printf("SUCCESS ASSEMBLING\n");
        fprintf(logAsm, "SUCCESS ASSEMBLING\n");
    }
}
