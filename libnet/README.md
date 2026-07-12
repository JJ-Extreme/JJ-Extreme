# 🌐 libnet

A lightweight, cross-platform C++ network utility library designed for Windows, Linux, and macOS. `libnet` provides a simplified interface for common network operations without requiring heavy dependencies.

## ✨ Features

- **Local IP Discovery**: Easily retrieve all local IPv4 addresses of the machine.
- **IPv4 Validation**: Robust validation of IPv4 address strings using regex.
- **Reachability Check**: Simple cross-platform check to verify if a host is reachable (TCP connect check).
- **Zero Dependencies**: Only depends on standard C++17 and system network APIs.

## 🛠 Project Structure

```text
libnet/
├── include/
│   └── libnet/
│       └── network.hpp    # Public API header
├── src/
│   └── network.cpp        # Platform-specific implementations
├── examples/
│   └── main.cpp           # Usage example
├── CMakeLists.txt         # Cross-platform build configuration
└── README.md              # Documentation
```

## 🚀 Building the Project

### Prerequisites
- **CMake** (3.15 or higher)
- **C++17 Compatible Compiler** (GCC, Clang, MSVC)

### Build Steps
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running the Example
After building, you can run the generated example binary:
- **Windows**: `.\net_example.exe`
- **Linux/macOS**: `./net_example`

## 💻 Usage Example

```cpp
#include "libnet/network.hpp"
#include <iostream>

int main() {
    // Get local IPs
    auto ips = libnet::Network::getLocalIpAddresses();
    for(const auto& ip : ips) std::cout << "IP: " << ip << "\n";

    // Validate IP
    if (libnet::Network::isValidIPv4("192.168.0.1")) {
        std::cout << "Valid IPv4\n";
    }

    // Check reachability
    if (libnet::Network::ping("8.8.8.8")) {
        std::cout << "Google DNS is reachable!\n";
    }

    return 0;
}
```

## ⚙️ Platform-Specific Notes

- **Windows**: Automatically links with `ws2_32` and `iphlpapi`.
- **Linux/macOS**: Uses standard POSIX sockets and `getifaddrs`.