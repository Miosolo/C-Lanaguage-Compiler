#include "stdafx.h"
#include "CommentParser.h"

CommentParser::CommentParser (int lineNum, int lineOffset, parserStates initMode):
  BasicParser(lineNum, lineOffset) {
  switch (initMode) {
  case parserStates::SWITCH_TO_COMMENT_DODUBLE_SLASH:
    tempStr = "//";
    mode = Mode::DS;
    state = CS::INIT;
    break;
  case parserStates::SWITCH_TO_COMMENT_SLASH_STAR:
    tempStr = "/*";
    mode = Mode::DS;
    state = CS::INIT;
    break;
  default:
    delete this;
    // TO-DO: Need an Error?
    break;
  }
}

CommentParser::~CommentParser () {}

parserStates CommentParser::feedChar (char feed) {
  if (mode == Mode::DS) {
    switch (state) {
    case CommentParser::CS::INIT: case CommentParser::CS::WORKING:
      if (feed != '\n') {
        tempStr += feed;
        state = CS::WORKING;
        return parserStates::CONTINUING;
      } else {
        thisID->unionValue.strValue = new std::string (tempStr);
        // TO-DO:Token?
        return parserStates::FINISHED;
      }    
    default:
      break;
    }
  } else { // SLASH_STAR
    switch (state) {
    case CommentParser::CS::INIT: case CommentParser::CS::WORKING:
      tempStr += feed;
      if (feed != '*') {
        state = CS::WORKING;
      } else {
        state = CS::SEMI_TERM;
      }
      return parserStates::CONTINUING;
      break;

    case CommentParser::CS::SEMI_TERM:
      tempStr += feed;
      switch (feed) {
      case '/':
        thisID->unionValue.strValue = new std::string (tempStr);
        //TO-DO: Token?
        return parserStates::FINISHED;
        break;
      case '*':
        state = CS::SEMI_TERM;
        return parserStates::CONTINUING;
      default:
        state = CS::WORKING;
        return parserStates::CONTINUING;
        break;
      }
      break;
    case CommentParser::CS::TERM: default:
      break;
    }
  }
}
