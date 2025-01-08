#include "MB1414.h"
#include <cstring>
#include <stdexcept>

MB1414::MB1414() {
    memset(data_read, 0, sizeof(uint8_t) * 8);
    thread_run = nullptr;
    is_running = false;
    do_print = false;
}

MB1414::~MB1414() {
    is_running = false;
    delete thread_run;
}



void MB1414::Run(string port) {
    is_running = true;
    thread_run = new std::thread(&MB1414::read, this, port);
    thread_run->detach();
}

void MB1414::Stop() {
    is_running = false;
}

void MB1414::read(string port) {
    serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));
    int idx;

    while (is_running) {
        try {
            if (my_serial.isOpen()) {
                size_t available = my_serial.available();
                //printf("Available bytes: %zu\n", available);
                my_serial.read(data_read, 8);

                if (do_print) {
                    for (int i = 0; i < 8; i++) {
                        printf("[%d] %c ", i, data_read[i]);
                    }
                    printf("\n");
                }

                for (idx = 0; idx < 8; idx++) {
                    if (data_read[idx] == 'R') {
                        val.store((data_read[idx + 2] - 48) * 10 + (data_read[idx + 3] - 48));
                        //printf("Stored value: %d\n", val.load());
                        break;
                    }
                }

                if (idx == 8) {
                    val.store(-1);
                    printf("No valid data found. Stored -1.\n");
                }
            }
        } catch (const std::exception& e) {
            printf("Error! Cannot find port: %s\n", port.c_str());
            break;
        }
    }
}

void MB1414::TogglePrint() {
    do_print = !do_print;
}

void MB1414::list() {
    device_info = serial::list_ports();
}

string MB1414::GetInfoStr() {
    list();
    str_device_info.clear();

    for (const auto& device : device_info) {
        if (strcmp("n/a", device.hardware_id.c_str())) {
            str_device_info += "\nPort = " + device.port +
                               "\ndescription = " + device.description +
                               "\nhardware ID = " + device.hardware_id + "\n";
        }
    }
    printf("C++ GetInfoStr: %s\n", str_device_info.c_str());
    return str_device_info;
}

vector<serial::PortInfo> MB1414::GetInfo() {
    return device_info;
}

short MB1414::GetValue() {
    return val.load();
}

extern "C" {
    MB1414* create_sensor() {
        return new MB1414();
    }

    void destroy_sensor(MB1414* sensor) {
        delete sensor;
    }

    void sensor_run(MB1414* sensor, const char* port) {
        sensor->Run(std::string(port));
    }

    void sensor_stop(MB1414* sensor) {
        sensor->Stop();
    }

    const char* sensor_get_info(MB1414* sensor) {
        static std::string info;
        info = sensor->GetInfoStr();
        printf("C++ sensor_get_info: %s\n", info.c_str());
        return info.c_str();
    }

    short sensor_get_value(MB1414* sensor) {
        return sensor->GetValue();
    }
}
