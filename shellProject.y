%{
#include <stdio.h>
#include <string.h>
#include "shellProject.h"

/*-------------------------------------------------------------------------------------
 * shellProject.y - yacc specification file for the parser generator
 *--------------------------------------------------------------------------------------*/

int eventcount = 0;

 /* parse local working data section */

 COMMAND *q, *p;

 int pfd[2];
%}

%union{
    int integer;
    char *string;
    char *word;
    char *option;
}

%token <integer> LT GT AMP LPAREN VBAR DOT DEBUG NEWLINE TILDE LS
%token <integer> SETENV PATH PROMPT CD BYE ALIAS UNALIAS PWD EXTEND
%token <integer> ALIASLOOP UNSETENV PRINTENV QUOTE PIPE BACKGROUND BACKSLASH
%token <word>    WORD SPACE VARIABLE
%token <string>  STRING
%token <option>  OPTION

%start cmd

%%

cmd:              bin.cmd
                        { eventcount++; }
                | simp.cmd
                        { eventcount++; }
                ;


bin.cmd:          CD NEWLINE
                        { bicmd = CDHome_CMD; builtin = 1; return 0;}

                | CD WORD NEWLINE
                        { bicmd = CDPath_CMD; builtin = 1; strPath = $2; return 0;}
                | BYE
                        { bicmd = BYE_CMD; return 0; }
                | NEWLINE
                        { bicmd = NEWLINE_CMD; builtin = 1; return 0;}
                | LS NEWLINE
                        { bicmd = LS_CMD; builtin = 1; return 0; }
                | LS WORD NEWLINE
                        { bicmd = LSWord_CMD; builtin = 1; fileName = $2; return 0; }
                | LS STRING NEWLINE
                        { bicmd = LSWord_CMD; builtin = 1; fileName = $3; return 0; }
                ;

simp.cmd:         WORD NEWLINE
                        { bistr = $1; argv[0] = $1; argv[1] = NULL; return 0; }
                | WORD WORD NEWLINE
                        { bistr = $1; argv[argc] = $1; argv[++argc] = $2; argv[++argc] = NULL; return 0; }
                | WORD STRING NEWLINE
                        { bistr = $1; argv[argc] = $1; argv[++argc] = $2; argv[++argc] = NULL; return 0; }
                ;
%%

int yyerror(char *s){
    fprintf(stderr, "*%s*\n", s);
    return 0;
}

