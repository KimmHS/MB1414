#ifndef _H_MB1414_
#define _H_MB1414_

#include "serial/serial.h"
#include <thread>
#include <vector>
#include <map>
#include <atomic>

using std::vector;
using std::string;
using std::map;
using std::to_string;
using std::make_pair;

class MB1414 {
    private:
        unsigned long baud = 57600;
        uint8_t data_read[8];
        std::atomic<short> val;

        vector<serial::PortInfo> device_info;
        map<string, unsigned int> cur_info;

        std::thread* thread_run;
        bool is_running;
        bool do_print;

        string str_device_info;

        inline void list();
        inline void read(string port);

    public:
        MB1414();
        ~MB1414();

        void Run(string port);
        void Stop();
        void TogglePrint();
        string GetInfoStr();
        short GetValue();
        vector<serial::PortInfo> GetInfo();
};

extern "C" {
    MB1414* create_sensor();
    void destroy_sensor(MB1414* sensor);
    void sensor_run(MB1414* sensor, const char* port);
    void sensor_stop(MB1414* sensor);
    const char* sensor_get_info(MB1414* sensor);
    short sensor_get_value(MB1414* sensor);
}

#endif
