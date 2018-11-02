#include "stdafx.h"
#include "Global.h"
#include "Reader.h"
#include "BasicParser.h"
#include "NumParser.h"
#include "CommentParser.h"
#include "PreprocessingParser.h"
#include "StringParser.h"
#include "IdentifierParser.h"
#include "SymbolParser.h"


Reader::Reader (std::string input) {
  infile = fopen (input.c_str (), "r");
  if (isWellInput ()) {
    lineNum = 1;
    lineOffset = 0;
    traceBackFlag = false;
    workingState = SEARCHING;
  } else {
    // TO-DO: show error: no such file
    delete this;
  }
}


Reader::~Reader () {
  fclose (infile);
}

char Reader::readChar () {
  char t = fgetc (infile);
  if (t == EOL) {
    lineNum++;
    lineOffset = 0;
  }
  return t;
}

void Reader::step () {
  char t = traceBackFlag ? thisChar : readChar ();
  switch (workingState) {
  case Reader::SEARCHING:
    if (isdigit (t)) {
      workingState = PARSING;
      parser = new struct NumParser ();
    } else if (isalpha (t) || t == '_') {
      workingState = PARSING;
      parser = new IdentifierParser ();
    } else if (t == )
    break;
  case Reader::PARSING:
    break;
  case Reader::PARSED:
    break;
  default:
    break;
  }
}


/*
class Reader {
private:
FILE *infile;
int lineNum;
int lineOffset; // the index of starting point in line
char thisChar;
bool traceBackFlag; // Set this flag = true if traceback occurs, will return thisChar as the next char.

static const char EOL = '\n';
static enum states {SEARCHING, PARSING, PARSED} workingState;

private:
void readFile ();
void setNewLine ();
void traceBack ();

public:
Reader (std::string input);
bool isWellInput ();
~Reader ();
};
*/