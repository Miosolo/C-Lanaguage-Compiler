#pragma once
#include "Global.h"
#include "BasicParser.h"
#include "Lexer.h"

class Reader {
private:
  Lexer *lexer;
  FILE *infile;
  int lineNum;
  int lineOffset; // the index of starting point in line
  char thisChar;
  bool traceBackFlag; // Set this flag = true if traceback occurs, will return thisChar as the next char.
  struct BasicParser *parser;

  static const char EOL = '\n';
  enum class ReaderStates {SEARCHING, PARSING, TERMINATE} readerState;

private:
  void setState (enum ReaderStates newState);
  char readChar ();
  void step ();
  void setNewLine ();
  void traceBack ();
  bool openInputFile (char* input);

public:
  Reader (char* input, Lexer *lexer);
  void run ();
  ~Reader ();
};