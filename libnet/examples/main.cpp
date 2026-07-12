#include "libnet/network.hpp"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   🚀 libnet: Network Utility Demo      " << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. Test Local IP Retrieval
    std::cout << "[1] Retrieving Local IP Addresses..." << std::endl;
    auto ips = libnet::Network::getLocalIpAddresses();
    if (ips.empty()) {
        std::cout << "    No local IP addresses found." << std::endl;
    } else {
        for (const auto& ip : ips) {
            std::cout << "    - Found IP: " << ip << std::endl;
        }
    }
    std::cout << std::endl;

    // 2. Test IPv4 Validation
    std::cout << "[2] Testing IPv4 Validation..." << std::endl;
    std::string test_ip1 = "192.168.0.1";
    std::string test_ip2 = "256.0.0.1"; // Invalid
    std::cout << "    " << test_ip1 << " : " << (libnet::Network::isValidIPv4(test_ip1) ? "VALID" : "INVALID") << std::endl;
    std::cout << "    " << test_ip2 << " : " << (libnet::Network::isValidIPv4(test_ip2) ? "VALID" : "INVALID") << std::endl;
    std::cout << std::endl;

    // 3. Test Reachability (Ping/Connect check)
    std::string target_host = "8.8.8.8"; // Google DNS
    std::cout << "[3] Checking reachability to " << target_host << "..." << std::endl;
    if (libnet::Network::ping(target_host)) {
        std::cout << "    ✅ Host is reachable!" << std::endl;
    } else {
        std::cout << "    ❌ Host is unreachable or port 80 is closed." << std::endl;
    }

    std::cout << "========================================" << std::endl;
    return 0;
}