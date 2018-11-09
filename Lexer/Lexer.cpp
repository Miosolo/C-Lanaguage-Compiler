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

int main()
{
  return 0;
}

