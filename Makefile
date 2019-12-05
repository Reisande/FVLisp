all: astgen

run: scanner_lexer.l FVLisp_grammar.y interpreter.c ast.h
	flex scanner_lexer.l
	bison -d FVLisp_grammar.y
	gcc -g lex.yy.c FVLisp_grammar.tab.c ast.c interpreter.c -o astgen
	./astgen

astgen: scanner_lexer.l FVLisp_grammar.y interpreter.c
	flex scanner_lexer.l
	bison -d -v FVLisp_grammar.y
	gcc -g lex.yy.c FVLisp_grammar.tab.c ast.c interpreter.c -o fvl

clean:
	rm -rf fvl *.tab.c *.tab.h interpreter *~ lex.yy.c
