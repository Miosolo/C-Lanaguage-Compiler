#pragma once
#include "Global.h"

class Writer {
private:
  FILE *outfile;
  std::list<PID> symbolList;

private:
  bool openOutputFile (char *outPara);

public:
  Writer (char* outPara);
  ~Writer ();
  void addItem (PID newID);
  void writeFile ();
};

