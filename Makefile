CC		= g++
CFLAGS  = -c -Wall 

all: main.o translator.o parser.o tree.o graphviz.o 
	$(CC) src/translator.cpp src/tree.cpp src/parser.cpp src/graphviz.cpp main.cpp -o main 

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

translator.o: src/translator.cpp
	$(CC) $(CFLAGS) src/translator.cpp

parser.o: src/parser.cpp
	$(CC) $(CFLAGS) src/parser.cpp

tree.o: src/tree.cpp
	$(CC) $(CFLAGS) src/tree.cpp

graphviz.o: src/graphviz.cpp
	$(CC) $(CFLAGS) src/graphviz.cpp




cpu: assembler.o disassembler.o proc.o stack.o sort.o start.o
	$(CC) CPU/Asm/assembler.cpp CPU/DisAsm/disassembler.cpp CPU/Proc/processor.cpp CPU/MyStack.cpp CPU/SortText.cpp CPU/main.cpp -o start

start.o: CPU/main.cpp
	$(CC) $(CFLAGS) CPU/main.cpp

assembler.o: CPU/Asm/assembler.o
	$(CC) $(CFLAGS) CPU/Asm/assembler.cpp

disassembler.o: CPU/DisAsm/disassembler.cpp
	$(CC) $(CFLAGS) CPU/DisAsm/disassembler.cpp

proc.o: CPU/Proc/processor.cpp
	$(CC) $(CFLAGS) CPU/Proc/processor.cpp

stack.o: CPU/MyStack.cpp
	$(CC) $(CFLAGS) CPU/MyStack.cpp

sort.o: CPU/SortText.cpp
	$(CC) $(CFLAGS) CPU/SortText.cpp






clean:
	rm -rf *.out *.o execute/a.asm execute/binCode.bin main start
