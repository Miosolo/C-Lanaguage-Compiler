#include "Reader.h"

Reader::Reader (char *input, Lexer *lexer) {
  this->lexer = lexer;
  if (openInputFile (input)) {
    lineNum = 1;
    lineOffset = 1;
    traceBackFlag = false;
    state = ReaderStates::SEARCHING;
  } else {
    ErrorNotifier::showError (GlobalError::NO_INPUT);
    Reader::~Reader ();
  }
}

Reader::~Reader () {
  if (infile != NULL) fclose (infile);
}

char Reader::readChar () {
  char t = fgetc (infile);
  if (t == EOL) {
    lineNum++;
    lineOffset = 1;
  }
  return t;
}

void Reader::traceBack () {
  traceBackFlag = true;
}

bool Reader::openInputFile (char *input) {
  if (strcmp (&input[strlen (input) - 2], ".c") != 0) {// check whether it's a .c file 
    return false; 
  } else if ((infile = fopen (input, "r")) == NULL) {
    ErrorNotifier::showError (GlobalError::NO_INPUT);
    return false;
  } else {
    return true;
  }
}

void Reader::run () {
  if (infile != NULL) do {
    step ();
  } while (state != ReaderStates::TERMINATE);
}

void Reader::step () {
  char t;
  if (traceBackFlag) {
    t = thisChar;
    traceBackFlag = false;
  } else {
    t = readChar (); //Exception: 即使读到了文件末尾，依然读EOF
    thisChar = t;
  }

  switch (state) {
  case ReaderStates::SEARCHING:
    if (!isspace (t) && t != EOF) {
      state = ReaderStates::PARSING;
      if (isalpha (t) || t == '_') {
        parser = new IdentifierParser (lineNum, lineOffset);
      } else if (isdigit (t)) {
        parser = new NumParser (lineNum, lineOffset);
      } else if (t == '"') {
        parser = new StringParser (lineNum, lineOffset);
      } else if (t == '#') {
        parser = new PreprocessingParser (lineNum, lineOffset);
      } else {
        parser = new SymbolParser (lineNum, lineOffset);
      }
      parser->feedChar (t);
    } else if (t != EOF) {
      state = ReaderStates::SEARCHING;
    } else { // t == EOF
      state = ReaderStates::TERMINATE;
    }
    break;

  case ReaderStates::PARSING:
    switch (GPS thisState = parser->feedChar (t)) {
    case GPS::CONTINUING:
      break;
    case GPS::OVERSTEP:
      traceBack (); //如果发生回溯，说明此个字符并非属于该symbol的一部分，应该属于下一个symbol
      /*-- NO BREAK; Intentional Fall-down-- */
    case GPS::FINISHED:
      lexer->deliverOutput (parser->getPID ());
      delete parser;
      parser = NULL;
      state = ReaderStates::SEARCHING;
      break;
    case GPS::SWITCH_TO_COMMENT_DODUBLE_SLASH:
      delete parser;
      parser = new CommentParser (lineNum, lineOffset - 2, GPS::SWITCH_TO_COMMENT_DODUBLE_SLASH); //-2: 已经读入两个字符"//"
      state = ReaderStates::PARSING;
      break;
    case GPS::SWITCH_TO_COMMENT_SLASH_STAR:
      delete parser;
      parser = new CommentParser (lineNum, lineOffset - 2, GPS::SWITCH_TO_COMMENT_SLASH_STAR);
      state = ReaderStates::PARSING;
      break;
   default:
      break;
    }
    break;

  case ReaderStates::TERMINATE: default: //TERMINATE: 由外部work()予以销毁
    break;
  }
}
  

