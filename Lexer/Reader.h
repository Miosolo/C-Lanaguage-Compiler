#pragma once
#include "Global.h"

class Reader;

#ifndef LEXER_H
#include "Lexer.h"
#endif // !LEXER_H

#include "ErrorNotifier.h"

#include "BasicParser.h"
#include "BasicParser.h"
#include "NumParser.h"
#include "CommentParser.h"
#include "PreprocessingParser.h"
#include "StringParser.h"
#include "IdentifierParser.h"
#include "SymbolParser.h"

class Reader {
//public://
private:
  Lexer *lexer;
  FILE *infile;
  int lineNum;
  int lineOffset; // the index of starting point in line
  char thisChar;
  bool traceBackFlag; // Set this flag = true if traceback occurs, will return thisChar as the next char.
  BasicParser *parser;

  static const char EOL = '\n';
  enum class ReaderStates {SEARCHING, PARSING, TERMINATE} state;

//public://
private:
  char readChar ();
  void step ();
  void traceBack ();
  bool openInputFile (char *input);

public:
  Reader (char *input, Lexer *lexer);
  void run ();
  ~Reader ();
};