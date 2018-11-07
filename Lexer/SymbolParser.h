#pragma once
#include "BasicParser.h"
#include <map>
#include <vector>

class SymbolParser :
  public BasicParser {
private:
  std::string tempSymbol;

private:
  const std::map<char, int> offsetMap = {
    {'I', 11}, // 'I' Stands for initial
    {'+', 0},  {'-', 1},  {'*', 2},  {'/', 3},
    {'<', 4},  {'>', 5},  {'!', 6},  {'=', 7},
    {'&', 8},  {'^', 9},  {'|', 10}, {'.', 11},
    {';', 12}, {'\'', 13},{'(', 14}, {')', 15},
    {'[', 16}, {']', 17}, {'{', 18 },{'}', 19},
    {'?', 20}, {',', 21}, {':', 22}, {'~', 23}
  };
  int thisOffset;
  enum class SS {INIT, PEND, FIN, OVER, ERROR} state; //Abbr. of SymbolStates, initial, pending, finish, overstep(traceback)
  struct trans { SS nextState; int nextOrToken; };
  std::vector<std::vector<struct trans> > transTable;

  const char *tableAddress = "./transTable.csv";

private:
  void readTable (); //read transTable.scv & fill the tranTable
  void setState (char feed);

public:
  parserStates feedChar (char feed);
  SymbolParser (int lineNum, int offset);
  ~SymbolParser ();
};

