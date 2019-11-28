#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// indicates terminal & non-terminal symbols as node type (.typ)
// note that, the first part of nodeType is identical to yytokentype,
// so they're compable.
enum node_type {
  // yytokentype
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
  // yytokentype
  PROGRAM,
  EXTERNAL_DECLARATION,
  TYPE_DECLARATOR,
  DECLARATOR_LIST,
  INTSTR_LIST,
  ID_INDEX,
  DECLARATOR,
  PARAMETER_LIST,
  PARAMETER,
  TYPE,
  STATEMENT,
  STATEMENT_LIST,
  EXPRESSION_STATEMENT,
  EXPR,
  ASSIGN_EXPR,
  PRIMARY_EXPR,
  EXPR_LIST,
  ID_LIST,
  CMP_EXPR,
  ADD_EXPR,
  MUL_EXPR,
  NOVALUE = -0xeeeee,  // prevent collision with legitimate integers
};
char* node_type_list[] = {"END",
                          "NOVALUE",
                          "ID",
                          "NUMBER",
                          "STRING",
                          "INT",
                          "STR",
                          "VOID",
                          "IF",
                          "ELSE",
                          "WHILE",
                          "RETURN",
                          "PRINT",
                          "SCAN",
                          "CMP",
                          "ASSIGN",
                          "PROGRAM",
                          "EXTERNAL_DECLARATION",
                          "TYPE_DECLARATOR",
                          "DECLARATOR_LIST",
                          "INTSTR_LIST",
                          "ID_INDEX",
                          "DECLARATOR",
                          "PARAMETER_LIST",
                          "PARAMETER",
                          "TYPE",
                          "STATEMENT",
                          "STATEMENT_LIST",
                          "EXPRESSION_STATEMENT",
                          "EXPR",
                          "ASSIGN_EXPR",
                          "PRIMARY_EXPR",
                          "EXPR_LIST",
                          "ID_LIST",
                          "CMP_EXPR",
                          "ADD_EXPR",
                          "MUL_EXPR"};

// indicates subtype as node info (.subtyp)
enum subtype {
  NOSUBTYP = 50,
  ASSIGN_TYP,
  FUNCTION,
  DECL,
  IF_TYP,
  IF_ELSE,
  WHILE_TYP,
  RETURN_TYP,
  PRINT_TYP,  // to distinguish with PRINT
  SCAN_TYP,   // the same reason
  ADD,
  SUB,
  MUL,
  FRAC,
  MOD,
  LESS,
  NLESS,
  GREATER,
  NGREATER,
  EQUAL,
  NEQUAL,
};
char* subtype_list[] = {
    "NOSUBTYP", "ASSIGN", "FUNCTION",  "DECL",     "IF",    "IF_ELSE",
    "WHILE",    "RETURN", "PRINT_TYP", "SCAN_TYP", "ADD",   "SUB",
    "MUL",      "FRAC",   "MOD",       "LESS",     "NLESS", "GREATER",
    "NGREATER", "EQUAL",  "NEQUAL"};

extern int yylex();
extern int yylval;
extern char* yytext;
int tok;

struct val {
  int i;    // maybe calculation value or sub-class(seen types above)
  char* s;  // = yytext, if any
};
const struct val NOVAL = {.i = NOVALUE};  // logical empty

typedef struct _ast ast;
typedef struct _ast* past;
struct _ast {
  past l;  // l is the "main" child, if only
  past r;
  struct val val;
  int typ, subtyp;
};

// return the value containing an int value (default)
struct val val(int i) {
  struct val tmp = {.i = i};
  return tmp;
}

// value of a string, str should be located at heap/text
struct val val_str(char* str) {
  struct val tmp = {.s = str};
  return tmp;
}

void advance() {
  static int call_cnt = 0;
  tok = yylex();
  if (tok != END) {
    printf("tok[%d]: %s\n", ++call_cnt, yytext);
  }
}

past new_node(struct val value, enum node_type type, enum subtype sub,
              past left, past right) {
  past node = malloc(sizeof(ast));
  if (node == NULL) {
    printf("run out of memory.\n");
    exit(1);
  }

  node->val = value;
  node->typ = type;
  node->subtyp = sub;
  node->l = left;
  node->r = right;

  return node;
}

// Non-terminal symbols
past program();
past external_declaration();
past decl_or_stmt();
past declarator_list();
past intstr_list();
past initializer();
past declarator();
past parameter_list();
past parameter();
past type();
past statement();
past statement_list();
past expression_statement();
past expr();
past cmp_expr();
past add_expr();
past mul_expr();
past primary_expr();
past expr_list();
past id_list();

