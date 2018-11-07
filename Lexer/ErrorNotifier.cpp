#include "stdafx.h"
#include "ErrorNotifier.h"

const std::map<GlobalError, std::string> errorMessageMap = {
  {GlobalError::NO_INPUT, "Error: No input file\n"},
  {GlobalError::NO_TRANSTABLE, "Error: No transTable.csv found in the working path.\n"}
};

void ErrorNotifier::showError (GlobalError error) {
  auto result = errorMessageMap.find (error);
  if (result != errorMessageMap.end ()) {
    printf ("Error: Unkown error.\n");
  } else {
    printf ("%s", result->second); // Error message found in map
  }
}
