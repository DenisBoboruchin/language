#include <stdio.h>

#include "Proc/processor.h"
#include "Asm/assembler.h"

const char* CMD     = "execute/a.asm";
const char* BINCODE = "execute/binCode.bin";

int main(void)
{
    Assembler(CMD);
    //DisAssembler(BINCODE);

    Execute(BINCODE);
}