// program
//     : external_declaration
//     | program external_declaration  # CHECK
//     ;
past program() {
  past l = external_declaration();
  while (tok != END) {
    advance();
    l = new_node(NOVAL, PROGRAM, NOSUBTYP, l, external_declaration());
  }
  return l;
}

// external_declaration
//     : type declarator decl_or_stmt
//     ;
past external_declaration() {
  past ll = type();
  past lr = declarator();
  // use var type for node type
  past l = new_node(NOVAL, TYPE_DECLARATOR, NOSUBTYP, ll, lr);
  return new_node(NOVAL, EXTERNAL_DECLARATION, NOSUBTYP, l, decl_or_stmt());
}

// decl_or_stmt
//     : '{' statement_list '}'
//     | '{' '}'
//     | ',' declarator_list ';'
//     | ';'
//     ;
past decl_or_stmt() {
  past child = NULL;
  switch (tok) {
    case '{':
      advance();
      if (tok != '}') {
        child = statement_list();
      }
      // should be '}'
      if (tok != '}') abort();
      advance();
      break;
    case ',':
      advance();
      child = declarator_list();
      if (tok != ';') abort();
      advance();
      break;
    case ';':
      advance();
      break;
    default:
      abort();
  }
  // since decl_or_stmt itself contains no additional info
  return child;
}

// declarator_list
//     : declarator
//     | declarator_list ',' declarator # CHECK
//     ;
past declarator_list() {
  past l = declarator();
  while (tok == ',') {
    advance();
    l = new_node(NOVAL, DECLARATOR_LIST, NOSUBTYP, l, declarator());
  }
  return l;
}

// intstr_list
//     : initializer
//     | intstr_list ',' initializer # CHECK
//     ;
past intstr_list() {
  past l = initializer();
  while (tok == ',') {
    advance();
    l = new_node(NOVAL, INTSTR_LIST, NOSUBTYP, l, initializer());
  }
  return l;
}

// initializer
//     : NUMBER
//     | STRING
//     ;
past initializer() {
  past n;
  if (tok == NUMBER) {
    n = new_node(val(atoi(yytext)), NUMBER, NOSUBTYP, NULL, NULL);
  } else if (tok == STRING) {
    n = new_node(val_str(yytext), STRING, NOSUBTYP, NULL, NULL);
  } else {
    abort();
  }

  advance();
  return n;
}

// declarator
//     : ID
//     | ID '=' expr
//     | ID '(' parameter_list ')'
//     | ID '(' ')'
//     | ID '[' ']'
//     | ID '[' expr ']'
//     | ID '[' ']' '=' '{' intstr_list '}'
//     | ID '[' expr ']' '=' '{' intstr_list '}'
//     ;
past declarator() {
  if (tok != ID) abort();
  past l = new_node(val_str(yytext), ID, NOSUBTYP, NULL, NULL);
  advance();

  switch (tok) {
    case '=':
      advance();
      // op as info
      l = new_node(NOVAL, DECLARATOR, ASSIGN_TYP, l, expr());
      break;
    case '(':
      advance();
      if (tok != ')') {
        // op as info
        l = new_node(NOVAL, DECLARATOR, FUNCTION, l, parameter_list());
      }
      if (tok != ')') abort();
      advance();
      break;
    case '[':
      advance();
      past r = NULL;  // ID_INDEX, the index part
      if (tok != ']') {
        r = expr();
      }
      // may have only one child, if empty in []
      l = new_node(NOVAL, ID_INDEX, NOSUBTYP, l, r);
      if (tok != ']') abort();
      advance();
      if (tok == '=') {
        advance();
        if (tok != '{') abort();
        advance();
        l = new_node(NOVAL, DECLARATOR, ASSIGN_TYP, l, intstr_list());
        if (tok != '}') abort();
        advance();
      }
      break;
    default:
      abort();
  }
  return l;
}

// parameter_list
//         : parameter
//         | parameter_list ',' parameter # CHECK
//         ;
past parameter_list() {
  past l = parameter();
  while (tok == ',') {
    advance();
    l = new_node(NOVAL, PARAMETER_LIST, NOSUBTYP, l, parameter());
  }

  return l;
}

// parameter
//         : type ID
//         ;
past parameter() {
  past l = type();
  if (tok != ID) abort();
  past r = new_node(val_str(yytext), ID, NOSUBTYP, NULL, NULL);
  advance();
  return new_node(NOVAL, PARAMETER, NOSUBTYP, l, r);
}

