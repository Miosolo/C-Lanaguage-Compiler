#pragma once
#include "Global.h"
#include <map>
#include <vector>

class SymbolParser :
  public BasicParser {
private:
  std::string tempSymbol;

private:
  static const std::map<char, int> offsetMap;
  int thisOffset;
  enum class SS {INIT, PEND, FIN, OVER, ERROR, DS, SS} state; 
      /*Abbr. of SymbolStates, initial, pending, finish, overstep(traceback), double-slash, slash-star*/
  struct trans { SS nextState; int nextOrToken; };
  std::vector<std::vector<struct trans> > transTable;

  const char *tableAddress = "./transTable.csv";

private:
  void readTable (); //read transTable.scv & fill the tranTable
  void setState (char feed);

public:
  GPS feedChar (char feed);
  SymbolParser (int lineNum, int offset);
  ~SymbolParser ();
};

