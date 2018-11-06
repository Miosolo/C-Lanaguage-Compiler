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
    readerState = ReaderStates::SEARCHING;
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

void Reader::setState (enum ReaderStates newState) {
  readerState = newState;
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
  } while (readerState != ReaderStates::TERMINATE);
}

void Reader::step () {
  char t;
  if (traceBackFlag) {
    t = thisChar;
    traceBackFlag = false;
  } else {
    t = readChar (); //即使读到了文件末尾，依然读EOF
  }

  switch (readerState) {
  case ReaderStates::SEARCHING:
    if (!isblank (t) && t != EOF) {
      setState (ReaderStates::PARSING);
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
      setState (ReaderStates::SEARCHING);
    } else { // t == EOF
      setState (ReaderStates::TERMINATE);
    }
    break;
  case ReaderStates::PARSING:
    parserStates thisState = parser->feedChar (t);
    switch (thisState) {
    case parserStates::CONTINUING:
      break;
    case parserStates::FINISHED:
      lexer->deliverOutput (parser->returnPID ());
      traceBack (); //如果解析完成，说明此个字符并非属于该symbol的一部分，应该属于下一个symbol
      delete parser;
      parser = NULL;
      setState (ReaderStates::SEARCHING);
      break;
    case parserStates::SWITCH_TO_PREPROCESSING:
      delete parser;
      parser = new PreprocessingParser (lineNum, lineOffset);
      setState (ReaderStates::PARSING);
      break;
    case parserStates::SWITCH_TO_COMMENT_DODUBLE_SLASH:
      delete parser;
      parser = new StringParser (lineNum, lineOffset, DOUBLE_SLASH);// TO-DO: parameter: enum common type
      setState (ReaderStates::PARSING);
      break;
    case parserStates::SWITCH_TO_COMMENT_SLASH_STAR:
      delete parser;
      parser = new StringParser (lineNum, lineOffset, SLASH_STAR);
      setState (ReaderStates::PARSING);
      break;
   default:
      break;
    }
    break;
  case ReaderStates::TERMINATE: default: //TERMINATE: 由外部work()予以销毁
    break;
  }
}
  

