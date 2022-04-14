#include "../include/differenciator.h"

CTree CtorTreeFromFile (const char* fileName)
{
    assert (fileName);

    int sizeBuf = (int) GetSizeBuf (fileName);
    char* buffer = CreateBuf (&sizeBuf, fileName);

    CTree tree;
    int index = 0;
    struct item* node = nullptr;

    while ((index < sizeBuf) && (*(buffer + index) != '\0'))
    {
        if (*(buffer + index) == '{')
        {
            node = tree.addItm (nullptr, buffer + index + 1);
            node->data.STR = buffer + index + 1;
            tree.addBuffer (buffer);
        }

        if (*(buffer + index) == '}')
            return tree;

        index++;
    }
    index++;

    if (!node)
    {
        printf ("Empty tree\n");
        return tree;
    }

//    node->left  = CtorItmFromFile (node, buffer, &index, sizeBuf);
//    node->right = CtorItmFromFile (node, buffer, &index, sizeBuf);

    return tree;
}

