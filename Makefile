CC		= g++
CFLAGS  = -c -Wall -Wextra -Werror -Walloc-zero -Walloca -Walloca-larger-than=8192 -Warray-bounds -Wcast-align -Wcast-qual              \
            -Wchar-subscripts -Wconversion -Wdangling-else -Wduplicated-branches -Wempty-body -Wfloat-equal -Wformat-nonliteral         \
            -Wformat-security -Wformat-signedness -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Winline -Wlarger-than=8192      \
            -Wvla-larger-than=8192 -Wlogical-op -Wmissing-declarations -Wopenmp-simd -Wpacked -Wpointer-arith -Wredundant-decls         \
            -Wrestrict -Wshadow -Wstack-usage=8192 -Wstrict-overflow=2 -Wstringop-overflow=4 -Wsuggest-attribute=noreturn               \
            -Wsuggest-final-types -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code -Wvariadic-macros     		 				\
            -Wno-missing-field-initializers -Wnarrowing -Wvarargs -fcheck-new -fstrict-overflow -flto-odr-type-merging 	

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

clean:
	rm -rf *.out *.o main
