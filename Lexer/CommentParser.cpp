#include "CommentParser.h"

CommentParser::CommentParser (int lineNum, int lineOffset, GPS initMode):
  BasicParser(lineNum, lineOffset) {
  thisID->token = 600; // token of comment
  switch (initMode) {
  case GPS::SWITCH_TO_COMMENT_DODUBLE_SLASH:
    tempStr = "//";
    mode = Mode::DS;
    state = CS::INIT;
    break;
  case GPS::SWITCH_TO_COMMENT_SLASH_STAR:
    tempStr = "/*";
    mode = Mode::SS;
    state = CS::INIT;
    break;
  default:
    delete this; // 不会发生的调用错误
    break;
  }
}

CommentParser::~CommentParser () {}

GPS CommentParser::feedChar (char feed) {
  if (mode == Mode::DS) {
    switch (state) {
    case CommentParser::CS::INIT: case CommentParser::CS::WORKING:
      if (feed != '\n') {
        tempStr += feed;
        state = CS::WORKING;
        return GPS::CONTINUING;
      } else {
        thisID->unionValue.strValue = new std::string (tempStr);
        return GPS::FINISHED;
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
      return GPS::CONTINUING;
      break;

    case CommentParser::CS::SEMI_TERM:
      tempStr += feed;
      switch (feed) {
      case '/':
        thisID->unionValue.strValue = new std::string (tempStr);
        return GPS::FINISHED;
        break;
      case '*':
        state = CS::SEMI_TERM;
        return GPS::CONTINUING;
      default:
        state = CS::WORKING;
        return GPS::CONTINUING;
        break;
      }
      break;
    case CommentParser::CS::TERM: default:
      break;
    }
  }
}
