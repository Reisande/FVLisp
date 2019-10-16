all: astgen

astgen: scanner_lexer.l FVLisp_grammar.y interpreter.c
	flex scanner_lexer.l
	bison -d FVLisp_grammar.y
	gcc lex.yy.c FVLisp_grammar.tab.c ast.c interpreter.c -o astgen 

clean:
	rm -rf astgen *.tab.c *.tab.h interpreter *~ lex.yy.c
