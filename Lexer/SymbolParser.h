#pragma once
#include "Global.h"
#include "BasicParser.h"
#include "ErrorNotifier.h"
#include <map>
#include <vector>

class SymbolParser :
  public BasicParser {
private:
  std::string tempSymbol;
  int thisOffset;
  enum class SS { INIT, PEND, FIN, OVER, ERROR, DS, SS } state;
      /*Abbr. of SymbolStates, initial, pending, finish, overstep(traceback), double-slash, slash-star*/
  struct trans { SS nextState; int nextOrToken; };

private:
  void setState (char feed);
  //static:
  static const std::map<char, int> offsetMap;
  static std::vector<std::vector<SymbolParser::trans> > transTable;
  static bool readTable (); //read transTable.scv & fill the tranTable

public:
  GPS feedChar (char feed);
  SymbolParser (int lineNum, int offset);
  ~SymbolParser ();
};

