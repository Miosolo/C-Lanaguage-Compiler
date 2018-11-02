#pragma once
#include "Global.h"
#include "BasicParser.h"

class Reader {
private:
  FILE *infile;
  int lineNum;
  int lineOffset; // the index of starting point in line
  char thisChar;
  bool traceBackFlag; // Set this flag = true if traceback occurs, will return thisChar as the next char.
  struct BasicParser *parser;

  static const char EOL = '\n';
  static enum workingStates {SEARCHING, PARSING, PARSED} workingState;

private:
  char readChar ();
  void step ();
  void setNewLine ();
  void traceBack ();
  void work ();

public:
  Reader (std::string input);
  bool isWellInput ();
  ~Reader ();
};

