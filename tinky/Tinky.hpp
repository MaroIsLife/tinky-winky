#ifndef TINKY_HPP
#define TINKY_HPP

#include <windows.h>
#include <string>
#include <iostream>
#include <string>

class Tinky {
public:
    static bool InstallService();
    static bool StartService();
    static bool StopService();
    static bool DeleteService();


private:
    static SC_HANDLE ConnectSCManager();
};

#endif