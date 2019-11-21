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

// Non-terminal symbols
void program();
void external_declaration();
void decl_or_stmt();
void declarator_list();
void intstr_list();
void initializer();
void declarator();
void parameter_list();
void parameter();
void type();
void statement();
void statement_list();
void expression_statement();
void expr();
void cmp_expr();
void add_expr();
void mul_expr();
void primary_expr();
void expr_list();
void id_list();

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
    // will abort in subroutine
    external_declaration();
    if (tok == END) {
      return;
    }
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
  switch (tok) {
    case '{':
      advance();
      if (tok != '}') {
        statement_list();
      }
      // should be '}'
      if (tok != '}') abort();
      advance();
      break;
    case ',':
      advance();
      declarator_list();
      if (tok != ';') abort();
      advance();
      break;
    case ';':
      advance();
      break;
    default:
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
      advance();
      expr();
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
        advance();
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
  if (tok != INT && tok != STR && tok != VOID) abort();
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
  switch (tok) {
    case INT:
    case STR:
    case VOID:  // type.first
      type();
      declarator_list();
      if (tok != ';') abort();
      advance();
      break;
    case ';': // expr_statement.first
    case '-':
    case '(':
    case ID:
    case NUMBER:
    case STRING:
      expression_statement();
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
    switch (tok) {  // statement.first
      case INT:
      case STR:
      case VOID:
      case '{':
      case IF:
      case WHILE:
      case RETURN:
      case PRINT:
      case SCAN:
        continue;
    }
    break;  // if tok not in statement.first
  }
}

// expression_statement
//     : ';'
//     | expr ';'
//     ;
void expression_statement() {
  if (tok != ';') {
    expr();
  }
  if (tok != ';') abort();
  advance();
}

// expr
//     : cmp_expr
//     ;
void expr() { cmp_expr(); }

// cmp_expr
//     : add_expr
//     | cmp_expr CMP add_expr # CHECK
//     ;
void cmp_expr() {
  add_expr();
  while (tok == CMP) {
    advance();
    add_expr();
  }
}

// add_expr
//     : mul_expr
//     | add_expr '+' mul_expr # CHECK
//     | add_expr '-' mul_expr # CHECK
//     ;
void add_expr() {
  mul_expr();
  if (tok == '+' || tok == '-') {
    advance();
    mul_expr();
  }
}

// mul_expr
//     : primary_expr
//     | mul_expr '*' primary_expr # CHECK
//     | mul_expr '/' primary_expr # CHECK
//     | mul_expr '%' primary_expr # CHECK
//     | '-' primary_expr
//     ;
void mul_expr() {
  if (tok == '-') advance();
  primary_expr();
  if (tok == '*' || tok == '/' || tok == '%') {
    advance();
    primary_expr();
  }
}

// primary_expr
//     : ID '(' expr_list ')'
//     | ID '(' ')'
//     | '(' expr ')'
//     | ID
//     | NUMBER
//     | STRING
//     | ID ASSIGN expr
//     | ID '=' expr
//     | ID '[' expr ']'
//     | ID '[' expr ']' '=' expr
//     ;
void primary_expr() {
  switch (tok) {
    case ID:
      advance();
      switch (tok) {
        case '(':
          advance();
          if (tok != ')') {
            expr_list();
          }
          if (tok != ')') abort();
          advance();
          break;
        case ASSIGN:
        case '=':
          advance();
          expr();
          break;
        case '[':
          advance();
          expr();
          if (tok != ']') abort();
          advance();
          if (tok == '=') {
            advance();
            expr();
          }
          break;
      }
      break;
    case NUMBER:
    case STRING:
      advance();
      break;
    case '(':
      advance();
      expr();
      if (tok != ')') abort();
      advance();
      break;
    default:
      abort();
  }
}

// expr_list
//     : expr
//     | expr_list ',' expr    # CHECK 
//     ;
void expr_list() {
  expr();
  while (tok == ',') {
    advance();
    expr();
  }
}

// id_list
//     : ID
//     | id_list ',' ID    # CHECK 
//     ;
void id_list() {
  if (tok != ID)  abort();
  advance();
  while (tok == ',') {
    advance();
    if (tok != ID)  abort();
    advance();
  }
}


void abort_handler(int sig) {
  printf("illegal sequence found at [%s], parsing aborted.\n", yytext);
  exit(1);
}

int main(int argc, char** argv) {
  // register exception handler
  signal(SIGABRT, abort_handler);

  // read from standard input
  advance();
  program();
  printf("all checks passed, congratulations!\n");
  return 0;
}
