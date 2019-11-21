/*
 ============================================================================
 Name        : rdparser.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype {
  ERR = -1,
  END = 0,
  ID,
  NUMBER,
  STRING,
  INT,
  STR,
  VOID,
  IF,
  ELSE,
  WHILE,
  RETURN,
  PRINT,
  SCAN,
  CMP,
  ASSIGN,
};

extern int yylex();
// extern int yylval;
extern char* yytext;
int tok;

void advance() {
  static int call_cnt = 0;
  tok = yylex();
  if (tok != END) {
    printf("tok[%d]: %s\n", ++call_cnt, yytext);
  } else {
    abort();
  }
}

// program
//     : external_declaration
//     | program external_declaration  # CHECK
//     ;
void program() {
  while (1) {
    external_declaration();
  }
}

// external_declaration
//     : type declarator decl_or_stmt
//     ;
void external_declaration() {
  type();
  declarator();
  decl_or_stmt();
}

// decl_or_stmt
//     : '{' statement_list '}'
//     | '{' '}'
//     | ',' declarator_list ';'
//     | ';'
//     ;
void decl_or_stmt() {
  if (tok == '{') {
    advance();
    if (tok != '}') {
      statement_list();
    }
    // should be '}'
    if (tok != '}') abort();
    advance();
  } else if (tok == ',') {
    advance();
    declarator_list();
    if (tok != ';') abort();
    advance();
  } else if (tok == ';') {
    advance();
  } else {
    abort();
  }
}

// declarator_list
//     : declarator
//     | declarator_list ',' declarator # CHECK
//     ;
void declarator_list() {
  declarator();
  while (tok == ',') {
    advance();
    declarator();
  }
}

// intstr_list
//     : initializer
//     | intstr_list ',' initializer # CHECK
//     ;
void intstr_list() {
  initializer();
  while (tok == ',') {
    advance();
    initializer();
  }
}

// initializer
//     : NUMBER
//     | STRING
//     ;
void initializer() {
  if (tok != NUMBER && tok != STRING) {
    abort();
  }
  advance();
}

// declarator
//     : ID
//     | ID '=' expr
//     | ID '(' parameter_list ')'
//     | ID '(' ')'
//     | ID '[' expr ']'
//     | ID '[' ']'
//     | ID '[' expr ']' '=' '{' intstr_list '}'
//     | ID '[' ']' '=' '{' intstr_list '}'
//     ;
void declarator() {
  if (tok != ID) abort();
  advance();
  switch (tok) {
    case '=':
      advance() expr();
      break;
    case '(':
      advance();
      if (tok != ')') {
        parameter_list();
      }
      if (tok != ')') abort();
      advance();
      break;
    case '[':
      advance();
      if (tok != ']') {
        expr();
      }
      if (tok != ']') abort();
      advance();
      if (tok == '=') {
        advance();
        if (tok != '{') abort();
        advance();
        intstr_list();
        if (tok != '}') abort();
      }
      break;
    default:
      abort();
  }
}

// parameter_list
//         : parameter
//         | parameter_list ',' parameter # CHECK
//         ;
void parameter_list() {
  parameter();
  while (tok == ',') {
    advance();
    parameter();
  }
}

// parameter
//         : type ID
//         ;
void parameter() {
  type();
  if (tok != ID) abort();
  advance();
}

// type
//         : INT
//         | STR
//         | VOID
//         ;
void type() {
  if (tok != INT && tok != STR && tok != VOID)  abort();
  advance();
}

// statement
//     : type declarator_list ';'
//     | '{' statement_list '}'
//     | expr_statement
//     | IF '(' expr ')' statement
//     | IF '(' expr ')' statement ELSE statement
//     | WHILE '(' expr ')' statement
//     | RETURN ';'
//     | RETURN expr ';'
//     | PRINT ';
//     | PRINT expr_list ';'
//     | SCAN id_list ';'
//     ;
void statement() {
  switch (tok)
  {
  case INT: case STR: case VOID: // type.first
    type();
    declarator_list();
    if (tok != ';') abort();
    advance();
    break;
  case '{':
    advance();
    statement_list();
    if (tok != '}') abort();
    advance();
    break;
  case IF:
    advance();
    if (tok != '(') abort();
    advance();
    expr();
    if (tok != ')') abort();
    advance();
    statement();
    if (tok == ELSE) {
      advance();
      statement();
    }
    break;
  case WHILE:
    advance();
    if (tok != '(') abort();
    advance();
    expr();
    if (tok != ')') abort();
    advance();
    statement();
    break;
  case RETURN:
    advance();
    if (tok != ';') {
      expr();
    }
    if (tok != ';') abort();
    advance();
    break;
  case PRINT:
    advance();
    if (tok != ';') {
      expr_list();
    }
    if (tok != ';') abort();
    advance();
    break;
  case SCAN:
    advance();
    id_list();
    if (tok != ';') abort();
    advance();
    break;
  default:
    abort();
  }
}

// statement_list
//     : statement
//     | statement_list statement # CHECK
//     ;
void statement_list() {
  while (1) {
    statement();
  }
}

void abort_handler(int sig) {
  if (tok == END) {
    printf("passed checking\n");
  } else {
    printf("illegal sequence found at [%s], aborting parsing\n", yytext);
  }
  exit(1);
}

int main(int argc, char** argv) {
  // register exception handler
  signal(SIGABRT, abort_handler);

  while (1) {
    printf("input expression, 'q' to exit>");
    advance();
    int r = expr();
    printf("result: %d\n", r);
    // past rr = astExpr();
    // showAst(rr, 0);
  }
  return 0;
}
