#include "MB1414.h"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char *argv[]) {
  MB1414 sonar;

  sonar.Run("/dev/ttyUSB0");
  std::string str = sonar.GetInfoStr();
  std::cout << " === Device info ===\n "<< str << std::endl;

  sonar.TogglePrint();
  int cnt = 0; 
  while(cnt++ < 10){
  //while(1){
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::cout<<sonar.GetValue()<<std::endl;
  }

  sonar.Stop();
  return 0;
}
