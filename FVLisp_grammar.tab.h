/* A Bison parser, made by GNU Bison 3.4.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_FVLISP_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_FVLISP_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "FVLisp_grammar.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
#include "ast.h"
#include "interpreter.h"
  
  void yyerror(const char *s);
	
  // declaration of lexical analyzer
  extern int yylex();
  extern int yyparse();

  node *topLevel;
 

#line 66 "FVLisp_grammar.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUM = 258,
    BOOL = 259,
    CHAR = 260,
    STR = 261,
    VAR = 262,
    LAMBDA = 263,
    ADD = 264,
    SUB = 265,
    MUL = 266,
    DIV = 267,
    IF = 268,
    LISTL = 269,
    LISTR = 270,
    LPAREN = 271,
    SPACE = 272,
    DEF = 273,
    NEWLINE = 274
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "FVLisp_grammar.y"

  int digit;
  char *name;
  int bool;
  char character;
  node *astNode;

#line 105 "FVLisp_grammar.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_FVLISP_GRAMMAR_TAB_H_INCLUDED  */
