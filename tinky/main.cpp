// main.cpp
#include <iostream>
#include <string>
#include "Tinky.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: Program requires exactly one argument." << std::endl;
        std::cerr << "Usage: " << argv[0] << " [install|start|stop|delete]" << std::endl;
        return 1;
    }

    std::string command = argv[1];
    bool result = false;
    
    if (command == "install") {
        result = Tinky::InstallService();
    }
    else if (command == "start") {
        result = Tinky::StartService();
    }
    else if (command == "stop") {
        result = Tinky::StopService();
    }
    else if (command == "delete") {
        result = Tinky::DeleteService();
    }
    else {
        std::cerr << "Error: Invalid command." << std::endl;
        std::cerr << "Valid commands are: install, start, stop, delete" << std::endl;
        return 1;
    }

    return result ? 0 : 1;
}