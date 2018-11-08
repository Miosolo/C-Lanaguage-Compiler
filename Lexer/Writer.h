#pragma once
#include "Global.h"
#include <vector>

class Writer {
private:
  FILE *outfile;
  std::vector<ID*> symbolList;

private:
  bool openOutputFile (char *outPara);

public:
  Writer (char *outPara);
  ~Writer ();
  void addItem (PID newID);
  void writeFile ();
};

