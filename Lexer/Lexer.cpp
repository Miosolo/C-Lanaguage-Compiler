// Lexer.cpp : Defines the entry point for the console application.
//
#include "Global.h"
#include "Lexer.h"

Lexer::Lexer (char *in, char *out) {
  aReader = new Reader (in, this);
  aWriter = new Writer (out);
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

int main()
{
  return 0;
}

