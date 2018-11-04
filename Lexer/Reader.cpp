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


Reader::Reader (char* input, Lexer *lexer) {
  this->lexer = lexer;
  if (openInputFile (input)) {
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
  if (t == EOL) setNewLine ();
  return t;
}

void Reader::setState (enum workingStates newState) {
  workingState = newState;
}

void Reader::setNewLine () {
  lineNum++;
  lineOffset = 0;
}

void Reader::traceBack () {
  traceBackFlag = true;
}

bool Reader::openInputFile (char* input) {
  if (strcmp (&input[strlen (input) - 2], ".c") != 0) {
    return false; // check whether it's a .c file 
  } else if ((infile = fopen (input, "r")) == NULL) {
    return false;
  } else {
    return true;
  }
}

void Reader::run () {
  do {
    step ();
  } while (workingState != TERMINATE);
}

void Reader::step () {
  char t;
  if (traceBackFlag) {
    t = thisChar;
    traceBackFlag = false;
  } else {
    t = readChar (); //��ʹ�������ļ�ĩβ����Ȼ��EOF
  }

  switch (workingState) {
  case Reader::SEARCHING:
    if (!isblank (t) && t != EOF) {
      setState (PARSING);
      if (isalpha (t) || t == '_') {
        parser = new IdentifierParser (lineNum, lineOffset);
      } else if (isdigit (t)) {
        parser = new struct NumParser (lineNum, lineOffset);
      } else if (t == '"') {
        parser = new StringParser (lineNum, lineOffset);
      } else if (t == '#') {
        parser = new PreprocessingParser (lineNum, lineOffset);
      } else {
        parser = new SymbolParser (lineNum, lineOffset);
      }
      parser->feedChar (t);
    } else if (t != EOF) {
      setState (SEARCHING);
    } else { // t == EOF
      setState (TERMINATE);
    }
    break;
  case Reader::PARSING:
    enum returnStates thisState = parser->feedChar (t);
    switch (thisState) {
    case FINISHED:
      lexer->deliverOutput (parser->returnPID ());
      traceBack (); //���������ɣ�˵���˸��ַ��������ڸ�symbol��һ���֣�Ӧ��������һ��symbol
      delete parser;
      parser = NULL;
      setState (SEARCHING);
      break;
    case SWITCH_TO_PREPROCESSING:
      delete parser;
      parser = new PreprocessingParser (lineNum, lineOffset);
      setState (PARSING);
      break;
    case SWITCH_TO_COMMENT_DODUBLE_SLASH:
      delete parser;
      parser = new StringParser (lineNum, lineOffset, DOUBLE_SLASH);// TO-DO: parameter: enum common type
      setState (PARSING);
      break;
    case SWITCH_TO_COMMENT_SLASH_STAR:
      delete parser;
      parser = new StringParser (lineNum, lineOffset, SLASH_STAR);
      setState (PARSING);
      break;
    case CONTINUING: default:
      break;
    }
    break;
  case TERMINATE: default: //TERMINATE: ���ⲿwork()��������
    break;
  }
}
  