// type
//         : INT
//         | STR
//         | VOID
//         ;
past type() {
  int typ = tok;
  if (tok != INT && tok != STR && tok != VOID) abort();
  advance();
  return new_node(val(typ), TYPE, NOSUBTYP, NULL, NULL);
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
past statement() {
  past l;
  switch (tok) {
    case INT:
    case STR:
    case VOID:  // type.first
      l = new_node(NOVAL, STATEMENT, DECL, type(), declarator_list());
      if (tok != ';') abort();
      advance();
      break;
    case '{':
      advance();
      l = statement_list();
      if (tok != '}') abort();
      advance();
      break;
    case ';':  // expr_statement.first
    case '-':
    case '(':
    case ID:
    case NUMBER:
    case STRING:
      l = expression_statement();
      break;
    case IF:
      advance();
      if (tok != '(') abort();
      advance();
      l = expr();
      if (tok != ')') abort();
      advance();
      l = new_node(NOVAL, STATEMENT, IF_TYP, l, statement());
      if (tok == ELSE) {
        advance();
        l = new_node(NOVAL, STATEMENT, IF_ELSE, l, statement());
      }
      break;
    case WHILE:
      advance();
      if (tok != '(') abort();
      advance();
      l = expr();
      if (tok != ')') abort();
      advance();
      l = new_node(NOVAL, STATEMENT, WHILE_TYP, l, statement());
      break;
    case RETURN:
      l = new_node(NOVAL, STATEMENT, RETURN_TYP, NULL, NULL);
      advance();
      if (tok != ';') {
        l->l = expr();
      }
      if (tok != ';') abort();
      advance();
      break;
    case PRINT:
      l = new_node(NOVAL, STATEMENT, PRINT_TYP, NULL, NULL);
      advance();
      if (tok != ';') {
        l->l = expr_list();
      }
      if (tok != ';') abort();
      advance();
      break;
    case SCAN:
      advance();
      l = new_node(NOVAL, STATEMENT, SCAN_TYP, id_list(), NULL);
      if (tok != ';') abort();
      advance();
      break;
    default:
      abort();
  }

  return l;
}

// statement_list
//     : statement
//     | statement_list statement # CHECK
//     ;
past statement_list() {
  past l = statement();
  while (1) {
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
        advance();
        l = new_node(NOVAL, STATEMENT_LIST, NOSUBTYP, l, statement());
        continue;
    }
    break;  // if tok not in statement.first
  }
  return l;
}

// expression_statement
//     : ';'
//     | expr ';'
//     ;
past expression_statement() {
  past n = NULL;
  if (tok != ';') {
    n = expr();
  }
  if (tok != ';') abort();
  advance();
  return n;
}

// expr
//     : cmp_expr
//     ;
past expr() { return cmp_expr(); }

// cmp_expr
//     : add_expr
//     | cmp_expr CMP add_expr # CHECK
//     ;
past cmp_expr() {
  past l = add_expr();
  while (tok == CMP) {
    char* op = yytext;
    advance();
    // op as sub-class
    past r = add_expr();
    int ans, typ;

    if (strcmp(op, "<") == 0) {
      ans = l->val.i < r->val.i;
      typ = LESS;
    } else if (strcmp(op, ">") == 0) {
      ans = l->val.i > r->val.i;
      typ = GREATER;
    } else if (strcmp(op, "<=") == 0) {
      ans = l->val.i <= r->val.i;
      typ = NGREATER;
    } else if (strcmp(op, ">=") == 0) {
      ans = l->val.i >= r->val.i;
      typ = NLESS;
    } else if (strcmp(op, "==") == 0) {
      ans = l->val.i == r->val.i;
      typ = EQUAL;
    } else if (strcmp(op, "!=") == 0) {
      ans = l->val.i != r->val.i;
      typ = NEQUAL;
    }

    l = new_node(val(ans), CMP_EXPR, typ, l, add_expr());
  }
  return l;
}

// add_expr
//     : mul_expr
//     | add_expr '+' mul_expr # CHECK
//     | add_expr '-' mul_expr # CHECK
//     ;
past add_expr() {
  past l = mul_expr();
  while (tok == '+' || tok == '-') {
    char op = tok;
    int ans, typ;
    advance();
    past r = mul_expr();

    if (op == '+') {
      ans = l->val.i + r->val.i;
      typ = ADD;
    } else {
      ans = l->val.i - r->val.i;
      typ = SUB;
    }
    l = new_node(val(ans), ADD_EXPR, typ, l, r);
  }
  return l;
}

