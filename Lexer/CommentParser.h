#pragma once
#include "StringParser.h"
class CommentParser :
  public StringParser {
public:
  CommentParser (int lineNum, int offset);
  ~CommentParser ();
};

