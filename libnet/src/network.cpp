#include "libnet/network.hpp"
#include <iostream>
#include <regex>
#include <map>

#ifdef PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
    #include <iomanip>
    #include <sstream>
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <ifaddrs.h>
    #include <unistd.h>
    #include <net/if.h>
    #include <iomanip>
    #include <sstream>
    #if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
        #include <net/if_dl.h>
    #elif defined(__linux__)
        #include <sys/ioctl.h>
    #endif
#endif

namespace libnet {

bool Network::isValidIPv4(const std::string& ip) {
    const std::regex ipv4_pattern("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    return std::regex_match(ip, ipv4_pattern);
}

bool Network::isValidIp(const std::string& ip) {
    if (isValidIPv4(ip)) return true;
    // Basic IPv6 regex check
    const std::regex ipv6_pattern("^(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))$");
    return std::regex_match(ip, ipv6_pattern);
}

std::vector<std::string> Network::resolveHost(const std::string& host) {
    std::vector<std::string> ips;
    struct addrinfo hints = {}, *res, *p;
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), nullptr, &hints, &res) != 0) {
        return ips;
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        char buf[NI_MAXHOST];
        if (getnameinfo(p->ai_addr, p->ai_addrlen, buf, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST) == 0) {
            ips.push_back(std::string(buf));
        }
    }

    freeaddrinfo(res);
    return ips;
}

std::vector<Adapter> Network::getAdapters() {
    std::vector<Adapter> adapters;
    std::map<std::string, Adapter> adapterMap;

#ifdef PLATFORM_WINDOWS
    ULONG outBufLen = 15000;
    PIP_ADAPTER_ADDRESSES pAddresses = (PIP_ADAPTER_ADDRESSES)malloc(outBufLen);

    if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen) == NO_ERROR) {
        for (PIP_ADAPTER_ADDRESSES pCurrAddr = pAddresses; pCurrAddr != NULL; pCurrAddr = pCurrAddr->FirstAdapterAddress) {
            std::string macStr;
            std::stringstream ss;
            for (ULONG i = 0; i < pCurrAddr->PhysicalAddressLength; ++i) {
                ss << std::hex << std::setw(2) << std::setfill('0') << (int)pCurrAddr->PhysicalAddress[i];
                if (i < pCurrAddr->PhysicalAddressLength - 1) ss << ":";
            }
            macStr = ss.str();

            Adapter adapter;
            adapter.mac = macStr;

            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddr->FirstUnicastAddress;
            while (pUnicast != NULL) {
                char buf[INET6_ADDRSTRLEN];
                if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
                    sockaddr_in* sa_in = (sockaddr_in*)pUnicast->Address.lpSockaddr;
                    inet_ntop(AF_INET, &sa_in->sin_addr, buf, INET_ADDRSTRLEN);
                } else if (pUnicast->Address.lpSockaddr->sa_family == AF_INET6) {
                    sockaddr_in6* sa_in6 = (sockaddr_in6*)pUnicast->Address.lpSockaddr;
                    inet_ntop(AF_INET6, &sa_in6->sin6_addr, buf, INET6_ADDRSTRLEN);
                } else {
                    pUnicast = pUnicast->NextUnicastAddress;
                    continue;
                }
                adapter.ips.push_back(std::string(buf));
                pUnicast = pUnicast->NextUnicastAddress;
            }
            adapters.push_back(adapter);
        }
    }
    free(pAddresses);

#else // Linux/macOS
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) return adapters;

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        std::string interfaceName = ifa->ifa_name;
        
        // Store IP addresses
        if (ifa->ifa_addr->sa_family == AF_INET || ifa->ifa_addr->sa_family == AF_INET6) {
            char host[NI_MAXHOST];
            int family = ifa->ifa_addr->sa_family;
            socklen_t len = (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            if (getnameinfo(ifa->ifa_addr, len, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) == 0) {
                adapterMap[interfaceName].ips.push_back(std::string(host));
            }
        } 
        // Store MAC address
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
        else if (ifa->ifa_addr->sa_family == AF_LINK) {
            struct sockaddr_dl* sdl = (struct sockaddr_dl*)ifa->ifa_addr;
            unsigned char* ptr = (unsigned char*)LLADDR(sdl);
            std::stringstream ss;
            for (int i = 0; i < sdl->sdl_alen; ++i) {
                ss << std::hex << std::setw(2) << std::setfill('0') << (int)ptr[i];
                if (i < sdl->sdl_alen - 1) ss << ":";
            }
            adapterMap[interfaceName].mac = ss.str();
        }
#elif defined(__linux__)
        else if (ifa->ifa_addr->sa_family == AF_PACKET) {
            struct sockaddr_ll* sll = (struct sockaddr_ll*)ifa->ifa_addr;
            std::stringstream ss;
            for (int i = 0; i < sll->sll_halen; ++i) {
                ss << std::hex << std::setw(2) << std::setfill('0') << (int)sll->sll_addr[i];
                if (i < sll->sll_halen - 1) ss << ":";
            }
            adapterMap[interfaceName].mac = ss.str();
        }
#endif
    }

    for (auto const& [name, adapter] : adapterMap) {
        adapters.push_back(adapter);
    }
    freeifaddrs(ifaddr);
#endif

    return adapters;
}

std::vector<std::string> Network::getLocalIpAddresses() {
    std::vector<std::string> ips;

#ifdef PLATFORM_WINDOWS
    ULONG outBufLen = 15000;
    PIP_ADAPTER_ADDRESSES pAddresses = (PIP_ADAPTER_ADDRESSES)malloc(outBufLen);

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_ANYCAST, NULL, pAddresses, &outBufLen) == NO_ERROR) {
        for (PIP_ADAPTER_ADDRESSES pCurrAddr = pAddresses; pCurrAddr != NULL; pCurrAddr = pCurrAddr->FirstAdapterAddress) {
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddr->FirstUnicastAddress;
            while (pUnicast != NULL) {
                sockaddr_in* sa_in = (sockaddr_in*)pUnicast->Address.lpSockaddr;
                char buf[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &sa_in->sin_addr, buf, INET_ADDRSTRLEN);
                ips.push_back(std::string(buf));
                pUnicast = pUnicast->NextUnicastAddress;
            }
        }
    }
    free(pAddresses);

#else // Linux/macOS
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        return ips;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET) 
            continue;

        char host[NI_MAXHOST];
        int s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                            host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if (s == 0) {
            ips.push_back(std::string(host));
        }
    }
    freeifaddrs(ifaddr);
#endif

    return ips;
}

bool Network::ping(const std::string& host) {
    // For a cross-platform utility library without requiring root/admin for raw sockets,
    // we implement a 'reachability check' using a TCP connect attempt to port 80.
    // In a real production lib, one would use platform-specific ICMP APIs or system commands.
    
#ifdef PLATFORM_WINDOWS
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;
#endif

    struct addrinfo hints = {}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), "80", &hints, &res) != 0) {
        return false;
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        freeaddrinfo(res);
        return false;
    }

    // Set a short timeout for the connect attempt
#ifdef PLATFORM_WINDOWS
    DWORD timeout = 1000;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#else
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
#endif

    bool reachable = (connect(sock, res->ai_addr, res->ai_addrlen) == 0);

#ifdef PLATFORM_WINDOWS
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    freeaddrinfo(res);
    return reachable;
}

} // namespace libnet