// Lexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Global.h"
#include "Lexer.h"

Lexer::Lexer (char* in, char *out) {
  aReader = new Reader (in);
  aWriter = new Writer (out);
}

Lexer::~Lexer () {}

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

