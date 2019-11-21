%{
enum yytokentype {
  ID = 1, 
  NUMBER, STRING, 
  INT, STR, VOID, 
  IF, ELSE, WHILE, RETURN, PRINT, SCAN,
  CMP, ASSIGN,
};
%}

%%
"int" {
  return INT;
}
"str" {
  return STR;
}
"void" {
  return VOID;
}
"if" {
  return IF;
}
"else" {
  return ELSE;
}
"while" {
  return WHILE;
}
"return" {
  return RETURN;
}
"print" {
  return PRINT;
}
"scan" {
  return SCAN;
}

(0|[1-9][0-9]*) {
  // NUMBER
  return NUMBER;
}
[A-Za-z_][A-Za-z_0-9]* {
  // ID
  return ID;
}
"\"".*"\"" {
  // STRING
  return STRING;
}

"+="|"-="|"*="|"/=" {
  return ASSIGN;
}
"=="|"!="|">"|"<"|">="|"<=" {
  return CMP;
}

","|";"|"("|")"|"["|"]"|"{"|"}"|"=" {
  // other valid symbols
  return yytext[0];
}

[ \t\n] { /* ignore whitespace */ }
. 	{ printf("Mystery character %c\n", *yytext); }
%%

int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}

int yywrap()
{
	return 1;
}