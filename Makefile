parser_202020790.out: main.l main.c token_type.h
	flex --header-file=Lexer.h main.l
	gcc -o parser_202020790.out main.c token_type.h lex.yy.c -lfl
.PHONY : clean
clean:
	rm -rf *.yy.c *.out Lexer.h