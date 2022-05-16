#include "../include/translator.h"

size_t GetSizeBuf(const char* origName)
{
    struct stat stbuf;

    assert (origName != NULL);

    stat(origName, &stbuf);

    return stbuf.st_size;
}

char* CreateBuf(size_t* sizeBuf, const char* origName)
{
    assert (sizeBuf != NULL);
    assert (*sizeBuf > 0);

    char* buffer = (char*) calloc(*sizeBuf + 1, sizeof(char));
    assert (buffer != NULL);

    FILE* text = fopen(origName, "r");
    assert (text != NULL);

    size_t newSize = fread(buffer, sizeof(*buffer), *sizeBuf, text);
    assert (fclose(text) == 0);

    if (newSize < *sizeBuf)
    {
        *sizeBuf = newSize;
        buffer = (char*) realloc(buffer, *sizeBuf);
    }

    return buffer;
}