// mul_expr
//     : primary_expr
//     | mul_expr '*' primary_expr # CHECK
//     | mul_expr '/' primary_expr # CHECK
//     | mul_expr '%' primary_expr # CHECK
//     | '-' primary_expr
//     ;
past mul_expr() {
  int rate = 1;
  if (tok == '-') {
    advance();
    rate = -1;
  }
  past l = primary_expr();
  l->val.i *= rate;

  while (tok == '*' || tok == '/' || tok == '%') {
    int ans, typ;
    char op = tok;
    advance();
    past r = primary_expr();
    switch (op) {
      case '*':
        ans = l->val.i * r->val.i;
        typ = MUL;
        break;
      case '/':
        ans = l->val.i / r->val.i;
        typ = FRAC;
        break;
      case '%':
        ans = l->val.i % r->val.i;
        typ = MOD;
        break;
    }
    l = new_node(val(ans), MUL_EXPR, typ, l, r);
  }
  return l;
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
past primary_expr() {
  past l, r;
  switch (tok) {
    case ID:
      l = new_node(val_str(yytext), ID, NOSUBTYP, NULL, NULL);
      advance();
      int assigntyp = 0;
      switch (tok) {
        case '(':
          advance();
          r = NULL;
          if (tok != ')') {
            r = expr_list();
          }
          l = new_node(NOVAL, PRIMARY_EXPR, FUNCTION, l, r);
          if (tok != ')') abort();
          advance();
          break;
        case ASSIGN:
          advance();
          r = expr();
          int ans;
          switch (yytext[0]) {
            case '+':
              assigntyp = ADD;
              ans = l->val.i + r->val.i;
              break;
            case '-':
              assigntyp = SUB;
              ans = l->val.i - r->val.i;
              break;
            case '*':
              assigntyp = MUL;
              ans = l->val.i * r->val.i;
              break;
            case '/':
              assigntyp = FRAC;
              ans = l->val.i / r->val.i;
              break;
            case '%':
              assigntyp = MOD;
              ans = l->val.i % r->val.i;
              break;
            default:
              abort();
          }
          l = new_node(val(ans), ASSIGN_EXPR, assigntyp, l, r);
          break;
        case '=':
          advance();
          r = expr();
          l = new_node(val(r->val.i), PRIMARY_EXPR, ASSIGN_TYP, l, r);
          break;
        case '[':
          advance();
          r = expr();
          l = new_node(NOVAL, ID_INDEX, NOSUBTYP, l, r);
          if (tok != ']') abort();
          advance();
          if (tok == '=') {
            advance();
            r = expr();
            l = new_node(NOVAL, PRIMARY_EXPR, ASSIGN_TYP, l, r);
          }
          break;
      }
      break;
    case NUMBER:
      l = new_node(val(atoi(yytext)), NUMBER, NOSUBTYP, NULL, NULL);
      advance();
      break;
    case STRING:
      l = new_node(val_str(yytext), STRING, NOSUBTYP, NULL, NULL);
      advance();
      break;
    case '(':
      advance();
      l = expr();
      if (tok != ')') abort();
      advance();
      break;
    default:
      abort();
  }

  return l;
}

// expr_list
//     : expr
//     | expr_list ',' expr    # CHECK
//     ;
past expr_list() {
  past l = expr();
  while (tok == ',') {
    advance();
    l = new_node(NOVAL, EXPR_LIST, NOSUBTYP, l, expr());
  }
  return l;
}

// id_list
//     : ID
//     | id_list ',' ID    # CHECK
//     ;
past id_list() {
  if (tok != ID) abort();
  past l = new_node(val_str(yytext), ID, NOSUBTYP, NULL, NULL);
  advance();
  while (tok == ',') {
    advance();
    if (tok != ID) abort();
    past r = new_node(val_str(yytext), ID, NOSUBTYP, NULL, NULL);
    l = new_node(NOVAL, ID_LIST, NOSUBTYP, l, r);
    advance();
  }

  return l;
}

void show_ast(past node, int nest) {
  if (node == NULL) return;

  int i = 0;
  for (i = 0; i < nest; i++) printf("  ");
  printf("[%s/%s]: ", node_type_list[node->typ],
         node->subtyp != NOSUBTYP ? subtype_list[50 + node->subtyp] : "");
  if (node->val.s) {
    printf("%s\n", node->val.i);
  } else if (node->val.i != NOVALUE) {
    printf("%d\n", node->val.i);
  } else {
    printf("(None)\n");
  }

  show_ast(node->l, nest + 1);
  show_ast(node->r, nest + 1);
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
  past root = program();
  printf("\nAST:\n");
  show_ast(root, 0);

  return 0;
}
