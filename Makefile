parser_202020790.out: main.l
	flex main.l
	gcc -o parser_202020790.out lex.yy.c -lfl
.PHONY : clean
clean:
	rm -rf *.yy.c *.out