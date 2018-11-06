#pragma once
#include "StringParser.h"
class CommentParser :
  public BasicParser {
private:
  std::string tempStr;
  enum class Mode{DS, SS} mode;// Abbr. of Double Slash, Slash & Star
  enum class CS { INIT, WORKING, SEMI_TERM, TERM } state;

public:
  parserStates feedChar (char feed);
  CommentParser (int lineNum, int offset, parserStates initMode);
  ~CommentParser ();
};

