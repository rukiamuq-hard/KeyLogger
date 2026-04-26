# Windows System Activity Monitor (Educational)

## Overview
Windows System Activity Monitor is a C++ project that demonstrates low-level interaction with the Windows operating system.

The code focuses on how user input and system data can be accessed through native Windows APIs, how this data can be structured, and how persistence and basic anti-analysis techniques are implemented.

This is not a production-ready tool — it is a technical demonstration intended for studying system behavior and security-related concepts.

---

> ⚠️ **WARNING — LEGAL & ETHICAL DISCLAIMER**
>
> **This project is created strictly for educational and research purposes.**  
> Any use of this software to collect data without the explicit consent of the system owner is **illegal**.  
>
> The author **is not responsible** for any damage caused by misuse of this code.  
>
> The project demonstrates how software can interact with the OS at a low level, including techniques often used in malware, so that developers and security researchers can better understand and defend against them.

---

## Technical Overview

### Persistence
The program shows how persistence can be achieved via:
- Windows Registry (autorun entry)

---

### Data Transmission
Data can be sent over the network using:
- Telegram Bot API
- `libcurl` for HTTP requests
- 
---

## Functionality

### 1. Low-Level Keyboard Hooking
Utilizes `SetWindowsHookExW` with `WH_KEYBOARD_LL` for global input interception.
* **Window Context:** Identifies active window titles via `GetForegroundWindow`.
* **Localization:** Handles various keyboard layouts using `ToUnicodeEx`.
* **Thread Safety:** Log writing is synchronized using `std::mutex`.

### 2. System Hardware Diagnostics
Collects detailed PC configuration data:
* **CPU:** Architecture and core count via `GetNativeSystemInfo`.
* **GPU:** Identifies active graphics adapters via `EnumDisplayDevicesA`.
* **RAM:** Physical memory statistics via `GlobalMemoryStatusEx`.

### 3. Anti-Analysis & Self-Protection
Demonstrates basic techniques to hinder analysis:
* **Debugger Detection:** Uses `IsDebuggerPresent` to alter behavior.
* **Process Protection:** Automatically terminates monitoring tools like `Task Manager`, `PowerShell`, `CMD`, and `Resource Monitor`.

### 4. Persistence Mechanism
Implements persistence through the Windows Registry:
* **Entry:** Creates a value in `Software\Microsoft\Windows\CurrentVersion\Run`.
* **Masking:** Obfuscates as a system service named `RealtekSoundsGood`.

### 5. Exfiltration & Obfuscation
* **Network:** Transmits collected logs and JSON reports to Telegram via `libcurl`.
* **Obfuscation:** Uses a custom XOR method to hide API tokens and file paths from static analysis.

---

## Dependencies

- [`nlohmann/json`](https://github.com/nlohmann/json)
- `libcurl`
- Windows SDK (WinAPI)

---

## Building

This repository currently contains **source code only**.

To build the project, you need to:
1. Create your own project (e.g. in Visual Studio or another compiler)
2. Add the source file(s) from `src/`
3. Link required dependencies:
   - `libcurl`
   - Windows libraries
4. Ensure include paths for:
   - [`nlohmann/json`](https://github.com/nlohmann/json)

No build system (CMake, .sln, etc.) is provided.

---

## Ethical Note

Understanding how these techniques work is important for security.

Mechanisms such as input interception, persistence, and process monitoring are commonly used both in legitimate software and in malware. Studying them helps in:
- malware analysis
- detection engineering
- development of AV/EDR solutions

---

## Notes

This project is a **technical demonstration**.

Do not use it on systems without explicit permission.
