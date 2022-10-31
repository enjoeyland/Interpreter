TARGET = parser_202020790.out

all: $(TARGET)
$(TARGET): main.l main.c token_type.h lex.yy.c
	gcc -lfl -o $@ main.c token_type.h lex.yy.c -I. 
lex.yy.c: main.l
	flex main.l
.PHONY : clean
clean:
	rm -rf *.yy.c *.out *.yy.h