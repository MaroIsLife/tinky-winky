#include "Tinky.hpp"
#include <iostream>

const LPCWSTR SERVICE_NAME = L"tinky";
const LPCWSTR DISPLAY_NAME = L"Tinky Service";
const LPCWSTR SERVICE_PATH = L"\"C:\\Users\\Maro\\Desktop\\tinkey\\a.exe\"";

const std::string SERVICE_NAME = "tinky";
const std::string DISPLAY_NAME = "Tinky Service";
const std::string SERVICE_PATH = "\"C:\\Users\\Maro\\Desktop\\tinkey\\a.exe\"";

SC_HANDLE Tinky::ConnectSCManager() {
    SC_HANDLE scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    
    if (scHandle == NULL) {
        std::cerr << "Error connecting to Service Control Manager: " << GetLastError() << std::endl;
    }
    
    return scHandle;
}

bool Tinky::InstallService() {
    SC_HANDLE scHandle = ConnectSCManager();
    if (scHandle == NULL)
        return false;

    SC_HANDLE schService = CreateService(
        scHandle,
        SERVICE_NAME,
        DISPLAY_NAME,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        SERVICE_PATH,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (schService == NULL) {
        std::cerr << "Error Creating a Service: " << GetLastError() << std::endl;
        CloseServiceHandle(scHandle);
        return false;
    }

    std::cout << "Service installed successfully" << std::endl;
    CloseServiceHandle(schService);
    CloseServiceHandle(scHandle);
    return true;
}

bool Tinky::StartService() {

    SC_HANDLE scHandle = ConnectSCManager();
    if (scHandle == NULL)
        return false;


    //Get HANDLE for my Tinky Service with all Access
    SC_HANDLE schService = OpenService(
        scHandle,
        SERVICE_NAME,
        SERVICE_ALL_ACCESS
    );

    if (schService == NULL) {
        std::cerr << "Error Opening Service: " << GetLastError() << std::endl;
        CloseServiceHandle(scHandle);
        return false;
    }

    //Starts The Service
    bool startServiceBool = ::StartService(schService, 0, NULL);

    CloseServiceHandle(schService);
    CloseServiceHandle(scHandle);

    if (!startServiceBool) {
        std::cerr << "Error Starting Service: " << GetLastError() << std::endl;
        return false;
    }

    std::cout << "Service started successfully" << std::endl;
    return true;
}

bool Tinky::StopService() {
    SC_HANDLE scHandle = ConnectSCManager();
    if (scHandle == NULL)
        return false;

    SC_HANDLE schService = OpenService(
        scHandle,
        SERVICE_NAME,
        SERVICE_ALL_ACCESS
    );

    if (schService == NULL) {
        std::cerr << "Error Opening Service: " << GetLastError() << std::endl;
        CloseServiceHandle(scHandle);
        return false;
    }

    SERVICE_STATUS_PROCESS serviceStatus;

    bool stopControlService = ControlService(schService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&serviceStatus);

    CloseServiceHandle(schService);
    CloseServiceHandle(scHandle);

    if (!stopControlService) {
        std::cerr << "Error Stopping Service: " << GetLastError() << std::endl;
        return false;
    }

    std::cout << "Service stopped successfully" << std::endl;
    return true;
}

bool Tinky::DeleteService() {
    SC_HANDLE scHandle = ConnectSCManager();
    if (scHandle == NULL)
        return false;

    SC_HANDLE schService = OpenService(
        scHandle,
        SERVICE_NAME,
        DELETE
    );

    if (schService == NULL) {
        std::cerr << "Error Opening Service: " << GetLastError() << std::endl;
        CloseServiceHandle(scHandle);
        return false;
    }

    if (!::DeleteService(schService)) {
        std::cerr << "Error Deleting Service: " << GetLastError() << std::endl;
        CloseServiceHandle(schService);
        CloseServiceHandle(scHandle);
        return false;
    }

    std::cout << "Service deleted successfully" << std::endl;
    CloseServiceHandle(schService);
    CloseServiceHandle(scHandle);
    return true;
}