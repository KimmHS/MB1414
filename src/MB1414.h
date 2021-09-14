#ifndef _H_MB1414_
#define _H_MB1414_

#include "serial/serial.h"
#include <thread>
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::map;

using std::to_string;
using std::make_pair;

class MB1414{
    private :
    unsigned long baud = 57600;
    uint8_t data_read[8];

    vector<serial::PortInfo> device_info;
    map<string,unsigned int> cur_info;

    std::thread *thread_run;
    bool is_running;
    bool do_print;

    string str_device_info;

    // update map_serial_device
    inline void list();

    inline void read(string port);

    public :

    inline MB1414();
    inline ~MB1414();

    inline void Run(string port);
    inline void Stop();
    inline void TogglePrint();
    inline string GetInfoStr();
    inline vector<serial::PortInfo> GetInfo();
};

MB1414::MB1414(){
    memset(data_read, 0, sizeof(uint8_t) * 8);
    thread_run = nullptr;
    is_running = false;
    do_print = false;
}

MB1414::~MB1414(){
    is_running  = false;
}

void MB1414::Run(string port){
  is_running = true;    
  thread_run = new std::thread(&MB1414::read,this,port);
  thread_run->detach();
}

void MB1414::Stop(){
  is_running = false;
  thread_run->join();
}

void MB1414::read(string port){

  serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000)); 

  while(is_running){
    try{
        //  cout << "Is the serial port open?";
        if (my_serial.isOpen()){
            size_t available;
            available = my_serial.available();
            size_t string_read = my_serial.read(data_read, 8);

            if(do_print){
              printf("%c",data_read[0]);
              printf("%c",data_read[1]);
              printf("%c",data_read[2]);
              printf("%c",data_read[3]);
              printf("%c",data_read[4]);
              printf("%c",data_read[5]);
              printf("%c",data_read[6]);
              printf("%c",data_read[7]);
              printf("\n");
            }
            //TB_device.setText(GetQStringFromUnsignedChar(data_read));
        }
    }
    catch (std::exception const& e){
      //TB_device.setText("error! port is not found\n");
      printf("error! Cannot find port : %s\n",port.c_str());
      break;
    }
  }

}

void MB1414::TogglePrint(){
  do_print = !do_print;
}

void MB1414::list(){
  device_info = serial::list_ports();
}

string MB1414::GetInfoStr(){
  int i = 0;
  list();
  str_device_info.clear();

  vector<serial::PortInfo>::iterator iter = device_info.begin();
  while (iter != device_info.end()) {
    serial::PortInfo device = *iter++;
    printf("%s\n",device.hardware_id.c_str());
    if (strcmp("n/a", device.hardware_id.c_str())){
      cur_info.insert(make_pair(device.port.c_str(), i));
      i++;
      str_device_info.append("\nPort = ");
      str_device_info.append(device.port); 
      str_device_info.append("\ndescription = ");
      str_device_info.append(device.description); 
      str_device_info.append("\nhardware ID = ");
      str_device_info.append(device.hardware_id);
      str_device_info.append("\n"); 
    }
  }
  printf("** %s\n",str_device_info.c_str());
  return str_device_info;
}

vector<serial::PortInfo> MB1414::GetInfo(){
    return device_info;
}

#endif