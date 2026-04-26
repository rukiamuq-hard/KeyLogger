# Windows System Activity Monitor (Educational)

## Overview
Windows System Activity Monitor is a C++ project that demonstrates low-level interaction with the Windows operating system. It is designed for educational use in the context of system internals and information security research.

The project shows how user activity and system data can be collected using native Windows APIs, how such data can be structured, and how typical persistence and anti-analysis techniques are implemented. The intent is to provide a clear, practical example of techniques that are commonly discussed in malware analysis, but from a defensive and research-oriented perspective.

---

> ⚠️ **WARNING — LEGAL & ETHICAL DISCLAIMER**
>
> **This project is created strictly for educational and research purposes.**  
> Any use of this software to collect data without the explicit consent of the system owner is **illegal** and may lead to legal consequences.  
>
> The author **is not responsible** for any direct or indirect damage caused by the use or misuse of this code.  
>
> This project demonstrates how software, including potentially malicious programs, can interact with the operating system. The goal is to help developers and security researchers better understand these mechanisms in order to build stronger defenses.

---

## Technical Overview

### Keyboard Hook (WH_KEYBOARD_LL)
The project uses the Windows API to install a low-level keyboard hook (`WH_KEYBOARD_LL`). This allows interception of keyboard events globally, outside the context of a single application.

Captured input is processed in real time. The implementation includes handling of virtual key codes, conversion to readable characters, and awareness of the current keyboard layout. It also tracks the active window to provide context for input events.

---

### System Information Collection
Basic system information is collected and serialized into JSON format. This includes:
- CPU details
- GPU information
- Memory (RAM) data

The project uses `nlohmann/json` to structure and serialize this data, making it suitable for logging or transmission.

---

### Anti-Analysis Techniques
The code demonstrates simple anti-analysis behavior:
- Detection of a debugger using `IsDebuggerPresent`
- Conditional behavior when analysis is detected
- Interaction with system utilities such as Task Manager or PowerShell

These mechanisms are included to illustrate how software may attempt to resist inspection, which is relevant for reverse engineering and defensive tooling.

---

### Persistence via Registry
The project includes an example of persistence by adding itself to Windows startup through the registry. This demonstrates how applications can maintain execution across system reboots.

---

### Data Transmission
Collected data can be sent over the network using the Telegram Bot API. HTTP requests are handled via `libcurl`.

Sensitive strings such as API tokens are not stored in plain form. A simple XOR-based obfuscation is used to reduce direct visibility in the binary. This is not strong encryption, but a demonstration of basic string hiding techniques.

---

## Functionality

- Intercepts keyboard input with awareness of:
  - active window
  - keyboard layout
- Collects and structures system information
- Monitors running processes and records activity
- Produces data dumps for later analysis
- Uses multiple threads (`std::thread`) to separate:
  - input capture
  - data processing
  - data transmission

---

## Dependencies

The project relies on the following:

- `nlohmann/json` — JSON serialization
- `libcurl` — HTTP communication
- Windows SDK — WinAPI access

---

## Build Instructions

### Visual Studio
1. Open the solution file (`.sln`)
2. Ensure the following are available:
   - Windows SDK
   - libcurl (via vcpkg or manual setup)
3. Select a build configuration (`Debug` or `Release`)
4. Build the solution

---

### CMake
```bash
git clone <repository-url>
cd project
mkdir build
cd build
cmake ..
cmake --build .
```

### Dependies
If dependencies are missing, install them using a package manager such as vcpkg:

vcpkg install curl nlohmann-json

### Ethical Note

Understanding how these techniques work is important for security. Many defensive systems—such as antivirus software and endpoint detection and response (EDR)—rely on recognizing patterns like input interception, persistence mechanisms, and unusual process behavior.
Studying these concepts in a controlled and transparent way helps build better detection and protection tools.

## Notes

**This project should be treated as a technical demonstration. It is not intended for real-world deployment or use on systems without full ownership and consent.**
