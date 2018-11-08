#pragma once
#define LEXER_H

#include "Global.h"

class Lexer;

#include "Reader.h"
#include "Writer.h"


class Lexer {
private:
  Reader *aReader;
  Writer *aWriter;

private:
  Lexer (char *in, char *out);
  ~Lexer ();

public:
  void run ();
  void deliverOutput (PID id);
};