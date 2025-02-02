#include "Tinky.hpp"
#include <iostream>

const LPCWSTR SERVICE_NAME = L"tinky";
const LPCWSTR DISPLAY_NAME = L"Tinky Service";
const LPCWSTR SERVICE_PATH = L"\"C:\\Users\\Maro\\Desktop\\tinkey\\a.exe\"";

//Function that handles the errors returned based on the DWORD value
void HandleServiceError(const std::string& operation, DWORD error) {
    std::cerr << "Error during " << operation << ": " << error << std::endl;
    switch (error) {
        case ERROR_ACCESS_DENIED:
            std::cerr << "Access denied. Please run the program as an administrator." << std::endl;
            break;
        case ERROR_SERVICE_EXISTS:
            std::cerr << "The service already exists." << std::endl;
            break;
        case ERROR_SERVICE_DOES_NOT_EXIST:
            std::cerr << "The service does not exist." << std::endl;
            break;
        case ERROR_PATH_NOT_FOUND:
            std::cerr << "The service executable path is invalid." << std::endl;
            break;
        case ERROR_SERVICE_ALREADY_RUNNING:
            std::cerr << "The service is already running." << std::endl;
            break;
        default:
            std::cerr << "Unknown error." << std::endl;
            break;
    }
}

// Connects to the Service Control Manager (SCM)
SC_HANDLE Tinky::ConnectSCManager() {
    SC_HANDLE scHandle = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (!scHandle) {
        std::cerr << "Error connecting to Service Control Manager: " << GetLastError() << std::endl;
    }
    return scHandle;
}

bool Tinky::InstallService() {
    SC_HANDLE scHandle = ConnectSCManager();
    if (!scHandle) {
        return false;
    }
    
    // Creates the service in the SCM database
    SC_HANDLE schService = CreateServiceW(
        scHandle,
        SERVICE_NAME,
        DISPLAY_NAME,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        SERVICE_PATH,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    if (!schService) {
        DWORD error = GetLastError();
        HandleServiceError("Creating Service", error);
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
    if (!scHandle)
        return false;

    // Open the service with full access rights
    SC_HANDLE schService = OpenServiceW(
        scHandle,
        SERVICE_NAME,
        SERVICE_ALL_ACCESS
    );

    if (!schService) {
        DWORD error = GetLastError();
        HandleServiceError("Opening Service", error);
        CloseServiceHandle(scHandle);
        return false;
    }

    if (!::StartServiceW(schService, 0, nullptr)) {
        DWORD error = GetLastError();
        HandleServiceError("Starting Service", error);
        CloseServiceHandle(schService);
        CloseServiceHandle(scHandle);
        return false;
    }

    std::cout << "Service started successfully" << std::endl;
    CloseServiceHandle(schService);
    CloseServiceHandle(scHandle);
    return true;
}

bool Tinky::StopService() {
    SC_HANDLE scHandle = ConnectSCManager();
    if (!scHandle)
        return false;

    SC_HANDLE schService = OpenServiceW(
        scHandle,
        SERVICE_NAME,
        SERVICE_ALL_ACCESS
    );

    if (!schService) {
        DWORD error = GetLastError();
        HandleServiceError("Opening Service", error);
        CloseServiceHandle(scHandle);
        return false;
    }

    SERVICE_STATUS_PROCESS ssp;
    if (!ControlService(schService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ssp)) {
        DWORD error = GetLastError();
        HandleServiceError("Stopping Service", error);
        CloseServiceHandle(schService);
        CloseServiceHandle(scHandle);
        return false;
    }

    std::cout << "Service stopped successfully" << std::endl;
    CloseServiceHandle(schService);
    CloseServiceHandle(scHandle);
    return true;
}

bool Tinky::DeleteService() {
    SC_HANDLE scHandle = ConnectSCManager();
    if (!scHandle)
        return false;

    SC_HANDLE schService = OpenServiceW(
        scHandle,
        SERVICE_NAME,
        DELETE
    );

    if (!schService) {
        DWORD error = GetLastError();
        HandleServiceError("Opening Service", error);
        CloseServiceHandle(scHandle);
        return false;
    }

    if (!::DeleteService(schService)) {
        DWORD error = GetLastError();
        HandleServiceError("Deleting Service", error);
        CloseServiceHandle(schService);
        CloseServiceHandle(scHandle);
        return false;
    }

    std::cout << "Service deleted successfully" << std::endl;
    CloseServiceHandle(schService);
    CloseServiceHandle(scHandle);
    return true;
}