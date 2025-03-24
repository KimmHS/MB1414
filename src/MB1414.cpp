#include "MB1414.h"
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <thread>

MB1414::MB1414() {
    memset(data_read, 0, sizeof(uint8_t) * 8);
    thread_run = nullptr;
    update_thread = nullptr;
    is_running = false;
    do_print = false;
    val.store(0);
    std::cout << "MB1414 constructor: Initializing device_info" << std::endl;
    {
        std::lock_guard<std::mutex> lock(device_info_mutex);
        device_info = std::vector<serial::PortInfo>();  // 명시적 초기화
    }
}

MB1414::~MB1414() {
    Stop();  // 객체 소멸 전에 스레드가 종료되도록 함
    if(thread_run) {
        delete thread_run;
    }
    if(update_thread) {
        delete update_thread;
    }
}

void MB1414::Run(string port) {
    is_running = true;
    thread_run = new std::thread(&MB1414::read, this, port);
    update_thread = new std::thread(&MB1414::update_device_info, this);
}

void MB1414::Stop() {
    is_running = false;
    if(thread_run && thread_run->joinable()) {
        thread_run->join();
    }
    if(update_thread && update_thread->joinable()) {
        update_thread->join();
    }
}

void MB1414::read(string port) {
    serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));
    int idx;
    while (is_running) {
        try {
            if (my_serial.isOpen()) {
                size_t available = my_serial.available();
                my_serial.read(data_read, 8);
                if (do_print) {
                    for (int i = 0; i < 8; i++) {
                        printf("[%d] %c ", i, data_read[i]);
                    }
                    printf("\n");
                }
                for (idx = 0; idx < 8; idx++) {
                    if (data_read[idx] == 'R') {
                        if (idx + 3 < 8) {
                            val.store((data_read[idx + 2] - '0') * 10 + (data_read[idx + 3] - '0'));
                        } else {
                            val.store(-1);
                        }
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

void MB1414::update_device_info() {
    while (is_running) {
        try {
            vector<serial::PortInfo> ports = serial::list_ports();
            {
                std::lock_guard<std::mutex> lock(device_info_mutex);
                device_info = ports;
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in update_device_info: " << e.what() << std::endl;
            {
                std::lock_guard<std::mutex> lock(device_info_mutex);
                device_info.clear();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void MB1414::TogglePrint() {
    do_print = !do_print;
}

void MB1414::list() {
    std::cout << "Listing devices..." << std::endl;
    try {
        vector<serial::PortInfo> ports = serial::list_ports();
        {
            std::lock_guard<std::mutex> lock(device_info_mutex);
            device_info = ports;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in list(): " << e.what() << std::endl;
        {
            std::lock_guard<std::mutex> lock(device_info_mutex);
            device_info.clear();
        }
    }
    {
        std::lock_guard<std::mutex> lock(device_info_mutex);
        if (device_info.empty()) {
            std::cerr << "Error: No devices found!" << std::endl;
            return;
        }
        std::cout << "Device count: " << device_info.size() << std::endl;
        for (const auto& dev : device_info) {
            std::cout << "Port: " << dev.port << std::endl;
        }
    }
}

string MB1414::GetInfoStr() {
    str_device_info.clear();
    {
        std::lock_guard<std::mutex> lock(device_info_mutex);
        std::cout << "Debug: device_info size = " << device_info.size() << std::endl;
        std::cout << "Debug: device_info address = " << &device_info << std::endl;
        if (device_info.empty()) {
            return "No devices found.";
        }
        for (const auto& device : device_info) {
            if (strcmp("n/a", device.hardware_id.c_str()) != 0) {
                str_device_info += "\nPort = " + device.port +
                                   "\ndescription = " + device.description +
                                   "\nhardware ID = " + device.hardware_id + "\n";
            }
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
