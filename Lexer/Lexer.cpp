// Lexer.cpp : Defines the entry point for the console application.
//
#include "Global.h"
#include "Lexer.h"

Lexer::Lexer (char *in, char *out) {
  aWriter = new Writer (out);
  aReader = new Reader (in, this);
}

Lexer::~Lexer () {
  delete aReader;
  delete aWriter;
}

void Lexer::run () {
  aReader->run ();
  aWriter->writeFile ();
}

void Lexer::deliverOutput (PID id) {
  aWriter->addItem (id);
}

int main (int argc, char *argv[]) {
#ifdef _DEBUG
  argc = 3;
  argv[1] = GLOBAL_ABSOLUTE_INPUT;
  argv[2] = "";
#endif // _DEBUG

  Lexer* lex;
  switch (argc) {
  case 2:
   lex = new Lexer (argv[1], "");
    break;
  case 3:
    lex = new Lexer (argv[1], argv[2]);
    break;
  default:
    printf ("Error: Parameter should be (InputLocation, OutputLocation)\n");
    return 1;
  }

  lex->run ();
  return 0;
}

