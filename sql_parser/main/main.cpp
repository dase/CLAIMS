#include <iostream>  //NOLINT
#include <string>
#include <stdio.h>
#include "../../common/log/logging.h"
#include "../parser/parser.h"

using namespace claims::common;

int main(int argc, char* argv[]) {
  Logging my_glog(argv[0]);
  int flag = 1;
  while (flag) {
    Parser* my_parser = new Parser();
    if (my_parser->GetRawAST() != NULL) {
      my_parser->GetRawAST()->Print();
    } else {
      LOG_WARNING<< "the raw ast is null";
    }
    delete my_parser;
    printf("continue 1 or not 0!\n");
    scanf("%d", &flag);
  }

  return 0;
}
