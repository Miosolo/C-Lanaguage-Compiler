#pragma once
#include "Global.h"
#include <map>

class ErrorNotifier {
private:
  static const std::map<GlobalError, std::string> errorMessageMap;
  
public:
  static void showError (GlobalError error);
};

