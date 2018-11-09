#include "ErrorNotifier.h"

const std::map<GlobalError, std::string> ErrorNotifier::errorMessageMap = {
  {GlobalError::NO_INPUT, "Error: No input file\n"},
  {GlobalError::NO_TRANSTABLE, "Error: No transTable.csv found in the working path.\n"},
  {GlobalError::NO_OUTPUT, "Error: Invalid output file, result will display on the screen." }
};

void ErrorNotifier::showError (GlobalError error) {
  auto result = errorMessageMap.find (error);
  if (result == errorMessageMap.end ()) {
    printf ("Error: Unkown error.\n");
  } else {
    printf ("%s", result->second.c_str()); // Error message found in map
  }
}
