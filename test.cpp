#include "MB1414.h"

#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
  MB1414 sonar;

  sonar.TogglePrint();

  
  sonar.Run("/dev/ttyUSB0");
  std::string str = sonar.GetInfoStr();
  std::cout << " === Device info ===\n "<< str << std::endl;

  int tmp;
  scanf("%d\n",&tmp);
  sonar.Stop();
  return 0;
}
