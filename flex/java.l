%option yylineno  
%%
abstract|assert|boolean|break|byte|case|catch|char|class|const|continue|default|do|double|else|enum|extends|final|finally|float|for|goto|if|implements|import|instanceof|int|interface|long|native|new|package|private|protected|public|return|strictfp|short|static|super|switch|synchronized|this|throw|throws|transient|try|void|volatile|while {
  // keywords
  printf("Line %d: (KEY, %s)\n", yylineno, yytext);
}
"+"|"-"|"*"|"/"|"%"|"++"|"--"|"=="|"!="|">"|"<"|">="|"<="|"&"|"|"|"^"|"~"|"<<"|">>"|">>>"|"&&"|"||"|"!"|"="|"+="|"-="|"*="|"/="|"&="|"<<="|">>="|"&="|"^="|"|="|"{"|"}"|"("|")"|"["|"]"|";"|"."|"," {
  // symbols
  printf("Line %d: (SYM, %s)\n", yylineno, yytext);
}
(0|[1-9][0-9]*)("."[0-9]+)? {
  // numbers
  printf("Line %d: (NUM, %s)\n", yylineno, yytext);
}
[A-Za-z_][A-Za-z_0-9]* {
  // identifiers
  printf("Line %d: (ID, %s)\n", yylineno, yytext);
}
[ \t\r\n] {// whitespaces}
"\"".*"\"" {
  // string
  printf("Line %d: (STR, %s)\n", yylineno, yytext);
}
"/*".*"*/"|"//".*$ {
  // comment
  // printf("Line %d: (COM, %s)\n", yylineno, yytext);
}
. {
  // others
  printf("Line %d: illegal character %s\n", yylineno, yytext);  
  yyterminate();
}
%%

int yywrap() {  
  return 1;  
}  
  
int main(int argc, char** argv) {  
  if(argc > 1)  
    yyin = fopen(argv[1], "r");  
  else  
    yyin = stdin;  
  yylex();  
} 
